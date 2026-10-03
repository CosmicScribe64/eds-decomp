# Yu-Gi-Oh! The Eternal Duelist Soul decompilation

[![Build](https://github.com/CosmicScribe64/eds-decomp/actions/workflows/progress.yml/badge.svg)](https://github.com/CosmicScribe64/eds-decomp/actions/workflows/progress.yml)
[![Code](https://decomp.dev/CosmicScribe64/eds-decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/CosmicScribe64/eds-decomp)
[![Functions](https://decomp.dev/CosmicScribe64/eds-decomp.svg?mode=shield&measure=functions&label=Functions)](https://decomp.dev/CosmicScribe64/eds-decomp)

A matching decompilation of *Yu-Gi-Oh! The Eternal Duelist Soul* for the Game Boy Advance (USA, `AY5E`).
The C and assembly in this repository rebuild a ROM that is byte-for-byte identical to the original.

Only the USA release is covered. The Japanese release (*Yu-Gi-Oh! Duel Monsters 5 Expert 1*, `AY5J`) is
not a translation of this game: its duel engine and data model were reworked, it has 928 cards instead of
821, and it links a Mobile Adapter GB library the USA build lacks. A matching Japanese build would be a
separate project; [`wiki/rom/rom-versions.md`](wiki/rom/rom-versions.md) has the full comparison.

As of October 2026 the decompilation is complete: all 1,976 of the game's functions are written in
matching C, and the built ROM is byte-for-byte identical to the original. The work now is readability:
shared headers, harmonized symbol names and per-unit cleanup, all while keeping the match.

**This repository contains no game data.** You supply your own copy of the game, and a setup step extracts
its graphics, text, card data, fonts and sound into a local `assets/` folder that the build reads.

## Building

The toolchain runs in Docker, so Docker is the only thing you install.

1. Copy your dump of the USA release into the `roms/` folder. Either the `.gba` file or the `.zip` it came in
   works. The build needs the version with SHA-1 `510fbba212aca9bab95ea12f8fd933e62ee34dea`.
2. Build the toolchain image once:
   ```
   docker build -t eds-decomp docker/
   ```
3. Check the ROM and extract its data:
   ```
   tools/dr make setup
   ```
   If the file isn't the right one, setup says why. It recognizes the Japanese release, patched or trimmed
   dumps, and other games.
4. Build the ROM and compare it with the original:
   ```
   tools/dr make -j8 compare        # prints "eds.gba: OK" when it matches
   ```

After setup, the build reads only `assets/` and the source code, so it no longer needs the ROM file.

### The assets folder

`make setup` writes every byte of the game's data into `assets/`. Every asset converts to an editable
format and builds back byte-identical; the only raw bytes left are the Nintendo logo inside the cartridge
header, which the BIOS checks.

| What | Format |
|---|---|
| Card names and descriptions | `assets/cards/names.json`, `descriptions.json` |
| Card stats (ATK, DEF, level, type, attribute) | `assets/cards/stats.csv` |
| Card art, 821 images with their palettes | `assets/cards/art/NNN.png` (indexed, 64 colours) |
| Card passwords | `assets/cards/passwords.csv` |
| Dialogue and duelist names | `assets/text/dialogue.json`, `duelists.json` |
| Fonts (Latin and Japanese) | `assets/fonts/*.png` |
| Sound | WAV samples plus text-format music and sound-effect tracks in `assets/sound/` |
| Scene graphics and graphics banks | editable PNGs in `assets/gfx/` |
| Game tables (deck lists, effect handlers, pointers) | JSON in `assets/tables/` |

Edit a file and run `tools/dr make` to build a ROM with your change. `make compare` reports a mismatch once
you've changed something, which is expected. There are 83 assets in total; `config/assets.tsv` lists every
one with its address range, `tools/assets.py` does the conversions, and
[`wiki/tools/assets.md`](wiki/tools/assets.md) documents each format.

## Progress

```
tools/dr make -j8 objdiff-report    # per-function progress report (build/report.json)
```
The report doesn't need the ROM. CI generates it on every push (`.github/workflows/progress.yml`) in the format
[decomp.dev](https://decomp.dev) reads.

## Working on it

The code is split into units, listed in link order in `units.txt`. Each unit is `src/<unit>.c`, and every
unit is now matching C (no `INCLUDE_ASM` remains). The original disassembly stays in
`asm/nonmatching/` as the comparison target. Shared types and constants live in `include/` and
`include/constants/`; `wiki/concepts/shared-headers.md` explains how units use them and which local
views remain.

- `tools/dr python3 tools/check.py <unit> [--diff FUNC] [--norm]` compares one unit with its original bytes.
- `tools/dr python3 tools/check_all.py` checks every unit. CI runs the same check.
- `tools/dr python3 tools/permute.py <unit> <func> --run` runs
  [decomp-permuter](https://github.com/simonlindholm/decomp-permuter) on a near-miss draft.

The [`wiki/`](wiki/index.md) holds what we know about the game, the ROM and the toolchain. It opens in Obsidian.
Start with `wiki/overview.md` and `wiki/concepts/decomp-workflow.md`. Contributors, human or agent, follow
[`CLAUDE.md`](CLAUDE.md).

Game code is built with `old_agbcc -O2`. The sound driver uses `agbcc -O2 -fprologue-bugfix` and the SDK save
code `agbcc -O1`. All three come from [pret/agbcc](https://github.com/pret/agbcc).

### Without the ROM

You can decompile code without the game. The original machine code of every function is in the repository as
assembly (`asm/nonmatching/<unit>/<func>.s`), and the tools compare compiled C against it.

- Without `baserom.gba`, `check.py`, `check_all.py` and `permute.py` assemble the unit's original assembly at its
  ROM address and compare against that (`tools/target.py`). The bytes are the same: all 112 code units
  reassemble to the ROM exactly, which `tools/dr python3 tools/target.py --verify` confirms when a ROM is present.
- Symbols whose address can't be read from their name are listed in `config/symbols.txt`. Every full build
  regenerates it.
- Without Docker, `tools/setup_native.sh` installs the toolchain natively, and `tools/dr` then runs commands
  directly.

The full-ROM check (`make compare`) and any work that reads game data, such as graphics, text or tables,
still need the ROM, so they happen locally.

## AI assistance

Most of the decompilation and the wiki were written with AI coding agents (Claude Code, Codex and others)
directed by the maintainer. A function counts as decompiled only when its compiled code is byte-identical to
the original, and CI checks that on every change.

## License

The code and documentation in this repository are released under the [MIT License](LICENSE). That covers our
own work only. *Yu-Gi-Oh! The Eternal Duelist Soul* and all of its data belong to their respective rights
holders, and none of it is included here.
