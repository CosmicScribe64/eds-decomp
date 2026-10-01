---
title: ROM Header
type: rom
status: verified
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# ROM Header

These facts come from the baserom's cartridge header (`ROM+0x00`–`ROM+0xBF`).

| Field | Offset | Value |
|---|---|---|
| Entry branch | `ROM+0x00` | `0xEA00002E` (ARM `b` to `0x080000C0`) |
| Game title | `ROM+0xA0` | `YU-GI-OH!EDS` |
| Game code | `ROM+0xAC` | `AY5E` (E = USA) |
| Maker code | `ROM+0xB0` | `A4` (Konami) |
| Version | `ROM+0xBC` | `0` |
| Header checksum | `ROM+0xBD` | `0x9C` |

## Image
- Size: 8 MiB (`0x800000`) ROM.
- Last non-zero byte is at about `ROM+0x7F8568`, so the image is almost full.
- SHA-1: `510fbba212aca9bab95ea12f8fd933e62ee34dea`
- MD5: `ec3f000ffde5754cb164a05d2d6f9053`

These hashes are the build target (see [[matching-decompilation]]).

## Startup
At `0x080000C0`, the code sets the CPU mode through `msr cpsr` (IRQ mode `0x12`, then System mode `0x1F`) and loads stack pointers. This is standard crt0 behavior. See [[crt0]] and [[gba-memory-map]].

## Verification
Checked with a Python header dump on 2026-09-29. The procedure is logged in [[log]].
