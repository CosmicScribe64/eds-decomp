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
- **Progress: 100% matched (2026-10-02, commit `d77fcef`).** All **1,976 of 1,976 functions** are in matching source, and `tools/progress.py` counts 0x7EC70/0x7EC70 code bytes (100.00%). `check_all.py` passes 112/112 units and `make compare` prints `eds.gba: OK`. No C unit has an `INCLUDE_ASM` left. 1,970 functions are C; the other 6 are authored assembly, the SWI stubs ([[bios-swi-stubs]]) and the ARM mixer ([[sound-mixer]]). [[crt0]] and the linker veneer, outside the function list, are authored assembly too. The last function was `CollectEffectTargets` ([[effect-target-collect-c]], 0x2514 bytes, the largest in the game).
  - History: the project started on 2026-09-29. 1,769 functions (89.52%) after waves 1-2 on 2026-10-01; 1,910 (96.66%, 83.35% of code bytes by `tools/progress.py`) at the wave-3 checkpoint `f7c9206`; 1,912 at `33809ed`; 1,975 (all but `CollectEffectTargets`) at `350cb09`; 1,976 (100%) at `d77fcef`, all on 2026-10-02.
  - The remaining matches on 2026-10-02 are recorded in git history (`git log 33809ed..`). They include the giant `DuelPhase_Standby`, the sound decoders `SoundSeTrackTick` and `SoundSequencerTick` ([[sound-driver]]), and several 1–2.4 KB functions written from scratch (`ListFilter_Update`, `BattleStage_DestroyMonsters`, `EffectPolymerizationResolve`, `DuelCmd_TurnEnd`, …). Most of them are not yet written up on their unit pages.
  - Per-function notes are on the unit pages and in [[log]]. The reusable compiler mechanics are in [[matching-tricks]], especially [[matching-tricks#Register allocation priority and reload rotation]].
- **Assets: complete (2026-10-02).** All 83 data ranges of `config/assets.tsv` extract from the user's ROM into editable files and build back byte-identical. The formats are JSON, CSV, one-event-per-line sound text, PNG, WAV and JASC-PAL. Only the 156-byte Nintendo logo stays raw.
  - Six format plugins in `tools/assetfmt/` cover sound samples, sound sequences, scene graphics (with an exact reconstruction of the original LZSS compressor), graphics banks, game tables and code tables.
  - A fresh clone with only the ROM ran `make setup` and `make compare` (`eds.gba: OK`), and it still matched after the ROM was deleted.
  - Edits reach the ROM as long as each item fits its original slot. Relocatable data is future work. See [[assets]], [[sound-sequence-format]], [[scene-sets]].
- **Compilers:** game code uses `old_agbcc -O2 -mthumb-interwork`, the Konami sound driver `agbcc -O2 -fprologue-bugfix`, and the SDK AgbSram library `agbcc -O1` ([[compiler-flags]]). libgcc and libc come straight from agbcc's own `libgcc.a` and `libc.a`, which match byte for byte.
- **Disassembly:** `tools/disasm.py` finds 1,976 functions in `0x08000228`–`0x0807EE98`. They are Thumb game code plus an ARM sound mixer at `0x0807EAD0`. The tool splits them into 112 code units of about 4 KiB, with one `.s` file per function under `asm/nonmatching/`. About 100 of agbcc's "far jump" `bl` instructions are recognized as branches inside a function.
- **Matching policy:** ordinary C is preferred. Byte-identical, behaviour-neutral fakematches, such as the permuter's redundant tests, register bindings and empty `asm` constraints, are accepted when marked `FAKEMATCH` and documented in [[matching-tricks]]. A whole-unit byte match is the acceptance test.
- **Last function (matched 2026-10-02):** `CollectEffectTargets` ([[effect-target-collect-c]]), a 9.5 KB target-filter switch. Its parked draft stood at wf score 338 (commit `2eafe08`). One round of a region-split workflow then matched it: six workers each fixed a group of case regions on private copies, and an integrator merged their patches (338 → 254 → 178 → 70 → 36 → 16 → 0). The fixes were loop.c movable thresholds, address grouping and allocation priorities. FAKEMATCH forms: empty `asm("")` loop padding, three dead stores, two `register ... asm("r0")` pins and an `asm` clobber. A new tool, [[regoracle]], helped: a patched old_agbcc traces every register-allocation decision, and the tool inverse-solves the allocation order against the ROM.
- **Shared headers:** `include/main.h`, `include/duel.h`, `include/duel_ui.h` and `include/sound.h` hold the canonical `gMain`, duel and duel-screen layouts. About 30 units use them ([[shared-headers]], [[duel-engine]]).
- **Tooling:** `tools/check.py` and `tools/check_all.py` compare units with their original bytes, and work without the ROM ([[rom-free-workflow]]). The permuter, a semantic hill-climber over parked drafts and the agent setup are described in [[decomp-permuter]] and [[agent-tooling]]. `tools/wf.py` gives each agent a private per-function working copy of a unit, RTL dumps, and a locked merge back into `src/` ([[agent-tooling]]). For the readability pass (2026-10-02), `tools/xref.py` gives static cross-references from `build/eds.elf` ([[xref]]), and `tools/emu.py` runs the game headless in mGBA, with traces, watchpoints and a savestate library ([[emulator]]). [[regoracle]] (2026-10-02) explains register-allocation differences from a traced compiler and checks its proposed fixes by recompiling.
- **Japanese ROM mapped (2026-10-02; JP build work paused by the user, see milestone 6):** [[jpmap]] paired every USA function with the Japanese release (AY5J). The compiler is the same (10 of 10 recompiled USA functions are byte-identical at their JP addresses), but JP is not a localisation. Its duel engine uses a different card and player data model and 928 cards, and it links a 44 KB Mobile Adapter GB library. Only 199 USA functions are identical or differ only in constants. 647 have a changed JP counterpart, 1130 have none, and JP has 1290 functions of its own ([[rom-versions]]).
- **Hardware reference:** the GBA sound, DMA, timer, memory and interrupt sections of [[gbatek]] were checked against ROM accesses. The NDS, DSi and 3DS portions were not reviewed.

## Hand-written and SDK source
- `src/sdk/agb_sram.c` (AgbSram v1.12) and `src/sdk/libagbsyscall.s` (BIOS stubs): see [[agb-sram]] and [[bios-swi-stubs]].
- `src/sound_mixer_arm.s`: the hand-written ARM PCM mixer ([[sound-mixer]]).
- `asm/crt0.s`: startup and `IntrMain` ([[crt0]]).
- Game code: C units `src/<unit>.c` named after what they do (`src/duel_zones.c`, `src/text_render.c`, ...; renamed from `src/code_<addr>.c` in commit 45642ef, map in `build/readability/unit_names.tsv`), with one wiki page per unit (`wiki/functions/<unit>-c.md`). `src/sound_driver.c` holds the Konami driver ([[sound-driver]]).

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
1. ~~Match the last function, `CollectEffectTargets`.~~ Done 2026-10-02 (commit `d77fcef`, [[effect-target-collect-c]]): the USA decompilation is 100% matched. Still to do: write up the other 2026-10-02 matches on their unit pages. Many unit pages still describe a function as `INCLUDE_ASM` or nonmatching; that text predates the match, and `src/` is authoritative.
2. **Readability pass (current work).** Do the global rename with the names proposed in `config/names.txt` and on the unit pages. The convention is pret-style: `CamelCase` functions and `gCamelCase` globals. Names are being proposed now, using evidence from [[xref]] and the [[emulator]]. Semantic names are also a prerequisite for a second game version ([[rom-versions]]).
3. **FAKEMATCH cleanup.** Where ordinary C can replace a FAKEMATCH form, replace it, one unit at a time, keeping each unit matching. The word FAKEMATCH appears on 519 lines in 93 source files (comments; one site can span several lines). The backlog file `build/wf/CLEANUP.md` (local) lists two items:
   - `DrawHandCards` reaches a field through a `struct DuelGlobals` cast with a wrong comment; plain `gDuel.f1B2C_0` also matches.
   - The local `struct PlayerState` in `duel_field_screen` may be 0xDC4 bytes rather than 0xD64. The `DrawHandCards` agent reported this; it is not yet checked. Fix it with the rename or the header work.

   Found while writing up the last match:
   - The `asm volatile` clobber in `CollectEffectTargets` case 0x2F has no `/* FAKEMATCH */` comment.
   - `src/duel_cmd_deck.c` lines 403–457 still hold a dead `#if 0 /* NONMATCHING */` draft of the now-matched `DuelCmd_BanishTopDeckCards`.
   - `src/effect_resolve8.c:83` calls `EffectTributeOpponentMonsterResolve` a non-matching draft, which it no longer is.

   `CollectEffectTargets`'s loop padding, dead stores and r0 pins are candidates too ([[effect-target-collect-c#FAKEMATCH forms in the final source]]).
4. ~~Replace `.incbin` data with real data files and extract assets.~~ Done 2026-10-02 ([[assets]]). Next for data: a relocatable layout, so edited items can grow and move instead of fitting their original slots.
5. Finish the shared headers by migrating the remaining units, folding the local views back into the headers, and adding save data ([[shared-headers]]).
6. **Japanese build: paused by the user (2026-10-02).** The plan stays on [[rom-versions]], to resume after the rename pass. It is a separate effort, not a re-target of the USA source. The shared core is about 200 functions, about 650 more can start from their USA C as templates, and about 1300 JP-only functions, including the Mobile Adapter GB library, need a new decompilation. Version plumbing, an all-assembly byte-matching JP build and a raw JP asset manifest do not depend on the rename. Sharing the C needs names with per-version addresses: first a scoped rename of about 320 symbols for the shared core, then the global rename (milestone 2).

See [[open-questions]] for everything that's still unresolved.
