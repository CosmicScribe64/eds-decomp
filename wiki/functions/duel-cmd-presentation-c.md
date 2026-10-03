---
title: Unit duel_cmd_presentation (duel command handlers, 4x5 sprite-grid effects)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit duel_cmd_presentation

`0x080150DC`-`0x080162C3`, Thumb, `old_agbcc -O2`. Source: `src/duel_cmd_presentation.c`. The unit sits between [[duel-cmd-turn-c]] and [[duel-cmd-screen-c]] and holds five duel "script command" handlers of the same kind as `DuelCmd_ShowCardAssemble` ([[duel-cmd-turn-c]]).

All five run a state machine on `gDuelCmd` `step` (+0x80A, 0..8) and `timer` (+0x80C bits 5-11). They call `DuelScreen_ScrollToZone(0, 0)`, `UnloadDuelUiGfx`, `LoadCardFrame/1D24/1E54(arg2)` and `TextCellsClear/0805F00C(arg2)`, and they animate a 4x5 grid of 32x32 sprites (tile `(i*2+1)*32 + j*4`, `AddSprite8bppAlpha` / `AddSprite8bpp` / `AddAffineSprite8bppAlpha`) with BLDALPHA / BLDY fades.

Unit status: `unit bytes MATCH`, **5/5 functions in C** after workflow wave 3 (2026-10-01: `0x080150DC` and `0x08015720` in wave 3); none stay `INCLUDE_ASM`. Before wave 3: 3/5 (`DuelCmd_ShowCardEffect`, `DuelCmd_ShowCardUnrollDown`, `DuelCmd_ShowCardUnrollSideways`). Names are proposals.

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x080150DC` | 0x2F8 | **matching** (wave 3, 2026-10-01; FAKEMATCH) | `DuelCmd_GridZoomIn` (hyp.) | step 6: grid zooms in around (0x68, 0x40) over 32 frames (`x*timer/32`), BLDALPHA fade-in, hold until timer 0x78 with a fade-out from 0x68, using the curve `gPulseScaleCurve` through `AddAffineSprite8bppAlpha` while timer < 16 |
| `0x080153D4` | 0x34C | matching (ordinary C) | `DuelCmd_GridZoomIn2` (hyp.) | like `DuelCmd_ShowCardZoomIn` but the alpha blend has two extra BLDY phases: BLDCNT `0x27A7` with BLDY ramping 0->0x1F (timer 16-47) then 0x1F->0 (timer 48-79), then the BLDALPHA fade-out from 0x68 |
| `0x08015720` | 0x30C | **matching** (wave 3, 2026-10-01; FAKEMATCH) | `DuelCmd_GridZoomIn3` (hyp.) | like `DuelCmd_ShowCardZoomIn` but once the timer passes 0x67 the grid is scaled by the curve `gScatterScaleCurve` (`*scale/256`) around (0x68, 0x40) instead of the `/32` zoom-in |
| `0x08015A2C` | 0x414 | matching | `DuelCmd_GridGrowFade` (hyp.) | step 5: rows grow from the top (`y*timer/16`, 16 frames) with a BLDALPHA fade-in, then `TextCellsClear`/`DuelInfo_DrawCardNameCentered(arg2)`; step 6 flashes the grid (`AddSprite8bpp`, 32 frames); step 7 shrinks it again (`y*(0x38-timer)/16`) with a fade-out; step 8 -> end |
| `0x08015E40` | 0x484 | matching (ordinary C) | `DuelCmd_GridSlideFade` (hyp.) | like `DuelCmd_ShowCardUnrollDown` but the grid slides in horizontally (`(x-0x68)*timer/16+0x68`), step 5 ends with SE 0x2C (`PlaySE`), step 6 flashes with BLDY (`0x04000054`, `0x1090`) |

Fast-forward (`gMain.heldKeys & 2` or `gDuelScreen` bit 0) skips the timer ahead by 3 or 7 frames.

## Structs and globals
Same as [[duel-cmd-turn-c]]: `gDuelCmd` `DuelCmd` (`arg2` +2, `step` +0x80A u16 bitfield:7, `timer` +0x80C bits 5-11 in a `u32` container, `running` +0x80D bit 5), `gDuelScreen.fast`, `gMain.heldKeys` (+4). `gPulseScaleCurve` is a u16 curve (hyp.). `gScatterScaleCurve` is a `const s32` zoom curve with `0x100` = 1.0 (shared with [[duel-cmd-turn-c]]). `DuelCmd_ShowCardEffect` and `DuelCmd_ShowCardScatter` index it at `(timer - 0x68)` and scale `pos = (pos - centre) * curve / 256 + centre`.

## Matching tricks
- **Don't keep the timer in a temporary when it is used once.** In `DuelCmd_ShowCardUnrollDown`, `if ((ta = timer) <= 0xF) y *= ta;` leaves `ta` in r2; `if (timer <= 0xF) { y *= timer; ...}` (re-reading the field, CSE merges the loads) puts it in r0 like the ROM. Keep the temp (`ta`) where the ROM has it in r2 (the step 7 fade).
- **`y *= t; y /= 16;` as two statements**, not `y = y * t / 16;` (the latter copies y into a new register before `muls`).
- In `DuelCmd_ShowCardUnrollDown` step 8 (`step++`) and the step 7 tail (`step++`) must appear in the order 7, 8 in the source; swapping them changes which scratch register the `0x80A` literal gets (r2/r0 versus the ROM's r1/r2).
- The `step++` tail after calls is cross-jumped by the compiler. Cases that happen to use the same scratch register share the tail, which is why only some `b` targets land on the common block.

> [!warning] Contradiction
> The next bullet (page text before 2026-10-01) says a `u32` timer container is needed for the signed compares. The wave 3 matches of `DuelCmd_ShowCardZoomIn` / `DuelCmd_ShowCardScatter` (2026-10-01, `build/wf/DuelCmd_ShowCardZoomIn/NOTES.md`, `build/wf/DuelCmd_ShowCardScatter/NOTES.md`) read the timer inside the step-6 grid loop through a **u16**-container view (`gDuelCmdT16`) with `(u8)` casts into `int` temporaries; the compares there are on the ints, and only the `td` compare after the loop still reads the `u32` field. Resolved in favour of the matched source: the `u32` container is needed where the field itself is compared, not for every read.

- A `u32` timer container is needed for the signed compares; the `gDuelCmd` base gets an extra copy register (`adds r7,r4,#0`) in this family, as in `DuelCmd_ShowCardAssemble`.

## Near-misses
The `0x080153D4` and `0x08015E40` entries below are superseded by the "Resolved" sections that follow them; the `0x080150DC` / `0x08015720` entries by [Wave 3 matches](#wave-3-matches-2026-10-0102).

- Historical (both matched in wave 3): `0x080150DC` / `0x08015720`: the C translation has the ROM's instruction stream, but two things differ. The ROM never CSEs the base address of `gDuelCmd` (cases 2-4 and the step++ tails reload the literal). In its step-6 loop the ROM hoists the two BLD register addresses, while the build hoists the timer address (`&gDuelCmd + 0x80C`) into r9 instead. In addition, `DuelCmd_ShowCardScatter`'s build keeps a base copy (`adds r5,r4,#0`) and spills `i*32`/`y0` where the ROM spills only `i*32`. Tried: pointer local, constant-cast access, explicit `(u32)&gDuelCmd + off` casts, `volatile`, break vs return, hoisted outer-loop locals, variants of the temporaries, case order. None changes it.
- `0x080153D4`: same skeleton but no base copy (build 0x334 vs target 0x34C, 0x18 short). Here the difference is pure cross-jumping. The ROM duplicates `ldr r2,=base; ldr r7,=0x80A; add r3,r2,r7` in cases 2,3,4 before branching to the increment body at `0x080156D6`, while old_agbcc merges that address computation and the `step++` read/modify/write into a single block that cases 2,3,4,7 branch to. A `goto inc_step` with `step_ptr = (u8*)((u32)(unsigned long long)&gDuelCmd + 0x80A)` (advisor suggestion, the permuter rematerialization trick) did **not** break the merge: the compiler CSEd the three assignments again, and for case 7 folded base+0x80A into the single literal `0x02018DCA`.
- `0x08015E40`: instruction-for-instruction identical except that the step 5 / 7 grid loops swap two
  loop-invariant registers. The ROM keeps the timer address `0x02018DCC` in **r7** and `y<<16` in **r6**,
  while the build always gives the hoisted `y<<16` r7 and the address r6 (and r5 for the tile term). Tried:
  shifted/unshifted `y` locals, `y <<= 16` two-step, `y` computed in the outer vs inner loop, `int`/`u32`
  `y`, `x`/`y` declaration order, `ta` timer temp, `!=` type tests. The allocator never puts the address
  in r7. The `y<<16` value must be hoisted as a local or the tile term `(i*2+1)<<5` stops hoisting.

### Sliding-grid lifetime refinement

The earlier parked `0x08015E40` candidate was the correct 0x484-byte size and
differs in only **four operand bytes**, all in case 6. The ROM keeps its packed
row coordinate in r7 and tile term in r6; C assigns those two values to r6/r7.
Cases 5 and 7 and every other instruction match.

Moving `y = (i*32+2)<<16` into the inner loop lets old_agbcc hoist it with the
ROM's outer-loop scheduling. Two empty inputs, one after the drawing call in
each of cases 5/7, keep the initialized row coordinate live through the call
and give the timer-address/row allocation recorded above. They preserve all
values and emit no instructions. The stronger draft remains disabled, and
the complete active unit still passes `unit bytes MATCH` (0x11E8).

Bounded failures: isolating case 6's row variable fixes that phase but
swaps the row/tile allocation in cases 5/7 instead; explicit fixed-register
locals interfere with other phases and increase code size. Declaration/type
order, additional initialized row/tile inputs, reused timer locals, and
read/write inputs did not resolve the last swap. The stable candidate and
variant results are in `build/decomp_large/game-batch/slide-best4.c` and the
`stable-*results.json` files. The older notes above describe the earlier
candidate and its wider scheduling differences.

### Separate row lifetimes in the slide phases (resolved)

`DuelCmd_ShowCardUnrollSideways` matches all **0x484 bytes in ordinary C**, with no asm
hints or fixed registers. The complete **0x11E8-byte unit** passes
`tools/dr python3 tools/check.py duel_cmd_presentation`; two of its five functions
are enabled C, and the three other functions still use assembly fallbacks.
Evidence: `build/bigguns-slide/accepted-check.log`.

Cases 5 and 7 each need their own `int rowY` declared in that case's scope.
Case 6 retains the function-level `y`. Keep the packed row calculation
inside each inner loop and remove both earlier empty lifetime constraints.
This gives cases 5/7 the timer address in r7 and row in r6, while case 6
gets the row in r7 and tile term in r6. Separating only case 6 had disturbed
the other two phases; separating both slide phases resolves all three
simultaneously. Both scoped variables are initialized on every use.

A private 37-candidate scope/lifetime grid reached the exact form; the
whole-unit result is `build/bigguns-slide/DuelCmd_ShowCardUnrollSideways/weights-57-0-0/check.txt`.
The source and evidence supersede the preceding four-byte near-miss and
the earlier claim that the empty inputs were necessary. No ABI, call,
branch behavior, timer update, or hardware access was changed.

### Shared coordinate and byte-range view in the three-phase zoom (resolved)

`DuelCmd_ShowCardEffect` matches all **0x34C bytes in ordinary C**. The full
unit stays exact at 0x11E8 (`build/bigguns-slide/accepted-153D4-check.log`),
which brings the enabled C count to three of five. The match uses no inline
assembly, fixed registers, signature changes or new data.

The old `dy` temporary prolonged a separate coordinate lifetime. Use
`y -= 0x40; y *= ta; y /= 32; y += 0x40;` in the zoom branch. This also
restores the previously mismatched switch-tail duplication and base loads
through the compiler's changed register allocation. The resulting draft
was 0x348 bytes, short only in the middle fade range.

Test that range with `(u32)((u8)tb - 0x10) <= 0x1F`, while retaining
`REG_BLDY = tb - 0x10`. This recovers the ROM's separate subtraction for
the store and all remaining operand choices. `tb` was loaded from the
7-bit timer and therefore lies in 0..127: the byte conversion preserves
its value. The next range test and the rest of the function are unchanged.
The decisive private candidate is
`build/bigguns-slide/DuelCmd_ShowCardEffect/range-cast-u8-cond/`.

These results supersede the earlier unresolved cross-jumping explanation
for this function; a partial fix to one block had hidden the effect of
coordinate lifetimes elsewhere in the same function.

### Follow-up bounds on the two remaining zoom drafts

Historical (both matched in wave 3, see below; the fix was loop.c's hoisting threshold, which none of these isolated changes touched).

The same pass tested separate loop/coordinate/timer scopes in
`DuelCmd_ShowCardZoomIn` and `DuelCmd_ShowCardScatter`; these did not change their generated
code. In `DuelCmd_ShowCardZoomIn`, merged coordinates, timer-width variants,
initialized lifetime constraints, and explicit blend-register pointers
(including safe callee-saved register constraints) did not produce an
exact match. The strongest baseline remains parked without replacing it
with a score-only candidate. Relevant finite-result logs are
`build/bigguns-slide/zoom-grid.log`, `zoom-expr.log`, `zoom-types.log`,
`zoom-bld.log`, and `zoom-table.log`. The successful `DuelCmd_ShowCardEffect` combination is not
evidence that an isolated coordinate or scope change fixes these two.

## Wave 3 matches (2026-10-01/02)

Both are FAKEMATCH. Working notes: `build/wf/DuelCmd_ShowCardZoomIn/NOTES.md`, `build/wf/DuelCmd_ShowCardScatter/NOTES.md`.

### `DuelCmd_ShowCardZoomIn` (0x2F8, start score 168; FAKEMATCH)

- With `int` types (the parked u16 `i`/`j` and u32 `x`/`y` gave unsigned division and u16 increments) the build hoisted the timer address `&gDuelCmd + 0x80C` out of the step-6 grid loop and kept the GCSE/PRE base copy of `&gDuelCmd` in r5. The ROM recomputes both in the loop (`ldr base; ldr 0x80C; add`).
- Diagnosis (`-dL` loop dump): the timer-address movable has savings 3 and life 3; with calls in the loop the threshold factor is 13, so the move is desirable when the loop has fewer than 13*3*3 = 117 insns. The second loop pass counted 106 insns in the inner loop; the ROM's loop must have had at least 118 at that point.
- FAKEMATCH: pad the loop's RTL with no-ops that combine deletes later. Read the timer through a u16-container bitfield view `gDuelCmdT16` (+2 insns per read), cast the three timer reads to `(u8)`, and use `(u8)tb` in the BLDALPHA value and `gPulseScaleCurve[(u8)tc]`. All are needed; removing any one gives 117 insns and the hoist returns.
- Coordinates in place (`x -= 0x68; y -= 0x40; x *= ta; y *= ta; x /= 32; y /= 32; x += 0x68; y += 0x40;`), as in `DuelCmd_ShowCardEffect`. A separate `dy` pseudo took r5 and pushed `j`/`i*32` out of the ROM's registers.
- Failed: int types with the `dy` temp (435), u8/s8/s16 temps (unsigned compares or leftover extensions), u16 `i` or `j`, `asm("")` padding (stops the hoist but stretches live ranges), s16/u16 `y`.

### `DuelCmd_ShowCardScatter` (0x30C, start score 226; FAKEMATCH)

- Same problem as `DuelCmd_ShowCardZoomIn` (timer address hoisted, base copy in r5), fixed by porting its `gDuelCmdT16` view and `(u8)` timer casts. Here the view plus either set of casts is enough; all were kept for consistency with the sibling.
- The scaled branch keeps separate `dx`/`dy` temps and an index temp: `dx = x - 0x68; dy = y - 0x40; k = ta - 0x68; dx *= gScatterScaleCurve[k]; dy *= gScatterScaleCurve[k]; dx /= 256; dy /= 256; x = dx + 0x68; y = dy + 0x40;`.
- Failed index forms: `tbl[ta - 0x68]` loads the table base first and wrecks allocation (308); `*(tbl + (ta - 0x68))` or `(ta - 0x68)[tbl]` fold the `-0x1A0` into the literal (27); `ta -= 0x68` (2).
