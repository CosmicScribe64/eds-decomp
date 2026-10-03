#include "global.h"

/* CPU duel state machine, part 3: see wiki/functions/code-0805c508.md and code-0805b3f4.md. */
struct AiState {
    u8 f0;
    u8 step;    /* +1 */
    u8 f2, f3, f4, f5;
    u8 f6;
    u8 f7, f8, f9;
    u8 phase;   /* +0xA */
    u8 f_idx;   /* +0xB */
};
extern struct AiState gAiState;
#define S gAiState

struct AiWork2 {
    u8 pad[0x1B24];
    /* Direct byte fields keep target at +0x1B25 under old_agbcc. */
    u8 foundFlag : 1;
    u8 strategy : 7;
    u8 target;
};
extern struct AiWork2 gAiWork;

/* State block written when the AI commits to a hand card (at 0x0201AE08..0x0201AE18). */
struct CommitBlk {
    u16 cardId;                                             /* +0x00 */
    u8 pad2[2];
    union { u8 raw; struct { u8 a : 1; u8 b : 1; u8 c : 6; } bf; } f4;   /* +0x04 bit 1 */
    struct { u16 a : 2; u16 b : 8; u16 c : 6; u8 pad; u8 f3; } s8;      /* +0x08 */
    struct { u16 a : 1; u16 idx : 8; u16 c : 7; u16 pad; } sC;         /* +0x0C */
};
/* Fields of the duel globals at 0x020192E0 that the AI state machine uses. */
struct DuelGlobals {
    u8 pad0[0x13EC];
    u32 hand1[80];          /* +0x13EC player 1 hand (card words, id = low 12 bits) */
    u8 pad1[0x1B14 - 0x152C];
    union {
        struct { u32 lo:9, first:8, hi:15; } w;
        struct { u16 skip; u16 lo:1, second:8, hi:7; } h;
    } counters;
    u8 padCounters[0x1B28 - 0x1B18];
    struct CommitBlk c;
};
extern struct DuelGlobals gDuel;
/* Same block seen through the player-array symbol at 0x020192E4. */
struct DuelGlobals4 {
    u8 pad0[0xD66];
    u8 handCount;           /* +0xD66 player 1 hand size */
    u8 padD67[0xD6C - 0xD67];
    u8 fD6C;                /* +0xD6C flag bits (bit 4) */
    u8 pad1[0x13E8 - 0xD6D];
    u32 hand1[80];          /* +0x13E8 player 1 hand */
};
extern struct DuelGlobals4 gDuelPlayers;
struct DuelZone {
    u32 card;               /* id = low 12 bits */
    u8 rest[0x94 - 4];
};
extern struct DuelZone gDuelZonesP1[];
extern const u16 gCardIdToNumber[];
extern const u32 gCardStats[];
extern u32 gDuelHandP1[];
int CountFaceUpMonstersByNumber(int, u16);
int CountActiveCardsOnField(int, u16);
int FindFaceUpMonsterByNumber(int, u16);
void DuelCmd_Push(u16, u16, u16, u16);
void Chain_AddPending(u32, int);
#define CARD_ID(w) (((w) << 20) >> 20)
void CardMenu_PlaySpellTrapFromHand(int, int, int);
int AiTryPlaySpellTrap(u16);

int AiStrategyCyberStein(void);
int AiStrategyValkyrion(void);
int AiStrategyFourTokensCannonSoldier(void);
int AiStrategyElegantEgotist(void);
int AiStrategyDoubleMachineAtk(void);
int AiStrategyNone(void);
int AiStrategyBanishThreeSummon(void);
int AiStrategyBanishTwoSummon(void);
int AiStrategyToonWorld(void);


int CountSpellTrapsFiltered(int player, u16 a, u16 b, u16 c);
int CountMonsters(int player);
int CanSummonFromHand(int player, u16 id);
void TributeMonster(int player, int zone);
void DiscardHandCard(int player, int index, u16 a, u16 b);
void QueueSpecialSummonFromHand(int player, int index, int zone, int tribute, int faceUp);
int FindFreeMonsterZone(int player);
extern const u16 gUnk_0862448E[];
/* Use one literal-address view in both searches so their pool entry is shared. */
static inline u16 StrategyCardNumber(u32 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}
#define STRATEGY_COMMIT() do { \
    struct AiState *st = &S; \
    gDuel.c.cardId = CARD_ID(gDuel.hand1[st->f_idx]); \
    gDuel.c.s8.f3 |= 2; \
    gDuel.c.sC.idx = st->f_idx; \
    gDuel.c.f4.bf.b = 1; \
    st->f2++; \
} while (0)

int AiStrategyValkyrion(void)
{
    int i, j;
    /* FAKEMATCH: each i=0..2 case initializes this callee-saved search key. */
    register int number asm("r6");
    int found;
    u32 target;
    switch (S.f2) {
    case 0:
        if (CountSpellTrapsFiltered(0, 0, 0, 0) > 0) {
            if (AiTryPlaySpellTrap(0x29F)) {
                gDuel.c.s8.b = 0;
                STRATEGY_COMMIT();
                return 0;
            }
            if (AiTryPlaySpellTrap(0x425) || AiTryPlaySpellTrap(0x438))
                goto commit;
        }
        S.f2 += 2;
        return 0;
    case 1:
    case 3:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if (!(gDuel.c.f4.raw & 2)) S.f2--;
        return 0;
    case 2:
        if (CountMonsters(0) > 0) {
            if (AiTryPlaySpellTrap(0x14F) || AiTryPlaySpellTrap(0x150)) goto commit;
        }
        S.f2 += 2;
        return 0;
    case 4:
        if (!CanSummonFromHand(1, gUnk_0862448E[0])) {
            {
                struct FlagByte { u8 found : 1, rest : 7; };
                /* FAKEMATCH: initialized address terms retain this clear prefix. */
                register u8 *base asm("r1") = (u8 *)&gAiWork;
                register u32 off asm("r2") = 0x1B24;
                asm("" : : "r"(base), "r"(off));
                {
                    struct FlagByte *p = (struct FlagByte *)((u32)base + off);
                    p->found = 0;
                }
            }
            return 0;
        }
        for (i=0;i<3;i++) {
            found=0;
            switch(i) {
            case 0: number=0x2E1; break;
            case 1: number=0x2F4; break;
            case 2: number=0x320; break;
            }
            for(j=0;j<5 && !found;j++) {
                u32 id=CARD_ID(*(u32 *)(j*0x94+(u32)gDuelZonesP1));
                if(id && StrategyCardNumber(id)==number) {
                    TributeMonster(1, j);
                    found=1;
                }
            }
            for(j=0;j<gDuelPlayers.handCount && !found;j++) {
                u32 id=CARD_ID(*(u32 *)(j*4+(u32)gDuelHandP1));
                if(id && StrategyCardNumber(id)==number) {
                    DiscardHandCard(1, j, 0, 0);
                    found=1;
                }
            }
        }
        {
            int target = FindFreeMonsterZone(1);
            u8 *work = (u8 *)&gAiWork;
            work[0x1B25] = target;
        }
        i=0;
        {
          u8 *players = (u8 *)&gDuelPlayers;
          u32 countOffset = 0xD66;
          int n = *(u8 *)((u32)players + countOffset);
          if(i<n) {
            u32 needle=0x34D;
            int bound;
            u32 off=0x13E8;
            u32 *cards;
            u32 mask;
            const u16 *table;
            /* FAKEMATCH: retain the ROM's second hand-count read. */
            asm("" : "+r"(players));
            bound=*(u8 *)((u32)players+countOffset);
            cards=(u32 *)((u32)players+off);
            mask=0x7FF; table=(const u16 *)0x08622AB4;
            do {
              if(table[CARD_ID(*cards)&mask]==needle)goto queue;
              cards++; i++;
            }while(i<bound);
          }
        }
        gAiWork.foundFlag=0;
        return 0;
    case 5:
        DuelCmd_Push(0x8008, 1, gAiWork.target << 8, 0);
        target=gAiWork.target;
        {
          /* FAKEMATCH: initialized packing temporaries preserve the OR order. */
          register u32 mask asm("r1") = 0x1F;
          register u32 hi asm("r0") = target & mask;
          hi <<= 16;
          {
          u32 lo=CARD_ID(*(u32 *)(target*0x94+(u32)gDuelZonesP1));
          lo |= 0x80400000;
          
          hi |= lo;
          Chain_AddPending(hi, 0);
          }
        }
        goto next;
    case 6:
        if(AiTryPlaySpellTrap(0x488) || AiTryPlaySpellTrap(0x3F0)) {
        commit:
            gDuel.c.s8.b=0;
            STRATEGY_COMMIT();
            return 0;
        }
        gAiWork.foundFlag=0;
        return 0;
    case 7:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if (!(gDuel.c.f4.raw & 2)) {
            gDuel.counters.w.first=0;
            gDuel.counters.h.second=0;
            S.f2++;
        }
        return 0;
    case 8:
        gAiWork.foundFlag=0;
        return 0;
    queue:
        {
            /* FAKEMATCH: initialize the work base here, after the hand search. */
            register u8 *work asm("r0") = (u8 *)&gAiWork;
            QueueSpecialSummonFromHand(1, i, work[0x1B25], 0, 1);
        }
    next:
        S.f2++;
        return 0;
    default:
        return 1;
    }
}
#undef STRATEGY_COMMIT

/* Strategy 2: commit 0x4DD, wait, then find a zone numbered 0x780..0x7CF. */
int AiStrategyFourTokensCannonSoldier(void)
{
    struct AiState *st = &S;
    int k;
    int r;
    /* FAKEMATCH: initialized before the zone scan, retained across found calls. */
    register struct DuelZone *base asm("r5");

    switch (st->f2) {
    case 0:
        if (CountFaceUpMonstersByNumber(1, 0x1FF) == 0)
            goto fail;
        if (AiTryPlaySpellTrap(0x4DD) != 0) {
            {
                struct DuelGlobals *duel = &gDuel;
                {
                    /* FAKEMATCH: initialized address terms preserve ADD order. */
                    register u32 baseAddr asm("r2") = (u32)duel;
                    register u32 off asm("r3") = 0x1B30;
                    asm("" : "+r"(off));
                    {
                        register u16 *p asm("r1") = (u16 *)(baseAddr + off);
                        u32 mask = 0xFFFFFC03;
                        register u32 old asm("r6");
                        old = *p;
                        asm("" : : "r"(old));
                        *p = mask & old;
                    }
                }
                duel->c.cardId = CARD_ID(duel->hand1[st->f_idx]);
                duel->c.s8.f3 |= 2;
                duel->c.sC.idx = st->f_idx;
                duel->c.f4.bf.b = 1;
            }
            goto inc;
        }
        gAiWork.foundFlag = 0;
        return 0;
    case 1:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gDuel.c.f4.raw & 2) == 0) {
        inc:
            st->f2++;
        }
        return 0;
    case 2:
        k = 0x58A;
        if (CountActiveCardsOnField(0, k) > 0)
            goto fail;
        if (CountActiveCardsOnField(1, k) > 0) {
            {
                /* FAKEMATCH: preserve this distinct flag-clear prefix. */
                register u8 *base asm("r1") = (u8 *)&gAiWork;
                register u32 off asm("r3") = 0x1B24;
                asm("" : : "r"(base), "r"(off));
                {
                    u8 *p = (u8 *)((u32)base + off);
                    int mask = ~1;
                    register u32 old asm("r6");
                    old = *p;
                    asm("" : : "r"(old));
                    *p = mask & old;
                }
            }
            return 0;
        }
        if (CountFaceUpMonstersByNumber(1, 0x1FF) == 0) {
        fail:
            gAiWork.foundFlag = 0;
            return 0;
        }
        base = gDuelZonesP1;
        {
            u32 mask = 0x7FF;
            struct DuelZone *zone = base;
            u32 span = 0x94 * 4;
            /* FAKEMATCH: initialized end pointer and range offset keep scratch registers. */
            register struct DuelZone *end asm("r3") = (struct DuelZone *)((u32)base + span);
            const u16 *table = gCardIdToNumber;

            do {
                u32 id = CARD_ID(zone->card);
                if (id) {
                    u32 n = table[id & mask];
                    register int off asm("r6");
                    off = -0x780;
                    n += off;
                    if ((u16)n <= 0x4F)
                        goto found;
                }
                zone++;
            } while ((s32)zone <= (s32)end);
        }
        {
            u32 zero = 0;
            S.f2 = zero;
        }
        return 0;
    found:
        r = FindFaceUpMonsterByNumber(1, 0x1FF);
        DuelCmd_Push(0x8008, 1, ((u32)r << 24) >> 16, 0);
        {
            u32 hi = (r & 0x1F) << 16;
            u32 lo = CARD_ID(base[r].card);
            lo |= 0x80400000;
            Chain_AddPending(hi | lo, 0);
        }
        return 0;
    default:
        return 1;
    }
}

int GetFaceUpFieldMagicNumber(void);
void QueueNormalSummon(int, int, int, u16, u16);

/* Strategy using card 0x13D / 0x4E1 / 0x3D / 0x468: picks a hand card, commits it and queues the play. */
int AiStrategyElegantEgotist(void)
{
    int j;
    struct AiState *st;
    u8 *targetPtr;
    struct AiWork2 *work;

    switch (S.f2) {
    case 0:
        if (AiTryPlaySpellTrap(0x13D) != 0)
            goto inc;
        if ((gDuelPlayers.fD6C & 0x10) != 0)
            goto fail;
        if (FindFreeMonsterZone(1) == -1) {
            {
                struct FlagByte { u8 found : 1, rest : 7; };
                /* FAKEMATCH: initialized address terms preserve this clear prefix. */
                register u8 *base asm("r1") = (u8 *)&gAiWork;
                register u32 off asm("r6") = 0x1B24;
                asm("" : : "r"(base), "r"(off));
                {
                    struct FlagByte *p = (struct FlagByte *)((u32)base + off);
                    p->found = 0;
                }
            }
            return 0;
        }
        gAiWork.target = FindFreeMonsterZone(1);
        j=0;
        {
          u32 offset = 0xD66;
          int n = *(u8 *)((u32)&gDuelPlayers + offset);
          if(j<n) {
            /* FAKEMATCH: initialized search key in the original saved register. */
            register u32 needle asm("r6")=0x4E1;
            int bound=n;
            u32 off=0x13E8;
            u32 *cards=(u32 *)((u32)&gDuelPlayers+off);
            u32 mask=0x7FF;
            const u16 *table=gCardIdToNumber;
            do {
              if (table[CARD_ID(*cards) & mask] == needle)
                goto queue1;
              cards++;
              j++;
            } while (j < bound);
          }
        }
        j=0;
        {
          u8 *players = (u8 *)&gDuelPlayers;
          u32 offset = 0xD66;
          u8 *count = (u8 *)((u32)players + offset);
          int n;
          work=&gAiWork;
          n=*count;
          if(j<n) {
            int bound;
            u32 off=0x13E8;
            u32 *cards;
            u32 mask;
            const u16 *table;
            /* FAKEMATCH: retain the second scan's count reload. */
            asm("" : "+r"(players));
            bound=*(u8 *)((u32)players+offset);
            cards=(u32 *)((u32)players+off);
            mask=0x7FF; table=gCardIdToNumber;
            do {
              if (table[CARD_ID(*cards) & mask] == 0x3D)
                goto queue2;
              cards++;
              j++;
            } while (j < bound);
          }
        }
        work->foundFlag=0;
        return 0;
    case 1:
        if (AiTryPlaySpellTrap(0x13D) == 0)
            goto fail;
        gDuel.c.s8.b = 0;
        st = &S;
        gDuel.c.cardId = CARD_ID(gDuel.hand1[st->f_idx]);
        gDuel.c.s8.f3 |= 2;
        gDuel.c.sC.idx = st->f_idx;
        gDuel.c.f4.bf.b = 1;
        st->f2++;
        return 0;
    fail:
        gAiWork.foundFlag=0;
        return 0;
    case 2:
    case 4:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gDuel.c.f4.raw & 2) == 0)
            S.f2++;
        return 0;
    case 3:
        if (GetFaceUpFieldMagicNumber() == 0x468)
            goto reset;
        if (AiTryPlaySpellTrap(0x468) == 0)
            goto reset;
        gDuel.c.s8.b = 0;
        st = &S;
        gDuel.c.cardId = CARD_ID(gDuel.hand1[st->f_idx]);
        gDuel.c.s8.f3 |= 2;
        gDuel.c.sC.idx = st->f_idx;
        gDuel.c.f4.bf.b = 1;
        st->f2++;
        return 0;
    case 5:
    reset:
        S.f2 = 0;
        return 0;
    queue1:
        {
            /* FAKEMATCH: initialized base is loaded after the first scan. */
            register u8 *first asm("r0") = (u8 *)&gAiWork;
            targetPtr = first + 0x1B25;
        }
        goto send;
    queue2:
        targetPtr=(u8 *)work+0x1B25;
    send:
        QueueNormalSummon(1, j, *targetPtr, 0, 1);
    inc:
        S.f2++;
        return 0;
    default:
        return 1;
    }
}

/* Sub-step machine for the strategy using card 0x522: commits the chosen hand card, then waits for the commit flag. */
int AiStrategyDoubleMachineAtk(void)
{
    struct AiState *st = &S;

    switch (st->f2) {
    case 0:
        if (AiTryPlaySpellTrap(0x522) != 0) {
            gDuel.c.s8.b = 0;
            gDuel.c.cardId = CARD_ID(gDuel.hand1[st->f_idx]);
            gDuel.c.s8.f3 |= 2;
            gDuel.c.sC.idx = st->f_idx;
            gDuel.c.f4.bf.b = 1;
            st->f2++;
            return 0;
        }
        gAiWork.foundFlag = 0;
        return 0;
    case 1:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gDuel.c.f4.raw & 2) == 0)
            st->f2++;
        return 0;
    case 2:
        gAiWork.foundFlag = 0;
        return 0;
    default:
        return 1;
    }
}
/* Value of a card for the "sacrifice the weakest" choice: 0 for type 0x15-0x17, 4000 for 0x18, else 10 * a 9-bit stats field. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
static inline int CardValue(u32 id)
{
    int r;

    switch ((int)((CARD_STATS(id) & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        r = ((CARD_STATS(id) << 14) >> 23) * 10;
        break;
    }
    return r;
}
extern u32 gCardListViewCards[];
int CollectEffectTargets(int, int, int);
void BanishGraveyardCard(int, u32 *);
void QueueSpecialSummonFromHand(int, int, int, int, int);
int FindFreeMonsterZone(int);

/* FAKEMATCH: count in r6 and candidate base in r8 steer old_agbcc allocation.
 * The initialized r3 mask input stages the loop-invariant 0x7FF before the base;
 * it emits no instruction and does not change the card-value calculation. */
int AiStrategyBanishThreeSummon(void)
{
    register int count __asm__("r6");
    int best;
    int bestValue;
    int i;
    u32 id;

    switch (S.f2) {
    case 0:
        S.f3 = 3;
        S.f2++;
    case 1:
        count = CollectEffectTargets(1, 0x5EA, 0);
        best = -1;
        bestValue = 9999;
        i = 0;
        if (i < count) {
            register u32 loadMask __asm__("r3") = 0x7FF;
            register u32 *cards __asm__("r8");
            __asm__ volatile("" : : "r"(loadMask));
            cards = gCardListViewCards;
            do {
                id = CARD_ID(cards[i]);
                if (bestValue > CardValue(id)) {
                    bestValue = CardValue(id);
                    best = i;
                }
                i++;
            } while (i < count);
        }
        if (best < 0)
            goto fail;
        BanishGraveyardCard(1, &gCardListViewCards[best]);
        if (--S.f3 == 0)
            S.f2++;
        return 0;
    case 2:
        i = 0;
        {
            u32 *cards;
            int n = gDuelPlayers.handCount;
            if (i < n) {
                u32 needle = 0x5EA;
                int bound = n;
                u32 offset = 0x13E8;
                u32 mask;
                const u16 *table;
                cards = (u32 *)((u32)&gDuelPlayers + offset);
                mask = 0x7FF;
                table = gCardIdToNumber;
                do {
                    if (table[CARD_ID(*cards) & mask] == needle) {
                        QueueSpecialSummonFromHand(1, i, FindFreeMonsterZone(1), 0, 1);
                        goto fail;
                    }
                    cards++;
                    i++;
                } while (i < bound);
            }
        }
        gAiWork.foundFlag = 0;
        return 0;
    fail:
        gAiWork.foundFlag = 0;
        return 0;
    default:
        return 1;
    }
}

/* FAKEMATCH: count in r6 and candidate base in r8 steer old_agbcc allocation.
 * The initialized r3 mask input stages the loop-invariant 0x7FF before the base;
 * it emits no instruction and does not change the card-value calculation. */
int AiStrategyBanishTwoSummon(void)
{
    register int count __asm__("r6");
    int best;
    int bestValue;
    int i;
    u32 id;

    switch (S.f2) {
    case 0:
        S.f3 = 2;
        S.f2++;
    case 1:
        count = CollectEffectTargets(1, 0x5EB, 0);
        best = -1;
        bestValue = 9999;
        i = 0;
        if (i < count) {
            register u32 loadMask __asm__("r3") = 0x7FF;
            register u32 *cards __asm__("r8");
            __asm__ volatile("" : : "r"(loadMask));
            cards = gCardListViewCards;
            do {
                id = CARD_ID(cards[i]);
                if (bestValue > CardValue(id)) {
                    bestValue = CardValue(id);
                    best = i;
                }
                i++;
            } while (i < count);
        }
        if (best < 0)
            goto fail;
        BanishGraveyardCard(1, &gCardListViewCards[best]);
        if (--S.f3 == 0)
            S.f2++;
        return 0;
    case 2:
        i = 0;
        {
            u32 *cards;
            int n = gDuelPlayers.handCount;
            if (i < n) {
                u32 needle = 0x5EB;
                int bound = n;
                u32 offset = 0x13E8;
                u32 mask;
                const u16 *table;
                cards = (u32 *)((u32)&gDuelPlayers + offset);
                mask = 0x7FF;
                table = gCardIdToNumber;
                do {
                    if (table[CARD_ID(*cards) & mask] == needle) {
                        QueueSpecialSummonFromHand(1, i, FindFreeMonsterZone(1), 0, 1);
                        goto fail;
                    }
                    cards++;
                    i++;
                } while (i < bound);
            }
        }
        gAiWork.foundFlag = 0;
        return 0;
    fail:
        gAiWork.foundFlag = 0;
        return 0;
    default:
        return 1;
    }
}

int HasFaceUpToonWorld(int);
u32 IsToonMonster(u16);
int CanSummonFromHand(int, u16);
int AiPickTributeMonster(int, u16);

/* FAKEMATCH: initialized address terms retain the loop-tail count load. */
static inline u8 StrategyHandCount(void)
{
    register u8 *base asm("r0") = (u8 *)&gDuelPlayers;
    register u32 off asm("r2") = 0xD66;
    asm("" : : "r"(base), "r"(off));
    return *(u8 *)((u32)base + off);
}

/* Strategy 7: commit 0x3BA, then queue cards 0x2D7 / 0x2D8 / 0x2FE. */
int AiStrategyToonWorld(void)
{
    struct AiState *st = &S;
    int j;
    int a;
    int b;
    u32 id;
    const u16 *p;

    switch (st->f2) {
    case 0:
        if (HasFaceUpToonWorld(1) != 0) {
            st->f2 = 2;
            return 0;
        }
        if (AiTryPlaySpellTrap(0x3BA) != 0) {
            gDuel.c.s8.b = 0;
            gDuel.c.cardId = CARD_ID(gDuel.hand1[st->f_idx]);
            gDuel.c.s8.f3 |= 2;
            gDuel.c.sC.idx = st->f_idx;
            gDuel.c.f4.bf.b = 1;
            goto inc;
        }
        gAiWork.foundFlag = 0;
        return 0;
    case 1:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gDuel.c.f4.raw & 2) == 0) {
        inc:
            st->f2++;
        }
        return 0;
    case 2:
        if (HasFaceUpToonWorld(1) != 0)
            goto inc;
        gAiWork.foundFlag = 0;
        return 0;
    case 3:
        j = 0;
        if (j < gDuelPlayers.handCount) {
            do {
                {
                    /* FAKEMATCH: initialized raw word stays in the load scratch. */
                    register u32 raw asm("r0") = *(u32 *)(j * 4 + (u32)gDuelHandP1);
                    raw <<= 20;
                    id = raw >> 20;
                }
                {
                    /* FAKEMATCH: retain the initialized mask copy before scaling id. */
                    register u32 mask asm("r0") = 0x7FF;
                    asm("" : : "r"(mask));
                    {
                        register u32 saved asm("r1") = mask;
                        register u32 off asm("r0");
                        asm("" : "+r"(saved));
                        off = id;
                        off &= saved;
                        off <<= 1;
                        off += (u32)gCardIdToNumber;
                        p = (const u16 *)off;
                    }
                }
                if (IsToonMonster(*p) == 0)
                    continue;
                {
                    /* FAKEMATCH: initialize the first argument before copying id. */
                    register int player asm("r0") = 1;
                    if (CanSummonFromHand(player, id) == 0)
                        continue;
                }
                switch (*p) {
                case 0x2D7:
                    QueueSpecialSummonFromHand(1, j, FindFreeMonsterZone(1), 0, 1);
                    S.f2--;
                    return 0;
                case 0x2D8:
                    a = AiPickTributeMonster(-1, 0);
                    if (a == -1)
                        break;
                    {
                        u32 packed = 0x90;
                        packed |= a;
                        QueueSpecialSummonFromHand(1, j, a, packed, 1);
                    }
                    S.f2--;
                    return 0;
                case 0x2FE:
                    a = AiPickTributeMonster(-1, 0);
                    b = AiPickTributeMonster(a, 0);
                    if (a == -1 || b == -1)
                        break;
                    {
                        /* FAKEMATCH: initialized tag and its copy keep the byte-pack order. */
                        register int t asm("r2") = -0x70;
                        asm("" : : "r"(t));
                        {
                            register u32 mask asm("r0") = t;
                            u32 lo = (u32)(mask | a) << 24;
                            u32 hi = (u32)(mask | b) << 24;
                            lo >>= 8;
                            QueueSpecialSummonFromHand(1, j, a, (lo | hi) >> 16, 1);
                        }
                    }
                    S.f2--;
                    return 0;
                }
            } while (++j < StrategyHandCount());
        }
        {
            /* FAKEMATCH: preserve the distinct exhausted-hand flag-clear prefix. */
            register u8 *base asm("r1") = (u8 *)&gAiWork;
            register u32 off asm("r3") = 0x1B24;
            asm("" : : "r"(base), "r"(off));
            {
                u8 *p = (u8 *)((u32)base + off);
                int mask = ~1;
                u32 old;
                old = *p;
                *p = mask & old;
            }
        }
        return 0;
    default:
        return 0;
    }
}

/* Clears the "candidate found" flag. */
int AiStrategyNone(void)
{
    gAiWork.foundFlag = 0;
    return 0;
}

/* Dispatches to the handler of the strategy chosen by AiChooseStrategy (index in 0x02017A24 bits 1+). */
int AiStepRunStrategy(void)
{
    u16 r = 1;

    switch (gAiWork.strategy) {
    case 0:
        r = AiStrategyCyberStein();
        break;
    case 1:
        r = AiStrategyValkyrion();
        break;
    case 2:
        r = AiStrategyFourTokensCannonSoldier();
        break;
    case 3:
        r = AiStrategyElegantEgotist();
        break;
    case 4:
        r = AiStrategyDoubleMachineAtk();
        break;
    case 5:
        r = AiStrategyNone();
        break;
    case 6:
        r = AiStrategyBanishThreeSummon();
        break;
    case 7:
        r = AiStrategyBanishTwoSummon();
        break;
    case 8:
        r = AiStrategyToonWorld();
        break;
    }
    if (gAiWork.foundFlag == 0) {
        S.step = 1;
        S.f2 = 0;
        S.f3 = 0;
        S.f4 = 0;
        S.f5 = 0;
        return 0;
    }
    return r;
}
