# EDS decomp and its LLM wiki

This project is a **matching decompilation** of *Yu-Gi-Oh! The Eternal Duelist Soul* (GBA, USA, `AY5E`).
Alongside the code, Claude maintains a persistent **LLM wiki** in `wiki/`, which gathers everything we learn about the ROM, the game, and the toolchain.

Claude writes and maintains the wiki. The human curates sources, directs the work, and asks questions.

## Build (all in Docker; never install toolchains on the host)

```
docker build -t eds-decomp docker/          # once
tools/dr make setup                         # once: check the ROM in roms/, extract assets/
tools/dr make -j8 compare                   # full build + SHA-1 check (eds.gba: OK)
tools/dr python3 tools/check.py <unit>      # per-unit match check (safe to run concurrently)
tools/dr python3 tools/check_all.py         # every unit (the CI gate)
```
`check.py`, `check_all.py` and `permute.py` also work without the ROM, taking their targets from the original
assembly (`tools/target.py`). See `wiki/tools/rom-free-workflow.md`.
Without Docker (e.g. web sandboxes): `tools/setup_native.sh`, then `. ~/.eds-tools/env.sh`; `tools/dr` then runs commands directly.
Code lives in units (`units.txt`): `src/<unit>.c` with `INCLUDE_ASM` for unmatched functions, or `asm/<unit>.s`.
The decomp workflow, conventions and matching tips are in `wiki/concepts/decomp-workflow.md`.
Naming convention for decompiled symbols: pret-style (`CamelCase` functions, `gCamelCase` globals, `sub_08XXXXXX`/`gUnk_0XXXXXXX` until named).

## Matching rules (for every contributor, human or agent)

- **No ROM, ever.** Never commit, upload, download or ask for the game image or its data. The original code of
  every function is in `asm/nonmatching/<unit>/<func>.s`, and the tools compare compiled C against it.
- Unmatched functions are `INCLUDE_ASM` lines, often with a parked attempt above them in
  `#if 0 /* NONMATCHING: what differs */ ... #endif`. Check with
  `tools/dr python3 tools/check.py <unit> --diff <func> --norm`. A function counts only when the whole unit
  reports `unit bytes MATCH`; otherwise put the `INCLUDE_ASM` back and park the best attempt.
- Near misses: `tools/dr python3 tools/permute.py <unit> <func> --run -j 2 --minutes 20`.
- Ordinary C is preferred. Byte-identical **FAKEMATCH** forms are accepted when commented `/* FAKEMATCH: why */`:
  the permuter's odd-but-valid C, `register T x asm("rN")`, empty `asm("" : "+r"(x))` constraints. Hand-written
  instructions inside `asm()` are not accepted.
- Keep placeholder names (`sub_08XXXXXX`, `gUnk_0XXXXXXX`); other units and the build depend on them. Declare
  prototypes, externs and structs locally in the unit; reference ROM data and RAM through address-suffixed
  `extern`s (no string literals, `const` tables or defined globals in C). One unit per agent or PR.
- CI (`.github/workflows/progress.yml`, no ROM): `check_all.py` must pass, a PR may not lower the number of
  matching functions, and the objdiff report feeds decomp.dev. `make compare` needs the ROM and is run locally.

## Layout

```
roms/      base_eng.gba (USA AY5E, SHA-1 510fbba2…) and base_jp.gba (Japan AY5J, SHA-1 dc25f733…)
           (never modify; never commit). baserom.gba is a symlink to roms/base_eng.gba.
assets/    Game data extracted from the ROM by `make setup` (never committed; see wiki/tools/assets.md)
raw/       Immutable sources: notes, disassembly dumps, forum posts, docs, screenshots
  notes/   Human-written session notes and dumps to ingest
  assets/  Images and other binaries referenced by sources
wiki/      LLM-owned markdown (Obsidian-compatible)
  index.md       Catalog of every page. Read this first for any query.
  log.md         Append-only chronological log
  overview.md    Top-level synthesis: project state, what we know, what's next
  rom/           Facts about the ROM image: header, memory map, sections, save type
  game/          Game-level knowledge: systems, cards, duel engine, menus, text
  concepts/      General GBA / decomp concepts (agbcc, Thumb, matching, etc.)
  tools/         Toolchain and utilities
  functions/     One page per notable function (or a cluster of related ones)
  data/          One page per data table/struct (card table, deck lists, etc.)
  sources/       One summary page per ingested raw source
  questions/     Open questions, hypotheses, and answered queries worth keeping
templates/  Page templates
scripts/    Wiki helper scripts (e.g. wiki_lint.py)
```

## Page conventions

- Use Obsidian wikilinks, such as `[[rom-header]]`. File names are kebab-case and unique across `wiki/`.
- Every page starts with YAML frontmatter:
  ```yaml
  ---
  title: Human Title
  type: rom | game | concept | tool | function | data | source | question | overview
  status: stub | draft | solid | verified
  confidence: low | medium | high      # how sure we are of the claims
  sources: [source-page-names or "rom-analysis"]
  updated: YYYY-MM-DD
  ---
  ```
- **Addresses:** always write ROM addresses as `0x08xxxxxx`, EWRAM as `0x02xxxxxx`, IWRAM as `0x03xxxxxx`. A ROM *file offset* is written as `ROM+0xNNNNN`. Keep the two forms distinct.
- **Symbols:** use the project's naming scheme (pret-style, see Build). Put unnamed items in the form `sub_08XXXXXX` / `gUnk_02XXXXXX`, and rename them across the wiki when a real name is decided.
- **Evidence:** separate *verified* claims (checked against the ROM or matched code) from *hypotheses*. Mark a hypothesis inline with `> [!question]` or "(hypothesis)". Never state a guess as fact.
- **Contradictions:** when new information conflicts with a page, don't silently overwrite it. Add a `> [!warning] Contradiction` callout that cites both sources, then resolve it or file it in `questions/open-questions.md`.
- Function pages record: address, size, mode (ARM/Thumb), callers/callees, match status (`nonmatching` / `matching` / `equivalent`), and known tricks needed to match.

## Workflows

### Ingest (a new file lands in `raw/`, or the user pastes findings)
1. Read the source fully, then briefly discuss what it shows with the user.
2. Write `wiki/sources/<name>.md` as a summary with a link back to the raw file.
3. Update or create the affected rom/game/function/data/concept pages. A single source often touches many pages.
4. Flag contradictions, and add new open questions.
5. Update `wiki/index.md` and `wiki/overview.md` if the big picture changed.
6. Append to `wiki/log.md`.

### Decomp progress (functions matched, data identified, files split)
- Create or update the `functions/` or `data/` page with the match status and the tricks used.
- Update `overview.md` progress notes, then append to the log.

### Query
1. Read `wiki/index.md`, then the relevant pages. Answer with `[[links]]` as citations.
2. If the answer is reusable (an analysis, a comparison, a derived table), file it as a new page in `questions/` or the right section, and log it.

### Lint (on request, or roughly every 10 log entries)
- Run `python3 scripts/wiki_lint.py` to find broken links, orphans, missing frontmatter, and index gaps.
- Also review manually for stale claims, contradictions, concepts that are mentioned but have no page, and hypotheses that can now be checked against the ROM.
- Log the pass.

## Log format

Append-only. Each entry starts with:
```
## [YYYY-MM-DD] <ingest|query|lint|progress|setup> | <short title>
```
followed by bullets listing the pages touched. To see recent activity, run `grep "^## \[" wiki/log.md | tail -5`.

## Rules
- Never modify the baserom or anything in `raw/`.
- Never commit ROMs or copyrighted game assets to git.
- Verify claims against the ROM with quick scripts whenever it's cheap to do so, and record the verification method on the page.
