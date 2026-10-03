---
title: Index
type: overview
status: solid
confidence: high
sources: []
updated: 2026-10-02
---
# Wiki Index

Start with [[overview]]. The decompilation is 100% matched since 2026-10-02: all 1,976 functions, the last being [[effect-target-collect-c]]. The schema and workflows live in `CLAUDE.md` at the repo root, and the build/decomp how-to is [[decomp-workflow]]. See the [[log]] for history.

## ROM
- [[rom-header]]: header fields, size, hashes, entry point. *verified*
- [[rom-map]]: segment map of the whole 8 MiB image (code, rodata, graphics, fonts, card bank). *verified (29 checks)*
- [[rom-versions]]: USA (`roms/base_eng.gba`) vs Japan AY5J (`roms/base_jp.gba`): same compiler (10/10 recompiled functions byte-identical) but reworked game logic; about 200 functions shared, 647 changed, 1290 JP-only (incl. a Mobile Adapter GB library); link order, RAM and asset deltas; staged plan for a JP build. *verified comparison, draft plan*
- [[save-type]]: AgbSram v1.12; 0x2170-byte save at SRAM `0x0E000000`, mirrored at `0x02011C20`. *verified*
- [[ram-map]]: EWRAM/IWRAM globals (`gMain` at `0x03000040`, save mirror, duel state, sound, link).
- [[gba-memory-map]]: GBA address regions and how ROM offsets map to addresses.

## Game
- [[duel-engine]]: duel state, player/zone layout, command runner and screen state (from the shared headers).
- [[game-overview]]: what EDS is, and the major systems.
- [[program-flow]]: boot → `GameInit` → `MainLoop`; the scene and step-runner system; scene table.
- [[cards]]: card model (1-based alphabetical IDs vs. classic "card numbers", kinds, types).
- [[sound-engine]]: custom Konami driver (PSG + 6 PCM voices, ARM mixer); samples, wave-RAM patterns, noise presets, pitch table. *verified data*
- [[text-system]]: ASCII text, Shift-JIS-capable renderer, `$`/`@` markup codes.

## Data
- [[card-name-table]]: 821 × `char[0x40]` English names at `0x0822C720`. *verified*
- [[card-table]]: packed `u32` stats at `0x08621DE0` (ATK/DEF/level/type/attribute bitfields). *verified*
- [[card-id-map]]: card ID ↔ card number tables. *verified*
- [[card-descriptions]]: 821 × `char[0x1E0]` descriptions at `0x082461A0`.
- [[card-art]]: 72×80 6bpp card art at `0x082A6500`, palettes at `0x08608360`.
- [[password-table]]: BCD passwords at `0x08623120`.
- [[duelist-table]]: 28 duelists at `0x08139F64`.
- [[deck-lists]]: opponent decks, alternate decks, initial-deck pools.
- [[booster-packs]]: pack info and contents, rarity thresholds.
- [[special-card-lists]]: AI priority lists (hypothesis), the 60 notable cards, the Forbidden/Limited list, fusion recipes.
- [[graphics-formats]]: image packs, sprite streams, Mode-4 bitmaps, raw tiles and maps; LZSS and its reconstructed compressor; 6bpp card art. *verified (round trip)*
- [[scene-sets]]: the 31 dialogue bust-up scenes (descriptor, set-to-character table, OBJ tiles, animation) and the dialogue box. *verified*
- [[sound-sequence-format]]: SE and BGM tables and bytecodes (every opcode), loop mechanism, driver lookup tables. *verified*
- [[font]]: eight 1bpp fonts at `0x081C0000` (3 Shift-JIS kanji, 5 CP1252 Latin).

## Functions
- [[crt0]]: SDK startup and the `IntrMain` IRQ dispatcher (ARM). *matching*
- [[agb-main]]: `AgbMain`, `GameInit`, `MainLoop`.
- [[interrupt-handlers]]: VBlank, Timer2, DMA1 (sound), serial.
- [[set-main-callback]]: scene switching (saves to SRAM first).
- [[frame-sync-update]]: per-frame BG/OAM copy, keys, sound, RNG.
- [[read-keys]], [[random]], [[fade-functions]], [[video-helpers]].
- [[save-game]]: SRAM save/verify with retries.
- [[sound-api]], [[sound-driver]], [[sound-mixer]]: game-side sound calls; Konami driver, 30/30 functions in matching C; ARM mixer at `0x0807EAD0`.
- [[main-menu]], [[title-screen]], [[license-sequence]], [[debug-menu]] (unused).
- [[card-data-functions]]: card stat/ID lookups.
- [[lzss-decompress]]: `LZSSDecompress`, the custom LZSS decoder.
- [[agb-sram]]: AgbSram v1.12 (`ReadSram`, `WriteSram`, `VerifySram`, ...). *matching*
- [[bios-swi-stubs]]: `CpuFastSet`, `CpuSet`, `Div`. *matching*
- Unit pages (per decomp unit; functions, match status, structs, tricks): [[bustup-scene-c]], [[bustup-runner-c]], [[campaign-select-c]], [[main-menu-c]], [[title-screen-c]], [[title-menu-c]], [[card-detail-c]], [[duel-card-lists-c]], [[duel-zones-c]], [[duel-piles-c]], [[card-stats-c]], [[duel-stat-queries-c]], [[duel-cmd-field-c]], [[duel-cmd-deck-c]], [[duel-cmd-piles-c]], [[duel-cmd-hand-c]], [[duel-cmd-status-c]], [[duel-cmd-moves-c]], [[duel-cmd-turn-c]], [[duel-cmd-presentation-c]], [[duel-cmd-screen-c]], [[duel-send-to-grave-c]], [[duel-field-moves-c]], [[duel-card-actions-c]], [[link-battle-c]], [[campaign-steps-c]], [[campaign-c]], [[duel-cmd-queue-c]], [[duel-setup-c]], [[duel-main-c]], [[duel-prompts-c]], [[duel-link-receive-c]], [[duel-field-view-c]], [[coin-toss-scene-c]], [[dice-scene-c]], [[destiny-board-scene-c]], [[turn-order-scene-c]], [[turn-order-steps-c]], [[card-list-viewer-c]], [[effect-checks-c]], [[effect-activation-c]], [[effect-prepare1-c]], [[effect-prepare2-c]], [[effect-prepare3-c]], [[effect-resolve1-c]], [[effect-resolve2-c]], [[effect-resolve3-c]], [[effect-resolve4-c]], [[effect-resolve5-c]], [[effect-resolve6-c]], [[effect-resolve7-c]], [[effect-resolve8-c]], [[effect-resolve9-c]], [[effect-resolve10-c]], [[effect-resolve11-c]], [[effect-fusion-c]], [[effect-targets1-c]], [[effect-targets2-c]], [[effect-targets3-c]], [[effect-targets4-c]], [[duel-response-c]], [[duel-ritual-c]], [[effect-target-collect-c]], [[effect-hooks-c]], [[card-command-menu-c]], [[card-menu-input-c]], [[battle-phase1-c]], [[battle-phase2-c]], [[battle-phase3-c]], [[duel-phases-c]], [[duel-turn-end-c]], [[duel-prompt-handlers-c]], [[duel-cursor-c]], [[summon-checks-c]], [[summon-action-c]], [[summon-builders-c]], [[ai-picks-c]], [[ai-summon-c]], [[ai-deck-c]], [[ai-turn-steps-c]], [[ai-steps-c]], [[ai-strategy-c]], [[duel-card-anim-c]], [[battle-scene-c]], [[duel-info-bar-c]], [[duel-field-screen-c]], [[card-canvas-c]], [[booster-pack-c]], [[booster-get-pack-c]], [[deck-edit-panel-c]], [[deck-edit-widgets-c]], [[deck-edit-list-c]], [[deck-edit-cards-c]], [[deck-edit-filter-c]], [[deck-edit-filter-steps-c]], [[deck-edit-stats-c]], [[deck-edit-view-c]], [[deck-edit-c]], [[deck-edit-prohibit-c]], [[text-bg-c]], [[bg-image-c]], [[text-canvas-c]], [[main-c]], [[sprite-c]], [[collection-c]], [[text-render-c]], [[bitmap-text-c]], [[gfx-util-c]], [[link-sio-c]], [[password-trade-c]].

## Concepts
- [[decomp-workflow]]: build, repo layout, per-unit decomp loop, conventions, agbcc matching tips.
- [[matching-tricks]]: combined matching catalog by discrepancy, with verified examples, initialized hints, failed variants, ABI constraints and tool-validation limits.
- [[matching-decompilation]]: the goal and the workflow.
- [[nintendo-sdk-libraries]]: SDK code linked into EDS (AgbSram, SWI stubs, libgcc, libc; no m4a).
- [[thumb-and-arm]]: instruction sets and interworking.
- [[compiler-flags]]: exact agbcc flags per code region.

## Tools
- [[assets]]: `make setup` extracts all 83 data ranges from your ROM into editable files (JSON, CSV, text, PNG, WAV, PAL) and the build reads them back byte-identical; plugin mechanism, every format family, edit constraints. *verified*
- [[rom-free-workflow]]: checking and permuting without the ROM (target bytes from the original assembly, `config/symbols.txt`, native setup, CI gate) for GitHub agents. *verified*
- [[agent-tooling]]: current lead-only continuation and no-restart policy; historical parallel tooling, handoffs, matching helpers and verification.
- [[toolchain]]: Docker image, disassembler, build, check tools.
- [[agbcc]]: the compiler (confirmed).
- [[decomp-permuter]]: random-rewrite search for near-miss drafts (`tools/permute.py`).
- [[regoracle]]: register-allocation oracle (`tools/regoracle.py` plus a tracing agbcc patch). It shows which pseudos differ from the ROM's registers and why, then inverse-solves the allocation order and checks the answer by recompiling. *verified (`--verify-compiler` 112/112)*
- [[function-pointer-tables]]: 88 tables of function pointers in ROM data (`tools/fnptr_tables.py`).
- [[shared-headers]]: canonical `include/main.h` and `include/duel.h`, and how units migrate to them.
- [[m2c]]: ARM decompiler for first drafts (`tools/m2c_draft.py`).
- [[objdiff]]: progress report in the decomp.dev format (`make objdiff-report`, CI workflow) and the object differ.
- [[xref]]: static cross-reference database from `build/eds.elf` (`tools/xref.py`): function cards, global users, strings, call graphs, units, subsystems, address lookup. *verified (selftest 16/16)*
- [[jpmap]]: USA-to-Japan mapper (`tools/jpmap.py`): every USA function, RAM address and asset range paired with the AY5J ROM, link-order runs, recompile test at JP addresses; outputs in `build/jp/`. *verified (compile test 10/10)*
- [[emulator]]: headless mGBA 0.10.5 harness (`tools/emu.py`, image `eds-emu`): traces, watchpoints, breakpoints, Lua, GDB, ROM comparison, and a savestate library from title to mid-duel. *verified (selftest)*

## Sources
- [[rom-analysis]]: facts checked directly against the baserom.
- [[web-card-references]]: public card lists and passwords used to cross-check the card data.
- [[general-knowledge]]: background knowledge from Claude; medium confidence.
- [[gbatek]]: GBA memory, sound, DMA, timer and interrupt sections ingested and checked against EDS.

## Questions
- [[open-questions]]: unresolved questions by area.
- [[decomp-pace-comparison]]: how fast other GBA decomps went, and what EDS should change.
