---
title: Unit ai_steps (CPU duel AI, state machines between the AI and the duel UI)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit ai_steps

`0x0805B3F4`-`0x0805C508`, Thumb, `old_agbcc -O2`. Source: `src/ai_steps.c`. Sits between the CPU AI units [[ai-summon-c]] / [[ai-deck-c]] and the duel UI animation helpers [[duel-card-anim-c]]. Unit status: `unit bytes MATCH`, **5/6 functions in C**; one stays `INCLUDE_ASM` with its best attempt under `#if 0 /* NONMATCHING */`. All names are proposals (hypotheses).

> [!warning] Contradiction: the unit is now 6/6
> The count above (5/6) predates later matches. `src/ai_steps.c` has no `INCLUDE_ASM` left (checked 2026-10-02), so all
> 6 functions are in matching C. The decompilation reached 100% at commit `d77fcef` ([[overview]]). Resolved in favour of
> the source. Text below that calls a function nonmatching, parked or `INCLUDE_ASM` is history. Some of the later matches
> are recorded only in git (`git log`) and not yet written up here.

The unit is the "CPU turn" state machine: `gAiState` is an `AiState` (`+1` step, `+2` sub-step, `+6..+9` scratch counters, `+0xA` phase, `+0xB` chosen hand index). Each function is one state handler, called through the table `gAiSteps` by `AiRunStep`; it returns 0 to keep running and 1 when done or aborted.

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x0805B3F4` | 0x490 | **matching C**, initialized hints | `AiMainPhase` (hyp.) | phase switch 0..10. Phase 0 picks a hand card for player 1 (`AiChooseSummonNoTribute` / `AiChooseSummonWithTribute`), computes its cost class (0 / 1-4 / 5-6 / >=7) and queues the play with `QueueNormalSummon(1, idx, zone, extraMask, flag)` (extra cards chosen with `AiPickTributeMonster`, packed as bytes `x\|0x90`); phases 1-4 reset counters `+6..+9` and wait on `AiActivateMonsterEffects` / `AiActivateExodiaTraps` / `AiPlaySpells`; phase 10 does the same selection with `AiPickMonsterToSet` |
| `0x0805B884` | 0x2FC | **matching C**, initialized hints | `AiZoneScanPhase` (hyp.) | phase 0: `AiActivateMonsterEffects`; phase 1: loops over player-1 zones 0-4 (counter `+6`), decides per zone (card present, face-up flag, not blocked by cards 0x148/0x15C/0x4DC) whether to queue `QueueFlipSummon(1, zone)` (cards 0x1F4/0x2FA/`HasFlipEffect`) or `ChangeBattlePosition(1, zone, 0, 0)`; special LP test for cards 0x2F/0x23D/0x463 |
| `0x0805BB80` | 0x60 | matching | `AiStartPhase(void)` | `u16`: first call: if `CanEnterBattlePhase(1)` is 0 returns 1, else bumps `+0xA` and clears bit fields at `0x0201ADF4` (bits 9-16) / `0x0201ADF6` (bits 1-8); then returns `BattlePhase_Run(1)` |
| `0x0805BBE0` | 0x44 | matching | `AiRunStep` | calls handler `gAiSteps[step]`; if it returns non-zero: clears counters `+2..+6`, `+0xA`, `step++`; returns 1 when the table entry is null |
| `0x0805BC24` | 0x47C | nonmatching (`#if 0`, reg alloc) | `AiPickCandidate` (hyp.) | loop `i = 0..8`: case `i` tests a set of conditions (LP thresholds, `CountFreeMonsterZones`, `AiFindHandCardByNumber(1, card)`, `CountFaceUpMonstersByNumber`...) that decide if the AI should use that scripted strategy (cards 0x290/0x1A3/0x17B, 0x34D, 0x58A/0x4DD, 0x13D/0x3D/0x4E1, 0x522, 0x5EA/0x5EB, 0x3BA); on success stores `(i << 1) \| 1` in `0x02017A24` and returns 1 |
| `0x0805C0A0` | 0x468 | **matching C**, initialized hints | `AiCommitPhase` (hyp.) | sub-step switch (`+2`, 0..9): commits hand card `+0xB` into the state block `0x0201AE08` (card id, flags) for cards 0x29F/0x425/0x438/0x14F/0x150/0x290, looks for card 0x1A3 in hand and queues `QueueNormalSummon`/`DuelCmd_Push(0x8008..)`/`Chain_AddPending`; final step returns `BattlePhase_Run(1)` |

## Structures and globals

- `gAiState` `AiState` (see above). `gAiWork` AI work area: `+0x1B24` byte = `bit0 found` + `bits1-7 index`, `+0x1B25` byte = candidate hand/zone index.
- `gDuel` / `gDuelPlayers` (player array, `0x020192E4` = player 0; the code reaches player 1 through `+0xD64`): `+0xD64` player 1 LP (hyp.), `+0xD66` hand count, `+0xD6B` flag byte, `+0xD6C` flag byte (bit 4), `+0x13E8` player-1 hand (u32 card words, id in low 12 bits), `+0x1B0C` u16, `+0x1B24..` commit block (`CommitBlk`: `+0` u16 card id, `+4` flags, `+8` and `+0xC` bitfield halfwords that get cleared/filled when a card is committed). The same block is addressed through both the `0x020192E0` and `0x020192E4` symbols, which is what the ROM's different literal-pool bases show.
- `gDuelZonesP1` = player-1 zones (`DuelZone`, 0x94 each), `+6` flag byte (bit 1 = temporary), `+0x91` bit 3.
- Card tables: `gCardIdToNumber` (card number by id), `gCardStats` (stats; type = bits 20-24, cost class = bits 25-28, see `CardCost` in [[ai-summon-c]]).

## Matching tricks learned

- **Bit-field clears as halfwords:** `x.b = 0` on a `u16 a:2; u16 b:8; u16 c:6;` bit-field inside a 4-byte struct gives `ldrh; and (ldr =0xFFFFFC03); strh` with the exact int mask. Plain `&= ~0x3FC` on a `u16` narrows the mask to `0xFC03` and does not match (`AiStepBattle`).
- Clearing bit 0 of a `u8` with a one-bit bit-field gives the `mov r0,#2; neg r0,r0; and` (-2) mask the ROM uses (`AiChooseStrategy`).
- A `switch` with `default:` that returns, placed first, produces the ROM's `cmp #n; bls; b <default>` head and the `mov pc, r0` jump table; cases that share a body (`case 1: case 3:`) share a table entry.
- `if (x != -1) break; continue;` versus `if (x == -1) continue; break;` flips the branch sense of the tail (`beq` vs `bne`); use the former when the ROM has `beq <continue>; b <check>`.
- A loop reading `gDuelZonesP1[j]` through a cast integer pointer (`(struct DuelZone *)0x0201A070`) reloads the base on each use; through the extern it is hoisted (`AiChooseStrategy` case 4).
- `CanNormalSummon` has to be prototyped `int` for `AiStepMainPhase` (no `lsl/lsr 16` before `cmp`) but the caller `AiStepChangePositions` casts the result to `u16`.
- Uninitialised local used as accumulator lands in `r8`/`r9` exactly like the ROM (`sum += info.value` in `AiChooseStrategy` case 4 never initialises it: an original bug).

## Near-misses

- `0x0805B884`: identical instruction stream; ROM keeps the state pointer in `sl` and reloads `0x02015EF0` inside the loop.
- `0x0805B3F4`: identical instructions except register numbers (idx r7, id r6, n r5, flag r8) and the ROM does not tail-merge the second branch with the first.
- `0x0805BC24`: same structure; loop counters and `id` swapped (r4/r5), constants in `sl`/`ip` where the build uses `r5`/`r7`.
- `0x0805C0A0`: same control flow; the commit tail is cross-jumped with another copy in the build, and its base registers differ.

## ABI audit of the parked zone scan

The parked `AiStepChangePositions` does not currently compile when enabled: its call to `HasFlipEffect` supplies one argument, whereas the callee takes `(u16 cardNo, u16 flag)`. At the ROM call, `r0` contains the table lookup and `r1` still contains `0x08622AB4`, the table base. The callee narrows `r1` to `0x2AB4`; cards 640, 735 and 1170 use that flag. The caller tests only whether the return is zero. These are observed instructions, not evidence that a zero or player index is the missing argument. An original unprototyped call is a hypothesis. Keep assembly active until both the effective argument and exact code generation are represented; adding an invented flag is not a repair.

A survey of 420 active literal-Boolean definitions found eight parked callers worth checking. Bounded return-width trials at `EventResponse_Run`, `UpdateSpellTrapNegation`, `DuelScreen_DrawCursorInfo` and `DrawHandCards` produced no exact matches; two other callers already had narrow declarations. The scan and private variants are in `build/bigguns-lead2/boolean-scout.json` and the corresponding function directories. No callee ABI or active caller changed.

## `AiStrategyCyberStein` matched (2026-10-01)

`AiStrategyCyberStein` is enabled C (0x468 including alignment, 0x466 symbol bytes). The full 0x1114-byte unit and entire ROM match. Its ten-state behavior passed 1,152 finite differential cases: every state and two invalid states, empty/full hands, LP thresholds and comparisons, commit writes, field-card guards, queued arguments, and a synthetic message helper that changes the target before the second target read. Return values, memory, call arguments and preserved registers/SP agree. This supports, but does not replace, the exact byte check.

- The guarded hand scan and message/action packing reuse the completed neighboring [[ai-strategy-c]] patterns. The search key's binding was removed. The case-5 byte load needs an initialized r0 scratch before copying the index for the later message. Packing keeps initialized r1/r0 mask/high locals, with unsigned shifts. The queue base is initialized in r0 only after the scan.
- Both case-6 and case-7 failures branch to the same source label after the last commit. This preserves the ROM's failure-tail placement and gives the original commit scratch registers without additional constraints.
- The player-1 LP reader needs an initialized r5 address copy, an initialized r3 offset with one read/write constraint, and an r0 pointer. Removing the address copy or the read/write constraint reverses the commutative ADD operands, which is behaviorally equivalent but not matching. Seven initialized bindings and one empty constraint remain after independent removal checks.

> [!warning] Corrected parked-draft behavior/layout
> This unit had the same four-byte found-union layout problem as [[ai-strategy-c]]. Direct byte bitfields now place the target at +0x1B25. The parked candidate-selector's raw-byte store was rewritten to the same address, preserving its intended byte write. The strategy's message argument order is `(0x8008,1,target<<8,0)`; the former draft reversed arguments 2/3. ROM register setup proves the corrected order. Other parked routines remain disabled, including the one-argument property call discussed above.

Evidence: `build/bigguns-lead2/ai_commit_neighbor{,_shapes,_regs,_life,_add,_minimize}.py`, `enable_ai_commit_neighbor.py`, `verify_ai_commit_neighbor.py`, `AiStrategyCyberStein/solo-neighbor-clean/`, and `build/lead-pass23/`. The historical near-miss statement for this function is superseded.

## `AiStepChangePositions` matched (2026-10-01)

`AiStepChangePositions` is enabled C, all 764 bytes exact; the unit is now 4/6 C. The full ROM and both affected units match. Its 1,920 differential fixtures include phases 0/1 and invalid phases, all low flag pairs, zero/positive/negative simulated ATK, LP boundaries, queued actions, and helpers mutating the selected zone between reads. Fixtures check memory, calls, preserved registers and SP. The property hook uses the actual card-number/flag rules and checks the full r1 argument, not a synthetic Boolean approximation.

> [!warning] Earlier parked-call blocker resolved
> The one-argument draft and historical prototype description above are superseded. The caller now passes the observed table-base word `0x08622AB4` explicitly; [[card-detail-c]] decodes it with `u16 flag = flags` on entry. Its entire 0x111C-byte unit is unchanged, including the 0x2AB4 return for numbers 735/1170. This recovers the effective machine interface; an original unprototyped call remains only a provenance hypothesis.

- Putting the scan inside switch case 1 preserves the initial branch order. A byte `ok` produces the ROM's branch-free nonzero conversion. The special-number cases are emitted in the order 0x2FA then 0x1F4; the LP alternatives use a signed integer switch.
- Reload the zone index after the ATK helper before clearing the temporary face-up bit. Distinct address lifetimes keep the first scan pointer, simulation pointer, number pointer and action pointers separate. The persistent state, field base and face-up mask occupy callee-saved registers; no caller-saved binding survives a call.
- Seventeen initialized bindings and eight empty constraints remain after two removal passes. These preserve selected address terms, the temporary flag set/clear, and the post-property count test. The bindings are compiler hints, marked FAKEMATCH; all values are assigned before use.

Evidence: `build/bigguns-lead2/ai_zone_{scan,shapes,flow,regs,lifetimes,addresses,flags,last,minimize,prepare,accept}.py`, `verify_ai_zone_scan.py`, `AiStepChangePositions/solo-zone-accepted/`, `HasFlipEffect/solo-zone-accepted/`, and `build/lead-pass24/`. The old zone-scanner near-miss statement is superseded.

### Parked main-phase improvement

The separate `AiStepMainPhase` candidate in `AiStepMainPhase/solo-main-inline-1/` passes 1,664 finite differential fixtures, but remains **nonmatching**: four normalized extra queue-call/setup instructions, symbol size 0x49A versus 0x48E. A 60-second safe compiler search and bounded shape/scope/tail grids found no exact result. Register-only idx=r7 binding produced an incorrect stack-address substitution and was rejected. The candidate uses word queue arguments justified by the callee's explicit low-half consumption, but that interface is not activated here. Merge any future candidate into the current live unit, because its saved whole-unit source predates the enabled zone scanner. No additional coverage is claimed.

## `AiStepMainPhase` matched (2026-10-01)

`AiStepMainPhase` is enabled C: 1,168 source bytes (1,166 symbol/code bytes plus alignment). The full 0x1114-byte unit and ROM match; this unit is now 5/6 C. The earlier main-phase near-miss notes are superseded.

- Assign the same ordinary state pointer after each phase-10 queue call, then jump to a common phase store. This resolves the last four normalized extra instructions by letting GCC share the call and setup tails. The prior break/goto/argument-sharing grids did not match; an RTL dump exposed the internal end-of-switch marker that prevented the previous tail merge.
- Keep the first cost calculation expanded locally so no extra ID parameter copy is introduced. Initialized table terms, retained mask/card locals, and unsigned tribute packing reproduce the earlier code. The post-queue index input deliberately preserves a separate phase-0 return tail. All fixed caller-saved values die before external calls.
- Two removal passes eliminated three bindings and two constraints. Thirteen initialized bindings and six empty constraints remain, marked FAKEMATCH. The final tail-pointer assignment itself needs no hints. An idx=r7 experiment from the earlier work remains rejected because it produced an incorrect stack-address substitution.
- The queue callee consumes two words by explicit low-half decoding, as its original entry shifts show. Its still-disabled C draft in [[summon-action-c]] now states those decodes. [[ai-summon-c]] exposes the original zero-extended `AiPlanAttack` result as a word; its complete unit bytes are unchanged. The local `AiPickTributeMonster` declaration uses the actual u16 second parameter.

The final source passes 3,328 finite ROM/C fixtures: all 0–11 phases plus 255, available real card cost classes, no-card exits, tribute failures, packed queue arguments, randomized upper card bits, and hooks that replace hand cards before the original reload. Return values, memory, calls, preserved registers and SP agree. This is finite evidence alongside exact bytes, not a completeness proof.

Evidence: `build/bigguns-lead2/ai_main_{resume,last_break,tail_location,tail_pointer,minimize,clean,abi,accept}.py`, `verify_ai_main_complete.py`, `AiStepMainPhase/solo-main-accepted/`, and `build/lead-pass28/`.
