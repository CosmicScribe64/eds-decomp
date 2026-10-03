---
title: Video and memory helpers (cluster)
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-09-29
---
# Video and memory helpers

These are small, heavily used system helpers. All Thumb, all nonmatching (not attempted). The behaviour below was verified by disassembly, and the names are proposals.

| Address | Size | Proposed name | Unit | Behaviour | Call sites |
|---|---|---|---|---|---|
| `0x08075278` | 0x1C | `MemClear16(dst, size)` | `code_080750E0` | stores `(size+1)/2` zero halfwords | many |
| `0x08075294` | 0x1A | `MemCopy16(dst, src, size)` | `code_080750E0` | copies `(size+1)/2` halfwords (VRAM/palette-safe) | 156 |
| `0x080752B0` | 0x1E | `CopyDoubleWords(dst, src, size)` | `code_080750E0` | `ldm/stm {r0,r1}` loop over `(size+7)/8` 8-byte blocks. Used for the BG map and OAM flushes. | |
| `0x080753CC` | 0x14 | `StrLen` | `code_080750E0` | byte string length | |
| `0x080753E0` | 0x14 | `StrLen2` (hypothesis: 2-byte text) | `code_080750E0` | counts 2-byte units until a 0 first byte | |
| `0x08075630` | 0x56 | `LoadSystemFontGfx` | `code_080750E0` | BG palette 0 and OBJ palette 0 ← `0x0822C300` (16 colors, color 0 forced to 0). BG tiles `0x06004000` and OBJ tiles `0x06010000` ← `0x0822C320` (0x200 bytes = 16 tiles). | |
| `0x08075744` | 0x28 | `ResetBgHofs` | `code_080750E0` | zeroes `gMain.bgHofs[0..3]` and `BG0..3HOFS` | |
| `0x08075778` | 0x28 | `ResetBgVofs` | `code_080750E0` | zeroes `gMain.bgVofs[0..3]` and `BG0..3VOFS` | |
| `0x080757AC` | 0x0E | `ResetBgScroll` | `code_080750E0` | `ResetBgHofs(); ResetBgVofs();` | |
| `0x08073498` | 0x34 | `ClearBgMapBuffers` | `code_08072FAC` | clears `gMain.bgMapBuffer` (8×0x800) and `0x02010014` (0x1C00), and sets `*(u16*)0x02010010 = 0` | |
| `0x08073574` | 0x54 | `ResetVideo` | `code_08072FAC` | `ClearBgMapBuffers`, `LoadSystemFontGfx`, `SetTextArea(0, 0x27E)`. Resets the BG2/BG3 affine registers to identity (`PA=PD=0x100`, others 0), `BG0CNT = 4`. | 18 |
| `0x08072FAC` | 0xF4 | `LoadBgImage(u16 mapBase, u16 palIdx, u16 tileBase, const void *img)` | `code_08072FAC` | copies the palette to `0x05000000 + palIdx*2`, 8bpp tiles to `0x06004000 + tileBase*32` (nonzero pixel bytes get `palIdx` added), and map entries into `gMain.bgMapBuffer` (value = tile + `tileBase/2`). Returns the tile count. | 4 |
| `0x080761F0` | 0x58 | `AddSprite(u32 yx, u16 shapeSize, u16 attr2)` | `code_08076144` | appends one OAM entry to `gMain.oamBuffer[gMain.oamCount++]` (max 128). `yx`: low 16 = x, high 16 = y. `shapeSize` high byte → attr0 high byte; `(shapeSize << 8) & 0xFE00` → attr1 high bits. | 139 |
| `0x08074B08` | 0x28 | `TextInit(a, b)` (hypothesis) | `code_080740BC` | `*(u8*)0x02010000 = a`, `*(u8*)0x02010001 = b`, `*(u8*)0x02010004 = 0`, clears 64 KiB at `0x02000000` | |
| `0x0807501C` | 0x32 | `DrawText(…)` (hypothesis) | `code_080740BC` | picks between `0x08074E60` and `0x08074F50` by `gSaveData[4] & 0x80` | |

Related: [[frame-sync-update]], [[license-sequence]], [[ram-map]].
