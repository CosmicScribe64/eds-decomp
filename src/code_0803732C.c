#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors: int f(struct CardRef *ref), returning 0 (or a step state 0x7F/0x80).
 * See wiki/functions/code-0803732c.md.
 */

/* struct DuelCard / DuelZone / DuelZonesPlayer and gUnk_0201930C come from duel.h. */
#define ZFLAGS(z) (((u8 *)(z))[6])
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))

struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;       /* +0x04 bit 2: effect already handled/skipped (hypothesis) */
    u8 unk4_3 : 5;
    u8 unk5;
    u16 pos;
    u16 unk8;
    u8 numTargets : 3;  /* +0x0A bits 0-2 */
    u8 unkA_3 : 5;
    u8 unkB;
    u16 targets[2];     /* +0x0C: low byte player, high byte zone */
};

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

extern u8 gUnk_02017A40[];
extern const u32 gUnk_08621DE0[];
#define EFF_PHASE gUnk_02017A40[0x3E0]  /* 0x7D-0x80: step of a multi-step effect (hypothesis) */

/* Duel card-list viewer at 0x0201D810 (see code_0802AAC0). */
struct ListView {
    u8 flags0;
    u8 step;
    u8 state;
    u8 unk3;
    u8 unk4;
    u8 row : 2;         /* +5 bits 0-1 */
    u8 unk5_2 : 6;
    u16 top;            /* +6 */
    u8 unk8[4];
    u32 cards[1];       /* +0xC */
};
extern struct ListView gUnk_0201D810;
struct Unk02015F00 {
    u8 pad[0x1B22];
    u16 savedTop;       /* +0x1B22: saved list position */
};
extern struct Unk02015F00 gUnk_02015F00;
/* u16 at 0x0201AE60+0x14 (nonzero = flag) */
struct AE60 {
    u8 unk0[0x14];
    u16 flag14;
};
extern struct AE60 gUnk_0201AE60;
extern char gUnk_08083164[];
extern char gUnk_080831A8[];
#define CARD_NUMBER_SYM(id) (gUnk_08622AB4[0x7FF & (id)])
#define EFF_SIDE gUnk_02017A40[0x3E1]
#define CARD_ID11(w) (((w) << 21) >> 21)
extern const u16 gUnk_08622AB4[];
int sub_08044224(int player, int number, int b);
void sub_08056ECC(u16 id);
void sub_080602A4(int a, int b, int c, char *s);
void sub_08060308(int a, int b, int c);
void sub_0802AF34(int player, int area, int a2, int a3);
int sub_08019554(int player, u16 no);
void sub_08019820(int player, u16 id);

int sub_08022834(int player);
void sub_08019840(int player, u16 id);
void sub_080189FC(int player, int zone, int c);
int sub_08007590(u16 number, int a);
void sub_0801A010(int player, u16 number);
#define CARD_STATS_T(id) CARD_TYPE(id)
/* Per-player duel state at 0x020192E4: struct DuelPlayer[2] from duel.h (same layout). */
#define PH(p) ((struct DuelPlayer *)((u32)gUnk_020192E4 + (p) * 0xD64))
#define ZBC(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + 0x0201930C))
int sub_08076F9C(void);
void sub_08024134(int player, int a, int b);
void sub_08019788(int player, int id);
extern u8 gUnk_02017E20[];
#define E28W (*(u32 *)((u32)gUnk_02017E20 + 8))
#define E28H0 (*(u16 *)((u32)gUnk_02017E20 + 8))
#define E28H1 (*(u16 *)((u32)gUnk_02017E20 + 10))
#define E21 (*(u8 *)((u32)gUnk_02017E20 + 1))
/* Effect scratch object at 0x02017E20 (only a byte at +1 and a card-ish word at +8 are used). */
struct Unk02017E20 {
    u8 unk0;
    u8 side;            /* +1: player being processed (hypothesis) */
    u8 pad2[6];
    u32 word8;          /* +8: card id in bits 0-11, bit 12 = player, bit 17 = flag (hypotheses) */
};
void sub_08007558(void *dst, void *src);
void sub_08055F70(int player, u32 *card, int a, int b, int c);
void sub_080193D4(int player, int a, int b, int c);
int sub_08008A1C(int player);
int sub_08007834(u16 id);
void sub_08018D64(int player, int a);
/* Monster level: Magic/Trap types 0x15-0x17 count as 0, type 0x18 as 10. */
#define CARD_LEVEL(id, r)                                     \
    switch ((int)CARD_TYPE(id)) {                             \
    case 0x15:                                                \
    case 0x16:                                                \
    case 0x17:                                                \
        r = 0;                                                \
        break;                                                \
    case 0x18:                                                \
        r = 10;                                               \
        break;                                                \
    default:                                                  \
        r = (CARD_STATS(id) & 0x1E000000) >> 25;              \
        break;                                                \
    }
/* The turn-side bit at 0x020192E0+0x1B12 bit 1 is duel.h's struct DuelState.linkSkip. */
extern u8 gUnk_02015EE8[];
struct PosWord {
    u32 lo : 12;
    u32 flag12 : 1;     /* bit 12: player (hypothesis) */
    u32 rest : 19;
};
void sub_08022678(int a, int b, int c, int d);
void sub_08056094(int player, u32 *card, int a, int b);
void sub_08019980(int player, int lp);
void sub_08019860(int player, int lp);
void sub_08019800(int player, int id);
void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
int sub_08009CAC(int player, u16 number);
void sub_0800A480(int a, int b);
void sub_0804325C(int a);
void sub_08018DC8();
int sub_08007730(int id);
int sub_08008524(int player, u16 number);
void sub_08018ED8(int player, int zone, int a, int b);
int sub_0800C8BC(int player, int zone);
int sub_0800C894(int player, int zone);
int sub_0807548C(int a);
void sub_08017AB4(int player, int a, u16 b, u16 c);

int sub_0803732C(struct CardRef *ref)
{
    if (!ref->skip4) {
        if (CARD_NUMBER(ref->id) == 0x45C && (0xE & ((u8 *)ref)[2]) != 6) {
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
            return 0;
        }
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = gUnk_020192E0.linkSkip;
            EFF_PHASE--;
        case 0x7F:
            if (sub_08008A1C(EFF_SIDE) > 0 && sub_08044224(EFF_SIDE, CARD_NUMBER(ref->id), 0) > 0) {
                sub_08022678(EFF_SIDE, 0xE, ref->id, ref->player);
                return 0x7E;
            }
            return 0x7D;
        case 0x7E: {
            struct PosWord x;
            struct PosWord *px;
            u8 *g = (u8 *)&gUnk_020192E0;

            *(u32 *)&x = *(u16 *)(g + 0x1B64) | *(u16 *)(g + 0x1B66) << 16;
            px = &x;
            if ((gUnk_02015EE8[1] & 1) && (g[0x1B12] & 2))
                px->flag12 = 1 - px->flag12;
            sub_0801EC58(EFF_SIDE ? 0x80D3 : 0xD3, *(u32 *)&x & 0xFFFF, *(u32 *)&x >> 16, 0);
            switch (CARD_NUMBER(ref->id)) {
            case 0x45C:
                sub_08056094(EFF_SIDE, (u32 *)&x, 0, 0x20);
                break;
            case 0x487:
                sub_08055F70(EFF_SIDE, (u32 *)&x, 0, 1, 0x20);
                break;
            }
            return 0x7D;
        }
        case 0x7D: {
            int sd;

            EFF_SIDE = 1 - EFF_SIDE;
            sd = gUnk_020192E0.linkSkip;
            if (*(volatile u8 *)&EFF_SIDE != sd)   /* volatile: the ROM re-reads the byte after the store */
                return 0x7F;
            return 0x64;
        }
        }
    }
    return 0;
}
int sub_0803752C(struct CardRef *ref)
{
    if (EFF_PHASE == 0x80 && !ref->skip4) {
        int msg;

        switch (CARD_NUMBER(ref->id)) {
        case 0x470:
            msg = (1 & ((u8 *)ref)[2]) ? 0x8015 : 0x15;
            break;
        case 0x471:
            msg = (1 & ((u8 *)ref)[2]) ? 0x8016 : 0x16;
            break;
        case 0x472:
            msg = (1 & ((u8 *)ref)[2]) ? 0x8018 : 0x18;
            break;
        case 0x473:
            msg = (1 & ((u8 *)ref)[2]) ? 0x8017 : 0x17;
            break;
        default:
            goto fail;
        }
        sub_0801EC58(msg, 1, 0, 0);
        return 0x7F;
    }
fail:
    sub_0804325C(0);
    return 0;
}
int sub_080375F8(struct CardRef *ref)
{
    if (!ref->skip4) {
        u16 n = CARD_NUMBER(ref->id);

        switch (n) {
        case 0x474: {
            int p = ref->player;

            sub_08019980(p, sub_08009CAC(ref->player, n) * 500 + 1000);
            break;
        }
        case 0x518: {
            sub_08019860(1 - ref->player, sub_08009CAC(ref->player, n) * 300 + 700);
            break;
        }
        }
    }
    return 0;
} /* 0x080375F8 size 0x90 */
int sub_08037688(struct CardRef *ref)
{
    if (!ref->skip4) {
        if (EFF_PHASE == 0x80) {
            if (ref->numTargets == 1 && ref->targets[0] != 0) {
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8087 : 0x87, ref->zone, ref->targets[0], 0);
                return 0x7F;
            }
        } else if (CARD_NUMBER(ref->id) == 0x479) {
            int i, j;

            for (i = 0; i <= 1; i++)
                for (j = 0; j <= 4; j++)
                    sub_0800A480(i, j);
        }
    }
    return 0;
}
int sub_08037720(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 3;
            EFF_PHASE--;
        case 0x7F:
            if (!sub_08044224(ref->player, 0x47B, 0))
                goto ret0;
            if (1 & ((u8 *)ref)[2]) {
                sub_08056ECC(ref->id);
                gUnk_0201D810.row = 0;
                gUnk_0201D810.top = gUnk_02015F00.savedTop;
                return 0x7C;
            } else {
                sub_080602A4(0x206, 0x712, 0xB, gUnk_08083164);
                sub_08060308(1, 0, 0);
                return 0x7E;
            }
        case 0x7E:
            if (gUnk_0201AE60.flag14 == 0)
                goto ret0;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_080831A8);
            return 0x7D;
        case 0x7D:
            sub_0802AF34(ref->player, -1, 0x47B, 0);
            return 0x7C;
        case 0x7C: {
            struct ListView *lv;

            sub_08019554(ref->player, CARD_NUMBER(CARD_ID11((lv = &gUnk_0201D810)->cards[lv->top + lv->row])));
            sub_08019820(ref->player, CARD_ID(lv->cards[lv->top + lv->row]));
            if (--EFF_SIDE != 0)
                return 0x7F;
            return 0x64;
        }
        }
    }
ret0:
    return 0;
}
int sub_080378BC(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int i, j;

            for (i = 0; i <= 1; i++) {
                ref->targets[i] = 0;
                for (j = 0; j <= 4; j++) {
                    struct DuelZone *z = ZB(i & 1, j);

                    if (CARD_WORD(z->card) << 20 != 0) {
                        sub_0801EC58(i ? 0x8080 : 0x80, j, 0, 0);
                        ref->targets[i]++;
                    }
                }
            }
            EFF_SIDE = ref->player;
            return 0x7F;
        }
        case 0x7F:
            if (ref->targets[ref->player] != 0 && sub_08022834(ref->player)) {
                ref->targets[ref->player]--;
                return 0x7F;
            }
            goto ret7E;
        case 0x7E:
            if (ref->targets[1 - ref->player] != 0 && sub_08022834(1 - ref->player)) {
                ref->targets[1 - ref->player]--;
ret7E:
                return 0x7E;
            }
            return 0x7D;
        }
    }
    return 0;
}
int sub_08037A1C(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i, j;
        int count;

        /* Approximately a flip-up of every card with flag bits & 3 == 1 in zones 0-4 of both sides (hypothesis) */
        for (i = 0; i <= 1; i++) {
            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(i & 1, j);

                if (CARD_WORD(z->card) << 20 != 0 && (3 & ZFLAGS(z)) == 1)
                    sub_08018DC8(i, j, 0);
            }
        }
        count = 0;
        for (i = 0; i <= 1; i++) {
            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(i & 1, j);
                int id = CARD_ID(CARD_WORD(z->card));

                if (id != 0 && sub_08007730(id) != 0)
                    count++;
            }
        }
        if (count > 0)
            sub_08019860(1 - ref->player, count * 500);
    }
    return 0;
}
int sub_08037AF4(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            u8 tp;
            int tz;
            struct DuelZone *z;
            int id;
            int pa;

            if (ref->numTargets != 1)
                goto ret0;
            tp = ref->targets[0];
            tz = ref->targets[0] >> 8;
            pa = tp & 1;
            z = ZB(pa, tz);
            id = CARD_ID(CARD_WORD(z->card));
            if (id == 0)
                goto ret0;
            if (2 & ZFLAGS(z))
                goto ret0;
            ref->targets[1] = id;
            sub_08018DC8(tp, tz, 0, 0);
            sub_08019840(tp, ref->targets[1]);
            sub_080189FC(tp, tz, 0);
            switch (CARD_NUMBER(ref->id)) {
            case 0x485:
                if (CARD_TYPE(ref->targets[1]) > 0x14)
                    goto ret0;
                if (sub_08007590(CARD_NUMBER(ref->targets[1]), 1) != 0 || sub_08007590(CARD_NUMBER(ref->targets[1]), 0) != 0)
                    goto ok;
                goto ret0;
            case 0x486:
                if (CARD_TYPE(ref->targets[1]) != 0x15)
                    goto ret0;
ok:
                return 0x7F;
            default:
                goto ret0;
            }
        }
        case 0x7F:
            sub_0801A010(ref->player, CARD_NUMBER(ref->targets[1]));
            return 0x7E;
        case 0x7E:
            sub_0801A010(1 - ref->player, CARD_NUMBER(ref->targets[1]));
            return 0x7D;
        case 0x7D:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
            return 0x7C;
        case 0x7C:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
            return 0x7B;
        }
    }
ret0:
    return 0;
}
int sub_08037CD4(struct CardRef *ref)
{
    u8 skip = 4 & ((u8 *)ref)[4];

    if (!skip && (0xFC & ((u8 *)ref)[3]) == 0xC) {
        switch (EFF_PHASE) {
        case 0x80: {
            int pp = 1 & ref->player;
            struct DuelPlayer *ph0;
            struct DuelZone *z = (struct DuelZone *)(ref->zone * 0x94 + pp * 0xD64 + (u32)gUnk_020192E4 + 0x28);

            if (CARD_WORD(z->card) << 20 == 0)
                goto ret0;
            ph0 = gUnk_020192E4;
            EFF_SIDE = ph0[(1 - ref->player) & 1].handCount * 2;
            if (ph0[(1 - ref->player) & 1].handCount == 1) {
                gUnk_02017A40[0x3E2] = 0;
                return 0x78;
            }
            EFF_PHASE--;
        }
        case 0x7F:
            if (EFF_SIDE != 0) {
                int side;
                int rnd;
                struct DuelPlayer *ph;

                EFF_SIDE--;
                side = 1 - ref->player;
                rnd = sub_08076F9C();
                ph = gUnk_020192E4;
                sub_08024134(side, 0xB, rnd % ph[(1 - ref->player) & 1].handCount);
                return 0x7F;
            } else {
                int rnd;
                struct DuelPlayer *ph;

                rnd = sub_08076F9C();
                ph = gUnk_020192E4;
                gUnk_02017A40[0x3E2] = rnd % ph[(1 - ref->player) & 1].handCount;
                return 0x78;
            }
        case 0x78: {
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, 1 - ref->player, gUnk_02017A40[0x3E2] << 8 | 0xB, 0);
            sub_08019788(ref->player, CARD_ID(CARD_WORD(PH((1 - ref->player) & 1)->hand[gUnk_02017A40[0x3E2]])));
        }
        }
    }
ret0:
    return 0;
}
int sub_08037E94(struct CardRef *ref)
{
    if (!ref->skip4) {
        sub_08019800(ref->player, ref->targets[0]);
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x808E : 0x8E, ref->zone, ref->targets[0], 0);
    }
    return 0;
}
#if 0 /* NONMATCHING: large 5-step state machine. ref lands in r7 (ROM: r6); the table symbol is hoisted in the zone loop instead of the 0x7FF mask; the zone pointer is strength-reduced; the base of 0x02017A40 is hoisted to ip (the ROM keeps it in r2 and reloads it); scratch registers differ in the 0x7E arm. */
int sub_08037ED8(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            u16 i, j;

            for (i = 0; i <= 1; i++) {
                ref->targets[i] = 0;
                for (j = 0; j <= 4; j++) {
                    int id = CARD_ID(CARD_WORD(ZB(i & 1, j)->card));

                    if (id != 0 && ((gUnk_08621DE0[id & 0x7FF] & 0x1F00000) >> 20) <= 0x14)
                        ref->targets[i]++;
                }
            }
            sub_08018D64(ref->player, 1);
            EFF_SIDE = ref->player;
ret7F:
            return 0x7F;
        }
        case 0x7F: {
            struct DuelPlayer *pl = gUnk_020192E4;
            s8 b = EFF_SIDE;

            if (pl[b & 1].deckCount != 0 && ref->targets[b] != 0) {
                sub_08007558(&gUnk_02017A40[0x3E8], &pl[b & 1].deck[0]);
                sub_0801EC58(EFF_SIDE ? 0x8061 : 0x61, 1, 1, 0);
                sub_08019840(EFF_SIDE, CARD_ID(CARD_WORD(pl[b & 1].deck[0])));
                return 0x7E;
            }
            EFF_SIDE = 1 - EFF_SIDE;
            if (ref->player != EFF_SIDE)
                goto ret7F;
            return 0x78;
        }
        case 0x7E: {
            u32 w = E28W;
            s8 side = E21;

            if (((w << 19) >> 31) != side && (int)(w << 14) < 0 && gUnk_08622AB4[CARD_ID11(w)] == 0x2FA) {
                if (sub_08008A1C(1 - side) > 0) {
                    sub_0801EC58(side ? 0x80C2 : 0xC2, E28H0, E28H1, 0);
                    return 0x7D;
                }
                sub_080193D4(E21, PH(E21 & 1)->handCount - 1, 0, 1);
                goto ret7F;
            }
            {
                int id = CARD_ID(E28W);
                u32 lvl;

                if (CARD_TYPE(id) <= 0x14) {
                    CARD_LEVEL(id, lvl);
                    if (lvl <= 4 && sub_08007834(id) == 0) {
                        sub_0801EC58(E21 ? 0x80C2 : 0xC2, E28H0, E28H1, 0);
                        ref->targets[E21]--;
                        return 0x7C;
                    }
                    ref->targets[EFF_SIDE]--;
                }
            }
            sub_080193D4(EFF_SIDE, PH(EFF_SIDE & 1)->handCount - 1, ref->player != EFF_SIDE, 1);
            goto ret7F;
        }
        case 0x7D:
            sub_08055F70(1 - EFF_SIDE, (u32 *)&gUnk_02017A40[0x3E8], 0, 1, 0);
            goto ret7F;
        case 0x7C:
            sub_08055F70(EFF_SIDE, (u32 *)&gUnk_02017A40[0x3E8], 0, 1, 0);
            goto ret7F;
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0803732C", sub_08037ED8); /* 0x08037ED8 size 0x344 */
int sub_0803821C(struct CardRef *ref)
{
    int flag = 0;

    if (!ref->skip4) {
        int i;

        if (sub_08008524(0, 0x148) != 0 || sub_08008524(1, 0x148) != 0)
            flag = 1;
        for (i = 0; i <= 4; i++) {
            int pp = (1 - ref->player) & 1;
            struct DuelZone *z = ZB(pp, i);

            if (CARD_WORD(z->card) << 20 != 0) {
                struct DuelZone *z2 = ZB((1 - ref->player) & 1, i);

                if ((2 & ZFLAGS(z2)) != 0) {
                    u8 ok = 1;

                    if (flag) {
                        struct DuelZone *z3 = ZB((1 - ref->player) & 1, i);

                        if ((1 & ZFLAGS(z3)) != 0)
                            ok = sub_0800C8BC(1 - ref->player, i) != 1;
                    }
                    if (ok)
                        sub_08018ED8(1 - ref->player, i, 0, 0);
                }
            }
        }
    }
    return 0;
}
int sub_08038300(struct CardRef *ref)
{
    if (!ref->skip4)
        sub_08019980(ref->player, ref->pos);
    return 0;
}
int sub_08038320(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        int tp1 = (u8)ref->targets[0];
        int tz1 = ref->targets[0] >> 8;
        int tp2 = (u8)ref->targets[1];
        int tz2 = ref->targets[1] >> 8;
        int pa = tp1 & 1;
        struct DuelZone *za = ZB(pa, tz1);

        if (CARD_WORD(za->card) << 20 != 0 && (2 & ZFLAGS(za)) != 0) {
            int pb = tp2 & 1;
            struct DuelZone *zb = ZB(pb, tz2);

            if (CARD_WORD(zb->card) << 20 != 0 && (2 & ZFLAGS(zb)) != 0) {
                sub_0801EC58(tp1 ? 0x8098 : 0x98, tz1, 0, 0);
                sub_08017AB4(ref->player, sub_0807548C(sub_0800C894(tp1, tz1)), ref->targets[1], 4);
            }
        }
    }
    return 0;
}
