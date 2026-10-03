---
title: Unit dice_scene (dice step runners, splash sequence on 0x02020310, reel helpers)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit dice_scene

`0x08026124`–`0x0802757F`, Thumb, `old_agbcc -O2`. Source: `src/dice_scene.c`.

Unit status: `unit bytes MATCH`, **26/26 functions in C**, no `INCLUDE_ASM` remaining (verified 2026-10-01 with `tools/dr python3 tools/check.py dice_scene`). `DestinyBoardScene_DrawFinalLetters` matches all `0x6FC` bytes using annotated, behavior-neutral FAKEMATCH register bindings and empty constraints.

The unit has three parts:
- Two step runners for the dice-roll screen of [[coin-toss-scene-c]] (`gDiceScreenGracefulSteps` / `gDiceScreenPlainSteps`, both `{0x08026030, 0x080254C8, …, 0x08025F50}`).
- A 9-step sequence (runner `ExodiaScene_Run`, table `gExodiaSceneSteps`) on the work area `0x02020310`: five sprites fly in to (0x68, 0x40), a zoom/fade, then three BG layers with an HBlank sine wave on BG0; B (`gMain.newKeys & 2`) or the end of the fade exits. Hypothesis: a splash / title-like sequence. It is reached from `CB_DebugExodiaScene`.
- Helpers for the slot-reel screen of [[destiny-board-scene-c]] (reels at `0x02020310+0xAB4`, reel-stop markers).

## Functions

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x08026124` | 0x38 | matching | | Step runner over `gDiceScreenGracefulSteps[0x02017A30+0xB]` (returns 1 at the NULL end) |
| `0x0802615C` | 0x38 | matching | | Same over `gDiceScreenPlainSteps` |
| `0x08026194` | 0x14 | matching | | `work+0xAAC = 0` (takes an unused pointer argument) |
| `0x080261A8` | 0x38 | matching | | Starts the 5 movers (`LineInit`) from `gExodiaPieceStartPos` towards (0x68, 0x40) |
| `0x080261E0` | 0x40 | matching | `HBlank_SplashWave` (hyp.) | HBlank: `BG0HOFS = sin[(VCOUNT + work.B24) & 0xFF] >> 5` |
| `0x08026220` | 0x9C | matching | | Draws a sprite group as affine OBJs: attr0 bit 8 set, attr1 `& 0xC1FF`, attr2 tile bits 4-7 → 5-8, `+0x200`, priority from the argument |
| `0x080262BC` | 0xCC | matching | | Step 0: clear the 0x1738-byte work, `gMain+0x40E = 1`, BG1-3 scroll 0, blend 0x3F3F/0x808/0x10 |
| `0x08026388` | 0x28 | matching | | Copies 4 rows of 0x80 bytes (dst stride 0x400) with `MemCopy16` |
| `0x080263B0` | 0x18 | matching | | `CopyObjTileBlock4x4(src + srcTile*32, 0x06014000 + tile*32)` (4th argument unused) |
| `0x080263C8` | 0x10C | matching | | Step 1: load graphics (`gMillenniumEyeBitmap`, `gMillenniumEyePal`, …), 8 OBJ tile blocks, `DISPCNT = 0x1F04`, timer 60 |
| `0x080264D4` | 0x1D0 | matching | | Step 5: load BG0-2 graphics and maps, `BGxCNT`, palette ramp (`PalFade_Start`), install HBlank `ExodiaScene_HBlank` |
| `0x080266A4` | 0x240 | matching | | Step 3: 4-state animation on `work+0xAAC` (fade in, reverse, wait, draw `count` copies, SE 0x13/0x14) |
| `0x080268E4` | 0x118 | matching | | Step 4: zoom (`sin`), 5 movers (`LineStep`) drawn with templates `gExodiaPieceOamTemplates[i]`, SE 0x15 at the end |
| `0x080269FC` | 0xE4 | matching | | Per-frame draw: group 0 (`OamListAddSpriteGroup`), group 1 (`ExodiaScene_DrawAffineAnim`), wave phase `B24/B25`, affine scale `0x80 + mul(0x90, 0x100 - sin[B16+0x40])` |
| `0x08026AE0` | 0xC8 | matching | | Step 8: fade, palette ramp while `B16 >= 0x50`, exit on B or fade end (clears the HBlank IRQ) |
| `0x08026BA8` | 0x28 | matching | | Step 2: wait for the timer at `+0xB20` |
| `0x08026BD0` | 0x68 | matching | | Step 6: wait for fade state 3, then fade out (`FadeStart(1, -0x40, 0)`), `BLDCNT 0x3F41` |
| `0x08026C38` | 0x58 | matching | | Step 7: `BLDALPHA = (blend >> 8) \| 0x800` until blend ≤ 0xA00 |
| `0x08026C90` | 0x38 | matching | | Step runner over `gExodiaSceneSteps` |
| `0x08026CC8` | 0x68 | matching | | HBlank: BG0/BG3 HOFS wave from `gDestinyBoardWaveTable` (reel screen, `+0xB06`) |
| `0x08026D30` | 0x30 | matching | | Every 9 frames `work+0xB06++` |
| `0x08026D60` | 0x24 | matching | `Reel_Init` (hyp.) | `Reel_Init(src, dst, speed, target, reel)` |
| `0x08026D84` | 0x44 | matching | `Reel_Move` (hyp.) | `Reel_Move`: `pos += speed`, stop at `target` |
| `0x08026DC8` | 0x48 | matching | `Reel_Draw` (hyp.) | `Reel_Draw`: copy the newly exposed tile row (`CopyMapRect`) when `(pos>>4)/8` changes |
| `0x08026E10` | 0x74 | matching | | `BGnHOFS = 0`, `BGnVOFS = reels[n].pos >> 4` |
| `0x08026E84` | 0x6FC | matching (C, FAKEMATCH) | `DrawReelStopMarkers` (hyp.) | Five reel-stop markers at (x, y): pulse (affine scale from `counters[i].pos`), then fly along a sine arc (per-marker radius/speed/end values), 2 sprites each from `work+0x954` templates |

## Data

- `0x02020310` work area (0x1738 bytes, cleared by `ExodiaScene_Init`). Different screens overlay it:
  - `+0x000` OAM buffer (`OamListFlush` / `OamListClear`), `+0x618` 32 × 0x18 affine sets (`ObjAffineInit`, `ObjAffineApply`), `+0x918` 5 × 0x14 sprite groups `{u32; u16 *templates; u16 x, y; u8 count, +D, +E, +F, +0x10}`, `+0xAA8` u16 group count, `+0xAAC` step state.
  - splash: `+0xAB0` 5 movers (0x14 each, `{u16 x, y, …}`), `+0xB14..B16` (B16 = sine phase), `+0xB18` fade object (`+2` u16 blend, `+4` s16 speed, `+6` state), `+0xB20` timer `{u8 state; u16 time}`, `+0xB24/B25` wave phase, `+0xB28` palette ramp, `+0x1728`, `+0x1734/1735` last group state.
  - reel screen: `+0xAB4` 4 reels (0x14: `state:3`, u16 speed/target/pos 12.4, s8 row, `u8 *src`, `u8 *dst`), `+0xB06/B07`, `+0xB0C` 5 counters `{u8 pos, u8 step, …}`.
- `gSineTable` s16 sine table (256 entries, 0x100 = 1.0).

## Matching tricks

- `ExodiaScene_FadeInFlames` / `ExodiaScene_BlendFlames` / `ExodiaScene_Finale`: `if (cond) { ...; return 1; } other; return 0;` gives the ROM's "other first" layout.
- `ExodiaScene_BlendFlames`: `REG_BLDALPHA = ((blend = gWork.fade.blend << 16) >> 24) | 0x800; if (blend <= 0xA000000)` is the only form found that loads `REG_BLDALPHA` first and keeps the shared `<< 16`.
- `ExodiaScene_AssemblePieces`: `speed *= -1` (with `s16 speed`) gives `ldrsh; neg`; `speed = -speed` drops the sign extension.
- `ScrollLayer_Move`: reel fields must be `u16` with `(s16)` casts in the compares; `s16` fields load with `ldrsh`.
- `ScrollLayer_StreamRow`: `int old = row; int y = (s16)pos >> 4; if (old != y / 8)` (separate `y`) gives the `add r1, r0, #0` copy of the division.
- `ExodiaScene_DrawSprites`: `x++; if (x & 2)` reuses the constant 2 from an argument; `if (++x & 2)` adds an `& 0xFF`.
- `ExodiaScene_GatherPieces`: template pointer to an 8-byte struct, `f(tmpl++, ...)` puts the increment before the call.
- `ExodiaScene_LoadFlames`: two literal-pool words are 0; they match as `extern u8 gUnkA_00000000[], gUnkB_00000000[]` (distinct names so the pool keeps two entries).
- `DestinyBoardScene_DrawFinalLetters`: the first `switch (i)` must have 4+ separate case labels (1, 2, 3, 4 with identical bodies) to get the jump table; `case 0` uses constant index 0. `pos * 3 / 2` (not `>> 1`) gives `r3 + (r3 << 1)`. `a = b = v` stores `b` first. The sine-arc terms are inline calls; left operand is called first, so the ROM's "B − A with A first" is `-(A >> 4) + (B >> 4)`. Local order `affine, dx1, dy1, dy2, dx2` gives the ROM stack slots; the final matching version widens `dy1` and `dx2` to `u32` (details below).

## Reel-marker refinement (2026-09-30)

This section records the work done before the complete match described under "Complete match" below. At that point `DestinyBoardScene_DrawFinalLetters` was still **assembly** and the active whole unit was **25/26 C**, `0x145C` bytes. The stronger draft was parked under `#if 0`, with the original `INCLUDE_ASM` intact. Its isolated full-unit compile produced the exact `0x6FC` function size, but **96 bytes differed** (first difference `+0x13A`), so it was not yet a matching C conversion.

- The ROM first packs each `u16` arc coordinate with `<< 16`, adds `0x200000` or `0x100000`, then shifts back by 16. Writing the calculation as `(((u32)dx2 << 16) + 0x200000) >> 16` reproduces that sequence while preserving wrapping behavior. Straight `dx2 + 0x20` instead adds before narrowing.
- Explicit byte offsets `i * 8 + 8` and `i * 8 + 0x30` reproduce the two template addresses. The initialized data address `extern struct SpriteGroup gFinalLettersAnim` is exactly `&gWork.grp[3]`, and using this separate symbol reproduces the draw group's independent base load. It adds no object or ABI change.
- The draft had an arithmetic error in marker 4's Y arc. Both fixed-point helper results shift by **8**, followed by second minus first, but the earlier draft shifted the first result by 4. The authoritative sequence is the two `asr #8` instructions after marker 4's Y helper calls in `asm/nonmatching/dice_scene/DestinyBoardScene_DrawFinalLetters.s`.
- The remaining differences are in register allocation. C holds the work base in `r9` and the retained X coordinate in `sl`, while the ROM uses the reverse. The packed-coordinate scratch registers and the count-zero copy through `r9` also differ. The stack frame and all coordinate stack slots match. This diagnosis does not validate the disabled routine's default counter states, but the existing fallback preserves the ROM for every input.

New bounded grids tried declaration order, signedness and width, separate arc scopes, explicit work pointers, named packed coordinates, zero-assignment forms and initialized empty constraints. None beat the ordinary-C draft. Explicit high-register bindings often changed the size substantially and were rejected. Unbound high-register read/write forms produced compiler errors and were rejected. These grids need not be repeated unchanged; their sources and results are under `build/decomp_large/game-batch/DestinyBoardScene_DrawFinalLetters` and `build/decomp_large/reel-*.py`.

One three-minute, single-worker [[decomp-permuter]] register-allocation search completed 3,684 iterations with 237 compile errors. Its best heuristic score was 1,310 (base 1,465), but it introduced an unset `new_var` on the skipped-draw path, so it was rejected without enabling or parking that output. Heuristic scores are separate from the byte count above. A fresh compile of the original draft gave `0x708` with a byte-plus-size-penalty score of 1,331, so the old source comment claiming a 12-byte deficit was stale.

Evidence: `build/decomp_large/game-batch/DestinyBoardScene_DrawFinalLetters/draw-group-symbol/check.txt`, `unit.s`, `text.bin`, and `build/decomp_large/game-batch/reel-base.diff`; search log `build/decomp_large/reel-permuter.log`. See [[matching-tricks]] for the grouped compiler and address-expression patterns. Full-unit byte validation remains the acceptance gate.

A private test applied the narrow parameter types of the now byte-verified `OamListAddSpriteGroup` callee to the reel draft. It grew the marker function to 0x700 and did not match (298 normalized diff lines), so no shared prototype or source change was enabled. Evidence: `build/bigguns-lead2/reel_abi.py`, `DestinyBoardScene_DrawFinalLetters/solo-verified-abi/`.

## Register refinement (2026-10-01)

The starting draft reproduces `0x6FC`, with **176 normalized diff lines / 96 differing bytes**. The active assembly fallback still matches. The trials use the same `check.py` compiler and linker with scratch candidate sources, leaving the active unit intact. Scratch notes and probe definitions are in `build/codex-scratch/codex-dice_scene/`.

- A 30-minute, two-worker permuter run started from the parked draft, under the policy that accepts behavior-neutral, annotated FAKEMATCHes.
- Splitting the zero-counter chain around an initialized empty `asm("" : "+r"(dx2))` only in marker 2 gives `0x6FC`, 142 normalized diff lines / 83 differing bytes. Marker 4 alone gives 150 lines / 85 bytes; all five gives `0x708` and is worse. These are scratch candidates, not accepted conversions.
- Empty `+h` constraints on unbound `u16` zero-case locals trigger old_agbcc internal compiler errors. The first draw-scope alias probes had a C89 declaration-placement error and must not be interpreted as evidence about high-register support.
- An input-only empty constraint in marker 2's zero case improves to `0x6FC`, **126 normalized diff lines / 71 differing bytes**, and fixes the `r9`/`sl` roles. Applying it in all five zero cases grows the function to `0x708`.
- Named packed-coordinate values with bindings to `r1` (packed X), `r2` (0x200000), and `r3` (0x100000), plus initialized empty read/write constraints, reproduce arcs 0–3 exactly. Marker 4 currently pins only the small offset in `r1`. Combined with the marker-2 input constraint, the scratch candidate reaches **30 normalized diff lines / 19 differing bytes**, exact `0x6FC`; the residual is four zero-copy instructions and marker 4's packing. Evidence: `build/codex-scratch/codex-dice_scene/review-best.c`, `review-pinsubset.py`, `review-pinsubset.log`.
- Pinning all three values in marker 4 changes layout and shrinks the function to `0x6B8`; distinct immediate operands on empty constraints did not fix that. Late draw-only aliases and broad zero-case register bindings also failed to improve the candidate.
- Marker 4 is resolved. The packing itself was correct with all three pins, but its following affine constant and position test selected `r0`/`r1`, allowing a different shared tail. Scoped `u32` input constraints for the affine constant in `r2` and loaded position in `r3` restore the exact layout. The combined scratch candidate now has **8 normalized diff lines / 8 differing bytes**, all four zero rematerializations; exact `0x6FC`. Evidence: `build/codex-scratch/codex-dice_scene/review-best8.c`, `review-lasttail.log`.

### Complete match

The final fix is `u32 dy1` and `register u32 dx2 asm("r9")`, keeping their original declaration positions and explicitly casting each initial X-arc result to `u16`. Both coordinates still carry 16-bit values: zero or an unsigned packed value shifted right by 16. The wider `dy1` destination lets the ordinary chained zero assignment copy from `r9` instead of rematerializing zero. No zero-case empty constraint is needed in the accepted source.

All register bindings, width choices, and empty constraints are annotated `/* FAKEMATCH: why */` in C. They contain no hand-written assembly instructions. The initial permuter run (30-minute budget, two workers) was stopped after the manual constrained candidate matched; its best observed heuristic score was 1310, not a byte match. The zero-case masks, late aliases, low-register zero temporaries, explicit cast/barrier variants, and redundant branch/scope forms tested along the way are recorded in `build/codex-scratch/codex-dice_scene/probe*.log` and `review-*.log`.

Acceptance: after applying the annotated C and removing the fallback, `tools/dr python3 tools/check.py dice_scene` reports **26/26 functions match; unit bytes MATCH (built 0x145C vs target 0x145C)**. The last function remains at `0x08026E84`, size `0x6FC`.
