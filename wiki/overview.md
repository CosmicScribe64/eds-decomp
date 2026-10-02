---
title: Overview
type: overview
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# EDS decomp overview

The project is a byte-matching decompilation of *Yu-Gi-Oh! The Eternal Duelist Soul* (USA, `AY5E`). The Japanese release is titled *Duel Monsters 5 Expert 1*. See [[matching-decompilation]] for what "matching" means here.

## Current state (2026-10-02)
- **The build matches.** `tools/dr make compare` rebuilds `eds.gba` with SHA-1 `510fbba2…`, identical to the baserom. The toolchain runs in Docker, or natively through `tools/setup_native.sh` ([[toolchain]], [[rom-free-workflow]]).
- **Progress:** **1,975 of 1,976 functions** are in matching source. The one left is `sub_08044224` ([[code-08044224]], 0x2514 bytes, a parked draft). It is 1.8% of the 0x7EC70 code bytes listed in `config/functions.tsv`. The other 111 C units have no `INCLUDE_ASM` left. The remaining assembly is authored: [[crt0]], the SWI stubs ([[bios-swi-stubs]]), the linker veneer and the ARM mixer ([[sound-mixer]]).
  - History: 1,769 functions (89.52%) after waves 1-2 on 2026-10-01; 1,910 (96.66%, 83.35% of code bytes by `tools/progress.py`) at the wave-3 checkpoint `f7c9206`; 1,912 at `33809ed`.
  - The remaining matches on 2026-10-02 are recorded in git history (`git log 33809ed..`). They include the giant `sub_0804FC4C`, the sound decoders `sub_0807D6B4` and `sub_0807DB58` ([[sound-driver]]), and several 1–2.4 KB functions written from scratch (`sub_08069FE4`, `sub_0804D320`, `sub_0803D57C`, `sub_0801401C`, …). Most of them are not yet written up on their unit pages.
  - Per-function notes are on the unit pages and in [[log]]. The reusable compiler mechanics are in [[matching-tricks]], especially [[matching-tricks#Register allocation priority and reload rotation]].
- **Assets: complete (2026-10-02).** All 83 data ranges of `config/assets.tsv` extract from the user's ROM into editable files and build back byte-identical. The formats are JSON, CSV, one-event-per-line sound text, PNG, WAV and JASC-PAL. Only the 156-byte Nintendo logo stays raw.
  - Six format plugins in `tools/assetfmt/` cover sound samples, sound sequences, scene graphics (with an exact reconstruction of the original LZSS compressor), graphics banks, game tables and code tables.
  - A fresh clone with only the ROM ran `make setup` and `make compare` (`eds.gba: OK`), and it still matched after the ROM was deleted.
  - Edits reach the ROM as long as each item fits its original slot. Relocatable data is future work. See [[assets]], [[sound-sequence-format]], [[scene-sets]].
- **Compilers:** game code uses `old_agbcc -O2 -mthumb-interwork`, the Konami sound driver `agbcc -O2 -fprologue-bugfix`, and the SDK AgbSram library `agbcc -O1` ([[compiler-flags]]). libgcc and libc come straight from agbcc's own `libgcc.a` and `libc.a`, which match byte for byte.
- **Disassembly:** `tools/disasm.py` finds 1,976 functions in `0x08000228`–`0x0807EE98`. They are Thumb game code plus an ARM sound mixer at `0x0807EAD0`. The tool splits them into 112 code units of about 4 KiB, with one `.s` file per function under `asm/nonmatching/`. About 100 of agbcc's "far jump" `bl` instructions are recognized as branches inside a function.
- **Matching policy:** ordinary C is preferred. Byte-identical, behaviour-neutral fakematches, such as the permuter's redundant tests, register bindings and empty `asm` constraints, are accepted when marked `FAKEMATCH` and documented in [[matching-tricks]]. A whole-unit byte match is the acceptance test.
- **Last function:** `sub_08044224` ([[code-08044224]], 0x2514 bytes) is the only `INCLUDE_ASM` left. Its parked draft is at score 338 (commit `2eafe08`; see [[code-08044224]] for the metric). The [[sound-driver]] is complete in C; the ARM mixer is authored assembly ([[sound-mixer]]).
- **Shared headers:** `include/main.h`, `include/duel.h`, `include/duel_ui.h` and `include/sound.h` hold the canonical `gMain`, duel and duel-screen layouts. About 30 units use them ([[shared-headers]], [[duel-engine]]).
- **Tooling:** `tools/check.py` and `tools/check_all.py` compare units with their original bytes, and work without the ROM ([[rom-free-workflow]]). The permuter, a semantic hill-climber over parked drafts and the agent setup are described in [[decomp-permuter]] and [[agent-tooling]]. `tools/wf.py` gives each agent a private per-function working copy of a unit, RTL dumps, and a locked merge back into `src/` ([[agent-tooling]]).
- **Hardware reference:** the GBA sound, DMA, timer, memory and interrupt sections of [[gbatek]] were checked against ROM accesses. The NDS, DSi and 3DS portions were not reviewed.

## Hand-written and SDK source
- `src/sdk/agb_sram.c` (AgbSram v1.12) and `src/sdk/libagbsyscall.s` (BIOS stubs): see [[agb-sram]] and [[bios-swi-stubs]].
- `src/sound_mixer_arm.s`: the hand-written ARM PCM mixer ([[sound-mixer]]).
- `asm/crt0.s`: startup and `IntrMain` ([[crt0]]).
- Game code: `src/code_*.c`, with one wiki page per unit. `src/sound_driver.c` holds the Konami driver ([[sound-driver]]).

## ROM layout (summary)
See [[rom-map]] for the full map.

| Range | Contents |
|---|---|
| `0x08000000`–`0x08000228` | Header, [[crt0]], `IntrMain` |
| `0x08000228`–`0x0807EAD0` | Game code (Thumb, agbcc -O2) |
| `0x0807EAD0`–`0x0807ECF8` | ARM sound mixer ([[sound-mixer]]); part of it is copied to IWRAM `0x03005A54` |
| `0x0807ECF8`–`0x0807EE98` | SDK: SWI stubs, AgbSram |
| `0x0807EE98`–`0x08080A18` | libgcc (including soft-float), then libc `memcpy`/`memset`/`strcpy` |
| `0x08080A18`–`0x08080A20` | Linker-generated Thumb-to-ARM veneer for the mixer |
| `0x08080A20`–`0x08800000` | `.rodata` and assets, all extracted to editable files ([[assets]]): sound at `0x08087FD0` ([[sound-engine]]), game tables, card bank at `0x0822C720` ([[cards]]), fonts at `0x081C0000` ([[font]]), graphics ([[graphics-formats]], [[scene-sets]]) |

## The game
- **Program structure** ([[program-flow]]): the main loop calls `gMain.callback` once per frame. Scenes are "step runners" over NULL-terminated tables of step functions. `SetMainCallback` saves to SRAM on every scene change.
- **Cards** ([[cards]]): per-card tables use a 1-based alphabetical ID as their index, and stats are packed bitfields ([[card-table]]). Decks, packs and the effect table use a second numbering, the "card number".
- **Sound** ([[sound-engine]]): a custom Konami driver, not m4a.
- **Text** ([[text-system]]): English-only ASCII on top of the Japanese engine.

## Next milestones
1. Match the last function, `sub_08044224` ([[code-08044224]]), and write up the 2026-10-02 matches on their unit pages.
2. Do the global rename pass with the names proposed in `config/names.txt` and on the unit pages. The convention is pret-style: `CamelCase` functions and `gCamelCase` globals. Semantic names are also a prerequisite for a second game version ([[rom-versions]]).
3. ~~Replace `.incbin` data with real data files and extract assets.~~ Done 2026-10-02 ([[assets]]). Next for data: a relocatable layout, so edited items can grow and move instead of fitting their original slots.
4. Finish the shared headers by migrating the remaining units, folding the local views back into the headers, and adding save data ([[shared-headers]]).
5. After USA reaches 100%, add the Japanese build to the same source ([[rom-versions]]).

See [[open-questions]] for everything that's still unresolved.
