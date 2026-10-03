---
title: Unit duel_main (duel main step, first/last phase, win check)
type: function
status: verified
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit duel_main

The unit covers `0x08020AF4`–`0x08021CC7` (Thumb, `old_agbcc -O2`), and its source is `src/duel_main.c`.
It holds the shared duel step `DuelMainStep` (`0x08021A48`, see [[program-flow]]), the handlers of duel phases 0 and 9 (entries 0 and 9 of the phase table `0x08198F80`), the win/lose check, and the action-list resolver. Related: [[duel-setup-c]] (command queue, phase 1, action lists), [[duel-prompts-c]] (duel messages).

Unit status: **8/8 functions in C**, `unit bytes MATCH` (verified with `tools/check.py duel_main`). All functions are enabled as ordinary C, with no compiler hints. The entire 0x11D4-byte unit and the full ROM match (evidence in `build/lead-pass6/`).

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x08020AF4` | 0x8CC | **matching**, ordinary C | resolves the action entries of list B (called from `0x080213C0` while `0x02017A40+0x3D2` bit 0 is set) | |
| `0x080213C0` | 0x1C0 | **matching**, ordinary C | Action-list step: continue `Chain_Build` / `Chain_Resolve` while their flags (`+0x3D0` / `+0x3D2` bit 0) are set. Otherwise copy list A to list B, then drop the card-0x4DA entries beyond the count `CountGraveyardCardsByNumber(p, 0x4DA)` of each player, clear list A, and set `+0x3D0 = countB != 0`. Returns u16 busy | `ActList_Step` |
| `0x08021580` | 0x4C | matching | `CountHandCardsByNumber(p, 0x10..0x14)` all true | |
| `0x080215CC` | 0x5C | matching | player has all of cards 0x5F8, 0x605, 0x606, 0x607, 0x608 (`CountActiveCardsOnField`) (hypothesis: the five Exodia pieces) | `HasAllExodiaPieces` (hypothesis) |
| `0x08021628` | 0x20C | **matching**, ordinary C | **Win check**, only in phases 2–8 and not in link-skip mode. Result `0x020192E0+0x1B12` bits 6–7 (3 = draw by default): if LP is 0, the higher LP wins; else the deck-out flag (player `+7` bit 0); else `HasExodiaInHand` (player `+7` bit 1); else `HasDestinyBoardComplete` (bit 2). Sets `+0x1ACC` bit 4 and returns 1 when the duel is decided | `Duel_CheckWin` |
| `0x08021834` | 0x78 | matching | **Duel phase 0**: `linkSkip = gMain+0x4870 bit 0`; in a link duel with link-skip, count `+0x1B10`, send link message `0xF001`, and jump to phase 8 | `DuelPhase_Init` |
| `0x080218AC` | 0x19C | matching | **Duel phase 9** (end): stop the BGM, clear `+0x1B13` bit 0, push commands 0x13/5/0x12 if a player has `+7` bit 1, 0x13/6/0x12 for bit 2, then command 4 with arg 0/1/2 for result 1/2/3. Wait for the command queue; in a link duel send `0xF003` with `1 - result` and wait 20 frames | `DuelPhase_End` |
| `0x08021A48` | 0x280 | **matching**, ordinary C | **DuelMainStep.** If the phase table entry is NULL: copy the result to `gMain+0x4870` bits 6–7, `SaveGame`, return 1. Else run, in priority order: `DuelScreen_Update`, the remote command (link, `0x02017FB0+0x307` bit 0), the command queue `DuelCmdQueue_Run`, `CardListView_Run`; the `0x0201AE60` callback; then `TextBoxUpdate`, the duel message `DuelPrompt_Run`, `SummonAction_Update`, the action lists `Chain_Update`, `UpdateSpellTrapNegation(1)`/`EventResponse_Update`. In phases 3–6 it handles the surrender flag (`+0x1B14` bit 1) and the link partner's request (`+0x306` bit 6, selection widget state 2 gives command `0x8041`, link `0xF005`). Then it calls the phase handler; `Duel_CheckWin` forces phase 9, and a handler return of 1 advances the phase (step and timer `+0x1B20`/`+0x1B21` reset) | `DuelMainStep` (names.txt) |

## Data

- `0x020192E0`: `+0x1ACC` bit 4 duel decided; `+0x1B10` u16 link counter; `+0x1B12` bit 1 link-skip, bit 5 link error, bits 6–7 result (1 = player 0 wins, 2 = player 1 wins, 3 = draw; hypothesis from the LP comparison); `+0x1B13` bit 0; `+0x1B14` bit 1; `+0x1B20` phase step, `+0x1B21` phase timer; `+0x1B2C` selection widget (`struct SelMask` from [[campaign-c]]; its `state:8` straddles `0x1B2F`/`0x1B30`).
- Per player (`0x020192E4`, 0xD64): `+0x000` u16 LP; `+0x007` bit 0 deck-out, bit 1 `HasExodiaInHand` win, bit 2 `HasDestinyBoardComplete` win.
- `gMain+0x4870`: bit 0 = start in link-skip mode, bits 6–7 = last duel result.
- `0x02017A40+0x3D0` bit 0, `+0x3D1`, `+0x3D2` bit 0: action-list resolver flags.

## Matching tricks

- **`gMain+0x4870` bit 0 must be a `u16` (or `u32`) bitfield** (`0x08021834`). With a `u8` container the `& 1` mask gets CSE'd with the later link-flag test.
- **Phase range 3–6 in `DuelMainStep`**: `switch (phase) { case 3: case 4: case 5: case 6: ... }` gives the ROM's `cmp #6; bgt; cmp #3; blt`. Every `&&` or `if` form folds into `sub #3; cmp #3; bhi`.
- **The 8-bit selection state at `+0x1B2C` bits 26–33 uses a `u16` container and straddles two words.** Its physical bit positions are unchanged from the old u32 draft. The narrower declared type restores the original register allocation and allows the compiler to merge the two phase-reset tails.
- `0x08021628`: player 0's flags go through a pointer `p0 = gDuel.players` (the ROM computes `base + 4` once), while its LP is read as `players[0].lifePoints`.

## Completing the controller unit

- `Chain_Update` (0x1C0 slice, including two alignment bytes): keep a real local
  `u16 number` for each action-list card lookup before comparing with 0x4DA.
  This restores the shared card constant and inner removal-loop allocation.
  After clearing list A, form the negated unsigned count, then the active-byte
  destination, then shift the value by 31. Since countB is u16, that expression
  is exactly its nonzero test. This preserves the original split scheduling.
- `Duel_CheckWin` (0x20C): both five-card predicates return u16. Their actual
  instructions return only full-word 0 or 1 on every path, so the declaration
  preserves their complete register result and argument ABI. Their definitions
  still match exactly. The corrected return types also recover the win check's
  register allocation without the former input hint. All uses/declarations are
  local to this unit; checked against the source and assembly call sites.
- `DuelMainStep` (0x280): the selection-state container is u16, as above. The
  original C branch structure now emits the shared phase-store/reset tail; no
  explicit goto rewrite or compiler hint is needed. The UI flag test reads byte
  zero of the same object, keeping the resolver's expanded UI view consistent.

Reproduction and exact checks: `build/bigguns-lead2/list_locals.py`,
`list_final.py`, `list_clean.py`, `win_returns.py`, `main_selection.py` and
`main_clean.py`. Whole-unit and full-ROM verification: `build/lead-pass6/`.

Historical local grids for fixed registers, alternate views and value hints
were non-improvements. Their private logs remain available; the final sources
supersede the old 14-byte win-check and 42-score action-list frontiers. Do not
repeat those searches on the now-matching functions.

## Action-list resolver in matching C

`Chain_Resolve` is enabled as ordinary C, and all 0x8CC bytes match. The complete
0x11D4-byte unit and full ROM compare pass. It preserves the original 0x12C
frame, ABI, dispatch states 0–6/100/default, packet workspaces and helper calls.
There are no register bindings or empty assembly constraints.

The final argument locals matter: separate u16 message/zone values, and u16
low/high halves of the saved card, restore argument scheduling. Initialize the
actual saved-card destination pointer before the player-relative board pointer,
then add the zone stride. This restores the original base-add order. Explicit
hints were tested and removed, because ordinary locals alone retain the complete match.
Evidence: `build/bigguns-chain/solo_arguments.py`, `solo_zones.py`,
`solo_copy.py`, `solo_clean.py`; final private exact check in
`build/bigguns-lead2/Chain_Resolve/solo-clean/check.txt`. Combined verification is in
`build/lead-pass5/`.

The reconstructed state has the signed effect index at +0x3D6, callback at
+0x3D8, saved card at +0x3DC, and effect bytes +0x3E0/+0x3E1. The ROM reserves
0x100 bytes for the first packet workspace, then a 44-byte pair packet with a
count, previous-entry flag and two 20-byte entries. When count is one, the
unused second entry remains uninitialized, preserving the original behavior.
State 4's blocked-effect response returns zero; other paths return one.
Spell subtype 3 checks mask 3 at duel +0x1ACD, as in the ROM.

Earlier reconstruction recovered separate outer-case returns, the state-3 early
wait return, the saved-card address through its actual containing object and a
u16 pure subtype reader. Four successive private variants each passed 7,592
finite Thumb comparisons across all 128 states and all 821 card IDs with eight
synthetic-callee profiles. The harness compares returns, EWRAM, ordered calls
and initialized packet bytes, clobbers caller-save registers and excludes the
unused packet tail. These finite checks were not proof, and acceptance now rests
on exact whole-unit and ROM bytes. Evidence: `build/bigguns-chain/verify.py`
and its `behavior*.log` files.

Historical failed searches remain recorded privately: field-container and
step-helper grids, fixed message registers and a 2,498-iteration bounded
permuter pass. Do not repeat them on the now-matching function.
