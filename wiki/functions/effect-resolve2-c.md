---
title: Unit effect_resolve2 (duel card-effect executors, part 2)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit effect_resolve2

`0x08031BC8`-`0x08032CAF`, Thumb, `old_agbcc -O2`. Source: `src/effect_resolve2.c`. Continues [[effect-resolve1-c]] (same `int f(struct CardRef *ref)` effect executors, same step machine at `0x02017A40 + 0x3E0`, same `CardRef` layout: `+0xA` bits 0-2 `numTargets`, `+0xC` `targets[]`). The condition predicates for these are in [[effect-prepare1-c]]; the target checks called here (`EffectDragonSeekerCheck`, `EffectDestroyByTypeCheck`, `IsZoneTargetable`) are in [[card-list-viewer-c]] / [[effect-checks-c]].

Unit status: `unit bytes MATCH`, **16/18 functions in C** after workflow waves 2-3 (2026-10-02: `0x080326C4` in wave 2); 2 stay `INCLUDE_ASM` (`0x08032390`, `0x0803283C`). Verified with `tools/check.py effect_resolve2`. Before the waves the line said 14/18, but the table already listed 15 matching functions (`EffectHirosShadowScoutResolve` had matched since), so the old count was one short.

> [!warning] Contradiction: the unit is now 18/18
> The count above (16/18) predates later matches. `src/effect_resolve2.c` has no `INCLUDE_ASM` left (checked 2026-10-02), so all
> 18 functions are in matching C. The decompilation reached 100% at commit `d77fcef` ([[overview]]). Resolved in favour of
> the source. Text below that calls a function nonmatching, parked or `INCLUDE_ASM` is history. Some of the later matches
> are recorded only in git (`git log`) and not yet written up here.

The unit uses the shared header `include/duel.h` (`#include "duel.h"` after `global.h`). It does not touch `struct Main`/`gMain`, so `include/main.h` is not needed. It was migrated on 2026-09-30, dropping the local definitions of `struct DuelCard`, `struct DuelZone`, `struct DuelZonesPlayer`, `struct PlayerLP` (replaced by the canonical `struct DuelPlayer`) and `struct PlayerDeck`, plus the local externs of `gDuelZones`, `gDuelPlayers`, `gDuel` and `gDuelDecks`; output bytes unchanged.

### Local views kept (and why)
- `struct DuelStateTail` / `extern struct DuelStateTail gUnk_020192E0Tail asm("gDuel")`: `duel.h`'s `struct DuelState` only covers the two players (size `0x1ACC`), but `EffectReverseTrapResolve` reads the flag byte at `+0x1ACD`, just past that range. The alias renames the symbol back to `gDuel`; access is now `gUnk_020192E0Tail.flags1ACD`.


## Functions

| Address | Size | Status | Purpose (hypotheses about role, verified about logic) |
|---|---|---|---|
| `0x08031BC8` | 0x4C | matching | one target accepted by `EffectDragonSeekerCheck(ref, pos)`: `DestroyFieldCardByEffect(tp, tz)`, `OnCardDestroyedByEffect(player, tp, tz)` |
| `0x08031C14` | 0x11C | matching | one target holding a card: for card numbers 0x3FF / 0x4BB, a set (flag `& 3 == 1`) card 0x4B1: message 0x7F + `ShowActivatedCard`; else (trap type 0x16 in a monster zone needs `IsZoneTargetable`), message 0x8B if the target is on the other side, `DestroyFieldCardByEffect` + `OnCardDestroyedByEffect` |
| `0x08031D30` | 0x50 | matching | one target holding a card -> `ReturnFieldCardToHand(tp, tz, 0)` |
| `0x08031D80` | 0x28 | matching | `SendTopDeckCardsToGraveyard(1 - p, 5, 1)` |
| `0x08031DA8` | 0xB8 | matching | one target holding a face-down card on the other side: message 0x7F, `ShowCardDetail(tp, id)`, message 0x92, message 0x7F again |
| `0x08031E60` | 0xD0 | matching | opponent's zones 5-9: face-up card with number 0x15B -> `DestroyFieldCard(1 - p, i, 1)`; if any: message 0x47 |
| `0x08031F30` | 0x7C | matching | two targets: each that holds a face-down card -> `DestroyFieldCard(tp, tz, 1)` |
| `0x08031FAC` | 0xAC | matching | step machine 0x80 / 0x7F: hand-count checks then `DiscardHandCard`; other steps call `DrawCards(p, 5)` for both players |
| `0x08032058` | 0x6C | matching | up to two targets that hold a card -> `ReturnFieldCardToHand(tp, tz, 0)` |
| `0x080320C4` | 0x104 | **matching** (FAKEMATCH) | 3 iterations over the opponent's deck (`0x02019AA8` list): message 0x61 / 0x8061, then `ShowDestroyedCard(p, id)` + `DiscardHandCard(1 - p, count, 1, 1)` for a Trap (type 0x16), else `ShowRevealedCard(p, id)` and `count++` (count starts as the opponent's hand count) |
| `0x080321C8` | 0x90 | matching | one target of the other side, own zone and target zone occupied -> `SwapFieldCards(p, zone<<8 \| p, tp \| tz<<8)` |
| `0x08032258` | 0xC4 | matching | two parts: if `kind == 0x10`, the "pos" target (`+6`) on the other side with flag bit 0 clear -> `ChangeBattlePosition(tp, tz, 0, 0)`; then for one target: (occupied, face-up, own side) -> `EquipCard`, otherwise `DestroyFieldCard(p, zone, 1)` |
| `0x0803231C` | 0x74 | matching | for both sides (opponent first), zones 0-4 accepted by `EffectDestroyByTypeCheck(ref, i << 8 \| side)`: `DestroyFieldCard(side, i, 1)` + `OnCardDestroyedByEffect`. A second copy of `p` (`q`) re-assigned from `pu` inside the loop forces the `lsrs r7,r0,#24` schedule (found by the permuter) |
| `0x08032390` | 0x2DC | **nonmatching (asm)** | step machine 0x7F/0x80 over a player's zones with card-type/stat-based thresholds (hypothesis: type 21-23 -> 0, 24 -> 4000, else a stats field * 10 compared with 0x5DB); not attempted |
| `0x0803266C` | 0x58 | matching | opponent's zones 5-10 that hold a card -> `DestroyFieldCard(1 - p, i, 5)` |
| `0x080326C4` | 0x130 | **matching** (wave 2, 2026-10-01; FAKEMATCH) | one target, phases 0x80 (needs a set card: `FlipFieldCard`, returns 0x7F) and 0x7F (`GetZoneCardDef > 2000` -> `ShowRevealedCard` + `FlipFieldCard`; else `ShowDestroyedCard`, the 0x1640 event `Chain_AddPending(packed)` when `HasFlipEffect` allows it and neither player has card 0x5FA, then `DestroyFieldCardByEffect`, `OnCardDestroyedByEffect`). See [Wave 2 matches](#wave-2-matches-2026-10-01) |
| `0x080327F4` | 0x48 | matching | message 0x1C (0x801C for player 1) with argument `!(gDuel[0x1ACD] >> 5 & 1)` |
| `0x0803283C` | 0x474 | not attempted (asm) | large step machine |

## Globals (in addition to [[effect-resolve1-c]])
- `0x020192E0 + 0x1ACD` byte: bit 5 tested by `0x080327F4` (hypothesis: a duel flag, near `+0x1B12` used in [[effect-prepare1-c]]). Beyond the `struct DuelState` declared in `duel.h` (ends at `+0x1ACC`), so read through the unit-local `gUnk_020192E0Tail` view.
- `0x02019AA8` = `0x020192E4 + 0x7C4`: each player's deck list (stride 0xD64, 80 card words); now expressed as the canonical `gDuelPlayers[p].deck[]` from `duel.h`.
- `Chain_AddPending(packed, 0)`: takes `player << 31 | (zone & 0x1F) << 16 | 0x16400000 | card id` (hypothesis: enqueue an animation/event).

## Matching tricks
Everything in [[effect-resolve1-c]] applies. Extra findings:
- **`int tp = (u8)ref->targets[0];`** (not `u8 tp`) avoids an extra callee-saved copy of `tp` when it is used as an argument to `u16` parameters (`EffectPatrolRoboResolve`); and a prototype `int id` instead of `u16 id` fixed operand order of the argument moves.
- **`for (i = 0; i < ref->numTargets && i <= 1; i++)`** with `ref->targets[i]` written out each time matches the ROM's loop (`EffectPenguinSoldierResolve`); `do`/`while` shapes strength-reduce the address instead.
- **Ternary in the message argument** is sometimes inverted in the ROM (`(1 & byte2) ? 0x61 : 0x8061`): look at whether the default `movs r1,#imm` sits *before* the `cmp` and which way the branch goes.
- **`1 & ~(byte >> 5)`** matches as `bic` only when assigned to a temporary first (`int f = ...; call(msg, f, 0, 0)`), or written `!((byte >> 5) & 1)`.
- **Second read of `ref->id`** (`EffectDestroyTargetResolve`): old_agbcc CSEs the load into a callee-saved reg, the ROM reloads it. A `volatile u16` read forces the reload. The ROM further loads the `0x7FF` mask *before* that volatile `ldrh`: putting the mask in a local (`u32 m = 0x7FF; u16 vid = *(volatile u16 *)ref;`) makes the constant load come first.
- **Signed compare from a `u8`** (`EffectDestroyTargetResolve`): `u8 tz` is needed for the ROM's register choice (`tz` lives in r6), but `tz <= 4` then compiles unsigned (`bhi`). Copying `tz` into an `int` local immediately before the compare (`tzi = tz; ... tzi <= 4`) gives the signed `bgt` at no extra instruction cost.
- **Permuter `FAKEMATCH`** (`EffectDestroyAllByTypeResolve`): a redundant second copy of `p` (`pu`/`q`) that is re-assigned inside the loop shifts the `lsrs r7,r0,#24` schedule to match.
- **`((ref->player & 1) ^ 1) & 1`** (`EffectHirosShadowScoutResolve`, FAKEMATCH): `(ref->player & 1) ^ 1` fixes the `eor` operand order, and the redundant outer `& 1` moves `movs r1, #1` after the bit extraction. Six other spellings (`!p`, `p ^ 1`, `1 ^ (p & 1)`, a `u8` copy, `+ 0`, `== 0`) do not.

## Open problems

Remaining `INCLUDE_ASM`: `EffectCrushCardResolve` (first C draft, 192 lines, 2026-10-01), `EffectFakeTrapResolve`. (Historical: `EffectAcidTrapHoleResolve` was on this list until it matched in wave 2.) Each keeps its original assembly; decoded drafts and current mismatch notes remain guarded by `#if 0` in the unit source.

## Verified C conversions (2026-09-30)

- `EffectInvaderOfTheThroneResolve` (Thumb, `0x90` bytes) is matching C. It is a single-target cross-player zone action. An empty r8 clobber at entry gives the ROM allocation of the two zone copies.

The compiler hints emit no instructions. Each conversion passed a whole-unit byte comparison with the baserom; remaining assembly functions retain their original bytes. The matching C above resolves earlier notes that described these functions as allocation near misses.

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/EffectAcidTrapHoleResolve/NOTES.md`.

### `EffectAcidTrapHoleResolve` (0x130, start score 74; FAKEMATCH)

- `int p` instead of `u16 p`: the u16 put the `and` in a temp and moved it; `int` gives the regmove copy `p = tp; p &= r0` with r0 the CSE'd 1 from `numTargets == 1`.
- `ShowRevealedCard` called through `(void (*)(int, int))`: the unit's u16 prototype adds a narrowing insn that swaps the r0/r1 argument moves and changes the refs of `id`, which lost r6 (111 to 67).
- Packed event argument in separate statements: `hi = (u32)p << 31; ev = (tz & 0x1F) << 16 | 0x16400000; hi | ev | id`. In one expression fold-const's associate step moves the constant next to `p << 31` whatever the parentheses (67 to 61).
- FAKEMATCH: `register int p asm("r8")` plus `p = tp; asm("" : "+r"(p)); p &= 1;`. Global-alloc priority put p (refs 5, live 54) above the CSE'd 0x5FA constant (refs 3, live 18), so p took r7; the ROM has the constant in r7, p in r8, ref in r9. Without the empty constraint, combine merges the copy into the `and` and reload emits `ands r0,r4; mov r8,r0`. Natural C would need p's live length in 61..75 (cleanup idea: a use of p on the `> 2000` path, hypothesis). The function-pointer cast is also commented FAKEMATCH in the source.
- Failed: `u8 p` (constant right, but p's 3 refs rank it below ref), `u32`/`u16 id`, `u8 tp`, `u32 tz`, `1 & tp`, every one-expression ordering of the packed argument, `((tz & 0x1F) | 0x1640) << 16`.
