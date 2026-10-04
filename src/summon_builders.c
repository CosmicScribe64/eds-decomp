/*
 * summon_builders (0x08055EB0-0x08056ECC): Special Summon builders and the CPU's hand and tribute pickers
 * (wiki/functions/summon-builders-c.md).
 *
 *  - QueueFlipSummon, QueueSpecialSummon, QueueSpecialSummonChoosePosition and QueueSpecialSummonFromHand fill the
 *    pending summon record gSummonAction (struct SummonAction, summon.h) for kinds 3-6 and call SummonAction_Start.
 *    The builders of kinds 1 and 2 are in summon_action.c.
 *  - The Ai* functions help the CPU opponent (player 1): which cards it keeps, which monsters it tributes, and
 *    which hand card it summons or discards. A card is judged by its stats word in gCardStats.
 *  - The Find*CardByNumber functions look up a card number in a player's hand, deck or fusion deck.
 */
#include "global.h"
#include "card_data.h"              /* gCardIdToNumber, CARD_ID_MASK, CARD_STATS_* extractors */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_STATS_* layout, enum CardType, enum CardKind */
#include "constants/duel.h"         /* ZoneStatusFlag, SummonActionKind */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that summon.h does not pull in the legacy header.
 * After H0, replace the block (BEGIN to END) with #include "legacy/duel.h"
 * (see build/readability/issues/summon_builders.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u8 unk4[0x94 - 0x4];
};

struct DuelPlayer {
    u8 unk0[0x2];
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 unk4;
    u8 fusionCount;                 /* +0x005: entries in fusionDeck[] */
    u8 unk6[0x684 - 0x6];
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    u8 unk904[0xA44 - 0x904];
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    u8 unkB84[0xD64 - 0xB84];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

/* Effective stats of the card in a zone (GetZoneCardStats). */
struct ZoneCardStats {
    u16 id;                         /* +0x0: card ID */
    u8 type:5;                      /* +0x2 bits 0-4: effective enum CardType */
    u8 attribute:3;                 /* +0x2 bits 5-7: effective enum CardAttribute */
    u8 unk3;
    s32 atk;                        /* +0x4: effective ATK */
    s32 def;                        /* +0x8: effective DEF */
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelCard gDuelHands[];            /* 0x02019968 = gDuelPlayers[0].hand (player stride 0xD64) */
extern struct DuelZone gDuelZonesP1[11];        /* 0x0201A070 = gDuelPlayers[1].zones */

u32 IsSpecialSummonOnly(u16 cardId);
u32 IsFusionMonster(u16 cardId);
void CopyDuelCard(u32 *dst, u32 *src);
int CountActiveCardsOnField(int player, u16 cardNo);
int FindHandCardByNumber(int player, u16 cardNo);
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);
void GetZoneCardStats(int player, int zone, struct ZoneCardStats *out);
int FindDeckCardByNumber(int player, u16 number, int limit);
int FindFusionDeckCardByNumber(int player, u16 number);
/* ---- END duel.h stand-in ---- */

#include "ai.h"                     /* the Ai* pickers defined here, AiHasUsableSpellTrap */
#include "effect.h"                 /* PayChainEnergyCost */
#include "summon.h"                 /* struct SummonAction, gSummonAction, the Queue* builders */

#define PLAYER_STRIDE 0xD64         /* sizeof(struct DuelPlayer) */
#define ZONE_STRIDE 0x94            /* sizeof(struct DuelZone) */

/* A card word and its card ID (bits 0-11), read through the whole word. */
#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)

/* Matching: ROM tables read through integer-constant addresses (the ROM reloads the table address at every
 * use; the symbols generate other literal pools). */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])     /* gCardStats */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])    /* gCardIdToNumber */
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))

/* The card word of the CPU's (player 1) field zone i. Matching: the ROM reads player 1's zones through their own
 * literal, gDuelZonesP1 (0x0201A070 = gDuelZones[1].zones), not through gDuelZones + PLAYER_STRIDE. */
#define ZONE1_WORD(i) CARD_WORD(gDuelZonesP1[i].card)

/* The AI values a Divine-Beast (the three Egyptian Gods) at 4000 ATK and DEF. */
#define AI_DIVINE_VALUE 4000

/* Tributes a Normal Summon needs by level: levels 1-4 none, 5-6 one monster, 7 and up two. */
#define LEVEL_MAX_NO_TRIBUTE 4
#define LEVEL_MAX_ONE_TRIBUTE 6

/* AiIsKeyCard tiers the pickers use: 1 keeps the Exodia pieces and the staple Magic and Trap cards. */
#define AI_KEY_TIER_STAPLES 1

/* The highest card type of a monster; Trap, Magic, Ticket and Divine (21-24) are not ordinary monsters. */
#define CARD_TYPE_LAST_MONSTER CARD_TYPE_REPTILE

/* Monster level as the game computes it: Trap, Magic and Ticket cards count as 0, Divine-Beasts as 10. A statement,
 * so that every arm assigns the result itself. */
#define CARD_LEVEL(id, r)                                     \
    switch ((int)CARD_TYPE(id)) {                             \
    case CARD_TYPE_TRAP:                                      \
    case CARD_TYPE_MAGIC:                                     \
    case CARD_TYPE_TICKET:                                    \
        r = 0;                                                \
        break;                                                \
    case CARD_TYPE_DIVINE:                                    \
        r = 10;                                               \
        break;                                                \
    default:                                                  \
        r = CARD_STATS_LEVEL(CARD_STATS(id));                 \
        break;                                                \
    }

/* AI value of a card by its printed ATK (stored / 10): 0 for Trap, Magic and Ticket cards, 4000 for a
 * Divine-Beast. */
static inline int CardValue(u16 id)
{
    int r;

    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        r = 0;
        break;
    case CARD_TYPE_DIVINE:
        r = AI_DIVINE_VALUE;
        break;
    default:
        r = CARD_STATS_ATK(CARD_STATS(id)) * CARD_STATS_POINTS_SCALE;
        break;
    }
    return r;
}

/* The same with the printed DEF. Matching: the ROM masks the low halfword. The u16 AND makes 0x1FF a halfword
 * constant, which reload loads and copies (ldr r3, =0x1FF; adds r0, r3). */
static inline int CardDefValue(u16 id)
{
    int r;

    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        r = 0;
        break;
    case CARD_TYPE_DIVINE:
        r = AI_DIVINE_VALUE;
        break;
    default:
        r = ((u16)CARD_STATS(id) & CARD_STATS_DEF_MASK) * CARD_STATS_POINTS_SCALE;
        break;
    }
    return r;
}

/* The same ATK value as a statement, so that every arm writes the result local itself (CardValue's return copy
 * gives the ROM an extra move). */
#define CARD_ATK_VALUE(id, r)                                       \
    switch ((int)CARD_TYPE(id)) {                                   \
    case CARD_TYPE_TRAP:                                            \
    case CARD_TYPE_MAGIC:                                           \
    case CARD_TYPE_TICKET:                                          \
        r = 0;                                                      \
        break;                                                      \
    case CARD_TYPE_DIVINE:                                          \
        r = AI_DIVINE_VALUE;                                        \
        break;                                                      \
    default:                                                        \
        r = CARD_STATS_ATK(CARD_STATS(id)) * CARD_STATS_POINTS_SCALE; \
        break;                                                      \
    }

/* "Kind" of a card (enum CardKind): 0 for an ordinary monster. The AI uses it to keep from picking special cards
 * first: Obelisk counts as Ritual, Slifer and Ra as Effect monsters, Trap/Magic/Ticket by their own kinds. */
static inline u8 CardKey(u16 id)
{
    int number = CARD_NUMBER(id);

    switch (number) {
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
    }
    return CARD_STATS_KIND(CARD_STATS(id));
}

/*
 * Kind 3: queue the Flip Summon of the face-down monster in (player, zone) and start it. The record gets
 * the zone as its source, face-up Attack Position, no tributes and statusFlags 3.
 */
void QueueFlipSummon(int player, int zone)
{
    struct SummonAction *action = &gSummonAction;

    action->player = player;
    action->zone = zone;
    action->sourceIndex = zone;
    action->isFaceUp = 1;
    action->isDefense = 0;
    action->tribute1Zone = 0;
    action->tribute2Zone = 0;
    action->hasTribute1 = 0;
    action->hasTribute2 = 0;
    action->cardId = CARD_ID(*(u32 *)((player & 1) * PLAYER_STRIDE + zone * ZONE_STRIDE + (u32)gDuelZones));
    action->kind = SUMMON_ACTION_FLIP;
    action->statusFlags = ZONE_STATUS_NORMAL_SUMMONED | ZONE_STATUS_UNK14;
    SummonAction_Start();
}

/*
 * Kind 4: queue the Special Summon of *card (a card that is not in the hand or on the field, for example from
 * the fusion deck or the graveyard) into the player's first free monster zone, and start it. faceUp and
 * isDefense choose the position. Light of Intervention on either field, or a card that cannot be Set
 * (IsSpecialSummonOnly), forces it face up. The card word is copied into the record. statusFlags
 * are the caller's flags plus ZONE_STATUS_SPECIAL_SUMMONED.
 */
void QueueSpecialSummon(int player, struct DuelCard *card, u16 faceUp, u16 isDefense, u16 statusFlags)
{
    struct SummonAction *action;

    if (CountActiveCardsOnField(0, CARD_LIGHT_OF_INTERVENTION) != 0
        || CountActiveCardsOnField(1, CARD_LIGHT_OF_INTERVENTION) != 0)
        faceUp = 1;
    if (IsSpecialSummonOnly(CARD_ID(CARD_WORD(*card))) != 0)
        faceUp = 1;
    action = &gSummonAction;
    action->player = player;
    action->zone = FindFreeMonsterZone(player);
    action->sourceIndex = 0;
    action->isFaceUp = faceUp;
    action->isDefense = isDefense;
    action->tribute1Zone = 0;
    action->tribute2Zone = 0;
    action->hasTribute1 = 0;
    action->hasTribute2 = 0;
    action->cardId = CARD_ID(CARD_WORD(*card));
    action->statusFlags = statusFlags;    /* the ROM stores the caller's flags before the copy and the kind */
    CopyDuelCard((u32 *)&action->card, (u32 *)card);
    action->kind = SUMMON_ACTION_SPECIAL;
    action->statusFlags = statusFlags | ZONE_STATUS_SPECIAL_SUMMONED;
    SummonAction_Start();
}

/*
 * Kind 5: like QueueSpecialSummon, but the position is asked when the step machine runs
 * (SummonStep_SpecialChoosePosition), so isDefense starts at 0. faceUp (forced as in QueueSpecialSummon) only
 * matters if Defense Position is chosen. statusFlags are the caller's flags plus
 * ZONE_STATUS_SPECIAL_SUMMONED.
 */
void QueueSpecialSummonChoosePosition(int player, struct DuelCard *card, u16 faceUp, u16 statusFlags)
{
    struct SummonAction *action;

    if (CountActiveCardsOnField(0, CARD_LIGHT_OF_INTERVENTION) != 0
        || CountActiveCardsOnField(1, CARD_LIGHT_OF_INTERVENTION) != 0)
        faceUp = 1;
    if (IsSpecialSummonOnly(CARD_ID(CARD_WORD(*card))) != 0)
        faceUp = 1;
    action = &gSummonAction;
    action->player = player;
    action->zone = FindFreeMonsterZone(player);
    action->sourceIndex = 0;
    action->isFaceUp = faceUp;
    action->isDefense = 0;
    action->tribute1Zone = 0;
    action->tribute2Zone = 0;
    action->hasTribute1 = 0;
    action->hasTribute2 = 0;
    action->cardId = CARD_ID(CARD_WORD(*card));
    CopyDuelCard((u32 *)&action->card, (u32 *)card);
    action->kind = SUMMON_ACTION_SPECIAL_CHOOSE_POSITION;
    action->statusFlags = statusFlags | ZONE_STATUS_SPECIAL_SUMMONED;
    SummonAction_Start();
}

/*
 * Kind 6: queue the Special Summon of hand[handIndex] into zone, always face up, and start it. attackPosition
 * nonzero chooses Attack Position, 0 face-up Defense. The tributes word is decoded as in QueueNormalSummon (the
 * summon step tributes the player's own monsters). statusFlags are 5. Chain Energy's cost is paid before the
 * start.
 *
 * The AI passes the packed tribute word directly in r3 and the callee decodes only the low halfwords of that
 * word and of the fifth (stack) argument.
 */
void QueueSpecialSummonFromHand(int player, int handIndex, int zone, int tributes, int attackPosition)
{
    u16 tributeBytes = tributes;
    u16 attackWord = attackPosition;

    gSummonAction.player = player;
    gSummonAction.zone = zone;
    gSummonAction.sourceIndex = handIndex;
    gSummonAction.isFaceUp = 1;
    gSummonAction.isDefense = attackWord == 0;
    /* Decode the two tribute bytes (zone | owner << 4 | present << 7). */
    if (tributeBytes != 0) {
        u8 lo = tributeBytes;
        u8 hi = tributeBytes >> 8;

        gSummonAction.tribute1Zone = lo & 7;
        gSummonAction.tribute2Zone = hi & 7;
        gSummonAction.hasTribute1 = lo >> 7;
        gSummonAction.hasTribute2 = hi >> 7;
        gSummonAction.tribute1Player = (lo >> 4) & 1;
        gSummonAction.tribute2Player = (hi >> 4) & 1;
    } else {
        gSummonAction.tribute1Zone = 0;
        gSummonAction.tribute2Zone = 0;
        gSummonAction.hasTribute1 = 0;
        gSummonAction.hasTribute2 = 0;
    }
    gSummonAction.cardId = CARD_ID(*(u32 *)((player & 1) * PLAYER_STRIDE + handIndex * 4 + (u32)gDuelHands));
    gSummonAction.kind = SUMMON_ACTION_SPECIAL_FROM_HAND;
    gSummonAction.statusFlags = ZONE_STATUS_SPECIAL_SUMMONED | ZONE_STATUS_UNK14;
    PayChainEnergyCost(player);
    SummonAction_Start();
}

/*
 * 1 if the CPU should keep the card with this card number rather than discard or sacrifice it.
 * Always: the five Exodia pieces. From tier 1: the staple Magic and Trap cards (Dark Hole, Raigeki, Mirror
 * Force, ...). From tier 2: Kuriboh and Cyber-Stein. The AI passes tier 1 or 2 as a "not this card" filter.
 */
int AiIsKeyCard(int tier, u16 number)
{
    switch (number) {
    case CARD_RIGHT_LEG_OF_THE_FORBIDDEN_ONE:
    case CARD_LEFT_LEG_OF_THE_FORBIDDEN_ONE:
    case CARD_RIGHT_ARM_OF_THE_FORBIDDEN_ONE:
    case CARD_LEFT_ARM_OF_THE_FORBIDDEN_ONE:
    case CARD_EXODIA_THE_FORBIDDEN_ONE:
        return 1;
    }
    if (tier > 0) {
        switch (number) {
        case CARD_DARK_HOLE:
        case CARD_RAIGEKI:
        case CARD_SWORDS_OF_REVEALING_LIGHT:
        case CARD_HARPIES_FEATHER_DUSTER:
        case CARD_RELINQUISHED:
        case CARD_TOON_WORLD:
        case CARD_MONSTER_REBORN:
        case CARD_POT_OF_GREED:
        case CARD_CHANGE_OF_HEART:
        case CARD_MAGIC_JAMMER:
        case CARD_SEVEN_TOOLS_OF_THE_BANDIT:
        case CARD_MIRROR_FORCE:
        case CARD_HEAVY_STORM:
        case CARD_SNATCH_STEAL:
        case CARD_SEBEKS_BLESSING:
            return 1;
        }
    }
    if (tier > 1) {
        if (number == CARD_KURIBOH || number == CARD_CYBER_STEIN)
            return 1;
    }
    return 0;
}

/*
 * The monster zone (0-4, not excludeZone) of player 1 that the CPU tributes, or -1. Sangan first, then Witch
 * of the Black Forest (both search the deck when sent to the graveyard), else the monster with the lowest
 * ATK * 2 + DEF after modifiers. A Fusion monster is only picked when allowFusion is nonzero.
 */
int AiPickTributeMonster(int excludeZone, u16 allowFusion)
{
    int i;
    int best;
    int bestZone;

    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && i != excludeZone && CARD_NUMBER(id) == CARD_SANGAN)
            return i;
    }
    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && i != excludeZone && CARD_NUMBER(id) == CARD_WITCH_OF_THE_BLACK_FOREST)
            return i;
    }
    best = 32000;       /* above any ATK * 2 + DEF */
    bestZone = -1;
    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && i != excludeZone) {
            struct ZoneCardStats stats;
            int allowed = 1;
            int value;

            if (IsFusionMonster(id))
                allowed = 0;
            if (allowed == 0 && allowFusion == 0)
                continue;
            GetZoneCardStats(1, i, &stats);
            value = stats.atk * 2 + stats.def;
            if (value < best) {
                bestZone = i;
                best = value;
            }
        }
    }
    return bestZone;
}

/*
 * 1 if player 1 has enough monsters to tribute for the card: levels 1-4 need none, 5-6 need one monster
 * (AiPickTributeMonster finds one), 7 and up (a Divine-Beast counts as 10) need two different ones.
 */
int AiHasTributesFor(u16 cardId)
{
    s8 level;
    int first, second;

    CARD_LEVEL(cardId, level);
    if (level < 0)      /* never true: the ROM tests it anyway (level is s8) */
        goto needTwo;
    if (level <= LEVEL_MAX_NO_TRIBUTE)
        return 1;
    if (level > LEVEL_MAX_ONE_TRIBUTE)
        goto needTwo;
    {
        int none = -1;
        if (AiPickTributeMonster(none, 0) != none)
            return 1;
        return 0;
    }
needTwo:
    {
        int none = -1;
        first = AiPickTributeMonster(none, 0);
        second = AiPickTributeMonster(first, 0);
        if (first == none)
            return 0;
        if (second == none)
            return 0;
    }
    return 1;
}

/*
 * The monster zone of player 1 that the CPU tributes as the cost of an effect, or -1. A token (card number
 * 1920-1999) comes first.
 *
 * Original bug: the Sangan and Witch of the Black Forest passes assign (=) instead of compare (==). They store
 * the number into the ROM table gCardIdToNumber, which does nothing, and return the first occupied zone
 * other than excludeZone. The Witch pass and the lowest ATK + DEF pass can therefore never find anything;
 * they are kept because the unit must match the ROM byte for byte.
 */
int AiPickEffectTribute(int excludeZone)
{
    int i;
    int best;
    int bestZone;
    struct ZoneCardStats stats;

    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && (u16)(CARD_NUMBER(id) - CARD_NUMBER_TOKEN_FIRST) <= CARD_NUMBER_TOKEN_END - CARD_NUMBER_TOKEN_FIRST - 1)
            return i;
    }
    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && i != excludeZone) {
            *(u16 *)&CARD_NUMBER(id) = CARD_SANGAN;     /* original bug: '=' for '==' */
            return i;
        }
    }
    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && i != excludeZone) {
            *(u16 *)&CARD_NUMBER(id) = CARD_WITCH_OF_THE_BLACK_FOREST;  /* original bug: '=' for '==' */
            return i;
        }
    }
    best = 99999;       /* above any ATK + DEF */
    bestZone = -1;
    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        if (CARD_ID(ZONE1_WORD(i)) != 0 && i != excludeZone) {
            int value;
            GetZoneCardStats(1, i, &stats);
            value = stats.atk + stats.def;
            if (best > value) {
                bestZone = i;
                best = value;
            }
        }
    }
    return bestZone;
}

/*
 * The hand index of the card of `player` with the lowest printed ATK + DEF that the CPU can part with, or -1.
 * Three passes of decreasing strictness (a God counts 4000 + 4000, Trap and Magic cards 0):
 *  1. monsters with kind 0 (Normal monsters) that are not AiIsKeyCard(1);
 *  2. any monster that is not a key card;
 *  3. any hand card that is not a key card.
 * `players` is gDuelPlayers (the second and third pass index it through a pointer, as the ROM does).
 */
int AiPickWeakestHandCard(struct DuelPlayer *players, int player)
{
    int bestIndex = -1;
    int best = 9999;    /* above any ATK + DEF (a God counts 8000) */
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));
        if (id != 0) {
            if (CARD_TYPE(id) <= CARD_TYPE_LAST_MONSTER && CardKey(id) == 0 && (u16)AiIsKeyCard(AI_KEY_TIER_STAPLES, CARD_NUMBER(id)) == 0) {
                int atk;
                int value;

                CARD_ATK_VALUE(id, atk);
                value = CardDefValue(id) + atk;
                if (best > value) {
                    best = value;
                    bestIndex = i;
                }
            }
        }
    }
    if (bestIndex >= 0)
        return bestIndex;
    best = 9999;
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        struct DuelPlayer *side = players + (player & 1);
        u32 offset = i * 4 + OFFSET_OF(struct DuelPlayer, hand);
        u16 id = CARD_ID(*(u32 *)((u32)side + offset));
        if (id != 0) {
            const u32 *stats = &CARD_STATS(id);
            if (CARD_STATS_TYPE(*stats) <= CARD_TYPE_LAST_MONSTER && (u16)AiIsKeyCard(AI_KEY_TIER_STAPLES, CARD_NUMBER(id)) == 0) {
                int atk;
                int value;

                CARD_ATK_VALUE(id, atk);
                value = CardDefValue(id) + atk;
                if (best > value) {
                    best = value;
                    bestIndex = i;
                }
            }
        }
    }
    if (bestIndex >= 0)
        return bestIndex;
    best = 9999;
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        struct DuelPlayer *side = players + (player & 1);
        u32 offset = i * 4 + OFFSET_OF(struct DuelPlayer, hand);
        u16 id = CARD_ID(*(u32 *)((u32)side + offset));
        if ((u16)AiIsKeyCard(AI_KEY_TIER_STAPLES, CARD_NUMBER(id)) == 0) {
            int atk;
            int value;

            CARD_ATK_VALUE(id, atk);
            value = CardDefValue(id) + atk;
            if (best > value) {
                best = value;
                bestIndex = i;
            }
        }
    }
    return bestIndex;
}

/*
 * The hand index of the monster of `player` that the CPU should play, or -1: the one with the highest printed
 * ATK (a God counts 4000) that is neither special-summon-only nor a key card (AiIsKeyCard(1)). The first
 * pass only takes monsters of level 5 or higher, the second any level. `players` is gDuelPlayers.
 */
int AiPickStrongestHandMonster(struct DuelPlayer *players, int player)
{
    int bestIndex = -1;
    int best = -1;
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        struct DuelPlayer *side = players + (player & 1);
        u32 offset = i * 4 + OFFSET_OF(struct DuelPlayer, hand);
        u16 id = CARD_ID(*(u32 *)((u32)side + offset));
        if (id != 0) {
            const u32 *stats = &CARD_STATS(id);
            if (CARD_STATS_TYPE(*stats) <= CARD_TYPE_LAST_MONSTER && IsSpecialSummonOnly(id) == 0
                && (u16)AiIsKeyCard(AI_KEY_TIER_STAPLES, CARD_NUMBER(id)) == 0) {
                int value = CardValue(id);
                if (best < value) {
                    u32 level;
                    CARD_LEVEL(id, level);
                    if (level > LEVEL_MAX_NO_TRIBUTE) {
                        best = value;
                        bestIndex = i;
                    }
                }
            }
        }
    }
    if (bestIndex >= 0)
        return bestIndex;
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        struct DuelPlayer *side = players + (player & 1);
        u32 offset = i * 4 + OFFSET_OF(struct DuelPlayer, hand);
        u16 id = CARD_ID(*(u32 *)((u32)side + offset));
        if (id != 0) {
            const u32 *stats = &CARD_STATS(id);
            if (CARD_STATS_TYPE(*stats) <= CARD_TYPE_LAST_MONSTER && IsSpecialSummonOnly(id) == 0
                && (u16)AiIsKeyCard(AI_KEY_TIER_STAPLES, CARD_NUMBER(id)) == 0) {
                int value = CardValue(id);
                if (best < value) {
                    best = value;
                    bestIndex = i;
                }
            }
        }
    }
    return bestIndex;
}

/*
 * The hand index that player 1 discards. With a way to revive a monster (Monster Reborn or Premature Burial in
 * hand, or a set Call of the Haunted it can activate) and a free monster zone, it throws away the strongest
 * monster so that it can bring it back. Otherwise it discards Sinister Serpent, which returns to the hand,
 * and as a last choice the weakest card.
 */
int AiPickDiscard(void)
{
    int pick = -1;
    int i;

    if (FindHandCardByNumber(1, CARD_MONSTER_REBORN) > pick || FindHandCardByNumber(1, CARD_PREMATURE_BURIAL) > pick
        || AiHasUsableSpellTrap(CARD_CALL_OF_THE_HAUNTED) != 0) {
        if (CountFreeMonsterZones(1) > 0) {
            pick = AiPickStrongestHandMonster(gDuelPlayers, 1);
            if (pick >= 0)
                return pick;
        }
    }
    for (i = 0; i < gDuelPlayers[1].handCount; i++) {
        if (CARD_NUMBER(CARD_ID(CARD_WORD(gDuelPlayers[1].hand[i]))) == CARD_SINISTER_SERPENT)
            return i;
    }
    return AiPickWeakestHandCard(gDuelPlayers, 1);
}

/* The index of the first card in the player's deck with that card number among the first `limit` cards, or -1. */
int FindDeckCardByNumber(int player, u16 number, int limit)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].deckCount && i < limit; i++) {
        u32 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].deck[i])) & CARD_ID_MASK;
        if (gCardIdToNumber[id] == number)
            return i;
    }
    return -1;
}

/*
 * The index of the first card in the player's hand with card number (u16)numberWord, or -1. The AI's own copy of
 * FindHandCardByNumber: both exist in the ROM. Signed table entries arrive as words; the original low-half decode
 * is kept.
 */
int AiFindHandCardByNumber(int player, int numberWord)
{
    u16 number = numberWord;
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u32 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i])) & CARD_ID_MASK;
        if (gCardIdToNumber[id] == number)
            return i;
    }
    return -1;
}

/* The index of the first card in the player's fusion deck with that card number, or -1. */
int FindFusionDeckCardByNumber(int player, u16 number)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].fusionCount; i++) {
        u32 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].fusionDeck[i])) & CARD_ID_MASK;
        if (gCardIdToNumber[id] == number)
            return i;
    }
    return -1;
}
