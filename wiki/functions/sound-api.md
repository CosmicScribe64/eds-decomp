---
title: Game-side sound API (PlaySE, PlayBGM, …)
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Game-side sound API (`0x08077A44`–`0x08077C10`)

These are thin wrappers in unit `asm/collection.s`, all Thumb, all nonmatching (not attempted). They gate driver calls on the option flags in the save (`gSaveData+0x2152`, u16 at `0x02013D72`) and track the current BGM in `gMain.currentBgm` (`0x0300489C`). Driver internals are in [[sound-engine]].

| Address | Size | Proposed name | Behaviour (verified) | Call sites |
|---|---|---|---|---|
| `0x08077A44` | 0x0E | `IsSeEnabled` | `gSaveData.options & 1` | |
| `0x08077A5C` | 0x0E | `IsBgmEnabled` | `(gSaveData.options >> 1) & 1` | |
| `0x08077A74` | 0x30 | `SetSeEnabled(bool)` | sets or clears bit 0 | `GameInit`, `InitSaveData` |
| `0x08077AB0` | 0x30 | `SetBgmEnabled(bool)` | sets or clears bit 1 | `GameInit`, `InitSaveData` |
| `0x08077AEC` | 0x2E | `PlaySE(id)` | if SE is enabled and `gMain.lastSeFrame != gMain.frameCounter`: record the frame, `SoundRequestSE(id)`. At most one SE per frame. | 288 |
| `0x08077B24` | 0x28 | `PlayBGM(id)` | if BGM is enabled and `id != gMain.currentBgm`: `SoundRequestBGM(id)`, `currentBgm = id` | 18 |
| `0x08077B54` | 0x1A | `PlayBGMNoTrack(id)` | if BGM is enabled: `SoundRequestBGM(id)`. Does not update `currentBgm`. Used for the title (0) and the main menu (3). | |
| `0x08077B70` | 0x28 | `PlayBGMIfSeEnabled(id)` (hypothesis: jingle/fanfare) | like `PlayBGM` but gated on the SE flag | |
| `0x08077BA0` | 0x20 | `StopBGM` | if BGM is enabled: `SoundStopBGM()`. Always sets `currentBgm = 0xFFFF`. | |
| `0x08077BCC` | 0x20 | `FadeOutBGM` | if BGM is enabled: `SoundFadeOutBGM(0x10)`. Always sets `currentBgm = 0xFFFF`. | menus on confirm |
| `0x08077BF8` | ? | `FadeOutBGMForce` (hypothesis) | `SoundFadeOutBGM(r0)` with no flag check, then `currentBgm = 0xFFFF` | |

SE ids seen in menus (hypothesis from context): 0 is the cursor move, 1 is confirm, 2 is cancel or back, and 3 is the error buzz ("Continue" with no save).

> [!note]
> `GameInit` calls `SetSeEnabled(1)` and `SetBgmEnabled(1)` after loading the save. Unless an options screen changes them later, the stored flags have no effect across power cycles. No options screen that writes these bits has been found yet.

Related: [[sound-engine]], [[save-game]], [[ram-map]].
