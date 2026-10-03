---
title: Agent tooling
type: tool
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Agent tooling

Scripts used to run AI agents on the decomp in parallel.

> [!note] Policy history
> Earlier on 2026-10-01 the user asked to conserve usage and the lead worked solo. Later that night the user
> turned on multi-agent workflows ("Don't stop until we have a full match"). Work now runs as Claude Code
> Workflow waves: one agent per function (or twin family), each in a private working copy made by
> `tools/wf.py`. The OpenCode/Codex launchers below are historical.

## Private working copies (`tools/wf.py`, 2026-10-01)
Several agents can now work on functions of the **same unit** at once. Each gets a whole-unit copy in
`build/wf/<func>/unit.c` with its function enabled between `WF-BEGIN`/`WF-END` markers; only `apply` and
`park` touch `src/`, under a per-unit `mkdir` lock, through a 3-way `git merge-file` against the copy's
base, and they keep the result only if the whole unit still matches.

| Command (host, from the repo root) | What it does |
|---|---|
| `python3 tools/wf.py prep F` | Make the working copy; print the starting score. |
| `python3 tools/wf.py check F [--ctx N]` | Compile the copy (`check.py --src`), print the normalized diff and the score. |
| `python3 tools/wf.py score F` | Print only the score line. |
| `python3 tools/wf.py perm F --minutes M -j J` | [[decomp-permuter]] on a snapshot of the copy (`permute.py --src`). |
| `python3 tools/wf.py dump F [-dg -dl ...]` | agbcc RTL dumps of the copy into `build/wf/F/dump/` (default `-dg -dl -df`: `.greg` global-alloc priorities, `.lreg` local-alloc, `.flow` live lengths; `-dL` loop, `-dJ` cross-jump, `-da` all). How to read them: [[matching-tricks#Register allocation priority and reload rotation]]. |
| `python3 tools/wf.py apply F` | At score 0: merge into `src/<unit>.c`, verify `unit bytes MATCH`, else restore. Rejects asm() with instructions. |
| `python3 tools/wf.py park F "note"` | Store the copy as the `#if 0 /* NONMATCHING (score N): note */` draft if it beats the starting score. |

Score = differing normalized lines + 4 x |size delta| (the same metric as the scout in
`build/solo-s49/scout2.py`). `check.py --src FILE` and `permute.py --src FILE` are the underlying options.
The size delta does not count the zero `.align 2, 0` pad that the function table includes after a function ending
on a 2-byte boundary (agbcc's `.size` excludes it). Before 2026-10-01 that pad showed as `-2 bytes`, held such
functions at score 8, and made `apply` refuse them ([[code-08019554]], `DiscardHandCardByNumber`).
Agents also write `build/wf/<func>/NOTES.md`; the lead folds those into the unit pages after each wave (waves 1-2 on 2026-10-01, waves 2-3 and the giants on 2026-10-02: see [[log]]).

**Register differences.** Since 2026-10-02, [[regoracle]] reads the allocation from a traced compiler instead of
hand-read dumps. It runs on the `wf.py` copy by default. Its output directory is fixed
(`build/regoracle/<func>/`), so parallel workers on one function need private copies of the tool, as the
[[code-08044224]] region workers made.

**Region-split rounds (2026-10-02).** The last function, [[code-08044224]], matched in one round with six
workers on private whole-unit copies. Each worker owned a group of case regions, scored by a per-case diff
(`rdiff.py`) plus the global `check.py --norm` line. An integrator then applied the patches in score order onto
copies and moved the result into `src/` with `wf.py apply`. A size error in one region shifts the absolute
jump-table words of every later switch. Those diffs clear only after integration, so workers should not chase
them.

**Score 0 is not yet a match.** Normalisation drops branch targets, so a function can score 0 while some of its branches still go to the wrong block. `apply` then refuses it, because the whole-unit byte check fails. `CardMenu_SummonMonster` reached score 0 with three wrong branch targets (a threaded switch arm and two cross-jumped tails, [[code-08046738]]). Before trusting a 0, read the raw diff: `tools/dr python3 tools/check.py <unit> --src build/wf/<func>/unit.c --diff <func>` without `--norm`.

**Parking twice.** `park` merges three ways against the copy's `base.c` and does not refresh `base.c` afterwards, so a second park of the same function conflicts with the first. Agents on the giants worked around this by copying `src/<unit>.c` to `base.c` after each park, once a diff showed only their draft block had changed (`syncbase.py` for `DuelPhase_Standby`, `park.sh` for `CollectEffectTargets`, which also checks that nobody else touched the unit).

**Pitfalls reported by wave 2-3 agents (2026-10-01/02):**
- While the Docker daemon was down, `wf.py check` printed the connection error and then `score: 0 (MATCH)` (`TextBoxDrawText`, `EffectCyberJarResolve`). Checks made at that time were not re-confirmed, so treat a 0 printed next to an error as a failed build. `apply` still re-checks the whole unit. `wf.py score` has also printed `score: None` (`DrawHandCards`).
- Helper scripts with common names in the shared scratchpad (`try.sh`, `put.py`) were overwritten by other agents. At least two runs wrote into another function's `build/wf/<func>/unit.c` (`UpdateMonsterControl`/`LinkSioSend`, `DuelCursor_FindTargetHorizontal`/`DuelCmd_PrepareBattlePhase`, and stray `WF-END` lines in `build/wf/DuelCmd_PrepareBattlePhase/`). Keep per-function helpers in `build/wf/<func>/`.
`apply` accepts empty `asm("" : ...)` constraints and `register ... asm("rN")` bindings but rejects any other asm string, including `asm("gUnk_...")` / `__asm__` symbol-alias declarations inside the markers; wave agents used a cast of an existing symbol or a function-pointer cast macro instead (`DeckEdit_UpdatePanelHighlight`, `DeckEdit_CountSideDeckMonsters`, `TradeCardSelect_Update`, `ProhibitCardSelect_Update`).

## Running agents
| Tool | What it does |
|---|---|
| `tools/opencode-agent [--model M] "<prompt>"` | One OpenCode agent in a Docker sandbox (image `eds-opencode`) that sees only the repo. It gets only the OpenRouter credential. Default model: DeepSeek v4.1 flash. |
| `tools/launch_fresh.sh <unit> [0x400] [logdir]` | Fresh-function task: undrafted functions under the size limit, starting from the [[m2c]] drafts. |
| `tools/launch_cleanup.sh <unit>` | Cleanup task: parked `#if 0` drafts, then pending permuter results, then undrafted functions. |
| `tools/launch_headers.sh <unit>` | Migrate a unit to the [[shared-headers]] without changing any output byte. |
| `staging/OPENCODE_*_PROMPT.txt` | Prompt templates (fresh, cleanup, headers, siblings, tricks). |
| `tools/watch_agents.sh` | Blocks until an agent container exits (used to wake the coordinator). |
| `tools/repair.py <unit>` | After an interrupted agent: parks every non-matching function back to `INCLUDE_ASM` (C kept under `#if 0`) until the unit matches. |
| `tools/dashboard.py` | Local live dashboard on port 8765. It shows each agent's role, unit, live command (from `docker top`), recent actions and last check, plus the permuter batch, progress and the Codex window. |

## Helpers the agents call
| Tool | What it does |
|---|---|
| `tools/similar.py <func>` / `--all` | Matched functions with the same normalised instruction pattern, i.e. siblings (`build/similar.tsv`). |
| `tools/advisor.py --unit U --func F "question"` | Asks a stronger model about a stuck function. `ADVISOR_BACKEND=pro` (deepseek-v4-pro) or `claude` (queued to `build/advisor/`, answered by a Claude subagent following `staging/ADVISOR_BRIEF.md`). |
| `tools/permute.py` / `tools/permute_batch.py` | [[decomp-permuter]] jobs and the background batch. |
| `tools/m2c_draft.py` | First drafts from the [[m2c]] decompiler. |

## Current continuation batches

`tools/dr python3 tools/decomp_queue.py` writes `build/decomp-queue/queue.json`
and `queue.md`. It uses the same source-status rule as `tools/progress.py`, checks
that every fallback occurs in exactly one batch, and groups up to eight functions
per unit. The JSON records old draft discrepancy notes, matching-pattern
categories, related callees, and relevant wiki pages. Draft notes can be stale;
check the current object before selecting a trick.

Read [[matching-tricks]], the unit page and related wiki tricks first, including failed sweeps. Use
the recorded sibling search to reuse matched source shapes before translating
from raw m2c. Prioritize related predicates, DMA sequences, state-machine tails,
or decoder commands so that discoveries about layout, width and ABI carry across the batch.
Keep candidate compilation isolated, accept conversions only after a whole-unit
ROM-byte check, and run the combined ROM check at settled checkpoints. Coordinate
ownership by source unit; do not let two workers rewrite the same file.

Start with small grids of source shapes, then short focused permuter runs when
appropriate. Record the source/compiler context and remaining discrepancy; avoid
repeating unchanged failed variants. Reuse prepared contexts and compile candidate
loops inside one Docker invocation. Keep a larger-function stream alongside quick
sibling wins, because remaining byte coverage lags the function count. The catalog
contains the complete batch recipe and the limits of prior experiments.

Work now continues with the lead alone, reusing `eds-decomp:latest` through
`tools/dr`. All delegated units have been released to the lead. Historical
external advisor/OpenCode launchers remain stopped. The authoritative policy,
current ownership and preserved former assignments are in
`build/lead-coordination.json`; queue owner labels are historical classifications.
Supporting final reports are collected in
`build/codex-continue/support-final-handoff.md`. Local final reports are linked
from the coordination map. No worker left a new enabled conversion after the
last checkpoint; their bounded failures and reusable drafts are documented.

The lead owns source changes, full ROM checks, objdiff reports and global wiki
updates. Every checkpoint reports both function and byte coverage. Checkpoint 48
is 1,685/1,976 functions (85.27%) and 57.72% validated code; the source counter is
57.76%. Its 98 queue batches contain 261 parked drafts and 30 raw references,
with none missing a starting reference. Logs: `build/lead-pass38/`.

The lead also privately compiled 112 unassigned parked routines of at least
0x180 bytes. `build/lead-scout/ranked.json` records current size and byte
differences; the score is a prioritization aid, not semantic evidence or coverage.
Assignments from that survey yielded exact `CanSummonFromHand`, `TurnOrder_DrawHandCarousel` and
`EffectMagicalHatsResolve`, then `SummonAction_Update`, `SideDeckSwap_Init`, `CardTrading_InitVideo`,
`EffectAttackResponseResolve` and `CardListView_DrawCardStatus` (6,124 bytes across both checkpoints), with original ABI/source review and full-unit checks.
A separate 89-variant local-width survey found no additional exact candidate;
its unchanged forms need not be repeated (`build/lead-width-scout/results.json`).
A further 31 initialized-input variants on close unassigned drafts also found
no exact result (`build/lead-join-scout/results.json`). Successful hints are
function-specific; broad application does not replace source/assembly analysis.

## Analysis
| Tool | What it does |
|---|---|
| `tools/typesurvey.py` | How units declare shared globals and structs. |
| `tools/structmap.py <addr>` | Per-field byte/bit offsets of a global across all units (agbcc layout rules). |
| `tools/mkheader.py` | Drafts a canonical struct from those maps. |
| `tools/fnptr_tables.py` | Function-pointer tables in ROM data ([[function-pointer-tables]]). |
| `tools/objdiff_resolve.py`, `tools/mkobjdiff.py`, `tools/check_report.py` | The [[objdiff]] progress report (decomp.dev format). |
| `tools/xref.py func\|global\|strings\|graph\|unit\|subsystems\|what` | Static cross-references from `build/eds.elf`: what a function touches, who uses a global, call trees, and candidate subsystems ([[xref]]). Used to propose names in the readability pass. |
| `tools/emu.py run\|trace\|watch\|break\|peek\|lua\|gdb\|compare\|states` | Headless mGBA 0.10.5 (image `eds-emu`) with a savestate library: function traces, watchpoints with backtraces, register logs, Lua and GDB ([[emulator]]). The run-time evidence for names. |
| `tools/regoracle.py <unit> <func> [--show LO HI] [--prio R=P]` | Register-allocation oracle. A patched old_agbcc (byte-identical code) traces every allocation decision. The tool lists the pseudos that sit in a different register than in the ROM and says why. It then inverse-solves the global-alloc order into refs/live levers and checks the answer by recompiling. Defaults to the `wf.py` copy `build/wf/<func>/unit.c` ([[regoracle]]). |
| `tools/jpmap.py [--compile-test] [--assets] [--diff F]` | Maps every USA function, RAM address and asset range to the Japanese ROM (AY5J): status per function, JP-only functions, shared link-order runs, RAM/data deltas, and a recompile of USA C at JP addresses. Read-only; writes `build/jp/` ([[jpmap]], [[rom-versions]]). |

**Ghidra** is not set up, on purpose. The matched C, [[xref]] and the [[emulator]] cover the analysis
this project needs. Anyone who wants to browse can load `build/eds.elf` into Ghidra directly. It is
an unstripped ARM ELF that carries every symbol (16,694 `.symtab` entries on 2026-10-02).

## What worked (2026-09-30)
Results by approach. Every unit was verified MATCH after every run.

| Approach | Result |
|---|---|
| Sibling sweep | 6/10 matched at ≥90% similarity; more at 80–90% were in progress |
| m2c fresh drafts | Mixed: 0–3 per unit, e.g. `CheckFusionRecipe3` (0x1D4 bytes) on the first try |
| Permuter batch | 26/94 at 5 minutes each |
| Header migrations | Every unit stayed MATCH; most needed 0 local views |
| Claude advisor | 2 verified fixes and 5 partial from 10 answers, about 0.8% of the 5-hour window per answer |
| deepseek-v4-pro as an agent | 0 matches in 3 runs |
| Targeted hoisting-trick sweep | 0/14; those drafts are register allocation at heart |
