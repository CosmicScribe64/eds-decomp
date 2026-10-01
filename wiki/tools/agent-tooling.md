---
title: Agent tooling
type: tool
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Agent tooling

Scripts used to run AI agents on the decomp in parallel. On 2026-10-01 the user
asked to conserve usage, so all six workers and the supporting coordinator
finished their current tasks and stopped. The lead is now the only active
decomp agent. **Do not restart workers or create replacements.** This policy
supersedes the historical restart instructions in `staging/ORCHESTRATION.md`.

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
Assignments from that survey yielded exact `sub_08054398`, `sub_08027D34` and
`sub_080334E4`, then `sub_08055728`, `sub_0806A9AC`, `sub_0807CDB4`,
`sub_080358AC` and `sub_0802A4CC` (6,124 bytes across both checkpoints), with original ABI/source review and full-unit checks.
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

## What worked (2026-09-30)
Results by approach. Every unit was verified MATCH after every run.

| Approach | Result |
|---|---|
| Sibling sweep | 6/10 matched at ≥90% similarity; more at 80–90% were in progress |
| m2c fresh drafts | Mixed: 0–3 per unit, e.g. `sub_0803CC18` (0x1D4 bytes) on the first try |
| Permuter batch | 26/94 at 5 minutes each |
| Header migrations | Every unit stayed MATCH; most needed 0 local views |
| Claude advisor | 2 verified fixes and 5 partial from 10 answers, about 0.8% of the 5-hour window per answer |
| deepseek-v4-pro as an agent | 0 matches in 3 runs |
| Targeted hoisting-trick sweep | 0/14; those drafts are register allocation at heart |
