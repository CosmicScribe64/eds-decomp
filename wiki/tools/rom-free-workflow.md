---
title: Working without the ROM (GitHub agents, CI)
type: tool
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Working without the ROM

Goal: let GitHub-connected agents and CI work on the matching decomp without the game ever being committed or provided. ROMs are never put on GitHub. Contributor and agent rules are in `CLAUDE.md` (Matching rules).

## How it works
- **Targets come from the original assembly.** `tools/target.py` exposes `ROM`, which is the real `baserom.gba` when present. Without it, a slice-only view assembles the containing unit's `asm/<unit>.s` (with `asm/nonmatching/<unit>/*.s`), links it alone at the unit's ROM address with symbol stubs, and returns the bytes. Results are cached in `build/target-cache/`. `check.py`, `permute.py`, `check_all.py` and the private scripts that slice `check.ROM` all use it.
- **Verified equivalence (2026-10-01):** `tools/dr python3 tools/target.py --verify` with the ROM reports **112/112 code units: assembled original == ROM**. Data, the header and graphics aren't covered; only the full link sees them.
- Without `build/eds.elf`, symbols come from `config/symbols.txt`. It lists the 69 global symbols whose address can't be derived from `config/functions.tsv`, `symbols.ld` or a `_XXXXXXXX` suffix (libgcc, `IntrMain`, SDK labels, …). `tools/dumpsyms.py` regenerates it after every full link (Makefile hook), and `check.py` merges it with `build/eds.elf` when that exists.
- **No Docker:** `tools/setup_native.sh [PREFIX]` installs binutils, builds pret/agbcc, and sets up a venv with capstone/pyelftools plus, optionally, decomp-permuter, m2c and objdiff-cli. It writes `PREFIX/env.sh`. `tools/dr` runs commands directly when Docker is missing or `EDS_NATIVE=1`. Tool paths are configurable through `AGBCC_DIR` (Makefile, `check.py`, `permute.py`, `m2c_draft.py`) and `PERMUTER_DIR`.

## The CI gate (`.github/workflows/progress.yml`)
1. `tools/check_all.py`: every C unit reports `unit bytes MATCH`, and every authored assembly unit (`src/<u>.s`, for example the ARM mixer) assembles to the same bytes as its original. 112/112 pass without a ROM in about 15 s.
2. On pull requests, `tools/progress.py`'s function count may not drop below the base commit's.
3. The objdiff report (`make objdiff-report`, ROM-free) for decomp.dev.

## Validation (2026-10-01)
- A copy of the repo with no ROM and no `build/`: `check.py` (summary, `--diff`, `--asm`), permuter setup (prints a "no ROM cross-check" note), `check_all.py` (112/112) and `make objdiff-report` (1,704/1,976) all pass.
- Native mode: a bare `debian:trixie-slim` container ran `tools/setup_native.sh`, then `check_all.py` natively. Result in the log entry for this date.

## Still local-only
`make compare` (full-ROM SHA-1, which also covers data, the header and the link layout) and anything that reads game data (`tools/extract_cards.py`, `tools/disasm.py`, graphics/text tools). Run a full compare after merging batches of agent PRs. Strictly, `make compare` needs `assets/`, not the ROM: `make setup` extracts the data once, and a clean build still matched after the ROM was deleted (2026-10-02, [[assets]]). The assets are game data, so they stay local like the ROM.

> [!question] Open decision (the user's)
> Publishing `asm/` (disassembly derived from the game's code) is standard practice for public decomps (pret, zeldaret, decomp.dev projects), but it is derived work. The ROM and assets stay out regardless.

Related: [[decomp-workflow]], [[decomp-permuter]], [[objdiff]], [[agent-tooling]], [[rom-versions]].
