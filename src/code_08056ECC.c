#include "global.h"

#include "gba.h"

/*
 * Duel AI helpers (zone evaluation, attack target selection, duel-state save/restore).
 * See wiki/functions/code-08056ecc.md.
 */

/* A card instance word: low 12 bits = card id (0 = none). */
struct DuelCard {
    u32 id : 12;
    u32 unk12 : 1;
    u32 unk13 : 19;
};

/* One field zone, 0x94 bytes, 11 per player starting at player+0x28 (0x0201930C). */
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4[2];
    u8 f6_0 : 1;            /* +0x06 bit 0 = face-up? */
    u8 f6_1 : 1;            /* bit 1 = temporary evaluation flag */
    u8 f6_rest : 6;
    u8 f7_0 : 1;            /* +0x07 */
    u8 f7_1 : 1;
    u8 f7_2 : 1;
    u8 f7_3 : 1;
    u8 f7_rest : 4;
    u8 filler8[2];
    u16 links[32];          /* +0x0A */
    u16 linkKinds[32];      /* +0x4A */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[5];
    u8 unk91;
    u8 filler92[2];
};

/* Per-player duel state, 0xD64 bytes, two of them at 0x020192E4. */
struct DuelPlayer {
    u8 unk0[2];
    u8 handCount;                   /* +0x002 */
    u8 deckCount;                   /* +0x003 */
    u8 count904;                    /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 countB84;                    /* +0x006 */
    u8 unk7[2];
    u8 unk9_0 : 1;
    u8 unk9_1 : 7;
    u8 fillerA[0x1C];
    u16 zoneMask;                   /* +0x026 per-zone bitmask */
    struct DuelZone zones[11];      /* +0x028 */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard list904[80];    /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard listB84[80];    /* +0xB84 */
    u16 arrCC4[80];                 /* +0xCC4 */
};
extern struct DuelPlayer gUnk_020192E4[2];

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gUnk_0201930C[2];
/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); callers pass player & 1. */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
/* ROM tables through integer-constant pointers (the ROM reloads the table address at every use). */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* Same tables through the extern symbols (the address is then a hoistable constant). */
extern const u32 gUnk_08621DE0[];
extern const u16 gUnk_08622AB4[];
#define CARD_STATS_A(id) (gUnk_08621DE0[(id) & 0x7FF])
#define CARD_NUMBER_A(id) (gUnk_08622AB4[(id) & 0x7FF])
#define CARD_TYPE_A(id) ((CARD_STATS_A(id) & 0x1F00000) >> 20)
/* AI value of a card: 0 for Magic/Trap/Ritual (types 0x15-0x17), 4000 for type 0x18, else ATK-like field * 10. */
static inline int CardValue(u16 id)
{
    int r;

    switch ((int)CARD_TYPE(id)) {
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
/* Same with the DEF-like field (bits 0-8) for ordinary monsters. */
static inline int CardDefValue(u16 id)
{
    int r;

    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        r = (CARD_STATS(id) & 0x1FF) * 10;
        break;
    }
    return r;
}
/* AI value of a card: 0 for Magic/Trap/Ritual (types 0x15-0x17), 4000 for type 0x18, else ATK-like field * 10. */
static inline int CardValueA(u16 id)
{
    int r;

    switch ((int)CARD_TYPE_A(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        r = ((CARD_STATS_A(id) << 14) >> 23) * 10;
        break;
    }
    return r;
}
/* Same with the DEF-like field (bits 0-8) for ordinary monsters. */
static inline int CardDefValueA(u16 id)
{
    int r;

    switch ((int)CARD_TYPE_A(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        r = (CARD_STATS_A(id) & 0x1FF) * 10;
        break;
    }
    return r;
}

/* One attack option evaluated by the AI (8 bytes, copied with sub_08075294). */
struct AttackPlan {
    u16 f0 : 1;
    u16 f1 : 1;
    u16 f2 : 1;
    u16 lose : 1;               /* bit 3: target would not be beaten (own value < opponent's) */
    u16 src : 3;                /* bits 4-6: attacking zone */
    u16 dst : 3;                /* bits 7-9: target zone */
    u16 gt : 1;                 /* bit 10 */
    u16 le : 1;                 /* bit 11 */
    u16 rest : 4;
    u16 unk2;                   /* +0x02 */
    s16 diff;                   /* +0x04 */
    u16 unk6;
};
void sub_08075294(void *dest, const void *src, u32 size);

/* AI working area at 0x02015F00: the best attack plan found is at +0xC. */
struct AiWork {
    u8 filler0[0xC];
    struct AttackPlan best;
    u8 filler14[0x1B22 - 0x14];
    u16 handPick;               /* +0x1B22 chosen hand index */
};
extern struct AiWork gUnk_02015F00;
extern struct AttackPlan gUnk_02015F0C;

struct Unk02015EE8 {
    u32 unk0;
    u32 flags;                  /* +0x04 bit 9 = restrict the hand pick to the forced list */
};
extern struct Unk02015EE8 gUnk_02015EE8;
extern u32 gUnk_0201D81C[];     /* hand/list card words (ListView.cards) */
extern const u16 gUnk_0819D2FC[];
int sub_08044224(int player, u16 number, int flag);

/* Output of sub_0800ABC8 (card-in-zone info); [1] and [2] are the two values summed by the AI. */
void sub_0800ABC8(int player, int zone, int *out);
int sub_08076F9C(void);
extern const u16 gUnk_0819D316[];
int sub_0804A3D8(int player, int zone);
void sub_0805797C(int a, int b, struct AttackPlan *out);
int sub_0804A528(int player, int zone, int flag);
u16 sub_08057A80(int a, struct AttackPlan *out);
int sub_0800A78C(int player, int zone, int number);
int sub_080090C8(int player, int number);
int sub_0800C8BC(int player, int zone);
int sub_08007590(u16 number, int flag);
int sub_080573D0(int p, int skip, u16 useAtk, u16 useDef);
int sub_0800C894(int player, int zone);
int sub_0800C8A8(int player, int zone);
int sub_08008860(int player);
u16 sub_08057BAC(int a, int b);

/* gMain (0x03000040): only the byte at +0x4870 is used here. */
struct Main {
    u8 filler0[0x4870];
    u8 unk4870_0 : 1;
    u8 handicap : 5;            /* bits 1-5: 0-10 = attack scaling numerator - 5 */
    u8 unk4870_6 : 2;
};
extern struct Main gUnk_03000040;

#if 0 /* NONMATCHING (score 30): same size/layout; only the scoring init differs: ROM n-test reload uses r0
       * (ours r1) and ROM inits maxAtk with mov sl,r7 (reload inheritance) where ours does mov r1,#0; mov sl,r1. Single
       * loop counter k for all loops needs global-alloc priority below the 1AB mask (7 empty asm volatile in the ABC8
       * loop pad k's live length = FAKEMATCH) */
/* AI: choose which card of the list at gUnk_0201D81C (n entries) to use for card `id`; stores it in gUnk_02015F00.handPick, -1 if none. */
static inline int AtkVal56(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 4000;
    default:
        return ((CARD_STATS(id) & 0x3FE00) >> 9) * 10;
    }
}
static inline int AtkVal56b(int id)
{
    int r;

    switch ((int)CARD_TYPE(id)) {
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
static inline u16 DefVal56(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 4000;
    default:
        return (CARD_STATS(id) & 0x1FF) * 10;
    }
}
#define CNS56(x) (*(const u16 *)(((x) & 0x7FF) * 2 + (u32)gUnk_08622AB4))
int sub_08056ECC(u16 id)
{
    int n;
    int j;
    int k;
    int ok;
    int best, bestIdx, maxAtk, maxDef;
    int buf[3];

    n = sub_08044224(1, CNS56(id), 0);
    gUnk_02015F00.handPick = 0;
    if (n == 0)
        goto fail;
    if (gUnk_02015EE8.flags & 0x200) {
        for (k = 0; k < n; k++) {
            u16 c;
            ok = 0;
            c = CARD_ID(*(gUnk_0201D81C + k));
            switch (CNS56(c)) {
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
                switch (CNS56(id)) {
                case 0x2F:
                case 0x23D:
                case 0x47B:
                    ok = 1;
                    break;
                }
                break;
            case 0x2F:
            case 0x23D: {
                int v = CNS56(id);
                if (v == 0x463)
                    ok = 1;
            }
                break;
            }
            if (ok) {
                gUnk_02015F00.handPick = k;
                return gUnk_02015F00.handPick;
            }
        }
        if (CARD_NUMBER(id) == 0x463)
            goto fail;
    }
    switch (CARD_NUMBER(id)) {
    case 0x1AB:
    case 0x65:
    case 0x443:
        for (k = 0; (u32)k <= 0xC; k++) {
            for (j = 0; j < n; j++) {
                if (CNS56(CARD_ID(gUnk_0201D81C[j])) == gUnk_0819D2FC[k])
                    goto found2;
            }
        }
        goto random;
    }
    if (n <= 0)
        goto fail;
    best = 0;
    bestIdx = -1;
    maxAtk = maxDef = 0;
    for (k = 0; k <= 4; k++) {
        sub_0800ABC8(0, k, buf);
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");
        asm volatile("");

        if (maxAtk < buf[1])
            maxAtk = buf[1];
    }
    for (k = 0; k < n; k++) {
        u16 cid = CARD_ID(gUnk_0201D81C[k]);

        if (best < AtkVal56(cid) + DefVal56(cid)) {
            if (maxAtk <= AtkVal56(cid) || maxDef < AtkVal56(cid) || maxAtk <= DefVal56(cid)) {
                best = AtkVal56(cid) + DefVal56(cid);
                bestIdx = k;
            }
        }
    }
    if (bestIdx <= -1) {
        bestIdx = -1;
        best = 0;
        for (k = 0; k < n; k++) {
            u16 cid = CARD_ID(gUnk_0201D81C[k]);

            if (best < CardValue(cid)) {
                best = CardValue(cid);
                bestIdx = k;
            }
        }
        if (bestIdx < 0)
            goto random;
    }
    gUnk_02015F00.handPick = bestIdx;
    return gUnk_02015F00.handPick;
random:
    gUnk_02015F00.handPick = sub_08076F9C() % n;
    return gUnk_02015F00.handPick;
found2:
    j = gUnk_02015F00.handPick;
    return j;
fail:
    return -1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08056ECC", sub_08056ECC); /* 0x08056ECC size 0x504 */
/* Best (largest) score among the occupied zones of player `p` other than `skip`; -1 if none. */
int sub_080573D0(int p, int skip, u16 useAtk, u16 useDef)
{
    int best = -1;
    int i;
    int buf[3];

    for (i = 0; i <= 4; i++) {
        struct DuelZone *z;
        int score;

        if (i == skip)
            continue;
        z = ZB(p & 1, i);
        if ((CARD_WORD(z->card) << 20) == 0)
            continue;
        score = 0;
        if (z->f6_1) {
            sub_0800ABC8(p, i, buf);
        } else {
            z->f6_1 = 1;
            sub_0800ABC8(p, i, buf);
            z->f6_1 = 0;
        }
        if (useAtk)
            score += buf[1];
        if (useDef)
            score += buf[2];
        if (score > best)
            best = score;
    }
    return best;
}
/* Zone index of player p with the largest score; -1 if none. */
int sub_0805748C(int p, int skip, u16 useAtk, u16 useDef)
{
    int bestIdx = -1;
    int best;
    int i;
    /* FAKEMATCH: initialized parity constraint prevents a different product hoist. */
    register int pl asm("r8");
    int buf[3];
    best = bestIdx;
    i = 0;
    pl = 1;
    pl &= p;
    for (; i <= 4; i++) {
        struct DuelZone *z;
        int score;

        if (i == skip)
            continue;
        z = ZB(pl, i);
        if ((CARD_WORD(z->card) << 20) == 0)
            continue;
        score = 0;
        if (z->f6_1) {
            sub_0800ABC8(p, i, buf);
        } else {
            z->f6_1 = 1;
            sub_0800ABC8(p, i, buf);
            z->f6_1 = 0;
        }
        if (useAtk)
            score += buf[1];
        if (useDef)
            score += buf[2];
        if (score > best) {
            bestIdx = i;
            best = score;
        }
    }
    return bestIdx;
}

/* Zone index (0-4, != skip) of player `p` with the smallest score (below 99999); -1 if none. */
int sub_08057550(int p, int skip, u16 useAtk, u16 useDef)
{
    register int bestIdx asm("r10") = -1;
    register int best asm("r9") = 99999;
    int i = 0;
    register int pl asm("r8");
    int buf[3];
    pl = 1;
    pl &= p;
    for (; i <= 4; i++) {
        struct DuelZone *z;
        int score;

        if (i == skip)
            continue;
        z = ZB(pl, i);
        if ((CARD_WORD(z->card) << 20) == 0)
            continue;
        score = 0;
        if (z->f6_1) {
            sub_0800ABC8(p, i, buf);
        } else {
            z->f6_1 = 1;
            sub_0800ABC8(p, i, buf);
            z->f6_1 = 0;
        }
        if (useAtk)
            score += buf[1];
        if (useDef)
            score += buf[2];
        if (score < best) {
            bestIdx = i;
            best = score;
        }
    }
    return bestIdx;
}
/* Sum of ATK over the five monster zones of `player`. */
int sub_0805761C(int player)
{
    int sum = 0;
    int i;

    for (i = 0; i <= 4; i++)
        sum += sub_0800C894(player, i);
    return sum;
}
/* Can the zone's monster be used (occupied, not flagged, not linked to card 0x15C, no 0x148 restriction)? */
int sub_0805763C(int p, int zone)
{
    int pl = p & 1;
    struct DuelZone *z = ZB(pl, zone);

    if ((CARD_WORD(z->card) << 20) == 0)
        return 0;
    if (z->f7_2)
        return 0;
    if (z->f7_3)
        return 0;
    if (sub_0800A78C(p, zone, 0x15C) != 0)
        return 0;
    if (sub_080090C8(0, 0x148) > 0 || sub_080090C8(1, 0x148) > 0) {
        if (sub_0800C8BC(p, zone) == 1)
            return 0;
    }
    return 1;
}
/* Should the AI play card `id`? (hypothesis: compares its ATK value with the opponent's best monster) */
int sub_080576BC(u16 id, u16 flag)
{
    int num, v;

    if (flag == 0) {
        const u16 *p = &CARD_NUMBER(id);

        if (sub_08007590(*p, 1) != 0)
            return 1;
        if (sub_08007590(*p, 0) != 0)
            return 1;
    }
    num = CARD_NUMBER(id);
    if (num == 0x16D)
        return 1;
    if (num == 0x2DA) {
        if (sub_08008860(0) > 0)
            return 0;
        return 1;
    }
    v = CardValue(id);
    if (v <= sub_080573D0(0, -1, 1, 0))
        return 1;
    v = CardValue(id);
    if ((u32)v > 1000)
        return 0;
    if (gUnk_020192E4[0].deckCount <= 4)
        return 0;
    if (*(u16 *)&gUnk_020192E4[0] > 999)
        return 1;
    return 0;
}
/* Count player 1's deck cards with number 0x10-0x14.
 * FAKEMATCH: initialized register constraints retain the ROM address-add order
 * and per-iteration number-table load. The offset input barrier places the
 * list-offset literal before the pointer addition; it emits no instructions. */
int sub_080577FC(void)
{
    s32 i = 0;
    s32 n = 0;
    register s32 count __asm__("r0") = gUnk_020192E4[1].deckCount;
    if (i < count) {
        s32 bound = count;
        u32 mask;
        register u32 offset __asm__("r6");
        struct DuelCard *p;
        register u32 base __asm__("r1") = (u32)gUnk_020192E4;
        mask = 0x7FF;
        offset = 0x1528;
        __asm__ volatile("" : : "r"(offset));
        base += offset;
        p = (struct DuelCard *)base;
        do {
            u32 id;
            register const u8 *table __asm__("r6");
            s32 number;
            id = CARD_ID(CARD_WORD(*p));
            id &= mask;
            id <<= 1;
            table = (const u8 *)0x08622AB4;
            number = *(const u16 *)(id + (u32)table);
            switch (number) {
            case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: n++; break;
            }
            p++;
            i++;
        } while (i < bound);
    }
    return n;
}

/* Count player 1's +0x904 list cards with number 0x10-0x14.
 * FAKEMATCH: initialized register constraints retain the ROM address-add order
 * and per-iteration number-table load. The offset input barrier places the
 * list-offset literal before the pointer addition; it emits no instructions. */
int sub_08057854(void)
{
    s32 i = 0;
    s32 n = 0;
    s32 count = gUnk_020192E4[1].count904;
    if (i < count) {
        s32 bound = count;
        u32 mask;
        register u32 offset __asm__("r6");
        struct DuelCard *p;
        register u32 base __asm__("r1") = (u32)gUnk_020192E4;
        mask = 0x7FF;
        offset = 0x1668;
        __asm__ volatile("" : : "r"(offset));
        base += offset;
        p = (struct DuelCard *)base;
        do {
            u32 id;
            register const u8 *table __asm__("r6");
            s32 number;
            id = CARD_ID(CARD_WORD(*p));
            id &= mask;
            id <<= 1;
            table = (const u8 *)0x08622AB4;
            number = *(const u16 *)(id + (u32)table);
            switch (number) {
            case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: n++; break;
            }
            p++;
            i++;
        } while (i < bound);
    }
    return n;
}

/* Number of player 1's monster-zone cards whose card number is 0x10-0x14. */
int sub_080578AC(void)
{
    int i = 0;
    int n = 0;

    for (; i <= 4; i++) {
        int t;
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[1].zones[i].card));

        if (id == 0)
            continue;
        t = CARD_NUMBER(id);
        switch (t) {
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
            n++;
            break;
        }
    }
    return n;
}

/* Index of the first hand card (scanning the priority list gUnk_0819D316 of 26 card numbers) or a random hand index. */
int sub_080578F4(void)
{
    u32 k;
    int j;

    for (k = 0; k <= 0x19; k++) {
        for (j = 0; j < gUnk_020192E4[0].handCount; j++) {
            if (CARD_NUMBER(CARD_ID(CARD_WORD(gUnk_020192E4[0].hand[j]))) == gUnk_0819D316[k])
                return j;
        }
    }
    return sub_08076F9C() % gUnk_020192E4[0].handCount;
}
/* Evaluate attacking player 0's zone `b` with player 1's zone `a` (hypothesis): fills `out`. */
void sub_0805797C(int a, int b, struct AttackPlan *out)
{
    int x = sub_0800C894(1, a);
    int y = 0;
    struct DuelZone *z;

    out->f1 = 0;
    out->lose = 0;
    out->src = a;
    out->dst = b;
    out->gt = 0;
    out->le = 0;
    out->unk2 = 0;
    out->diff = 0;
    z = ZB(0, b);
    if (z->f6_0) {
        y = sub_0800C8A8(0, b);
        if (!z->f6_1) {
            if (gUnk_03000040.handicap <= 10) {
                y *= gUnk_03000040.handicap + 5;
                y /= 16;
            }
        }
        if (y < x) {
            out->lose = 1;
            out->le = 1;
        } else if (y > x) {
            out->diff = x - y;
        }
    } else {
        y = sub_0800C894(0, b);
        if (y < x) {
            out->lose = 1;
            out->le = 1;
            out->diff = x - y;
        } else if (y > x) {
            out->gt = 1;
            out->diff = x - y;
        } else {
            out->gt = 1;
            out->le = 1;
        }
    }
}
/* Choose the best player-0 zone to attack with player 1's zone `a`; result in `out`. Returns 1 if a winning option exists. */
u16 sub_08057A80(int a, struct AttackPlan *out)
{
    struct AttackPlan tmp;
    int i;

    if (sub_0804A3D8(1, a) != 0 || sub_08008860(0) == 0) {
        if (CARD_NUMBER(CARD_ID(CARD_WORD(ZB(1, a)->card))) != 0x5F3) {
            out->f2 = 1;
            out->f1 = 1;
            out->diff = sub_0800C894(1, a);
            out->src = a;
            out->gt = 0;
            out->le = 0;
            return 1;
        }
    }
    out->lose = 0;
    out->f1 = 0;
    for (i = 0; i <= 4; i++) {
        if ((CARD_WORD(ZB(0, i)->card) << 20) == 0)
            continue;
        sub_0805797C(a, i, &tmp);
        if (!tmp.lose)
            continue;
        if (!out->lose) {
            sub_08075294(out, &tmp, 8);
        } else if (tmp.diff > out->diff) {
            sub_08075294(out, &tmp, 8);
        } else if (sub_0800C8A8(0, tmp.dst) > sub_0800C8A8(0, out->dst)) {
            sub_08075294(out, &tmp, 8);
        }
    }
    return out->lose;
}
/* Is monster zone `zone` of player 0 (ATK/DEF value, scaled when face-down and above... ) weaker than `value`? */
u16 sub_08057BAC(int value, int zone)
{
    struct DuelZone *z = ZB(0, zone);
    int v;

    if (z->f6_0) {
        v = sub_0800C8A8(0, zone);
        if (!(z->f6_1)) {
            if (gUnk_03000040.handicap <= 10) {
                v *= gUnk_03000040.handicap + 5;
                v /= 16;
            }
        }
    } else {
        v = sub_0800C894(0, zone);
    }
    if (v < value)
        return 1;
    if (v == value) {
        if (!(ZB(0, zone)->f6_0)) {
            if (sub_08008860(1) > sub_08008860(0))
                return 1;
        }
    }
    return 0;
}
/* 1 if player 0 has no monsters, or any occupied zone of player 0 is weaker than `value`. */
int sub_08057C50(int value)
{
    int i;

    if (sub_08008860(0) == 0)
        return 1;
    for (i = 0; i <= 4; i++) {
        if ((CARD_WORD(ZB(0, i)->card) << 20) != 0 && sub_08057BAC(value, i))
            return 1;
    }
    return 0;
}
/* Pick an attacker among player 1's eligible monster zones (sorted by ATK, ascending) whose best attack plan wins; 1 if found. */
int sub_08057C94(void)
{
    u16 cand[5];
    struct AttackPlan plan;
    int n, done, last, i, next;

    gUnk_02015F00.best.f2 = 0;
    gUnk_02015F00.best.f1 = 0;
    gUnk_02015F00.best.diff = 0;
    gUnk_02015F00.best.gt = 0;
    gUnk_02015F00.best.le = 0;
    n = 0;
    for (i = 0; i <= 4; i++) {
        if (sub_0804A528(1, i, 1)) {
            cand[n] = i;
            n++;
        }
    }
    if (n == 0)
        return 0;
    last = n - 1;
    do {
        done = 1;
        for (i = 0; i < last; i++) {
            u16 a = cand[i];
            u16 b;
            u16 ida, idb;
            int va, vb, ok;

            next = i + 1;
            b = cand[next];
            /* ida is never used; its dead load is deleted, but CSE keeps its 0x94 and zone-base registers, which orders the ROM's mov/ldr/mul */
            ida = CARD_ID(CARD_WORD(ZB(1, a)->card));
            idb = CARD_ID(CARD_WORD(ZB(1, b)->card));
            va = sub_0800C894(1, a);
            vb = sub_0800C894(1, b);
            ok = 1;
            switch (CARD_NUMBER(idb)) {
            case 0x226:
            case 0x2E8:
            case 0x2F9:
                ok = 0;
                break;
            }
            if (va > vb && ok) {
                cand[i] = b;
                cand[i + 1] = a;
                done = 0;
            }
        }
    } while (!done);
    for (i = 0; i < n; i++) {
        u16 z = cand[i];

        sub_0800C894(1, z);
        if (sub_08057A80(z, &plan)) {
            sub_08075294(&gUnk_02015F0C, &plan, 8);
            gUnk_02015F0C.f2 = 1;
            return 1;
        }
    }
    return 0;
}
/* Save the duel state (0x020192E4, 0xD86 words) to the backup buffer 0x02015F14. */
void sub_08057E08(void)
{
    vu32 *dma = (vu32 *)0x040000D4;

    dma[0] = 0x020192E4;
    dma[1] = 0x02015F14;
    dma[2] = 0x80000D86;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
}
/* Restore the duel state from the backup buffer. */
void sub_08057E3C(void)
{
    vu32 *dma = (vu32 *)0x040000D4;

    dma[0] = 0x02015F14;
    dma[1] = 0x020192E4;
    dma[2] = 0x80000D86;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
}
/* For each occupied zone of player 1 whose card fails sub_08007590(number, 0): mark it face-down/flagged and clear its mask bit. */
void sub_08057E70(void)
{
    int i;

    for (i = 0; i <= 4; i++) {
        struct DuelZone *z = &gUnk_020192E4[1].zones[i];
        u16 id = CARD_ID(CARD_WORD(z->card));

        if (id != 0) {
            if (sub_08007590(CARD_NUMBER(id), 0) == 0) {
                z->f6_0 = 0;
                z->f6_1 = 1;
                z->f7_2 = 0;
                gUnk_020192E4[1].zoneMask &= ~(1 << i);
            }
        }
    }
}
