/*
 * effect_resolve3 (0x08032CB0-0x08033DAB): resolve handlers of twelve cards (wiki/functions/effect-resolve3-c.md).
 *
 * Every function here is the resolve slot (+0x04) of a gCardEffects row (struct CardEffect, include/effect.h).
 * Chain_Resolve calls it with the resolving link: the card ID, its player and zone, the event that opened
 * the response window, and the targets chosen by the chainB handler (link->targets[], player | zone << 8).
 * A negated link does nothing. Multi-step effects are step machines on gChain.effectStep: Chain_Resolve
 * starts it at EFFECT_STEP_START (0x80) and stores the handler's return value, until the handler returns
 * EFFECT_STEP_DONE. A returned step that has no case (0x78, 0x7E) ends the link on the next call.
 *
 * Cards: Dark-Eyes Illusionist, Relinquished, Jigen Bakudan, Royal Decree / Jinzo / Imperial Order,
 * Parasite Paracide, Valkyrion the Magna Warrior, Bell of Destruction, Magical Hats, Time Machine,
 * Negate Attack / The Unhappy Maiden, Lightforce Sword and The Flute of Summoning Dragon.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_ALT_ART, CARD_STATS_* extractors, gCardNumberToId */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, SpellSubtype */
#include "constants/duel.h"         /* DUEL_LOC, enum DuelZoneIndex, ResponseEventKind, ZoneLinkKind, FieldPickMask */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* SE_ERROR */
#include "gba.h"                    /* B_BUTTON */
#include "main.h"                   /* gMain.newKeys */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, card_list_view.h, duel_cmd.h, duel_screen.h and summon.h do not pull in the legacy
 * header. After H0, replace the block (BEGIN to END) with #include "duel.h"
 * (see build/readability/issues/effect_resolve3.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

/* The status bits of a card word as single-bit u8 fields (applied to a zone, hence the size). */
struct DuelCardStatusBytes {
    u8 cardIdLow;                   /* +0x00: card word bits 0-7 */
    u8 unk1;
    u8 unk2_0:5;                    /* +0x02 bits 0-4 = card bits 16-20 */
    u8 destroyedInBattle:1;         /* +0x02 bit 5 = card bit 21 */
    u8 unk2_6:2;
    u8 unk3;
    u8 restOfZone[0x94 - 4];
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
    u8 unk3[0x28 - 0x3];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0xD64 - 0x7C4];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelCard gDuelHands[];            /* 0x02019968 = gDuelPlayers[0].hand (player stride 0xD64) */

u32 IsToonMonster(u16 cardNo);
void CopyDuelCard(u32 *dst, u32 *src);
int GetGraveyardCardById(int player, u16 cardId, struct DuelCard *out);
int IsCardInGraveyard(int player, struct DuelCard *card);
int CountGraveyardCardsByNumber(int player, u16 cardNo);
int CountActiveCardsOnField(int player, u16 cardNo);
int HasFaceUpToonWorld(int player);
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);
int FindFreeSpellTrapZone(int player);
u32 GetZoneCardAtk(u32 player, u32 slot);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain */
#include "duel_actions.h"           /* LoseLifePoints, MoveFieldCard, QueueAddZoneLink, TributeMonster, ... */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_prompt.h"            /* DuelPrompt_PostRandomBanishFaceDown */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* enum EffectStep, DestroyFieldCardByEffect, OnCardDestroyedByEffect */
#include "effect_handlers.h"        /* the handlers defined here */
#include "summon.h"                 /* QueueSpecialSummon, QueueSpecialSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* FormatStr, HalveRoundUp, MemCopy16, Random */

/*
 * Local views kept on purpose (matching choices, see build/readability/HEADERS.md).
 */
/* DuelCmd_Push (duel_cmd.h) takes arg4 and arg6 as int. This unit's matched code narrowed one operand to
 * u16 (it called through a u16 prototype); that call casts the operand to u16 (Parasite Paracide). */
/* Matching: CollectEffectTargets is defined to return u16; the ROM compares the count as an int (cmp; ble). */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
/* Matching: the Flute calls its prepare handler with the three arguments of the prepare slot (r1 = r2 = 0);
 * the definition (effect_handlers.h) takes only the link. */
extern int EffectTheFluteOfSummoningDragonPrepare3(struct ChainEntry *card, int chainLink, int fromHand)
    asm("EffectTheFluteOfSummoningDragonPrepare");

/*
 * gBattle (struct Battle, include/battle.h) as rows of 12 bytes from its start: row p's bytes 8-11 are the
 * first four bytes of gBattle.side[p] (struct BattleSide). Matching: Time Machine indexes the rows from the
 * gBattle literal and reads +0x08 / +0x0A, and clears destroyedCopy with a byte store (struct BattleSide has
 * a u16 container there).
 */
struct BattleSideRow {
    u8 unk0[8];                     /* the Battle header (row 0) or the end of the previous side */
    u8 slot:3;                      /* +0x08 bits 0-2: monster zone of this side's monster */
    u8 destroyed:1;                 /* +0x08 bit 3 */
    u8 defensePos:1;                /* +0x08 bit 4: it was in defense position */
    u8 destroyedCopy:1;             /* +0x08 bit 5: destroyed by the battle (copy kept for Time Machine) */
    u8 unk8_6:2;
    u8 unk9;
    u16 cardId;                     /* +0x0A: card ID of this side's monster */
};
extern struct BattleSideRow gBattleSideRows[] asm("gBattle");

/*
 * gChain.scratch.effect.effectCards[] (include/chain.h) as halfword pairs. Matching: Magical Hats reads a
 * card's low halfword as a member of an array element, which gives the ROM's address order
 * (index * 4 + gChain) + 0x544; struct DuelCard has only a u32 bitfield container.
 */
struct ChainEffectCardHalves {
    u8 unk0[0x544];
    struct {
        u16 lo;                     /* card word bits 0-15 */
        u16 hi;                     /* card word bits 16-31 */
    } effectCards[8];               /* +0x544 */
};
/* A cast of gChain, not a second symbol: the ROM shares the gChain literal with the effectSubStep load. */
#define gChainEffectCardHalves (*(struct ChainEffectCardHalves *)&gChain)

/* Prompts of this unit (ROM; the @n colour codes are left out here). */
extern const u8 gStrMagicalHatsSelectFirst[];       /* 0x08082B9C "Please select a Magic or Trap card from the
                                                     * list." */
extern const u8 gStrMagicalHatsSelectSecond[];      /* 0x08082BD0 "Select an additional Magic or Trap Card." */
extern const char gStrFluteSpecialSummonQuestion[]; /* 0x08082BFC "Do you wish to Special-Summon %s-type
                                                     * monster?" */
extern const char gStrDragon[];                     /* 0x08082C34 "Dragon": the %s of the two Flute prompts */
extern const u8 gStrSpecialSummonAnotherQuestion[]; /* 0x08082C3C "Do you wish to Special-Summon another
                                                     * monster?" */
extern const char gStrFluteSelectFromHand[];        /* 0x08082C70 "As a Special Summon, select %s from your
                                                     * hand." */


/* The card word of a zone or pile entry as one u32. Matching: the ROM loads the whole word (ldr) and
 * extracts the ID with shifts; a .id bitfield read would load only the halfword that holds it. */
#define CARD_WORD(card)     (*(u32 *)&(card))
/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word)       (((word) << 20) >> 20)
/* 1 if the card word holds a card (ID != 0), tested with a single lsl #20; the ROM uses both forms. */
#define HAS_CARD(card)      (CARD_WORD(card) << 20 != 0)
/* The 11 bits of a card word the card tables index with (CARD_ID_MASK): lsl #21; lsr #21. */
#define CARD_ID11(word)     (((word) << 21) >> 21)
/* Low and high halfword of a card word in memory: the operands of the card-word duel commands. */
#define CARD_WORD_LO(card)  (((u16 *)&(card))[0])
#define CARD_WORD_HI(card)  (((u16 *)&(card))[1])

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber
 * (0x08622AB4). Matching: these give the ROM's literal pools; the symbol forms generate other code.
 */
#define CARD_STATS(id)      (((const u32 *)0x08621DE0)[CARD_ID_MASK & (id)])
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[CARD_ID_MASK & (id)])
#define CARD_TYPE(id)       CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType */

/*
 * &gDuelZones[player].zones[zone] by integer arithmetic, zone term first. Matching: this is the ROM's
 * address order (array indexing emits the player term first). player must be 0 or 1: a ChainEntry's
 * player bit, or a location's player masked with & 1 in its own statement.
 */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))

/* A packed location player | zone << 8 (DUEL_LOC). Matching: the ROM evaluates the player first; DUEL_LOC's
 * zone-first order gives other registers. */
#define LOC(player, zone)   ((player) | ((zone) << 8))
/* The player byte of a packed location (DUEL_LOC) in memory: its low byte, read with ldrb. */
#define LOC_PLAYER_BYTE(loc) (((u8 *)&(loc))[0])

/*
 * The player bit of a chain entry read as the raw byte at +0x02. Matching: the duel-command selectors test
 * it as ldrb; and #1 (the ChainEntry.player bitfield gives lsl/lsr).
 */
#define LINK_PLAYER_BYTE(link) (1 & ((u8 *)(link))[2])
/* The number of targets (ChainEntry.numTargets, +0x0A bits 0-2) as ldrb; and #7. Matching: where the count
 * is kept in a variable, a bitfield read gives other code (compares of link->numTargets match either way). */
#define LINK_NUM_TARGETS_BYTE(link) (7 & ((u8 *)(link))[0xA])
/* The command id for the link's player (player 1's commands carry DUEL_CMD_PLAYER), and for its opponent. */
#define CMD_FOR(link, cmd)          (LINK_PLAYER_BYTE(link) ? DUEL_CMD_PLAYER | (cmd) : (cmd))
#define CMD_FOR_OPPONENT(link, cmd) (LINK_PLAYER_BYTE(link) == 0 ? DUEL_CMD_PLAYER | (cmd) : (cmd))
/* The command id for a player number. */
#define CMD_FOR_PLAYER(player, cmd) ((player) ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/* Text box placement of the effect prompts (TextBoxOpen pos = x | y << 8, size = width | height << 8). */
#define PROMPT_POS          0x205   /* x 5, y 2 */
#define PROMPT_SIZE         0x914   /* 20 x 9 cells */

/* Card ID of a card number: gCardNumberToId, with numbers 2000+ (alternate art) mapping to the ID after the
 * original's. 0xFFFF maps to 0. */
static inline u16 CardNumberToId(u32 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number < CARD_NUMBER_ALT_ART)
        return *(gCardNumberToId + (number & CARD_ID_MASK));
    return *(gCardNumberToId + ((number - CARD_NUMBER_ALT_ART) & CARD_ID_MASK)) + 1;
}

/* 1 if card id's type (enum CardType) is type. Matching: the if form gives the ROM's movs #0; cmp; bne;
 * movs #1 (a direct compare does not). */
static inline int IsCardType(u32 id, u32 type)
{
    u32 cardType = CARD_TYPE(id);
    int result = 0;

    if (cardType == type)
        result = 1;
    return result;
}

/*
 * Dark-Eyes Illusionist (also key 1332): link the targeted monster to the Illusionist (ZONE_LINK_CONTINUOUS),
 * which stops it attacking while the link lasts. Needs one target that is still on the field.
 */
int EffectDarkEyesIllusionistResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
        int zone = DUEL_LOC_ZONE(link->targets[0]);
        int side = LOC_PLAYER_BYTE(link->targets[0]) & 1;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)))
            QueueAddZoneLink(link->player, LOC(link->player, link->zone), link->targets[0],
                             ZONE_LINK_CONTINUOUS);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Relinquished (also key 1334): use up the once-per-turn effect, then absorb the targeted monster: move it
 * into a free spell/trap zone of the player, destroy the cards linked to it, and link it to Relinquished as
 * ZONE_LINK_ABSORBED (it is destroyed with Relinquished). Needs one target, a free spell/trap zone and
 * Relinquished still face up in its zone.
 */
int EffectRelinquishedResolve(struct ChainEntry *link)
{
    int freeZone = FindFreeSpellTrapZone(link->player);

    if (!link->negated) {
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SET_EFFECT_UNUSED), link->zone, 0, 0);
        if (link->numTargets == 1 && freeZone >= 0) {
            u16 targetPlayer;
            register int targetZone asm("sl"); /* FAKEMATCH: ROM keeps the zone in sl and the target id in r9 */
            struct DuelZone *target;
            u32 targetId;
            int ownSide;
            int targetSide;
            struct DuelZone *own;

            targetPlayer = LOC_PLAYER_BYTE(link->targets[0]);
            targetZone = DUEL_LOC_ZONE(link->targets[0]);
            targetSide = targetPlayer & 1;
            target = ZONE_AT(targetSide, targetZone);
            targetId = CARD_ID(CARD_WORD(target->card));
            ownSide = 1 & link->player;
            own = ZONE_AT(ownSide, link->zone);

            /* Relinquished's zone still holds a card, (Matching: each test recomputes the zone address into
             * its own variables, as the ROM does) */
            if (HAS_CARD(own->card)) {
                int side2 = 1 & link->player;
                struct DuelZone *own2 = ZONE_AT(side2, link->zone);

                /* FAKEMATCH: the r7 clobber stops reload reusing the 0xD64 constant in r7, as the ROM reloads it */
                asm volatile("" ::: "r7");
                /* it is Relinquished, */
                if (CARD_NUMBER(CARD_ID11(CARD_WORD(own2->card))) == CARD_RELINQUISHED) {
                    int side3 = 1 & link->player;
                    struct DuelZone *own3 = ZONE_AT(side3, link->zone);

                    /* face up, and the target is still there */
                    if (own3->isFaceUp && targetId != 0) {
                        MoveFieldCard(link->player, link->targets[0], LOC(link->player, (u8)freeZone));
                        DestroyLinkedCards(targetPlayer, targetZone, 0);
                        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_CLEAR_ZONE_LINKS), freeZone, 1, 0);
                        QueueAddZoneLink(link->player, LOC(link->player, (u8)freeZone), LOC(link->player, link->zone),
                                         ZONE_LINK_ABSORBED);
                    }
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Jigen Bakudan. Activated in the player's own Standby Phase (its chainA tributed the bomb itself): tribute
 * all the player's monsters and burn the opponent for half their total ATK (rounded up). On the flip: arm
 * that effect (DUEL_CMD_SET_EFFECT_UNUSED with 0 clears the zone flag that EffectJigenBakudanPrepare tests).
 */
int EffectJigenBakudanResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (link->event == RESPONSE_OWN_STANDBY) {
            int totalAtk = 0;
            int zone;

            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                /* FAKEMATCH: the shifted player byte stays in r3 and its bit in r2, as in the ROM. Both
                 * values are initialized; no extra instructions are emitted. */
                register u32 shifted asm("r3") = (u32)((u8 *)link)[2] << 31;
                register int side asm("r2") = 1 & (shifted >> 31);
                struct DuelZone *monster = ZONE_AT(side, zone);

                if (CARD_ID(CARD_WORD(monster->card))) {
                    totalAtk += GetZoneCardAtk(side, zone);
                    TributeMonster(link->player, zone);
                }
            }
            LoseLifePoints(1 - link->player, HalveRoundUp(totalAtk));
        } else {
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SET_EFFECT_UNUSED), link->zone, 0, 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Steps of EffectNegateTrapsOrMagicResolve. */
enum NegateCardsStep {
    NEGATE_STEP_START = EFFECT_STEP_START,  /* 0x80: start with the opponent's side */
    NEGATE_STEP_NEXT_SIDE = 0x7F,           /* start the side's scan at ZONE_SPELL_0 */
    NEGATE_STEP_CHECK_ZONE = 0x7E,          /* disable the card in gChain.effectZone if it matches */
    NEGATE_STEP_NEXT_ZONE = 0x7D,           /* next zone, then the player's own side */
    NEGATE_STEP_SET_FLAG = 0x78,            /* set the duel-wide negation flag; done */
};

/*
 * Royal Decree and Jinzo (Traps), Imperial Order (Magic) and key 1537 (Equip Magic): disable every other
 * face-up card of that kind in the spell/trap zones, one zone per call, the opponent's side first
 * (gChain.effectCount: 0 opponent, 1 own; gChain.effectZone: the zone), then set the global flag that keeps
 * new ones negated.
 */
int EffectNegateTrapsOrMagicResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case NEGATE_STEP_START:
            gChain.effectCount = 0;
            return NEGATE_STEP_NEXT_SIDE;
        case NEGATE_STEP_NEXT_SIDE:
            gChain.effectZone = ZONE_SPELL_0;
            return NEGATE_STEP_CHECK_ZONE;
        case NEGATE_STEP_CHECK_ZONE: {
            int player;
            int side;
            int zone;
            u32 id;
            struct DuelZone *card;

            if (gChain.effectCount != 0)
                player = link->player;
            else
                player = 1 - link->player;
            zone = gChain.effectZone;
            side = 1 & player;
            card = ZONE_AT(side, zone);
            id = CARD_ID(CARD_WORD(card->card));
            /* an occupied face-up zone other than the negating card's own */
            if (id != 0 && !(player == link->player && zone == link->zone) && card->isFaceUp) {
                int matches = 0;

                switch (CARD_NUMBER(link->card)) {
                case CARD_ROYAL_DECREE:
                case CARD_JINZO:
                    matches = IsCardType(id, CARD_TYPE_TRAP);
                    break;
                case CARD_IMPERIAL_ORDER:
                    matches = IsCardType(id, CARD_TYPE_MAGIC);
                    break;
                case CARD_1537:
                    if (CARD_TYPE(id) == CARD_TYPE_MAGIC && CARD_STATS_SUBTYPE(CARD_STATS(id)) == SPELL_EQUIP)
                        matches = 1;
                    break;
                }
                if (matches != 0) {
                    DuelCmd_Push(CMD_FOR(link, DUEL_CMD_POINT_AT_CARD), player, zone << 8, 0);
                    {
                        /* Matching: the command id in a local gives the ROM's register for it (r1) */
                        int cmd = CMD_FOR(link, DUEL_CMD_SHOW_CARD_SCATTER);

                        DuelCmd_Push(cmd, id, 1, 0);
                    }
                    DuelCmd_Push(CMD_FOR_PLAYER(player, DUEL_CMD_SET_SPELL_TRAP_DISABLED), zone, 1, 0);
                }
            }
            return NEGATE_STEP_NEXT_ZONE;
        }
        case NEGATE_STEP_NEXT_ZONE:
            gChain.effectZone++;
            if (gChain.effectZone <= ZONE_SPELL_4)
                return NEGATE_STEP_CHECK_ZONE;
            gChain.effectCount++;
            if (gChain.effectCount <= 1)
                return NEGATE_STEP_NEXT_SIDE;
            return NEGATE_STEP_SET_FLAG;
        case NEGATE_STEP_SET_FLAG:
            switch (CARD_NUMBER(link->card)) {
            case CARD_ROYAL_DECREE:
            case CARD_JINZO:
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_TRAPS), 1, 0, 0);
                break;
            case CARD_IMPERIAL_ORDER:
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_MAGIC), 1, 0, 0);
                break;
            case CARD_1537:
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_EQUIP), 1, 0, 0);
                break;
            }
            break;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Parasite Paracide: plant it in the opponent's deck, then shuffle that deck. */
int EffectParasiteParacideResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_PLANT_IN_OPPONENT_DECK), link->zone, (u16)(1 - link->player), 0);
        DuelCmd_Push(CMD_FOR_OPPONENT(link, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
    }
    return EFFECT_STEP_DONE;
}

/* Steps of EffectValkyrionTheMagnaWarriorResolve. */
enum ValkyrionStep {
    VALKYRION_STEP_START = EFFECT_STEP_START,   /* 0x80: check the conditions again */
    VALKYRION_STEP_SUMMON = 0x7F,               /* summon magnet warrior effectSubStep (0-2) */
    VALKYRION_STEP_END = 0x7E,                  /* no case: done on the next call */
};

/*
 * Valkyrion the Magna Warrior: Special Summon Alpha, Beta and Gamma The Magnet Warrior from the graveyard,
 * one per call. Needs two free monster zones and all three in the graveyard.
 */
int EffectValkyrionTheMagnaWarriorResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        u32 cardNo;

        switch (gChain.effectStep) {
        case VALKYRION_STEP_START:
            if (CountFreeMonsterZones(link->player) <= 1)
                return EFFECT_STEP_DONE;
            if (CountGraveyardCardsByNumber(link->player, CARD_ALPHA_THE_MAGNET_WARRIOR) == 0)
                return EFFECT_STEP_DONE;
            if (CountGraveyardCardsByNumber(link->player, CARD_BETA_THE_MAGNET_WARRIOR) == 0)
                return EFFECT_STEP_DONE;
            if (CountGraveyardCardsByNumber(link->player, CARD_GAMMA_THE_MAGNET_WARRIOR) == 0)
                return EFFECT_STEP_DONE;
            gChain.effectSubStep = 0;
            gChain.effectStep--;
            /* fall through */
        case VALKYRION_STEP_SUMMON: {
            struct DuelCard card;

            switch (gChain.effectSubStep) {
            case 0:
                cardNo = CARD_ALPHA_THE_MAGNET_WARRIOR;
                break;
            case 1:
                cardNo = CARD_BETA_THE_MAGNET_WARRIOR;
                break;
            case 2:
                cardNo = CARD_GAMMA_THE_MAGNET_WARRIOR;
                break;
            }
            if (GetGraveyardCardById(link->player, CardNumberToId(cardNo), &card) != 0) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD), CARD_WORD(card), CARD_WORD(card) >> 16,
                             0);
                QueueSpecialSummonChoosePosition(link->player, &card, 1, ZONE_STATUS_FROM_GRAVEYARD);
            }
            gChain.effectSubStep++;
            if (gChain.effectSubStep <= 2)
                return VALKYRION_STEP_SUMMON;
            return VALKYRION_STEP_END;
        }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Bell of Destruction (also key 1451): destroy the targeted face-up card. Bell of Destruction then makes both
 * players lose its ATK; key 1451 skips the player's next Draw Phase instead.
 */
int EffectBellOfDestructionResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
        int targetPlayer = LOC_PLAYER_BYTE(link->targets[0]);
        int zone = DUEL_LOC_ZONE(link->targets[0]);
        int side = targetPlayer & 1;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)) && target->isFaceUp) {
            int atk = GetZoneCardAtk(targetPlayer, zone);

            DestroyFieldCardByEffect(targetPlayer, zone);
            OnCardDestroyedByEffect(link->player, targetPlayer, zone);
            switch ((int)CARD_NUMBER(link->card)) {
            case CARD_BELL_OF_DESTRUCTION:
                LoseLifePoints(1 - link->player, atk);
                LoseLifePoints(link->player, atk);
                break;
            case CARD_1451:
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SKIP_NEXT_DRAW_PHASE), 0, 0, 0);
                break;
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Steps of EffectMagicalHatsResolve. */
enum MagicalHatsStep {
    HATS_STEP_START = EFFECT_STEP_START,    /* 0x80: conditions; ask for the first deck card */
    HATS_STEP_OPEN_FIRST_LIST = 0x7F,       /* show the candidates (CollectEffectTargets' list) */
    HATS_STEP_TAKE_FIRST = 0x7E,            /* take the picked card out of the deck; ask for the second */
    HATS_STEP_OPEN_SECOND_LIST = 0x7D,
    HATS_STEP_TAKE_SECOND = 0x7C,           /* take the second card; lift the attacked monster off the field */
    HATS_STEP_SHUFFLE_HATS = 0x7B,          /* shuffle the three cards */
    HATS_STEP_SET_HATS = 0x7A,              /* set hat effectSubStep (0-2) in a free monster zone */
    HATS_STEP_SHUFFLE_DECK = 0x79,
    HATS_STEP_END = 0x78,                   /* no case: done on the next call */
};

/*
 * Magical Hats: answering an attack on one of the player's monsters (targets[0]), pick two Magic or Trap
 * cards from the deck, hide the monster among them and set the three face down in defense position in free
 * monster zones (face up if Light of Intervention is active). The three card words are kept in
 * gChain.scratch.effect.effectCards[0-2] (the monster third) and the monster's zone in
 * gChain.effectSavedZone; its zone links wait in the field zone until it is set again.
 */
int EffectMagicalHatsResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case HATS_STEP_START: {
            int numTargets = LINK_NUM_TARGETS_BYTE(link);

            if (numTargets != 1)
                return EFFECT_STEP_DONE;
            if (CollectEffectTargetsInt(link->player, CARD_NUMBER(link->card), 0) <= 1)
                return EFFECT_STEP_DONE;
            {
                int targetPlayer = LOC_PLAYER_BYTE(link->targets[0]);
                int zone = DUEL_LOC_ZONE(link->targets[0]);
                /* FAKEMATCH: numTargets is 1 here; the ROM reuses that register as the player mask */
                int side = numTargets & targetPlayer;
                struct DuelZone *target = ZONE_AT(side, zone);

                if (!CARD_ID(CARD_WORD(target->card)))
                    return EFFECT_STEP_DONE;
                if (link->player != targetPlayer)
                    return EFFECT_STEP_DONE;
            }
            if (EffectAttackResponsePrepare(link, 0, 0) == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrMagicalHatsSelectFirst);
            return HATS_STEP_OPEN_FIRST_LIST;
        }
        case HATS_STEP_OPEN_FIRST_LIST:
            CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
            return HATS_STEP_TAKE_FIRST;
        case HATS_STEP_TAKE_FIRST: {
            u32 *pick = &gCardListView.cards[gCardListView.top + gCardListView.cursorRow];

            CopyDuelCard((u32 *)gChain.scratch.effect.effectCards, pick);
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_REMOVE_CARD_FROM_DECK), CARD_WORD_LO(*pick), CARD_WORD_HI(*pick), 0);
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrMagicalHatsSelectSecond);
            return HATS_STEP_OPEN_SECOND_LIST;
        }
        case HATS_STEP_OPEN_SECOND_LIST:
            CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
            return HATS_STEP_TAKE_SECOND;
        case HATS_STEP_TAKE_SECOND: {
            u32 *pick = &gCardListView.cards[gCardListView.top + gCardListView.cursorRow];
            u32 *secondHat = (u32 *)&gChain.scratch.effect.effectCards[1];

            CopyDuelCard(secondHat, pick);
            /* FAKEMATCH: retain the initialized destination before the player mask. */
            __asm__("" : : "r"(secondHat));
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_REMOVE_CARD_FROM_DECK), CARD_WORD_LO(*pick), CARD_WORD_HI(*pick), 0);
            /* Lift the attacked monster off the field: its card word becomes the third hat and its whole zone
             * is saved. Matching: dst is set before the zone pointer, and the side pointer in its own
             * statement keeps the ROM's (side + zone * 0x94) order. */
            {
                struct DuelCard *dst = &gChain.scratch.effect.effectCards[2];
                struct DuelZonesPlayer *targetSide = &gDuelZones[1 & link->targets[0]];
                struct DuelZone *target = &targetSide->zones[DUEL_LOC_ZONE(link->targets[0])];

                CopyDuelCard((u32 *)dst, (u32 *)target);
            }
            {
                struct DuelZone *dst = &gChain.effectSavedZone;
                struct DuelZonesPlayer *targetSide = &gDuelZones[1 & link->targets[0]];
                struct DuelZone *target = &targetSide->zones[DUEL_LOC_ZONE(link->targets[0])];

                MemCopy16(dst, target, sizeof(struct DuelZone));
            }
            DuelCmd_Push(CMD_FOR_PLAYER((u8)link->targets[0], DUEL_CMD_CLEAR_ZONE_CARD), DUEL_LOC_ZONE(link->targets[0]),
                         8, 0);
            /* park the monster's links in the field zone */
            DuelCmd_Push(CMD_FOR_PLAYER((u8)link->targets[0], DUEL_CMD_MOVE_ZONE_LINKS), DUEL_LOC_ZONE(link->targets[0]),
                         ZONE_FIELD, 0);
            return HATS_STEP_SHUFFLE_HATS;
        }
        case HATS_STEP_SHUFFLE_HATS: {
            int swaps = 0;
            u32 *temp = (u32 *)&gChain.effectCard;
            u32 *hats = (u32 *)gChain.scratch.effect.effectCards;

            /* 16 swaps of two different hats */
            do {
                int a;
                int b;

                do {
                    a = Random() % 3;
                    b = Random() % 3;
                } while (a == b);
                CopyDuelCard(temp, &hats[a]);
                CopyDuelCard(&hats[a], &hats[b]);
                CopyDuelCard(&hats[b], temp);
                swaps++;
            } while (swaps <= 15);
            gChain.effectSubStep = 0;
            return HATS_STEP_SET_HATS;
        }
        case HATS_STEP_SET_HATS: {
            int zone = FindFreeMonsterZone(link->player);
            struct DuelCard *hat = &gChain.scratch.effect.effectCards[gChain.effectSubStep];
            int faceUp = 0;

            /* FAKEMATCH: keep the formed card pointer alive across the flag checks. */
            __asm__("" : : "r"(hat));
            if (CountActiveCardsOnField(0, CARD_LIGHT_OF_INTERVENTION) != 0
                || CountActiveCardsOnField(1, CARD_LIGHT_OF_INTERVENTION) != 0)
                faceUp = 1;
            /* zone | faceUp << 8 | defense position << 9 */
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SET_MAGICAL_HATS_CARD), (u8)zone | ((faceUp | 2) << 8),
                         CARD_WORD_LO(*hat), CARD_WORD_HI(*hat));
            /* the monster gets its links back */
            if (CARD_TYPE(CARD_ID11((u32)gChainEffectCardHalves.effectCards[gChain.effectSubStep].lo))
                <= CARD_TYPE_REPTILE)
                DuelCmd_Push(CMD_FOR_PLAYER((u8)link->targets[0], DUEL_CMD_MOVE_ZONE_LINKS), ZONE_FIELD, (u16)zone, 0);
            gChain.effectSubStep++;
            if (gChain.effectSubStep <= 2)
                return HATS_STEP_SET_HATS;
            return HATS_STEP_SHUFFLE_DECK;
        }
        case HATS_STEP_SHUFFLE_DECK:
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
            return HATS_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Steps of EffectTimeMachineResolve: one battle side per call. */
enum TimeMachineStep {
    TIME_MACHINE_STEP_OWN = EFFECT_STEP_START,  /* 0x80: the player's side of the battle */
    TIME_MACHINE_STEP_OPPONENT = 0x7F,          /* the opponent's side */
};

/*
 * Time Machine: return each monster destroyed in the battle (gBattle.side[p].destroyedCopy) from the
 * graveyard to its controller's field, in its battle position. Step 0x80 handles the player's side and 0x7F
 * the opponent's (the handler returns effectStep - 1). A Toon needs a face-up Toon World. Both graveyards are
 * searched, so a monster the side controlled but did not own also comes back to that side.
 */
u16 EffectTimeMachineResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int player;
        int canSummon;
        struct DuelCard card;
        struct BattleSideRow *side;
        struct BattleSideRow *rows;
        int step = gChain.effectStep;

        /* Matching: the nested switch gives the ROM's range checks (cmp #0x80; ble / cmp #0x7F; bge) */
        switch (step) {
        case TIME_MACHINE_STEP_OPPONENT:
        case TIME_MACHINE_STEP_OWN:
            {
                switch (step) {
                case TIME_MACHINE_STEP_OWN:
                    player = link->player;
                    break;
                case TIME_MACHINE_STEP_OPPONENT:
                    player = 1 - link->player;
                    break;
                }
                canSummon = 1;
                /* Matching: the literal is loaded before the index arithmetic */
                rows = gBattleSideRows;
                side = rows + player;
                if (IsToonMonster(CARD_NUMBER(side->cardId)) != 0)
                {
                    canSummon = 0;
                    if (HasFaceUpToonWorld(player) != 0)
                        canSummon = 1;
                }
                /* destroyedCopy (bit 5) tested as the sign bit: lsl #26 */
                if (((int)((u32)((u8 *)side)[8] << 26) < 0) && canSummon && FindFreeMonsterZone(player) >= 0) {
                    int other;

                    if (GetGraveyardCardById(player, side->cardId, &card) != 0 && IsCardInGraveyard(player, &card) != 0) {
                        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD), CARD_WORD(card),
                                     CARD_WORD(card) >> 16, 0);
                        /* byte store into card word bit 21 */
                        ((struct DuelCardStatusBytes *)&card)->destroyedInBattle = 0;
                        QueueSpecialSummon(player, &card, 1, side->defensePos, ZONE_STATUS_FROM_GRAVEYARD);
                    }
                    other = 1 - player;
                    if (GetGraveyardCardById(other, gBattleSideRows[player].cardId, &card) != 0
                        && IsCardInGraveyard(other, &card) != 0) {
                        DuelCmd_Push(CMD_FOR_OPPONENT(link, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD), CARD_WORD(card),
                                     CARD_WORD(card) >> 16, 0);
                        ((struct DuelCardStatusBytes *)&card)->destroyedInBattle = 0;
                        QueueSpecialSummon(player, &card, 1, gBattleSideRows[player].defensePos,
                                           ZONE_STATUS_FROM_GRAVEYARD);
                    }
                }
                gBattleSideRows[player].destroyedCopy = 0;
                return gChain.effectStep - 1;
            }
        default:
            return EFFECT_STEP_DONE;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Negate Attack, The Unhappy Maiden: end the Battle Phase. */
int EffectEndBattlePhaseResolve(struct ChainEntry *link)
{
    if (!link->negated)
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_END_BATTLE_PHASE), 0, 0, 0);
    return EFFECT_STEP_DONE;
}

/* Lightforce Sword: banish a random card of the opponent's hand face down (if it has one). */
int EffectLightforceSwordResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        struct DuelPlayer *players = gDuelPlayers;
        int opponent = 1 - link->player;

        if (players[opponent & 1].handCount != 0)
            DuelPrompt_PostRandomBanishFaceDown(1 - link->player);
    }
    return EFFECT_STEP_DONE;
}

/* Steps of EffectTheFluteOfSummoningDragonResolve. */
enum FluteStep {
    FLUTE_STEP_START = EFFECT_STEP_START,   /* 0x80: reset the summon count */
    FLUTE_STEP_ASK = 0x7F,                  /* "Special-Summon a Dragon-type monster?" (Yes/No) */
    FLUTE_STEP_PICK = 0x7E,                 /* pick a Dragon in the hand; B goes back to the question */
    FLUTE_STEP_SUMMONED = 0x7D,             /* count it; offer another one */
    FLUTE_STEP_ANSWER = 0x7C,               /* No ends the effect */
};

/*
 * The Flute of Summoning Dragon (human player only): Special Summon up to two Dragons from the hand
 * (gChain.effectSubStep counts them). Each summon is offered with a Yes/No question and picked with the hand
 * cursor; a Toon needs a face-up Toon World.
 */
int EffectTheFluteOfSummoningDragonResolve(struct ChainEntry *link)
{
    char question[0x100];
    char questionAgain[0x100];
    char pickPrompt[0x100];

    if (link->negated)
        return EFFECT_STEP_DONE;
    switch (gChain.effectStep) {
    case FLUTE_STEP_START:
        gChain.effectSubStep = 0;
        gChain.effectStep--;
        /* fall through */
    case FLUTE_STEP_ASK:
        if (CountFreeMonsterZones(link->player) == 0)
            return EFFECT_STEP_DONE;
        if (EffectTheFluteOfSummoningDragonPrepare3(link, 0, 0) == 0)
            return EFFECT_STEP_DONE;
        FormatStr(question, gStrFluteSpecialSummonQuestion, gStrDragon);
        TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (u8 *)question);
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
        return FLUTE_STEP_ANSWER;
    case FLUTE_STEP_PICK:
        if (gMain.newKeys & B_BUTTON) {
            FormatStr(questionAgain, gStrFluteSpecialSummonQuestion, gStrDragon);
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (u8 *)questionAgain);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return FLUTE_STEP_ANSWER;
        }
        if (DuelCursor_PickTarget(PICK_HAND) != 0) {
            int player = link->player;
            /* gDuelPlayers[player].hand[selIndex] by integer arithmetic on gDuelHands (the ROM's form) */
            u32 word = *(u32 *)(player * sizeof(struct DuelPlayer) + gDuelScreen.selIndex * sizeof(struct DuelCard)
                                + (u32)gDuelHands);
            u32 id = CARD_ID11(word);

            if (CARD_TYPE(id) == CARD_TYPE_DRAGON
                && (IsToonMonster(CARD_NUMBER(id)) == 0 || HasFaceUpToonWorld(link->player) != 0)) {
                struct DuelCard *hand = (struct DuelCard *)((1 & link->player) * sizeof(struct DuelPlayer)
                                                            + (u32)gDuelHands);
                struct DuelCard *card = hand + gDuelScreen.selIndex;
                /* FAKEMATCH: the signed byte/halfword flag adds RTL insns while the card pointer is live, so
                 * global alloc ranks the type (r4) above the card pointer (r5) as in the ROM. */
                s16 isPlayer1 = (s8)LINK_PLAYER_BYTE(link);

                DuelCmd_Push(CMD_FOR_PLAYER(isPlayer1, DUEL_CMD_REMOVE_CARD_FROM_HAND), CARD_WORD_LO(*card),
                             CARD_WORD_HI(*card), 0);
                QueueSpecialSummonChoosePosition(link->player, card, 1, 0);
                return FLUTE_STEP_SUMMONED;
            }
            PlaySE(SE_ERROR);
        }
        return FLUTE_STEP_PICK;
    case FLUTE_STEP_SUMMONED:
        gChain.effectSubStep++;
        if (gChain.effectSubStep == 2)
            return EFFECT_STEP_DONE;
        if (CountFreeMonsterZones(link->player) != 0) {
            int i;

            for (i = 0; i < gDuelPlayers[1 & link->player].handCount; i++) {
                u16 id = CARD_ID(CARD_WORD(gDuelPlayers[1 & link->player].hand[i]));
                /* FAKEMATCH: a named type-minus-one keeps the base copy before the 0x7FF load (r7/r4 as in
                 * the ROM) and compares against the immediate 1. */
                int notDragon = CARD_TYPE(id) - CARD_TYPE_DRAGON;

                if (notDragon != 0)
                    continue;
                TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSpecialSummonAnotherQuestion);
                TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
                return FLUTE_STEP_ANSWER;
            }
        }
        return EFFECT_STEP_DONE;
    case FLUTE_STEP_ANSWER:
        if (gTextBox.result == 0)
            return EFFECT_STEP_DONE;
        FormatStr(pickPrompt, gStrFluteSelectFromHand, gStrDragon);
        TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (u8 *)pickPrompt);
        return FLUTE_STEP_PICK;
    }
    return EFFECT_STEP_DONE;
}
