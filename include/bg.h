#ifndef GUARD_BG_H
#define GUARD_BG_H

/*
 * BG maps and tiles: the image-pack loaders (LoadBgImage*), rectangle fill/copy/crop helpers for tilemaps
 * in RAM and in VRAM, tile sheet copies, the LZSS decoder, the card art loaders and the BG scroll/affine
 * registers. Defined in bg_image, gfx_util, bitmap_text, text_bg, collection, text_render, main and link_sio.
 *
 * Terms used below:
 *   map entry    one text-BG map halfword: tile number (bits 0-9) | 0x400 hflip | 0x800 vflip | palette << 12.
 *   cell         index of an entry in a 32-entry-wide map, x + y * 32.
 *   map buffer   gMain.bgMapBuffer[n] (n = 0..7, 0x800 bytes each): the RAM copies of the BG maps that the
 *                VBlank handler uploads.
 *   screenBlock  VRAM BG map n at 0x06000000 + n * 0x800 (32 x 32 entries). The *Vram* and *Screenblock*
 *                helpers write VRAM directly.
 *   colors       0x10 = 4bpp tiles (32 bytes), 0x100 = 8bpp tiles (64 bytes); see enum TileColors.
 *
 * Every prototype is the function's definition as compiled. Some units call these functions through a
 * different local declaration (other widths, fewer or more arguments); they keep that view as a commented
 * local alias prototype (build/readability/proto_mismatches.txt).
 */

#include "global.h"

struct ObjAffine; /* sprite.h */

/*
 * Image pack: a raw ROM container holding a palette, tiles and a sparse tile map (wiki data/graphics-formats).
 * Each section starts with its element count, stored four times:
 *     struct ImagePackCount colorCount;  u16 palette[colorCount.count];               BGR555
 *     struct ImagePackCount tileCount;   u8 tiles[tileCount.count][64];                [32] for 4bpp packs
 *     struct ImagePackCount cellCount;   struct ImagePackCell cells[cellCount.count];
 * The loaders take the pack as a const u16 * and walk it section by section.
 */
struct ImagePackCount {
    u16 count;      /* +0x0: element count; the loaders read only this copy */
    u16 copies[3];  /* +0x2: the same count three more times */
};

/* One non-blank 8x8 block of an image pack. Blocks that are fully transparent have no cell. */
struct ImagePackCell {
    u16 pos;   /* +0x0: x (bits 0-5) | y << 8, in map cells */
    u16 tile;  /* +0x2: tile number within the pack; the loader adds its tileBase */
};

/* `colors` argument of CopyTileSheetTo2D, CopyTileSheetRowsTo2D, CopyTileRectTo2D and CopyTileRows. Any other
 * value copies nothing. */
enum TileColors {
    TILE_COLORS_16 = 0x10,  /* 4bpp, 32 bytes per tile */
    TILE_COLORS_256 = 0x100 /* 8bpp, 64 bytes per tile */
};

/* `mode` argument of DrawVramMapNumber3 (compare NumberMode in text.h and NumberSpriteMode in sprite.h). */
enum NumberDrawMode {
    NUMBER_DRAW_ALL_DIGITS = 0, /* always 3 digits, leading zeros included */
    NUMBER_DRAW_SKIP_ZEROS = 1  /* every 0 digit is left out without advancing, so 105 shows as "15" */
};

/* 0xE0 4bpp tiles (0x1C00 bytes) of BG tile staging at 0x02010014. ClearBgMapBuffers and ClearBgMapBuffer0
 * zero it, and only the unused CopyBgTileBufferToVram uploads it (to charblock 1, tile 0x20). */
extern u8 gBgTileBuffer[0x1C00];

/* ---- Image-pack loaders ----
 * Each one copies the palette to BG palette RAM from colour palStart, the tiles to 0x06004000 + tileBase * 32,
 * and one map entry per cell to a map buffer at cell mapOffset + x + y * 32; it returns the tile count.
 * 8bpp packs (LoadBgImage, LoadBgImageMap1): palStart is added to every non-zero pixel (colour 0 stays
 * transparent) and the entries are tile + tileBase / 2, with no palette bits.
 * 4bpp packs: the entries are (tile + tileBase) | (palStart >> 4) << 12. */

/* 8bpp image pack into gMain.bgMapBuffer[0]. */
u16 LoadBgImage(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack);
/* 8bpp image pack into gMain.bgMapBuffer[1]; otherwise the same code as LoadBgImage. */
u16 LoadBgImageMap1(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack);
/* 4bpp image pack into gMain.bgMapBuffer[0]. */
u16 LoadBgImage4bpp(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack);
/* 4bpp image pack into gMain.bgMapBuffer[1]. */
u16 LoadBgImage4bppMap1(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack);
/* 4bpp image pack into gMain.bgMapBuffer[map]. */
u16 LoadBgImage4bppToMap(u32 map, u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack);
/* 4bpp image pack: palette and tiles only, no map entries. */
u16 LoadBgImage4bppGfx(u16 palStart, u16 tileBase, const u16 *pack);
/* Unused. 4bpp pack into gMain.bgMapBuffer[1] with each pos taken minus the first cell's pos, so the first cell
 * lands at mapOffset. The packed subtraction makes a cell left of the first one borrow from y. */
u16 LoadBgImage4bppMap1Rel(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack);
/* Unused. 4bpp pack into gMain.bgMapBuffer[4] with cell positions relative to the first cell; positions break
 * (u16 wrap) when a later cell has a smaller x than the first. */
u16 LoadBgImage4bppMap4Rel(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack);

/* ---- Map buffers (gMain.bgMapBuffer) and video setup ---- */

/* Clears all 8 map buffers, gBgTileBuffer and the u16 at 0x02010010 (which nothing else uses). */
void ClearBgMapBuffers(void);
/* Clears map buffer 0, gBgTileBuffer and the u16 at 0x02010010. */
void ClearBgMapBuffer0(void);
/* Fills a width x height block of map buffer `map` from `cell` with gMapFillTile[0] (0, so it clears it). */
void FillMapRect(u16 map, u16 cell, u16 width, u16 height);
/* gMain.bgMapBuffer[screenBlock][cell] = entry. */
void SetBgMapEntry(u16 screenBlock, u16 cell, u16 entry);
/* Card picture as a 9 x 10-cell block of 8bpp tiles: map entries from tileBase / 2 at `cell` of map buffer
 * screenBlock & 7, the card's 64 colours to palette bank palBase >> 4, and the 72 x 80 art unpacked to
 * 0x06004000 + tileBase * 32 with (u8)palBase added to every pixel. */
void DrawCardPortrait(u16 screenBlock, u16 cell, u16 cardId, u16 tileBase, u16 palBase);
/* Unused. Uploads gBgTileBuffer to 0x06004400 (charblock 1, tiles 0x20-0xFF); the buffer is only ever zero. */
void CopyBgTileBufferToVram(void);
/* Video reset: clears the map buffers, loads the system font, sets the text area to cells 0..0x27E, BG2/BG3
 * reference points 0 and identity matrices, BG0CNT = charblock 1, screenblock 0. */
void ResetVideo(void);
/* System palette (gSystemFontPal) to BG and OBJ palette 0, colour 0 forced to black, and the first 16 system
 * tiles to BG tile 0 at 0x06004000 and OBJ tile 0 at 0x06010000. */
void LoadSystemGfx(void);

/* ---- Rectangles in 32-entry-wide RAM maps ---- */

/* Fills a w x h rectangle with `tile`, wrapping both coordinates at 32. */
void FillMapRectWrap(u16 tile, u16 *map, u16 x, u16 y, u16 w, u16 h);
/* Zeroes a w x h rectangle starting at map. */
void ClearMapRect(u16 *map, u8 w, u8 h);
/* Unused. Writes ascending entries (tile & 0x3FF)++ | (pal & 0xF) << 12 into a w x h rectangle at dst. */
void FillMapRectSeqPal(u16 *dst, u16 tile, u8 pal, u8 w, u8 h);
/* Copies h rows of w entries from a packed w-wide source (u16 entries) into a 32-wide map. */
void CopyMapRect(const void *src, void *dst, u8 w, u8 h);
/* Unused. CopyMapRect with an explicit destination stride in entries. */
void CopyMapRectStride(const void *src, void *dst, u8 w, u8 h, u8 dstStride);
/* Copies a w x h block (source stride srcStride) adding pal << 12 + tileHi << 8 to every entry. */
void CopyMapRectAddOffset(u16 *src, u16 *dst, u8 w, u8 h, u8 srcStride, u8 pal, u8 tileHi);
/* Copies a w x h block keeping only the source tile number (& 0x3FF), ORed with pal << 12 | tileHi << 8. */
void CopyMapRectSetPalette(u16 *src, u16 *dst, u8 w, u8 h, u8 srcStride, u8 pal, u8 tileHi);
/* Unused. Streams h rows of a wide map into a 32 x 32 ring map: columns wrap at 32, rows wrap back 31 rows
 * when the row counter (starting at dstRow) reaches 32. */
void CopyMapRectWrapped(u16 *src, u16 *dst, u8 w, u8 h, u16 srcX, u16 dstRow, u8 srcStride);
/* Unused. Copies a packed w x h block, renumbering tiles of a 16-tile-wide sheet for a 32-wide one:
 * (e & 0xFC0F) | (e & 0x3F0) << 1 (bit 9 spills into the hflip bit). */
void CopyMapRectRemapTiles16To32(u16 *src, u16 *dst, u8 w, u8 h);
/* Unused. Sets the palette nibble of every entry of a w x h rectangle. */
void SetMapRectPalette(u16 *dst, u8 w, u8 h, u8 pal);
/* Unused. For a w x h block writes dst[j] = (src[2j] & 0xFF) | (src[2j + 1] & 0xFF) << 8, two entries per
 * halfword, dst stride 32 halfwords (hypothesis: converts a text-BG map to 8-bit affine-BG entries). */
void PackMapRectBytes(u16 *src, u16 *dst, u8 w, u8 h);
/* Byte offset of cell (x, y) in a multi-screenblock map: (x + y * 32) * 2, +0x800 per extra screenblock.
 * The x/y > 0xFF tests and the +0x800 for screenSize 3 look wrong for a 64 x 64 map; every caller passes
 * screenSize 0 and small coordinates. */
u16 GetTilemapOffset(u16 x, u16 y, u8 screenSize);
/* Copies a w x h block from cell (sx, sy) of srcMap (row stride srcStride) to cell (dx, dy) of dstMap; both
 * cell offsets come from GetTilemapOffset. Returns nothing: the u32 return type only reproduces the ROM's
 * pop {r1} epilogue. */
u32 CopyMapBlock(u16 *srcMap, u16 sx, u16 sy, u16 srcStride, u16 *dstMap, u16 dx, u16 dy, u8 w, u8 h,
                 u8 mapSize);
/* Crops the w x h block at (sx, sy) of a plain srcW-wide map array and copies it to cell (dx, dy) of dstMap.
 * Returns nothing (u32 for the epilogue, as CopyMapBlock). */
u32 CropMapBlock(u16 *src, u16 sx, u16 sy, u16 srcW, u16 *dstMap, u16 dx, u16 dy, u8 w, u8 h, u8 mapSize);

/* ---- Rectangles in VRAM screenblocks ----
 * For a 64-wide map, columns 0x20-0x3F live in the next screenblock and columns >= 0x40 wrap to the start of
 * the row. The *Vram* fills, SetVramMapTile and DrawVramMapNumber3 have no callers in the ROM. */

/* Fills w / 2 words (tile | tile << 16) of one row of screenblock `screenblock` from (x, y); no wrap. */
void FillScreenblockRow32(u8 tile, u8 screenblock, u8 x, u8 y, u16 w);
/* Fills a w x h rectangle with one tile (16-bit stores) without column wrap; when w > 31 every entry goes
 * 0x800 bytes later (the next screenblock) instead. */
void FillScreenblockRectNoWrap(u8 tile, u8 screenblock, u8 x, u8 y, u8 w, u8 h);
/* Fills a w x h rectangle (w even) with ascending tile numbers, two per 32-bit store, 64-wide wrap. */
void FillScreenblockRectAscending32(u16 tile, u8 screenblock, u8 x, u8 y, u8 w, u8 h);
/* Writes ascending tile numbers (tile, tile + 1, ...) into a w x h rectangle, 64-wide wrap. */
void FillVramMapRectSeq(u16 tile, u8 screenBlock, u8 x, u8 y, u8 w, u8 h);
/* Fills a w x h rectangle with one tile using 32-bit stores (x and w must be even), 64-wide wrap. */
void FillVramMapRect32(u16 tile, u8 screenBlock, u8 x, u8 y, u8 w, u8 h);
/* Fills a w x h rectangle with one map entry (16-bit stores), 64-wide wrap. */
void FillVramMapRect(u16 tile, u8 screenBlock, u8 x, u8 y, u8 w, u8 h);
/* Sets the tile number of cell (x, y), keeping the palette and flip bits: *p = (*p & 0xFC00) | tile. */
void SetVramMapTile(u8 screenBlock, u8 x, u8 y, u8 tile);
/* Draws the 3 low decimal digits of num leftwards from cell (x, y) as entries (digitTile + d) | pal << 12;
 * mode is an enum NumberDrawMode. */
void DrawVramMapNumber3(u8 screenBlock, u16 digitTile, u8 x, u8 y, u16 num, u8 pal, u32 unused, u8 mode);

/* ---- Tiles ---- */

/* Adds `add` to every non-zero byte of buf in place (shifts the colour of opaque 8bpp pixels), then copies
 * size bytes to dst with CpuFastSet. */
void OffsetNonZeroPixelsAndCopy(u8 *buf, u8 *dst, u16 size, u8 add);
/* Copies a 16 x 16-tile (128 x 128 px) sheet into a 32-tile-wide 2D OBJ layout (dst rows 0x400 bytes apart). */
void CopyTileSheetTo2D(const u8 *src, u8 *dst, u16 colors);
/* CopyTileSheetTo2D for the first `rows` tile rows only. */
void CopyTileSheetRowsTo2D(const u8 *src, u8 *dst, u16 colors, u8 rows);
/* Copies a width x height pixel image (row-major tile rows) to tile `tileIndex` of a 32-tile-wide 2D layout. */
void CopyTileRectTo2D(const u8 *src, u8 *dst, u16 tileIndex, u16 width, u16 height, u16 colors);
/* Copies rowCount rows of tilesPerRow tiles; source rows are 0x200 bytes apart, destination rows 0x400. */
void CopyTileRows(const u8 *src, u8 *dst, u16 colors, u8 rowCount, u8 tilesPerRow);
/* Okumura LZSS (N = 4096, F = 18, threshold 2) with a zero-filled ring at gScratchBuffer (0x02030000).
 * srcSize is the compressed size; the loop ends only when it reaches exactly 0. */
void LZSSDecompress(u8 *src, u8 *dst, s32 srcSize);

/* ---- Card art ----
 * Card art is 72 x 80 px, 6bpp (8 pixels per 3 halfwords), 64 colours per card. Callers pass
 * dst = 0x06008000 + page * 0x1680 with page 0 or 1 (double-buffered, BG palettes 0x80-0xBF / 0xC0-0xFF). */

/* Copies the card's palette to colour 0x80 + page * 0x40 and unpacks its art into 90 8bpp tiles at dst, with
 * the palette base added to every pixel. */
void UnpackCardArt8bpp(u16 cardId, u32 dst, u16 page);
/* Public entry: narrows its arguments to u16 and calls UnpackCardArt8bpp. */
void LoadCardArt8bpp(u16 cardId, u32 dst, u16 page);

/* ---- BG registers ---- */

/* One 32-bit write of hofs & 0x1FF | (vofs & 0x1FF) << 16 to BGnHOFS/BGnVOFS (registers, no gMain shadow). */
void SetBgScrollRegs(u32 hofs, u32 vofs, u8 bg);
/* Unused. For bg 2 or 3 sets the reference point BGxX/BGxY = M * (p - c) + c, M being the matrix of an
 * ObjAffine record (see sprite.h); any other bg does nothing. */
void SetBgAffineRefPoint(u8 bg, s32 x, s32 y, s32 cx, s32 cy, struct ObjAffine *affine);

/* Compile-time layout checks (agbcc pads every struct to 4 bytes). */
typedef char bg_h_check_cell[sizeof(struct ImagePackCell) == 0x4 ? 1 : -1];
typedef char bg_h_check_count[sizeof(struct ImagePackCount) == 0x8 ? 1 : -1];
typedef char bg_h_check_cell_tile[(u32)&((struct ImagePackCell *)0)->tile == 0x2 ? 1 : -1];

#endif /* GUARD_BG_H */
