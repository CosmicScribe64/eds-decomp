#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_TOKEN_*, CARD_STATS_* extractors */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, CardAttribute, CardKind, SpellSubtype */
#include "constants/duel.h"         /* enum DuelZoneIndex, ResponseEventKind, FieldPickMask */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* enum SoundEffect */
#include "legacy/gba.h"                    /* B_BUTTON */
#include "legacy/main.h"                   /* gMain.newKeys */

/*
 * Card effect handlers, part 2: target checks (the Check slot of gCardEffects) and activation costs (the
 * ChainA slot). See wiki/functions/effect-checks-c.md; the first part is card_list_viewer.c.
 *
 * Check handlers: int f(struct ChainEntry *card, u16 pos) with pos = player | zone << 8 (DUEL_LOC). They
 * return nonzero when the card in (player, zone) is a legal target for card's effect (either player's side
 * unless the handler compares player with card->player). CanActivateEffect scans every position with them
 * (an effect with a Check handler needs at least one valid target), CanEffectTargetZone asks about one
 * position, and some target pickers and resolvers call them by name to re-check a target. Handlers that call
 * CanCardTargetZone respect targeting protection (Lord of D., Umi); the ones without it (Fissure, Nobleman of
 * Crossout, Acid Trap Hole, Patrol Robo, ...) belong to effects that do not target. Each one unpacks pos as
 * (u8)pos and pos >> 8 (DUEL_LOC_PLAYER's & 0xFF gives other code).
 *
 * ChainA handlers pay the activation cost when the link joins the chain (Chain_Build): LP (DUEL_CMD_LOSE_LP),
 * discards (DuelPrompt_PostDiscardCost) or a tribute (TributeMonster, possibly after a cursor prompt). They
 * are called every frame until they return 1; gChain.costStep is their step counter.
 *
 * Zones: 0-4 monsters, 5-9 spells and traps, 10 the Field Magic (enum DuelZoneIndex). DuelZone +0x06 holds
 * isDefense (bit 0) and isFaceUp (bit 1).
 */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, duel_cmd.h and duel_screen.h do not pull in the legacy header. After H0, replace
 * the block (BEGIN to END) with #include "legacy/duel.h" (see build/readability/issues/effect_checks.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12 */
    u32 unk13:3;
    u32 specialSummoned:1;          /* bit 16: Special Summoned (summon actions 4-6, tokens, Parasite Paracide) */
    u32 unk17:15;
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
    u8 unk2[0x28 - 0x2];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    u8 unk684[0xD64 - 0x684];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

/* Effective stats of the card in a zone (GetZoneCardStats), after field, equip and link modifiers. */
struct ZoneCardStats {
    u16 id;                         /* +0x0: card ID */
    u8 type:5;                      /* +0x2 bits 0-4: effective enum CardType */
    u8 attribute:3;                 /* +0x2 bits 5-7: effective enum CardAttribute */
    u8 unk3;
    s32 atk;                        /* +0x4: effective ATK */
    s32 def;                        /* +0x8: effective DEF */
};

extern struct DuelPlayer gDuelPlayers[2];
extern struct DuelZonesPlayer gDuelZones[2];

int CountActiveCardsOnField(int player, u16 cardNo);
int CountZoneEquips(int player, int zone, u16 requireMagic, u16 requireEquipSubtype);
s32 IsCardLinkedToMonster(s32 player, s32 slot);
int CountValidEquipTargets(u32 equipPlayer, u32 equipSlot);
void GetZoneCardStats(int player, int zone, struct ZoneCardStats *out);
u32 GetZoneCardType(s32 player, s32 slot);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "chain.h"                  /* struct ChainEntry, gChain */
#include "duel_actions.h"           /* TributeMonster */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_prompt.h"            /* DuelPrompt_PostDiscardCost */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* CanCardTargetZone */
#include "effect_handlers.h"        /* the handlers defined here */
#include "text_box.h"               /* TextBoxOpen */
#include "util.h"                   /* HalveRoundDown */

/* Local views kept on purpose (matching choices, see build/readability/HEADERS.md): this unit tests the
 * return values of these two functions with other widths than their definitions. */
/* Matching: IsZoneTargetable is defined to return int; the ROM tests it as a u16 (lsl #16; cmp). */
extern u16 IsZoneTargetableU16(int player, int zone) asm("IsZoneTargetable");
/* Matching: CollectEffectTargets is defined to return u16; the ROM compares it as an int (cmp; ble). */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");

/* Cost prompts (not in a header: only this unit uses them). */
extern const u8 gStrCrushCardTributePrompt[];   /* 0x0808277C: "Select a DARK monster with ATK factors of 1000
                                                 * or below as Tribute." */
extern const u8 gStrSelectTributeMonster[];     /* 0x080827C4: "Please select a monster as Tribute." */

/* The card word of a zone as one u32. Matching: the ROM loads the whole word (ldr) for the ID and status
 * tests; a member read of card.id generates other code. */
#define CARD_WORD(card) (*(u32 *)&(card))
/* Card ID (DuelCard.id, bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word) (((word) << 20) >> 20)
/* DuelCard.specialSummoned (bit 16) of a card word, tested in place (and; lsr #16). */
#define CARD_WORD_SPECIAL_SUMMONED 0x10000

/*
 * &gDuelZones[player].zones[zone] by byte arithmetic. Matching: the ROM adds the zone term, then the player
 * term, then the gDuelZones literal. Callers pass a side = player & 1 computed in its own statement (the
 * ROM masks the player before the multiplies; ZONE_AT(player & 1, zone) changes the order).
 */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))

/* &gDuelZones[player].zones[zone] with the player term first: the order of EffectDestroyByTypeCheck's second
 * address computation. */
#define ZONE_AT_PLAYER_FIRST(player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))

/*
 * DuelZone +0x06 (isDefense bit 0, isFaceUp bit 1) as a whole byte. The handlers test isFaceUp as a bitfield
 * (ldrb; and #2). Matching: they return a flag as (byte >> 1) & 1 or 1 & ~byte, where a bitfield read gives
 * a lsl/lsr pair, and Acid Trap Hole tests isDefense after !isFaceUp with a separate and #1 (two bitfield
 * tests merge into one (byte & 3) == 1 compare).
 * FAKEMATCH: those handlers keep the 1 in a variable (`int one = 1`) and use it for both the player mask
 * (player & one) and the flag mask; that keeps the ROM's single mov rN, #1 in a callee-saved register.
 */
#define ZONE_POSITION_BYTE(zone) (((u8 *)(zone))[6])
#define ZONE_POSITION_DEFENSE 0x1   /* isDefense */

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4).
 * Matching: the ROM reloads the table address at each use, which the symbols do not give.
 */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[CARD_ID_MASK & (id)])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[CARD_ID_MASK & (id)])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))   /* enum CardType */

/* Card numbers 1920-1999 are monster tokens. */
#define IS_TOKEN_NUMBER(number) \
    ((u16)((number) - CARD_NUMBER_TOKEN_FIRST) < CARD_NUMBER_TOKEN_END - CARD_NUMBER_TOKEN_FIRST)

/* Level of a card from its stats word: 0 for Trap, Magic and Ticket cards, 10 for the Egyptian Gods, else
 * the stars. */
static inline int GetCardLevel(int id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS(id));
    }
}

/* enum SpellSubtype of a Magic or Trap card from its stats word; 0 for other cards. */
static inline int GetSpellSubtype(int id)
{
    u32 stats = CARD_STATS(id);

    switch ((int)CARD_STATS_TYPE(stats)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return CARD_STATS_SUBTYPE(stats);
    default:
        return 0;
    }
}

/* The command id for the link's player: player 1's commands carry DUEL_CMD_PLAYER. */
#define CMD_FOR(link, cmd) ((link)->player ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/* Text box placement shared by the two cost prompts: x 6, y 2; 18 x 7 cells. */
#define COST_PROMPT_POS 0x206
#define COST_PROMPT_SIZE 0x712

/* ---- Check handlers ---- */

/* Dragon Seeker: a face-up Dragon (effective type, GetZoneCardType) in either player's monster zones that
 * card may target. */
int EffectDragonSeekerCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    u16 type = GetZoneCardType(player, zone);   /* u16: the ROM narrows the result */
    int side;
    struct DuelZone *target;

    if (zone > ZONE_MONSTER_4)
        return 0;
    side = player & 1;
    target = ZONE_AT(side, zone);
    if (!CARD_ID(CARD_WORD(target->card)) || !target->isFaceUp
        || !CanCardTargetZone(card->card, player, zone))
        return 0;
    return type == CARD_TYPE_DRAGON;
}

/* Any monster, face up or down, on either side that card may target (Man-Eater Bug, Hane-Hane, Penguin
 * Soldier, Tribute to The Doomed, keys 1211/1213/1220/1301). */
int EffectTargetableMonsterCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int ret;
    int side;
    struct DuelZone *target;

    if (zone > ZONE_MONSTER_4 || !CanCardTargetZone(card->card, player, zone))
        return 0;
    ret = 0;
    side = player & 1;
    target = ZONE_AT(side, zone);
    if (CARD_ID(CARD_WORD(target->card)))
        ret = zone = 1; /* FAKEMATCH (found by decomp-permuter): reusing `zone` keeps ret in r3 */
    return ret;
}

/* Patrol Robo: a face-down card in any of the opponent's zones (no range or targeting test). */
int EffectPatrolRoboCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (player != card->player) {
        int side = player & 1;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)) && !target->isFaceUp)
            return 1;
    }
    return 0;
}

/*
 * The "destroy every monster of type X" cards: a face-up monster in either player's monster zones that card
 * may target (CanCardTargetZone, IsZoneTargetable) and whose effective type is the card's type. Eternal Rest
 * instead needs an equipped monster.
 */
int EffectDestroyByTypeCheck(struct ChainEntry *card, u16 pos)
{
    u16 cardId = card->card;
    int player = (u8)pos;
    int zone = pos >> 8;
    struct DuelZonesPlayer *sideZones = &gDuelZones[player & 1];   /* Matching: through a pointer to the side */
    struct DuelZone *target = &sideZones->zones[zone];
    u16 targetId = CARD_ID(CARD_WORD(target->card));
    u16 type = GetZoneCardType(player, zone);
    int side;

    side = player & 1;
    if (zone > ZONE_MONSTER_4)
        return 0;
    /* Matching: the ROM tests the card word a second time, from an address computed anew. */
    if (CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(side, zone)->card))
        && CanCardTargetZone(card->card, player, zone) && IsZoneTargetableU16(player, zone)
        && target->isFaceUp && targetId && cardId) {
        switch (CARD_NUMBER(cardId)) {
        case CARD_WARRIOR_ELIMINATION:
            if (type == CARD_TYPE_WARRIOR)
                return 1;
            break;
        case CARD_ETERNAL_REST:
            if (CountZoneEquips(player, zone, 0, 0) > 0)
                return 1;
            break;
        case CARD_STAIN_STORM:
            if (type == CARD_TYPE_MACHINE)
                return 1;
            break;
        case CARD_ERADICATING_AEROSOL:
            if (type == CARD_TYPE_INSECT)
                return 1;
            break;
        case CARD_BREATH_OF_LIGHT:
            if (type == CARD_TYPE_ROCK)
                return 1;
            break;
        case CARD_ETERNAL_DRAUGHT:
            if (type == CARD_TYPE_FISH)
                return 1;
            break;
        case CARD_LAST_DAY_OF_WITCH:
            if (type == CARD_TYPE_SPELLCASTER)
                return 1;
            break;
        case CARD_EXILE_OF_THE_WICKED:
            if (type == CARD_TYPE_FIEND)
                return 1;
            break;
        }
    }
    return 0;
}

/* Acid Trap Hole: a face-down defense-position monster on either side (no targeting test), unless card 1418
 * (an effect key without an EDS card) is active on either field. */
int EffectAcidTrapHoleCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (CountActiveCardsOnField(0, CARD_1418) <= 0 && CountActiveCardsOnField(1, CARD_1418) <= 0
        && zone <= ZONE_MONSTER_4) {
        int side = player & 1;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)) && !target->isFaceUp
            && (ZONE_POSITION_BYTE(target) & ZONE_POSITION_DEFENSE))
            return 1;
    }
    return 0;
}

/* A face-up monster on either side that card may target (Bell of Destruction, Reinforcements, Castle Walls,
 * Rush Recklessly, The Reliable Guardian, Snake Fang, Riryoku, keys 1330/1451/1534). Returns its isFaceUp
 * bit. */
int EffectTargetableFaceUpMonsterCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone <= ZONE_MONSTER_4) {
        int one = 1;    /* FAKEMATCH: shared 1, see ZONE_POSITION_BYTE */
        int side = player & one;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)) && CanCardTargetZone(card->card, player, zone))
            return (ZONE_POSITION_BYTE(target) >> 1) & one;
    }
    return 0;
}

/* Darkness Approaches: a face-up monster on either side that card may target and that is not a token (a
 * token cannot be turned face down). Returns its isFaceUp bit. */
int EffectDarknessApproachesCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int one = 1;    /* FAKEMATCH: shared 1, see ZONE_POSITION_BYTE */
    int side = player & one;
    struct DuelZone *target = ZONE_AT(side, zone);
    int targetId = CARD_ID(CARD_WORD(target->card));
    u16 idCopy; /* FAKEMATCH (found by decomp-permuter): the extra copy keeps the id live in r1 so `0x7FF & id` masks into r1 */

    idCopy = targetId;
    if (zone <= ZONE_MONSTER_4 && targetId != 0 && !IS_TOKEN_NUMBER(CARD_NUMBER(idCopy))
        && CanCardTargetZone(card->card, player, zone))
        return (ZONE_POSITION_BYTE(target) >> 1) & one;
    return 0;
}

/* Magic-Arm Shield: an opponent's face-up monster that card may target, other than the monster at
 * card->loc0 (the attacker, hypothesis). Returns its isFaceUp bit. */
int EffectMagicArmShieldCheck(struct ChainEntry *card, u16 pos)
{
    register u16 normalized __asm__("r0") = pos;
    int player;
    int zone;
    u16 posCopy;

    /* FAKEMATCH: narrow the initialized position in r0 before retaining
     * its copy in r7, preserving the ROM's separate narrowing/copy. */
    __asm__("" : : "r"(normalized));
    posCopy = normalized;
    player = (u8)posCopy;
    zone = pos >> 8;

    if (zone <= ZONE_MONSTER_4 && card->player != player) {
        int one = 1;    /* FAKEMATCH: shared 1, see ZONE_POSITION_BYTE */
        int side = player & one;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)) && CanCardTargetZone(card->card, player, zone)
            && posCopy != card->loc0)
            return (ZONE_POSITION_BYTE(target) >> 1) & one;
    }
    return 0;
}

/* A face-up monster of the opponent's (no targeting test: Fissure, Windstorm of Etaqua, key 1244). Returns
 * its isFaceUp bit. */
int EffectOpponentFaceUpMonsterCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone <= ZONE_MONSTER_4 && card->player != player) {
        int one = 1;    /* FAKEMATCH: shared 1, see ZONE_POSITION_BYTE */
        int side = player & one;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)))
            return (ZONE_POSITION_BYTE(target) >> 1) & one;
    }
    return 0;
}

/* Remove Trap: a face-up Trap card in zones 5-10 of either player. */
int EffectRemoveTrapCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int side = player & 1;
    struct DuelZone *target = ZONE_AT(side, zone);
    int targetId = CARD_ID(CARD_WORD(target->card));
    u16 idCopy; /* FAKEMATCH (found by decomp-permuter): the extra copy keeps the id live in r2 so `0x7FF & id` masks into r0 */
    int type;

    idCopy = targetId;
    if (targetId && (u32)(zone - ZONE_SPELL_0) <= ZONE_FIELD - ZONE_SPELL_0
        && target->isFaceUp) {
        type = CARD_TYPE(idCopy);
        return type == CARD_TYPE_TRAP;
    }
    return 0;
}

/* Block Attack: an opponent's attack-position monster that card may target. Returns 1 when isDefense is
 * clear. */
int EffectBlockAttackCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone <= ZONE_MONSTER_4 && player != card->player) {
        int one = 1;    /* FAKEMATCH: shared 1, see ZONE_POSITION_BYTE */
        int side = player & one;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)) && CanCardTargetZone(card->card, player, zone))
            return one & ~ZONE_POSITION_BYTE(target);
    }
    return 0;
}

/* Snatch Steal (and key 1514): an opponent's face-up monster that card may target, except card number 1351
 * (a non-EDS card, no constant in constants/cards.h). Returns its isFaceUp bit. */
int EffectSnatchStealCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone <= ZONE_MONSTER_4 && card->player != player) {
        int one = 1;    /* FAKEMATCH: shared 1, see ZONE_POSITION_BYTE */
        int side = player & one;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)) && CanCardTargetZone(card->card, player, zone)
            && CARD_NUMBER(CARD_ID(CARD_WORD(target->card))) != 1351)
            return (ZONE_POSITION_BYTE(target) >> 1) & one;
    }
    return 0;
}

/*
 * Tailor of the Fickle: a face-up equip card in zones 5-10 of either player that can be moved to another
 * monster. The card must be one of the movable equips listed below, an Equip Magic/Trap (stats subtype),
 * linked to a monster (IsCardLinkedToMonster), and have more than one monster it may equip.
 */
int EffectTailorOfTheFickleCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone > ZONE_MONSTER_4) {
        int side = player & 1;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (target->isFaceUp) {
            int targetId = CARD_ID(CARD_WORD(target->card));

            if (targetId) {
                switch (CARD_NUMBER(targetId)) {
                default:    /* Matching: first; at the end it changes the comparison tree */
                    return 0;
                case CARD_LEGENDARY_SWORD ... CARD_CYBER_SHIELD:        /* 300-316 */
                case CARD_MYSTICAL_MOON:
                case CARD_MALEVOLENT_NUZZLER ... CARD_POWER_OF_KAISHIN: /* 320-327 */
                case CARD_KUNAI_WITH_CHAIN:
                case CARD_MAGICAL_LABYRINTH:
                case CARD_SALAMANDRA:
                case CARD_MEGAMORPH:
                case CARD_METALMORPH:
                case CARD_BRIGHT_CASTLE:
                case CARD_7_COMPLETED:
                case CARD_BURNING_SPEAR:
                case CARD_GUST_FAN:
                case CARD_SWORD_OF_DEEP_SEATED:
                case CARD_GERM_INFECTION:
                case CARD_PARALYZING_POTION:
                case CARD_RING_OF_MAGNETISM:
                case CARD_STIM_PACK:
                case CARD_SWORD_OF_DRAGONS_SOUL:
                case CARD_1419:
                case CARD_1420:
                case CARD_1422:
                case CARD_1448 ... CARD_1450:
                case CARD_1540:
                case CARD_1548:
                case CARD_1550:
                {
                    /* Matching: the ROM recomputes the zone address and reloads the card word here. */
                    int equipSide = player & 1;
                    struct DuelZone *equip = ZONE_AT(equipSide, zone);

                    if (GetSpellSubtype(CARD_ID(CARD_WORD(equip->card))) == SPELL_EQUIP
                        && IsCardLinkedToMonster(player, zone) && CountValidEquipTargets(player, zone) > 1)
                        return 1;
                    break;
                }
                }
            }
        }
    }
    return 0;
}

/* Dust Tornado: any card, face up or down, in the opponent's zones 5-10. */
int EffectDustTornadoCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (card->player != player && (u32)(zone - ZONE_SPELL_0) <= ZONE_FIELD - ZONE_SPELL_0) {
        int side = player & 1;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)))
            return 1;
    }
    return 0;
}

/* Any card, face up or down, in zones 5-10 of either player (Heavy Storm, Mystical Space Typhoon, Giant
 * Trunade, Gust, Driving Snow). */
int EffectAnySpellTrapCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone >= ZONE_SPELL_0 && zone <= ZONE_FIELD) {
        int side = player & 1;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)))
            return 1;
    }
    return 0;
}

/* Nobleman of Crossout: a face-down monster on either side (no targeting test). Returns 1 when isFaceUp is
 * clear. */
int EffectNoblemanOfCrossoutCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone <= ZONE_MONSTER_4) {
        int one = 1;    /* FAKEMATCH: shared 1, see ZONE_POSITION_BYTE */
        int side = player & one;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)))
            return one & ~(ZONE_POSITION_BYTE(target) >> 1);
    }
    return 0;
}

/* Nobleman of Extermination: a face-down card in the spell/trap zones 5-9 of either player (not the field
 * zone). Returns 1 when isFaceUp is clear. */
int EffectNoblemanOfExterminationCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone >= ZONE_SPELL_0 && zone <= ZONE_SPELL_4) {
        int one = 1;    /* FAKEMATCH: shared 1, see ZONE_POSITION_BYTE */
        int side = player & one;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)))
            return one & ~(ZONE_POSITION_BYTE(target) >> 1);
    }
    return 0;
}

/* Key 1248: one of card's player's own face-up monsters that card may target and that is Summoned Skull or a
 * Thunder monster (effective type). */
int EffectOwnSkullOrThunderCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int side = player & 1;
    struct DuelZone *target = ZONE_AT(side, zone);
    int targetId = CARD_ID(CARD_WORD(target->card));
    u16 idCopy; /* FAKEMATCH (found by decomp-permuter): the extra copy keeps the id live in r4 so `0x7FF & id` masks into r4 */

    idCopy = targetId;
    if (player == card->player && zone <= ZONE_MONSTER_4 && target->isFaceUp
        && targetId && CanCardTargetZone(card->card, player, zone)
        && (CARD_NUMBER(idCopy) == CARD_SUMMONED_SKULL || GetZoneCardType(player, zone) == CARD_TYPE_THUNDER))
        return 1;
    return 0;
}

/*
 * Key 1318: one of card's player's own monsters with a level (not 0), while card 1418 is active on neither
 * field, if the deck holds a monster the effect may Special Summon for it: CollectEffectTargets(player,
 * 1318, level + 1) finds an Insect of exactly one level more (OCG Insect Imitation, hypothesis).
 */
int EffectTributeForInsectCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int side = player & 1;
    struct DuelZone *target = ZONE_AT(side, zone);
    int targetId = CARD_ID(CARD_WORD(target->card));

    if (player == card->player && zone <= ZONE_MONSTER_4 && targetId
        && CountActiveCardsOnField(0, CARD_1418) <= 0 && CountActiveCardsOnField(1, CARD_1418) <= 0) {
        int level = GetCardLevel(targetId);

        /* FAKEMATCH: retain the initialized result through the shared test
         * instead of jump-threading constant switch arms. No instructions. */
        __asm__("" : "+r"(level));
        if (level != 0) {
            int cardPlayer = card->player;
            /* The ROM evaluates the level a second time for the search. */
            int nextLevel = GetCardLevel(targetId) + 1;
            /* FAKEMATCH: the initialized r0 copy keeps argument 0
             * before the constant argument 1 in the original call. */
            register int arg0 __asm__("r0") = cardPlayer;

            if (CollectEffectTargetsInt(arg0, CARD_1318, nextLevel) > 0)
                return 1;
        }
    }
    return 0;
}

/* Keys 1316 and 1319: any of card's player's own monsters, face up or down, that card may target. */
int EffectTargetableOwnMonsterCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int side = player & 1;
    struct DuelZone *target = ZONE_AT(side, zone);
    int targetId = CARD_ID(CARD_WORD(target->card));

    if (CanCardTargetZone(card->card, player, zone) && player == card->player && zone <= ZONE_MONSTER_4
        && targetId)
        return 1;
    return 0;
}

/* Key 1417: a face-up Magic card in zones 5-10 of either player. */
int EffectFaceUpMagicCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if ((u32)(zone - ZONE_SPELL_0) <= ZONE_FIELD - ZONE_SPELL_0) {
        int side = player & 1;
        struct DuelZone *target = ZONE_AT(side, zone);
        int targetId = CARD_ID(CARD_WORD(target->card));

        if (targetId && target->isFaceUp && CARD_TYPE(targetId) == CARD_TYPE_MAGIC)
            return 1;
    }
    return 0;
}

/* Key 1539: any card, face up or down, in the opponent's zones 5-10. Same result as EffectDustTornadoCheck;
 * a separate copy in the ROM with another zone test. */
int EffectOpponentSpellTrapCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone > ZONE_MONSTER_4) {
        int side = player & 1;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)) && card->player != player)
            return 1;
    }
    return 0;
}

/* Key 1510: a face-up monster on either side; returns its specialSummoned bit (card word bit 16). */
int EffectSpecialSummonedMonsterCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int side = player & 1;
    struct DuelZone *target = ZONE_AT(side, zone);
    u32 word = CARD_WORD(target->card);
    int targetId = CARD_ID(word);

    if (zone <= ZONE_MONSTER_4 && targetId && target->isFaceUp)
        return (word & CARD_WORD_SPECIAL_SUMMONED) >> 16;
    return 0;
}

/* Key 1545: a face-down card in the spell/trap zones 5-9 of either player. Same result as
 * EffectNoblemanOfExterminationCheck; a separate copy in the ROM. */
int EffectFaceDownSpellTrapCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if ((u32)(zone - ZONE_SPELL_0) <= ZONE_SPELL_4 - ZONE_SPELL_0) {
        int side = player & 1;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(target->card)) && !target->isFaceUp)
            return 1;
    }
    return 0;
}

/* enum CardKind of a card (as GetCardKind in other units): Obelisk counts as Ritual, Slifer and Ra as Effect,
 * Magic, Trap and Ticket cards get their own kinds, other cards the kind bits of their stats. */
static inline int GetCardKind(int id)
{
    switch ((int)CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    default:
        return CARD_STATS_KIND(CARD_STATS(id));
    }
}

/* Key 1546: a face-up Fusion monster on either side that card may target. */
int EffectFaceUpFusionMonsterCheck(struct ChainEntry *card, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int side = player & 1;
    struct DuelZone *target = ZONE_AT(side, zone);
    int targetId = CARD_ID(CARD_WORD(target->card));

    if (zone <= ZONE_MONSTER_4 && targetId && target->isFaceUp
        && CanCardTargetZone(card->card, player, zone)) {
        if (CARD_TYPE(targetId) > CARD_TYPE_REPTILE)  /* not a monster */
            return 0;
        if (GetCardKind(targetId) == CARD_KIND_FUSION)
            return 1;
    }
    return 0;
}

/* ---- ChainA handlers (activation costs) ---- */

/* Pay the LP cost of link's card (DUEL_CMD_LOSE_LP): 500, 800, 1000, 3000, 5000, or half the LP for Solemn
 * Judgment. Inspection's case is unreachable: its gCardEffects row has no ChainA. Returns 1 (done). */
int EffectPayLifePointsChainA(struct ChainEntry *link)
{
    int cost = 0;

    switch (CARD_NUMBER(link->card)) {
    case CARD_ULTIMATE_OFFERING:
    case CARD_INSPECTION:
        cost = 500;
        break;
    case CARD_PREMATURE_BURIAL:
        cost = 800;
        break;
    case CARD_MONSTER_EYE:
    case CARD_SEVEN_TOOLS_OF_THE_BANDIT:
    case CARD_CONFISCATION:
    case CARD_DELINQUENT_DUO:
    case CARD_SEAL_OF_THE_ANCIENTS:
        cost = 1000;
        break;
    case CARD_GALE_DOGRA:
        cost = 3000;
        break;
    case CARD_CYBER_STEIN:
        cost = 5000;
        break;
    case CARD_SOLEMN_JUDGMENT:
        cost = HalveRoundDown(gDuelPlayers[link->player].lifePoints);
        break;
    }
    if (cost > 0)
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_LOSE_LP), cost, 1, 0);
    return 1;
}

/* Tribute the activating monster itself (Valkyrion the Magna Warrior, keys 1430/1433/1439/1444). Returns 1. */
int EffectTributeSelfChainA(struct ChainEntry *link, u16 unused)
{
    TributeMonster(link->player, link->zone);
    return 1;
}

/* Jigen Bakudan: the Standby Phase activation (event RESPONSE_OWN_STANDBY) tributes the bomb itself; other
 * activations pay nothing. Returns 1. */
int EffectJigenBakudanChainA(struct ChainEntry *link, u16 unused)
{
    if (link->event == RESPONSE_OWN_STANDBY)
        TributeMonster(link->player, link->zone);
    return 1;
}

/*
 * Crush Card's cost: tribute a DARK monster with 1000 ATK or less. Step 0 opens the prompt. Step 1 runs the
 * monster cursor; a picked monster that does not qualify plays the error sound, a valid one is tributed
 * (returns 1). B steps back to step 0. Returns 0 while the prompt runs.
 */
int EffectCrushCardChainA(struct ChainEntry *link)
{
    struct ZoneCardStats stats;

    switch (gChain.costStep) {
    case 0:
        TextBoxOpen(COST_PROMPT_POS, COST_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrCrushCardTributePrompt);
        gChain.costStep++;
        return 0;
    case 1:
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            GetZoneCardStats(link->player, gDuelScreen.selIndex, &stats);
            if (stats.atk > 1000 || stats.attribute != ATTRIBUTE_DARK) {
                PlaySE(SE_ERROR);
            } else {
                TributeMonster(link->player, gDuelScreen.selIndex);
                return 1;
            }
        }
        if (gMain.newKeys & B_BUTTON)
            gChain.costStep--;      /* back to step 0 */
        return 0;
    }
    return 0;
}

/* Toon World's cost: pay 1000 LP, then reset the LP-paid counter of its zone (DUEL_CMD_UPDATE_ZONE_LP_PAID
 * with 0). Returns 1. */
int EffectToonWorldChainA(struct ChainEntry *link, u16 unused)
{
    DuelCmd_Push(CMD_FOR(link, DUEL_CMD_LOSE_LP), 1000, 1, 0);
    DuelCmd_Push(CMD_FOR(link, DUEL_CMD_UPDATE_ZONE_LP_PAID), link->zone, 0, 0);
    return 1;
}

/* Discard cost of link's card: 1 card (Tribute to The Doomed, Magic Jammer), 2 (Darkness Approaches) or 5
 * (Final Destiny), through the PROMPT_DISCARD_COST prompt. Returns 1. */
int EffectDiscardCostChainA(struct ChainEntry *link)
{
    int count = 0;

    switch (CARD_NUMBER(link->card)) {
    case CARD_MAGIC_JAMMER:
    case CARD_TRIBUTE_TO_THE_DOOMED:
        count = 1;
        break;
    case CARD_DARKNESS_APPROACHES:
        count = 2;
        break;
    case CARD_FINAL_DESTINY:
        count = 5;
        break;
    }
    if (count > 0)
        DuelPrompt_PostDiscardCost(link->player, count, 0, 0);
    return 1;
}

/* Tribute one monster as the cost (Horn of Heaven, Share the Pain): step 0 opens the prompt, step 1 runs the
 * monster cursor and tributes the picked monster (no other test, no cancel). Returns 1 when done. */
int EffectTributeChosenMonsterChainA(struct ChainEntry *link)
{
    switch (gChain.costStep) {
    case 0:
        TextBoxOpen(COST_PROMPT_POS, COST_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectTributeMonster);
        gChain.costStep++;
        return 0;
    case 1:
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            TributeMonster(gDuelScreen.selPlayer, gDuelScreen.selIndex);
            return 1;
        }
        return 0;
    }
    return 0;
}

/* Key 1212: pay half of the player's LP (HalveRoundDown). Returns 1. */
int EffectPayHalfLifePointsChainA(struct ChainEntry *link)
{
    DuelCmd_Push(CMD_FOR(link, DUEL_CMD_LOSE_LP), HalveRoundDown(gDuelPlayers[link->player & 1].lifePoints), 1, 0);
    return 1;
}
