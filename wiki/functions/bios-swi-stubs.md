---
title: BIOS SWI Stubs (libagbsyscall)
type: function
status: verified
confidence: high
sources: [rom-analysis]
updated: 2026-09-29
---
# BIOS SWI stubs (libagbsyscall)

| Field | Value |
|---|---|
| Address | `0x0807ECF8`–`0x0807ED04` (0xC bytes) |
| Size | 3 × 4 bytes |
| Mode | Thumb |
| File | `src/sdk/libagbsyscall.s` (future `src/sdk/libagbsyscall.s`) |
| Match | **matching** (assembled and compared byte-for-byte) |

| Stub | Address | SWI | BIOS function | Call sites (BL) |
|---|---|---|---|---|
| `CpuFastSet` | `0x0807ECF8` | `0x0C` | CpuFastSet (32-byte-block copy/fill) | 48 |
| `CpuSet` | `0x0807ECFC` | `0x0B` | CpuSet (16/32-bit copy/fill) | 153 |
| `Div` | `0x0807ED00` | `0x06` | Div (signed divide) | 2 (`0x0807B50E`, `0x0807B528`) |

Each stub is `svc #N; bx lr`.

## Purpose
These are Nintendo SDK `libagbsyscall.a` members. The linker only pulls the members that are referenced, which gives the three here. They sit directly after the Konami ARM sound mixer (which ends with its literal pool at `0x0807ECF0`–`0x0807ECF8`) and directly before [[agb-sram]].

## Findings
- **These are the only SWIs in the game.** A scan of all of `.text` (`0x080000C0`–`0x08080A20`) for Thumb `swi` (`0xDFxx`) and ARM `swi` (`0xEFxxxxxx`) finds nothing else that is code (4 other `0xDFxx` hits are inside data). So EDS never calls `VBlankIntrWait`, `LZ77UnComp*`, `SoftReset`, `MultiBoot`, `Sqrt`, `ArcTan` or any other BIOS function through SWI. BIOS decompression is **not** used. Graphics compression, if any, is custom (see [[open-questions]]).
- The member order is CpuFastSet, CpuSet, Div (alphabetical). pokeemerald's `libagbsyscall.s`, which mirrors a different SDK archive's member order, would put Div first. That hints at a different SDK release (hypothesis).

## Matching notes
- Written pret-style with `thumb_func_start`/`svc` and `.syntax unified`. Each function is `.align 2`.
- Verified by linking at `0x0807ECF8` and comparing with the baserom, and again as part of a combined link with [[agb-sram]] (see there).
