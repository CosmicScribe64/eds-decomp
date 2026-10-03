---
title: Unit duel_cmd_moves (duel command handlers, summon / LP banners / rule flags)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit duel_cmd_moves

`0x08012C4C`–`0x08013CDB`, Thumb, `old_agbcc -O2`. Source: `src/duel_cmd_moves.c`.
This unit continues the duel "script command" handlers of [[duel-cmd-hand-c]]. Each handler reads its operands from the command block `gDuelCmd`, runs a small per-frame state machine on `gDuelCmd.step`, and clears `running` (bit 5 of byte `0x020185C0+0x80D`) when it finishes.

Unit status: `unit bytes MATCH`, **12/12 functions in C** after workflow wave 2 (2026-10-01: `0x08012D7C` in wave 2); none stay `INCLUDE_ASM`. Before wave 2: 11/12. Verified with `tools/check.py duel_cmd_moves`.

## Shared headers

The unit uses the canonical shared headers instead of local definitions:

- `main.h`: `struct Main`, `extern struct Main gMain` (aliased locally as `gMain`).
- `duel.h`: `struct DuelCard`, `struct DuelZone`, `struct DuelPlayer`, `struct DuelState`, `struct DuelZonesPlayer` and the externs `gDuel`, `gDuelPlayers`, `gDuelZones`.

Five local struct definitions (`Main`, `DuelCard`, `DuelZone`, `DuelPlayer`, `DuelState`) and the local externs for the four globals were removed. Field renames: zone `unk6_0`/`unk6_1` became `flag6_0`/`flag6_1`, `unk8C` became `unk8C[0]`, and the draft's `unk8_0` became `unk8[0]`.

Two unit-local views are kept because the canonical layout differs from what the ROM code needs:

- `struct DuelZone08012C4C` (used in `sub_08013104`): `duel.h` declares zone `+0x08` as `u8 unk8[2]`, but this function writes only bit 0 and the ROM compiles a bitfield read-modify-write; a plain byte store does not match.
- `struct DuelFlags08012C4C` / `gUnk_020192E0_flags asm("gDuel")` (used in `DuelCmd_SetNegationFlag`): `duel.h` folds 0x1ACC bits 6-7 and 0x1ACD bits 0-4 into `unk1ACC_0` (bits 0..14) and `queueCount` (bits 15..18); this unit needs the finer bit split.

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x08012C4C` | 0x130 | **matching**, ordinary C | Summon/set a card: `arg2` = slot \| face-down bit 8, `arg4/arg6` = card word; card numbers 1920–1999 are forced face-down. Sets zone +0x8C bit 1 for card types > 20 | |
| `0x08012D7C` | 0x158 | **matching** (wave 2, 2026-10-01; FAKEMATCH) | Take the card off field zone `arg2` into `cmd.card`, animate it to the owner's area 15, then `AddCardToBanishedTemporarily(card, slot)` | |
| `0x08012ED4` | 0xD0 | matching | Area 15 → field zone `arg2` animation, then `ReturnTemporarilyBanishedCard(player, slot)` | |
| `0x08012FA4` | 0x160 | matching | Field zone `arg2` → owner's area 14/15 (`arg4 != 0` → 15). Sets the zone card's flag20, copies it to `cmd.card`, calls `SendZoneCardToGraveyardOrBanished(player, slot, arg4)`. Zone +6 bits 0/1 → `from.flag14/15` | |
| `0x08013104` | 0x50 | matching | `zone[arg2].unk8_0 = arg4` | |
| `0x08013154` | 0x3C | matching | `zone[arg2].numLinks (+0x8A) = 0` | |
| `0x08013190` | 0x200 | matching | Banner `arg2` (0–4, table `gDuelResultBanners` of {pal, gfx, jingle}): load graphics, start jingle `PlayJingle`, zoom in over 0x40 frames, wait for `SoundIsBGMPlaying(jingle) == 0`, hold 0x1E frames | |
| `0x08013390` | 0x15C | matching | "LP 0" banner (0x60 frames, sprite `0xF364` with a wave from `gBounceScaleCurve`, fast-forward +7), then `lifePoints = 0` and `DrawLifePoints(player)` | |
| `0x080134EC` | 0x30C | matching | Banner that slides in from the acting player's side (`gBannerSlideOffsets` offsets), pulses 0x40 frames (`gPulseScaleCurve`), then slides out; SE 0x16 | |
| `0x080137F8` | 0x90 | matching | Draw a signed decimal number as 16-px digit sprites, right-aligned at x+0x50 | `DrawLpNumber` (hypothesis) |
| `0x08013888` | 0x328 | matching | `(u16 gain)`: LP change animation. Shows ±`arg2` with `DrawLpChangeAmount`, then counts it into `lifePoints` 100/10/1 per frame (`SubtractLifePoints` subtracts, clamped). SE 12 (gain) / 13 (loss) every 10 frames. Stops early if LP hits 0 | `DuelLpChangeAnim` (hypothesis) |
| `0x08013BB0` | 0x12C | matching | Rule-flag commands 0x15–0x1B: store `arg2` into one bit of `gDuel+0x1ACC/0x1ACD` | |

## Data

- `gDuelCmd` command block: +0x0 `cmd` (bits 0–11 id, bit 15 acting player), +0x2 `arg2`, +0x4 `arg4`, +0x6 `arg6`, +0x80A bits 0–6 `step`, +0x80C bits 5–11 `timer`, bit 13 `running`, +0x814 card word (`struct DuelCard`).
- `gDuelPlayers[2]` per-player duel state (0xD64 bytes): +0x0 `lifePoints`, +0x28 11 zones of 0x94 bytes (`0x0201930C`).
- Zone: +0x0 card word (id:12, owner:1, flag20 bit 20), +0x6 bit 0/1 (copied into CardLoc flag14/15), +0x8 bit 0, +0x8A `numLinks` (hypothesis), +0x8C bit 1.
- `gDuelScreen` duel screen state: +0x0 bit 0 `fast` (fast-forward), +0x808 bit 3 `busy`, +0x85C word.
- `gMain.heldKeys` (+0x04), bit 1 (B button) fast-forwards animations.

## Matching tricks

- **`switch` on a signed local.** Handlers with cases 0–3 compile to a `cmp 1; beq; cmp 1; bgt; cmp 0; beq` tree with signed branches. That needs `s32 step = gDuelCmd.step; switch (step)`. A `u32` local gives `bcc`.
- **`gDuelCmd.step = step + 1`** (with the value of `step` known in that case) compiles to the constant-first `mov r1,#3; ... orr` that the ROM uses. A literal `= 3` loads the constant after the mask.
- **Hoisted base pointer (`DuelCmd_ChangeLifePoints`):** `players = gDuelPlayers; lp = &players[player];` and pass `players` to `SubtractLifePoints`. This loads the base into r8 before the `0xD64` multiply.
- In `DuelCmd_ShowJustAMomentBanner`, case 2 must read `gDuelCmd.timer` directly, while cases 1 and 3 read it into an `s32` local first.

- **`timer` must be a `u32` bitfield** (`u32 unk80C_0:5; u32 timer:7; ...`). With a `u16` container, `timer < 0x60` / `timer <= 0x77` compile to unsigned `bhi`. With `u32`, they compile to the signed `bgt` that the ROM uses (`DuelCmd_Surrender`, same as [[duel-cmd-screen-c]]).
- **Use `struct CardLoc` with `u16` bitfield containers** for `DuelAnim_MoveCard` arguments. With `u32` containers the compiler stops caching `&gDuelCmd.step` in a register (`ldr =0x80A; add rX, base`), which the ROM does.
- **Zone addresses:** compute both expressions from `gDuelPlayers`, not the `gDuelZones` alias. `ZONE(player & 1, slot)` (= `(u8 *)gDuelPlayers[0].zones + s*0x94 + p*0xD64`) and `&gDuelPlayers[player & 1].zones[slot]` then share one base constant, as in the ROM (`DuelCmd_SendFusionMaterialToGrave`). Mixing the two symbols loads `0x0201930C` twice.
- `DuelCmd_SetMagicalHatsCard`: integer-address card-number and stats lookups fix table scheduling. Inside the nonmonster case, compute a local byte pointer from the zone base plus `(slot * 0x94 + player * 0xD64)`, then update byte `+0x8C`. This preserves the copied slot multiply and flag-store allocation; using a full zone-struct pointer does not.
- Historical (matched in wave 2, see below): `DuelCmd_BanishMonsterUntilEndPhase` (asm): only register allocation differs. The ROM keeps player/slot/step in r7/r6/r5 and `&step` in sl. Our build puts step in a high register and merges the case 0 and case 1 `step++` tails.

## Summon/set command matched (2026-10-01)

`DuelCmd_SetMagicalHatsCard` matches all 0x130 bytes as ordinary C after the table/address changes above. Both table expressions preserve the existing masks and fixed ROM data; the raw flag byte remains an OR with 2. No call ABI, scene state or animation arguments changed. The clean whole-unit check passes with 11/12 C functions; full ROM and objdiff validation passed. Evidence: `build/bigguns-lead2/summon_address.py`, `DuelCmd_SetMagicalHatsCard/solo-clean/`, `build/lead-pass9/`.

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/DuelCmd_BanishMonsterUntilEndPhase/NOTES.md` (helper script `build/wf/DuelCmd_BanishMonsterUntilEndPhase/exp/ana.py`).

### `DuelCmd_BanishMonsterUntilEndPhase` (0x158, start score 52; FAKEMATCH)

- Only register choices differed (ROM: player r7, slot r6, step r5, card pointer r8, `0x0201930C` r9). The draft's `s8 player` made a block-local copy of player in case 1, and local-alloc never hands out r7 (the Thumb frame pointer). `u32 player` (as in the matched siblings `DuelCmd_BanishHandCardFaceDown` in [[duel-cmd-hand-c]]) put player in r7 but step in r9, and the case 0 / case 1 `step++` tails cross-jumped (194).
- Diagnosis from the `.lreg`/`.greg` dumps: in case 1, local-alloc gives r4/r5/r6 to call-crossing locals (`slot*0x94`, `(p&1)*0xD64` and the `-2` mask, the base). Slot and step find no free low register, so global.c `find_reg` evicts a local-alloc'd hard register, scanning r6, r5, r4, when the local's refs/live-length ratio is below the global's. r6's ratio was 3/20 = 0.150 and slot's 7/49 = 0.143, so slot evicted r5 instead of r6. (`update_equiv_regs` doubles the live length of the REG_EQUIV constant-pool pseudo: flow 10 becomes 20.)
- FAKEMATCH: `asm("" :: "r"(player), "r"(slot));` at the top of case 1 adds one use to each (player 8/43, slot 8/50 > 0.15), keeping the priority order player > slot. Every natural variant (zone forms, slot/player/step types, a step local, case order, `CardPos` vs `CardLoc`) gave the same counts; only `s16 slot` changed them and it adds sign extensions.
- Failed: `register u32 slot asm("r6")` (56; recomputes `slot*0x94`, swaps r8/r9), a `"+r"` asm on slot alone (24; slot takes r7), `"+r"` on both (108/176).
