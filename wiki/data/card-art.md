---
title: Card Art
type: data
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Card Art

| Field | Value |
|---|---|
| Pixels | `0x082A6500` + id × 0x10E0 (`ROM+0x2A6500`), 821 slots, ends `0x08608360` |
| Palettes | `0x08608360` + id × 0x80, 821 slots, ends `0x08621DE0` |
| Index | internal card ID 1..820. Slot 0 is all zero in both tables |
| Image | **72 × 80 px** = 9 × 10 tiles of 8×8, row-major |
| Pixel format | **6 bits per pixel**, packed as a little-endian bit stream, LSB first (8 px in 6 bytes). Values 0–63 |
| Palette | 64 × BGR555 (0x80 bytes) per card |
| Compression | none (the 6bpp packing *is* the size reduction, from 0x1680 bytes of 8bpp down to 0x10E0) |
| Total | 3.46 MiB of pixels + 0.1 MiB of palettes = 45% of the ROM |

> [!warning] Page history
> The card-tables pass wrote this page first. The ROM-map pass overwrote it by accident at 20:46 on 2026-09-29, because both passes wrote `card-art.md` at the same time. Anything the first version had that this one lacks should be merged back. What remains is consistent with [[card-table]] and [[card-data-functions]].

**Verified 2026-09-29.** Slots 1, 2 and 100 render as 7 Colored Fish, 7 Completed (three "7" magic cards), and Castle of Dark Illusions (the 闇 crest). Those are exactly name slots 1, 2 and 100 of the [[card-name-table]] (code base `0x0822C720` = slot 0). So **art, palette, name and description all share the alphabetical internal card ID**.

## Loaders
Exactly four functions load the art base `0x082A6500` (and the palette base `0x08608360`) from their literal pools. They are four copies of the same unpacker, each writing to a different destination:

| Function | Art literal | Callers (`bl`) | Notes |
|---|---|---|---|
| `BattleScene_LoadCardArt` | `0x0805E044` | 2 | Copies the palette to PRAM and the tiles to VRAM. See [[card-data-functions]] |
| `LoadCardPicture` | `0x08061E48` | 6 | |
| `DrawCardPortrait` | `0x08072E84` | 2 | draws into the card-frame view after the frame pack (`0x08006CF2`) |
| `UnpackCardArt8bpp` | `0x0807AFF0` | 1, the wrapper `LoadCardArt8bpp` (13 callers in duel/deck code: `0x08067290`, `0x08067AEC`, `0x0806BB82`, …) | walked through below |

### `UnpackCardArt8bpp(u16 id, u16 *dst, u8 slot)`
1. Copies the palette with `MemCopy16(0x05000000 + (0x80 + 0x40·slot)·2, 0x08608360 + id·0x80, 0x80)`, so a card uses BG palette entries **128–191** (slot 0) or **192–255** (slot 1).
2. Computes `src = 0x082A6500 + id·0x10E0`, written in the code as `id·((17·8 − 1)·32)`.
3. Loops 720 times. Each pass reads 3 × u16 and writes 4 × u16, which unpacks 8 six-bit pixels into 8 bytes of 8bpp tile data:
   ```c
   // a,b,c = next three u16s
   out[0] = (a & 0x3F)              | ((a & 0xFC0) << 2);
   out[1] = (a >> 12) | ((b & 3) << 4) | ((b & 0xFC) << 6);
   out[2] = ((b >> 8) & 0x3F) | (((b >> 14) | ((c & 0xF) << 2)) << 8);
   out[3] = ((c >> 4) & 0x3F) | ((c >> 10) << 8);
   ```
   Put another way, pixel *i* of a 6-byte group is `(group48 >> 6i) & 0x3F`, with the group read little-endian.
4. Adds the palette base to every pixel: `px = (px & 0x3F3F) + ((0x80 + 0x40·slot) * 0x0101)` over 0xB40 halfwords (literals `0x0B3F` and `0x3F3F` at `0x0807AFF4`).

The result is 90 8bpp tiles (0x1680 bytes) in the caller's buffer. They are shown as a 9×10-tile block.

## Method
- The address arithmetic, literal pool (`0x08608360`, `0x082A6500` at `0x0807AFEC`) and loop counts were read from the disassembly of `0x0807AEF0`–`0x0807AFFA` (`tools/dr python3 tools/cs.py t 0x0807AEF0 0x110`).
- `821 × 0x10E0` from `0x082A6500` ends exactly at the palette table, and `821 × 0x80` ends exactly at the card data table `0x08621DE0` ([[card-table]]). Checked by `tools/verify_rom_map.py`.
- Rendered with `tools/render_gfx.py card <id> <out.png>` (Pillow in a throwaway container, output outside the repo). A column-major tile order and a 10×9 layout were also tried and give scrambled images. Only row-major 9 wide is coherent.

Related: [[graphics-formats]], [[rom-map]], [[card-name-table]], [[card-table]], [[card-id-map]].
