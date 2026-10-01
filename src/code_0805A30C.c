#include "global.h"

/* CPU turn step handlers (see code_0805B3F4): gUnk_02015EF0 is the AiState record,
 * +1 is the step index into the table gUnk_0819DD6C, +2..+5 sub-step counters,
 * +6 a loop counter over the card-number table gUnk_08086448, +0xA a phase byte. */
struct AiState {
    u8 f0;
    u8 step;    /* +1: index into gUnk_0819DD6C */
    u8 f2;      /* +2: step sub-state */
    u8 f3;      /* +3: hand index */
    u8 f4;
    u8 f5;      /* +5: zone index */
    u8 f6;      /* +6: index into gUnk_08086448 */
    u8 f7;
    u8 f8;
    u8 f9;
    u8 phase;   /* +0xA */
    u8 fB;
};
extern struct AiState gUnk_02015EF0;
extern u8 gUnk_02015F00[];  /* AI work area, 0x1B28 bytes */
extern void sub_08075278(void *dst, u32 size);
extern void sub_0801EC58(u16 msg, u16 a, int b, int c);
extern int sub_0805BC24(void);

/* One zone of a player (0x94 bytes). */
struct DuelZone {
    u32 card;       /* +0: card word, card id in the low 12 bits */
    u8 unk4[2];
    u8 flags6;      /* +6: bit 1 = face up */
    u8 unk7[0x8A];
    u8 f91;         /* +0x91: bit 2 */
    u8 unk92[2];
};

/* Per-player duel state (0xD64 bytes each at 0x020192E4); player 1 = the CPU. */
struct DuelPlayer {
    u16 lp;         /* +0x000 life points (hypothesis) */
    u8 handCount;   /* +0x002 */
    u8 deckCount;   /* +0x003 */
    u8 pad4[0x28 - 4];
    struct DuelZone zones[11];  /* +0x028 (0x0201930C for player 0) */
    u32 hand[80];   /* +0x684: u32 card words, card id = (w << 20) >> 20 */
    u8 pad7C4[0xD64 - 0x7C4];
};
extern struct DuelPlayer gUnk_020192E4[2];

extern u8 gUnk_0201A070[];          /* player 1 zones */
extern u8 gUnk_0201930C[];          /* player 0 zones (== 0x020192E4 + 0x28) */
extern u32 gUnk_0201A6CC[];         /* player 1 hand (== 0x0201A048 + 0x684) */
extern u8 gUnk_0201A04A;            /* player 1 hand count (== 0x0201A048 + 2) */
extern const u16 gUnk_08622AB4[];   /* maps card ID to card number */
extern const u32 gUnk_08621DE0[];   /* card stats: type = bits 20-24, spell subtype = bits 17-19 */
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_NUMBER(id) (*((const u16 *)(((id) & 0x7FF) + gUnk_08622AB4)))
#define ZONE(z) ((struct DuelZone *)(gUnk_0201A070 + (z) * 0x94))
extern u32 sub_08007590(u16 cardNo, u16 flag);
extern u16 sub_08008860(int player);
extern int sub_080090C8(int player, u16 number);
extern int sub_080091B4(int player);
extern int sub_08009280(int player, int number);
extern int sub_08009150(int player, u16 type);
extern void sub_08055EB0(int player, int zone);
extern int sub_08008C6C(int player);
extern int sub_0800A2A8(int player, u16 number);
extern void sub_08046A74(int player);

INCLUDE_ASM("asm/nonmatching/code_0805A30C", sub_0805A30C); /* 0x0805A30C size 0x590 */
/* Step 0: reset the AI work area, announce the turn and ask the scripted-strategy
 * picker (sub_0805BC24). When one applies, jump to step 8, else let the runner
 * advance to step 1. */
int sub_0805A89C(void)
{
    sub_08075278(gUnk_02015F00, 0x1B28);
    sub_0801EC58(0x8052, 0, 0, 0);
    if (sub_0805BC24() != 0) {
        gUnk_02015EF0.step = 8;
        gUnk_02015EF0.f2 = 0;
        gUnk_02015EF0.f3 = 0;
        gUnk_02015EF0.f4 = 0;
        gUnk_02015EF0.f5 = 0;
        return 0;
    }
    return 1;
}


/* Scan the CPU hand, test opponent spell/trap targets, then queue a play.
 * FAKEMATCH: all register bindings and empty constraints below are initialized
 * lifetime/scheduling hints. Field and hand loop masks are distinct from the
 * outer mask; bound caller-saved values are dead before external calls.
 * Type bits are 20..24 (mask 0x01F00000). */
int sub_0805A8E8(void)
{
    struct AiState *initial = &gUnk_02015EF0;
    u32 phase = initial->f2;
    register struct AiState *p asm("r9");
    p = initial;
    switch (phase) {
    case 0:
        {
            struct AiState *q = p;
            q->f3 = phase;
            {
                register u32 next asm("r0") = q->f2 + 1;
                struct AiState *nextp = p;
                nextp->f2 = next;
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
            register u32 base asm("r0") = (u32)&gUnk_020192E4;
            register u32 off asm("r2") = 0xD66;
            asm("" : : "r"(base), "r"(off));
            count = *(u8 *)(base + off);
        }
        if (count == 0) goto done;
        q = p;
        {
            register u32 index asm("r3") = q->f3;
            asm("" : : "r"(index));
            if (index >= count) goto done;
        }
        {
            register u32 seed asm("r4") = 0x7FF;
            u32 lookupMask;
            asm("" : : "r"(seed));
            lookupMask = seed;
        hand_next:
            {
                register u32 w asm("r8");
                int c;
                u16 ok;
                {
                    u32 index = q->f3;
                    u32 off = index << 2;
                    u32 table = (u32)gUnk_0201A6CC;
                    u32 raw;
                    raw = *(u32 *)(off + table);
                    w = CARD_ID(raw);
                }
                c = sub_08008C6C(1);
                ok = 0;
                if (c < 0) return 1;
                switch (({
                    u32 index = w;
                    register u32 mask asm("r6");
                    asm("" : : "r"(index));
                    mask = lookupMask;
                    asm("" : "+r"(mask));
                    ((const u16 *)0x08622AB4)[index & mask];
                }
                )) {
                case 0x3FB:
                    {
                        register int player asm("r0") = 0;
                        ok = sub_0800A2A8(player, 0x14F) > 0;
                    }
                    break;
                case 0x3FE:
                    {
                        register int player asm("r0") = 0;
                        ok = sub_0800A2A8(player, 0x150) > 0;
                    }
                    break;
                case 0x402:
                    {
                        int player = 0;
                        asm("" : : "r"(player));
                        ok = sub_0800A2A8(player, 0x3F0) > 0;
                    }
                    break;
                case 0x405:
                case 0x482:
                    {
                        int z = 5;
                        register struct DuelPlayer *base asm("r5") = &gUnk_020192E4[0];
                        register u32 off asm("r2") = 0x28;
                        register u8 *zb asm("r12");
                        u32 mask;
                        asm("" : : "r"(z), "r"(base), "r"(off));
                        zb = (u8 *)((u32)base + off);
                        mask = 0x7FF;
                    field22:
                        {
                            u32 off = 0x94 * z;
                            u8 *basecopy = zb;
                            struct DuelZone *zp;
                            u32 id;
                            zp = (struct DuelZone *)(off + (u32)basecopy);
                            id = CARD_ID(zp->card);
                            if (id != 0) {
                                id &= mask;
                                if ((((const u32 *)0x08621DE0)[id] & 0x1F00000) >> 20 == 0x16 && !(zp->flags6 & 2)) ok = 1;
                            }
                        }
                        if (++z <= 9) goto field22;
                        if (base->handCount != 0) {
                            u32 mask = 0x7FF;
                            register struct DuelPlayer *cur asm("r0") = &gUnk_020192E4[0];
                            register u32 off asm("r2") = 0x684;
                            register u32 *hp asm("r1");
                            register u32 typeMask asm("r2");
                            int n;
                            asm("" : : "r"(mask), "r"(cur), "r"(off));
                            hp = (u32 *)((u32)cur + off);
                            typeMask = 0x1F00000;
                            n = cur->handCount;
                        hand22:
                            {
                                u32 id = CARD_ID(*hp);
                                register u32 table asm("r6");
                                id &= mask;
                                id <<= 2;
                                table = 0x08621DE0;
                                asm("" : : "r"(table));
                                if ((*(u32 *)(id + table) & typeMask) >> 20 == 0x16) ok = 1;
                            }
                            hp++;
                            if (--n != 0) goto hand22;
                        }
                        break;
                    }
                case 0x406:
                case 0x409:
                    {
                        int z = 5;
                        u8 *zb = gUnk_0201930C;
                        u32 mask = 0x7FF;
                    field21:
                        {
                            u32 off = 0x94 * z;
                            u8 *basecopy = zb;
                            struct DuelZone *zp;
                            u32 id;
                            zp = (struct DuelZone *)(off + (u32)basecopy);
                            id = CARD_ID(zp->card);
                            if (id != 0) {
                                u32 off;
                                register u32 table asm("r6");
                                id &= mask;
                                off = id << 2;
                                table = 0x08621DE0;
                                asm("" : : "r"(off), "r"(table));
                                if ((*(u32 *)(off + table) & 0x1F00000) >> 20 == 0x15 && !(zp->flags6 & 2)) ok = 1;
                            }
                        }
                        if (++z <= 9) goto field21;
                        break;
                    }
                case 0x4DD:
                case 0x522:
                    goto skip;
                default:
                    {
                        u32 index = w;
                        u32 mask;
                        u32 stats, ty;
                        asm("" : : "r"(index));
                        mask = lookupMask;
                        index &= mask;
                        stats = ((const u32 *)0x08621DE0)[index];
                        ty = (stats & 0x1F00000) >> 20;
                        if (ty == 0x15) goto doAction;
                        if (ty != 0x16) break;
                        if (((stats & 0xE0000) >> 17) == 5) ok = 1;
                        {
                            u32 off = index << 1;
                            register u32 table asm("r2") = 0x08622AB4;
                            asm("" : : "r"(off), "r"(table));
                            if (*(const u16 *)(off + table) == 0x136) ok = 1;
                        }
                        break;
                    }
                }
                if (!ok) goto skip;
            doAction:
                {
                    register u32 packed asm("r2");
                    sub_08046A74(1);
                    {
                        struct AiState *state = &gUnk_02015EF0;
                        register u32 nibble asm("r2");
                        nibble = state->f3 & 15;
                        nibble <<= 4;
                        c &= 15;
                        packed = nibble | c;
                    }
                    sub_0801EC58(0x80C5, w, packed, 0);
                    return 0;
                }
            skip:
                q = &gUnk_02015EF0;
                {
                    u32 next = q->f3 + 1;
                    register u8 *cp asm("r3");
                    q->f3 = next;
                    cp = &gUnk_0201A04A;
                    count = *cp;
                    if (count != 0 && (u8)next < count) goto hand_next;
                }
            }
        }
    }
done:
    {
        register struct AiState *last asm("r4") = p;
        last->f2++;
    }
    return 0;
}

struct AiCardRef {
    u16 id;
    u8 player : 1;
    u8 reserved2 : 7;
    u8 positionHigh : 2;
    u8 kind : 6;
    u32 data[4];
};
extern const s16 gUnk_08086448[];
extern const u16 gUnk_08623DF4[];
int sub_080090E0(int player, u16 type);
int sub_08056E04(int player, int number);
int sub_08008524(int player, u16 number);
int sub_08008C94(int player, u16 id);
int sub_08041DC4(struct AiCardRef *, int player, int zone);
int sub_0802CE38(struct AiCardRef *, struct AiCardRef *, u16);
void sub_0801FBCC(u32 action, u32 flags);
static inline u16 AiScanCardId(u16 number)
{
    if (number == 0xFFFF) return 0;
    if (number <= 0x7CF) return ((const u16 *)0x08623DF4)[number & 0x7FF];
    return ((const u16 *)0x08623DF4)[(number - 0x7D0) & 0x7FF] + 1;
}
/* Scan field/hand spell candidates, apply LP gates, then queue the selected action. */
int sub_0805AB90(void)
{
    struct AiCardRef ref;
    switch (gUnk_02015EF0.phase) {
    case 0:
        gUnk_02015EF0.f6 = 0;
        do {
            if (sub_08009280(1, gUnk_08086448[gUnk_02015EF0.f6])) {
                int ok = 0;
                switch (gUnk_08086448[gUnk_02015EF0.f6]) {
                case 0x151: case 0x152: case 0x153: case 0x154: case 0x155:
                case 0x156: case 0x157: case 0x158: case 0x159:
                case 0x3EE: case 0x3EF:
                    ok = 1; break;
                case 0x15A:
                    if (gUnk_020192E4[0].lp <= 1000 && gUnk_020192E4[1].lp > 500)
                        ok = 1;
                    if (gUnk_020192E4[0].lp < gUnk_020192E4[1].lp + 500)
                        ok = 1;
                    if (gUnk_020192E4[1].lp > 500) { ok = 1; break; }
                    break;
                case 0x40F:
                    if (200 * gUnk_020192E4[0].handCount > gUnk_020192E4[0].lp)
                        ok = 1;
                    if (gUnk_020192E4[0].handCount > 2) { ok = 1; break; }
                    break;
                case 0x3F1:
                    if (sub_080090E0(0, 0x16) > 0) { ok = 1; break; }
                    break;
                case 0x3EC:
                    if (sub_080090E0(0, 0x15) > 0) { ok = 1; break; }
                    break;
                case 0x29F: case 0x437: case 0x438:
                    if (sub_080091B4(0) > 0) { ok = 1; break; }
                    break;
                case 0x425:
                    if (sub_080091B4(0) > 0 && sub_080091B4(1) == 0) { ok = 1; break; }
                    break;
                case 0x42F:
                    if (gUnk_020192E4[1].lp > 1000 && gUnk_020192E4[0].handCount)
                        ok = 1;
                    break;
                }
                if (ok) {
                    int z;
                    u32 id;
                    ref.kind = 0;
                    for (z = 5; z <= 9; z++) {
                        {
                            u32 raw = ZONE(z)->card;
                            id = CARD_ID(raw);
                        }
                        /* Original code indexes this flag by table position, not z. */
                        if (id && ({
                            /* FAKEMATCH: initialized mask copies retain the lookup registers. */
                            register u32 loadMask asm("r1") = 0x7FF;
                            register u32 mask asm("r0");
                            u32 index;
                            asm("" : : "r"(loadMask));
                            mask = loadMask;
                            asm("" : "+r"(mask));
                            index = id;
                            ((const u16 *)0x08622AB4)[index & mask];
                        }) == (u16)gUnk_08086448[gUnk_02015EF0.f6]
                            && (ZONE(gUnk_02015EF0.f6)->f91 & 4)
                            && sub_08041DC4(&ref, 1, z)) {
                            sub_0801EC58(0x807F, z, 0, 0);
                            {
                                u32 bits = (u32)(z & 31) << 16;
                                id |= 0x80200000;
                                sub_0801FBCC(bits | id, 0);
                            }
                            return 0;
                        }
                    }
                }
            }
            gUnk_02015EF0.f6++;
        } while (gUnk_02015EF0.f6 <= 0x13);
        {
            /* FAKEMATCH: keep the initialized phase pointer separate from the next loop. */
            register struct AiState *state asm("r2") = &gUnk_02015EF0;
            state->phase++;
        }
        /* fall through */
    case 1: {
        /* FAKEMATCH: stage the initialized reset pointer before retaining state. */
        register struct AiState *initial asm("r1");
        struct AiState *state;
        const s16 *numbers;
        struct AiCardRef *record;
        int zero = 0;

        initial = &gUnk_02015EF0;

        initial->f6 = zero;
        numbers = gUnk_08086448;

        state = initial;
        record = &ref;

        do {
            if (sub_08056E04(1, numbers[state->f6]) >= 0) {
                int ok = 0;
                switch (numbers[state->f6]) {
                case 0x151: case 0x152: case 0x153: case 0x154: case 0x155:
                case 0x156: case 0x157: case 0x158: case 0x159:
                case 0x3EE: case 0x3EF:
                    ok = 1; break;
                case 0x15A:
                    if (gUnk_020192E4[0].lp <= 1999 && gUnk_020192E4[1].lp > 1500)
                        { ok = 1; break; }
                    break;
                case 0x3F1:
                    if (sub_080090E0(0, 0x16) > 0) { ok = 1; break; }
                    break;
                case 0x3EC:
                    if (sub_080090E0(0, 0x15) > 0) { ok = 1; break; }
                    break;
                case 0x29F: case 0x437:
                    if (sub_080091B4(0) > 0) { ok = 1; break; }
                    break;
                case 0x425: case 0x438:
                    if (sub_080091B4(0) > 0 && sub_080091B4(1) == 0) { ok = 1; break; }
                    break;
                case 0x42F:
                    if (gUnk_020192E4[1].lp > 1000 && gUnk_020192E4[0].handCount)
                        ok = 1;
                    break;
                }
                if (ok) {
                    record->player = 1;
                    record->kind = 0;
                    record->id = AiScanCardId(numbers[state->f6]);
                    if (sub_0802CE38(&ref, 0, 1) && sub_08008C94(1, record->id)) {
                        gUnk_02015EF0.phase++;
                        return 0;
                    }
                }
            }
            state->f6++;
        } while (state->f6 <= 0x13);
        break;
    }
    case 2:
        if (sub_08008524(0, 0x49C) || sub_08008524(1, 0x49C)) {
            u16 id = AiScanCardId(gUnk_08086448[gUnk_02015EF0.f6]);
            int hand = sub_08056E04(1, gUnk_08086448[gUnk_02015EF0.f6]);
            int freeZone = sub_08008C6C(1);
            {
                int packed = ((hand & 15) << 4) | (freeZone & 15);
                /* FAKEMATCH: finish packing before loading the message constant. */
                asm("" : : "r"(packed));
                sub_0801EC58(0x80C5, id, packed, 0);
            }
        } else {
            u16 id = AiScanCardId(gUnk_08086448[gUnk_02015EF0.f6]);
            int hand = sub_08056E04(1, gUnk_08086448[gUnk_02015EF0.f6]);
            int freeZone = sub_08008C6C(1);
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
                sub_0801EC58(0x80C5, id, packed, 0);
            }
            zone = sub_08008C6C(1);
            {
                /* FAKEMATCH: initialize the card word in the final packing register. */
                register u32 card asm("r1");
                u32 bits;
                u16 number = gUnk_08086448[gUnk_02015EF0.f6];
                if (number == 0xFFFF)
                    card = 0;
                else if (number <= 0x7CF) {
                    /* FAKEMATCH: preserve the initialized reverse-table scratch. */
                    register u32 table asm("r2");
                    u32 off = (number & 0x7FF) * 2;
                    table = 0x08623DF4;
                    asm("" : : "r"(table));
                    card = *(const u16 *)(off + table);
                } else
                    card = ((const u16 *)0x08623DF4)[(number - 0x7D0) & 0x7FF] + 1;
                bits = (u32)(zone & 31) << 16;
                card = (u16)card;
                card |= 0x80200000;
                sub_0801FBCC(bits | card, 0);
            }
        }
        gUnk_02015EF0.phase = 0;
        return 0;
    }
    return 1;
}


/* Step 6: scan player 1's monster zones 0-4 (counter f5) for the card numbers that
 * let the CPU attack / activate from that zone; on a hit advance the sub-state (f6),
 * on a miss try the next zone.
 * NONMATCHING: structure/control flow decoded. The ROM uses dispatch ptr r4 (dead after the
 * dispatch, reused for 0x7FF), body ptr r5, zones base r6 and 0x94 in r7; the build keeps the
 * gUnk_08622AB4 table address and 0x7FF in r6/r7 across the sub_08007590 call, so zones/0x94
 * spill to r8. Case 0x231 must use the body pointer (r5), not p. */
#if 0 /* NONMATCHING */
int sub_0805B184(void)
{
    struct AiState *p = &gUnk_02015EF0;
    struct AiState *q = &gUnk_02015EF0;

    switch (p->f6) {
    case 0:
        p->f4 = 0;
        p->f5 = 0;
        p->f6++;
        /* fall through */
    case 1:
        {
        int z;
        u8 id;
        if (q->f5 > 4)
            return 1;
        z = q->f5;
        id = CARD_ID(ZONE(z)->card);
        if (id == 0) {
            return 0;
            q->f5 = z + 1;
        }
        if (ZONE(z)->flags6 & 2) {
            q->f5 = z + 1;
            return 0;
        }
        if (sub_08007590(CARD_NUMBER(id), 0) == 0) {
            return 0;
            q->f5++;
        }
        id = CARD_ID(ZONE(q->f5)->card);
        switch (CARD_NUMBER(id)) {
        case 0x27:
            if (sub_080090C8(0, 0x148) > 0) {
                q->f6++;
                return 0;
            }
            break;
        case 0x53:
        case 0xDF:
            if (sub_08009280(0, CARD_NUMBER(id)) > 0) {
                gUnk_02015EF0.f6++;
                return 0;
            }
            break;
        case 0x65:
            if (sub_08009150(1, 0x15) > 0) {
                q->f6++;
                return 0;
            }
            break;
        case 0x109:
            if (sub_080091B4(0) > 1) {
                q->f6++;
                return 0;
            }
            break;
        case 0x1AB:
            if (sub_08009150(1, 0x16) > 0) {
                q->f6++;
                return 0;
            }
            break;
        case 0x1F4:
        case 0x21C:
        case 0x259:
            if (sub_08008860(0) > 0) {
                gUnk_02015EF0.f6++;
                return 0;
            }
            break;
        case 0x21B:
        case 0x24E: {
            s8 hc1 = gUnk_020192E4[1].handCount;
            if (hc1 <= 2 || gUnk_020192E4[0].handCount > hc1 + 2) {
                return 0;
                gUnk_02015EF0.f6++;
            }
            break;
        }
        case 0x231:
            if (sub_08008860(0) != 0)
                break;
            q->f6++;
            return 0;
        case 0x246:
            if (sub_080090C8(0, 0x15B) > 0) {
                q->f6++;
                return 0;
            }
            break;
        case 0x249:
            if (sub_08009280(0, CARD_NUMBER(id)) > 1) {
                q->f6++;
                return 0;
            }
            break;
        case 0x262:
        case 0x2FA:
            return 0;
            gUnk_02015EF0.f6++;
        case 0x280:
            if (sub_08008860(0) > sub_08008860(1) + 1) {
                q->f6++;
                return 0;
            }
            break;
        case 0x452:
        case 0x48B:
            if (sub_08008860(1) == 1 && sub_08008860(0) > 1) {
                gUnk_02015EF0.f6++;
                return 0;
            }
            break;
        }
        gUnk_02015EF0.f5++;
        return 0;
        }
    case 2:
        sub_08055EB0(1, p->f5);
        p->f5++;
        p->f6 = 1;
        return 0;
    }
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0805A30C", sub_0805B184); /* 0x0805B184 size 0x270 */

