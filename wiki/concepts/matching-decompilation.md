---
title: Matching Decompilation
type: concept
status: solid
confidence: high
sources: [general-knowledge]
updated: 2026-10-01
---
# Matching decompilation

The goal is C source (plus minimal asm) that, when compiled with the original compiler and flags, rebuilds a byte-identical ROM (SHA-1 `510fbba2…4dea`, see [[rom-header]]).

## Typical GBA workflow
1. Disassemble the baserom into `asm/` and `data/`, split by address ranges, and keep `make compare` passing with 100% asm.
2. Identify the compiler. Most 2001–2003 GBA titles used agbcc (GCC 2.95-era, `-O2 -mthumb-interwork`). Some used ARM SDT/ADS. See [[agbcc]].
3. Decompile function by function. Write C, compile it, and diff against the target asm (with [[objdiff]], asm-differ, or decomp.me). Adjust the C until register allocation and instruction order match.
4. Replace the asm with C, keep the build matching, and repeat.
5. Name symbols and document structs. This is where the wiki is most useful.

## Terms
- **matching**: byte-identical output.
- **nonmatching / equivalent**: functionally correct but not byte-identical. Keep these behind `NONMATCHING` guards.
- **baserom**: the original ROM, used only for comparison and data extraction. Never commit it.
