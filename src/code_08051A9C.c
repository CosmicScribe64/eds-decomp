#include "global.h"

/* Menu block 0x0201AE60 (see code_0804EFF0) */
struct Ui {
    u8 u0[8];
    u16 x;          /* +8 */
    u8 uA[4];
    u16 h;          /* +0xE */
    u8 u10[4];
    u16 sel;        /* +0x14 */
    u8 u16_[0x21 - 0x16];
    u8 b21;
    u8 state;       /* +0x22 */
    u8 timer;       /* +0x23 */
};
extern struct Ui gUnk_0201AE60;
struct MainKeys { u8 u0[4]; u16 keysHeld; u16 keys; };
extern struct MainKeys gUnk_03000040;
/* Duel global 0x020192E0 */
struct Duel {
    u8 u0[0x1B50];
    u8 b1B50;
    u8 pad;
    u16 h1B52[2];       /* +0x1B52 candidate card ids */
    u8 u1B56[0x1B62 - 0x1B56];
    u8 step;            /* +0x1B62 */
    u8 pad2;
    u16 sel;            /* +0x1B64 */
    u16 sel2;           /* +0x1B66 */
};
extern struct Duel gUnk_020192E0;
extern u16 gUnk_0201AE44;
extern const u8 gUnk_0808628C[], gUnk_08086290[];
extern const char *const gUnk_0819D264[];
void sub_080752D0(char *dst, const void *src);
void sub_080752E8(char *dst, const void *src);
int sub_08076F9C(void);
void sub_0805FBA4(void);
int sub_08052668(void);
struct Player { u8 unk0[2]; u8 handCount; u8 pad[0x684 - 3]; u32 hand[80]; u8 pad2[0xD64 - 0x684 - 0x140]; };
struct DuelP { u8 pad[4]; struct Player players[2]; };
struct Q1 { u8 pad[9]; u8 f0 : 1; u8 rest : 7; };
struct DuelQ { u8 pad[0xD68]; struct Q1 q1; };
extern const u32 gUnk_08621DE0[];
#define CARD_STATS(id) (*(gUnk_08621DE0 + ((id) & 0x7FF)))
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_ID(w) (((w) << 20) >> 20)
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
int sub_08052F38(u32 mask);
int sub_080578F4(void);
int sub_08008AF8(int player, int a);
int sub_080563B8(int a, int b);
void sub_08017FF4(int a, int b);
int sub_08054398(int player, int id);
int sub_08007834(int id);
int sub_08008A44(int player);
void sub_08077AEC(u16 se);
void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2);
void sub_08075294(u32 dst, const void *src, u32 n);
void sub_08074B08(u32 a, u32 b);
void sub_0807501C(u32 a, u32 b, u32 c, const char *s);
void sub_08075114(u32 a, u32 b);
extern const u8 gUnk_0822C300[];
extern const char *const gUnk_0819D214[];
extern struct Player gUnk_020192E4[];
extern u8 gUnk_0201AE42;
struct ActBlk { u8 u0[0x4FC]; u8 b4FC; u8 count; };
extern struct ActBlk gUnk_02017A40;
struct DuelScreen { u8 unk0[0x808]; u8 b808; u8 pad[0x824 - 0x809]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreen gUnk_0201CFB0;
void sub_080240A8(int player, int a);
void sub_08024134(u32 player, u32 a, u32 b);
void sub_0801EC58(u16 msg, u16 a, u16 b, u16 c);
void sub_080193D4(int player, int idx, int a, int b);
extern const u8 gUnk_08086298[];
extern const u8 gUnk_08086210[], gUnk_08086254[], gUnk_0808626C[], gUnk_08086018[], gUnk_08085FF4[];
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
void sub_08060308(u32 a, void (*b)(void), int (*c)(void));
int sub_080522C0(void);
void sub_08052390(void);
void sub_08052190(void);
int sub_0805243C(void);

#if 0 /* NONMATCHING: register allocation only. The ROM keeps ps in r7 and
       * count/&b4FC in r8; the build swaps them (ps r8, count r7). */
int sub_08051A9C(int player, u16 x, int count)
{
    u8 *base = (u8 *)gUnk_020192E4;
    struct Player *ps = (struct Player *)(base + (player & 1) * 0xD64);
    u8 *step;
    if (ps->handCount != 0) {
        step = base + 0x1B5E;
        if (*step == 0) {
            sub_080240A8(player, 0xB);
            gUnk_02017A40.b4FC = 0;
            gUnk_02017A40.count = count;
            (*step)++;
            return 0;
        }
        if (gUnk_02017A40.count != 0) {
            u8 *bp = &gUnk_02017A40.b4FC;
            if (*bp <= 9) {
                sub_08024134(player, 0xB, sub_08076F9C() % ps->handCount);
                gUnk_0201CFB0.b808 |= 8;
                (*bp)++;
                return 0;
            } else {
                u16 msg = 8;
                if (player != 0)
                    msg = 0x8008;
                sub_0801EC58(msg, (u16)player, (*(u8 *)&gUnk_0201CFB0.w82C << 8) | 0xB, 0);
                sub_080193D4(player, gUnk_0201CFB0.w82C, x, 1);
                gUnk_02017A40.count--;
                *bp = 0;
                return 0;
            }
        }
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatching/code_08051A9C", sub_08051A9C); /* 0x08051A9C size 0x120 */
#endif
#if 0 /* NONMATCHING: temp register numbering only (count ptr r4 vs r5, constants r3 vs r1) */
int sub_08051BBC(int player, int unused, int count)
{
    u8 *base = (u8 *)gUnk_020192E4;
    struct Player *ps = (struct Player *)(base + (player & 1) * 0xD64);
    u8 *step;
    if (ps->handCount != 0) {
        step = base + 0x1B5E;
        if (*step == 0) {
            sub_080240A8(player, 0xB);
            gUnk_02017A40.b4FC = 0;
            gUnk_02017A40.count = count;
            (*step)++;
            return 0;
        }
        if (gUnk_02017A40.count != 0) {
            u8 *bp = &gUnk_02017A40.b4FC;
            if (*bp <= 9) {
                gUnk_0201CFB0.b808 |= 8;
                sub_08024134(player, 0xB, sub_08076F9C() % ps->handCount);
                (*bp)++;
                return 0;
            } else {
                u32 msg = 8;
                if (player != 0)
                    msg = 0x8008;
                sub_0801EC58(msg, (u16)player, (*(u8 *)&gUnk_0201CFB0.w82C << 8) | 0xB, 0);
                msg = 0xC1;
                if (player != 0)
                    msg = 0x80C1;
                gUnk_02017A40.count--;
                sub_0801EC58(msg, (u16)gUnk_0201CFB0.w82C, 1, 0);
                *bp = 0;
                return 0;
            }
        }
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatching/code_08051A9C", sub_08051BBC); /* 0x08051BBC size 0x11C */
#endif
#if 0 /* NONMATCHING: register allocation (ps/count swap, see sub_08051A9C) */
int sub_08051CD8(int player, int unused, int count)
{
    u8 *base = (u8 *)gUnk_020192E4;
    struct Player *ps = (struct Player *)(base + (player & 1) * 0xD64);
    u8 *step;
    if (ps->handCount != 0) {
        step = base + 0x1B5E;
        if (*step == 0) {
            sub_080240A8(player, 0xB);
            gUnk_02017A40.b4FC = 0;
            gUnk_02017A40.count = count;
            (*step)++;
            return 0;
        }
        if (gUnk_02017A40.count != 0) {
            u8 *bp = &gUnk_02017A40.b4FC;
            if (*bp <= 9) {
                gUnk_0201CFB0.b808 |= 8;
                sub_08024134(player, 0xB, sub_08076F9C() % ps->handCount);
                (*bp)++;
                return 0;
            } else {
                u16 msg = 8;
                u32 msg2 = 0xCE;
                if (player != 0) {
                    msg = 0x8008;
                    msg2 = 0x80CE;
                }
                sub_0801EC58(msg, (u16)player, (*(u8 *)&gUnk_0201CFB0.w82C << 8) | 0xB, 0);
                sub_0801EC58(msg2, (u16)gUnk_0201CFB0.w82C, 1, 0);
                gUnk_02017A40.count--;
                *bp = 0;
                return 0;
            }
        }
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatching/code_08051A9C", sub_08051CD8); /* 0x08051CD8 size 0x11C */
#endif
#if 0 /* NONMATCHING: 20 diff lines, only the register order of the three `0x0201CFB0` field addresses: ROM keeps &w824/&w828/&w82C in r4/r6/r7, built in r6/r7/r4 (and therefore swaps the two `ldrb` operands of the message). The ternary for the message id and computing `*pb + *pc` before `*pa` are both needed. Tried: msg local, a before b, pc from its own literal, &w82C written directly, sc without a local. */
int sub_08051DF4(int player)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *step = &d->step;
    if (*step == 0) {
        int m = -1;
        if (sub_08008AF8(player, m) == 0)
            return 1;
        if (player != 0) {
            int r = sub_080563B8(m, 1);
            if (r > m)
                sub_08017FF4(player, r);
            return 1;
        }
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08085FF4);
        (*step)++;
        return 0;
    }
    if (sub_08052F38(0xF0)) {
        struct DuelScreen *sc = &gUnk_0201CFB0;
        u32 *pa = &sc->w824;
        u32 *pb = &sc->w828;
        u32 *pc = pb + 1;
        u32 b = *pb + *pc;
        u32 a = *pa;
        sub_08077AEC(1);
        sub_0801EC58(player != 0 ? 0x8008 : 8, (u16)*pa, (((u8)*pc) << 8) | (u8)*pb, 0);
        sub_08017FF4(a, b);
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatching/code_08051A9C", sub_08051DF4); /* 0x08051DF4 size 0xDC */
#endif
#if 0 /* NONMATCHING: register allocation differs. The ROM puts id in r4
       * (reusing p's register) and msg in r5; the build has id in r5 and msg
       * in r4. */
int sub_08051ED0(int player)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *step = &d->step;
    s8 st = *step;
    switch (st) {
    case 0:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08086018);
        (*step)++;
        return 0;
    case 1:
        if (sub_08052F38(1)) {
            int p = 1 & player;
            u16 id = CARD_ID(((struct DuelP *)d)->players[p].hand[gUnk_0201CFB0.w82C]);
            if (sub_08054398(player, id) != 0 && sub_08007834(id) == 0) {
                u32 lvl;
                CARD_LEVEL(id, lvl);
                if (lvl <= 4) {
                    sub_0801EC58(player != 0 ? 0x80C4 : 0xC4, id,
                                 ((gUnk_0201CFB0.w82C & 0xF) << 4) | (sub_08008A44(player) & 0xF) | 0x200, 0);
                    gUnk_020192E0.step++;
                    return 0;
                }
            }
            sub_08077AEC(3);
        }
        return 0;
    default:
        return 1;
    }
}
#else
INCLUDE_ASM("asm/nonmatching/code_08051A9C", sub_08051ED0); /* 0x08051ED0 size 0x148 */
#endif
#if 0 /* NONMATCHING: the selector/card fields and switch branches agree, but
       * the duel-base lifetime adds r8, cursor/card pointer allocation
       * differs, and the local flag-store pointer absorbs +0xC into its byte
       * offset. */
struct PickCard { u32 id : 12; u32 flag12 : 1; u32 rest : 19; };
struct PickCursor { u8 pad0[5]; u8 sub : 2; u8 flags : 6; u16 index; u8 pad8[4]; struct PickCard cards[80]; };
extern struct PickCursor gUnk_0201D810;
extern struct PickCard gUnk_0201D81C[];
extern const u8 gUnk_08086058[], gUnk_080860A4[], gUnk_080860FC[];
int sub_08056ECC(u16 id);
void sub_0802AF34(s32 player, s32 skip, u16 number, u16 mode);
int sub_08052018(s32 player, u16 id, u16 mode)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *step = &d->step;
    struct PickCard *chosen;
    switch (*step) {
    case 0:
        if (player != 0) {
            chosen = &gUnk_0201D81C[sub_08056ECC(id)];
            break;
        }
        switch (((const u16 *)0x08622AB4)[id & 0x7FF]) {
        case 0x45C: sub_080602A4(0x206, 0x712, 0xB, gUnk_08086058); break;
        case 0x487: sub_080602A4(0x206, 0x712, 0xB, gUnk_080860A4); break;
        case 0x5F0: sub_080602A4(0x206, 0x712, 0xB, gUnk_080860FC); break;
        default: return 1;
        }
        d->step++;
        return 0;
    case 1:
        (*step)++;
        sub_0802AF34(player, -1, ((const u16 *)0x08622AB4)[id & 0x7FF], mode);
        return 0;
    default:
        chosen = &gUnk_0201D810.cards[gUnk_0201D810.sub + gUnk_0201D810.index];
        if (d->u0[0x1B12] & 2)
            gUnk_0201D810.cards[gUnk_0201D810.sub + gUnk_0201D810.index].flag12 =
                1 - ((*(u32 *)&gUnk_0201D810.cards[gUnk_0201D810.sub + gUnk_0201D810.index] << 19) >> 31);
        break;
    }
    d->sel = *(u32 *)chosen;
    d->sel2 = ((u16 *)chosen)[1];
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08051A9C", sub_08052018); /* 0x08052018 size 0x178 */
#if 0 /* NONMATCHING: only the allocation of the (u->b21 - u->h) value differs.
       * gcc's combine pass folds the first `y << 16` into `D << 19` (D = b21 -
       * h), so D is kept in a register (r5) and the timer pointer is computed
       * as r6+0x23 into r8. The ROM keeps D in r0, spills y and reloads it for
       * both rows (base r5, x r7, &timer r6). All 6 sub_080761F0 calls and the
       * palette/text block match. Tried: yh variable, inline y<<16, y*0x10000,
       * y as int, ybuf[1], y as u32*(base+..). */
void sub_08052190(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u32 x = (u->x + 1) << 3;
    u16 y = (u->b21 - u->h) << 3;
    u8 *t = &u->timer;
    u32 attr;
    if (*t == 0) {
        sub_08075294(0x050003E0, gUnk_0822C300, 0x20);
        sub_08074B08(0xC, 2);
        sub_0807501C(1, 1, 0xA0F, gUnk_0819D214[u->sel]);
        sub_0807501C(0, 0, 0xA02, gUnk_0819D214[u->sel]);
        sub_08075114(0x06016C80, 0);
        (*t)++;
    }
    attr = 0x364;
    sub_080761F0(x | (y << 16), 0x4040, attr | 0xF000);
    attr += 4;
    sub_080761F0((x + 0x20) | (y << 16), 0x4040, attr | 0xF000);
    attr += 4;
    sub_080761F0((y << 16) | (x + 0x40), 0x4040, attr | 0xF000);
    attr += 4;
    sub_080761F0(x | ((y + 8) << 16), 0x4040, attr | 0xF000);
    attr += 4;
    sub_080761F0((x + 0x20) | ((y + 8) << 16), 0x4040, attr | 0xF000);
    attr += 4;
    sub_080761F0((x + 0x40) | ((y + 8) << 16), 0x4040, attr | 0xF000);
}
#else
INCLUDE_ASM("asm/nonmatching/code_08051A9C", sub_08052190); /* 0x08052190 size 0x130 */
#endif
int sub_080522C0(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *t = &u->timer;
    if (*t == 0)
        return 0;
    if (u->state == 0) {
        u->state++;
        return 0;
    }
    /* gUnk_03000040.keys must be read in every test (no local): a local makes the
     * final `& 1` compile with the and's result in the key register. */
    if (gUnk_03000040.keys & 0x20) {
        u->sel += 0x13;
        u->sel = u->sel % 0x14;
        *t = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 0x10) {
        u->sel += 1;
        u->sel = u->sel % 0x14;
        *t = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 1)
        return 1;
    return 0;
}
int sub_0805232C(void)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *s = &d->step;
    if (*s == 0) {
        sub_080602A4(0x206, 0x40F, 0xB, gUnk_08086210);
        sub_08060308(5, (void (*)(void))sub_08052190, sub_080522C0);
        (*s)++;
        return 0;
    }
    d->sel = gUnk_0201AE60.sel;
    return 1;
}
void sub_08052390(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u32 x = (u->x + 1) << 3;
    int y = (u->b21 - u->h) << 3;
    u8 *t = &u->timer;
    if (*t == 0) {
        sub_08075294(0x050003E0, gUnk_0822C300, 0x20);
        sub_08074B08(8, 4);
        sub_0807501C(4, 0x12, 0xC0F, gUnk_0819D264[u->sel]);
        sub_0807501C(3, 0x11, 0xC01, gUnk_0819D264[u->sel]);
        sub_08075114(0x06016C80, 0);
        (*t)++;
    }
    sub_080761F0(x | ((y - 0x10) << 16), 0x40C0, 0xF364);
}
int sub_0805243C(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *t = &u->timer;
    if (*t == 0)
        return 0;
    if (u->state == 0) {
        u->state++;
        return 0;
    }
    if (gUnk_03000040.keys & 0x20) {
        u->sel += 5;
        u->sel = u->sel % 6;
        *t = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 0x10) {
        u->sel += 1;
        u->sel = u->sel % 6;
        *t = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 1)
        return 1;
    return 0;
}
int sub_080524A8(void)
{
    u16 prev;
    if (gUnk_0201AE60.timer == 0)
        return 0;
    if (gUnk_0201AE60.state == 0) {
        gUnk_0201AE60.state++;
        return 0;
    }
    if (gUnk_03000040.keys & 0x20) {
        struct Ui *p = &gUnk_0201AE60;
        prev = gUnk_0201AE44;
        do {
            p->sel += 5;
            p->sel = p->sel % 6;
        } while (p->sel == prev);
        gUnk_0201AE60.timer = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 0x10) {
        struct Ui *p = &gUnk_0201AE60;
        prev = gUnk_0201AE44;
        do {
            p->sel += 1;
            p->sel = p->sel % 6;
        } while (p->sel == prev);
        gUnk_0201AE60.timer = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 1) {
        if (gUnk_0201AE60.sel != gUnk_020192E0.sel)
            return 1;
    }
    return 0;
}
int sub_08052560(void)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *s = &d->step;
    if (*s == 0) {
        sub_080602A4(0x206, 0x40F, 0xB, gUnk_08086254);
        sub_08060308(5, (void (*)(void))sub_08052390, sub_0805243C);
        (*s)++;
        return 0;
    }
    d->sel = gUnk_0201AE60.sel;
    return 1;
}
int sub_080525C4(void)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *step = &d->step;
    /* A switch rather than an if/else chain keeps both step blocks out of line. */
    switch (*step) {
    case 0:
        sub_080602A4(0x206, 0x40F, 0xB, gUnk_08086254);
        sub_08060308(5, (void (*)(void))sub_08052390, sub_0805243C);
        (*step)++;
        return 0;
    case 1:
        d->sel = gUnk_0201AE60.sel;
        sub_080602A4(0x206, 0x411, 0xB, gUnk_0808626C);
        sub_08060308(5, (void (*)(void))sub_08052390, sub_080524A8);
        (*step)++;
        return 0;
    }
    d->sel2 = gUnk_0201AE60.sel;
    return 1;
}
#if 0 /* NONMATCHING: register allocation differs. The ROM has
       * `ldrb r0,[r7]; adds r4,r0,#0` (r0 preserved for the tail, r4 for the
       * switch and the `&`), and reloads state r0 in case 0 before the shared
       * `adds r0,#1; strb r0,[r7]` tail. The build loads state straight into
       * r4 and constant-folds the case 1 tail. The ROM also repurposes r5 as
       * the timer pointer in case 1 (`adds r5,#35`). Closest attempt: a single
       * s0 var, int t3 for case 1, reloading s0 in case 0, and an inline tail.
       * The remaining differences are `ldrb r4,[r7]` vs
       * `ldrb r0,[r7]; adds r4,r0,#0`, and the case 1 tail constant 2 vs
       * `adds r0,#1`. */
int sub_08052668(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *s = &u->state;
    s8 st = *s;
    switch (st) {
    case 0:
        if (u->timer <= 0x3F)
            u->sel = (u->timer >> 2) & 1;
        if (u->timer == 0x40)
            u->sel = sub_08076F9C() & 1;
        if (u->timer > 0xC0) {
            u->timer = 0;
            break;
        }
        u->timer++;
        return 0;
    case 1: {
        u8 v = u->timer;
        if (v <= 0x3B) {
            u->timer = v + 1;
            if ((gUnk_03000040.keysHeld & 2) || (*(u8 *)&gUnk_0201CFB0 & st)) {
                if (u->timer <= 0x33)
                    u->timer = v + 8;
            }
            return 0;
        }
        break;
    }
    case 2:
        return 1;
    }
    *s = *s + 1;
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatching/code_08051A9C", sub_08052668); /* 0x08052668 size 0xAC */
#endif
int sub_08052714(void)
{
    char buf[0x80];
    if (gUnk_020192E0.step == 0) {
        const char *const *tbl;
        u16 *p;
        int i;
        sub_080752D0(buf, gUnk_08086254);
        sub_080752E8(buf, gUnk_0808628C);
        tbl = gUnk_0819D264;
        p = gUnk_020192E0.h1B52;
        for (i = 1; i >= 0; i--) {
            sub_080752E8(buf, gUnk_08086290);
            sub_080752E8(buf, tbl[*p]);
            sub_080752E8(buf, gUnk_0808628C);
            p++;
        }
        sub_080602A4(0x206, 0x50F, 0xB, buf);
        if (gUnk_020192E0.b1B50 & 4) {
            sub_08060308(5, sub_0805FBA4, sub_08052668);
        } else {
            sub_08060308(2, 0, 0);
            gUnk_020192E0.step++;
        }
        gUnk_020192E0.step++;
        return 0;
    }
    gUnk_020192E0.sel = gUnk_020192E0.h1B52[gUnk_0201AE60.sel];
    return 1;
}
#if 0 /* NONMATCHING: first path builds (r << 8 | 0xB) then narrows to u16 (ROM folds to (r << 24 | 0xB0000) >> 16 and stores sel first); the bitfield byte is addressed [r1+0xD71] instead of [r1+0xD68]+9; w824/w828/w82C address temporaries differ */
int sub_08052810(int a)
{
    if (a != 0) {
        u16 r = sub_080578F4();
        gUnk_020192E0.sel = r;
        sub_0801EC58(0x8008, 0, ((r << 8) | 0xB) & 0xFFFF, 0);
        return 1;
    } else {
        struct Duel *d = &gUnk_020192E0;
        u8 *step = &d->step;
        switch (*step) {
        case 0:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08086298);
            (*step)++;
            return 0;
        case 1:
            ((struct DuelQ *)d)->q1.f0 = 1;
            if (sub_08052F38(0x10000)) {
                struct DuelScreen *sc;
                sub_08077AEC(1);
                sc = &gUnk_0201CFB0;
                sub_0801EC58(8, (u16)sc->w824, (((u8)sc->w82C) << 8) | (u8)sc->w828, 0);
                (*step)++;
            }
            return 0;
        default:
            ((struct DuelQ *)d)->q1.f0 = 0;
            d->sel = gUnk_0201CFB0.w82C;
            return 1;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatching/code_08051A9C", sub_08052810); /* 0x08052810 size 0xF8 */
#endif
#if 0 /* NONMATCHING: the eligibility logic and attachment scan are typed to
       * follow the ROM, but compiler allocation and loop structure differ.
       * Player parity, 16-bit mask banks, flag tests and matching attachment
       * kinds 1/5/6 are verified against the ROM. */
struct PickZone {
    u32 card;
    u8 pad4[2];
    u8 flags;
    u8 pad7[3];
    u16 kinds[32];
    u16 links[32];
    s16 count;
    u8 pad8C[8];
};
extern u8 gUnk_0201930C[];
int sub_08052908(u32 player, s32 kind, s32 slot, u32 mask)
{
    u32 playerOffset = (player & 1) * 0xD64;
    struct PickZone *zone = (struct PickZone *)(gUnk_0201930C + playerOffset + (kind + slot) * 0x94);
    u32 packed = zone->card << 20;
    s16 type = (gUnk_08621DE0[(packed << 1) >> 21] & 0x1F00000) >> 20;
    s16 shift = player * 16;
    switch (kind) {
    case 0: {
        int mode = 0;
        int face = 0;
        if (packed == 0) return 0;
        if (!((0xF0U << shift) & mask)) return 0;
        if (((0x20U << shift) & mask) && (zone->flags & 2)) mode = 1;
        if (((0x10U << shift) & mask) && !(zone->flags & 2)) mode = 1;
        if (((0x80U << shift) & mask) && (zone->flags & 1)) face = 1;
        if (((0x40U << shift) & mask) && !(zone->flags & 1)) face = 1;
        if (face != 0 && mode != 0) return 1;
        return 0;
    }
    case 5: {
        u32 selected = (0xEU << shift) & mask;
        s8 p, i;
        if (selected == 0 || packed == 0) return 0;
        if (!(zone->flags & 2)) return ((2U << shift) & mask) != 0;
        if (selected == (2U << shift)) return 0;
        if (type == 0x15) return ((8U << shift) & mask) != 0;
        if (type == 0x16) return ((4U << shift) & mask) != 0;
        for (p = 0; p <= 1; p++) {
            struct PickZone *z = (struct PickZone *)(gUnk_0201930C + (p & 1) * 0xD64);
            for (i = 0; i <= 4; i++, z++) {
                s8 k;
                if ((z->card << 20) == 0 || !(z->flags & 2)) continue;
                for (k = 0; k < z->count; k++) {
                    s8 entry = z->kinds[k];
                    switch (entry) {
                    case 1:
                    case 5:
                    case 6:
                        if (z->links[k] == ((u16)((slot + 5) << 8) | (u8)player)) return 1;
                        break;
                    }
                }
            }
        }
        return 0;
    }
    case 10: {
        struct PickZone *z = (struct PickZone *)(gUnk_0201930C + playerOffset + 10 * 0x94);
        u32 ret = 0;
        if ((z->card << 20) == 0) return 0;
        if (z->flags & 2) {
            int bits = (4U << shift) & mask;
            ret = ((0U - bits) | bits) >> 31;
        } else if ((2U << shift) & mask) ret = 1;
        if (ret != 0) return 1;
        return 0;
    }
    case 11:
        if (!((1U << shift) & mask)) return 0;
        if (slot < *((u8 *)gUnk_0201930C - 0x28 + playerOffset + 2)) return 1;
        return 0;
    default: return 0;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08051A9C", sub_08052908); /* 0x08052908 size 0x270 */
