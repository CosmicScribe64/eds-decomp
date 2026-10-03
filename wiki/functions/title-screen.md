---
title: CB_Title (proposed) and its steps
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# CB_Title `CB_Title`

| Function | Address | Size | Proposed name | Unit |
|---|---|---|---|---|
| runner | `0x080057BC` | 0x5C | `CB_Title` | `code_08005500` |
| step 0 | `0x0800527C` | 0x94 | `Title_Init` | `code_080044E4` |
| step 1 | `0x08005310` | 0x54 | `Title_Setup` | `code_080044E4` |
| step 2 | `0x08005368` | 0xF4 | `Title_Intro` (hypothesis) | `code_080044E4` |
| step 3 | `0x0800548C` | 0x74 | `Title_HandleInput` | `code_080044E4` |
| step 4 | `0x0800545C` | 0x26 | `Title_FadeOut` | `code_080044E4` |
| step 5 | `0x0800553C` | 0x1D4 | (none; 13-case state machine, jump table `0x08005564`) | `code_08005500` |
| step 6 | `0x08005718` | 0xA2 | `Title_StartGame` | `code_08005500` |
| helper | `0x08004FD8` | 0x232 | `Title_Draw` (hypothesis) | `code_080044E4` |

Mode: Thumb. Match: nonmatching. Runner table `0x081988B0`, index `gMain+0x4878`, sub-state `+0x4858`. The debug table calls it "TITLE".

## Purpose
Verified from the code:
- **Init:** clears `gTitleState` (`0x0201527C`, 4 bytes). `bit0 = IsSaveChecksumValid()` (save present), and `bit1 = bit0`, so the cursor starts on **Continue** when a save exists. Then `DISPCNT = 0`, `SetBrightnessBlack`, `ResetVideo`, `ResetBgScroll`, default BGxCNT (`Title_InitBgCnt`), `vblankFlags = 3`.
- **Setup:** repeats the video reset, then `Title_Draw` (`0x08004FD8`, which has the strings "New Game"/"Continue", installs the HBlank handler `0x08004ABD` and the VBlank callback `0x08004F09`), then `PlayBGMNoTrack(0)`. **The title BGM is song 0.**
- **Intro** (step 2): uses `FadeFromBlack`, `FadeToWhite`, `LoadBgImage(…, 0x087C056C)` and `FadeFromWhite`. It sets and clears the HBlank IRQ (hypothesis: the title logo flash/reveal).
- **HandleInput:** Left/Right (`newKeys & 0x30`) toggles between New Game and Continue, but only when a save exists (`PlaySE(0)`); otherwise it plays `PlaySE(3)` (error). A plays `PlaySE(1)`, calls `FadeOutBGM()` and advances to the next step.
- **FadeOut:** `FadeToBlack(1)`, clears the VBlank callback.
- **StartGame:** if **Continue** (bit1), it returns 1 immediately, so `CB_Title` returns 1 and `MainLoop` enters `CB_MainMenu`. For **New Game**: `StartDialogue(100)` (`0x08001C10`), `PlayBGM(1)`, runs the text-box runner `0x08001AE4` until it finishes, then `StarterDeckSelect_Run` (new-game setup: it calls `InitSaveData` and `SaveGame`), then `StartDialogue(101)` and runs it. Dialogue events 100/101 are Tea's intro ("Nice to meet you! I am $q02, a friend of …"); see [[text-system]]. Only then does it return 1, and the game goes to the main menu.

Hypothesis: step 5 (`0x0800553C`) is the attract/intro animation that plays between the title fade-out and the game start (it loads OBJ graphics `0x087CC1D4`/`0x087CBFD4` and BG `0x087C29D4`). It hasn't been traced case by case.

## Notes
- `0x08001AE4` is the entry the debug menu calls "Bustup" (a bust-up portrait dialogue), and `0x08001B34` is "Auto Bustup". They step through the text-box module's function table at `0x0813ADD4` ([[text-system]]) and also work as stand-alone scenes.

Related: [[program-flow]], [[license-sequence]], [[main-menu]], [[save-game]].
