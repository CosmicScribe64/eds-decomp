---
title: Unit destiny_board_scene (reel / roulette screen at 0x02020310, scroller helpers)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit destiny_board_scene

The unit covers `0x08027580`–`0x08028683` (Thumb, `old_agbcc -O2`), and its source is `src/destiny_board_scene.c`.

Unit status: `unit bytes MATCH`, **17/17 functions in C** after workflow wave 2 (2026-10-01: `0x080283BC` in wave 2); none stay `INCLUDE_ASM`. Before wave 2: 16/17 (wave 1 on 2026-10-01 added `TurnOrder_DrawBanner`). Before wave 1: 15/17 (`0xD94` bytes in C, `0x370` in asm). Verified with `tools/check.py destiny_board_scene` (0x1104 bytes).

It is a self-contained screen on the work area `0x02020310` (0xB24 bytes). Three to four objects rotate on a drum or wheel (sine table `gSineTable`, per-object OBJ affine sets at `+0x618`). A hand/arm sprite pair swings in, a counter ticks every 20 frames with SE 0x2F, and there are sprite-group animations with SE 0x2D/0x2E. It uses an HBlank handler (`DestinyBoardScene_HBlank`) and a VBlank callback (`DestinyBoardScene_VBlank`). Hypothesis: a roulette/slot-style card effect screen. The step table is `gDestinyBoardSceneSteps` (runner `DestinyBoardScene_Run`). `Scroller_Move/CDC/D1C` are generic 4-byte "scroller" helpers that [[duel-field-view-c]] calls on the toss work area `+0xB14`.

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x08027580` | 0xA0 | matching | Every 21 frames (`timer` 20→0xFF), for ticks 0–5: bump `counters[gFinalLetterLaunchOrder[tick]].count` (SE 0x2F). When `counters[2].pos == 0x60`, it calls `FadeStart(0, 0x60, 0, +0xAAC)` and clears DISPCNT bit 8 | |
| `0x08027620` | 0x50 | matching | `timer = 20`, clear 5 counters, `tick = 0` | |
| `0x08027670` | 0xE4 | matching | Init: CpuSet-fill the work area (0x592 halfwords), zero BG0–3 offsets and BG2X/BG3Y halves, `AnimBlockInit(gDestinyBoardAnimList, grp[0])`, 4 × `ScrollLayer_Init(init[i]…, &objs[i])`, `unkB05 = 0x62` | `RouletteScreen_Init` (hypothesis) |
| `0x08027754` | 0x258 | matching | Load: clear VRAM, tilemaps (`CopyMapRect`), 8 small maps, BG/OBJ tiles and palettes, BGxCNT, `DISPCNT = 0x1F00`, VBlank callback `DestinyBoardScene_VBlank`, HBlank handler `DestinyBoardScene_HBlank` (IE bit 1), BLDCNT 0x3F41 / BLDALPHA 0x100B, BGM 20 | `RouletteScreen_Load` |
| `0x080279AC` | 0x288 | matching | Per-frame update: objects (`ScrollLayer_Move/DC8`), a sprite-group state machine at `+0xB04` (0→1→2→3, SE 0x2D/0x2E, a graphics swap at frame 0xA00), then affine updates for 5 sets. Returns 1 when `+0xAB2 == 2` | `RouletteScreen_Update` |
| `0x08027C34` | 0x24 | matching | Disable the HBlank IRQ (`IME=0; IE &= ~2; IME=1`), return 1 | |
| `0x08027C58` | 0x38 | matching | Step runner over `gDestinyBoardSceneSteps[0x02017A30.step]` | |
| `0x08027C90` / `0x08027CA4` | 0x14 | matching | `grp[0].ctl = 1` / `0xFF` | |
| `0x08027CB8` | 0x24 | matching | 4 scrollers: `pos += speed` | `Scroller_Move` |
| `0x08027CDC` | 0x40 | matching | Snap a scroller to the stop `gHandCarouselStops[i]` (3 stops) when `pos` is within `|speed|` past it: `speed = 0, pos = stop, stop index = i` | `Scroller_Snap` |
| `0x08027D1C` | 0x18 | matching | Stop a scroller at `pos` 0x30 or 0 | `Scroller_Clamp` |
| `0x08027D34` | 0x39C | **matching** (ordinary C) | Draw 3 drum items at angles `angle+spread`, `angle+(0xFF−spread)` and `angle`. The x/y come from the sine table (`0x58 + 0x30·sin`, `0x32 + 0x10·cos + dy`); an item is in front when `0x40 ≤ a < 0xC0` or `flags&1`. Each item gets affine set 1/2/0 with scale `0xC0 + cos/4`. Coordinate staging and function-scope initialized sprite extents recover the ROM's argument setup | `Drum_Draw` |
| `0x080280D0` | 0xA8 | **matching** (wave 1, 2026-10-01) | Draw a 2-part 64×32 banner (`gTurnOrderBannerTileNums[idx*2+i]`, y 0x2C/0x6E), H-flip (0x400) when `flags & 4`. Ordinary C; see [Banner matched](#banner-matched-wave-1-2026-10-01) | |
| `0x08028178` | 0xC0 | matching | Draw a 32×64 sprite at y `0xC0 + t·0x160/256` with affine set 3 | |
| `0x08028238` | 0x184 | matching | Draw the two hand sprites bobbing on `sin(angle*2)·scale[k]`, with affine sets 4/5 | |
| `0x080283BC` | 0x2C8 | **matching** (wave 2, 2026-10-01) | Hand swing: decay `scale[0/2]` by 0x80, and draw 2 sprites per `sel` with the `anim[0]` cosine swing and the `anim[1]²/2` drop. Affine set 4 angle `±anim[1]<<9`. Ordinary C; see [Wave 2 matches](#wave-2-matches-2026-10-01) | |

## Data (`0x02020310`, `struct Work20310`)

- `+0x618` `aff[6]`: 24-byte OBJ affine parameter sets `{u16 scaleX, scaleY, angle; …}`, fed to `ObjAffineInit` / `ObjAffineApply`. OAM attr1 bits 9–13 select them (`0x02000000` = set 1, `0x04000000` = set 2, `0x06000000` = 3, `0x08000000` = 4, `0x0A000000` = 5).
- `+0x918` `grp[5]`: 0x14-byte sprite-group animators (`AnimBlockInit`, `AnimBlockTick`, `AnimBlockDraw`). `+0xC` is the current frame word (compared under the mask `0xFF00FF00`), `+0xE` is the control byte (1 play, 0 finished, 0xFF stopped).
- `+0xAAC` object for `FadeStart/FadeTick` (`+6 == 2` means the screen is done). `+0xAB4` `objs[4]` of 0x14 bytes (`+0` bits 0–2 state, `+2` s16, `+6` s16 position in 12.4). `+0xB04` state, `+0xB05` countdown, `+0xB08` timer, `+0xB09` tick, `+0xB0C` `counters[5]` (4 bytes), `+0xB20` delay.
- Scroller (4 bytes): `u8 pos; s8 speed; u8 stop; u8`.

## Historical tool issue (resolved)

`tools/check.py` `baseline_syms()` does `v & ~1` on **every** symbol from `build/eds.elf`, data included. The odd-addressed rodata label `gHandCardPalNums` (a u8 table, `data/rodata_08080A20.s`) therefore gets linked at `0x08082702` in per-unit checks. This affects `TurnOrder_DrawOpponentCard` here and `TurnOrder_RpsMain` in turn_order_steps, even when they are pure asm (verified, because the unit made entirely of `INCLUDE_ASM` also reports this DIFF). The full `make compare` build is not affected. The fix is to clear bit 0 only for `FUNC` symbols.

## Matching tricks

- **Locals initialised with constants are often *not* constant-propagated by old_agbcc.** `s32 w = 0x20, h = 0x40;` produces the ROM's `mov r6,#0x20; mov r8,…` held across calls (`TurnOrder_DrawOpponentCard`, and likewise `TurnOrder_DrawTurnChoice` / `TurnOrder_DrawBanner`). Which uses get folded is hard to predict, though (`TurnOrder_DrawTurnChoiceConfirm`).
- **`(s8)field == 0` on a u8 field** gives the ROM's odd `ldrb` + `ldsb` pair (`DestinyBoardScene_Update`). With an `s8` field, `|= 0xFF` becomes `= -1`, which breaks `DestinyBoardScene_Load`.
- **`s32 pos = s->pos;` then signed compares** (`blt`), and `u8 i = 0` declared before the `pos` load, reproduce the ROM's order (`Scroller_SnapToStop`).

> [!warning] Contradiction
> The next bullet (page text before 2026-10-01) gives `attr = *oam; *oam = attr | (c ? A : B);` as the read-before-test form. The wave 2 match of `TurnOrder_DrawTurnChoiceConfirm` (2026-10-01, `build/wf/TurnOrder_DrawTurnChoiceConfirm/NOTES.md`) needed `*oam |= c ? A : B;` at all four OAM sites: both forms load before the test and differ only in register choice (the shared `attr` variable collects many refs and outranks `oam` in global-alloc). Resolved in favour of the matched source: try both forms.

- **Read-before-test OAM OR:** `attr = *oam; *oam = attr | (c ? A : B);`, and `attr = *oam; if (f) *oam = attr | X;`.
- Pure-constant BG/VRAM setup functions (`DestinyBoardScene_Init`, `DestinyBoardScene_Load`) matched on the first try. Use `vu16` / `vu32` for the CpuSet/CpuFastSet fill source.

The source also contains the verified matching C for `DestinyBoardScene_LaunchLetters` and `TurnOrder_DrawTurnChoice`. Status counts and function-table rows above reflect those recovered matches.

`TurnOrder_DrawTurnChoice` keeps the second call's computed y, loaded tile and x in separate initialized locals. An empty input constraint on x preserves `mov r2,#0x88` after the tile load and before stack arguments. This documented FAKEMATCH hint emits no instructions and retains the original five-argument ABI, including the unused first parameter.

### `TurnOrder_DrawHandCarousel` acceptance

Accepted one ordinary-C function (`0x39C` bytes) with no fixed registers or empty compiler constraints. The complete unit is **`0x1104` bytes MATCH**, with 15/17 functions in C (`0xD94` bytes) plus two assembly fallbacks (`0x370`). Evidence: `build/decomp_large/drum-accepted-check.log` and isolated candidate `build/decomp_large/game-batch/TurnOrder_DrawHandCarousel/extent-function-initialized-int/check.txt`.

The initial scout matched size and missed 18 operand bytes. Preserve the signed Y calculation as separate assignments inside the fourth call argument: `(y = MulFix8(...), y >>= 8, y += 0x32, y += dy[i])`. This makes the return-value copy happen before the signed shift and removes twelve bytes of mismatch across the three draws. Direct `call >> 8` before assigning y reverses that order.

For the final six bytes, declare genuine sprite extents once at function scope, **`s32 width = 0x20, height = 0x40`**, and pass them to all three OAM calls. This recovers the ROM's r2 use for the first draw's height and second/third draws' width. Per-call assignments, per-call initialized locals and varying their narrow types all retain the six-byte miss. The matching width/height locals are ordinary C values, initialized on every path, and no ABI or helper argument changes are involved.

ROM review checked the seven-argument function signature and stack narrowing, wrapped byte bump offsets, signed modulo-by-256 sequences on promoted angles, signed sine-table loads, front/back OAM masks, repeated affine scale calls and halfword stores. Whole-unit exact bytes are the acceptance criterion; neither normalized instruction similarity nor isolated heuristic scores were counted as conversion. Private bounded scripts: `drum_y_shapes.py` and `drum_extent_shapes.py`.

The historical odd-data-symbol issue described above is resolved in the current `tools/check.py`, where `baseline_syms()` clears bit zero only when the ELF type is FUNC and the address is odd. The current exact unit check includes the odd-addressed data table used by `TurnOrder_DrawOpponentCard`, and no tool change was made during this pass.

### Bounded renderer sibling refinement

Historical (both functions matched in waves 1-2: `TurnOrder_DrawBanner` in wave 1, `TurnOrder_DrawTurnChoiceConfirm` in wave 2).

After 27D34 acceptance, the two remaining functions received a focused paired pass, and no further functions were enabled. Actual C count stays 15/17. Ordinary function-scope sprite extents restore both target sizes: 280D0 `0xA8` with 53 differing bytes; 283BC `0x2C8` with 50. In the hand renderer, the extents also keep y0 live in sl and reproduce the ROM's 0x28 frame/angle spill, resolving the old four-byte deficit. The ordinary hand reference is now parked with its assembly fallback intact. The private initialized OAM-word r2 binding reaches 42 differing bytes, but remains unaccepted; selector/X bindings, separate case-0 locals, literal-table scopes, arithmetic staging, dependency and declaration-order variants do not settle the function. Evidence: `build/decomp_large/game-batch/TurnOrder_DrawTurnChoiceConfirm/angle-u32-u16/`, `hand-lifetime-attr-r2/`; grids `drum_sibling_shapes.py`, `hand_shapes.py`, `hand_lifetimes.py`, `hand_x_scope.py`.

ROM review also found a source-level issue in the old hand draft: `anim[1] << 9` shifts a potentially negative s16. The actual angle stores use **ldrh**, shift and optional negation before the halfword write. The corrected reference uses `((u32)(u16)anim[1]) << 9` and its unsigned negation, preserving all low 16 output bits with defined C arithmetic. Both u16 and u32/u16 forms produce the same private 50-byte miss; all reads used for squared drop positions remain signed. The fixed-point callee's own ROM normalization returns an s16 value in r0; existing caller prototypes were retained to avoid adding caller-side extensions.

Historical (superseded by the wave 1 match below): the older banner frontier was reproduced in current unit context: **two differing bytes** at exact `0xA8`, with the seventh call argument's #4 move/store using r0 instead of r7. It has initialized table/mask/X/Y hints and is only a private reference (`old-frontier-current-context/`). Eight fresh direct argument statement-expression and shared narrow draw-mode forms do not fix it. The old failed four-argument register/clobber/lifetime grids were inspected and not rerun. Original banner draft/fallback is intact. Its 53-byte ordinary extent result is not an improvement over the two-byte private hinted frontier. Scripts: `banner_old_frontier.py`, `banner_mode_shapes.py`.

## Banner matched (wave 1, 2026-10-01)

`TurnOrder_DrawBanner` (0xA8, start score 32) matches in ordinary C, with no FAKEMATCH. Working notes: `build/wf/TurnOrder_DrawBanner/NOTES.md`.

- `u8` loop counter and `u16 tile` (the old `u32 i` strength-reduced `i*w`; `s16 tile` added a sign extension).
- Constants in function-level locals (`x0 = 0x38`, `y0 = 0x6E`, `pal = 4`, `h = 0x20`, `w = 0x40`). The loop label hides their values from CSE, so `y -= 0x42` stays a `sub`; since x0/y0/pal get no hard register, reload rematerializes them (`mov r7,#0x38`, `mov r3,#0x6E`, `mov r7,#4`).
- An explicit `tbl = gTurnOrderBannerTileNums` assignment between `i = 0` and `hflip = flags & 4` gives the prologue order (sl = tbl before the hflip AND).
- `h = 0x20` as a variable: its store needs a reload, which advances reload's round-robin register (`last_spill_reg` over spill regs [r0, r3, r7]) so the next reload (pal from `[sp+8]`) lands in r7 rather than r0. That fixed the last four lines, the seventh-argument `#4` move the older frontier could not place.
- Failed: hflip computed inside the loop (more loop insns, so loop.c no longer hoists the `gTurnOrderBannerPalNums+idx` pointer); `four` declared inside the loop (hoisted into r9).

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/TurnOrder_DrawTurnChoiceConfirm/NOTES.md`.

### `TurnOrder_DrawTurnChoiceConfirm` (0x2C8, start score 92; ordinary C)

Three experiments, each guided by the `.greg` priority list:

1. Selector / zero-constant swap (r4/r5) and the case-0 x shape: the draft wrote `x = A - 0x50; x = x0 + x;` (one pseudo set twice) and the case-1 x inline, where `x0 - (A - 0x10)` folds at tree level to `(x0 + 0x10) - A`. An offset temp, `t = (call >> 8) - 0x50; x = x0 + t;` (and `t = ... - 0x10; x1 = x0 - t;` in case 1), gives the ROM's `sub r0, #..; mov r1, r9; add r7, r1, r0` (92 -> 48).
2. One `x` shared by both cases had 4 refs over a live length of 75 and beat the case-0 table pointer for r5. A separate variable per case (`x`, `x1`; 2 refs each) drops below the table pointer: table r5 / x r7 in case 0, table r4 / x r5 in case 1 (48 -> 32).
3. `oam`/`attr` swapped (r2/r1 vs ROM r1/r2) at all four OAM sites: `attr = *oam; *oam = attr | (c ? A : B);` gives `attr` 28 refs (fold distributes the `?:`, so each arm has its own IOR) and it outranks `oam`. `*oam |= c ? A : B;` (fold wraps `*oam` in a SAVE_EXPR, a per-site temp) lets `oam` win r1 (32 -> 0).

The function-scope `s32 width = 0x40, height = 0x20` extents and the defined `((u32)(u16)anim[1]) << 9` angle from the earlier passes (above) stay in the final source.
