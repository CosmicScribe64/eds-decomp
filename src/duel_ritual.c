/*
 * duel_ritual (0x080431E4-0x08044223): the spell/trap negation refresh, the shared Ritual summon handlers and
 * the graveyard revive test (wiki/functions/duel-ritual-c.md).
 *
 * - IsJinzoOrRoyalDecreeActive, IsImperialOrderActive and UpdateSpellTrapNegation keep the global negation
 *   flags of gDuel (+0x1ACC, +0x1ACD) and the isDisabled bit of every face-up spell/trap up to date.
 *   DuelMainStep runs UpdateSpellTrapNegation on its idle frames, and the "negate this turn" traps run it
 *   after they resolve.
 * - EffectRitualSummonPrepare and EffectRitualSummonResolve are the Prepare and Resolve handlers of every
 *   ritual spell. gRitualRecipes gives, per ritual spell, the ritual monster and the level total that the
 *   tributes must reach. The human pays that total through the text box callbacks DrawRitualStarGauge and
 *   RitualTributeSelectStep; the CPU tributes its highest-level card on each step.
 * - CanReviveGraveyardCard: may a graveyard monster be Special Summoned (Monster Reborn, Call of the Haunted,
 *   ...)?
 *
 * Level rule of the ritual code (the *Level inlines below): Trap, Magic and Ticket cards count 0, the
 * Divine-Beasts 10, monsters their stars (gCardStats bits 25-28).
 *
 * The inline helpers repeat one rule in several spellings (argument and result widths, int or u8 locals,
 * order of the address terms). Each spelling is the one its caller needs to match; they are not merged.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_* masks */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, CardKind, SpellSubtype */
#include "constants/duel.h"         /* enum DuelZoneIndex, DuelArea, FieldPickMask */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* enum SoundEffect */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, summon.h and duel_screen.h do not pull in the legacy header. After H0, replace the
 * block (BEGIN to END) with #include "duel.h" (see build/readability/issues/duel_ritual.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

/* Needed by duel_screen.h (DuelScreen.from / .to). */
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13 */
    u16 isDefense:1;                /* bit 14 */
    u16 isFaceUp:1;                 /* bit 15 */
    u16 unk2;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u8 unk6_6:2;
    u8 unk7[0x94 - 0x7];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003 */
    u8 graveCount;                  /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 banishedCount;               /* +0x006 */
    u8 unk7[0x28 - 0x7];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard banished[80];   /* +0xB84 */
    u16 banishedInfo[80];           /* +0xCC4 */
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];     /* the rest of the player stride */
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelCard gDuelHands[];            /* 0x02019968 = gDuelPlayers[0].hand (player stride 0xD64) */
extern struct DuelCard gDuelGraveyards[];       /* 0x02019BE8 = gDuelPlayers[0].graveyard */

u32 IsSpecialSummonOnly(u16 cardId);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountHandCardsByNumber(int player, u16 cardNo);
int CountMonsters(int player);
void CopyDuelCard(u32 *dst, u32 *src);
/* ---- END duel.h stand-in ---- */

#include "chain.h"                  /* struct ChainEntry, gChain */
#include "debug.h"                  /* DebugPrintf, DebugPrintFlush */
#include "duel_actions.h"           /* ShowCardEffect, TributeMonster, DiscardHandCard */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* struct RitualRecipe, enum EffectStep / RitualStep, this unit's helpers */
#include "effect_handlers.h"        /* EffectRitualSummonPrepare, EffectRitualSummonResolve */
#include "sprite.h"                 /* AddSprite */
#include "summon.h"                 /* CanSpecialSummon, QueueSpecialSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */

/* Pre-H0: the staged sound.h declares PlaySE, the legacy include/sound.h does not. After H0, replace this
 * line with #include "sound.h". */
void PlaySE(u32 seId);

/* ---- Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view") ---- */

/* Matching: duel.h declares IsTributableMonster with a u16 return; this unit tests the result as an int
 * (no narrowing at the call). */
extern int IsTributableMonsterInt(int player, int zone) asm("IsTributableMonster");

/* Matching: IsImperialOrderActive is defined int; UpdateSpellTrapNegation masks the result as a u16 (the
 * view a u16 definition would give). */
extern u16 IsImperialOrderActiveU16(void) asm("IsImperialOrderActive");

/* gDuel as bytes: UpdateSpellTrapNegation forms the rule-flag addresses from one gDuel base register
 * (gDuel + 0x1ACC, gDuel + 0x1ACD), which the DuelState members would load as two literals. */
extern u8 gDuelBytes[] asm("gDuel");

/* The rule flags at gDuel+0x1ACC and +0x1ACD as two one-byte views (DuelState.magicNegated,
 * trapsNegated, equipMagicNegated). */
struct RuleFlags1ACC {
    u8 unk0:6;                      /* fieldBackground, duelOver, unk1ACC_5 */
    u8 magicNegated:1;              /* bit 6: Imperial Order */
    u8 trapsNegated:1;              /* bit 7: Jinzo / Royal Decree */
};
struct RuleFlags1ACD {
    u8 equipMagicNegated:1;         /* bit 0: key 1537 */
    u8 unk1:7;                      /* the "negated this turn" bits */
};

/* gDuelCtrl with its flag byte whole. Matching: DuelCtrl.isLinkDuel (a u8:1 bitfield) is read with
 * lsl #31; the ROM tests the byte with and #1. */
struct DuelCtrlBytes {
    u8 phase;
    u8 flags;                       /* bit 0: isLinkDuel */
};
extern struct DuelCtrlBytes gDuelCtrlBytes asm("gDuelCtrl");

/* Alias symbols into gDuel (duel.h "Alias symbols"): the matched code loads these addresses by name. */
extern u8 gUnk_0201ADAD[];          /* gDuel+0x1ACD: equip/field/continuous negation bits (DuelState) */
extern u8 gUnk_0201ADF2[];          /* gDuel+0x1B12: bit 1 = turnPlayer */
/* Alias of gChain.effectCard (0x02017A40 + 0x3E8). Matching: EffectRitualSummonResolve passes the card
 * word both as this symbol and as &gChain.effectCard; the two literal-pool entries are separate in the ROM. */
extern u32 gUnk_02017E28[];

/* A zone as the card word and two whole flag bytes. Matching: UpdateSpellTrapNegation tests the flags as
 * ldrb + and through int temporaries (DuelZone.isFaceUp is +0x06 bit 1, isDisabled +0x91 bit 3). */
struct ZoneFlagBytes {
    u32 cardWord;                   /* +0x00: struct DuelCard as one word */
    u8 pad4[2];
    u8 positionFlags;               /* +0x06: bit 0 isDefense, bit 1 isFaceUp */
    u8 pad7[0x91 - 7];
    u8 disableFlags;                /* +0x91: bit 3 isDisabled */
    u8 pad92[0x94 - 0x92];
};
#define ZONE_FLAG_FACE_UP       0x02    /* positionFlags */
#define ZONE_FLAG_DISABLED      0x08    /* disableFlags */

/* The rule flags read through the gDuelZones base: gDuelZones + 0x1AA0 = gDuel + 0x1ACC. */
#define ZONES_TO_RULE_FLAGS     0x1AA0
#define RULE_FLAG_MAGIC_NEGATED 0x40    /* +0x1ACC bit 6 */
#define RULE_FLAG_TRAPS_NEGATED 0x80    /* +0x1ACC bit 7 */
/* gUnk_0201ADAD (+0x1ACD) bits, by spell subtype. */
#define RULE_FLAG_EQUIP_NEGATED (0x01 | 0x02) /* equipMagicNegated, equipMagicNegatedThisTurn */
#define RULE_FLAG_FIELD_NEGATED 0x04    /* fieldMagicNegatedThisTurn (World Suppression) */
#define RULE_FLAG_CONT_MAGIC_NEGATED 0x08 /* contMagicNegatedThisTurn (Mystic Probe) */
#define RULE_FLAG_CONT_TRAP_NEGATED 0x10 /* contTrapNegatedThisTurn (Metal Detector) */

/* gRitualRecipes (0x0819A990): 16 rows and a terminator of zeros. */
extern const struct RitualRecipe gRitualRecipes[];

extern const char gStrDebugSpellTrapEnabled[];  /* 0x08085434 "Enabled!![%d/%d]\n" (player, zone) */
extern const char gStrDebugSpellTrapDisabled[]; /* 0x08085448 "Disabled...[%d/%d]\n" (player, zone) */
/* 0x0808545C "Please tribute the necessary monsters to match the required number of stars." */
extern const u8 gStrRitualTributePrompt[];

/* The card tables through integer-constant addresses. Matching: these forms and the symbols gCardStats,
 * gCardIdToNumber and gCardNumberToId give different code (literal pools); each function keeps its form. */
#define CARD_STATS_TABLE    ((const u32 *)0x08621DE0)   /* gCardStats */
#define CARD_NUMBER_TABLE   ((const u16 *)0x08622AB4)   /* gCardIdToNumber */
#define CARD_ID_TABLE       ((const u16 *)0x08623DF4)   /* gCardNumberToId */

/* Card ID of a card word (bits 0-11), from the whole word. */
#define CARD_ID(word)       (((word) << 20) >> 20)
/* The ritual monster of a recipe row (bits 0-12), read as a halfword. */
#define RECIPE_MONSTER_U16(row) (((u32)*(u16 *)(row) << 19) >> 19)
/* ChainEntry.negated (+0x04 bit 2) as the masked byte. Matching: the resolve keeps the masked byte in a
 * local and stores it (0) into gChain.effectSubStep; link->negated changes the code. */
#define LINK_NEGATED_BYTE(link) (((u8 *)(link))[4] & 4)
/* The resolving link: the last one of the chain. */
#define RESOLVING_LINK      (gChain.links[gChain.linkCount - 1])

/* 1 if Jinzo or Royal Decree is active on either field: Traps are negated. */
int IsJinzoOrRoyalDecreeActive(void)
{
    if (CountActiveCardsOnField(0, CARD_JINZO) == 0 && CountActiveCardsOnField(1, CARD_JINZO) == 0
        && CountActiveCardsOnField(0, CARD_ROYAL_DECREE) == 0 && CountActiveCardsOnField(1, CARD_ROYAL_DECREE) == 0)
        return 0;
    return 1;
}

/* 1 if Imperial Order is active on either field: Magic cards are negated. */
int IsImperialOrderActive(void)
{
    if (CountActiveCardsOnField(0, CARD_IMPERIAL_ORDER) == 0 && CountActiveCardsOnField(1, CARD_IMPERIAL_ORDER) == 0)
        return 0;
    return 1;
}

/* FAKEMATCH: flag tests go through int-typed helpers. A plain u8 & const test is shortened to a
 * QImode and plus a zero-extension, which pushes the zone loop past loop.c's hoist threshold
 * (0x7FF and (p & 1) * 0xD64 then stay in the loop). Pointer-first keeps the hoisted 0x0201ADAD
 * address live across the mask load; mask-first gives the ROM's order for 0x0201ADF2. */
static inline int TestFlagsPtrFirst(u8 *flags, int mask)
{
    return mask & *flags;
}
static inline int TestFlagsMaskFirst(int mask, u8 *flags)
{
    return mask & *flags;
}

/* enum SpellSubtype of a Trap or Magic card, 0 for any other card. */
static inline int GetSpellSubtype(u32 cardId)
{
    u32 stats = CARD_STATS_TABLE[cardId & CARD_ID_MASK];
    switch ((int)((stats & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return (stats & CARD_STATS_SUBTYPE_MASK) >> CARD_STATS_SUBTYPE_SHIFT;
    default:
        return 0;
    }
}

/* The zone (player, zone) from a gDuelZones base address, zone term first. */
static inline struct ZoneFlagBytes *GetZoneAt(int player, int zone, u32 zonesBase)
{
    int side = player & 1;
    return (struct ZoneFlagBytes *)(zone * 0x94 + side * 0xD64 + zonesBase);
}

/*
 * Recompute gDuel's negation flags, then bring the isDisabled bit of every face-up card in spell/trap zones
 * 5-10 of both players in line with them by queueing DUEL_CMD_SET_SPELL_TRAP_DISABLED (0 enables, 1 disables).
 *   trapsNegated       Jinzo or Royal Decree active: every Trap is disabled.
 *   magicNegated       Imperial Order active: every Magic card is disabled.
 *   equipMagicNegated  key 1537 active; with the per-turn bits of +0x1ACD it disables Equip, Field and
 *                      Continuous cards by subtype.
 * Card number 1539 is never disabled, and Royal Decree (a Trap that sets trapsNegated itself) only while Jinzo
 * is active. In a link duel nothing changes while gDuel.turnPlayer is 1 (the partner's GBA does it).
 */
void UpdateSpellTrapNegation(void)
{
    u8 *duel;
    int player;
    int zone;
    /* FAKEMATCH: zone base pinned to r9; as a hoisted constant its doubled REG_EQUIV live length
     * ranks it below (p & 1) * 0xD64. zonesBaseCopy is a second, spilled copy used only for the
     * disable == 0 test so that its load is a round-robin reload. */
    register u32 zonesBase asm("r9");
    u32 zonesBaseCopy = (u32)gDuelZones;
    int trapsNegated = IsJinzoOrRoyalDecreeActive();
    duel = gDuelBytes;
    ((struct RuleFlags1ACC *)(duel + 0x1ACC))->trapsNegated = trapsNegated;
    ((struct RuleFlags1ACC *)(duel + 0x1ACC))->magicNegated = IsImperialOrderActiveU16();
    ((struct RuleFlags1ACD *)(duel + 0x1ACD))->equipMagicNegated = 0;
    if (CountActiveCardsOnField(0, CARD_1537) != 0 || CountActiveCardsOnField(1, CARD_1537) != 0)
        ((struct RuleFlags1ACD *)(duel + 0x1ACD))->equipMagicNegated = 1;
    for (player = 0, zonesBase = (u32)gDuelZones; player <= 1; player++) {
        for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
            struct ZoneFlagBytes *zn = (struct ZoneFlagBytes *)((zone * 0x94) + ((player & 1) * 0xD64) + zonesBase);
            u32 cardId;
            int disable;
            int flags; /* FAKEMATCH: int temporaries keep the byte tests in SImode (see TestFlagsPtrFirst) */
            if (CARD_ID(zn->cardWord) == 0)
                continue;
            if (!((flags = zn->positionFlags) & ZONE_FLAG_FACE_UP))
                continue;
            disable = 0;
            cardId = CARD_ID(zn->cardWord);
            /* Trap and Magic cards under Jinzo / Royal Decree and Imperial Order */
            switch (((CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT)) {
            case CARD_TYPE_TRAP:
                if ((flags = *(u8 *)(zonesBase + ZONES_TO_RULE_FLAGS)) & RULE_FLAG_TRAPS_NEGATED)
                    disable = 1;
                break;
            case CARD_TYPE_MAGIC:
                if ((flags = *(u8 *)(zonesBase + ZONES_TO_RULE_FLAGS)) & RULE_FLAG_MAGIC_NEGATED)
                    disable = 1;
                break;
            }
            /* Equip, Field and Continuous cards under the subtype negations */
            switch (GetSpellSubtype(cardId)) {
            case SPELL_EQUIP:
                if (TestFlagsPtrFirst(gUnk_0201ADAD, RULE_FLAG_EQUIP_NEGATED))
                    disable = 1;
                break;
            case SPELL_FIELD:
                if (TestFlagsPtrFirst(gUnk_0201ADAD, RULE_FLAG_FIELD_NEGATED))
                    disable = 1;
                break;
            case SPELL_CONTINUOUS:
                switch (((CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT)) {
                case CARD_TYPE_TRAP:
                    if (TestFlagsPtrFirst(gUnk_0201ADAD, RULE_FLAG_CONT_TRAP_NEGATED))
                        disable = 1;
                    break;
                case CARD_TYPE_MAGIC:
                    if (TestFlagsPtrFirst(gUnk_0201ADAD, RULE_FLAG_CONT_MAGIC_NEGATED))
                        disable = 1;
                    break;
                }
                break;
            }
            switch (CARD_NUMBER_TABLE[cardId & CARD_ID_MASK]) {
            case CARD_1539:
                disable = 0;
                break;
            }
            /* link duel, partner's turn (gDuel.turnPlayer = 1): leave it to the partner */
            if (((flags = gDuelCtrlBytes.flags) & 1) && TestFlagsMaskFirst(2, gUnk_0201ADF2))
                continue;
            if (disable == 0) {
                if ((flags = GetZoneAt(player, zone, zonesBaseCopy)->disableFlags) & ZONE_FLAG_DISABLED) {
                    DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_SET_SPELL_TRAP_DISABLED
                                        : DUEL_CMD_SET_SPELL_TRAP_DISABLED, zone, 0, 0);
                    DebugPrintf(gStrDebugSpellTrapEnabled, player, zone);
                    DebugPrintFlush();
                }
            }
            if (disable != 0) {
                if ((flags = ((struct ZoneFlagBytes *)((zone * 0x94) + ((player & 1) * 0xD64) + zonesBase))->disableFlags)
                    & ZONE_FLAG_DISABLED)
                    continue;
                disable = 1;
                DebugPrintf(gStrDebugSpellTrapDisabled, player, zone);
                DebugPrintFlush();
                /* Royal Decree negates Traps itself: only Jinzo disables it */
                switch (CARD_NUMBER_TABLE[cardId & CARD_ID_MASK]) {
                case CARD_ROYAL_DECREE:
                    if (CountActiveCardsOnField(0, CARD_JINZO) == 0) {
                        disable = 0;
                        if (CountActiveCardsOnField(1, CARD_JINZO) != 0)
                            disable = 1;
                    }
                    break;
                }
                if (disable != 0)
                    DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_SET_SPELL_TRAP_DISABLED
                                        : DUEL_CMD_SET_SPELL_TRAP_DISABLED, zone, 1, 0);
            }
        }
    }
}

/* Index of the gRitualRecipes row whose ritual spell is the card number of cardId, or -1. */
int FindRitualRecipe(u16 cardId)
{
    int i = 0;
    const struct RitualRecipe *recipes = gRitualRecipes;
    /* the first row is tested through a local pointer, the loop indexes the table directly */
    if (recipes->ritualSpell != 0) {
        for (i = 0; gRitualRecipes[i].ritualSpell != 0; i++) {
            if (gRitualRecipes[i].ritualSpell == CARD_NUMBER_TABLE[CARD_ID_MASK & cardId])
                return i;
        }
    }
    return -1;
}

/* The whole hand word hand[index] of player; the index, player offset and hand base stay separate terms. */
static inline u32 GetHandWord(int player, int index)
{
    u32 side = (player & 1) * 0xD64;
    return *(u32 *)(index * 4 + side + (u32)gDuelPlayers + 0x684);
}

/* The ritual monster's card number of recipe row recipeIdx. */
static inline u32 GetRecipeMonster(int recipeIdx)
{
    u32 base = (u32)gRitualRecipes;
    u32 rowOffset = recipeIdx * 4;
    return ((u32)*(u16 *)(base + rowOffset) << 19) >> 19;
}

/* 1 if player's hand holds the ritual monster of gRitualRecipes[recipeIdx]. */
int HandHasRitualMonster(int player, int recipeIdx)
{
    u16 i;
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u32 card = GetHandWord(player, i);
        u16 monster = GetRecipeMonster(recipeIdx);
        /* card number of the hand card: (word << 21) >> 20 is (id & 0x7FF) * 2, the byte offset */
        if (*(u16 *)((u8 *)CARD_NUMBER_TABLE + ((card << 21) >> 20)) == monster)
            return 1;
    }
    return 0;
}

/* Ritual level of a card (see the top of the file). */
static inline int GetRitualLevel(u16 cardId)
{
    int type = (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT;
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_LEVEL_MASK) >> CARD_STATS_LEVEL_SHIFT;
    }
}

/* Total ritual level of player's hand, skipping the first card whose ID is excludeId (the ritual monster
 * cannot be its own tribute). */
int SumHandLevelsExcept(int player, u16 excludeId)
{
    int total = 0;
    int skipped = 0;
    int i;
    for (i = 0; i < gDuelPlayers[1 & player].handCount; i++) {
        u16 cardId = (GetHandWord(player, i) << 20) >> 20;
        if (skipped != 0 || cardId != excludeId) {
            int level = GetRitualLevel(cardId);
            /* this operand order gives the ROM's adds */
            level += total;
            total = level;
        }
        if (excludeId == cardId)
            skipped = 1;
    }
    return total;
}

/* Total ritual level of player's monsters in zones 0-4 that IsTributableMonster accepts. */
int SumTributableMonsterLevels(int player)
{
    int total = 0;
    int zone;
    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        /* player term first: the ROM spills player & 1 and the card mask around the call */
        u16 cardId = (*(u32 *)((player & 1) * 0xD64 + zone * 0x94 + (u32)gDuelZones) << 20) >> 20;
        if (IsTributableMonsterInt(player, zone) != 0) {
            int level = GetRitualLevel(cardId);
            level += total;
            total = level;
        }
    }
    return total;
}

/* Text box draw callback of the human's ritual tribute pick: one star sprite per required level
 * (gChain.effectCount), 10 px apart, on the line below the box's current row. The first
 * gChain.effectSubStep stars (still to pay) use attr2 0x431E, the paid ones 0x431D (palette 4). */
void DrawRitualStarGauge(void)
{
    int x = (gTextBox.x + 1) << 3;
    int y = (gTextBox.y - (gTextBox.height - gTextBox.revealRow)) << 3;
    int i;
    y += 8;
    for (i = 0; i < gChain.effectCount; i++) {
        /* (x + i * 10) | (y << 16) in this operand order: the ROM's strength-reduced x */
        AddSprite((x + i * 10) | (y << 16), 0, i < gChain.effectSubStep ? 0x431E : 0x431D);
    }
}

/* gChain.effectSubStep -= level, clamped at 0. Matching: the s8 level passed between the two inline helpers
 * produces the ROM's join-point copy (adds r2, r0, #0). */
static inline void PayRitualLevel(s8 level)
{
    if (gChain.effectSubStep < level)
        gChain.effectSubStep = 0;
    else
        gChain.effectSubStep -= level;
}

/* Ritual level of a card of the given type (the caller has the type already). */
static inline s8 GetRitualLevelOfType(int type, u16 cardId)
{
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_LEVEL_MASK) >> CARD_STATS_LEVEL_SHIFT;
    }
}

/*
 * Text box input callback of the human's ritual tribute pick (TextBoxSetMenu by EffectRitualSummonResolve).
 * The cursor picks a monster, or a hand card while the player has at most 4 monsters (a zone must stay free
 * for the ritual monster). Its level is taken off gChain.effectSubStep and the card is tributed or discarded.
 * Picking the ritual monster itself from the hand is refused unless the hand holds another copy. Returns 1
 * once the required level is paid.
 */
int RitualTributeSelectStep(void)
{
    u32 pickMask = PICK_ANY_MONSTER;
    u16 cardId;
    /* the result is not used */
    FindRitualRecipe(RESOLVING_LINK.card);
    if (CountMonsters(RESOLVING_LINK.player) <= 4)
        pickMask = PICK_ANY_MONSTER | PICK_HAND;
    if (DuelCursor_PickTarget(pickMask) != 0) {
        switch (gDuelScreen.selArea) {
        case DUEL_AREA_HAND: {
            int player;
            u32 type;
            player = RESOLVING_LINK.player;
            cardId = CARD_ID(*(u32 *)(player * 0xD64 + gDuelScreen.selIndex * 4 + (u32)gDuelHands));
            if (CARD_NUMBER_TABLE[cardId & CARD_ID_MASK] == gRitualRecipes[FindRitualRecipe(RESOLVING_LINK.card)].monster
                && CountHandCardsByNumber(RESOLVING_LINK.player, CARD_NUMBER_TABLE[cardId & CARD_ID_MASK]) <= 1) {
                PlaySE(SE_ERROR);
                return 0;
            }
            type = (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT;
            if (type <= CARD_TYPE_REPTILE) {
                PayRitualLevel(GetRitualLevelOfType(type, cardId));
                DiscardHandCard(RESOLVING_LINK.player, gDuelScreen.selIndex, 0, 1);
            }
            break;
        }
        case DUEL_AREA_MONSTER: {
            int player;
            u32 type;
            player = RESOLVING_LINK.player;
            cardId = CARD_ID(*(u32 *)(player * 0xD64 + gDuelScreen.selIndex * 0x94 + (u32)gDuelZones));
            type = (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT;
            if (type <= CARD_TYPE_REPTILE) {
                PayRitualLevel(GetRitualLevelOfType(type, cardId));
                TributeMonster(RESOLVING_LINK.player, gDuelScreen.selIndex);
            }
            break;
        }
        }
    }
    return gChain.effectSubStep == 0;
}

/* Prepare handler of the ritual spells: 1 if player may Special Summon, the card has a gRitualRecipes row,
 * its ritual monster is in the hand, and the other hand cards plus the tributable monsters reach the
 * recipe's level. */
int EffectRitualSummonPrepare(struct ChainEntry *card)
{
    int i;
    const struct RitualRecipe *recipes;
    if (CanSpecialSummon(card->player) == 0)
        return 0;
    i = 0;
    recipes = gRitualRecipes;
again:
    {
        /* the row from the integer byte offset plus the table base */
        const struct RitualRecipe *row = (const struct RitualRecipe *)(i * 4 + (u32)recipes);
        if ((*(u16 *)row << 19) == 0) /* end of the table (monster 0) */
            return 0;
        if (row->ritualSpell == CARD_NUMBER_TABLE[CARD_ID_MASK & card->card]) {
            u32 monster;
            u32 monsterId;
            int levels;
            int player;
            if ((u16)HandHasRitualMonster(card->player, i) == 0)
                return 0;
            player = card->player;
            monster = RECIPE_MONSTER_U16(row);
            /* card number -> card ID; numbers 2000+ are the alternate art (ID + 1) */
            if (monster == 0xFFFF) {
                monsterId = 0;
            } else if (monster <= 0x7CF) {
                monsterId = CARD_ID_TABLE[monster & CARD_ID_MASK];
            } else {
                monsterId = CARD_ID_TABLE[(monster - CARD_NUMBER_ALT_ART) & CARD_ID_MASK] + 1;
            }
            levels = SumHandLevelsExcept(player, (u16)monsterId);
            levels += SumTributableMonsterLevels(card->player);
            if (levels >= gRitualRecipes[i].level)
                return 1;
            return 0;
        }
        i++;
        goto again;
    }
}

/* Ritual level of a card (int switch, result through an int local). */
static inline u8 GetRitualLevelV(u16 cardId)
{
    int type = (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT;
    int level;
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        level = 0;
        break;
    case CARD_TYPE_DIVINE:
        level = 10;
        break;
    default:
        level = (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_LEVEL_MASK) >> CARD_STATS_LEVEL_SHIFT;
        break;
    }
    return level;
}

/* Ritual level for the hand loop's "usable" test. */
static inline u8 GetRitualLevelT(u16 cardId)
{
    int type = (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT;
    int level;
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        level = 0;
        break;
    case CARD_TYPE_DIVINE:
        level = 10;
        break;
    default:
        level = (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_LEVEL_MASK) >> CARD_STATS_LEVEL_SHIFT;
        break;
    }
    /* FAKEMATCH: consuming level keeps the constant arms (0, 10) jumping to the shared zero test; without it
     * jump2 threads them past the test. */
    asm volatile("" : : "r"(level));
    return level;
}

/* Ritual level of a card (u16 type, u8 result local). */
static inline u8 GetRitualLevelW(u16 cardId)
{
    u16 type = (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT;
    u8 level;
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        level = 0;
        break;
    case CARD_TYPE_DIVINE:
        level = 10;
        break;
    default:
        level = (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_LEVEL_MASK) >> CARD_STATS_LEVEL_SHIFT;
        break;
    }
    return level;
}

/* Matching: the resolve calls the Prepare handler with the three arguments of the Prepare slot
 * (card, chainLink = NULL, fromHand = 0); the definition takes one. */
#define CALL_RITUAL_PREPARE(card) ((int (*)(struct ChainEntry *, int, int))EffectRitualSummonPrepare)(card, 0, 0)
/* Matching: TextBoxSetMenu takes a u16-returning input callback; RitualTributeSelectStep is int (void). */
#define RITUAL_TRIBUTE_INPUT ((u16 (*)(void))RitualTributeSelectStep)

/*
 * Resolve handler of the ritual spells, a state machine on gChain.effectStep:
 *   EFFECT_STEP_START         Blocked by key 1418 (forbids Tributes) on either field. Check Prepare and the hand
 *                             again, set the required level (gChain.effectCount and effectSubStep), then the CPU
 *                             goes to RITUAL_STEP_CPU_TRIBUTE, the human gets the tribute prompt (star gauge and
 *                             cursor pick through the text box) and RITUAL_STEP_TAKE_MONSTER.
 *   RITUAL_STEP_CPU_TRIBUTE   The CPU tributes its highest-level hand card (only while it has at most 4 monsters)
 *                             or monster, once per call, until the level is paid.
 *   RITUAL_STEP_TAKE_MONSTER  Copy the ritual monster's hand word to gChain.effectCard and take it out of the hand.
 *   RITUAL_STEP_SUMMON        Special Summon it (the player chooses the position).
 *   other                     gChain.effectStep = 0, done.
 * A negated link returns EFFECT_STEP_DONE at once.
 */
int EffectRitualSummonResolve(struct ChainEntry *link)
{
    u16 cardId = link->card;
    u32 numberTable = (u32)gCardIdToNumber;
    int i;
    u8 negated;

    if ((negated = LINK_NEGATED_BYTE(link)) != 0)
        return EFFECT_STEP_DONE;
    switch (gChain.effectStep) {
    case EFFECT_STEP_START: {
        int recipeIdx;
        int key;
        gChain.effectSubStep = negated; /* 0 */
        key = CARD_1418;
        if (CountActiveCardsOnField(0, key) > 0) {
            ShowCardEffect(link->player, CARD_ID_TABLE[key]);
            return EFFECT_STEP_DONE;
        } else if (CountActiveCardsOnField(1, key) > 0) {
            ShowCardEffect(link->player, CARD_ID_TABLE[key]);
            return EFFECT_STEP_DONE;
        }
        if ((u16)CALL_RITUAL_PREPARE(link) == 0)
            return EFFECT_STEP_DONE;
        recipeIdx = FindRitualRecipe(link->card);
        if (recipeIdx < 0 || (u16)HandHasRitualMonster(link->player, recipeIdx) == 0)
            return EFFECT_STEP_DONE;
        { u8 level = gRitualRecipes[recipeIdx].level; gChain.effectSubStep = level; }
        /* FAKEMATCH: volatile re-read keeps the ROM's ldrb of effectSubStep after the store */
        gChain.effectCount = *(volatile u8 *)&gChain.effectSubStep;
        if (link->player)
            return RITUAL_STEP_CPU_TRIBUTE;
        TextBoxOpen(0x206, 0x612, TEXTBOX_FLAGS_DEFAULT, gStrRitualTributePrompt);
        TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, DrawRitualStarGauge, RITUAL_TRIBUTE_INPUT);
        return RITUAL_STEP_TAKE_MONSTER;
    }
    case RITUAL_STEP_TAKE_MONSTER:
        for (i = 0; i < gDuelPlayers[link->player & 1].handCount; i++) {
            u32 side = link->player & 1;
            /* Hand word through the integer base gDuelPlayers + 0x684 (as GetHandWord), with the offset as a
             * separate statement: the pre-test base then feeds the hoisted base+0x684 add, and the table load
             * stays in the loop. */
            u32 offset = i * 4 + side * 0xD64;
            if (gCardIdToNumber[(*(u32 *)(offset + ((u32)gDuelPlayers + 0x684)) << 21) >> 21]
                == gRitualRecipes[FindRitualRecipe(link->card)].monster) {
                u32 *handCard = (u32 *)&gDuelPlayers[link->player & 1].hand[i];
                CopyDuelCard(gUnk_02017E28, handCard);
                /* the card word as two halves */
                DuelCmd_Push(link->player ? DUEL_CMD_PLAYER | DUEL_CMD_REMOVE_CARD_FROM_HAND
                                          : DUEL_CMD_REMOVE_CARD_FROM_HAND,
                             ((u16 *)handCard)[0], ((u16 *)handCard)[1], 0);
                return RITUAL_STEP_SUMMON;
            }
        }
        return EFFECT_STEP_DONE;
    case RITUAL_STEP_SUMMON:
        QueueSpecialSummonChoosePosition(link->player, &gChain.effectCard, 1, 1);
        return RITUAL_STEP_END;
    case RITUAL_STEP_CPU_TRIBUTE: {
        int handLevel = 0;
        int handIdx = -1;
        int fieldLevel = 0;
        int fieldZone = -1;
        /* the highest-level hand card that may go: not the ritual monster itself unless the hand has another
         * copy, and not a level-0 card */
        if (CountMonsters(link->player) <= 4) {
            for (i = 0; i < gDuelPlayers[link->player].handCount; i++) {
                int usable = 1;
                int level;
                u16 handId = (*(u32 *)((link->player & 1) * 0xD64 + i * 4 + (u32)gDuelHands) << 20) >> 20;
                const u16 *number = (const u16 *)((handId & CARD_ID_MASK) * 2 + numberTable);
                if (*number == gRitualRecipes[FindRitualRecipe(cardId)].monster
                    && CountHandCardsByNumber(link->player, *number) <= 1)
                    usable = 0;
                level = GetRitualLevelT(handId);
                if (level <= 0)
                    usable = 0;
                if (usable && handLevel < GetRitualLevelW(handId)) {
                    handLevel = GetRitualLevelW(handId);
                    handIdx = i;
                }
            }
        }
        /* the highest-level monster */
        for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
            if ((*(u32 *)((link->player & 1) * 0xD64 + i * 0x94 + (u32)gDuelZones)) << 20) {
                u16 zoneId = ((*(u32 *)((link->player & 1) * 0xD64 + i * 0x94 + (u32)gDuelZones)) << 20) >> 20;
                if (fieldLevel < GetRitualLevelV(zoneId)) {
                    fieldLevel = GetRitualLevelV(((*(u32 *)((link->player & 1) * 0xD64 + i * 0x94 + (u32)gDuelZones)) << 20) >> 20);
                    fieldZone = i;
                }
            }
        }
        if (handLevel == 0 && fieldLevel == 0)
            return EFFECT_STEP_DONE;
        if (handLevel > fieldLevel) {
            DuelCmd_Push(link->player ? DUEL_CMD_PLAYER | DUEL_CMD_SEND_HAND_CARD_TO_GRAVEYARD
                                      : DUEL_CMD_SEND_HAND_CARD_TO_GRAVEYARD, handIdx, 0, 0);
            if ((int)gChain.effectSubStep > handLevel) gChain.effectSubStep -= handLevel; else gChain.effectSubStep = 0;
        } else {
            TributeMonster(link->player, fieldZone);
            if ((int)gChain.effectSubStep > fieldLevel) gChain.effectSubStep -= fieldLevel; else gChain.effectSubStep = 0;
        }
        return gChain.effectSubStep != 0 ? RITUAL_STEP_CPU_TRIBUTE : RITUAL_STEP_TAKE_MONSTER;
    }
    default:
        gChain.effectStep = 0;
        return EFFECT_STEP_DONE;
    }
}

/* graveyard[index] of player as a whole word; the player term, the index term and the base stay separate. */
static inline u32 GetGraveyardWord(int player, int index)
{
    u32 side = player & 1;
    u32 indexOffset = index * 4;
    u32 sideOffset = side * 0xD64;
    return *(u32 *)(indexOffset + sideOffset + (u32)gDuelGraveyards);
}

/* enum CardKind of a card: Obelisk counts as Ritual, Slifer and Ra as Effect monsters, Magic/Trap/Ticket
 * cards get their own kinds. Matching: every value fits an s8, and the narrowing keeps the common
 * signed test of the caller. */
static inline s8 GetCardKind(u16 cardId)
{
    int number = CARD_NUMBER_TABLE[CARD_ID_MASK & cardId];
    int type;
    int kind;
    switch (number) {
    case CARD_OBELISK_THE_TORMENTOR:
        kind = CARD_KIND_RITUAL;
        break;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        kind = CARD_KIND_EFFECT;
        break;
    default:
        type = ((CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT);
        switch (type) {
        case CARD_TYPE_MAGIC:
            kind = CARD_KIND_MAGIC;
            break;
        case CARD_TYPE_TRAP:
            kind = CARD_KIND_TRAP;
            break;
        case CARD_TYPE_TICKET:
            kind = CARD_KIND_TICKET;
            break;
        default:
            kind = (CARD_STATS_TABLE[cardId & CARD_ID_MASK] & CARD_STATS_KIND_MASK) >> CARD_STATS_KIND_SHIFT;
            break;
        }
        break;
    }
    return kind;
}

/*
 * 1 if graveyard[graveIdx] of player may be Special Summoned from the graveyard. Normal monsters always
 * may; Effect monsters (and the other kinds) unless they are special-summon-only; Fusion and Ritual monsters
 * only with card-word bit 14 set (set by tribute, flip and special summons: properly summoned, hypothesis).
 * Otherwise the result is bit 14 itself. The callers test the low halfword.
 */
int CanReviveGraveyardCard(int player, int graveIdx)
{
    u32 sideOffset = (1 & player) * 0xD64;
    u32 base = (u32)gDuelGraveyards;
    u32 *slot = (u32 *)(sideOffset + base + graveIdx * 4);
    u32 cardId = (*(u32 *)(graveIdx * 4 + sideOffset + base) << 20) >> 20;
    int kind = GetCardKind(cardId);

    switch (kind) {
    case CARD_KIND_NORMAL:
        goto yes;
    case CARD_KIND_FUSION:
    case CARD_KIND_RITUAL:
        break;
    default:
        if (IsSpecialSummonOnly((GetGraveyardWord(player, graveIdx) << 20) >> 20) == 0)
            goto yes;
        break;
    }
    /* card-word bit 14 (DuelCard.unk14) */
    return (*slot << 17) >> 31;
yes:
    return 1;
}
