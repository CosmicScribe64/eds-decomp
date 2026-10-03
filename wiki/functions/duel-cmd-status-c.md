---
title: Unit duel_cmd_status (duel command handlers, zone flags)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit duel_cmd_status

`0x08011BE0`–`0x08012C4B`, Thumb, `old_agbcc -O2`. Source: `src/duel_cmd_status.c`.
More duel "script command" handlers (see [[duel-cmd-hand-c]] for the card-move ones). Each reads its operands from the command block `gDuelCmd` (+2 `arg2`, +4 `arg4`, +6 `arg6`) and clears `running` (bit 5 of `+0x80D`) when done. Most set or clear one flag of a field zone (`0x0201930C + (p&1)*0xD64 + slot*0x94`).

Unit status: `unit bytes MATCH`, **29/29 functions in C** after workflow waves 2-3 (2026-10-01/02: `0x08011FFC` in wave 2, `0x08011CB4` in wave 3); none stay `INCLUDE_ASM`. Before wave 2: 27/29 (wave 1 on 2026-10-01 added `DuelCmd_TributeMonster`). Verified with `tools/check.py duel_cmd_status`.

## Shared headers

As of 2026-09-30 the unit `#include`s the canonical `duel.h` and `duel_ui.h` instead of
declaring the duel structs/globals itself. The local definitions of `struct
DuelCmd`, `struct DuelPlayers` and `struct CardLoc` were fully removed (they are now `DuelCmd` from `duel_ui.h`,
`DuelState`/`DuelPlayer` and `DuelLoc` from the headers). `gDuelCmd`,
`gDuel`, `gDuelPlayers` and `gDuelZones` now come from the headers, and
`CMD_PLAYER()` reads the canonical `gDuelCmd.cmd`.

Local views kept because the canonical declaration differs from what this unit matches
(the headers are shared and were not changed):

| Local view | Canonical type | Why kept |
|---|---|---|
| `ZoneCard08011BE0` | `struct DuelCard` (+0x00) | canonical groups bits 13..19 as `unk13` and names bit 20 `flag20`; this unit sets bits 14..17 individually (same 4-byte u32 container) |
| `DuelZone08011BE0` | `struct DuelZone` | canonical exposes +0x06/+0x07/+0x8C only as bytes (`counter6`/`unk7`/`unk8C[]`); this unit matches the per-bit split (u32 containers at +0x04/+0x8C, u8 at +0x07) |
| `DuelPlayer08011BE0` (`gUnk_020192E4_unkE`) | `struct DuelPlayer` | canonical names +0x0E as part of the `u8 unkD[]` region; this unit reads `u16 unkE[11]` from +0x0E |
| `DuelFlags08011BE0` (`gUnk_020192E0_flags`) | `struct DuelState` +0x1B12 | canonical splits +0x1B12 into bitfields; `LINK_SKIP()` tests bit 1 as a whole byte |
| `CmdCard` (`gDuelCmdCard`) | `struct DuelCard` | canonical names bit 20 `flag20`; this unit writes bit 17 |

`ZoneHead8` (u8 overlay of zone +0x06) and `ZoneHead16` (u16 overlay of +0x06) are unit
overlays for fields the canonical `struct DuelZone` does not split the same way.

## Functions

| Address | Size | Status | Purpose |
|---|---|---|---|
| `0x08011BE0` | 0x38 | **matching**, ordinary C | `AddCardToGraveyard(&(arg2 \| arg4 << 16))`, `DrawAllAreaTiles()` |
| `0x08011C18` | 0x80 | **matching**, ordinary C | Unless link-skip: if entry count of `0x02017A40` > 1, set bit 2 (and bit 3 if arg2) of the current entry |
| `0x08011C98` | 0x1C | **matching**, ordinary C | no-op |
| `0x08011CB4` | 0x284 | **matching** (wave 3, 2026-10-02) | Set zone flag91_3 = arg4; if set, effect (`0x0868EC38` / `0x0868DB94` by flag6_0) and step 3 (10 when link-skip); steps 1/3: if the card number is 1068 (`0x42C`), `UpdateMonsterControl` on the result of `FindMonsterWithLinkTo` |
| `0x08011F38` | 0x68 | **matching**, ordinary C | `unkE[slot]` of player: `+= 500` if arg4 == 500, else `= arg4` |
| `0x08011FA0` | 0x5C | **matching**, ordinary C | Zone counter (+6 bits 2–5) `++`, max 15 |
| `0x08011FFC` | 0xE4 | **matching** (wave 2, 2026-10-01; FAKEMATCH) | Set zone flags from the bits of arg4; in state 3 of `0x020192E4+0x1B0E` (`gDuel.phase1B12`), if the player equals `linkSkip`, clear the slot bit in `unk26` (`zoneMask`) |
| `0x080120E0` | 0x184 | **matching**, ordinary C | Counterpart of `0x08011FFC`: step 0 effect on the zone, then clear the zone flags selected by arg4 |
| `0x08012264` | 0x50 | **matching**, ordinary C | zone flag7_5 = arg4 |
| `0x080122B4` | 0x234 | **matching** (wave 1, 2026-10-01) | Destroy: effect `0x08690D0C` and `ClearZoneTiles`; step 1 `SendZoneCardToGraveyardOrBanished(player, slot, arg4)`, then, unless the card is a token (number 1920–1999), animate it to the owner's area 15 (arg4) or 14 |
| `0x080124E8` | 0x188 | **matching**, ordinary C | Change of control: zone card → opponent (area 13 animation), `AddCardToDeckTop(player-1, card)`, clear zone |
| `0x08012670` | 0x64 | **matching**, ordinary C | zone +6 bits 6–9: set to arg4 if 0 or larger |
| `0x080126D4`–`0x0801277C` | 7 × 0x1C | matching | no-ops |
| `0x08012798` | 0x48 | **matching**, ordinary C | zone flag8C_5 = 1 |
| `0x080127E0` / `0x08012834` | 0x54 each | matching | zone flag8C_3 / flag8C_4 = arg4 |
| `0x08012888` | 0x1C | **matching**, ordinary C | no-op |
| `0x080128A4` | 0x54 | **matching**, ordinary C | zone flag7_2 = arg4 |
| `0x080128F8` | 0x58 | **matching**, ordinary C | zone (arg2 low byte = player, high byte = slot) flag8C_2 = arg4 |
| `0x08012950` | 0x1C | **matching**, ordinary C | no-op |
| `0x0801296C` | 0x174 | **matching**, ordinary C | Summon token: card number 1920+arg4 → ID via `0x08623DF4`, `PlaceMonsterCard(player, slot, &card, face, 1)`, zone flag7_2 = 1 |
| `0x08012AE0` | 0x54 | **matching**, ordinary C | Write card word `arg4 \| arg6 << 16` into zone (u8)arg2 (`CopyDuelCard`) |
| `0x08012B34` | 0x118 | **matching**, ordinary C | Move a zone: `MemCopy16(zone[arg4], zone[arg2])`, set dst flag6_0, clear src card, effect `DuelAnim_PlayZoneEffect(&loc, 0x0869771C, 0, 0)`, `ClearZoneTiles(player, arg2)` |

## Data

- `gDuelCmd` +0x80A: `step:7` in a **u8** container (a u16 container compiles `step == 0` as `and #0x7F` instead of `lsl #25`).
- Zone (0x94 bytes) +0x06: bit 0 flag6_0, bit 1 flag6_1, bits 2–5 counter (max 15), bits 6–9 a 4-bit value (u16 container). +0x07 bits 5–7 flags, +0x8C bits 2–5 flags.
- `gDuelPlayers` (players) +0x0E `u16 unkE[11]` per slot, +0x26 per-slot bitmask. +0x1B0E (= `0x020192E0 + 0x1B12`): bit 1 a player index, bits 2–4 a state (hypothesis).
- `gChain`: 20-byte entries from +0x258, u16 count at +0x3C0 (hypothesis: a duel log or chain list).
- Link-skip test: `(0x02015EE8.b1 & 1) && (0x020192E0.b1B12 & 2)`, same as `LINK_SKIP()` in `src/duel_stat_queries.c`.

## Matching tricks

- **Zone address shape decides the match.** The ROM computes `(slot*0x94 + p*0xD64) + base` with `(struct DuelZone *)(slot * 0x94 + player * 0xD64 + gDuelZones)` (`0x08011FA0`, `0x08012670`). `&gDuelPlayers.p[p].zones[slot]` gives `(p*0xD64 + 0x0201930C) + slot*0x94` (`0x08012AE0`). In `0x08012B34`, the call arguments come from a local `zones = (struct DuelZone *)(gDuelZones + (player & 1) * 0xD64)` plus index, and the later zone accesses from `(player & 1) * 0xD64 + s * 0x94 + gDuelZones`.
- **Bitfield container width decides the compare.** The counter at +6 only matches through a `u8` bitfield overlay (unsigned `bhi`, `lsl r1; lsr r0`). The bits 6–9 value needs a `u16` overlay plus an `int` temporary (signed `ble`).
- Two bitfield stores to one byte are merged into one RMW in source order: `flag6_1 = 0; flag6_0 = 1;` gives `and ~2; orr 1`.

> [!warning] Contradiction
> The next bullet (page text before 2026-10-01) says both accesses go through `gDuelPlayers`, not `gDuel`. The wave 2 match of `DuelCmd_SetZoneStatusFlags` (2026-10-01, `build/wf/DuelCmd_SetZoneStatusFlags/NOTES.md`) puts every access on `gDuel` (`.players[p & 1].zones[slot]`, `.phase1B12`, `.linkSkip`, and the players base as `gDuel + 4`), and `DuelCmd_SetSpellTrapDisabled` (wave 3) tests the link-skip byte through `gDuelZones` (`[0x1AE6]`). What matters is that all the related addresses use **one** symbol, so CSE's related-value reuse can derive them from one literal; which symbol depends on the function. Resolved in favour of the matched source.

- CSE between different symbols: `0x020192E4+0x1B0E` built as `r(0x0201930C) + 0x1AE6` means both accesses go through the same symbol (`gDuelPlayers`), not `gDuel`.
- **Zone card word as a separate 4-byte struct.** `struct ZoneCardWord w` at +0 makes `cardId` reads word loads (`ldr; lsl #20`, `ldr; lsl #21; lsr #20` for `& 0x7FF` indexing), while stores still use `ldrh/strh` / `ldrb/strb`.
- **A reused constant-1 register leaves an extra `& 1`.** When `player & 1` puts 1 in a register, CSE reuses that register as the bitfield-insert mask, so combine can't drop the `and` after a 1-bit extract. `0x080120E0` shows it (`0x080122B4` had the same symptom until its wave 1 match).
- `0x080120E0`: `zones = (struct DuelZone *)(gDuelZones + (player & 1) * 0xD64); zone = zones + slot;` gives `(p*0xD64 + base) + slot*0x94`. Casting inline instead gets folded into a different association.
- `0x08011C18`: the entry pointer is a local (`e = &entries[count]`), and `count` is read twice.

## Exact C conversion (2026-10-01)

`DuelCmd_SummonToken` now matches all 0x174 bytes. The token-summon helper reads the reverse-ID table through its integer address. This resolves the last two differing bytes. The four valid token kinds and original call arguments are unchanged. As in the ROM stack RMW sequence, defined card bits 0–18 are initialized and reserved bits 19–31 are left untouched; this is existing behavior, not a matching scaffold. The cleaned whole-unit check and the combined full-ROM check passed; no signatures or emitted instructions were fabricated. Evidence: `build/bigguns-lead2/DuelCmd_SummonToken/solo-clean/`, `build/lead-pass10/` and `build/lead-pass11/`.

## Destroy command matched (wave 1, 2026-10-01)

`DuelCmd_TributeMonster` (0x234, start score 44) matches in ordinary C, with no FAKEMATCH. Working notes: `build/wf/DuelCmd_TributeMonster/NOTES.md`.

- Case 0: the ROM calls `PlaySE(0x10)` before `from.player = player` is stored; the draft had them the other way round.
- Card-number index: the ROM does `ldrh; lsl #21; lsr #20` from a halfword read of the card word, then loads the table address after the index. The form that matches is `*(const u16 *)(0x08622AB4 + (((u32)*(u16 *)card << 21) >> 20))` (same idiom as `DuelCmd_ReturnToHand` in [[duel-cmd-field-c]]). `card->id & 0x7FF` through a u32 bitfield, or a u16-bitfield view, CSEs with the later word load (`ldr`); the `gCardIdToNumber[...]` symbol form loads the table before the index.
- Widening `u16 slot` / `u16 mode` locals to `u32` fixed the `0x1FF` constant register (r1 plus a copy), an extra `mov r1, sp`, the zone-address registers, and a 4-byte size shortfall.

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/DuelCmd_SetZoneStatusFlags/NOTES.md`.

### `DuelCmd_SetZoneStatusFlags` (0xE4, start score 56; FAKEMATCH)

1. The ROM loads one literal `0x0201930C` (`gDuel + 0x2C`, the zones base) and derives the `0x1B12` flag byte (`r3 + 0x1AE6`) and the players base (`r3 - 0x28`) from it. That is CSE's related-value reuse, which works only when every address is the same symbol plus offsets: the zone as `gDuel.players[p & 1].zones[slot]` (not `gDuelPlayers[...]`) and `gDuel.phase1B12` (56 -> 26).
2. The `zoneMask` write used `sym+0` with offset `0x2A`; the ROM uses `sym+4` with offset `0x26`. A cast pointer `((struct DuelPlayer *)((u8 *)&gDuel + 4))[p & 1].zoneMask` (`PLAYERS_08011FFC`) keeps the +4 in the base; `gDuel.players[p&1].zoneMask` folds `4 + 0x26` together (26 -> 10).
3. FAKEMATCH (labelled so in the source, although it is plain C): `u32 p = CMD_PLAYER(); u32 player = p;`, with `p & 1` for the zone index and `player` later. The copy gives the `cmd >> 15` result its own short-lived pseudo in r0, which the `& 1` reuses, while the player stays in r6 as in the ROM (10 -> 0).

## Wave 3 matches (2026-10-01/02)

Working notes: `build/wf/DuelCmd_SetSpellTrapDisabled/NOTES.md`.

### `DuelCmd_SetSpellTrapDisabled` (0x284, start score 192; ordinary C)

The parked draft was structurally wrong (the `loc.flag14/15` writes after `DuelAnim_PlayZoneEffect`, case 1 falling into a dead `running = 0`, the card word read through a bitfield as `ldrh`, the link flag through `gDuel`), so it was rewritten from the asm: 192 -> 176 -> 20 -> 0. The final source is ordinary C plus a local struct view.

- Card word: `(*(u32 *)zone << 20) == 0` and `*(const u16 *)(0x08622AB4 + ((*(u32 *)z << 21) >> 20))`. Both give a 32-bit `ldr`; the integer table address keeps the literal load after the index (the symbol form hoists it), as in `DuelCmd_TributeMonster` above.
- Link-skip byte: `((u8 *)gDuelZones)[0x1AE6] & 2` (`LINK_SKIP3()`), so CSE forms `r9 + 0x1AE6` from the zones symbol.
- Zone address: a static inline `Zone08011CB4(player & 1, slot)` returning `s*0x94 + p*0xD64 + base`, used in all three cases. The argument evaluates `player & 1` before both products. Cases 1 and 3 use no `zone` variable, so the zone pseudo drops below player and slot in priority (r4/r5/r6).
- r7/r8 swap (176 -> 20): the step pointer must get r7 and the constant-1 pseudo r8, which needs const1 at 7 refs, not 8. Through the u8 container of `DuelZone08011BE0`, CSE reused the const1 register as the `flag91_3` store mask. A local view `Zone90View08011CB4` with `flag91_3` as bit 11 of a **u16** container at `+0x90` keeps `movs r0, #1` there (a u32 container scored 22).
- Last 20: the real prototype is `UpdateMonsterControl(int player, int zone, u16 link0)` (see [[duel-send-to-grave-c]]); `UpdateMonsterControl((u8)r, r >> 8, 0xFFFF)` reuses the compare's `0xFFFF` register as r2.
