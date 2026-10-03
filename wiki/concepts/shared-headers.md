---
title: Shared headers
type: concept
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-03
---
# Shared headers

Until 2026-09-30 every C unit declared its own view of the shared globals. A survey (`tools/typesurvey.py`) found
`gMain` declared with 9 different types across 65 units, and `gDuelPlayers` with 20. `struct Main` alone
had 43 distinct layouts. This caused near-misses, because a field declared u8 in one unit and u16 in another
compiles differently.

## Canonical headers
- `include/main.h`: `struct Main` (gMain, `gMain`, known up to `+0x488C`).
- `include/duel.h`: `struct DuelCard`, `DuelZone` (0x94), `DuelPlayer` (0xD64), `DuelState` (prefix only),
  `DuelZonesPlayer`, with externs for `gDuel`, `gDuelPlayers` and `gDuelZones`.
- `include/duel_ui.h`: `struct DuelCmd` (`gDuelCmd`, the command runner) and `struct DuelScreen`
  (`gDuelScreen`), plus `DuelCmdEntry` and `DuelLoc`. It is kept separate so that adding it did not break units that
  already included duel.h.
- `struct DuelState` covers the players plus the queue/flags tail up to `+0x1B20` (the contested bitfields are left unknown).
- All headers carry compile-time size and offset checks. Field access was verified by compiling probes with old_agbcc
  (for example `opponent` is bits 1..5 of `+0x4870`, and `zones[3].counter6` is at `+0x1EA` bits 2..5).

## How they were built
- `tools/structmap.py <addr>` parses every unit with pycparser. It computes each named field's byte and bit offset
  with agbcc's layout rules, and lists which names and types the units use at each offset.
- The layout rules were verified against old_agbcc:
  - every struct is aligned and padded to 4 bytes (STRUCTURE_SIZE_BOUNDARY 32; `sizeof(struct {u8 a;}) == 4`);
  - bitfields pack LSB-first and contiguously, across declared types and across byte/halfword/word boundaries.

> [!warning] Contradiction (resolved 2026-10-03)
> This page used to say that a bitfield jumps to the next boundary of its own type's size when it would straddle
> one, and `tools/structmap.py` implemented that rule. Compiling probes shows otherwise: with old_agbcc,
> `struct { u16 a:12; u16 b:8; }` puts `b` at bits 12-19 (initialiser bytes `00 f0 0f`, sizeof 4), a `u32 x:8` at
> bit 26 is read with two `ldrb` (duel_core writer's probe), and the matched code relies on it (`SummonAction.cardId` spans bits 31-46,
> `DeckReorder_Run` reads a timer across `+0x53F`/`+0x540`). Only a zero-width bitfield aligns (to 32 bits).
> structmap.py was fixed (the straddle rule removed); the duel_core and duel_flow header writers found the bug
> independently.
- `tools/mkheader.py` drafts a struct from those maps (the most-used meaningful name wins, with alternatives in
  comments). The drafts were finalised by hand.

## Per-subsystem headers (readability pass, 2026-10-03)
- `tools/headerplan.py` writes the plan (`build/readability/header_plan.json`): 47 headers in a fixed include order
  (`include/constants/*.h` first, then `gba.h`, `main.h`, `util.h` ... `duel_link.h`), which header owns every
  struct, enum, global and prototype, and each prototype exactly as its definition compiles today.
- Eleven writer groups wrote the headers; one integrator pass made them consistent: all 47 pass
  `tools/hdrcheck.py --all` with 0 errors and 0 warnings, and one TU that includes all of them (in order, twice, and
  in reverse order) compiles with old_agbcc and agbcc at `-W -Wall` with no diagnostics.
- Step H0 has not run yet: `include/gba.h`, `main.h`, `duel.h`, `duel_ui.h` and `sound.h` are still the legacy
  headers above, which most units include. The new versions of `gba.h`, `main.h`, `duel.h` and `sound.h` are
  staged under `build/readability/hcheck/` until H0 moves the legacy files to `include/legacy/`.
- `src/text_render.c` is the first unit migrated to the new headers (palette.h, bg.h, sprite.h, text.h, plus gba.h):
  `unit bytes MATCH`, and the same assembly with the legacy and the staged gba.h. See [[text-render-c]].
- The migration guide (which header for what, how to keep a deliberate local view, per-unit notes from the header
  writers) is `build/readability/HEADERS.md`.

## Player layout (verified offsets; some meanings are hypotheses)
Five 80-card lists (0x140 each), whose counts sit at `+0x2`..`+0x6`:

| Offset | List | Count field |
|---|---|---|
| `+0x684` | `hand` | `handCount` |
| `+0x7C4` | `deck` (hypothesis) | `deckCount` |
| `+0x904` | `graveyard` | `graveCount` |
| `+0xA44` | `fusionDeck` (hypothesis) | `fusionCount` |
| `+0xB84` | `listB84` | `countB84` |

The 11 zones start at `+0x28`, and a u16 table sits at `+0xCC4`.

## Migration status (2026-09-30)
About 30 units were migrated, all still MATCH. Most needed 0 local views. The views that were kept (11 at one count)
are mostly the `+0x1ACC`..`+0x1B18` duel-state bytes, a DuelCard flag at bit 18 and DuelZone `+0x7` bits. Folding them
into the headers is an open task. Change `include/*.h` only between migration waves, and re-check every
including unit afterwards. One unit lost its match when duel.h changed during its migration; it was restored.

## Migration
Units move to the headers one at a time: `tools/launch_headers.sh <unit>` runs a DeepSeek agent that includes the
headers, deletes the local definitions, renames fields by offset and verifies with check.py. Where a function only
matches with a different declared type, the unit keeps a small local view with a comment. The headers are not
bent to fit one unit.
