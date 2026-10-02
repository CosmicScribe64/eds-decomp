#include "global.h"
#include "gba.h"

extern u8 gUnk_02011C20[];

struct TextFlags {
    u8 newline : 1; /* set at the start of a line (after \n or at entry) */
    u8 rest : 7;
};

void sub_08078E80(void *dst, u8 ch, u8 x, u16 y, u8 a, u16 b, struct TextFlags *flags);
void sub_080798B8(u8 bits, u16 x, u16 y, u8 color, u16 *dst, u16 w, u8 mode);
void sub_080799BC(u16 bits, u16 x, u16 y, u8 color, u16 *dst, u16 w, u8 mode);
extern int sub_08074A90(int);
extern int sub_08072584(int);
extern u8 gUnk_081C0000[];
extern u8 gUnk_08228D00[];
extern u8 gUnk_0822BB00[];
extern u8 gUnk_08229500[];
extern u8 gUnk_08229F00[];
extern u8 gUnk_081D0200[];
extern u8 gUnk_081F8700[];
void sub_08079A48(u16 ch, u16 x, u16 y, u16 *dst, u8 a, u8 c, u16 d, u8 e);
void sub_08079B88(u8 ch, u16 x, u16 y, u16 *dst, u8 a, u8 c, u16 d, u8 e);

/* Sprite list: 20 layer heads (-1 = empty) + 128 linked entries of 12 bytes. */
struct OamEntry {
    u32 w;
    u16 h;
    u16 pad;
    s8 next;
    u8 pad2[3];
};

struct OamList {
    s8 head[0x14];
    struct OamEntry e[128];
    u8 count : 7;
    u8 flag : 1;
};

struct OamShadow {
    u32 w;
    u16 h;
    u16 pad;
};

extern struct OamShadow gUnk_03004470[];

/* Bresenham-style stepper. */
struct Line {
    u16 x, y;
    u16 x1, y1;
    u16 sx, sy;
    s16 dx, dy;
    u16 acc;
    u8 major;
};

/* sub_08078E80's real x parameter is u16: with it, all four `& 0x1F` constants are one movable, so loop.c hoists
   `y & 0x1F` (not `y - 1`), as in the ROM. The switch keeps cse_around_loop from reusing the loop test's *str load. */
typedef void (*DrawGlyphCellFunc960C)(void *dst, u8 ch, u16 x, u16 y, u8 a, u16 b, void *flags);
#define sub_08078E80_960C ((DrawGlyphCellFunc960C)sub_08078E80)

/* Draws a string of 1-byte glyphs into a tilemap; \n / \r toggle flag bit 0, 0xDE/0xDF (dakuten marks) are drawn one cell up-left and do not advance. */
void sub_0807960C(u8 *str, void *dst, u16 x, u16 y, u8 a, u16 b, u8 max, struct TextFlags *flags)
{
    u8 count = 0;

    flags->newline = 1;
    while (*str != 0 && count < max) {
        switch (*str) {
        case '\r':
            flags->newline = 0;
            str++;
            break;
        case '\n':
            flags->newline = 1;
            str++;
            break;
        }
        if ((u8)(*str + 0x22) <= 1) {
            sub_08078E80_960C(dst, *str++, (x - 1) & 0x1F, (y - 1) & 0x1F, a, b, flags);
        } else {
            sub_08078E80_960C(dst, *str++, x++ & 0x1F, y & 0x1F, a, b, flags);
            count++;
        }
    }
}
/* sub_08078E80's real x parameter is u16 (see its definition in code_080784E4.c); the u8 prototype above
   narrows `x & 0x1F` in QImode, so its 0x1F constant is not shared with `y & 0x1F`. */
typedef void (*DrawGlyphCellFunc)(void *dst, u8 ch, u16 x, u16 y, u8 a, u16 b, void *flags);
#define sub_08078E80_x16 ((DrawGlyphCellFunc)sub_08078E80)

/* Draws num as decimal digits right to left from x; mode 0 = always count digits, mode 1 = no leading zeros. */
void sub_08079700(u16 num, u8 count, u8 mode, void *dst, u16 x, u16 y, u8 a, u16 b, void *flags)
{
    u8 i;
    u16 d;

    switch (mode) {
    case 0:
        for (i = 0; i < count; i++) {
            d = num % 10;
            num = num / 10;
            sub_08078E80_x16(dst, d + '0', x-- & 0x1F, y & 0x1F, a, b, flags);
        }
        break;
    case 1:
        if (num == 0) {
            sub_08078E80_x16(dst, '0', x-- & 0x1F, y & 0x1F, a, b, flags);
            return;
        }
        for (i = 0; i < count; i++) {
            d = num % 10;
            num = num / 10;
            if (d == 0 && num == 0)
                return;
            sub_08078E80_x16(dst, d + '0', x-- & 0x1F, y & 0x1F, a, b, flags);
        }
        break;
    }
}
/* Fills a w x h rectangle of a 32x32 tilemap (wrapping at 32) with one tile. */
void sub_08079834(u16 tile, u16 *dst, u16 x, u16 y, u16 w, u16 h)
{
    u16 i;
    u16 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++)
            dst[((s16)x + j & 0x1F) + (((s16)y + i & 0x1F) << 5)] = tile;
    }
}
/* Draws one 8-pixel row of a 1bpp glyph into a 4bpp (mode 4) or 8bpp (mode 8) tile buffer. */
void sub_080798B8(u8 bits, u16 x, u16 stride, u8 color, u16 *dst, u16 w, u8 mode)
{
    u8 buf[0xC];
    u8 i;
    u32 xo = x & 1;
    u32 xh = x >> 1;
    u32 st;
    u32 half;
    u32 row2;

    /* FAKEMATCH: a no-op shift pair that combine folds into a plain copy only after GCSE,
       so the target's separate copy of stride (used by mode 4 only) survives copy propagation */
    st = ((u32)stride << 16) >> 16;
    half = w >> 1;
    switch (mode) {
    case 4: {
        /* block-scoped row (one pseudo per case) and the row2 copy keep the target's r4 -> sl split */
        u32 row = half * st;
        CpuSet(&dst[xh + row], buf, 5);
        for (i = 0, row2 = row; i < 4; i++) {
            if (bits & (0x80 >> (i * 2)))
                buf[i + xo] = (buf[i + xo] & 0xF0) + color;
            if (bits & (0x80 >> (i * 2 + 1)))
                buf[i + xo] = 0;
        }
        CpuSet(buf, &dst[xh + row2], 5);
        break;
    }
    case 8: {
        u32 row = half * stride;
        CpuSet(&dst[xh + row], buf, 5);
        for (i = 0, row2 = row; i < 8; i++) {
            if (bits & (0x80 >> i))
                buf[i + xo] = color;
        }
        CpuSet(buf, &dst[xh + row2], 5);
        break;
    }
    }
}
/* Draws one 16-pixel row of a 1bpp glyph into an 8bpp tile buffer (x may be odd). */
void sub_080799BC(u16 bits, u16 x, u16 stride, u8 color, u16 *dst, u16 w, u8 mode)
{
    u8 buf[0x14];
    int addr;
    u8 i;
    u32 xo = x & 1;
    u32 xh = x >> 1;
    u32 row2;
    u32 row;
    u32 half;

    half = w >> 1;
    if (mode == 8) {
        row = half * stride;
        /* FAKEMATCH: dead addr/row2 force the target's register copy of the row offset */
        CpuSet(&dst[addr = xh + row], buf, 10);
        row2 = row;
        for (i = 0; i < 16; i++) {
            if (bits & (0x8000 >> i))
                buf[i + xo] = color;
        }
        CpuSet(buf, &dst[xh + row2], 10);
    }
}
/* Draws one 2-byte (JIS) glyph. mode = glyph size (8/10/12), e = bit depth. */
void sub_08079A48(u16 code, u16 x, u16 y, u16 *dst, u8 color, u8 mode, u16 w, u8 e)
{
    u16 *p;
    u32 t;
    u32 g;
    int i;
    u32 hi;

    if (code <= 0x813F) {
        code = sub_08074A90(code);
        if (code == 0)
            return;
    }
    if (mode == 8) {
        p = (u16 *)(gUnk_081C0000 + sub_08072584(code) * 8);
        for (i = 3; i >= 0; i--) {
            t = *p++;
            t <<= 17;
            sub_080798B8((t << 8) >> 24, x, y++, color, dst, w, e);
            sub_080798B8(t >> 24, x, y++, color, dst, w, e);
        }
    } else {
        switch (mode) {
        case 10:
            p = (u16 *)(gUnk_081D0200 + sub_08072584(code) * 20);
            break;
        case 12:
            p = (u16 *)(gUnk_081F8700 + sub_08072584(code) * 24);
            break;
        default:
            return;
        }
        if (mode != 0) {
            i = mode;
            do {
                g = *p++;
                /* FAKEMATCH: hi = g >> 8 keeps the swap in the target's registers */
                hi = g >> 8;
                sub_080799BC((g << 24 >> 16 | hi) << 17 >> 16, x, y++, color, dst, w, e);
            } while (--i != 0);
        }
    }
}
#if 0 /* NONMATCHING: register allocation differs. The target keeps ch in r5, w in r9, e in r8, i in sl and spills y+1 to [sp+0x1C/0x20]; the build uses ip/sl/r8. Structure identical */
/* Draws one 1-byte glyph (8x8, 10-row or 12-row font). */
void sub_08079B88(u8 ch, u16 x, u16 y, u16 *dst, u8 color, u8 mode, u16 w, u8 e)
{
    u16 *p;
    u32 t;
    int i;

    if (mode == 8) {
        p = (u16 *)(gUnk_08228D00 + ch * 8);
        for (i = 3; i >= 0; i--) {
            t = *p++;
            t <<= 17;
            sub_080798B8((t << 8) >> 24, x, y++, color, dst, w, e);
            sub_080798B8(t >> 24, x, y++, color, dst, w, e);
        }
    } else {
        switch (mode) {
        case 10:
            p = (u16 *)(gUnk_08229500 + ch * 10);
            break;
        case 12:
            p = (u16 *)(gUnk_08229F00 + ch * 12);
            break;
        default:
            return;
        }
        mode >>= 1;
        if (mode != 0) {
            i = mode;
            do {
                t = *p++;
                t <<= 17;
                sub_080798B8((t << 8) >> 24, x, y++, color, dst, w, e);
                sub_080798B8(t >> 24, x, y++, color, dst, w, e);
            } while (--i != 0);
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0807960C", sub_08079B88); /* 0x08079B88 size 0x140 */
void sub_08079CC8(u8 *str, u16 x, u16 y, void *dst, u8 a, u8 b, u8 c, u16 d, u8 e);
/* Draws a string of 1-byte glyphs with a drop shadow (colour b at +1,+1, colour a on top). */
void sub_08079CC8(u8 *str, u16 x, u16 y, void *dst, u8 a, u8 b, u8 c, u16 d, u8 e)
{
    u16 n = 0;
    u32 t;
    u32 t1;

    while (*str != 0) {
        sub_08079B88(*str, (t1 = (t = n * (c >> 1)) + 1) + x, y + 1, dst, b, c, d, e);
        sub_08079B88(*str++, t + x, y, dst, a, c, d, e);
        n++;
    }
}
void sub_08079D88(u8 *str, u16 x, u16 y, void *dst, u8 a, u8 b, u8 c, u16 d, u8 e);
/* Same for 2-byte glyphs (big-endian character code). */
void sub_08079D88(u8 *str, u16 x, u16 y, void *dst, u8 a, u8 b, u8 c, u16 d, u8 e)
{
    u16 n = 0;
    u32 t;
    u32 t1;

    while (*str != 0) {
        sub_08079A48(*str << 8 | str[1], (t1 = (t = n * (c >> 1)) + 1) + x, y + 1, dst, b, c, d, e);
        sub_08079A48(*str << 8 | str[1], t + x, y, dst, a, c, d, e);
        str += 2;
        n += 2;
    }
}
/* Draws a string with the 1-byte (ASCII) or 2-byte (Shift-JIS style) glyph routine,
 * depending on flag bit 7 of the save mirror byte at 0x02011C24. */
void sub_08079E50(u8 *str, u16 x, u16 y, void *dst, u8 a, u8 b, u8 c, u16 d, u8 e)
{
    if (gUnk_02011C20[4] & 0x80)
        sub_08079D88(str, x, y, dst, a, b, c, d, e);
    else
        sub_08079CC8(str, x, y, dst, a, b, c, d, e);
}
u8 sub_08079ED4(const u8 *s)
{
    u32 v;
    int t;
    int r;

    if ((u8)(s[0] - '0') <= 9)
        v = (s[0] - '0') * 10;
    else
        v = 0;
    /* FAKEMATCH: dead assignments r/t force the target's "subtract first, then reload s[1]" order */
    return (u8)(s[1] - '0') <= 9 ? (u8)(r = (t = v - '0') + s[1]) : (u8)v;
}
u32 sub_08079F10(const u8 *s, u32 n)
{
    u32 v = 0;
    int c;
    while (n != 0) {
        v *= 10;
        c = *s;
        switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            v += c - '0';
            break;
        }
        n--;
        s++;
    }
    return v;
}
/* Returns 1 if the next word/segment of the string (width w) still fits: a + w <= limit.
 * FAKEMATCH: the two `*(volatile u8 *)s` reads stop agbcc CSEing the '$' test load with the loop-condition load, as the ROM does. */
u8 sub_08079F40(u8 *s, u8 a, u8 limit)
{
    u8 w = 0;
    u8 cont;

    if (gUnk_02011C20[4] & 0x80) {
        cont = 1;
        while (*s != 0 && cont) {
            if (*s <= 0x7E) {
                if (*(volatile u8 *)s == '$') {
                    if (s[1] == 'r')
                        s += 3;
                    else
                        s += 2;
                } else {
                    w++;
                    s++;
                }
            } else {
                w += 2;
                s += 2;
                cont = 0;
            }
        }
    } else {
        while (*s != ' ' && *s != 0 && *s != '.') {
            if (*(volatile u8 *)s == '$') {
                if (s[1] == 'r')
                    s += 3;
                else
                    s += 2;
            } else {
                w++;
                s++;
            }
        }
    }
    if (a + w > limit)
        return 0;
    return 1;
}
/* Expands an 8x8 1bpp glyph (index *idx into the table at 0x0822BB00) into a 4bpp (depth 4)
 * or 8bpp (otherwise) tile at dst by painting `color` into the set bits. */
void sub_08079FDC(u32 *dst, u8 *idx, u8 color, u8 depth)
{
    u8 *src;
    u16 i;
    u32 w;
    u16 n = 8;
    u8 ix = *idx;

    if (depth == 4)
        color &= 0xF;
    src = (u8 *)0x0822BB00 + ix * 8;
    if (depth == 8) {
        i = 0;
        do {
            w = dst[0];
            if (*src & 0x80)
                w = (w & 0xFFFFFF00) + color;
            if (*src & 0x40)
                w = (w & 0xFFFF00FF) + (color << 8);
            if (*src & 0x20)
                w = (w & 0xFF00FFFF) + (color << 16);
            if (*src & 0x10)
                w = (w & 0x00FFFFFF) + (color << 24);
            *dst++ = w;
            w = dst[0];
            if (*src & 0x8)
                w = (w & 0xFFFFFF00) + color;
            if (*src & 0x4)
                w = (w & 0xFFFF00FF) + (color << 8);
            if (*src & 0x2)
                w = (w & 0xFF00FFFF) + (color << 16);
            if (*src & 0x1)
                w = (w & 0x00FFFFFF) + (color << 24);
            *dst++ = w;
            src++;
            i++;
        } while (i < n);
    } else {
        i = 0;
        do {
            w = dst[0];
            if (*src & 0x80)
                w = (w & 0xFFFFFFF0) + color;
            if (*src & 0x40)
                w = (w & 0xFFFFFF0F) + (color << 4);
            if (*src & 0x20)
                w = (w & 0xFFFFF0FF) + (color << 8);
            if (*src & 0x10)
                w = (w & 0xFFFF0FFF) + (color << 12);
            if (*src & 0x8)
                w = (w & 0xFFF0FFFF) + (color << 16);
            if (*src & 0x4)
                w = (w & 0xFF0FFFFF) + (color << 20);
            if (*src & 0x2)
                w = (w & 0xF0FFFFFF) + (color << 24);
            if (*src & 0x1)
                w = (w & 0x0FFFFFFF) + (color << 28);
            *dst++ = w;
            src++;
            i++;
        } while (i < n);
    }
}
u16 sub_0807A17C(u16 a, u16 b, u16 c)
{
    if (a != 0)
        c = b;
    return c;
}
u8 sub_0807A190(const u8 *s)
{
    u8 n = 0;
    while (*s++ != 0)
        n++;
    return n;
}
/* LZSS decoder: 4 KiB ring at 0x02030000, initial write position 0xFEE. */
void sub_0807A1A8(u8 *src, u8 *dst, s32 size)
{
    u32 flags = 0;
    u8 mask = 0;
    u8 *ring = (u8 *)0x02030000;
    u16 r = 0xFEE;
    u16 i;
    u16 p;
    u16 b;

    for (i = 0; i <= 0xFED; i++)
        ring[i] = 0;
    do {
        mask <<= 1;
        if (mask == 0) {
            flags = *src++;
            size--;
            mask = 1;
        }
        if (flags & mask) {
            b = *src;
            *dst++ = b;
            ring[r++] = b;
            src++;
            r &= 0xFFF;
            size--;
        } else {
            p = *src++;
            b = *src++;
            p |= (b & 0xF0) << 4;
            b &= 0xFF0F;
            size -= 2;
            for (i = 0; i < b + 3; i++) {
                *dst++ = ring[p];
                ring[r++] = ring[p++];
                r &= 0xFFF;
                p &= 0xFFF;
            }
        }
    } while (size != 0);
}

u8 sub_0807A298(struct OamList *l)
{
    u8 n = 0;
    u8 i;
    s8 idx;
    u32 *s;
    u32 *d;

    for (i = 0; i < 0x14; i++) {
        idx = l->head[i];
        while (idx >= 0) {
            d = (u32 *)&gUnk_03004470[n];
            s = (u32 *)&l->e[idx];
            *d++ = *s++;
            *(u16 *)d = *(u16 *)s;
            n++;
            idx = l->e[idx].next;
        }
    }
    return n;
}
void sub_0807A2EC(u8 *list)
{
    u8 i = 0;
    u8 f = 0xFF;

    for (; i < 0x14; i++) {
        u8 *p = list + i;
        u8 w = *p;
        /* Keep the byte load as the OR accumulator used by the ROM. */
        __asm__ __volatile__("" : : "r"(w));
        *p = w | f;
    }
    ((struct OamList *)list)->count = 0;
}
struct OamEntry *sub_0807A320(u8 layer, struct OamList *l)
{
    u32 n, m = l->count;

    if (m > 0x7F)
        return 0;
    n = l->count;
    l->e[n].next = l->head[layer];
    l->head[layer] = n;
    l->count++;
    return &l->e[l->count - 1];
}
void sub_0807A37C(u8 idx, u8 layer, struct OamList *l)
{
    l->e[idx].next = l->head[layer];
    l->head[layer] = idx;
}
void sub_0807A398(s16 x0, s16 y0, s16 x1, s16 y1, struct Line *l)
{
    s16 dx, dy;

    l->dx = dx = x1 - x0;
    l->dy = dy = y1 - y0;
    if (dx >= 0) {
        l->sx = 1;
    } else {
        int n;
        l->sx = -1;
        /* Preserve the ROM's signed reload after storing the direction. */
        __asm__ __volatile__("" : : : "memory");
        n = l->dx;
        __asm__ __volatile__("" : : "r"(n));
        l->dx = -n;
    }
    __asm__ __volatile__("" : : : "memory");
    if (l->dy >= 0) {
        l->sy = 1;
    } else {
        int n;
        l->sy = -1;
        __asm__ __volatile__("" : : : "memory");
        n = l->dy;
        __asm__ __volatile__("" : : "r"(n));
        l->dy = -n;
    }
    __asm__ __volatile__("" : : : "memory");
    if (l->dx >= l->dy)
        l->major = 1;
    else
        l->major = 2;
    l->x = x0;
    l->y = y0;
    l->x1 = x1;
    l->y1 = y1;
    l->acc = 0;
}
void sub_0807A420(struct Line *l)
{
    if (l->major == 0)
        return;
    if (*(u32 *)&l->x == *(u32 *)&l->x1) {
        l->major = 0;
        return;
    }
    if (l->major == 2) {
        l->y += l->sy;
        l->acc += l->dx;
        if (l->acc >= l->dy) {
            l->acc -= l->dy;
            l->x += l->sx;
        }
    } else {
        l->x += l->sx;
        l->acc += l->dy;
        if (l->acc >= l->dx) {
            l->acc -= l->dx;
            l->y += l->sy;
        }
    }
}
u16 sub_0807A490(u16 x, u16 y, u8 shift)
{
    u16 off = 0;

    if (x > 0xFF) {
        off = 0x800;
        x -= 0x20;
    }
    if (y > 0xFF) {
        off += 0x800;
        y -= 0x20;
    }
    if (shift == 3)
        off += 0x800;
    off += (x + y * 32) * 2;
    return off;
}
void sub_0807A4E8(u8 tile, u8 bg, u8 x, u8 y, u16 w)
{
    u32 *p = (u32 *)(0x06000000 + bg * 0x800 + x * 2 + y * 64);
    u8 i;

    for (i = 0; i < w / 2; i++)
        *p++ = tile | tile << 16;
}
void sub_0807A528(u16 *p, u8 w, u8 h)
{
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++)
            *p++ = 0;
        p += 0x20 - w;
    }
}
void sub_0807A568(u8 tile, u8 bg, u8 x, u8 y, u8 w, u8 h)
{
    u16 *p = (u16 *)(0x06000000 + bg * 0x800 + x * 2 + y * 64);
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            if (w > 0x1F)
                *(u16 *)((u8 *)p + 0x800) = tile;
            else
                *p = tile;
            p++;
        }
        p += 0x20 - w;
    }
}
void sub_0807A5D4(u16 tile, u8 bg, u8 x, u8 y, u8 w, u8 h)
{
    u32 *p = (u32 *)(0x06000000 + bg * 0x800 + x * 2 + y * 64);
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w / 2; j++) {
            s32 xx = x + j * 2;
            if ((u32)(xx - 0x20) <= 0x1F) {
                *(u32 *)((u8 *)p + 0x7C0) = tile++ | tile++ << 16;
                p++;
            } else if (xx > 0x3F) {
                *(u32 *)((u8 *)p - 0x100) = tile++ | tile++ << 16;
                p++;
            } else {
                *p++ = tile++ | tile++ << 16;
            }
        }
        p += 0x10 - w / 2;
    }
}
