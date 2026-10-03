---
title: Unit effect_targets4 (duel target-selection prompts, part 2)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit effect_targets4

`0x08040EBC`-`0x08041F9B`, Thumb, `old_agbcc -O2`. Source: `src/effect_targets4.c`. Continues [[effect-targets3-c]] (same "selector" family: `int f(struct CardRef *ref)`, step byte `0x02017A40 + 0x3E5`, prompt text via `TextBoxOpen(0x206, 0x712, 0xB, text)`, cursor at `gDuelScreen + 0x824/0x828/0x82C`, `TryAddEffectTarget(ref, player, zone)` adds the picked zone, `PlaySE(3)` plays the "cannot pick" sound, and the cancel bit is `gMain+6 & 2`). The last function is a different kind (a validity test that fills a `CardRef`).

Unit status: `unit bytes MATCH` (0x10E0 bytes), **14/14 functions in C** after workflow waves 2-3 (2026-10-01: `0x08041898` in wave 2); none stay `INCLUDE_ASM`, so the unit is complete in C. After wave 1: 13/14 (2026-10-01, `EffectBlockMonsterZonesChainB` added). Rechecked with `tools/check.py effect_targets4` on 2026-09-30 after enabling `EffectDeclareAttributeEquipChainB`.

| Address | Size | Status | Purpose (hypotheses) |
|---|---|---|---|
| `0x08040EBC` | 0xF4 | matching | prompt = name of card `gUnk_086248EE[0]` (`FormatStr`, `gStrSelectAttackTargetFmt`); keys `0xE0`; the cursor zone must hold a face-down card (`flags6 & 2`) whose card number is 0x57D -> `AddEffectTargetUnchecked` |
| `0x08040FB0` | 0x17C | matching | 2 steps: prompt `gStrDesignateMonsterYouWishToTribute`; keys `0xF0`, `EffectTributeForInsectCheck(ref, w)` accepts the cursor zone -> sound 1, message 8/0x8008, `TributeMonster(p, w)`, then the monster level of the card (`CARD_LEVEL`) + 1 is added as a target (`AddEffectTarget`) |
| `0x0804112C` | 0x160 | **matching** (wave 1, 2026-10-01) | 4 steps: prompts `gStrSelectZoneToBlock` / `gStrSelectAnotherZoneToBlock`, `DuelCursor_PickAny()` (input poll), needs `w828 == 0`, `IsMonsterZoneFree(p, w82C)`; step 3 also needs an empty zone (`ZB(..)->card == 0`) different from `ref->targets[0]`; picks with `AddEffectTargetUnchecked`. Matched by steering cross-jumping with per-case returns (see below) |
| `0x0804128C` | 0x110 | matching | step 0: at least one opponent zone 0-4 holds a face-down card (`(flags6 & 3) == 1`), else return 1; prompt `gStrDesignateOpponentMonsterToFlip`; step 1: keys `0x900000` -> `TryAddEffectTarget` |
| `0x0804139C` | 0xC0 | matching | prompt `gStrDesignateMagicForMask`, keys `0x40004`; picks any zone other than the card's own (`ref->player`/`ref->zone`) |
| `0x0804145C` | 0x130 | matching | 4 steps: prompts `gStrDesignateMonsterYouWishToTribute` / `gStrDesignateAnotherMonsterToTribute`, keys `0xF0`; step 3 refuses the same target as `ref->targets[0]` |
| `0x0804158C` | 0x110 | matching | steps 0-2: `DuelPrompt_Post(p, 9, 0, 0)`, `AddEffectTarget(ref, DG.w1B64 + 1)`, prompt `gStrDesignateMonsterToEquip`; later steps: keys `0xE000E0`, `EffectEquipTargetCheck(ref, w)` accepts -> `TryAddEffectTarget` |
| `0x0804169C` | 0x140 | matching | `(ref, a)`: 4 steps, gate `EffectTwoSpellTrapsOnFieldPrepare(ref, a, 0)`, prompts `gStrDesignateFirstCardToReturn` / `gStrDesignateSecondCardToReturn`, keys `0xE000E`; step 3 refuses the target equal to `ref->targets[0]` |
| `0x080417DC` | 0xBC | matching | prompt `gStrSelectReplacementAttacker`, keys `0xE00000`; the cursor zone must hold a card and differ from `ref->pos >> 8` |
| `0x08041898` | 0x328 | **matching** (wave 2, 2026-10-01) | 6-step machine with a saved level at `0x02017A40+0x3E6`: prompt `gStrDesignateMonsterToDestroy`; keys `0xE000E0`, the level of the picked card must be `<= CountGraveyardMonsters(ref->player)`, add target + level; `FormatInt(buf, gStrSelectGraveMonstersToBanishFmt, level)` prompt; list viewer `CardListView_Open(p, -1, 0x5FC, 0)`, message 0xD4 (0x80D4 for player 1) with the viewer card halves, counts the saved level down; step 5 `gStrRemainingCountFmt` prompt with the remaining count, returns 1 when 0. Ordinary C, see Wave 2 matches below |
| `0x08041BC0` | 0xA0 | matching | prompt `gStrDesignateOpponentSpellTrapToReturn`, keys `0xE0000` |
| `0x08041C60` | 0xA0 | matching | prompt `gStrSelectTrapToForceActivate`, keys `0x20002` |
| `0x08041D00` | 0xC4 | matching | prompt `gStrDesignateFusionToReturnToDeck`, keys `0xF000F0`, `EffectFaceUpFusionMonsterCheck(ref, w)` accepts the cursor zone |
| `0x08041DC4` | 0x1D8 | matching | `u16 f(ref, p, z)`: can the card in zone `(p, z)` be selected as an effect source? Requires a card of type > 0x14 (Magic/Trap), not face-up-restricted (`flags6 & 2`) unless card number in {0x52C, 0x3F9, 0x594, 0x5FC}, zone byte `+0x91` bit 2 set and bit 3 clear, no card 0x2EF on either side for Magic, level/side conditions (`GetCardSpellSpeed`, `0x0201ADF2`/`0x020192E0+0x1B12` bits); on success fills `ref` (id, player, zone) and returns `CanActivateEffect(ref, 0, 0)` |

## Structs and globals (in addition to [[effect-targets3-c]])
- `struct DuelZone` (0x94 bytes): `+0` card word (`id` 12 bits), `+6` flags (bit 1 = face-down, bit 0 ...), `+0x91` a flag byte (bit 2 / bit 3 tested by `0x08041DC4`).
- `0x0201930C + 0x1AE6` (= `0x0201ADF2`) byte, bit 1 compared with the acting player in `0x08041DC4`; `0x020192E0 + 0x1B12` byte: bits 2-4 a small mode (2 or 4 required), bit 1 a player.
- `PlayerState[p] + 7` bits 6-7 (`0x020192E4 + p * 0xD64`) tested for Magic/Trap effect sources.
- `0x02017A40 + 0x3E6` saved level counter of `0x08041898`; list viewer `gCardListView` (`struct ListView`: `+5` row, `+6` top, `+0xC` cards).

## Matching tricks
Everything in [[effect-targets3-c]] applies. This unit adds:
- **The `es` copy for a jump-table switch**: `u8 *es; int sw = gChain[0x3E5]; es = gChain; switch (sw)` makes gcc keep the constant in one register and copy it (`ldr r1,=sym; ...; adds r5,r1,#0`) as the ROM does (`0x08041898`); with `u8 *es = ..; switch (*(es + 0x3E5))` the constant is folded into `es` directly.
- **`u16 w`** for the packed target word gives the ROM's argument-move order (`mov r0,r8; adds r1,r4,#0`, `0x08040FB0`).
- **`ZB2((1 - ref->player) & 1, i)` in both uses of a loop** reproduces `i * 0x94` before `player * 0xD64` and the `z + p` add order (`0x0804128C`); for an index that is a plain parameter use `ZB` (z-first text) (`0x08041DC4`).
- **Bit fields for single-bit zone/state flags** (`struct DuelZone { ... u8 f91_2 : 1; u8 f91_3 : 1; }`) make gcc form `zone + 0x91` with a separate `adds r0,#0x91` and load the byte once; `struct PS7 { u8 pad[7]; u8 b7; ... }` indexed by player gives `base + player * 0xD64` then `ldrb [r0,#7]`; `struct DG12 { u8 pad[0x1B12]; u8 b; }` + `u32 b = g->b;` and shifts on `b` avoid a spurious register copy.
- **`switch ((int)CARD_TYPE(id)) { case 0x15: case 0x16: ... }`** instead of `ty >= 0x15 && ty <= 0x16` reproduces the two-sided compare (`cmp #22; bgt; cmp #21; blt`).
- **`goto fill`/`goto ret0` labels** to reproduce a return block that sits before the "fill" code and is shared by all failing checks (`0x08041DC4`).
- **Empty `r0` clobber before the final card-type switch** (`0x08041DC4`). This compiler-only hint changes allocation over the function so `p` and `z` remain in the ROM's `r7` and `r6`. It emits no instructions and is marked FAKEMATCH in C.
- **Named `r1` byte local for clearing target count** (`0x08040EBC`, also `0x08040110` in [[effect-targets3-c]]): load `ref` byte `+0xA`, pass that local as an input to an empty asm, then store `~7 & fields`. This preserves the other five bits and reproduces the original `r1` scratch. The compiler hint emits no instructions and is marked FAKEMATCH in C.
- **Duplicated tails**: in `0x0804145C`/`0x0804169C`/`0x08040FB0` the shared inc tail lives after the first (or last) case; simply writing `(*st)++; return 0;` in each case (see the other page) picks the same one as the ROM.
- **Cancellation pointer before zero also fixes earlier switch registers** (`0x0804158C`): initialize `u8 *e2 = gChain; u8 *q = e2 + 0x3E5; int z = 0;`, then `*q = z; return z;`. Reusing [[effect-targets3-c]]'s pointer-before-zero pattern reproduces the cancellation address in r1, zero in r0, and its direct epilogue branch. It also gives the ROM's initial switch value r2 / base copy r3, resolving the apparent unrelated register mismatch without any asm constraint or ABI change. The earlier `*(e2 + 0x3E5) = z` form computes the pointer after initializing zero and differs by 14 bytes; named switch/base-register constraints do not fix that source-order cause. Complete `0x10E0`-byte unit verified exact.

## Open problems
None: the unit is complete in C since wave 2 (`0x08041898`, see below). The notes in this section are historical.

`0x08040EBC`, `0x08040110`, and `0x08040CA8` now match; their former scratch-register issues are resolved by the verified compiler hints above and in [[effect-targets3-c]]. `0x0804158C` now matches through the cancellation-pointer declaration order above. Formerly remaining in `0x0804112C` (resolved in wave 1, see below): the two `AddEffectTargetUnchecked` call sites use different registers in the ROM (case 1: r5/r4, case 3: r4/r3), so gcc does not merge them, but our C does merge them. The ROM also merges the text calls into case 2. Historical (matched in wave 2): remaining in `0x08041898` were the `CARD_LEVEL` value in r0 copied to r4, and the shared return-0 block.

> [!warning] Contradiction
> The 2026-09-30 follow-up below says that for `EffectBanishGraveToDestroyChainB` "explicit shared return labels/zero constraints ... did not yield an exact match". The wave 2 match (2026-10-01, `build/wf/EffectBanishGraveToDestroyChainB/NOTES.md`) does use one shared label, `ret0:`, but placed inside case 5 before its `return 0` (case 1's paths `goto ret0`); with the label in case 4 no combination worked. The level copy came from a `u8` inline with per-arm returns, not from a constraint. Resolved in favour of the matched source.

The bounded follow-ups of 2026-09-30 stayed isolated. `EffectBlockMonsterZonesChainB` prompt-pointer/goto sharing (with declaration-order and initialized-pointer variants) did not improve its 280 differing-byte baseline. For `EffectBanishGraveToDestroyChainB`, an initialized first-level local with an empty read/write constraint recovers the first level calculation and copy to r4; later CardLevel-to-argument setup, return-block placement, and case-5 count setup still differ. Explicit shared return labels/zero constraints and first/second-level input constraints did not yield an exact match. A named r0 count used directly as a later call argument can be overwritten by preparation of argument zero; do not use that variant as a valid C proposal. Scratch scripts: `build/middle_experiments/selector_prompt_share.py`, `selector_level*.py`. (Historical: at that time the active source kept the guarded drafts and assembly fallbacks.)

## Card-scan interface reconciliation

`CanActivateFieldCard` now returns `int` with an explicit `(u16)` cast on its final dispatcher result. Its `CanActivateEffect` declaration agrees with the word predicate and actual pointer/u16 parameters in [[effect-activation-c]]. This preserves the original zero-extended result for [[ai-turn-steps-c]]. The complete unit bytes are unchanged. These declaration repairs add no coverage; per-unit artifacts are under `build/bigguns-lead2/` and the full ROM passes at `build/lead-pass27/`.

## Target selector matched (wave 1, 2026-10-01)

`EffectBlockMonsterZonesChainB` (0x160, start score 21) matches in ordinary C. Working notes: `build/wf/EffectBlockMonsterZonesChainB/NOTES.md`.

The difference was cross-jump layout. The ROM merges case 0's prompt call into case 2 (from `mov r2,#0xB`), sends both `AddEffectTargetUnchecked` successes to case 2's increment and both failures to case 2's `return 0`, and keeps the two `AddEffectTargetUnchecked` / `PlaySE` call sites separate. What made it match:

1. Case 0 order: clear `numTargets`, call the prompt, then `(*st)++` (the draft incremented first).
2. Case 1 keeps an explicit `(*st)++; return 0;` after `AddEffectTargetUnchecked`, with the input check as `if (DuelCursor_PickAny() != 0) { ... PlaySE(3); } return 0;`.
3. Case 3 uses `if (cond) { AddEffectTargetUnchecked(..); (*st)++; } else PlaySE(3);` with one `return 0` after the block. Without that explicit return, case 3's success tail does not jump to the return, so cases 0 and 1 cross-jump into case 2 instead of case 3 (score 2).
4. The packed position comparison is `(u16)((u8)p | (u8)zn << 8) != ref->targets[0]`. The pre-shifted form `((p<<24)>>8 | zn<<24)>>16` puts `lsr #8` before `lsl r1,r3,#24`; swapping the OR operands or dropping the `(u16)` cast is worse (8-10).

The earlier note that prompt-pointer/goto sharing did not help still holds: the fix is the per-case statement order and return placement, not shared labels. See [[matching-tricks#Switches, branches and shared tails]].

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/EffectBanishGraveToDestroyChainB/NOTES.md` (variants in `exp/`, scored by `run.sh`; `gen.py` enumerates return/goto combinations; `-dg`/`-dJ` dumps printed with `build/wf/BattleStage_SelectAttacker/rtl.py`).

### `EffectBanishGraveToDestroyChainB` (0x328, start score 85; ordinary C)

About 40 single variants plus two enumerations of 64 return/goto combinations each. Four differences:
1. Case 1 read `p` as `u16` (`ldrh`); `u32 p` and `int pl = 1 & p`, as in the neighbouring selectors, fix it.
2. Level in r0 plus the copies `adds r4,r0,#0` / `adds r1,r0,#0`: a `static inline u8 CardLevelU8(u32 id)` with a `return` in each switch arm. The inline result is a QImode pseudo, combine leaves the widening at the use as `(set (reg:SI) (subreg:SI (reg:QI)))`, and global.c gives no copy preference through a subreg source, so the level stays in r0 and the copy survives. Every other form scored 87: int/u16/s16/s8/u32 inline returns, a single-`return` inline, the `CARD_LEVEL` macro into a local of any width, `u8 lvl = CardLevel(id)` with an int inline. (A `u16` per-arm-return inline scored 18.)
3. Case 5's `adds r2,r0,#0` for the second count read comes from **post-reload CSE** (`reload_cse_regs`): the second `zero_extend` load becomes a register copy only when no CODE_LABEL lies between the two loads. `if (*(es + 0x3E6) != 0) { ... }` keeps the fall-through label-free; `if (n == 0) return 1;` does not. Failed: an int/u8/u16 local (gets r2 directly through the hard-register copy preference), a `register asm("r0")` pin (wrong code, r0 is clobbered before the copy), reusing the switch variable.
4. The shared `mov r0,#0; b end` after case 4's increment: put `ret0:` inside case 5 before its `return 0`, and send case 1's no-input and failure paths there with `goto ret0`. In `find_cross_jump`, an i1 that hits a CODE_LABEL lowers `minimum`, so case 5's `r0 = 0` merges into case 4's copy, and case 4's copy (no label) cannot merge into case 5's. With `ret0:` in case 4 (the old draft) the merge went the other way, whatever the returns. See [[matching-tricks#Switches, branches and shared tails]].
