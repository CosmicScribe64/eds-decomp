---
title: SaveGame (proposed) and save-data helpers
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# SaveGame and save helpers

All Thumb and nonmatching. The save image is `gSaveData` (`0x02011C20`, 0x2170 bytes), mirrored to SRAM `0x0E000000` through [[agb-sram]]. See also [[save-type]].

| Address | Size | Proposed name | Unit | Behaviour (verified) |
|---|---|---|---|---|
| `0x080754BC` | 0x34 | `SaveGame` | `code_080750E0` | `UpdateSaveChecksum()`, then up to 32 tries of `WriteSram(gSaveData, 0x0E000000, 0x2170)`; `if (!VerifySram(...)) break;` |
| `0x08077080` | 0x30 | `UpdateSaveChecksum` | `code_08076144` | `sum = Σ u16 gSaveData[0..0x10B5]`; `*(u16*)(gSaveData+0x216E) = -sum` |
| `0x08077034` | 0x4C | `IsSaveChecksumValid` | `code_08076144` | recomputes the sum and compares it against `+0x216E` → 1/0. The title screen uses it to enable **Continue**. |
| `0x08076FC4` | 0x32 | `MemDiffers(a, b, u8 n)` | `code_08076144` | returns 1 on the first differing byte, else 0 |
| `0x08076FF8` | 0x20 | `IsSaveSignatureValid` | `code_08076144` | `!MemDiffers("DMEX1INT" @0x081A78A8, gSaveData+0x2166, 8)`. **No callers found.** |
| `0x0807701C` | 0x10 | `WriteSaveSignature` | `code_08076144` | copies `"DMEX1INT"` to `gSaveData+0x2166` (through `0x080809CC`, a libc `strcpy`) |
| `0x080770BC` | 0x1A |  | `code_08076144` | `gSaveData[4] = x & 0x7F`, and sets bit 7 if `x == 0`. Bit 7 switches the text renderer path. |
| `0x080770DC` | 0xC |  | `code_08076144` | `sub_080770BC(1)`, called by `GameInit` |
| `0x080770E8` | 0x22 | `InitSaveData` | `code_08076144` | `MemClear16(gSaveData, 0x2170)`, `SetSeEnabled(1)`, `SetBgmEnabled(1)`, `sub_080770DC()`, `WriteSaveSignature()`. Called from new-game setup `0x08064604` and from `0x08074260`. |
| `0x080755A0` | 0x6C | `SaveAndResetSceneState` | `code_080750E0` | `SaveGame()`, clears the VBlank callbacks and HBlank, zeroes `gMain+0x4858..0x485B`, returns 1. **No callers or pointer references found (dead code).** A word `0xFFFFF01F` at `0x080555A0` decodes as a `bl` to `0x080755A2`, but it's a literal-pool entry. |

## Save layout so far (verified offsets)
| Offset | Meaning |
|---|---|
| +0x0004 | u8 text-mode byte (bit 7 → alternate text path) |
| +0x0008 | card trunk: `u32` per card ID, bits 0–9 = owned count (from [[special-card-lists]], `sub_0801B640`) |
| +0x2152 | u16 options: bit0 SE on, bit1 BGM on ([[sound-api]]) |
| +0x2166 | char[8] `"DMEX1INT"`. *DM EX1* matches the Japanese title *Duel Monsters 5 **Ex**pert **1*** (hypothesis about naming). |
| +0x216E | u16 checksum (two's-complement of the u16 sum of the first 0x216C bytes) |

The rest (card collection, decks, records, calendar) isn't mapped yet.

> [!question] Signature check
> Nothing calls `IsSaveSignatureValid`, and boot never checks the checksum, because `GameInit` loads the SRAM without validating it. The title screen only uses the checksum to decide whether Continue is offered. What happens with a corrupt save when the player picks New Game? `InitSaveData` probably overwrites it (hypothesis).

Related: [[save-type]], [[agb-sram]], [[set-main-callback]], [[ram-map]].
