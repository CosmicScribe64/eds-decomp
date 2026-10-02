---
title: Unit code_08040EBC (duel target-selection prompts, part 2)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit code_08040EBC

`0x08040EBC`-`0x08041F9B`, Thumb, `old_agbcc -O2`. Source: `src/code_08040EBC.c`. Continues [[code-0803fe70]] (same "selector" family: `int f(struct CardRef *ref)`, step byte `0x02017A40 + 0x3E5`, prompt text via `sub_080602A4(0x206, 0x712, 0xB, text)`, cursor at `gUnk_0201CFB0 + 0x824/0x828/0x82C`, `sub_0803DDAC(ref, player, zone)` adds the picked zone, `sub_08077AEC(3)` plays the "cannot pick" sound, and the cancel bit is `gMain+6 & 2`). The last function is a different kind (a validity test that fills a `CardRef`).

Unit status: `unit bytes MATCH` (0x10E0 bytes), **13/14 functions in C** after workflow wave 1 (2026-10-01, `sub_0804112C` added); `sub_08041898` stays `INCLUDE_ASM` (best attempt under `#if 0`). Rechecked with `tools/check.py code_08040EBC` on 2026-09-30 after enabling `sub_0804158C`.

| Address | Size | Status | Purpose (hypotheses) |
|---|---|---|---|
| `0x08040EBC` | 0xF4 | matching | prompt = name of card `gUnk_086248EE[0]` (`sub_080753F4`, `gUnk_080849B4`); keys `0xE0`; the cursor zone must hold a face-down card (`flags6 & 2`) whose card number is 0x57D -> `sub_0803DE40` |
| `0x08040FB0` | 0x17C | matching | 2 steps: prompt `gUnk_080849F4`; keys `0xF0`, `sub_0802C3C0(ref, w)` accepts the cursor zone -> sound 1, message 8/0x8008, `sub_08017FF4(p, w)`, then the monster level of the card (`CARD_LEVEL`) + 1 is added as a target (`sub_0803DD7C`) |
| `0x0804112C` | 0x160 | **matching** (wave 1, 2026-10-01) | 4 steps: prompts `gUnk_08084A30` / `gUnk_08084A68`, `sub_0805304C()` (input poll), needs `w828 == 0`, `sub_08008940(p, w82C)`; step 3 also needs an empty zone (`ZB(..)->card == 0`) different from `ref->targets[0]`; picks with `sub_0803DE40`. Matched by steering cross-jumping with per-case returns (see below) |
| `0x0804128C` | 0x110 | matching | step 0: at least one opponent zone 0-4 holds a face-down card (`(flags6 & 3) == 1`), else return 1; prompt `gUnk_08084AA8`; step 1: keys `0x900000` -> `sub_0803DDAC` |
| `0x0804139C` | 0xC0 | matching | prompt `gUnk_08084AF8`, keys `0x40004`; picks any zone other than the card's own (`ref->player`/`ref->zone`) |
| `0x0804145C` | 0x130 | matching | 4 steps: prompts `gUnk_080849F4` / `gUnk_08084B38`, keys `0xF0`; step 3 refuses the same target as `ref->targets[0]` |
| `0x0804158C` | 0x110 | matching | steps 0-2: `sub_08022678(p, 9, 0, 0)`, `sub_0803DD7C(ref, DG.w1B64 + 1)`, prompt `gUnk_08083E14`; later steps: keys `0xE000E0`, `sub_0802B558(ref, w)` accepts -> `sub_0803DDAC` |
| `0x0804169C` | 0x140 | matching | `(ref, a)`: 4 steps, gate `sub_0802FCEC(ref, a, 0)`, prompts `gUnk_08084B6C` / `gUnk_08084BA0`, keys `0xE000E`; step 3 refuses the target equal to `ref->targets[0]` |
| `0x080417DC` | 0xBC | matching | prompt `gUnk_08084BD4`, keys `0xE00000`; the cursor zone must hold a card and differ from `ref->pos >> 8` |
| `0x08041898` | 0x328 | **nonmatching (asm)** | 6-step machine with a saved level at `0x02017A40+0x3E6`: prompt `gUnk_08083FD0`; keys `0xE000E0`, the level of the picked card must be `<= sub_08009DEC(ref->player)`, add target + level; `sub_08075434(buf, gUnk_08084C3C, level)` prompt; list viewer `sub_0802AF34(p, -1, 0x5FC, 0)`, message 0xD4 (0x80D4 for player 1) with the viewer card halves, counts the saved level down; step 5 `gUnk_08084C84` prompt with the remaining count, returns 1 when 0. C attempt (complete) under `#if 0` |
| `0x08041BC0` | 0xA0 | matching | prompt `gUnk_08084C98`, keys `0xE0000` |
| `0x08041C60` | 0xA0 | matching | prompt `gUnk_08084CEC`, keys `0x20002` |
| `0x08041D00` | 0xC4 | matching | prompt `gUnk_08084D20`, keys `0xF000F0`, `sub_0802C674(ref, w)` accepts the cursor zone |
| `0x08041DC4` | 0x1D8 | matching | `u16 f(ref, p, z)`: can the card in zone `(p, z)` be selected as an effect source? Requires a card of type > 0x14 (Magic/Trap), not face-up-restricted (`flags6 & 2`) unless card number in {0x52C, 0x3F9, 0x594, 0x5FC}, zone byte `+0x91` bit 2 set and bit 3 clear, no card 0x2EF on either side for Magic, level/side conditions (`sub_0802CD28`, `0x0201ADF2`/`0x020192E0+0x1B12` bits); on success fills `ref` (id, player, zone) and returns `sub_0802CE38(ref, 0, 0)` |

## Structs and globals (in addition to [[code-0803fe70]])
- `struct DuelZone` (0x94 bytes): `+0` card word (`id` 12 bits), `+6` flags (bit 1 = face-down, bit 0 ...), `+0x91` a flag byte (bit 2 / bit 3 tested by `0x08041DC4`).
- `0x0201930C + 0x1AE6` (= `0x0201ADF2`) byte, bit 1 compared with the acting player in `0x08041DC4`; `0x020192E0 + 0x1B12` byte: bits 2-4 a small mode (2 or 4 required), bit 1 a player.
- `PlayerState[p] + 7` bits 6-7 (`0x020192E4 + p * 0xD64`) tested for Magic/Trap effect sources.
- `0x02017A40 + 0x3E6` saved level counter of `0x08041898`; list viewer `gUnk_0201D810` (`struct ListView`: `+5` row, `+6` top, `+0xC` cards).

## Matching tricks
Everything in [[code-0803fe70]] applies. This unit adds:
- **The `es` copy for a jump-table switch**: `u8 *es; int sw = gUnk_02017A40[0x3E5]; es = gUnk_02017A40; switch (sw)` makes gcc keep the constant in one register and copy it (`ldr r1,=sym; ...; adds r5,r1,#0`) as the ROM does (`0x08041898`); with `u8 *es = ..; switch (*(es + 0x3E5))` the constant is folded into `es` directly.
- **`u16 w`** for the packed target word gives the ROM's argument-move order (`mov r0,r8; adds r1,r4,#0`, `0x08040FB0`).
- **`ZB2((1 - ref->player) & 1, i)` in both uses of a loop** reproduces `i * 0x94` before `player * 0xD64` and the `z + p` add order (`0x0804128C`); for an index that is a plain parameter use `ZB` (z-first text) (`0x08041DC4`).
- **Bit fields for single-bit zone/state flags** (`struct DuelZone { ... u8 f91_2 : 1; u8 f91_3 : 1; }`) make gcc form `zone + 0x91` with a separate `adds r0,#0x91` and load the byte once; `struct PS7 { u8 pad[7]; u8 b7; ... }` indexed by player gives `base + player * 0xD64` then `ldrb [r0,#7]`; `struct DG12 { u8 pad[0x1B12]; u8 b; }` + `u32 b = g->b;` and shifts on `b` avoid a spurious register copy.
- **`switch ((int)CARD_TYPE(id)) { case 0x15: case 0x16: ... }`** instead of `ty >= 0x15 && ty <= 0x16` reproduces the two-sided compare (`cmp #22; bgt; cmp #21; blt`).
- **`goto fill`/`goto ret0` labels** to reproduce a return block that sits before the "fill" code and is shared by all failing checks (`0x08041DC4`).
- **Empty `r0` clobber before the final card-type switch** (`0x08041DC4`). This compiler-only hint changes allocation over the function so `p` and `z` remain in the ROM's `r7` and `r6`. It emits no instructions and is marked FAKEMATCH in C.
- **Named `r1` byte local for clearing target count** (`0x08040EBC`, also `0x08040110` in [[code-0803fe70]]): load `ref` byte `+0xA`, pass that local as an input to an empty asm, then store `~7 & fields`. This preserves the other five bits and reproduces the original `r1` scratch. The compiler hint emits no instructions and is marked FAKEMATCH in C.
- **Duplicated tails**: in `0x0804145C`/`0x0804169C`/`0x08040FB0` the shared inc tail lives after the first (or last) case; simply writing `(*st)++; return 0;` in each case (see the other page) picks the same one as the ROM.
- **Cancellation pointer before zero also fixes earlier switch registers** (`0x0804158C`): initialize `u8 *e2 = gUnk_02017A40; u8 *q = e2 + 0x3E5; int z = 0;`, then `*q = z; return z;`. Reusing [[code-0803fe70]]'s pointer-before-zero pattern reproduces the cancellation address in r1, zero in r0, and its direct epilogue branch. It also gives the ROM's initial switch value r2 / base copy r3, resolving the apparent unrelated register mismatch without any asm constraint or ABI change. The earlier `*(e2 + 0x3E5) = z` form computes the pointer after initializing zero and differs by 14 bytes; named switch/base-register constraints do not fix that source-order cause. Complete `0x10E0`-byte unit verified exact.

## Open problems
`0x08040EBC`, `0x08040110`, and `0x08040CA8` now match; their former scratch-register issues are resolved by the verified compiler hints above and in [[code-0803fe70]]. `0x0804158C` now matches through the cancellation-pointer declaration order above. Formerly remaining in `0x0804112C` (resolved in wave 1, see below): the two `sub_0803DE40` call sites use different registers in the ROM (case 1: r5/r4, case 3: r4/r3), so gcc does not merge them, but our C does merge them. The ROM also merges the text calls into case 2. Remaining in `0x08041898`: the `CARD_LEVEL` value in r0 is copied to r4, and the return-0 block is shared.

The bounded follow-ups of 2026-09-30 stayed isolated. `sub_0804112C` prompt-pointer/goto sharing (with declaration-order and initialized-pointer variants) did not improve its 280 differing-byte baseline. For `sub_08041898`, an initialized first-level local with an empty read/write constraint recovers the first level calculation and copy to r4; later CardLevel-to-argument setup, return-block placement, and case-5 count setup still differ. Explicit shared return labels/zero constraints and first/second-level input constraints did not yield an exact match. A named r0 count used directly as a later call argument can be overwritten by preparation of argument zero; do not use that variant as a valid C proposal. Scratch scripts: `build/middle_experiments/selector_prompt_share.py`, `selector_level*.py`. Active source remains the original guarded drafts and assembly fallbacks.

## Card-scan interface reconciliation

`sub_08041DC4` now returns `int` with an explicit `(u16)` cast on its final dispatcher result. Its `sub_0802CE38` declaration agrees with the word predicate and actual pointer/u16 parameters in [[code-0802cae8]]. This preserves the original zero-extended result for [[code-0805a30c]]. The complete unit bytes are unchanged. These declaration repairs add no coverage; per-unit artifacts are under `build/bigguns-lead2/` and the full ROM passes at `build/lead-pass27/`.

## Target selector matched (wave 1, 2026-10-01)

`sub_0804112C` (0x160, start score 21) matches in ordinary C. Working notes: `build/wf/sub_0804112C/NOTES.md`.

The difference was cross-jump layout. The ROM merges case 0's prompt call into case 2 (from `mov r2,#0xB`), sends both `sub_0803DE40` successes to case 2's increment and both failures to case 2's `return 0`, and keeps the two `sub_0803DE40` / `sub_08077AEC` call sites separate. What made it match:

1. Case 0 order: clear `numTargets`, call the prompt, then `(*st)++` (the draft incremented first).
2. Case 1 keeps an explicit `(*st)++; return 0;` after `sub_0803DE40`, with the input check as `if (sub_0805304C() != 0) { ... sub_08077AEC(3); } return 0;`.
3. Case 3 uses `if (cond) { sub_0803DE40(..); (*st)++; } else sub_08077AEC(3);` with one `return 0` after the block. Without that explicit return, case 3's success tail does not jump to the return, so cases 0 and 1 cross-jump into case 2 instead of case 3 (score 2).
4. The packed position comparison is `(u16)((u8)p | (u8)zn << 8) != ref->targets[0]`. The pre-shifted form `((p<<24)>>8 | zn<<24)>>16` puts `lsr #8` before `lsl r1,r3,#24`; swapping the OR operands or dropping the `(u16)` cast is worse (8-10).

The earlier note that prompt-pointer/goto sharing did not help still holds: the fix is the per-case statement order and return placement, not shared labels. See [[matching-tricks#Switches, branches and shared tails]].
