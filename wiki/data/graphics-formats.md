---
title: Graphics formats
type: data
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Graphics formats

Summary (verified): EDS makes no BIOS decompression calls. The only SWIs are CpuFastSet, CpuSet and Div ([[bios-swi-stubs]]). Almost all graphics are stored raw. Only two formats are compressed:
1. LZSS (Okumura-style, with the custom decoder [[lzss-decompress]]). It is used only for the dialogue-scene bitmaps, their sprite sheets and the dialogue-box strip, about 0.7 MiB in total.
2. 6bpp packing for card art ([[card-art]]).

Everything else is one of the raw containers below. Where they sit in the ROM is on [[rom-map]].

## Format inventory

In the V/H column, V means verified and H means hypothesis.

| Format | Where | Count | Loader | V/H |
|---|---|---|---|---|
| LZSS blob `{u32 packedSize; stream}` | `0x0870C640`–`0x087BDAA8`, `0x0874C650`, `0x0874D5B0` | 57 blobs in scene sets (31 bitmaps + 26 OBJ sheets), plus 2 | [[lzss-decompress]] `0x0807A1A8` | V |
| Scene set (Mode 4) | descriptors `0x081976A0` (31 × 0x14) | 31 | `0x08000420`, `0x08000570`, `0x08000708` | V |
| Image pack (palette + tiles + sparse map) | graphics banks A and B, card frames | ~100 found by scan | `sub_08072EB0`, `sub_08072FAC` (8bpp BG) | V |
| Mode-4 bitmap (240×160 8bpp + 256-colour palette) | `0x0871CE50`…, `0x086A12EC`, `0x086B8568`, `0x087C29D4` | 8 | via tables such as `0x08198440` | V |
| Card art (6bpp, 72×80, 64 colours) | `0x082A6500` | 820 | see [[card-art]] | V |
| 1bpp fonts | `0x081C0000`–`0x0822C300` | 8 fonts | see [[font]] | V |
| Raw palettes (0x20 / 0x200) and raw 4bpp/8bpp tile and map blocks (0x800, 0x2000) | graphics banks A and B | many | various (e.g. `LoadSystemFontGfx`, [[video-helpers]]) | V (sizes), H (individual use) |

## LZSS
```
u32 packedSize          // read as two u16 halves, so 2-byte alignment is enough; blobs are 4-aligned in practice
u8  stream[packedSize]  // flag byte, LSB first: 1 = literal byte, 0 = 2-byte back-reference
                        // ref b0,b1: pos = b0 | (b1 & 0xF0) << 4 ; len = (b1 & 0x0F) + 3
                        // 4 KiB ring buffer, first write at 0xFEE, pre-filled with 0 (the ring is 0x02030000)
```
This is Haruhiko Okumura's classic LZSS (N = 4096, F = 18, THRESHOLD = 2). It is not BIOS LZ77, which has a `0x10` header byte and a big-endian 12-bit displacement. The unpacked size is not stored: callers know it (0x4B00, 0x5A00, 0x2000, 0x3C00, 0x1680). Many blobs (17 of 57) decode to size+1 because a trailing literal is padding. Decode with `python3 tools/lzss.py <addr> [out.bin]`.

> [!warning] False positives
> A plausible `u32` size followed by a stream that happens to parse is common in u16 tables. `0x08624CF4` inside the card-number map looked like a valid blob ([[rom-map]] correction). Only trust blobs that a descriptor or a loader's literal pool points at.

## Scene sets (dialogue "bust-up" scenes)
The dialogue screens run in BG mode 4 (8bpp bitmap, 2 pages). Descriptor table at `0x081976A0`, 31 × 0x14:

```c
struct SceneSet {           // 0x14 bytes
    const u32 *bitmapLz;    // LZSS: 240x80 (0x4B00, sets 0,1,3,4) or 240x96 (0x5A00) 8bpp, linear rows
    const u16 *bgPal;       // raw 256 colours (0x200)
    const u16 *objPal;      // raw 256 colours, or NULL (sets 0-4)
    const u32 *objTilesLz;  // LZSS 0x2000: OBJ tiles, copied to 0x06014000 in 16 x 0x200 chunks; NULL for sets 0-4
    const void *anim;       // animation/sprite script in .rodata 2 (e.g. 0x08197954), NULL for sets 0-4
};
```
- The loaders (`0x08000420` and two twins at `0x08000570` and `0x08000708`) decode the bitmap into an EWRAM buffer. `sub_080008A4(page, …, size)` then copies it to page 0 (`0x06000000`) or page 1 (`0x0600A000`). The lower screen area comes from the dialogue-box bitmap (LZSS `0x0874C650`, 240×64 = 0x3C00, table `0x08139F5C`), which is copied to `0x06005A00`/`0x0600FA00`, that is, rows 96–159 of each page.
- Set 0 (240×80) is a duel-arena view, and set 5 (`0x08197704`, 240×96) is Tea in front of a card-art backdrop (both rendered). The rest of 0–4 are probably venue backgrounds and the rest of 5–30 duelist portraits (H).
- V: all 31 descriptors decode (checked by `tools/verify_rom_map.py`), and sets 0, 5 and the dialogue box were rendered as linear 8bpp bitmaps.

## Image pack
A self-describing raw container: palette, tiles, and a sparse tile map. Each count is stored 4 times (four identical u16), which makes packs easy to scan for.

```c
u16 nColors, nColors, nColors, nColors;
u16 palette[nColors];                 // BGR555; bit 15 is often set, and ignored
u16 nTiles,  nTiles,  nTiles,  nTiles;
u8  tiles[nTiles][64];                // 8bpp 8x8 (a 4bpp variant with [32] also exists)
u16 nCells,  nCells,  nCells,  nCells;
struct { u16 pos; u16 tile; } cell[nCells];   // pos = x | (y << 8), on a 32-wide map
```
Loaders `sub_08072EB0` (6 callers) and `sub_08072FAC` (4 callers, `LoadBgImage` on [[video-helpers]]) are identical except for the map buffer (`0x03000C5C` versus `0x0300045C`). Each one:
- copies the palette to `0x05000000 + palIdx*2`;
- copies the tiles to `0x06004000 + tileBase*32`, adding `palIdx` to every non-zero pixel byte (so colour 0 stays transparent);
- writes `mapBuffer[mapBase + (pos & 0x3F) + (pos >> 8)*32] = tile + tileBase/2` for each cell;
- returns `nTiles`.

Examples (all rendered): the 7 card frames `0x08625460`… (32 colours, 144 tiles, a 12×12-tile frame); `0x087D4B24` (254 colours, 583 tiles, 600 cells = a full 30×20 sky BG); `0x087C0CD4` (4bpp, 16 colours, a coin). The 4bpp variant (for example `0x087C056C`, `0x087E795C`…) parses only with 32-byte tiles. Its loader has not been identified (H). `python3 tools/scan_objpack.py [start end]` lists every pack. It has a few false positives inside the font bank.

## Mode-4 bitmaps
Raw 240×160 8bpp (0x9600 bytes), each paired with a raw 256-colour palette. The table at `0x08198440` holds 5 × `{u16 *pal; u8 *bitmap}` for `0x0871CE50`…`0x08742E50`, and one of those is the character-select screen. Other bitmaps are `0x086A12EC` (palette `0x086AA8EC`), `0x086B8568` and `0x087C29D4` (palette `0x087CBFD4`).

## Method
- **SWI absence:** `tools/verify_rom_map.py` checks that the only `svc` + `bx lr` stubs are 0x0C, 0x0B and 0x06.
- **Finding the LZSS decoder:** searched `.text` literal pools for the Okumura constant `0xFEE`. It occurs only at `0x0807A220`, inside `0x0807A1A8`. Its 10 `bl` callers are all scene loaders (`python3 tools/blrefs.py 0x0807A1A8`).
- **Formats:** read from the loader disassembly (`tools/cs.py`), then confirmed by rendering with `tools/render_gfx.py` (`pack`, `bitmap lz:<addr>`, `card`), with Pillow installed in a throwaway container and PNGs kept outside the repo.

Related: [[rom-map]], [[card-art]], [[font]], [[lzss-decompress]], [[text-system]].
