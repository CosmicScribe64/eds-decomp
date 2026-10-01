---
title: Interrupt handlers (VBlankIntr, Timer2Intr, SoundDma1Intr, serial, gamepak)
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Interrupt handlers

The dispatcher is `IntrMain` (see [[crt0]]). It runs from IWRAM `0x0300004C` after boot and jumps through `IntrTable` (16 words at `0x03000000`). Slot order is also dispatcher priority: **0 Serial, 1 HBlank, 2 VBlank, 3 VCount, 4–7 Timer0–3, 8–11 DMA0–3, 12 Keypad, 13 Gamepak** (never dispatched).

| Slot | Handler | Address | Size | Installed by |
|---|---|---|---|---|
| 2 VBlank | `VBlankIntr` (proposed) | `0x0807569C` | 0x58 | `GameInit` |
| 6 Timer2 | `Timer2Intr` (proposed) | `0x0807570C` | 0x22 | `GameInit` |
| 9 DMA1 | `SoundDma1Intr` (proposed) | `0x0807E324` | 0x62 | `GameInit` (or `SoundInit(slot)`) |
| 13 | `GamepakIntr` (proposed) | `0x08075740` | 2 (`b .`) | `GameInit`. Unreachable, because `IntrMain` loops on GAMEPAK itself. |
| 1 HBlank | per scene | e.g. `0x08004ABD` (title), `0x080261E1`, `0x08026CC9`, `0x0805DC39`, `0x08062421` | | scene code, which writes `*(u32*)0x03000004` with IME off and IE bit 1 cleared first. Cleared by `SetMainCallback`. |
| 0 Serial + 7 Timer3 | link IRQ (proposed `LinkSerialIntr`) | `0x08075F74` | 0x102 | `sub_080735D4(slotA, slotB)` stores `0x08075F75` into **both** slots it is given and enables IE SERIAL and TIMER3 |

All handlers are Thumb. They're called from ARM `IntrMain` through `bx r0`, in SYS mode with IRQs re-enabled (nesting allowed for Serial, DMA1 and Gamepak only).

## VBlankIntr `0x0807569C` (verified)
```c
void VBlankIntr(void) {
    gMain.intrCheck |= 1;           // 0x0300044C (volatile)
    gMain.vblankCounter++;          // u16 0x030048A4
    gMain.lagCounter++;             // u16 0x030048A6 (MainLoop zeroes it)
    if (gMain.vblankCallbackEarly)  // 0x03000458 (the link code uses it)
        gMain.vblankCallbackEarly();
    SoundVBlank();                  // 0x0807E3B0: sequencer tick + ARM mixer
    if (gMain.vblankCallback)       // 0x03000454 (per scene)
        gMain.vblankCallback();
    gMain.vblankCounter8++;         // u8 0x030048A1
}
```
There is no OAM/palette DMA and no key read in the IRQ. That work happens in the main thread in [[frame-sync-update]]. The handler doesn't write BIOS `INTR_CHECK` (`0x03007FF8`), which is fine because nothing uses `VBlankIntrWait`.

## Timer2Intr `0x0807570C` (verified)
The handler rewrites `TM2CNT_L = 0xF400`, sets `TM2CNT_H |= 0xC3`, and increments the u16 at `0x030049D0+0x82C` (`0x030051FC`). The period is 0xC00 × 1024 cycles ≈ 0.1875 s (≈ 5.33 Hz). Nothing else was found reading `0x030051FC` directly, so its purpose is unknown (see [[program-flow]] open questions).

## SoundDma1Intr `0x0807E324` (verified)
```c
pos = gSoundDmaPos[0] + 16;                // u16 @0x0300540C
if (pos > 0x2BF) {                         // end of the 0x2C0-byte ring
    DMA1CNT_H &= 0xC5FF; DMA1CNT_H &= 0x7FFF;   // stop DMA1
    DMA2CNT_H &= 0xC5FF; DMA2CNT_H &= 0x7FFF;   // stop DMA2
    DMA1SAD = 0x03005414; DMA1DAD = FIFO_A; DMA1CNT = 0xF6000004;
    DMA2SAD = 0x03005734; DMA2DAD = FIFO_B; DMA2CNT = 0xF6000004;
    pos = 0;
}
gSoundDmaPos[2] = pos; gSoundDmaPos[0] = pos;   // FIFO B pos, FIFO A pos
```
DMA2 is configured with its IRQ bit set, but `SoundDmaInit` leaves IE bit 10 (DMA2) cleared, so only DMA1 interrupts. See [[sound-engine]].

## Link serial IRQ `0x08075F74` (verified mechanics, hypothesis for its origin)
It reads `SIOMULTI0–3` into `0x03006644`. If the first word is `0xFEFE` (sync) and the state counter at `0x0300658C` is ≥ 10, it resets that counter to −3. Otherwise it copies received words into a buffer, advances the counter (up to 10) and, while the counter is ≤ 9, loads `SIOMLT_SEND` (`0x0400012A`) from a send table at `0x03005B60+0xA3C`. If the "master" flag (`+0xA1E`) is set, it starts the next transfer (`SIOCNT |= 0x80`) and restarts Timer3 (`TM3CNT_H = 0xC0`). The design (the 0xFEFE sync word, and Timer3 driving a multi-player SIO block) looks like **Nintendo's MultiSio sample library** (hypothesis, not byte-compared). It is set up by `sub_080735D4(slotA, slotB)`:
1. `IE &= ~(SERIAL|TIMER3)` and CpuSet-clear the state at `0x03005B60`.
2. `RCNT = 0xC000`, then `SIOCNT = 0x1000`, `0`, `3`, `|= 0x2000` (multi-player mode, 115200 bps), and `RCNT = 0`.
3. With IME off: `IE |= SERIAL`, store `0x08075F75` into both slots, `SIOCNT |= 0x4000` (IRQ).
4. Unless bit 2 of `+0xB0C` is set, `IE |= TIMER3`.

This confirms the two slots are Serial (0) and Timer3 (7). The IE bits verify it; the caller's arguments were not traced.

## Matching notes
- `VBlankIntr` re-reads `gMain.intrCheck` (`ldrh r2; ldrh r3; orr; strh`), which is consistent with `volatile`.
- `GamepakIntr` is `while (1);`, which compiles to `b .`.

Related: [[crt0]], [[program-flow]], [[agb-main]], [[sound-engine]], [[ram-map]].
