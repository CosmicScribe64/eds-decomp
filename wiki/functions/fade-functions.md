---
title: Screen brightness / fade helpers (cluster)
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Fade helpers (`0x080757F4`–`0x08075C3C`)

All of these live in unit `asm/code_080750E0.s`. They are all Thumb and all nonmatching (not attempted). They drive the hardware brightness effect (`BLDCNT` `0x04000050`, `BLDY` `0x04000054`) and keep the current level in the low 6 bits of `gMain.brightness` (`0x03004872`, u8). The top 2 bits of that byte are preserved.

| Address | Size | Proposed name | Behaviour (verified) |
|---|---|---|---|
| `0x080757F4` | 0x1C | `ClearBlend` | level = 0, `BLDCNT = 0`, `BLDY = 0` |
| `0x080759F4` | 0x26 | `SetBrightnessBlack` | level = 0x1F, `BLDCNT = 0x3FFF` (all layers, darken), `BLDY = 0x1F` |
| `0x08075A30` | 0x26 | `SetBrightnessWhite` | level = 0x1F, `BLDCNT = 0x3FBF` (all layers, brighten), `BLDY = 0x1F` |
| `0x08075A6C` | 0x78 | `FadeToBlack(step)` | `BLDCNT = 0x3FFF`; level += step (clamped to 0x1F); `BLDY = level`; returns 1 once the level is > 0x1E |
| `0x08075AE4` | 0x6C | `FadeFromBlack(step)` | level −= step (floor 0). At 0 it calls `ClearBlend` and returns 1; otherwise `BLDY = level`, `BLDCNT = 0x3FFF` |
| `0x08075B58` | 0x78 | `FadeToWhite(step)` | like `FadeToBlack` but `BLDCNT = 0x3FBF` |
| `0x08075BD0` | 0x6C | `FadeFromWhite(step)` | like `FadeFromBlack` but `BLDCNT = 0x3FBF` |

Scene steps call a fade every frame and advance when it returns 1. For example, the main menu uses `FadeFromBlack(1)` to fade in and `FadeToBlack(2)` before launching a mode. `FadeToBlack` has 25 call sites and `FadeFromBlack` 21. `GameInit` starts the game with `SetBrightnessWhite`, and the License steps fade both from white and to white.

## Matching notes
- Level updates look like `gMain.brightness = (gMain.brightness & 0xC0) | (x & 0x3F)`, which suggests a `u8 level:6; u8 flags:2;` bitfield (agbcc emits `lsls #0x1a; lsrs #0x1a` to extract it).
- The `FadeTo*`/`FadeFrom*` pairs differ only in the BLDCNT constant. They were probably written as separate copies rather than one parameterised function.

Related: [[program-flow]], [[ram-map]].
