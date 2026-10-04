---
title: Unit duel_stat_queries
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit duel_stat_queries

Duel-engine unit (`0x0800C894`–`0x0800D8A4`). All functions are in `src/duel_stat_queries.c` and the unit matches (`tools/check.py`). This unit handles card-effect targeting, zone attribute queries, and duel-command dispatch. The neighbouring pages [[duel-cmd-field-c]] and [[duel-piles-c]] describe the duel structs it uses.

Unit status: `unit bytes MATCH`, **23/24 functions in C** after workflow waves 2-3 (2026-10-02: `0x0800D398` in wave 2, `0x0800CAF0`, `0x0800CE28` in wave 3); 1 stays `INCLUDE_ASM` (`GetZoneCardType`, parked draft under `#if 0`). Before wave 2: 20/24 (after wave 1).

> [!warning] Contradiction: the unit is now 24/24
> The count above (23/24) predates later matches. `src/duel_stat_queries.c` has no `INCLUDE_ASM` left (checked 2026-10-02), so all
> 24 functions are in matching C. The decompilation reached 100% at commit `d77fcef` ([[overview]]). Resolved in favour of
> the source. Text below that calls a function nonmatching, parked or `INCLUDE_ASM` is history. Some of the later matches
> are recorded only in git (`git log`) and not yet written up here.

## Function table

> [!warning] Contradiction
> The table row for `0x0800D398` (2026-10-01 and earlier) says it clears `+0x8C` "bits 0-1, and if bit 3 set: clear bits 0-3 + set bit 4". The wave 2 match (2026-10-01, `build/wf/DuelCmd_PrepareBattlePhase/NOTES.md` and `src/duel_stat_queries.c`) writes `if (b3) { b3 = 0; b4 = 1; } b1 = 0;` on the `+0x8C` byte: bit 3 moves to bit 4 and bit 1 is cleared; bit 0 is not touched. Resolved in favour of the matched source; the row is updated.

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x0800C894` | 0x14 | matching | `GetZoneCardUnk4` | Returns `zone[unk4]` field via `GetZoneCardStats`. |
| `0x0800C8A8` | 0x14 | matching | `GetZoneCardUnk8` | Returns `zone[unk8]` field via `GetZoneCardStats`. |
| `0x0800C8BC` | 0x234 | nonmatching (draft) | `FindBestTarget` (hyp.) | Given a zone, searches both fields for the best card to target: own zones for type 0x2FA, opponent zones (via `gDuelSpellTrapZones`) for type 0x479, and attached cards for type 0x60E. Returns a priority value (0, 1, 0xA, or bits from zone+0x90). NONMATCHING draft in `#if 0`. |
| `0x0800CAF0` | 0x128 | **matching** (wave 3, 2026-10-01; FAKEMATCH) | `GetCardAttribute` (hyp.) | Returns stats bits 29-31 of `gCardStats[cardId]` (0 for an empty zone). For a face-up monster (slot 0-4), a kind-1 link to a card whose number (`0x08622AB4`) is 0x5A8 overrides it with the linked zone's `+0x90` bits 13-17, if that zone's `+0x91` bit 3 is clear, neither side has 0x601 (`CountActiveCardsOnField`) and `gDuelNegationFlags` bits 0-1 are clear. |
| `0x0800CC18` | 0xB4 | **matching** (wave 1, 2026-10-01) | `ZoneHasCompatibleCard` (hyp.) | Searches both players' monster zones for a face-up card (+6 bit 1) compatible with the given zone's card (via `FindZoneLinkFromCard`). Returns 1 if found, 0 otherwise (also 0 if the source zone is empty). |
| `0x0800CCCC` | 0x58 | matching | `EvalZoneAgainstTarget` | Evaluates a card at (player,slot) against (targetPlayer,targetSlot) via `EffectEquipTargetCheck`. |
| `0x0800CD24` | 0x44 | matching | `CountMatchingZones` | Counts zones where `IsValidEquipTarget` returns nonzero (both players, slots 0-4). |
| `0x0800CD68` | 0xC0 | **matching** (wave 1, 2026-10-01) | `FindCompatibleZonePos` (hyp.) | Like `IsCardLinkedToMonster` but returns `(slot<<8)|player` of the matching zone, or 0xFFFF if none found, or 0 if zone empty. |
| `0x0800CE28` | 0x40C | **matching** (wave 3, 2026-10-01) | `StepTransferAnimation` (hyp.) | Seven-stage two-zone animation: prepares both positions, draws a timed pulse and interpolated motion (32 frames), copies banner graphics, then draws the banner for 96 frames (fast-forward +3) and finishes. |
| `0x0800D234` | 0x164 | matching | `StepAnimHandler` (hyp.) | State-machine animation handler. Step 0: set busy, call `DuelCursor_Select`. Step 1: clear busy, copy VRAM data, reset timer. Step 2: animated banner with timer and fast-forward support. Active C. |
| `0x0800D398` | 0xA4 | **matching** (wave 2, 2026-10-01; FAKEMATCH) | `ClearZoneFlagsAndCheckType` (hyp.) | Iterates the acting player's 5 monster zones: in the flags byte at zone+0x8C moves bit 3 to bit 4 (if set) and clears bit 1, then checks card type == 0x538 → sets bit 5 at zone+7. Ends with `CMD_DONE()` (`cmd->running = 0`). |
| `0x0800D43C` | 0x2C | matching | `CmdCallA39C` | Calls `MarkMonsterAttacked` with `CMD_PLAYER()` and `arg1`, then `CMD_DONE()`. |
| `0x0800D468` | 0x30 | matching | `CmdCallE3B8If` | If `DuelScreen_FadeOutStep()` returns true, calls `BattleScene_Init(arg1, arg2)`, then `CMD_DONE()`. |
| `0x0800D498` | 0x30 | matching | `CmdCallE788If` | If `BattleScene_Update(arg1, arg2, arg3)` returns true, `CMD_DONE()`. |
| `0x0800D4C8` | 0x5C | matching | `CmdSetPlayerFlags` | Sets `flag8_0` and `flag8_1` on the acting player based on `arg1`/`arg2`. |
| `0x0800D524` | 0x70 | matching | `CmdResetCounterAndPhase` | Unless `LINK_SKIP()`, resets `unk24` for both players and sets phase to 12. |
| `0x0800D594` | 0xA0 | matching | `CmdSetUnk0_9` | Unless `LINK_SKIP()`, sets `gBattle.unk0_9` from `arg1>>8`; if `arg2`, sets phase 6. |
| `0x0800D634` | 0xA0 | matching | `CmdSetUnk0_6` | Unless `LINK_SKIP()`, sets `gBattle.unk0_6` from `arg1>>8`; if `arg2`, sets phase 6. |
| `0x0800D6D4` | 0x28 | matching | `CmdSetUnk0_5` | Sets `gBattle.unk0_5 = 1`, then `CMD_DONE()`. |
| `0x0800D6FC` | 0x88 | matching | `CmdSetPhase11AndCall` | Unless `LINK_SKIP()`, sets phase 11; calls `MarkMonsterAttacked(player, arg1)`. |
| `0x0800D784` | 0x50 | matching | `CmdZoneWrite` | Calls `PlaceMonsterCard` with unpacked args, then `DrawAllAreaTiles()`, then `CMD_DONE()`. |
| `0x0800D7D4` | 0x50 | matching | `CmdClearZone` | Clears `cardId` of the acting player's zone at `arg1`, calls `DrawAllAreaTiles()`, `CMD_DONE()`. |
| `0x0800D824` | 0x40 | matching | `CmdCall096F4If` | If upper 12 bits of `(arg2<<16)|arg1` are nonzero, calls `AddCardToGraveyard`. |
| `0x0800D864` | 0x40 | matching | `CmdCall09768If` | If upper 12 bits of `(arg2<<16)|arg1` are nonzero, calls `AddCardToBanished`. |

## Globals used

| Name | Address | Type | Purpose |
|---|---|---|---|
| `gDuelCmd` | 0x020185C0 | `struct DuelCmd` | Duel command block (hdr, args, step/timer/running at +0x80C/+0x80D). |
| `gDuel` | 0x020192E0 | `struct DuelGlobal` | Global duel state (flags, phase word). |
| `gDuelPlayers` | 0x020192E4 | `struct DuelPlayers` | Both players' duel state (0xD64 bytes each). |
| `gDuelZones` | 0x0201930C | `u8[]` | Field zone array (0x94 bytes per zone, indexed by player*0xD64 + slot*0x94). |
| `gDuelSpellTrapZones` | 0x020195F0 | `u8[]` | Another field/card array used by `GetZoneCardType` (same zone layout). |
| `gDuelCtrl` | 0x02015EE8 | `struct Unk02015EE8` | Link duel flag. |
| `gBattle` | 0x02018450 | `struct Unk02018450` | Duel UI state flags. |
| `gCardIdToNumber` | 0x08622AB4 | `u16[]` | Card type/attribute table (indexed by card ID). |
| `gCardStats` | 0x08621DE0 | `u32[]` | Card attribute table (u32 per card ID, used in `GetZoneCardAttribute`, `GetZoneCardType`). |
| `gDuelBannerPal` | 0x08687B9C | `u8[]` | VRAM data for banner DMA. |
| `gDirectAttackBannerGfx` | 0x086883BC | `u8[]` | VRAM data for banner DMA (0x400 bytes). |
| `gBounceScaleCurve` | 0x081A43E4 | `u16[]` | Sine/effect table for banner animation. |
| `gDuelNegationFlags` | 0x0201ADAD | `u8` | Global flag checked in `GetZoneCardAttribute` and `GetZoneCardType`. |
| `gDuelScreen` | 0x0201CFB0 | `u8[]` | Busy/lock flags for duel UI. |
| `gMain` | 0x03000040 | `u16[]` | GBA I/O registers (key input at +4). |

## Matching tricks

- `CMD_PLAYER()` / `CMD_DONE()` macros for the command block.
- `ZONE(p,s)` macro for field zone access via `gDuelZones`.
- `LINK_SKIP()` macro for link-duel guard conditions.
- `FindMonsterLinkedToCard` return: the shift-chain expression `(((u32)p << 24) >> 8 | (u32)s << 24) >> 16` reproduced the return sequence in the old draft, but the matched source (wave 1) uses the plain `(u8)p | ((u8)s << 8)` with a `u16` return type.
- Card ID extraction uses `(*(u32 *)zone << 20) >> 20` for 12-bit (or `<< 21 >> 20` for 11-bit with *2). The shift amount depends on whether the result is used as an index or raw value.
- The `DuelCmd` step is byte `+0x80A` bits 0-6; timer is halfword `+0x80C` bits 5-11; running is byte `+0x80D` bit 5. The u32 timer container preserves the ROM's signed branch shape.

## NONMATCHING drafts

The unit had **18/24 functions in C** before workflow wave 1; `IsCardLinkedToMonster` and `FindMonsterLinkedToCard` matched on 2026-10-01 (20/24), and `DuelCmd_PrepareBattlePhase`, `GetZoneCardAttribute` and `DuelCmd_Attack` in waves 2-3 (23/24). Only `GetZoneCardType` is still `INCLUDE_ASM` with an `#if 0` draft. These are reconstruction references; unaccepted drafts are not proven semantic equivalents. Common issues:
- Historical (matched in wave 2): `DuelCmd_PrepareBattlePhase`: register allocation (masks in immediates vs hoisted negs; gCardIdToNumber cached vs pool load)
- Historical: `IsCardLinkedToMonster` / `FindMonsterLinkedToCard`: cardType computation hoisted outside inner loop; 3 high regs needed but only 2 saved (resolved in wave 1, see below)
- Historical (matched in wave 3): `GetZoneCardAttribute`: stack frame (`sub sp, #4`) missing; var_r7 in wrong register; loop pointer arithmetic differs
- `GetZoneCardType`: complex nested loops with many register allocation and branch pattern differences

### Bounded September 30 helper pass

Historical (CC18 matched in wave 1, D398 in wave 2, CAF0 in wave 3). Private candidates under `build/bigguns-early/` did not add a C match in this unit. For CC18, goto outer loops and initialized masked-ID boundaries recover some per-outer table work; explicit high-register pins or loop-counter pins either preserve wrong register lifetimes or add an entry check, so none were accepted. D398 goto-loop/staged-pointer variants reach the target size but still hoist the card table and fail the separate flags/card address lifetimes. CAF0 trials moved the target-card load before the kind guard and tested separate high-halfword/low-byte reference reads plus guarded loops; frame and scheduling remained different. Active source was unchanged throughout these trials.

### Paired compatibility searches and animation reconstruction

The September 30 post-checkpoint pass keeps **18/24 functions in C**, with no new enabled C in this unit. Whole-unit acceptance is still required, and isolated sizes, byte scores and normalized instruction diffs are diagnostics only.

- CC18/CD68 (superseded by the wave 1 match below): a per-outer initialized packed-ID temporary with an empty read/write constraint prevents the invariant card-type pointer from being formed before the player offset. Declaring that offset `s16` is lossless because `(p & 1) * 0xD64` is only 0 or `0xD64`. This combination restores the target save set and sizes (`0xB4`/`0xC0`), reducing the misses to 38/42 bytes. An empty input on the initialized outer counter before mask setup further reduces them to **30/34 bytes**. Remaining differences are initial mask/counter scheduling and the per-outer offset/type pointer register assignment. The same offset declared u16 or widened immediately to s32 does not reproduce the improvement. Initialized counter/offset dependency, explicit bindings and mask-width variants did not settle it. Private candidates: `build/decomp_large/game-batch/IsCardLinkedToMonster/outer-constraint-counter-before-mask-input/` and its CD68 sibling; grids `c894_s16_offsets.py`, `c894_outer_lifetimes.py`, `c894_outer_constraints.py`, `c894_outer_narrow.py`.
- Historical (CE28 matched in wave 3, see below): CE28: the parked ordinary-C draft follows the ROM state graph and preserves access widths, repeated helper calls, player-zero flag packing, wrapped u32 interpolation products followed by signed `/32`, and the final stage's **+3** fast-forward versus D234's +7. Both source-coordinate helpers are called again after interpolation; replacing them with cached coordinates would alter observable calls. Packed coordinates use unsigned shifts. The ordinary draft is `0x408` against target `0x40C`. Private initialized player-r6/case-4-one-sl lifetimes plus delta reuse and reversed flag OR operands recover exact size with **218 differing bytes**, still unaccepted. Combined entry pins, timer raw-mask rewrites, narrow slots, early returns, arithmetic association and case-6 pin variants failed. No pins or empty constraints were added to the parked ordinary draft. Evidence: `build/decomp_large/game-batch/DuelCmd_Attack/typed-v1/check.txt`, `combined-player-reuse-reverse-one/check.txt`; grids `c894_ce28_draft.py`, `c894_ce28_shapes.py`, `c894_ce28_combined.py`, `c894_ce28_lifetimes.py`.

> [!warning] Resolved local layout contradiction
> The earlier matching-notes sentence placed step and running in the same byte at `+0x80D`. ROM loads step at `0x02018DCA` (`gCmd+0x80A`) and clears running at `gCmd+0x80D`; the corrected sentence above records the actual separate locations. The existing active struct already used these separate offsets, so this documentation repair changes no emitted code.

A single CC18 register-allocation permuter run lasted three minutes with two workers and ended after 43,317 iterations with heuristic score 291 against a base of 311. Its only retained change was `s16 off` to `int off`, and no exact candidate was found. Earlier outputs are preserved in `build/decomp_large/cc18-pre-s16-permuter/`, and the new run's output is in `build/permuter/IsCardLinkedToMonster/output-291-1/`. Byte comparison of the initialized manual frontier remains authoritative, and the lower permutation score does not establish an improvement in exact byte count. After saving, the parked CE28 reference was checked (`build/decomp_large/c894-ce28-parked-check.log`): all `0x1010` bytes match, with actual **18/24 C** and six assembly fallbacks.

The final CC18 permutation change was checked in whole-unit context: int-offset versions miss 96 bytes in CC18 and 100 in CD68 at their exact sizes, worse than the manual signed16 30/34-byte frontier. The permutation result was rejected. Checks: `outer-final-int-offset/check.txt` under the two private function directories.

Historical (D398 matched in wave 2): a six-case D398 width pass tested the same lossless signed16 player offset, initialized counter placement, explicit player-base lifetime and flags-pointer form. No variant matched. The best reaches the target `0xA4` size but differs in 124 bytes. The earlier staged-pointer/loop/word-mask grid was inspected before these width combinations and was not swept again. The source and fallback are unchanged. Results: `build/decomp_large/game-batch/c894-d398-width-results.json`; script `build/decomp_large/c894_d398_widths.py`.

## `DuelCmd_PrepareBattlePhase` draft rewrite (2026-10-01)

Historical (matched in wave 2, see below). The parked draft went from 86 to 48 diff lines. In the ROM, `i = 0` is initialized before the hoisted player offset, so declarations are ordered `cmd, i = 0, off, base, pb, m9, m3`. The `+0x8C` flags byte is reached through a hoisted `pb = base + off`, and the card word through `zOff + off + base`. The remaining difference is that the ROM keeps `cmd` in r9 and reloads the `0x08622AB4` table and the 0x538 constant inside the loop, while the build hoists both and spills `cmd`. The function is queued for the permuter.

## Compatibility searches matched (wave 1, 2026-10-01)

`FindMonsterLinkedToCard` (start score 20) and its twin `IsCardLinkedToMonster` (start score 78) match in ordinary C, with no hints. The `s16`/`u32` offset hacks and constraints of the earlier frontier are gone. Working notes: `build/wf/FindMonsterLinkedToCard/NOTES.md`, `build/wf/IsCardLinkedToMonster/NOTES.md`.

- `u16 *tab = gCardIdToNumber;` as the **first local** at function top, indexed as `tab[cardId & 0x7FF]`. The pseudo is set at entry, so it has the longest live range and the lowest global-alloc priority (`floor_log2(refs) * refs / live_length`). It loses the last callee-saved register, and reload deletes its REG_EQUIV init and rematerializes `ldr rX, =gCardIdToNumber` at the use, which is the ROM's per-outer-iteration table reload. The hoisted constant 1 (for `p & 1`) then gets sl as in the ROM.
- The entry address is `(player & 1) * 0xD64 + slot * 0x94`; the inner loop needs the opposite order, `s * 0x94 + (p & 1) * 0xD64`.
- `u16 cardId`, `s32` counters, and `return (u8)p | ((u8)s << 8);` with a `u16` return type.
- `IsCardLinkedToMonster` matched by porting the `FindMonsterLinkedToCard` shape unchanged.
- Method: `old_agbcc -dL -dg` dumps. The `.loop` dump shows each movable's savings/lifetime/threshold decision; the `.greg` dump lists "Registers to be allocated in sorted order" with refs and live length. See [[matching-tricks]].

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/DuelCmd_PrepareBattlePhase/NOTES.md`.

### `DuelCmd_PrepareBattlePhase` (0xA4, start score 80; FAKEMATCH)

1. **Masks -9/-3 hoisted into r8/ip.** The `+0x8C` byte is written through a `u8` bitfield struct (`if (f->b3) { f->b3 = 0; f->b4 = 1; } f->b1 = 0;`). That gives SImode `mov #9; neg` chains that loop.c hoists, and merges the b3/b4 stores into one read-modify-write. An explicit `& ~8` narrows to `#0xF7`, and `s32 m9 = -9` locals spread the register pressure.
2. **Card table and 0x538 reloaded inside the loop.** The table goes through the integer-constant pointer `((u16 *)0x08622AB4)[id & 0x7FF]`: GCSE does not handle a const_int address, and loop.c finds a single insn with life 1 "not desirable"; the `gCardIdToNumber` symbol is a constant-pool MEM that GCSE/PRE hoists. 0x538 is tested with a **one-case `switch`**: with `==`, the HImode constant and its zero-extend chain (life 4, savings 2) are hoisted, the case compare is not. `0x538 == x`, `(s32)x == 0x538`, `!= ... goto` and the table-symbol form were all still hoisted.
3. **`adds r2, r0, r6` operand order:** an integer sum `z = (u8 *)(zOff + off + (u32)gBase)`; pointer arithmetic puts the base first.
4. **Prologue `ldr r0,=gCmd; movs r4,#0; mov r9,r0; ldrh r0,[r0]`.** The address is a short-lived `t` loaded before `i = 0` and copied into `cmd` after it; the hdr read goes through `cmd` (r9), so reload finds r0 via `find_equiv_reg` and the -9/-3 reloads shift to r1/r2. combine merges `t`'s load into the copy unless a memory store sits between them, hence the FAKEMATCH `asm volatile("" ::: "memory");` between `i = 0` and `cmd = t` (combine's `use_crosses_set_p` rejects a MEM source across a memory set). A plain `asm volatile("")` does not block it.
- Failed: `register ... asm("r9")` on `cmd` (right only when combine merged `t`, score 2); `t` pinned to r0 with the hdr read through `t` (a dummy reload put -9/-3 in r0/r1, score 10); `asm("+r")` on `t` or `cmd` (moved the load or added `mov r0, r9`); a `for` loop instead of `do/while` (2). `CMD_PLAYER()` inside the loop is not hoisted, because a QImode store may alias the hdr load.

## Wave 3 matches (2026-10-01/02)

Working notes: `build/wf/GetZoneCardAttribute/NOTES.md`, `build/wf/DuelCmd_Attack/NOTES.md`.

### `GetZoneCardAttribute` (0x128, start score 120; FAKEMATCH)

- Zone access through a pointer macro re-evaluated at every use (`CAF0_ZONE(p, s)` = `gDuelZones + (s*0x94 + (p&1)*0xD64)`): loop.c hoists the whole zone chain plus zone+0x4A and zone+0x8A, and cse2 turns the hoisted chain into copies of the entry pseudos (`mov r8, r2`, `mov r9, r4`). A local `z` pointer gives strength-reduced walking pointers instead (+16 bytes).
- The link's player byte is a separate `ldrb` from the same address as the `ldrh`: a u8 view struct, `CAF0ZoneB.linkBytes[i * 2]`. `(u8)links[i]` is CSE'd into the one `ldrh`, and `*(u8 *)&z->links[i]` folds the +0xA into the symbol.
- Linked zone: `lz = link >> 8; lp = byte & 1;` and a macro without its own `& 1`, with `(s*0x94 + p*0xD64)` parenthesised, give the ROM order (ldrh, lsr, ldrb, and, mul 0x94, mul 0xD64, add, add base). Card tables go through integer-constant pointers with `u16 tid` (table load after the index, the `ldr r2; adds r0, r2, #0` copy of 0x7FF); `u16 link` and `u8 kind` are read at the top of the loop.
- `u8 *flags = &gDuelNegationFlags` set before the loop: the spilled constant pointer is rematerialized after `movs r0, #3`, which also fixed every reload-register difference in the loop.
- FAKEMATCH: `asm("" :: "r"(gDuelZones));` at the top of the loop body gives the hoisted base 5 refs instead of 3. Its live length is doubled (REG_EQUIV symbol), and the extra refs lift it above zone+0x8A/zone+0x4A in global-alloc priority, so it gets r9 and zone+0x4A spills to `[sp]`. Removing it costs 42.
- An early `return result` for slot > 4 / face-down raises `result`'s refs from 4 to 5, so it is allocated before the zone pointer (r7 vs r8). Entry: `off = (player & 1) * 0xD64; zones = gDuelZones;` then `(zones + (slot * 0x94 + off))->card` loads the base before `slot*0x94` and adds `(s94 + off) + base`.
- Failed: `s16 i` (sign extension), a struct-array rvalue `gZ[p&1].zones[s].x` (`links[i]` associates as `base + (i*2 + s94 + off) + 0xA`), a union for `links` (agbcc pads it to 4 bytes), `3 & x` / `== 0` / `*(u8 *)0x0201ADAD` for the flag test order, `zones` for the linked zone (50).

### `DuelCmd_Attack` (0x40C, start score 186; ordinary C)

1. Case 3: the x/y coordinates are `s32` (the draft's `u16 x` added an `lsl/lsr` truncation).
2. Case 4 call order: `GetAreaX(other)`, `GetAreaY(other)`, then `dx -= GetAreaX(player)`, `dy -= GetAreaY(player)` (the draft called EC first).
3. Signed interpolation as separate statements: `dx *= gCmd.timer; dy *= gCmd.timer; dx /= 32; dy /= 32;` (the 7-bit u32 timer bitfield promotes to int, so no casts).
4. `dx += GetAreaX(player) + 8;`: fold turns `x + (call + 8)` into `(x + 8) + call`, the ROM's `adds r1,r5,#0; adds r1,#8; adds r5,r1,r0`; `dx + 8 + call` folds the other way. Fixes 1-4 put player/slot/otherSlot in r6/r7/r8 and kept the constant 1 in sl (CSE ties the second `fast & 1` constant to the pseudo from `1 - player`): score 40.
5. The `0x5200` argument is loaded straight into r2 after r0/r1 only when the 4th argument is a conditional expression: the precomputed constant pseudo is then set in one basic block and used in another, so local-alloc's `update_equiv_regs` replaces the use with the constant. Case 3: `(gCmd.timer * 4 + (player ? 0x40 : 0)) | 0x1000000` (fold distributes the `|` into both arms through a SAVE_EXPR, giving the ROM's odd arms and a shared `orr r3, r0`); case 4: `(tab[timer] << 16) | (player ? 0x40 : 0)`.
6. Case 4's packed position reuses `dy` (`dy = (dy << 16) | dx;`) so it stays in r4; a fresh `packed` variable took r2 and forced the timer-address reload into r4.
- Not needed: a u16-parameter prototype for `AddAffineSprite` (call through a cast) made no difference.
