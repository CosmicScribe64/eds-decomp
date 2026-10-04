---
title: bustup_runner (Bustup dialogue steps / Calendar helpers)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# bustup_runner: Bustup dialogue steps and Calendar helpers

Unit `bustup_runner` (`0x08001364`–`0x08002221`, Thumb, `old_agbcc -O2`). It holds the
step functions of the dialogue ("Bustup") scene runner, the dialogue/scene loaders, the
8bpp text canvas writer and the calendar screen's date/event drawing. It follows
[[bustup-scene-c]] (the rest of the same dialogue module) and shares its structs.

**Match status: 22/22 functions in C** after workflow wave 3 (2026-10-02: `0x08001374`,
`0x08001464`, `0x080017E8` in wave 3); none stay `INCLUDE_ASM`. The unit matches byte for byte
(`check.py`: `unit bytes MATCH`). Before wave 3: 19/22 (three assembly fallbacks with parked C drafts).

> [!warning] Contradiction
> The table (2026-10-01 and earlier) described `0x08001374` as `DrawCardStats`, drawing "the selected card's three
> stat counters (attack/defence/level)" from a per-card table. The wave 3 match (2026-10-02,
> `build/wf/Bustup_DrawOpponentRecord/NOTES.md`) reads `records[gBustupOpponentIds[unk12E9 + unk12EA * 5]]` with fields
> `wins:11, losses:11, draws:10`, the same opponent duel record as `struct DuelRecord` in [[link-battle-c]].
> Resolved in favour of the matched source: it draws a duel record (the `src/` comment above the function still says
> "stat counters").

> [!warning] Contradiction
> The table (2026-10-01 and earlier) said `0x08001464` "programs `REG_BGxHOFS`". The wave 3 match (2026-10-02,
> `build/wf/Bustup_UnusedOpponentPreview/NOTES.md` and `src/bustup_runner.c`) writes `0x04000028`-`0x0400002E`, the BG2 affine
> reference point registers BG2X/BG2Y. Resolved in favour of the matched source.

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x08001364` | 0x10 | matching | `Bustup_Step0` | calls `Bustup_InitState(0)`, returns 1 |
| `0x08001374` | 0xF0 | **matching** (wave 3, 2026-10-01) | `DrawDuelRecord` (hypothesis; was `DrawCardStats`) | draws the win/loss/draw counters (`Bustup_DrawNumber`, 3 digits) of the duel record `gSaveData+0x20D0` `records[gBustupOpponentIds[unk12E9 + unk12EA * 5]]`, and an icon (`Bustup_DrawLabel`) when `(u16)arg0 != 0` |
| `0x08001464` | 0x244 | **matching** (wave 3, 2026-10-02) | `Bustup_UpdateScroll` | re-inits both anim-object arrays and the sparkle trail, programs the BG2X/BG2Y reference point (`0x04000028`-`0x0400002E`) from the dialogue state, finalises the text box |
| `0x080016A8` | 0xC8 | matching | `Bustup_LoadScene` | loads the current dialogue as a 240x96 or 240x80 scene (large box for speakers 0x20–0x23), resets the text box, enables forced blank + Mode 4 |
| `0x08001770` | 0x78 | matching | `Bustup_LoadStrip` | loads the selected dialogue as a 240x80 scene with `Bustup_ChangeSceneSet`, then rewinds `gMain.seqIndex1` by 4 |
| `0x080017E8` | 0x2FC | **matching** (wave 3, 2026-10-02; FAKEMATCH) | `Bustup_Advance` | A-button dialogue advance, box settle/page states, dirty-flag redraw, object-array rebuild; returns 1 when done |
| `0x08001AE4` | 0x50 | matching | `Bustup_Run` | runs the step table `0x0813ADD4[gMain.seqIndex1]` |
| `0x08001B34` | 0x64 | matching | `Bustup_RunAuto` | as above but forces auto-advance |
| `0x08001B98` | 0x30 | matching | `Dialogue_FindEvent` | event id → dialogue table index (490 if absent) |
| `0x08001BC8` | 0x24 | matching | `Dialogue_EventOf` | index → event id |
| `0x08001BEC` | 0x24 | matching | `Dialogue_SpeakerOf` | index → speaker/portrait id |
| `0x08001C10` | 0x68 | matching | `Dialogue_Start` | event id → sets `gMain.speaker`/`dialogueIndex`, resets `seqIndex1` |
| `0x08001C78` | 0x1AC | matching | `GetBustupSet` | character id → scene-set descriptor (jump table on `charId`) |
| `0x08001E24` | 0x28 | matching | `PlotPixel8bpp` | masked 16-bit VRAM pixel write (VRAM only takes 16-bit writes) |
| `0x08001E4C` | 0xBC | matching | `DrawGlyph` | one 8x10 glyph (ASCII 1bpp or Shift-JIS 16x16) into the text canvas |
| `0x08001F08` | 0xA8 | matching | `DrawString` | draws a string into the Mode-4 canvas at (x, y), with an outline |
| `0x08001FB0` | 0x58 | matching | `DrawCalendarEvents` | lists up to two calendar event names whose flag matches `mask` |
| `0x08002008` | 0x40 | matching | `RedrawCalendarEvents` | copies the seasonal event strip to the visible page |
| `0x08002048` | 0x4C | matching | `FlipCalendarPage` | flips the displayed Mode-4 page and DISPCNT bit 4 |
| `0x08002094` | 0xE8 | matching | `Calendar_MoveCursor` | repositions the day cursor and redraws the events if the cell changed |
| `0x0800217C` | 0xA4 | matching | `Calendar_SetCursor` | unpacks a date, sets weekday/week flags and clamps to the second half of the month |
| `0x08002220` | 0x168 | matching | `Calendar_Draw` | draws the calendar cursor, season, weekday headers and month tiles |

## Shared headers (migrated 2026-09-30)

This unit includes the shared header `include/main.h` and uses the canonical
`struct Main` / `extern struct Main gMain` instead of its own local `struct Main`.
The local view described `newKeys` +0x006, `intrCheck` +0x40C, `seqIndex1` +0x4859,
`speaker` +0x4884 bits 4-11 and `dialogueIndex` +0x4886, all of which map to the same
offsets and bitfield positions in the header. The migration removed one local struct
definition and its `extern`. No local views were kept, and every function still matches
byte for byte.

The unit does not use `duel.h` or `duel_ui.h`: it touches no `DuelCard`/`DuelZone`/
`DuelPlayer`/`DuelState`/`DuelCmd`/`DuelScreen` data, so those headers are intentionally
not included.

## Globals and structs (extended from [[bustup-scene-c]])

**`gBustup`** is the large script and text-box state (0x02013DE0). This unit adds
these fields:

- `+0x9A4`: `objCount:3`
- `+0x9AC`: **TextBox** (0x0201478C, as in [[bustup-scene-c]])
- `+0xAA8`: a line/sparkle block passed to `AnimBlockDraw`/`OamListClear` (0x02014888)
- `+0x10C0`: a second 0x14-stride animation-object array with `unkE` at +0x0E (0x02014EA0)
- `+0x12E9`/`+0x12EA`: sparkle start indices
- `+0x12EB`: `speaker`
- `+0x12EE`: u16 scroll value
- `+0x136C`: u16 dialogue index
- `+0x136F`: purpose not recorded
- `+0x137C`: auto-advance flags (bit 0 finished, bit 1 auto)

**`gBustupFade`** is `gBustup + 0x12EC` (0x020150CC), the state the Bustup step
functions use. Its fields are `+0x04` s16 scroll, `+0x06` box state (2 = settle), `+0x07`
step delta, `+0x80` u16 dialogue index and `+0x90` flags (bit 0 started, bit 1 advancing).
A byte at `-1` mirrors the current speaker. `Bustup_Update` reaches the bytes at `-0x927`
(TextBox `state`) and `-0x920` (TextBox `page`) through this base.

> [!warning] Contradiction
> This bullet (2026-10-01 and earlier) calls `+0x20D0` per-card state and says the target reads u16 / u32 / u16. The
> wave 3 match of `Bustup_DrawOpponentRecord` (2026-10-02, `build/wf/Bustup_DrawOpponentRecord/NOTES.md`) indexes it by opponent and reads it
> through a plain `{u32 wins:11, losses:11, draws:10}` record array, as in [[link-battle-c]]. Resolved in favour of the
> matched source: it is the per-opponent duel record; the load widths below are what agbcc emits for those bitfields.

- **`gSaveData` (0x02011C20)** holds the packed per-card state at `+0x20D0`. Each entry
  is a 4-byte record indexed by the byte table `gBustupOpponentIds`, with three counters in
  bits 0–10, 11–21 and 22–31 (the target reads u16 / u32 / u16 respectively).
- `gBustupBannerTiles` (u16[]), `gBustupRecordPos` (six u16 positions), `gBustupOpponentIds` (u8[]),
  `gStrDebugDM5Script`/`gStrDebugMoveToScript` (dialogue name formats), `gDialogueTableText`
  (= `gDialogueTable + 4`, the `text[]` of each 0x304-byte `DialogueEntry`).
- `gBustupCursor` (TextBox + 0x28) is the sparkle sub-state: `+0x91A` u16, `+0x91E` u8,
  `+0x99B` s8.
- `gBustupSprites` is the text-box module base. `+0x842`/`+0x84A`/`+0x851` mirror
  `gBustup+0x12EA`/`+0x12F2`/`+0x12F9`, `+0x844` is `gBustupFade`, and
  `-0xFC` is the TextBox, `-0xDA` its `boxDirty`.

## Matching notes

- `Bustup_ChangeSpeaker` matches with an `u8 box = 1;` local hoisted to a callee-saved register
  before the first `bl`; passing the literal `1` puts it in a scratch register and the
  prologue loses `r5`.
- `Calendar_SetCursorDate`: write the week numerator as `d.day - 1 + first` instead of
  `d.day + (first - 1)`. This changes the compiler's subtraction scheduling and
  produces the exact ROM code without a compiler barrier.
- `Bustup_LoadScene`: a switch containing the four adjacent speaker cases `0x20`–`0x23`
  reproduces the ROM's signed upper/lower bound comparisons. Separate `if` tests
  compiled the lower bound as `cmp #0x1F; ble`; the switch gives `cmp #0x20; blt`.
  This is ordinary C, with no compiler barrier. The whole unit was checked again
  after enabling it (`0x1024` bytes, exact match).
- Historical `Bustup_DrawOpponentRecord` (matched in wave 3 with plain bitfields; see the contradiction above and
  the wave 3 write-up below): `gSaveData`'s record must be read as u16 / u32 / u16 with
  11/11/10-bit shifts to match the load widths; the remaining diff is register
  allocation of the two hoisted bases (`r7 = 0x20D0` in the target).
- Historical `Bustup_UnusedOpponentPreview` (matched in wave 3): the draft includes the ROM's scroll-value reload after
  the calls and signed object-sentinel comparison. Base allocation, literal
  materialization and layout still differ (`0x230` candidate versus `0x244` ROM).
- Historical `Bustup_Update` (matched in wave 3): the goto-join layout and the `switch` on the TextBox `state`
  come out quite differently from the target, whose object loops derive their count from the
  `0x02014EA0`/`0x02013DE0` bases.
- `DrawGlyph8bpp`: an empty read/write constraint on the initialized destination
  pointer preserves its entry copy. An r0-bound font index plus empty input
  constraints keep index addition/scaling before the font-base loads. The wide
  font uses an explicit byte offset before reading its u16 pixels. These hints
  emit no instructions and retain the original three-argument ABI (`FAKEMATCH`).
  The final full unit check confirms all `0x1024` bytes match with 19 C bodies.

## Wave 3 matches (2026-10-01/02)

Working notes: `build/wf/Bustup_DrawOpponentRecord/NOTES.md`, `build/wf/Bustup_UnusedOpponentPreview/NOTES.md`, `build/wf/Bustup_Update/NOTES.md`.

### `Bustup_DrawOpponentRecord` (0xF0, start score 145; ordinary C)

- The old draft built the address by hand (`(u8 *)&gSaveData + 0x20D0 + idx*4` with u16/u32/u16 casts and
  shifts). It folded `save+0x20D0` into one constant and allocated the hoisted bases differently (score 137-145).
- What matched, on the first try: the real record struct from [[link-battle-c]] (`u32 wins:11, losses:11, draws:10`,
  `records[32]` at `+0x20D0`) through a local view `gSave08001374`, read as `records[idx].wins/.losses/.draws`. The
  array reference expands as `((idx*4 + save) + 0x20D0)`, so `0x20D0` stays in its own register (r7) and `draws` is
  read as a halfword at `+0x20D2`, as in the ROM. No FAKEMATCH (the unit's own `struct SaveData` has only two fields,
  hence the local view).

### `Bustup_UnusedOpponentPreview` (0x244, start score 225; ordinary C)

- The old draft indexed a `u8 objs[]` byte array with casts (object addresses in the wrong form and register order),
  used s8/u16 temporaries for the scroll value and an if-chain for the `+0x99B` state. It was rewritten from the asm.
- A local overlay `struct Scr464` with a real `struct Obj464 objs[27]` (0x14 bytes, `s8 unkE` at +0xE): `&objs[i]` and
  `objs[i].unkE` give the ROM's two address forms (`sym+0x10C0` hoisted, `+0xE` as a CSE related value). agbcc pads the
  struct to 4-byte alignment, so the filler after the array starts at `0x12DC` (not `0x12E4`).
- `switch (unk99B) { default: BG2Y ...; break; case 1: ...; case 2: ... }`: putting `default` first places it on the
  fall-through path after `cmp 1; beq; cmp 2; beq`.
- Scroll writes as `L = v * 2; H = ((v * 2) >> 16) & 0xFFF` (and the `-v * 2` form) with `u16 v`; a local
  `vu16 intrCheck` overlay (`gMain464`) reproduces the double `ldrh` of `&= 0xFFFE`.

### `Bustup_Update` (0x2FC, start score 228; FAKEMATCH)

- The old draft's structure was wrong (goto joins into blocks); it also cleared bit 1 instead of bit 0 and passed an
  uninitialised pointer to `Bustup_ClearHiddenBox`. The ROM shape is `if (!started) { A-switch on the TextBox state;
  if (B) { PlaySE(2); return 1; } } else if (autoAdvance) { ...; if (EventOf() == 0) { ...; return 1; } }`;
  cross-jumping merges the two return-1 tails.
- The `+0x90` flags are `u8 started:1; u8 autoAdvance:1` bitfields; clearing a bitfield gives the ROM's `mov #2; neg`.
- FAKEMATCH 1: `GetDialogueEventId` is called through a `u32 (*)(u32)` cast (`EventOf17E8`), because the ROM uses its result
  without re-extending it while the unit's prototype returns u16.
- FAKEMATCH 2: in the A / state -2 auto-advance branch, `gAnim17E8.dialogueIndex` is copied to gMain through a `u32 idx`
  temporary. That forces the ROM's reload after the speaker store; without it CSE2 reuses the incremented value. With
  this reload fixed, the constant-register choices in the auto-advance block (`0x4884` -> `0x4886` by move2add) matched
  too.
- `intrCheck` goes through the unit's `vu16` `gMain464` view for the double `ldrh`.
