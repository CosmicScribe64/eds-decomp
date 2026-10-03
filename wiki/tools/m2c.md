---
title: m2c
type: tool
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# m2c

[m2c](https://github.com/matt-kempster/m2c) ("machine code to C") is the decompiler behind decomp.me. It supports ARM (`-t arm-gcc-c`). It is installed in the Docker image at `/opt/m2c`.

## Usage
```
tools/dr python3 tools/m2c_draft.py <unit> [func ...]   # default: the unit's INCLUDE_ASM functions with no draft
tools/dr python3 tools/m2c_draft.py --all
```
Drafts go to `build/m2c/<unit>/<func>.c`; `src/` is never touched. A draft is a starting point. It rarely compiles as-is, because it has `?` types, `unkXX` fields and goto-heavy control flow. Fresh-function agents (`tools/launch_fresh.sh`) refine them.

## Preparing our asm for m2c (`tools/m2c_draft.py`)
- **Jump tables.** A literal-pool word pointing at a label in the same function becomes that label. m2c only finds agbcc's Thumb switch tables (`lsl; ldr =table; add; ldr; mov pc`) when the pool word is a symbol. Without this, 33 of 162 functions failed.
- **Old spellings.** Divided-syntax `ldsh`/`ldsb` become `ldrsh`/`ldrsb`.
- **Symbol names.** Pool words holding RAM or ROM-data addresses become symbols: real names from `build/eds.elf`, else `gUnk_XXXXXXXX`.
- **Context.** The unit's own C, preprocessed with other function bodies stripped, is passed as `--context`, so drafts use our structs and field names, e.g. `gAiState.phase`.

## Results (2026-09-30)
Drafts were generated for all 162 functions that had no C attempt, with 0 failures, in about 2.5 minutes.
