---
title: Unit effect_prepare3 (duel effect handlers and conditions)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit effect_prepare3

`0x0802FB64`-`0x08030B87`, Thumb, `old_agbcc -O2`. Source: `src/effect_prepare3.c`. Follows [[effect-prepare2-c]] / [[effect-prepare1-c]] (same `CardRef`, `DuelZone`, `DuelPlayer`, `DuelGlobal` layouts).

Unit status: `unit bytes MATCH` (0x1024 bytes), **32/32 functions in C** after workflow waves 2-3 (2026-10-02: `0x08030620` in wave 2); none stay `INCLUDE_ASM`. Before the waves: 31/32, verified with `tools/check.py effect_prepare3` on 2026-09-30 after enabling `DestroyFieldCardByEffect`, `EffectMaskOfDarknessResolve`, `EffectAxeOfDespairResolve`, and `EffectHornOfLightResolve`.

The unit has two kinds of functions:
- **Conditions** `int f(struct CardRef *ref, [tgt], [u16 flag])` (0x0802FB64-0x0802FFE4): "can this card effect be used?" predicates, like the previous units.
- **Effect steps** `int f(struct CardRef *ref)` (0x08030028-0x08030B4C): they run one step of a card effect. Most start with `if (ref+4 & 4) return 0` (an "already handled" flag), test `ref+0xA & 7` (step number 1 or 2) or `gChain[0x3E0]` (the step state 0x7D-0x80 of a small state machine that returns the next state, 0x64 = done) and call the animation/sound helper `DuelCmd_Push(id | 0x8000 if player 1, ...)`, `DestroyFieldCard(player, zone, 1)`, `LoseLifePoints(player, amount)` etc. (hypothesis: the dispatcher is elsewhere).

## Functions

| Address | Size | Status | Purpose (roles are hypotheses; the logic is verified where matching) |
|---|---|---|---|
| `0x0802FB64` | 0x28 | matching | flag 0, `ref` kind 5-7: `EffectDarkHolePrepare(ref, tgt, 0)` (u16) |
| `0x0802FB8C` | 0x68 | matching | flag 0, `tgt == 0`, kind 0x10: zone at `ref+8` occupied, player matches, own card 0x57D on the field |
| `0x0802FBF4` | 0x44 | matching | no card 0x58A on either side and `CollectEffectTargets(p, 0x58D, 0) > 0` |
| `0x0802FC38` | 0x20 | matching | `CountTributableMonsters(p, -1) > 1` |
| `0x0802FC58` | 0x44 | matching | like `0x0802FBF4` with card 0x59F |
| `0x0802FC9C` | 0x24 | matching | own hand count > 1 |
| `0x0802FCC0` | 0x2C | matching | flag 0 and own hand not empty |
| `0x0802FCEC` | 0x2C | matching | `CountSpellTrapsFiltered(0,0,0,1) + CountSpellTrapsFiltered(1,0,0,1) > 1` |
| `0x0802FD18` | 0x78 | matching | `tgt` is a Trap (type 22), no 0x58A on either side, not card 0x603 |
| `0x0802FD90` | 0xDC | matching | some face-up card in either player's zones 0-4 pairs with a (player, zone 5-9) position via `EffectTailorOfTheFickleCheck`, `IsValidEquipTarget`, `FindMonsterLinkedToCard` |
| `0x0802FE6C` | 0x38 | matching | kind 0x10, other side, `CountMonstersFiltered(1-p,1,0) > 1` |
| `0x0802FEA4` | 0xEC | matching | flag 0, `tgt == 0`, no 0x5E7 on the other side, `n = CountGraveyardMonsters(p) != 0`; some face-up zone card (either player, zones 0-4) with level <= n (64-bit-base trick) |
| `0x0802FF90` | 0x54 | matching | kind 0xE, `(ref+8 & 0xF) != player`, byte 6 bit 0 of the zone encoded in the high byte of `ref+8` (player = low nibble & 1, zone = high nibble) |
| `0x0802FFE4` | 0x44 | matching | no 0x453 on either side, `CollectEffectTargets(p, 0x60D, 0) > 4` |
| `0x08030028` | 0xB0 | matching | `(player, zone)`: occupied zone whose card number is 0x2F/0x23D/0x4D9/0x4E9 plays sound `0x90 (+0x8000 for player 1)` and sets zone byte1 bit 6; then `DestroyFieldCard(player, zone, 1)` |
| `0x080300D8` | 0xE8 | matching | effect step: for each occupied face-up spell/trap zone 5-9 with card number 0x148 -> `DestroyFieldCard`; for each face-up monster zone with `GetZoneCardType == 1` -> `ChangeBattlePosition(i, j, 0, 0)` |
| `0x080301C0` | 0x17C | matching | state machine (0x80/0x7F/0x7E) with the card-list viewer `gCardListView` ([[card-list-viewer-c]]): opens the list with `CardListView_Open(player, -1, number, 0)`, plays a message via `TextBoxOpen`, returns next state |
| `0x0803033C` | 0xC4 | matching | plays sound `0x92`, then for every face-up monster zone with `GetZoneCardType == 2` calls `QueueAddZoneLink(player, posRef, posZone, 2)` |
| `0x08030400` | 0xE4 | matching | step 1: card at `ref+0xC`: face-up Magic -> `DestroyFieldCard`; face-down: sound 0x7F, `sub_08019820`, then same or sound again (explicit `return 0;` after the face-up branch stops old_agbcc cross-jumping the two `DestroyFieldCard` blocks) |
| `0x080304E4` | 0x94 | matching | step 1: card number 0x58 -> amount = `HalveRoundUp(ATK)`, 0x1FF -> 500; `TributeMonster` then `LoseLifePoints(1-p, amount)` |
| `0x08030578` | 0x74 | matching | step 2: `CountGraveyardCardsByNumber(p, number(card))`, `sub_08019820`, sound 0xD2 |
| `0x080305EC` | 0x34 | matching | plays sound 0x60 |
| `0x08030620` | 0x140 | **matching** (wave 2, 2026-10-01) | 4-state machine on the effect step `gChain[0x3E0]` (0x80 saves the deck words `+0x7C4` to `0x02017F84` = `0x02017A40+0x544` and clears four bitfields, 0x7F `DeckReorder_Run`, 0x7E restores them and on a link duel calls `DuelLink_SendDeck`, 0x7D waits for `0x02017FB0+0x305` bit 1). See [Wave 2 matches](#wave-2-matches-2026-10-01) |
| `0x08030760` | 0x4C | matching | plays sounds 0xD6 and 0x60 |
| `0x080307AC` | 0x28 | matching | `BanishFieldCard` for the two positions at `ref+6` and `ref+8` |
| `0x080307D4` | 0x64 | matching | step 1: occupied card, `TributeMonster`, then `QueueAddZoneLink(player, id, pos, 3)` |
| `0x08030838` | 0x48 | matching | `n = CountSpellTrapsFiltered(1-p,0,0,0)`; if not handled and `n > 0`: `LoseLifePoints(1-p, n * 500)` |
| `0x08030880` | 0xA8 | matching | dispatch step 1 / step 2 (card 0x3C2 needs `EquipCard` then sound 0x87), returns u16 0 |
| `0x08030928` | 0x100 | matching | state machine 0x80/0x7F/0x7E/0x7D with a `char buf[0x100]` message (`FormatStr(buf, fmt, table[gUnk_08624052 << 6])`, `TextBoxOpen`, `TextBoxSetMenu`) |
| `0x08030A28` | 0x3C | matching | mode 3 (`ref[2] & 0xE == 6`) else `EffectEquipResolve`; `LoseLifePoints(1-p, 500)` |
| `0x08030A64` | 0xE8 | matching | state machine 0x80/0x7F: life points > 499 message; sounds 0x43/0xD0 |
| `0x08030B4C` | 0x3C | matching | mode 3 else `EffectEquipResolve`; sound 0xD0 |

## Data
- `CardRef` gains `+0xA` (byte, `& 7` = step number), `+0xC` u16 `posC` (player low byte, zone high byte; the low 12 bits are also a card id in `0x08030578`), `+0xE` u16, `+4` byte bit 2 "handled" flag, `+2` bits 1-3 (mode, 3 = the effect-step variant).
- `gChain + 0x3E0`: effect state byte (0x7D-0x80); `+0x3E5` a flag byte.

> [!warning] Contradiction
> The next bullet (2026-09-30, rom-analysis) describes a separate object `struct Unk02017E20` with a counter byte at `+0`. The wave 2 match of `EffectBigEyeResolve` (2026-10-01, `build/wf/EffectBigEyeResolve/NOTES.md`) shows the "counter" is the effect step byte `gChain[0x3E0]`, and the bitfields and saved words are `0x02017A40+0x53C` and `+0x544` of the same effect scratch. Only the `0x02017A40` base reproduces the ROM's CSE (`r5-0x164` at the use, base register dead). Resolved in favour of the matched source; the old `struct Unk02017E20` / `gChainEffectWork` declarations in the unit source are now unused.

- Historical (superseded in wave 2): `struct Unk02017E20` (hypothesis): `+0` counter byte, `+0x15C` four 8-bit bitfields starting at bit 12 (cleared by `EffectBigEyeResolve`), `+0x164` u32[5] copy of a player's `+0x7C4` block (deck words). The compiler derived all these addresses from one register, so they are one object.
- `gChain` effect scratch (verified by the `EffectBigEyeResolve` match): `+0x3E0` step byte, `+0x53C` four 8-bit bitfields from bit 12, `+0x544` u32[5] saved deck words. `gLinkState+0x305` bit 1 (`dirtyDeck`, as in [[duel-cmd-deck-c]] and [[duel-prompts-c]]) gates state 0x7D.
- Card level (bits 25-28 of the stats word; types 0x15-0x17 give 0 and 0x18 gives 10): macro `CARD_LEVEL`, as in [[effect-prepare2-c]].
- `0x0201D810` `ListView` (row = `+5` bits 0-1, top = `+6`, cards = `+0xC`), see [[card-list-viewer-c]]. `0x02015F00 + 0x1B22` holds the saved list position.

## Proposed names (hypotheses)
`EffectEquipResolve` becomes `CardEffect_RunStep12`. `EffectAxeOfDespairResolve`, `EffectHornOfLightResolve`, `EffectBigEyeResolve` and `EffectSanganResolve` become `CardEffect_*State` (state machines returning the next state). `DuelCmd_Push` becomes `PlayEffectSound` (arg 1 is the sound id, `| 0x8000` for player 1).

## Matching tricks
- **Chained `||` vs separate `if ... return 0;`**: `EffectSwitchAttackerPrepare` needs the separate form; agbcc then re-extracts the `player` bitfield (`lsrs r1,r1,#31`) for `1 - player` instead of reusing the compare's register.
- **Drop a byte local and inline it**: `EffectRepelledAttackerPrepare` matched only after replacing `u16 w = ref->unk8; if ((w & 0xF) != ...)` with the inline `if ((ref->unk8 & 0xF) != ...)` (the local made the compiler copy the wrong operand into r0).
- **Move a loop-invariant mask into the inner loop body**: `EffectMoveEquipPrepare` matched with `p = i & 1;` as a statement inside the inner `for` (agbcc hoists it but emits the ROM's `ldr …; adds …; movs …; ands …` instead of the 2-byte-shorter form).
- **64-bit base stops table-address hoisting** (see [[decomp-permuter]]): `EffectBanishGraveToDestroyPrepare` needed `unsigned long long base = 0x08621DE0;` for the *type* access while the `default` level access keeps the literal, so the table reloads inside the loop like the ROM.
- **An explicit `return 0;` after a branch stops old_agbcc cross-jumping** two identical tail calls (`EffectReaperOfTheCardsResolve`).
- **Symbol addresses are hoisted, constant addresses are not.** A `gUnk_xxx[...]` reference used twice inside a loop gets its `ldr rX,=sym` hoisted into a callee-saved register; `((const u16 *)0x08622AB4)[...]` (integer constant) is loaded where used (`EffectSanganResolve`, `0802FEA4`).
- **`u8`/`u16` flag variable gives the `neg / orr / lsr #31` idiom** for `ok = f() != 0` (a plain `int ok` compiles to a branch): `u8 ok = f(x) != 0; if (...) ok = 0;` (`EffectMoveEquipPrepare`).
- **One struct with CSE'd offsets.** When the ROM addresses several RAM objects as `r5 - 8`, `r5 - 4`, `r5 - 0x164` from one literal, they are members of one struct (one symbol); declare it once (`EffectBigEyeResolve`; the right base turned out to be `0x02017A40`, see the contradiction note under Data and [Wave 2 matches](#wave-2-matches-2026-10-01)). Bitfield assignments of 8-bit fields at bit 12/20/28/36 give exactly the u32 / u16 / two u8 / u16 read-modify-write sequences.
- **Integer compare on `int id`:** `int id = CARD_ID(word); if (id > 0 ...)` gives `ble`, the `u16` form gives `beq` (`EffectDragonPiperResolve`).
- **`switch` bodies are laid out in source order and a body without `break` falls into the next**: 0x80 case first, then 0x7F, 0x7E, 0x7D (`EffectBigEyeResolve`, `08030928`). Use `goto sNN;` back to an earlier `return` to keep one shared block (`EffectAxeOfDespairResolve`).
- **Raw byte `& 1` for the ternary** `(((u8 *)ref)[2] & 1) ? 0x8060 : 0x60` is `movs r0,#1; ldrb; ands`; `1 & ref->player` is the bitfield form (`lsls/lsrs`). Sound ids are `id | 0x8000` for player 1.
- Use `u32` shifts for a bitfield read from a halfword (`(u32)(ref->posC << 20) >> 20`); `int` gives `asrs`.
- **Assignment inside an argument** fixes the evaluation order of a local (`CountGraveyardCardsByNumber(ref->player, CARD_NUMBER(id = ...))`).

## Open problems

Historical (matched in wave 2): remaining `INCLUDE_ASM` was `EffectBigEyeResolve`, kept as assembly with its decoded draft under `#if 0`. The unit is now fully in C.

## Verified C conversions (2026-09-30)

- `EffectBlackPendantResolve`, Thumb, `0x3C` bytes, matching C. It is the mode-3 LP effect, and other modes delegate to `EffectEquipResolve`. An empty r1 clobber after reading byte 2 preserves the shared byte in r3.
- `EffectHornOfTheUnicornResolve`, Thumb, `0x3C` bytes, matching C. It wraps event 0xD0 for mode 3, and other modes delegate to `EffectEquipResolve`. The same byte hint plus player/message locals assigned to r0/r3 reproduces the ROM ternary and call setup.
- `EffectHornOfLightResolve`, Thumb, `0xE8` bytes, matching C. Byte `+2` is held in `r3` for the mode check and first sound message. The inline `PlayerBitFromModeByte` helper constrains the shifted player bit to `r0` before the right shift, preserving the ROM's `lsl #31; lsr #31` instead of an `and #1`. The empty input constraint is marked FAKEMATCH and emits no instructions. The second sound re-reads byte `+2` after the first call, as the ROM does.
- `EffectMaskOfDarknessResolve`, Thumb, `0x74` bytes, matching C. A scoped sound-id local in `r3` reproduces the explicit 0xD2/0x80D2 branch. Initialized position/zero locals and an empty input constraint with `r0` clobbered prepare `r1/r2` before the sound-id copy to `r0`, preserving the original call setup. This compiler hint emits no instructions and is marked FAKEMATCH.
- `DestroyFieldCardByEffect`, Thumb, `0xB0` bytes, matching C. Empty input constraints on the first zone address and recomputed player mask preserve the original first multiply/address in `r0` and the later mask in `r2`. Both are marked FAKEMATCH and emit no instructions.
- `EffectAxeOfDespairResolve`, Thumb, `0x100` bytes, matching C. An initialized `0x3E5` offset constrained after its byte store preserves the offset in `r2` and zero in `r0`. A separate initialized `u16` position constrained after the zone shift with `r2` clobbered preserves the position in `r3` and the later mode-byte scratch in `r1`. Both empty compiler hints are marked FAKEMATCH, emit no instructions, and retain the original ABI.

> [!note] Resolved size correction (2026-09-30)
> Earlier source/wiki notes gave `DestroyFieldCardByEffect` size `0x3C`. Its range in `config/functions.tsv` and the ROM is `0x08030028` through `0x080300D7`, size `0xB0`. The complete-unit comparison validates the corrected extent.

The compiler hints emit no instructions. Each conversion passed a whole-unit byte comparison with the baserom; remaining assembly functions retain their original bytes. Earlier notes describing these functions as allocation near misses are resolved by the matching C above.

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/EffectBigEyeResolve/NOTES.md`.

### `EffectBigEyeResolve` (0x140, start score 81; ordinary C)

- The fix was the struct base. The "counter" is the effect step byte `gChain[0x3E0]`, not offset 0 of an object at `0x02017E20`. The final source reads a local `struct Effect30620` (step `+0x3E0`, bitfields `+0x53C`, `saved[5]` `+0x544`) through `(*(struct Effect30620 *)gChain)`. With the `0x02017E20` base, the counter address is the plain symbol register that CSE keeps alive from the first bitfield access (computed early in r3). With the `0x02017A40` base, CSE rewrites it as `r5-0x164` at the use and the base register dies. This one change fixed all the r3/r4 differences.
- State 0x7D tests the u32 bitfield `gLinkState.dirtyDeck` (`lsl #30; cmp; bge`); a `goto` back to the `return 0x7D` inside case 0x7E gives the cross-jumped tail.
- State 0x7E indexes `gDuelPlayers[ref->player & 1]`; the `& 1` constant stays live in r5 through the loop and CSE reuses it for `1 & gDuelCtrl[1]`, which must be a symbol array (`extern u8 gDuelCtrl[]`) to give `ldrb [r0,#1]`.
- Failed: a local pointer to the bitfields (gives `[r2,#2]` offsets instead of separate `r5-k` addresses); an unused label before the decrement (no change).
