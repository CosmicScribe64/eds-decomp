---
title: Save type (SRAM)
type: rom
status: verified
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Save type

The ROM contains the Nintendo SDK library tag `SRAM_V112` at `ROM+0x87FB4` (`0x08087FB4`). That makes the game a 32 KiB SRAM save using the official AgbSram library, version 1.12. This is the plain variant, not the "fast" `SRAM_F_V1xx` one. The library code has been located and matched (see [[agb-sram]]).

- **Library code:** `.text` `0x0807ED04`–`0x0807EE98` has `ReadSram`, `WriteSram`, `VerifySram`, `WriteSramEx`, and the two static `_Core` routines. All match with agbcc `-O1`.
- **The words after the tag** at `0x08087FC0`–`0x08087FCF` (`0x0807ED05`, `0x0807ED29`, `0x0807EDCD`, `0x0807EDFD`) are *not* a function table. They're dead constant-pool entries that agbcc `-O1` emitted into the object's `.rodata` (the addresses of `ReadSram_Core`, `ReadSram`, `VerifySram_Core`, `VerifySram`). They're reproduced automatically by compiling [[agb-sram]]. *(This resolves the earlier hypothesis on this page.)*
- The library's `.rodata` is `0x08087FB4`–`0x08087FD0`. It's followed by an unrelated game table, so `agb_sram.o` was linked among the game objects and not at the very end.

## Save layout (from the call sites)
- **Size:** `0x2170` bytes (8560) at SRAM `0x0E000000`, well under the 32 KiB chip.
- **RAM mirror:** EWRAM `0x02011C20`.
- **Load:** once at boot, in `sub_08075DF4`, which AgbMain calls: `ReadSram(0x0E000000, 0x02011C20, 0x2170)`.
- **Save:** `sub_080754BC` (9 callers) calls `WriteSram` and then `VerifySram`, up to 32 times until verify returns 0. It doesn't use the library's `WriteSramEx` (3 tries), which is linked but never called.
- Checksums and the internal layout of the 0x2170-byte block haven't been mapped yet.

Related: [[gba-memory-map]] (SRAM at `0x0E000000`), [[nintendo-sdk-libraries]].
