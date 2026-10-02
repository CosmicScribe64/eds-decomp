---
title: Log
type: overview
status: solid
confidence: high
sources: []
updated: 2026-09-29
---
# Log

## [2026-09-29] setup | Wiki created
- Created the schema (`CLAUDE.md`), the directory layout, templates, and `scripts/wiki_lint.py`.
- Analyzed the baserom header and hashes → [[rom-header]].
- Found the `SRAM_V112` library tag → [[save-type]], [[nintendo-sdk-libraries]].
- Found the alphabetical card name table (820 × 0x40 at `ROM+0x22C760`) → [[card-name-table]].
- Searched for Blue-Eyes ATK/DEF as u16 (no hits) → [[card-table]].
- Seeded [[overview]], [[game-overview]], concept and tool pages, and [[open-questions]].

## [2026-09-29] setup | Docker toolchain and agbcc confirmed
- Built the `eds-decomp` Docker image (agbcc, binutils 2.44, capstone) and added the `tools/dr` runner → [[toolchain]].
- agbcc's `libgcc.a`/`libc.a` members match the ROM byte for byte (`_call_via_rX` through `strcpy`, `0x0807EE98`–`0x08080A18`), which confirms agbcc → [[agbcc]], [[nintendo-sdk-libraries]].

## [2026-09-29] progress | Matching asm build
- `tools/disasm.py`: 1,976 functions. Recognises agbcc far-jump `bl`s and jump tables, and rejects data-seeded fake functions. Output: `asm/nonmatching/<unit>/<func>.s`, `data/*.s`, `units.txt`.
- The build links agbcc's libgcc/libc in the original member order (with `_dvmd_tls` after `_divsi3`); the libgcc `.bss` is pinned at `0x03005B38`.
- `make compare` → **eds.gba: OK** (SHA-1 `510fbba2…`) → [[overview]], [[decomp-workflow]].

## [2026-09-29] ingest | Research agents (SDK, card data, program flow, assets)
- SDK: AgbSram v1.12 matches as C (`-O1`); BIOS stubs, crt0 and IntrMain match. Now `src/sdk/` → [[agb-sram]], [[bios-swi-stubs]], [[crt0]], [[save-type]], [[nintendo-sdk-libraries]].
- Card data: 13 per-card tables share one alphabetical ID; stats bitfield at `0x08621DE0` → [[card-table]], [[card-id-map]], [[card-name-table]], [[card-descriptions]], [[card-art]], [[password-table]], [[duelist-table]], [[deck-lists]], [[booster-packs]], [[special-card-lists]], [[cards]], [[card-data-functions]], [[web-card-references]].
- Program flow: boot, main loop, scenes, IRQs, sound engine, RAM map → [[program-flow]], [[ram-map]], [[sound-engine]], [[agb-main]], [[interrupt-handlers]], [[set-main-callback]], [[frame-sync-update]], [[read-keys]], [[random]], [[fade-functions]], [[save-game]], [[sound-api]], [[sound-mixer]], [[main-menu]], [[title-screen]], [[license-sequence]], [[debug-menu]], [[video-helpers]]. Proposed names are in `staging/names.txt`.
- Assets: ROM segment map, text system, fonts, LZSS, graphics formats → [[rom-map]], [[text-system]], [[font]], [[graphics-formats]], [[lzss-decompress]].
- Contradictions resolved: `.rodata` starts at `0x08080A20`, not `0x08080A24`; the LZSS blob claimed at `0x08624CF4` is really a card-number map entry.

## [2026-09-29] progress | C workflow validated; decomp wave 1
- First C functions match (`sub_08000838`, `sub_0800093C`, `sub_08000880`). Found that `-fprologue-bugfix` is required → [[decomp-workflow]], [[compiler-flags]].
- `tools/check.py` (per-unit, concurrent-safe) and `tools/mkunit.py`; per-unit flags in `config/cflags.txt`.
- Launched 12 decomp agents on units `code_08000228`…`code_0800EAA8`.
- Updated [[index]], [[overview]], [[toolchain]], [[open-questions]].

## [2026-09-29] progress | Compiler correction: old_agbcc for game code
- Compiler survey (53 matched functions in `staging/compiler_tests/`) → game code is `old_agbcc -O2`, the sound driver `0x0807D3D0`–`0x0807EACF` is `agbcc -O2 -fprologue-bugfix`, and AgbSram is `-O1` → [[compiler-flags]], [[agbcc]].
- Makefile, `check.py` and `config/cflags.txt` now pick the compiler per unit. Added a `sound_driver` split at `0x0807D3D0`. The infra build still matches.
- Resolved the contradiction on [[decomp-workflow]]. Running agents were notified.

## [2026-09-29] progress | Wave interrupted by usage limit; resumed with 5 agents
- All 20 unit agents stopped at the usage limit. Fully matching: `code_08001364`, `code_08002388`, `code_080034B8`, `code_0800C894`, `code_080184D8`. The other 15 units were left mid-edit.
- Full `make compare` with those 15 forced back to asm (`ASM_UNITS=…`) → OK. Progress: 267/1,976 functions (13.5%) in C, counting in-progress units.
- Relaunched as 5 agents × 3 units, with an "interruptible" rule added to `staging/AGENT_BRIEF.md` (keep the unit matching after each function).

## [2026-09-30] progress | Waves with 5 agents (checkpoint at usage limit)
- Verified MATCH (check.py) and returned: `code_08000228`, `code_080044E4`, `code_08005500`, `code_08006878`, `code_08007994`, `code_08008A1C`, `code_0800FB10`, `code_08010BDC`, `code_08011BE0`, `code_08012C4C`, `code_080162C4`, `code_08013CDC`, `code_0801BCFC`, `code_0801CE68`, `code_0801E260`, `code_0802408C`, `code_08025108`, `code_08027580`, `code_08021CC8`, `code_0801F454`, `code_08020AF4`. Unit pages under `wiki/functions/code-*.md`.
- Fixed `tools/check.py`: it cleared bit 0 on data symbols, so the odd-addressed `gUnk_08082703` was linked wrong.
- Still with agents at the checkpoint: `code_08009A68`/`0800D8A4`/`0800EAA8` (repair), `code_08017314`/`08019554`/`0801A7B4`, `code_08026124`/`08028684`/`08029750`, `code_0802AAC0`/`0802BAD0`/`0802CAE8`.
- Returned and verified MATCH: `code_08017314`, `code_08019554`, `code_0801A7B4` (49/61 functions in C).

## [2026-09-30] progress | Resumed after usage limit: 3 Sonnet agents
- The previous wave was cut off. Only `code_08028684` and `code_0802BAD0` were left non-matching; every other `src/` unit still MATCHes.
- Full `make compare` with the in-progress units forced to asm → OK.
- The user asked for at most 3 concurrent agents, on Sonnet. Launched: repair `code_08028684`→`code_08029750`; repair `code_0802BAD0`→`code_0802CAE8`; `code_0800D8A4`→`code_0802DB30`.
- Returned and verified MATCH: `code_08028684`, `code_08029750`, `code_0800D8A4`, `code_0802DB30`, `code_0802BAD0`, `code_0802CAE8`, `code_08030B88`, `code_08031BC8`, `code_0802EB58`, `code_0802FB64`. Open question filed on possible `-fno-gcse` functions.
- Returned and verified MATCH: `code_08035198`, `code_080361D0`, `code_0803732C`, `code_080383F0`, `code_08032CB0`, `code_08033DAC`, `code_08039638`, `code_0803A654`, `code_0803B670`, `code_0803C838`, `code_0803DD7C`, `code_0803EDC4`, `code_08041F9C`, `code_080431E4`, `code_0803FE70`, `code_08040EBC`, `code_08046738`, `code_08048FE0`. Progress: 909/1,976 functions (46.0%), 29.0% of code bytes. `code_08044224` (one 0x2514-byte function) is deferred.

## [2026-09-30] progress | Full-ROM checkpoint at 60%
- Full `make compare` with 83 game units linked from C (only the two in-progress units forced to asm) → **eds.gba: OK**.
- Newly verified units: `code_0804A008`, `code_0804B640`, `code_0804CB58`, `code_0804DB6C`, `code_0804EFF0`, `code_08050A70`, `code_080750E0`, `code_0807A6AC`, `code_0807717C`, `code_08063A28`, `code_080740BC`, `code_08076144`, `code_0807960C`, `code_0807B6B8`, `code_080784E4`, `code_080609C4`.
- Progress: 1,184/1,976 functions (59.9%), 33.8% of code bytes. The remaining untouched units are now scheduled by function density (most small functions first).

## [2026-09-30] progress | Mixed Claude / Codex / GLM agents
- The user enabled Codex CLI agents (`gpt-6.1-sol`, sandboxed with Docker socket access) and sandboxed OpenCode GLM-5.3-flash agents (`tools/glm-agent`, Docker image `eds-opencode`). Task instructions are in `staging/CODEX_TASK.md` and `staging/GLM_TASK.md`.
- Verified MATCH: `code_08057EE0`, `code_08069284`, `code_0805B3F4`, `code_0805C508` (Claude); `code_0806A92C`, `code_0806C4E4` (Codex).
- Progress: 1,360+/1,976 functions (≈69%). The progress map (`tools/mkmap.py`) now also has a Whole ROM view built from [[rom-map]].
- `code_0806D51C` (GLM): the agent exited mid-edit and left the unit non-matching. The coordinator parked `sub_0806D644` under `#if 0` → MATCH, 3/5 in C. Stub page [[code-0806d51c]] written.
- Codex giants: `code_08022D5C` 1/2 in C, `code_0800AB08` 2/3 in C. Both big functions have full drafts.
- `code_08071F40` cleanup (Space Bunny): the agent exited leaving a half-edited `sub_080725B0` that didn't compile. The coordinator parked it → MATCH; net 0 drafts converted.

## [2026-09-30] progress | Codex cleanup of code_08046738 (+2)
- `sub_08046738` and `sub_08046CB0` matched: the trick was a separate symbol-backed base for the duel zones. 16/23 functions are now in C; the coordinator checked the unit and it still matches (unit bytes MATCH).
- Pages touched: `wiki/functions/code-08046738.md` (by Codex), `staging/ORCHESTRATION.md`.

## [2026-09-30] progress | Codex cleanup of code_08009A68 (+6)
- Six drafts converted: `sub_08009C08`, `sub_08009DEC`, `sub_08009E4C`, `sub_08009FC8`, `sub_0800A004`, `sub_0800A0A8`. The unit now has 23/29 functions in C, and the coordinator re-checked it (unit bytes MATCH).
- The fixes were inline accessors, narrow helper arguments, and loading the hand count before computing the slot pointer.
- Pages touched: `wiki/functions/code-08009a68.md` (by Codex), `staging/ORCHESTRATION.md`.

## [2026-09-30] progress | DeepSeek cleanup of code_080784E4 (+4)
- Four drafts converted: `sub_08078C48`, `sub_08078ED4`, `sub_08078FD4`, `sub_08078534`. The unit now has 19/25 functions in C, and the coordinator re-checked it (unit bytes MATCH).
- The tricks were a `u16` mask local declared before the loop, and an `int zero = 0` local that keeps the zero in r8.
- DeepSeek has moved on to cleaning up `code_08063A28`.

## [2026-09-30] setup | decomp-permuter
- Added decomp-permuter to the Docker image (`docker/Dockerfile`) and rebuilt `eds-decomp` and `eds-opencode`.
- New `tools/permute.py`, which turns an `#if 0` draft into a permuter job. Target and candidates are both linked at the function's ROM address, so the comparison is on final bytes.
- First run: `sub_0802BB40` (`code_0802BAD0`) went from score 840 to 0 in under 5 minutes, with the fakematch `ret = zone = 1;`. The unit still matches.
- Pages touched: new [[decomp-permuter]], `index.md`, `decomp-workflow.md` (new section "When a draft won't match"), `staging/AGENT_BRIEF.md`, `staging/GLM_TASK.md`, `staging/CODEX_TASK.md`.

## [2026-09-30] progress | DeepSeek code_08063A28 (+2); permuter matches in code_08068180 (+2)
- DeepSeek converted `sub_08063A28` and `sub_08064604`. The unit now has 25/32 functions in C, and the coordinator re-checked it (unit bytes MATCH). DeepSeek has moved on to cleaning up `code_080740BC`, now with the permuter.
- `sub_08068FBC` and `sub_08068FEC` were matched by the permuter batch. Its results were tidied by hand into a single 64-bit temp trick. `code_08068180` still matches.
- Pages touched: [[decomp-permuter]] (results and known fakematch patterns), `staging/ORCHESTRATION.md`.
- Also: `sub_080650D4` (`code_08064AF0`) is a fakematch. It needs a non-void return type with no `return` statement, and it is labelled as a fakematch. `sub_080750E0` was deliberately not applied, because the permuter's match declares a u16 parameter as `unsigned long long`. `tools/permute.py` now refuses to overwrite a score-0 output without `--fresh`.

## [2026-09-30] setup | Switched to DeepSeek only; repair and watch tools
- Per the user, the GLM and Space Bunny agents were stopped. `code_08030B88` and `code_08072FAC` were left matching.
- New `tools/repair.py`: it parks broken functions back to INCLUDE_ASM until the unit matches. It repaired `code_0807960C` (`sub_0807A2EC`), `code_0802FB64` (`sub_08030B4C`) and `code_0805A30C` (`sub_0805A8E8`).
- New `tools/watch_agents.sh`: it exits when an agent container finishes.
- 10 DeepSeek agents are now running cleanup, using the permuter. Pages touched: `staging/ORCHESTRATION.md`.

## [2026-09-30] setup | Renamed the OpenCode launcher
- `tools/glm-agent` became `tools/opencode-agent`, and its default model is now DeepSeek v4.1 flash (`--model` or `OPENCODE_MODEL` override it). `staging/GLM_TASK.md` became `staging/OPENCODE_TASK.md`; the old name is kept as a symlink until the running agents finish. The permission rule in `.claude/settings.local.json` was updated to match.

## [2026-09-30] setup | objdiff progress report (decomp.dev format)
- objdiff-cli is now in the Docker image. New: `tools/mkobjdiff.py` (objdiff.json), `tools/objdiff_resolve.py`, `make objdiff-report`, and `.github/workflows/progress.yml`. `global.h` gained an `OBJDIFF_BASE` mode for `INCLUDE_ASM`.
- `.size` now comes before the trailing alignment padding: changed in `disasm.py` and in 340 asm files. The full `make compare` is still OK.
- The report gives 71.5% of functions and 40.6% of code, which matches `progress.py`. It runs without the baserom.
- Fixed a stale `#if 1 ... #else INCLUDE_ASM` wrapper around `sub_080512F0` in `code_08050A70`, which still matches.
- Pages touched: [[objdiff]] (rewritten), `staging/OPENCODE_TASK.md` (run the permuter in the foreground: two DeepSeek agents had exited while "waiting" on a background permuter).

## [2026-09-30] setup | Public-release audit
- Audit: no ROM bytes are committed. `data/*.s` and `asm/crt0.s` `.incbin` from the user's `baserom.gba`. There are no binaries, extracted assets or string dumps in asm/src. Card text is documented by location only (`tools/extract_cards.py` prints it to stdout). The wiki quotes a few card names as examples.
- Added `README.md` with build instructions and a "no game data" notice. The Makefile now prints a clear error when `baserom.gba` is missing, and the data units and `crt0` depend on it. Removed the local absolute path from `staging/AGENT_BRIEF.md`.
- Still to decide before publishing: a license, and whether to publish `staging/` (the agent orchestration notes).

## [2026-09-30] setup | MIT license, staging/ ignored
- Per the user: `LICENSE` (MIT, "EDS decomp contributors"), a License section in `README.md`, and `staging/` added to `.gitignore`.
- Public project data moved out of staging: `tools/compiler_tests/` (compiler evidence) and `config/names.txt` (proposed names; the old path is kept as a symlink). Wiki links to `staging/` were updated to the files' current homes.

## [2026-09-30] setup | Agent dashboard
- New `tools/dashboard.py` and `tools/dashboard.html`: a local page on port 8765 that refreshes every 10 s. It shows each agent's unit, state, last check result and recent commands (read from `docker logs`), plus the permuter batch, overall progress, the Codex window and recent log entries. It costs no model usage. It is launched from `.claude/launch.json` ("dashboard").

## [2026-09-30] progress | Applied permuter matches
- `sub_08064928` (`code_08063A28`): the DMA sequence wrapped in `do { ... } while (0)`, the shape of an SDK DMA macro, so probably the original form.
- `sub_08011F38` (`code_08011BE0`): reads `arg4` from memory at each use instead of keeping it in a local.
- Both applied in clean form, and both units still match. `sub_08009C64`: the raw permuter output was nonsense (an empty `if` with a repeated condition) and was not applied; the permuter was rerun from a cleaner base. `sub_080750E0` was marked REJECTED (ABI change).
- Pages touched: [[decomp-permuter]] (known patterns).

## [2026-09-30] progress | DeepSeek code_0802BAD0 (+3), code_080740BC (+2)
- Both units were verified (MATCH). The `code_0802BAD0` agent found a permuter false positive: by default the permuter ignores branch targets. `tools/permute.py` now passes `--no-ignore-branch-targets`. Everything applied earlier had been verified with check.py.

## [2026-09-30] setup | m2c first drafts; agents retasked
- m2c is now in the Docker image, plus `tools/m2c_draft.py` (asm preparation: jump-table labels, `ldsh`/`ldsb`, symbol names, unit context). Drafts exist for all 162 undrafted functions in `build/m2c/`.
- New `staging/OPENCODE_FRESH_PROMPT.txt` and `tools/launch_fresh.sh`. Four old cleanup agents were stopped (units repaired; `sub_0803B1EC` parked). DeepSeek is now 5 fresh + 5 cleanup; functions of 0x400 bytes and up are reserved for Codex and Claude.
- Pages touched: new [[m2c]], `index.md`.

## [2026-09-30] setup | Agents stop running the permuter themselves
- Agents were spending about 10 minutes idle per function in foreground permuter runs, while competing with the batch for CPU. They now park near-misses and move on. `tools/permute_batch.py` rescans continuously (3 workers). Both prompts start by applying finished score-0 results for their unit.
- All 10 agents were restarted on the new prompts (6 fresh, 4 cleanup). All units were repaired and match.

## [2026-09-30] progress | Fresh agent on code_0805D58C: 0 matched, 7 parked
- Unit is still MATCH. All 7 m2c-based attempts were parked as `#if 0` drafts for the permuter batch. Behaviour was documented on the unit page: `sub_0805D58C` is the 10-frame animation sibling of `D708`, and `sub_0805E3B8` is the duel scene init.

## [2026-09-30] setup | Similar-function finder; deepseek-v4-pro trial
- New `tools/similar.py`: it compares normalised instruction sequences with matched functions. Of 535 unmatched functions, 147 have a matched sibling at 60% or more, 47 at 80% or more, and some are 100% identical (e.g. `sub_080080D8` and `sub_0800A004`). Results are in `build/similar.tsv` and in notes at the top of the m2c drafts; both agent prompts now say to check it first.
- `deepseek-v4-pro` trial, at most 2 at a time (user limit): cleanup of `code_0805D58C` (flash got 0/7 there) and fresh work on `code_0800C894`. The launchers take `MODEL=`.
- Finished and verified (all MATCH): `code_08072FAC` (+1), `code_0805D58C` (0, 7 parked), `code_08053E58` (+1 via a permuter result), `code_0805E788` (+3, two FAKEMATCH register pins), `code_0800D8A4` (0, 5 parked).

## [2026-09-30] setup | Advisor tool
- New `tools/advisor.py`: a flash agent that is stuck asks `deepseek-v4-pro` once, sending the annotated asm, its current C, the diff, the closest matched sibling and our tips. It is capped at 6 calls per agent run and logged to `build/advisor.log`. First call: $0.022, 263 s. `tools/opencode-agent` now also passes `OPENROUTER_API_KEY` into the sandbox (OpenCode already holds the key there). Both prompts mention it.
- Pro trial so far: cleanup of `code_0805D58C` gave 0/8 (pure register allocation; that is permuter work, not a question of model strength).
- Verified MATCH after their runs: `code_08001364`, `code_080034B8`, `code_080184D8`, `code_08048FE0`, `code_08009A68` (+2 from applying permuter results).

## [2026-09-30] setup | Shared headers: include/main.h, include/duel.h
- New tools: `tools/typesurvey.py` (declaration survey), `tools/structmap.py` (per-field byte/bit offsets under agbcc layout rules, verified against old_agbcc) and `tools/mkheader.py` (draft structs).
- New headers `include/main.h` and `include/duel.h`, finalised by hand, with compile-time layout checks and probe-verified field access.
- New migration task `staging/OPENCODE_HEADERS_PROMPT.txt` + `tools/launch_headers.sh`; the dashboard shows a "headers" role.
- Pages touched: new [[shared-headers]], `index.md`.

## [2026-09-30] progress | First header migration; ideas from khcom and kleod
- `code_08009A68` migrated to `duel.h` (still MATCH). One local view was kept (`+0x9` bit 0 is read as a bitfield by `sub_0800A368`). The next migration is `code_0802AAC0`.
- Adopted from khcom: `decomp.yaml` (standard project metadata) and `tools/check_report.py` (refuses an inconsistent report; runs in `make objdiff-report` and in CI before upload).
- Noted for later: khcom's function-pointer and ROM-data "evidence" tools, YAML asset manifests, and CI with private ROMs. kleod's CONTRIBUTING bans AI-generated code; mention AI assistance in the README when publishing.

## [2026-09-30] progress | Function pointer tables; code_0803DD7C
- New `tools/fnptr_tables.py`, which writes `config/fnptr_tables.tsv`: 88 tables, 636 entries. New page [[function-pointer-tables]].
- `code_0803DD7C` finished (12/14 in C, MATCH). Its 2 advisor calls did not help (the suggestions had already been tried).
- `code_0802FB64`: the `sub_08030A28` family is pure register allocation (r3 vs r1). A register pin makes it worse; noted on the drafts.
- Header migration continues: `code_08002388` (struct Main).

## [2026-09-30] progress | Pro trial ended; migrations clean
- The deepseek-v4-pro trial is over: 0 matches in 3 runs (0/8 and 0/5 register-allocation leftovers, 0/6 fresh functions on `code_0800C894`), against several flash matches in the same period. No more pro agents; the on-demand advisor stays.
- `code_08002388` (struct Main) and `code_0802AAC0` (duel.h + main.h) migrated with no local views; both MATCH. `code_08031BC8` cleanup: 13/18 in C.
- Next migrations: `code_08007994`, `code_08030B88`, `code_0800D8A4`. Candidate for the next shared header: the multi-purpose `DuelCmd` at `0x020185C0` (noted by the `code_0800C894` agent).

## [2026-09-30] setup | duel_ui.h; Claude as advisor (trial)
- `struct DuelCmd` (gUnk_020185C0) and `struct DuelScreen` (gUnk_0201CFB0), with `DuelCmdEntry` and `DuelLoc`, now live in the new `include/duel_ui.h`. They were first added to `duel.h`, which broke `code_08030B88` because its own declarations conflicted. Lesson: never add new externs to a header that units already include.
- Advisor trial with Claude (user request): `tools/advisor.py` defaults to ADVISOR_BACKEND=claude. It queues the question to `build/advisor/queue/`, waits up to 20 min for `build/advisor/answers/`, then falls back to deepseek-v4-pro. At most 2 Claude answers per agent run. `tools/watch_advisor.sh` wakes the coordinator.

## [2026-09-30] progress | Migrations; Claude advisor subagent
- Migrated to the shared headers and verified MATCH: `code_0800D8A4` (0 views), `code_08007994` (4 views: DuelCard bit 18, DuelZone +0x8C..), `code_0802BAD0`, `code_08032CB0`, `code_0803DD7C` (0 views each), `code_08031BC8` (1 view: DuelState past the players). 9 units done.
- First Claude advisor answer (by the coordinator): +1% of the 5-hour window, weekly unchanged, about 15k tokens of context. User asked for a dedicated Opus subagent to watch the queue (`staging/ADVISOR_BRIEF.md`) with usage self-checks.

## [2026-09-30] progress | DuelState tail added to duel.h
- `struct DuelState` now covers up to `+0x1B20`: the queue (`queueCount`, `queueZone[16]`, `queueArg[16]`), the `+0x1B12` flags (`linkSkip`, `phase1B12`, `linkError`, `result`) and `phaseStep`. Contested bitfields are left as unk. Verified by probe and by all 11 migrated units (MATCH). `code_08035198` migrated (1 view, for the old tail).

## [2026-09-30] progress | More migrations
- `code_080383F0` and `code_0803B670` migrated (MATCH; 1 local view each). `code_0803732C` lost `sub_0803732C` in an interrupted migration (probably overlapping the duel.h edit); it was parked by repair.py and the migration was relaunched with a restore note. New rule in ORCHESTRATION: change headers only between migration waves.
- Watcher hardened (a container vanishing mid-inspect caused a false wake-up).
- Also verified: `code_0800EAA8` migrated (4 commented local views), `code_0803C838` fresh (`sub_0803CC18`, 0x1D4 bytes, matched on the first attempt from its m2c draft), `code_080609C4` cleanup (0 converted, 9 parked).
- `code_0803732C`: `sub_0803732C` restored to matching C, migration finished with 0 views (the extended DuelState covered its tail). `code_08012C4C` migrated (2 views).
- Migrated (MATCH): `code_08013CDC` (2 views: DuelCmd and DuelScreen, before duel_ui.h was in the prompt), `code_0801A7B4` (2 views), `code_0802DB30` (1 view: DuelZone +0x7 bit 5). The migration prompt now includes duel_ui.h.
- Migrated (MATCH): `code_0802CAE8`, `code_08033DAC` (1 view each). 19 units now use the shared headers. Header refinement from the 11 local views is noted in ORCHESTRATION for Codex/Claude.

## [2026-09-30] progress | Pattern sweep for quick wins
- `tools/similar.py`: 11 unmatched functions have a matched sibling at 90% or more (two at 100%: `sub_08072EB0` ~ `sub_08072FAC`, `sub_080750E0` ~ `sub_0807501C`). Two sibling agents (`staging/OPENCODE_SIBLINGS_PROMPT.txt`) adapt the siblings' C.
- Of 427 parked drafts, the NONMATCHING notes mention: register allocation 319, hoisting/loop-invariant/CSE 130, constants 115, scheduling 74, branch layout 71, stack 65, types 54. A trick-sweep prompt (`staging/OPENCODE_TRICKS_PROMPT.txt`: re-read, u64 temp, goto loop, local cache, do/while(0), declaration order) runs on the 3 idle units with the most hoist/CSE drafts: `code_0807960C`, `code_080619E8`, `code_080431E4`.
- Verified MATCH: `code_08002388` (+1: `sub_08002728`), `code_08017314` (0/5), `code_08050A70` (0), `code_0800AB08` (migrated, 3 views), `code_08010BDC` (migrated, 0 views).
- `code_08052B78` fresh: +1 (`sub_08052CE8`; switch cases written in the ROM's source order).

## [2026-09-30] progress | Advisor shift 1 results; regalloc permuter profile
- Claude advisor subagent, first shift: 10 answers (2 verified full matches, 5 partial, 3 none). Cost: weekly 83 to 84%, 5-hour 22 to 30% (about 0.8% of the 5-hour window per answer), about 4.5 min per answer. `sub_0800257C` was applied by the coordinator (+1): the agent had moved on, so the brief now requires the complete function in FULL MATCH answers. Shift 2 is running (stops at weekly 87%).
- Advisor findings were added to the matching tips in [[decomp-workflow]] (DMA set/wait split, switch range tests, loop.c/PRE behaviour, u16 counters, shared return label).
- `tools/permute.py --profile regalloc` (weights favouring declaration/statement order, commutative swaps, temporaries, chained assignments, local types) and `tools/permute_batch.py --profile auto` (regalloc profile for drafts whose note mentions register allocation) plus `--retry-below N` (closest-first second pass, logged to batch2.log). Containers: `eds-permuter-batch` (auto, 3 workers, 5 min) and `eds-permuter-regalloc` (retry-below 200, 1 worker, 15 min).
- Sibling sweep: 6 of 10 matched (`sub_0800EC54`, `sub_0802B48C`, `sub_0802BA68`, `sub_08031940`, `sub_08072EB0`, `sub_08039EDC`). Trick sweep: 0 of 9 (those drafts are really register allocation); not scaled up.
- Migrated (MATCH): `code_08000228`, `code_08001364`, `code_080034B8` (0 views each), `code_080162C4` (1 view). `code_080784E4`: +1 by applying the batch's permuter result for `sub_080792A0`.

## [2026-09-30] setup | Paused (user request)
- The Claude advisor subagent (shift 2) was stopped and `tools/advisor.py` defaults to the pro model again. Both permuter containers were stopped and removed. No new agents or Codex; the running DeepSeek agents are finishing and are verified as they end. See `staging/ORCHESTRATION.md` (PAUSED).
- Since the last entries: `code_08005500` (0 views), `code_08006878` (1 view), `code_080184D8` (0), `code_0801CE68`, `code_0801E260` migrated, all MATCH.

## [2026-09-30] progress | End of day: everything stopped (user request)
- The last sibling agents finished or were stopped and every unit was verified. Siblings at 80-90%: group 1 4/7 (`sub_08074C80`, `sub_0800D990`, `sub_08057550`, `sub_0807C1D0`), group 2 3/7 (`sub_0800D234`, `sub_0807761C`, `sub_0802F958`), group 3 1/8 (`sub_0801A7B4`). In total the sibling sweep matched 14 of 31.
- Final full `make compare`: eds.gba OK with every unit in C. 1465/1976 functions (74.14%), 42.72% of code bytes; objdiff report identical (1465, 42.68%).
- Migrated (MATCH): `code_08005500`, `code_08006878`, `code_080184D8`, `code_08019554`, `code_0801BCFC`, `code_0801CE68`, `code_0801E260`, `code_0801F454`.
- Wiki: lint is clean (index rebuilt with all 110 unit pages; 6 broken links fixed). New pages [[duel-engine]], [[code-08044224]] and [[agent-tooling]]; updated [[overview]], [[shared-headers]], [[decomp-permuter]] and [[decomp-workflow]] (advisor findings).
- Stopped: all agents, the advisor subagent, the permuter containers, the watcher and the dashboard.


## [2026-09-30] progress | Decomp continuation, two verified checkpoints
- Resumed at the user's request from 1465/1976. The first continuation checkpoint reached 1524/1976 (77.13%); full `make compare` passed and objdiff reported 44.44% code. The later settled checkpoint reached **1542/1976 (78.04%)**, +77 functions overall, with 45.27% by the simple byte counter and 45.23% by objdiff. Full `eds.gba: OK`; report validation passed. Logs are in `build/codex-continue/`.
- Recovered 12 saved exact C results, then matched `sub_08033E44` through a whole-unit parked-draft recheck. Rejected legacy permuter outputs with a wrong ABI, ignored branch targets, or artificial uninitialized scaffolds; none were enabled. Two finished agents delivered 20 additional C matches and updated their unit pages.
- Added [[sound-driver]] and `include/sound.h`: 26/30 Thumb functions now match in C, with unchanged MMIO widths and the original PCM `(voice, id, volume, note)` ABI. The four remaining routines retain assembly; typed/decoded drafts are explicitly marked nonmatching. Updated [[sound-engine]] and [[index]].
- The next wave added nine middle-unit matches, five later-unit matches, and four coordinator matches (`sub_08009EAC`, `sub_080016A8`, `sub_0800217C`, `sub_08001E4C`). The calendar/speaker changes use ordinary C expression/switch forms; some register-allocation matches use documented empty compiler constraints that emit no instructions.
- [[code-08044224]] now has a complete 42-body C draft for 60 supported dispatch inputs. The final synthetic-callee differential experiment passed 7,609 cases, measuring 821/821 card IDs plus list/player boundary cases and caller-save clobbers. This is behavioral evidence under the stated test model, not a byte match; the smaller nonmatching C body remains disabled and contributes no source-progress count.
- Updated [[overview]] with the verified counts and active worker state. Sources were frozen for each combined build, then released for further matching work.

## [2026-09-30] setup | Exact draft rechecks and extraction repairs
- Added `tools/match_drafts.py`: enables one parked draft at a time and retains only exact whole-unit ROM matches, restoring failed attempts verbatim and saving logs/backups. It supports both fallback preprocessor forms and multiline annotations; selected units must have no concurrent writer.
- Fixed `tools/permute.py` extraction of multiline draft comments and brace scanning through comments/quoted literals. Verified with real extracted compiler jobs, including the calendar/scene helper, and Docker Python compilation. A fresh 90-draft early-unit sweep found no additional automatic matches. Documented in [[decomp-permuter]].

## [2026-09-30] ingest | Relevant GBATEK GBA hardware sections
- Read the author's smaller GBA memory-map, DMA, sound FIFO/control, timer and interrupt pages after the complete HTML page timed out. [[gbatek]] stores attributed summaries and direct source links, with the unreviewed DS-family scope stated explicitly.
- Cross-checked FIFO IRQ accounting, DMA control values, Timer0 reload/start and IRQ bits against the ROM and matched sound driver. Preserved the distinction between sample timer rate and PWM output rate. Recorded the irrelevant apparent Game Pak DMA-address ambiguity as a source caveat.
- Updated [[sound-engine]], [[sound-driver]], [[overview]] and [[index]] references. No full copyrighted reference text or ROM assets were copied.


## [2026-09-30] lint | Continuation wiki checks
- `scripts/wiki_lint.py` checked 175 pages: zero broken links, orphans, catalog omissions, or frontmatter problems. The continuation's progress/tooling/GBATEK entries preserve the historical log.


## [2026-09-30] progress | Third continuation checkpoint: 1561 matching functions
- The settled combined build is still **`eds.gba: OK`**. Progress is **1561/1976 (79.00%)**, +96 from the starting checkpoint; byte coverage is `0x3A588/0x7EC70` (46.02%) by the simple counter and 45.98% by the validated objdiff report. 415 functions still retain assembly. Report fully-linked coverage is 5.26%.
- Since the previous checkpoint, middle units added six matches / 0x4CC bytes, later units added six / 0x238, and early units added four / 0x488. `code_08006878` and `code_0800FB10` now contain only C bodies; their complete units match.
- The coordinator matched `sub_080036FC` (0x154) using separate width/position locals and a staged save-base pointer, and `sub_08041F9C` (0xDC) with a shared fail label plus faithful u16 return narrowing. Both are ordinary C, with exact complete-unit checks. Updated [[code-080034b8]] and [[code-08041f9c]].
- The sound worker matched `sub_0807D3D0` (0x148): an empty initialized voice-pointer input after its reset loop extends the pointer lifetime and fixes all three hoisted register choices. [[sound-driver]] is now 27/30 C; full 0x1700 unit exact. Updated [[sound-engine]] and [[overview]].
- Saved improved disabled dialogue/record drafts: [[code-08001364]] scroll reload and signed sentinel now mirror ROM; [[code-080034b8]] input draft has extracted direction and explicit scroll branches. These remain nonmatching and add no source coverage. Sources were held for the combined verification, then released for more work.


## [2026-09-30] progress | Fourth continuation checkpoint: 1567 matching functions
- Full `tools/dr make -j8 compare` passed: **`eds.gba: OK`**. Settled matching source is **1567/1976 (79.30%)**, +102 from 1465, with `0x3AD10/0x7EC70` source bytes (46.39%). Validated objdiff reports 46.35% code and 5.26% fully linked. 409 functions retain assembly. Progress/report logs are in `build/codex-continue/`.
- Since 1561, middle units added `3415C` / `2D6D4` (0x19C), later units added `64984` / `63EDC` / `64698` (0x470), and sound added `7E3D8` (0x17C). Total new source bytes: 0x788. [[sound-driver]] is 28/30 Thumb C; [[code-08063a28]] is 30/32 C. All complete units and the combined ROM match.
- The coordinator translated the related `4244C` / `42BE0` state machines, corrected m2c access-width/pointer/variadic artifacts, and checked shared layout assertions with old_agbcc. Both drafts compile and remain disabled: 0x638 vs 0x664, and 0x5D8 vs 0x604. Their behavior has not been differentially verified. [[code-08041f9c]] remains 6/9 C and exact as a whole unit.
- Updated [[overview]] and [[sound-engine]]. All writers were held for the combined verification, then released. The user's workflow question prompted a hybrid: draft related routines together when shared layouts help, and retain each conversion only after exact whole-unit acceptance.
- Correction to the previous checkpoint entry: its later-unit subtotal is **0x284**, not 0x238. Six later matches plus middle 0x4CC, early 0x488, coordinator 0x230 and sound 0x148 total the correct 0xF50 new bytes at checkpoint 1561.

## [2026-09-30] progress | Wiki-led batching and reliable isolated extraction

- [[agent-tooling]], [[overview]]: added `tools/decomp_queue.py`, which assigns every remaining assembly fallback to one unit batch, records parked drafts and wiki links, and prioritizes already matched siblings. Refreshed sibling metadata now includes sound and SDK C sources. All remaining fallbacks have an existing parked draft or a raw m2c reference; these references do not count as converted C.
- [[decomp-permuter]]: fixed retained K&R function bodies that had shifted an isolated target's address. Other K&R bodies become unprototyped declarations, preserving the old caller narrowing behavior. Also fixed extraction for the `#else INCLUDE_ASM #endif` parked-draft form.
- [[code-08025108]]: matched `sub_08026030` (`0xBC`), completing all 22 functions and all `0x101C` unit bytes. Reused the documented allocation diagnosis, then minimized the result-address lifetime hint and split the raw table byte from its signed adjusted value.
- [[code-08008a1c]]: bounded graveyard-count variants remain private. A two-byte isolated near miss omitted the original `r7` save/restore and was rejected; the active assembly fallback remains intact.
- [[sound-driver]]: corrected typed decoder drafts remain disabled, with bounded differential checks and exclusions documented by the sound owner.
- Wiki lint after batch/tool documentation: 175 pages, no broken links, orphans, frontmatter omissions, or index gaps.

## [2026-09-30] progress | Fifth continuation checkpoint: 1,575 matching functions

- [[overview]]: all active source writers held during verification. `tools/dr make -j8 compare` returned `eds.gba: OK`; complete output is saved in `build/codex-continue/full-compare-fifth.log`. The independent objdiff report passed (`build/codex-continue/objdiff-report-fifth.log`).
- Progress: **1,575/1,976 (79.71%)**, 401 remaining; source counter `0x3B314/0x7EC70` bytes (46.69%), validated objdiff code 46.65%, fully linked 6.06%. Game 1,538/1,934, sound 28/33, SDK 9/9.
- Eight new matching C functions / `0x604` bytes since the 1,567 checkpoint: middle `28930`, `2BE70`, `31180`, `4158C` (`0x27C`); later `5748C`, `561A0`, `7A6AC` (`0x2CC`); coordinator `26030` (`0xBC`). Unit-level checks preceded the full build.
- Completed units in this batch: [[code-08025108]], [[code-08056ecc]], [[code-08055eb0]], [[code-0807a6ac]]. Source and function pages describe the minimized matching constraints.
- [[agent-tooling]]: refreshed queue contains 108 unit batches, 348 parked C drafts and 53 raw-reference-only fallbacks, with no missing starting references. Fixed extraction fixtures for both parked-draft formats and retained K&R ABI declarations passed; edited tools compile.
- [[sound-driver]]: two complete typed decoder drafts remain disabled. Differential evidence is finite and scoped on the function page; their compilation size still differs from ROM, so neither is counted as matching C.

## [2026-09-30] progress | Clarify unit completion at fifth checkpoint

- Correction to the preceding entry: only [[code-08025108]] became fully C (22/22). [[code-08056ecc]] still has four assembly fallbacks, [[code-08055eb0]] one, and [[code-0807a6ac]] five. Their reported 19/19, 14/14 and 42/42 byte-check counts include existing assembly bodies. All three gained exact C conversions, and all whole-unit byte checks passed, but they are not completely converted.
- The overall source counter, eight-function delta, byte totals and full ROM verification in the checkpoint remain correct.


## [2026-09-30] progress | Sixth continuation checkpoint: 1,585 matching functions

- [[overview]]: the held full build returned **`eds.gba: OK`** (`build/codex-continue/full-compare-sixth.log`); independent objdiff report validation passed (`objdiff-report-sixth.log`).
- Settled source progress is **1,585/1,976 (80.21%)**, +120 from 1,465; 391 remain. Source bytes are `0x3B8A0/0x7EC70` (46.96%); validated objdiff code is 46.92%, fully linked 6.06%. Game 1,548/1,934, sound 28/33, SDK 9/9.
- Ten new exact C functions / `0x58C` bytes since 1,575: coordinator `sub_08009150`, `sub_08008538` (`0xDC`); middle `sub_080342C0`, `sub_0803B47C`, `sub_08029A50`, `sub_08029F04` (`0x2FC`); later `sub_080577FC`, `sub_08057854` (`0xB0`); early link worker `sub_08017F98`, `sub_08017D38` (`0x104`). Whole-unit checks preceded the full build; source writers held throughout.
- Updated [[code-08008a1c]], [[code-08007994]], [[code-08033dac]], [[code-0803a654]], [[code-08029750]], [[code-08056ecc]], [[code-08017314]] with exact shapes, minimized initialized hints and remaining fallbacks. The count covers matching source bodies, including authored SDK assembly, rather than all functions printed as MATCH by a checker.
- Queue checkpoint: 108 unit batches, 340 parked C drafts and 51 raw-reference-only fallbacks, zero missing m2c references. Larger-function stream remains necessary: 64 routines of at least `0x400` bytes hold 48.5% of remaining code bytes.

## [2026-09-30] setup | Combined matching-tricks catalog

- [[matching-tricks]] consolidates the audit of all 110 unit pages plus named helper, compiler, sound and tool pages. Recipes are grouped by discrepancy; source links preserve exact examples, successful initialized hints, failed sweeps, ABI limits, behavioral-test exclusions and resolved historical diagnoses.
- Linked the catalog from [[index]], [[decomp-workflow]], [[agent-tooling]] and [[overview]], and made it a common reference in every generated `tools/decomp_queue.py` batch. Workflow now starts with sibling reuse, small Docker variant grids, short focused searches and complete-unit acceptance.
- Private held-source searches found no further exact matches: `sub_08015E40` finished its three-minute one-worker run with the four-byte miss unchanged; `sub_0802A09C` finished its 90-second search with the two-byte miss unchanged. Their disabled drafts and detailed failed variants remain available; neither adds matching progress.
- Corrected the stale odd-symbol workaround in the catalog: the current checker clears only Thumb FUNC bits and preserves odd data addresses. Original unset-register paths, decoder frame aliases and custom ARM mixer ABI are documented evidence limits, not generic matching shortcuts.


## [2026-09-30] lint | Combined tricks and batch references

- Docker `scripts/wiki_lint.py`: 176 pages, zero broken links, orphans, index gaps or frontmatter problems. Log: `build/codex-continue/wiki-lint-tricks.log`.
- Verified that [[matching-tricks]] directly cites all 110 unit pages and that all 108 generated batches reference the catalog. Queue refresh preserves 1,585 matching source / 391 fallbacks, 340 parked C / 51 raw references, with none missing a starting reference.


## [2026-09-30] progress | Seventh continuation checkpoint: 1,588 matching functions

- Held full ROM build returned **`eds.gba: OK`** (`build/codex-continue/full-compare-seventh.log`), and objdiff validation passed (`objdiff-report-seventh.log`): **1,588/1,976 (80.36%)**, 388 remaining, validated code 47.12%, fully linked 6.06%.
- Three exact C conversions / `0x3F4` bytes since 1,585: `sub_0805CEAC` / `sub_0805D080` (`0x3A8`) in [[code-0805c508]], and `sub_0801FE54` (`0x4C`) in [[code-0801f454]]. Their whole units are `0x1084` / `0x16A0` exact. Actual C counts are 5/9 and 13/15; assembly-inclusive checker counts are higher.
- [[matching-tricks]] now records the combined guarded scan/literal-order fix and minimized step-copy constraint. For `sub_0801FE54`, 53 plain width/copy variants and 33 unscoped hints failed; scoped pointer/copy construction found an exact candidate, and a 36-variant minimization removed all register bindings and all but one initialized read/write value constraint. Source behavior and original ABI are preserved.
- Catalog review corrected generic unit labels mistaken for function labels, added narrowing-based ABI inference, original data/rodata placement rules and original forwarded/unused parameter examples, and marked unresolved middle address/list-row observations partial.


## [2026-09-30] progress | Eighth continuation checkpoint: 1,589 matching functions

- [[overview]]: held full build returned **`eds.gba: OK`** (`build/codex-continue/full-compare-eighth.log`); objdiff validation passed (`objdiff-report-eighth.log`). Settled matching source is **1,589/1,976 (80.41%)**, +124 from 1,465, with 387 fallbacks. Counter bytes `0x3C0C8/0x7EC70` (47.37%); validated report code 47.32%, fully linked 6.06%. Game 1,552/1,934, sound 28/33, SDK 9/9.
- Added ordinary-C `sub_0802B558` / `0x434` bytes since the preceding checkpoint. [[code-0802aac0]] is 13/14 C and all 0x1010 unit bytes match. Explicit failed-case returns, the shared true label inside case 0x604, the initialized case-0x47 result, and the separately staged first-zone pointer recover every branch destination and final ADD operands without hints or ABI changes.
- The catalog-led resumed pass added four matching C functions / `0x828` bytes total since 1,585. Updated [[matching-tricks]] with the successful larger predicate; prior partial address notes are resolved.
- Refreshed queue: 108 batches, 337 parked C drafts and 50 raw-reference-only fallbacks, zero missing starting references. Of 0x42BA8 remaining source bytes, 63 routines of at least 0x400 bytes hold 0x205F8 (48.5%). Writers held through all combined checks.


## [2026-09-30] lint | Eighth checkpoint and catalog

- Docker wiki lint: 176 pages, zero problems (`build/codex-continue/wiki-lint-eighth.log`). Catalog coverage of all 110 unit pages and references from all 108 batches remain intact.

## [2026-09-30] progress | Ninth continuation checkpoint: 1,601 matching functions

- Both user-authorized teams held every source writer for combined validation. Full Docker build returned **`eds.gba: OK`** (`build/codex-continue/full-compare-ninth.log`); independent objdiff validation passed (`objdiff-report-ninth.log`). Settled source is **1,601/1,976 (81.02%)**, 375 fallbacks; counter bytes `0x3D2B4/0x7EC70` (48.25%), validated code 48.21%, fully linked 6.06%. Game 1,564/1,934, sound 28/33, SDK 9/9.
- Twelve exact C conversions / **0x11EC (4,588) bytes** since 1,589: coordinator `sub_08009298` (0xC4); later `sub_08060344` (0xBC), `sub_0807766C` / `sub_080776F8` (2×0x8C); helper animation `sub_080153D4` (0x34C) / `sub_08015E40` (0x484); helper recipe `sub_0804353C` (0x58), `sub_08043594` (0x6C), `sub_08043600` (0xB8), `sub_0804412C` (0xF8); helper early `sub_0800DA84` (0x280) / `sub_0800DD04` (0x290). Strict whole-unit acceptance preceded the held full build; byte-check counts include assembly fallbacks and are not C-body counts.
- [[matching-tricks]] now includes the staged masked-player scans, minimized text-state store hints, list-removal scan/compaction hints and ABI rejection, ordinary-C inline recipe helpers, shared escaped stack objects, and the two animation scope/range fixes. Resolved historical near-miss entries retain their failed experiments without presenting them as current blockers. Local pages are [[code-08008a1c]], [[code-0805f96c]], [[code-0807717c]], [[code-080150dc]], [[code-080431e4]], [[code-0800d8a4]].
- Refreshed queue: 107 batches, 325 parked C drafts and 50 raw-reference-only fallbacks, zero missing starting references. Of `0x419BC` remaining source bytes, 62 routines of at least `0x400` hold `0x20174` (48.91%).
- Larger reel [[code-08026124]] has a corrected exact-size `0x6FC` parked draft with 96 differing bytes; marker-4 Y's fixed-point shift is 8, not 4. The invalid lower-score unset-value candidate was rejected. [[sound-driver]] has a more accurate disabled main decoder/frame draft with finite ordinary-duration evidence; neither adds source progress. Fresh bounded root failures are retained in [[code-08008a1c]], [[code-08009a68]] and [[code-08019554]].
- A subsequent private HOLD search found an isolated `sub_0803FC88` match. It is excluded from this checkpoint and requires minimized-hint/ABI review and whole-unit acceptance before the authorized final recheck. All other source writers remain held.

## [2026-09-30] progress | Tenth continuation checkpoint: 1,602 matching functions

- All source writers remained held for the authorized final integration and recheck. `sub_0803FC88` adds **0x1E8 / 488 bytes**; [[code-0803edc4]] is now 10/12 C and all `0x10AC` unit bytes match (`build/middle_experiments/sub_0803FC88/unit-check.log`). Its two remaining empty hints consume initialized offset/sound values; minimization removed all r0 pins/inputs and extra case-3 constraints. Original ABI and branch behavior are retained.
- Held full ROM build returned **`eds.gba: OK`** (`build/codex-continue/full-compare-tenth.log`); objdiff validation passed (`objdiff-report-tenth.log`). Settled source: **1,602/1,976 (81.07%)**, 374 fallbacks, counter bytes `0x3D49C/0x7EC70` (48.34%), validated code 48.30%, fully linked 6.06%. Game 1,565/1,934, sound 28/33, SDK 9/9. `progress-tenth.txt` retains the source-counter output.
- The combined resumed pass adds **13 exact C functions / 0x13D4 (5,076) bytes** since 1,589. The preceding ninth-checkpoint entry records the other twelve. These are accepted source conversions; the decompilation is not complete.
- [[decomp-permuter]]: fixed blank-line tolerant parked-draft recognition in `tools/match_drafts.py`, shared by permute setup and queue generation. The old adjacency rule omitted existing `sub_08026E84` and `sub_08028D9C` C drafts. Six fixtures across both wrapper formats and all 326 real parked blocks passed in Docker (`draft-spacing-check.log`). This is an inventory correction, not additional C coverage; historical queue counts remain as recorded at the time.
- Final queue (`queue-tenth.log`): **107 batches, 326 parked C drafts, 48 raw-reference-only fallbacks, zero missing starting references**. Of `0x417D4` remaining bytes, 62 routines of at least `0x400` hold `0x20174` (49.00%).
- Updated [[overview]], [[matching-tricks]] and [[agent-tooling]] with both coverage metrics, new matching recipes, current queue and coordinated ownership. The user-directed larger coverage stream assigns [[code-08044224]], [[code-0800ab08]], [[code-08046738]] and the two [[sound-driver]] decoders to the helper lead's team; this team continues separate family batches. Live writes remain held until the completed checkpoint is reported and released.

## [2026-09-30] lint | Tenth checkpoint and catalog

- Docker wiki lint passed: 176 pages, zero broken links, orphans, index gaps or frontmatter problems (`build/codex-continue/wiki-lint-tenth.log`). The shared catalog still directly cites all 110 unit pages; all 107 refreshed batches reference it, and its navigation anchors resolve. Final source and report remain at the held 1,602-function checkpoint.

## [2026-09-30] progress | Eleventh continuation checkpoint: 1,609 matching functions

- Both coordinated teams held all source writers. Full Docker ROM build returned **`eds.gba: OK`** (`build/codex-continue/full-compare-eleventh.log`), and the independent objdiff report passed (`objdiff-report-eleventh.log`). Settled matching source: **1,609/1,976 (81.43%)**, 367 fallbacks; source bytes `0x3DD58/0x7EC70` (48.77%), validated code 48.73%, fully linked 6.06%. Game 1,572/1,934, sound 28/33, SDK 9/9. Full source counters are in `progress-eleventh.txt`.
- Seven new exact C conversions / **0x8BC (2,236) bytes** since 1,602: ordinary-C `sub_0800DF94` (0x24C), `sub_0800E438` (0x1F8), `sub_0801A010` (0x8C), `sub_0802C080` (0x1B0), and `sub_080469DC` (0x98); initialized compiler-hint conversions `sub_0802C3C0` (0x11C) and `sub_08077784` (0x88). Strict whole-unit acceptance and cross-team source/ABI review preceded combined verification. Actual C counts: [[code-0800d8a4]] 6/9, [[code-08019554]] 22/25, [[code-0802bad0]] 32/33, [[code-08046738]] 19/23, [[code-0807717c]] 33/36. Assembly-inclusive byte-check totals are higher.
- [[matching-tricks]] and the unit pages record the new escaped-object/zone staging, fresh halfword ID, ordinary narrow reader/local boundary, default-before-case-group tail, initialized level/call-order and count-copy recipes. A010's initial exact candidate used an initialized constraint; minimization removed every hint and the stride local. Fresh finite kind-3 link/count grids remain unsuccessful; [[code-08009a68]] retains their limits.
- Refreshed queue (`queue-eleventh.log`): **107 batches, 320 parked C drafts, 47 raw-reference-only fallbacks, zero missing starting references**. Remaining bytes `0x40F18`; 62 functions of at least `0x400` hold `0x20174` (49.41%). The new guarded [[code-08046738]] summon reconstruction accounts for one raw-to-parked inventory change without adding anchor coverage.
- Larger frontiers remain nonmatching: [[code-08044224]] private 0x2340/0x2514 candidate passed its 7,609 finite cases; [[code-0800ab08]] 0x1CD0/0x1CCC has compilation/layout/source-assembly audit, without executed behavior validation; [[code-08046738]] summon draft 0x1E18/0x1DF8 has unresolved invalid-state guards. [[sound-driver]] sequencer 0x7DC/0x7CC passed 15,350 ordinary-duration and 1,280 prefix cases; its corrected private SE loop 0x4A0/0x4A4 passed 13,440 finite cases. These scoped results contribute no matching coverage. The sound interpreter rejected a delay-guarded cursor-jump rewrite and a conflicting live r7 pointer candidate.
- All source writers remain held through documentation/lint. The other user-authorized chat now owns allocation, future shared builds and global coverage/wiki reporting. This team will take disjoint next lanes from that lead. A newly exact private `sub_0807780C` is excluded from this checkpoint pending minimized-hint review and live whole-unit acceptance.

## [2026-09-30] lint | Eleventh checkpoint and catalog

- Docker wiki lint passed: **176 pages, zero problems** (`build/codex-continue/wiki-lint-eleventh.log`). The catalog retains all 110 unit-page references and is linked from every one of the 107 refreshed batches. Source/coverage claims remain at the held 1,609-function checkpoint.


## [2026-09-30] progress | Twelfth continuation checkpoint: lead coordination and 1,624 matching functions

- The user appointed this chat as lead over both teams. Six subagents and the supporting coordinator now work disjoint families; the lead owns allocation, reviews, combined builds and global reporting. `build/lead-coordination.json` is the ownership authority. Source holds preserve private work, and live writes were released immediately after combined validation.
- Full Docker ROM build returned **`eds.gba: OK`**; objdiff report validation passed. Source hashes remained unchanged during the checkpoint. Settled source: **1,624/1,976 (82.19%)**, 352 fallbacks, source bytes `0x3F6D8/0x7EC70` (50.03%), validated code **49.99%**, fully linked 6.06%. Game 1,587/1,934 with 50.05% code; sound 28/33; SDK 9/9. Logs, report and queue snapshots: `build/lead-pass2/`.
- Added **15 exact C functions / 0x1980 (6,528) bytes** since 1,609: `4A528`/0x320, `4BAAC`/0x1CC, `4B640`/0x3DC, `4DB6C`/0x11C, `4E31C`/0xA0, `4F6EC`/0xB8, `4F460`/0x1F4, `656B4`/0x144, `62F6C`/0xD4, `64AF4`/0xAC, `19E0C`/0x204, `7780C`/0x13C, `0E1E0`/0x258, `193D4`/0x180 and `370B8`/0x274 (all `sub_080` symbols). Both teams performed source/ABI review alongside strict whole-unit acceptance. `build/lead-pass2/accepted.json` identifies each writer and review. The combined work since 1,589 adds 35 functions / 13,840 bytes.
- Updated [[matching-tricks]] with narrow reader/local pairs, corrected UI layout, snapshot and field-width staging, destination/source ordering and minimized initialized constraints. Unit pages retain function-level evidence and original quirks. Corrected parked drafts or private near matches are not additional coverage.
- Refreshed queue: **107 batches, 305 parked C drafts, 47 raw-reference-only fallbacks, zero missing starting references**. Remaining source bytes `0x3F598`; 62 routines of at least 0x400 retain `0x20174` bytes. Updated [[overview]], [[agent-tooling]] and [[index]]; worker source writes resumed while global documentation continued.

## [2026-09-30] lint | Twelfth checkpoint and catalog

- Docker wiki lint passed: **176 pages, zero problems** (`build/lead-pass2/wiki-lint.log`). The catalog directly references all 110 unit pages and is referenced by all 107 checkpoint batches (`catalog-check.log`). Coverage claims use the saved, source-stable 1,624-function report; subsequent live work will enter the next held checkpoint.


## [2026-09-30] progress | Thirteenth continuation checkpoint: over half of code bytes matched

- Both coordinated teams held source writes while all six workers continued private experiments. Full Docker build returned **`eds.gba: OK`**; objdiff validation passed; source hashes were unchanged. Settled source: **1,631/1,976 (82.54%)**, 345 fallbacks; source bytes `0x40960/0x7EC70` (50.94%), validated code **50.90%**, fully linked 6.06%. Source writes were released immediately after the saved report/queue snapshot. Evidence: `build/lead-pass3/`.
- Seven exact C conversions / **0x1288 (4,744) bytes** since 1,624: `sub_08018ED8`/0x1A0, `sub_08018DC8`/0x110, `sub_08061E54`/0x2EC, `sub_0806347C`/0x190, `sub_08054398`/0x3D8, `sub_08027D34`/0x39C and `sub_080334E4`/0x3E8. Each passed original source/ABI review and complete-unit comparison. The accumulated work since 1,589 adds **42 functions / 18,584 bytes**; no nonmatching private reconstruction is counted.
- The coordinator's read-only survey compiled 112 unassigned larger drafts, then assigned close candidates across both teams. Three of those assignments yielded 2,908 exact bytes this checkpoint. A separate 89-variant local-width survey yielded no additional exact candidates. Rankings retain compiler/size limits and require behavior/ABI review; see [[agent-tooling]].
- Updated [[matching-tricks]] with preserved switch joins, initialized pointer/value lifetimes, ordinary sprite coordinate/extents staging and packed scroll completion. The correct u32 destination declaration in `sub_080370B8` removed its warning while preserving all unit bytes; zero coverage delta.
- Refreshed queue: **107 batches, 299 parked C drafts, 46 raw-reference-only fallbacks, zero missing starting references**. Remaining bytes `0x3E310`; 62 routines of at least 0x400 hold `0x20174` (51.60%). The newly parked `sub_0800CE28` draft affects the reference inventory only. Updated [[overview]] and [[agent-tooling]].

## [2026-09-30] lint | Thirteenth checkpoint and catalog

- Docker wiki lint passed: **176 pages, zero problems** (`build/lead-pass3/wiki-lint.log`). The catalog retains all 110 unit-page references and every one of the 107 checkpoint batches links to it (`catalog-check.log`). Current coverage claims use the saved 1,631-function report; workers continue the next batch.


## [2026-09-30] progress | Checkpoint 14: broader scout lanes yield exact conversions

- Lead-controlled source hold confirmed by both coordinators and all six workers; private experiments continued. Full Docker build: **`eds.gba: OK`**. Objdiff report validation and stable source hashes passed. Settled coverage: **1,636/1,976 functions (82.79%)**, **51.52% validated code**, source `0x415F0/0x7EC70` (51.56%), fully linked 6.06%. Evidence: `build/lead-pass4/`. Live writes were released immediately afterward.
- **5 exact functions / 3,216 bytes** added since 1,631: `sub_08055728`/0x2D8, `sub_0806A9AC`/0x364, `sub_0807CDB4`/0x1B8, `sub_080358AC`/0x310, `sub_0802A4CC`/0x18C. Each passed source/ABI review and whole-unit comparison. Accumulated since the initial 1,589 checkpoint: **47 functions / 21,800 bytes**. Since lead coordination began at 1,609: **27 functions / 14,488 bytes**.
- Newly assigned source units came from the coordinator's 112-draft survey. Workers retain substantial anchors alongside related conversions. Updated [[overview]], [[agent-tooling]] and [[matching-tricks]] with count extraction and text-coordinate lifetime recipes.
- `sub_08020AF4` now has a complete ordinary C reconstruction, 0x874 versus 0x8CC. Two private variants each passed 7,592 finite comparisons across every dispatch state and all 821 card IDs using synthetic callees. Original 0x12C frame and saved registers recovered; state-tail merging/address allocation remain. Draft parked, no matching coverage. Evidence and limitations: [[code-08020af4]]. Larger effect and UI worker drafts also remain outside the coverage count.
- Refreshed inventory: 107 batches; 302 parked C and 38 raw-reference fallbacks; zero missing starting references. Remaining 0x3D680 bytes; 62 routines of at least 0x400 contain 0x20174 (52.26%). No baserom/raw files changed.


## [2026-09-30] lint | Checkpoint 14 wiki and queue references

- `scripts/wiki_lint.py`: 176 pages, zero broken links, orphans, index gaps or frontmatter issues. All 110 unit and 107 queue-batch catalog references resolve. Evidence: `build/lead-pass4/wiki-lint.log` and `catalog-check.log`.
- Added reviewed structure-gap, inline-count, selected-row and minimized-binding recipes to [[matching-tricks]]. Global docs use the stable 1,636-function checkpoint; ongoing private drafts are excluded from coverage.

## [2026-10-01] progress | Workers retired; lead-only continuation

- Honored the user's usage-conservation instruction: all three local workers, all three supporting workers and the supporting coordinator finished their current tasks and stopped. No restarts or replacements. The lead alone continues; all assigned units were released to it. Updated [[overview]], [[agent-tooling]], [[index]] and `build/lead-coordination.json`, preserving previous assignments and handoff locations.
- The final worker handoffs add no enabled conversions beyond checkpoint 14: 1,636/1,976 functions (82.79%), 51.52% validated code. The coordinated pass added 27 functions / 14,488 bytes from the 1,609-function baseline. Supporting final evidence is consolidated in `build/codex-continue/support-final-handoff.md`.
- Preserved the large near-matching frontiers and recorded bounded failures in [[code-0804b640]] and [[code-08020af4]]. The lead's ten additional state-33 variants did not improve BFF0's 18-byte gap; a prior two-minute 20AF4 permuter search found no improvement. No active C coverage changed.
- Final combined verification after all handoffs: `tools/dr make -j8 compare` returned `eds.gba: OK`; wiki lint checked 176 pages with zero issues.


## [2026-10-01] progress | Solo checkpoint 15: exact action-list resolver

- Enabled `sub_08020AF4`, 0x8CC / 2,252 bytes of ordinary C. Argument locals and destination-first pointer construction close the remaining scheduling differences; all experimental hints removed. [[code-08020af4]] is now 5/8 C with the full 0x11D4-byte unit exact.
- Full Docker ROM compare: **`eds.gba: OK`**. Objdiff report validation, queue regeneration and source-stability checks passed. Coverage **1,637/1,976 (82.84%)**, **51.96% validated code**; source `0x41EBC/0x7EC70` (52.00%). Logs: `build/lead-pass5/`.
- 339 remaining fallbacks: 301 parked C and 38 raw references; 107 batches. Updated [[overview]], [[agent-tooling]], [[matching-tricks]] and the private ownership record. All agents remain stopped; lead works alone.


## [2026-10-01] progress | Solo checkpoint 16: complete duel-control C unit

- Enabled the action-list filter `sub_080213C0` (448 bytes), win check `sub_08021628` (524) and shared controller `sub_08021A48` (640). Combined with checkpoint 15, this solo continuation adds **4 functions / 3,864 bytes**. All eight functions of [[code-08020af4]] now match in ordinary C, with no compiler hints.
- Named card-number/count intermediates, verified u16 predicate returns and the u16 crossing selection field recover the original allocation. The original controller tails match without a goto rewrite. Updated [[matching-tricks]] and the unit page with evidence and ABI limits.
- Full Docker ROM: **`eds.gba: OK`**. Objdiff validation, source stability and refreshed queue passed. **1,640/1,976 (83.00%)**, **52.27% validated code**, source `0x42508/0x7EC70` (52.31%); fully linked 6.94%. Logs: `build/lead-pass6/`.
- Remaining **336 functions**: 298 parked drafts and 38 raw references in 106 unit batches. Updated [[overview]], [[agent-tooling]] and private checkpoint records. All workers remain stopped.
- Documented bounded non-improvements in [[code-0804b640]], [[code-0806a92c]] and [[code-08053e58]]. Private candidate sizes/scores are not accepted coverage. No baserom or raw files changed.

## [2026-10-01] progress | Solo ABI survey and finish-controller experiments
- [[code-0805b3f4]] records the 420-definition Boolean survey, bounded negative caller trials, and the observed residual second argument at the parked zone-scan call.
- [[code-0804b640]] records twelve unsuccessful cross-byte destination-field variants. [[matching-tricks]] links the ABI finding.
- All changes in this survey are private experiments or documentation. The verified checkpoint remains 1,640/1,976 functions and 52.27% validated code bytes.

## [2026-10-01] progress | Solo checkpoints 17–19: battle and deck initialization
- Enabled four ordinary-C functions totaling **3,852 bytes**: battle calculation `sub_0801D264` (1,500), deck initializers `sub_0806F0F8` (776) / `sub_08070A1C` (1,272), and summon/set command `sub_08012C4C` (304). [[code-0801ce68]] is now 9/9 C; [[code-0806ed44]] is 10/12, [[code-0807093c]] 3/4, and [[code-08012c4c]] 11/12.
- Shared table-address recipe plus local packed-field and zone-byte views; no new compiler hints or ABI changes. Updated [[matching-tricks]] and documented the bounded negative menu-field/stat tests in [[code-0801e260]].
- Each held checkpoint passed complete ROM, objdiff, queue and source stability checks. Latest: **1,644/1,976 (83.20%)**, **53.01% validated bytes**; source **0x43414/0x7EC70 (53.05%)**, fully linked 7.92%. `eds.gba: OK`; latest logs `build/lead-pass9/`.
- Remaining **332 functions**, 296 parked drafts and 36 raw references in 105 batches, 0x3b85c bytes. Updated [[overview]], [[agent-tooling]] and private coordination records. All workers remain stopped.

## [2026-10-01] progress | Solo checkpoints 20–21: four table-address conversions
- Enabled link prompt `sub_08050E48` (428-byte slice), recipe eligibility `sub_08043AA8` (240, including two padding bytes), token summon `sub_0801296C` (372) and selected-card display `sub_08068B90` (184): **four functions / 1,224 source-slice bytes**. Their clean C has no new compiler hints. Updated [[code-08050a70]], [[code-080431e4]], [[code-08011be0]], [[code-08068180]] and [[matching-tricks]].
- Whole-ROM **`eds.gba: OK`**, objdiff report, source stability and queue passed at both checkpoints. Latest **1,648/1,976 (83.40%)**, **53.25% validated code**; source **0x438DC/0x7EC70 (53.29%)**. Objdiff measures the recipe body as 0xEE, excluding its two alignment bytes. Logs: `build/lead-pass11/`.
- Remaining **328 functions**: 292 parked drafts, 36 raw references, 105 batches, 0x3B394 bytes. [[overview]] and [[agent-tooling]] updated. All workers remain stopped; all tools reuse Docker.

## [2026-10-01] progress | Solo checkpoint 22: complete card copy-limit switch
- Enabled `sub_080771A8` (**704 bytes**): fixed reverse-ID table view, cached count byte, one initialized r1 binding, no empty constraints. [[code-0807717c]] now has 35/36 C functions; only the OAM emitter remains. Updated [[matching-tricks]].
- Complete unit and full ROM **`eds.gba: OK`**. Objdiff, source stability and queue passed: **1,649/1,976 (83.45%)**, **53.38% validated bytes**; source **0x43B9C/0x7EC70 (53.42%)**. Logs: `build/lead-pass12/`.
- **327 functions** remain (291 parked / 36 raw references), 105 batches and 0x3B0D4 bytes. Updated [[overview]], [[agent-tooling]] and private coordination records. All agents remain retired except the lead.

## [2026-10-01] progress | Solo checkpoint 23: entire OAM and collection unit in C
- Enabled `sub_08077EF4` (**1,520 bytes**) in ordinary C with no compiler hints; [[code-0807717c]] is **36/36 C**. Verified signed helper-coordinate signatures and the original nonempty/supported-mode return contract. Corrected the wiki's unit endpoint and argument count.
- Complete unit and full ROM **`eds.gba: OK`**. Objdiff, source stability and queue passed: **1,650/1,976 (83.50%)**, **53.68% validated bytes**; source **0x4418C/0x7EC70 (53.71%)**. Logs: `build/lead-pass13/`.
- **326 functions** remain (291 parked / 35 raw references), 0x3AAE4 bytes. Updated [[overview]], [[agent-tooling]], [[matching-tricks]] and private records. All supporting agents remain retired.

## [2026-10-01] progress | Solo checkpoint 24: zone-value effect and duel-screen setup
- Enabled `sub_08032E7C` (164 bytes, two initialized bindings) and `sub_080609C4` (232 bytes, one initialized binding). Neither has empty constraints. Updated [[code-08032cb0]], [[code-080609c4]], [[matching-tricks]].
- Complete units and full ROM **`eds.gba: OK`**. Objdiff, source stability and queue passed: **1,652/1,976 (83.60%)**, **53.75% validated bytes**; source **0x44318/0x7EC70 (53.78%)**. Logs: `build/lead-pass14/`.
- **324 functions** remain (289 parked / 35 raw references), 0x3A958 bytes. Recorded the negative 658-variant fixed-RAM survey. Updated [[overview]], [[agent-tooling]] and private records; all other agents remain retired.

## [2026-10-01] progress | Solo checkpoint 25: card move and swap animations
- Enabled `sub_0805D848` (468 bytes, new reconstruction) and `sub_0805DA1C` (372 bytes), both ordinary C with no bindings or constraints. Updated [[code-0805d58c]], [[matching-tricks]].
- Whole units and full ROM **`eds.gba: OK`**. Objdiff and source stability passed: **1,654/1,976 (83.70%)**, **53.91% validated code**; source **0x44660/0x7EC70 (53.95%)**. Logs: `build/lead-pass15/`.
- **322 fallbacks**, 288 parked / 34 raw, 0x3A610 bytes remain. Rejected the earlier empty-branch animation match. Recorded negative statistics/address experiments in [[code-0806c4e4]]. Corrected stale batch/large-function totals in [[overview]] and updated [[agent-tooling]]; no workers restarted.

## [2026-10-01] progress | Solo checkpoint 26: complete dialogue text processor
- Enabled `sub_08000C54` (1,436 bytes), with three initialized bindings and one empty constraint after minimization. [[code-08000228]] is 19/20 C. Preserved the cursor's unresolved original call/stack contract rather than inventing arguments.
- Whole unit and full ROM **`eds.gba: OK`**. Objdiff and source stability passed: **1,655/1,976 (83.76%)**, **54.19% validated code**; source **0x44BFC/0x7EC70 (54.22%)**. Logs: `build/lead-pass16/`.
- **321 fallbacks**, 287 parked / 34 raw, 0x3A074 bytes remain. Updated [[overview]], [[agent-tooling]], [[matching-tricks]] and private records. Documented private list/AI/tile reconstructions and finite behavioral evidence in [[code-08068180]], [[code-0805a30c]], [[code-0805d58c]]. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 27: banner fade completes another unit
- Enabled `sub_08016848` (464 bytes), completing [[code-080162c4]] at 17/17 C. Two initialized bindings and one empty constraint remain after minimization.
- Whole unit and full ROM **`eds.gba: OK`**. Objdiff/source stability passed: **1,656/1,976 (83.81%)**, **54.28% validated code**; source **0x44DCC/0x7EC70 (54.32%)**. 13 complete units. Logs: `build/lead-pass17/`.
- **320 fallbacks**, 286 parked / 34 raw, 0x39EA4 bytes, 102 unit batches. Updated [[overview]], [[agent-tooling]], [[matching-tricks]]. The fresh reel ABI experiment remains private and unmatched. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 28: starting-deck builder
- Enabled `sub_0800495C` (352 bytes); [[code-080044e4]] is 17/20 C. The caller's 0..2 choice domain and all eleven positive pool counts were checked.
- Whole unit and full ROM **`eds.gba: OK`**. Objdiff/source stability passed: **1,657/1,976 (83.86%)**, **54.35% validated code**; source **0x44F2C/0x7EC70 (54.38%)**. Logs: `build/lead-pass18/`.
- **319 fallbacks**, 285 parked / 34 raw, 0x39D44 bytes, 102 unit batches. Fresh private compilation of 134 smaller drafts identified close candidates but no already exact draft. Recorded AI state-machine reconstruction and 832 finite behavioral fixtures in [[code-080590e4]]. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 29: two duel-action routines
- Enabled `sub_08054E7C` (1,052 bytes) and `sub_080555B0` (376 bytes); [[code-08054e7c]] is 8/9 C. Reconciled the verified word-return declaration of `sub_0802CFA0` without changing its bytes.
- Whole units and full ROM **`eds.gba: OK`**. Objdiff/source stability passed: **1,659/1,976 (83.96%)**, **54.62% validated code**; source **0x454C0/0x7EC70 (54.66%)**. Logs: `build/lead-pass19/`.
- **317 fallbacks**, 284 parked / 33 raw, 0x397B0 bytes, 102 unit batches. All workers remain retired. Checkpoint 28 catalog and wiki lint passed (110 unit references, 102 batches, 176 pages, no issues).

## [2026-10-01] progress | Solo checkpoint 30: sibling actions and graveyard scan
- Enabled `sub_08054900` (608 bytes), `sub_08054B60` (796 bytes), and `sub_08009C64` (72 bytes). [[code-08053e58]] is 8/11 C; [[code-08009a68]] is 27/29 C. Corrected the second action draft's tribute-player arguments against the ROM.
- Whole units and full ROM **`eds.gba: OK`**. Objdiff/source stability passed: **1,662/1,976 (84.11%)**, **54.91% validated code**; source **0x45A84/0x7EC70 (54.94%)**. Logs: `build/lead-pass20/`.
- **314 fallbacks**, 281 parked / 33 raw, 0x391EC bytes, 102 batches. Checkpoint 29 catalog/wiki lint passed (110 unit references, 102 batches, 176 pages, no issues). All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 31: nine-state AI strategy
- Enabled `sub_0805C508` (1,072 bytes), 1,056 differential fixtures, exact whole unit. Fixed its adjacent strategy/target layout and reconciled four callee word-return definitions while preserving all unit bytes. [[code-0805c508]], [[code-08007994]], [[code-08008a1c]], [[code-08053e58]], [[code-080590e4]].
- Full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,663/1,976 (84.16%)**, **55.11% validated code**; source **0x45EB4/0x7EC70 (55.15%)**. Logs `build/lead-pass21/`.
- **313 fallbacks**, 281 parked / 32 raw; 0x38DBC bytes, 102 batches. The 55 remaining large functions hold 0x1D9FC bytes (52.10%). All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 32: complete nine-function strategy unit
- Enabled `sub_0805C938`, `sub_0805CB2C`, `sub_0805D254`: **3 functions / 1,740 bytes**, completing [[code-0805c508]] at 9/9 C. Finite differential fixtures passed 640/1,024/768 cases respectively; the middle suite explicitly includes simulated table reads for the unavailable 0x4E1 number.
- Reconciled the action helper's word interface with explicit halfword decoding in [[code-08055eb0]] and the field-card word return in [[code-08008a1c]], preserving every unit byte.
- Full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,666/1,976 (84.31%)**, **55.45% validated code**; source **0x46580/0x7EC70 (55.49%)**. **310 fallbacks**, 278 parked / 32 raw, 101 batches; 14 complete units. Large-function share 52.49%. Logs `build/lead-pass22/`.
- Checkpoint 31 catalog/wiki lint passed: 110 unit references, 102 queue batches, 176 pages, no issues. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 33: neighboring ten-state AI routine
- Enabled `sub_0805C0A0` (**1 function / 1,128 bytes**), bringing [[code-0805b3f4]] to 3/6 C. Corrected its parked target layout and message arguments; 1,152 finite behavior fixtures passed.
- Whole unit and full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,667/1,976 (84.36%)**, **55.67% validated code**, source **0x469E8/0x7EC70 (55.70%)**. **309 fallbacks**, 277 parked / 32 raw, 101 batches. Remaining 0x38288; 54 large functions hold 0x1D594 bytes (52.26%). Logs `build/lead-pass23/`.
- Checkpoint 32 catalog/wiki lint passed: 110 unit references, 101 batches, 176 pages, no issues. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 34: AI zone scanner and effective flag ABI
- Enabled `sub_0805B884` (**1 function / 764 bytes**), bringing [[code-0805b3f4]] to 4/6 C. Reconciled the property helper's word input/explicit halfword decode in [[code-08006878]] with no byte change. 1,920 finite scanner fixtures pass.
- Full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,668/1,976 (84.41%)**, **55.81% validated code**, source **0x46CE4/0x7EC70 (55.85%)**. **308 fallbacks**, 276 parked / 32 raw, 101 batches. Remaining 0x37F8C; 54 large functions hold 0x1D594 bytes (52.43%). Logs `build/lead-pass24/`.
- Main phase reached four normalized extra instructions and passed 1,664 behavior fixtures, but remains assembly; no coverage credit. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 35: candidate menu unit complete
- Enabled the renderer, picker and monster-set check: **3 functions / 816 bytes**. [[code-08053e58]] is 11/11 C, all 0x1024 bytes exact. Reconciled callback/setup prototypes and the byte-identical RNG word result in [[code-08076144]]. 1,536 finite differential fixtures pass.
- Full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,671/1,976 (84.56%)**, **55.97% validated code**, source **0x47014/0x7EC70 (56.01%)**. **305 fallbacks**, 273 parked / 32 raw, 100 batches. Remaining 0x37C5C; 54 large functions hold 0x1D594 bytes (52.62%). Logs `build/lead-pass25/`. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 36: large AI state machine and complete unit
- Enabled `sub_08059B14`: **1 function / 2,040 source bytes** (2,038 objdiff code bytes), completing [[code-080590e4]] at 12/12 C. Reconciled byte-identical interfaces in [[code-08007994]] and [[code-0802db30]]. Expanded finite verification passes 1,664 fixtures, with the prior first-hand-loop coverage gap explicitly corrected.
- Full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,672/1,976 (84.62%)**, **56.36% validated code**, source **0x4780C/0x7EC70 (56.40%)**. **304 fallbacks**, 273 parked / 31 raw, 99 batches. Remaining 0x37464; 53 large functions hold 0x1CD9C bytes (52.20%). Logs `build/lead-pass26/`. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 37: large AI card scan
- Enabled `sub_0805AB90`: **1 function / 1,524 source bytes** (1,522 objdiff code bytes), bringing [[code-0805a30c]] to 2/5 C. Reconciled four byte-identical callee interfaces. The expanded 2,400-case finite check passes, including high card bits and table-index mutations between calls.
- Full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,673/1,976 (84.67%)**, **56.66% validated code**, source **0x47E00/0x7EC70 (56.69%)**. **303 fallbacks**, 273 parked / 30 raw, 99 batches. Remaining 0x36E70; 52 large functions hold 0x1C7A8 bytes (51.87%). Logs `build/lead-pass27/`. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 38: main AI phase
- Enabled `sub_0805B3F4`: **1 function / 1,168 source bytes** (1,166 objdiff bytes), bringing [[code-0805b3f4]] to 5/6 C. Shared post-call state pointers resolve the final tail mismatch. 3,328 finite cases pass; two callee interface descriptions were reconciled.
- Full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,674/1,976 (84.72%)**, **56.88% validated code**, source **0x48290/0x7EC70 (56.92%)**. **302 fallbacks**, 272 parked / 30 raw, 99 batches. Remaining 0x369E0; 51 large functions hold 0x1C318 bytes (51.62%). Logs `build/lead-pass28/`. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 39: hand scanner
- Enabled `sub_0805A8E8`: **1 function / 680 bytes**, bringing [[code-0805a30c]] to 3/5 C. Corrected the parked type mask, reproduced loop backedges and message packing, and reconciled the byte-identical message interface in [[code-0801e260]]. **13,952 finite fixtures PASS**.
- Full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,675/1,976 (84.77%)**, **57.01% validated code**, source **0x48538/0x7EC70 (57.05%)**. **301 fallbacks**, 271 parked / 30 raw, 99 batches. Remaining 0x36738; 51 large functions hold 0x1C318 bytes (51.78%). Logs `build/lead-pass29/`. Workers remain retired.

## [2026-10-01] progress | Solo checkpoint 40: complete hand-action unit
- Enabled `sub_08055B28`: **1 function / 532 bytes**; [[code-08054e7c]] is now **9/9 matching C**, all 0x1034 unit bytes exact. Five initialized bindings/two empty constraints remain after minimizing; **8,616 finite fixtures PASS**. Recorded unresolved branch-estimate and register-residue frontiers in [[code-0805a30c]].
- Full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,676/1,976 (84.82%)**, **57.12% validated code**, source **0x4874C/0x7EC70 (57.15%)**. **300 fallbacks**, 270 parked / 30 raw, 98 batches. Remaining 0x36524; 51 large functions hold 0x1C318 bytes (51.90%). Seventeen complete units, **12.99%** complete code. Logs `build/lead-pass30/`. Workers remain retired.

## [2026-10-01] progress | Solo checkpoint 41: statistics percentages
- Enabled `sub_0806CB68`: **1 function / 428 bytes**, [[code-0806c4e4]] now **5/9 matching C**. Minimized initialized hints; **1,024 finite fixtures PASS**. Reconciled the signed fixed-point divide word ABI in [[code-0807a6ac]] and [[code-08065e6c]], retaining exact complete-unit bytes.
- Full ROM **`eds.gba: OK`**, objdiff/source stability PASS: **1,677/1,976 (84.87%)**, **57.20% validated code**, source **0x488F8/0x7EC70 (57.23%)**. **299 fallbacks**, 269 parked / 30 raw, 98 batches. Remaining 0x36378; 51 large functions hold 0x1C318 bytes (52.00%). Seventeen complete units. Logs `build/lead-pass31/`. Workers remain retired.

## [2026-10-01] progress | Solo checkpoint 42: statistics graphics initializer
- Enabled `sub_0806CDD4`: **1 function / 572 bytes**, [[code-0806c4e4]] now **6/9 matching C**. Two initialized bindings/three empty constraints remain; **256 graphics fixtures PASS**. Full ROM, objdiff, source stability, catalog and wiki lint pass.
- Settled coverage: **1,678/1,976 (84.92%)**, **57.31% validated code**, source **0x48B34/0x7EC70 (57.34%)**. **298 fallbacks**, 268 parked / 30 raw, 98 batches; remaining 0x3613C. 51 large functions hold 0x1C318 bytes (52.14%). Evidence `build/lead-pass32/`. All workers remain retired.

## [2026-10-01] progress | Solo checkpoint 43: copy-count selector
- Enabled `sub_0806C534`: **1 function / 92 bytes**, [[code-0806c4e4]] now **7/9 matching C**. One initialized binding, no empty constraints; **6,144 fixtures PASS** including default-selector behavior and full-word argument decoding.
- Full ROM, objdiff and source stability pass. Settled coverage **1,679/1,976 (84.97%)**, **57.33% validated code**, source **0x48B90/0x7EC70 (57.36%)**. **297 fallbacks**, 267 parked / 30 raw, 98 batches; remaining 0x360E0. 51 large functions hold 0x1C318 bytes (52.16%). Logs `build/lead-pass33/`. Workers remain retired.

## [2026-10-01] progress | Solo checkpoint 44: ordinary C touch bookkeeping
- Enabled `sub_080666D8`: **1 function / 516 bytes**, [[code-08065e6c]] now **11/15 matching C**; no bindings or constraints, **7,680 fixtures PASS**. Fixed its parked case-2 pointer and unsigned shifts. Complete card-list reader views in [[code-08068180]] retain all bytes and its existing ABI.
- Full ROM, objdiff and source stability pass. Settled coverage **1,680/1,976 (85.02%)**, **57.43% validated code**, source **0x48D94/0x7EC70 (57.46%)**. **296 fallbacks**, 266 parked / 30 raw, 98 batches, remaining 0x35EDC. The 51 large routines hold 0x1C318 (52.28%). Logs: `build/lead-pass34/`. Workers remain retired.

## [2026-10-01] progress | Solo checkpoint 45: deck copy-count reader
- Enabled `sub_08068434`: **1 function / 564 bytes**, [[code-08068180]] now **12/15 matching C**; four bindings/two constraints after minimization; **24,829 fixtures PASS**. Complete two-row views and explicit unsigned shifts preserve semantics.
- Full ROM, objdiff and source stability pass. Settled coverage **1,681/1,976 (85.07%)**, **57.53% validated code**, source **0x48FC8/0x7EC70 (57.57%)**. **295 fallbacks**, 265 parked / 30 raw, 98 batches, remaining 0x35CA8. The 51 large routines hold 0x1C318 (52.41%). Logs: `build/lead-pass35/`. Workers remain retired.

## [2026-10-01] progress | Solo checkpoint 46: link queue helpers
- Enabled `sub_08071FA0` and `sub_08072238`: **2 functions / 232 bytes**, [[code-08071f40]] now **18/24 matching C**; one binding each, no empty constraints; **910 + 3,840 fixtures PASS**.
- Full ROM, objdiff and source stability pass. Settled coverage **1,683/1,976 (85.17%)**, **57.58% validated code**, source **0x490B0/0x7EC70 (57.62%)**. **293 fallbacks**, 263 parked / 30 raw, 98 batches, remaining 0x35BC0. The 51 large routines hold 0x1C318 (52.47%). Logs: `build/lead-pass36/`. Workers remain retired.
- Private frontiers: [[code-08068180]] panel restores an omitted lookup and passes 6,144 cases; [[code-08055eb0]] AI selector reaches 13 differing bytes and passes 2,666 cases. Neither counts as matching C.

## [2026-10-01] progress | Solo checkpoint 47: link receive dispatch
- Enabled `sub_08072054`: **1 function / 484 bytes**, ordinary C with no bindings/hints; [[code-08071f40]] now **19/24 C**. **9,200 fixtures PASS**, including busy flags, every type/sequence, duplicate packets and timer wrap.
- Full ROM, objdiff and source stability pass. Settled coverage **1,684/1,976 (85.22%)**, **57.67% validated code**, source **0x49294/0x7EC70 (57.71%)**. **292 fallbacks**, 262 parked / 30 raw, 98 batches, remaining 0x359DC. The 51 large routines hold 0x1C318 (52.58%). Logs: `build/lead-pass37/`. Workers remain retired.

## [2026-10-01] progress | Solo checkpoint 48: link message reassembly
- Enabled `sub_080722B0`: **1 function / 260 source bytes**; [[code-08071f40]] now **20/24 C**. **6,644 completing fixtures and 128 bounded busy-loop cases PASS**. Four address bindings, no empty constraints.
- Full ROM, objdiff and source stability pass. Settled coverage **1,685/1,976 (85.27%)**, **57.72% validated code**, source **0x49398/0x7EC70 (57.76%)**. **291 fallbacks**, 261 parked / 30 raw, 98 batches. Remaining 0x358D8; 51 large routines contain 0x1C318 bytes. Logs: `build/lead-pass38/`. Workers remain retired.

## [2026-10-01] progress | Solo checkpoint 49: mixer source, two near-misses closed
- New session, solo at the user's request ("Work solo"). A fresh compile scout of all 261 parked drafts is in `build/solo-s49/scout/ranked.json`. `tools/check.py --diff F --norm` hides branch targets and pool offsets.
- Enabled `sub_080365F0` ([[code-080361d0]], permuter FAKEMATCH: empty repeated guard) and `sub_08057EE0` ([[code-08057ee0]], up-counting loop reversed by loop.c, plus existing r2 hints).
- Authored `src/sound_mixer_arm.s` for the three hand-written ARM mixer routines ([[sound-mixer]]). Corrected the page: DMA0 not DMA3, 15-bit loop-sample index, stale unit names. `tools/mkobjdiff.py` and the Makefile now treat any `src/<unit>.s` as complete authored assembly.
- Improved parked drafts: `sub_080044E4` (`WeekOfMonth` inline fixes the switch-index copy; only the prologue order differs), `sub_0804E240` (4 diff lines). Notes in [[code-080044e4]], [[code-0804db6c]], with new entries in [[matching-tricks]].
- Policy: documented byte-identical FAKEMATCHes are accepted; fixture suites are no longer required ([[overview]]).
- Full ROM **`eds.gba: OK`**; objdiff report OK: **1,690/1,976 (85.53%)**, 57.95% code.

## [2026-10-01] progress | Solo checkpoint 50: signed reload and flag bitfields
- Enabled `sub_08066260` ([[code-08065e6c]], `int` temp for the `s16` reload) and `sub_08049DF0` ([[code-08048fe0]], the `0x1B12` flags read as bitfields). Both are ordinary C.
- Improved parked drafts: `sub_0800D398` (86 → 48 lines, [[code-0800c894]]), `sub_08076448` / `sub_08076714` (212 → 22 / 14, [[code-08076144]]).
- Tooling: `build/solo-s49/classify.py` separates register-only from structural diffs, and `patterns.py` mines recurring diff shapes across all drafts (for example a systematic `lsrs #9`/`#10` field-width error in `sub_0804FC4C`). A background permuter queue (`build/solo-s49/pbatch.py`, 3 × 10 min) runs over the near-misses, nearest first.
- Full ROM **`eds.gba: OK`**; **1,692/1,976 (85.63%)**, source 0x49B54/0x7EC70 (58.14%).

## [2026-10-01] progress | Checkpoint 51: climber, Codex big functions, byte priority
- The user re-opened parallel help (Codex `gpt-6-astra`), asked for a nonstop permuter and **byte-first prioritisation**, and accepts ugly but byte-identical FAKEMATCHes.
- **Codex:** [[code-0804b640]] complete (`sub_0804BFF0` 0xB68 + `sub_0804BC78` 0x378) and [[code-08026124]] complete (`sub_08026E84` 0x6FC), both with FAKEMATCH register hints. Agents now own `code_08046738`, `code_0800AB08`, `code_0807093C`+`code_0806ED44`, `code_08022D5C` and `code_0806D51C` (`build/solo-s49/owned.txt`).
- **Lead:** `sub_08060ECC` (permuter: `u16 id`, [[code-080609c4]]), `sub_0803D048` (per-branch returns, [[code-0803c838]]), `sub_0807A1A8` LZSS (climber types/order plus loop bound in the condition, [[lzss-decompress]]).
- **Semantic hill-climber** (`build/solo-s49/climb.py`, [[decomp-permuter]]): three passes improved about 220 parked drafts (types, declaration order, comparison/ternary/add flips, statement swaps). Saved "best candidates" in `build/` beat the parked drafts for `sub_0807332C` (→ 2 lines), `sub_0802A09C` (→ 4), `sub_08036030` (→ 22), `sub_080649D8` (→ 30) and `sub_0805664C` (13-byte frontier); they are now the parked drafts.
- Permuter: `build/solo-s49/pforever.py` runs nonstop, ranked by bytes ÷ distance, skipping owned units.
- Full ROM **`eds.gba: OK`**; **1,698/1,976 (85.93%)**, source 0x4B394/0x7EC70 (**59.34%**).

## [2026-10-01] ingest | ROM versions and the JP plan
- The user supplied all dumps they could find. Organized them into `roms/base_eng.gba` (USA, the baserom; `baserom.gba` now symlinks there) and `roms/base_jp.gba` (Japan AY5J). Duplicates and a cracktro-patched `[Eng]` dump went to the Trash. `.gitignore` covers `roms/` and `*.zip`; the `CLAUDE.md` layout is updated.
- New page [[rom-versions]]: hashes and headers, a USA↔JP comparison (same compiler, LZSS byte-identical at `0x08002C80`; 166 functions / 3.1% of bytes identical modulo relocation), how other decomps handle versions, and the agreed roadmap (USA 100% → semantic rename → `make jp` → shared C with `#if VERSION_JP`). Updated [[index]].

## [2026-10-01] progress | Checkpoint 52: Codex wave ends (usage limit), harvest and cleanup
- **Codex (gpt-6-astra) results:** [[code-08022d5c]] complete (`sub_08023228` 0xE64); `sub_0806D644` (0x560, [[code-0806d51c]]); `sub_0806F404` ([[code-0806ed44]], permuter); the remaining small drafts in [[code-08046738]]. All accepted fixes are marked FAKEMATCH on the unit pages.
- The other agents hit the Codex weekly limit (resets 2026-10-08). `code_0800AB08` was left with its candidate enabled (0xC bytes too large): `tools/repair.py` re-parked it, and the following full build restored `build/eds.elf` (a stale shifted map had produced false `check.py` diffs).
- **Harvest:** the newest scratch candidates were evaluated against the parked drafts (`build/solo-s49/evalcand.py`). Better drafts installed: `sub_0800ABC8` (1006 → 676 normalized lines) and `sub_0806DBB0` (0x1194, previously undrafted, now 46 lines, with its helper structs).
- **Cleanup:** stopped three orphaned Codex permuter containers (none near zero) and moved the Codex scratch folders from `/tmp` to `build/codex-scratch/` (wiki evidence paths updated). Only the nonstop queue (`pforever.py`) still runs.
- Full ROM **`eds.gba: OK`**; 1704/1976 functions (86.23%), 0x4cf80/0x7ec70 bytes (60.71%) of game+SDK code.

## [2026-10-01] setup | ROM-free workflow for GitHub agents and CI
- Goal (user): move to GitHub and let web agents work without ever committing or providing a ROM. New page [[rom-free-workflow]], plus `AGENTS.md` (the committed agent brief) and a README "Without the ROM" section; `CLAUDE.md` build notes updated.
- `tools/target.py`: target bytes from the real ROM, or from the unit's original assembly assembled at its address. **Verified 112/112 code units: assembled original == ROM.** `check.py`, `permute.py` (ROM sanity check skipped without a ROM) and `check_all.py` use it.
- `config/symbols.txt` (69 symbols not derivable from their names) is generated by `tools/dumpsyms.py` after every full link (Makefile hook). The `AGBCC_DIR` / `PERMUTER_DIR` paths are configurable.
- `tools/check_all.py` (gate: all C units plus authored assembly units) is wired into `.github/workflows/progress.yml`, together with a no-regression check on PR function counts and the existing objdiff report.
- `tools/setup_native.sh` plus a Docker-less fallback in `tools/dr`. **Validated in a bare `debian:trixie-slim` container with no Docker and no ROM: setup, then 112/112 units match.** A ROM-free repo copy also passes `check.py`, the permuter setup and `make objdiff-report`.
- Publishing scan: no binary game data outside the gitignored `roms/` and `build/`. Publishing `asm/` remains the user's call ([[rom-free-workflow]]).
- Climber pass 5 has converged: 7 small improvements, no matches.

## [2026-10-01] setup | Public release prep and deslop pass (part 1)
- Installed the `deslop` writing skill (user-level, from CosmicScribe64/dotfiles) and applied it to the public-facing writing: `README.md` (rewritten for public readers, with a progress snapshot and an AI-assistance section), `AGENTS.md`, `CLAUDE.md` (wording only), CI/Makefile/Dockerfile comments, tool docstrings, `templates/source.md`, [[overview]] (rewritten as a current-state summary) and [[matching-tricks]] (265 em-dash status labels converted, curly quotes straightened).
- Release prep: `.gitignore` now also excludes `.claude/`, `.DS_Store` and the local multi-agent orchestration tools (`tools/opencode-agent`, `advisor.py`, `launch_*.sh`, `watch_*.sh`, the dashboard, `docker/opencode/`), which depend on gitignored prompts and local credentials. A dry-run index lists 2,534 files (16 MB): no game data, no local paths beyond one wiki note handled by the pass.
- Remaining: the other 175 wiki pages and the C comments, split into eight slices for subagents (`build/solo-s49/deslop-slices.json`, brief `build/solo-s49/deslop-brief.md`). `wiki/log.md` stays as written (append-only history).
- Checks: `eds.gba: OK`, `check_all.py` 112/112, wiki lint clean.

## [2026-10-01] setup | Asset extraction, deslop pass (part 2) and release prep
- **Asset extraction** ([[assets]]): `make setup` (`tools/setup.py`) finds the user's ROM in `roms/` (a `.gba`, or a `.zip`), checks its SHA-1, explains a wrong file (Japanese release, patched or trimmed dumps, other games), links `baserom.gba` and extracts the data into `assets/`. `config/assets.tsv` covers every data byte with 54 assets. Card names, descriptions, stats, dialogue, duelists, card art (821 PNGs), fonts, the system palette and tiles are editable. Sound and the image banks stay `.bin` for now. The data units are generated (`tools/assets.py gen-data`), and the build no longer reads the ROM. Verified: 54/54 assets round-trip; a fresh clone with a zipped ROM builds `eds.gba: OK`; the build still matches with the ROM deleted; asset edits reach the ROM.
- **Deslop:** subagents rewrote all 175 remaining wiki pages (5 slices) and the comments in the C sources (3 slices, code verified unchanged, `check_all.py` 112/112). Stale counts in [[code-080361d0]], [[code-08009a68]], [[code-080740bc]] and [[code-080044e4]] were corrected, [[game-overview]] was refreshed, and one new open question went to [[open-questions]].
- **Release:** the README has the setup flow, an assets table and badges (CI and decomp.dev). The decomp.dev convention was checked against zeldaret/tp (`<VERSION>_report` artifact; ours is `AY5E_report`). The repository is initialized with a GitHub noreply author address.

## [2026-10-01] setup | Published on GitHub
- Public repository: https://github.com/CosmicScribe64/eds-decomp (commits use the account's noreply address). CI passed on the first push: the match gate (112/112), the no-regression check and the objdiff report (`AY5E_report` artifact).
- README badges are self-hosted. CI writes `code.json` and `functions.json` (`tools/badges.py`) to the `badges` branch, and shields.io renders them. The decomp.dev GitHub app is installed; the project wasn't listed on decomp.dev yet at the time of writing, so its badges aren't used.

## [2026-10-01] query | Pace of other GBA decomps
- New [[decomp-pace-comparison]]: GBA projects on decomp.dev with start dates, team sizes and days to 50%/100%, how the fast ones handle the tail, and eight ranked changes for EDS. Indexed in [[index]].
- Progress: two permuter matches applied (`sub_0802F200`, `sub_08065E6C`), completing `code_0802EB58` and `code_08065E6C`. Full `make compare`: `eds.gba: OK`. 1,706/1,976 functions, 60.86% of code bytes.

## [2026-10-01] progress | Fable pass on the eight largest functions
- Eight Fable agents (brief in `build/fable/BRIEF.md`) worked on `sub_08044224`, `sub_080471E8`, `sub_0800ABC8`, `sub_0806DBB0`, `sub_0806B3B0`, `sub_08070F18`, `sub_0806F934` and `sub_0804FC4C`. No new matches. Improved parked drafts: `sub_08070F18` 539 to 257 differing lines (rebuilt from its matched sibling `sub_0806DBB0`); `sub_0806B3B0` now has a complete draft. Per-function notes with ordered blockers are in `build/fable/<func>/NOTES.md`.
- Cost: about 30 points of the 5-hour limit and 16% of the weekly Fable quota in roughly 20 minutes of work; Fable use also counts toward the all-models limits.
- Tools: `check.py --diff` prints the first-difference offset, matching prefix and differing-line count. New `tools/corpus.py` compiles C from nine other agbcc decomps (35,333 functions) and finds similar functions by instruction pattern.
- Full `make compare`: `eds.gba: OK`; `check_all` 112/112.

## [2026-10-01] progress | Solo pass: one match, five first drafts
- `CLAUDE.md` is now the only contributor guide (AGENTS.md removed upstream; its rules folded into a "Matching rules" section). `.gitignore` now ignores `wiki/.obsidian/workspace*`.
- New plan in `build/solo-s50/PLAN.md` from the pace research and a fresh measurement of all 245 parked drafts.
- [[code-08031bc8]]: `sub_080320C4` matches with a FAKEMATCH mask (`((ref->player & 1) ^ 1) & 1`). First C draft for `sub_08032390` (192 lines).
- First C drafts (m2c now runs in the image; the old drafts were empty failures): `sub_0805E1D0` (174 lines), `sub_08076DAC` (289, follows `sub_08076BEC`), `sub_080437CC` (99), `sub_08065AB4` (282, first 37% exact).
- `sub_0806DBB0` draft 46 to 43 lines (LEFT menu tail pinned to the RIGHT tail's registers).
- Full `make compare`: `eds.gba: OK`; `check_all` 112/112. 1,707/1,976 functions, 60.91% of code bytes.

## [2026-10-01] progress | Solo pass 2: first drafts for the no-draft list
- First C drafts (each parked with its blocker noted): `sub_0807C4CC` password keypad (63 lines), `sub_0805E788` screen transition (115), `sub_080064AC` card detail stars (381), `sub_0803283C` sweep effect (487), `sub_080686E8` deck-edit list builder (760), `sub_08043B98` ritual summon (1,221), `sub_08020330` effect chain resolution (1,142).
- `sub_08051ED0` draft 18 to 13 lines (only the level-table reload differs).
- `build/solo-s49/climb.py` gained two mutation kinds (redundant re-mask, index casts); one run over the 53 drafts within 40 lines gave three small improvements and no matches.
- Full `make compare`: `eds.gba: OK`; `check_all` 112/112. Still 1,707/1,976 functions (60.91%).

## [2026-10-01] progress | sub_08043B98 draft improved (still nonmatching)
- `src/code_080431E4.c`: reordered cases to the target order (0x80, 0x64, 0x63, 0x78), early `skip` return, case 0x80 now matches (two-branch call, `int n = 0x58A`, 3-arg call to `sub_08043AA8` via cast, `volatile u8` re-read of `need` as FAKEMATCH), `& 1` in the 0x64 loop condition, single-return level helper.
- Remaining: `ref` lives in r8 (target r9), loop counter/`ok` registers differ in 0x64 and 0x78, and the `level == 0` test folds the shift in the build. `register ... asm("r9")` for `ref` made it worse. Permuter score stayed around 9000+, unhelpful.
- Updated draft parked; unit still `MATCH`.

## [2026-10-01] progress | sub_08043B98 draft near-match (still nonmatching)
- Same size as target; remaining diffs are a few register choices (0x80 call temps, an extra base copy in the 0x64 hoist, a 0x78 word-loop reg swap). Key finds: `p` local before struct hand access, `nv`/`m` locals, u8-returning single-exit level helper, if/else cost clamp, address-sum zone access, `*(arr + n)` index. Permuter best 364 from base 519.

## [2026-10-01] progress | sub_080471E8 case 40 matches; case 80 remains
- Updated [[code-08046738]]: case 40 of the parked draft now byte-identical (pinned-register/barrier forms, spill-register liveness trick); only case 80 still differs (table-pointer spill and reload-pick rotation). Recorded the reload/cse/local-alloc mechanics found in the agbcc sources. Still nonmatching, unit `MATCH` with `INCLUDE_ASM`.


## [2026-10-01] progress | sub_08044224 region-diff pass (still nonmatching)
- `src/code_08044224.c`: parked draft replaced by the improved candidate (volatile stats table, `u16` type inline, `b/off` pointer forms, `off += 4` loops, `cardNo`/`switch` compares, tail `word++` filters); unit still `MATCH`.
- Masked region-diff metric 4667 -> about 1300; size 0x24A0-0x24EC vs 0x2514. Remaining gap is mostly register allocation (list base sl vs r9, count pointer ip) and the tail loop shapes.
- Updated [[code-08044224]] with the confirmed idioms and allocation notes.

## [2026-10-01] progress | sub_0801A130 matches; check.py ignores trailing alignment pad
- `tools/check.py --diff`: a target size that exceeds the built `.size` only by the zero `.align 2, 0` pad up to a 4-byte boundary no longer prints a `size:` line, so `wf.py` scores byte-identical functions 0 instead of 8. Real size differences are still reported (tested with a +10-byte variant).
- `src/code_08019554.c`: `sub_0801A130` enabled through `wf.py apply` (fresh hand pseudo `offset + (players + 0x684)`); unit `25/25 functions match; unit bytes MATCH`, enabled C 24/25. `check_all` 112/112.
- Updated [[code-08019554]] (table row, unit status, resolved near-miss note, new "Hand search match" section) and [[agent-tooling]] (score note).

## [2026-10-01] progress | Workflow waves 1-2: 61 functions matched
- Multi-agent `tools/wf.py` waves (private per-function working copies, locked merge into `src/`). Wave 1 matched 52 functions, wave 2 nine more. With `sub_0807332C` (permuter, just before wave 1) the session moved from 1,707 to 1,769 of 1,976 functions (89.52%) and from 60.91% to 66.91% of code bytes (after wave 1: 1,760, 89.07%, 64.85%).
- Wave 1 (52): `sub_080044E4`, `sub_08005860`, `sub_0800A9C8`, `sub_0800CC18`, `sub_0800CD68`, `sub_080122B4`, `sub_0801A130` (logged separately above), `sub_080280D0`, `sub_0802A09C`, `sub_0802BBDC`, `sub_0802D058`, `sub_08030FFC`, `sub_08032D10`, `sub_08033AEC`, `sub_08035C0C`, `sub_08035E0C`, `sub_08036030`, `sub_08037AF4`, `sub_0803C254`, `sub_0803EE6C`, `sub_0804112C`, `sub_080420A4`, `sub_0804AC18`, `sub_0804E240`, `sub_0804E5B4`, `sub_080515A4`, `sub_080516D8`, `sub_08051DF4`, `sub_08052190`, `sub_08052668`, `sub_080536D4`, `sub_08053770`, `sub_0805664C`, `sub_0805DF34`, `sub_080649D8`, `sub_080657F8`, `sub_0806704C`, `sub_0806710C`, `sub_0806DBB0`, `sub_080730A8`, `sub_08074260`, `sub_08074868`, `sub_08075F74`, `sub_0807609C`, `sub_08076448`, `sub_08076714`, `sub_0807A5D4`, `sub_0807A754`, `sub_0807B628`, `sub_0807B9D4`, `sub_0807CCAC`, `sub_0807D1F4`.
- Wave 2 (9): `sub_0803DEB8`, `sub_08070F18`, `sub_08051ED0`, `sub_08052810`, `sub_0807AEF0`, `sub_08019078`, `sub_0807C058`, `sub_0806F934`, `sub_08017FF4`.
- Units now complete in C: `code_0802BAD0`, `code_0802CAE8`, `code_08032CB0`, `code_08055EB0`, `code_0806D51C`, `code_0807C7C8`, `code_0803DD7C`, `code_0807093C`, `code_0806ED44`.
- Unit pages updated (status counts, table rows, a waves section per unit, stale notes marked historical): [[code-080044e4]], [[code-08005500]], [[code-08009a68]], [[code-0800c894]], [[code-08011be0]], [[code-08017314]], [[code-080184d8]], [[code-08027580]], [[code-08029750]], [[code-0802bad0]], [[code-0802cae8]], [[code-08030b88]], [[code-08032cb0]], [[code-08035198]], [[code-0803732c]], [[code-0803b670]], [[code-0803dd7c]], [[code-0803edc4]], [[code-08040ebc]], [[code-08041f9c]], [[code-0804a008]], [[code-0804db6c]], [[code-08050a70]], [[code-08051a9c]], [[code-08052b78]], [[code-08055eb0]], [[code-0805d58c]], [[code-08063a28]], [[code-08064af0]], [[code-0806704c]], [[code-0806d51c]], [[code-0806ed44]], [[code-0807093c]], [[code-08072fac]] (also records `sub_0807332C`), [[code-080740bc]], [[code-080750e0]], [[code-08076144]], [[code-0807960c]], [[code-0807a6ac]], [[code-0807b6b8]], [[code-0807c7c8]].
- [[matching-tricks]]: new section "Register allocation priority and reload rotation" (global-alloc priority `floor_log2(refs)*refs/live_length` from the `.greg`/`.lreg` dumps, pseudo-number ties, REG_EQUIV live-length doubling, local-alloc's three-quantity sort bug, reload round-robin `last_spill_reg`, busy-register asm pairs, reload inheritance); loop.c threshold and two-pass hoisting, GCSE/PRE on constant-pool loads before loop.c; callee prototype widths and call-type casts; packed-argument operand order; `* 64` versus `<< 6`; empty `asm("" : "+r"(x))` against combine/CSE folds; DImode copies; `switch ((u8)st)`; cross-jump survivor rule; struct 4-byte alignment, ARRAY_REF versus `pointer_int_sum`. About fifteen stale partial/failed entries marked resolved.
- Contradictions flagged (`> [!warning] Contradiction`, each resolved in favour of the matched source): [[code-08035198]] (a dead `p = 0` store rejected earlier, accepted as FAKEMATCH in `sub_08036030` under the current rules); [[code-0803732c]] and [[matching-tricks]] (`sub_08037AF4` cited as a symbol-form example, but it matched with integer-constant tables); [[code-0804a008]] ("cross-jumping is not controllable from C", overturned by `sub_0804AC18`); [[code-08051a9c]] ("sum before load" for `sub_08051DF4`; the match loads `w824` first).
- [[decomp-workflow]] ("When a draft won't match" links `tools/wf.py` and notes the `.align 2, 0` pad rule in `check.py`), [[agent-tooling]] (`wf.py dump`/`score` rows; `apply` rejects asm symbol aliases), [[overview]] (progress, largest remaining functions, tooling), [[open-questions]] (deck-edit cell array start). `scripts/wiki_lint.py`: no broken links, orphans or frontmatter problems.

## [2026-10-02] progress | Workflow waves 2-3 and giants: 141 functions matched
- From the end of waves 1-2 (1,769/1,976) to the wave 3 checkpoint, commit `f7c9206`: **1,910/1,976 functions (96.66%), 83.35% of code bytes** (`tools/progress.py` on that commit). That is 62 functions in the rest of wave 2, 77 in wave 3, and 2 in the giants loop. The matched set is every `build/wf/sub_*/applied.json` not recorded in the waves 1-2 entry. 42 units became entirely C, for 71 of 111 C units.
- Giants (the 7 KB functions): `sub_0800ABC8` ([[code-0800ab08]]; score 676 -> 0; top-down reload-rotation fixes, two FAKEMATCH forms, no pins) and `sub_080471E8` ([[code-08046738]]; 287 -> 0; case 80 reloads, plus three branch-target differences hidden by a score of 0; 70 FAKEMATCH comments). Two giants remain, with their notes updated: `sub_08044224` ([[code-08044224]], wf score 6266 -> 4321) and `sub_0804FC4C` ([[code-0804eff0]], 1026 -> 656).
- Wave 2, rest (62): `sub_08000AC8`, `sub_08004B84`, `sub_08008D3C`, `sub_08009424`, `sub_0800D398`, `sub_0800EE50`, `sub_08010D94`, `sub_08011FFC`, `sub_08012D7C`, `sub_08017314`, `sub_08017DE0`, `sub_080283BC`, `sub_0802B2FC`, `sub_08030620`, `sub_080326C4`, `sub_08034644`, `sub_08035198`, `sub_080384F4`, `sub_080398B0`, `sub_08039F68`, `sub_0803BCE4`, `sub_0803D3D0`, `sub_08040478`, `sub_08041898`, `sub_080436B8`, `sub_08043B98`, `sub_08049450`, `sub_08049B74`, `sub_0804A008`, `sub_0804A848`, `sub_0804E420`, `sub_0804F168`, `sub_08051A9C`, `sub_08051BBC`, `sub_08051CD8`, `sub_08052018`, `sub_08058358`, `sub_0805E100`, `sub_08061580`, `sub_080616D0`, `sub_080624A4`, `sub_080647A4`, `sub_08064BA0`, `sub_080671E8`, `sub_08067540`, `sub_0806D1D8`, `sub_08072AF8`, `sub_08072D28`, `sub_080735D4`, `sub_08073784`, `sub_080740BC`, `sub_08075114`, `sub_080788AC`, `sub_08078CC8`, `sub_080794E0`, `sub_0807960C`, `sub_080798B8`, `sub_08079FDC`, `sub_0807AD40`, `sub_0807ADE8`, `sub_0807B6B8`, `sub_0807B864`.
- Wave 3 (77): `sub_08001374`, `sub_08001464`, `sub_080017E8`, `sub_08002388`, `sub_080034B8`, `sub_08003C78`, `sub_08003F88`, `sub_080040E4`, `sub_08004FD8`, `sub_08007D50`, `sub_08008940`, `sub_0800935C`, `sub_08009538`, `sub_080097F0`, `sub_080098C0`, `sub_0800A78C`, `sub_0800CAF0`, `sub_0800CE28`, `sub_08011780`, `sub_080119F8`, `sub_08011CB4`, `sub_080150DC`, `sub_08015720`, `sub_08018690`, `sub_0801919C`, `sub_08028D9C`, `sub_08029B0C`, `sub_08030B88`, `sub_08031550`, `sub_08031780`, `sub_08034768`, `sub_08036254`, `sub_080367E4`, `sub_08036A68`, `sub_080386B0`, `sub_0803C838`, `sub_0803CE90`, `sub_0803D0E8`, `sub_0803D460`, `sub_0803F034`, `sub_08040294`, `sub_0804325C`, `sub_080437CC`, `sub_0804E780`, `sub_0804EFF0`, `sub_0804F7A4`, `sub_08051140`, `sub_08052B78`, `sub_08057C94`, `sub_080580C8`, `sub_080589C8`, `sub_08058EDC`, `sub_0805B184`, `sub_0805D58C`, `sub_0805D708`, `sub_0805DC38`, `sub_0805E1D0`, `sub_0805E3B8`, `sub_0805E788`, `sub_0805EF00`, `sub_0805F270`, `sub_0805F96C`, `sub_0805FD28`, `sub_0805FEA4`, `sub_08061004`, `sub_08061A1C`, `sub_08061D24`, `sub_08063040`, `sub_080636AC`, `sub_080690C4`, `sub_0806B3B0`, `sub_08072A14`, `sub_08076A20`, `sub_08079068`, `sub_08079700`, `sub_08079B88`, `sub_0807C4CC`. The largest is `sub_0806B3B0` (4.4 KB, [[code-0806a92c]]).
- Giants loop (2): `sub_0800ABC8`, `sub_080471E8`.
- After the checkpoint, recorded on [[code-08076144]]: `sub_08076DAC` (wave 3), which also enabled its twin `sub_08076BEC` (commit `33809ed`, 1,912/1,976, 96.76%, 83.53%). Not yet written up on their unit pages: `sub_08068180`, `sub_0802A6DC`, `sub_0804A1C8`, `sub_0802A188`, `sub_08037ED8`. Their rows on [[code-08029750]], [[code-0804a008]] and [[code-08068180]] say the write-up is pending; [[code-0803732c]] is unchanged.
- Unit pages updated. Each got status counts, table rows, wave 2/3 sections with the tricks and FAKEMATCH forms, stale notes marked historical, and `updated: 2026-10-02`; 42 pages went from draft to solid because their unit is now all C: [[code-08000228]], [[code-08001364]], [[code-08002388]], [[code-080034b8]], [[code-080044e4]], [[code-08007994]], [[code-08008a1c]], [[code-08009a68]], [[code-0800ab08]], [[code-0800c894]], [[code-0800eaa8]], [[code-08010bdc]], [[code-08011be0]], [[code-08012c4c]], [[code-080150dc]], [[code-08017314]], [[code-080184d8]], [[code-08027580]], [[code-08028684]], [[code-08029750]], [[code-0802aac0]], [[code-0802fb64]], [[code-08030b88]], [[code-08031bc8]], [[code-08033dac]], [[code-08035198]], [[code-080361d0]], [[code-080383f0]], [[code-08039638]], [[code-0803b670]], [[code-0803c838]], [[code-0803edc4]], [[code-0803fe70]], [[code-08040ebc]], [[code-080431e4]], [[code-08044224]], [[code-08046738]], [[code-08048fe0]], [[code-0804a008]], [[code-0804db6c]], [[code-0804eff0]], [[code-08050a70]], [[code-08051a9c]], [[code-08052b78]], [[code-08056ecc]], [[code-08057ee0]], [[code-0805a30c]], [[code-0805d58c]], [[code-0805e788]], [[code-0805f96c]], [[code-080609c4]], [[code-080619e8]], [[code-080629f0]], [[code-08063a28]], [[code-08064af0]], [[code-0806704c]], [[code-08068180]], [[code-0806a92c]], [[code-0806c4e4]], [[code-08071f40]], [[code-08072fac]], [[code-080740bc]], [[code-080750e0]], [[code-08076144]], [[code-080784e4]], [[code-0807960c]], [[code-0807a6ac]], [[code-0807b6b8]].
- [[matching-tricks]]: new section "CSE, loop and jump mechanics (waves 2-3)". It covers `find_reg` eviction, r7 pin pitfalls, regmove replacement quality, CSE path following and per-mode constants, front-end narrowing of ternaries and IORs, combine's per-block LOG_LINKS, post-reload CSE, LCM destroying a biv, the giv test, what counts toward loop.c's threshold, `break` versus `return` loop rotation, folded literal loop bounds, PRE hash order, jump threading and user variables, `goto` versus `return` before reload, `redirect_jump` survivors, and empty asm statements counting 2 bytes in branch shortening. Citations and new detail were added to existing entries: priority ties, REG_EQUIV doubling, spill set, levers that change refs, the three-quantity local-alloc fix, top-down reload work, cast-constant reloads, cross-jump without labels, switch-result consumption, `switch` versus `if` on bitfields, and ARRAY_REF with a conditional index. The "Failed, partial and rejected" section gained a status note listing the functions there that have since matched; their table rows are marked.
- Contradictions flagged with `> [!warning] Contradiction` (all resolved in favour of the matched source). In [[matching-tricks]]: u32 timer container (`sub_080150DC`); `sub_08030620` base object; `t = player & 1` folding (`sub_08017314`); OAM read-before-test form (`sub_080283BC`); 64-bit `mb` temporary (`sub_08072AF8`); "goto is not an anti-hoist tool" (`sub_08061D24`); `goto inc` (`sub_08040294`). On unit pages: 61 callouts, for example [[code-0800ab08]] (label FAKEMATCH not needed), [[code-08044224]] (non-volatile stats loads in 0x454/0x5EB), [[code-08051a9c]] (the r7/r8 swap was a global-alloc priority tie), [[code-08001364]] / [[code-080034b8]] (records are per-opponent duel records, not card stats), and [[code-080609c4]] (C `struct PlayerState` is 0xDC4 bytes against the ROM stride 0xD64).
- [[agent-tooling]]: a score of 0 is not a match (normalisation drops branch targets); the `park` / `base.c` gotcha; Docker outages printed `score: 0 (MATCH)`; shared-scratchpad helper collisions. [[overview]]: progress, giants left, next-largest functions.
- Reported for cleanup, outside the wiki: `struct Unk02017E20` is unused in `src/code_0802FB64.c`; `src/code_08039638.c` has a local `EffState` with a 0x14-byte CardRef (compiled 0x10); `src/code_080361D0.c` declares `sub_08019820(int)` but the callee is `(int, u16)`; the `sub_08000AC8` prototype differs between units; `wf.py check` should fail when Docker or the compile fails. `sub_0800257C` ([[code-08002388]]) has been C since 2026-09-30 but its row is still stale.
- `scripts/wiki_lint.py`: no broken links, orphans, index gaps or frontmatter problems.

## [2026-10-02] progress | Asset pipeline complete: 83 assets, all editable and round-tripping
- Commit `2cf1b05` (with the plugin mechanism from `4a58b22`): every non-code byte now extracts to an editable format and builds back byte-identical. There are 83 assets, `fallback.txt` is empty, and only the 156-byte Nintendo logo stays raw. Six plugins in `tools/assetfmt/`: `sound_samples`, `sound_seq`, `gfx_scenes`, `gfx_banks`, `tables_game` and `tables_code`. A fresh clone with only the ROM ran `make setup` and `make compare` (`eds.gba: OK`) and still matched after the ROM was deleted. Re-run for this entry: `tools/assets.py check` (83 assets) and `verify` (83/83). Sources: `build/assetwf/<family>/NOTES.md`.
- [[assets]] rewritten:
  - the plugin mechanism (`register(A)`, `EDS_ASSET_MANIFEST` / `EDS_ASSETS_DIR` / `EDS_ASSETS_OUT`, `--only`);
  - one table per format family (asset, files, how to edit, constraints);
  - verification and edit tests;
  - limitations: fixed slots, raw cross-asset pointers, and tool gaps (`--only` rewrites `fallback.txt`, stale files are not removed, `out_path` keeps the extension);
  - future work: relocatable layout, merged table+track assets.
- New pages:
  - [[sound-sequence-format]]: the SE and song tables, every SE and BGM opcode, the loop mechanism and quirks, and the four driver lookup tables with formulas.
  - [[scene-sets]]: the descriptor, set-to-character table, ROM order, OBJ tile placement and palette banks, animation lists, and the dialogue box and header strip.
- Data folded in:
  - [[sound-engine]]: data section rewritten; sample rate (9,998 Hz at pitch 0), bank statistics, amplitude headroom, wave-RAM patterns, noise presets, `lockTicks` and the +0x195 lock counter; BGM per opponent. Open questions on the bytecode and the channel assignment answered.
  - [[graphics-formats]]: 133 image packs with the 4bpp loaders, the pack encoder rules, the sprite animation stream, 9 Mode-4 bitmaps, OBJ-mapping and bank findings, `scan_objpack.py` caveats. New "The original compressor" section (an Okumura `LZSS.C` variant that reproduces all 59 streams). Scene-set detail moved to [[scene-sets]].
  - [[lzss-decompress]]: a note on the encoder.
  - [[rom-map]]:
    - `.rodata` 1 contents (sine table `0x08087BA4`, 320 values; animation step arrays; Shift-JIS debug strings; hiragana table; unreferenced items);
    - exact sound row boundaries;
    - the scene/list range contents (fusion lists, calendar events, BGM per opponent, image-pack lists);
    - effect-table fields, pointer-table contents and deck header;
    - a new `.rodata` 2 sub-table list;
    - sound lookup tables split into four;
    - graphics rows with item counts and bank contents (36 booster covers, delete-save and calendar bitmaps, 8bpp OBJ tiles).
    - Two open questions answered.
  - [[deck-lists]]: header struct, editing (0x920-byte budget, re-pack), starter-pool field bits 25–31.
  - [[password-table]]: `cards/passwords.csv`, and the out-of-bounds read `gUnk_08623326` (`gCardIdToNumber[0x439]`) landing on Curse of Fiend's password.
  - [[special-card-lists]]: the Forbidden/Limited list `0x081A78B4` (47 entries, limits 0/1/2), the AI scan list `0x0819DD64`, the fusion recipe lists `0x0819A7C8` (52) and `0x0819A970` (3).
  - [[function-pointer-tables]]: named step tables from the converters.
  - [[booster-packs]]: 36 cover slots, 23 used and 13 unreferenced (re-checked); pack layout confirmed by re-pack.
  - [[cards]]: effect handler fields and slot names.
  - [[rom-header]]: `header.json` and the `"auto"` checksum (formula re-checked).
  - [[duelist-table]]: portraits and duel BGM.
  - [[text-system]]: the dialogue terminator record.
- Contradictions flagged with `> [!warning] Contradiction`, all resolved in favour of the decoded, round-tripping data:
  - [[sound-engine]]: pitch table "u16 per note" → per 1/32 semitone, starting at `0x081A8A0C`, not `0x081A8D48`; SE halfword "flags", "about 48 entries" → `lockTicks`, exactly 48.
  - [[graphics-formats]]: the size+1 LZSS streams end in a lengthened final match, not a padding literal; card frames are 13×18 tiles, not 12×12.
  - [[scene-sets]]: the `anim` pointer targets a track list in the scene/list range, not ".rodata 2".
  - [[deck-lists]]: `u32 count` → `u16 count; u16 pad` (matched source).
  - [[rom-map]], sound rows: bank-1 PCM table 28 entries, not 26; song data ends at `0x0811B417`, not `0x0811B415`; `0x08139F50` is the noise preset table.
  - [[rom-map]], `.rodata` 2: the three "0x2800 tilemap" blocks are HBlank warp tables (the third is 0x1400); the sound lookup range is four tables.
  - [[function-pointer-tables]]: the 77-, 29- and 22-entry runs each span several step tables.
  - [[text-system]]: three Shift-JIS debug strings and custom-glyph strings contain bytes ≥ 0x80.
- Verified against the ROM for this entry:
  - the pitch and PSG-frequency formulas (all values; the frequency table needs the exact C2);
  - sine truncation (both tables);
  - deck header pads;
  - SE table count, song and SE data ends, bank-1 NULL entries, noise values;
  - header checksum;
  - fusion terminators;
  - card-frame cell extent;
  - booster cover references.
- Also updated:
  - [[sound-driver]]: `sub_0807D6B4` and `sub_0807DB58` now match (commits `acd0f2c`, `01c08ce`), 30/30 C; write-up pending.
  - [[decomp-workflow]], [[rom-free-workflow]]: the build no longer needs the ROM after `make setup`.
  - [[open-questions]]: three answered; new ones on unreferenced data, unused driver tables and the hiragana table.
  - [[overview]]: 1,975/1,976 functions with only `sub_08044224` left; assets complete; milestones.
  - [[index]]: the new pages.
- `scripts/wiki_lint.py`: no broken links, orphans, index gaps or frontmatter problems.

## [2026-10-02] setup | Emulator and xref analysis tools
- Two analysis tools for the readability pass (commit `c441483`), written up from their authors' READMEs (`build/emu/README.md`, `build/xref/README.md`, local only):
  - [[emulator]] (new): `tools/emu.py` and `docker/emu/`, a headless mGBA 0.10.5 harness with Lua and a GDB stub. The page covers setup, every command with an example, the six-state savestate library (`title` to `duel_turn2`), outputs under `build/emu/` (gitignored; the ROM never leaves the machine), the author's verification, findings from those runs (LP write path, duel phase byte sequence, IRQ counts; phases 6–8 are hypotheses) and limitations.
  - [[xref]] (new): `tools/xref.py`, a static cross-reference database built from `build/eds.elf`. The page covers build and rebuild rules, every command with an example, how to read a `func` card, how it works, verification (selftest 16/16, 2940/2940 globals, 1936/1960 parameter counts) and limitations.
- Re-checked for this entry: `python3 tools/xref.py selftest` gives 16 PASS. `tools/emu.py sym` and `states list` run, and all six states report `ok`. `build/eds.elf` is unstripped, with 16,694 symbols.
- Also updated:
  - [[agent-tooling]]: Analysis table rows for both tools, and a Ghidra note (not set up on purpose; `build/eds.elf` loads directly).
  - [[decomp-workflow]]: check names with [[xref]] and the [[emulator]] before proposing them.
  - [[overview]]: tooling bullet; milestone 2 notes that the readability pass has started.
  - [[index]]: the two new pages under Tools.

## [2026-10-02] ingest | Japanese ROM mapping (jpmap)
- Source: the read-only analysis of the Japanese ROM (AY5J) with `tools/jpmap.py` (commit `446546b`) and its outputs in `build/jp/` (local only): `summary.txt`, `PLAN.md`, `map.tsv`, `units.tsv`, `blocks.tsv`, `link_order.tsv`, `ram_map.tsv`, `romdata_map.tsv`, `assets.tsv`, `compile_test.txt`, `sub_08044224_diff.md`.
- Findings: 158 USA functions identical modulo relocation, 41 same shape with different constants, 647 changed, 1130 without a counterpart; 1290 JP-only functions, 195 of them a Mobile Adapter GB library (`MAGB`, `gameboy.datacenter.ne.jp`) linked after AgbSram. Same compiler and flags (10/10 recompiled USA functions byte-identical at JP addresses). Different duel data model (`u16` card words, small player blocks, 928 cards). Link order shares 428 runs in chance-level order. RAM deltas for `oamBuffer`, the link block, the sound driver and the save mirror. 5 + 21 of 83 asset ranges shared.
- Pages:
  - [[rom-versions]]: rewritten comparison section (layout, function mapping, compiler identity, data model, shared units, link-order runs, RAM shifts, asset overlap, crt0/ARM/libgcc/libc differences) and the staged JP plan with estimates, dependencies and risks. The 2026-10-01 numbers are kept as a superseded measurement.
  - [[jpmap]] (new): usage, outputs, method, verification, limitations.
  - [[agent-tooling]]: Analysis table row for `tools/jpmap.py`.
  - [[overview]]: Current state bullet; milestone 5 now describes JP as a separate effort sharing about 200 functions, with stages 1–3 startable now.
  - [[game-overview]]: corrected the JP sentence.
  - [[open-questions]]: Mobile Adapter library compiler; rewritten no-counterpart functions; JP IWRAM −0x10; JP card order vs the kana sort keys; link-order runs as TU-boundary evidence.
  - [[index]]: [[rom-versions]] summary updated, [[jpmap]] added under Tools.
- Contradictions flagged with `> [!warning] Contradiction`, both resolved in favour of jpmap (`build/jp/PLAN.md` §6):
  - [[rom-versions]]: "JP game code ends earlier, about 0x1BB4 bytes less code" is wrong; `.text` runs on through the Mobile Adapter library to `0x08089B50`, about 37 KB more than USA.
  - [[rom-versions]]: the hypothesis that localisation changing RAM and struct layouts explains the low exact-match share is refuted; the game logic and data model were reworked.
  - [[game-overview]]: "built from the same source with … shifted data layouts" (same cause).
- Verified for this entry (read-only Python on both ROMs): JP SHA-1 and header; crt0 literal `0x2008` at JP `0x08000224` vs USA `mov r1,#0x2280` at `0x080001C8`; `MAGB` at JP `0x0807FFF8` and `gameboy.datacenter.ne.jp` at `0x0809ED97`, neither in USA; APCS `mov ip, sp` at JP `0x0805CA04`. Status, confidence and JP-only counts recounted from `map.tsv`; longest runs from `blocks.tsv`.
