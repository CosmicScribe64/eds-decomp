---
title: Index
type: overview
status: solid
confidence: high
sources: []
updated: 2026-10-02
---
# Wiki Index

Start with [[overview]]. The schema and workflows live in `CLAUDE.md` at the repo root, and the build/decomp how-to is [[decomp-workflow]]. See the [[log]] for history.

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
- [[sound-api]], [[sound-driver]], [[sound-mixer]]: game-side sound calls; Konami driver matching C and remaining functions; ARM mixer at `0x0807EAD0`.
- [[main-menu]], [[title-screen]], [[license-sequence]], [[debug-menu]] (unused).
- [[card-data-functions]]: card stat/ID lookups.
- [[lzss-decompress]]: `sub_0807A1A8`, the custom LZSS decoder.
- [[agb-sram]]: AgbSram v1.12 (`ReadSram`, `WriteSram`, `VerifySram`, ...). *matching*
- [[bios-swi-stubs]]: `CpuFastSet`, `CpuSet`, `Div`. *matching*
- Unit pages (per decomp unit; functions, match status, structs, tricks): [[code-08000228]], [[code-08001364]], [[code-08002388]], [[code-080034b8]], [[code-080044e4]], [[code-08005500]], [[code-08006878]], [[code-08007994]], [[code-08008a1c]], [[code-08009a68]], [[code-0800ab08]], [[code-0800c894]], [[code-0800d8a4]], [[code-0800eaa8]], [[code-0800fb10]], [[code-08010bdc]], [[code-08011be0]], [[code-08012c4c]], [[code-08013cdc]], [[code-080150dc]], [[code-080162c4]], [[code-08017314]], [[code-080184d8]], [[code-08019554]], [[code-0801a7b4]], [[code-0801bcfc]], [[code-0801ce68]], [[code-0801e260]], [[code-0801f454]], [[code-08020af4]], [[code-08021cc8]], [[code-08022d5c]], [[code-0802408c]], [[code-08025108]], [[code-08026124]], [[code-08027580]], [[code-08028684]], [[code-08029750]], [[code-0802aac0]], [[code-0802bad0]], [[code-0802cae8]], [[code-0802db30]], [[code-0802eb58]], [[code-0802fb64]], [[code-08030b88]], [[code-08031bc8]], [[code-08032cb0]], [[code-08033dac]], [[code-08035198]], [[code-080361d0]], [[code-0803732c]], [[code-080383f0]], [[code-08039638]], [[code-0803a654]], [[code-0803b670]], [[code-0803c838]], [[code-0803dd7c]], [[code-0803edc4]], [[code-0803fe70]], [[code-08040ebc]], [[code-08041f9c]], [[code-080431e4]], [[code-08044224]], [[code-08046738]], [[code-08048fe0]], [[code-0804a008]], [[code-0804b640]], [[code-0804cb58]], [[code-0804db6c]], [[code-0804eff0]], [[code-08050a70]], [[code-08051a9c]], [[code-08052b78]], [[code-08053e58]], [[code-08054e7c]], [[code-08055eb0]], [[code-08056ecc]], [[code-08057ee0]], [[code-080590e4]], [[code-0805a30c]], [[code-0805b3f4]], [[code-0805c508]], [[code-0805d58c]], [[code-0805e788]], [[code-0805f96c]], [[code-080609c4]], [[code-080619e8]], [[code-080629f0]], [[code-08063a28]], [[code-08064af0]], [[code-08065e6c]], [[code-0806704c]], [[code-08068180]], [[code-08069284]], [[code-0806a92c]], [[code-0806c4e4]], [[code-0806d51c]], [[code-0806ed44]], [[code-0807093c]], [[code-08071f40]], [[code-08072fac]], [[code-080740bc]], [[code-080750e0]], [[code-08076144]], [[code-0807717c]], [[code-080784e4]], [[code-0807960c]], [[code-0807a6ac]], [[code-0807b6b8]], [[code-0807c7c8]].

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
