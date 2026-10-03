---
title: AgbSram v1.12 (ReadSram / WriteSram / VerifySram / WriteSramEx)
type: function
status: verified
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# AgbSram v1.12

This is Nintendo's SRAM access library, the plain (non-"fast") `SRAM_V112` build. EDS links the whole object, so there are 6 functions and one `.rodata` blob. All of them **match byte-for-byte**. See [[save-type]] and [[nintendo-sdk-libraries]].

| Field | Value |
|---|---|
| Address | `.text` `0x0807ED04`–`0x0807EE98` (0x194 bytes); `.rodata` `0x08087FB4`–`0x08087FD0` (0x1C bytes) |
| Mode | Thumb |
| File | `src/sdk/agb_sram.c` + `include/agb_sram.h` |
| Compiler | agbcc **`-mthumb-interwork -O1`** (old_agbcc `-O1` gives identical output). `-O2` does **not** match. |
| Match | **matching** (all 6 functions + rodata) |

| Function | Address | Size | Match | Notes |
|---|---|---|---|---|
| `ReadSram_Core` (static) | `0x0807ED04` | 0x24 | matching | byte-copy loop; copied to the stack by `ReadSram` |
| `ReadSram` | `0x0807ED28` | 0x64 | matching | sets WAITCNT, copies `_Core` to `u16[0x40]` on the stack, calls it via `_call_via_r3` |
| `WriteSram` | `0x0807ED8C` | 0x40 | matching | sets WAITCNT, byte-copy loop (runs from ROM) |
| `VerifySram_Core` (static) | `0x0807EDCC` | 0x30 | matching | returns the address of the first mismatch in `tgt`, or 0 |
| `VerifySram` | `0x0807EDFC` | 0x64 | matching | same trick as `ReadSram`, with a `u16[0x60]` buffer |
| `WriteSramEx` | `0x0807EE60` | 0x38 | matching | write + verify, up to 3 tries. **Never called by the game.** |

## Purpose
The GBA can only read SRAM correctly while it runs code from outside the ROM, because the game pak bus is shared. So `ReadSram` and `VerifySram` copy their `_Core` loop into a stack buffer on every call and run it from there, in Thumb, through `bx` to `buf+1`. The copy size is computed as the distance between adjacent functions, for example `((u32)ReadSram - (u32)ReadSram_Core) >> 1` halfwords, so **the link order inside this object matters**. v1.12 has no `SetSramFastFunc`, `ReadSramFast`, or `VerifySramFast`. Those belong to the separate `SRAM_F_V1xx` "fast" library, which fireemblem8u uses, for example.

## Callers / Callees
- `ReadSram` is called once, from `0x08075E28`, inside the boot init routine `GameInit`, which AgbMain calls at `0x08075F66`. That call is `ReadSram(0x0E000000, 0x02011C20, 0x2170)`.
- `WriteSram` is called from `0x080754D2`, and from `WriteSramEx`.
- `VerifySram` is called from `0x080754DC`, and from `WriteSramEx`.
- The game's save routine is `SaveGame` (9 callers). It does `WriteSram(0x02011C20, 0x0E000000, 0x2170)` and then `VerifySram`, and repeats while verify fails, **up to 32 tries**. It uses its own retry loop instead of the library's `WriteSramEx`, which only tries 3 times.
- Calls: `_call_via_r3` (libgcc `_call_via_rX.o`, `0x0807EEA4`).
- So the save data is **0x2170 bytes** at SRAM `0x0E000000`, mirrored in EWRAM at `0x02011C20` (verified from the literal pools at the call sites).

## Matching notes
- **Use `-O1`.** At `-O2` the prologues and loops differ, and the function-pointer call moves to `_call_via_r4`. `-O1` also explains the rodata (next point). This suggests Konami built the SDK library with Nintendo's own `-O1` flags. Game code may use different flags.
- **The rodata comes from the compiler.** After `"SRAM_V112\0"` (10 bytes, padded to 12) come four words: `0x0807ED05`, `0x0807ED29`, `0x0807EDCD`, `0x0807EDFD` (`ReadSram_Core`, `ReadSram`, `VerifySram_Core`, `VerifySram`). These are dead `.LC` constant-pool entries that agbcc `-O1` emits into `.rodata` for the function addresses used in the size computation. The code itself loads the addresses from the `.text` literal pools. Don't write these words as data. Compile the C and link its `.rodata` at `0x08087FB4`. An unrelated table follows at `0x08087FD0`.
- **Load `Func_src`, then XOR it in a second statement:** `Func_src = (u16 *)ReadSram_Core; Func_src = (u16 *)((u32)Func_src ^ 1);`. A one-expression `((u32)ReadSram_Core ^ 1)` makes CSE reuse the register for the size computation. The ROM reloads `ReadSram_Core` from the pool instead, because the XOR clobbered the register.
- The Thumb bit is cleared with `eor #1`, not `& ~1`.
- `while (size--)` and `while (--size != -1)` produce the same code.
- **Verification:** assemble the object, then link it with `ld` (`.text=0x0807ED04`, `.rodata=0x08087FB4`, `_call_via_r3` as a Thumb stub at `0x0807EEA4`). Compare byte-for-byte with the baserom: 0 differing bytes. A combined link of `libagbsyscall.o` + `agb_sram.o` + libgcc `_call_via_rX.o` at `0x0807ECF8` also reproduces `0x0807ECF8`–`0x0807EED4` exactly. Both `tools/dr python3 tools/check.py sdk/agb_sram` and the full `make compare` verify `src/sdk/agb_sram.c`.

Neighbors: [[bios-swi-stubs]] (before, `0x0807ECF8`); libgcc `_call_via_rX` (after, `0x0807EE98`). See [[nintendo-sdk-libraries]] for the full `.text` tail map.
