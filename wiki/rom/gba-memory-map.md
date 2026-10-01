---
title: GBA memory map
type: rom
status: solid
confidence: high
sources: [gbatek]
updated: 2026-10-01
---
# GBA memory map

| Region | Range | Size | Notes |
|---|---|---|---|
| BIOS | `0x00000000` | 16 KiB | SWI handlers |
| EWRAM | `0x02000000` | 256 KiB | 16-bit bus, slow; main game state |
| IWRAM | `0x03000000` | 32 KiB | 32-bit bus, fast; ARM code and hot data are often copied here |
| I/O | `0x04000000` | | Hardware registers |
| Palette | `0x05000000` | 1 KiB | |
| VRAM | `0x06000000` | 96 KiB | |
| OAM | `0x07000000` | 1 KiB | |
| ROM | `0x08000000` | up to 32 MiB | EDS uses 8 MiB (see [[rom-header]]) |
| SRAM | `0x0E000000` | 32 KiB here | See [[save-type]] |

In this wiki, a ROM file offset `ROM+0xN` corresponds to address `0x08000000 + N`.
