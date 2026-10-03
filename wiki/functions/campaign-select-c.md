---
title: campaign_select (Calendar + Campaign opponent-select screens)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit `campaign_select` (`0x08002388`-`0x080034B8`)

The Calendar screen (`0x08002388`-`0x08002930`) and the Campaign opponent-select
screen (`0x08002940`-`0x08003298`). Compiler: `old_agbcc -mipsel -O2` i.e.
`old_agbcc -O2` ([[compiler-flags]]). The unit as a whole matches
(`check.py`: `unit bytes MATCH`, built `0x1130` vs target `0x1130`): **20/20 functions in C**
after workflow wave 3 (2026-10-02: `0x08002388` in wave 3); none stay `INCLUDE_ASM`.
Before wave 3: 19/20. `Calendar_Init` is also in C (matched 2026-09-30, see the log);
its match is not yet written up on this page, and its notes below still describe the
parked draft. Earlier text here said 18/20 with both functions parked.

## Shared headers

This unit was migrated to the canonical shared headers (2026-09-30):

- `#include "global.h"` (as before)
- `#include "main.h"`: the local `struct Main` and its `extern struct Main
  gMain;` were deleted; the unit now uses the canonical `struct Main` from
  `include/main.h`. It touches `newKeys` (+0x0006), `vblankFlags` (+0x040E),
  `brightness` (+0x4832), `seqIndex1` (+0x4859), `seqState1` (+0x485A),
  `seqState2` (+0x485B) and `opponent` (+0x4870 bits 1-5). The unit's `#define
  gMain gMain` is kept (a convenience alias, not a declaration).
- `include/duel.h` is **not** used: this unit defines no duel structures and never
  references `gDuel`/`gDuelPlayers`/`gDuelZones`.

Local views kept: `struct Calendar`, `struct Date` and `struct SaveData` (calendar
state at `gCalendar`, unpacked date, and the save-image `days` field) are
defined locally in this unit too, mirroring [[bustup-runner-c]] and
[[title-screen-c]]; `struct OpponentSelect` (below) is the other local view.

## Functions

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x08002388` | 0x1F4 | **matching** (wave 3, 2026-10-01; FAKEMATCH) | `Calendar_Draw` (hyp.) | draw the month grid: year digits, day numbers, event icons, cursor |
| `0x0800257C` | 0x188 | parked | `Calendar_Init` (hyp.) | clear calendar state, set up video, load graphics |
| `0x08002704` | 0x24 | matching | `Calendar_Show` (hyp.) | set DISPCNT and fade in |
| `0x08002728` | 0x208 | matching | `Calendar_Input` (hyp.) | calendar date/cursor input, blink state |
| `0x08002930` | 0x10 | matching | `Calendar_FadeOut` | fade to black |
| `0x08002940` | 0x40 | matching | `CB_Calendar` | step runner over `gCalendarSteps` |
| `0x08002980` | 0xC8 | matching | `OpponentSelect_Init` (hyp.) | clear state, reset video, load graphics |
| `0x08002A48` | 0x58 | matching | `OpponentSelect_Show` (hyp.) | draw the screen and fade in |
| `0x08002AA0` | 0x248 | matching | `OpponentSelect_Input` (hyp.) | cursor/input for the opponent grid |
| `0x08002CE8` | 0x4C | matching | `OpponentSelect_FadeOut` (hyp.) | draw the screen and fade out |
| `0x08002D34` | 0x14 | matching | `SetBg2X` (hyp.) | set BG2X to `x * 2` (20.8 fixed) |
| `0x08002D48` | 0x138 | matching | `OpponentSelect_PrevPage` (hyp.) | page-turn animation (left) |
| `0x08002E80` | 0x13C | matching | `OpponentSelect_NextPage` (hyp.) | page-turn animation (right) |
| `0x08002FBC` | 0x14 | matching | `OpponentSelect_Close` (hyp.) | switch back to the main menu |
| `0x08002FD0` | 0x50 | matching | `CB_OpponentSelect` | step runner over `gOpponentSelectSteps` |
| `0x08003020` | 0xDC | matching | `DrawPortraitFrame` (hyp.) | 64x64 frame from four mirrored corners |
| `0x080030FC` | 0x78 | matching | `DrawPageArrows` (hyp.) | page-scroll arrows |
| `0x08003174` | 0xC8 | matching | `DrawOpponents` (hyp.) | portrait frames of unlocked opponents |
| `0x0800323C` | 0x5C | matching | `MoveCursor` (hyp.) | move the cursor to `slot` and reset its animation |
| `0x08003298` | 0x220 | matching | `LoadOpponentPage` (hyp.) | load a page background and draw its labels |

## Structures and globals

- `gMain` (`0x03000040`): canonical `struct Main` from `include/main.h` ([[shared-headers]]); fields listed above.
- `gSel`/`gOpponentSelect` (`0x0201F7E0`, shared with [[main-menu-c]]): local
  `struct OpponentSelect`: 4 bytes of bitfields (`page:3 | cursor:3 | unk6:3 |
  unk9:4 | unk13:4 | unk17:15`), then `u16 cursorX[8]` (+0x04), `u16 cursorY[8]`
  (+0x14).
- `gCalendar` (`0x0201F7D0`): the calendar state, same layout as `struct
  Calendar` in [[bustup-runner-c]]: `u16 unk0` (anchor date, +0x00), `u16 date`
  (current date, +0x02), `u32 shownEvents` (+0x04), then a `u16` bitfield word at
  +0x08: `blink:2`, `secondHalf:1`, `page:1`, `weekday:3`, `week:3`.
  The scene functions here (257C/2388/2728) operate on it.
- `gSaveData` (`0x02011C20`, save image): `u16 days` at +0x2150 (in-game
  calendar day counter). 257C copies it into both `unk0` and `date`; 2388 uses
  `DayCountToDate` to unpack both.
- Date helper `DayCountToDate(struct Date *, u16 days)` (title_screen) outputs
  `struct Date { u32 year:12; u32 month:4; u32 day:5; u32 weekday:3; }`; the
  calendar routines read `.year`/`.month`/`.day` from it.
- Data tables: `gCalendarSteps`/`gOpponentSelectSteps` step tables; `gOpponentSelectDuelists`
  opponent id `[page * 5 + slot]`; `gOpponentSelectSlotPos[5]` slot positions
  (`struct Pos16 {s16 x, y;}`); `gOpponentSelectPageBgs[5]` page backgrounds
  (`struct Bitmap {const u16 *pal; const u8 *bitmap;}`); `gOpponentSelectNames`,
  `gWinLoseDrawLabels` (`"Win"/"Lose"/"Draw"`), `gUnknownOpponentName` (`"[ Unknown ]"`);
  OBJ palettes/tiles `gOpponentSelectObjPal`/`gOpponentSelectObjTiles` and `gOpponentSelectTextPal`/
  `gOpponentSelectDigitTiles`/`gOpponentSelectDimDigitTiles`.

## Function notes and matching tricks

- `Calendar_HandleInput` (matching): Calendar input step. If `blink == 3` (steady):
  `R` advances the date one month (`date += daysInMonth + 1` after
  `date -= d.day`), `L` steps back one month when `date > 30`; both clear
  `blink` and redraw. D-pad moves the cursor: RIGHT `weekday = (weekday + 1) % 7`,
  LEFT `(weekday + 6) % 7`, UP moves the week back (or flips `secondHalf` off /
  `week = 2`), DOWN moves forward (or flips `secondHalf` on / `week = 0`), with
  `PlaySE(0/2/3)` sound effects; A/B return 1 after `PlaySE(2)`.
  Tricks: write the month advance as `date = date + GetDaysInMonth(...) + 1` (left
  fold, target adds `date` first then `1`); DOWN's condition is `week <= 1` (the
  `bhi` in the ROM branches to the second-half case).
- `Calendar_DrawMonth` (parked): Calendar draw step. `Calendar_DrawCursorAndHeader()` draws the static
  frame, then it unpacks `date`/`unk0`, draws the 4-digit year right-to-left
  (`% 10` / `/ 10` on a signed copy, tiles `0x6240+`), computes the first
  weekday and days-in-month, skips to the active half-month when `secondHalf`,
  then walks cell 0..20 drawing each day's number (`0x6200 + day`, palette 1 on
  Sunday / 2 on Saturday) and event icons (`& 0x100000` / `0x200000` /
  `0x3F400000` select tiles 0x172/0x174/0x170 respectively) plus the cursor tile 0x17C when the
  cell matches `unk0`'s year/month and `date`'s day.
  Historical (matched in wave 3, see below): parked because agbcc gives it a 24-byte frame (spills `rowY` too) instead of
  20, and assigns `day`/`col`/`cell`/`events` to different callee-saved
  registers; semantics are otherwise identical. Permuter candidate.
- `Calendar_Init` (parked): Calendar setup step. DMA-fills `gCalendar`
  (0x81000006) from a stack zero, copies `gSaveData.days` into `unk0`/`date`,
  resets the video registers (MOSAIC/BLD/BGxCNT/DISPCNT), sets
  `gMain.vblankFlags = 1`, loads the page palette/tiles (087F3D18, 0822C300,
  087EA718 ×2, 087F3F18, 087F4118, 087F7DF8, 087F7E18, 087F5DD8, 087F4D18,
  087F4DD8), clears `blink`, calls `Calendar_SetCursorDate(date)` and `Calendar_UpdateEventNames()`.
  Parked: the ROM loads `0x040000D4` into `r1`, recomputes `mov r0,sp`, loads
  `&gCalendar` into `r2` then copies `r2->r6`, and reads `gSaveData.days` twice;
  our build CSEs the `sp` address and the two `days` loads and loads `&gCalendar`
  straight into `r6`, so it is 4 bytes short. Keeping a local
  `struct Calendar *cal` live from `dma[1] = (u32)(cal = &gCalendar)` fixes the
  `dma = r1` choice but not the `r2->r6` copy or the double `days` load.
  Permuter candidate.
- Trick: this unit's `gCalendar` is the same object as bustup_runner's
  `struct Calendar`; the `u16` bitfield word at +0x08 compiles to `ldrb`/`strb`
  for the low fields and `ldrh`/`strh` for `weekday`/`week`, matching the ROM.

## Wave 3 matches (2026-10-01/02)

Working notes: `build/wf/Calendar_DrawMonth/NOTES.md`.

### `Calendar_DrawMonth` (0x1F4, start score 144; FAKEMATCH)

The parked draft declared `iconY`/`dayY` as `u16`, which truncated the constants `0x300000`/`0x280000` to 0; they are
`u32`. Four separate mechanisms then had to match:

1. **Pre-tests and loop shape.** Explicit `if (day > daysInMonth) return; if (cell > 20) return;`, then the inits in
   ROM order (iconY, dayY, rowY, iconX), then `while (day <= daysInMonth && cell <= 20)`. CSE deletes the loop-entry
   test that jump.c duplicates, because the explicit tests already decided it. A bottom-tested `do {} while (a && b)`
   fails (`expand_end_loop` stops rolling the exit test after 30 insns, so the `cell` test lands above the loop top),
   and a goto loop has no loop notes, which reorders the argument registers.
2. **Event-icon y terms** `(rowY + 0x30) << 16` for icons 2 and 3: with rowY a plain pseudo, loop.c sees two identical
   general induction variables, combines them and strength-reduces them into a register. Pinning rowY (below) makes it
   no longer a basic induction variable, so no `y` temporary is needed.
3. **Constant `0x180000`** must be rematerialised by reload at each use (`mov r2,#192; lsl r2,#13`). A literal becomes
   one CSE-shared pseudo in r0, and `asm volatile("")` between the adds does not stop CSE. A local `u32 step = 0x180000;`
   set before the loop lives across it, gets no hard register, and its REG_EQUIV constant is reloaded per use.
4. **day vs rowY** (ROM: day in sl, rowY in r9). Both pseudos start with constant sets, so `update_equiv_regs` doubles
   both live lengths, leaving day's priority (4*18/318 = .226) above rowY's (3*11/222 = .149). FAKEMATCH:
   `register s32 rowY asm("r9");`. Also matched: `asm("" : "=r"(rowY) : "0"(0));` as rowY's init (its first set is
   then not a constant), or three `asm("" : : "r"(rowY))` uses in the loop. Pinning both day and rowY breaks the CSE
   deletion of the duplicated test (score 106).

Failed: making iconY/dayY loop-reduced givs of rowY (their inits land after `rowY = 0`), `(u16)(rowY + 0x30) << 16`, an
early `day = 1` (priority .189, not enough), a goto second-half skip loop (184). No natural source without the pin
was found (rowY would need a non-constant first set or 16+ references).

Related: [[bustup-runner-c]], [[main-menu-c]], [[title-screen-c]], [[ram-map]].
