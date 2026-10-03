---
title: main_menu unit (main-menu + record/opponent-select drawing & input)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit `main_menu` (`0x080034B8`-`0x080044E4`)

The unit as a whole matches (`unit bytes MATCH`): **27/27 functions in C** after workflow wave 3 (2026-10-02: `0x080034B8`, `0x08003C78`, `0x08003F88`, `0x080040E4` in wave 3); none stay `INCLUDE_ASM`. Before wave 3: 23/27. Compiler: `old_agbcc -mipsel -O2`, which is `old_agbcc -O2` ([[compiler-flags]]).

It contains three tightly related screens that share state with [[link-battle-c]] / [[campaign-select-c]]:
- the **main menu** (`0x08003850`-`0x08003A58`, callback `0x08003AA4`),
- the **record / duelist-stats screen** (`0x08003AF4`-`0x080040E4`), callback `0x08003E94`,
- the campaign **opponent-select cursor drawing** (`0x080034B8`, `0x080036FC`, `0x0800366C`).

The date helpers (`0x08004280`-`0x08004494`, leap-year / day-of-week / Japanese holidays) are also here.

## Functions

> [!warning] Contradiction
> The table (2026-10-01 and earlier) describes the record screen in terms of "cards" (`0x08003F88` draws arrows "next to each of the 4-5 shown cards"; `0x08003C78` "when a card is selected animate/scroll it"). The wave 3 matches (2026-10-01, `build/wf/Record_DrawResultMarkers/NOTES.md`, `build/wf/Record_HandleInput/NOTES.md`) show the rows are opponents: the arrow is chosen by the sign of wins minus losses of `rec[unk1_5 * 5 + i + 1]`, and `0x08003C78` runs a BG1/BG2 page scroll. Resolved in favour of the matched source; the purposes below are updated.

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x080034B8` | 0x1B4 | **matching** (wave 3, 2026-10-01) | opponent-select cursor: shift the 8-slot trail, ease it toward the slot over 15 frames (else bob on idle using `gMain.frameCounter`), draw the trail sprites and the portrait frame | `DrawOpponentCursor` (hyp.) |
| `0x0800366C` | 0x90 | matching | draw a clamped 0..99 value as up to two decimal digit sprites at (x,y) | `DrawSmallNumber` (hyp.) |
| `0x080036FC` | 0x154 | matching | record screen: centre and draw the "Win/Lose/Draw" labels and the three counters for a duelist `1..0x18` | `Record_DrawDuelistStats` (hyp.) |
| `0x08003850` | 0xD0 | matching | main menu: "MENU" header plus 7 rows of 4 sprites (row under the cursor uses tiles +0x10) | `MainMenu_DrawItems` |
| `0x08003920` | 0xA0 | matching | main-menu init sub-state machine (clear DISPCNT, reset video/BGM, load graphics) | `MainMenu_Init` |
| `0x080039C0` | 0x20 | matching | main-menu step 1: enable BG1+OBJ, draw items, fade in | `MainMenu_Show` (hyp.) |
| `0x080039E0` | 0x78 | matching | main-menu input: Up/Down wrap the cursor over 7 items, A confirms | `MainMenu_HandleInput` |
| `0x08003A58` | 0x4C | matching | main-menu launch: fade out then switch scene via `gMainMenuTable[cursor]` | `MainMenu_Launch` |
| `0x08003AA4` | 0x50 | matching | main-menu step runner over `gMainMenuSteps` | `CB_MainMenu` |
| `0x08003AF4` | 0x70 | matching | record step 0: clear record state, reset video, set BG0-3 | `Record_Init` (hyp.) |
| `0x08003B64` | 0xF4 | matching | record step 1: load palettes/tiles/BG images, draw both panels | `Record_LoadGfx` (hyp.) |
| `0x08003C58` | 0x20 | matching | record: draw panels, enable BG, fade in | `Record_Show` (hyp.) |
| `0x08003C78` | 0x208 | **matching** (wave 3, 2026-10-01) | record input: while a page scroll runs (`unk0_1` = direction) count `unk0_3` down and copy BG1/BG2 hofs from the scroll table `gRecordScrollHofs`, then move `unk1_5` and flip `unk0_0`; when idle, R/RIGHT (`0x110`) or L/LEFT (`0x220`) start a scroll if `Record_HasNextPage`/`Record_HasPrevPage` allow it; returns 1 on A/B | `Record_HandleInput` (hyp.) |
| `0x08003E80` | 0x14 | matching | record: draw panels, fade out | `Record_FadeOut` (hyp.) |
| `0x08003E94` | 0x40 | matching | record step runner over `gRecordSteps` | `CB_Record` |
| `0x08003ED4` | 0x18 | matching | `gRecord.unk1_5 != 0` (a record tier done?) | `Record_IsTier1` (hyp.) |
| `0x08003EEC` | 0x48 | matching | dispatch to `IsCampaignLevel2Unlocked/C14/C7C/CE4` by `gRecord.unk1_5` | `Record_CheckTier` (hyp.) |
| `0x08003F34` | 0x54 | matching | draw the left/right selection arrows for the current cards | `Record_DrawArrows` (hyp.) |
| `0x08003F88` | 0xE4 | **matching** (wave 3, 2026-10-01) | draw an up/down arrow next to each of the 4-5 shown opponents, chosen by the sign of wins minus losses | `Record_DrawUpDown` (hyp.) |
| `0x0800406C` | 0x10 | matching | draw the record panels (`Record_DrawResultMarkers` + `Record_DrawPageArrows`) | `Record_Draw` (hyp.) |
| `0x0800407C` | 0x68 | matching | write a 3-digit decimal into BG map buffer `bg` at (x,y); digit tiles start at 4 | `Record_Draw3Digits` (hyp.) |
| `0x080040E4` | 0x19C | **matching** (wave 3, 2026-10-01) | draw the opponent-record panels: 4/5 rows, each a portrait + W/L/D counters (or placeholder) | `Record_DrawPanels` (hyp.) |
| `0x08004280` | 0x34 | matching | Gregorian leap-year test | `IsLeapYear` |
| `0x080042B4` | 0x24 | matching | days in a month (1-based, Feb +1 in leap years) | `DaysInMonth` |
| `0x080042D8` | 0x80 | matching | day of week (0=Sun) counting days since 2000-01-01 | `DayOfWeek` |
| `0x08004358` | 0x13C | matching | holiday bit mask for a date | `HolidayMask` |
| `0x08004494` | 0x50 | matching | true if a date is a "red" day (Sunday, holiday, or the Monday after) | `IsRedDay` |

## Structures and globals (inferred)

The unit uses the shared `include/main.h` (`struct Main`) for `gMain` (`0x03000040`) instead of its own local definition, which was removed. Fields used: `+0x0006 newKeys`, `+0x040E vblankFlags`, `+0x4859 seqIndex1`, `+0x485A seqState1`, `+0x485B seqState2`, `+0x485E frameCounter`, `+0x488A` bits 4-11 (`step488A`), and (used by `Record_HandleInput`, matched in wave 3) `+0x442A`/`+0x442C` = `bgHofs[1]`/`bgHofs[2]`. No local view had to be kept, because every used field has the same declared type and split in the canonical header, so all 27 functions still report `match`. The unit needs neither `duel.h` nor `duel_ui.h`.

`gRecord` (`0x0201F814`, see [[ram-map]]): the first u16 is `unk0_0:1 | unk0_1:2 | unk0_3:10`, then `unk1_5:3` (bits 13-15), `unk2`:
- `unk0_1` = 1/2 selects the pending cursor move direction (right/left); 0 = idle.
- `unk0_3` = a 10-bit animation/scroll counter (set to `0x10` on a new selection, decremented while animating).
- `unk1_5` = index of the selected card group (used as `5*unk1_5 + row` into the save counters). The wave 3 matches show it is the opponent page: `rec[unk1_5 * 5 + i + 1]` are the shown opponents' duel records.

`gSel`/`gOpponentSelect` (`0x0201F7E0`, shared with [[campaign-select-c]]): 4 bytes of bitfields (`page:3 | cursor:3 | unk6:3 | unk9:4 | unk13:4 | unk17:15`), then `u16 cursorX[8]` (+4), `u16 cursorY[8]` (+0x14), `s32 unk24` (+0x24), `s32 unk28` (+0x28), `s32 targetX` (+0x2C), `s32 targetY` (+0x30).

> [!warning] Contradiction
> The paragraph below (2026-10-01 and earlier, copied from [[collection-c]]) says the `0x20D0` record "has to be viewed through three wrapper structs". The wave 3 matches of `Record_DrawResultMarkers` and `Record_DrawPage` (2026-10-01, `build/wf/Record_DrawResultMarkers/NOTES.md`, `build/wf/Record_DrawPage/NOTES.md`) use one padded view `struct WF3F88Save { u8 pad0[0x20D0]; struct { u32 wins:11; u32 losses:11; u32 draws:10; } rec[32]; }`: agbcc picks `ldrh` for `wins` and `ldr` for `losses` by itself. Resolved in favour of the matched source: the single view is enough for array accesses `rec[idx].field`; `OpponentSelect_DrawDuelistInfo` still uses the three views (it matched that way and was not reworked). The fields are wins (a), losses (b) and draws (c), per opponent, as in [[link-battle-c]].

Save mirror `gSaveData` (`0x02011C20`): `+0x20D0 + idx*4` is a 4-byte record with three fields: the low 11 bits (a), bits 11-21 (b) and bits 22-31 (c). Because the three fields are read with different load widths, the same address has to be viewed through three wrapper structs (`struct Card2A` = `u16 a:11; u16 rest:5; u16 hi;` for `ldrh`; `struct Card2B` = `u32 lo:11; u32 b:11; u32 c:10;` for `ldr`; `struct Card2C` = `u16 lo; u16 pad:6; u16 c:10;` for `ldrh` at +2), each prefixed with `u8 pad0[0x20D0]`. This is copied from [[collection-c]].

> [!warning] Contradiction
> The list below (2026-10-01 and earlier) calls `gRecordScrollHofs` "u16 record tile-map data". The wave 3 match of `Record_HandleInput` (2026-10-01, `build/wf/Record_HandleInput/NOTES.md`) declares it `u16 gRecordScrollHofs[2][2][16]` (0x80 bytes, up to `gRecordSteps`) and copies `[unk0_0][unk0_1 - 1][unk0_3]` into `gMain.bgHofs[1]`/`[2]`. Resolved in favour of the matched source: it is the page-scroll HOFS table.

Other globals:
- `gOpponentSelectSlotPos[5]`: `struct Pos16 {s16 x,y;}` slot positions
- `gOpponentCursorWobble[]`: `{u16 x,y;}` idle-bob offsets
- `gWinLoseDrawLabels[3]`: `{"Win","Lose","Draw"}` strings
- `gMainMenuTable`/`gMainMenuSteps`/`gRecordSteps`: step tables
- `gRecordScrollHofs[2][2][16]`: u16 page-scroll HOFS values (indexed by page side, direction, counter)
- `gRecordPortraitImages[]`/`gRecordPageNameImages[]`: image-pack pointers
- `gRecordMarkerAnimTiles[]`: u16 arrow tiles
- `gDaysPerMonth[]`: days-per-month
- `gRecordRows5Image`/`...5CF4`/`...77B4`: panel images
- `gMainMenuCursor` (`0x02015ED8`)

## Matching tricks learned

- `OpponentSelect_DrawDuelistInfo`: keep the initial accumulated label width in a `total` local and
  use a separate `x` for the later centered position. This fixes the sum/table
  pointer register swap. In the first counter block, assign the save base to a
  local before deriving the per-duelist record pointer; that puts its literal
  load before the index shift. No compiler barrier is needed. The complete
  `0x102C` unit remains exact, with **23/27 matching C functions**.
- `OpponentSelect_DrawNumber`: write the pixel pair as `(x + 8) | (y << 16)` (x-term first) so agbcc schedules the `lsl r6,r1,#16` after `add r4,#8`, exactly like the ROM. Keep the clamp `if (v>99) v=99; if (v<0) v=0;` and the `((v%10)+0x280)|0x3000` constant.
- Historical (superseded by the wave 3 matches; see the contradiction under Structures): for the `0x20D0` save records use the three wrapper views above; a plain 32-bit bitfield struct makes agbcc emit `ldr` where the ROM has `ldrh` and breaks the field ordering.
- Use a local `struct *s = &gUnk_XXXXXXXX;` (assigned after any preceding call) when the ROM keeps the base in a register (`ldr r,=base; ldr r2,=off; add`) instead of folding the address into a pooled literal ([[collection-c]]).

## Open items (parked drafts for the permuter)

Historical (all four matched in wave 3, 2026-10-01; see [Wave 3 matches](#wave-3-matches-2026-10-0102)). Each was in an `#if 0 /* NONMATCHING: ... */` block directly above its `INCLUDE_ASM` line:

- `0x08003F88`: logic and nearly all instructions match; agbcc hoists `&gRecordMarkerAnimTiles[frame]` out of the loop and gives the base pointer `r8`, while the ROM keeps the table in `r9`, `0x20D0` in `sl`, `n` in `r8`, the base spilled in `r3`, and recomputes the record index for the `ldr`.
- `0x080040E4`: the frame is 0x28 bytes where the ROM's is 0x1C; agbcc keeps `a` in `sl` and rematerialises loop variables differently (ROM: `b r5`, tileBase `r7`, `y r8`, row `r9`, `col sp18`).
- `0x080034B8`: the instruction sequence nearly matches (size 0x1A8 against the ROM's 0x1B4); register map differs (ROM: `slot r3`, `&gSel r6`, `&pos r5`, `flag r9`, `slot*4 sl`).
- `0x08003C78`: the saved draft extracts direction through an initialized input hint and uses separate 0x100/0 scroll branches. It has the ROM's `0x208` size, but byte differences remain in offset scheduling, allocation and layout; it is still disabled.

## Wave 3 matches (2026-10-01/02)

All four match in ordinary C (no FAKEMATCH comments in the final source). Working notes: `build/wf/OpponentSelect_DrawCursor/NOTES.md`, `build/wf/Record_HandleInput/NOTES.md`, `build/wf/Record_DrawResultMarkers/NOTES.md`, `build/wf/Record_DrawPage/NOTES.md`.

### `OpponentSelect_DrawCursor` (0x1B4, start score 195; ordinary C)

- Rewritten from the asm with direct `gSel.field` accesses everywhere (the old draft had a local `sel` pointer, `s8 i`, `s16 dx/dy`). GCSE produced the ROM's `adds r4,r6,#0` copy and the fresh literal before the draw loop by itself.
- Interpolation as separate statements: `s32 dx = unk24 - targetX; s32 dy = unk28 - targetY; dx *= unk9; dy *= unk9; dx /= 16; dy /= 16;`. One statement `dx * unk9 / 16` interleaves the mul/div; `(a - b) * unk9` re-derives `unk9` twice.
- Draw loop: the trail call takes `gSel.cursorX[i] | (gSel.cursorY[i] << 16)` (X term first). The Y-first form makes loop.c strength-reduce `cursorX[i]` instead of `cursorY[i]`.
- Scores: 195 -> 164 (rewrite) -> 116 (temporaries, X-first operands) -> 0 (mul, mul, div, div order). Shift loop: `s32 i; for (i = 7; i != 0; i--)`.

### `Record_HandleInput` (0x208, start score 129; ordinary C)

1. Scroll table: `gRecordScrollHofs` was retyped from `u8 []` to `u16 [2][2][16]` (only this function uses it). Indexing a real 3-D array object, `gRecordScrollHofs[unk0_0][unk0_1 - 1][unk0_3]`, gives the ROM's `k*2 + (j-1)*32 + i*64 + base` with the `sub #1` kept. Pointer arithmetic, a cast to `u16 (*)[2][16]`, a dereferenced cast or flat u16 indexing fold the -1 into the base literal (EXPAND_SUM distributive law) or reorder the sum. An asm-label alias also matched, but `wf.py` rejects non-register asm strings.
2. `switch (unk0_1) { case 1: unk1_5++; break; case 2: unk1_5--; break; }` gives `cmp #1; beq`, `cmp #2; beq` with a cross-jumped `orr; strb` tail (the old if/else-if plus asm hint did not).
3. `Record_HasNextPage`/`Record_HasPrevPage` are defined later in the unit and were called undeclared (implicit int); calling them through `int (*)(void)` drops the `lsl r0,#16` before the test.
4. End as `if (newKeys & 3) { PlaySE(2); return 1; } return 0;`. The `if (!...) return 0;` form lets jump.c merge the return-0 paths, flipping the branch and the cross-jump direction of the two start-scroll blocks.
- Failed: a u16 `ret` variable (score 30), shift-sum byte offsets (134), flat u16 index (153).

### `Record_DrawResultMarkers` (0xE4, start score 151; ordinary C)

- Rewritten from scratch; the parked draft's two `Card2Rec` views, manual `*(u32 *)&gRecord` shifts, `s16 i`/`s8 n` and running `y` variable were a dead end.
- One plain record struct `{ u32 wins:11; u32 losses:11; u32 draws:10; }` behind a `u8 pad0[0x20D0]` save view: agbcc picks `ldrh` for `wins` and `ldr` for `losses` on its own. `rec[gRecord.unk1_5 * 5 + i + 1]` written twice gives the ROM's repeated index computation and its `ldr; lsl 16; lsr 29` read of `gRecord`.
- `n = (gRecord.unk1_5 <= 3) ? 5 : 4;` as a ternary (an if/else put `movs r7,#4` before the `ldrb`).
- y written inline as `((i * 24 + 0x2B) << 16)`: loop.c strength-reduces it to a giv whose init (`0x2B0000`) sits in the preheader after the loop test, as in the ROM; a running `y += 0x180000` put the init before the test.
- Arrow choice: `d = (u32)diff >> 31; if (diff > 0) d = -1;` with `s32 d`, then `0x84 + d * 0x30`. The applied C still declares an unused `y` (harmless).

### `Record_DrawPage` (0x19C, start score 188; ordinary C)

1. The old draft hand-strength-reduced the loop counters (frame 0x28 vs the ROM's 0x1C). Plain `i`-based C lets loop.c do it: `(u16)(i * 3 + 4) * 32 + 4`, `tileBase + 0xE0 + i * 12`, `i * 3 + 5`, and `i * 0x60 + 0x63` (the `<< 16` giv, because `FillMapRect` takes u16). Score 78.
2. `IsOpponentUnlocked` is called through an `s32 (*)(s32)` cast, because the ROM passes `k + 1` untruncated.
3. Key trick: the ternary 5th argument `(a <= 3) ? (const void *)gRecordRows5Image : (const void *)gRecordRows4Image`, both arms cast to the parameter type. Without the casts, the NOP_EXPR of the `const u8 *` to `const void *` conversion makes `expand_expr` pass the reduced `target` (NULL for a MEM) instead of `original_target`, so the COND_EXPR is evaluated into a pseudo (`x = B; if (c) x = A`). With them each arm stores straight into the stack slot (`ldr A; b; ldr B; join: str [sp]`), and CSE stops unifying the early `b + 1` across the join. Score 16.
4. The y coordinate as `(i + 5 + b * 5) * 16`. `(b*5+5+i)`, `(b*5+i+5)`, `(i+b*5+5)`, `(5+b*5+i)` and `(i+(b*5+5))` all scored 16 (y giv incremented third rather than last, init computed as `b*4 + (b+5)`); `((b+1)*5+i)` scored 106.
- The counters reuse `Record_DrawResultMarkers`'s view: `WF3F88Save.rec[k + 1].wins/.draws/.losses`, drawn in the ROM's order wins, draws, losses.

Related: [[campaign-select-c]], [[collection-c]], [[ram-map]], [[main-menu]], [[decomp-permuter]].
