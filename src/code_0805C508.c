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
extern struct AiState gUnk_02015EF0;
#define S gUnk_02015EF0

struct AiWork2 {
    u8 pad[0x1B24];
    /* Direct byte fields keep target at +0x1B25 under old_agbcc. */
    u8 foundFlag : 1;
    u8 strategy : 7;
    u8 target;
};
extern struct AiWork2 gUnk_02015F00;

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
extern struct DuelGlobals gUnk_020192E0;
/* Same block seen through the player-array symbol at 0x020192E4. */
struct DuelGlobals4 {
    u8 pad0[0xD66];
    u8 handCount;           /* +0xD66 player 1 hand size */
    u8 padD67[0xD6C - 0xD67];
    u8 fD6C;                /* +0xD6C flag bits (bit 4) */
    u8 pad1[0x13E8 - 0xD6D];
    u32 hand1[80];          /* +0x13E8 player 1 hand */
};
extern struct DuelGlobals4 gUnk_020192E4;
struct DuelZone {
    u32 card;               /* id = low 12 bits */
    u8 rest[0x94 - 4];
};
extern struct DuelZone gUnk_0201A070[];
extern const u16 gUnk_08622AB4[];
extern const u32 gUnk_08621DE0[];
extern u32 gUnk_0201A6CC[];
int sub_080086CC(int, u16);
int sub_08008524(int, u16);
int sub_08008730(int, u16);
void sub_0801EC58(u16, u16, u16, u16);
void sub_0801FBCC(u32, int);
#define CARD_ID(w) (((w) << 20) >> 20)
void sub_08049048(int, int, int);
int sub_08059408(u16);

int sub_0805C0A0(void);
int sub_0805C508(void);
int sub_0805C938(void);
int sub_0805CB2C(void);
int sub_0805CDA4(void);
int sub_0805D4B4(void);
int sub_0805CEAC(void);
int sub_0805D080(void);
int sub_0805D254(void);


int sub_08008B70(int player, u16 a, u16 b, u16 c);
int sub_08008860(int player);
int sub_08054398(int player, u16 id);
void sub_08017FF4(int player, int zone);
void sub_080193D4(int player, int index, u16 a, u16 b);
void sub_080561A0(int player, int index, int zone, int tribute, int faceUp);
int sub_08008A44(int player);
extern const u16 gUnk_0862448E[];
/* Use one literal-address view in both searches so their pool entry is shared. */
static inline u16 StrategyCardNumber(u32 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}
#define STRATEGY_COMMIT() do { \
    struct AiState *st = &S; \
    gUnk_020192E0.c.cardId = CARD_ID(gUnk_020192E0.hand1[st->f_idx]); \
    gUnk_020192E0.c.s8.f3 |= 2; \
    gUnk_020192E0.c.sC.idx = st->f_idx; \
    gUnk_020192E0.c.f4.bf.b = 1; \
    st->f2++; \
} while (0)

int sub_0805C508(void)
{
    int i, j;
    /* FAKEMATCH: each i=0..2 case initializes this callee-saved search key. */
    register int number asm("r6");
    int found;
    u32 target;
    switch (S.f2) {
    case 0:
        if (sub_08008B70(0, 0, 0, 0) > 0) {
            if (sub_08059408(0x29F)) {
                gUnk_020192E0.c.s8.b = 0;
                STRATEGY_COMMIT();
                return 0;
            }
            if (sub_08059408(0x425) || sub_08059408(0x438))
                goto commit;
        }
        S.f2 += 2;
        return 0;
    case 1:
    case 3:
        sub_08049048(1, 0, 0);
        if (!(gUnk_020192E0.c.f4.raw & 2)) S.f2--;
        return 0;
    case 2:
        if (sub_08008860(0) > 0) {
            if (sub_08059408(0x14F) || sub_08059408(0x150)) goto commit;
        }
        S.f2 += 2;
        return 0;
    case 4:
        if (!sub_08054398(1, gUnk_0862448E[0])) {
            {
                struct FlagByte { u8 found : 1, rest : 7; };
                /* FAKEMATCH: initialized address terms retain this clear prefix. */
                register u8 *base asm("r1") = (u8 *)&gUnk_02015F00;
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
                u32 id=CARD_ID(*(u32 *)(j*0x94+(u32)gUnk_0201A070));
                if(id && StrategyCardNumber(id)==number) {
                    sub_08017FF4(1, j);
                    found=1;
                }
            }
            for(j=0;j<gUnk_020192E4.handCount && !found;j++) {
                u32 id=CARD_ID(*(u32 *)(j*4+(u32)gUnk_0201A6CC));
                if(id && StrategyCardNumber(id)==number) {
                    sub_080193D4(1, j, 0, 0);
                    found=1;
                }
            }
        }
        {
            int target = sub_08008A44(1);
            u8 *work = (u8 *)&gUnk_02015F00;
            work[0x1B25] = target;
        }
        i=0;
        {
          u8 *players = (u8 *)&gUnk_020192E4;
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
        gUnk_02015F00.foundFlag=0;
        return 0;
    case 5:
        sub_0801EC58(0x8008, 1, gUnk_02015F00.target << 8, 0);
        target=gUnk_02015F00.target;
        {
          /* FAKEMATCH: initialized packing temporaries preserve the OR order. */
          register u32 mask asm("r1") = 0x1F;
          register u32 hi asm("r0") = target & mask;
          hi <<= 16;
          {
          u32 lo=CARD_ID(*(u32 *)(target*0x94+(u32)gUnk_0201A070));
          lo |= 0x80400000;
          
          hi |= lo;
          sub_0801FBCC(hi, 0);
          }
        }
        goto next;
    case 6:
        if(sub_08059408(0x488) || sub_08059408(0x3F0)) {
        commit:
            gUnk_020192E0.c.s8.b=0;
            STRATEGY_COMMIT();
            return 0;
        }
        gUnk_02015F00.foundFlag=0;
        return 0;
    case 7:
        sub_08049048(1, 0, 0);
        if (!(gUnk_020192E0.c.f4.raw & 2)) {
            gUnk_020192E0.counters.w.first=0;
            gUnk_020192E0.counters.h.second=0;
            S.f2++;
        }
        return 0;
    case 8:
        gUnk_02015F00.foundFlag=0;
        return 0;
    queue:
        {
            /* FAKEMATCH: initialize the work base here, after the hand search. */
            register u8 *work asm("r0") = (u8 *)&gUnk_02015F00;
            sub_080561A0(1, i, work[0x1B25], 0, 1);
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
int sub_0805C938(void)
{
    struct AiState *st = &S;
    int k;
    int r;
    /* FAKEMATCH: initialized before the zone scan, retained across found calls. */
    register struct DuelZone *base asm("r5");

    switch (st->f2) {
    case 0:
        if (sub_080086CC(1, 0x1FF) == 0)
            goto fail;
        if (sub_08059408(0x4DD) != 0) {
            {
                struct DuelGlobals *duel = &gUnk_020192E0;
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
        gUnk_02015F00.foundFlag = 0;
        return 0;
    case 1:
        sub_08049048(1, 0, 0);
        if ((gUnk_020192E0.c.f4.raw & 2) == 0) {
        inc:
            st->f2++;
        }
        return 0;
    case 2:
        k = 0x58A;
        if (sub_08008524(0, k) > 0)
            goto fail;
        if (sub_08008524(1, k) > 0) {
            {
                /* FAKEMATCH: preserve this distinct flag-clear prefix. */
                register u8 *base asm("r1") = (u8 *)&gUnk_02015F00;
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
        if (sub_080086CC(1, 0x1FF) == 0) {
        fail:
            gUnk_02015F00.foundFlag = 0;
            return 0;
        }
        base = gUnk_0201A070;
        {
            u32 mask = 0x7FF;
            struct DuelZone *zone = base;
            u32 span = 0x94 * 4;
            /* FAKEMATCH: initialized end pointer and range offset keep scratch registers. */
            register struct DuelZone *end asm("r3") = (struct DuelZone *)((u32)base + span);
            const u16 *table = gUnk_08622AB4;

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
        r = sub_08008730(1, 0x1FF);
        sub_0801EC58(0x8008, 1, ((u32)r << 24) >> 16, 0);
        {
            u32 hi = (r & 0x1F) << 16;
            u32 lo = CARD_ID(base[r].card);
            lo |= 0x80400000;
            sub_0801FBCC(hi | lo, 0);
        }
        return 0;
    default:
        return 1;
    }
}

int sub_080094E4(void);
void sub_08055B28(int, int, int, u16, u16);

/* Strategy using card 0x13D / 0x4E1 / 0x3D / 0x468: picks a hand card, commits it and queues the play. */
int sub_0805CB2C(void)
{
    int j;
    struct AiState *st;
    u8 *targetPtr;
    struct AiWork2 *work;

    switch (S.f2) {
    case 0:
        if (sub_08059408(0x13D) != 0)
            goto inc;
        if ((gUnk_020192E4.fD6C & 0x10) != 0)
            goto fail;
        if (sub_08008A44(1) == -1) {
            {
                struct FlagByte { u8 found : 1, rest : 7; };
                /* FAKEMATCH: initialized address terms preserve this clear prefix. */
                register u8 *base asm("r1") = (u8 *)&gUnk_02015F00;
                register u32 off asm("r6") = 0x1B24;
                asm("" : : "r"(base), "r"(off));
                {
                    struct FlagByte *p = (struct FlagByte *)((u32)base + off);
                    p->found = 0;
                }
            }
            return 0;
        }
        gUnk_02015F00.target = sub_08008A44(1);
        j=0;
        {
          u32 offset = 0xD66;
          int n = *(u8 *)((u32)&gUnk_020192E4 + offset);
          if(j<n) {
            /* FAKEMATCH: initialized search key in the original saved register. */
            register u32 needle asm("r6")=0x4E1;
            int bound=n;
            u32 off=0x13E8;
            u32 *cards=(u32 *)((u32)&gUnk_020192E4+off);
            u32 mask=0x7FF;
            const u16 *table=gUnk_08622AB4;
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
          u8 *players = (u8 *)&gUnk_020192E4;
          u32 offset = 0xD66;
          u8 *count = (u8 *)((u32)players + offset);
          int n;
          work=&gUnk_02015F00;
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
            mask=0x7FF; table=gUnk_08622AB4;
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
        if (sub_08059408(0x13D) == 0)
            goto fail;
        gUnk_020192E0.c.s8.b = 0;
        st = &S;
        gUnk_020192E0.c.cardId = CARD_ID(gUnk_020192E0.hand1[st->f_idx]);
        gUnk_020192E0.c.s8.f3 |= 2;
        gUnk_020192E0.c.sC.idx = st->f_idx;
        gUnk_020192E0.c.f4.bf.b = 1;
        st->f2++;
        return 0;
    fail:
        gUnk_02015F00.foundFlag=0;
        return 0;
    case 2:
    case 4:
        sub_08049048(1, 0, 0);
        if ((gUnk_020192E0.c.f4.raw & 2) == 0)
            S.f2++;
        return 0;
    case 3:
        if (sub_080094E4() == 0x468)
            goto reset;
        if (sub_08059408(0x468) == 0)
            goto reset;
        gUnk_020192E0.c.s8.b = 0;
        st = &S;
        gUnk_020192E0.c.cardId = CARD_ID(gUnk_020192E0.hand1[st->f_idx]);
        gUnk_020192E0.c.s8.f3 |= 2;
        gUnk_020192E0.c.sC.idx = st->f_idx;
        gUnk_020192E0.c.f4.bf.b = 1;
        st->f2++;
        return 0;
    case 5:
    reset:
        S.f2 = 0;
        return 0;
    queue1:
        {
            /* FAKEMATCH: initialized base is loaded after the first scan. */
            register u8 *first asm("r0") = (u8 *)&gUnk_02015F00;
            targetPtr = first + 0x1B25;
        }
        goto send;
    queue2:
        targetPtr=(u8 *)work+0x1B25;
    send:
        sub_08055B28(1, j, *targetPtr, 0, 1);
    inc:
        S.f2++;
        return 0;
    default:
        return 1;
    }
}

/* Sub-step machine for the strategy using card 0x522: commits the chosen hand card, then waits for the commit flag. */
int sub_0805CDA4(void)
{
    struct AiState *st = &S;

    switch (st->f2) {
    case 0:
        if (sub_08059408(0x522) != 0) {
            gUnk_020192E0.c.s8.b = 0;
            gUnk_020192E0.c.cardId = CARD_ID(gUnk_020192E0.hand1[st->f_idx]);
            gUnk_020192E0.c.s8.f3 |= 2;
            gUnk_020192E0.c.sC.idx = st->f_idx;
            gUnk_020192E0.c.f4.bf.b = 1;
            st->f2++;
            return 0;
        }
        gUnk_02015F00.foundFlag = 0;
        return 0;
    case 1:
        sub_08049048(1, 0, 0);
        if ((gUnk_020192E0.c.f4.raw & 2) == 0)
            st->f2++;
        return 0;
    case 2:
        gUnk_02015F00.foundFlag = 0;
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
extern u32 gUnk_0201D81C[];
int sub_08044224(int, int, int);
void sub_0801965C(int, u32 *);
void sub_080561A0(int, int, int, int, int);
int sub_08008A44(int);

/* FAKEMATCH: count in r6 and candidate base in r8 steer old_agbcc allocation.
 * The initialized r3 mask input stages the loop-invariant 0x7FF before the base;
 * it emits no instruction and does not change the card-value calculation. */
int sub_0805CEAC(void)
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
        count = sub_08044224(1, 0x5EA, 0);
        best = -1;
        bestValue = 9999;
        i = 0;
        if (i < count) {
            register u32 loadMask __asm__("r3") = 0x7FF;
            register u32 *cards __asm__("r8");
            __asm__ volatile("" : : "r"(loadMask));
            cards = gUnk_0201D81C;
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
        sub_0801965C(1, &gUnk_0201D81C[best]);
        if (--S.f3 == 0)
            S.f2++;
        return 0;
    case 2:
        i = 0;
        {
            u32 *cards;
            int n = gUnk_020192E4.handCount;
            if (i < n) {
                u32 needle = 0x5EA;
                int bound = n;
                u32 offset = 0x13E8;
                u32 mask;
                const u16 *table;
                cards = (u32 *)((u32)&gUnk_020192E4 + offset);
                mask = 0x7FF;
                table = gUnk_08622AB4;
                do {
                    if (table[CARD_ID(*cards) & mask] == needle) {
                        sub_080561A0(1, i, sub_08008A44(1), 0, 1);
                        goto fail;
                    }
                    cards++;
                    i++;
                } while (i < bound);
            }
        }
        gUnk_02015F00.foundFlag = 0;
        return 0;
    fail:
        gUnk_02015F00.foundFlag = 0;
        return 0;
    default:
        return 1;
    }
}

/* FAKEMATCH: count in r6 and candidate base in r8 steer old_agbcc allocation.
 * The initialized r3 mask input stages the loop-invariant 0x7FF before the base;
 * it emits no instruction and does not change the card-value calculation. */
int sub_0805D080(void)
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
        count = sub_08044224(1, 0x5EB, 0);
        best = -1;
        bestValue = 9999;
        i = 0;
        if (i < count) {
            register u32 loadMask __asm__("r3") = 0x7FF;
            register u32 *cards __asm__("r8");
            __asm__ volatile("" : : "r"(loadMask));
            cards = gUnk_0201D81C;
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
        sub_0801965C(1, &gUnk_0201D81C[best]);
        if (--S.f3 == 0)
            S.f2++;
        return 0;
    case 2:
        i = 0;
        {
            u32 *cards;
            int n = gUnk_020192E4.handCount;
            if (i < n) {
                u32 needle = 0x5EB;
                int bound = n;
                u32 offset = 0x13E8;
                u32 mask;
                const u16 *table;
                cards = (u32 *)((u32)&gUnk_020192E4 + offset);
                mask = 0x7FF;
                table = gUnk_08622AB4;
                do {
                    if (table[CARD_ID(*cards) & mask] == needle) {
                        sub_080561A0(1, i, sub_08008A44(1), 0, 1);
                        goto fail;
                    }
                    cards++;
                    i++;
                } while (i < bound);
            }
        }
        gUnk_02015F00.foundFlag = 0;
        return 0;
    fail:
        gUnk_02015F00.foundFlag = 0;
        return 0;
    default:
        return 1;
    }
}

int sub_08008668(int);
u32 sub_0800756C(u16);
int sub_08054398(int, u16);
int sub_080563B8(int, u16);

/* FAKEMATCH: initialized address terms retain the loop-tail count load. */
static inline u8 StrategyHandCount(void)
{
    register u8 *base asm("r0") = (u8 *)&gUnk_020192E4;
    register u32 off asm("r2") = 0xD66;
    asm("" : : "r"(base), "r"(off));
    return *(u8 *)((u32)base + off);
}

/* Strategy 7: commit 0x3BA, then queue cards 0x2D7 / 0x2D8 / 0x2FE. */
int sub_0805D254(void)
{
    struct AiState *st = &S;
    int j;
    int a;
    int b;
    u32 id;
    const u16 *p;

    switch (st->f2) {
    case 0:
        if (sub_08008668(1) != 0) {
            st->f2 = 2;
            return 0;
        }
        if (sub_08059408(0x3BA) != 0) {
            gUnk_020192E0.c.s8.b = 0;
            gUnk_020192E0.c.cardId = CARD_ID(gUnk_020192E0.hand1[st->f_idx]);
            gUnk_020192E0.c.s8.f3 |= 2;
            gUnk_020192E0.c.sC.idx = st->f_idx;
            gUnk_020192E0.c.f4.bf.b = 1;
            goto inc;
        }
        gUnk_02015F00.foundFlag = 0;
        return 0;
    case 1:
        sub_08049048(1, 0, 0);
        if ((gUnk_020192E0.c.f4.raw & 2) == 0) {
        inc:
            st->f2++;
        }
        return 0;
    case 2:
        if (sub_08008668(1) != 0)
            goto inc;
        gUnk_02015F00.foundFlag = 0;
        return 0;
    case 3:
        j = 0;
        if (j < gUnk_020192E4.handCount) {
            do {
                {
                    /* FAKEMATCH: initialized raw word stays in the load scratch. */
                    register u32 raw asm("r0") = *(u32 *)(j * 4 + (u32)gUnk_0201A6CC);
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
                        off += (u32)gUnk_08622AB4;
                        p = (const u16 *)off;
                    }
                }
                if (sub_0800756C(*p) == 0)
                    continue;
                {
                    /* FAKEMATCH: initialize the first argument before copying id. */
                    register int player asm("r0") = 1;
                    if (sub_08054398(player, id) == 0)
                        continue;
                }
                switch (*p) {
                case 0x2D7:
                    sub_080561A0(1, j, sub_08008A44(1), 0, 1);
                    S.f2--;
                    return 0;
                case 0x2D8:
                    a = sub_080563B8(-1, 0);
                    if (a == -1)
                        break;
                    {
                        u32 packed = 0x90;
                        packed |= a;
                        sub_080561A0(1, j, a, packed, 1);
                    }
                    S.f2--;
                    return 0;
                case 0x2FE:
                    a = sub_080563B8(-1, 0);
                    b = sub_080563B8(a, 0);
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
                            sub_080561A0(1, j, a, (lo | hi) >> 16, 1);
                        }
                    }
                    S.f2--;
                    return 0;
                }
            } while (++j < StrategyHandCount());
        }
        {
            /* FAKEMATCH: preserve the distinct exhausted-hand flag-clear prefix. */
            register u8 *base asm("r1") = (u8 *)&gUnk_02015F00;
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
int sub_0805D4B4(void)
{
    gUnk_02015F00.foundFlag = 0;
    return 0;
}

/* Dispatches to the handler of the strategy chosen by sub_0805BC24 (index in 0x02017A24 bits 1+). */
int sub_0805D4D0(void)
{
    u16 r = 1;

    switch (gUnk_02015F00.strategy) {
    case 0:
        r = sub_0805C0A0();
        break;
    case 1:
        r = sub_0805C508();
        break;
    case 2:
        r = sub_0805C938();
        break;
    case 3:
        r = sub_0805CB2C();
        break;
    case 4:
        r = sub_0805CDA4();
        break;
    case 5:
        r = sub_0805D4B4();
        break;
    case 6:
        r = sub_0805CEAC();
        break;
    case 7:
        r = sub_0805D080();
        break;
    case 8:
        r = sub_0805D254();
        break;
    }
    if (gUnk_02015F00.foundFlag == 0) {
        S.step = 1;
        S.f2 = 0;
        S.f3 = 0;
        S.f4 = 0;
        S.f5 = 0;
        return 0;
    }
    return r;
}
