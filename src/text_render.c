/*
 * text_render (0x080784E4-0x0807960C): script-animation playback, Fade objects and the
 * glyph/string/number renderers that turn text into 4bpp tiles and BG map entries
 * (wiki/functions/text-render-c.md).
 *
 * AnimStateDrawRaw / AnimBlockDraw / AnimBlockInit / AnimStateTick / AnimBlockTick play the
 * sprite animation scripts (struct AnimSeq / AnimState / AnimBlock from sprite.h) into an
 * OamList. CopyTileRows copies tile rows between sheets of different widths, and
 * FadeStart/FadeTick drive the struct Fade brightness fades of palette.h. The rest renders
 * text: RenderShadowedGlyph, RenderHalfWidthGlyph, RenderFullWidthGlyph and
 * RenderKanji10x10Glyph expand one glyph of the Latin and Shift-JIS fonts into tile
 * halfwords, RenderStringToTiles a whole string, and PutMapTileRun, PutMapChar,
 * DrawTextStrip, DrawStringTiles, LoadDigitTiles and DrawNumberTiles place tiles and digit
 * strings on BG maps. ParseDecimal / ParseSignedDecimal are the small string parsers.
 *
 * Every type and prototype used here is canonical (palette.h, bg.h, sprite.h, text.h); the
 * unit was the first migrated to the per-subsystem headers (2026-10-03). The local views
 * kept for matching are listed in build/readability/issues/text_render.md.
 */
#include "global.h"
#include "gba.h"        /* CpuSet, REG_BLDCNT, REG_BLDY, VRAM */
#include "palette.h"    /* struct Fade, FadeStart, FadeTick, SetBldY */
#include "bg.h"         /* CopyTileRows */
#include "sprite.h"     /* struct AnimSeq, AnimState, AnimBlock; OamListAddTemplate, OamListAddSpriteGroup */
#include "text.h"       /* glyph and map-text renderers defined here; TextCanvas*, TextDraw*, fonts */

/* Local views kept on purpose (matching choices, see build/readability/issues/text_render.md):
 * - the matched code indexes the text canvas and the save image as bytes (gTextCanvas[0x10000] folds
 *   base + offset into one literal; gSaveData[4] & 0x80 is the sjisText bit read as a whole byte);
 * - DrawStringTiles calls RenderStringToTiles with a fifth argument, which the ROM passes on the stack. */
extern u8 gTextCanvasBytes[] asm("gTextCanvas");    /* 0x02000000: byte view of struct TextCanvas gTextCanvas */
extern u8 gSaveDataBytes[] asm("gSaveData");        /* 0x02011C20: byte view of struct SaveData gSaveData */
extern void RenderStringToTiles5(u8 *str, u8 *dst, u8 fg, u8 bg, u8 extra) asm("RenderStringToTiles");

extern void *memcpy(void *dst, const void *src, unsigned int n);
extern u8 gDigitTileChars[];        /* 0x08087B94: only used here: the 13 characters LoadDigitTiles turns into tiles */

void AnimStateDrawRaw(struct AnimState *anim, u8 layer, u32 unusedPriority, u8 tileOffset, u8 palette, u32 oam)
{
    u8 i;

    for (i = 0; i < anim->pieceCount; i++)
        OamListAddTemplate((u16 *)&anim->pieces[i], layer, tileOffset, palette, (void *)oam);
}
/* Draw/update every active animation state of a block; returns int (callers ignore it, but the ROM epilogue pops r1). */
int AnimBlockDraw(u8 *block, u8 layer, u8 priority, u8 tileOffset, u8 palette, u8 format, u8 mode, u16 x, u16 y,
                  u32 oam)
{
    u8 i;
    struct AnimState *anim;

    switch (mode) {
    /* The positioned modes (enum OamGroupMode): the block position (x, y) is stored into each
     * state first; the other modes draw at the states' own positions. */
    case OAM_GROUP_ABS_POS:
    case OAM_GROUP_REL_POS:
    case OAM_GROUP_QUAD_ABS_POS:
    case OAM_GROUP_QUAD_REL_POS: {
        int zero = 0;
        for (i = 0; i < ((struct AnimBlock *)block)->count; i++) {
            anim = &((struct AnimBlock *)block)->anims[i];
            if ((s8)anim->active != -1) {
                anim->x = x;
                anim->y = y;
                OamListAddSpriteGroup((u16 *)anim->pieces, layer, anim->pieceCount, anim->x, anim->y, mode,
                                      priority, tileOffset, palette, format, zero, (void *)oam);
            }
        }
        break;
    }
    default:
        for (i = 0; i < ((struct AnimBlock *)block)->count; i++) {
            anim = &((struct AnimBlock *)block)->anims[i];
            if ((s8)anim->active != -1) {
                OamListAddSpriteGroup((u16 *)anim->pieces, layer, anim->pieceCount, anim->x, anim->y, mode,
                                      priority, tileOffset, palette, format, 0, (void *)oam);
            }
        }
        break;
    }
}
u8 AnimBlockInit(struct AnimSeq **scripts, u8 *block)
{
    u8 i = 0;
    struct AnimState *anim;
    struct AnimSeq *seq;

    do {
        anim = &((struct AnimBlock *)block)->anims[i];
        seq = *scripts++;
        anim->seq = seq;
        anim->pieces = seq->pieces;
        anim->pieceCount = seq->pieceCount;
        anim->x |= 0xFFFF;
        anim->y |= 0xFFFF;
        anim->stepIdx = 0;
        anim->active = ANIM_PLAYING;
        anim->timer = seq->frames;
        anim->layer = 0x13 - i;
        i++;
    } while (*scripts != 0);
    ((struct AnimBlock *)block)->count = i;
    return i;
}
void AnimStateTick(struct AnimState *arg)
{
    /* FAKEMATCH: retain the ROM's state/index registers and separate byte mask. */
    register struct AnimState *anim __asm__("r1") = arg;
    const struct AnimSeq *seq;
    u32 next;
    register u32 i __asm__("r3");
    u32 t;
    u32 mask;

    if (anim->active == ANIM_PLAYING) {
        t = anim->timer - 1;
        anim->timer = t;
        mask = 0xFF;
        __asm__ __volatile__("" : "+r"(mask));
        if ((u8)t == 0xFF) {
            next = anim->stepIdx + 1;
            anim->stepIdx = next;
            next &= mask;
            seq = anim->seq;
            if (seq[next].frames == 0) {
                anim->stepIdx = 0;
                anim->active = ANIM_FINISHED;
            }
            i = anim->stepIdx;
            anim->timer = seq[i].frames;
            anim->pieces = seq[i].pieces;
            anim->pieceCount = seq[i].pieceCount;
            /* Keep the unscaled index live through the final sequence-byte load. */
            __asm__ __volatile__("" : : "r"(i));
        }
    }
}

void AnimBlockTick(u8 *block)
{
    u8 i;

    for (i = 0; i < ((struct AnimBlock *)block)->count; i++)
        AnimStateTick(&((struct AnimBlock *)block)->anims[i]);
}

void CopyTileRows(const u8 *src, u8 *dst, u16 colors, u8 rowCount, u8 tilesPerRow)
{
    u16 i;

    if (colors != TILE_COLORS_16) {
        if (colors == TILE_COLORS_256) {
            for (i = 0; i < rowCount; i++) {
                CpuSet(src, dst, (tilesPerRow << 5) & 0x1FFFFF);
                src += 0x200;
                dst += 0x400;
            }
        }
    } else {
        for (i = 0; i < rowCount; i++) {
            CpuSet(src, dst, (tilesPerRow << 4) & 0x1FFFFF);
            src += 0x200;
            dst += 0x400;
        }
    }
}

void FadeStart(u8 color, s16 step, u8 param, struct Fade *fade)
{
    u16 level;
    u32 bld;

    fade->color = color;
    if (step >= 0)
        fade->level = 0;
    else
        fade->level = 0x1000;
    fade->step = step;
    fade->state = FADE_STATE_RUNNING;
    fade->param = param;
    REG_BLDY = fade->level >> 8;
    bld = 0xBF;                     /* BLDCNT: brightness increase (enum FadeColor FADE_WHITE) */
    if (color == FADE_BLACK)
        bld = 0xFF;                 /* BLDCNT: all layers, brightness decrease */
    REG_BLDCNT = bld;
}
u32 FadeTick(struct Fade *fade)
{
    if (fade->state == FADE_STATE_RUNNING && fade->step != 0) {
        fade->level += fade->step;
        if (fade->step > 0) {
            if (fade->level > 0x1000) {
                fade->level = 0x1000;
                fade->step = 0;
                fade->state = FADE_STATE_FADED_OUT;
                SetBldY(0x10);
                return 1;
            }
        } else {
            if (fade->level > 0x1000) {
                fade->level = 0;
                fade->step = 0;
                fade->state = FADE_STATE_FADED_IN;
                SetBldY(0);
                return 1;
            }
        }
        SetBldY(fade->level >> 8);
    }
    return 0;
}
void ClearKatakanaFlag(u8 *flags)
{
    /* Matching: the s32 ~1 gives the ROM's `mov r1,#2; neg r1,r1` form; a plain byte
     * &= ~1 would be narrowed to a #254 immediate instead. */
    s32 m = ~1;

    *flags = *flags & m;
}
/* Render one 8x8 glyph (font 0x0822BB00, 8 bytes each) into 8 4bpp words with a drop shadow:
 * glyph bits get colour `color` and mark buf[row + 1]; buf[row] bits left unset get colour
 * `shadowColor`. The loop bound lives in a variable (n = 8): reload substitutes its constant,
 * which keeps the ROM's unfolded `cmp #8; bcc` (a literal 8 is canonicalised to `cmp #7; bls`). */
void RenderShadowedGlyph(u32 *tile, u8 ch, u8 color, u8 shadowColor, u8 *flags)
{
    u8 buf[12];
    u32 zero;
    u32 c4, c8, c12, c16, c20, c24, c28;
    u32 one = 1;
    u32 w;
    const u8 *g;
    u16 i;
    u16 n = 8;

    c4 = (u32)color << 4;
    c8 = (u32)color << 8;
    c12 = (u32)color << 12;
    c16 = (u32)color << 16;
    c20 = (u32)color << 20;
    c24 = (u32)color << 24;
    c28 = (u32)color << 28;
    zero = 0;
    CpuSet(&zero, buf, 0x05000003);
    g = gFontLatin8x8Bold;
    if (ch > 0x9F) {
        ch -= 0x20;
        if (!(*flags & one))
            ch += 0x40;
    }
    g += ch * (one << 3);
    w = *tile;
    if (*g & 1) { w = (w & ~0xF) | color; buf[1] |= 2; }
    if (*g & 2) { w = (w & ~0xF0) | c4; buf[1] |= 4; }
    if (*g & 4) { w = (w & ~0xF00) | c8; buf[1] |= 8; }
    if (*g & 8) { w = (w & ~0xF000) | c12; buf[1] |= 0x10; }
    if (*g & 0x10) { w = (w & ~0xF0000) | c16; buf[1] |= 0x20; }
    if (*g & 0x20) { w = (w & ~0xF00000) | c20; buf[1] |= 0x40; }
    if (*g & 0x40) { w = (w & ~0xF000000) | c24; buf[1] |= 0x80; }
    if (*g & 0x80) { w = (w & ~0xF0000000) | c28; }
    *tile++ = w;
    g++;
    for (i = 1; i < n; g++, i++) {
        w = *tile;
        if (*g & 1) { w = (w & ~0xF) | color; buf[i + 1] |= 2; }
        if (*g & 2) { w = (w & ~0xF0) | c4; buf[i + 1] |= 4; }
        else if (buf[i] & 2) { w = (w & ~0xF0) | ((u32)shadowColor << 4); }
        if (*g & 4) { w = (w & ~0xF00) | c8; buf[i + 1] |= 8; }
        else if (buf[i] & 4) { w = (w & ~0xF00) | ((u32)shadowColor << 8); }
        if (*g & 8) { w = (w & ~0xF000) | c12; buf[i + 1] |= 0x10; }
        else if (buf[i] & 8) { w = (w & ~0xF000) | ((u32)shadowColor << 12); }
        if (*g & 0x10) { w = (w & ~0xF0000) | c16; buf[i + 1] |= 0x20; }
        else if (buf[i] & 0x10) { w = (w & ~0xF0000) | ((u32)shadowColor << 16); }
        if (*g & 0x20) { w = (w & ~0xF00000) | c20; buf[i + 1] |= 0x40; }
        else if (buf[i] & 0x20) { w = (w & ~0xF00000) | ((u32)shadowColor << 20); }
        if (*g & 0x40) { w = (w & ~0xF000000) | c24; buf[i + 1] |= 0x80; }
        else if (buf[i] & 0x40) { w = (w & ~0xF000000) | ((u32)shadowColor << 24); }
        if (*g & 0x80) { w = (w & ~0xF0000000) | c28; }
        else if (buf[i] & 0x80) { w = (w & ~0xF0000000) | ((u32)shadowColor << 28); }
        *tile++ = w;
    }
}
s16 ParseSignedDecimal(u8 **cursor)
{
    u16 val = 0;
    u8 *p = *cursor;
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
    *cursor = p;
    return (s16)sign * val;
}
/* The unknown 0x20-byte object of UnusedObjectInit (unused in the USA ROM). */
struct Bf {
    u8 flag : 1;                    /* +0x00 bit 0: cleared by UnusedObjectInit */
    u8 rest : 7;
    u8 pad[3];
    u32 value;                      /* +0x04 */
    u8 b[0x18];                     /* +0x08: bytes +0x10 and +0x1C are set to 2 */
};
void UnusedObjectInit(u32 value, u8 *obj)
{
    struct Bf *p = (struct Bf *)obj;
    u8 z, two;

    p->value = value;
    p->flag = 0;
    z = 0;
    two = 2;
    obj[0x10] = two;
    obj[8] = z;
    obj[0x1C] = two;
    obj[0xD] = z;
    obj[0xC] = z;
    obj[0x1D] = z;
}
/* Composite a nibble-per-pixel glyph buffer (EWRAM 0x02000000, width byte at +0x10000) onto solid colour `color`, writing 4bpp tile data. */
void TextCanvasRowsToTiles(u16 *dst, u16 color, u8 x)
{
    s32 i, next;
    s32 w;
    u8 *buf;

    /* FAKEMATCH: do-while(0) raises the loop depth of these refs so fill gets r2 (same trick as TextCanvasToTiles) */
    do {
        color &= 0xF;
        color |= color << 4;
        color |= color << 8;
    } while (0);
    w = gTextCanvasBytes[0x10000];  /* gTextCanvas.width */
    /* FAKEMATCH: do-while(0) weights x's refs so x is allocated (r5) before the block-0 base pointer (r6) */
    do {
        dst += (x << 4) * w;
    } while (0);
    /* FAKEMATCH: explicit guard + do-while (instead of for) keeps the guard on plain w, so w stays in r3 with no copy */
    i = 0;
    if (i < w * 3) {
        /* FAKEMATCH: assigning the base in the preheader keeps the block-0 base live into it (allocated r6) */
        buf = gTextCanvasBytes;
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
void TextDrawGlyphShadowed(u8 *chr, u8 x, u8 y, u8 color, u8 shadowColor, u8 size)
{
    TextDrawGlyph((chr[0] << 8) | chr[1], x + 1, y + 1, shadowColor | (size << 8));
    TextDrawGlyph((chr[0] << 8) | chr[1], x, y, color | (size << 8));
}
u16 ParseDecimal(u8 **cursor)
{
    u16 val = 0;
    u8 n = 0;
    u8 mul = 1;
    u8 i;
    u8 *p = *cursor;

    while (*p != 0x29 && *p != 0x2C) {
        p++;
        n++;
    }
    *cursor = p + 1;
    p--;
    for (i = 0; i < n; i++) {
        val = val + (*p - 0x30) * mul;
        mul = mul * 10;
        p--;
    }
    return val;
}
void PutMapChar(u16 *map, u8 ch, u16 x, u16 y, u8 pal, u16 tileBase, u8 *flags)
{
    if (ch > 0x9F) {
        ch -= 0x20;
        if (!(*flags & 1))
            ch += 0x40;
    }
    map[x + (y << 5)] = ch | tileBase | (pal << 12);
}
/* Expand one 8x8 ASCII glyph (font gFontLatin8x8, addressed by literal, 8 bytes each) to tile
 * halfwords. rightHalf != 0 ORs into the existing data. */
void RenderHalfWidthGlyph(u8 ch, u16 *tile, u16 fg, u16 bg, u16 rightHalf)
{
    u16 *src = (u16 *)(0x08228D00 + ch * 8);
    s32 i;
    u16 m = 0xF;

    for (i = 3; i >= 0; i--) {
        if (rightHalf == 0) {
            *tile++ = ExpandGlyphNibble((*src >> 4) & m, fg, bg);
            *tile++ = ExpandGlyphNibble(*src & 0xF, fg, bg);
            *tile++ = ExpandGlyphNibble((*src >> 12) & m, fg, bg);
            *tile = ExpandGlyphNibble((*src >> 8) & m, fg, bg);
        } else {
            *tile++ |= ExpandGlyphNibble(*src & 0xF, fg, bg);
            *tile++ |= ExpandGlyphNibble((*src >> 4) & m, fg, bg);
            *tile++ |= ExpandGlyphNibble((*src >> 8) & m, fg, bg);
            *tile |= ExpandGlyphNibble((*src >> 12) & m, fg, bg);
        }
        tile++;
        src++;
    }
}
/* Expand one Shift-JIS glyph (font gFontKanji8x8, addressed by literal) to 16 tile halfwords. */
void RenderFullWidthGlyph(u16 sjis, u16 *tile, u16 fg, u16 bg)
{
    u16 *src = (u16 *)(0x081C0000 + SjisToGlyphIndex(sjis) * 8);
    s32 i;
    u16 m = 0xF;

    for (i = 3; i >= 0; i--) {
        *tile++ = ExpandGlyphNibble((*src >> 4) & m, fg, bg);
        *tile++ = ExpandGlyphNibble(*src & 0xF, fg, bg);
        *tile++ = ExpandGlyphNibble((*src >> 12) & m, fg, bg);
        *tile++ = ExpandGlyphNibble((*src >> 8) & m, fg, bg);
        src++;
    }
}
/* Render one 10-row glyph (gFontKanji10x10, addressed by literal, 0x14 bytes per glyph, index from
 * SjisToGlyphIndex) as two 4bpp tile columns: bit i of each row goes to nibble (shift + 7 - i) of the
 * word at dst, and bit (shift + 8 - i) of the byte-swapped row to nibble i of the word at dst + 0x10.
 * Set bits take colour fg, clear bits colour bg; with gSaveData.sjisText (+4 bit 7) the words start as
 * solid bg, otherwise they keep the existing tile data. Rows run 8 per block, two blocks 0x150
 * halfwords apart, stopping after 10 rows. */
void RenderKanji10x10Glyph(u16 sjis, u16 *dst, u8 shift, u16 fg, u16 bg)
{
    u16 *src = (u16 *)(0x081D0200 + SjisToGlyphIndex(sjis) * 20);
    u16 *dst2 = dst + 0x10;
    u8 i, row, blk;
    u8 n = 10;
    u32 w[2]; /* an array, so the pair lives in one DImode pseudo (r5:r6) */
    u32 col;
    u16 b1;
    short b2;

    for (blk = 0; blk < 2; blk++) {
        for (row = 0; row < 8; row++) {
            if (gSaveDataBytes[4] & 0x80) {     /* gSaveData.sjisText */
                w[0] = w[1] = bg | (bg << 4) | (bg << 8) | (bg << 12) | (bg << 16) | (bg << 20) | (bg << 24) | (bg << 28);
            } else {
                w[0] = dst[0] | (dst[1] << 16);
                w[1] = dst2[0] | (dst2[1] << 16);
            }
            for (i = 0; i < 8; i++) {
                b1 = src[0] >> i;
                if (b1 & 1)
                    col = fg;
                else
                    col = bg;
                w[0] = (w[0] & ~(0xF << ((7 - i + shift) * 4))) | (col << ((7 - i + shift) * 4));
                b2 = ((((u8)src[0]) << 8) | (src[0] >> 8)) >> (8 - i + shift);
                if (b2 & 1)
                    col = fg;
                else
                    col = bg;
                w[1] = (w[1] & ~(0xF << (i * 4))) | (col << (i * 4));
            }
            /* FAKEMATCH: dead store; it only enlarges the row loop for loop.c so that just
             * (bg | bg << 4 | bg << 8) is hoisted out of the loops, as in the ROM. */
            b2 = ((((u8)src[0]) << 8) | (src[0] >> 8)) >> (8 - i + shift);
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
/* Render a string as glyph tiles into `tiles` (0x20 bytes per tile); ASCII glyphs are half width so two share a tile. */
void RenderStringToTiles(u8 *str, u8 *tiles, u8 fg, u8 bg)
{
    u8 half = 0;
    u16 col = 0;
    u8 row = 0;
    u16 ch;
    u8 *p;

    p = str;
    while (*p != 0) {
        if (gSaveDataBytes[4] & 0x80) {         /* gSaveData.sjisText */
            ch = (p[0] << 8) | p[1];
            if (ch > 0x813F) {
                RenderFullWidthGlyph(ch, (u16 *)(tiles + ((col++ + row * 32) << 5)), fg, bg);
            }
            p += 2;
        } else {
            RenderHalfWidthGlyph(p[0], (u16 *)(tiles + ((col + row * 32) << 5)), fg, bg, half);
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
/* Write `count` consecutive tile indices starting at `firstTile` into a tilemap: MAP_CELL_1X1 = one
 * map cell each, MAP_CELL_2X2 = 2x2 cells (tile*4 + 0..3). */
void PutMapTileRun(u16 firstTile, u16 *map, u8 pal, u8 cellMode, u8 count)
{
    u8 i;
    u8 col;
    short row;

    col = 0;
    row = 0;
    for (i = 0; i < count; i++) {
        switch (cellMode) {
        case MAP_CELL_1X1:
            map[row + col++] = firstTile | (pal << 12);
            break;
        case MAP_CELL_2X2: {
            u16 *e = map + (row + col);
            u32 t = firstTile << 2;
            u32 pl = pal << 12;

            e[0] = pl | t;
            e[1] = (t + 1) | pl;
            e[0x20] = (t + 2) | pl;
            e[0x21] = (t + 3) | pl;
            col += 2;
            break;
        }
        }
        firstTile++;
    }
}
/* Draw a string as text on a 2-row tilemap strip (upper and lower half of each glyph). A map at
 * VRAM + 0xC7C8 (row 31) sends its second row to row 0 (VRAM + 0xBFC8). */
void DrawTextStrip(u8 *str, u8 *map, u32 tiles, u16 firstTile, u8 pal, u8 color, u8 bgColor, u8 cellMode)
{
    u8 n;
    u8 *q;
    u8 len;

    n = 0;
    q = str;
    while (*q++ != 0)
        n++;
    len = (n * 5 + 7) >> 3;
    TextCanvasInit(len, 2);
    TextDrawString(0, 0, color | 0xA00, str);       /* sizeColor: 10 px font, colour `color` */
    TextCanvasToTiles((u16 *)tiles, bgColor);
    PutMapTileRun(firstTile, (u16 *)map, pal, cellMode, len);
    if (map == (u8 *)(VRAM + 0xC7C8))
        map = (u8 *)(VRAM + 0xBFC8);
    PutMapTileRun(firstTile + len, (u16 *)(map + 0x40), pal, cellMode, len);
}

void DrawStringTiles(u8 *str, u32 map, u32 tiles, u16 firstTile, u8 pal, u8 fg, u8 bg, u8 cellMode)
{
    u8 n;

    RenderStringToTiles5(str, (u8 *)tiles, fg, bg, cellMode);
    n = 0;
    while (*str++ != 0)
        n++;
    PutMapTileRun(firstTile, (u16 *)map, pal, cellMode, (n + 1) >> 1);
}
void LoadDigitTiles(u8 *tiles, u8 unused, u8 color)
{
    u8 buf[0x10];
    vu16 zero;
    vu32 *dma;
    vu32 *d2;
    u8 i;

    memcpy(buf, gDigitTileChars, 13);
    zero = 0;
    /* DMA3 (registers at 0x040000D4): fill 0xD0 halfwords at tiles with zero, then wait for it.
     * Matching: the wait loop reads through a second pointer d2 so the ROM keeps a base copy. */
    dma = (vu32 *)0x040000D4;                       /* REG_DMA3SAD */
    dma[0] = (u32)&zero;                            /* REG_DMA3SAD */
    dma[1] = (u32)tiles;                            /* REG_DMA3DAD */
    dma[2] = 0x810000D0;                            /* REG_DMA3CNT: enable, fill 0xD0 halfwords */
    dma[2];
    d2 = (vu32 *)0x040000D4;
    while (d2[2] & 0x80000000)
        ;
    for (i = 0; i < 13; i++)
        OverlayBoldGlyphTile((u32 *)(tiles + i * 32), buf + i, color, 4);
}
/* Print `value` as decimal digits right to left starting at (x, y) of a tilemap; mode 0 = zero
 * padded to `digits` digits, mode 1 = no leading zeros. */
void DrawNumberTiles(u16 value, u8 digits, u8 mode, u16 *map, u8 x, u8 y, u8 pal, u16 digitTile0, u8 cellMode)
{
    u8 i;
    u16 d;

    switch (mode) {
    case NUMBER_ZERO_PAD:
        for (i = 0; i < digits; i++) {
            d = value % 10;
            value = value / 10;
            PutMapTileRun(digitTile0 + d, map + (x-- + y * 32), pal, cellMode, 1);
        }
        break;
    case NUMBER_NO_LEADING_ZEROS:
        if (value == 0) {
            PutMapTileRun(digitTile0, map + (x-- + y * 32), pal, cellMode, 1);
            return;
        }
        for (i = 0; i < digits; i++) {
            d = value % 10;
            value = value / 10;
            if (d == 0 && value == 0)
                return;
            PutMapTileRun(digitTile0 + d, map + (x-- + y * 32), pal, cellMode, 1);
        }
        break;
    }
}
