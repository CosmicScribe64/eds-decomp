---
title: Thumb and ARM code
type: concept
status: solid
confidence: high
sources: [gbatek]
updated: 2026-10-01
---
# Thumb and ARM code

- The ARM7TDMI has two instruction sets: 32-bit **ARM** and 16-bit **Thumb**. Code running from ROM is usually Thumb, because the ROM bus is 16-bit.
- Interworking uses `bx`. A branch target with its low bit set is Thumb, so a pointer like `0x0807ED05` points to Thumb code at `0x0807ED04` (see [[save-type]]).
- Performance-critical ARM routines (IRQ handlers, math) are often copied to IWRAM at boot. See [[gba-memory-map]].
- The [[rom-header]] entry point and crt0 are ARM.
