---
title: Unit code_08013CDC (duel command handlers, turn bookkeeping / player flags / grid effect)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit code_08013CDC

`0x08013CDC`–`0x080150DB`, Thumb, `old_agbcc -O2`. Source: `src/code_08013CDC.c`.
Duel "script command" handlers, dispatched like those in [[code-08012c4c]] and [[code-080162c4]]. Most of them set one per-player flag from `arg2`. Two large handlers do per-turn bookkeeping.

Unit status: `unit bytes MATCH`, 14/16 functions in C (verified with `tools/check.py code_08013CDC`).

> [!warning] Contradiction: the unit is now 16/16
> The count above (14/16) predates later matches. `src/code_08013CDC.c` has no `INCLUDE_ASM` left (checked 2026-10-02), so all
> 16 functions are in matching C. The decompilation reached 100% at commit `d77fcef` ([[overview]]). Resolved in favour of
> the source. Text below that calls a function nonmatching, parked or `INCLUDE_ASM` is history. Some of the later matches
> are recorded only in git (`git log`) and not yet written up here.

The unit uses the shared headers `include/main.h` (struct `Main`) and `include/duel.h`
(struct `DuelCard` / `DuelZone` / `DuelPlayer` / `DuelState`); its own copies of those
structs and of the `gDuel` / `gDuelPlayers` / `gMain` externs were removed.

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x08013CDC` | 0x340 | nonmatching (asm, attempt in `#if 0`) | Turn bookkeeping for the acting player. Counts down `turnsB_0` and both players' `turns6_14` (clears both when either reaches 0), clears `flag8_3`. For monster zones 0–4 it bumps the zone counter (+6 bits 2–5, cap 15) and applies per-card effects by card number (0x0F/0x1AC/0x243/0x2DA/0x2E6/0x458/0x536/0x5E9 → `QueueRemoveZoneLink(player, id, slot<<8\|player, 11)`; 0x52 cap 6; 0x267 cap 4; 0x540 sets zone +7 bits 2/5). For spell/trap zones 5–9 it bumps the counter of card number 0x47 (cap 12) or sets +0x91 bit 2 for card types > 20. Link entries of kind 2 get their value +1 (up to 5) | `DuelCmd_TurnStart` (hypothesis) |
| `0x0801401C` | 0x6F4 | nonmatching (asm) | Large turn-end pass over both players' zones and lists (not attempted: many loops, 0x1C-byte frame with spills) | |
| `0x08014710` | 0xF4 | matching | 32-frame sparkle sprite at the zone position from `GetAreaX/GetAreaY(player, 0, 2)`, SE 15 | |
| `0x08014804` | 0x40 | matching | `player.flag8_10 = 1` | |
| `0x08014844` | 0x40 | matching | `player.flag8_9 = 1` | |
| `0x08014884` | 0x40 | matching | `player.flag8_11 = 1` | |
| `0x080148C4` | 0x50 | matching | `player.flag8_6 = arg2` | |
| `0x08014914` | 0x50 | matching | `player.flag6_13 = arg2` | |
| `0x08014964` | 0x74 | matching | `player.flag6_11 = arg2; player.flag6_12 = arg4` | |
| `0x080149D8` | 0x4C | matching | `player.turns6_14 = arg2` | |
| `0x08014A24` | 0x40 | matching | `gDuel.flag1ACD_5 = arg2` | |
| `0x08014A64` | 0x44 | matching | `gDuel.flag1ACD_6 = arg2` | |
| `0x08014AA8` | 0xB4 | matching | `arg2 ? counterC++ : counterC--` (not below 0) | |
| `0x08014B5C` | 0x50 | matching | `player.flagC_4 = arg2` | |
| `0x08014BAC` | 0x84 | matching | Wait for `DuelScreen_FadeOutStep`, then open the Card Detail view `CardDetail_Init(arg2, 300, 0)` and wait for `CardDetail_Run` | |
| `0x08014C30` | 0x4AC | matching | 9-step effect: `UnloadDuelUiGfx`, `LoadCardFrame/1D24/1E54(arg2)`, then a 4×5 grid of 32×32 sprites that zooms in (`gScatterScaleCurve`, alpha fade, `AddSprite8bppAlpha`), flashes (BLDY, `AddSprite8bpp`), and fades out. Calls `TextCellsClear` / `DuelInfo_DrawCardNameCentered(arg2)` and SE 0x2C in between, and `LoadDuelUiGfx` at the end | |

## Shared headers and local views

The unit includes `main.h` and `duel.h` and no longer defines `struct Main`, `DuelCard`,
`DuelZone`, `DuelPlayer` or `DuelState` (5 local struct definitions removed; the
`gDuel` / `gDuelPlayers` / `gMain` externs also come from the headers).
Two small per-unit views are kept because `duel.h` declares the bytes differently; the
headers are shared and were not changed:

- `struct DuelPlayer08013CDC`, aliased as `gUnk_020192E4_lp[2] asm("gDuelPlayers")`.
  `duel.h` declares player +0x06..+0x0D as `u8` fields/bitfields (`countB84`,
  `flag7_3..turns7_6`, `unk8`, `unk9`, `unkB_0`, `flagsC`), but this unit's original
  `u16` bitfield containers are needed by:
  - `DuelCmd_SetSummonLocks`: writes two +0x07 bits (`flag6_11`, `flag6_12`) and only matches
    with a single `u16` load/store (canonical `u8 flag7_3`/`flag7_4` writes are 4 bytes longer);
  - `DuelCmd_SkipNextDrawPhase`/`DuelCmd_SkipNextStandbyPhase`/`DuelCmd_SkipNextTurn` (+0x09 bits 1/2/3) and `DuelCmd_SetExtraBattlePhase`
    (+0x08 bit 6), whose bits have no canonical field name (`unk9`/`unk8` are plain bytes);
  - `DuelCmd_AdjustDelayedSummonCount`/`sub_08014B5C` (+0x0C bits 1–3 / bit 4), likewise `flagsC` is a plain byte.
  `DuelCmd_SetPositionChangeLock` (canonical `flag7_5`) and `DuelCmd_SetMagicTrapLockTurns` (canonical `turns7_6`) do match
  the header and use it directly.
- `struct DuelFlags08013CDC`, aliased as `gUnk_020192E0_flags asm("gDuel")`:
  `duel.h` folds +0x1ACC..+0x1ACE into `u32` bitfields (`unk1ACC_0`/`queueCount`), so the
  +0x1ACD bits 5/6 written by `DuelCmd_SetStatChangesReversed`/`DuelCmd_SetAtkDefSwapped` need this finer view.

## Data

- `gDuelPlayers[2]` player state (0xD64): canonical `duel.h` names +0x6 `countB84` (u8), +0x7 `deckOut`/`winA`/`winExodia`/`flag7_3`/`flag7_4`/`flag7_5` and `turns7_6`, +0x8 `unk8`, +0x9 `unk9`, +0xB `unkB_0`, +0xC `flagsC`; +0x28 `zones`; +0xCC4 `arrCC4[80]`. The unit's local view re-splits +0x06..+0x0D as `u16` bitfields (`numLinks`, `flag6_11..turns6_14`, `flag8_3..unk8_12`, `unkA`, `turnsB_0`, `counterC`, `flagC_4`).
- Zone (0x94 bytes): canonical `duel.h` +0x0 `card.id` (bits 0–11), +0x6 `flag6_1` and `counter6` (bits 2–5), +0x7 `unk7` (plain byte), +0x91 (`unk8C[5]`) bit 2. The unit's register-level accesses are **byte** operations (`ldrb`/`strb`, `and #2` tests); the `#if 0` draft masks `unk7`/`unk8C[5]` directly because the header has no bit names for +0x07/+0x91.
- `gDuelScreen` bit 0 `fast` (fast-forward), bit 2.

## Matching tricks

- **One temporary per read (`DuelCmd_ShowCardAssemble`).** Reusing a single `int t` for every `timer` read across cases 5–7 gives one pseudo and shifts registers everywhere. Separate locals (`ta`…`te`) match.
- **`dy = y - 0x40` as a new variable, `x -= 0x68` in place (`DuelCmd_ShowCardAssemble`).** CSE then folds `dy` to `i*32 - 0x3E` while `x` stays in its register, as in the ROM.
- `timer` must be a `u32` bitfield for the signed compares. The earlier `(s16)` cast in `DuelCmd_ShowEndTurnHand` still matches with the `u32` container.
- Card tables use the pointer form `*(gCardIdToNumber + (id & 0x7FF))`, which computes the index before loading the table address.
- `DuelCmd_TurnStart` (asm): with `if ((s8)self->turnsB_0)` the `lsl #29` test matches. The ROM then recomputes `player*0xD64` inside the zone loop and hoists only the `0x0201930C` base and the `~0x3C` mask. Our build hoists `player*0xD64`, which moves `player` to a high register and shifts every later allocation. This was not resolved.
