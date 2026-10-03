#include "global.h"

struct AiState {
    u8 f0;
    u8 step;    /* +1 */
    u8 f2, f3, f4, f5;
    u8 f6;      /* +6 loop counter */
    u8 f7, f8, f9;
    u8 phase;   /* +0xA */
    u8 f_idx;   /* +0xB hand index of the chosen card */
};
extern struct AiState gAiState;

/* Duel zone of the CPU (player 1), 0x94 bytes each. */
struct DuelCard {
    u32 id : 12;
    u32 unk12 : 1;
    u32 unk13 : 19;
};
#define CARD_ID(w) (((w) << 20) >> 20)
static inline u16 CardNumber(u32 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}
#define CARD_NUMBER(id) CardNumber(id)
struct CardLo {
    u16 id : 12;
    u16 rest : 4;
    u16 hi;
};
struct DuelZone {
    struct DuelCard card;
    u8 unk4[2];
    u8 flags6;
    u8 rest[0x91 - 7];
    u8 unk91;
    u8 rest2[2];
};
extern struct DuelZone gDuelZonesP1[];
extern const u16 gCardIdToNumber[];
struct Unk02015EE8 {
    u32 unk0;
    u32 flags;
};
extern struct Unk02015EE8 gDuelCtrl;
/* State block written when the AI commits to a hand card (seen at 0x0201AE08..0x0201AE18). */
struct CommitBlk {
    u16 cardId;                                             /* +0x00 */
    u8 pad2[2];
    union { u8 raw; struct { u8 a : 1; u8 b : 1; u8 c : 6; } bf; } f4;   /* +0x04 bit 1 */
    struct { u16 a : 2; u16 b : 8; u16 c : 6; u8 pad; u8 f3; } s8;      /* +0x08 */
    struct { u16 a : 1; u16 idx : 8; u16 c : 7; u16 pad; } sC;         /* +0x0C */
};
struct DuelPlayers {
    u16 lp0;
    u8 pad[0xD64 - 2];
    u16 lp;         /* player 1 life points (hypothesis) */
    u8 handCount;   /* player 1 +2 */
    u8 deckCount;
    u8 pad2[3];
    u8 f7;          /* player 1 byte +7 */
    u8 fD6C;        /* player 1 byte +8 */
    u8 padD6D[0x13E8 - 0xD6D];
    u32 hand1[80];  /* +0x13E8 player 1 hand (== 0x0201A6CC) */
    u8 pad152[0x1B0C - 0x1528];
    u16 f1B0C;
    u8 pad1B0E[0x1B24 - 0x1B0E];
    struct CommitBlk c;
};
extern struct DuelPlayers gDuelPlayers;
int AiActivateMonsterEffects(void);
/* The ASM callee explicitly decodes both final word arguments as u16. */
void QueueNormalSummon(int, int, int, int, int);
int FindFreeMonsterZone(int);
void QueueFlipSummon(int, int);
u32 GetZoneCardType(int, int);
int CountActiveCardsOnField(int, u16);
int CountZoneLinksFromCard(int, int, u16);
int AiCanChangePosition(int, int);
int CanNormalSummon(int);
u32 GetZoneCardAtk(u32, u32);
int CountMonsters(int);
int AiCanBeatAnyMonster(int);
void ChangeBattlePosition(int, int, u16, u16);
int AiCountExodiaInDeck(void);
int AiCountExodiaInGraveyard(void);
extern int (*const gAiSteps[])(void);
int CanEnterBattlePhase(int);
u16 BattlePhase_Run(int);
struct Unk020192E0 {
    u8 pad0[4];
    u16 lp0;                /* +4 player 0 life points (hypothesis) */
    u8 pad1[0xD68 - 6];
    u16 lp1;                /* +0xD68 player 1 life points (hypothesis) */
    u8 padD[0xD70 - 0xD6A];
    u8 fD70;                /* bit 4 */
    u8 pad2[0x13EC - 0xD71];
    u32 hand1[80];          /* +0x13EC player 1 hand */
    u8 pad2b[0x1B10 - 0x152C];
    u16 f1B10;
    u8 pad3[2];
    union {
        struct { u32 a : 9; u32 b : 8; u32 c : 15; } w;
        struct { u16 skip; u16 pad : 1; u16 b : 8; u16 c : 7; } h;
    } u;
    u8 pad4[0x1B28 - 0x1B18];
    struct CommitBlk c;
};
extern struct Unk020192E0 gDuel;

/* Cost class of a card for the AI (see ai_summon): 0 for type 0x15-0x17, 10 for 0x18, else a 4-bit stats field. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
static inline u32 CardCost(u32 id)
{
    u32 r;

    switch ((int)((CARD_STATS(id) & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 10;
        break;
    default:
        r = (CARD_STATS(id) & 0x1E000000) >> 25;
        break;
    }
    return r;
}
u32 HasFlipEffect(u16 cardNo, int flags);
int CountActiveCardsOnFieldExcept(int player, u16 cardNo, int skipZone);
int AiPlanAttack(u16 drop);
int AiChooseSummonNoTribute(void);
int AiChooseSummonWithTribute(int *out);
int AiPickTributeMonster(int a, u16 b);
int AiPickMonsterToSet(void);
int AiActivateExodiaTraps(void);
int AiPlaySpells(void);
#define E4 gDuelPlayers
#define S gAiState

/* Main AI phase: choose a hand card, pack tribute choices and queue its action.
 * The phase-10 tails assign the same state pointer after each call so the
 * compiler shares the original call/store tail without changing its reads. */
int AiStepMainPhase(void)
{
    int out;
    int idx;
    /* FAKEMATCH: initialized pointer, card and mask lifetimes retain r4/r6/r5. */
    register u32 w asm("r4");
    register u32 id asm("r6");
    u16 flag;
    u16 flag2;
    register u32 n asm("r5");
    int a;
    int b;
    int mask;
    const u16 *p;

    switch (S.phase) {
    case 0:
        if ((E4.fD6C & 0x10) != 0 || CanNormalSummon(1) == 0) {
            if (S.step <= 3) {
                if (E4.f1B0C != 0) {
                    S.phase = 1;
                    return 0;
                }
            }
            return 1;
        }
        if (CountActiveCardsOnFieldExcept(0, 0x15B, -1) > 0) {
            S.phase = 1;
            return 0;
        }
        if (E4.f1B0C == 0) {
            S.phase = 10;
            return 0;
        }
        if (AiPlanAttack(1) != 0) {
            if (CanNormalSummon(1) == 0)
                return 1;
            idx = AiChooseSummonNoTribute();
            if (idx <= -1)
                return 1;
            w = E4.hand1[idx];
            w <<= 21;
            w >>= 20;
            w += (u32)gCardIdToNumber;
            p = (const u16 *)w;
            flag = HasFlipEffect(*p, 1);
            if (HasFlipEffect(*p, 0) != 0)
                flag = 1;
            QueueNormalSummon(1, idx, FindFreeMonsterZone(1), 0, flag == 0);
            return 1;
        }
        if (CanNormalSummon(1) == 0)
            return 1;
        idx = AiChooseSummonWithTribute(&out);
        if (idx > -1) {
            {
                /* FAKEMATCH: stage the initialized card word before extraction. */
                register u32 raw asm("r0") = E4.hand1[idx];
                raw <<= 20;
                id = raw >> 20;
            }
            n = 0x7FF;
            n &= id;
            w = n << 1;
            w += (u32)gCardIdToNumber;
            p = (const u16 *)w;
            flag2 = HasFlipEffect(*p, 1);
            if (HasFlipEffect(*p, 0) != 0) {
                /* FAKEMATCH: retain the initialized flag-copy scratch. */
                register int one asm("r1") = 1;
                asm("" : : "r"(one));
                flag2 = one;
            }
            {
                u32 cost;
                {
                    /* FAKEMATCH: initialized table terms preserve cost-load scheduling. */
                    register u32 off asm("r0") = n << 2;
                    register u32 base asm("r2") = 0x08621DE0;
                    u32 stats;
                    asm("" : : "r"(off), "r"(base));
                    stats = *(const u32 *)(off + base);
                    switch ((int)((stats & 0x1F00000) >> 20)) {
                    case 0x15:
                    case 0x16:
                    case 0x17:
                        cost = 0;
                        break;
                    case 0x18:
                        cost = 10;
                        break;
                    default: {
                        register u32 off asm("r0");
                        u32 base;
                        id &= 0x7FF;
                        off = id << 2;
                        base = 0x08621DE0;

                        cost = (*(const u32 *)(off + base) & 0x1E000000) >> 25;
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
                        int face = flag2 == 0;
                        QueueNormalSummon(1, idx, target, 0, face);
                        /* FAKEMATCH: retain the index to keep this return tail separate. */
                        asm("" : : "r"(idx));
                    }
                    return 1;
                case 5:
                case 6:
                    a = AiPickTributeMonster(-1, 0);
                    if (a == -1)
                        break;
                    mask = 0x90 | a;
                queue_first:
                    QueueNormalSummon(1, idx, a, mask, flag2 == 0);
                    return 1;
                default:
                    a = AiPickTributeMonster(-1, 0);
                    b = AiPickTributeMonster(a, 0);
                    if (a == -1 || b == -1)
                        break;
                    {
                        /* FAKEMATCH: initialized tag copies preserve packed tribute bytes. */
                        register int tag asm("r1") = -0x70;
                        asm("" : : "r"(tag));
                        {
                            register u32 m asm("r0") = tag;
                            u32 lo = (u32)(m | a) << 24;
                            u32 hi = (u32)(m | b) << 24;
                            lo >>= 8;
                            mask = (lo | hi) >> 16;
                        }
                    }
                    goto queue_first;
                }
            }
        }
        S.phase++;
        return 0;
    case 1:
        S.f6 = 0;
        S.f7 = 0;
        S.f8 = 0;
        S.f9 = 0;
        S.phase++;
        return 0;
    case 2:
        if (AiActivateMonsterEffects() != 0) {
            S.phase = 0;
            return 0;
        }
        S.phase++;
        return 0;
    case 3:
        if (AiActivateExodiaTraps() != 0) {
            S.f6 = 0;
            S.f7 = 0;
            S.f8 = 0;
            S.f9 = 0;
            S.phase++;
        }
        return 0;
    case 4:
        if (AiPlaySpells() != 0) {
            S.f6 = 0;
            S.f7 = 0;
            S.f8 = 0;
            S.f9 = 0;
            S.phase = 10;
        }
        return 0;
    case 10: {
        u32 id;
        int flag;
        struct AiState *next;
        if ((E4.fD6C & 0x10) != 0)
            return 1;
        idx = AiPickMonsterToSet();
        if (idx < 0)
            return 1;
        id = CARD_ID(E4.hand1[idx]);
        switch (CardCost(id)) {
        case 0:
            return 1;
        case 1:
        case 2:
        case 3:
        case 4:
            if (FindFreeMonsterZone(1) < 0)
                break;
            flag = 0;
            {
                u32 base = 0x0201A6CC;
                u32 raw;
                u16 num;
                /* FAKEMATCH: retain the initialized hand-base lifetime. */
                asm("" : : "r"(base));
                raw = *(u32 *)(idx * 4 + base);
                num = *(const u16 *)(((raw << 21) >> 20) + (u32)gCardIdToNumber);
                if (num == 0xF || num == 0x1FF)
                    flag = 1;
            }
            QueueNormalSummon(1, idx, FindFreeMonsterZone(1), 0, flag);
            next = &S;
            goto phase_store;
        case 5:
        case 6:
            a = AiPickTributeMonster(-1, 0);
            if (a == -1)
                break;
            QueueNormalSummon(1, idx, a, 0x90 | a, 0);
            next = &S;
            goto phase_store;
        default:
            a = AiPickTributeMonster(-1, 0);
            b = AiPickTributeMonster(a, 0);
            if (a == -1 || b == -1)
                break;
            {
                /* FAKEMATCH: initialized tag copies preserve packed tribute bytes. */
                register int tag asm("r1") = -0x70;
                asm("" : : "r"(tag));
                {
                    register u32 m asm("r0") = tag;
                    register u32 lo asm("r3") = (u32)(m | a) << 24;
                    u32 hi = (u32)(m | b) << 24;
                    lo >>= 8;
                    lo |= hi;
                    lo >>= 16;
                    mask = lo;
                }
            }
            QueueNormalSummon(1, idx, a, mask, 1);
            next = &S;
            goto phase_store;
        }
        next = &S;
    phase_store:

        next->phase = 1;
        return 0;
    }
    default:
        return 1;
    }
}

#define AI ((struct AiState *)0x02015EF0)
static inline u16 ZoneLife(void)
{
    u32 base = (u32)&gDuelPlayers;
    u32 off = 0xD64;
    return *(u16 *)(base + off);
}

/* FAKEMATCH: retain the initialized base and byte offset in separate registers. */
static inline u8 ZonePlayerFlags(void)
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
        st->f6 = 0;
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
        if (ZonePlayerFlags() & 0x20)
            return 1;
        if (st->f6 <= 4) {
            do {
                int index = AI->f6;
                u32 addr = index;
                struct DuelZone *z;
                u32 id;
                u8 ok;
                int attack;
                int freeZones;

                addr *= 0x94;
                addr += (u32)gDuelZonesP1;
                z = (struct DuelZone *)addr;
                id = CARD_ID(*(u32 *)&z->card);
                ok = id != 0;
                if (z->flags6 & 2) {
                    if (GetZoneCardType(1, index) == 1) {
                        if (CountActiveCardsOnField(0, 0x148) != 0 || CountActiveCardsOnField(1, 0x148) != 0)
                            ok = 0;
                    }
                }
                if (CountZoneLinksFromCard(1, AI->f6, 0x15C) != 0)
                    ok = 0;
                if (CountZoneLinksFromCard(1, AI->f6, 0x4DC) != 0)
                    ok = 0;
                if (ok == 0) {
                    AI->f6++;
                    return 0;
                }
                if (AiCanChangePosition(1, AI->f6) != 0) {
                    freeZones = (u16)CanNormalSummon(1);
                    attack = GetZoneCardAtk(1, AI->f6);
                    {
                        /* FAKEMATCH: initialized address terms and flag copies
                         * preserve the simulation's load/store order. Only the
                         * callee-saved fieldBase and savedMask survive calls. */
                        register u32 savedMask asm("r8");
                        u32 stride = 0x94;
                        u32 offset;
                        register int index asm("r1") = AI->f6;
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
                                attack = GetZoneCardAtk(1, AI->f6);
                                {
                                    /* The helper may change f6, so clear the newly
                                     * selected zone, as the ROM does. */
                                    register u32 index asm("r2") = AI->f6;
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
                            int zone = AI->f6;
                            u32 off = zone * stride;
                            struct DuelZone *z = (struct DuelZone *)(off + fieldBase);
                            {
                                int num = *(const u16 *)(((*(u32 *)&z->card << 21) >> 20) + 0x08622AB4);
                                switch (num) {
                                case 0x2FA:
                                    if (ZoneFace(savedMask, z) == 0 && freeZones) {
                                        QueueFlipSummon(1, zone);
                                        AI->f6++;
                                        return 0;
                                    }
                                    break;
                                case 0x1F4:
                                    if (ZoneFace(savedMask, z) == 0 && CountMonsters(0) > 0 && freeZones) {
                                        QueueFlipSummon(1, AI->f6);
                                        AI->f6++;
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
                        register u32 index asm("r2") = cur->f6;
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
                            if (HasFlipEffect(*(const u16 *)(((*(u32 *)&z->card << 21) >> 20) + 0x08622AB4), 0x08622AB4) == 0 && ZoneSpaceCount(freeZones)) {
                                QueueFlipSummon(1, cur->f6);
                                cur->f6++;
                                return 0;
                            }
                        }
                        {
                            u32 index = AI->f6;
                            u32 off;
                            off = index;
                            off *= 0x94;
                            off += (u32)gDuelZonesP1;
                            z = (struct DuelZone *)off;
                            if ((z->flags6 & 3) == 3) {
                                ChangeBattlePosition(1, index, 0, 0);
                                AI->f6++;
                                return 0;
                            }
                        }
                    } else {
                        struct AiState *cur = AI;
                        /* FAKEMATCH: initialize stride before loading the index. */
                        register u32 stride asm("r0") = 0x94;
                        u32 off;
                        struct DuelZone *z;
                        off = cur->f6;
                        off *= stride;
                        z = (struct DuelZone *)(off + (u32)gDuelZonesP1);
                        if ((z->flags6 & 3) == 2 && (gDuelCtrl.flags & 0x200) != 0) {
                            int num = *(const u16 *)(((*(u32 *)&z->card << 21) >> 20) + 0x08622AB4);
                            switch (num) {
                            case 0x23D:
                            case 0x2F:
                            case 0x463:
                                if (ZoneLife() > 0xBB8 && AiCountExodiaInDeck() > 0)
                                    AiCountExodiaInGraveyard();
                                break;
                            }
                        }
                    }
                }
                AI->f6++;
            } while (AI->f6 <= 4);
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
        gDuel.u.w.b = 0;
        gDuel.u.h.b = 0;
    }
    return BattlePhase_Run(1);
}

int AiRunStep(void)
{
    int (*fn)(void) = gAiSteps[gAiState.step];
    struct AiState *st = &gAiState;
    if (fn != 0) {
        if ((u16)fn() != 0) {
            st->f2 = 0;
            st->f3 = 0;
            st->f4 = 0;
            st->f5 = 0;
            st->phase = 0;
            st->f6 = 0;
            st->step++;
        }
        return 0;
    }
    return 1;
}

struct ZoneInfo {
    u8 b0, b1;
    u8 kind;
    u8 b3;
    s32 value;
    u32 w8;
};
struct AiWork2 {
    u8 pad[0x1B24];
    /* Direct byte fields keep target adjacent at +0x1B25. */
    u8 foundFlag : 1;
    u8 strategy : 7;
    u8 target;      /* +0x1B25 zone/hand index of the candidate */
};
extern struct AiWork2 gAiWork;
extern const u16 gCardNumberToId[];
extern const u16 gUnk_08624568[];
extern u32 gDuelHandP1[];
int CountFreeMonsterZones(int);
int AiFindHandCardByNumber(int, int);
int FindFusionDeckCardByNumber(int, int);
int CanSummonFromHand(int, int);
void DebugPrintFlush(void);
int CountFaceUpMonstersByNumber(int, u16);
void GetZoneCardStats(int, int, struct ZoneInfo *);
int IsToonMonster(int);
void AiBackupDuelState(void);
void AiRestoreDuelState(void);
int CountActiveCardsOnField(int, u16);
int CountMonsters(int);

struct BcZone {
    u32 id : 12;
    u32 rest : 20;
    u8 unk4[2];
    u8 f6_0 : 1;
    u8 f6_1 : 1;
    u8 f6_2 : 6;
    u8 pad7[0x91 - 7];
    u8 f91_0 : 3;
    u8 f91_3 : 1;
    u8 f91_4 : 4;
    u8 pad92[2];
};
struct BcF91 {
    u8 b0 : 3;
    u8 b3 : 1;
    u8 b4 : 4;
};
int AiChooseStrategy(void)
{
    int i;
    int ok;
    int sum;
    int k;
    struct ZoneInfo info;
    gAiWork.foundFlag = 0;
    for (i = 0; i <= 8; i++) {
        ok = 1;
        switch (i) {
        case 0:
            if (gDuel.f1B10 == 0)
                ok = 0;
            if (gDuel.fD70 & 0x10)
                ok = 0;
            if (gDuel.lp1 <= 5000)
                ok = 0;
            if (CountFreeMonsterZones(1) <= 1)
                ok = 0;
            if (AiFindHandCardByNumber(1, 0x290) == -1 && gDuel.lp0 > gDuel.lp1)
                ok = 0;
            if (AiFindHandCardByNumber(1, 0x1A3) == -1)
                ok = 0;
            if (FindFusionDeckCardByNumber(1, 0x17B) != -1)
                break;
            continue;
        case 1:
            if (gDuel.f1B10 == 0)
                ok = 0;
            if (CountFreeMonsterZones(1) == 0)
                ok = 0;
            k = 0x34D;
            if (AiFindHandCardByNumber(1, k) == -1)
                ok = 0;
            if (CanSummonFromHand(1, ((const u16 *)0x08623DF4)[k]) == 0)
                ok = 0;
            DebugPrintFlush();
            break;
        case 2:
            if (gDuel.f1B10 == 0)
                ok = 0;
            k = 0x58A;
            if (CountActiveCardsOnField(0, k) > 0)
                ok = 0;
            if (CountActiveCardsOnField(1, k) > 0)
                ok = 0;
            if (CountFaceUpMonstersByNumber(1, 0x1FF) == 0)
                ok = 0;
            if (CountFreeMonsterZones(1) <= 1)
                ok = 0;
            if (AiFindHandCardByNumber(1, 0x4DD) != -1)
                break;
            continue;
        case 3: {
            int c;
            int r;
            if (CountFreeMonsterZones(1) <= 1)
                ok = 0;
            if (AiFindHandCardByNumber(1, 0x13D) == -1)
                ok = 0;
            if (CountFaceUpMonstersByNumber(0, 0x3D) != 0)
                break;
            c = 0x4E1;
            if (CountFaceUpMonstersByNumber(0, c) != 0)
                break;
            if (CountFaceUpMonstersByNumber(1, 0x3D) != 0)
                break;
            if (CountFaceUpMonstersByNumber(1, c) != 0)
                break;
            if (gDuelPlayers.fD6C & 0x10)
                continue;
            r = AiFindHandCardByNumber(1, 0x3D);
            if (r == -1 && AiFindHandCardByNumber(1, c) == r)
                ok = 0;
            if (CountFreeMonsterZones(1) > 2)
                break;
            continue;
        }
        case 6:
            /* Same tail as case 7: jump2 cross-jumps it after reload, so its reloads
             * still advance the reload round-robin (case 4's base then lands in r0). */
            k = 0x5EA;
            if (AiFindHandCardByNumber(1, k) == -1)
                continue;
            if (CanSummonFromHand(1, ((const u16 *)0x08623DF4)[k]) != 0)
                break;
            continue;
        case 4: {
            int j;
            ok = 0;
            if (AiFindHandCardByNumber(1, 0x522) < 0)
                break;
            if (CountMonsters(1) <= 0)
                break;
            for (j = 0; j <= 4; j++) {
                if (((struct DuelZone *)0x0201A070)[j].flags6 & 2) {
                    GetZoneCardStats(1, j, &info);
                    if ((info.kind & 0x1F) == 7)
                        sum += info.value;
                }
            }
            if (gDuelPlayers.lp0 > sum * 2)
                break;
            goto success;
        }
        case 5:
            continue;
        case 7:
            k = 0x5EB;
            if (AiFindHandCardByNumber(1, k) == -1)
                continue;
            if (CanSummonFromHand(1, ((const u16 *)0x08623DF4)[k]) != 0)
                break;
            continue;
        case 8: {
            int n;
            struct DuelZone *zones;
            const u16 *src;
            struct DuelZone *dz;
            struct BcF91 *f;
            ok = 0;
            for (n = 0; n < gDuelPlayers.handCount; n++) {
                u16 id = CARD_ID(*(u32 *)(n * 4 + (u32)gDuelHandP1));
                if (IsToonMonster(((const u16 *)0x08622AB4)[id & 0x7FF]) != 0 && CanSummonFromHand(1, id) != 0)
                    ok = 1;
            }
            if (AiFindHandCardByNumber(1, 0x3BA) < 0)
                break;
            AiBackupDuelState();
            zones = gDuelZonesP1;
            src = gUnk_08624568;
            /* FAKEMATCH: two pointers stepped by hand (flag byte +0x91 first) and a signed
             * pointer compare. The ROM sets them up before the masks, which loop pass 1 hoists,
             * and before the end value, so the loop cannot be an indexed z loop (strength
             * reduction would put its inits after the hoisted masks). */
            f = (struct BcF91 *)&zones[5].unk91;
            dz = &zones[5];
            do {
                u16 cid = CARD_ID(*(u32 *)&dz->card);
                /* FAKEMATCH: two empty insns raise the loop's insn count so loop pass 2 keeps
                 * the 0xFFF and #2 constants inside the loop, as in the ROM. */
                asm volatile("");
                asm volatile("");
                if (cid == 0) {
                    dz->card.id = src[0];
                    ((struct BcZone *)dz)->f6_1 = 1;
                    f->b3 = 0;
                }
                f = (struct BcF91 *)((u8 *)f + 0x94);
                dz++;
            } while ((int)dz <= (int)&zones[9]);
            for (n = 0; n < gDuelPlayers.handCount; n++) {
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
            u8 *w;
        success:
            w = (u8 *)&gAiWork;
            {
                u8 *p = w + 0x1B24;
                int s = i << 1;
                int one = 1;
                /* FAKEMATCH: opaque 1 so the orr output ties to the constant register (ROM: mov r1, #1; orr r1, r2) */
                asm("" : "+r"(one));
                *p = one | s;
            }
            return 1;
        }
    }
    return 0;
}

int CountSpellTrapsFiltered(int, u16, u16, u16);
int AiTryPlaySpellTrap(u16);
void CardMenu_PlaySpellTrapFromHand(int, int, int);
int FindFreeMonsterZone(int);
void DuelCmd_Push(u16, u16, u16, u16);
void Chain_AddPending(u32, int);
void QueueNormalSummon(int, int, int, int, int);

/* Commit hand card `idx` of player 1 into the state block at base->c. */
#define COMMIT_HEAD(base) ((base).c.s8.b = 0)
#define COMMIT_TAIL(base) \
    do { \
        struct AiState *st_ = &gAiState; \
        (base).c.cardId = CARD_ID((base).hand1[st_->f_idx]); \
        (base).c.s8.f3 |= 2; \
        (base).c.sC.idx = st_->f_idx; \
        (base).c.f4.bf.b = 1; \
        st_->f2++; \
    } while (0)

/* FAKEMATCH: initialized address copies preserve the LP load's ADD operands. */
static inline u16 NeighborLife(void)
{
    struct DuelPlayers *base = &gDuelPlayers;
    register u32 baseAddr asm("r5") = (u32)base;
    register u32 off asm("r3") = 0xD64;
    asm("" : "+r"(off));
    {
        register u16 *p asm("r0") = (u16 *)(baseAddr + off);
        return *p;
    }
}

int AiStrategyCyberStein(void)
{
    int j;
    u32 tgt;
    u32 id;

    switch (gAiState.f2) {
    case 0:
        if (CountSpellTrapsFiltered(0, 0, 0, 0) > 0) {
            if (AiTryPlaySpellTrap(0x29F) != 0) {
                gDuel.c.s8.b = 0;
                COMMIT_TAIL(gDuel);
                return 0;
            }
            if (AiTryPlaySpellTrap(0x425) != 0 || AiTryPlaySpellTrap(0x438) != 0)
                goto head2;
        }
        gAiState.f2 += 2;
        return 0;
    case 1:
    case 3:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gDuel.c.f4.raw & 2) == 0)
            gAiState.f2--;
        return 0;
    case 2:
        if (CountMonsters(0) > 0) {
            if (AiTryPlaySpellTrap(0x14F) != 0 || AiTryPlaySpellTrap(0x150) != 0) {
            head2:
                COMMIT_HEAD(gDuel);
                COMMIT_TAIL(gDuel);
                return 0;
            }
        }
        gAiState.f2 += 2;
        return 0;
    case 4:
        if (FindFreeMonsterZone(1) == -1) {
            gAiWork.foundFlag = 0;
            return 0;
        }
        gAiWork.target = FindFreeMonsterZone(1);
        j=0;
        {
          int n = gDuelPlayers.handCount;
          if(j<n) {
            u32 needle=0x1A3;
            int bound=n;
            u32 off=0x13E8;
            u32 *cards=(u32 *)((u32)&gDuelPlayers+off);
            u32 mask=0x7FF;
            const u16 *table=gCardIdToNumber;
            do {
              if (table[CARD_ID(*cards) & mask] == needle)
                goto queue;
              cards++;
              j++;
            } while (j < bound);
          }
        }
        gAiWork.foundFlag = 0;
        return 0;
    case 5: {
        struct AiWork2 *w = &gAiWork;
        u8 *tp = &w->target;
        struct DuelZone *z;

        {
            /* FAKEMATCH: load the target before making its persistent copy. */
            register u32 raw asm("r0") = *tp;
            tgt = raw;
            z = &gDuelZonesP1[raw];
        }
        id = CARD_ID(*(u32 *)&z->card);
        if (id == 0) {
            w->foundFlag = 0;
            return 0;
        }
        if ((z->flags6 & 2) == 0) {
            w->foundFlag = 0;
            return 0;
        }
        if (((const u16 *)0x08622AB4)[id & 0x7FF] != 0x1A3) {
            w->foundFlag = 0;
            return 0;
        }
        DuelCmd_Push(0x8008, 1, tgt << 8, 0);
        tgt = *tp;
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
        gAiState.f2++;
        return 0;
    }
    case 6:
        if (CountFaceUpMonstersByNumber(1, 0x17B) == 0)
            goto fail;
        gAiState.f2++;
        return 0;
    case 7:
        if (gDuelPlayers.lp0 <= 0x13EB || gDuelPlayers.lp0 < NeighborLife()) {
            gAiState.f2 = 8;
            return 0;
        }
        if (AiTryPlaySpellTrap(0x290) == 0)
            goto fail;
        COMMIT_HEAD(gDuelPlayers);
        COMMIT_TAIL(gDuelPlayers);
        return 0;
    fail:
        gAiWork.foundFlag=0;
        return 0;
    case 8:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gDuel.c.f4.raw & 2) == 0) {
            gDuel.u.w.b = 0;
            gDuel.u.h.b = 0;
            gAiState.f2++;
        }
        return 0;
    case 9:
        return BattlePhase_Run(1);
    queue:
        {
            /* FAKEMATCH: initialize the queue base after the hand search. */
            register u8 *work asm("r0") = (u8 *)&gAiWork;
            QueueNormalSummon(1, j, work[0x1B25], 0, 1);
        }
        gAiState.f2++;
        return 0;
    default:
        return 1;
    }
}

