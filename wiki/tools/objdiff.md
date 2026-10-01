---
title: objdiff
type: tool
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# objdiff

[objdiff](https://github.com/encounter/objdiff) is an object-file differ, and it generates the progress report that [decomp.dev](https://decomp.dev/projects) displays. `objdiff-cli` v3.8.2 is installed in the Docker image at `/usr/local/bin/objdiff-cli`.

## Usage in this repo
```
tools/dr make -j8 objdiff-report      # -> build/report.json + a summary
```
- `tools/mkobjdiff.py` generates `objdiff.json`, which is git-ignored.
  - There is one unit per code unit in `units.txt`, named `game/<u>`, `sound/<u>` or `sdk/<u>`, with matching progress categories.
  - `complete` (decomp.dev's "fully linked") means the unit's C has no INCLUDE_ASM left.
  - Each unit carries decomp.me scratch settings (platform `gba`, compiler, flags).
- The target object is `build/objdiff/target/<u>.o`, the unit's original assembly (`asm/<u>.s`).
- The base object is `build/objdiff/base/<u>.o`, the unit's C compiled with `-DOBJDIFF_BASE`. That makes `INCLUDE_ASM` expand to nothing (`include/global.h`), so asm functions count as unmatched instead of trivially matching themselves.
- **Relocation resolving** (`tools/objdiff_resolve.py`). The original asm writes some literal-pool words as numbers and others as symbols, and so does the C, but never consistently. Both sides get their `R_ARM_ABS32` references resolved to final values. The addresses come from the `_XXXXXXXX` name suffix, `config/functions.tsv`, `symbols.ld`, and the containing function's ROM address for `.text`-relative references. Without this, about 440 matched functions scored below 100%.
- **Function sizes.** The `.size` of each asm function comes before its trailing `.align 2, 0` padding, as agbcc emits it (fixed in `tools/disasm.py` and in 340 `asm/nonmatching` files; the ROM is still byte-identical). Without this, every function followed by padding scored below 100%.
- **No baserom is needed**, so the report runs in CI (`.github/workflows/progress.yml`, which uploads the `AY5E_report` artifact).

## Cross-check (2026-09-30)
objdiff counted 1413/1976 functions (71.5%) and 40.6% of code bytes. `tools/progress.py` counted 1415 functions and 40.7%. The only functions in C but below 100% were in units an agent was editing at that moment.

## Listing on decomp.dev
1. Make the repo public on GitHub. First replace the committed ROM data (`data/*.s`, graphics and audio) with extraction from the user's own ROM; see [[rom-map]].
2. CI uploads `AY5E_report` on every push.
3. Ask the decomp.dev maintainers to add the project; the site's admins register projects. The exact process is unconfirmed (hypothesis).
