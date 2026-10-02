---
title: Scene sets (dialogue bust-up scenes)
type: data
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Scene sets

Dialogue screens run in BG mode 4 (8bpp bitmap, two pages). Each speaker has a **scene set**: an LZSS bitmap for the upper screen, its BG palette, and for most duelists an OBJ palette, LZSS sprite tiles (eyes, mouths) and an animation list. The lower screen is the shared dialogue box. The formats here are **verified**: `tools/assetfmt/gfx_scenes.py` turns all 31 sets and the box into PNGs and builds them back byte-identical, re-encoding every LZSS stream exactly ([[graphics-formats#The original compressor]]). Sets 0, 5 and 25, their sprites, the box and the header strip were rendered and look right.

## Descriptor table (`0x081976A0`, 31 × 0x14)
```c
struct SceneSet {               /* 0x14 bytes */
    const u32 *bitmapLz;        /* LZSS: 240x80 (0x4B00) for sets 0, 1, 3, 4; 240x96 (0x5A00) for set 2 and 5-30 */
    const u16 *bgPal;           /* 256 colours; the game loads entries 16-255 */
    const u16 *objPal;          /* 256 colours, or NULL (sets 0-4) */
    const u32 *objTilesLz;      /* LZSS 0x2000: 256 4bpp OBJ tiles, or NULL (sets 0-4) */
    const void *const *anim;    /* NULL-terminated animation track list, or NULL (sets 0-4) */
};
```
`GetBustupSet` (`sub_08001C78`, [[code-08001364]]) maps a character ID to a set, with a jump table on the ID.

> [!warning] Contradiction
> [[graphics-formats]] described `anim` as a "script in .rodata 2 (e.g. `0x08197954`)". The table plugin's decoding shows it is a NULL-terminated list of step-array pointers in the scene/list range `0x0819790C`–`0x0819A9D4`. The step arrays themselves live in `.rodata` 1, and [[rom-map]] reserves the name ".rodata 2" for `0x0819DD64`–`0x081A7A0C`. Resolved in favour of the decoded tables; [[graphics-formats]] is updated.

## Set to character
Character IDs follow the [[duelist-table]]. IDs 32–35 and 37 are not in that table (sets 0–4, the scenes without sprites).

| Set | Character | Set | Character | Set | Character |
|---|---|---|---|---|---|
| 0 | ID 34 | 11 | Kaiba (16) | 22 | Strings (13) |
| 1 | ID 35 | 12 | Yami Bakura (19) | 23 | Shadi (18) |
| 2 | ID 37 | 13 | Grandpa (24) | 24 | Marik (15) |
| 3 | ID 32 | 14 | Weevil (8) | 25 | Duel Computer (21) |
| 4 | ID 33 | 15 | Mai (10) | 26 | Simon (22) |
| 5 | Tea (2) | 16 | Rex (6) | 27 | Pegasus (23) |
| 6 | Yugi (1); also the default | 17 | Espa Roba (7) | 28 | Umbra & Lumis (14) |
| 7 | Joey (3) | 18 | Mako (9) | 29 | Lumis (39) |
| 8 | Ryou (5) | 19 | Rare Hunter (11) | 30 | Umbra (38) |
| 9 | Tristan (4) | 20 | Arkana (12) | | |
| 10 | Yami Yugi (20) | 21 | Ishizu (17) | | |

Set 0 is a duel-arena view and set 5 is Tea in front of a card-art backdrop (both rendered). Sets 1–4 are probably other venue backgrounds (hypothesis).

## ROM layout
Each set is one contiguous slot: `LZSS bitmap | pad | bgPal[256] | (objPal[256] | LZSS objTiles | pad)`. The OBJ part exists only for sets 5–30.
- Sets 0–4 occupy `0x0870C640`–`0x0871B650` in the order 2, 0, 3, 4, 1.
- Sets 5–30 occupy `0x0874E324`–`0x087BDAA8` in the order 5, 6, 7, 8, 9, 14, 15, 16, 17, 18, 10, 19, 20, 11, 21, 23, 22, 24, 25, 13, 26, 12, 27, 28, 29, 30.
- Padding after a blob is 0–3 bytes of alignment.

## Loading
- The loaders (`0x08000420`, `0x08000570`, `0x08000708`) decode the bitmap into an EWRAM buffer with [[lzss-decompress]]. `sub_080008A4(page, …, size)` then copies it to page 0 (`0x06000000`) or page 1 (`0x0600A000`).
- **BG palette.** Entries 16–255 come from the set. Entries 0–15 come from the dialogue text palette.
- **OBJ tiles.** The 0x2000-byte sheet is 16 rows of 16 tiles. Each 0x200-byte row goes to `0x06014000 + row * 0x400`, so the sprites use 2D mapping. The OAM templates address a tile as `row << 4 | column`.
- **OBJ palette bank.** Each tile's 16-colour bank comes from the set's animation templates (attr2 bits 12–15). The extracted `sprites.png` shows each tile in that bank, but the build stores only `value & 15`.

## Animation
- `anim` points to a NULL-terminated list of tracks. `sub_08078670` makes one sprite group per track, `sub_080786D0` steps it and `sub_08077EF4` draws it.
- A track is an `AnimStep[]` array `{u8 frames; u8 count; u16 unk2; const OamTemplate *sprites}`, ended by a step with `frames == 0`. Each step shows `count` sprite templates for `frames + 1` frames.
- The step arrays of the scene sets are in `.rodata` 1 at `0x08080B08`–`0x08081260`. Their sprite templates (`OamTemplate`: OAM attributes 0–2 plus an unused halfword) are in the scene/list range.

## Dialogue box (`0x0874C650`–`0x0874E324`)
| Item | Address | Content |
|---|---|---|
| Box | `0x0874C650` | LZSS 240×64 (0x3C00), drawn at rows 96–159 of both Mode-4 pages (`0x06005A00` / `0x0600FA00`) |
| Header strip | `0x0874D5B0` | LZSS 240×24. Only the top 16 rows are copied, to rows 0–15 of the 240×80 scenes. The code reads it through `gUnk_0874D5B0`/`B2`/`B4` |
| Box palette | `0x0874E104` | 256 colours. The 240×80 loader copies entries 0–31. Both images use only indices 16–31 |
| Text palette | `0x0874E304` | 16 colours, loaded to BG palette 0–15 for every scene |

Related tables:
- `0x08139F5C`: 2 pointers to the box blob (`gUnk_08139F5C[box]`), both `0x0874C650`.
- `0x0813ADD4`: the 8-entry step table of the bust-up runner (`sub_08001AE4`/`sub_08001B34`).

## Data facts
- **Size+1 streams.** 16 of the 240×96 bitmaps decode to 0x5A01 bytes, and set 25's sprite sheet decodes to 0x2001. The extra byte is 0x18 for the bitmaps and 0x01 for the sprites. It was part of the compressor's input, not padding ([[graphics-formats#The original compressor]]).
- **Palette bit 15** is set on about half the BG palette entries and never on an OBJ entry. The hardware ignores it.
- **Room for edits.** The stream must fit its original slot. The optimal re-parse is about 1.6% smaller than the original encoder's output: 170–310 bytes per bitmap and 2–16 bytes per sprite sheet.

## Method
- Descriptors and sizes: `tools/verify_rom_map.py`, and the exact round trip in `gfx_scenes.py` (`verify --only gfx/scene` 32/32, 2026-10-02).
- Character mapping: the plugin's table from `sub_08001C78`, checked against the duelist table.
- Tile placement and palette banks: read from the loaders and confirmed by rendering set 25, whose sprites use three banks.
- Notes and edit tests: `build/assetwf/gfx_scenes/NOTES.md`.

Related: [[graphics-formats]], [[lzss-decompress]], [[duelist-table]], [[text-system]], [[assets]].
