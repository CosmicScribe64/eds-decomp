# Agent guide

How to work on this matching decompilation as an automated agent (Codex, Claude Code on the web, or
any other GitHub-connected agent). Humans are welcome to follow it too.

## The one rule about the game
**No ROM, ever.** Never commit, upload, download or ask for the game image, its data or its assets. You
don't need them. The original machine code of every function is in the repository as assembly
(`asm/nonmatching/<unit>/<func>.s`), and the tools compare your compiled C against it.
`.gitignore` already excludes `*.gba`, `*.zip`, `roms/` and `build/`.

## Setup
- **With Docker:** `docker build -t eds-decomp docker/`. Then prefix every tool with `tools/dr`, which runs it
  in the image with the repo mounted.
- **Without Docker** (most web sandboxes): `tools/setup_native.sh` (about 5 minutes; builds pret/agbcc), then
  `. ~/.eds-tools/env.sh`. `tools/dr` then runs commands directly.

## Workflow
The code is split into units (`units.txt`). Each unit is `src/<unit>.c`; functions that don't match yet are
`INCLUDE_ASM("asm/nonmatching/<unit>", <func>);` lines, usually with a parked C attempt directly above them in
`#if 0 /* NONMATCHING: what differs */ ... #endif`.

1. Read `wiki/concepts/decomp-workflow.md`, `wiki/concepts/matching-tricks.md` (the catalog of what has worked)
   and your unit's page `wiki/functions/<unit-with-dashes>.md`.
2. Convert one function at a time: replace its `INCLUDE_ASM` with C (or enable the parked draft), then
   `tools/dr python3 tools/check.py <unit> --diff <func> --norm`.
3. A function counts only when `check.py` reports `match` **and** the whole unit reports `unit bytes MATCH`.
   If it won't match, put the `INCLUDE_ASM` back and keep your best attempt under `#if 0 /* NONMATCHING: ... */`.
4. Near misses: `tools/dr python3 tools/permute.py <unit> <func> --run -j 2 --minutes 20` (one run at a time). On
   score 0, apply `build/permuter/<func>/output-0-*/diff.txt` by hand and re-check.
5. Before opening a PR: `tools/dr python3 tools/check_all.py` must end with all units matching.

## Accepted matching techniques
- Ordinary C is preferred. Byte-identical, behaviour-neutral **FAKEMATCH** forms are accepted when commented
  `/* FAKEMATCH: why */`. These include the permuter's odd-but-valid C (redundant tests, temporaries, casts),
  `register T x asm("rN")` bindings and empty `asm("" : "+r"(x))` constraints.
- **Not accepted:** hand-written instructions inside `asm()`, or anything that only matches in isolation.
- Whole-unit byte equality is the acceptance test; behavioural test suites aren't required.

## Conventions
- Keep the placeholder names (`sub_08XXXXXX`, `gUnk_0XXXXXXX`): other units refer to them, and the build
  resolves `_XXXXXXXX` suffixes to addresses. Propose real names on the unit's wiki page.
- Declare prototypes, externs and structs locally in your unit; don't edit shared headers (`include/`) unless
  that is the task. No string literals, `const` tables or defined globals in C: reference ROM data and RAM
  through `extern` declarations with address-suffixed names.
- One unit per agent or PR, to avoid conflicts. Don't touch the Makefile, `units.txt`, `config/`, `asm/` or the
  tools unless that is the task.
- Keep the wiki (`wiki/`) in step with the code. Update your unit's page (function table, match status, tricks
  used) and add an entry to `wiki/log.md`. The wiki's rules are in `CLAUDE.md`.

## CI checks
1. `tools/check_all.py`: every unit still matches its original bytes.
2. On pull requests, the number of functions in matching source must not go down.
3. The objdiff progress report for decomp.dev.

CI runs `.github/workflows/progress.yml` without a ROM. The full-ROM check (`make compare`, which also covers
data and the header) needs the ROM, so the maintainer runs it locally after merging.
