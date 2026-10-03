---
title: Unit text_render (animation scripts, fade, text-to-tile glyph renderers, number printing)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit `text_render` (`0x080784E4`-`0x0807960C`)

Thumb, `old_agbcc -O2` ([[compiler-flags]]). Source: `src/text_render.c`. **25/25 functions in C** after workflow waves 2-3 (2026-10-01/02: `0x080788AC`, `0x08078CC8`, `0x080794E0` in wave 2, `0x08079068` in wave 3); none stay `INCLUDE_ASM`. Before wave 2: 21/25. The unit as a whole matches (`unit bytes MATCH`).
Neighbours: [[collection-c]] (sound API / OAM helpers before it), [[gfx-util-c]] (tilemap/palette library after it), [[sprite-c]] (sprite emitters).

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x080784E4` | 0x50 | matching | for each 8-byte entry of a sprite list (`+4` items, `+0xC` count) call `OamListAddTemplate(entry, a, c, d, e)` | `SpriteListUpdate` |
| `0x08078534` | 0x13C | matching | for every animation state of a block (0x14 bytes each, count u16 at `+0x190`) whose `active` is not `-1`, call the 12-argument `OamListAddSpriteGroup`; when `mode` is 1/2/4/8 also store `g`,`h` into `+8/+0xA` first | `AnimBlockDraw` (hypothesis) |
| `0x08078670` | 0x60 | matching | build an animation-state array from a null-terminated list of script pointers; state i: `+0 seq`, `+4 seq->data`, `+C seq->unk1`, `+D idx=0`, `+E active=1`, `+F timer=seq->frames`, `+0x10 prio=0x13-i`; stores the count at `+0x190`, returns it | `AnimBlockInit` |
| `0x080786D0` | 0x4C | matching C | tick one animation state: decrement timer, on wrap advance `idx`; `frames == 0` terminates (idx = 0, active = 0); reload timer/data/unkC from the new step | `AnimStateTick` |
| `0x0807871C` | 0x2C | matching | tick every state of a block | `AnimBlockTick` |
| `0x08078748` | 0xAC | matching | run `CpuSet` `n` times, `(rows << 4)` (mode 0x10) or `(rows << 5)` (mode 0x100) units, stepping src by 0x200 and dst by 0x400 (copy tile rows from a sheet 32 tiles wide to 64 tiles wide) | `CopyTileRows` |
| `0x080787F4` | 0x48 | matching | start a screen fade: `f->kind`, `level = step < 0 ? 0x1000 : 0`, `step`, `state = 1`; `BLDY = level >> 8`; `BLDCNT = kind == 0 ? 0xFF : 0xBF` | `FadeStart` |
| `0x0807883C` | 0x64 | matching | fade tick: `level += step`, clamp at 0x1000 (state 2, `SetBldY(0x10)`) or wrap below 0 (state 3, `SetBldY(0)`), return 1 when finished, else `SetBldY(level >> 8)` (sets BLDY) | `FadeTick` |
| `0x080788A0` | 0xC | matching | `*p &= ~1` | `ClearBit0` |
| `0x080788AC` | 0x39C | **matching** (wave 2, 2026-10-01) | render one 8x8 glyph (font `0x0822BB00`, 8 bytes per glyph) into 8 4bpp words with an outline / drop-shadow: each set glyph bit writes the base colour `a` nibble into the word and marks `buf[row+1]`; `buf[row]` bits write the outline colour `b` nibble; `ch > 0x9F` remaps via `-0x20` and, if bit 0 of `*flags` is clear, `+0x40` | `RenderGlyphOutlined` (hypothesis) |
| `0x08078C48` | 0x60 | matching | parse an optionally `-`-prefixed decimal number at `*pp` up to `,` `)` `-`; advances `*pp` one past the terminator; returns s16 | `ParseSignedDecimal` (hypothesis) |
| `0x08078CA8` | 0x20 | matching | init a 0x20-byte object: `+4 = a`, clear bit 0 of `+0`, `+0x10 = +0x1C = 2`, zero `+8 +0xC +0xD +0x1D` | |
| `0x08078CC8` | 0xFC | **matching** (wave 2, 2026-10-01; FAKEMATCH) | composite a glyph buffer with one 4-bit pixel per byte (EWRAM `0x02000000`, width byte at `0x02010000`) over a solid 4bpp colour into tile data, 16 halfwords per tile: 3 rows of `w` tiles starting at tile row `x` (`dst += x*16*w` halfwords, source tile `w*x + i` for `i < 3w`). Same inner loop as `TextCanvasToTiles` ([[main-c]]) | `BlendGlyphBuffer` (hypothesis) |
| `0x08078DC4` | 0x68 | matching | draw a two-byte character `(p[0] << 8) \| p[1]` twice with `TextDrawGlyph`: shadow at `(x+1, y+1)` and main at `(x, y)` | `DrawCharShadow` |
| `0x08078E2C` | 0x54 | matching | parse a decimal number written backwards from the terminator (`)` or `,`), sets `*pp` past it; returns u16 | `ParseDecimal` (hypothesis) |
| `0x08078E80` | 0x54 | matching | write one map entry `dst[x + y*32] = c \| base \| pal << 12`; for `c > 0x9F` subtract 0x20 and, if bit 0 of `*flags` is clear, add 0x40 (half-width katakana remap) | `PutMapChar` |
| `0x08078ED4` | 0x100 | matching | expand one 8x8 ASCII glyph (font `0x08228D00`, 8 bytes/glyph) to 16 tile halfwords via `ExpandGlyphNibble(nibble, a, b)`; `mode != 0` ORs into existing data | `RenderAsciiGlyph` |
| `0x08078FD4` | 0x94 | matching | same for a Shift-JIS glyph (`SjisToGlyphIndex(ch)` index, 8 bytes each at `0x081C0000`) | `RenderSjisGlyph` |
| `0x08079068` | 0x18C | **matching** (wave 3, 2026-10-02; FAKEMATCH) | build a two-tile glyph from `SjisToGlyphIndex` index into `0x081D0200` (0x14 bytes/glyph = 10 rows of 8 bits): for each of 10 rows write a 32-bit word to `dst` and one to `dst+0x10`, shifting the row bits into 4bpp nibbles (set bits take colour `arg3`, clear bits `arg4`; with `0x02011C20[4]` bit 7 both words start as solid `arg4`, otherwise they keep the existing tile data); 8 rows per block, two blocks 0x150 halfwords apart, a countdown stops after 10 rows | `RenderWideGlyph` (hypothesis) |
| `0x080791F4` | 0xAC | matching | render a string into `dst` (0x20 bytes per tile): Shift-JIS mode (`0x02011C20+4` bit 7) takes 2 bytes per glyph through `RenderFullWidthGlyph` (codes above `0x813F`), otherwise ASCII half-width glyphs, two per tile via `half` toggle | `RenderStringToTiles` |
| `0x080792A0` | 0xA0 | matching | write `count` consecutive tile indices `start..` into a tilemap: mode 0 one cell each (`start \| pal << 12`), mode 1 a 2x2 block (`start*4 + 0..3`, second row `+0x20`) | `FillMapTiles` |
| `0x08079340` | 0xC4 | matching | draw a string as a text strip: `len = (n*5 + 7) >> 3`, `TextCanvasInit(len, 2)`, `TextDrawString(0,0,attr,str)`, `TextCanvasToTiles`, then fill two map rows with `PutMapTileRun` (second call `+0x40` bytes, wrapping `0x0600C7C8 -> 0x0600BFC8`) | `DrawTextStrip` |
| `0x08079404` | 0x70 | matching | `RenderStringToTiles`, then `FillMapTiles(p3, p1, q0, q3, (len+1)>>1)` | `DrawStringTiles` |
| `0x08079474` | 0x6C | matching | copy 13 bytes from `0x08087B94`, DMA3-fill `0xD0` halfwords at `dst` with 0, then 13 calls `OverlayBoldGlyphTile(dst + i*32, &buf[i], pal, 4)` | `InitDigitTiles` (hypothesis) |
| `0x080794E0` | 0x12C | **matching** (wave 2, 2026-10-01) | print `val` in decimal right to left through `FillMapTiles(base + digit, &dst[col-- + row*32], pal, m2, 1)`; mode 0 = exactly `n` digits, mode 1 = stops when the remaining value is 0 (special case for 0) | `PrintNumberRtl` |

## Structures (inferred)

- Animation script step (8 bytes, hypothesis): `+0 u8 frames` (duration, 0 terminates), `+1 u8 unk1`, `+4 u32 data`.
- Animation state (0x14 bytes): `+0 seq*`, `+4 data`, `+8 u16`, `+A u16` (both initialised to 0xFFFF), `+C u8`, `+D u8 idx`, `+E u8 active` (1 = running, 0xFF = skip in `AnimBlockDraw`), `+F u8 timer`, `+0x10 u8 prio`. A block is a run of states with the state count at block `+0x190`.
- Fade object: `+0 u8 kind` (0 = BLDCNT 0xFF, else 0xBF), `+2 u16 level` (8.8, 0..0x1000), `+4 s16 step`, `+6 u8 state` (1 running, 2 full, 3 zero), `+7 u8`.
- Text mode flag: `0x02011C20 + 4` bit 7 selects Shift-JIS rendering ([[sprite-c]] save mirror).

## Matching tricks

- `s32 m = ~1; *p = *p & m;` gives `mov r1,#2; neg r1,r1; ldrb; and` (a plain `&= ~1` on a byte is narrowed to `#254`). A bitfield `p->flag = 0` (`u8 flag : 1`) also gives the `neg` form (`sub_08078CA8`), and two locals `z = 0; two = 2;` fix the constant-load order.
- `if (step >= 0) x = 0; else x = 0x1000;` matches; the ternary version becomes branchless.
- `if (mode != 0x10) { if (mode == 0x100) B } else A` reproduces `cmp; beq A` with A laid out last (`CopyTileRows`); a `switch` gives a different layout.
- Helper returning `u16` used as `SjisToGlyphIndex(ch) * 8` must be declared `int` (no `lsl/lsr 16` after the call).
- A 5-argument call to a 4-parameter function (`DrawStringTiles` passes a leftover stack arg): declare a second prototype `void f5(...) asm("RenderStringToTiles");`.
- `dst[x + (y << 5)]` versus `dst[y * 32 + x]` decides `adds r0,r2,r0` vs `adds r0,r0,r2`.
- Post-increment placement (`p++; *pp = p;`) and local declaration order decide where `adds r4,r0,#0` (parameter copy) is emitted (`RenderStringToTiles`).
- DMA wait loop: use a second pointer `d2 = (vu32 *)0x040000D4` for the `while (d2[2] & 0x80000000)` to get the base register copy (`LoadDigitTiles`).
- Functions where the ROM keeps a constant `0` in a high register across a loop (`0x080792A0` row offset in `sl`) otherwise constant-fold it whatever the source shape (declaration, in-loop, end-of-loop assignment, `zero = i`); the fix was a wider local (see the `short row` bullet).
- `PutMapTileRun` is solved: declaring the tilemap row offset as `short row;` (instead of `u8`) makes agbcc emit the `row = 0` hoist *after* the loop guard as the ROM does (permuter score-0 result).
- `ParseSignedDecimal`: writing the accumulator update as `val = c - 0x30 + val * 10;` (rather than `c + (val * 10 - 0x30)`) makes agbcc hoist the pooled `0xFFD0` straight into `r6` instead of `ldr r0; adds r6, r0, #0`.
- `RenderHalfWidthGlyph`/`RenderFullWidthGlyph`: declare a `u16 m = 0xF;` before the loop, use `& m` for the three *shifted* nibble extracts and leave the plain `*src & 0xF` as a literal. That fixes the hoist order so the mask lands in the ROM's register (`r8`/`r9`) and the counter in the other; using a literal everywhere or `m` everywhere gives the wrong order.
- Declaring the zero padding argument as `int zero = 0;` inside the `case 1/2/4/8 { ... }` block (and the function return type `int`, not `void`) makes agbcc keep zero in `r8` and rematerialise `-1` per iteration, exactly like the ROM (`AnimBlockDraw`). With `void` it hoists `-1` into `r8` instead and the epilogue pops `r0` rather than `r1`.

## Open items

None remain: the whole unit is in C since wave 3 (2026-10-02). The entries below are historical.

- Historical (matched in wave 2 with FAKEMATCH forms, see below): `0x08078CC8`: register allocation is completely different (`x` in `r5` / `fill` in `r2` / `w` in `r3` in ROM; ours puts the fill in `r5`, stashes `x` in `r8` and spills `w`). Tried >15 shapes (declaration order, `x`/`w` locals, `(x<<4)*w` versus `x*w*16`, `u16`/`u32`/`int` fill, `fill = n*0x1111`, explicit masks): no improvement. Best attempt is the original draft.

> [!warning] Contradiction
> The next item (2026-10-01 and earlier) says the allocator keeps the wrong value in `sl` for every shape tried in `0x080794E0`. The wave 2 match (2026-10-01, `build/wf/DrawNumberTiles/NOTES.md`) fixed `n` in sl and `row` on the stack with a control-flow change: `return` instead of `break` in the mode-1 loop, which gives the ROM's rotated loop. Resolved in favour of the matched source.

- Historical (matched in wave 2, see below): `0x080794E0`: ROM keeps `n` in `sl` and `row` on the stack; ours swaps them (and keeps `col` in `r0` rather than reloading). Tried >15 shapes (`cnt = n` local, `row` local/reordered declarations, `u8`/`u16`/`int`/`u32` `d` and `zero`, `while` loops, if/else for the mode dispatch): the allocator always keeps the wrong one in `sl`.

- `0x080786D0` now matches. A separate timer local avoids a byte reload after decrement; a full-width next index is stored as a byte, then masked before indexing the cached sequence pointer. A read/write empty barrier on the initialized `0xFF` mask prevents CSE with the timer comparison. State and final index are constrained to r1/r3, and an empty input barrier keeps the unscaled index live through the final sequence-byte load. Two additional barriers were tested and removed as unnecessary. These constraints emit no instructions and are marked `FAKEMATCH`. `tools/check.py text_render` verifies 25/25 functions and all 0x1128 unit bytes MATCH.
- `0x080792A0` now matches. The old blocker was the hoisted `row = 0` placement; `short row` fixed it.

> [!warning] Contradiction
> The next item (2026-10-01 and earlier) calls the remaining `0x08079068` difference "pure allocation". The match (an earlier session plus wave 3, finished 2026-10-02, `build/wf/RenderKanji10x10Glyph/NOTES.md`) needed a DImode `u32 w[2]` pair, GCSE hash-order declaration tuning and a dead store that changes which invariants loop.c hoists (the clean build hoists three colour-fill pairs, the ROM two: one per loop pass). Resolved in favour of the matched source.

- Historical (matched in wave 3, see below): `0x08079068`: structurally faithful draft (10 rows of 8 bits from `0x081D0200`, two 32-bit words per row at `dst` and `dst+0x10`, base colour `arg4`, alternate `arg3`, countdown 10, two 0x150 blocks; the 32-bit fill stays inside the `if (0x02011C20[4] & 0x80)` branch). agbcc still hoists the glyph into `ip` instead of `sl`, puts `w1`/`w2` in `r4`/`r3` instead of `r5`/`r6`, `i` in `r6` instead of `r3`, and hoists the constant `1` into `sl` (~124 differing instructions, pure allocation). Best attempt parked under `#if 0`.

> [!warning] Contradiction
> The next item (2026-10-01 and earlier) says `i` is `u32` and reused for the glyph stride, "matching the ROM's reuse of the constant `1`". The wave 2 match (2026-10-01, `build/wf/RenderShadowedGlyph/NOTES.md`) found that the ROM's counter is a `u16` (`lsl #16; lsr #16`) separate from a `u32 one = 1`; the two only share r5. The `cmp #7` is fold's canonical form of a literal `i < 8`, fixed with a variable bound. Resolved in favour of the matched source.

- Historical (matched in wave 2, see below): `0x080788AC`: structure matches (peeled row 0 with only the base-colour branches, then rows 1..7 with the `buf[i]` outline else-branches; `buf[i+1]` shadow marking; index is `ch*8` into `0x0822BB00`). Down to ~138 diff lines: agbcc keeps the row counter `i` in `r6` and the glyph pointer `g` in `r5` (ROM: `i r5`, `g r6`), swaps the `c24`/`c28` stack-vs-`sl` slots, and uses `cmp #7` where ROM uses `cmp #8`. `i` is `u32` and is reused for the glyph stride (`i << 3 == 8`, matching the ROM's reuse of the constant `1`). Declaration/type permutations all give 138; register pins make it worse. Best permuter seed parked under `#if 0`.

Related: [[decomp-workflow]], [[compiler-flags]].

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/RenderShadowedGlyph/NOTES.md`, `build/wf/TextCanvasRowsToTiles/NOTES.md`, `build/wf/DrawNumberTiles/NOTES.md`.

### `RenderShadowedGlyph` (`RenderGlyphOutlined`, 0x39C, start score 82; ordinary C)

- The ROM compares the row counter with `cmp r5,#8; bcc/bcs`, both before the loop (unfolded, after `movs r5,#1`) and at the bottom. A literal `i < 8` is canonicalised by fold to `i <= 7` (`cmp #7; bls`) whatever the spelling: `8 > i`, `8u`, casts, `!(i >= 8)`, do/while, goto loops and `sizeof` all gave 7.
- What matched:
  1. `u16 i` with a separate `u32 one = 1`, used for `*flags & one` and the stride `ch * (one << 3)`. The old u32 counter doubled as the constant 1.
  2. `u16 n = 8;` and `for (i = 1; i < n; g++, i++)`. The bound is a variable at tree level, so fold leaves the `<` alone. The pseudo `n` loses allocation and reload substitutes its REG_EQUIV constant, giving `cmp r5,#8` with the unsigned `<` kept. CSE cannot fold the pre-test because it does not know `n` in that block, and this compiler's GCSE cprop does no constant propagation into cc0 compares. Score 82 to 70.
  3. `g = gFontLatin8x8Bold;` before the `if (ch > 0x9F)` remap, then `g += ch * (one << 3);`, because the ROM loads the glyph base before the remap. Score 70 to 0.
- Failed: `for (i = one; ...)` (148); `i < (one << 3)` as the bound (202, CSE keeps `one << 3` in a register).

### `TextCanvasRowsToTiles` (`BlendGlyphBuffer`, 0xFC, start score 112; FAKEMATCH)

Twin of `TextCanvasToTiles` ([[main-c]]) for 3 tile rows. Each device is commented `FAKEMATCH` in the source:
1. Same inner loop as the twin (`i = next` counter, `src += 4`, `*dst &= mask; *dst |= v`). The fill is computed on the `color` parameter inside `do { } while (0)`; loop depth weights its refs, so fill gets r2.
2. The ROM preheader stores a copy of `w` to `[sp+4]`, then `w*x` to `[sp]`, and the end test recomputes `3 * [sp+4]` each iteration. The copy is a loop-invariant no-op narrowing `(w & 0xFF)` used in both `src` and the end test: loop.c hoists it, the forces chain drags `(w & 0xFF) * x` out after it, and CSE2/combine turn it into a plain copy of w. Plain `w` lets GCSE share `3w` with the guard, which is hoisted to a high register; `(u8)w` expands to shift pairs that PRE breaks up.
3. An explicit guard, `i = 0; if (i < w * 3) do { ... } while (i < (w & 0xFF) * 3);`. A `for` loop duplicates the exit test into the guard, which then computes `(w & 0xFF)` into its own pseudo, makes w block-local (r1) and moves the `[sp+4]` store.
4. `buf = gTextCanvas;` in the preheader: CSE2 makes it a copy of the entry-block base load, so that load becomes a global pseudo in r6; reload deletes the copy (REG_EQUIV) and the loop rematerializes the base with `ldr r5,=`.
5. `src = (const u8 *)((i + (w & 0xFF) * x) * 64 + (u32)buf)`: the integer sum puts the offset first (`adds r3,r0,r5`); pointer arithmetic emits the base first.
6. `do { dst += (x << 4) * w; } while (0);` raises x's ref weight, so x (r5) is allocated before the entry-block base (r6); `(x << 4) * w` gives the ROM's `lsl x,#4; mul w` order.
- Failed or irrelevant: w types (u8/u16/s16/u32, all promoted to SImode); a const extern alias for the width read; an inner-loop-indexed `src` (it hoists `w*x` in the wrong order); a `t` variable set at the loop top; register pins to r6 (they shift every later allocation); reg+reg addressing (every variant added first).
- Reload note: per-insn spill registers come from `order_regs_for_reload`, free call-used registers first, then free callee-saved ones, then by pseudo use count. A reload landing in r6 instead of r1/r3 means a global pseudo held the low register at that insn.

### `DrawNumberTiles` (`PrintNumberRtl`, 0x12C, start score 79; ordinary C)

- The digit `d` must be `u16`: the ROM narrows the `%` result with `lsl/lsr #16`.
- The mode-1 loop's `if (d == 0 && val == 0) break;` made jump.c lay the loop out with an entry jump into the middle. `return;` gives the ROM's rotated loop (79 to 27) and also puts `n` in sl and `row` on the stack.
- Last diff: the ROM copies col to r0 in the prologue (`mov r0, r9`) and uses that copy in the `val == 0` call. That is what remains of a `col--` in that call too (copy-pasted source): the post-decrement's saved old value survives as a separate pseudo while the dead decrement is deleted. Writing `dst + (col-- + row * 32)` in the `val == 0` call, followed by `return;`, gives 0.
- Failed: a local `u8 x = col` for the loops (103); an if/else-if chain instead of the `switch` (42). The same shape matched `PutMapNumber` ([[bitmap-text-c]]) and `DrawNumberSprites` ([[link-sio-c]]).

## Wave 3 matches (2026-10-01/02)

Working notes: `build/wf/RenderKanji10x10Glyph/NOTES.md`.

### `RenderKanji10x10Glyph` (`RenderWideGlyph`, 0x18C, start score 27; FAKEMATCH)

An earlier session took the draft from score 217 to 27 with steps 1-6 and parked it (its `NONMATCHING` note is the wave 3 `prep.json` note); wave 3 added step 7. In order of impact:
1. `u32 w[2]` instead of two scalars. The pair becomes one DImode pseudo (r5:r6) accessed through subregs, and regmove ignores subreg destinations, so the bic/orr temporaries stay untied: the ROM's `adds r4,r5,#0; bics r4,r0; adds r0,r4,#0 ... adds r5,r0,#0; orrs r5,r2`, and the fill's `adds r6,r0; adds r5,r0` (217 to 181).
2. `if (bit) col = c; else col = color;` as statements, not a ternary inside the expression: jump.c turns it into `col = color; if (bit) col = c`, with the bit computed first.
3. Shift positions written `(7 - i + a)` and `(8 - i + a)`: fold rewrites `a + 7 - i` as `a - (i - 7)`, but `7 - i + a` becomes `(a + 7) - i`, so GCSE and loop hoist `a + 7` / `a + 8` into `[sp+0x24]` / `[sp+0x28]`.
4. Declaration order `u8 i, row, blk; u8 n = 10;`: GCSE PRE numbers its reaching registers in hash-bucket order (hash = regno + const + 69 mod 71), which sets the stack slots of `dst+2`, `a+7`, `a+8`, `dst2+2` and `n-1`; n's regno must be at least dst2's + 4.
5. Narrow temporaries `u16 b1 = src[0] >> i; if (b1 & 1)` and `short b2 = swapped >> (8 - i + a); if (b2 & 1)` (permuter-found). Combine deletes their extensions, but they change the RTL size and allocation (165 to 81: dst2 to r7, `0xF` to ip).
6. The integer address `(u16 *)(0x081D0200 + SjisToGlyphIndex(ch) * 20)`, as in the neighbour `RenderFullWidthGlyph`. The symbol form gives a different REG_DEAD note order on `src = t + addr`; reload's combine_reloads then reuses the first dying register for the output (r1 instead of r0) and the entry-block reload rotation shifts (81 to 27).
7. FAKEMATCH: a dead `b2 = ...` store after the inner loop (27 to 0). loop.c hoists while `threshold * savings * life >= insn_count`, with the threshold starting at 26 and dropping by 3 per moved insn; each colour-fill pair has savings 2 and life 3. The ROM hoists only `color | color << 4 | color << 8`, one pair in each loop pass, which needs the row loop at 121-156 RTL insns in both passes; clean C gives 122 and 117. The dead store adds enough insns and is deleted later.
- Failed natural ways to add the insns: split `&=`/`|=`, u16/s16 temporaries, an explicit `g = *src` before the loop, `(b1 & 1) != 0`, `b1 >>= i`, `col` as u16/int, a duplicated fill expression (CSE deletes it before loop), `ch++; ch--;` (20), a dead store in the else branch (10). Also failed: `(u16)SjisToGlyphIndex(...)` (an extra lsl/lsr pair) and an explicit `int o7 = a + 7` (wrong hoist bookkeeping).
- Cleanup idea: a natural construct that adds about 4 RTL insns to the row loop and survives into the second loop pass (rerun-loop-opt), such as more narrow-type temporaries whose extensions combine removes.
