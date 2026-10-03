---
title: Unit effect_prepare2 (duel effect-condition predicates, part 2)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit effect_prepare2

`0x0802EB58`-`0x0802FB63`, Thumb, `old_agbcc -O2`. Source: `src/effect_prepare2.c`.
Continues [[effect-prepare1-c]]: 33 more "can this card effect be used now?" predicates `int f(struct CardRef *ref, [struct CardRef *tgt | int a], [u16 flag])` (roles are hypotheses; matching the code verifies the logic). Same `CardRef` / `DuelZone` / `DuelPlayer` layouts as [[effect-prepare1-c]], [[card-list-viewer-c]], [[duel-piles-c]].

Unit status: `unit bytes MATCH` (0x100C bytes, `tools/check.py effect_prepare2`), 32/33 functions in C. One stays `INCLUDE_ASM` (attempt under `#if 0`). Rechecked on 2026-09-30 after enabling `EffectNoSummonFourFreeZonesPrepare` and `EffectStartOfMainPhase1Prepare`.

> [!warning] Contradiction: the unit is now 33/33
> The count above (32/33) predates later matches. `src/effect_prepare2.c` has no `INCLUDE_ASM` left (checked 2026-10-02), so all
> 33 functions are in matching C. The decompilation reached 100% at commit `d77fcef` ([[overview]]). Resolved in favour of
> the source. Text below that calls a function nonmatching, parked or `INCLUDE_ASM` is history. Some of the later matches
> are recorded only in git (`git log`) and not yet written up here.

## Functions

| Address | Size | Status | Purpose (hypothesis about role) |
|---|---|---|---|
| `0x0802EB58` | 0x60 | matching | some opponent monster zone 0-4 holds a face-up card |
| `0x0802EBB8` | 0x260 | matching | switch on `ref` card number 0x46E-0x518 (12 cases): checks `ref` kind (0x14/0x15/0x18/0x19/0x1A/0x1B/0x1D), `tgt` type/subtype (Trap subtype 2/4, Magic subtype 4), player of the effect position (`ref+6`) and `CountActiveCardsOnFieldExcept(1-p, number, -1)` |
| `0x0802EE18` | 0x94 | matching | own `+0x904` list holds more than 4 monsters (type <= 0x14) |
| `0x0802EEAC` | 0x54 | matching | some monster zone 0-4 of either player holds a face-down card |
| `0x0802EF00` | 0x5C | matching | `CanSpecialSummon`; `CountFreeMonsterZones(i) > 0` for a player and `CollectEffectTargets(p, number(ref), 0) != 0` |
| `0x0802EF5C` | 0x44 | matching | own life points > 799 and `EffectCallOfTheHauntedPrepare` (u16 result) |
| `0x0802EFA0` | 0x24 | matching | kind 0xD and `(ref+8 & 0xF) != player` |
| `0x0802EFC4` | 0x2C | matching | `CountMonstersFiltered(0,1,0) + CountMonstersFiltered(1,1,0) > 1` |
| `0x0802EFF0` | 0x78 | matching | life > 999 and an opponent monster zone 0-10 holds a face-down card |
| `0x0802F068` | 0x20 | matching | `CountMonstersFiltered(p, 1, 0) > 0` |
| `0x0802F088` | 0x24 | matching | `CountMonstersFiltered(1-p, 1, 0) > 0` |
| `0x0802F0AC` | 0x48 | matching | opponent hand not empty and own hand count - n > 0 |
| `0x0802F0F4` | 0x60 | matching | `CountMonstersFiltered(1-p,0,0) != 0` and own field has card number 0x22, 0x4BA or 0x7F2 (`CountActiveCardsOnField > 0`) |
| `0x0802F154` | 0x74 | matching | `CountFreeMonsterZones`, `CanSpecialSummon`, player flag `+8` bit 4 clear, `CollectEffectTargets(p, number(ref), 0) > 0` |
| `0x0802F1C8` | 0x38 | matching | `CountMonstersFiltered(1-p,0,0) != 0 && CountMonstersFiltered(p,0,0) != 0` |
| `0x0802F200` | 0x250 | **nonmatching (asm)** | duel phase == 2, `CountMonsters`, no 0x58A; own zone 0-4 has card number 0x58/0x105/0x1FF face-up (then true), else player flag `+8` bit 4 clear and a hand monster (`IsSpecialSummonOnly == 0`) with level > 4: level <= 6 needs `FindFreeMonsterZone(p) != -1`, level > 6 needs `CountTributableMonsters(p, -1) > 0` (likely a tribute-summon check) |
| `0x0802F450` | 0x40 | matching | no card 0x58A on either side and `CountFaceUpMonstersByNumber(p, 0x39) > 0` |
| `0x0802F490` | 0x38 | matching | player flag `+8` bit 5 clear and `CountFreeMonsterZones(p) > 3` (ROM re-extracts the player for the call) |
| `0x0802F4C8` | 0x20 | matching | duel phase (`byte 0x1B12 & 0x1C`) == 8 |
| `0x0802F4E8` | 0x30 | matching | opponent's life points <= 3000 |
| `0x0802F518` | 0x48 | matching | flag 0, opponent hand > 5 and own hand <= 2 |
| `0x0802F560` | 0x58 | matching | phase == 8, own flag `+9` bit 5 clear, flag `+8` bit 4 clear |
| `0x0802F5B8` | 0x40 | matching | `CountMonsters(p) != 0` and own hand count != n |
| `0x0802F5F8` | 0x210 | matching | `(u16 number, ref, player a, zone b)`: for number 0x431 the card must be a Trap; card number of `ref` in a 38-entry set (compare tree); then the zone (a, b) holds a card and is not `ref`'s own second position (`ref+0xC`); returns `CanEffectTargetZone(ref, a, b)` |
| `0x0802F808` | 0x80 | matching | counts zones for effect 0x431 (both players x zones 0-4) or 0x525 (given player) that satisfy `CanRedirectEffectToZone` |
| `0x0802F888` | 0x60 | matching | kind 0x10 and other-side: `CountMonsters > 1`; else `tgt` on the other side and `CountRedirectTargets(0x525, p, tgt) > 0` |
| `0x0802F8E8` | 0x24 | matching | `CountFreeMonsterZones(0) + CountFreeMonsterZones(1) > 1` |
| `0x0802F90C` | 0x4C | matching | `tgt` is a Trap (type 22) and not card 0x603 |
| `0x0802F958` | 0x8C | matching | some own hand card is a Trap (type 22); matched with a dead-var fakematch (see tricks) |
| `0x0802F9E4` | 0x60 | matching | kind 8, target position on the other side, target zone occupied, byte 6 bit 0 set and bit 1 clear |
| `0x0802FA44` | 0xD0 | matching | counts hand monsters without `IsSpecialSummonOnly` (needs >= 1) and other cards (flag 1 discounts one, needs > 1) |
| `0x0802FB14` | 0x34 | matching | `CountHandMonsters(p)` and `CollectEffectTargets(p, 0x58D, 0) > 0` |
| `0x0802FB48` | 0x1C | matching | `GetFaceUpFieldMagicNumber() == 0x14D` |

## Data
- `DuelGlobal` at `0x020192E0`: `+0` u32, `+4` the two `DuelPlayer` (0xD64 each; `zones` at `+0x28` of a player = `0x0201930C`), `+0x1B12` byte (bits 2-4 = phase; 8 and 2 seen here, hypothesis). New `DuelPlayer` bits used here: `+8` bit 4 and bit 5, `+9` bit 5 (unknown flags).
- `CardRef +0xC` u16: a second position (player low byte | zone high byte), used by `CanRedirectEffectToZone`. `CardRef +8` low byte (used as `& 0xF` and whole) and `+6` low byte are compared with the player number.
- Card level (bits 25-28 of `gCardStats[id]`): types 0x15-0x17 count as 0, type 0x18 as 10 (`CARD_LEVEL` macro in the unit, `#if 0` block).

## Proposed names (hypotheses)
- `CanRedirectEffectToZone` as `CanEffectAffectZone`
- `CountRedirectTargets` as `CountAffectableZones`
- `EffectEventResponsePrepare` as `CanEffectTargetCard`
- `EffectMainPhase1Prepare` as `IsPhase8`

## Matching tricks
- **gcse hoists a load that is used in both arms.** `CanRedirectEffectToZone` reads `ref->id` in the `if (number == 0x431)` block and again after it. old_agbcc hoists the first load above the compare (`ldrh r2,[r5]`); the ROM does not. Reading the first one through `((volatile struct CardRef *)ref)->id & mask` (with `u32 mask = 0x7FF;` a separate local so the constant is loaded before the load) matches. Check with `old_agbcc ... -fno-gcse` to see whether gcse is the cause.
- **Two zone pointers for two reads.** `z1 = ZB(p, i)` for the card word and a fresh `z2 = ZB(p2, i)` for the flag byte (`EffectEarthshakerPrepare`, `EFF0`); a reused pointer, or `ZFLAGS(ZB(..))` directly, gives a different register/offset layout (`add r0,r7,#6`).
- **Nested `j & 1` player index** (`EffectCeasefirePrepare`): `int p = j & 1;` as its own statement, then `ZB(p, i)`.
- **Return layout: `||` chain with `return 0` first.** When the ROM has `beq ret0` for every condition and the `ret1` block last, write `if (a == 0 || b <= 0) return 0; return 1;` (`EffectDarkMagicianOnFieldPrepare`, `F154`, `FB14`, `F450`); `if (c) return 1; return 0;` gives the opposite polarity.
- **`return x > 0;`** as a value gives `movs r1,#0 ... movs r1,#1; adds r0,r1,#0` (`F068`, `F088`, `F154`, `F8E8`); `int r = 0; if (...) r = 1;` gives an extra callee-saved register.
- **Byte reads of `ref` fields.** `(((u8 *)ref)[3] >> 2)` for a kind compared with a small constant; `ref->kind == K` (u16 bitfield) is the `& 0xFC` form.
- **Shared tails and cross-jump survivors** (`EffectEventResponsePrepare`, `F5F8`): old_agbcc merges identical `return 1` / `return 0` blocks. Which copy survives decides the block layout. `goto ret1;` / `goto ret0;` to a label placed inside the *first* case body (`ret1:` in the 0x474 case, `ret0:` at the very end) pins the survivor where the ROM has it. Duplicated (non-goto) test code that the ROM merges with `b`, like the 0x472/0x473 subtype test, comes out right when written twice.
- **Case order = fall-through order.** In a 3-value switch the body after the last compare is the first *in source order*: put `case 15:` before `case 13: case 14:` to get the ROM order (`EffectEventResponsePrepare`).
- **Range case labels** (`case 0x12c ... 0x13c:`) reproduce the exact compare tree of a 38-value switch; decode the tree first with a small interpreter that reports which values reach the "true" label, to get the case sets.
- **`switch ((int)x)`** for signed compares of an unsigned bitfield-derived value; level result as `u32` gives `bls`/`bhi`.
- **Dead-variable fakematch for a loop-invariant register swap** (`EffectMagicCardInHandPrepare`, found by the permuter): the ROM holds the loop base in `r7` and the `0x7FF` constant in `r4`; a plain draft swaps them. Declaring `int new_var;` and folding the loop condition into it (`if ((new_var = CARD_TYPE(id) != 0x16))`) reproduces the ROM's allocation (the extra live value changes register priorities).
- **Player-bit re-extraction after a flag test** (`EffectNoSummonFourFreeZonesPrepare`): form `u32 shifted = (u32)refByte2 << 31`, read flag byte `+8`, and test bit 5 through `(s32)((u32)flags << 26) >= 0`. An empty read/write asm constraint on the initialized shifted value after that test prevents common-subexpression reuse of the first normalized player index, reproducing `lsr r0,r3,#31` at the call. The constraint emits no instructions and is marked FAKEMATCH in C.

## Open problems
- `EffectCanTributeOpponentMonsterPrepare`: everything matches except the loop entry test of the hand loop. The ROM keeps `1 & player` in the first check (`movs r1,#1; ands r1,r0`) and a `base` in r4, while our version folds the `&` and uses r2 plus `mov sl,r2`.
- `EffectMagicCardInHandPrepare` is now matched by the dead-variable fakematch described under the tricks above.
- Resolved `EffectStartOfMainPhase1Prepare`: use a separate player-array pointer, raw signed-shift flag tests, an initialized shifted player in `r3`, and an initialized mask of one. Empty read/write input on the mask preserves the ROM's explicit `and`; input constraints keep the raw shifted value alive through both tests, making each normalized player use `r0`. The FAKEMATCH hints emit no instructions, preserve behavior and introduce no unset values.

> [!note] Resolved near miss (2026-09-30)
> `EffectNoSummonFourFreeZonesPrepare` previously remained in assembly after a 14-minute permuter run (246k iterations) could not resolve its player-index CSE. The raw flag-byte test and empty shifted-value constraint above now reproduce the exact ROM bytes. Its complete unit passed `tools/check.py` with matching C enabled.
