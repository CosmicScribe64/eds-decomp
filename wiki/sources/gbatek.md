---
title: "Source: GBATEK"
type: source
status: solid
confidence: high
sources: []
updated: 2026-10-01
---
# Source: GBATEK

GBATEK is Martin Korth's hardware reference. The user requested ingestion of [the complete document](https://problemkaputt.de/gbatek.htm). The GBA sections below were read directly from the author's site on 2026-09-30, using the smaller section pages because the complete HTML page timed out. This page records the relevant findings and links; it does not claim review of the unrelated NDS/DSi/3DS sections.

## Reviewed sections and EDS cross-checks

| Section | Hardware facts reviewed | EDS evidence |
|---|---|---|
| [Memory map](https://problemkaputt.de/gbatek-gba-memory-map.htm) | EWRAM at `0x02000000`, IWRAM at `0x03000000`, MMIO at `0x04000000`; IWRAM supports full-width code/data accesses. SRAM has an 8-bit bus. | Matches [[ram-map]], the mixer's IWRAM destination `0x03005A54`, and the bytewise AgbSram routines in [[save-type]]. |
| [Sound DMA](https://problemkaputt.de/gbatek-gba-sound-channel-a-and-b-dma-sound.htm) | FIFO A/B consume signed 8-bit samples, are clocked by Timer0 or Timer1, and request a 16-byte DMA refill at the half-full threshold. | `sub_0807E324` advances the position by `0x10` per DMA1 IRQ, explaining the driver's IRQ accounting. |
| [DMA transfers](https://problemkaputt.de/gbatek-gba-dma-transfers.htm) | DMA1/2 support FIFO timing; this mode transfers 16 bytes and ignores the programmed count and width. Repeat retains enable; clearing enable stops future requests. The document requires an enable delay. | The driver writes `0xF6000004`, retains dummy reads, and re-enables FIFO DMA at buffer wrap. The literal count 4 is not the source of the 16-byte refill size. |
| [Sound control](https://problemkaputt.de/gbatek-gba-sound-control-registers.htm) | SOUNDCNT_H controls each FIFO's left/right routing, timer, reset, and volume. SOUNDBIAS selects output resolution and PWM rate. | High-byte `0xBB` sends both FIFO buses to both speakers, selects Timer0, and resets both. Initialization's bias bits select 8-bit/65.536-kHz output, separately from the sample timer rate. |
| [Timers](https://problemkaputt.de/gbatek-gba-timers.htm) | Reload is copied at overflow or a start transition. A 32-bit write can set reload and start together. Control bit 6 is IRQ enable, bit 7 start. | `0x0080FCB9` starts Timer0 at reload `0xFCB9`, without its own IRQ: `65536 - 0xFCB9 = 839` cycles per sample. |
| [Interrupt control](https://problemkaputt.de/gbatek-gba-interrupt-control.htm) | IE bits 3/9/10 select Timer0/DMA1/DMA2; IF is acknowledged by writing set bits. The BIOS forwards to an ARM handler pointer at `0x03007FFC`. | `IE &= 0xF9F7` clears these three sources; `IE |= 0x208` restores Timer0 and DMA1 enables. This agrees with [[crt0]] and [[interrupt-handlers]]. |

These checks confirm the existing hardware interpretation in [[sound-engine]] and the matched C in [[sound-driver]]. They do not establish the Konami track-bytecode format or justify changing the ROM's register accesses. In particular, EDS preserves its Timer0 IE bit even though timer control does not enable Timer0 IRQs.

## Caveats

GBATEK marks several hardware details as uncertain. Its memory-map paragraph says only DMA3 accesses Game Pak ROM, while its DMA register section describes DMA1/2 source addresses as unrestricted. No conclusion about that apparent conflict is needed for this driver, because its FIFO sources are in IWRAM and its ROM-to-IWRAM copy uses DMA3. Keep uncertain hardware claims separate from ROM-verified behavior.

The reference is copyrighted by Martin Korth; this wiki stores attributed summaries and direct section links. Earlier memory-only citations now have reviewed source coverage for the sections listed above. Further ARM instruction, BIOS, graphics and serial sections can be ingested as the corresponding routines are examined.

Related: [[gba-memory-map]], [[thumb-and-arm]], [[sound-engine]], [[sound-driver]], [[sound-mixer]], [[ram-map]].
