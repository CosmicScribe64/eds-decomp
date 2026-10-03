---
title: CB_MainMenu (proposed) and its steps
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# CB_MainMenu `CB_MainMenu`

| Function | Address | Size | Proposed name |
|---|---|---|---|
| runner | `0x08003AA4` | 0x50 | `CB_MainMenu` |
| step 0 | `0x08003920` | 0x98 | `MainMenu_Init` |
| step 1 | `0x080039C0` | 0x20 | `MainMenu_FadeIn` |
| step 2 | `0x080039E0` | 0x76 | `MainMenu_HandleInput` |
| step 3 | `0x08003A58` | 0x36 | `MainMenu_Launch` |
| helper | `0x08003850` | 0xBA | `MainMenu_DrawItems` |

Mode: Thumb. Unit: `asm/main_menu.s`. Match: nonmatching (not attempted).

## Purpose (verified)
This is the hub scene, listed as "Menu" in the debug table. `MainLoop` falls back to it whenever a scene's callback returns nonzero. It's a step runner (table `0x081984F4`, index `gMain+0x4859`, sub-state `+0x485A`; see [[set-main-callback]]).

- **Init** (sub-state 0): `DISPCNT = 0`, `gMainMenuCursor %= 7`. Sub-state 1: `SetBrightnessBlack`, `ResetVideo`, `ResetBgScroll`, `BG1CNT = 0x84`, `vblankFlags = 3`, `PlayBGMNoTrack(3)`. Sub-state 2: The OBJ palette at `0x087DE858` goes to `0x05000200` and the OBJ tiles at `0x087DE878` (0x4000 bytes) go to `0x06010000`, then `LoadBgImage(0, 0, 0x20, 0x087D4B24)`.
- **FadeIn:** `DISPCNT = 0x1200` (BG1 + OBJ, 2D OBJ mapping), draws the items, `FadeFromBlack(1)`.
- **HandleInput:** Up sets `cursor = (cursor+6) % 7` and Down sets `cursor = (cursor+8) % 7`; each plays `PlaySE(0)`. A plays `PlaySE(1)`, calls `FadeOutBGM()`, and the step is done.
- **Launch:** draws, `FadeToBlack(2)`. When the fade finishes, it does `gMain+0x488A &= 0xF00F` and then `SetMainCallback(gMainMenuTable[cursor])`.
- **DrawItems:** a "MENU" header (4 sprites at y=0x0A, tiles 0/4/8/12), then 7 rows of 4 sprites, tile base `(k+1)*0x40`, +0x10 for the highlighted row, 16 px apart.

The cursor is `gMainMenuCursor` (u16 at `0x02015ED8`). It isn't reset between visits.

## Launch table `gMainMenuTable` at `0x081984D8` (verified)
The labels were read from the menu's own sprite sheet (`0x087DE878`, rendered):

| Cursor | Label | Callback | Notes |
|---|---|---|---|
| 0 | Campaign | `0x0801D1E8` | runner, index `+0x4857`, table `0x08198EAC` |
| 1 | Link Battle | `0x0801AD18` | runner, index `+0x4858`, table `0x08198E7C` |
| 2 | Deck Edit | `0x0806EF04` | table `0x081A725C` |
| 3 | Record | `0x08003E94` | table `0x08198588` |
| 4 | Calendar | `0x08002940` | 4 steps |
| 5 | Card Trading | `0x0807D348` | table `0x081A79A4` |
| 6 | Password | `0x0807CC28` | table `0x081A7970` |

The table is preceded by a pointer table (`0x08198480`–`0x081984D4`) to the opponent-name strings ("Duel Computer", "Trusdale", "Maximillion Pegasus", "Simon", "Shadi", "Yami Yugi", …) and to "Draw"/"Lose"/"Win". That table belongs to other code (it's referenced from `0x08003298` and `0x080036FC`).

## Matching notes
- The `% 7` goes through `0x0807F124` (libgcc modulo). The source keeps the cursor as `u16` and writes `(cursor + 6) % 7`.

Related: [[program-flow]], [[set-main-callback]], [[fade-functions]], [[sound-api]].
