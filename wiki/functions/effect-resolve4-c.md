---
title: Unit effect_resolve4 (duel card-effect step handlers, part 2)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit effect_resolve4

`0x08033DAC`-`0x08035197`, Thumb, `old_agbcc -O2`. Source: `src/effect_resolve4.c`. Continuation of [[effect-resolve3-c]]: more card-effect step handlers `int f(struct EffCtx *ctx)` (same `EffCtx` head as `CardRef` in [[effect-activation-c]]: `+0 id`, `+2 player/zone/kind`, `+4` flags (bit 2 = skip), `+6 u16` (player | zone << 8 of a card, "CardRef view"), `+0xA` bits 0-2 phase or target count, `+0xC u16 pos` (target), `+0xE u16`). Return value is the next step (0x64-0x80) or 0; the step byte is `0x02017A40+0x3E0`, sub-step `+0x3E1`.

Unit status: `unit bytes MATCH`, **16/17 functions in C** after workflow waves 2-3 (2026-10-02: `0x08034644` in wave 2, `0x08034768` in wave 3); 1 stays `INCLUDE_ASM` (`0x08034BFC`). Before the waves: 14/17. Verified with `tools/check.py effect_resolve4` (0x13EC bytes).

> [!warning] Contradiction: the unit is now 17/17
> The count above (16/17) predates later matches. `src/effect_resolve4.c` has no `INCLUDE_ASM` left (checked 2026-10-02), so all
> 17 functions are in matching C. The decompilation reached 100% at commit `d77fcef` ([[overview]]). Resolved in favour of
> the source. Text below that calls a function nonmatching, parked or `INCLUDE_ASM` is history. Some of the later matches
> are recorded only in git (`git log`) and not yet written up here.

## Shared headers
The unit includes `include/duel.h` and `include/duel_ui.h` (the latter pulls in `duel.h`); `include/main.h` is not needed (`gMain` is unused here). It removed four local struct definitions (`DuelCard`, `DuelZone`, `DuelZonesPlayer`, `DuelPlayerHead`) and the local externs for `gDuel`/`gDuelPlayers`/`gDuelZones`/`gDuelScreen`, and switched to the canonical tags and field names. `flags6 & 2` became `flag6_1`, `w824`/`b828` became `player`/`zone`, and the `#if 0` drafts' hand entries go through `CARD_WORD` since `hand[]` is now `struct DuelCard[]`.

Local views kept:
- `struct DuelStateByteView` (`gDuelStateBytes asm("gDuel")`): `duel.h`'s `struct DuelState` models `+0x1ACC` as a `u32` bitfield (`unk1ACC_0:15`) and has no byte-sized field at `+0x1ACD`. `EffectShieldAndSwordResolve` tests bit 6 of the byte at `+0x1ACD` (u32 bit 14) and only matches with a `u8` load, so the unit keeps a byte view for that one function.

## Functions

| Address | Size | Status | Purpose (hypotheses) |
|---|---|---|---|
| `0x08033DAC` | 0x48 | matching | log msg 0x1D with argument `!((byte 0x020192E0+0x1ACD >> 6) & 1)` |
| `0x08033DF4` | 0x50 | matching | step 0x80: `DrawCards(player, 3)` -> 0x7F; 0x7F: `DuelPrompt_PostDiscard(player, 2, 0, 0)` -> 0x7E |
| `0x08033E44` | 0x138 | matching | CardRef-view handler: card at `pos6` zone; 0x80 (needs a card) resets sub-step, 0x7F scans the own hand for a card `IsSameCardName(hand id, zone card id)`, 0x7E `SendDeckCopiesToGraveyard(player, number, 1)`, 0x7D log 0x60 -> 0x78 |
| `0x08033F7C` | 0x34 | matching | log msg 0x48 |
| `0x08033FB0` | 0x94 | matching | `(ctx, a)`: `EffectAttackResponsePrepare`, phase 1, `EffectMagicArmShieldCheck(ctx, pos)`: stores `player \| FindFreeMonsterZone(player) << 8` into `ctx+0xE`, logs 0xA2 and 0x38, `MoveFieldCard` |
| `0x08034044` | 0x118 | matching | among the opponent's zones 0-4 (face-up cards) picks the one with the lowest ATK (`GetZoneCardAtk`, start 99999); if `IsZoneTargetable` accepts it does `DestroyFieldCardByEffect`/`OnCardDestroyedByEffect`, else `ShowCardEffect(player, card id)` |
| `0x0803415C` | 0xE4 | matching | CardRef-view: kind 5/6, zone card present and `CanCardTargetZone`; card numbers 0x2A8 (`GetZoneCardDef <= 500`), 0x2A9 (ATK <= 500), 0x3EA (ATK > 999) gate `DestroyFieldCardByEffect` + `OnCardDestroyedByEffect` |
| `0x08034240` | 0x80 | matching | phase 1, target zone holds a face-up Magic (type 0x15): `DestroyFieldCard(player, zone, 1)` |
| `0x080342C0` | 0x9C | matching | phase 3, `phaseA & 7` targets `list[0..n]` at `ctx+0xC` (u16 pos each): all must be occupied, then `DestroyFieldCardByEffect` + `OnCardDestroyedByEffect` for each |
| `0x0803435C` | 0xB8 | matching | card number 0x3F0 with card 0x402 on either side aborts; phase 2 and `CountFreeMonsterZones`: builds a card word from `ctx+0xE:ctx+0xC`, `GetGraveyardCardById`, logs 0xD3, `QueueSpecialSummonChoosePosition(..., 0x30)` |
| `0x08034414` | 0x68 | matching | by card number 0x21B/0x5A7 -> 1, 0x3F2 -> 2: `DrawCards(player, n)` |
| `0x0803447C` | 0x1C8 | matching | 6-step machine 0x7B-0x80 (prompt `gStrSelectGraveyardCardToBanish`/`CE0`, selected card block `0x0201D810`, log 0xD4, sub-step countdown) |
| `0x08034644` | 0xC4 | **matching** (wave 2, 2026-10-01) | own hand non-empty and `DuelCursor_PickTarget(0x10000)`: logs 0x08 with the hand cursor `0x0201CFB0+0x824..0x82C`, `ShowCardDetail(player, card id of the opponent hand card at the cursor)`; else returns 0x80. The ROM keeps the `0x020192E4` base in r8 and 0xD64 in r9 (see [Wave 2 matches](#wave-2-matches-2026-10-01)) |
| `0x08034708` | 0x60 | matching | phase 1, target zone face-up and occupied: `QueueAddZoneLink(player, id, pos, 3)` |
| `0x08034768` | 0x440 | **matching** (wave 3, 2026-10-01) | 29-entry jump table, steps 0x64-0x80: the hand-card selection wizard. 0x80 scans the own hand for a card with `CanSummonFromHand != 0 && IsSpecialSummonOnly == 0` (like `0x0802E058` in [[effect-prepare1-c]]) and shows prompt `gStrSelectMonsterToSummonFromHand`; 0x7F/0x76/0x74/0x6C read the hand cursor (`0x0201CFB0+0x82C`, `IsTributableMonster(+0x824, cursor)`) and store picks into `ctx+0xC/0xE/0x10`; card type/level (0x15-0x17 -> 0, 0x18 -> 10, else stats bits 25-28) picks the next step 0x64 / 0x6E / 0x78; prompts are entries 0-4 of the pointer table `0x0819D1C4`; 0x64 finishes with `QueueNormalSummonChoosePosition(player, ctx+0xC, +0xE, +0x10)` and returns 0xA. See [Wave 3 matches](#wave-3-matches-2026-10-0102) |
| `0x08034BA8` | 0x54 | matching | `CollectEffectTargets(player, number, 0)` then `CardListView_Open(player, -1, number, 0)` |
| `0x08034BFC` | 0x59C | not attempted | |

## Matching tricks
- `!((byte >> 6) & 1)` (not `1 & ~(byte >> 6)`) gives `bic r1, r0` with the shared constant 1 in `0x08033DAC`.
- `u16 id = CARD_ID(...)` gives the `adds r5, r4, #0` copy and avoids the `lsl #16; lsr #16` before a `u16` parameter (`0x08033E44`).
- A card-word bitfield struct (`u32 id:12; u32 flag12:1; u32 rest:19`) with `.flag12` reproduces `lsl #19; lsr #31` (`0x0803435C`).
- Switch with a shared tail (`0x0803415C`): order the cases as the ROM lays the blocks out (0x3EA, 0x2A8, 0x2A9) and agbcc cross-jumps the identical tails. Inline `Le500(value)` / `Gt999(value)` helpers keep their bounds inside the comparison, reproducing the ROM's zero-result setup before loading the comparison constant. An empty read/write constraint on initialized `ok = 1` prevents reusing it as the later player mask; an empty `r5` clobber at return gives ctx/zone registers r5/r4. Both FAKEMATCH hints emit no instructions and use no unset values.
- `EffectDrawCardsResolve`: `n = 0; switch { case A: case B: n = 1; break; case C: n = 2; } if (n > 0) call(n)` makes agbcc thread the `n = 1` cases straight to the call.

> [!warning] Contradiction
> The next bullet (pre-wave, rom-analysis) says returning `0x80` from an `else` of the `DuelCursor_PickTarget` test puts the return block last in `0x08034644`. The wave 2 match (2026-10-01, `build/wf/EffectTheInexperiencedSpyResolve/NOTES.md`) found that `else return 0x80` scores 12; the matched source uses `else goto r80;` with `return 0; r80: return 0x80;` at the end. Resolved in favour of the matched source.

- Historical (see the contradiction note above): returning `0x80` from an `else` of the `DuelCursor_PickTarget` test puts the return block last (`0x08034644`).

> [!warning] Contradiction
> The next bullet (pre-wave, rom-analysis) prescribes a `found:` label placed after the last case for `0x08034768`. The wave 3 match (2026-10-01, `build/wf/EffectUltimateOfferingResolve/NOTES.md`) found that this layout lets CSE carry 0xD64 and the hand base into the loop latch, so loop.c hoists them into r7-sl; the matched source puts the found block inside the loop (`if (a && !b) { prompt(); return 0x7F; }`). Resolved in favour of the matched source; the `switch` compare-chain part of the bullet still holds.

- Big state machines: `goto` labels reproduce the ROM's shared returns (`found: prompt(); ret7f: return 0x7F;` placed after the last case; historical for `0x08034768`, see the note above); a `switch ((int)v)` with cases 1-4 / 5-6 / default gives the separate `cmp #1; blt; cmp #4; bgt; cmp #6; bgt` compare chain instead of an unsigned range test (`0x08034768`).
- A `for (i = 0; i < arr[1 & ctx->player].handCount; i++)` loop shows the ROM's guard without the `& 1` (folded) and the loop-end test with it.
- The sign test of a byte bit as `((int)((u32)byte << 26) < 0)` works in [[effect-resolve3-c]]; `flag = 0; if (x) flag = 1;` gives the `negs; orrs; lsrs` setcc.
- **Recomputed list addressing through initialized offset/base constraints** (`EffectTwoProngedAttackResolve`): each iteration assigns `off = i * 2` in r1 and `loopbase = (u8 *)ctx` in r0 and passes both initialized values through an empty read/write constraint. It then adds `0xC` to the base and passes that initialized base through an empty input before adding `off`. This preserves the ROM's `lsl r1,index,#1; copy ctx to r0; add r0,#0xC; add r0,r0,r1` in both loops. The first constraint prevents strength reduction into a walking pointer; the second prevents folding `+0xC` into the index. Hints emit no instructions and are marked FAKEMATCH; original ABI remains unchanged. Index-only constraints changed allocation/length and did not match, while the two-value form without the second input differed at the two `+0xC` instructions. Complete unit `0x13EC` bytes verified exact.

## Unsolved

Remaining `INCLUDE_ASM`: `EffectNegateChainedCardResolve` (0x59C, not attempted; no `#if 0` draft in the source). Historical: `EffectTheInexperiencedSpyResolve` and `EffectUltimateOfferingResolve` were on this list, with drafts under `#if 0`, until they matched in waves 2 and 3.

## Verified C conversions (2026-09-30)

- `EffectFissureResolve` (Thumb, `0x118` bytes) is matching C. It scans occupied face-up opponent monster zones for the smallest ATK, then performs the selected action. An empty r1 clobber in the loop and reversing the player/zone terms in the final address sum resolve allocation and add operand order.

The compiler hints emit no instructions. Each conversion passed a whole-unit byte comparison with the baserom; remaining assembly functions retain their original bytes. The matching C above resolves earlier notes that described these functions as allocation near misses.

The source also contains the verified matching C for `EffectChainDestructionResolve`. Status counts and function-table rows above reflect those recovered matches.

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/EffectTheInexperiencedSpyResolve/NOTES.md`.

### `EffectTheInexperiencedSpyResolve` (0xC4, start score 81; ordinary C)

- Constant 1 kept in r6 from the first block: index `gDuelPlayers[1 & ctx->player]`. Expand puts the 1 in a pseudo for the `and`, combine keeps that set alive, and the later `1 & byte2` and `(1 - p) & 1` reuse it. This also brings the `0x020192E4` base into r8 and 0xD64 into r9 (the second access is written `gDuelPlayers[(1 - ctx->player) & 1].hand[gDuelScreen.cursor]`, not through `0x02019968`).
- Third argument of `DuelCmd_Push`: `*(u8 *)&zone | (*(u8 *)&cursor << 8)`, a u8 read of zone (no u16 narrowing) and zone first, so `0x828` loads before `0x82C` (which becomes `0x824 + 8`).
- Return layout: `else goto r80;` with `return 0; r80: return 0x80;` places the 0x80 block last. `else return 0x80` (score 12), an early `if (!sub) return 0x80` (28) and a `ret` variable (55) do not.

## Wave 3 matches (2026-10-01/02)

Working notes: `build/wf/EffectUltimateOfferingResolve/NOTES.md`.

### `EffectUltimateOfferingResolve` (0x440, start score 153; ordinary C)

- The parked draft was stale: dead `TextBoxSetMenu` after `return 0x77`; a plain `.zone` read gave `ldr` because `duel_ui.h` now types `gDuelScreen.zone` as u32; `DuelCmd_Push` before `PlaySE(1)` in case 0x74; `u8 id` in the loop.
- Found block inside the loop (`if (CanSummonFromHand(..) && !IsSpecialSummonOnly(id)) { prompt(); return 0x7F; }`): loop.c's `find_and_verify_loops` moves the exit block after the loop, giving the ROM layout. The continue label now has two uses (one per `&&` test), so cse.c's skip-blocks path cannot run from the body into the latch; the latch reloads 0xD64 and nothing is hoisted (base r8, constant 1 r7). With `goto found` to a label after the switch, CSE carried 0xD64 and the hand base into the latch and loop.c hoisted 0xD64, base+0x684 and the 1 into r7/r8/r9/sl.
- `-128` constant (`movs #128; negs`): `(u8)((int)gDuelScreen.cursor | 0x80)`. `convert_to_integer` narrows the IOR into *signed* char because the int operand is signed, so the constant becomes `(s8)0x80`. A u32 operand gives `movs #128`, as did `(s8)(...)` on a u32.
- Packed argument `*(u8 *)&zone | (*(u8 *)&cursor << 8)` as in `EffectTheInexperiencedSpyResolve`; hand indexed `gDuelPlayers[1 & ctx->player].hand[i]` in the loop and `gDuelPlayers[ctx->player].hand[cursor]` (no `1 &`) in case 0x7F.
