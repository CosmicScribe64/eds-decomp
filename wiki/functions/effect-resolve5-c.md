---
title: Unit effect_resolve5 (duel card-effect executors, part 3)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit effect_resolve5

`0x08035198`-`0x080361CF`, Thumb, `old_agbcc -O2`. Source: `src/effect_resolve5.c`. Continues [[effect-resolve2-c]] (same `int f(struct CardRef *ref)` effect executors, step machine at `0x02017A40 + 0x3E0`, `CardRef` layout from [[effect-resolve1-c]]). Same idioms; see the tricks there.

Unit status: `unit bytes MATCH` (`tools/check.py effect_resolve5`), **25/25 functions in C** after workflow waves 2-3 (2026-10-02: `0x08035198` in wave 2); none stay `INCLUDE_ASM`. After wave 1: 24/25 (2026-10-01: `EffectHeavyStormResolve`, `EffectSnatchStealResolve`, `EffectFairysHandMirrorResolve` added). Before wave 1: 21/25. Latest complete-unit check: `build/middle_experiments/EffectAttackResponseResolve/unit-check.log`, all 0x1038 bytes exact.

## Shared headers (migration 2026-09-30)
`src/effect_resolve5.c` has `#include "duel.h"` for the canonical duel structs (`struct DuelCard`, `struct DuelZone`, `struct DuelPlayer`, `struct DuelZonesPlayer`) and the `gDuel` / `gDuelPlayers` / `gDuelZones` externs. Four local struct definitions were removed (`DuelCard`, `DuelZone`, `DuelZonesPlayer`, `PlayerLP`) together with their externs. `PlayerLP` was the unit's tag for `struct DuelPlayer` and was unused, so its uses simply dropped. `include/main.h` is not included because this unit never references `gMain`/`struct Main`. Every field this unit reads has the same offset and type in the headers, so no code changed (`unit bytes MATCH`, built 0x1038 = target 0x1038). `ZFLAGS(z)` stays a raw byte view of the zone flags byte at `+0x06` (canonical `DuelZone.flag6_0/flag6_1/counter6`).

Unit-local views kept (the canonical header does not cover these):
- `struct DuelGlobals` over `gDuel`: canonical `struct DuelState` only describes `+0x0000..+0x1ACC` (u32 + two `DuelPlayer`s), while this unit reads `+0x1B12` bit 1 and the `u16` at `+0x1B64`; `DG` casts `&gDuel`.
- `struct CardRef`, `PlayerCard5F0`, `PlayerDeck`, `AE60`, `DuelScreen82C`/`DuelScreenView`, `ListView`, `EffState`/`EffEntry`, `PlayerWords`: views of RAM/ROM globals that no shared header defines.

## Functions

| Address | Size | Status | Purpose (hypotheses about role, verified about logic) |
|---|---|---|---|
| `0x08035198` | 0x17C | **matching** (wave 2, 2026-10-01) | `(ref, arg)`: 4-step machine 0x80/0x7F/0x7E/0x7D: `EFF_SIDE = 3` counter; `EffectTheCheerfulCoffinPrepare(ref, arg, 0)` then a text box (`gStrCheerfulCoffinDiscardPrompt`); 0x7E text `gStrCheerfulCoffinSelectMonster` when `0x0201AE60+0x14` set; 0x7D `DuelCursor_PickTarget(1)` key wait, then the card word (u32) at `0x02019968[p][w82C]` with type <= 0x14: `PlaySE(1)`, message 8/0x8008, `DiscardHandCard`, `EFF_SIDE--`, return 0x7F; else `PlaySE(3)` |
| `0x08035314` | 0x90 | matching | both sides (opponent first), zones 0-4 with `unk7 & 0x40`: `DestroyFieldCardByEffect`, `OnCardDestroyedByEffect` |
| `0x080353A4` | 0xDC | matching | one target on the other side, `FindFreeMonsterZone(player) != -1`: card 0x4B1 set -> message 0x7F + `ShowActivatedCard`; else `MoveFieldCard` + `QueueAddZoneLink(.., 3)` |
| `0x08035480` | 0xAC | matching | `(ref, u16 *idp)`: idp set: type 0x15/0x16 -> message 0xB0; else phase 0x80: `EffectSolemnJudgmentPrepare` -> message 0x91 (pos), return 0x7F; other phase `DestroyFieldCard(pos, 1)` |
| `0x0803552C` | 0x54 | matching | phase 0x80: message 0x91 for `ref->pos` target, return 0x7F; else `DestroyFieldCard(tp, tz, 1)` |
| `0x08035580` | 0x40 | matching | `n = CountMonsters(opp)`; if n > 0 `LoseLifePoints(opp, n * 500)` |
| `0x080355C0` | 0x54 | matching | own `AddDeckCardToHand(p, 0x3EB)` or `0x40A` -> message 0x60 |
| `0x08035614` | 0x54 | matching | one target, flag bit 0 clear -> `ChangeBattlePosition(tp, tz, 1, 0)` |
| `0x08035668` | 0xFC | matching | both sides, zones 0-10: occupied and face-up-ish (flag 2 clear): messages 8, 0x7F, `ShowCardDetail`, 0x7F |
| `0x08035764` | 0x18 | matching | `ReturnFieldCardToHand(pos target, 0)` |
| `0x0803577C` | 0xEC | matching | list-viewer step machine (0x80 `EffectLastWillPrepare` then `CardListView_Open(p, -1, number, 0)` -> 0x7F; 0x7F message 0x65 -> 0x7E; 0x7E `QueueSpecialSummonChoosePosition` -> 0x7D; 0x7D message 0x60 -> 0x64); like [[effect-resolve1-c]] `0x08030B88` |
| `0x08035868` | 0x44 | matching | `(gDuel[0x1B12] >> 1 & 1) != player` -> message 0x36 |
| `0x080358AC` | 0x310 | matching (ordinary C) | `switch (CARD_NUMBER(ref->id))` 0x420 / 0x44A / 0x4BE / 0x587 / (default 0x2AD with steps 0x80 and 0x78); see below |
| `0x08035BBC` | 0x50 | matching | phase 0x80: `CountTributableMonsters(opp, -1)` -> `DuelPrompt_PostTribute(opp)`, return 0x7F |
| `0x08035C0C` | 0xA4 | **matching** (wave 1, 2026-10-01; FAKEMATCH) | phases 0x80/0x7F: scans the side `EFF_SIDE`'s zones 5-10; a hit -> `DestroyFieldCard(side, j, j)`, return 0x7F; else flips the side, returns 0x7F once it is `ref->player` |
| `0x08035CB0` | 0x94 | matching | both sides, zones 0-4 holding a card: `ChangeBattlePosition(i, j, 1, 1)`; message 0x48 |
| `0x08035D44` | 0x34 | matching | `DrawCards(p, 1)`, `GainLifePoints(opp, 1000)` |
| `0x08035D78` | 0x94 | matching | both sides, zones 0-10 holding a card (zones 0-4 also need `IsZoneTargetable`): `DestroyFieldCardByEffect`, `OnCardDestroyedByEffect` |
| `0x08035E0C` | 0x10C | **matching** (wave 1, 2026-10-01) | card 0x42C set-up face-down own card (`(ref->flags4 & 8) == 0`, zone flag 2), one target of the other side: `EquipCard`, `MoveFieldCard` |
| `0x08035F18` | 0x68 | matching | 0x80: `DuelPrompt_Post(p, 6, 0, 0)` -> 0x7F; 0x7F: `DiscardHandCard(opp, w1B64, 1, 1)` -> 0x64 |
| `0x08035F80` | 0x5C | matching | 0x80: `DuelPrompt_PostRandomDiscard(opp, 1, 1)` -> 0x7F; 0x7F: `DuelPrompt_PostDiscard(opp, 1, 0, 1)` -> 0x7E |
| `0x08035FDC` | 0x54 | matching | one target with flag 2: `FlipFieldCard(tp, tz, 0)` |
| `0x08036030` | 0xB8 | **matching** (wave 1, 2026-10-01; FAKEMATCH) | **dispatcher**: `(ref, card)`: at phase 0x80 copies the CardRef (0x14 bytes, `MemCopy16`) into `0x02017A40+0x4E4`, overrides `id`/`player` from `card`, picks `fn = gCardEffects[FindCardEffect(card->id)].fn` (0x18-byte entries, fn at +4) into `+0x4F8`; then `EFF_PHASE = fn(&cur, 0)`; a 0 result or a null fn prints message 0xB0 and returns 0 |
| `0x080360E8` | 0x68 | matching | two targets: `EffectTailorOfTheFickleCheck`, `IsValidEquipTarget(p0, z0, p1, z1)`, `FindMonsterLinkedToCard(p0,z0) != target1` -> `MoveEquipCard(target0)` |
| `0x08036150` | 0x80 | matching | 0x80: `DuelPrompt_Post(p, 6, 0, 0)` -> 0x7F; 0x7F: `ReturnHandCardToDeck(opp, w1B64, 1)`, message 0x60 -> 0x64 |

`0x080358AC` handles these card numbers:

- 0x420: the opponent-of-target zones 0-4 with flag 0 clear get `DestroyFieldCardByEffect` and `OnCardDestroyedByEffect`.
- 0x44A: a face-up target calls `GainLifePoints(p, GetZoneCardAtk(tp, tz))`.
- 0x4BE: the same, with message 0x3B and `LoseLifePoints`.
- 0x587: calls `QueueAddZoneLink(p, id, pos, 3)`.
- 0x2AD: in phase 0x80 it finds the face-up zone with the greatest `GetZoneCardAtk` value. On a tie it asks the player (`FormatInt` builds text from `gStrWidespreadRuinTiePrompt`, the value is saved in `0x02017A40+0x542`, step 0x78). Phase 0x78 waits for a key and compares.

## New structs and globals
- `0x02017A40` effect state (larger view): `+0x3E0` phase, `+0x3E1` side, `+0x4E4` a working copy of the `CardRef` (0x14 bytes), `+0x4F8` executor function pointer (`u8 (*)(struct CardRef *, int)`), `+0x542` u16 saved value.
- `gCardEffects` (ROM, 0x18-byte entries): table indexed by `FindCardEffect(id)`; `+4` is the executor function pointer (hypothesis: effect handler table).
- `0x020192E0` (duel globals, unit-local view `struct DuelGlobals`): `+0x1B12` bit 1 a player index, `+0x1B64` u16 (passed to `DiscardHandCard` / `ReturnHandCardToDeck`).
- `0x0201CFB0`: `+0x824` u16, `+0x828` u8, `+0x82C` u32 (a selected entry index).
- `0x02019968` = `0x020192E4 + 0x684`: a per-player (stride 0xD64) list of card words.

## Matching tricks (in addition to [[effect-resolve1-c]], [[effect-resolve2-c]])
- **Range check as `switch`.** `type == 0x15 || type == 0x16` compiles to `sub #21; cmp #1; bhi`, but `int type = ...; switch (type) { case 0x15: case 0x16: ... }` gives the ROM's signed `cmp #22; bgt; cmp #21; blt`.
- **Struct view for big byte offsets.** `((struct DuelGlobals *)gDuel)->b1` gives `ldr base; ldr #off; add` like the ROM; a plain `gDuel[0x1B12]` byte access does the same, but a `u16` read at a constant offset folds into a single pool literal. A struct field (`DG->w1B64`) reproduces the ROM's separate `add`. For a `u16` in `gChain` (+0x542) use a struct field too.
- **`int n = 7 & ((u8 *)ref)[0xA];` then use `n` for `& n`** (as `p = tp & n`, `flags & n`): reproduces `movs r3,#7; ldrb; ands` and reuses `r3` as the constant 1 (`EffectBlockAttackResolve`, `EffectChangeOfHeartResolve`, `EffectDarknessApproachesResolve`).
- **`int tp = (u8)ref->targets[0];`** (not `u8 tp`) avoided a callee-saved copy (`EffectChangeOfHeartResolve`); the ROM passes `ref->player | (u8)r << 8` written inline twice (no temp).
- **First-loop side selection**: `if (i) p = ref->player; else p = 1 - ref->player;` and `ZB(p & 1, j)` written inline (no `p2` local) matched `EffectCallOfTheDarkResolve`/`EffectFinalDestinyResolve`/`EffectTheSternMysticResolve`. In `EffectTheSternMysticResolve` an argument `(u8)j << 8` also made the ROM's hoisted `(u16)p` appear.
- **Ternary message operand order**: `ref->pos` low byte test uses `(u8)ref->pos ? 0x8091 : 0x91` written directly in the call (not through a `tp` local) to get `ldrh` before `ldrb` (`EffectHornOfHeavenResolve`).
- **Bitfield read** `(u32)gUnk[..] << 30 >> 31` vs a `bitfield` struct: use a bitfield struct member, otherwise `asr`/`lsr` scheduling differs (`EffectWabokuResolve`).

- **No local pointer to a global struct** (`struct ListView *lv = &gCardListView;` made gcc keep it in a callee-saved reg): write `gCardListView.row` directly and let CSE find the base. `u32 *card = &gCardListView.cards[row + top];` at the top plus a second `&gCardListView.cards[row + top]` in the `0x7E` case matched `EffectLastWillResolve` (the ROM recomputes it there).

## Open problems
Historical (matched in wave 2): the `#if 0` block for `0x08035198` was the last open problem (`0x08035C0C`, `0x08035E0C` and `0x08036030` matched in wave 1, see below). The unit is now fully in C and has no `#if 0` drafts left.

## Exact stat-selection executor follow-up

`EffectAttackResponseResolve` matches all 0x310 bytes in ordinary C, with the original `int(struct CardRef *)` ABI and no compiler hints. The whole 0x1038-byte unit is exact after enabling it.

> [!warning] Contradiction resolved
> The historical draft annotation claimed only case-0x78 block order differed. A fresh isolated compile also exposed an incorrect local `EffState` layout: `CardRef` occupies 0x10 bytes, whereas the state reserves a 0x14-byte working copy at +0x4E4. Without four explicit reserved bytes after `cur`, the function pointer and saved stat compiled at +0x4F4/+0x53E rather than the ROM's +0x4F8/+0x542. The local state view includes that gap; the `CardRef` definition and external ABI are unchanged. The ROM's dispatcher copy length and executor stat accesses independently establish these offsets.

Placing `default: return 0;` **after** the phase switch's case-0x78 failure block preserves success before failure and the shared zero-return block last. This ordinary source ordering fixes the remaining tail and pool placement. A default before case 0x80 or case 0x78, positive/negative guard forms, explicit success returns, and moving the failure label outside the switch stayed nonmatching in a bounded grid. Private evidence: `build/middle_experiments/effect_stat_tail.py`, `EffectAttackResponseResolve/match.c`, and `EffectAttackResponseResolve/private-unit-check.log`; live acceptance: `EffectAttackResponseResolve/unit-check.log` (all under `build/middle_experiments/`).

## Private dispatcher frontier

> Superseded: `EffectFairysHandMirrorResolve` matches since 2026-10-01; see [the wave 1 section](#wave-1-matches-2026-10-01).

`EffectFairysHandMirrorResolve` remains ASM. A new baseline has 176 differing bytes at 0xBC versus the ROM's 0xB8. A private word-return callback view, separate setup/callback scopes, an initialized wide base, and a wide phase offset recover all but the reload at `0x0803608C`. The ROM uses `ldr r4` from the existing state literal, while the C reuses the setup base with `adds r4,r6,#0`. This candidate is exactly 0xB8 bytes but is **not an accepted C conversion**. Record: `build/middle_experiments/EffectFairysHandMirrorResolve/two-byte.c` and `return-diff.txt`.

The ROM callback call stores r0 as a byte before testing the low byte. A narrow `u8` callback view instead normalizes the returned value before that store in the new source shape. Scanning all 426 24-byte table records at `0x0819A9D4`, field +4, gives 404 nonnull handler entries: current source has 324 `int` and 53 `u16` signatures, while 27 are ASM-only and have no C signature; none has a `u8` signature. These are entry counts, not unique-function counts or a proof of formal original C types. Both integer widths use the machine's r0 result; the caller consumes only its low byte. The private view leaves all external definitions unchanged. Evidence: `callback-return-evidence.json` in that directory. Enabled callback views have not been changed.

Literal/symbol/alias base forms, initialized base/offset constraints, narrow/wide offset grids and callback-result staging did not remove the final reload discrepancy. A one-minute two-worker permutation run reached score 0 at iteration 1656 (196 errors), with branch-target checking enabled, by moving `p=0` into a failure path where p is never read. That dead assignment is rejected and stays private. Removing it and using the pointer for a real initial phase read did not match. No source or coverage change resulted.

## Bounded side-scan follow-up

> Superseded: `EffectHeavyStormResolve` matches since 2026-10-01; see [the wave 1 section](#wave-1-matches-2026-10-01).

`EffectHeavyStormResolve` remains parked after a 2026-09-30 bounded batch. Staged phase pointers/offsets, late base assignment, a walking zone pointer, a found-call tail, flat guards, initialized pointer barriers, and explicit side rereads did not produce an exact candidate. Loading the step before assigning the effect base improves the isolated draft to 95 differing bytes at 0xA8 (target 0xA4). The ref/base/counter homes remain wrong, the phase/side offsets do not share the target constant, and the stored side is forwarded instead of reread. The private record is `build/middle_experiments/EffectHeavyStormResolve/load-first-95.c`; this is a partial shape observation, not matching C.

## Wave 1 matches (2026-10-01)

Three of the four fallbacks matched in workflow wave 1; the whole unit check reports 25/25 including the fallback, bytes exact. Working notes: `build/wf/<func>/NOTES.md`.

### `EffectHeavyStormResolve` (0xA4, start score 66; FAKEMATCH)

Ported from the `EffectDarkHoleResolve` match in [[effect-resolve1-c]], then two fixes:
- Same switch/fallthrough head and the same r0-pinned tail pointer FAKEMATCH as `EffectDarkHoleResolve`.
- Read the side **inside** the loop (`int side = EFF_SIDE;` in the body): loop.c hoists it after `j = 5` (the ROM order) and the found call reuses the hoisted register.
- Zone pointer `&gDuelZones[(u8)side & 1].zones[j]` (struct indexing, not `ZB()`: base + p*0xD64 is built first, then +5*0x94). The `(u8)` cast gives the ROM's `mov r1,#1; add r0,r4,#0; and r0,r1` and also fixes the global allocation (ref r3, side r4) without a pin on ref.
- Failed: `ZB()` order (adds the symbol inside the loop), side read outside the loop, `u8 side` (48), `side % 2`, `1 & side`, `int p = side; p &= 1` (163); pinning side to r4 inside the loop kills loop-invariant motion (204).

### `EffectSnatchStealResolve` (0x10C, start score 40; ordinary C)

Register allocation only. The parked draft wrote the packed arguments shifted term first (`ref->zone << 8 | ref->player`, `(u8)r << 8 | ref->player`). Putting `ref->player` first in both ORs (`ref->player | ref->zone << 8`, then `ref->player | (u8)r << 8`) matches after two experiments. OR operand order in packed `(player | x << 8)` arguments drives agbcc's register choices across the whole function, not just local scheduling.

### `EffectFairysHandMirrorResolve` (0xB8, start score 18; FAKEMATCH)

- A private int-returning view of the callback, `(*(EffFn36030 *)&e->fn)(&e->cur, 0)`; the shared `u8`-returning `ES->fn` zero-extends after the call, which the ROM does not.
- A plain inner-block local `struct EffState *e = ES;` for the call half gives the ROM's base reload `ldr r4,=0x02017A40`, so the `unsigned long long` base/offset permuter casts are not needed.
- The success return goes after the fail block (`if (*p != 0) goto success;` with `success: return *p;` at the end).
- FAKEMATCH: a dead `p = 0;` before `goto fail` in the null-fn branch. Without it, gcc shares the phase address across the join and homes ref in r8 (score 52); `u8 *p = 0;` at the declaration does not work.
- Failed: straight-line form without `e`, using `ES` directly (21); declaration init `p = 0` (52); the natural form with no goto (75).

> [!warning] Contradiction
> The [Private dispatcher frontier](#private-dispatcher-frontier) section above records that a permuter score-0 result "moving `p=0` into a failure path where p is never read" was **rejected** and kept private. Wave 1 (`build/wf/EffectFairysHandMirrorResolve/NOTES.md`) accepted the same kind of dead store as a commented FAKEMATCH. Resolution: the current matching rules in `CLAUDE.md` accept "the permuter's odd-but-valid C" when commented. A dead store of the constant 0 to a pointer that is never read on that path is defined, behavior-preserving C (unlike the rejected unset-scratch reads elsewhere), so the wave 1 acceptance stands under the current rules. The older rejection reflected a stricter policy at the time.

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/EffectTheCheerfulCoffinResolve/NOTES.md`.

### `EffectTheCheerfulCoffinResolve` (0x17C, start score 92; ordinary C)

The NOTES mention FAKEMATCH only to say none was needed; the final source has no FAKEMATCH comment for this function.
- The parked draft was wrong in two places: the ROM calls `PlaySE(1)`, then `DuelCmd_Push`, then `DiscardHandCard`; and the card is read as a `u32` word, not `s8`.
- `int p = ref->player & 1;` (92 to 34). The literal `& 1` forces a constant-1 pseudo; CSE reuses it for the later `ref->player ? 0x8008 : 8` test (fold makes that `byte & 1`). Combine merges the `one = 1`, the `lsr`/zero-extend and the `and` into a PARALLEL of two SETs and places `one = 1` at i2's position, between the `lsl` and `lsr` (ROM `lsl r2,r4,#31; movs r4,#1; lsrs r2,r2,#31`), with the constant kept in a callee-saved register. `ref->player & one` with a local `int one = 1`, or `1 & byte`, gave `ands` instead of the shifts.
- `ds->b828 | (u8)*sel << 8` with the b828 operand first (34 to 8), the same operand-order effect as `EffectSnatchStealResolve` above.
- `*(u32 *)((u32)gDuelHands + *sel * 4 + p * 0xD64)`, integer address arithmetic as in [[effect-activation-c]] (8 to 0): the ROM loads the base late. The struct-array form `gDuelHands[p].w[*sel]` loads it early; a constant-pointer cast `((struct PlayerWords *)0x02019968)[p]` gives the wrong add order.
