#include "global.h"

#include "gba.h"

/*
 * Password scene (CB 0x0807CC28, step table 0x081A7970) and Card Trading
 * scene (CB 0x0807D348, step table 0x081A79A4). See wiki/functions/code-0807c7c8.md.
 */

/* Main state at 0x03000040: only the fields used here. */
struct Main {
    u8 filler0[6];
    u16 keyNew;                 /* +0x06 newly pressed keys */
    u8 filler8[0x40E - 8];
    u16 unk40E;
    u8 filler410[4];
    u32 unk414;
    u8 filler418[0x85C - 0x418];
    u16 tilemap[0x60];          /* +0x85C */
    u8 filler91C[0x442C - 0x91C];
    u16 unk442C;
    u8 filler442E[0x4859 - 0x442E];
    u8 step;                    /* +0x4859 scene step index */
    u8 sub;                     /* +0x485A sub-step */
    u8 sub2;                    /* +0x485B */
    u8 filler485C[0x16];
    u16 unk4872;
    u8 unk4874_0:2;
};

/* Password / trade scene state at 0x0201F780. */
struct PwState {
    u16 code;                   /* +0x00 */
    u16 a:1;                    /* +0x02 */
    u16 b:1;
    u16 c:7;                    /* bits 2-8 */
    u16 d:4;                    /* bits 9-12 */
    u16 e:3;
    u8 filler4[0xC];
    u8 link[0x4];               /* +0x10 link sync tx entry */
    u8 linkRx[2];               /* +0x14 link rx entry */
    u16 peerData;               /* +0x16 data received from the peer */
    u8 link18[6];
    u16 timer;                  /* +0x1E */
    u16 card;                   /* +0x20 */
    u8 unk22;
    u8 filler23[5];
};

/* Link / password menu state at 0x0201F7B0 (size 0x18), see code_0807B6B8. */
struct LinkState {
    u8 digits[8];               /* +0x00 */
    u8 pw[8];                   /* +0x08 */
    u8 slot:4;                  /* +0x10 selected digit slot */
    u8 cursor:4;                /* +0x10 cursor entry */
    u8 a:6;                     /* +0x11 */
    u32 b:6;
    u16 c:12;                   /* +0x12 (>> 4) frame counter */
    u16 card;                   /* +0x14 */
    u8 filler16[2];
};

/* Save mirror at 0x02011C20: per-card entries of 4 bytes starting at +0xA. */
struct SaveCard {
    u8 filler0[2];
    u8 flag0:1;
    u8 flag1:1;
    u8 filler[1];
};
struct SaveMirror {
    u8 filler0[8];
    struct SaveCard card[1];
};

extern struct Main gUnk_03000040;
extern struct LinkState gUnk_0201F7B0;
extern struct SaveMirror gUnk_02011C20;
int sub_08076F9C(void);
void sub_08075278(void *dst, u32 size);
void sub_08075294(void *dest, const void *src, u32 size);
void sub_08077AEC(int);
void sub_0807BF68(void);
void sub_08073498(void);
void sub_08073574(void);
void sub_08075630(void);
void sub_080759F4(void);
void sub_080757AC(void);
void sub_080731D0(u32 a, u32 b, u32 c, const void *d);
int sub_080753E0(const void *p);
void sub_0807501C(int x, int y, u16 attr, const void *str);
void sub_08075114(void *dest, u16 v);
void sub_08074B08(u8 a, u8 b);
struct Hblank {
    u32 filler0;
    void *cb;
};
extern struct Hblank gUnk_03000000;
extern const u8 gUnk_0870B5E0[], gUnk_0870B600[], gUnk_0870C620[];
extern const u8 gUnk_087095E0[], gUnk_0870A5E0[], gUnk_0870B620[];
extern const u8 gUnk_0822C300[], gUnk_0822C720[][64], gUnk_08707B28[];
void sub_080761F0(u32 a, u32 b, u16 c);
void sub_08076714(u32 a, u32 b, u32 c, u32 d);
struct Pair16 {
    u16 lo;
    u16 hi;
};
extern const u32 gUnk_08087E88[];
extern const struct Pair16 gUnk_08087F08[];
void sub_0807BFE0(void);
void sub_0807C1D0(u16 id);
extern struct PwState gUnk_0201F780;
u16 sub_08075A6C(int);
u16 sub_08075AE4(int);
u16 sub_08075A6C(int);
u16 sub_08075AE4(int);
void sub_0807CCAC(int a, u16 b, int c);
u32 sub_0807D348(void);
void sub_0800688C(u16 card, int a, int b);
u16 sub_08006D08(void);
u16 sub_0806F01C(void);
void sub_0807BCF4(u8 *);
u32 sub_0807BCFC(u16 id, u16 data, void *p);
u32 sub_0807BE60(void *a, void *b);
void sub_08077498(u16);
void sub_0807761C(u16);
void sub_080754BC(void);
extern const u16 gUnk_08622AB4[];
extern const u16 gUnk_08623DF4[];
void sub_0801A7DC(const void *);
void sub_0801A7E8(void);
extern const u8 gUnk_08087FA0[];
extern u16 (*const gUnk_081A7970[])(void);
extern u16 (*const gUnk_081A79A4[])(void);

/* Second view of the a/b flag pair as one 2-bit value. */
struct PwPair {
    u16 filler0;
    u16 ab:2;
    u16 rest:14;
    u8 filler4[0x24];
};

u32 sub_0807C7C8(void)
{
    struct LinkState *s = &gUnk_0201F7B0;

    s->a = 0x20;
    s->b = 0x20;
    sub_0807BF68();
    sub_0807BFE0();
    if (gUnk_03000040.keyNew & 1)
        s->c = 0xB4;
    if (s->c++ <= 0xB3) {
                int i;
        u32 base;

        i = 0;
        base = (u32)s;

        for (; i < 8; i++)
            *(u8 *)(i + base) = sub_08076F9C() % 10;
        gUnk_0201F7B0.cursor = sub_08076F9C() % 10;
        gUnk_0201F7B0.slot = sub_08076F9C() & 7;
        if ((gUnk_0201F7B0.c & 0xF) == 8)
            sub_08077AEC(0x27);
    } else {
        s->c = 0;
        s->a = 0;
        s->b = 0;
        if (s->card == 0) {
            sub_08075278(s, 8);
            gUnk_03000040.step += 3;
        } else if (gUnk_02011C20.card[s->card].flag1 == 0) {
            sub_08075294(s, s->pw, 8);
            sub_0807C1D0(s->card);
            gUnk_03000040.unk442C = 0;
            REG_BG2HOFS = 0;
            return 1;
        } else {
            sub_08075278(s, 8);
            gUnk_03000040.step += 6;
        }
    }
    return 0;
}

u32 sub_0807C924(void)
{
    if (gUnk_03000040.keyNew & 1)
        gUnk_0201F7B0.c = 0x12C;
    if (gUnk_0201F7B0.c <= 0x12B) {
        gUnk_0201F7B0.a++;
        if ((gUnk_0201F7B0.a & 0x1F) == 0x1F)
            sub_08077AEC(0x29);
        if ((gUnk_0201F7B0.a & 0x1F) <= 0x1C)
            sub_0807BF68();
        if (gUnk_0201F7B0.c <= 0x1F) {
            gUnk_03000040.unk442C = gUnk_08087E88[gUnk_0201F7B0.c];
            sub_080761F0(0x1C0070, 0, gUnk_08087F08[gUnk_0201F7B0.c].lo);
            sub_080761F0(0x4C0070, 0, gUnk_08087F08[gUnk_0201F7B0.c].lo);
            sub_080761F0(0x7C0070, 0, gUnk_08087F08[gUnk_0201F7B0.c].lo);
        }
        gUnk_0201F7B0.c++;
        return 0;
    }
    switch (gUnk_03000040.sub) {
    case 0:
        sub_0807BF68();
        if (gUnk_03000040.keyNew & 3)
            gUnk_03000040.sub++;
        return 0;
    case 1:
        sub_0807BF68();
        if (sub_08075A6C(4)) {
            gUnk_02011C20.card[gUnk_0201F7B0.card].flag1 = 1;
            sub_08077498(gUnk_0201F7B0.card);
            sub_0800688C(gUnk_0201F7B0.card, 0, 0);
            gUnk_03000040.sub++;
        }
        return 0;
    case 2:
        if (sub_08006D08())
            gUnk_03000040.sub++;
        return 0;
    default:
        return 1;
    }
}
u32 sub_0807CAA0(void)
{
    if (gUnk_03000040.keyNew & 1)
        gUnk_0201F7B0.c = 0x12C;
    if (gUnk_0201F7B0.c <= 0x12B) {
        if (gUnk_0201F7B0.c <= 0xB3) {
            gUnk_0201F7B0.a++;
            if ((gUnk_0201F7B0.a & 0x1F) == 0x1F)
                sub_08077AEC(0x28);
        }
        if (gUnk_0201F7B0.a & 0x20) {
            sub_080761F0(0x80018, 0x40, 0x10CA);
            sub_080761F0(0x80028, 0x4080, 0x10CC);
        }
        if (!(gUnk_03000040.keyNew & 8)) {
            gUnk_0201F7B0.c++;
            return 0;
        }
    }
    return 1;
}

u16 sub_0807CB64(void)
{
    return sub_08075A6C(2);
}
u32 sub_0807CB74(void)
{
    if (gUnk_03000040.keyNew & 1)
        gUnk_0201F7B0.c = 0x12C;
    if (gUnk_0201F7B0.c <= 0x12B) {
        if (gUnk_0201F7B0.c <= 0xB3) {
            gUnk_0201F7B0.a++;
            if ((gUnk_0201F7B0.a & 0x1F) == 0x1F)
                sub_08077AEC(0x28);
        }
        if (gUnk_0201F7B0.a & 0x20)
            sub_080761F0(0x80028, 0x4080, 0x110B);
        if (!(gUnk_03000040.keyNew & 8)) {
            gUnk_0201F7B0.c++;
            return 0;
        }
    }
    return 1;
}
/* Password scene callback: runs the current step of the table. */
u32 sub_0807CC28(void)
{
    u16 (*fn)(void) = gUnk_081A7970[gUnk_03000040.step];

    if (fn) {
        if (fn()) {
            gUnk_03000040.step++;
            gUnk_03000040.sub = 0;
            gUnk_03000040.sub2 = 0;
        }
        return 0;
    }
    return 1;
}
/* Copies `rows` rows of `width` tiles into VRAM 0x06010000 (stride 0x400 bytes). */
void sub_0807CC78(const void *src, int tile, int width, int rows)
{
    u8 *dst = (u8 *)0x06010000 + tile * 32;
    int i;

    for (i = 0; i < rows; i++) {
        sub_08075294(dst, src, width * 32);
        dst += 0x400;
        src = (const u8 *)src + width * 32;
    }
}
#if 0 /* NONMATCHING: loop matches; differs in temp register choice (limit/a init, constant temps in the sub_08076714 arg setup) */
void sub_0807CCAC(int a, u16 b, int c)
{
    int i, j, limit;

    if (b) {
        limit = 2;
        a &= 1;
    } else {
        limit = 1;
        a = 0;
    }
    i = 0;
    while (i < limit) {
        u8 iy, next;
        u32 yx;

        j = 0;
        iy = i << 7;
        next = i + 1;
        yx = (i << 21) + 0x300000;
        for (; j < 4; j++) {
            int on = 0;
            int x, y;

            if (i == a)
                on = 1;
            x = 0;
            if (on == 0)
                x = 0x10;
            y = 0;
            if (on == 0)
                y = 0x1000;
            sub_080761F0(yx | (j << 5), 0x80, (x + j * 4 + iy) | y);
        }
        i = next;
    }
    if (b) {
        sub_08076714(0x380098, 0x80, ((c & 0xC) + 0x100) | 0x2000, ((c << 21) + 0x1000000) | gUnk_0201F780.c);
        if (c == 0)
            gUnk_0201F780.c = gUnk_0201F780.c + 1;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0807C7C8", sub_0807CCAC); /* 0x0807CCAC size 0xF4 */
u32 sub_0807CDA0(void)
{
    sub_08075278(&gUnk_0201F780, 0x28);
    return 1;
}
u32 sub_0807CDB4(void)
{
    int len;
    u8 w;
    int half;
    int extent;
    int x;

    gUnk_03000040.unk40E = 3;
    REG_DISPCNT = 0;
    REG_BG0CNT = 4;
    REG_BG1CNT = 0x105;
    sub_08073574();
    sub_08073498();
    sub_08075630();
    REG_MOSAIC = 0;
    sub_080759F4();
    sub_080757AC();
    gUnk_03000040.unk414 = 0;
    REG_IME = 0;
    REG_IE &= ~2;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= ~2;
    gUnk_03000000.cb = 0;
    REG_IME = 1;
    sub_08075294((void *)0x05000200, gUnk_0870B5E0, 0x20);
    sub_08075294((void *)0x05000220, gUnk_0870B600, 0x20);
    sub_08075294((void *)0x05000240, gUnk_0870C620, 0x20);
    sub_0807CC78(gUnk_087095E0, 0, 0x10, 8);
    sub_0807CC78(gUnk_0870A5E0, 0x10, 0x10, 8);
    sub_0807CC78(gUnk_0870B620, 0x100, 0x10, 8);
    sub_08075294((void *)0x05000000, gUnk_0822C300, 0x20);
    sub_080731D0(0, 0x10, 0x200, gUnk_08707B28);
    if (gUnk_0201F780.code != 0) {
        int i;

        len = sub_080753E0(gUnk_0822C720[gUnk_0201F780.code]);
        w = 12;
        if (len > 0x12)
            w = 10;
        sub_08074B08(0x20, 3);
        extent = (len * w) >> 1;
        x = 0x78 - extent;
        half = w >> 1;
        sub_0807501C(x, 0xD - half, (w << 8) | 8, gUnk_0822C720[gUnk_0201F780.code]);
        sub_0807501C(0x77 - extent, 0xC - half, (w << 8) | 7, gUnk_0822C720[gUnk_0201F780.code]);
        sub_08075114((void *)0x06005000, 0);
        for (i = 0; i < 0x60; i++)
            gUnk_03000040.tilemap[i] = 0x80 + i;
    }
    return 1;
}

u32 sub_0807CF6C(void)
{
    sub_0807CCAC(gUnk_0201F780.b, gUnk_0201F780.a, gUnk_0201F780.d);
    REG_DISPCNT |= 0x1300;
    return sub_08075AE4(2);
}
u32 sub_0807CFA8(void)
{
    struct PwState *st = &gUnk_0201F780;

    sub_0807CCAC(st->b, st->a, st->d);
    if (gUnk_03000040.keyNew & 2) {
        sub_08077AEC(2);
        gUnk_03000040.step = 4;
    } else if (gUnk_03000040.keyNew & 1) {
        if (((struct PwPair *)st)->ab != 3)
            gUnk_03000040.step = 6;
        else
            gUnk_03000040.step = 8;
        sub_08077AEC(1);
    } else if (gUnk_03000040.keyNew & 0xC0) {
        if (st->a) {
            sub_08077AEC(0);
            st->b = 1 - st->b;
        } else {
            sub_08077AEC(3);
        }
    }
    return 0;
}
u32 sub_0807D060(void)
{
    return 0;
}
u32 sub_0807D064(void)
{
    sub_0807CCAC(gUnk_0201F780.b, gUnk_0201F780.a, gUnk_0201F780.d);
    if (sub_08075A6C(2) != 0) {
        REG_DISPCNT = 0;
        return 1;
    }
    return 0;
}
u32 sub_0807D0A0(void)
{
    if (sub_0806F01C()) {
        if (gUnk_03000040.unk4872 != 0) {
            gUnk_0201F780.code = gUnk_03000040.unk4872;
            gUnk_0201F780.a = 1;
            gUnk_0201F780.b = 1;
        } else {
            gUnk_0201F780.code = 0;
            gUnk_0201F780.a = 0;
            gUnk_0201F780.b = 0;
        }
        gUnk_03000040.step = 1;
        gUnk_03000040.sub = 0;
        gUnk_03000040.sub2 = 0;
    }
    return 0;
}
u32 sub_0807D118(void)
{
    struct PwState *st = &gUnk_0201F780;

    sub_0807CCAC(st->b, st->a, st->d);
    if (st->c != 0x60) {
        if (st->c <= 0x5B)
            st->c += 4;
    } else {
        if (st->d > 14) {
            sub_0807BCF4(st->link);
            sub_0801A7DC(gUnk_08087FA0);
            sub_0801A7E8();
            st->timer = 0x200;
            return 1;
        }
        st->d++;
    }
    return 0;
}
u32 sub_0807D1AC(void)
{
    sub_0807CCAC(gUnk_0201F780.b, gUnk_0201F780.a, gUnk_0201F780.d);
    if (gUnk_0201F780.d > 1) {
        gUnk_0201F780.d--;
        return 0;
    }
    return 1;
}
u32 sub_0807D1F4(void)
{
    sub_0807CCAC(gUnk_0201F780.b, gUnk_0201F780.a, gUnk_0201F780.d);
    if (sub_0807BCFC(gUnk_0201F780.unk22, ((const u16 *)0x08622AB4)[gUnk_0201F780.code & 0x7FF], gUnk_0201F780.link)) {
        u16 id;
        u32 n;

        sub_0807BE60(gUnk_0201F780.filler4, gUnk_0201F780.linkRx);
        id = gUnk_0201F780.peerData;
        if (id == 0xFFFF)
            n = 0;
        else if (id < 2000)
            n = ((const u16 *)0x08623DF4)[id & 0x7FF];
        else
            n = ((const u16 *)0x08623DF4)[(id - 2000) & 0x7FF] + 1;
        gUnk_0201F780.card = n;
        sub_08077498(gUnk_0201F780.card);
        sub_0807761C(gUnk_0201F780.code);
        sub_080754BC();
        return 1;
    }
    if (--gUnk_0201F780.timer == 0)
        gUnk_03000040.step = 0xE;
    return 0;
}
u32 sub_0807D2D0(void)
{
    switch (gUnk_03000040.sub) {
    case 0:
        sub_0800688C(gUnk_0201F780.card, 0, 0);
        gUnk_03000040.sub++;
        break;
    case 1:
        if (sub_08006D08())
            gUnk_03000040.sub++;
        break;
    default:
        gUnk_0201F780.code = 0;
        gUnk_0201F780.b = 0;
        gUnk_0201F780.a = 0;
        gUnk_0201F780.d = 0;
        gUnk_03000040.step = 1;
        break;
    }
    return 0;
}
/* Card Trading scene callback. */
u32 sub_0807D348(void)
{
    gUnk_0201F780.unk22 = 0x50;
    gUnk_03000040.unk4874_0 = 1;
    {
        u16 (*fn)(void) = gUnk_081A79A4[gUnk_03000040.step];

        if (fn) {
            if (fn()) {
                gUnk_03000040.step++;
                gUnk_03000040.sub = 0;
                gUnk_03000040.sub2 = 0;
            }
            return 0;
        }
        return 1;
    }
}
u16 sub_0807D3C0(void)
{
    return sub_0807D348();
}
