#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

/*
 * Text rendering. The game draws text in four ways:
 *   1. the text canvas gTextCanvas: TextDraw* plot 1bpp font glyphs into an 8bpp tile-ordered bitmap at
 *      0x02000000, and TextCanvasToTiles / TextCanvasRowsToTiles convert it to 4bpp tiles for VRAM;
 *   2. tile renderers: Render*Glyph* expand one glyph into a 4bpp tile, and DrawBg* / DrawStringTiles /
 *      DrawNumberTiles put the tiles and their map entries on a BG;
 *   3. map characters (PutMap*): one map entry per character on a BG whose tiles hold a whole font (dead
 *      Japanese-engine code);
 *   4. bitmap printers (BitmapDraw*): glyphs straight into a linear 8bpp bitmap such as a Mode-4 page (the
 *      bustup text box).
 * Strings are 1-byte CP1252 in the USA game. When gSaveData.sjisText (save byte +4 bit 7) is set, the
 * dispatchers (TextDrawGlyph, TextDrawString, TextDrawNumber, BitmapDrawStringShadow, RenderStringToTiles)
 * take the 2-byte Shift-JIS path and the kanji fonts instead; the USA game always clears it (SetTextModeLatin).
 *
 * sizeColor arguments pack the font size (8, 10, 12 or 16 px) in the high byte and the colour index in the
 * low byte (TEXT_SIZE_COLOR). Defined in text_canvas, text_render, text_bg, bitmap_text, main, sprite and
 * deck_edit_panel.
 *
 * Every prototype is the function's definition as compiled. Some units call these functions through a
 * different local declaration (other widths, fewer or more arguments); they keep that view as a commented
 * local alias prototype (build/readability/proto_mismatches.txt).
 */

#include "global.h"

/* The sizeColor argument: font size in the high byte, colour index in the low byte. TEXT_SIZE alone is for
 * places where the ROM ORs the colour in first (colour | TEXT_SIZE(12)); the operand order can matter. */
#define TEXT_SIZE(size) ((size) << 8)
#define TEXT_SIZE_COLOR(size, color) (((size) << 8) | (color))

/* ---- The text canvas ---- */

/* gTextCanvas (0x02000000): the canvas the TextDraw* functions plot into. One byte per pixel, 64 bytes per
 * 8 x 8 tile, tiles in row-major order `width` tiles per row. The matched code reaches the header through
 * the byte view gTextCanvas[0x10000 + n] (a struct view folds base + offset into one literal). */
struct TextCanvas {
    u8 pixels[0x10000];   /* +0x00000: colour index per pixel, tile-ordered */
    u8 width;             /* +0x10000: width in tiles (TextCanvasInit) */
    u8 height;            /* +0x10001: height in tiles */
    u8 right;             /* +0x10002: max x + advance reached by the last TextDraw*String */
    u8 bottom;            /* +0x10003: max y + glyph size reached by the last TextDraw*String */
    union {
        u8 raw;           /* the whole byte; TextCanvasInit clears it */
        struct {
            u8 lineGap:7;   /* bits 0-6: extra pixels between wrapped lines */
            u8 wordWrap:1;  /* bit 7: wrap strings at the canvas width */
        } bits;
    } layout;             /* +0x10004: set by TextCanvasInitEx */
};

extern struct TextCanvas gTextCanvas;

/* Sets gTextCanvas.width/height, no word wrap or line gap, and clears the pixels. */
void TextCanvasInit(u32 widthTiles, u32 heightTiles);
/* As TextCanvasInit with layout = (lineGap & 0x7F) | wordWrap << 7. */
void TextCanvasInitEx(u32 widthTiles, u32 heightTiles, u16 wordWrap, u32 lineGap);
/* Packs the whole canvas (width x height tiles) into 4bpp tiles at dst; colour 0 becomes bgColor & 0xF.
 * Source pixels are not masked, so they must be < 16. */
void TextCanvasToTiles(u16 *dst, u16 bgColor);
/* Packs 3 tile rows of the canvas, from tile row tileRow, into 4bpp tiles at the same tile offsets in dst. */
void TextCanvasRowsToTiles(u16 *dst, u16 bgColor, u8 tileRow);
/* Plots an 8-pixel 1bpp row (bit 7 = leftmost) at (x, y) of the canvas in `color`. */
void TextPlotRow8(u8 bits, s32 x, s32 y, u32 color);
/* Plots a 16-pixel 1bpp row (bit 15 = leftmost). */
void TextPlotRow16(u16 bits, s32 x, s32 y, u32 color);
/* Shift-JIS glyph from the 8x8, 10x10 or 12x12 kanji font. Every row is drawn one pixel left. */
void TextDrawSjisGlyph(u16 sjis, s32 x, s32 y, u16 sizeColor);
/* CP1252 glyph from the 8x8, 8x10, 8x12 or 8x16 Latin font. */
void TextDrawLatinGlyph(u8 ch, s32 x, s32 y, u16 sizeColor);
/* TextDrawSjisGlyph or TextDrawLatinGlyph, by gSaveData.sjisText. */
void TextDrawGlyph(u16 ch, s32 x, s32 y, u16 sizeColor);
/* Draws a drop-shadowed glyph: the big-endian 2-byte char at chr in shadowColor at (x + 1, y + 1), then in
 * color at (x, y). */
void TextDrawGlyphShadowed(u8 *chr, u8 x, u8 y, u8 color, u8 shadowColor, u8 size);
/* 2-byte Shift-JIS string, advance `size` per glyph; with word wrap it applies Japanese line breaking
 * (kinsoku, see IsLineStartForbidden / IsLineEndForbidden). Tracks gTextCanvas.right/bottom. */
void TextDrawSjisString(s32 x, s32 y, u16 sizeColor, const u8 *str);
/* 1-byte string, advance size / 2 (+1 for size 16); with word wrap, wraps before a word that does not fit.
 * Tracks gTextCanvas.right/bottom. */
void TextDrawLatinString(s32 x, s32 y, u16 sizeColor, const u8 *str);
/* TextDrawSjisString or TextDrawLatinString, by gSaveData.sjisText. */
void TextDrawString(s32 x, s32 y, u16 sizeColor, const u8 *str);
/* Right-aligned decimal in full-width Shift-JIS digits: x is the position of the last digit. */
void TextDrawSjisNumber(s32 x, s32 y, u16 sizeColor, s32 value);
/* Right-aligned decimal in Latin digits. */
void TextDrawLatinNumber(s32 x, s32 y, u16 sizeColor, s32 value);
/* TextDrawSjisNumber or TextDrawLatinNumber, by gSaveData.sjisText. The definition names only three
 * parameters: the value (s32, 4th argument) passes through r3 untouched, so callers pass four arguments
 * through a local 4-parameter view. */
void TextDrawNumber(u32 x, u32 y, u16 sizeColor);
/* Visible characters of the next word: up to a NUL, space, LF or backslash-n pair; the colour codes @0, @2
 * and @3 have no width. */
u8 TextWordLength(const u8 *str);

/* ---- Glyphs and strings to 4bpp tiles ---- */

/* The kana flags byte of PutMapString / PutMapNumber / PutMapChar / RenderShadowedGlyph (gDeckEdit + 0x640). */
struct TextFlags {
    u8 katakana:1;  /* bit 0: kana codes 0xA0+ use the katakana bank (ch - 0x20) instead of the hiragana bank
                       (ch + 0x20). PutMapString sets it at the start and on '\n' and clears it on '\r' */
    u8 unused:7;    /* bits 1-7 */
};

/* Four 1bpp font bits (bit 3 = leftmost) -> four 4bpp pixels: set bits fg & 0xF, clear bits bg & 0xF. */
u16 ExpandGlyphNibble(u16 bits, u16 fg, u16 bg);
/* Glyph ch of gFontLatin8x8Bold into one 4bpp tile at dst. */
void RenderBoldGlyphTile(u8 ch, u16 *dst, u16 fg, u16 bg);
/* 8x8 Shift-JIS glyph into one 4bpp tile at dst (same code as RenderFullWidthGlyph). */
void RenderSjisGlyphTile(u16 sjis, u16 *dst, u16 fg, u16 bg);
/* 8x8 Shift-JIS glyph into one 4bpp tile (same code as RenderSjisGlyphTile: two copies in the ROM). */
void RenderFullWidthGlyph(u16 sjis, u16 *tile, u16 fg, u16 bg);
/* Glyph ch of gFontLatin8x8 (ASCII glyphs are 4 px wide, in the left half). rightHalf == 0 writes the whole
 * tile; rightHalf != 0 ORs the glyph into pixels 4-7 and bg into 0-3, so bg must be 0 for two per tile. */
void RenderHalfWidthGlyph(u8 ch, u16 *tile, u16 fg, u16 bg, u16 rightHalf);
/* 10x10 Shift-JIS glyph across two 4bpp tiles: dst (moved right by `shift`) and dst + 0x20 bytes. */
void RenderKanji10x10Glyph(u16 sjis, u16 *dst, u8 shift, u16 fg, u16 bg);
/* NUL-terminated string into consecutive 4bpp tiles: 2-byte chars one per tile (Shift-JIS mode), else 1-byte
 * chars two per tile. */
void RenderStringToTiles(u8 *str, u8 *tiles, u8 fg, u8 bg);
/* Paints glyph *ch of gFontLatin8x8Bold into an existing tile (bpp 4 or 8); clear bits keep the old pixel. */
void OverlayBoldGlyphTile(u32 *tile, u8 *ch, u8 color, u8 bpp);
/* Draws glyph ch of gFontLatin8x8Bold into an existing 4bpp tile with a shadow at (+1, +1) in shadowColor.
 * Kana codes 0xA0+ use the bank picked by flags (a struct TextFlags). Japanese-engine leftover: it expects
 * LSB-left rows, so USA glyphs would come out mirrored; its only caller is RenderOutlinedFontTiles. */
void RenderShadowedGlyph(u32 *tile, u8 ch, u8 color, u8 shadowColor, u8 *flags);
/* Unused. Clears 0x400 bytes at dst, then renders codes 0x20-0xFF (0x20 bytes each) after it with
 * RenderShadowedGlyph: 0x80-0xBF and 0xC0-0xFF both start from glyph 0xA0 and differ only in flags->katakana. */
void RenderOutlinedFontTiles(u32 dst, u8 color, u8 outlineColor, struct TextFlags *flags);

/* ---- Strings on BG maps ---- */

/* `cellMode` argument of PutMapTileRun, DrawStringTiles, DrawTextStrip and DrawNumberTiles. */
enum MapCellMode {
    MAP_CELL_1X1 = 0, /* one map cell per tile */
    MAP_CELL_2X2 = 1  /* a 2 x 2 cell block per tile (tile * 4 + 0..3, two columns per tile) */
};

/* `mode` argument of DrawNumberTiles (compare NumberDrawMode in bg.h and NumberSpriteMode in sprite.h). */
enum NumberMode {
    NUMBER_ZERO_PAD = 0,         /* exactly `digits` digits */
    NUMBER_NO_LEADING_ZEROS = 1  /* stops once the rest of the value is 0 (a single 0 for value 0) */
};

/* Sets gMain.textAreaStart / textAreaEnd (map cells, row * 32 + col) used by the DrawBg* wrapping. */
void SetTextArea(u16 startCell, u16 endCell);
/* Draws a byte string with the 8x8 bold font into gMain.bgMapBuffer from `cell`: one new tile per char at
 * 0x06004000 + tile * 32 (fg = colors & 0xFF, bg = colors >> 8), no wrapping. */
void DrawBgString(u16 cell, u16 colors, u16 tile, char *str);
/* Unused. As DrawBgString for a 2-byte Shift-JIS string (big-endian), wrapping with the kinsoku rules. */
void DrawBgSjisString(u16 cell, u16 colors, u16 tile, u8 *str);
/* As DrawBgString with full-width Shift-JIS glyphs for an ASCII string, wrapping at the text area. */
void DrawBgFullwidthString(u16 cell, u16 colors, u16 tile, u8 *str);
/* Prints |value| right-aligned with DrawBgString. Packed arguments: cellColors = cell | colors << 16,
 * tileDigits = firstTile | digits << 16 (at most 8); leading positions are '0' if zeroPad, else ' '. */
void DrawBgDecimal(u32 cellColors, u32 tileDigits, int value, u16 zeroPad);
/* As DrawBgDecimal in uppercase hexadecimal, always zero padded. */
void DrawBgHex(u32 cellColors, u32 tileDigits, int value);
/* Writes `count` consecutive tile numbers from firstTile (| pal << 12) into a map row, per cellMode
 * (enum MapCellMode); any other mode writes nothing. */
void PutMapTileRun(u16 firstTile, u16 *map, u8 pal, u8 cellMode, u8 count);
/* RenderStringToTiles into `tiles` (VRAM address), then one map row of PutMapTileRun at `map` (VRAM address):
 * two half-width chars per tile. */
void DrawStringTiles(u8 *str, u32 map, u32 tiles, u16 firstTile, u8 pal, u8 fg, u8 bg, u8 cellMode);
/* Draws str with the 10-px font through the text canvas (5 px per char, 2 tile rows) into `tiles` (VRAM
 * address), then two map rows from firstTile. A map at 0x0600C7C8 (row 31) sends its second row to row 0. */
void DrawTextStrip(u8 *str, u8 *map, u32 tiles, u16 firstTile, u8 pal, u8 color, u8 bgColor, u8 cellMode);
/* Clears 13 tiles at `tiles` and renders "0123456789X?" plus a blank into them in `color`: tile 0-9 digits,
 * 10 'X', 11 '?', 12 blank (the layout DrawNumberTiles expects). */
void LoadDigitTiles(u8 *tiles, u8 unused, u8 color);
/* Prints value leftwards from cell (x, y) using digit tiles digitTile0 + d; mode is an enum NumberMode. */
void DrawNumberTiles(u16 value, u8 digits, u8 mode, u16 *map, u8 x, u8 y, u8 pal, u16 digitTile0, u8 cellMode);

/* ---- Map characters (Japanese-engine leftovers) ----
 * PutMapString and PutMapNumber have no callers in the USA ROM (PutMapChar is reached only through them);
 * the Deck Edit screens still clear the kana flag at gDeckEdit + 0x640 with ClearKatakanaFlag. */

/* `mode` argument of PutMapNumber. */
enum NumberPadMode {
    NUMBER_PAD_ZEROS = 0, /* exactly `digits` digits, zero padded */
    NUMBER_PAD_NONE = 1   /* stops at the first leading zero (a single '0' for 0); other modes draw nothing */
};

/* flags->katakana = 0; `flags` is a struct TextFlags. */
void ClearKatakanaFlag(u8 *flags);
/* map[x + y * 32] = tile | tileBase | pal << 12; kana codes 0xA0-0xDF map to the bank picked by flags
 * (a struct TextFlags). */
void PutMapChar(u16 *map, u8 ch, u16 x, u16 y, u8 pal, u16 tileBase, u8 *flags);
/* Writes up to maxChars 1-byte characters at (x++ & 31, y & 31) with PutMapChar. '\n' / '\r' switch the kana
 * bank instead of breaking lines; dakuten/handakuten (0xDE/0xDF) go one cell up-left. */
void PutMapString(u8 *str, void *map, u16 x, u16 y, u8 palette, u16 tileBase, u8 maxChars,
                  struct TextFlags *kana);
/* Writes num as decimal map characters right to left from column x; mode is an enum NumberPadMode and `kana`
 * a struct TextFlags *. */
void PutMapNumber(u16 num, u8 digits, u8 mode, void *map, u16 x, u16 y, u8 palette, u16 tileBase, void *kana);

/* ---- Bitmap printers (linear 8bpp bitmaps, e.g. a Mode-4 page) ----
 * `width` is the bitmap width in pixels; only bpp 8 works (the bpp 4 paths are broken and unused). */

/* Plots an 8-pixel 1bpp row (bit 7 = left) at (x, y), by read-modify-write of halfwords (VRAM takes no byte
 * writes). */
void BitmapPlotRow8(u8 bits, u16 x, u16 y, u8 color, u16 *bitmap, u16 width, u8 bpp);
/* Plots a 16-pixel 1bpp row (bit 15 = left). */
void BitmapPlotRow16(u16 bits, u16 x, u16 y, u8 color, u16 *bitmap, u16 width, u8 bpp);
/* One Shift-JIS glyph, size 8, 10 or 12 (other sizes draw nothing); codes <= 0x813F are converted with
 * AsciiToFullwidthSjis first. */
void BitmapDrawSjisGlyph(u16 sjis, u16 x, u16 y, u16 *bitmap, u8 color, u8 size, u16 width, u8 bpp);
/* One CP1252 glyph, size 8, 10 or 12. The fonts are addressed by integer literals on purpose (matching). */
void BitmapDrawLatinGlyph(u8 ch, u16 x, u16 y, u16 *bitmap, u8 color, u8 size, u16 width, u8 bpp);
/* 1-byte string, shadow at (+1, +1) first, advance size / 2 per char. */
void BitmapDrawLatinStringShadow(u8 *str, u16 x, u16 y, void *bitmap, u8 color, u8 shadowColor, u8 size,
                                 u16 width, u8 bpp);
/* 2-byte Shift-JIS string, shadow first, advance `size` per char. */
void BitmapDrawSjisStringShadow(u8 *str, u16 x, u16 y, void *bitmap, u8 color, u8 shadowColor, u8 size,
                                u16 width, u8 bpp);
/* BitmapDrawSjisStringShadow or BitmapDrawLatinStringShadow, by gSaveData.sjisText. The bustup text box uses it
 * on a 240-px page with shadow colour 14, size 12, bpp 8. */
void BitmapDrawStringShadow(u8 *str, u16 x, u16 y, void *bitmap, u8 color, u8 shadowColor, u8 size, u16 width,
                            u8 bpp);
/* Word-wrap test of the bustup text box: 1 if col + width of the next word <= maxCol. `$x` codes have no
 * width; in Shift-JIS mode every 2-byte char ends a word. */
u8 NextWordFits(u8 *s, u8 col, u8 maxCol);

/* ---- Shift-JIS and text modes ---- */

/* Selects the text path: gSaveData.sjisText = (mode == 0), and the language bits = mode & 0x7F. The USA
 * game always passes 1 (hypothesis: a localisation switch). */
void SetTextMode(u16 mode);
/* SetTextMode(1): the 1-byte Latin renderers. */
void SetTextModeLatin(void);
/* Glyph index of a Shift-JIS code in the kanji fonts: (lead - 0x80, or - 0xC0 above 0x9F) * 0xC0 +
 * trail - 0x40. */
u32 SjisToGlyphIndex(u16 sjis);
/* ASCII 0x20-0x7E -> full-width Shift-JIS code ('A' -> 0x8260); anything else -> 0. */
u16 AsciiToFullwidthSjis(u16 ch);
/* 1 for the 32 Shift-JIS characters that may not begin a line (closing punctuation, small kana). */
int IsLineStartForbidden(u16 sjis);
/* 1 for the opening brackets 0x8169 and 0x8175, which may not end a line. */
int IsLineEndForbidden(u16 sjis);

/* ---- Small parsers and helpers ---- */

/* Reads the digits before the next ')' or ',' and leaves *cursor one past it. Only up to 3 digits are right
 * (u8 place multiplier). */
u16 ParseDecimal(u8 **cursor);
/* As ParseDecimal with a '-' terminator too; the sign branch is unreachable, so "-n" parses as 0. */
s16 ParseSignedDecimal(u8 **cursor);
/* Two ASCII digits as a number; a non-digit counts as 0. */
u8 ParseTwoDigits(const u8 *s);
/* `count` characters as a decimal number (non-digits add 0 but still shift). */
u32 ParseDigits(const u8 *s, u32 count);
/* Length of a NUL-terminated string as a u8 (wraps past 255). */
u8 StrLenU8(const u8 *s);
/* cond ? a : b. */
u16 SelectU16(u16 cond, u16 a, u16 b);
/* Unused. Initialises an unknown 0x20-byte object: bit 0 of +0 cleared, +4 = value, +0x10 = +0x1C = 2,
 * +8 / +0xC / +0xD / +0x1D = 0. */
void sub_08078CA8(u32 value, u8 *obj);

/* ---- Fonts and the system palette (ROM) ----
 * 1bpp fonts, bit 7 (or bit 15) = leftmost pixel. Several units address the fonts by integer literal on
 * purpose (matching); keep those accesses as they are. */

extern const u8 gFontKanji8x8[];    /* Shift-JIS 8x8, 8256 glyphs x 8 bytes */
extern const u16 gFontKanji10x10[]; /* Shift-JIS 10x10, 8256 glyphs x 10 big-endian u16 rows */
extern const u8 gFontKanji12x12[];  /* Shift-JIS 12x12, 24 bytes per glyph */
extern const u8 gFontLatin8x8[];    /* CP1252 8x8, 256 x 8 bytes; ASCII glyphs 4 px wide in the left half */
extern const u8 gFontLatin8x10[];   /* CP1252 8x10, 10 bytes per glyph */
extern const u8 gFontLatin8x12[];   /* CP1252 8x12, 12 bytes per glyph (the bustup text font) */
extern const u8 gFontLatin8x8Bold[]; /* CP1252 8x8 bold, 256 x 8 bytes */
/* System font 16-colour palette (gfx/system.pal), loaded to BG palette 15 by the text screens and to
 * palette 0 by LoadSystemGfx. */
extern const u8 gSystemFontPal[];

/* Compile-time layout checks (agbcc pads every struct to 4 bytes). */
typedef char text_h_check_canvas[sizeof(struct TextCanvas) == 0x10008 ? 1 : -1];
typedef char text_h_check_canvas_width[(u32)&((struct TextCanvas *)0)->width == 0x10000 ? 1 : -1];
typedef char text_h_check_canvas_layout[(u32)&((struct TextCanvas *)0)->layout == 0x10004 ? 1 : -1];
typedef char text_h_check_flags[sizeof(struct TextFlags) == 0x4 ? 1 : -1];

#endif /* GUARD_TEXT_H */
