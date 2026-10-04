/*
 * ai_picks (0x08056ECC-0x08057EDF): decision helpers of the CPU opponent (wiki/functions/ai-picks-c.md).
 *
 * The CPU is always player 1, the human is player 0. This unit holds:
 *  - picks from card lists and hands: AiPickCardListEntry (search / revive effects), AiPickOpponentHandCard
 *    (which card to take from the human's hand), AiShouldSetMonster (Defense or Attack Position);
 *  - monster scoring over the five monster zones: strongest, weakest, total ATK, position-change test,
 *    "can this ATK beat that monster";
 *  - the attack planner: AiEvalAttack -> AiFindAttackTarget -> AiChooseAttack, which stores the winning
 *    AttackPlan in gAiWork.bestAttack;
 *  - Exodia counters for the Rare Hunter strategy (AI_FLAG_EXODIA);
 *  - the duel-state sandbox: AiBackupDuelState / AiRestoreDuelState DMA both players into
 *    gAiWork.duelBackup, so a simulated summon and Battle Phase can be undone, and
 *    AiSimSetAttackPositions prepares the field for such a simulation (include/ai.h).
 */
#include "global.h"
#include "card_data.h"              /* gCardStats, gCardIdToNumber, CARD_ID_MASK, CARD_STATS_* extractors */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, CARD_STATS_* layout */
#include "constants/duel.h"         /* MONSTER_ZONE_COUNT */
#include "constants/game.h"         /* enum DuelistId */
#include "legacy/gba.h"                    /* REG_BASE */
#include "legacy/main.h"                   /* gMain.opponent */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that battle.h and card_list_view.h do not pull in the legacy header. After H0, replace the
 * block (BEGIN to END) with #include "legacy/duel.h" (see build/readability/issues/ai_picks.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 unk6_2:6;
    u8 unk7_0:2;
    u8 positionLocked:1;            /* +0x07 bit 2: cannot change position */
    u8 unk7_3:1;                    /* +0x07 bit 3: blocks AiCanChangePosition; meaning unknown */
    u8 unk7_4:4;
    u8 unk8[0x94 - 0x8];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 unk5[0x26 - 0x5];
    u16 attackedMask;               /* +0x026: monster zones that have attacked this turn */
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    struct DuelCard graveyard[80];  /* +0x904 */
    u8 unkA44[0xD64 - 0xA44];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

/* Effective stats of the card in a zone (GetZoneCardStats). */
struct ZoneCardStats {
    u16 id;                         /* +0x0: card ID */
    u8 type:5;                      /* +0x2 bits 0-4: effective enum CardType */
    u8 attribute:3;                 /* +0x2 bits 5-7 */
    u8 unk3;
    s32 atk;                        /* +0x4: effective ATK */
    s32 def;                        /* +0x8: effective DEF */
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

/* 1 if card number cardNo has a flip effect that applies (inBattle: flipped face up by an attack). */
u32 HasFlipEffect(u16 cardNo, int inBattle);
/* Number of occupied monster zones. */
int CountMonsters(int player);
/* Number of active copies of the card in zones 0-10: is the card in effect for this player. */
int CountActiveCardsOnField2(int player, u16 cardNo);
/* Number of links on (player, zone) that come from card number cardNo. */
int CountZoneLinksFromCard(int player, int zone, u16 cardNo);
/* Card ID, effective type, attribute, ATK and DEF of the card in (player, zone). */
void GetZoneCardStats(int player, int zone, struct ZoneCardStats *out);
/* Effective ATK / DEF / card type of the card in (player, slot). */
u32 GetZoneCardAtk(u32 player, u32 slot);
u32 GetZoneCardDef(u32 player, u32 slot);
u32 GetZoneCardType(s32 player, s32 slot);
/* ---- END duel.h stand-in ---- */

#include "ai.h"                     /* struct AttackPlan, gAiWork, AI_FLAG_EXODIA, the Ai* functions defined here */
#include "battle.h"                 /* CanAttackDirectly, CanMonsterAttack */
#include "card_list_view.h"         /* gCardListViewCards */
#include "duel_flow.h"              /* gDuelCtrl */
#include "util.h"                   /* MemCopy16, Random */

/* Matching: effect.h declares CollectEffectTargets with a u16 return, which adds a zero-extension at this
 * call; the ROM keeps the count as an int. Same alias as effect_checks / effect_resolve3. */
extern int CollectEffectTargetsInt(int player, u16 cardNumber, int param) asm("CollectEffectTargets");

/* Matching: the ROM tests CanMonsterAttack's result as an int; battle.h declares a u16 return, which adds a
 * zero-extension in AiChooseAttack. */
extern int CanMonsterAttackInt(int player, int zone, int checkCost) asm("CanMonsterAttack");

/* The CPU's tables of card numbers (not in a shared header yet). */
extern const u16 gAiPowerCards[];           /* 0x0819D2FC: the 13 Magic/Trap cards the CPU values most */
extern const u16 gAiHandPickPriority[];     /* 0x0819D316: the 26 cards it takes first from the human's hand */
/* DMA3 control word of the backup copy: enable (bit 31), 16-bit units, 0xD86 of them (0x1B0C bytes). */
#define AI_DMA_ENABLE               0x80000000
#define AI_BACKUP_DMA_CNT           (AI_DMA_ENABLE | 0xD86)
#define AI_SCORE_UNSET              99999   /* start value of the running minimum: above any real score */
#define AI_POWER_CARD_COUNT         13
#define AI_HAND_PICK_PRIORITY_COUNT 26

/* ---- Local views (matching choices) ----
 * The ROM reads a card word, its table entries and zones in these exact forms; the unit keeps them. */

/* A card word read whole, then masked or shifted by hand. */
#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_ID_OF(c) CARD_ID(CARD_WORD(c))
/* "The slot holds no card", tested as the ROM does: the word shifted left by 20 keeps only the ID bits
 * (`card.id == 0` compiles to different code). */
#define CARD_EMPTY(c) ((CARD_WORD(c) << 20) == 0)
/* The card tables through integer-constant addresses: the ROM reloads the table address at every use. These
 * are gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4) indexed by card ID. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))
/* The same card number through the table symbol, with the index computed first (the ROM's add order). */
#define CARD_NUMBER_SYM(id) (*(const u16 *)(((id) & CARD_ID_MASK) * 2 + (u32)gCardIdToNumber))
/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); callers pass player & 1.
 * (gDuelZones[p].zones[z] gives the same address with the terms in another order.) */
#define ZONE_AT(p, z) \
    ((struct DuelZone *)((z) * sizeof(struct DuelZone) + (p) * sizeof(struct DuelPlayer) + (u32)gDuelZones))

/* AI value of a card by its stats word: 0 for Magic/Trap/Ticket cards (no ATK), 4000 for a Divine card, else
 * the printed ATK (stored / 10, hence * 10). */
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
        r = 4000;
        break;
    default:
        r = CARD_STATS_ATK(CARD_STATS(id)) * CARD_STATS_POINTS_SCALE;
        break;
    }
    return r;
}

/* The two halves of that value for AiPickCardListEntry: ATK (bits 9-17, mask-then-shift form) and DEF
 * (bits 0-8), each * 10; 0 for Magic/Trap/Ticket cards and 4000 for Divine cards in both. The u16 return
 * of DefValue gives the ROM's add operand order. */
static inline int AtkValue(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return ((CARD_STATS(id) & CARD_STATS_ATK_MASK) >> CARD_STATS_ATK_SHIFT) * CARD_STATS_POINTS_SCALE;
    }
}
static inline u16 DefValue(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_DEF(CARD_STATS(id)) * CARD_STATS_POINTS_SCALE;
    }
}

/* AI: choose which entry of the card list in gCardListView to use for the effect of card `cardId`.
 * CollectEffectTargets fills the list with the CPU's candidates; the choice is stored in gAiWork.listPick
 * (the effect resolves copy it into gCardListView.top) and returned, -1 if there are no candidates.
 * With AI_FLAG_EXODIA (Rare Hunter) it digs for Exodia: Sangan, Witch of the Black Forest and Backup Soldier
 * take the first Exodia piece (card numbers 16-20), Mystic Tomato takes Sangan or the Witch. Magician of
 * Faith, Mask of Darkness and Graverobber look for one of the gAiPowerCards. Anything else takes the entry
 * with the best ATK + DEF that is not obviously outclassed by the human's strongest monster, else the best
 * ATK, else a random entry. */
int AiPickCardListEntry(u16 cardId)
{
    int count;
    int entry;
    int i;
    int matched;
    int best, bestIdx, maxAtk, maxDef;
    struct ZoneCardStats stats;

    count = CollectEffectTargetsInt(1, CARD_NUMBER_SYM(cardId), 0);
    gAiWork.listPick = 0;
    if (count == 0)
        goto no_pick;
    if (gDuelCtrl.aiFlags & AI_FLAG_EXODIA) {
        /* Exodia strategy: Sangan, Witch of the Black Forest and Backup Soldier fetch an Exodia piece;
         * Mystic Tomato fetches Sangan or the Witch. */
        for (i = 0; i < count; i++) {
            u16 listCardId;
            matched = 0;
            listCardId = CARD_ID_OF(gCardListViewCards[i]);
            switch (CARD_NUMBER_SYM(listCardId)) {
            case CARD_RIGHT_LEG_OF_THE_FORBIDDEN_ONE:
            case CARD_LEFT_LEG_OF_THE_FORBIDDEN_ONE:
            case CARD_RIGHT_ARM_OF_THE_FORBIDDEN_ONE:
            case CARD_LEFT_ARM_OF_THE_FORBIDDEN_ONE:
            case CARD_EXODIA_THE_FORBIDDEN_ONE:
                switch (CARD_NUMBER_SYM(cardId)) {
                case CARD_SANGAN:
                case CARD_WITCH_OF_THE_BLACK_FOREST:
                case CARD_BACKUP_SOLDIER:
                    matched = 1;
                    break;
                }
                break;
            case CARD_SANGAN:
            case CARD_WITCH_OF_THE_BLACK_FOREST: {
                int number = CARD_NUMBER_SYM(cardId);
                if (number == CARD_MYSTIC_TOMATO)
                    matched = 1;
            }
                break;
            }
            if (matched) {
                gAiWork.listPick = i;
                return gAiWork.listPick;
            }
        }
        /* Mystic Tomato with no Sangan or Witch in the list: do not use it. */
        if (CARD_NUMBER(cardId) == CARD_MYSTIC_TOMATO)
            goto no_pick;
    }
    switch (CARD_NUMBER(cardId)) {
    case CARD_MAGICIAN_OF_FAITH:
    case CARD_MASK_OF_DARKNESS:
    case CARD_GRAVEROBBER:
        /* Is any of the 13 power cards in the list? */
        for (i = 0; (u32)i <= AI_POWER_CARD_COUNT - 1; i++) {
            for (entry = 0; entry < count; entry++) {
                if (CARD_NUMBER_SYM(CARD_ID_OF(gCardListViewCards[entry])) == gAiPowerCards[i])
                    goto found_power_card;
            }
        }
        /* FAKEMATCH: a copy of the random pick below (cross-jumped with it); its reloads keep the
         * spill-register round-robin in step with the ROM. */
        gAiWork.listPick = Random() % count;
        return gAiWork.listPick;
    }
    if (count <= 0)
        goto no_pick;
    best = 0;
    /* FAKEMATCH: initialised twice; cse then keeps the second one as a copy of best (mov sl, r7). */
    maxAtk = maxDef = 0;
    bestIdx = -1;
    maxAtk = maxDef = 0;
    /* maxAtk = the strongest ATK among the human's monster zones. */
    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        GetZoneCardStats(0, i, &stats);
        /* FAKEMATCH: empty asm statements lengthen i's live range so that global-alloc
         * gives i (shared by all five loops) a priority below the Magician-of-Faith loop's mask and
         * table, which puts i in r6 as in the ROM. They emit no instructions. */
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        if (maxAtk < stats.atk)
            maxAtk = stats.atk;
    }
    /* Best ATK + DEF among the entries that can keep up with the human's strongest monster (ATK >= maxAtk,
     * or maxDef < ATK, or DEF >= maxAtk). ROM quirk: maxDef is never updated, so "maxDef < ATK" holds for
     * any card with ATK > 0 and the test almost always passes. */
    for (i = 0; i < count; i++) {
        u16 listCardId = CARD_ID_OF(gCardListViewCards[i]);

        if (best < AtkValue(listCardId) + DefValue(listCardId)) {
            if (maxAtk <= AtkValue(listCardId) || maxDef < AtkValue(listCardId) || maxAtk <= DefValue(listCardId)) {
                best = AtkValue(listCardId) + DefValue(listCardId);
                bestIdx = i;
            }
        }
    }
    if (bestIdx <= -1) {
        /* Nothing qualified: take the entry with the highest ATK value. */
        bestIdx = -1;
        best = 0;
        for (i = 0; i < count; i++) {
            u16 listCardId = CARD_ID_OF(gCardListViewCards[i]);

            if (best < CardValue(listCardId)) {
                best = CardValue(listCardId);
                bestIdx = i;
            }
        }
        if (bestIdx < 0)
            goto pick_random;
    }
    gAiWork.listPick = bestIdx;
    return gAiWork.listPick;
pick_random:
    gAiWork.listPick = Random() % count;
    return gAiWork.listPick;
found_power_card:
    /* ROM quirk: the power card's entry index is dropped and the unchanged listPick (0) is returned.
     * FAKEMATCH: the temporary `entry` reproduces the ROM's load through a register. */
    entry = gAiWork.listPick;
    return entry;
no_pick:
    return -1;
}

/* Best (largest) score among the occupied monster zones of `player` other than `skipZone`; -1 if none.
 * score = ATK (useAtk) + DEF (useDef) from GetZoneCardStats. A face-down monster is evaluated with isFaceUp
 * set for the call, so the CPU reads its real stats (it looks through face-down cards). */
int AiGetStrongestMonsterScore(int player, int skipZone, u16 useAtk, u16 useDef)
{
    int best = -1;
    int i;
    struct ZoneCardStats stats;

    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        struct DuelZone *zone;
        int score;

        if (i == skipZone)
            continue;
        zone = ZONE_AT(player & 1, i);
        if (CARD_EMPTY(zone->card))
            continue;
        score = 0;
        if (zone->isFaceUp) {
            GetZoneCardStats(player, i, &stats);
        } else {
            zone->isFaceUp = 1;
            GetZoneCardStats(player, i, &stats);
            zone->isFaceUp = 0;
        }
        if (useAtk)
            score += stats.atk;
        if (useDef)
            score += stats.def;
        if (score > best)
            best = score;
    }
    return best;
}

/* Monster zone (0-4, != skipZone) of `player` with the largest score (as AiGetStrongestMonsterScore);
 * -1 if none. */
int AiFindStrongestMonster(int player, int skipZone, u16 useAtk, u16 useDef)
{
    int bestIdx = -1;
    int best;
    int i;
    /* FAKEMATCH: initialized parity constraint (side = player & 1 in r8) prevents a different product hoist. */
    register int side asm("r8");
    struct ZoneCardStats stats;
    best = bestIdx;
    i = 0;
    side = 1;
    side &= player;
    for (; i < MONSTER_ZONE_COUNT; i++) {
        struct DuelZone *zone;
        int score;

        if (i == skipZone)
            continue;
        zone = ZONE_AT(side, i);
        if (CARD_EMPTY(zone->card))
            continue;
        score = 0;
        if (zone->isFaceUp) {
            GetZoneCardStats(player, i, &stats);
        } else {
            zone->isFaceUp = 1;
            GetZoneCardStats(player, i, &stats);
            zone->isFaceUp = 0;
        }
        if (useAtk)
            score += stats.atk;
        if (useDef)
            score += stats.def;
        if (score > best) {
            bestIdx = i;
            best = score;
        }
    }
    return bestIdx;
}

/* Monster zone (0-4, != skipZone) of `player` with the smallest score (below AI_SCORE_UNSET); -1 if none. */
int AiFindWeakestMonster(int player, int skipZone, u16 useAtk, u16 useDef)
{
    /* FAKEMATCH: hard registers for the running minimum, its zone and the side keep the ROM's allocation. */
    register int bestIdx asm("r10") = -1;
    register int best asm("r9") = AI_SCORE_UNSET;
    int i = 0;
    register int side asm("r8");
    struct ZoneCardStats stats;
    side = 1;
    side &= player;
    for (; i < MONSTER_ZONE_COUNT; i++) {
        struct DuelZone *zone;
        int score;

        if (i == skipZone)
            continue;
        zone = ZONE_AT(side, i);
        if (CARD_EMPTY(zone->card))
            continue;
        score = 0;
        if (zone->isFaceUp) {
            GetZoneCardStats(player, i, &stats);
        } else {
            zone->isFaceUp = 1;
            GetZoneCardStats(player, i, &stats);
            zone->isFaceUp = 0;
        }
        if (useAtk)
            score += stats.atk;
        if (useDef)
            score += stats.def;
        if (score < best) {
            bestIdx = i;
            best = score;
        }
    }
    return bestIdx;
}

/* Sum of the effective ATK over the five monster zones of `player` (empty zones count 0). */
int SumMonsterAtk(int player)
{
    int sum = 0;
    int i;

    for (i = 0; i < MONSTER_ZONE_COUNT; i++)
        sum += GetZoneCardAtk(player, i);
    return sum;
}

/* Is the monster in (player, zone) allowed to change its battle position? 1 if the zone is occupied, not
 * position-locked, not flagged by unk7_3, not held by a Spellbinding Circle link, and not a Dragon while a
 * Dragon Capture Jar is active on either side; else 0. Used by the CPU's position-change phase. */
int AiCanChangePosition(int player, int zone)
{
    int side = player & 1;
    struct DuelZone *z = ZONE_AT(side, zone);

    if (CARD_EMPTY(z->card))
        return 0;
    if (z->positionLocked)
        return 0;
    if (z->unk7_3)
        return 0;
    if (CountZoneLinksFromCard(player, zone, CARD_SPELLBINDING_CIRCLE) != 0)
        return 0;
    if (CountActiveCardsOnField2(0, CARD_DRAGON_CAPTURE_JAR) > 0 || CountActiveCardsOnField2(1, CARD_DRAGON_CAPTURE_JAR) > 0) {
        if (GetZoneCardType(player, zone) == CARD_TYPE_DRAGON)
            return 0;
    }
    return 1;
}

/* CPU answer to the "Select display position of card." prompt for card `cardId`: 1 = Defense Position (face
 * down unless the caller forces it face up), 0 = face-up Attack Position. `faceUp` != 0 means the monster
 * will be face up anyway, which skips the flip-effect rule. Rules in order: a flip-effect monster gives 1;
 * Labyrinth Wall gives 1; Relinquished gives 1 only if the human has no monsters; an ATK value at or below
 * the human's strongest ATK gives 1; an ATK value above 1000 gives 0; otherwise 1 only if the human still
 * has more than 4 deck cards and more than 999 life points. */
int AiShouldSetMonster(u16 cardId, u16 faceUp)
{
    int number, value;

    if (faceUp == 0) {
        const u16 *numberPtr = &CARD_NUMBER(cardId);

        if (HasFlipEffect(*numberPtr, 1) != 0)
            return 1;
        if (HasFlipEffect(*numberPtr, 0) != 0)
            return 1;
    }
    number = CARD_NUMBER(cardId);
    if (number == CARD_LABYRINTH_WALL)
        return 1;
    if (number == CARD_RELINQUISHED) {
        if (CountMonsters(0) > 0)
            return 0;
        return 1;
    }
    value = CardValue(cardId);
    if (value <= AiGetStrongestMonsterScore(0, -1, 1, 0))
        return 1;
    value = CardValue(cardId);
    if ((u32)value > 1000)
        return 0;
    if (gDuelPlayers[0].deckCount <= 4)
        return 0;
    if (gDuelPlayers[0].lifePoints > 999)
        return 1;
    return 0;
}

/* Number of cards in player 1's deck whose card number is 16-20 (the five Exodia pieces). */
int AiCountExodiaInDeck(void)
{
    s32 i = 0;
    s32 pieces = 0;
    /* FAKEMATCH: the register constraints below (count, offset, base, table) retain the ROM's address-add
     * order and its per-iteration number-table load. The empty asm after `offset` places the list-offset
     * literal before the pointer addition; it emits no instructions. */
    register s32 count __asm__("r0") = gDuelPlayers[1].deckCount;
    if (i < count) {
        s32 bound = count;
        u32 mask;
        register u32 offset __asm__("r6");
        struct DuelCard *card;
        register u32 base __asm__("r1") = (u32)gDuelPlayers;
        mask = CARD_ID_MASK;
        offset = sizeof(struct DuelPlayer) + OFFSET_OF(struct DuelPlayer, deck);    /* &gDuelPlayers[1].deck */
        __asm__ volatile("" : : "r"(offset));
        base += offset;
        card = (struct DuelCard *)base;
        do {
            u32 id;
            register const u8 *table __asm__("r6");
            s32 number;
            id = CARD_ID_OF(*card);
            id &= mask;
            id <<= 1;
            table = (const u8 *)0x08622AB4;     /* gCardIdToNumber */
            number = *(const u16 *)(id + (u32)table);
            switch (number) {
            case CARD_RIGHT_LEG_OF_THE_FORBIDDEN_ONE:
            case CARD_LEFT_LEG_OF_THE_FORBIDDEN_ONE:
            case CARD_RIGHT_ARM_OF_THE_FORBIDDEN_ONE:
            case CARD_LEFT_ARM_OF_THE_FORBIDDEN_ONE:
            case CARD_EXODIA_THE_FORBIDDEN_ONE:
                pieces++;
                break;
            }
            card++;
            i++;
        } while (i < bound);
    }
    return pieces;
}

/* Number of Exodia pieces (card numbers 16-20) in player 1's graveyard. */
int AiCountExodiaInGraveyard(void)
{
    s32 i = 0;
    s32 pieces = 0;
    s32 count = gDuelPlayers[1].graveCount;
    if (i < count) {
        s32 bound = count;
        u32 mask;
        /* FAKEMATCH: the same register constraints as AiCountExodiaInDeck (offset, base, table). */
        register u32 offset __asm__("r6");
        struct DuelCard *card;
        register u32 base __asm__("r1") = (u32)gDuelPlayers;
        mask = CARD_ID_MASK;
        offset = sizeof(struct DuelPlayer) + OFFSET_OF(struct DuelPlayer, graveyard);    /* &gDuelPlayers[1].graveyard */
        __asm__ volatile("" : : "r"(offset));
        base += offset;
        card = (struct DuelCard *)base;
        do {
            u32 id;
            register const u8 *table __asm__("r6");
            s32 number;
            id = CARD_ID_OF(*card);
            id &= mask;
            id <<= 1;
            table = (const u8 *)0x08622AB4;     /* gCardIdToNumber */
            number = *(const u16 *)(id + (u32)table);
            switch (number) {
            case CARD_RIGHT_LEG_OF_THE_FORBIDDEN_ONE:
            case CARD_LEFT_LEG_OF_THE_FORBIDDEN_ONE:
            case CARD_RIGHT_ARM_OF_THE_FORBIDDEN_ONE:
            case CARD_LEFT_ARM_OF_THE_FORBIDDEN_ONE:
            case CARD_EXODIA_THE_FORBIDDEN_ONE:
                pieces++;
                break;
            }
            card++;
            i++;
        } while (i < bound);
    }
    return pieces;
}

/* Number of player 1's monster zones (0-4) holding an Exodia piece (card numbers 16-20). */
int AiCountExodiaOnField(void)
{
    int i = 0;
    int n = 0;

    for (; i < MONSTER_ZONE_COUNT; i++) {
        int number;
        u16 id = CARD_ID_OF(gDuelPlayers[1].zones[i].card);

        if (id == 0)
            continue;
        number = CARD_NUMBER(id);
        switch (number) {
        case CARD_RIGHT_LEG_OF_THE_FORBIDDEN_ONE:
        case CARD_LEFT_LEG_OF_THE_FORBIDDEN_ONE:
        case CARD_RIGHT_ARM_OF_THE_FORBIDDEN_ONE:
        case CARD_LEFT_ARM_OF_THE_FORBIDDEN_ONE:
        case CARD_EXODIA_THE_FORBIDDEN_ONE:
            n++;
            break;
        }
    }
    return n;
}

/* Index into the human's hand of the card the CPU takes: the first hand card that matches an entry of the
 * 26-card priority list gAiHandPickPriority (the list is scanned in order: Change of Heart, Mirror Force,
 * Raigeki, ...), else a random hand index. */
int AiPickOpponentHandCard(void)
{
    u32 i;
    int handIndex;

    for (i = 0; i <= AI_HAND_PICK_PRIORITY_COUNT - 1; i++) {
        for (handIndex = 0; handIndex < gDuelPlayers[0].handCount; handIndex++) {
            if (CARD_NUMBER(CARD_ID_OF(gDuelPlayers[0].hand[handIndex])) == gAiHandPickPriority[i])
                return handIndex;
        }
    }
    return Random() % gDuelPlayers[0].handCount;
}

/* Predict the battle of player 1's monster in `attackerZone` against player 0's monster in `targetZone`
 * and store the outcome in *plan (the human's life points take `damage`; a negative value hurts the CPU).
 *  - Defense Position target: compared by DEF. A face-down defender's DEF is scaled by (opponent + 5) / 16
 *    for duelists 1-10, so the early opponents misjudge Set monsters; later ones see the real DEF.
 *    Beating it sets wins and targetDestroyed (damage 0); losing to it sets damage = ATK - DEF.
 *  - Attack Position target: compared by ATK. Beating it sets wins, targetDestroyed and damage = difference;
 *    losing sets attackerDestroyed and a negative damage; a tie destroys both and leaves damage 0. */
void AiEvalAttack(int attackerZone, int targetZone, struct AttackPlan *plan)
{
    int attackerAtk = GetZoneCardAtk(1, attackerZone);
    int targetValue = 0;
    struct DuelZone *target;

    plan->isDirect = 0;
    plan->wins = 0;
    plan->attackerZone = attackerZone;
    plan->targetZone = targetZone;
    plan->attackerDestroyed = 0;
    plan->targetDestroyed = 0;
    plan->unk2 = 0;
    plan->damage = 0;
    target = ZONE_AT(0, targetZone);
    if (target->isDefense) {
        targetValue = GetZoneCardDef(0, targetZone);
        if (!target->isFaceUp) {
            if (gMain.opponent <= DUELIST_MAI) {
                targetValue *= gMain.opponent + 5;
                targetValue /= 16;
            }
        }
        if (targetValue < attackerAtk) {
            plan->wins = 1;
            plan->targetDestroyed = 1;
        } else if (targetValue > attackerAtk) {
            plan->damage = attackerAtk - targetValue;
        }
    } else {
        targetValue = GetZoneCardAtk(0, targetZone);
        if (targetValue < attackerAtk) {
            plan->wins = 1;
            plan->targetDestroyed = 1;
            plan->damage = attackerAtk - targetValue;
        } else if (targetValue > attackerAtk) {
            plan->attackerDestroyed = 1;
            plan->damage = attackerAtk - targetValue;
        } else {
            plan->attackerDestroyed = 1;
            plan->targetDestroyed = 1;
        }
    }
}

/* Choose the best attack for player 1's monster in `attackerZone`; the plan goes to *plan.
 * A direct attack (CanAttackDirectly, or the human has no monsters; not for card number 1523) is planned at
 * once with damage = the monster's ATK, and 1 is returned. Otherwise AiEvalAttack runs against every
 * occupied human monster zone and the best winning plan is kept: the first one found, replaced by one with
 * more damage or, when the damage is not larger (not only on a tie), by one whose target has more DEF.
 * Returns plan->wins: 1 if a winning attack exists, else 0. */
u16 AiFindAttackTarget(int attackerZone, struct AttackPlan *plan)
{
    struct AttackPlan candidate;
    int i;

    if (CanAttackDirectly(1, attackerZone) != 0 || CountMonsters(0) == 0) {
        if (CARD_NUMBER(CARD_ID_OF(ZONE_AT(1, attackerZone)->card)) != CARD_1523) {
            plan->isValid = 1;
            plan->isDirect = 1;
            plan->damage = GetZoneCardAtk(1, attackerZone);
            plan->attackerZone = attackerZone;
            plan->attackerDestroyed = 0;
            plan->targetDestroyed = 0;
            return 1;
        }
    }
    plan->wins = 0;
    plan->isDirect = 0;
    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        if (CARD_EMPTY(ZONE_AT(0, i)->card))
            continue;
        AiEvalAttack(attackerZone, i, &candidate);
        if (!candidate.wins)
            continue;
        /* Matching: the DEF compare below is signed (the ROM's int GetZoneCardDef); the shared prototype
         * returns u32, hence the casts. */
        if (!plan->wins) {
            MemCopy16(plan, &candidate, sizeof(struct AttackPlan));
        } else if (candidate.damage > plan->damage) {
            MemCopy16(plan, &candidate, sizeof(struct AttackPlan));
        } else if ((int)GetZoneCardDef(0, candidate.targetZone) > (int)GetZoneCardDef(0, plan->targetZone)) {
            MemCopy16(plan, &candidate, sizeof(struct AttackPlan));
        }
    }
    return plan->wins;
}

/* Does an attacker with `atk` beat player 0's monster in `zone`? The monster counts by DEF in Defense
 * Position (scaled when face down, for duelists 1-10, as in AiEvalAttack) or by ATK in Attack Position.
 * An Attack Position monster with equal ATK counts as beaten only when the CPU has more monsters than the
 * human. Returns 1 or 0. */
u16 AiCanBeatMonster(int atk, int zone)
{
    struct DuelZone *target = ZONE_AT(0, zone);
    int value;

    if (target->isDefense) {
        value = GetZoneCardDef(0, zone);
        if (!(target->isFaceUp)) {
            if (gMain.opponent <= DUELIST_MAI) {
                value *= gMain.opponent + 5;
                value /= 16;
            }
        }
    } else {
        value = GetZoneCardAtk(0, zone);
    }
    if (value < atk)
        return 1;
    if (value == atk) {
        if (!(ZONE_AT(0, zone)->isDefense)) {
            if (CountMonsters(1) > CountMonsters(0))
                return 1;
        }
    }
    return 0;
}

/* 1 if the human has no monsters, or an attacker with `atk` beats the monster in some occupied zone of
 * player 0. Used by the CPU's position-change phase. */
int AiCanBeatAnyMonster(int atk)
{
    int i;

    if (CountMonsters(0) == 0)
        return 1;
    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        if (!CARD_EMPTY(ZONE_AT(0, i)->card) && AiCanBeatMonster(atk, i))
            return 1;
    }
    return 0;
}

/* Pick the attack the CPU makes. Collects player 1's monsters that may attack (CanMonsterAttack), sorts
 * them by ATK ascending with a bubble sort (a pair is not swapped when the later monster is Dark Elf,
 * Panther Warrior or Insect Queen, which have attack costs), then tries them in that order: the first one
 * for which AiFindAttackTarget finds a winning plan has its plan stored in gAiWork.bestAttack (isValid
 * set). Returns 1 if there is such an attack, else 0. */
int AiChooseAttack(void)
{
    u16 attackers[5];
    struct AttackPlan plan;
    int count, sorted, last, i, next;

    gAiWork.bestAttack.isValid = 0;
    gAiWork.bestAttack.isDirect = 0;
    gAiWork.bestAttack.damage = 0;
    gAiWork.bestAttack.attackerDestroyed = 0;
    gAiWork.bestAttack.targetDestroyed = 0;
    count = 0;
    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        if (CanMonsterAttackInt(1, i, 1)) {
            attackers[count] = i;
            count++;
        }
    }
    if (count == 0)
        return 0;
    last = count - 1;
    do {
        sorted = 1;
        for (i = 0; i < last; i++) {
            u16 zoneA = attackers[i];
            u16 zoneB;
            u16 idA, idB;
            int atkA, atkB, swappable;

            next = i + 1;
            zoneB = attackers[next];
            /* FAKEMATCH: idA is never used; its dead load is deleted, but CSE keeps its 0x94 and zone-base
             * registers, which orders the ROM's mov/ldr/mul. */
            idA = CARD_ID_OF(ZONE_AT(1, zoneA)->card);
            idB = CARD_ID_OF(ZONE_AT(1, zoneB)->card);
            atkA = GetZoneCardAtk(1, zoneA);
            atkB = GetZoneCardAtk(1, zoneB);
            swappable = 1;
            switch (CARD_NUMBER(idB)) {
            case CARD_DARK_ELF:
            case CARD_PANTHER_WARRIOR:
            case CARD_INSECT_QUEEN:
                swappable = 0;
                break;
            }
            if (atkA > atkB && swappable) {
                attackers[i] = zoneB;
                attackers[i + 1] = zoneA;
                sorted = 0;
            }
        }
    } while (!sorted);
    for (i = 0; i < count; i++) {
        u16 zone = attackers[i];

        GetZoneCardAtk(1, zone);    /* result unused, but the ROM makes the call */
        if (AiFindAttackTarget(zone, &plan)) {
            MemCopy16(&gAiWork.bestAttack, &plan, sizeof(struct AttackPlan));
            gAiWork.bestAttack.isValid = 1;
            return 1;
        }
    }
    return 0;
}

/* Save the duel state to gAiWork.duelBackup: a 16-bit DMA3 copy of 0xD86 halfwords (0x1B0C bytes) from
 * 0x020192E4 (both DuelPlayers and gDuel +0x1ACC..+0x1B0F) to 0x02015F14, then busy-wait for it to finish.
 * The simulation helpers change the real duel state and put it back with AiRestoreDuelState. */
void AiBackupDuelState(void)
{
    vu32 *dma = (vu32 *)(REG_BASE + 0xD4);      /* DMA3SAD, DMA3DAD, DMA3CNT */

    dma[0] = 0x020192E4;                        /* source: gDuelPlayers */
    dma[1] = 0x02015F14;                        /* destination: gAiWork.duelBackup */
    dma[2] = AI_BACKUP_DMA_CNT;
    dma[2];                                     /* read back (as the ROM does) before polling */
    while (dma[2] & AI_DMA_ENABLE)
        ;
}

/* Restore the duel state from gAiWork.duelBackup: the reverse DMA of AiBackupDuelState. */
void AiRestoreDuelState(void)
{
    vu32 *dma = (vu32 *)(REG_BASE + 0xD4);      /* DMA3SAD, DMA3DAD, DMA3CNT */

    dma[0] = 0x02015F14;                        /* source: gAiWork.duelBackup */
    dma[1] = 0x020192E4;                        /* destination: gDuelPlayers */
    dma[2] = AI_BACKUP_DMA_CNT;
    dma[2];                                     /* read back (as the ROM does) before polling */
    while (dma[2] & AI_DMA_ENABLE)
        ;
}

/* Simulation prep: every occupied monster zone of player 1 whose card has no flip effect is made a face-up
 * Attack Position monster (isDefense = 0, isFaceUp = 1), unlocked, and its bit in attackedMask is cleared so
 * it may attack. Only run inside the AiBackupDuelState / AiRestoreDuelState sandbox. */
void AiSimSetAttackPositions(void)
{
    int i;

    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        struct DuelZone *zone = &gDuelPlayers[1].zones[i];
        u16 id = CARD_ID_OF(zone->card);

        if (id != 0) {
            if (HasFlipEffect(CARD_NUMBER(id), 0) == 0) {
                zone->isDefense = 0;
                zone->isFaceUp = 1;
                zone->positionLocked = 0;
                gDuelPlayers[1].attackedMask &= ~(1 << i);
            }
        }
    }
}
