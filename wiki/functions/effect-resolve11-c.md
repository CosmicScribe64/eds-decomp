---
title: Unit effect_resolve11 (duel card-effect executors)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit effect_resolve11

`0x0803B670`-`0x0803C837`, Thumb, `old_agbcc -O2`. Source: `src/effect_resolve11.c`. Same family as [[effect-resolve8-c]] / [[effect-resolve7-c]] (`int f(struct CardRef *ref)` effect executors, state byte `gChain[0x3E0]`, side byte `[0x3E1]`, counter `[0x3E2]`; 0x64 = done, 0x7B-0x80 = steps).

Unit status: `unit bytes MATCH`, **17/17 functions in C** after workflow waves 2-3 (2026-10-02: `0x0803BCE4` in wave 2); none stay `INCLUDE_ASM`. After wave 1: 16/17 (2026-10-01, `0x0803C254` added).

| Address | Size | Status | Purpose (hypotheses) |
|---|---|---|---|
| `0x0803B670` | 0x50 | matching | one target: message 0x87 (zone, targets[0]), `EquipCard(p, p \| zone << 8, targets[1])` |
| `0x0803B6C0` | 0x104 | matching | 0x80/0x7F: `EFF_SIDE = 1 - p`; for zones 0-4 `EffectSpecialSummonedMonsterCheck(ref, i << 8 \| side)` -> `DestroyFieldCardByEffect` + `OnCardDestroyedByEffect`; flips the side, returns 0x7F until it is the player again; default message 0x49 with the `+7 bit 3` flag of both players |
| `0x0803B7C4` | 0x160 | matching | 5-step machine 0x80..0x7C with the list viewer: 0x80 `EFF_SIDE = 2`; 0x7F `CollectEffectTargets(p, 0x5E7)`, text `gStrBanishAnotherOpponentGraveMonsterQuestion/080837D4`, `TextBoxSetMenu`; 0x7E waits for `0x0201AE60+0x14`; 0x7D `CardListView_Open`; 0x7C message 0xD4 with the viewer card halves, `--EFF_SIDE`, returns 0x80 |
| `0x0803B924` | 0x1C4 | matching | 7-step machine 0x80..0x7A: check (`CollectEffectTargets(p, 0x5E8)`, `CountTributableMonsters`), key input `DuelCursor_PickTarget(0xF0)` chooses a zone (`0x0201CFB0+0x82C`), `TributeMonster`, list viewer, `QueueSpecialSummonChoosePosition`, finally `QueueAddZoneLink(p, id, p \| pos << 8, 3)` with `pos` = bits 1-5 of `0x0201CF90`; returns 0x78 |
| `0x0803BAE8` | 0x1FC | matching | 9-case machine 0x80..0x78 (card 0x5EA family: `EFF_SIDE = 3` picks, `EFF_CNT` counts picks), viewer message 0xD4 (bit 12 of the card word selects the 0x8000 variant), finally `QueueAddZoneLink(p, id, p \| zone << 8, cnt << 8 \| 0xB)` and message 0x92 |
| `0x0803BCE4` | 0x154 | **matching** (wave 2, 2026-10-01; FAKEMATCH) | card 0x5EA: `FindFreeSpellTrapZone` / `FindFreeMonsterZone` both != -1, one target of the opponent with flag 2: `GetGraveyardCardById(p, id, 0x02017F84)`, messages 0xD3 / 0x77, `EquipCard`, then unless `skip4` `MoveFieldCard(p, targets[0], p \| (u8)a << 8)`. The ROM keeps the first -1 in sl (see [Wave 2 matches](#wave-2-matches-2026-10-01)) |
| `0x0803BE38` | 0x98 | matching | 0x80 text `gStrSelectOpponentMonsterToChangePosition`; 0x7F key input `DuelCursor_PickTarget(0xE00000)`, message 0xA1 with `0x0201CFB0+0x82C`, `ChangeBattlePosition(1 - p, sel, 0, 0)` -> 0x7E |
| `0x0803BED0` | 0x190 | matching | opponent-side machine 0x80/0x7F/0x7E: `CollectEffectTargets(1-p, 0x447)`, `CountFreeMonsterZones`, `CanSpecialSummon`, `DuelPrompt_Post(1-p, 0x13/0xE, ..)`, builds a `PosWord` from `0x020192E0+0x1B64` (bit 12 flipped twice under `0x02015EE8[1] & 1` and `[0x1B12] & 2`), message 0xD3, `QueueSpecialSummonChoosePosition(1-p, &pos, 1, 0x20)` |
| `0x0803C060` | 0x70 | matching | two targets, each zone holding a card: `ReturnFieldCardToHand(tp, tz, 1)` |
| `0x0803C0D0` | 0x104 | matching | 0x80 count = `CollectEffectTargets(p, 0x5F4, 0)` (needs >= 2, `EFF_SIDE = 2`); text `gStrSelectFusionMaterialToAddToHand`; 0x7E `CardListView_Open`; 0x7D viewer message 0xD2, `--EFF_SIDE`, returns 0xA when done else 0x7F |
| `0x0803C1D4` | 0x80 | matching | `(ref, card word *)`: neither player has card 0x58A -> `TributeMonster(p, zone)`; if the given card has type 0x16 message 0xB0 |
| `0x0803C254` | 0x114 | **matching** (wave 1, 2026-10-01) | opponent-relative search over players 0-1, zones 5-9 for a card with flag 2 and type 0x15/0x16 with `(stats & 0xE0000) >> 17 == 3`; if `EffectTailorOfTheFickleCheck` and `IsValidEquipTarget(i, j, tp, tz)` hold: `MoveEquipCard(i \| j << 8, targets[0])` else `DestroyFieldCard(i, j, 1)`. Ordinary C (see below) |
| `0x0803C368` | 0xCC | matching | one target with flag 2 and a card: messages 0x35, 8, 0x39 and `ChangeBattlePosition(tp, tz, 0, 0)` when flag 1 |
| `0x0803C434` | 0xC4 | matching | two targets: target zone holds a card with flag 2 and level (`CARD_LEVEL`) == `targets[1]`: `DestroyFieldCardByEffect` + `OnCardDestroyedByEffect` |
| `0x0803C4F8` | 0x58 | matching | `ref+8` byte = player (low nibble) / zone (high nibble): `DestroyFieldCardByEffect` + `OnCardDestroyedByEffect` when the zone holds a card |
| `0x0803C550` | 0x50 | matching | one target holding a card that is not mine: `ReturnFieldCardToHand(tp, tz, 1)` |
| `0x0803C5A0` | 0x298 | matching | one target (no skip check for the setup): flips a face-up (flag & 2 == 0) card: `FlipFieldCard`; non-Magic (type != 0x15) cards go to `ShowRevealedCard`; Magic cards whose card number is in a list of 43 numbers (0x2A8, 0x2A9, 0x2AD, 0x2B1, 0x3B1 ... 0x5FF) are handled by `ShowDestroyedCard` + `DestroyFieldCard`, others test `CanActivateEffect(&tmpRef, 0, 0)` (a scratch `CardRef` with the card id/zone) then `ShowActivatedCard` + `Chain_AddPending(bit31 \| zone << 16 \| 0x200000 \| id)`; tail `ReturnFieldCardToDeck(p, zone)` + message 0x60 |

## Shared headers
- This unit now has `#include "duel.h"` and uses the canonical `struct DuelCard`, `struct DuelZone`,
  `struct DuelZonesPlayer`, `struct DuelPlayer`, `struct DuelState` and the externs `gDuel`,
  `gDuelPlayers`, `gDuelZones` from it (local definitions removed). It does not use `main.h`
  (no `gMain` access).
- Local view kept: `struct DuelStateBytes` / `extern struct DuelStateBytes gDuelStateView asm("gDuel")`,
  used only by `0x0803BED0`. That function reads `gDuel` as raw bytes (byte +0x1B12, u16s at
  +0x1B64/+0x1B66); indexing the canonical `struct DuelState` folds the offsets into one reloc
  constant, which no longer matches the ROM's base+offset codegen.

## Structs and globals
- `PlayerState` (stride 0xD64 at `0x020192E4`) is now the canonical `struct DuelPlayer` from `duel.h`; the flag used by `0x0803B6C0` is `+7` bit 3 (`flag7_3`, not `+8` bit 4 as in [[effect-resolve8-c]]); this still needs checking.
- `struct CardRef`, `struct HandRow`, `struct DuelScreen`, `struct ListView`, `struct AE60`, `struct CardRef20` and `struct PosWord` stay local (not in the shared headers).
- `0x0201CF90`: a byte whose bits 1-5 are the zone position (`<< 26 >> 27`).
- `0x02017A40 + 0x3E2` is `EFF_CNT`, a counter byte.
- `struct CardRef20`: local scratch `CardRef` (0x10) followed by a word; the ROM reserves 0x14 bytes for it in `0x0803C5A0`. `CanActivateEffect(&ref, 0, 0)` returns u16.
- The 43-entry card-number list in `0x0803C5A0` is probably "cards that act when flipped up / are moved to the graveyard" (hypothesis).

## Matching tricks
- **`int tp = (u8)ref->targets[0];` instead of `u8 tp`**: changes register allocation and the `& 1` code (`movs r1,#1; ands r1,r4` becomes `movs r0,#1; adds r1,r4,#0; ands r1,r0` as in the ROM). Fixed `0x0803C5A0`, `0x0803C060` and `0x0803C368` after `u8 tp` had swapped registers.
- **`u16 msg = cond ? 0x80A1 : 0xA1;` local before the call** (with the `0x0201CFB0+0x82C` pointer computed afterwards) reproduces the ROM's ldr order in `0x0803BE38`.
- **Shared `return` label inside the failing branch**: `if (x == 0) { ret78: return 0x78; }` in one case and `goto ret78` from other cases puts the shared tail exactly where the ROM has it (`0x0803BAE8`); same for a `goto hit` tail after the `default:` block of a big switch (`0x0803C5A0`).
- **Fall-through case order**: `case 0x80: ...; EFF_PHASE--;` falling into `case 0x7F:` (no `break`) and a `default:` message that the `0x78` case also falls into.
- **`EFF_SIDE--` versus `EFF_PHASE--`**: the byte at `+0x3E1` is decremented as a loop counter in the viewer steps (0x7C), while the state byte is decremented only on the step from 0x80 to 0x7F.
- **`(u32)byte << 26 >> 27` for a bit field of an `extern u8`**: a bitfield struct is read with `ldr`, the shift form gives the ROM's `ldrb`.
- **Dead-looking 4th argument**: `IsValidEquipTarget(i, j, tp, tz)` takes 4 arguments (a `tz` spilled/reloaded around the previous call is the argument).
- **`(1 & tp)` / numTargets == const**: when the ROM reuses the compare constant (`numTargets == 1`) as the `& 1` mask, plain `1 & x` produces it automatically.
- Test for a set bit 12 of a card word with `(int)(word << 19) < 0`.

## Open problems
- Historical (matched in wave 2, FAKEMATCH): `0x0803BCE4`: the ROM keeps the constant -1 in `sl` across the whole function (the later `a != -1` also uses it) and `tp` in r7; every variant gives r4/r8 shuffles.
- Resolved `0x0803C254` (wave 1): the ROM keeps a stack copy of the outer loop index `i` and a pre-incremented `i + 1`, `(u8)i` hoisted into `sl`, and a copy of `j` in r5. See below.

## Opponent-zone search matched (wave 1, 2026-10-01)

`EffectMoveAllEquipsResolve` (0x114, start score 47) matches in ordinary C. Working notes and priority scripts (`dump.sh`, `prio.py`): `build/wf/EffectMoveAllEquipsResolve/`.

- `u8 j` gave a byte-truncated increment and `bls`; the ROM uses `int j`.
- The ROM hoists `(u8)pl` into sl. Loop-invariant motion moves it only if its lifetime is long enough. Writing `(u8)pl | (u8)sj << 8` (pl first) expands `(u8)pl` before the sj part, which makes the lifetime long enough; `(u8)sj << 8 | (u8)pl` does not.
- `int pl = i` stays inside the inner body; loop motion hoists it after the `i+1` precompute, which gives the ROM's store order.
- The ROM computes `i & 1` before `j*0x94`: a separate `p = i & 1` declared after `ok = 1` reproduces this, including the reuse of r0 = 1.
- The `MoveEquipCard` argument is `(u8)i | (u8)j << 8`; the u16 parameter does the rest.
- Last step (score 32, a ref/i swap between r6 and r7): global-alloc priority `floor_log2(refs) * refs / live_length` gave i 4*20/216 = 0.3704 against ref 3*15/122 = 0.3689. `i` is set twice, so each loop insn adds 2 to its live length and only 1 to ref's. Adding pre-combine insns inside the loop flips the order: `u8 p = i & 1;` plus `u8 lvl;` each add zero-extend insns that combine later deletes, giving ref 0.3543 against i 0.3540.
- The permuter also reached 0 with an uglier form (`short lvl` plus a copy for pl).

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/EffectEquipFromGraveTakeControlResolve/NOTES.md` (priority-table script `prio.sh` and scoring script `try.sh` in the same directory).

### `EffectEquipFromGraveTakeControlResolve` (0x154, start score 58; FAKEMATCH)

- A literal `-1` in all four compares instead of an `int none = -1` user variable (58 to 44). With `-fcse-skip-blocks` the if-converted ternaries are skippable blocks, so CSE keeps the first -1 temp across the whole function; that temp is what lives in sl. A user variable set at the top loses global-alloc and is rematerialised instead.
- `int a` (not `u16`): no `lsl/lsr 16` after the call, and `a` is the pseudo that spills to `[sp]`. `int tp` (not `s16`): one pseudo loaded straight into r7.
- Operand order `tp != ref->player` (`cmp r7, r0`) and `ref->player | (u8)a << 8` (player copy before the shift).
- FAKEMATCH: `asm("" :: "r"(tp));` before `a = FindFreeMonsterZone(...)`. tp (6 refs, live 110, priority 1090) lost to the 0x77/0x8077 ternary temp (3 refs, live 13 doubled to 26 by `update_equiv_regs` because its first set has a constant REG_EQUAL, priority 1153). The extra use gives tp 7 refs (1261), so tp is allocated first and gets r7 and the ternary falls to ip.
- Natural forms tried without the asm all scored 80-98 (u8/u16/u32 tp, tz forms, `ZB(tp & 1, tz)` without `pa`, a `u16 m` ternary local, argument variants, int-param casts of `EquipCard`/`GetGraveyardCardById` (tp live 110 to 108, needs <= 104), a local player-bitfield copy). The permuter's only score-0 output returned an uninitialised variable (undefined behaviour) and was rejected.
- Cleanup idea from the agent: a shape that makes the second ternary's range one pre-combine insn longer (live 13 to 14), or tp's 6 insns shorter (hypothesis).
