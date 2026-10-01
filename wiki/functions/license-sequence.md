---
title: CB_License (proposed) and the boot-logo steps
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# CB_License `sub_08004EAC`

| Function | Address | Size | Proposed name |
|---|---|---|---|
| runner | `0x08004EAC` | 0x5C | `CB_License` |
| step 0 | `0x08004AF8` | 0x72 | `License_InitVideo` |
| step 1 | `0x08004B84` | 0x134 | `License_ShowNintendoNotice` |
| step 2 | `0x08004CBC` | 0xA0 | `License_ShowKonamiLogo` |
| step 3 | `0x08004D60` | 0x12E | `License_ShowKcejLogoThenTitle` |

Mode: Thumb. Unit: `asm/code_080044E4.s`. Match: nonmatching.

## Purpose (verified)
This is the first scene. `GameInit` installs it, and the debug table calls it "License". The runner reads the step table at `0x0819879C`, indexed by `gMain+0x4878`, with the sub-state at `+0x4858`.

1. **InitVideo:** `SetBrightnessWhite`, `DISPCNT = 0`. Then `vblankFlags = 3`, `ResetVideo`, `ResetBgScroll`, `BG0CNT..BG3CNT = 0x84, 0x105, 0x206, 0x307`, backdrop color = white (`0x05000000 = 0xFFFF`).
2. **Nintendo notice:** draws the string **"LICENSED BY NINTENDO"** (`0x080813F0`), centred using `StrLen`. Enables BG1, `FadeFromWhite(1)`, waits 120 frames (`gMain+0x4859` as a timer), `FadeToWhite(1)`, disables BG1.
3. **Konami logo:** `LoadBgImage(0, 0, 0x20, 0x087D01F4)`, BG0 on, fade in from white, 120 frames, fade out to white. The image is the Konami logo (rendered from the ROM).
4. **KCEJ logo:** `LoadBgImage(…, 0x087D292C)` is the text "Konami Computer Entertainment Japan". Fade in, 120 frames, then `FadeToBlack(1)`. After that it does an **inline "set main callback"** without saving: clears the VBlank callbacks and HBlank, zeroes the sequencer bytes, and sets `gMain.callback = CB_Title` (`0x080057BD`).

The runner can only return 1 (sequence finished) if step 3 didn't replace the callback, so in practice the License scene always hands over to the Title.

## Notes
- `LoadBgImage` format (verified from these images): `u16 nPal; …; u16 pal[nPal] @+8; u16 nTiles; …; u8 tiles[nTiles][64] (8bpp) @+0x10+2*nPal; u16 nMap; …; {u16 pos (x = pos & 0x3F, y = pos >> 8); u16 tile}[nMap] @+8`. See [[video-helpers]].

Related: [[program-flow]], [[title-screen]], [[fade-functions]].
