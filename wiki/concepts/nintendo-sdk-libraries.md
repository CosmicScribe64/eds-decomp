---
title: Nintendo SDK Libraries
type: concept
status: solid
confidence: high
sources: [rom-analysis, general-knowledge]
updated: 2026-10-01
---
# Nintendo SDK libraries in EDS

GBA games statically link Nintendo SDK libraries and the compiler runtime (libgcc/libc). This code is cheap to match because reference sources exist in other decomps (pret, fireemblem8u, SAT-R/sa2). The matched SDK sources are in `src/sdk/`; `tools/dr make compare` verifies them as part of the full build.

## Linked libraries

| Library | Location | Mode | Status | Page |
|---|---|---|---|---|
| SDK `crt0.s` (start-up + `IntrMain` multi-IRQ dispatcher) | `0x080000C0`–`0x08000228` | ARM | **matching** (`asm/crt0.s`) | [[crt0]] |
| `libagbsyscall`: CpuFastSet, CpuSet, Div | `0x0807ECF8`–`0x0807ED04` | Thumb | **matching** (`src/sdk/libagbsyscall.s`) | [[bios-swi-stubs]] |
| AgbSram v1.12 (`SRAM_V112`) | `0x0807ED04`–`0x0807EE98`; rodata `0x08087FB4`–`0x08087FD0` | Thumb | **matching** (`src/sdk/agb_sram.c`, agbcc `-O1`) | [[agb-sram]] |
| libgcc `_call_via_rX` | `0x0807EE98`–`0x0807EED4` | Thumb | byte-identical to agbcc `libgcc.a` | |
| libgcc `__divsi3`, `__modsi3`, `__muldi3`, `__udivsi3`, `__umodsi3` | `0x0807EED4`–`0x0807F1E4` | Thumb | byte-identical (relocations masked) | |
| libgcc `dp-bit.o` (soft double) | `0x0807F1E4`–`0x0807FF80` | Thumb | byte-identical | |
| libgcc `fp-bit.o` (soft float) | `0x0807FF80`–`0x080808CC` | Thumb | byte-identical | |
| libgcc `__lshrdi3`, `__negdi2` | `0x080808CC`–`0x08080918` | Thumb | byte-identical | |
| libc `memcpy`, `memset`, `strcpy` | `0x08080918`–`0x08080A18` | Thumb | byte-identical | |
| ld interworking veneer (Thumb→ARM) | `0x08080A18`–`0x08080A20` | Thumb/ARM | linker-generated | |

- The libgcc/libc rows come from `tools/libmatch.py`, run over the extracted members of `/opt/agbcc/lib/libgcc.a` and `libc.a` with relocations masked. `.text` from `0x0807EE98` to the veneer is fully accounted for, with no gaps.
- **The veneer** is `bx pc; nop; b 0x0807EAD0`. It is a GNU-ld `.glue_7t`-style Thumb-to-ARM stub into the Konami ARM sound mixer, with one caller (`0x0807E3CC`, in the sound driver). It suggests the game was linked with GNU ld (hypothesis, but it fits agbcc).
- **`.rodata` starts at `0x08080A20`**, not `0x08080A24`. The veneer is 8 bytes, and the first string, `"Change BG:%d\n"`, is referenced as `0x08080A20` from the literal pool at `0x08000FA0`.

> [!warning] Contradiction
> The task brief said ".rodata starts 0x08080A24". The ROM shows `0x08080A20`: literal-pool references point at `0x08080A20`, and the veneer ends there. Resolved in favor of `0x08080A20` (verified).

## Libraries not linked (checked)

| Library | Evidence | Verdict |
|---|---|---|
| m4a / MP2K sound | No `m4a`/`Sappy` strings, no `0x68736D53` ("Smsh") ID word, no SWI calls to the sound BIOS. The sound code is a custom Konami driver (Thumb, about `0x0807D4xx`–`0x0807E3xx`) plus an ARM mixer at `0x0807EAD0`–`0x0807ECF0` (pool to `0x0807ECF8`). | **absent** |
| AgbPrint / mgba / no$ debug print | No `0x09FE2FFE`/`0x09FE209D` (ISAGB) or `0x04FFF780` (mGBA) literals. The debug printf is the empty variadic stub `0x0801A7DC` (`push {r0-r3}; add sp,#0x10; bx lr`), which 19 sites call with the `"DM5:Script=..."`-style strings. | **absent** (stubbed out) |
| printf / vsprintf (libc) | libc has only `memcpy`/`memset`/`strcpy`. In-game `%s`/`%d` text uses Konami helpers: `0x080753F4` (substitutes `%s`, 61 callers) and `0x08075434` (substitutes `%d`, 13 callers). | **absent** (custom) |
| MultiBoot (single-pak download) | No `swi 0x25`, no MultiBoot handshake constants (`0x6200`/`0x7202`), no library tag. | **absent** |
| Nintendo MultiSio library | The link code is Konami's own. The debug string `"BASICSIO:Time Out Recieve:%d"` is referenced from `0x0807222C`. Its structure is similar to MultiSio (multi-player SIO plus Timer3, one work area at `0x03005B60` of 0xB38 bytes). It has no `MultiSio...` version tag, and its init at `0x0807373C` differs from SDK `MultiSioInit` (no RCNT write, no CpuFill, `SIOCNT=0x2000`). Main cluster about `0x08072054`–`0x08074300`. Small SIO register accessors are at `0x0807BE7C`–`0x0807BF3C`. | **not SDK** (hypothesis: Konami "BASICSIO") |
| Other BIOS calls | Only 3 SWI stubs exist, and there are no inline SWIs. | see [[bios-swi-stubs]] |
| AgbFlash / EEPROM | Only the `SRAM_V112` tag is present. | **absent** |

## Method notes
- **Tags:** searched for `[A-Z0-9_]+_V\d{2,3}`, `AGB`, `Nintendo`, `MultiBoot`, `SIO`. The only library tag is `SRAM_V112`.
- **I/O register fingerprints:** tallied every literal-pool word in `.text` that falls in `0x04000000`–`0x040003FF`. SIO registers (`0x04000120`–`0x04000134`) appear only in the Konami link code, the SIO accessors, one game function (`0x08023228`), and the boot init.
- **Byte matching:** SDK objects were rebuilt with agbcc/binutils in Docker, linked at the ROM address with `ld`, and compared byte-for-byte (`tools/cmpobj.py`).
- **False positive:** `libmatch.py` "matches" libc `__errno` at `0x0807E9BC`. It is really a sound-driver getter (`ldr r0,=0x03005210; ldr r0,[r0]; bx lr`) that happens to share the masked pattern.
