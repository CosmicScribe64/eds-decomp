---
title: decomp-permuter
type: tool
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# decomp-permuter

[decomp-permuter](https://github.com/simonlindholm/decomp-permuter) takes a near-miss C function and rewrites it at random, thousands of times. It adds temporaries, reorders statements, changes types, and reuses variables. Each variant is compiled and scored against the target assembly, and a score of 0 identifies a candidate to inspect. Acceptance still requires an exact whole-unit byte comparison against the ROM; a score alone is insufficient.

It is the tool for drafts that are only off by a few registers or by instruction order ([[decomp-workflow]]).

## Setup
- It is installed in the Docker image at `/opt/permuter`, with pycparser, toml and Levenshtein in the venv. Both `eds-decomp` and `eds-opencode` include it.
- `tools/permute.py <unit> <func>` builds a job in `build/permuter/<func>/`:
  - `base.c`: the `#if 0 /* NONMATCHING */` draft directly above the function's `INCLUDE_ASM` line, or `--from F.c`. The unit is preprocessed, and every other function is reduced to a prototype. `static inline` helpers are kept so they still inline.
  - `target.o`: `asm/nonmatching/<unit>/<func>.s` assembled and linked at the function's ROM address. The script checks that its bytes equal the ROM.
  - `compile.sh`: the unit's compiler and flags from `config/cflags.txt`. It then links at the same address, with the same external-symbol stub (`syms.o`, from `build/eds.elf` or the `_XXXXXXXX` name suffix).
- Because both sides are fully linked, the permuter compares final bytes. Relocation noise is gone.
- Draft extraction preserves multiline comments attached to `#if 0`, so their continuation
  lines do not become invalid C. The function-body scanner skips comments and quoted
  literals when counting braces, including apostrophes in prose comments. These fixes
  were verified by preparing a real `Bustup_LoadScene` job and compiling its extracted draft.
- K&R parameter declarations now stay attached to their function header during
  extraction. Other K&R bodies become unprototyped declarations, preserving the
  caller's original argument narrowing. Previously the extractor retained two
  unrelated bodies in the isolated `DiceScreen_PrepareRoll` job, inflating its extent from
  0xBC to 0x1EC and shifting the target's address. The repaired real job contains
  only `DiceScreen_PrepareRoll` at 0x08026030 and compiles to 0xBC. K&R kept-body, unprototyped
  declaration, inline-helper and comment-brace checks passed in Docker.

## Rechecking parked drafts

`tools/dr python3 tools/match_drafts.py <unit> ...` enables one parked draft at a time,
checks the complete unit, and retains only exact matches. Failed candidates restore the
source verbatim; original sources and results are saved under `build/match_drafts/`.
The selected units must have no concurrent source writer. Both the `#endif` + fallback
and `#else` fallback forms are supported, including multiline draft annotations.
This catches drafts whose old NONMATCHING notes became stale after header changes.
It recovered `EffectChainDestructionResolve` in the continuation pass; a later 90-draft early-unit
recheck found no further matches.

## Usage
```
tools/dr python3 tools/permute.py <unit> <func> --run -j 2 --minutes 15
```
Results go to `build/permuter/<func>/output-<score>-<n>/source.c`. Look at `diff.txt` in the best output, apply the change to `src/<unit>.c` by hand, and verify with `tools/check.py`.

Tidy the result if you can, because the permuter's output is pycparser-normalised. Mark changes that make no semantic sense with `/* FAKEMATCH */`.

Register-allocation profile: `--profile regalloc` weights the passes that move register choice (declaration and
statement order, commutative swaps, temporaries, chained assignments, local types) 4-8x higher and damps the rest.
`tools/permute_batch.py --profile auto` uses it for drafts whose NONMATCHING note mentions register allocation.
`--retry-below N` runs a second, closest-first pass (15 min each) over drafts whose first-pass best score was 1..N,
logging to `batch2.log`. The default false positive (branch targets ignored) is fixed with `--no-ignore-branch-targets`.

Budget: the host has 10 CPUs shared by all agents. Run at most two permuter jobs at a time, with `-j 2` each.

## Results
| Function | Base score | Time | Change found |
|---|---|---|---|
| `EffectTargetableMonsterCheck` | 840 | < 5 min | `ret = zone = 1;` (reusing a dead variable) keeps `ret` in r3 instead of a `negs/orrs/lsrs` setcc |
| `CompareCardsByType` | n/a | 22 s (batch) | tidied by hand to `unsigned long long base = 0x08621DE0;` used for the first table access: agbcc can't CSE the literal, so it reloads it at each use as the ROM does |
| `CompareCardsByAttribute` | n/a | 18 s (batch) | the same 64-bit temp trick |

## Known fakematch patterns (found by the permuter)
- **The ROM reloads a constant address at each use, but agbcc keeps it in one register**: route one use through an `unsigned long long` temp (`(const u32 *)(u32)base`). This stops the CSE without changing semantics.
- **Wrong register for a result set on one branch**: assign it through a dead variable (`ret = zone = 1;`).
- **SDK-style DMA sequence scheduled wrong**: wrap the register writes in `do { ... } while (0)`, as the SDK DMA macros are written (`PackList_LoadCoverGfx`).
- **A value is kept in a register but the ROM re-reads it**: read the struct field at each use instead of caching it in a local (`DuelCmd_UpdateZoneLpPaid`).
- **Cheap constant hoisted where the ROM rematerialises it** (manual trick sweep, `LoadCardFrame`): caching loop constants in locals assigned just before the loop reproduces the ROM's hoists of the expensive ones (`0xFF00`/`0x10`/`0x1000`) but agbcc then hoists a `0xFF` mask into the scratch register instead of rematerialising it; no local/`(u8)v`/`!= 0`/64-bit form avoided it.
- **Explicit induction local for a derived index** (manual trick sweep, `GetPack_DrawCardSprites`): `i2 += 2` / `y += 0x20` fixes the ROM's `r8=2i`, `r7=32i` allocation, but the `0x7CF` compare is still hoisted; the 64-bit temp un-hoists it at the cost of a dead double-word load.
- **Trick sweep that did *not* work** (`duel_ritual`: `UpdateSpellTrapNegation`, `SumHandLevelsExcept`, `SumTributableMonsterLevels`, `EffectRitualSummonPrepare`): re-reading fields at each use, the `unsigned long long` address temp, converting a `for` loop to a `goto` loop (old_agbcc still hoists through it here), caching invariants in top-declared locals, and `do { ... } while (0)` wrapping never reproduced these functions' register/hoist choices; the remaining differences are pure register-allocation scheduling (`SumHandLevelsExcept` is 2 instructions off, the others 70-340 diff lines).
- **Trick sweep for a hoisted loop invariant**: if the ROM hoists `X` and agbcc hoists `Y` instead, a `goto` loop stops *all* loop-invariant motion, so it also drops the high-register hoisting the ROM keeps (constants/shifts into `r8`/`r9`/`sl`/`ip`) and over-spills. It is not a drop-in fix (`bitmap_text` `OverlayBoldGlyphTile`).
- **Named local for a hoisted invariant**: declaring `u16 yy = y & 0x1F;` does not replace the compiler's own hoist; agbcc hoists *both*, giving an extra live value (`bitmap_text`).
- **Forcing a spill by swapping the `if`/`else` bodies** can make agbcc spill the variable the ROM spills, but it also flips the branch layout and the scratch-register choice, so it still misses (`bitmap_text` `PutMapString`).
- **`volatile s16` reload of a struct field**: forces the ROM's re-read, but old_agbcc emits `ldrh` (zero-extend) rather than the target's `ldrsh`, even through an `int` temp. The sign extension is optimised away because only the low 16 bits are stored (`bitmap_text` `LineInit`).
- **A constant base address the ROM keeps in a register** (`ring = (u8 *)0x02030000`): writing it as a local, an extern symbol, a `u32`, or an inline literal all get rematerialised from the pool by agbcc; none forces it into a high register (`bitmap_text` `LZSSDecompress`).

### Both parked-draft formats

`tools/permute.py` now shares the draft matcher and multiline annotation removal with `tools/match_drafts.py`. It accepts both `#if 0 … #endif` followed by `INCLUDE_ASM`, and `#if 0 … #else INCLUDE_ASM … #endif`. Previously the latter failed setup, even though it is a common format in the list-search units. Replacing an existing draft with `--from` removes its entire inactive wrapper before preprocessing. Real setup jobs for `CountGraveyardCardsOfType` (first form) and `DiscardHandCardByNumber` (second form) verified this extraction.

Blank lines between the closing directive and `INCLUDE_ASM` are also accepted. The strict adjacency rule had omitted `DestinyBoardScene_DrawFinalLetters` and `TurnOrder_ChooseHand` from the parked-draft queue despite their existing C. Six spacing fixtures across both wrapper forms and all 326 real parked blocks passed in Docker (`build/codex-continue/draft-spacing-check.log`). This changes inventory classification, not matching-source coverage.

## Queue runners and the semantic hill-climber (2026-10-01)
- `build/solo-s49/pforever.py` runs **nonstop**: 3 workers × `-j 2`, 15/25/40 minutes by size, ranked by bytes ÷ (1 + normalized diff lines). It skips units in `build/solo-s49/owned.txt` (agents or manual work) and drafts whose text hasn't changed since their last run (`pforever.state.json`). Every result is logged to `pforever.log`, with `*** MATCH ***` on a score-0 hit.
- `build/solo-s49/climb.py` is a **greedy semantic hill-climber**. For every parked draft it tries single mutations: each local's type among `int/u32/s32/u16/s16/u8/s8`, `1 & x` rewritten as `x & 1` and back, and swaps of adjacent declaration lines. It keeps strict improvements of the normalized diff inside the `#if 0` text, so the build is unaffected. The first pass improved 183 drafts in about 15 minutes (for example `DeckReorder_DrawSwap` went from 139 to 18 lines by changing `int id` to `u16 id`). Mutations can be lossy (narrowing), so they are search moves only; exact whole-unit bytes remain the acceptance test.
- `build/solo-s49/mutate.py` applies one regex mutation family to every draft and reports the effect (used for the sweep that changes `u32 id` to `u16 id`).
