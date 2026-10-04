/*
 * ai_steps (0x0805B3F4-0x0805C508): the CPU's main-phase step handlers, between the AI decision
 * units and the duel UI (wiki/functions/ai-steps-c.md).
 *
 * AiRunStep runs one frame of the CPU main phase: it calls gAiSteps[gAiState.step] and, when the
 * handler returns nonzero, clears the step state (stepState, stepIndex, unk4, flipZone, phase,
 * subState) and advances to the next step. The handlers here:
 *   - AiStepMainPhase (AiStep steps 2 and 5): summon or Set the chosen hand monster, then run the
 *     monster-effect, Exodia-trap and spell sub-phases (enum AiMainState in gAiState.phase).
 *   - AiStepChangePositions (step 3): activate monster effects, then flip-summon or change the
 *     battle position of the CPU's monsters, decided on simulated stats.
 *   - AiStepBattle (step 4): hand over to BattlePhase_Run once a monster can attack.
 *   - AiChooseStrategy and AiStrategyCyberStein: the scripted combos. AiChooseStrategy probes the
 *     nine enum AiStrategy combos at the start of the main phase; AiStepRunStrategy (ai_strategy.c)
 *     then dispatches to the chosen handler. Only the Cyber-Stein combo lives here.
 * The CPU is player 1 in every call in this unit.
 */
#include "global.h"
#include "ai.h"                     /* struct AiState, struct AiWork, gAiState, gAiWork, enum AiMainState, enum AiStrategy, AI_FLAG_EXODIA, the Ai* prototypes */
#include "card_data.h"              /* CARD_ID_MASK, gCardIdToNumber, gCardNumberToId, gCardStats */
#include "constants/card_stats.h"   /* enum CardType, CARD_STATS_TYPE_MASK/SHIFT, CARD_STATS_LEVEL_MASK/SHIFT */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/duel_cmds.h"    /* DUEL_CMD_PLAYER, DUEL_CMD_POINT_AT_CARD */
#include "debug.h"                  /* DebugPrintFlush */
#include "duel_flow.h"              /* struct DuelCtrl, gDuelCtrl */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and summon.h below use, with the
 * header's names and types, and defines duel.h's include guard so that summon.h does not pull in the
 * legacy header. DuelZone is the one deliberate divergence: the canonical header splits the +0x06 byte
 * into bitfields, but this unit loads, stores and masks that byte whole, so the byte form is kept (a
 * local view in the sense of build/readability/HEADERS.md). After H0, replace the block (BEGIN to END)
 * with the include line of duel.h and keep the DuelZone byte view as a local view
 * (build/readability/issues/ai_steps.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:7;
    u32 flag20:1;
    u32 unk21:11;
};

/* One field zone (0x94 bytes); each player has 11. Only the fields this unit touches are named; the
 * link tables in between are padding here. */
struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 flags6;                      /* +0x06: bits 0-1 position/face state as read below; bit 1 is also
                                     * the simulation's temporary face-up mark */
    u8 unk7[0x91 - 0x7];
    u8 flags91;                     /* +0x91: bit 3 = isDisabled (the duel_phases.c view) */
    u8 unk92[2];
};
/* ---- END duel.h stand-in ---- */

#include "summon.h"                 /* QueueNormalSummon (its callee explicitly decodes both final word arguments as u16), QueueFlipSummon, CanNormalSummon, CanSummonFromHand */

/* ---- Local views kept for matching (build/readability/HEADERS.md) ---- */

/* The commit block at gDuel +0x1B28 (DuelState.cardMenuCard, then struct CardMenu): written when the
 * AI commits to the hand card gAiState.cardIndex; CardMenu_PlaySpellTrapFromHand then plays it. The
 * halfword bitfield containers are load-bearing: clearing s8.step as a u16 bitfield gives the ROM's
 * ldrh/mask/strh sequence (wiki/functions/ai-steps-c.md). */
struct CommitBlk {
    u16 cardMenuCard;               /* +0x00: card ID of the committed card */
    u8 pad02[2];
    union {                         /* +0x04: struct CardMenu, first byte */
        u8 raw;
        struct { u8 open:1; u8 confirmed:1; u8 unk2:6; } bf;
    } f4;
    struct { u16 unk0:2; u16 step:8; u16 unk10:6; u8 pad; u8 flags0B; } s8;  /* +0x08: step = CardMenu.step; flags0B bit 1 = CardMenu.player */
    struct { u16 unk0:1; u16 index:8; u16 unk9:7; u16 pad; } sC;             /* +0x0C: index = CardMenu.index (hand index of the committed card) */
};

/* gDuelPlayers (0x020192E4) as one block: player 0, player 1, then the gDuel tail up to the commit
 * block. The ROM also reaches the same fields from &gDuel (the view below) with the other literal
 * base, so both views stay. The hand is read as raw card words, not struct DuelCard. */
struct DuelPlayersView {
    u16 lifePoints0;                /* +0x0000 player 0 lifePoints */
    u8 pad0002[0xD64 - 0x2];
    u16 lifePoints1;                /* +0xD64 player 1 lifePoints */
    u8 handCount1;                  /* +0xD66 player 1 handCount */
    u8 deckCount1;                  /* +0xD67 player 1 deckCount */
    u8 pad0D68[3];
    u8 flags1_7;                    /* +0xD6B player 1 +0x007 byte: bit 5 read by AiStepChangePositions */
    u8 flags1_8;                    /* +0xD6C player 1 +0x008 byte: bit 4 = normalSummonUsed (the duel_phases.c view) */
    u8 pad0D6D[0x13E8 - 0xD6D];
    u32 hand1[80];                  /* +0x13E8 player 1 hand[] as card words (== 0x0201A6CC) */
    u8 pad1528[0x1B0C - 0x1528];
    u16 unk1B0C;                    /* +0x1B0C: the same short as gDuel.unk1B10 below */
    u8 pad1B0E[0x1B24 - 0x1B0E];
    struct CommitBlk commit;        /* +0x1B24: = gDuel +0x1B28 */
};
extern struct DuelPlayersView gDuelPlayers; /* 0x020192E4 */

/* gDuel (0x020192E0) as this unit reads it. */
struct DuelStateView {
    u8 pad0000[4];                  /* +0x0000 DuelState.unk0 */
    u16 lifePoints0;                /* +0x0004 players[0].lifePoints */
    u8 pad0006[0xD68 - 0x6];
    u16 lifePoints1;                /* +0xD68 players[1].lifePoints */
    u8 pad0D6A[0xD70 - 0xD6A];
    u8 flags1_8;                    /* +0xD70 players[1] +0x008 byte (the same byte as DuelPlayersView.flags1_8) */
    u8 pad0D71[0x13EC - 0xD71];
    u32 hand1[80];                  /* +0x13EC players[1].hand as card words */
    u8 pad152C[0x1B10 - 0x152C];
    u16 unk1B10;                    /* +0x1B10 */
    u8 pad1B12[2];
    union {                         /* +0x1B14 DuelState.unk1B14: AiStepBattle clears the two mid fields as halfwords */
        struct { u32 lo:9; u32 mid:8; u32 hi:15; } w;
        struct { u16 lo; u16 pad:1; u16 mid:8; u16 hi:7; } h;
    } flags1B14;
    u8 pad1B18[0x1B28 - 0x1B18];
    struct CommitBlk commit;        /* +0x1B28 */
};
extern struct DuelStateView gDuel;          /* 0x020192E0 */

extern struct DuelZone gDuelZonesP1[];      /* 0x0201A070: player 1 zones */
extern u32 gDuelHandP1[];                   /* 0x0201A6CC: player 1 hand as card words (= gDuelPlayers.hand1) */
extern const u16 gCardNumberToId_ToonWorld[];           /* card IDs planted into empty zones by the Toon simulation (meaning unknown) */

/* The effective stats of a zone card, as filled by GetZoneCardStats; this unit reads the type byte
 * whole (masked to the enum CardType bits) and the ATK. */
struct ZoneCardStats {
    u16 id;                         /* +0x0 */
    u8 type;                        /* +0x2: enum CardType in bits 0-4 */
    u8 unk3;
    s32 atk;                        /* +0x4 */
    s32 def;                        /* +0x8 */
};

/* ---- Prototypes not covered by the headers above ---- */

extern int (*const gAiSteps[])(void);       /* 0x0819DD6C */
int FindFreeMonsterZone(int player);
u32 GetZoneCardType(int player, int zone);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountActiveCardsOnFieldExcept(int player, u16 cardNo, int skipZone);
int CountZoneLinksFromCard(int player, int zone, u16 cardNo);
u32 GetZoneCardAtk(u32 player, u32 zone);
int CountMonsters(int player);
void ChangeBattlePosition(int player, int zone, u16 arg2, u16 arg3);
u32 HasFlipEffect(u16 cardNo, int flags);
int CountFreeMonsterZones(int player);
int FindFusionDeckCardByNumber(int player, int cardNo);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
void GetZoneCardStats(int player, int zone, struct ZoneCardStats *out);
int IsToonMonster(int cardNo);
int CountSpellTrapsFiltered(int player, u16 cardNo, u16 arg2, u16 arg3);
/* card_menu.h declares CardMenu_PlaySpellTrapFromHand(u16, u16, struct ChainEntry *); the all-int
 * form here is the matching one (build/readability/proto_mismatches.txt). */
void CardMenu_PlaySpellTrapFromHand(int activate, int asChainLink, int unused);
/* duel_cmd.h declares DuelCmd_Push(u16, u16, int, int); the all-u16 form is the matching one. */
void DuelCmd_Push(u16 cmd, u16 arg2, u16 arg4, u16 arg6);
/* chain.h declares Chain_AddPending(u32, u32); the int second argument is kept here. */
void Chain_AddPending(u32 packed, int locs);
/* battle.h declares BattlePhase_Run with an int return; the u16 return is the matching one
 * (wiki/functions/ai-steps-c.md). */
u16 BattlePhase_Run(int player);
int CanEnterBattlePhase(int player);

/* Card ID in the low 12 bits of a card word, kept in this shift form (table readers mask the ID
 * with CARD_ID_MASK instead). */
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])   /* gCardStats, kept as a literal address: the symbol form generates different code (card_data.h) */

/* Cost class of a card for the AI (see ai_summon): 0 for Trap, Magic and Ticket cards, 10 for
 * Divine cards, otherwise the stats level field (bits 25-28). */
static inline u32 CardCost(u32 id)
{
    u32 r;

    switch ((int)CARD_STATS_TYPE(CARD_STATS(id))) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        r = 0;
        break;
    case CARD_TYPE_DIVINE:
        r = 10;
        break;
    default:
        r = CARD_STATS_LEVEL(CARD_STATS(id));
        break;
    }
    return r;
}

/* Main AI phase: choose a hand card, pack the tribute choices and queue its summon; the later
 * phases run the activations.
 * The phase-10 tails assign the same state pointer after each call so the compiler shares the
 * original call/store tail without changing its reads. */
int AiStepMainPhase(void)
{
    int damage;
    int handIdx;
    /* FAKEMATCH: initialized pointer, card and mask lifetimes retain r4/r6/r5. */
    register u32 word asm("r4");
    register u32 id asm("r6");
    u16 hasFlip;
    u16 hasFlip2;
    register u32 idMasked asm("r5");
    int tribute1;
    int tribute2;
    int tributeMask;
    const u16 *numPtr;

    switch (gAiState.phase) {
    case AI_MAIN_SUMMON:
        if ((gDuelPlayers.flags1_8 & 0x10) != 0 || CanNormalSummon(1) == 0) {
            if (gAiState.step <= 3) {
                if (gDuelPlayers.unk1B0C != 0) {
                    gAiState.phase = AI_MAIN_RESET;
                    return 0;
                }
            }
            return 1;
        }
        if (CountActiveCardsOnFieldExcept(0, CARD_SWORDS_OF_REVEALING_LIGHT, -1) > 0) {
            gAiState.phase = AI_MAIN_RESET;
            return 0;
        }
        if (gDuelPlayers.unk1B0C == 0) {
            gAiState.phase = AI_MAIN_SET_MONSTER;
            return 0;
        }
        if (AiPlanAttack(1) != 0) {
            if (CanNormalSummon(1) == 0)
                return 1;
            handIdx = AiChooseSummonNoTribute();
            if (handIdx <= -1)
                return 1;
            word = gDuelPlayers.hand1[handIdx];
            word <<= 21;
            word >>= 20;
            word += (u32)gCardIdToNumber;
            numPtr = (const u16 *)word;
            hasFlip = HasFlipEffect(*numPtr, 1);
            if (HasFlipEffect(*numPtr, 0) != 0)
                hasFlip = 1;
            QueueNormalSummon(1, handIdx, FindFreeMonsterZone(1), 0, hasFlip == 0);
            return 1;
        }
        if (CanNormalSummon(1) == 0)
            return 1;
        handIdx = AiChooseSummonWithTribute(&damage);
        if (handIdx > -1) {
            {
                /* FAKEMATCH: stage the initialized card word before extraction. */
                register u32 raw asm("r0") = gDuelPlayers.hand1[handIdx];
                raw <<= 20;
                id = raw >> 20;
            }
            idMasked = CARD_ID_MASK;
            idMasked &= id;
            word = idMasked << 1;
            word += (u32)gCardIdToNumber;
            numPtr = (const u16 *)word;
            hasFlip2 = HasFlipEffect(*numPtr, 1);
            if (HasFlipEffect(*numPtr, 0) != 0) {
                /* FAKEMATCH: retain the initialized flag-copy scratch. */
                register int one asm("r1") = 1;
                asm("" : : "r"(one));
                hasFlip2 = one;
            }
            {
                u32 cost;
                {
                    /* FAKEMATCH: initialized table terms preserve cost-load scheduling.
                     * The table base stays the literal gCardStats address (0x08621DE0). */
                    register u32 off asm("r0") = idMasked << 2;
                    register u32 base asm("r2") = 0x08621DE0;
                    u32 stats;
                    asm("" : : "r"(off), "r"(base));
                    stats = *(const u32 *)(off + base);
                    switch ((int)CARD_STATS_TYPE(stats)) {
                    case CARD_TYPE_TRAP:
                    case CARD_TYPE_MAGIC:
                    case CARD_TYPE_TICKET:
                        cost = 0;
                        break;
                    case CARD_TYPE_DIVINE:
                        cost = 10;
                        break;
                    default: {
                        register u32 off asm("r0");
                        u32 base;
                        id &= CARD_ID_MASK;
                        off = id << 2;
                        base = 0x08621DE0;

                        cost = CARD_STATS_LEVEL(*(const u32 *)(off + base));
                    }
                    }
                }
                switch (cost) {
                case 0:
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                    {
                        int target = FindFreeMonsterZone(1);
                        int face = hasFlip2 == 0;
                        QueueNormalSummon(1, handIdx, target, 0, face);
                        /* FAKEMATCH: retain the index to keep this return tail separate. */
                        asm("" : : "r"(handIdx));
                    }
                    return 1;
                case 5:
                case 6:
                    tribute1 = AiPickTributeMonster(-1, 0);
                    if (tribute1 == -1)
                        break;
                    /* Tribute byte: zone | owner (bit 4) | present (bit 7), see QueueNormalSummon. */
                    tributeMask = 0x90 | tribute1;
                queueWithTribute:
                    QueueNormalSummon(1, handIdx, tribute1, tributeMask, hasFlip2 == 0);
                    return 1;
                default:
                    tribute1 = AiPickTributeMonster(-1, 0);
                    tribute2 = AiPickTributeMonster(tribute1, 0);
                    if (tribute1 == -1 || tribute2 == -1)
                        break;
                    {
                        /* FAKEMATCH: initialized tag copies preserve packed tribute bytes. */
                        register int tag asm("r1") = -0x70;
                        asm("" : : "r"(tag));
                        {
                            register u32 m asm("r0") = tag;
                            u32 lo = (u32)(m | tribute1) << 24;
                            u32 hi = (u32)(m | tribute2) << 24;
                            lo >>= 8;
                            tributeMask = (lo | hi) >> 16;
                        }
                    }
                    goto queueWithTribute;
                }
            }
        }
        gAiState.phase++;
        return 0;
    case AI_MAIN_RESET:
        gAiState.subState = 0;
        gAiState.listIndex = 0;
        gAiState.zoneIndex = 0;
        gAiState.unk9 = 0;
        gAiState.phase++;
        return 0;
    case AI_MAIN_MONSTER_EFFECTS:
        if (AiActivateMonsterEffects() != 0) {
            gAiState.phase = AI_MAIN_SUMMON;
            return 0;
        }
        gAiState.phase++;
        return 0;
    case AI_MAIN_EXODIA_TRAPS:
        if (AiActivateExodiaTraps() != 0) {
            gAiState.subState = 0;
            gAiState.listIndex = 0;
            gAiState.zoneIndex = 0;
            gAiState.unk9 = 0;
            gAiState.phase++;
        }
        return 0;
    case AI_MAIN_SPELLS:
        if (AiPlaySpells() != 0) {
            gAiState.subState = 0;
            gAiState.listIndex = 0;
            gAiState.zoneIndex = 0;
            gAiState.unk9 = 0;
            gAiState.phase = AI_MAIN_SET_MONSTER;
        }
        return 0;
    case AI_MAIN_SET_MONSTER: {
        u32 id;
        int faceUp;
        struct AiState *next;
        if ((gDuelPlayers.flags1_8 & 0x10) != 0)
            return 1;
        handIdx = AiPickMonsterToSet();
        if (handIdx < 0)
            return 1;
        id = CARD_ID(gDuelPlayers.hand1[handIdx]);
        switch (CardCost(id)) {
        case 0:
            return 1;
        case 1:
        case 2:
        case 3:
        case 4:
            if (FindFreeMonsterZone(1) < 0)
                break;
            faceUp = 0;
            {
                /* The literal hand base (0x0201A6CC = gDuelPlayers.hand1) is kept: the symbol
                 * form generates different code (card_data.h). */
                u32 base = 0x0201A6CC;
                u32 raw;
                u16 number;
                /* FAKEMATCH: retain the initialized hand-base lifetime. */
                asm("" : : "r"(base));
                raw = *(u32 *)(handIdx * 4 + base);
                number = *(const u16 *)(((raw << 21) >> 20) + (u32)gCardIdToNumber);
                if (number == CARD_TIME_WIZARD || number == CARD_CANNON_SOLDIER)
                    faceUp = 1;
            }
            QueueNormalSummon(1, handIdx, FindFreeMonsterZone(1), 0, faceUp);
            next = &gAiState;
            goto phase_store;
        case 5:
        case 6:
            tribute1 = AiPickTributeMonster(-1, 0);
            if (tribute1 == -1)
                break;
            QueueNormalSummon(1, handIdx, tribute1, 0x90 | tribute1, 0);
            next = &gAiState;
            goto phase_store;
        default:
            tribute1 = AiPickTributeMonster(-1, 0);
            tribute2 = AiPickTributeMonster(tribute1, 0);
            if (tribute1 == -1 || tribute2 == -1)
                break;
            {
                /* FAKEMATCH: initialized tag copies preserve packed tribute bytes. */
                register int tag asm("r1") = -0x70;
                asm("" : : "r"(tag));
                {
                    register u32 m asm("r0") = tag;
                    register u32 lo asm("r3") = (u32)(m | tribute1) << 24;
                    u32 hi = (u32)(m | tribute2) << 24;
                    lo >>= 8;
                    lo |= hi;
                    lo >>= 16;
                    tributeMask = lo;
                }
            }
            QueueNormalSummon(1, handIdx, tribute1, tributeMask, 1);
            next = &gAiState;
            goto phase_store;
        }
        next = &gAiState;
    phase_store:

        next->phase = AI_MAIN_RESET;
        return 0;
    }
    default:
        return 1;
    }
}

/* gAiState, kept as the literal address: the ROM reloads it inside the zone loop. */
#define AI ((struct AiState *)0x02015EF0)

/* Player 1 life points (gDuelPlayers.lifePoints1), read by address arithmetic. */
static inline u16 Player1LifePoints(void)
{
    u32 base = (u32)&gDuelPlayers;
    u32 off = 0xD64;
    return *(u16 *)(base + off);
}

/* FAKEMATCH: retain the initialized base and byte offset in separate registers.
 * Reads the player 1 +0x007 byte (gDuelPlayers.flags1_7). */
static inline u8 Player1FlagByte(void)
{
    register u32 base asm("r1") = (u32)&gDuelPlayers;
    register u32 off asm("r2") = 0xD6B;
    asm("" : : "r"(base), "r"(off));
    return *(u8 *)(base + off);
}

/* FAKEMATCH: retain the initialized count copy after the property test. */
static inline int ZoneSpaceCount(int count)
{
    register int x asm("r2") = count;
    asm("" : : "r"(x));
    return x;
}

/* FAKEMATCH: load the zone flags after copying the persistent face-up mask. */
static inline int ZoneFace(u32 flagMask, struct DuelZone *zone)
{
    u32 mask = flagMask;
    register u32 flags asm("r3") = zone->flags6;
    asm("" : : "r"(mask), "r"(flags));
    return mask & flags;
}

/* Scan CPU zones, simulate face-up stats, and queue a position/action change. */
int AiStepChangePositions(void)
{
    struct AiState *base = AI;
    u8 phase = base->phase;
    /* FAKEMATCH: this saved pointer survives every loop helper. */
    register struct AiState *st asm("r10") = base;

    switch (phase) {
    case 0:
        AiActivateMonsterEffects();
        st->subState = 0;
        {
            u32 phase = st->phase;
            phase++;
            {
                struct AiState *last = st;
                last->phase = phase;
            }
        }
        return 0;
    case 1:
        if (Player1FlagByte() & 0x20)
            return 1;
        if (st->subState <= 4) {
            do {
                int index = AI->subState;
                u32 addr = index;
                struct DuelZone *z;
                u32 id;
                u8 ok;
                int attack;
                int canSummon;

                addr *= 0x94;
                addr += (u32)gDuelZonesP1;
                z = (struct DuelZone *)addr;
                id = CARD_ID(*(u32 *)&z->card);
                ok = id != 0;
                if (z->flags6 & 2) {
                    if (GetZoneCardType(1, index) == 1) {
                        if (CountActiveCardsOnField(0, CARD_DRAGON_CAPTURE_JAR) != 0 || CountActiveCardsOnField(1, CARD_DRAGON_CAPTURE_JAR) != 0)
                            ok = 0;
                    }
                }
                if (CountZoneLinksFromCard(1, AI->subState, CARD_SPELLBINDING_CIRCLE) != 0)
                    ok = 0;
                if (CountZoneLinksFromCard(1, AI->subState, CARD_1244) != 0)
                    ok = 0;
                if (ok == 0) {
                    AI->subState++;
                    return 0;
                }
                if (AiCanChangePosition(1, AI->subState) != 0) {
                    canSummon = (u16)CanNormalSummon(1);
                    attack = GetZoneCardAtk(1, AI->subState);
                    {
                        /* FAKEMATCH: initialized address terms and flag copies
                         * preserve the simulation's load/store order. Only the
                         * callee-saved fieldBase and savedMask survive calls. */
                        register u32 savedMask asm("r8");
                        u32 stride = 0x94;
                        u32 offset;
                        register int index asm("r1") = AI->subState;
                        register u32 fieldBase asm("r4");
                        asm("" : : "r"(stride), "r"(index));
                        offset = index;
                        offset *= stride;
                        fieldBase = (u32)gDuelZonesP1;
                        {
                            struct DuelZone *z = (struct DuelZone *)(offset + fieldBase);
                            register u32 flags asm("r1") = z->flags6;
                            u8 set = 2;
                            register u32 mask asm("r0") = 2;
                            savedMask = mask;
                            if ((flags & mask) == 0) {
                                register u32 raw asm("r0") = flags;
                                raw |= set;
                                z->flags6 = raw;
                                attack = GetZoneCardAtk(1, AI->subState);
                                {
                                    /* The helper may change subState, so clear the newly
                                     * selected zone, as the ROM does. */
                                    register u32 index asm("r2") = AI->subState;
                                    u32 offset;
                                    asm("" : : "r"(index));
                                    offset = index;
                                    offset *= stride;
                                    offset += fieldBase;
                                    {
                                        register int tag asm("r2") = ~2;
                                        u32 copy;
                                        asm("" : : "r"(tag));
                                        copy = tag;
                                        {
                                            register u32 old asm("r2") = *(u8 *)(offset + 6);
                                            asm("" : : "r"(old));
                                            *(u8 *)(offset + 6) = copy & old;
                                        }
                                    }
                                }
                            }
                        }
                        {
                            int zone = AI->subState;
                            u32 off = zone * stride;
                            struct DuelZone *z = (struct DuelZone *)(off + fieldBase);
                            {
                                int number = *(const u16 *)(((*(u32 *)&z->card << 21) >> 20) + 0x08622AB4);
                                switch (number) {
                                case CARD_PARASITE_PARACIDE:
                                    if (ZoneFace(savedMask, z) == 0 && canSummon) {
                                        QueueFlipSummon(1, zone);
                                        AI->subState++;
                                        return 0;
                                    }
                                    break;
                                case CARD_MAN_EATER_BUG:
                                    if (ZoneFace(savedMask, z) == 0 && CountMonsters(0) > 0 && canSummon) {
                                        QueueFlipSummon(1, AI->subState);
                                        AI->subState++;
                                        return 0;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    if (AiCanBeatAnyMonster(attack) != 0 && attack > 0) {
                        struct AiState *cur = AI;
                        u32 stride = 0x94;
                        /* FAKEMATCH: retain the initialized index copy. */
                        register u32 index asm("r2") = cur->subState;
                        u32 off;
                        struct DuelZone *z;
                        asm("" : : "r"(cur), "r"(stride), "r"(index));
                        off = index;
                        off *= stride;
                        off += (u32)gDuelZonesP1;
                        z = (struct DuelZone *)off;
                        if ((z->flags6 & 3) == 1) {
                            /* The original call leaves the table base in r1.
                             * HasFlipEffect explicitly consumes its low half. */
                            if (HasFlipEffect(*(const u16 *)(((*(u32 *)&z->card << 21) >> 20) + 0x08622AB4), 0x08622AB4) == 0 && ZoneSpaceCount(canSummon)) {
                                QueueFlipSummon(1, cur->subState);
                                cur->subState++;
                                return 0;
                            }
                        }
                        {
                            u32 index = AI->subState;
                            u32 off;
                            off = index;
                            off *= 0x94;
                            off += (u32)gDuelZonesP1;
                            z = (struct DuelZone *)off;
                            if ((z->flags6 & 3) == 3) {
                                ChangeBattlePosition(1, index, 0, 0);
                                AI->subState++;
                                return 0;
                            }
                        }
                    } else {
                        struct AiState *cur = AI;
                        /* FAKEMATCH: initialize stride before loading the index. */
                        register u32 stride asm("r0") = 0x94;
                        u32 off;
                        struct DuelZone *z;
                        off = cur->subState;
                        off *= stride;
                        z = (struct DuelZone *)(off + (u32)gDuelZonesP1);
                        if ((z->flags6 & 3) == 2 && (gDuelCtrl.aiFlags & AI_FLAG_EXODIA) != 0) {
                            int number = *(const u16 *)(((*(u32 *)&z->card << 21) >> 20) + 0x08622AB4);
                            switch (number) {
                            case CARD_WITCH_OF_THE_BLACK_FOREST:
                            case CARD_SANGAN:
                            case CARD_MYSTIC_TOMATO:
                                if (Player1LifePoints() > 3000 && AiCountExodiaInDeck() > 0)
                                    AiCountExodiaInGraveyard();
                                break;
                            }
                        }
                    }
                }
                AI->subState++;
            } while (AI->subState <= 4);
        }
        {
            /* FAKEMATCH: keep the final phase increment separate from case 0. */
            register struct AiState *last asm("r2") = st;
            last->phase++;
        }
        return 0;
    default:
        return 1;
    }
}


u16 AiStepBattle(void)
{
    struct AiState *st = &gAiState;
    if (st->phase == 0) {
        if (CanEnterBattlePhase(1) == 0)
            return 1;
        st->phase++;
        /* Clear the two mid fields of gDuel +0x1B14 as halfword bitfield stores. */
        gDuel.flags1B14.w.mid = 0;
        gDuel.flags1B14.h.mid = 0;
    }
    return BattlePhase_Run(1);
}

int AiRunStep(void)
{
    int (*fn)(void) = gAiSteps[gAiState.step];
    struct AiState *st = &gAiState;
    if (fn != 0) {
        if ((u16)fn() != 0) {
            st->stepState = 0;
            st->stepIndex = 0;
            st->unk4 = 0;
            st->flipZone = 0;
            st->phase = 0;
            st->subState = 0;
            st->step++;
        }
        return 0;
    }
    return 1;
}

/* Zone views for the Toon simulation in AiChooseStrategy (case AI_STRATEGY_TOON_WORLD): while the
 * duel state is backed up, empty monster zones get a planted card word, the face-up bit set and the
 * disabled bit cleared. */
struct ZoneSimView {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 flag6_0:1;                   /* +0x06 bit 0 */
    u8 isFaceUp:1;                  /* +0x06 bit 1 */
    u8 unk6_2:6;
    u8 unk7[0x91 - 0x7];
    u8 unk91;
    u8 unk92[2];
};
struct ZoneSim91 {
    u8 unk0:3;
    u8 isDisabled:1;                /* +0x91 bit 3 */
    u8 unk4:4;
};

/* Probe the nine scripted strategies in order; on the first whose cards and conditions are met,
 * store (strategy << 1) | 1 in gAiWork's strategy byte (strategyActive + strategy) and return 1. */
int AiChooseStrategy(void)
{
    int i;
    int ok;
    int sum;
    int k;
    struct ZoneCardStats info;
    gAiWork.strategyActive = 0;
    for (i = 0; i <= 8; i++) {
        ok = 1;
        switch (i) {
        case AI_STRATEGY_CYBER_STEIN:
            if (gDuel.unk1B10 == 0)
                ok = 0;
            if (gDuel.flags1_8 & 0x10)
                ok = 0;
            if (gDuel.lifePoints1 <= 5000)  /* Cyber-Stein pays 5000 LP */
                ok = 0;
            if (CountFreeMonsterZones(1) <= 1)
                ok = 0;
            if (AiFindHandCardByNumber(1, CARD_MEGAMORPH) == -1 && gDuel.lifePoints0 > gDuel.lifePoints1)
                ok = 0;
            if (AiFindHandCardByNumber(1, CARD_CYBER_STEIN) == -1)
                ok = 0;
            if (FindFusionDeckCardByNumber(1, CARD_BLUE_EYES_ULTIMATE_DRAGON) != -1)
                break;
            continue;
        case AI_STRATEGY_VALKYRION:
            if (gDuel.unk1B10 == 0)
                ok = 0;
            if (CountFreeMonsterZones(1) == 0)
                ok = 0;
            k = CARD_VALKYRION_THE_MAGNA_WARRIOR;
            if (AiFindHandCardByNumber(1, k) == -1)
                ok = 0;
            if (CanSummonFromHand(1, ((const u16 *)0x08623DF4)[k]) == 0)  /* 0x08623DF4 = gCardNumberToId, kept as a literal: the symbol form generates different code (card_data.h) */
                ok = 0;
            DebugPrintFlush();
            break;
        case AI_STRATEGY_FOUR_TOKENS_CANNON_SOLDIER:
            if (gDuel.unk1B10 == 0)
                ok = 0;
            k = CARD_1418;
            if (CountActiveCardsOnField(0, k) > 0)
                ok = 0;
            if (CountActiveCardsOnField(1, k) > 0)
                ok = 0;
            if (CountFaceUpMonstersByNumber(1, CARD_CANNON_SOLDIER) == 0)
                ok = 0;
            if (CountFreeMonsterZones(1) <= 1)
                ok = 0;
            if (AiFindHandCardByNumber(1, CARD_1245) != -1)
                break;
            continue;
        case AI_STRATEGY_ELEGANT_EGOTIST: {
            int c;
            int r;
            if (CountFreeMonsterZones(1) <= 1)
                ok = 0;
            if (AiFindHandCardByNumber(1, CARD_ELEGANT_EGOTIST) == -1)
                ok = 0;
            if (CountFaceUpMonstersByNumber(0, CARD_HARPIE_LADY) != 0)
                break;
            c = 0x4E1;  /* key 1249: no EDS card, kept numeric */
            if (CountFaceUpMonstersByNumber(0, c) != 0)
                break;
            if (CountFaceUpMonstersByNumber(1, CARD_HARPIE_LADY) != 0)
                break;
            if (CountFaceUpMonstersByNumber(1, c) != 0)
                break;
            if (gDuelPlayers.flags1_8 & 0x10)
                continue;
            r = AiFindHandCardByNumber(1, CARD_HARPIE_LADY);
            if (r == -1 && AiFindHandCardByNumber(1, c) == r)
                ok = 0;
            if (CountFreeMonsterZones(1) > 2)
                break;
            continue;
        }
        case AI_STRATEGY_BANISH_THREE_SUMMON:
            /* Same tail as case 7: jump2 cross-jumps it after reload, so its reloads
             * still advance the reload round-robin (case 4's base then lands in r0). */
            k = CARD_1514;
            if (AiFindHandCardByNumber(1, k) == -1)
                continue;
            if (CanSummonFromHand(1, ((const u16 *)0x08623DF4)[k]) != 0)
                break;
            continue;
        case AI_STRATEGY_DOUBLE_MACHINE_ATK: {
            int j;
            ok = 0;
            if (AiFindHandCardByNumber(1, CARD_1314) < 0)
                break;
            if (CountMonsters(1) <= 0)
                break;
            /* The literal zone base (0x0201A070 = gDuelZonesP1) is kept: through the extern the
             * base is hoisted out of the loop, through the literal it is reloaded each use. */
            for (j = 0; j <= 4; j++) {
                if (((struct DuelZone *)0x0201A070)[j].flags6 & 2) {
                    GetZoneCardStats(1, j, &info);
                    if ((info.type & 0x1F) == CARD_TYPE_MACHINE)
                        sum += info.atk;  /* sum is never initialized (an original bug, kept) */
                }
            }
            if (gDuelPlayers.lifePoints0 > sum * 2)
                break;
            goto success;
        }
        case AI_STRATEGY_NONE:
            continue;
        case AI_STRATEGY_BANISH_TWO_SUMMON:
            k = 0x5EB;  /* key 1515: no EDS card, kept numeric */
            if (AiFindHandCardByNumber(1, k) == -1)
                continue;
            if (CanSummonFromHand(1, ((const u16 *)0x08623DF4)[k]) != 0)
                break;
            continue;
        case AI_STRATEGY_TOON_WORLD: {
            int n;
            struct DuelZone *zones;
            const u16 *src;
            struct DuelZone *dz;
            struct ZoneSim91 *f91;
            ok = 0;
            for (n = 0; n < gDuelPlayers.handCount1; n++) {
                u16 id = CARD_ID(*(u32 *)(n * 4 + (u32)gDuelHandP1));
                if (IsToonMonster(((const u16 *)0x08622AB4)[id & 0x7FF]) != 0 && CanSummonFromHand(1, id) != 0)
                    ok = 1;
            }
            if (AiFindHandCardByNumber(1, CARD_TOON_WORLD) < 0)
                break;
            AiBackupDuelState();
            zones = gDuelZonesP1;
            src = gCardNumberToId_ToonWorld;
            /* FAKEMATCH: two pointers stepped by hand (flag byte +0x91 first) and a signed
             * pointer compare. The ROM sets them up before the masks, which loop pass 1 hoists,
             * and before the end value, so the loop cannot be an indexed z loop (strength
             * reduction would put its inits after the hoisted masks). */
            f91 = (struct ZoneSim91 *)&zones[5].flags91;
            dz = &zones[5];
            do {
                u16 cid = CARD_ID(*(u32 *)&dz->card);
                /* FAKEMATCH: two empty insns raise the loop's insn count so loop pass 2 keeps
                 * the 0xFFF and #2 constants inside the loop, as in the ROM. */
                asm volatile("");
                asm volatile("");
                if (cid == 0) {
                    dz->card.id = src[0];
                    ((struct ZoneSimView *)dz)->isFaceUp = 1;
                    f91->isDisabled = 0;
                }
                f91 = (struct ZoneSim91 *)((u8 *)f91 + 0x94);
                dz++;
            } while ((int)dz <= (int)&zones[9]);
            for (n = 0; n < gDuelPlayers.handCount1; n++) {
                u16 id = CARD_ID(*(u32 *)(n * 4 + (u32)gDuelHandP1));
                if (IsToonMonster(((const u16 *)0x08622AB4)[id & 0x7FF]) != 0 && CanSummonFromHand(1, id) != 0)
                    ok = 1;
            }
            AiRestoreDuelState();
            break;
        }
        default:
            ok = 0;
            break;
        }
        if (ok != 0) {
            u8 *work;
        success:
            work = (u8 *)&gAiWork;
            {
                u8 *strategyByte = work + 0x1B24;
                int strategyBits = i << 1;
                int one = 1;
                /* FAKEMATCH: opaque 1 so the orr output ties to the constant register (ROM: mov r1, #1; orr r1, r2) */
                asm("" : "+r"(one));
                *strategyByte = one | strategyBits;
            }
            return 1;
        }
    }
    return 0;
}

/* Commit hand card cardIndex of player 1 into the commit block of base (gDuel or gDuelPlayers):
 * store its card ID, mark the menu confirmed for player 1, and advance the strategy step. */
#define COMMIT_HEAD(base) ((base).commit.s8.step = 0)
#define COMMIT_TAIL(base) \
    do { \
        struct AiState *st_ = &gAiState; \
        (base).commit.cardMenuCard = CARD_ID((base).hand1[st_->cardIndex]); \
        (base).commit.s8.flags0B |= 2; \
        (base).commit.sC.index = st_->cardIndex; \
        (base).commit.f4.bf.confirmed = 1; \
        st_->stepState++; \
    } while (0)

/* FAKEMATCH: initialized address copies preserve the LP load's ADD operands.
 * Reads player 1 life points (gDuelPlayers.lifePoints1). */
static inline u16 NeighborLife(void)
{
    struct DuelPlayersView *base = &gDuelPlayers;
    register u32 baseAddr asm("r5") = (u32)base;
    register u32 off asm("r3") = 0xD64;
    asm("" : "+r"(off));
    {
        register u16 *p asm("r0") = (u16 *)(baseAddr + off);
        return *p;
    }
}

/* Strategy AI_STRATEGY_CYBER_STEIN: clear the opponent's field, summon Cyber-Stein, use its effect
 * for Blue-Eyes Ultimate Dragon, play Megamorph, then enter the Battle Phase. The step machine
 * (gAiState.stepState) is run by AiStepRunStrategy. */
int AiStrategyCyberStein(void)
{
    int j;
    u32 tgt;
    u32 id;

    switch (gAiState.stepState) {
    case 0:
        if (CountSpellTrapsFiltered(0, 0, 0, 0) > 0) {
            if (AiTryPlaySpellTrap(CARD_HARPIES_FEATHER_DUSTER) != 0) {
                gDuel.commit.s8.step = 0;
                COMMIT_TAIL(gDuel);
                return 0;
            }
            if (AiTryPlaySpellTrap(CARD_HEAVY_STORM) != 0 || AiTryPlaySpellTrap(CARD_GIANT_TRUNADE) != 0)
                goto head2;
        }
        gAiState.stepState += 2;
        return 0;
    case 1:
    case 3:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gDuel.commit.f4.raw & 2) == 0)  /* struct CardMenu confirmed bit */
            gAiState.stepState--;
        return 0;
    case 2:
        if (CountMonsters(0) > 0) {
            if (AiTryPlaySpellTrap(CARD_DARK_HOLE) != 0 || AiTryPlaySpellTrap(CARD_RAIGEKI) != 0) {
            head2:
                COMMIT_HEAD(gDuel);
                COMMIT_TAIL(gDuel);
                return 0;
            }
        }
        gAiState.stepState += 2;
        return 0;
    case 4:
        if (FindFreeMonsterZone(1) == -1) {
            gAiWork.strategyActive = 0;
            return 0;
        }
        gAiWork.strategyZone = FindFreeMonsterZone(1);
        j=0;
        {
          int n = gDuelPlayers.handCount1;
          if(j<n) {
            u32 needle=CARD_CYBER_STEIN;
            int bound=n;
            u32 off=0x13E8;  /* offset of DuelPlayersView.hand1 */
            u32 *cards=(u32 *)((u32)&gDuelPlayers+off);
            u32 mask=CARD_ID_MASK;
            const u16 *table=gCardIdToNumber;
            do {
              if (table[CARD_ID(*cards) & mask] == needle)
                goto queue;
              cards++;
              j++;
            } while (j < bound);
          }
        }
        gAiWork.strategyActive = 0;
        return 0;
    case 5: {
        struct AiWork *aiWork = &gAiWork;
        u8 *targetPtr = &aiWork->strategyZone;
        struct DuelZone *z;

        {
            /* FAKEMATCH: load the target before making its persistent copy. */
            register u32 raw asm("r0") = *targetPtr;
            tgt = raw;
            z = &gDuelZonesP1[raw];
        }
        id = CARD_ID(*(u32 *)&z->card);
        if (id == 0) {
            aiWork->strategyActive = 0;
            return 0;
        }
        if ((z->flags6 & 2) == 0) {
            aiWork->strategyActive = 0;
            return 0;
        }
        if (((const u16 *)0x08622AB4)[id & 0x7FF] != CARD_CYBER_STEIN) {
            aiWork->strategyActive = 0;
            return 0;
        }
        DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD, 1, tgt << 8, 0);
        tgt = *targetPtr;
        {
            /* FAKEMATCH: initialized packing values preserve the mask/copy order. */
            register u32 mask asm("r1") = 0x1F;
            register u32 hi asm("r0") = tgt & mask;
            hi <<= 16;
            {
                u32 lo = CARD_ID(*(u32 *)&gDuelZonesP1[tgt].card);
                lo |= 0x80400000;
                hi |= lo;
                Chain_AddPending(hi, 0);
            }
        }
        gAiState.stepState++;
        return 0;
    }
    case 6:
        if (CountFaceUpMonstersByNumber(1, CARD_BLUE_EYES_ULTIMATE_DRAGON) == 0)
            goto fail;
        gAiState.stepState++;
        return 0;
    case 7:
        if (gDuelPlayers.lifePoints0 <= 0x13EB || gDuelPlayers.lifePoints0 < NeighborLife()) {  /* 0x13EB = 5099 */
            gAiState.stepState = 8;
            return 0;
        }
        if (AiTryPlaySpellTrap(CARD_MEGAMORPH) == 0)
            goto fail;
        COMMIT_HEAD(gDuelPlayers);
        COMMIT_TAIL(gDuelPlayers);
        return 0;
    fail:
        gAiWork.strategyActive=0;
        return 0;
    case 8:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gDuel.commit.f4.raw & 2) == 0) {
            gDuel.flags1B14.w.mid = 0;
            gDuel.flags1B14.h.mid = 0;
            gAiState.stepState++;
        }
        return 0;
    case 9:
        return BattlePhase_Run(1);
    queue:
        {
            /* FAKEMATCH: initialize the queue base after the hand search. */
            register u8 *work asm("r0") = (u8 *)&gAiWork;
            QueueNormalSummon(1, j, work[0x1B25], 0, 1);  /* work[0x1B25] = AiWork.strategyZone */
        }
        gAiState.stepState++;
        return 0;
    default:
        return 1;
    }
}
