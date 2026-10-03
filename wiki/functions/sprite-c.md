---
title: Unit sprite (OAM sprite/affine emitters, sprite animation streams, Random, save signature and checksum)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit `sprite` (`0x08076144`-`0x0807717C`)

Thumb, `old_agbcc -O2` ([[compiler-flags]]). Source: `src/sprite.c`. **28/28 functions in C**, the whole unit (2026-10-02). The animation-frame family matched last: `0x08076A20` at the wave 3 checkpoint, then `0x08076BEC` and `0x08076DAC` together after it (see [Animation-frame family matched](#animation-frame-family-matched-wave-3-2026-10-02)). Before that: 25/28 after workflow wave 1 (2026-10-01: the twins `0x08076448` and `0x08076714`). `unit bytes MATCH`.
Related pages: [[video-helpers]] (`AddSprite`), [[random]], [[collection-c]] (the save code that follows), [[ram-map]], [[frame-sync-update]] (the OAM flush).

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x08076144` | 0x1C | matching | affine matrix `idx`: `PA = PD = scale`, `PB = PC = 0` (writes the 4th halfword of OAM entries `idx*4..idx*4+3`) | `SetOamAffineScale` |
| `0x08076160` | 0x6C | matching | affine matrix `idx` from `scale` (8.8) and `angle` (128 steps per turn, sine table `gSineTable128`, s16[128], cos = `+0x20`): `PA = PD = cos*scale>>8`, `PB = sin*scale>>8`, `PC = -sin*scale>>8` | `SetOamAffineRotScale` |
| `0x080761CC` | 0x24 | matching | affine matrix with shear: `PA = PD = scale`, `PB = shear`, `PC = -shear` | `SetOamAffineShear` |
| `0x080761F0` | 0x6C | matching | append one OAM entry to `gMain.oam[oamCount++]` (max 0x80). `yx = y<<16 \| x`, `shape` = attr0 high byte (bits 14-15) and attr1 size (bits 6-7 shifted up 8), `attr2` = tile/palette | `AddSprite` |
| `0x0807625C` | 0x74 | matching | same with attr0 `\|= 0x400` (semi-transparent) | `AddSpriteAlpha` |
| `0x080762D0` | 0x78 | matching | same with attr0 `\|= 0x2000` (256 colours) and `attr2 << 1` (8bpp tile index) | `AddSprite8bpp` |
| `0x08076348` | 0x88 | matching | as above plus an extra u16 OR-ed into attr1 (flip / affine index) | `AddSprite8bppEx` |
| `0x080763D0` | 0x78 | matching | 8bpp and semi-transparent (`0x2400`), `attr2 << 1` | `AddSprite8bppAlpha` |
| `0x08076448` | 0x1DC | **matching** (wave 1, 2026-10-01) | rotating/scaling sprite, 8bpp + alpha (`attr0 \|= 0x2700`, double-size box): `(x, y)` is the sprite centre, a 12-way `switch` on the shape/size code subtracts half the sprite size; attr1 gets the next affine index (`affineCount << 9`), `attr2 << 1`; calls `SetOamAffineRotScale(affineCount, sa >> 16, sa & 0xFFFF)`; bumps `oamCount` and `affineCount` (max 0x20) | `AddAffineSprite8bppAlpha` |
| `0x08076624` | 0x80 | matching | `AddSprite` with an extra u16 OR-ed into attr1 | `AddSpriteEx` |
| `0x080766A4` | 0x70 | matching | `AddSprite` taking `x`, `y` (signed, low 8 bits used), `shape`, `attr2` separately | `AddSpriteXY` |
| `0x08076714` | 0x1DC | **matching** (wave 1, 2026-10-01) | as `0x08076448` but 4bpp: `attr0 \|= 0x300`, attr2 not doubled | `AddAffineSprite` |
| `0x080768F0` | 0x6C | matching | two DMA3 fills of zero: OBJ palette `0x05000200` (256 halfwords) and OBJ VRAM `0x06010000` (0x40 halfwords) | `ClearObjPaletteAndFirstTiles` |
| `0x0807695C` | 0x80 | matching | load a sprite animation stream `src` into a `SprAnim` state: `DISPCNT \|= 0x40` (1D OBJ mapping), palette 15 (`0x050003E0`, 0x20 bytes), then the tile blocks (`u16 n; n*32 bytes`) into OBJ VRAM from tile 1 (`0x06010020`), then reads the frame count and calls `SprAnimRewind` | `SprAnimLoad` |
| `0x080769DC` | 0x44 | matching | rewind: skip the frame table and all tile blocks, read the frame count into `+8`, `+0xA = 0` | `SprAnimRewind` |
| `0x08076A20` | 0x1CC | **matching** (wave 3, 2026-10-02; FAKEMATCH) | draw the current animation frame at `(x, y)`: per piece read `{u16 fmt; s16 dx; s16 dy}`, write OAM entry `i` (attr0 y = `dy + y`, bit 5 cleared; attr1 x = `(dx + x) & 0x1FF`; attr2 tile = `(fmt * len + 1) & 0x3FF`, palette 15, priority 0; attr1 size bits from the table entry `base[0x22 + fmt*4]` = 0/0x4000/0x8000/0xC000); when `flag != 0` advance to the next frame (`+4`, `+0xA++`); if `+0xA >= +8` rewind instead | `SprAnimDrawAt` (hypothesis) |
| `0x08076BEC` | 0x1C0 | **matching** (wave 3, 2026-10-02; FAKEMATCH) | same but pieces are 6 bytes with only `fmt` used (x taken from the parameter, low 9 bits; attr0 shape bits cleared) | `SprAnimDraw` (hypothesis) |
| `0x08076DAC` | 0x1F0 | **matching** (wave 3, 2026-10-02; FAKEMATCH) | same family with horizontal/vertical flip flags from the fourth parameter (`and 1`, `<<4`, ...) | `SprAnimDrawFlip` (hypothesis) |
| `0x08076F9C` | 0x28 | matching | `Random` (see [[random]]) | `Random` |
| `0x08076FC4` | 0x34 | matching | byte compare of `n` bytes, returns 1 when they differ | `MemDiffers` |
| `0x08076FF8` | 0x24 | matching | 1 when the 8 bytes at `0x02013D86` equal `gSaveSignature` (save signature) | `IsSaveSignatureValid` |
| `0x0807701C` | 0x18 | matching | `strcpy(0x02013D86, gSaveSignature)` | `WriteSaveSignature` |
| `0x08077034` | 0x4C | matching | sum of the first `0x10B6` halfwords of the save mirror (`0x02011C20`), negated, equals the halfword at `+0x216E` | `IsSaveChecksumValid` |
| `0x08077080` | 0x3C | matching | store that negated sum at `+0x216E` | `UpdateSaveChecksum` |
| `0x080770BC` | 0x20 | matching | `save+4 = v & 0x7F`, and `\| 0x80` when `v == 0` (mode/flag byte) | `SetSaveMode` |
| `0x080770DC` | 0xC | matching | `SetSaveMode(1)` | |
| `0x080770E8` | 0x2C | matching | reset the save mirror: clear `0x2170` bytes, `SetSeEnabled(1)`, `SetBgmEnabled(1)` (sound options), `SetSaveMode(1)`, `WriteSaveSignature` | `ResetSaveData` |
| `0x08077114` | 0x68 | matching | debug "Get all card" body: for every card id 1..0x334 whose key `gCardIdToNumber[id & 0x7FF] - 0x780` is above 0x4F, call `AddCardToTrunk(id)` (add a copy) until `count + n1 + n2 + n3 > 2`, i.e. 3 copies each | `DebugGetAllCards` |

## Structures and globals (inferred)

- `gMain` (`0x03000040`): `+0 u32 rngState`; `+0x4430` OAM shadow buffer `struct OamEntry[128]` (8 bytes: `attr0, attr1, attr2, affine`; the 4th halfword of entries `4k..4k+3` is affine matrix `k`'s `PA, PB, PC, PD`, so the buffer at `0x03004470` is used both ways); `+0x4830 u8 oamCount`; `+0x4831 u8 affineCount` (max 0x20).
- Sprite animation state `SprAnim` (hypothesis, 0x10 bytes): `+0 u8* base` (stream start), `+4 u8* cur` (read pointer inside the current frame), `+8 u16` (frame count), `+0xA u16` (current frame index), `+0xC u16` (tile block count), `+0xE u16` (piece count of the current frame).
- Sprite animation stream (hypothesis): `+0` palette (0x20 bytes, loaded to OBJ palette 15), `+0x20 u16 nBlocks`, `+0x22 nBlocks * {u16 shapeCode (0/0x4000/0x8000/0xC000, attr1 size bits); u16 pad}`, then `nBlocks * {u16 tiles; tiles * 32 bytes}`, then `u16 frameCount`, then frames `{u16 pieces; pieces * {u16 fmt; s16 dx; s16 dy}}`.
- Save mirror (`0x02011C20`): `+4` byte `mode:7 | jpFont:1<<7` (bit 7 selects the Shift-JIS text renderers, see [[text-canvas-c]]), `+0x216C..` header/pad, `+0x216E u16` checksum, size `0x2170`; the signature lives outside at `0x02013D86` (8 bytes copied from `gSaveSignature`).

## Matching tricks learned

- **Rotate idiom:** `(x << 16) | (x >> 16)` compiles to `rors` in `old_agbcc`; the ROM has shifts. `u32 t = x << 16; x >>= 16; x |= t;` gives `lsl; lsr; orr` (Random).
- **Affine matrices / OAM base:** `struct OamEntry *o = gMain_oamBuffer; o += (u32)idx << 2;` produces `ldr base; lsr #11; adds` like the ROM (the `o = &arr[idx*4]` forms load the literal after the shift). Negating a `u16` param needs `s16 t = shear; o[2].affine = -t;` to keep the `lsl 16; asr 16` sign extension.
- **`SetOamAffineScale` scheduling:** the ROM interleaves `strh scale,[6]` before `movs r0,#0`; the struct form `o[0..3].affine =` always hoists the constant. Indexing the OAM buffer as a flat `u16 *p; p += (u32)idx << 4; p[3]=scale; p[7]=0; p[11]=0; p[15]=scale;` gives the ROM schedule.
- **Register allocation via a same-width temp (found with the permuter):** for `AddSpriteXY`, `x` landed in `ip` and `attr2` in `r6`, the reverse of the ROM. Introducing a local `s16 nv;` (declared after `struct Main *m;`, before `u32 a0`), then `nv = x;` and `e->attr1 = (nv & 0x1FF) | a1;`, moves `x` to `r6` and `attr2` to `ip` and matches. A `u16` temp does not; the signed temp does.
- **Multiplications then shifts:** write `s *= scale; c *= scale; ns *= scale; s >>= 8; ...` as separate statements to get three `muls` then three `asrs`.
- **OAM emitters:** copy `x`/`y` from `yx` into locals, `u16 a1 = (shape << 8) & ~0x1FF;` (keeps the 0xFFFFFE00 literal), pointer `struct Main *m; ... m = &gMain; cnt = &m->oamCount;` assigned after the locals, and index with `u32 off = *cnt << 3; arr = m->oam; e = (u8*)arr + off;` (shift first, then `m + 0x4430`, then add). `x & 0x1FF` written as `((u32)x << 23) >> 23` gives `lsl; lsr` only when `x` is a parameter (`AddSpriteXY`).
- **DMA fill:** `vu16 zero; vu32 *dma = (vu32*)0x040000D4; dma[0..2]; dma[2]; while (dma[2] & 0x80000000);` written twice in one block sequence (the second time re-assigning `zero = 0; dma = ...`) matches.
- **Loops that keep `n` in a register:** `if (i < n) { do { ... } while (i < a->count); }` with `u16 i` (the counter reloads `a->count` from memory each iteration, so `n` is not live across the call in `SprAnimLoad`).
- **Checksums:** to get the pointer copy on the right register, initialise in this order: `sum = 0; p = &save; i = 0; base = (u8*)p; for (; i <= 0x10B5; p++, i++) sum += *p;` (pointer bump before the counter bump), and `(u16)(~sum + 1)` rather than `-sum`.
- **`SetTextMode`:** `u8 t = v & 0x7F;` (not a wider temp) to get `adds r2, r0, #0; ands r2, r1`.
- **`DebugGetAllCards`:** assign `s = (u8*)&gSaveData;` inside the `if` (after computing the key and `next = id + 1`), then `r = (struct CardRec*)(s + id * 4)`; the key lookup is `*(const u16*)((const u8*)tbl + ((id & 0x7FF) << 1))` (see [[collection-c]]).

## Open items

- Historical (both matched in wave 1, see below): `0x08076448`, `0x08076714`: identical code, the ROM keeps `shape` in r4 / `y` in r6 / `a0` in r7 with scale and angle in r9/r8; ours in r5 / r4 / r6 and r8/r9. Permuting declaration orders did not help (198 differing lines before/after); permuter on `0x08076714` got no lower than score 35 (its best moved some `m->` uses to the global directly).
- Historical (all three matched in wave 3, see below): `0x08076A20`, `0x08076BEC`, `0x08076DAC`: same family; the `a`/`cur` pointers land in different registers (ours uses `ip` where the ROM uses `r7`), plus the `0x4433` literal hoisting. Permuter on `0x08076A20` stayed at score ~5500.
- `0x080766A4`: solved with the `s16 nv` temp trick (see above).
- `0x080769DC` is matching: retain the stream pointer in `r0`, the iterator in `r4`, and a separate increment temporary in `r0`; explicitly narrow that increment to `u16`, preserving the original wrap behavior. A word-sized count local loaded from the stream halfword avoids extra narrowing. An empty count input barrier after the pointer increment and an initialized-zero iterator read/write barrier retain the target's instruction order and unsigned initial comparison. These compiler constraints are documented `FAKEMATCH`; each was tested for removal, and the redundant pointer input on the first barrier was removed. Verified with `tools/check.py`: 28/28 functions match and unit bytes MATCH (0x1038 bytes).

Related: [[video-helpers]], [[random]], [[collection-c]], [[text-canvas-c]], [[decomp-workflow]].

## 2026-10-01 RNG result declaration

`Random` now returns `int` with an explicit `(u16)` result cast. Its original shifts still produce a zero-extended 15-bit value, 0..32767; the complete unit is byte-identical. This agrees with the word-valued signed-modulo consumer in [[summon-checks-c]]. Existing halfword callers receive the same value. Evidence: `build/bigguns-lead2/Random/solo-menu-rng-word/` and the full-ROM check in `build/lead-pass25/`. No coverage is added for this declaration repair.

## Affine-sprite twins (2026-10-01)
Historical (superseded by the wave 1 match below): `AddAffineSprite8bppAlpha` and `AddAffineSprite` drop from 212 to 22 and 14 diff lines. The first `oamCount == 0x80` test reads `gMain` directly, and `m = &gMain` is assigned after it. **Remaining:** the ROM copies `m` before the first compare (the build copies it after the branch). It also computes the OAM entry as `count * 8 + (m + oam)` after the count load. `AddAffineSprite8bppAlpha` also swaps scale/angle in r8/r9. An explicit `oam` base variable fixes the association but moves the base computation earlier. Both are queued for the permuter.

## Affine sprite twins matched (wave 1, 2026-10-01)

`AddAffineSprite` (`AddAffineSprite`) and `AddAffineSprite8bppAlpha` (`AddAffineSprite8bppAlpha`), both 0x1DC with start score 14, match in ordinary C. Working notes: `build/wf/AddAffineSprite/NOTES.md`, `build/wf/AddAffineSprite8bppAlpha/NOTES.md`.

- Drop the local `struct Main *m` entirely and write every access as `gMain.field`. GCSE (PRE) then creates the long-lived base-address pseudo plus the copy in the first block (the ROM's copy **before** the first compare), and `&gMain.oam[gMain.oamCount]` gives the right `count*8 + (gMain+0x4430)` association. Score 14 to 8 for both.
- Scale/angle in r8/r9. In `AddAffineSprite` they tie in global-alloc priority (2 refs / ~174 insns, truncated to int), so the **lower pseudo number** wins r8: `u16 angle, scale; scale = sa >> 16; angle = sa;` (angle declared first, assigned second) puts angle in r8.
- In `AddAffineSprite8bppAlpha` the ROM wants scale in r8; the extra `attr2 << 1` makes the live lengths 176/175, so angle won. `u32 scale = sa >> 16;` (or `int`) instead of u16 gives the matching allocation.
- Failed: m-pointer variants (assigned before/after the compare, a cnt pointer at the top, combined `||`, nested if: 14-212); off/arr temporaries for the entry address (16-173); in `AddAffineSprite8bppAlpha`, u16 scale in either declaration order (8).
- These twins were also listed as a failed fixed-register family in [[matching-tricks]]; the match needs no pins at all.

## Animation-frame family matched (wave 3, 2026-10-02)

`SprAnimDrawFrame` (0x1CC) matched at the wave 3 checkpoint (commit `f7c9206`, 2026-10-02 01:05). Its twins `SprAnimDrawFrameAt` (0x1C0) and `SprAnimDrawFrameAtFlip` (0x1F0, the hflip variant taking `u32 yx`) followed with the same fix in commit `33809ed` (2026-10-02 08:52), after the 1910-function checkpoint. Working notes: `build/wf/SprAnimDrawFrameAtFlip/NOTES.md`. `SprAnimDrawFrame` has no NOTES.md; its `prep.json` note and the commit diff record the steps. `SprAnimDrawFrameAt` had no wf work directory of its own.

- **Size switch as an inline that takes the frame pointer.** The attr1 size bits come from `base[0x22 + fmt*4]` (0/0x4000/0x8000/0xC000). An open-coded switch let loop.c hoist the `0x4433` OAM-offset literals and let CSE share them with the hflip address (scores 218, 201 and 593 for the three functions). An inline `SetSize(int i, u16 sz)` stopped the hoisting, because integrate.c folds the offsets into the adds (218 -> 28 for `0x08076A20`, 201 -> 50 for `0x08076BEC`, 593 -> 28 for `0x08076DAC`).
- **The last difference was the switch index copy**: the ROM has `ldrh r0; adds r1, r0, #0` and compares on r1. What matched is the inline `SetSize(int i, struct SprAnim *a, u16 fmt)`, which does the table read itself: `t = *(u16 *)p; asm("" : "+r"(t)); sz = t; asm("" : "+r"(sz)); switch (sz) ...` (FAKEMATCH, the same form in all three functions). The two empty constraints keep `t` and `sz` as separate pseudos, so the copy survives and every compare uses it.
- `0x08076BEC`'s remaining "shared `0x3F` register (r2 vs r4)" difference disappeared with the same change. `0x08076DAC` also needed what was already in its parked draft: the `0x08076BEC`-style loop (`next = i + 1` before the inner do/while), the OAM bitfield struct with `matrixNum:3/hflip:1/vflip:1`, and the `u16` bitfield store for x.

