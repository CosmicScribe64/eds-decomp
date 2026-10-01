---
title: Toolchain
type: tool
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Toolchain

Everything runs in a Docker image, so nothing is installed on the host.

| Purpose | Tool | Notes |
|---|---|---|
| Container | `docker/Dockerfile` → image `eds-decomp:latest` | Debian trixie-slim. `tools/dr <cmd>` runs a command in it with the repo mounted at `/work`. |
| Compiler | [[agbcc]] (`/opt/agbcc/bin/agbcc`, plus `old_agbcc` and `agbcc_arm`) | Built from pret/agbcc. Flags: [[compiler-flags]]. |
| Runtime libs | agbcc's `libgcc.a` / `libc.a` | Linked as-is; they match the ROM byte for byte. The member order is pinned in `tools/mkld.py`. |
| Assembler/linker | Debian `binutils-arm-none-eabi` 2.44 | `as -mcpu=arm7tdmi -mthumb-interwork` |
| Preprocessor | host `cpp` (GCC 14) with `-nostdinc -undef` | |
| Disassembler | `tools/disasm.py` (own, Python + capstone for ARM) | Recursive descent. Detects agbcc far jumps and jump tables, and fills gaps with prologue-first heuristics. Output is agbcc "divided" syntax. |
| Unit checker | `tools/check.py <unit> [--diff F]` | Compiles one unit, links it alone at its ROM address, and compares per function. Safe to run concurrently. |
| Skeletons | `tools/mkunit.py <unit>` | All-`INCLUDE_ASM` C file for a unit. |
| Linker script | `tools/mkld.py` (generated from `units.txt`) | Libraries sit at `@libs`; `@rodata` places a unit's rodata. |
| Auto symbols | `tools/autosyms.py` | Gives any undefined `name_XXXXXXXX` symbol the address in its name. |
| ROM diff | `tools/romdiff.py` | Runs automatically when `make compare` fails. |
| Card data | `tools/extract_cards.py` | stdlib only; `--verify` cross-checks known cards. |
| ROM map check | `tools/verify_rom_map.py` | Checks the [[rom-map]] boundaries. |

Decisions made on 2026-09-29: the compiler is agbcc, confirmed by the libgcc/libc byte match; everything runs in Docker; the project uses its own disassembler instead of gbadisasm or luvdis; and C units keep one asm file per function, included with `INCLUDE_ASM` (the splat-style workflow). See the [[log]].
