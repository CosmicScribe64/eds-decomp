---
title: Unit code_08010BDC (duel card-move command handlers)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit code_08010BDC

`0x08010BDC`–`0x08011BDF`, Thumb, `old_agbcc -O2`. Source: `src/code_08010BDC.c`.
Duel command handlers (continuing [[code-0800fb10]], continued in [[code-08011be0]]) that move a card between areas (hand, field zones, graveyard-like areas). They are dispatched from `sub_0801ECA8` (hypothesis, per the call sites in `config/functions.tsv`) with their operands in the command block `gUnk_020185C0`. Each runs as a three-step state machine on `gUnk_020185C0.step`: 0 opens the area (`sub_080240A8(player, 11)` or `sub_08060FD0`), 1 starts the card-move animation `sub_080242C4(cardId, &from, &to)`, and 2 commits the move to the duel state. `running` is cleared when it finishes.

Unit status: `unit bytes MATCH`, 10/13 functions in C (verified with `tools/check.py code_08010BDC`). See the table for which functions are in C.

## Shared headers

This unit includes `include/duel.h` and `include/duel_ui.h` (which itself includes `duel.h`) instead of local struct definitions. It uses the canonical `struct DuelCard`, `struct DuelZone`, `struct DuelPlayer` (`numList684` renamed to `handCount`, `list684` to `hand`, `listB84` kept, and the unused `list904`/`arrCC4` dropped), `struct DuelLoc` (was local `struct CardLoc`), and `struct DuelCmd` (`gUnk_020185C0`, byte-array card field replaced by `struct DuelCard card`; `CMD_CARD` is now `&gUnk_020185C0.card`). Removed 5 local struct definitions (`DuelCard`, `CardLoc`, `DuelCmd`, `DuelZone`, `DuelPlayer`) and the corresponding local `extern`s. No local views were needed: the canonical u16 `step`/u32 `running` bitfields still compile to the same bytes as the unit's old u8 bitfields. `include/main.h` is not needed (no `gUnk_03000040` access).

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x08010BDC` | 0x38 | matching | `sub_0800A004(player, arg2 \| arg4 << 16)` | |
| `0x08010C14` | 0x180 | matching | Summon: hand[arg4>>4] → monster zone arg4&0xF (flags from arg4 bits 8/9) | |
| `0x08010D94` | 0x1F0 | nonmatching (asm) | Set magic/trap: hand → S/T zone (arg4&0xF, 5..9 → 0..4), Field magic → field zone (area 10) | |
| `0x08010F84` | 0x28 | matching | `sub_0800A0A8(player)` | |
| `0x08010FAC` | 0x38 | matching | `sub_08009EAC(player, arg2 \| arg4 << 16)` | |
| `0x08010FE4` | 0x164 | matching | Discard hand[arg2] → owner's area 15, commit `sub_080099E8` | |
| `0x08011148` | 0x130 | matching | listB84[arg2] (area 15) → hand, commit `sub_08009EAC` + `sub_08009D08` | |
| `0x08011278` | 0x220 | matching | Hand[arg2] ↔ opponent hand[arg4] animation both ways, then `sub_08007560` | |
| `0x08011498` | 0x178 | matching | hand[arg2] → owner's area 14 (hypothesis: removed from play), sets card flag20, commit `sub_080096F4` | |
| `0x08011610` | 0x170 | matching | hand[arg2] → owner's area 15 with flag15, sets flag20, commit `sub_08009768` | |
| `0x08011780` | 0x230 | nonmatching (asm, attempt in `#if 0`) | Field slot arg2 (0–9 → zones 5–9 / area 5, >9 → field zone, area 10) → owner's hand; zone flags → from.flag14/15; commit `sub_08008EB4` | |
| `0x080119B0` | 0x48 | matching | Clear card in zone (arg2 & 7) of the acting player | |
| `0x080119F8` | 0x1E8 | nonmatching (asm, attempt in `#if 0`) | Field slot arg2 → owner's area 14, zone cleared with `sub_08008E44` | |

## Data

- `gUnk_020185C0` duel command block (`struct DuelCmd`, `include/duel_ui.h`): +0x0 `cmd` (bit 15 = acting player), +0x2 `arg2`, +0x4 `arg4`, +0x80A bits 0–6 `step`, +0x80D bit 5 `running`, +0x814 `card` (`struct DuelCard`, id bits 0–11, owner bit 12, flag20 bit 20).
- `gUnk_020192E4[2]` (`struct DuelPlayer`, `include/duel.h`): per-player duel state, 0xD64 bytes each. +0x2 `handCount` (used as the next hand slot), +0x28 11 `zones` of 0x94 bytes (0–4 monster, 5–9 spell/trap, 10 field), +0x684 `hand`, +0x904 `graveyard`, +0xB84 `listB84` (area 15 source, hypothesis: graveyard).
- `struct DuelLoc` (4 bytes, `include/duel_ui.h`, `sub_080242C4` args): player:1, area:4 (0 monster, 5 S/T, 10 field, 11 hand, 14, 15), index:9, flag14:1, flag15:1. Zone +0x06 bit 0 / bit 1 are copied into flag14 / flag15 (hypothesis: face-down / defence).

## Matching tricks

- **Word vs byte bitfield access on the same local (`0x08011278`).** A small struct local lives in a pseudo register (ADDRESSOF) until the C front end sees `&loc`. Stores before that point are whole-word `ldr/and/orr/str`. After it, the local is a MEM and stores become `ldrb/strb` / `ldrh/strh` for each field. GCC 2.95 expands statement by statement, so the case that comes after the first `sub_080242C4(.., &from, &to)` in the source uses byte/halfword stores. Reusing the same `from`/`to` in both cases reproduces the ROM (the frame stays 8 bytes).
- `CMD_CARD->flag20 = 1` doesn't match. The ROM builds `0x020185C0 + 0x816` separately: `((u8 *)&gUnk_020185C0)[0x816] |= 0x10;`.
- `sub_08011780` / `sub_080119F8` (asm): the ROM keeps `(u16)player` in a register for both `from.player` and `to.player`. It masks the zone flag bits again with `& 1` (the register holding step==1), spills `(player&1)*0xD64` (frame 16/8), and adds the zone address as `(p*0xD64 + slot*0x94) + 0x020195F0` after computing the `sub_08007558` argument as `0x0201930C + p*0xD64 + (0x2E4 + slot*0x94)`. None of the attempted forms reproduce all of this.
- `sub_08010D94` (asm, attempt under `#if 0`): the only difference is register allocation. The `0xF` mask gets its own pseudo (r7), and `zone -= 5` is rematerialised as `r7 - 20` instead of `r0(=1) - 6`.
