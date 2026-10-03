---
title: USA-to-Japan mapper (tools/jpmap.py)
type: tool
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# USA-to-Japan mapper (`tools/jpmap.py`)

`tools/jpmap.py` maps every USA function, RAM address and data range to its counterpart in the Japanese ROM
(*Duel Monsters 5 Expert 1*, AY5J, `roms/base_jp.gba`). It answers three questions for the multi-version plan:
which USA C can the JP build reuse, where did RAM and data move, and in what order did JP link the shared code.
It is a read-only research tool, added in commit `446546b` (2026-10-02). The results and the staged plan built
on them are on [[rom-versions]].

## Usage
```
tools/dr python3 tools/jpmap.py                       # analyse, match, write build/jp/* (about 30 s)
tools/dr python3 tools/jpmap.py --compile-test        # also recompile matched USA C at JP addresses
tools/dr python3 tools/jpmap.py --compile-test -n 20  # ... with 20 functions instead of 10
tools/dr python3 tools/jpmap.py --assets              # also locate each config/assets.tsv range in JP
tools/dr python3 tools/jpmap.py --diff CollectEffectTargets   # also write an aligned USA/JP listing of one function
```
- It needs **both ROMs**: `baserom.gba` and `roms/base_jp.gba`, whose SHA-1 it checks
  (`dc25f733…`). Unlike `check.py`, it has no ROM-free mode ([[rom-free-workflow]]).
- Run it through `tools/dr`. The image provides `rapidfuzz` for the similarity search (without it the tool
  falls back to `difflib`'s ratio), and `--compile-test` needs the agbcc toolchain and binutils.
- It reads the ROMs and the repo and writes only under `build/jp/`, which git ignores. The listings contain
  disassembly of the JP ROM, so keep them local.
- `jpmap.side_by_side()`, `jpmap.render()` and `jpmap.case_table()` can be imported from a scratch script to
  compare any USA and JP address ranges, for example one switch case.

## Outputs (`build/jp/`)
| File | Contents |
|---|---|
| `summary.txt` | Function counts, status table, confidence split, JP-only count, link-order blocks, RAM and ROM-data pair counts, the JP library symbols. Also printed on stdout. |
| `map.tsv` | One row per USA function: JP address and size, similarity, score, status (`identical-mod-relocation`, `same-shape-different-constants` with detail `offsets-only` or `immediates`, `changed`, `no-match`), how it was matched, whether the aligned calls agree with the map, confidence. Then one row per JP-only function. |
| `units.tsv` | Per USA unit: function counts by status, byte shares, the JP address range its counterparts span. |
| `blocks.tsv` | Runs of USA functions whose JP counterparts keep the same relative order. |
| `link_order.tsv` | The same runs (two or more functions) sorted by JP address: the JP link order in terms of USA code. |
| `ram_map.tsv`, `romdata_map.tsv` | USA → JP address pairs from aligned literal pools, marked `strong` (identical/same-shape pairs) or `weak` (changed pairs), with votes, competing JP values and constant-delta runs. |
| `compile_test.txt` | `--compile-test`: per function, whether the recompiled USA C equals the JP bytes, relocation count, how many relocated words moved, and how many relocations agree with the map. |
| `assets.tsv` | `--assets`: per USA asset range, `identical`, `mostly-shared`, `partly-shared`, `not-found` or `zero-fill`, with the JP address and the most common delta. |
| `<func>_listing.txt` | `--diff`: a two-column USA/JP listing, plus case maps for switch dispatchers. |

`build/jp/PLAN.md` (the staged plan) and `build/jp/sub_08044224_diff.md` (matching hints for
[[code-08044224]]) were written by hand from these outputs.

## Method
1. **Function discovery.** USA function extents come from `config/functions.tsv`. Instructions, literals and
   jump tables are classified by `tools/disasm.py`'s recursive-descent analyzer ([[toolchain]]). For JP, the
   same analyzer runs with its module globals pointed at the JP image: the code range, the JP-only ARM routine
   at `0x0805CA00`, the library symbols found by searching for the USA library bytes, and the `AgbMain` seed
   from [[crt0]]'s literal. `jp_layout()` asserts each of these facts, so a wrong ROM fails early.
2. **Tokens** at three strictness levels per function:
   - **E**, exact modulo relocation: raw instructions with `bl` targets, ROM and RAM addresses in literal pools
     and jump-table targets masked.
   - **O**: E with load/store offsets, add/sub immediates and non-address pool constants also masked, which
     catches struct layout and buffer size changes.
   - **S**, shape: instruction format and registers only.
   - **F** (format only, registers dropped) feeds the similarity `2*LCS/(len_a+len_b)`, the same measure as
     `tools/similar.py`.
3. **Matching.** Unique E, O and S hashes become anchors. Aligned calls and Thumb function pointers of matched
   pairs then vote for their callees' counterparts (call graph). Matches extend to neighbours along both link
   orders. Last, a global search at decreasing thresholds preselects candidates with rapidfuzz and scores them
   by format similarity, shared constants (card numbers, masks, sizes) and shared references through the map
   learned so far. Each pair records how it was found (`unique-E/O/S`, `callgraph`, `neighbour`,
   `similarity`, `gap`).
4. **Confidence.** Identical and same-shape pairs are `high`. A changed pair is `high` at score ≥ 0.7, or
   ≥ 0.5 when found through the call graph or a neighbour; `medium` at ≥ 0.5; otherwise `low`.
5. **Reports.** Address pairs are read from aligned literal pools of matched pairs. Link-order blocks are
   maximal runs of USA functions whose counterparts appear in the same order in JP, with no other matched
   function in between. `--assets` first searches JP for each whole range (`identical`). Otherwise it samples
   up to 48 non-zero 64-byte chunks of the range, searches for each anywhere in JP, and reports the share found:
   `mostly-shared` at ≥ 0.8, `partly-shared` at ≥ 0.2, else `not-found`.
6. **Compile test.** It picks the largest matched C functions whose pairs are identical modulo relocation and
   that have relocations, one per unit. Each is compiled with the USA compiler and flags, its relocations are
   resolved to the JP values read from the JP bytes, it is linked at the JP address, and the result is compared
   with the JP ROM. Each relocation's JP target is also checked against the map.

## Verification (2026-10-02)
- The analyzer reproduces `config/functions.tsv` exactly on USA. On JP it finds 2136 functions with 0
  analysis messages. 636 JP data words point at Thumb `push` starts, and none of them is a function the gap
  scan missed, so seeding from pointer tables adds nothing.
- `--compile-test`: 10 of 10 functions byte-identical, and every relocation agrees with the map, including
  the 20 of `LinkSioRecvMultiBlock` (11 of them to RAM that moved) ([[rom-versions]]).
- 706 of 846 pairs have every aligned call and function pointer consistent with the map.
- Re-checked for the wiki: the status counts recounted from `map.tsv` (158 / 41 / 647 / 1130, 1290 JP-only,
  confidence 311 / 164 / 172); the JP header and SHA-1; the crt0 literal, the `MAGB` and
  `gameboy.datacenter.ne.jp` strings, and the ARM routine's APCS prologue, read directly from both ROMs
  ([[rom-versions]]).

## Limitations
- **Low-confidence pairs are guesses.** The 172 low-confidence changed pairs are similarity matches, and some
  are wrong. 140 of the 846 pairs have at least one aligned call that disagrees with the map.
- **"No match" is not "absent".** Code rewritten past recognition scores at chance level. `CanReviveGraveyardCard`
  has its JP counterpart (`sub_0806172C`) right before the counterpart of `CollectEffectTargets`, but scores 0.36.
  Pairing by position or shared callers is not implemented.
- **The JP function list is provisional.** It comes from the analyzer, not from a matching JP build. Re-run
  the tool after the JP all-assembly build exists (stage 2 on [[rom-versions]]).
- **Weak RAM pairs are many-to-one.** JP often loads one struct base where USA uses several absolute aliases
  into the struct, so weak pairs are evidence of a relationship, not an address map.
- **The asset comparison is a sample.** It finds byte-identical chunks only. A recompressed, re-encoded or
  re-ordered asset shows as `not-found` even when the content is the same.
- **The compile test is selective.** It covers identical-modulo-relocation functions only. It shows the
  compiler and flags are the same; it says nothing about how many changed functions can be rewritten to match.
- **Naming.** JP functions are reported as `sub_<JP address>`. Some of these names also exist in the USA
  address space (`sub_0806172C`), so always read them together with the JP column.
- The output path `build/jp/` is the path the planned JP build will use. Rename the research folder first
  (for example to `build/jp-research/`).

Related: [[rom-versions]], [[agent-tooling]], [[xref]].
