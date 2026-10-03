---
title: FrameSyncUpdate (proposed) and FlushOamBuffer (proposed)
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# FrameSyncUpdate `FrameSyncUpdate` / FlushOamBuffer `FlushOamBuffer`

| Function | Address | Size | Mode | Unit | Match |
|---|---|---|---|---|---|
| `FrameSyncUpdate` | `0x08075CB4` | 0x9C | Thumb | `asm/code_080750E0.s` | nonmatching |
| `FlushOamBuffer` | `0x08075C44` | 0x5C | Thumb | same | nonmatching |

## Purpose (verified)
`MainLoop` calls this right after it sees the VBlank flag, so it runs early in VBlank but in the main thread. It commits the previous frame's shadow state to the hardware and gathers input:
```c
void FrameSyncUpdate(void) {
    for (i = 0; i < 4; i++) {
        if (gMain.vblankFlags & sHofsRegs[i].mask)      // table 0x081A7764: {0x04000010,0x10} {..14,0x20} {..18,0x40} {..1C,0x80}
            *sHofsRegs[i].reg = gMain.bgHofs[i];        // 0x03004468
        if (gMain.vblankFlags & sVofsRegs[i].mask)      // table 0x081A7784: {0x04000012,0x100} … {0x0400001E,0x800}
            *sVofsRegs[i].reg = gMain.bgVofs[i];        // 0x03004460
    }
    if (gMain.vblankFlags & 2)                          // BG tilemaps
        for (i = 7; i >= 0; i--)
            CopyDoubleWords(VRAM + i*0x800, gMain.bgMapBuffer[i], 0x800);   // 0x080752B0
    FlushOamBuffer();       // 0x08075C44
    ReadKeys();             // 0x08075228, see [[read-keys]]
    SoundMain();            // 0x0807E554, starts pending BGM/SE, see [[sound-engine]]
    Random();               // 0x08076F9C, advance the RNG once per frame, see [[random]]
}
```
`FlushOamBuffer`: if `vblankFlags & 1`, it copies `gMain.oamBuffer` (`0x03004470`, 0x400 bytes) to OAM `0x07000000`, zeroes `oamCount` (`0x03004870`) and `0x03004871`, and resets all 128 entries to 0 with `attr2` priority bits set to 3 (`|= 0x0C` on byte +5). Scenes rebuild OAM from scratch every frame through `AddSprite` (`0x080761F0`).

> [!note]
> All 8 screenblocks (16 KiB) are copied with CPU `ldm/stm` every frame while flag bit 1 is set. There's no DMA here. Screenblocks 0–7 live at `0x06000000`–`0x06003FFF`, and BG tiles are loaded to charblock 1 (`0x06004000`) by `LoadBgImage`.

## Callers and callees
- Called by: `MainLoop` `0x08075D70`.
- Calls: `CopyDoubleWords` `0x080752B0`, `FlushOamBuffer` `0x08075C44`, `ReadKeys` `0x08075228`, `SoundMain` `0x0807E554`, `Random` `0x08076F9C`.

## Matching notes
- The scroll loop keeps `gMain` in `ip` and the two register-table pointers in `r8`/`r3`, with a shared index. The source is probably two parallel struct arrays `{vu16 *reg; u16 mask;}` (8 bytes each, with padding).
- The BG copy counts down from 7 to 0 with `bge`, so it was likely written as a `for (i = 0; i < 8; i++)` loop that got reversed, or a `do … while` countdown.

Related: [[program-flow]], [[ram-map]], [[video-helpers]].
