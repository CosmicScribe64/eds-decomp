/*
 * bitmap_text (0x0807960C-0x0807A6AC): the bitmap glyph printers, the OAM layer list, the LZSS decoder
 * and the tilemap fill helpers (wiki/functions/bitmap-text-c.md).
 *
 * The Bitmap* printers plot 1bpp font glyphs straight into a linear 8bpp bitmap (a Mode-4 page, the
 * bustup text box): BitmapPlotRow8 / BitmapPlotRow16 read-modify-write one glyph row at a time,
 * BitmapDrawSjisGlyph / BitmapDrawLatinGlyph draw one glyph of the 8/10/12 px fonts, and the
 * *StringShadow functions draw a string with a drop shadow, BitmapDrawStringShadow picking the Latin or
 * Shift-JIS path from gSaveData.sjisText. PutMapString / PutMapNumber are the older map-cell printers
 * (Japanese-engine leftovers, no callers in the USA ROM). Around them: OverlayBoldGlyphTile (one bold
 * glyph expanded into a tile), NextWordFits (the text box word-wrap test), the small parsers
 * (ParseTwoDigits, ParseDigits, SelectU16, StrLenU8), the OAM layer list (OamList*, flushed to the OAM
 * shadow each frame), the Bresenham walker LineInit / LineStep, the LZSS decoder, and the Fill* /
 * Clear* tilemap helpers.
 */
#include "global.h"
#include "gba.h"                           /* VRAM, CpuSet */
#include "bg.h"                     /* FillMapRectWrap, ClearMapRect, GetTilemapOffset, FillScreenblock*, LZSSDecompress (defined here) */
#include "sprite.h"                 /* struct OamList / OamListEntry, OamListFlush / Clear / Alloc / LinkEntry (defined here) */
#include "util.h"                   /* struct Line, enum LineState, LineInit / LineStep (defined here) */

/* The save mirror read as bytes: [4] bit 7 is gSaveData.sjisText (save.h), the Latin/Shift-JIS switch. */
extern u8 gSaveDataBytes[] asm("gSaveData");    /* byte view of struct SaveData gSaveData (0x02011C20) */

/* The kana flags byte of the PutMap* printers (text.h). */
struct TextFlags {
    u8 katakana:1;  /* bit 0: kana codes 0xA0+ use the katakana bank; set at entry and on '\n', cleared on '\r' */
    u8 unused:7;
};

/* ---- Local views kept for matching (build/readability/issues/bitmap_text.md) ----
 * text.h declares these functions with the widths they are defined with, but this unit calls
 * PutMapChar through narrower views (see the two call aliases below) and the Shift-JIS converters
 * through int-returning declarations; the matched code depends on those views, so the declarations
 * stay here instead of including text.h. */
void PutMapChar(void *map, u8 ch, u8 x, u16 y, u8 pal, u16 tileBase, struct TextFlags *flags);
extern int AsciiToFullwidthSjis(int ch);
extern int SjisToGlyphIndex(int sjis);

/* The kanji fonts (the Latin fonts are addressed by integer literal below, on purpose). */
extern u8 gFontKanji8x8[];          /* 0x081C0000 */
extern u8 gFontKanji10x10[];        /* 0x081D0200 */
extern u8 gFontKanji12x12[];        /* 0x081F8700 */

void BitmapPlotRow8(u8 bits, u16 x, u16 y, u8 color, u16 *bitmap, u16 width, u8 bpp);
void BitmapPlotRow16(u16 bits, u16 x, u16 y, u8 color, u16 *bitmap, u16 width, u8 bpp);
void BitmapDrawSjisGlyph(u16 sjis, u16 x, u16 y, u16 *bitmap, u8 color, u8 size, u16 width, u8 bpp);
void BitmapDrawLatinGlyph(u8 ch, u16 x, u16 y, u16 *bitmap, u8 color, u8 size, u16 width, u8 bpp);

/* One slot of the OAM shadow buffer (8 bytes): OamListFlush writes attr0/attr1 as one word, then attr2. */
struct OamShadow {
    u32 attr01; /* +0x0: attr0 | attr1 << 16 */
    u16 attr2;  /* +0x4 */
    u16 pad;    /* +0x6: an affine parameter in hardware OAM; not written here */
};

extern struct OamShadow gMain_oamBuffer[];  /* 0x03004470 (= gMain.oamBuffer, main.h) */

/* PutMapChar's real x parameter is u16: with it, all four `& 0x1F` constants are one movable, so loop.c hoists
   `y & 0x1F` (not `y - 1`), as in the ROM. The switch keeps cse_around_loop from reusing the loop test's *str load. */
typedef void (*DrawGlyphCellFunc960C)(void *map, u8 ch, u16 x, u16 y, u8 pal, u16 tileBase, void *kana);
#define sub_08078E80_960C ((DrawGlyphCellFunc960C)PutMapChar)

/* Draws a string of 1-byte glyphs into a tilemap; \n / \r toggle the kana bank flag, 0xDE/0xDF (dakuten marks) are drawn one cell up-left and do not advance. */
void PutMapString(u8 *str, void *map, u16 x, u16 y, u8 palette, u16 tileBase, u8 maxChars, struct TextFlags *kana)
{
    u8 count = 0;

    kana->katakana = 1;
    while (*str != 0 && count < maxChars) {
        switch (*str) {
        case '\r':
            kana->katakana = 0;
            str++;
            break;
        case '\n':
            kana->katakana = 1;
            str++;
            break;
        }
        if ((u8)(*str + 0x22) <= 1) {
            sub_08078E80_960C(map, *str++, (x - 1) & 0x1F, (y - 1) & 0x1F, palette, tileBase, kana);
        } else {
            sub_08078E80_960C(map, *str++, x++ & 0x1F, y & 0x1F, palette, tileBase, kana);
            count++;
        }
    }
}
/* PutMapChar's real x parameter is u16 (see its definition in text_render.c); the u8 prototype above
   narrows `x & 0x1F` in QImode, so its 0x1F constant is not shared with `y & 0x1F`. */
typedef void (*DrawGlyphCellFunc)(void *map, u8 ch, u16 x, u16 y, u8 pal, u16 tileBase, void *kana);
#define sub_08078E80_x16 ((DrawGlyphCellFunc)PutMapChar)

/* Draws num as decimal digits right to left from x; mode is an enum NumberPadMode (0 = always count digits, 1 = no leading zeros). */
void PutMapNumber(u16 num, u8 digits, u8 mode, void *map, u16 x, u16 y, u8 palette, u16 tileBase, void *kana)
{
    u8 i;
    u16 d;

    switch (mode) {
    case 0:
        for (i = 0; i < digits; i++) {
            d = num % 10;
            num = num / 10;
            sub_08078E80_x16(map, d + '0', x-- & 0x1F, y & 0x1F, palette, tileBase, kana);
        }
        break;
    case 1:
        if (num == 0) {
            sub_08078E80_x16(map, '0', x-- & 0x1F, y & 0x1F, palette, tileBase, kana);
            return;
        }
        for (i = 0; i < digits; i++) {
            d = num % 10;
            num = num / 10;
            if (d == 0 && num == 0)
                return;
            sub_08078E80_x16(map, d + '0', x-- & 0x1F, y & 0x1F, palette, tileBase, kana);
        }
        break;
    }
}
/* Fills a w x h rectangle of a 32x32 tilemap (wrapping at 32) with one tile. */
void FillMapRectWrap(u16 tile, u16 *map, u16 x, u16 y, u16 w, u16 h)
{
    u16 i;
    u16 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++)
            map[((s16)x + j & 0x1F) + (((s16)y + i & 0x1F) << 5)] = tile;
    }
}
/* Draws one 8-pixel row of a 1bpp glyph into a 4bpp (bpp 4) or 8bpp (bpp 8) tile buffer. */
void BitmapPlotRow8(u8 bits, u16 x, u16 y, u8 color, u16 *bitmap, u16 width, u8 bpp)
{
    u8 buf[0xC];
    u8 i;
    u32 xo = x & 1;
    u32 xh = x >> 1;
    u32 st;
    u32 half;
    u32 row2;

    /* FAKEMATCH: a no-op shift pair that combine folds into a plain copy only after GCSE,
       so the target's separate copy of y (used by bpp 4 only) survives copy propagation */
    st = ((u32)y << 16) >> 16;
    half = width >> 1;
    switch (bpp) {
    case 4: {
        /* block-scoped row (one pseudo per case) and the row2 copy keep the target's r4 -> sl split */
        u32 row = half * st;
        CpuSet(&bitmap[xh + row], buf, 5);
        for (i = 0, row2 = row; i < 4; i++) {
            if (bits & (0x80 >> (i * 2)))
                buf[i + xo] = (buf[i + xo] & 0xF0) + color;
            if (bits & (0x80 >> (i * 2 + 1)))
                buf[i + xo] = 0;
        }
        CpuSet(buf, &bitmap[xh + row2], 5);
        break;
    }
    case 8: {
        u32 row = half * y;
        CpuSet(&bitmap[xh + row], buf, 5);
        for (i = 0, row2 = row; i < 8; i++) {
            if (bits & (0x80 >> i))
                buf[i + xo] = color;
        }
        CpuSet(buf, &bitmap[xh + row2], 5);
        break;
    }
    }
}
/* Draws one 16-pixel row of a 1bpp glyph into an 8bpp tile buffer (x may be odd). */
void BitmapPlotRow16(u16 bits, u16 x, u16 y, u8 color, u16 *bitmap, u16 width, u8 bpp)
{
    u8 buf[0x14];
    int addr;
    u8 i;
    u32 xo = x & 1;
    u32 xh = x >> 1;
    u32 row2;
    u32 row;
    u32 half;

    half = width >> 1;
    if (bpp == 8) {
        row = half * y;
        /* FAKEMATCH: dead addr/row2 force the target's register copy of the row offset */
        CpuSet(&bitmap[addr = xh + row], buf, 10);
        row2 = row;
        for (i = 0; i < 16; i++) {
            if (bits & (0x8000 >> i))
                buf[i + xo] = color;
        }
        CpuSet(buf, &bitmap[xh + row2], 10);
    }
}
/* Draws one 2-byte (JIS) glyph. size = glyph size (8/10/12), bpp = bit depth. */
void BitmapDrawSjisGlyph(u16 sjis, u16 x, u16 y, u16 *bitmap, u8 color, u8 size, u16 width, u8 bpp)
{
    u16 *p;
    u32 t;
    u32 g;
    int i;
    u32 hi;

    if (sjis <= 0x813F) {
        sjis = AsciiToFullwidthSjis(sjis);
        if (sjis == 0)
            return;
    }
    if (size == 8) {
        p = (u16 *)(gFontKanji8x8 + SjisToGlyphIndex(sjis) * 8);
        for (i = 3; i >= 0; i--) {
            t = *p++;
            t <<= 17;
            BitmapPlotRow8((t << 8) >> 24, x, y++, color, bitmap, width, bpp);
            BitmapPlotRow8(t >> 24, x, y++, color, bitmap, width, bpp);
        }
    } else {
        switch (size) {
        case 10:
            p = (u16 *)(gFontKanji10x10 + SjisToGlyphIndex(sjis) * 20);
            break;
        case 12:
            p = (u16 *)(gFontKanji12x12 + SjisToGlyphIndex(sjis) * 24);
            break;
        default:
            return;
        }
        if (size != 0) {
            i = size;
            do {
                g = *p++;
                /* FAKEMATCH: hi = g >> 8 keeps the swap in the target's registers */
                hi = g >> 8;
                BitmapPlotRow16((g << 24 >> 16 | hi) << 17 >> 16, x, y++, color, bitmap, width, bpp);
            } while (--i != 0);
        }
    }
}
/* Draws one 1-byte glyph (8x8, 10-row or 12-row font). */
void BitmapDrawLatinGlyph(u8 ch, u16 x, u16 y, u16 *bitmap, u8 color, u8 size, u16 width, u8 bpp)
{
    u16 *p;
    int i;

    /* FAKEMATCH: one extra flow ref each for bpp and width (no code) puts them ahead of i
       in global-alloc priority, giving the target's bpp = r8, width = r9, i = sl */
    asm("" : : "r"(bpp), "r"(width));

    /* Integer font addresses keep the const_int inside the add, so reload loads each
       base into its rotating spill register (r1 / r1 / r5) as in the target. */
    if (size == 8) {
        p = (u16 *)(0x08228D00 + ch * 8);           /* gFontLatin8x8 */
        for (i = 0; i < 4; i++) {
            u32 t = *p++;
            t <<= 17;
            BitmapPlotRow8((t << 8) >> 24, x, y++, color, bitmap, width, bpp);
            BitmapPlotRow8(t >> 24, x, y++, color, bitmap, width, bpp);
        }
    } else {
        switch (size) {
        case 10:
            p = (u16 *)(0x08229500 + ch * 10);      /* gFontLatin8x10 */
            break;
        case 12:
            p = (u16 *)(0x08229F00 + ch * 12);      /* gFontLatin8x12 */
            break;
        default:
            return;
        }
        size >>= 1;
        for (i = 0; i < size; i++) {
            u32 t = *p++;
            t <<= 17;
            BitmapPlotRow8((t << 8) >> 24, x, y++, color, bitmap, width, bpp);
            BitmapPlotRow8(t >> 24, x, y++, color, bitmap, width, bpp);
        }
    }
}
/* Draws a string of 1-byte glyphs with a drop shadow (shadowColor at +1,+1, color on top). */
void BitmapDrawLatinStringShadow(u8 *str, u16 x, u16 y, void *bitmap, u8 color, u8 shadowColor, u8 size, u16 width, u8 bpp)
{
    u16 n = 0;
    u32 t;
    u32 t1;

    while (*str != 0) {
        BitmapDrawLatinGlyph(*str, (t1 = (t = n * (size >> 1)) + 1) + x, y + 1, bitmap, shadowColor, size, width, bpp);
        BitmapDrawLatinGlyph(*str++, t + x, y, bitmap, color, size, width, bpp);
        n++;
    }
}
/* Same for 2-byte glyphs (big-endian character code). */
void BitmapDrawSjisStringShadow(u8 *str, u16 x, u16 y, void *bitmap, u8 color, u8 shadowColor, u8 size, u16 width, u8 bpp)
{
    u16 n = 0;
    u32 t;
    u32 t1;

    while (*str != 0) {
        BitmapDrawSjisGlyph(*str << 8 | str[1], (t1 = (t = n * (size >> 1)) + 1) + x, y + 1, bitmap, shadowColor, size, width, bpp);
        BitmapDrawSjisGlyph(*str << 8 | str[1], t + x, y, bitmap, color, size, width, bpp);
        str += 2;
        n += 2;
    }
}
/* Draws a string with the 1-byte (ASCII) or 2-byte (Shift-JIS style) glyph routine,
 * depending on flag bit 7 of the save mirror byte at 0x02011C24. */
void BitmapDrawStringShadow(u8 *str, u16 x, u16 y, void *bitmap, u8 color, u8 shadowColor, u8 size, u16 width, u8 bpp)
{
    if (gSaveDataBytes[4] & 0x80)
        BitmapDrawSjisStringShadow(str, x, y, bitmap, color, shadowColor, size, width, bpp);
    else
        BitmapDrawLatinStringShadow(str, x, y, bitmap, color, shadowColor, size, width, bpp);
}
u8 ParseTwoDigits(const u8 *s)
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
u32 ParseDigits(const u8 *s, u32 count)
{
    u32 v = 0;
    int c;
    while (count != 0) {
        v *= 10;
        c = *s;
        switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            v += c - '0';
            break;
        }
        count--;
        s++;
    }
    return v;
}
/* Returns 1 if the next word/segment of the string still fits: col + the word's width <= maxCol.
 * FAKEMATCH: the two `*(volatile u8 *)s` reads stop agbcc CSEing the '$' test load with the loop-condition load, as the ROM does. */
u8 NextWordFits(u8 *s, u8 col, u8 maxCol)
{
    u8 w = 0;
    u8 cont;

    if (gSaveDataBytes[4] & 0x80) {
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
    if (col + w > maxCol)
        return 0;
    return 1;
}
/* Expands an 8x8 1bpp glyph (the entry at index *ch of gFontLatin8x8Bold) into a 4bpp (bpp 4)
 * or 8bpp (otherwise) tile at tile by painting `color` into the set bits. */
void OverlayBoldGlyphTile(u32 *tile, u8 *ch, u8 color, u8 bpp)
{
    u8 *src;
    u16 i;
    u32 w;
    u16 n = 8;
    u8 ix = *ch;

    if (bpp == 4)
        color &= 0xF;
    src = (u8 *)0x0822BB00 + ix * 8;    /* gFontLatin8x8Bold */
    if (bpp == 8) {
        i = 0;
        do {
            w = tile[0];
            if (*src & 0x80)
                w = (w & 0xFFFFFF00) + color;
            if (*src & 0x40)
                w = (w & 0xFFFF00FF) + (color << 8);
            if (*src & 0x20)
                w = (w & 0xFF00FFFF) + (color << 16);
            if (*src & 0x10)
                w = (w & 0x00FFFFFF) + (color << 24);
            *tile++ = w;
            w = tile[0];
            if (*src & 0x8)
                w = (w & 0xFFFFFF00) + color;
            if (*src & 0x4)
                w = (w & 0xFFFF00FF) + (color << 8);
            if (*src & 0x2)
                w = (w & 0xFF00FFFF) + (color << 16);
            if (*src & 0x1)
                w = (w & 0x00FFFFFF) + (color << 24);
            *tile++ = w;
            src++;
            i++;
        } while (i < n);
    } else {
        i = 0;
        do {
            w = tile[0];
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
            *tile++ = w;
            src++;
            i++;
        } while (i < n);
    }
}
u16 SelectU16(u16 cond, u16 a, u16 b)
{
    if (cond != 0)
        b = a;
    return b;
}
u8 StrLenU8(const u8 *s)
{
    u8 n = 0;
    while (*s++ != 0)
        n++;
    return n;
}
/* LZSS decoder: 4 KiB ring at 0x02030000 (the scratch buffer), initial write position 0xFEE. */
void LZSSDecompress(u8 *src, u8 *dst, s32 srcSize)
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
            srcSize--;
            mask = 1;
        }
        if (flags & mask) {
            b = *src;
            *dst++ = b;
            ring[r++] = b;
            src++;
            r &= 0xFFF;
            srcSize--;
        } else {
            p = *src++;
            b = *src++;
            p |= (b & 0xF0) << 4;
            b &= 0xFF0F;
            srcSize -= 2;
            for (i = 0; i < b + 3; i++) {
                *dst++ = ring[p];
                ring[r++] = ring[p++];
                r &= 0xFFF;
                p &= 0xFFF;
            }
        }
    } while (srcSize != 0);
}

u8 OamListFlush(struct OamList *list)
{
    u8 n = 0;
    u8 i;
    s8 idx;
    u32 *s;
    u32 *d;

    for (i = 0; i < 0x14; i++) {
        idx = list->head[i];
        while (idx >= 0) {
            d = (u32 *)&gMain_oamBuffer[n];
            s = (u32 *)&list->entries[idx];
            *d++ = *s++;
            *(u16 *)d = *(u16 *)s;
            n++;
            idx = list->entries[idx].next;
        }
    }
    return n;
}
void OamListClear(u8 *list)
{
    u8 i = 0;
    u8 f = 0xFF;

    for (; i < 0x14; i++) {
        u8 *p = list + i;
        u8 w = *p;
        /* FAKEMATCH: keeps the byte load as the OR accumulator used by the ROM. */
        __asm__ __volatile__("" : : "r"(w));
        *p = w | f;
    }
    ((struct OamList *)list)->count = 0;
}
struct OamListEntry *OamListAlloc(u8 layer, struct OamList *list)
{
    u32 n, m = list->count;

    if (m > 0x7F)
        return 0;
    n = list->count;
    list->entries[n].next = list->head[layer];
    list->head[layer] = n;
    list->count++;
    return &list->entries[list->count - 1];
}
void OamListLinkEntry(u8 idx, u8 layer, struct OamList *list)
{
    list->entries[idx].next = list->head[layer];
    list->head[layer] = idx;
}
void LineInit(s16 x0, s16 y0, s16 x1, s16 y1, struct Line *line)
{
    s16 dx, dy;

    line->dx = dx = x1 - x0;
    line->dy = dy = y1 - y0;
    if (dx >= 0) {
        line->stepX = 1;
    } else {
        int n;
        line->stepX = -1;
        /* FAKEMATCH: preserves the ROM's signed reload after storing the direction. */
        __asm__ __volatile__("" : : : "memory");
        n = line->dx;
        __asm__ __volatile__("" : : "r"(n));
        line->dx = -n;
    }
    __asm__ __volatile__("" : : : "memory");
    if (line->dy >= 0) {
        line->stepY = 1;
    } else {
        int n;
        line->stepY = -1;
        /* FAKEMATCH: same signed reload of dy as for dx above. */
        __asm__ __volatile__("" : : : "memory");
        n = line->dy;
        __asm__ __volatile__("" : : "r"(n));
        line->dy = -n;
    }
    __asm__ __volatile__("" : : : "memory");
    if (line->dx >= line->dy)
        line->state = LINE_X_MAJOR;
    else
        line->state = LINE_Y_MAJOR;
    line->x = x0;
    line->y = y0;
    line->endX = x1;
    line->endY = y1;
    line->error = 0;
}
void LineStep(struct Line *line)
{
    if (line->state == LINE_DONE)
        return;
    if (*(u32 *)&line->x == *(u32 *)&line->endX) {
        line->state = LINE_DONE;
        return;
    }
    if (line->state == LINE_Y_MAJOR) {
        line->y += line->stepY;
        line->error += line->dx;
        if (line->error >= line->dy) {
            line->error -= line->dy;
            line->x += line->stepX;
        }
    } else {
        line->x += line->stepX;
        line->error += line->dy;
        if (line->error >= line->dx) {
            line->error -= line->dx;
            line->y += line->stepY;
        }
    }
}
u16 GetTilemapOffset(u16 x, u16 y, u8 screenSize)
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
    if (screenSize == 3)
        off += 0x800;
    off += (x + y * 32) * 2;
    return off;
}
void FillScreenblockRow32(u8 tile, u8 screenblock, u8 x, u8 y, u16 w)
{
    u32 *p = (u32 *)(VRAM + screenblock * 0x800 + x * 2 + y * 64);
    u8 i;

    for (i = 0; i < w / 2; i++)
        *p++ = tile | tile << 16;
}
void ClearMapRect(u16 *map, u8 w, u8 h)
{
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++)
            *map++ = 0;
        map += 0x20 - w;
    }
}
void FillScreenblockRectNoWrap(u8 tile, u8 screenblock, u8 x, u8 y, u8 w, u8 h)
{
    u16 *p = (u16 *)(VRAM + screenblock * 0x800 + x * 2 + y * 64);
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
void FillScreenblockRectAscending32(u16 tile, u8 screenblock, u8 x, u8 y, u8 w, u8 h)
{
    u32 *p = (u32 *)(VRAM + screenblock * 0x800 + x * 2 + y * 64);
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
