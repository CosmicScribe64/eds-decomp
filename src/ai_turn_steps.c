/*
 * ai_turn_steps (0x0805A30C-0x0805B3F3): the CPU's step handlers that are not part of the main phase proper
 * (wiki/functions/ai-turn-steps-c.md). The handlers are the entries of gAiSteps (0x0819DD6C) that AiRunStep
 * (ai_steps.c) runs, one per frame, while gAiState.step walks the table:
 *
 *  - AiStepStartMainPhase (step 0) clears the CPU's work area, announces Main Phase 1 and asks AiChooseStrategy
 *    whether a scripted combo should be played instead of the normal steps.
 *  - AiStepPlaySimpleSpells (step 1) plays the Magic cards of gAiSimpleSpells (healing, burn and removal) whose
 *    condition holds: first as set cards on the field, then from the hand.
 *  - AiStepSetSpellTraps (step 6) sets Traps and some Quick-Play Magic from the hand, partly by reading the human's
 *    hand and field.
 *  - AiPickMonsterToSet is the hand pick of AiStepMainPhase's last phase: the monster the CPU Sets face down.
 *  - AiStepFlipSummon applies the same flip-effect rules to monsters that are already Set. Nothing calls it
 *    (gAiSteps[7] is NULL), so it is dead code of the ROM.
 *
 * The CPU is always player 1; the human is player 0.
 */
#include "global.h"
#include "card_data.h"              /* gCardIdToNumber, gCardNumberToId, CARD_ID_MASK, CARD_STATS_* extractors */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_TYPE_*, SPELL_QUICK_PLAY, CARD_STATS_* layout */
#include "constants/duel.h"         /* ZONE_*, CHAIN_KIND_* */
#include "constants/duel_cmds.h"    /* DUEL_CMD_PLAYER, DUEL_CMD_MAIN1_PHASE, DUEL_CMD_FLIP_CARD, ... */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, summon.h and duel_cmd.h do not pull in the legacy header.
 * After H0, replace the block (BEGIN to END) with #include "legacy/duel.h"
 * (see build/readability/issues/ai_turn_steps.md). */
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
    u8 unk7[0x94 - 7];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 unk4[0x28 - 4];
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
extern struct DuelCard gDuelHandP1[80];         /* 0x0201A6CC = gDuelPlayers[1].hand */

u32 IsSpecialSummonOnly(u16 cardId);
u32 HasFlipEffect(u16 cardNo, int inBattle);
int CountGraveyardCardsOfType(int player, u16 type);
int CountHandCardsByNumber(int player, u16 cardNo);
int FindHandCardByNumber(int player, u16 cardNo);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountActiveCardsOnField2(int player, u16 cardNo);
int CountMonstersByNumber(int player, u16 cardNo);
int CountMonsters(int player);
int CountSpellTraps(int player);
int CountFaceUpSpellTrapsOfType(int player, u16 type);
int CountActivatableSetCards(int player, int cardNoWord);
int CountFreeMonsterZones(int player);
int FindFreeSpellTrapZone(int player);
int CanPlaceSpellTrapCard(int player, u16 cardId);
u32 GetZoneCardAtk(u32 player, u32 slot);
/* ---- END duel.h stand-in ---- */

#include "ai.h"                     /* struct AiState, gAiState, gAiWork, the Ai* functions */
#include "chain.h"                  /* struct ChainEntry, Chain_AddPending, CanActivateFieldCard */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl */
#include "effect.h"                 /* CanActivateEffect, PayChainEnergyCost */
#include "summon.h"                 /* QueueFlipSummon, CanSummonFromHand */
#include "util.h"                   /* MemClear16 */

/* The ROM tables of this unit, read by AiStepPlaySimpleSpells (the 20 card numbers of gAiSimpleSpells, 0x08086448:
 * Mooyan Curry .. Blue Medicine, Tremendous Fire, Ookazi, Final Flame, Sparks, Hinotama, Raimei, De-Spell, Remove
 * Trap, Mystical Space Typhoon, Harpie's Feather Duster, Giant Trunade, Heavy Storm, Gravekeeper's Servant and
 * Delinquent Duo). Matching: the ROM reads them as signed halfwords. */
extern const s16 gAiSimpleSpells[];
#define AI_SIMPLE_SPELL_LAST 0x13       /* the table has 20 entries */

/* Matching: ROM tables and zones are read through integer-constant addresses or byte views, as the ROM does. */
extern u8 gDuelZonesP1Bytes[] asm("gDuelZonesP1");      /* player 1's zones (0x0201A070) as bytes */
#define ZONE_STRIDE 0x94                                /* sizeof(struct DuelZone) */
#define ZONE1(z) ((struct DuelZone *)(gDuelZonesP1Bytes + (z) * ZONE_STRIDE))
/* Matching: the canActivate flag (+0x91 bit 2) is tested as a byte; the header's u32 bitfield container would read a
 * word. */
struct DuelZoneFlagsView {
    u8 unk0[0x91];
    u8 flags91;
};
extern u8 gUnk_0201A04A;        /* = gDuelPlayers[1].handCount (0x0201A048 + 2), read through its own symbol */

/* Card word of a zone or pile entry and its card ID (bits 0-11). */
#define CARD_ID(word) (((word) << 20) >> 20)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])   /* gCardIdToNumber */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])    /* gCardStats */
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))

/* The word of Chain_AddPending for a CPU activation: card ID | kind << 21 | player 1 << 31 (chain.h). */
#define CPU_CHAIN_KIND(kind) (0x80000000 | ((kind) << 21))

/* Player 1's hand word i. Matching: the integer sum computes i << 2 before the symbol load, so the hand address is
 * a short-lived pseudo (not a reload) and loop.c leaves it in the loop. */
#define HAND1_WORD(i) (*(u32 *)(((i) << 2) + (u32)gDuelHandP1))
/* Player 0's deck word i (0x02019AA8 = gDuelPlayers[0].deck): a constant address, reloaded inside the loop. */
#define HUMAN_DECK_WORD(i) (((u32 *)0x02019AA8)[i])
/* FAKEMATCH: the ROM calls CountActivatableSetCards without setting r1 (one-argument call through a cast). */
#define COUNT_SET_CARDS_1ARG(p) (((int (*)(int))CountActivatableSetCards)(p))

/* AiIsKeyCard tier 2: also keeps Kuriboh and Cyber-Stein (tier 1 keeps the Exodia pieces and the staples). */
#define AI_KEY_TIER_MONSTERS 2

/* Highest level of a monster that is Summoned without tributes. */
#define LEVEL_MAX_NO_TRIBUTE 4

/*
 * DEF-like value of a card for the Set pick: 0 for Trap, Magic and Ticket cards, 4000 for the Divine-Beasts, else
 * DEF * 10.
 * Matching: the u16 AND makes 0x1FF a halfword constant (ldr r2; adds r0, r2) and the u16 return gives the r0
 * result plus a copy.
 */
static inline u16 AiCardDef(u16 id)
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
        r = ((u16)CARD_STATS(id) & CARD_STATS_DEF_MASK) * CARD_STATS_POINTS_SCALE;
        break;
    }
    return r;
}

/*
 * Level of a card: 0 for Trap, Magic and Ticket cards, 10 for the Divine-Beasts, else the stars (stats bits 25-28).
 * Matching: the u8 return adds RTL that combine removes later; it keeps the second hand loop big enough that loop.c
 * does not hoist the count address.
 */
static inline u8 AiCardLevel(u16 id)
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

/*
 * The hand card the CPU Sets in the last phase of AiStepMainPhase (AI_MAIN_SET_MONSTER): its hand index, or -1.
 * Nothing is picked while no monster zone is free. The checks run in this order and the first hit wins:
 *  1. Rare Hunter (AI_FLAG_EXODIA): Penguin Soldier while an Exodia piece is on its field; Sangan, Witch of the Black
 *     Forest or Mystic Tomato while pieces are left in its deck;
 *  2. flip-effect monsters by situation: Penguin Soldier against two monsters, Man-Eater Bug / Hane-Hane / Dimensional
 *     Warrior / Wall of Illusion against one, Magician of Faith and Mask of Darkness for a Magic or Trap in the
 *     graveyard, Skelengel and Morphing Jar on small hands, Needle Worm against a nearly empty deck or key cards on
 *     top of it (the CPU reads the human's deck), Weather Report against Swords of Revealing Light, Dragon Piper
 *     against Dragon Capture Jar, Princess of Tsurugi / Greenkappa / Trap Master against set cards, and more;
 *  3. the monster with the highest DEF that is at least the human's best ATK and passes the playability filters;
 *  4. the first monster of level 4 or less that passes them.
 * Some checks test idx > -1 and others idx >= 0, as the ROM does.
 */
int AiPickMonsterToSet(void)
{
    int idx;
    int i;
    int maxAtk, bestDef, def;
    u16 id;

    if (CountFreeMonsterZones(1) == 0)
        return -1;
    if (gDuelCtrl.aiFlags & AI_FLAG_EXODIA) {
        if (AiCountExodiaOnField() != 0) {
            idx = FindHandCardByNumber(1, CARD_PENGUIN_SOLDIER);
            if (idx >= 0)
                return idx;
        }
        if (AiCountExodiaInDeck() != 0) {
            idx = FindHandCardByNumber(1, CARD_SANGAN);
            if (idx > -1)
                return idx;
            idx = FindHandCardByNumber(1, CARD_WITCH_OF_THE_BLACK_FOREST);
            if (idx > -1)
                return idx;
            if (CountMonstersByNumber(1, CARD_SANGAN) > 0 || CountMonstersByNumber(1, CARD_WITCH_OF_THE_BLACK_FOREST) > 0) {
                idx = FindHandCardByNumber(1, CARD_WITCH_OF_THE_BLACK_FOREST);
                if (idx > -1)
                    return idx;
            }
            idx = FindHandCardByNumber(1, CARD_MYSTIC_TOMATO);
            if (idx >= 0)
                return idx;
        }
    }
    if (CountMonsters(0) > 1) {
        idx = FindHandCardByNumber(1, CARD_PENGUIN_SOLDIER);
        if (idx >= 0)
            return idx;
    }
    if (CountMonsters(0) > 0) {
        idx = FindHandCardByNumber(1, CARD_MAN_EATER_BUG);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, CARD_HANE_HANE);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, CARD_PENGUIN_SOLDIER);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, CARD_DIMENSIONAL_WARRIOR);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, CARD_WALL_OF_ILLUSION);
        if (idx > -1)
            return idx;
    }
    if (CountGraveyardCardsOfType(1, CARD_TYPE_MAGIC) > 0) {
        idx = FindHandCardByNumber(1, CARD_MAGICIAN_OF_FAITH);
        if (idx >= 0)
            return idx;
    }
    if (CountGraveyardCardsOfType(1, CARD_TYPE_TRAP) > 0) {
        idx = FindHandCardByNumber(1, CARD_MASK_OF_DARKNESS);
        if (idx >= 0)
            return idx;
    }
    if (gDuelPlayers[1].handCount <= 2 || gDuelPlayers[0].handCount > gDuelPlayers[1].handCount + 2) {
        idx = FindHandCardByNumber(1, CARD_SKELENGEL);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, CARD_MORPHING_JAR);
        if (idx > -1)
            return idx;
    }
    if (gDuelPlayers[0].deckCount <= 4) {
        idx = FindHandCardByNumber(1, CARD_NEEDLE_WORM);
        if (idx >= 0)
            return idx;
    }
    /* Needle Worm also when one of the top five cards of the human's deck is a card worth destroying. */
    for (i = 0; i <= 4; i++) {
        /* Matching: the table is read through its symbol here (the other reads use the constant address). */
        switch (*((CARD_ID(HUMAN_DECK_WORD(i)) & CARD_ID_MASK) + gCardIdToNumber)) {
        case CARD_RIGHT_LEG_OF_THE_FORBIDDEN_ONE:
        case CARD_LEFT_LEG_OF_THE_FORBIDDEN_ONE:
        case CARD_RIGHT_ARM_OF_THE_FORBIDDEN_ONE:
        case CARD_LEFT_ARM_OF_THE_FORBIDDEN_ONE:
        case CARD_EXODIA_THE_FORBIDDEN_ONE:
        case CARD_DARK_HOLE:
        case CARD_RAIGEKI:
        case CARD_SINISTER_SERPENT:
        case CARD_MEGAMORPH:
        case CARD_HARPIES_FEATHER_DUSTER:
        case CARD_JINZO:
        case CARD_BELL_OF_DESTRUCTION:
        case CARD_GRACEFUL_CHARITY:
        case CARD_MONSTER_REBORN:
        case CARD_POT_OF_GREED:
        case CARD_CHANGE_OF_HEART:
        case CARD_MIRROR_FORCE:
        case CARD_SNATCH_STEAL:
            idx = FindHandCardByNumber(1, CARD_NEEDLE_WORM);
            if (idx >= 0)
                goto done;
            break;
        }
    }
    if (CountActiveCardsOnField2(0, CARD_SWORDS_OF_REVEALING_LIGHT) > 0) {
        idx = FindHandCardByNumber(1, CARD_WEATHER_REPORT);
        if (idx >= 0)
            return idx;
    }
    if (CountActiveCardsOnField2(0, CARD_DRAGON_CAPTURE_JAR) > 0) {
        idx = FindHandCardByNumber(1, CARD_DRAGON_PIPER);
        if (idx >= 0)
            return idx;
    }
    if (CountSpellTraps(0) > 1) {
        idx = FindHandCardByNumber(1, CARD_PRINCESS_OF_TSURUGI);
        if (idx >= 0)
            return idx;
    }
    if (COUNT_SET_CARDS_1ARG(0) > 1) {
        idx = FindHandCardByNumber(1, CARD_GREENKAPPA);
        if (idx >= 0)
            return idx;
    }
    if (COUNT_SET_CARDS_1ARG(0) > 0) {
        idx = FindHandCardByNumber(1, CARD_TRAP_MASTER);
        if (idx >= 0)
            return idx;
    }
    /* Only the human has monsters: the flip effects that punish or bounce them. */
    if (CountMonsters(0) > 0 && CountMonsters(1) == 0) {
        idx = FindHandCardByNumber(1, CARD_MORPHING_JAR_2);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, CARD_CYBER_JAR);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, CARD_GIANT_GERM);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, CARD_NIMBLE_MOMONGA);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, CARD_1307);
        if (idx > -1)
            return idx;
    }
    /* The human's strongest ATK, then the hand monster with the highest DEF that holds against it. */
    maxAtk = 0;
    for (i = 0; i <= 4; i++) {
        int atk = GetZoneCardAtk(0, i);
        if (maxAtk < atk)
            maxAtk = atk;
    }
    bestDef = 0;
    idx = -1;
    for (i = 0; i < gDuelPlayers[1].handCount; i++) {
        id = CARD_ID(HAND1_WORD(i));
        def = AiCardDef(id);
        if (bestDef < def && maxAtk <= def && CanSummonFromHand(1, id) != 0
            && AiIsKeyCard(AI_KEY_TIER_MONSTERS, CARD_NUMBER(id)) == 0 && IsSpecialSummonOnly(id) == 0
            && AiHasTributesFor(id) != 0) {
            idx = i;
            bestDef = def;
        }
    }
    if (idx >= 0) {
    done:
        return idx;
    }
    /* Otherwise the first monster that needs no tribute. */
    for (i = 0; i < gDuelPlayers[1].handCount; i++) {
        u16 id2 = CARD_ID(HAND1_WORD(i));
        if (AiCardLevel(id2) <= LEVEL_MAX_NO_TRIBUTE && CARD_TYPE(id2) <= CARD_TYPE_REPTILE
            && AiIsKeyCard(AI_KEY_TIER_MONSTERS, CARD_NUMBER(id2)) == 0 && IsSpecialSummonOnly(id2) == 0
            && AiHasTributesFor(id2) != 0)
            return i;
    }
    return -1;
}

/*
 * Step 0 (AI_STEP_START_MAIN_PHASE): clear the CPU's work area, announce Main Phase 1 and ask AiChooseStrategy for a
 * scripted combo. If one applies, jump to AI_STEP_STRATEGY and return 0; otherwise return 1 and AiRunStep goes on
 * to step 1.
 */
int AiStepStartMainPhase(void)
{
    MemClear16(&gAiWork, sizeof(struct AiWork));
    DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_MAIN1_PHASE, 0, 0, 0);
    if (AiChooseStrategy() != 0) {
        gAiState.step = AI_STEP_STRATEGY;
        gAiState.stepState = 0;
        gAiState.stepIndex = 0;
        gAiState.unk4 = 0;
        gAiState.flipZone = 0;
        return 0;
    }
    return 1;
}

/*
 * Step 6 (AI_STEP_SET_SPELL_TRAPS): set Traps and some Quick-Play Magic from the CPU's hand, one card per call.
 * stepState 0 resets stepIndex (the hand index being looked at); state 1 scans the hand from there. A hit queues
 * "place from hand, face down" for that card (DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND with the hand index and the free
 * spell/trap zone) and returns 0; the card leaves the hand, so stepIndex stays. The scan ends with stepState++, and
 * the next call returns 1. It returns 1 at once when no spell/trap zone is free.
 *
 * Which cards (by card number, reading the human's hand and field):
 *  - White Hole, Anti-Raigeki, Call of the Dark if the human holds Dark Hole, Raigeki or Monster Reborn;
 *  - Magic Jammer, Imperial Order if the human has a face-down Magic on its field or a Magic in its hand;
 *  - Seven Tools of the Bandit, Royal Decree if the human has a face-down Trap;
 *  - never keys 1245 and 1314 (the strategy cards);
 *  - any other Trap, and Quick-Play Magic and Black Pendant.
 *
 * FAKEMATCH: all register bindings and empty constraints below are initialized lifetime/scheduling hints. Field and
 * hand loop masks are distinct from the outer mask; bound caller-saved values are dead before external calls.
 * The type bits are 20..24 (mask 0x01F00000).
 */
int AiStepSetSpellTraps(void)
{
    struct AiState *initial = &gAiState;
    u32 stepState = initial->stepState;
    register struct AiState *state asm("r9");
    state = initial;
    switch (stepState) {
    case 0:
        {
            struct AiState *q = state;
            q->stepIndex = stepState;
            {
                register u32 next asm("r0") = q->stepState + 1;
                struct AiState *nextState = state;
                nextState->stepState = next;
            }
            break;
        }
    case 1:
        break;
    default:
        return 1;
    }
    {
        u32 count;
        struct AiState *q;
        {
            register u32 base asm("r0") = (u32)&gDuelPlayers;
            register u32 off asm("r2") = 0xD66;     /* gDuelPlayers[1].handCount */
            asm("" : : "r"(base), "r"(off));
            count = *(u8 *)(base + off);
        }
        if (count == 0) goto done;
        q = state;
        {
            register u32 index asm("r3") = q->stepIndex;
            asm("" : : "r"(index));
            if (index >= count) goto done;
        }
        {
            register u32 seed asm("r4") = CARD_ID_MASK;
            u32 lookupMask;
            asm("" : : "r"(seed));
            lookupMask = seed;
        hand_next:
            {
                register u32 cardId asm("r8");
                int zone;
                u16 ok;
                {
                    u32 index = q->stepIndex;
                    u32 off = index << 2;
                    u32 table = (u32)gDuelHandP1;
                    u32 raw;
                    raw = *(u32 *)(off + table);
                    cardId = CARD_ID(raw);
                }
                zone = FindFreeSpellTrapZone(1);
                ok = 0;
                if (zone < 0) return 1;
                switch (({
                    u32 index = cardId;
                    register u32 mask asm("r6");
                    asm("" : : "r"(index));
                    mask = lookupMask;
                    asm("" : "+r"(mask));
                    ((const u16 *)0x08622AB4)[index & mask];
                }
                )) {
                case CARD_WHITE_HOLE:
                    {
                        register int player asm("r0") = 0;
                        ok = CountHandCardsByNumber(player, CARD_DARK_HOLE) > 0;
                    }
                    break;
                case CARD_ANTI_RAIGEKI:
                    {
                        register int player asm("r0") = 0;
                        ok = CountHandCardsByNumber(player, CARD_RAIGEKI) > 0;
                    }
                    break;
                case CARD_CALL_OF_THE_DARK:
                    {
                        int player = 0;
                        asm("" : : "r"(player));
                        ok = CountHandCardsByNumber(player, CARD_MONSTER_REBORN) > 0;
                    }
                    break;
                case CARD_MAGIC_JAMMER:
                case CARD_IMPERIAL_ORDER:
                    {
                        int z = ZONE_SPELL_0;
                        register struct DuelPlayer *base asm("r5") = &gDuelPlayers[0];
                        register u32 off asm("r2") = 0x28;
                        register u8 *zoneBase asm("r12");
                        u32 mask;
                        asm("" : : "r"(z), "r"(base), "r"(off));
                        zoneBase = (u8 *)((u32)base + off);
                        mask = CARD_ID_MASK;
                    humanFieldMagic:
                        {
                            u32 off = ZONE_STRIDE * z;
                            u8 *basecopy = zoneBase;
                            struct DuelZone *zp;
                            u32 id;
                            zp = (struct DuelZone *)(off + (u32)basecopy);
                            id = CARD_ID(*(u32 *)&zp->card);
                            if (id != 0) {
                                id &= mask;
                                if ((CARD_STATS(id) & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT == CARD_TYPE_MAGIC
                                    && !zp->isFaceUp) ok = 1;
                            }
                        }
                        if (++z <= ZONE_SPELL_4) goto humanFieldMagic;
                        if (base->handCount != 0) {
                            u32 mask = CARD_ID_MASK;
                            register struct DuelPlayer *cur asm("r0") = &gDuelPlayers[0];
                            register u32 off asm("r2") = 0x684;
                            register u32 *hp asm("r1");
                            register u32 typeMask asm("r2");
                            int n;
                            asm("" : : "r"(mask), "r"(cur), "r"(off));
                            hp = (u32 *)((u32)cur + off);
                            typeMask = CARD_STATS_TYPE_MASK;
                            n = cur->handCount;
                        humanHandMagic:
                            {
                                u32 id = CARD_ID(*hp);
                                register u32 table asm("r6");
                                id &= mask;
                                id <<= 2;
                                table = 0x08621DE0;
                                asm("" : : "r"(table));
                                if ((*(u32 *)(id + table) & typeMask) >> CARD_STATS_TYPE_SHIFT == CARD_TYPE_MAGIC) ok = 1;
                            }
                            hp++;
                            if (--n != 0) goto humanHandMagic;
                        }
                        break;
                    }
                case CARD_SEVEN_TOOLS_OF_THE_BANDIT:
                case CARD_ROYAL_DECREE:
                    {
                        int z = ZONE_SPELL_0;
                        u8 *zoneBase = (u8 *)gDuelZones;
                        u32 mask = CARD_ID_MASK;
                    humanFieldTrap:
                        {
                            u32 off = ZONE_STRIDE * z;
                            u8 *basecopy = zoneBase;
                            struct DuelZone *zp;
                            u32 id;
                            zp = (struct DuelZone *)(off + (u32)basecopy);
                            id = CARD_ID(*(u32 *)&zp->card);
                            if (id != 0) {
                                u32 off;
                                register u32 table asm("r6");
                                id &= mask;
                                off = id << 2;
                                table = 0x08621DE0;
                                asm("" : : "r"(off), "r"(table));
                                if ((*(u32 *)(off + table) & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT == CARD_TYPE_TRAP
                                    && !zp->isFaceUp) ok = 1;
                            }
                        }
                        if (++z <= ZONE_SPELL_4) goto humanFieldTrap;
                        break;
                    }
                case CARD_1245:
                case CARD_1314:
                    goto skip;
                default:
                    {
                        u32 index = cardId;
                        u32 mask;
                        u32 stats, type;
                        asm("" : : "r"(index));
                        mask = lookupMask;
                        index &= mask;
                        stats = ((const u32 *)0x08621DE0)[index];
                        type = (stats & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT;
                        if (type == CARD_TYPE_TRAP) goto doAction;
                        if (type != CARD_TYPE_MAGIC) break;
                        if (CARD_STATS_SUBTYPE(stats) == SPELL_QUICK_PLAY) ok = 1;
                        {
                            u32 off = index << 1;
                            register u32 table asm("r2") = 0x08622AB4;
                            asm("" : : "r"(off), "r"(table));
                            if (*(const u16 *)(off + table) == CARD_BLACK_PENDANT) ok = 1;
                        }
                        break;
                    }
                }
                if (!ok) goto skip;
            doAction:
                {
                    register u32 packed asm("r2");
                    PayChainEnergyCost(1);
                    {
                        struct AiState *cur = &gAiState;
                        register u32 nibble asm("r2");
                        nibble = cur->stepIndex & 15;
                        nibble <<= 4;
                        zone &= 15;
                        packed = nibble | zone;     /* hand index << 4 | spell/trap zone; face down */
                    }
                    DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND, cardId, packed, 0);
                    return 0;
                }
            skip:
                q = &gAiState;
                {
                    u32 next = q->stepIndex + 1;
                    register u8 *cp asm("r3");
                    q->stepIndex = next;
                    cp = &gUnk_0201A04A;
                    count = *cp;
                    if (count != 0 && (u8)next < count) goto hand_next;
                }
            }
        }
    }
done:
    {
        register struct AiState *last asm("r4") = state;
        last->stepState++;
    }
    return 0;
}

/* Card ID of a card number as the ROM computes it (see CardNumberToId in ai_deck.c): 0xFFFF is none, numbers below
 * 2000 map directly, alternate art 2000 + n uses the ID of n plus one. */
static inline u16 AiScanCardId(u16 number)
{
    if (number == 0xFFFF) return 0;
    if (number <= CARD_NUMBER_ALT_ART - 1) return ((const u16 *)0x08623DF4)[number & CARD_ID_MASK];
    return ((const u16 *)0x08623DF4)[(number - CARD_NUMBER_ALT_ART) & CARD_ID_MASK] + 1;
}

/*
 * Step 1 (AI_STEP_PLAY_SIMPLE_SPELLS): play the cards of gAiSimpleSpells whose condition holds, one per call.
 * gAiState.phase is the phase (enum AiSimpleSpellsPhase) and gAiState.subState the index into the table:
 *  - AI_SIMPLE_SPELLS_FIELD: for each table card with an activatable set copy (CountActivatableSetCards) and a
 *    condition that holds, find it in the spell/trap zones 5-9 (CanActivateFieldCard), point at and flip it and add
 *    it to the chain; return 0. When no card is found, go on to the hand phase.
 *  - AI_SIMPLE_SPELLS_HAND: the same table for hand copies (AiFindHandCardByNumber); the first one whose condition
 *    holds and which can be activated and placed (CanActivateEffect, CanPlaceSpellTrapCard) moves to the next phase;
 *    if there is none the step is done (return 1).
 *  - AI_SIMPLE_SPELLS_PLACE: place that hand card (DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND), face down only while
 *    Anti-Magic Fragrance is on the field, otherwise face up and add it to the chain; back to the field phase.
 * Conditions: healing and burn cards always; Tremendous Fire by life points; De-Spell and Remove Trap against a
 * face-up Magic or Trap of the human; Mystical Space Typhoon, Harpie's Feather Duster and Giant Trunade when the
 * human has spell/trap cards; Heavy Storm only when the CPU has none; Delinquent Duo with more than 1000 LP against a
 * human hand. Restructer Revolution (0x40F) is unreachable (it is not in the table) and Gravekeeper's Servant has no
 * case, so it is never played.
 */
int AiStepPlaySimpleSpells(void)
{
    struct ChainEntry ref;
    switch (gAiState.phase) {
    case AI_SIMPLE_SPELLS_FIELD:
        gAiState.subState = 0;
        do {
            if (CountActivatableSetCards(1, gAiSimpleSpells[gAiState.subState])) {
                int ok = 0;
                switch (gAiSimpleSpells[gAiState.subState]) {
                case CARD_MOOYAN_CURRY: case CARD_RED_MEDICINE: case CARD_GOBLINS_SECRET_REMEDY:
                case CARD_SOUL_OF_THE_PURE: case CARD_DIAN_KETO_THE_CURE_MASTER: case CARD_SPARKS:
                case CARD_HINOTAMA: case CARD_FINAL_FLAME: case CARD_OOKAZI:
                case CARD_BLUE_MEDICINE: case CARD_RAIMEI:
                    ok = 1; break;
                case CARD_TREMENDOUS_FIRE:
                    if (gDuelPlayers[0].lifePoints <= 1000 && gDuelPlayers[1].lifePoints > 500)
                        ok = 1;
                    if (gDuelPlayers[0].lifePoints < gDuelPlayers[1].lifePoints + 500)
                        ok = 1;
                    if (gDuelPlayers[1].lifePoints > 500) { ok = 1; break; }
                    break;
                case CARD_RESTRUCTER_REVOLUTION:
                    if (200 * gDuelPlayers[0].handCount > gDuelPlayers[0].lifePoints)
                        ok = 1;
                    if (gDuelPlayers[0].handCount > 2) { ok = 1; break; }
                    break;
                case CARD_DE_SPELL:
                    if (CountFaceUpSpellTrapsOfType(0, CARD_TYPE_MAGIC) > 0) { ok = 1; break; }
                    break;
                case CARD_REMOVE_TRAP:
                    if (CountFaceUpSpellTrapsOfType(0, CARD_TYPE_TRAP) > 0) { ok = 1; break; }
                    break;
                case CARD_HARPIES_FEATHER_DUSTER: case CARD_MYSTICAL_SPACE_TYPHOON: case CARD_GIANT_TRUNADE:
                    if (CountSpellTraps(0) > 0) { ok = 1; break; }
                    break;
                case CARD_HEAVY_STORM:
                    if (CountSpellTraps(0) > 0 && CountSpellTraps(1) == 0) { ok = 1; break; }
                    break;
                case CARD_DELINQUENT_DUO:
                    if (gDuelPlayers[1].lifePoints > 1000 && gDuelPlayers[0].handCount)
                        ok = 1;
                    break;
                }
                if (ok) {
                    int z;
                    u32 id;
                    ref.event = 0;
                    for (z = ZONE_SPELL_0; z <= ZONE_SPELL_4; z++) {
                        {
                            u32 raw = *(u32 *)&ZONE1(z)->card;
                            id = CARD_ID(raw);
                        }
                        /* ROM bug kept: the canActivate flag (+0x91 bit 2) is read from the zone indexed by the table
                         * position (subState), not by z. */
                        if (id && ({
                            /* FAKEMATCH: initialized mask copies retain the lookup registers. */
                            register u32 loadMask asm("r1") = CARD_ID_MASK;
                            register u32 mask asm("r0");
                            u32 index;
                            asm("" : : "r"(loadMask));
                            mask = loadMask;
                            asm("" : "+r"(mask));
                            index = id;
                            ((const u16 *)0x08622AB4)[index & mask];
                        }) == (u16)gAiSimpleSpells[gAiState.subState]
                            && (((struct DuelZoneFlagsView *)ZONE1(gAiState.subState))->flags91 & 4)
                            && CanActivateFieldCard(&ref, 1, z)) {
                            DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_FLIP_CARD, z, 0, 0);
                            {
                                u32 zoneBits = (u32)(z & 31) << 16;
                                id |= CPU_CHAIN_KIND(CHAIN_KIND_SPELL_TRAP);
                                Chain_AddPending(zoneBits | id, 0);
                            }
                            return 0;
                        }
                    }
                }
            }
            gAiState.subState++;
        } while (gAiState.subState <= AI_SIMPLE_SPELL_LAST);
        {
            /* FAKEMATCH: keep the initialized phase pointer separate from the next loop. */
            register struct AiState *state asm("r2") = &gAiState;
            state->phase++;
        }
        /* fall through */
    case AI_SIMPLE_SPELLS_HAND: {
        /* FAKEMATCH: stage the initialized reset pointer before retaining state. */
        register struct AiState *initial asm("r1");
        struct AiState *state;
        const s16 *numbers;
        struct ChainEntry *record;
        int zero = 0;

        initial = &gAiState;

        initial->subState = zero;
        numbers = gAiSimpleSpells;

        state = initial;
        record = &ref;

        do {
            if (AiFindHandCardByNumber(1, numbers[state->subState]) >= 0) {
                int ok = 0;
                switch (numbers[state->subState]) {
                case CARD_MOOYAN_CURRY: case CARD_RED_MEDICINE: case CARD_GOBLINS_SECRET_REMEDY:
                case CARD_SOUL_OF_THE_PURE: case CARD_DIAN_KETO_THE_CURE_MASTER: case CARD_SPARKS:
                case CARD_HINOTAMA: case CARD_FINAL_FLAME: case CARD_OOKAZI:
                case CARD_BLUE_MEDICINE: case CARD_RAIMEI:
                    ok = 1; break;
                case CARD_TREMENDOUS_FIRE:
                    if (gDuelPlayers[0].lifePoints <= 1999 && gDuelPlayers[1].lifePoints > 1500)
                        { ok = 1; break; }
                    break;
                case CARD_DE_SPELL:
                    if (CountFaceUpSpellTrapsOfType(0, CARD_TYPE_MAGIC) > 0) { ok = 1; break; }
                    break;
                case CARD_REMOVE_TRAP:
                    if (CountFaceUpSpellTrapsOfType(0, CARD_TYPE_TRAP) > 0) { ok = 1; break; }
                    break;
                case CARD_HARPIES_FEATHER_DUSTER: case CARD_MYSTICAL_SPACE_TYPHOON:
                    if (CountSpellTraps(0) > 0) { ok = 1; break; }
                    break;
                case CARD_HEAVY_STORM: case CARD_GIANT_TRUNADE:
                    if (CountSpellTraps(0) > 0 && CountSpellTraps(1) == 0) { ok = 1; break; }
                    break;
                case CARD_DELINQUENT_DUO:
                    if (gDuelPlayers[1].lifePoints > 1000 && gDuelPlayers[0].handCount)
                        ok = 1;
                    break;
                }
                if (ok) {
                    record->player = 1;
                    record->event = 0;
                    record->card = AiScanCardId(numbers[state->subState]);
                    if (CanActivateEffect(&ref, 0, 1) && CanPlaceSpellTrapCard(1, record->card)) {
                        gAiState.phase++;
                        return 0;
                    }
                }
            }
            state->subState++;
        } while (state->subState <= AI_SIMPLE_SPELL_LAST);
        break;
    }
    case AI_SIMPLE_SPELLS_PLACE:
        if (CountActiveCardsOnField(0, CARD_ANTI_MAGIC_FRAGRANCE) || CountActiveCardsOnField(1, CARD_ANTI_MAGIC_FRAGRANCE)) {
            /* Anti-Magic Fragrance: Magic cannot be activated, so the card is only Set. */
            u16 id = AiScanCardId(gAiSimpleSpells[gAiState.subState]);
            int hand = AiFindHandCardByNumber(1, gAiSimpleSpells[gAiState.subState]);
            int freeZone = FindFreeSpellTrapZone(1);
            {
                int packed = ((hand & 15) << 4) | (freeZone & 15);
                /* FAKEMATCH: finish packing before loading the message constant. */
                asm("" : : "r"(packed));
                DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND, id, packed, 0);
            }
        } else {
            /* Place the card face up (bit 8 of the operand) and add it to the chain. */
            u16 id = AiScanCardId(gAiSimpleSpells[gAiState.subState]);
            int hand = AiFindHandCardByNumber(1, gAiSimpleSpells[gAiState.subState]);
            int freeZone = FindFreeSpellTrapZone(1);
            int zone;
            {
                int packed = ((hand & 15) << 4) | (freeZone & 15);
                /* FAKEMATCH: the old scratch is dead here; rematerialize the flag
                 * and retain its initialized copy before the message setup. */
                asm("" : : "r"(packed) : "r1");
                {
                    register u32 flag asm("r1") = 0x100;
                    asm("" : : "r"(flag));
                    {
                        u32 value = 0x100;
                        packed |= value;
                    }
                }
                asm("" : : "r"(packed));
                DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND, id, packed, 0);
            }
            zone = FindFreeSpellTrapZone(1);
            {
                /* FAKEMATCH: initialize the card word in the final packing register. */
                register u32 card asm("r1");
                u32 zoneBits;
                u16 number = gAiSimpleSpells[gAiState.subState];
                if (number == 0xFFFF)
                    card = 0;
                else if (number <= CARD_NUMBER_ALT_ART - 1) {
                    /* FAKEMATCH: preserve the initialized reverse-table scratch. */
                    register u32 table asm("r2");
                    u32 off = (number & CARD_ID_MASK) * 2;
                    table = 0x08623DF4;
                    asm("" : : "r"(table));
                    card = *(const u16 *)(off + table);
                } else
                    card = ((const u16 *)0x08623DF4)[(number - CARD_NUMBER_ALT_ART) & CARD_ID_MASK] + 1;
                zoneBits = (u32)(zone & 31) << 16;
                card = (u16)card;
                card |= CPU_CHAIN_KIND(CHAIN_KIND_SPELL_TRAP);
                Chain_AddPending(zoneBits | card, 0);
            }
        }
        gAiState.phase = AI_SIMPLE_SPELLS_FIELD;
        return 0;
    }
    return 1;
}

/*
 * Flip-summon the CPU's set flip-effect monsters when their effect helps (unreferenced: gAiSteps[7] is NULL).
 * A state machine on gAiState.subState over player 1's monster zones (flipZone 0-4):
 *  - subState 0 resets the counters; state 1 looks at zone flipZone: skip an empty zone or a face-up monster, and a
 *    monster without a flip effect (HasFlipEffect);
 *  - for a face-down flip-effect monster, decide by its card number, using the same rules as AiPickMonsterToSet
 *    (Needle Worm, Skelengel / Morphing Jar, Man-Eater Bug / Hane-Hane / Penguin Soldier, Cyber Jar, Invader of the
 *    Throne, Dragon Piper, Weather Report, Princess of Tsurugi, Reaper of the Cards / Trap Master, Hiro's Shadow Scout /
 *    Parasite Paracide, Greenkappa, Magician of Faith, Mask of Darkness): when the rule says yes, go to state 2;
 *  - state 2 queues the flip summon (QueueFlipSummon), moves to the next zone and returns to state 1.
 * Returns 1 after zone 4.
 * Matching: the dispatch pointer (r4) and the body pointer (r1 -> r5) are separate locals, and the success
 * increments mix pointer and global forms, as the ROM's unmerged tails show.
 */
int AiStepFlipSummon(void)
{
    struct AiState *state = &gAiState;
    int subState = state->subState;
    struct AiState *dispatch = &gAiState;

    switch (subState) {
    case 0:
        state->unk4 = 0;
        state->flipZone = 0;
        state->subState++;
        /* fall through */
    case 1:
        {
        struct AiState *q = dispatch;
        int z;
        u32 id;
        u16 number;
        if (q->flipZone > 4)
            return 1;
        z = q->flipZone;
        id = CARD_ID(*(u32 *)&ZONE1(z)->card);
        if (id == 0) {
            q->flipZone = z + 1;
            return 0;
        }
        if (ZONE1(z)->isFaceUp) {
            q->flipZone = z + 1;
            return 0;
        }
        if (HasFlipEffect(((const u16 *)0x08622AB4)[id & CARD_ID_MASK], 0) == 0) {
            q->flipZone++;
            return 0;
        }
        number = ((const u16 *)0x08622AB4)[CARD_ID(*(u32 *)&ZONE1(q->flipZone)->card) & CARD_ID_MASK];
        switch (number) {
        case CARD_NEEDLE_WORM:
            if (CountMonsters(0) != 0)
                break;
            q->subState++;
            return 0;
        case CARD_SKELENGEL:
        case CARD_MORPHING_JAR:
            if (gDuelPlayers[1].handCount <= 2 || gDuelPlayers[0].handCount > gDuelPlayers[1].handCount + 2) {
                gAiState.subState++;
                return 0;
            }
            break;
        case CARD_MAN_EATER_BUG:
        case CARD_HANE_HANE:
        case CARD_PENGUIN_SOLDIER:
            if (CountMonsters(0) > 0) {
                gAiState.subState++;
                return 0;
            }
            break;
        case CARD_CYBER_JAR:
        case CARD_MORPHING_JAR_2:
            if (CountMonsters(1) == 1 && CountMonsters(0) > 1) {
                gAiState.subState++;
                return 0;
            }
            break;
        case CARD_INVADER_OF_THE_THRONE:
            if (CountMonsters(0) > CountMonsters(1) + 1) {
                q->subState++;
                return 0;
            }
            break;
        case CARD_DRAGON_PIPER:
            if (CountActiveCardsOnField2(0, CARD_DRAGON_CAPTURE_JAR) > 0) {
                q->subState++;
                return 0;
            }
            break;
        case CARD_WEATHER_REPORT:
            if (CountActiveCardsOnField2(0, CARD_SWORDS_OF_REVEALING_LIGHT) > 0) {
                q->subState++;
                return 0;
            }
            break;
        case CARD_PRINCESS_OF_TSURUGI:
            if (CountSpellTraps(0) > 1) {
                q->subState++;
                return 0;
            }
            break;
        case CARD_REAPER_OF_THE_CARDS:
        case CARD_TRAP_MASTER:
            if (CountActivatableSetCards(0, number) > 0) {
                gAiState.subState++;
                return 0;
            }
            break;
        case CARD_HIROS_SHADOW_SCOUT:
        case CARD_PARASITE_PARACIDE:
            gAiState.subState++;
            return 0;
        case CARD_GREENKAPPA:
            if (CountActivatableSetCards(0, number) > 1) {
                q->subState++;
                return 0;
            }
            break;
        case CARD_MAGICIAN_OF_FAITH:
            if (CountGraveyardCardsOfType(1, CARD_TYPE_MAGIC) > 0) {
                q->subState++;
                return 0;
            }
            break;
        case CARD_MASK_OF_DARKNESS:
            if (CountGraveyardCardsOfType(1, CARD_TYPE_TRAP) > 0) {
                q->subState++;
                return 0;
            }
            break;
        }
        gAiState.flipZone++;
        return 0;
        }
    case 2:
        QueueFlipSummon(1, state->flipZone);
        state->flipZone++;
        state->subState = 1;
        return 0;
    }
    return 1;
}
