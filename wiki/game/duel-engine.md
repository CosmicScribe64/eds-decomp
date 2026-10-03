---
title: Duel engine
type: game
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Duel engine

An overview of the duel's data and control structures, as established while decompiling. Layouts are verified
(compile-time checked in the shared headers); meanings marked "(hypothesis)" are not.

## State (see [[shared-headers]] for the full structs)
- **`gDuel`, `struct DuelState`**: a u32, then the two players, then the marked-card queue
  (`queueCount`, `queueZone[16]`, `queueArg[16]`) and the duel flags at `+0x1B12` (`linkSkip`, `linkError`,
  `result`, a phase field), with a step byte at `+0x1B20`.
- **`struct DuelPlayer`** (0xD64 bytes each, at `gDuelPlayers`):
  - Life points.
  - Counts at `+0x2`..`+0x6`.
  - 11 field zones at `+0x28` (`struct DuelZone`, 0x94 bytes each: a card word, a serial, flags and counters, and up
    to 32 links).
  - Five 80-card lists at `+0x684` (hand), `+0x7C4` (deck, hypothesis), `+0x904` (graveyard), `+0xA44` (fusion deck,
    hypothesis) and `+0xB84`.
- **`struct DuelCard`**: a 32-bit card word. Bits 0..11 hold the card ID (alphabetical index into the
  [[card-table]]), bit 12 the owner, and the rest are flags.

## Control
- **`gDuelCmd`, `struct DuelCmd`** (`include/duel_ui.h`): the duel command runner.
  - The current command (`cmd`, with the player in bit 15, plus three args) and a 256-entry command queue.
  - A step machine: a 7-bit `step` plus `timer` and `running` bits.
  - Saved copies of the deck and fusion lists (hypothesis).
- **`gDuelScreen`, `struct DuelScreen`** (`include/duel_ui.h`): the field view.
  - Cursor coordinates and their animation.
  - Scrolling and a tile buffer.
  - The selected player, zone and index.
  - A move animation (`from` and `to` as `struct DuelLoc`: player, area, index).
- Many duel routines are dispatched through function-pointer tables; see [[function-pointer-tables]]. For example,
  a 40-entry table at `0x08198E7C` is used by `CB_LinkBattle`, and a 39-entry table at `0x0819A6B0` by `TurnOrder_RpsMain`.
- Card effects: see [[cards]] and the effect table described there. The largest function in the game,
  [[code-08044224]], appears to be a card-condition search used by the list views (hypothesis).

## Open
The turn structure, the phases and the CPU AI are still open (units around `code_08057EE0` to `code_0805C508`; see their unit pages).
