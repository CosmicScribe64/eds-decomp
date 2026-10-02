---
title: code_0807A6AC (tilemap / palette / easing utilities) decompilation status
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# BG tilemap, palette-fade and easing helpers in code_0807A6AC (`0x0807A6AC`-`0x0807B6B8`)

The unit is `src/code_0807A6AC.c` (42 functions, 0x100C bytes), compiled with `old_agbcc -O2`. **42/42 functions in C** after workflow waves 2-3 (2026-10-01: `0x0807AEF0`, `0x0807AD40`, `0x0807ADE8` in wave 2; none in wave 3); none stay `INCLUDE_ASM`. Before wave 2: 39/42 (`0x0807A754`, `0x0807B628` added in wave 1). The unit still links to the exact target bytes. Names below are proposals, and the code keeps `sub_08XXXXXX`.

This unit sits between the LZSS decoder (`sub_0807A1A8`, see [[lzss-decompress]]) and the sound driver. It is a small graphics-utility library used by menus and duel screens. It provides rectangle fills and copies on BG screenblocks (32-entry rows, with the 64-wide "second screenblock" wrap at column 0x20), palette fades toward a target colour, rotation/scale objects, fixed-point helpers and tiny state machines. Related: [[video-helpers]].

## Functions

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x0807A6AC` | 0xA8 | matching | `FillTilesAscending(tile, bg, x, y, w, h)` | w x h rect of BG screenblock `bg` (`VRAM + bg*0x800`), tile numbers ascending; columns 0x20-0x3F land in the next screenblock (`+0x7C0`), >= 0x40 wrap (`-0x80`) |
| `0x0807A754` | 0xB4 | **matching** (wave 1, 2026-10-01) | `FillTiles32(tile, bg, x, y, w, h)` | same with 32-bit stores of `tile \| tile<<16`, w/2 stores per row |
| `0x0807A808` | 0x90 | matching | `FillTilesConst(tile, bg, x, y, w, h)` | constant tile |
| `0x0807A898` | 0x70 | matching | `FillTilesPal(dst, tile, pal, w, h)` | ascending 10-bit tile numbers with palette nibble, flat `dst` |
| `0x0807A908` | 0x58 | matching | `CopyRows(src, dst, w, h)` | `CpuSet` w halfwords per row, dst stride 0x40 |
| `0x0807A960` | 0x60 | matching | `CopyRowsStride(src, dst, w, h, stride)` | same with dst stride `stride*2` |
| `0x0807A9C0` | 0x8C | matching | `CopyRectAdd(src, dst, w, h, srcW, pal, hi)` | copy adding `pal<<12` and `hi<<8` to each entry |
| `0x0807AA4C` | 0x98 | matching | `CopyRectMask(...)` | same but keeps `src & 0x3FF` |
| `0x0807AAE4` | 0xA8 | matching | `CopyRingRows(src, dst, w, h, srcX, dstRow, srcStride)` | `dst[(srcX+j)&0x1F] = src[srcX+j]`, dst wraps every 32 rows |
| `0x0807AB8C` | 0x74 | matching | `ShiftPalBits(src, dst, w, h)` | `(e & 0xFC0F) \| (e & 0x3F0) << 1` |
| `0x0807AC00` | 0x5C | matching | `SetRectPalette(dst, w, h, pal)` | replaces the top nibble of every entry |
| `0x0807AC5C` | 0x2C | matching | `SetTileNumber(bg, x, y, tile)` | keeps the top 6 attribute bits, sets the tile |
| `0x0807AC88` | 0xB8 | matching | `DrawNumber3(bg, base, x, y, num, pal, _, mode)` | 3 decimal digits right to left (mode 1 skips zero digits) |
| `0x0807AD40` | 0xA8 | **matching** (wave 2, 2026-10-01) | `CopyBitmapBlock(...)` | h rows of w halfwords copied with `CpuSet` between two bitmaps (source stride `srcW`, destination stride 0x20 halfwords); both start at `base + sub_0807A490(x, y, shift) / 2` (the helper's byte offset as a halfword index) |
| `0x0807ADE8` | 0xA0 | **matching** (wave 2, 2026-10-01) | `CopyBitmapBlockFlat(...)` | same, but the source is a plain srcW-wide halfword array (`srcBase + sx + sy * srcW`) |
| `0x0807AE88` | 0x68 | matching | `PackBytePairs(src, dst, w, h)` | `dst[i] = (src[2i] & 0xFF) \| (src[2i+1] & 0xFF) << 8` |
| `0x0807AEF0` | 0x10C | **matching** (wave 2, 2026-10-01; FAKEMATCH) | `LoadPackedImage6bpp(idx, dst, bank)` | unpacks 6-bit pixels (record idx of 0x10E0 bytes at `0x082A6500`, 720 x 6 bytes -> 720 x 8 bytes) to 8 bpp; copies a 64-colour palette (`0x08608360 + idx*0x80`) to `PLTT + ((bank&0x3FF)*64 + 0x80)*2`; ORs the sub-palette bits into every pixel byte |
| `0x0807AFFC` | 0x14 | matching | `LoadPackedImage6bppWrap` | wrapper narrowing args to u16 and calling `sub_0807AEF0` |
| `0x0807B010` | 0x18 | matching | `CallbackQueue_Init` | zero 4 slots |
| `0x0807B028` | 0x30 | matching | `CallbackQueue_Add(fn, q)` | slot `[(head+1)&3] = fn`, returns old head, fails (0) if `slot[head]` is busy |
| `0x0807B058` | 0x30 | matching | `CallbackQueue_Run(q)` | calls each non-null slot; a non-zero (u16) return clears it |
| `0x0807B088` | 0x8 | matching | `CallbackList_Init(table, l)` | `l->idx = 0; l->table = table` |
| `0x0807B090` | 0x30 | matching | `CallbackList_Step(l)` | run entry `idx`; advance when it returns non-zero; returns 1 when the table ends |
| `0x0807B0C0` / `0x0807B0C8` | 0x8 | matching | `Timer_Reset` / `Timer_Start(count)` | `{u8 state; u16 count}` |
| `0x0807B0D0` | 0x1C | matching | `Timer_Tick` | `--count == 0` -> state 2 |
| `0x0807B0EC` / `0x0807B100` | 0x14 | matching | `Ease_Init` / `Ease_Start(cur, end, step, e)` | `{u8 state; u16 cur, end; s16 step}` |
| `0x0807B114` | 0x3C | matching | `Ease_Tick` | `cur += step`, stops at `end` |
| `0x0807B150` | 0xD4 | matching | `PalFade_Start(pal, count, color, f)` | copies `count` BGR555 colours into `f->cur`, stores per-channel `target - current` in `f->delta[i][r,g,b]` |
| `0x0807B224` | 0xF8 | matching | `PalFade_Apply(f)` | writes `cur + delta*step/32` per channel to `f->dst` while `state == 1`; state 2 at `step == 0x20` |
| `0x0807B31C` / `0x0807B3DC` | 0xC0 / 0xCC | matching | `PalFadeSmall_Start` / `_Apply` | 8-colour variant with a source stride and struct layout `{cur[8], delta[8][4] @0x10, step @0x30, count @0x32, dst @0x34, stride @0x38, state @0x3A}` |
| `0x0807B4A8` | 0x18 | matching | `SetBldAlpha(a)` | `BLDALPHA = a<<8 \| (0x10 - a)` |
| `0x0807B4C0` | 0x10 | matching | `SetBldY(a)` | |
| `0x0807B4D0` | 0x10 | matching | `MulFix8(a, b)` | `a*b >> 8` (s16) |
| `0x0807B4E0` | 0x24 | matching | `MulFix8Wide(a, b)` | 64-bit `a*b >> 8` |
| `0x0807B504` | 0x18 | matching | `DivFix8(a, b)` | `Div(a << 8, b)` |
| `0x0807B51C` | 0x18 | matching | `Reciprocal8(a)` | `Div(0x10000, a)` |
| `0x0807B534` | 0x6C | matching | `ObjAffineInit(a)` | 32 x `ObjAffine` (stride 0x18): scale 0x100/0x100, angle 0, four `s16 *` to the OAM affine slots `0x03004476 + (i*4+j)*8` (`oam[i*4+j].pad`) |
| `0x0807B5A0` | 0x88 | matching | `ObjAffineApply(a)` | writes pa..pd from `1/scale` and the sine table `gUnk_08087BA4` (index `angle>>8`, +0x40 for cos) |
| `0x0807B628` | 0x90 | **matching** (wave 1, 2026-10-01) | `BgAffineSetRef(bg, x, y, cx, cy, a)` | writes BG2/BG3 reference point (`0x04000028/2C`, `0x04000038/3C`) = matrix * (p - c) + c |

## Structures (as declared in the unit)

- `CallbackQueue {u8 head; u16 (*slot[4])(void) @+4}`; `CallbackList {u8 idx; u16 (**table)(void) @+4}`; `Timer {u8 state; u16 count @+2}`; `Ease {u8 state; u16 cur @+2, end @+4; s16 step @+6}`.
- `PalDelta {s8 r, g, b, pad}`; `PalFade {u16 cur[0x200]; PalDelta delta[0x200] @+0x400; u8 step @+0xC00; u16 count @+0xC02; u16 *dst @+0xC04; u16 state @+0xC08}`; `PalFadeSmall` (same with `delta[8]` at +0x10). The caller advances `step` (0..0x20).
- `ObjAffine {s16 scaleX, scaleY (8.8); u16 angle (high byte = 256-step angle); s16 *param[4] @+8}`, stride 0x18.

## Matching tricks learned (old_agbcc)

- **Write the natural code first.** Several functions that looked hopeless matched once the temporaries were removed. `Ease_Tick` is just `e->cur += e->step;` with an early `if (state != 1) return;` and the two `state = 2; cur = end;` tails duplicated. `CallbackQueue_Run` is `if (q->slot[i] != NULL) { if (q->slot[i]()) q->slot[i] = NULL; }` with no pointer local. The palette fades use `f->cur[i]` directly in every expression, because a `u16 c = f->cur[i]` local changes the register/hoisting picture completely.
- **Pre-increment index**: `q->slot[++q->head & 3] = fn;` (plus the single-`ret` if/else form) kept the `and r0,#0xFF` that the target has before `& 3` in `CallbackQueue_Add`; `q->head = head + 1; q->slot[(head+1) & 3]` folds it away.
- **Array of small structs for the fade deltas**: `struct PalDelta {s8 r, g, b, pad;} delta[0x200]` (accessed `f->delta[i].r`) gives the `f + i*4` then `+0x400/0x401/0x402` addressing and the `ldrsb rD,[rB,rIdx]` form; `s8 delta[..][4]` or `((s8 *)f)[i*4+0x400]` do not.
- **`f->cur[i] = *pal++; ... (f->cur[i] & 0x1F)`** (source pointer post-incremented in the store, all later uses read back `f->cur[i]`) matched `PalFade_Start`; a `u16 c` local hoists `0x1F` into a register.

- **`i = 0` before other loop-init work**: `for (i = 0, bits = pal << 12; i < 3; i++)` reproduced the target order `movs r6,#0` before `lsls #12` (`DrawNumber3`); a plain `u32 bits = pal << 12;` earlier swaps the registers.
- **Compute-once `u16 d = num % ten;`** and a `u32 ten = 10;` local reproduce the constant kept in r7 and the reused remainder in `DrawNumber3`.
- **Local `u32 size = w * 2;` *inside* the row loop** (loop-invariant motion hoists it) matched `CopyRowsStride`; declaring it before the loop merges it into `lsrs r7,r2,#23`.
- **`long long p = (long long)a * b; return (s32)(p >> 8);`** (two statements) matched the 64-bit fixed multiply; the one-expression form differs in the final register moves.
- **Calling s16-returning helpers**: the target's callers do not re-extend the s16 result of `sub_0807B51C` / `sub_0807B4D0`. Reproduced with `extern int Reciprocal(int) asm("sub_0807B51C");` style declarations (an asm-labelled int prototype) in the caller, since a C prototype `s16 f(s16)` makes the caller emit `lsl 16; asr 16`. The original probably saw these functions through an int-typed declaration from another translation unit.
- **`*a->param[0] = ...; a->scaleX` as `s16`**, sign-extending loads for `s8 *d` use `ldrsb r0,[rN,rM]` with the offset in a register (`d[0x10]`); a `s8 delta[i][j]` array member compiles to `ldrb; lsl 24; asr 24` instead.
- **Switch with two cases and a common tail** (`BgAffineSetRef`): `switch (bg) { case 2: reg = ...; break; case 3: reg = ...; break; default: return; }`. Embedded `x -= cx` inside the first call's argument list (`sub_0807B4E0(*a->param[0], x -= cx)`) reproduced the in-place `subs r6,r6,r3`.
- **`(u32)(xx - 0x20) <= 0x1F`** gives the `sub #0x20; cmp #0x1F; bhi` unsigned range test seen in the wrap checks.

- **Signed row-size temporary**: `s32 size = w * 2;` inside the row loop matches `CopyRows` (`0x0807A908`); `u32` gives the wrong register allocation.
- **Byte palette temporary**: `u8 nib = pal & 0xF;` after the tile mask matches `FillTilesPal` (`0x0807A898`), including the target's early palette shift and mask into r0.
- **Ring-copy counters**: `CopyRingRows` (`0x0807AAE4`) uses `u16 i`, `u16 dstRow` narrowed into `u32 row = (u8)dstRow`, and `for (j = 0, row++; ...)`. Assigning the narrowing back with `if ((row = (u8)row) == 0x20)` matches the wrap test and register scheduling.
- **Affine pointer indices**: `pp[(j * 4 + i * 24) >> 2]` matches `ObjAffineInit` (`0x0807B534`). The byte-offset sum with `j` first prevents factoring to `(i*6+j)*4` and retains the target's separate `i*24` and `j*4` shifts.

`0x0807A808` is matching C. Its retained `FAKEMATCH` duplicate-store branch preserves the separate wrap branches required by the ROM; the unit bytes were checked after applying it.

## Nonmatching notes

All entries below are historical: the whole unit is in C since wave 2 (2026-10-01). The remaining five drafts were re-checked by hand (2026-09-30, clean-up pass). No permuter job for this unit has a score-0
`output-0-*` result, so none were applied; the parked drafts below include the improvements the score-50/200 runs
pointed at. The remaining blockers concern compiler allocation and loop scheduling.

- `0x0807A6AC` matches. Compute the wrap destination pointer before `value = tile++`, and keep that initialized old tile value live with an empty input barrier before the store. This reproduces the address-first scheduling, separate branch stores and target value-copy register. The destination-pointer barrier was removed after exact whole-unit checks; no register constraints are needed. The remaining value barrier emits no instructions and is documented `FAKEMATCH`. Verified `tools/check.py code_0807A6AC`: 42/42 functions and all 0x100C unit bytes MATCH.
- **32-bit tile fill**:
  - Historical `0x0807A754` (matched in wave 1, see below): the build folds `half = w>>1` into a register (`frame sub sp,#4`) and hoists the tile word; the
    target keeps `half` in `[sp]` (reloaded per row), `next = i+1` in `[sp+8]` and the row stride `(0x10-half)*4`
    in `[sp+4]` (`sub sp,#12`) and recomputes `v = tile|tile<<16` per row. Adding an explicit `next`/`v` and
    changing `half`'s type did not raise register pressure enough to force the spills.

> [!warning] Contradiction
> The next note and this section's introduction (2026-09-30 clean-up pass) put the `0x0807AD40` / `0x0807ADE8` blockers down to compiler allocation and loop scheduling, with source order, type and offset-local variants all leaving the same split. The wave 2 matches (2026-10-01, `build/wf/sub_0807AD40/NOTES.md`, `build/wf/sub_0807ADE8/NOTES.md`) came from the source: a `u32` return type with no value (`pop {r1}`), `/ 2` on the real u16 return of `sub_0807A490` instead of `& 0xFFFE`, and the CpuCopy16 size shape. Resolved in favour of the matched source.

- Historical (matched in wave 2, see below): `0x0807AD40` / `0x0807ADE8`: target spills `srcBase`/`srcW` to `[sp]`, keeps the `w & 0x1FFFFF` mask (0x1FFFFF
  rebuilt in the loop) and reloads the 0xFFFE mask from the literal pool per call; the build keeps the base in
  `r7`, spills the other value and CSEs the masks. Source order/type/off-local variants all leave the same split.
- Historical `0x0807AEF0` (matched in wave 2, see below): the `0x3F`/`0xFC0` masks **do** need to be locals (`u16 m6, m12`) so they hoist into `r8`/`r9`
  as the target does (bare literals leave the first use an immediate and swap r8/r9). This is now in the parked
  draft. ~80 instruction lines of register allocation in the two loops still differ; `u32`/`s32` masks, `u16 i`,
  `dst = dstAddr` before the call and 2nd-loop `i < 0xB40` did not close it.
- Historical `0x0807B628` (matched in wave 1, see below): target keeps `bg` in `sl` (a third pushed callee-saved reg) and `sx` in a fresh `r7` while `cx`
  stays in `r9` (`mov r0,r9; adds r7,r4,r0`), storing `str r7,[r0]; adds r0,#4`; the build keeps `bg` in low `r7`,
  lets `cx` die and reuses `r9` for `sx`, storing through `r1`. Putting `cx` first in the expression, a separate
  `sx += cx;`, `if/else` versus `switch` and `reg[0]/reg[1]` all keep the split.

## Open questions

> [!question] `sub_0807A490(x, y, shift)` (in the previous unit) returns a byte offset into a tiled bitmap; its exact meaning (hypothesis: 8x8-tile offset for a bitmap of width `1 << shift` tiles) was not verified. Its return type is `u16` (matched definition in `src/code_0807960C.c`; the wave 2 crop-copy matches depend on it).
> [!question] The callers of the palette-fade and callback-queue helpers are in menu/duel code; the fields written by the callers (`step` in particular) were not traced.

Related: [[decomp-workflow]], [[compiler-flags]].

## Signed fixed-point divide ABI audit

`sub_0807B504` receives full words from its callers, explicitly decodes each to s16, computes `Div(a * 256, b)`, then sign-extends the s16 result. Its enabled C definition expresses that word ABI, matching declarations in [[code-08065e6c]] and [[code-0806c4e4]]. Multiplication also avoids the former negative signed left shift. All three complete units and the full ROM remain byte-exact. Zero-denominator BIOS behavior is unchanged and not established by the synthetic percentage fixtures.

## Private crop-helper audit

Historical (the function matched in wave 2 with the ordinary C below): `sub_0807ADE8` consumes its fifth argument, the destination base, from the adjusted stack after the coordinate helper call. Explicit word parameters preserve the caller interface, but `crop_rows_{resume,roles}.py` did not match. One fixed-register candidate overwrote dy with width before consuming dy; it is unsafe and rejected. No candidate from this grid is enabled or counted. Evidence under `build/bigguns-lead2/`.

## Workflow waves 1-2 matches (2026-10-01)

Working notes: `build/wf/<func>/NOTES.md` (for the wave 2 crop copies added on 2026-10-02: `build/wf/sub_0807AD40/NOTES.md`, `build/wf/sub_0807ADE8/NOTES.md`).

### `sub_0807A754` (`FillTiles32`, 0xB4, start score 61; wave 1, ordinary C)

Same fix as its sibling `sub_0807A5D4` in [[code-0807960c]]: no `half` local. Write `w / 2` inline in both the inner loop bound (`j < w / 2`) and the stride (`p += 0x10 - w / 2`), with `u8 i, j`. c-typeck shortens `u8 / 2` to an unsigned-char division (the redundant u8 narrowing), and the rotated loop's two condition copies are hoisted differently, which produces the `[sp]` spill and the per-row bound reload. Matched on the first try.

### `sub_0807B628` (`BgAffineSetRef`, 0x90, start score 10; wave 1, ordinary C)

The parked draft selected a `vu32 *reg` in the switch and stored `*reg++ = sx; *reg = sy` (with a `u16 sx` hack). What matched: plain `s32 sx, sy` and writing both registers by literal address inside each case (`*(vu32 *)0x04000028 = sx; *(vu32 *)0x0400002C = sy; break;`, and the same for 0x38/0x3C). agbcc derives the second address as `adds r0,#4` from the first, and cross-jumping merges the two case tails into the shared `str r7,[r0]; adds r0,#4; str r4,[r0]`, which also gives the target's allocation (bg in sl, sx in a fresh r7). Matched on the first experiment.

### `sub_0807AEF0` (`LoadPackedImage6bpp`, 0x10C, start score 68; wave 2, FAKEMATCH)

Twin of `sub_0805DF34` ([[code-0805d58c]]); its matched body was ported (mask locals, u16/u32 types, `* 64`, integer ROM addresses). Extra for this twin:
- Use the `u32 dst` parameter directly (cast at each use, `dst += 2`); a separate `u16 *dst = (u16 *)dstAddr` copy moves the `adds r7, r1, #0` later in the prologue.
- The ROM keeps the second-loop counter in ip (0x3F3F in r2, 0xB3F in r3). No ordinary form reproduced that: a shared counter for both loops outranks dst (gets r7); u16/int/for/while/do/index forms keep it in r2. FAKEMATCH: `register u32 i asm("ip")`. With the pin, a `for` keeps its entry test (`cmp/bhi`), so the loop is a do-while, and the bound is a local `lim = 0xB3F` set before the loop so the literal loads first, as in the ROM.
- Cleanup idea: find why global alloc gives the counter ip (it must be allocated after the 0x3F3F/0xB3F invariants and see r4-r6 as unavailable), perhaps via an inline helper shared with `sub_0805DF34`.

### `sub_0807AD40` (`CopyBitmapBlock`, 0xA8, start score 81; wave 2, ordinary C)

- The ROM's `pop {r1}` epilogue: the function is declared `u32` but returns no value. A `void` function pops into r0.
- The `0xFFFE` mask reloaded from the pool after each call: the source is not `& 0xFFFE` but `srcBase + sub_0807A490(...) / 2` on `u16 *` pointers, with `sub_0807A490` returning `u16` (its real type; the unit's prototype said `u32` and is now `u16 sub_0807A490(u16, u16, u8)`). Combine turns `(x >> 1) << 1` into `x & 0xFFFE` after CSE has run, so every use loads the constant again. This also removes a call-crossing pseudo and fixes the allocation (srcBase spilled to `[sp]`, srcW in r9).
- The kept `w & 0x1FFFFF` mask with 0x1FFFFF hoisted to r8: the CpuCopy16 shape `s32 size = w * 2; CpuSet(src, dst, (size / 2) & 0x1FFFFF);` inside the row loop, as in `CopyRows` (`0x0807A908`). `u32 size` swaps the dst and i registers (16); a plain `w & 0x1FFFFF` folds the mask away.

### `sub_0807ADE8` (`CopyBitmapBlockFlat`, 0xA0, start score 71; wave 2, ordinary C)

Ported unchanged from the matched `sub_0807AD40`: `u32` return type with no value, `u16 *` pointers with `dstBase + sub_0807A490(...) / 2`, and the CpuCopy16 size shape; the source pointer is `srcBase + sx + sy * srcW`. Matched on the first try.
