---
title: Overview
type: overview
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# EDS decomp overview

The project is a byte-matching decompilation of *Yu-Gi-Oh! The Eternal Duelist Soul* (USA, `AY5E`). The Japanese release is titled *Duel Monsters 5 Expert 1*. See [[matching-decompilation]] for what "matching" means here.

## Current state (2026-10-01)
- **The build matches.** `tools/dr make compare` rebuilds `eds.gba` with SHA-1 `510fbba2…`, identical to the baserom. The toolchain runs in Docker, or natively through `tools/setup_native.sh` ([[toolchain]], [[rom-free-workflow]]).
- **Progress:** 1,769 of 1,976 functions (89.52%) are in matching source, covering 66.91% of code bytes, after workflow waves 1-2 on 2026-10-01. 207 functions still build from their original assembly. The session started at 1,707 functions (60.91% of code bytes); wave 1 reached 1,760 (89.07%, 64.85%) and wave 2 added nine more, including the three 4 KB deck-edit frame handlers. Per-function notes are on the unit pages and in [[log]]; the reusable compiler mechanics they uncovered are in [[matching-tricks#Register allocation priority and reload rotation]].
- **Compilers:** game code uses `old_agbcc -O2 -mthumb-interwork`, the Konami sound driver `agbcc -O2 -fprologue-bugfix`, and the SDK AgbSram library `agbcc -O1` ([[compiler-flags]]). libgcc and libc come straight from agbcc's own `libgcc.a` and `libc.a`, which match byte for byte.
- **Disassembly:** `tools/disasm.py` finds 1,976 functions in `0x08000228`–`0x0807EE98`. They are Thumb game code plus an ARM sound mixer at `0x0807EAD0`. The tool splits them into 112 code units of about 4 KiB, with one `.s` file per function under `asm/nonmatching/`. About 100 of agbcc's "far jump" `bl` instructions are recognized as branches inside a function.
- **Matching policy:** ordinary C is preferred. Byte-identical, behaviour-neutral fakematches, such as the permuter's redundant tests, register bindings and empty `asm` constraints, are accepted when marked `FAKEMATCH` and documented in [[matching-tricks]]. A whole-unit byte match is the acceptance test.
- **Largest remaining functions:** [[code-08044224]] (one 0x2514-byte function with a complete but nonmatching C draft), [[code-08046738]] and [[code-0800ab08]]. The deck-edit frame handlers (`sub_0806DBB0`, `sub_08070F18`, `sub_0806F934`; [[code-0806d51c]], [[code-0807093c]], [[code-0806ed44]]) matched in waves 1-2. The [[sound-driver]] has two Thumb fallbacks left; the ARM mixer is authored assembly ([[sound-mixer]]).
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
| `0x08080A20`–`0x08800000` | `.rodata` and assets: card bank at `0x0822C720` ([[cards]]), fonts at `0x081C0000` ([[font]]), graphics ([[graphics-formats]]) |

## The game
- **Program structure** ([[program-flow]]): the main loop calls `gMain.callback` once per frame. Scenes are "step runners" over NULL-terminated tables of step functions. `SetMainCallback` saves to SRAM on every scene change.
- **Cards** ([[cards]]): per-card tables use a 1-based alphabetical ID as their index, and stats are packed bitfields ([[card-table]]). Decks, packs and the effect table use a second numbering, the "card number".
- **Sound** ([[sound-engine]]): a custom Konami driver, not m4a.
- **Text** ([[text-system]]): English-only ASCII on top of the Japanese engine.

## Next milestones
1. Convert the remaining code to matching C, largest functions first ([[decomp-workflow]]).
2. Do the global rename pass with the names proposed in `config/names.txt` and on the unit pages. The convention is pret-style: `CamelCase` functions and `gCamelCase` globals. Semantic names are also a prerequisite for a second game version ([[rom-versions]]).
3. Replace `.incbin` data with real data files and extract assets, card tables first.
4. Finish the shared headers by migrating the remaining units, folding the local views back into the headers, and adding save data ([[shared-headers]]).
5. After USA reaches 100%, add the Japanese build to the same source ([[rom-versions]]).

See [[open-questions]] for everything that's still unresolved.
