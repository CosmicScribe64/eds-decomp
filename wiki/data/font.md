---
title: Fonts
type: data
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Fonts

The fonts sit in their own bank at `0x081C0000`–`0x0822C300`, starting at a round address after 80 KiB of zero padding. There are eight **1bpp** fonts: three Shift-JIS kanji fonts left over from the Japanese engine, and five 256-glyph Latin fonts in **Windows-1252** order. Glyphs are drawn at run time by the renderers listed on [[text-system]].

| Address | End | Font | Glyphs | Bytes/glyph | Row format | V/H |
|---|---|---|---|---|---|---|
| `0x081C0000` | `0x081D0200` | SJIS kanji 8×8 | 8256 | 8 | 1 byte per row, MSB = left | V |
| `0x081D0200` | `0x081F8700` | SJIS kanji 10×10 | 8256 | 20 | 10 × u16, stored **big-endian** (MSB of the first byte = left) | V |
| `0x081F8700` | `0x08228D00` | SJIS kanji 12×12 | 8256 | 24 | 12 × u16 big-endian | V |
| `0x08228D00` | `0x08229500` | CP1252 8×8 | 256 | 8 | 1 byte per row | V |
| `0x08229500` | `0x08229F00` | CP1252 8×10 | 256 | 10 | 1 byte per row | V |
| `0x08229F00` | `0x0822AB00` | CP1252 8×12 | 256 | 12 | 1 byte per row | V |
| `0x0822AB00` | `0x0822BB00` | CP1252 8×16 | 256 | 16 | 1 byte per row | V |
| `0x0822BB00` | `0x0822C300` | CP1252 8×8 **bold** (7-px glyphs) | 256 | 8 | 1 byte per row | V |

Right after the fonts is a 16-colour system palette at `0x0822C300` (index 0 = `0xFE3F` key colour; 1–15 = blue, red, magenta, green, cyan, yellow, white, black, dark blue, dark red, dark magenta, dark green, dark cyan, olive, grey) and 32 4bpp system tiles at `0x0822C320` (2 blank, then level star, cursor, gems). `LoadSystemFontGfx` (`0x08075630`, [[video-helpers]]) loads both. See [[rom-map]].

## Glyph indexing
- **CP1252 fonts:** the glyph index is the byte value. 0x00–0x1F are blank. 0x20–0x7E are ASCII. 0x80–0xFF follow Windows-1252: `0x80` = €, `0xC0` = À, `0xC7` = Ç, `0xE0` = à, and so on. The 8×8 font has 207 non-blank glyphs, the 8×12 has 215, and the bold has 222. The game text never uses 0x80 and above (see [[text-system]]), so the accented glyphs are unused in this USA build.
- **SJIS fonts:** the glyph index is `row × 192 + (trail − 0x40)`, where `row = lead − 0x80` for lead bytes 0x81–0x9F and `lead − 0xC0` for 0xE0–0xEA. That gives 43 rows × 192 = 8256 glyphs. Row 0 (indices 0–191) is blank. Index 192 is SJIS `0x8140` (ideographic space) and 193 is `、`. `SjisToGlyphIndex` implements this.
- **ASCII to full-width:** `AsciiToFullwidthSjis` uses the table at `0x081A76A0` (95 × u16) to map 0x20–0x7E to SJIS full-width forms (`'A'` becomes `0x8260`).

## Renderers (code)
- `TextDrawLatinGlyph`: CP1252. It picks the font by size (8/10/12/16), reads two rows per `ldrh`, and plots with `TextPlotRow8` (8-px row).
- `TextDrawSjisGlyph`: SJIS. It picks the font by size (8/10/12). The 10 and 12 px fonts are drawn with `TextPlotRow16` (16-px row, byte-swapped first).
- `RenderHalfWidthGlyph` / `RenderFullWidthGlyph`: 8×8 ASCII and 8×8 SJIS into tiles.
- `OverlayBoldGlyphTile`: expands the bold 8×8 font into 4bpp or 8bpp tiles (literal `0x0822BB00` at `0x0807A0AC`).

## Method
- **Sizes:** each font's byte size divided by 256 (Latin) or 8256 (SJIS) gives an integer glyph size. For every Latin font, the first non-blank glyph is 0x21 `!` (checked in `tools/verify_rom_map.py`).
- **Rendering:** `tools/render_gfx.py font1bpp <addr> <h> <out>` draws a 16×16 sheet. Glyph `あ` (index 480) and `日` (index 3834) were rendered from all three SJIS fonts and match. The CP1252 layout was checked visually on the 8×12 sheet (`€`, `¡`…`ÿ`).
- **Selection code:** the switch on size 8/10/12/16 and the literal pools with all font addresses are at `0x08074C9A`–`0x08074E1C`.

Related: [[text-system]], [[graphics-formats]], [[rom-map]].
