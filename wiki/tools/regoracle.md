---
title: Register-allocation oracle (tools/regoracle.py)
type: tool
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Register-allocation oracle (`tools/regoracle.py`)

`tools/regoracle.py` explains register differences between a C draft and the ROM. It reports which pseudos agbcc put
in a different register than the ROM did, and why. Possible causes are global-alloc order, preferences, conflicts,
local-alloc and reload rotation. For differences that the allocation order can fix, it also says what in the
source has to change, and it checks that answer by recompiling. It ships with `tools/regoracle_agbcc.patch`, a
tracing patch for agbcc. Both were added in commit `d77fcef` (2026-10-02), together with the match of the last
function, [[code-08044224]], where the region workers used it.

It automates the manual method of [[matching-tricks#Register allocation priority and reload rotation]]: reading
`.greg`/`.lreg` dumps, computing priorities and tracing `last_spill_reg`.

## Usage
Run it through `tools/dr` from the repo root. It works on any C unit and does not need the ROM. The target layout
comes from `asm/nonmatching/<unit>/<func>.s`, and the target bytes come from `tools/target.py`, which uses
`baserom.gba` when it is present and otherwise assembles the original assembly ([[rom-free-workflow]]).

```
tools/dr python3 tools/regoracle.py <unit> <func>                     # analyse, solve, verify by recompiling
tools/dr python3 tools/regoracle.py <unit> <func> --src FILE           # another C file (default below)
tools/dr python3 tools/regoracle.py <unit> <func> --range LO HI        # use only ROM insns in [LO, HI) as evidence
tools/dr python3 tools/regoracle.py <unit> <func> --show LO HI         # aligned ROM/built listing with the pseudo per operand
tools/dr python3 tools/regoracle.py <unit> <func> --prio R=P,...       # what-if: global-alloc priority P for pseudo R
tools/dr python3 tools/regoracle.py <unit> <func> --force R=H,...      # what-if: pseudo R only gets hard reg H
tools/dr python3 tools/regoracle.py --verify-compiler [unit ...]        # patched == stock compiler, traced or not
tools/dr python3 tools/regoracle.py --selftest <unit> <func> [...] [--perturb N [--seed S]]
```

- `--src` defaults to `build/wf/<func>/unit.c` when it exists (the [[agent-tooling]] `wf.py` working copy), else
  `src/<unit>.c`. The function must be C in that file, not `INCLUDE_ASM`.
- Other options: `--json` (the same content, machine-readable), `--top N` (rows per table, default 25),
  `--no-solve`, `--no-verify` (skip the recompile check), `--budget N` (simulator runs per solve round, default
  6000), `--rounds N` (solve again on the solved build while it keeps improving, default 3), `--quiet`.
- Pseudo numbers are stable for a given source, so `--prio`/`--force` strings, and the `REGORACLE_PRIO` strings the
  report prints, can be reused across runs.
- `--show` marks each aligned line: blank for identical, `r` for other registers, `~` for a high/low register
  form difference, `*` for different code.

## Output
Output files go to `build/regoracle/<func>/`: `unit.s` (with `-dp` insn uids), `trace.txt`, `trace-solved.txt`
and `unit-solved.s`. The text report has these parts:
- **Header:** built vs target size, instruction counts, how many are structurally equal (registers masked) and
  identical, and the differing lines. It also gives the allocno count and how many `find_reg` decisions the
  simulator reproduces (`** SIM DISAGREES **` if not all), and for how many pseudos the ROM register is
  known, agrees, differs or is unclear.
- **Pseudos in a different register than in the ROM**, in global-alloc order: built → ROM register, votes, order,
  refs, live, priority, source line, name (decls, also inline-function locals) and case. `~` marks evidence that
  sits next to code that differs structurally.
- **Inverse solve**: the priority changes that reproduce the ROM's registers. For each moved pseudo it gives the
  priority window that works and the refs-only or live-only change that lands in it (the "levers"). It also lists
  where the refs come from (lines, loop depth), where the pseudo is live, and which pseudos it must pass. Then the
  compiler check (`REGORACLE_PRIO=...`, differing lines before → after), or REJECTED when the build gets worse.
- **Not reproducible by reordering** (the conflict graph differs: a liveness or statement-order difference,
  with the insns where they overlap), and **impossible by ordering** (the ROM register is a hard conflict, either a
  live hard register or a local-alloc pseudo).
- **Local-alloc mismatches** with the quantity that took the register.
- **Reload registers**: the spill set and the first rotation mismatch. Either the ROM is N slots ahead, which means
  extra or missing reloads, listed with the ones in code that differs from the ROM flagged, or the choice differs
  because register availability differs at that insn.

## How it works
1. **Patched compiler.** `build/regoracle/old_agbcc-src` (and `agbcc-src` for units that use agbcc) is pret/agbcc at a
   fixed commit plus `tools/regoracle_agbcc.patch`. It is cloned and built inside `tools/dr` on first use, and
   rebuilt when the patch's SHA-1 no longer matches the stamp file. The patch adds `gcc/regoracle.c`/`.h` and
   hooks in `emit-rtl.c`, `flow.c`, `local-alloc.c`, `global.c`, `reload1.c`, `final.c` and `toplev.c`. It writes a
   line-oriented trace only when `REGORACLE_OUT` and `REGORACLE_FUNC` are set. The trace records:
   - decl names and source lines of pseudos, the expression and pass that created each pseudo;
   - `REG_N_REFS` contributions per insn with loop depth, the live sets, and `REG_EQUIV` live-length doubling;
   - the global-alloc tables (refs, live, conflicts, preferences, classes) and every `find_reg` decision;
   - local-alloc quantities and reload register choices (`last_spill_reg` rotation);
   - for every output insn, which pseudo each hard-register operand holds.

   `REGORACLE_PRIO` and `REGORACLE_FORCE` are what-if overrides, and they do change the code.
2. **Alignment and votes.** The built function is aligned with the ROM instruction by instruction, with
   registers masked. Low and high register forms are kept apart first, branch targets and pool offsets are
   normalised. Where the code is structurally equal, the ROM's register for each operand is a vote for the
   pseudo behind the built operand. Commutative operands are oriented by the other votes, and a spilled pseudo
   gets votes from its reload registers.
3. **Simulation and inverse solve.** A Python port of `global.c` (`prune_preferences`, `find_reg` and the
   allocation loop) replays global allocation. It is checked against the compiler's own decisions on every run.
   A beam search over targeted moves then looks for priority changes that reproduce the ROM's registers: pass
   a blocker, let the ROM's holder go first, preferred registers, and exhaustive one-moves when stuck. The
   solution is recompiled with `REGORACLE_PRIO` and kept only if the build gets closer to the ROM. With
   `--rounds`, the solved build is analysed again, because its newly equal code gives new evidence.
4. **Explanation** of what reordering cannot fix, as listed under Output.

Priority is `int(floor_log2(refs) * refs / live * 10000 * size)`; higher goes first, and ties go to the lower pseudo
number. refs is `REG_N_REFS`: each set or use adds the loop depth, so 1 outside loops and +1 per enclosing loop.
live is `REG_LIVE_LENGTH` in insns, doubled for a pseudo with a `REG_EQUIV` note.

**Changing the patch.** Edit `build/regoracle/old_agbcc-src/gcc` and run `tools/dr make -C <that>/gcc old -j1`.
Then run `git -C <that> add -N gcc/regoracle.c gcc/regoracle.h && git -C <that> diff > tools/regoracle_agbcc.patch`
and rerun `--verify-compiler`. The next run rebuilds every tree whose stamp no longer matches.

## Validation
- **`--verify-compiler`.** It compiles each unit with the stock and the patched compiler, then again with
  tracing on for the first three functions of the unit, and compares the assembly. Re-run for this page on
  2026-10-02 over all 112 C units in `units.txt`: **112/112 identical, traced and untraced**, in about 18 s. Without arguments the option checks only the first 12 C units.
- **`--selftest`** (reported in the tool's docstring and in commit `d77fcef`; not re-run for this page). On 7
  large matched functions the simulator reproduced every `find_reg` decision and no pseudo differed.
  - `--perturb` mis-orders 1–3 random pseudos of a matched function through `REGORACLE_PRIO`, then checks that
    the solve recompiles to the ROM bytes. On 12 functions, 53 of 60 trials were undone exactly. The other 7
    involve pseudos with no aligned evidence: a high register on one side turns their code into other insns.
  - Selftest was not re-run here because it rewrites `build/regoracle/<func>/`.

## Use on `CollectEffectTargets`
The six region workers of the last round ([[code-08044224#How it matched (2026-10-02)]]) used it as follows:
- They used its differing-line count as a second metric next to `check.py`. For example, r1B went from 183 to
  129 lines. r1C's scratch build combining the fixes for 0x3FA, 0x400, 0x41E, 0x439 and 0x47B was at 68.
- The solver asked for **live +3 on pseudo 5415** in case 0x439. Three dead stores supplied exactly that
  (FAKEMATCH). Three `asm volatile("")` statements fixed the loop structure as well, but swapped r4/r5/r6.
- r1D used it to confirm that pseudo 5456 (the 0x455 loop-test copy) no longer differed.
- r1F worked from the trace: the `number` and `attribute` refs, live lengths, `REG_EQUAL` doubling and
  priorities (0.7232 vs 0.750) explained why `number` lost r4, and pointed to the fix.

## Limitations
- **Fixed work directory.** Output always goes to `build/regoracle/<func>/`, so parallel runs on one function
  overwrite each other's trace. Every wf44 worker made a private copy that took the directory from an
  environment variable (`RO_WDIR` or `REGORACLE_WDIR`). The workers suggested adding this option to the tool.
- **First run needs the network** to clone pret/agbcc, and building the compiler takes a while. Only
  `old_agbcc` and `agbcc` units are supported.
- **It only solves allocation order.** Structural differences such as loop motion, CSE or a different insn count
  are reported, not solved. Priority changes that chase registers inside structurally different code are
  rejected by the compiler check. Pseudos with no aligned evidence cannot be placed.
- **Summaries can misread structure.** In 0x58D its summary said the ROM hoists `g + off`, but `0x08045F6E` is
  the loop label and the add stays in the loop (r1E). Check such claims with `--show` or the listing.
- A solved `REGORACLE_PRIO` string is evidence, not source. It still has to be turned into refs/live levers in C.
