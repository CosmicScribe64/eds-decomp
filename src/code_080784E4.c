#include "global.h"
#include "gba.h"

struct AnimSeq {            /* 8-byte step of an animation script */
    u8 frames;              /* +0 duration (0 = end) */
    u8 unk1;
    u8 pad[2];
    u32 data;               /* +4 */
};
struct AnimState {          /* 0x14 bytes */
    struct AnimSeq *seq;    /* +0 */
    u32 data;               /* +4 */
    u16 unk8;
    u16 unkA;
    u8 unkC;                /* +C */
    u8 idx;                 /* +D current step */
    u8 active;              /* +E */
    u8 timer;               /* +F */
    u8 prio;                /* +0x10 */
    u8 pad[3];
};

extern void sub_08077ED4(void *p, u8 a, u8 b, u8 c, u32 d);
extern void sub_080786D0(struct AnimState *st);
extern void sub_0807B4C0(u32 v);
extern void sub_08074E20(u16 ch, s32 x, s32 y, u16 sc);
extern int sub_08072584(u16 sjis);
extern u16 sub_080725B0(u32 nib, u16 a, u16 b);
extern void sub_08074B08(u8 a, u8 b);
extern void sub_0807501C(s32 x, s32 y, u16 attr, const u8 *str);
extern void sub_08075114(void *dest, u16 b);
extern void sub_08079FDC(u8 *dst, u8 *src, u8 pal, u8 n);
extern void *memcpy(void *dst, const void *src, unsigned int n);
extern void sub_08078FD4(u16 ch, u16 *dst, u16 a, u16 b);
extern void sub_08078ED4(u8 ch, u16 *dst, u16 a, u16 b, u16 mode);
extern void sub_080792A0(u16 start, u16 *dst, u8 pal, u8 mode, u8 count);
extern void sub_08077EF4(u32 data, u8 a, u8 c1, u16 d8, u16 dA, u8 mode, u8 b, u8 c, u8 d, u8 e, u8 zero, u32 last);
extern u8 gUnk_02000000[];
extern u8 gUnk_02011C20[];
extern u8 gUnk_08087B94[];

struct Fade {
    u8 kind;            /* +0 */
    u8 pad1;
    u16 level;          /* +2, 8.8 fixed, 0..0x1000 */
    s16 step;           /* +4 */
    u8 state;           /* +6 1 = running, 2 = full, 3 = zero */
    u8 unk7;
};

struct SpriteList {
    u8 pad0[4];
    u8 *items;              /* +4, array of 8-byte entries */
    u8 pad8[4];
    u8 count;               /* +C */
};
void sub_080784E4(struct SpriteList *l, u8 a, u32 unused, u8 c, u8 d, u32 e)
{
    u8 i;

    for (i = 0; i < l->count; i++)
        sub_08077ED4(l->items + i * 8, a, c, d, e);
}
/* Draw/update every active animation state of a block; returns int (callers ignore it, but the ROM epilogue pops r1). */
int sub_08078534(u8 *list, u8 a, u8 b, u8 c, u8 d, u8 e, u8 mode, u16 g, u16 h, u32 last)
{
    u8 i;
    struct AnimState *st;

    switch (mode) {
    case 1:
    case 2:
    case 4:
    case 8: {
        int zero = 0;
        for (i = 0; i < *(u16 *)(list + 0x190); i++) {
            st = (struct AnimState *)(list + i * 0x14);
            if ((s8)st->active != -1) {
                st->unk8 = g;
                st->unkA = h;
                sub_08077EF4(st->data, a, st->unkC, st->unk8, st->unkA, mode, b, c, d, e, zero, last);
            }
        }
        break;
    }
    default:
        for (i = 0; i < *(u16 *)(list + 0x190); i++) {
            st = (struct AnimState *)(list + i * 0x14);
            if ((s8)st->active != -1) {
                sub_08077EF4(st->data, a, st->unkC, st->unk8, st->unkA, mode, b, c, d, e, 0, last);
            }
        }
        break;
    }
}
u8 sub_08078670(struct AnimSeq **list, u8 *base)
{
    u8 i = 0;
    struct AnimState *st;
    struct AnimSeq *sq;

    do {
        st = (struct AnimState *)(base + i * 0x14);
        sq = *list++;
        st->seq = sq;
        st->data = sq->data;
        st->unkC = sq->unk1;
        st->unk8 |= 0xFFFF;
        st->unkA |= 0xFFFF;
        st->idx = 0;
        st->active = 1;
        st->timer = sq->frames;
        st->prio = 0x13 - i;
        i++;
    } while (*list != 0);
    *(u16 *)(base + 0x190) = i;
    return i;
}
void sub_080786D0(struct AnimState *arg)
{
    /* FAKEMATCH: retain the ROM's state/index registers and separate byte mask. */
    register struct AnimState *st __asm__("r1") = arg;
    struct AnimSeq *sq;
    u32 next;
    register u32 i __asm__("r3");
    u32 t;
    u32 mask;

    if (st->active == 1) {
        t = st->timer - 1;
        st->timer = t;
        mask = 0xFF;
        __asm__ __volatile__("" : "+r"(mask));
        if ((u8)t == 0xFF) {
            next = st->idx + 1;
            st->idx = next;
            next &= mask;
            sq = st->seq;
            if (sq[next].frames == 0) {
                st->idx = 0;
                st->active = 0;
            }
            i = st->idx;
            st->timer = sq[i].frames;
            st->data = sq[i].data;
            st->unkC = sq[i].unk1;
            /* Keep the unscaled index live through the final sequence-byte load. */
            __asm__ __volatile__("" : : "r"(i));
        }
    }
}

void sub_0807871C(u8 *base)
{
    u8 i;

    for (i = 0; i < *(u16 *)(base + 0x190); i++)
        sub_080786D0((struct AnimState *)(base + i * 0x14));
}

void sub_08078748(u8 *src, u8 *dst, u16 mode, u8 n, u8 rows)
{
    u16 i;

    if (mode != 0x10) {
        if (mode == 0x100) {
            for (i = 0; i < n; i++) {
                CpuSet(src, dst, (rows << 5) & 0x1FFFFF);
                src += 0x200;
                dst += 0x400;
            }
        }
    } else {
        for (i = 0; i < n; i++) {
            CpuSet(src, dst, (rows << 4) & 0x1FFFFF);
            src += 0x200;
            dst += 0x400;
        }
    }
}

void sub_080787F4(u8 kind, s16 step, u8 c, struct Fade *f)
{
    u16 level;
    u32 bld;

    f->kind = kind;
    if (step >= 0)
        f->level = 0;
    else
        f->level = 0x1000;
    f->step = step;
    f->state = 1;
    f->unk7 = c;
    *(vu16 *)0x04000054 = f->level >> 8;
    bld = 0xBF;
    if (kind == 0)
        bld = 0xFF;
    *(vu16 *)0x04000050 = bld;
}
u32 sub_0807883C(struct Fade *f)
{
    if (f->state == 1 && f->step != 0) {
        f->level += f->step;
        if (f->step > 0) {
            if (f->level > 0x1000) {
                f->level = 0x1000;
                f->step = 0;
                f->state = 2;
                sub_0807B4C0(0x10);
                return 1;
            }
        } else {
            if (f->level > 0x1000) {
                f->level = 0;
                f->step = 0;
                f->state = 3;
                sub_0807B4C0(0);
                return 1;
            }
        }
        sub_0807B4C0(f->level >> 8);
    }
    return 0;
}
void sub_080788A0(u8 *p)
{
    s32 m = ~1;

    *p = *p & m;
}
extern u8 gUnk_0822BB00[];
/* Render one 8x8 glyph (font 0x0822BB00, 8 bytes each) into 8 4bpp words with a drop shadow:
 * glyph bits get colour a and mark buf[row + 1]; buf[row] bits left unset get colour b.
 * The loop bound lives in a variable (n = 8): reload substitutes its constant, which keeps the
 * ROM's unfolded `cmp #8; bcc` (a literal 8 is canonicalised to `cmp #7; bls`). */
void sub_080788AC(u32 *dst, u8 ch, u8 a, u8 b, u8 *flags)
{
    u8 buf[12];
    u32 zero;
    u32 c4, c8, c12, c16, c20, c24, c28;
    u32 one = 1;
    u32 w;
    u8 *g;
    u16 i;
    u16 n = 8;

    c4 = (u32)a << 4;
    c8 = (u32)a << 8;
    c12 = (u32)a << 12;
    c16 = (u32)a << 16;
    c20 = (u32)a << 20;
    c24 = (u32)a << 24;
    c28 = (u32)a << 28;
    zero = 0;
    CpuSet(&zero, buf, 0x05000003);
    g = gUnk_0822BB00;
    if (ch > 0x9F) {
        ch -= 0x20;
        if (!(*flags & one))
            ch += 0x40;
    }
    g += ch * (one << 3);
    w = *dst;
    if (*g & 1) { w = (w & ~0xF) | a; buf[1] |= 2; }
    if (*g & 2) { w = (w & ~0xF0) | c4; buf[1] |= 4; }
    if (*g & 4) { w = (w & ~0xF00) | c8; buf[1] |= 8; }
    if (*g & 8) { w = (w & ~0xF000) | c12; buf[1] |= 0x10; }
    if (*g & 0x10) { w = (w & ~0xF0000) | c16; buf[1] |= 0x20; }
    if (*g & 0x20) { w = (w & ~0xF00000) | c20; buf[1] |= 0x40; }
    if (*g & 0x40) { w = (w & ~0xF000000) | c24; buf[1] |= 0x80; }
    if (*g & 0x80) { w = (w & ~0xF0000000) | c28; }
    *dst++ = w;
    g++;
    for (i = 1; i < n; g++, i++) {
        w = *dst;
        if (*g & 1) { w = (w & ~0xF) | a; buf[i + 1] |= 2; }
        if (*g & 2) { w = (w & ~0xF0) | c4; buf[i + 1] |= 4; }
        else if (buf[i] & 2) { w = (w & ~0xF0) | ((u32)b << 4); }
        if (*g & 4) { w = (w & ~0xF00) | c8; buf[i + 1] |= 8; }
        else if (buf[i] & 4) { w = (w & ~0xF00) | ((u32)b << 8); }
        if (*g & 8) { w = (w & ~0xF000) | c12; buf[i + 1] |= 0x10; }
        else if (buf[i] & 8) { w = (w & ~0xF000) | ((u32)b << 12); }
        if (*g & 0x10) { w = (w & ~0xF0000) | c16; buf[i + 1] |= 0x20; }
        else if (buf[i] & 0x10) { w = (w & ~0xF0000) | ((u32)b << 16); }
        if (*g & 0x20) { w = (w & ~0xF00000) | c20; buf[i + 1] |= 0x40; }
        else if (buf[i] & 0x20) { w = (w & ~0xF00000) | ((u32)b << 20); }
        if (*g & 0x40) { w = (w & ~0xF000000) | c24; buf[i + 1] |= 0x80; }
        else if (buf[i] & 0x40) { w = (w & ~0xF000000) | ((u32)b << 24); }
        if (*g & 0x80) { w = (w & ~0xF0000000) | c28; }
        else if (buf[i] & 0x80) { w = (w & ~0xF0000000) | ((u32)b << 28); }
        *dst++ = w;
    }
}
s16 sub_08078C48(u8 **pp)
{
    u16 val = 0;
    u8 *p = *pp;
    u16 sign = 1;

    while (*p != 0x2C && *p != 0x29 && *p != 0x2D) {
        u8 c = p[0];
        if (c == 0x2D) {
            sign = -1;
        } else {
            val = c - 0x30 + val * 10;
            p++;
        }
    }
    p++;
    *pp = p;
    return (s16)sign * val;
}
struct Bf {
    u8 flag : 1;
    u8 rest : 7;
    u8 pad[3];
    u32 a;
    u8 b[0x18];
};
void sub_08078CA8(u32 a, u8 *q)
{
    struct Bf *p = (struct Bf *)q;
    u8 z, two;

    p->a = a;
    p->flag = 0;
    z = 0;
    two = 2;
    q[0x10] = two;
    q[8] = z;
    q[0x1C] = two;
    q[0xD] = z;
    q[0xC] = z;
    q[0x1D] = z;
}
/* Composite a nibble-per-pixel glyph buffer (EWRAM 0x02000000, width byte at +0x10000) onto solid colour `color`, writing 4bpp tile data. */
void sub_08078CC8(u16 *dst, u16 color, u8 x)
{
    s32 i, next;
    s32 w;
    u8 *buf;

    /* FAKEMATCH: do-while(0) raises the loop depth of these refs so fill gets r2 (same trick as sub_08075114) */
    do {
        color &= 0xF;
        color |= color << 4;
        color |= color << 8;
    } while (0);
    w = gUnk_02000000[0x10000];
    /* FAKEMATCH: do-while(0) weights x's refs so x is allocated (r5) before the block-0 base pointer (r6) */
    do {
        dst += (x << 4) * w;
    } while (0);
    /* FAKEMATCH: explicit guard + do-while (instead of for) keeps the guard on plain w, so w stays in r3 with no copy */
    i = 0;
    if (i < w * 3) {
        /* FAKEMATCH: assigning the base in the preheader keeps the block-0 base live into it (allocated r6) */
        buf = gUnk_02000000;
        do {
            /* FAKEMATCH: (w & 0xFF) is a loop-invariant no-op that loop.c hoists to the [sp+4] copy the ROM spills
               (and drags w*x out with it); the u32 sum puts the tile offset before the base in the add */
            const u8 *src = (const u8 *)((i + (w & 0xFF) * x) * 64 + (u32)buf);
            s32 j;

            next = i + 1;
            for (j = 15; j >= 0; j--) {
                *dst = color;
                if (src[0] != 0) {
                    *dst &= 0xFFF0;
                    *dst |= src[0];
                }
                if (src[1] != 0) {
                    *dst &= 0xFF0F;
                    *dst |= src[1] << 4;
                }
                if (src[2] != 0) {
                    *dst &= 0xF0FF;
                    *dst |= src[2] << 8;
                }
                if (src[3] != 0) {
                    *dst &= 0x0FFF;
                    *dst |= src[3] << 12;
                }
                dst++;
                src += 4;
            }
            i = next;
        } while (i < (w & 0xFF) * 3);
    }
}
void sub_08078DC4(u8 *p, u8 a, u8 b, u8 c, u8 d, u8 e)
{
    sub_08074E20((p[0] << 8) | p[1], a + 1, b + 1, d | (e << 8));
    sub_08074E20((p[0] << 8) | p[1], a, b, c | (e << 8));
}
u16 sub_08078E2C(u8 **pp)
{
    u16 val = 0;
    u8 n = 0;
    u8 mul = 1;
    u8 i;
    u8 *p = *pp;

    while (*p != 0x29 && *p != 0x2C) {
        p++;
        n++;
    }
    *pp = p + 1;
    p--;
    for (i = 0; i < n; i++) {
        val = val + (*p - 0x30) * mul;
        mul = mul * 10;
        p--;
    }
    return val;
}
void sub_08078E80(u16 *dst, u8 c, u16 x, u16 y, u8 pal, u16 base, u8 *flags)
{
    if (c > 0x9F) {
        c -= 0x20;
        if (!(*flags & 1))
            c += 0x40;
    }
    dst[x + (y << 5)] = c | base | (pal << 12);
}
/* Expand one 8x8 ASCII glyph (font at 0x08228D00, 8 bytes each) to tile halfwords. mode != 0 ORs into the existing data. */
void sub_08078ED4(u8 ch, u16 *dst, u16 a, u16 b, u16 mode)
{
    u16 *src = (u16 *)(0x08228D00 + ch * 8);
    s32 i;
    u16 m = 0xF;

    for (i = 3; i >= 0; i--) {
        if (mode == 0) {
            *dst++ = sub_080725B0((*src >> 4) & m, a, b);
            *dst++ = sub_080725B0(*src & 0xF, a, b);
            *dst++ = sub_080725B0((*src >> 12) & m, a, b);
            *dst = sub_080725B0((*src >> 8) & m, a, b);
        } else {
            *dst++ |= sub_080725B0(*src & 0xF, a, b);
            *dst++ |= sub_080725B0((*src >> 4) & m, a, b);
            *dst++ |= sub_080725B0((*src >> 8) & m, a, b);
            *dst |= sub_080725B0((*src >> 12) & m, a, b);
        }
        dst++;
        src++;
    }
}
/* Expand one Shift-JIS glyph (8 bytes at 0x081C0000) to 16 tile halfwords. */
void sub_08078FD4(u16 ch, u16 *dst, u16 a, u16 b)
{
    u16 *src = (u16 *)(0x081C0000 + sub_08072584(ch) * 8);
    s32 i;
    u16 m = 0xF;

    for (i = 3; i >= 0; i--) {
        *dst++ = sub_080725B0((*src >> 4) & m, a, b);
        *dst++ = sub_080725B0(*src & 0xF, a, b);
        *dst++ = sub_080725B0((*src >> 12) & m, a, b);
        *dst++ = sub_080725B0((*src >> 8) & m, a, b);
        src++;
    }
}
#if 0 /* NONMATCHING (score 81): NONMATCHING: u32 w[2] array (DImode pseudo r5:r6, regmove can't tie subregs) +
       * u16 b1 / short b2 temps for the two bit tests (extensions later removed) + u8 n declared after i,row,blk (GCSE
       * reaching-reg order -> stack slots). Only diff left: loop.c hoists 4 terms of the colour fill (ROM 3): row loop
       * has 122/117 RTL insns in loop passes 1/2, needs >=121 in pass 2 (threshold 26-3/move, savings 2 * life 3). */
extern u8 gUnk_081D0200[];
void sub_08079068(u16 ch, u16 *dst, u8 a, u16 c, u16 color)
{
    u16 *src = (u16 *)(gUnk_081D0200 + sub_08072584(ch) * 20);
    u16 *dst2 = dst + 0x10;
    u8 i, row, blk;
    u8 n = 10;
    u32 w[2];
    u32 col;
    u16 b1;
    short b2;

    
    for (blk = 0; blk < 2; blk++) {
        
        for (row = 0; row < 8; row++) {
            
            if (gUnk_02011C20[4] & 0x80) {
                w[0] = w[1] = color | (color << 4) | (color << 8) | (color << 12) | (color << 16) | (color << 20) | (color << 24) | (color << 28);
            } else {
                w[0] = dst[0] | (dst[1] << 16);
                w[1] = dst2[0] | (dst2[1] << 16);
            }
            for (i = 0; i < 8; i++) {
                b1 = src[0] >> i;
                if (b1 & 1)
                    col = c;
                else
                    col = color;
                w[0] = (w[0] & ~(0xF << ((7 - i + a) * 4))) | (col << ((7 - i + a) * 4));
                b2 = ((((u8)src[0]) << 8) | (src[0] >> 8)) >> (8 - i + a);
                if (b2 & 1)
                    col = c;
                else
                    col = color;
                w[1] = (w[1] & ~(0xF << (i * 4))) | (col << (i * 4));
                
            }
            *dst++ = w[0];
            *dst++ = w[0] >> 16;
            *dst2++ = w[1];
            *dst2++ = w[1] >> 16;
            if (--n == 0)
                return;
            src++;
        }
        dst += 0x150;
        dst2 += 0x150;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080784E4", sub_08079068); /* 0x08079068 size 0x18C */
/* Render a string as glyph tiles into `dst` (0x20 bytes per tile); ASCII glyphs are half width so two share a tile. */
void sub_080791F4(u8 *str, u8 *dst, u8 a, u8 b)
{
    u8 half = 0;
    u16 col = 0;
    u8 row = 0;
    u16 ch;
    u8 *p;

    p = str;
    while (*p != 0) {
        if (gUnk_02011C20[4] & 0x80) {
            ch = (p[0] << 8) | p[1];
            if (ch > 0x813F) {
                sub_08078FD4(ch, (u16 *)(dst + ((col++ + row * 32) << 5)), a, b);
            }
            p += 2;
        } else {
            sub_08078ED4(p[0], (u16 *)(dst + ((col + row * 32) << 5)), a, b, half);
            if (half != 0) {
                half = 0;
                col++;
            } else {
                half = 1;
            }
            p++;
        }
    }
}
/* Write `count` consecutive tile indices starting at `start` into a tilemap: mode 0 = one map cell each, mode 1 = 2x2 cells (tile*4 + 0..3). */
void sub_080792A0(u16 start, u16 *dst, u8 pal, u8 mode, u8 count)
{
    u8 i;
    u8 col;
    short row;

    col = 0;
    row = 0;
    for (i = 0; i < count; i++) {
        switch (mode) {
        case 0:
            dst[row + col++] = start | (pal << 12);
            break;
        case 1: {
            u16 *e = dst + (row + col);
            u32 t = start << 2;
            u32 pl = pal << 12;

            e[0] = pl | t;
            e[1] = (t + 1) | pl;
            e[0x20] = (t + 2) | pl;
            e[0x21] = (t + 3) | pl;
            col += 2;
            break;
        }
        }
        start++;
    }
}
/* Draw a string as text on a 2-row tilemap strip (upper and lower half of each glyph). */
void sub_08079340(u8 *str, u8 *dst, u32 p2, u16 x, u8 q0, u8 q1, u8 q2, u8 q3)
{
    u8 n;
    u8 *q;
    u8 len;

    n = 0;
    q = str;
    while (*q++ != 0)
        n++;
    len = (n * 5 + 7) >> 3;
    sub_08074B08(len, 2);
    sub_0807501C(0, 0, q1 | 0xA00, str);
    sub_08075114((void *)p2, q2);
    sub_080792A0(x, (u16 *)dst, q0, q3, len);
    if (dst == (u8 *)0x0600C7C8)
        dst = (u8 *)0x0600BFC8;
    sub_080792A0(x + len, (u16 *)(dst + 0x40), q0, q3, len);
}
extern void sub_080791F4_5(u8 *str, u8 *dst, u8 a, u8 b, u8 extra) asm("sub_080791F4");

void sub_08079404(u8 *s, u32 p1, u32 p2, u16 p3, u8 q0, u8 q1, u8 q2, u8 q3)
{
    u8 n;

    sub_080791F4_5(s, (u8 *)p2, q1, q2, q3);
    n = 0;
    while (*s++ != 0)
        n++;
    sub_080792A0(p3, (u16 *)p1, q0, q3, (n + 1) >> 1);
}
void sub_08079474(u8 *dst, u8 unused, u8 pal)
{
    u8 buf[0x10];
    vu16 zero;
    vu32 *dma;
    vu32 *d2;
    u8 i;

    memcpy(buf, gUnk_08087B94, 13);
    zero = 0;
    dma = (vu32 *)0x040000D4;
    dma[0] = (u32)&zero;
    dma[1] = (u32)dst;
    dma[2] = 0x810000D0;
    dma[2];
    d2 = (vu32 *)0x040000D4;
    while (d2[2] & 0x80000000)
        ;
    for (i = 0; i < 13; i++)
        sub_08079FDC(dst + i * 32, buf + i, pal, 4);
}
/* Print `val` as decimal digits right to left starting at (col, row) of a tilemap; mode 0 = zero padded to n digits, mode 1 = no leading zeros. */
void sub_080794E0(u16 val, u8 n, u8 mode, u16 *dst, u8 col, u8 row, u8 pal, u16 base, u8 m2)
{
    u8 i;
    u16 d;

    switch (mode) {
    case 0:
        for (i = 0; i < n; i++) {
            d = val % 10;
            val = val / 10;
            sub_080792A0(base + d, dst + (col-- + row * 32), pal, m2, 1);
        }
        break;
    case 1:
        if (val == 0) {
            sub_080792A0(base, dst + (col-- + row * 32), pal, m2, 1);
            return;
        }
        for (i = 0; i < n; i++) {
            d = val % 10;
            val = val / 10;
            if (d == 0 && val == 0)
                return;
            sub_080792A0(base + d, dst + (col-- + row * 32), pal, m2, 1);
        }
        break;
    }
}
