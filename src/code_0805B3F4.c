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
extern struct AiState gUnk_02015EF0;

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
extern struct DuelZone gUnk_0201A070[];
extern const u16 gUnk_08622AB4[];
struct Unk02015EE8 {
    u32 unk0;
    u32 flags;
};
extern struct Unk02015EE8 gUnk_02015EE8;
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
extern struct DuelPlayers gUnk_020192E4;
int sub_08059780(void);
/* The ASM callee explicitly decodes both final word arguments as u16. */
void sub_08055B28(int, int, int, int, int);
int sub_08008A44(int);
void sub_08055EB0(int, int);
u32 sub_0800C8BC(int, int);
int sub_08008524(int, u16);
int sub_0800A78C(int, int, u16);
int sub_0805763C(int, int);
int sub_08047114(int);
u32 sub_0800C894(u32, u32);
int sub_08008860(int);
int sub_08057C50(int);
void sub_08018ED8(int, int, u16, u16);
int sub_080577FC(void);
int sub_08057854(void);
extern int (*const gUnk_0819DD6C[])(void);
int sub_0804A92C(int);
u16 sub_0804E31C(int);
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
extern struct Unk020192E0 gUnk_020192E0;

/* Cost class of a card for the AI (see code_08057EE0): 0 for type 0x15-0x17, 10 for 0x18, else a 4-bit stats field. */
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
u32 sub_08007590(u16 cardNo, int flags);
int sub_0800849C(int player, u16 cardNo, int skipZone);
int sub_0805809C(u16 drop);
int sub_08058358(void);
int sub_080580C8(int *out);
int sub_080563B8(int a, u16 b);
int sub_0805A30C(void);
int sub_08059954(void);
int sub_08059B14(void);
#define E4 gUnk_020192E4
#define S gUnk_02015EF0

/* Main AI phase: choose a hand card, pack tribute choices and queue its action.
 * The phase-10 tails assign the same state pointer after each call so the
 * compiler shares the original call/store tail without changing its reads. */
int sub_0805B3F4(void)
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
        if ((E4.fD6C & 0x10) != 0 || sub_08047114(1) == 0) {
            if (S.step <= 3) {
                if (E4.f1B0C != 0) {
                    S.phase = 1;
                    return 0;
                }
            }
            return 1;
        }
        if (sub_0800849C(0, 0x15B, -1) > 0) {
            S.phase = 1;
            return 0;
        }
        if (E4.f1B0C == 0) {
            S.phase = 10;
            return 0;
        }
        if (sub_0805809C(1) != 0) {
            if (sub_08047114(1) == 0)
                return 1;
            idx = sub_08058358();
            if (idx <= -1)
                return 1;
            w = E4.hand1[idx];
            w <<= 21;
            w >>= 20;
            w += (u32)gUnk_08622AB4;
            p = (const u16 *)w;
            flag = sub_08007590(*p, 1);
            if (sub_08007590(*p, 0) != 0)
                flag = 1;
            sub_08055B28(1, idx, sub_08008A44(1), 0, flag == 0);
            return 1;
        }
        if (sub_08047114(1) == 0)
            return 1;
        idx = sub_080580C8(&out);
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
            w += (u32)gUnk_08622AB4;
            p = (const u16 *)w;
            flag2 = sub_08007590(*p, 1);
            if (sub_08007590(*p, 0) != 0) {
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
                        int target = sub_08008A44(1);
                        int face = flag2 == 0;
                        sub_08055B28(1, idx, target, 0, face);
                        /* FAKEMATCH: retain the index to keep this return tail separate. */
                        asm("" : : "r"(idx));
                    }
                    return 1;
                case 5:
                case 6:
                    a = sub_080563B8(-1, 0);
                    if (a == -1)
                        break;
                    mask = 0x90 | a;
                queue_first:
                    sub_08055B28(1, idx, a, mask, flag2 == 0);
                    return 1;
                default:
                    a = sub_080563B8(-1, 0);
                    b = sub_080563B8(a, 0);
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
        if (sub_08059780() != 0) {
            S.phase = 0;
            return 0;
        }
        S.phase++;
        return 0;
    case 3:
        if (sub_08059954() != 0) {
            S.f6 = 0;
            S.f7 = 0;
            S.f8 = 0;
            S.f9 = 0;
            S.phase++;
        }
        return 0;
    case 4:
        if (sub_08059B14() != 0) {
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
        idx = sub_0805A30C();
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
            if (sub_08008A44(1) < 0)
                break;
            flag = 0;
            {
                u32 base = 0x0201A6CC;
                u32 raw;
                u16 num;
                /* FAKEMATCH: retain the initialized hand-base lifetime. */
                asm("" : : "r"(base));
                raw = *(u32 *)(idx * 4 + base);
                num = *(const u16 *)(((raw << 21) >> 20) + (u32)gUnk_08622AB4);
                if (num == 0xF || num == 0x1FF)
                    flag = 1;
            }
            sub_08055B28(1, idx, sub_08008A44(1), 0, flag);
            next = &S;
            goto phase_store;
        case 5:
        case 6:
            a = sub_080563B8(-1, 0);
            if (a == -1)
                break;
            sub_08055B28(1, idx, a, 0x90 | a, 0);
            next = &S;
            goto phase_store;
        default:
            a = sub_080563B8(-1, 0);
            b = sub_080563B8(a, 0);
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
            sub_08055B28(1, idx, a, mask, 1);
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
    u32 base = (u32)&gUnk_020192E4;
    u32 off = 0xD64;
    return *(u16 *)(base + off);
}

/* FAKEMATCH: retain the initialized base and byte offset in separate registers. */
static inline u8 ZonePlayerFlags(void)
{
    register u32 base asm("r1") = (u32)&gUnk_020192E4;
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
int sub_0805B884(void)
{
    struct AiState *base = AI;
    u8 phase = base->phase;
    /* FAKEMATCH: this saved pointer survives every loop helper. */
    register struct AiState *st asm("r10") = base;

    switch (phase) {
    case 0:
        sub_08059780();
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
                addr += (u32)gUnk_0201A070;
                z = (struct DuelZone *)addr;
                id = CARD_ID(*(u32 *)&z->card);
                ok = id != 0;
                if (z->flags6 & 2) {
                    if (sub_0800C8BC(1, index) == 1) {
                        if (sub_08008524(0, 0x148) != 0 || sub_08008524(1, 0x148) != 0)
                            ok = 0;
                    }
                }
                if (sub_0800A78C(1, AI->f6, 0x15C) != 0)
                    ok = 0;
                if (sub_0800A78C(1, AI->f6, 0x4DC) != 0)
                    ok = 0;
                if (ok == 0) {
                    AI->f6++;
                    return 0;
                }
                if (sub_0805763C(1, AI->f6) != 0) {
                    freeZones = (u16)sub_08047114(1);
                    attack = sub_0800C894(1, AI->f6);
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
                        fieldBase = (u32)gUnk_0201A070;
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
                                attack = sub_0800C894(1, AI->f6);
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
                                        sub_08055EB0(1, zone);
                                        AI->f6++;
                                        return 0;
                                    }
                                    break;
                                case 0x1F4:
                                    if (ZoneFace(savedMask, z) == 0 && sub_08008860(0) > 0 && freeZones) {
                                        sub_08055EB0(1, AI->f6);
                                        AI->f6++;
                                        return 0;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    if (sub_08057C50(attack) != 0 && attack > 0) {
                        struct AiState *cur = AI;
                        u32 stride = 0x94;
                        /* FAKEMATCH: retain the initialized index copy. */
                        register u32 index asm("r2") = cur->f6;
                        u32 off;
                        struct DuelZone *z;
                        asm("" : : "r"(cur), "r"(stride), "r"(index));
                        off = index;
                        off *= stride;
                        off += (u32)gUnk_0201A070;
                        z = (struct DuelZone *)off;
                        if ((z->flags6 & 3) == 1) {
                            /* The original call leaves the table base in r1.
                             * sub_08007590 explicitly consumes its low half. */
                            if (sub_08007590(*(const u16 *)(((*(u32 *)&z->card << 21) >> 20) + 0x08622AB4), 0x08622AB4) == 0 && ZoneSpaceCount(freeZones)) {
                                sub_08055EB0(1, cur->f6);
                                cur->f6++;
                                return 0;
                            }
                        }
                        {
                            u32 index = AI->f6;
                            u32 off;
                            off = index;
                            off *= 0x94;
                            off += (u32)gUnk_0201A070;
                            z = (struct DuelZone *)off;
                            if ((z->flags6 & 3) == 3) {
                                sub_08018ED8(1, index, 0, 0);
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
                        z = (struct DuelZone *)(off + (u32)gUnk_0201A070);
                        if ((z->flags6 & 3) == 2 && (gUnk_02015EE8.flags & 0x200) != 0) {
                            int num = *(const u16 *)(((*(u32 *)&z->card << 21) >> 20) + 0x08622AB4);
                            switch (num) {
                            case 0x23D:
                            case 0x2F:
                            case 0x463:
                                if (ZoneLife() > 0xBB8 && sub_080577FC() > 0)
                                    sub_08057854();
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


u16 sub_0805BB80(void)
{
    struct AiState *st = &gUnk_02015EF0;
    if (st->phase == 0) {
        if (sub_0804A92C(1) == 0)
            return 1;
        st->phase++;
        gUnk_020192E0.u.w.b = 0;
        gUnk_020192E0.u.h.b = 0;
    }
    return sub_0804E31C(1);
}

int sub_0805BBE0(void)
{
    int (*fn)(void) = gUnk_0819DD6C[gUnk_02015EF0.step];
    struct AiState *st = &gUnk_02015EF0;
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
extern struct AiWork2 gUnk_02015F00;
extern const u16 gUnk_08623DF4[];
extern const u16 gUnk_08624568[];
extern u32 gUnk_0201A6CC[];
int sub_08008A1C(int);
int sub_08056E04(int, int);
int sub_08056E68(int, int);
int sub_08054398(int, int);
void sub_0801A7E8(void);
int sub_080086CC(int, u16);
void sub_0800ABC8(int, int, struct ZoneInfo *);
int sub_0800756C(int);
void sub_08057E08(void);
void sub_08057E3C(void);
int sub_08008524(int, u16);
int sub_08008860(int);

#if 0 /* NONMATCHING: sub_0805BC24 has the same structure and control flow, but register allocation differs (loop counters r4/r5, id r4/r5, r8/r9 roles), as do the sl/ip constants */
int sub_0805BC24(void)
{
    s16 i;
    int r6;
    u16 sum;
    int j;
    s16 k;
    struct ZoneInfo info;
    gUnk_02015F00.foundFlag = 0;
    for (i = 0; i <= 8; i++) {
        r6 = 1;
        switch (i) {
        case 0:
            if (gUnk_020192E0.f1B10 == 0)
                r6 = 0;
            if (gUnk_020192E0.fD70 & 0x10)
                r6 = 0;
            if (gUnk_020192E0.lp1 <= 5000)
                r6 = 0;
            if (sub_08008A1C(1) <= 1)
                r6 = 0;
            if (sub_08056E04(1, 0x290) == -1 && gUnk_020192E0.lp0 > gUnk_020192E0.lp1)
                r6 = 0;
            if (sub_08056E04(1, 0x1A3) == -1)
                r6 = 0;
            if (sub_08056E68(1, 0x17B) != -1)
                break;
            continue;
        case 1:
            if (gUnk_020192E0.f1B10 == 0)
                r6 = 0;
            if (sub_08008A1C(1) == 0)
                r6 = 0;
            k = 0x34D;
            if (sub_08056E04(1, k) == -1)
                r6 = 0;
            if (sub_08054398(1, gUnk_08623DF4[k]) == 0)
                r6 = 0;
            sub_0801A7E8();
            break;
        case 2:
            if (gUnk_020192E0.f1B10 == 0)
                r6 = 0;
            k = 0x58A;
            if (sub_08008524(0, k) > 0)
                r6 = 0;
            if (sub_08008524(1, k) > 0)
                r6 = 0;
            if (sub_080086CC(1, 0x1FF) == 0)
                r6 = 0;
            if (sub_08008A1C(1) <= 1)
                r6 = 0;
            if (sub_08056E04(1, 0x4DD) != -1)
                break;
            continue;
        case 3:
            if (sub_08008A1C(1) <= 1)
                r6 = 0;
            if (sub_08056E04(1, 0x13D) == -1)
                r6 = 0;
            if (sub_080086CC(0, 0x3D) != 0)
                break;
            k = 0x4E1;
            if (sub_080086CC(0, k) != 0)
                break;
            if (sub_080086CC(1, 0x3D) != 0)
                break;
            if (sub_080086CC(1, k) != 0)
                break;
            if (gUnk_020192E4.fD6C & 0x10)
                continue;
            j = sub_08056E04(1, 0x3D);
            if (j == -1 && sub_08056E04(1, k) == j)
                r6 = 0;
            if (sub_08008A1C(1) > 2)
                break;
            continue;
        case 6:
            k = 0x5EA;
            goto common;
        case 4:
            r6 = 0;
            if (sub_08056E04(1, 0x522) < 0)
                break;
            if (sub_08008860(1) <= 0)
                break;
            for (j = 0; j <= 4; j++) {
                if (((struct DuelZone *)0x0201A070)[j].flags6 & 2) {
                    sub_0800ABC8(1, j, &info);
                    if ((info.kind & 0x1F) == 7)
                        sum += info.value;
                }
            }
            if (gUnk_020192E4.lp0 > sum * 2)
                break;
            goto success;
        case 5:
            continue;
        case 7:
            k = 0x5EB;
        common:
            if (sub_08056E04(1, k) == -1)
                continue;
            if (sub_08054398(1, gUnk_08623DF4[k]) != 0)
                break;
            continue;
        case 8:
            r6 = 0;
            for (j = 0; j < gUnk_020192E4.handCount; j++) {
                u32 id = CARD_ID(*(u32 *)(j * 4 + (u32)gUnk_0201A6CC));
                if (sub_0800756C(CARD_NUMBER(id)) != 0 && sub_08054398(1, id) != 0)
                    r6 = 1;
            }
            if (sub_08056E04(1, 0x3BA) < 0)
                break;
            sub_08057E08();
            for (j = 5; j <= 9; j++) {
                if (CARD_ID(*(u32 *)&gUnk_0201A070[j].card) == 0) {
                    ((struct CardLo *)&gUnk_0201A070[j])->id = gUnk_08624568[0];
                    gUnk_0201A070[j].unk91 &= ~8;
                    gUnk_0201A070[j].flags6 |= 2;
                }
            }
            for (j = 0; j < gUnk_020192E4.handCount; j++) {
                u32 id = CARD_ID(*(u32 *)(j * 4 + (u32)gUnk_0201A6CC));
                if (sub_0800756C(CARD_NUMBER(id)) != 0 && sub_08054398(1, id) != 0)
                    r6 = 1;
            }
            sub_08057E3C();
            break;
        default:
            r6 = 0;
            break;
        }
        if (r6 != 0) {
        success:
            *(u8 *)((u8 *)&gUnk_02015F00+0x1B24) = 1 | i << 1;
            return 1;
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0805B3F4", sub_0805BC24); /* 0x0805BC24 size 0x47C */

int sub_08008B70(int, u16, u16, u16);
int sub_08059408(u16);
void sub_08049048(int, int, int);
int sub_08008A44(int);
void sub_0801EC58(u16, u16, u16, u16);
void sub_0801FBCC(u32, int);
void sub_08055B28(int, int, int, int, int);

/* Commit hand card `idx` of player 1 into the state block at base->c. */
#define COMMIT_HEAD(base) ((base).c.s8.b = 0)
#define COMMIT_TAIL(base) \
    do { \
        struct AiState *st_ = &gUnk_02015EF0; \
        (base).c.cardId = CARD_ID((base).hand1[st_->f_idx]); \
        (base).c.s8.f3 |= 2; \
        (base).c.sC.idx = st_->f_idx; \
        (base).c.f4.bf.b = 1; \
        st_->f2++; \
    } while (0)

/* FAKEMATCH: initialized address copies preserve the LP load's ADD operands. */
static inline u16 NeighborLife(void)
{
    struct DuelPlayers *base = &gUnk_020192E4;
    register u32 baseAddr asm("r5") = (u32)base;
    register u32 off asm("r3") = 0xD64;
    asm("" : "+r"(off));
    {
        register u16 *p asm("r0") = (u16 *)(baseAddr + off);
        return *p;
    }
}

int sub_0805C0A0(void)
{
    int j;
    u32 tgt;
    u32 id;

    switch (gUnk_02015EF0.f2) {
    case 0:
        if (sub_08008B70(0, 0, 0, 0) > 0) {
            if (sub_08059408(0x29F) != 0) {
                gUnk_020192E0.c.s8.b = 0;
                COMMIT_TAIL(gUnk_020192E0);
                return 0;
            }
            if (sub_08059408(0x425) != 0 || sub_08059408(0x438) != 0)
                goto head2;
        }
        gUnk_02015EF0.f2 += 2;
        return 0;
    case 1:
    case 3:
        sub_08049048(1, 0, 0);
        if ((gUnk_020192E0.c.f4.raw & 2) == 0)
            gUnk_02015EF0.f2--;
        return 0;
    case 2:
        if (sub_08008860(0) > 0) {
            if (sub_08059408(0x14F) != 0 || sub_08059408(0x150) != 0) {
            head2:
                COMMIT_HEAD(gUnk_020192E0);
                COMMIT_TAIL(gUnk_020192E0);
                return 0;
            }
        }
        gUnk_02015EF0.f2 += 2;
        return 0;
    case 4:
        if (sub_08008A44(1) == -1) {
            gUnk_02015F00.foundFlag = 0;
            return 0;
        }
        gUnk_02015F00.target = sub_08008A44(1);
        j=0;
        {
          int n = gUnk_020192E4.handCount;
          if(j<n) {
            u32 needle=0x1A3;
            int bound=n;
            u32 off=0x13E8;
            u32 *cards=(u32 *)((u32)&gUnk_020192E4+off);
            u32 mask=0x7FF;
            const u16 *table=gUnk_08622AB4;
            do {
              if (table[CARD_ID(*cards) & mask] == needle)
                goto queue;
              cards++;
              j++;
            } while (j < bound);
          }
        }
        gUnk_02015F00.foundFlag = 0;
        return 0;
    case 5: {
        struct AiWork2 *w = &gUnk_02015F00;
        u8 *tp = &w->target;
        struct DuelZone *z;

        {
            /* FAKEMATCH: load the target before making its persistent copy. */
            register u32 raw asm("r0") = *tp;
            tgt = raw;
            z = &gUnk_0201A070[raw];
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
        sub_0801EC58(0x8008, 1, tgt << 8, 0);
        tgt = *tp;
        {
            /* FAKEMATCH: initialized packing values preserve the mask/copy order. */
            register u32 mask asm("r1") = 0x1F;
            register u32 hi asm("r0") = tgt & mask;
            hi <<= 16;
            {
                u32 lo = CARD_ID(*(u32 *)&gUnk_0201A070[tgt].card);
                lo |= 0x80400000;
                hi |= lo;
                sub_0801FBCC(hi, 0);
            }
        }
        gUnk_02015EF0.f2++;
        return 0;
    }
    case 6:
        if (sub_080086CC(1, 0x17B) == 0)
            goto fail;
        gUnk_02015EF0.f2++;
        return 0;
    case 7:
        if (gUnk_020192E4.lp0 <= 0x13EB || gUnk_020192E4.lp0 < NeighborLife()) {
            gUnk_02015EF0.f2 = 8;
            return 0;
        }
        if (sub_08059408(0x290) == 0)
            goto fail;
        COMMIT_HEAD(gUnk_020192E4);
        COMMIT_TAIL(gUnk_020192E4);
        return 0;
    fail:
        gUnk_02015F00.foundFlag=0;
        return 0;
    case 8:
        sub_08049048(1, 0, 0);
        if ((gUnk_020192E0.c.f4.raw & 2) == 0) {
            gUnk_020192E0.u.w.b = 0;
            gUnk_020192E0.u.h.b = 0;
            gUnk_02015EF0.f2++;
        }
        return 0;
    case 9:
        return sub_0804E31C(1);
    queue:
        {
            /* FAKEMATCH: initialize the queue base after the hand search. */
            register u8 *work asm("r0") = (u8 *)&gUnk_02015F00;
            sub_08055B28(1, j, work[0x1B25], 0, 1);
        }
        gUnk_02015EF0.f2++;
        return 0;
    default:
        return 1;
    }
}

