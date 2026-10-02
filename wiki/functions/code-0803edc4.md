---
title: Unit code_0803EDC4 (duel target selectors, part 2)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit code_0803EDC4

`0x0803EDC4`-`0x0803FE6F`, Thumb, `old_agbcc -O2`. Source: `src/code_0803EDC4.c`. Continues [[code-0803dd7c]] (same target pickers: `int pick(struct CardRef *ref)` returns 1 when the targets are filled in, 0 while waiting; step byte at `0x02017A40 + 0x3E5`; the common helper is `sub_0803DDAC(ref, player, zone)`, which adds a target if accepted; see that page). `CardRef` is the one from [[code-08030b88]] with three target slots.

Unit status: `unit bytes MATCH` (`tools/check.py code_0803EDC4`, complete 0x10AC-byte unit), 11/12 functions in C after workflow wave 1 (2026-10-01, `sub_0803EE6C` added; `sub_0803FC88` was enabled earlier); `sub_0803F034` stays `INCLUDE_ASM` (attempt under `#if 0`). Verification log: `build/middle_experiments/sub_0803FC88/unit-check.log`.

## Functions

| Address | Size | Status | Purpose (hypotheses about role, verified about logic) |
|---|---|---|---|
| `0x0803EDC4` | 0xA8 | matching | prompt `gUnk_0808405C`, keys `0xD2 << 16`, add the cursor position (`sub_0803DDAC` accepts, else `sub_08077AEC(3)`) |
| `0x0803EE6C` | 0x1C8 | **matching** (wave 1, 2026-10-01) | AI: adds the first two occupied unflagged spell/trap zones (5-9); player 0: 4 steps 0..3 (`sub_0802D800` gate, prompts `gUnk_080840A4` / `gUnk_080840D8`, keys `0x20002` twice, the second pick must differ from `targets[0]`). Matched once the real second parameter `arg` was restored (see below) |
| `0x0803F034` | 0x310 | **nonmatching (asm)** | `(ref, arg)`; AI: up to two picks, preferring player-1 zones whose card number is in 0x10-0x14 (`gUnk_02015EE8+4 & 0x200` enables it), else `sub_0805748C(0, i > 0 ? 0 : -1, 1, 1)`; the second pick must differ from `targets[0]`; player 0: 6-step machine (`sub_0802CE38` gate, `sub_08008860(0) + (1)` count, prompts `gUnk_0808410C` / `0808413C` / `08084178`, keys `0xF000F0`). Draft in `#if 0`; the ROM keeps `ref` in r6, `i` in r7 and the constants (1, 0xFFFF, zone base) in r8/sl/ip, ours does not |
| `0x0803F344` | 0x2AC | matching | AI: `sub_0805748C(0, -1, 1, 1)` result as target 0; player 0: card-dependent prompt (0x280 / 0x403 / 0x42C+0x5EA / other, each guarded by `sub_080088A4(1 - player, ...)`), step 1: keys 0xF0<<16 or 0xE0<<16 by card, then the pick is validated with `sub_0802B1B8` and per card (0x42C: not card 0x547; 0x42C/0x4DC: face-down flag 2) |
| `0x0803F5F0` | 0xAC | matching | prompt `gUnk_08084290` only if `sub_080088A4(p, 1, 0)` (else done at once), keys 0xE0 |
| `0x0803F69C` | 0x9C | matching | prompt `gUnk_080842CC`, keys `0x900090`, `sub_0803DDAC` result ignored, returns 1 |
| `0x0803F738` | 0x19C | matching | AI: per side i in 0-1 the face-down (flag 2) zone 0-4 with the highest `sub_0800C894(i, j)`; player 0: prompt (cards 0x3AB/0x5AB use `gUnk_08084318`, else `gUnk_08084044`), keys `0xE000E0` |
| `0x0803F8D4` | 0x120 | matching | 2-step: prompt per card (0x3B1 / 0x524 / 0x527: `gUnk_08084348` / `08084368` / `080843A0`, else none), keys 0xF0, returns 1 |
| `0x0803F9F4` | 0x10C | matching | 4-step: prompt `gUnk_08083E14`, cursor pick checked by `sub_0802B558` (then `sub_0803DDAC`), prompt `gUnk_080843E4` (text box id 0x613) + `sub_08060308(2, 0, 0)`, then `AddTarget(ref, gUnk_0201AE60.flag14 + 1)` |
| `0x0803FB00` | 0xEC | matching | AI: first zone 0-4 of the opponent side accepted by `sub_0802BE70`; player 0: prompt `gUnk_08084420`, keys `0xE0 << 16`, pick must be accepted by `sub_0802BE70` (with the opponent side) |
| `0x0803FBEC` | 0x9C | matching | prompt `gUnk_08084470`, keys `0x80008`, returns 1 |
| `0x0803FC88` | 0x1E8 | matching | 6-step machine (jump table): prompts `gUnk_0808449C` / `080844C0` / `080844E4`, three picks (keys 0xF0, 0xF0, 0xF0<<16), the second must differ from `targets[0]`, the last must be accepted; steps > 5 return 1. Explicit case-1 failures, per-arm pointer lifetimes, initialized r2 offset constraint, equivalent signed subtraction and separate initialized sound constraint reproduce all branch tails and literal registers |

## Matching tricks (in addition to [[code-0803dd7c]])
- **Several increments of the step byte.** Give each arm its own `{ u8 *e = gUnk_02017A40; e[0x3E5]++; } return 0;` rather than a shared `break` or `goto`. Duplicating them raises the priority of `st` in the register allocator (`0x0803F9F4`: `st` in r6, `p` in r7 as in the ROM; with a shared tail it was swapped) and gcc cross-jumps them into the layout the ROM has.
- **`u8 z = 0; *st = z; return z;`** for every `h6 & 2` reset (shared tail with `mov r0,#0; strb; b epilogue`).
- **`if (f(..) != 0) {..} else {sound}`-style code** as the ROM writes it: `if (tg != pos) { if (DDAC) {inc} } else { sound }` puts the sound block after the increment (`0x0803FC88`), and `if (B1B8) { switch ..; DDAC; return 1; } sound(3);` puts the failure sound after the `return 1` (`0x0803F344`).
- **Uninitialised `u32 keys;`** in a switch without a default (`0x0803F344`): the ROM leaves the register untouched for the unlisted card numbers, so `sub_08052F38` gets garbage there (hypothesis: never reached).
- **`int pp = 1 & p; ZB(pp, z)`** with `ZB` for the first zone read and `ZB2` (p-first text order) for the second (`0x0803F344`), as noted in [[code-0803a654]].
- **Fallthrough between `case 0x42C:` and `case 0x4DC:`** (`0x0803F344`): the first case tests the card, then falls into the flag test.
- The prompt call `sub_080602A4(0x206, 0x712, 0xB, text)` is written out in every arm; gcc cross-jumps the shared `mov r2,#0xB; bl` tails on its own.
- **Two-copy `1 & ref->player`**: `pl = 1 & ((u8 *)ref)[2]; one = 1;` then `one - ref->player` (bitfield read) in the same function, see `0x0803F344`.

## Open problems
`0x0803F034` (AI arm register assignment). `0x0803EE6C` (cursor-offset reload and r1/r2 temporaries) was resolved in wave 1; see below.

### Bounded experiments

Fresh isolated compilation of `sub_0803EE6C` is 0x1CC rather than the ROM's 0x1C8, with 150 differing bytes. The old "12 diff lines, only temporaries" annotation understates the present discrepancy: case 1 reloads 0x828 after forming 0x824, whereas the ROM advances the same r5 offset by four. Cursor-array forms, p-first/z-first loads, cancellation-zero forms, and initialized offset/input/read-write constraints did not settle the function. A raw target-count clear reaches the right size but substitutes an immediate AND for the ROM's negative-mask sequence, so that is not a matching recipe. Scripts: `build/middle_experiments/two_pick_cursor*.py`.

`sub_0803F034`'s initial occupied-card test reads zone `i`, while its inner card-number lookup reads zone `j`; preserve this ROM behavior. Fresh baseline is 0x300 versus ROM 0x310. The ROM retains the normalized 12-bit id in r9, one in r8, sentinel in sl and zone base in ip, and spills the first-target read inside the `i != 0` branch. Initialized id/sentinel constraints and a conditional first-target load reduce instruction-layout discrepancies, but still change saved registers, frame size and branch tails. A private string-replacement defect in the first invariant grid was corrected before evaluating its follow-up; no production draft was changed or candidate accepted. Named-register/lifetime grids remained nonmatching. Instruction-sequence ranking in the private harness is only a search aid; it never replaces exact byte comparison. Scripts: `build/middle_experiments/ai_two_pick_invariants.py`, `ai_two_pick_register_life.py`.

Fresh `sub_0803FC88` baseline was 0x1D0 against ROM 0x1E8 (299 differing bytes), so its old "only increment tails" annotation was also stale. Initialized per-arm step offsets passed through empty read/write constraints stopped complete increment merging and restored the separate 0x824/0x828 cursor literals. Keeping the resulting pointer in initialized r0, expressing case 1's failures as explicit returns to `ret0`, and retaining initialized sound id 3 separately in case 3 reached the exact 0x1E8 size with six differing bytes: two reversed address-add operands and the shared increment/reset literal-register choices (`build/middle_experiments/sub_0803FC88/six-byte.c`). Staged/cast additions and extra common/reset offset constraints did not settle those bytes. This intermediate recipe was never enabled; the final accepted version is described below. Scripts: `three_pick_tails.py`, `three_pick_finish.py`, `three_pick_adds.py` in `build/middle_experiments`.

That six-byte proposal was subsequently settled and enabled. A bounded 60-second permuter found that retaining case 3's derived step pointer separately before its use restores the shared-tail and reset registers. Case 2's equivalent signed addition form `p = e - (-off)` fixes the final reversed ADD operand; `off` is an initialized `int` 0x3E5, so negation is defined and the subtraction is exactly `e + 0x3E5`. Hint minimization removed both r0 pointer pins/inputs and every case-3 offset/pointer hint. Only two empty read/write constraints remain: initialized named r2 case-2 offset keeps that address setup separate, and initialized sound id 3 keeps case 3's sound tail distinct from case 5. The hints emit no asm instructions and add no ABI parameters or uninitialized values. The reformatted clean body and complete 0x10AC unit both match. Archives: `clean-body.c`, `minimal-match.c`, `unit-check.log` in `build/middle_experiments/sub_0803FC88`; `three_pick_permute_finish.py` and `three_pick_minimize.py` record the bounded follow-up and removed hints.

A later follow-up on `sub_0803EE6C` tried staged cursor pointers, field-mask order and separate sound lifetimes without an exact result. A bounded permuter reached score 15 only by placing an initializer before a switch case, where control flow skips it; all such outputs are rejected. Valid initialization outside the switch plus an empty result constraint reaches the correct 0x1C8 size with 24 differing bytes, but adds a zero-register initializer, changes the step-offset register and compares key result against that register instead of the ROM's immediate zero. Copying the existing proven-zero player or leaving the extra result unused did not settle this. Private scripts `two_pick_lifetimes.py`, `two_pick_initialized_zero.py`, `two_pick_dead_zero.py`; `sub_0803EE6C/permuter-new.log` records the bounded search. No invalid or near-match proposal was enabled.

## Target picker matched (wave 1, 2026-10-01)

`sub_0803EE6C` (0x1C8, start score 18) matches in ordinary C. Working notes and reload-tracing scripts: `build/wf/sub_0803EE6C/` (`dump.sh`, `reloads.py`).

- Case 3: `u8 pos` gave `ldrb` where the ROM has `ldrh`; `int pos` with `ref->targets[0] != pos` fixes it.
- Root cause of the r1/r2 temporaries and the extra `0x828` literal: the two temporaries are **reload registers**, not pseudos (combine folds the `ldrb` into the AND as a memory operand, and the large offsets stay `(plus reg const)` until reload). Reload hands out spill registers round-robin across the whole function (`last_spill_reg` in `allocate_reload_reg`), skipping spill registers that hold a live pseudo. With spill set {r0, r1, r2, r4, r5}, our build gave case 0 r1 and case 1 r2.
- In the ROM r1 is occupied at case 0: the function has a **second parameter** `arg`, live in r1 from entry and passed on to `sub_0802D800(ref, arg)` (the r1 move is a no-op, so no instruction shows it). Case 0 therefore takes r2, and case 1 takes r5, then `r5+4` and `r5+8`, which is the ROM's advance instead of a second literal.
- Fix: `int sub_0803EE6C(struct CardRef *ref, int arg)`, with case 0 calling `((CondFunc_0803EE6C)sub_0802D800)(ref, arg)`. The unit's existing prototype for `sub_0802D800` takes only ref, so the call goes through a typedef cast; if that prototype is widened to `(ref, int)`, the cast can go.
- General lesson: if a reload temporary lands one register off, look for a hidden live value, such as an unused-looking incoming argument passed on to a callee. See [[matching-tricks#Register allocation priority and reload rotation]].
