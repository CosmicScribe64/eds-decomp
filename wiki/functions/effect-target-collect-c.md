---
title: effect_target_collect (card-effect target collector)
type: function
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# effect_target_collect

The unit is a single Thumb function, `CollectEffectTargets` (`0x08044224`, 0x2514 bytes, about 9.5 KB), the largest in
the game. It collects card-effect targets in the list-view overlay. **Status: matching** since 2026-10-02
(commit `d77fcef`). It was the last of the 1,976 functions, so this match completed the decompilation
([[overview]]). The whole function is C. Five cases use commented FAKEMATCH forms, and case 0x2F has an uncommented
`asm` clobber ([FAKEMATCH forms](#fakematch-forms-in-the-final-source)).

| | |
|---|---|
| Address, size, mode | `0x08044224`, 0x2514 bytes, Thumb, `old_agbcc -O2`; frame 0x28 |
| Signature | `u16 CollectEffectTargets(int player, u16 number, int arg)` |
| Callers | `ListView_Open` ([[card-list-viewer-c]]), [[effect-prepare1-c]] |
| Callees | `CopyDuelCard`, `IsToonMonster`, `IsEffectMonster`, `IsSpecialSummonOnly`, `HasFaceUpToonWorld`, `IsCardProhibited`, `IsMaterialOfFusion`, `FindFusionMaterials`, `CanReviveGraveyardCard` |
| Match status | **matching** (`check.py`: 1/1, `unit bytes MATCH`, built 0x2514 vs target 0x2514) |
| JP counterpart | `0x08061840`, 0x18CC bytes ([[jpmap]], [JP notes](#what-the-jp-counterpart-showed)) |

## How it matched (2026-10-02)

**Score path.** The wf score is the count of normalised differing lines plus 4 × |size delta in bytes|
([[agent-tooling]]). It fell from 6266 (prep) to 4321, parked at `f7c9206`
("round 2" in the NOTES, the first workflow round below). Later rounds took it to 2802, then about 568, then 338 (parked at `2eafe08`). One round of a
region-split workflow then took it from 338 to 0: **338 → 254 → 178 → 70 → 36 → 16 → 0**. Notes:
`build/wf/CollectEffectTargets/NOTES.md` (rounds and the integration summary at the end), `build/wf44/r1A`–`r1F/NOTES.md`
(one per worker), `build/wf44/int1/` (integration copies `s1.c`–`s7.c` and the scorer `sc.sh`). These are local
files, not in git.

**Method.** At score 338 the remaining difference sat in 14 of the function's case regions. The region table
comes from a per-case diff, `rdiff.py`. Each worker got one group of regions and a private copy of the whole unit. Each
group's fixes had to stay inside its own case bodies (plus new helpers). A worker reported its region rows,
the global `check.py --norm` score and a `patch.diff`. The workers also used [[regoracle]] and loop.c dumps
(`lp.py`/`lp2.py`: movable and giv decisions of both loop passes). Each worker's tools wrote to a private
directory, so parallel runs did not overwrite each other.

| Worker | Regions (rdiff lines before) | Result | Fix |
|---|---|---|---|
| r1A | 0x2F (10), 0x462 (10), 0x447 (10), 0x45C (2) | all 0; global 338 → 304 | Pad loop pass 1 with insns that combine deletes later, so the `&count` hoist moves to pass 2: `TargetAttack16(u16 id)` in 0x2F, `u8 TargetType8(u16 id)` in 0x447/0x45C. 0x462: grouped address `(u8 *)gDuelPlayers + 0x7C4 + (i * 4 + (player & 1) * 0xD64)` for the read, plain array copy. Ordinary C. |
| r1B | 0x3FA (18), 0x400 (36) | both 0; 338 → 262 | Same index expression in the loop test and the body (`(1 - player) & 1` / `player & 1`), so cse1 merges the latch chain and loop pass 1 hoists the latch pointer before the giv init. Then empty `asm("")` padding (1 in 0x3FA, 3 per 0x400 loop) lifts pass 2 to 27 insns, so `cards = list + 12` stays in the loop. FAKEMATCH. |
| r1C | 0x41E (34), 0x439 (14) | both 0; 338 → 266 | 0x41E (ordinary C): copy via the array reference `gDuelPlayers[player & 1].w.deck[i]`, test read via `(u8 *)gDuelPlayers + 0x7C4 + ...` (its `REG_EQUAL` constant matches the copy's `G + 0x7C4`, so the add stays in the loop), and `TARGET_TYPE_NV(idv)` as an inline (cse1 shares its stats load, so `0x1F00000` is short-lived and not hoisted). 0x439: plain `player & 1` plus three dead stores `j = i; flag = i; fieldCount = i;` (FAKEMATCH). |
| r1D | 0x454 (34, jump-table words only), 0x455 (16) | 0x455 0; 0x454 cleared once sizes were right; 338 → 322 | `TargetKind16(u16 id)`: with a `u16` parameter all three `id & 0x7FF` ANDs are HImode, their constants match (savings 6, life 6), and `0x7FF` moves in pass 1. That leaves 106 pass-2 insns, so `0x1F00000` stays in the loop as in the ROM. Plus the array-reference copy. Ordinary C. |
| r1E | 0x47B (28), 0x58D (4), 0x59F (4) | all 0; 338 → 254 | 0x47B: the r1C recipe with `+ 0x904` and `.w.graveyard[i]` (ordinary C). 0x58D/0x59F: `register u32 t asm("r0") = off + (u32)g;` keeps `off + g` in the loop ahead of the `i << 2`, because a hard register is never a loop movable (FAKEMATCH). |
| r1F | 0x5EB (36) | instructions identical; 18 jump-table lines left until the sizes upstream were fixed | `attribute = 0` after the id read (ROM order). `u16` id, `u8` attribute and two `(u8)` casts lengthen the attribute's live range (35 → 70 after `REG_EQUAL` doubling). Its priority drops to 0.600, below `number`'s 0.714, so `number` keeps r4 with about 19% margin. Ordinary C. |

The integrator applied the six patches in order of each patch's own score, onto copies in `build/wf44/int1/`.
All six were kept; none conflicted, and none made another worker's region worse:

| Step | Patch | Score | Size delta | Regions left |
|---|---|---|---|---|
| start | | 338 | +16 | 14 regions |
| s1 | r1E | 254 | +8 | 0x2F 0x3FA 0x400 0x41E 0x439 0x454 0x455 0x462 0x447 0x45C 0x5EB |
| s2 | r1B | 178 | +4 | 0x2F 0x41E 0x439 0x454 0x455 0x462 0x447 0x45C 0x5EB |
| s3 | r1C | 70 | 0 | 0x2F 0x455 0x462 0x447 0x45C 0x5EB (0x454's jump table aligned) |
| s4 | r1A | 36 | 0 | 0x455 0x5EB |
| s5 | r1F | 16 | 0 | 0x455 |
| s6 | r1D | **0** | 0 | none: `unit bytes MATCH` |

- The fixes were independent loop-local levers: loop.c pass-1/pass-2 movable thresholds and address grouping.
  The global allocation (list in `sl`, count pointer in `ip`, `i` in r8, `number` in r4) survived every
  combination, and r1F's attribute/number priority margin held.
- The size errors cancelled exactly (0x400 −4, 0x41E −4, 0x439 +4, 0x47B −4 bytes). That cleared every
  jump-table word diff in 0x454 and 0x5EB, which no worker could fix from inside those cases.
- Step s7 renamed the workers' suffixed helpers and changed nothing else. The renames were
  `TargetKind_D1` → `TargetKind16`, `TargetType8_A1` → `TargetType8`, `TargetAttack_A1` → `TargetAttack16`,
  `id_F1` → `id16` and `attr_F1` → `attr8`. s7 still scored 0.
- Applying: `src/` had gained the parked 338 draft since the copy was made, so the first `wf.py apply` hit a
  merge conflict. The fix was `wf.py prep --force`, putting s7's marked region into the new copy (the text
  outside the markers was identical), a check (0), then `wf.py apply`, which printed APPLIED.

### Earlier rounds that the final round built on
These findings from the rounds between scores 4321 and 338 are all in the matched source
(`build/wf/CollectEffectTargets/NOTES.md`, rounds 3–5). The general mechanisms are in [[matching-tricks#Loop motion and allocation in CollectEffectTargets (2026-10-02)]].
- **Global allocation fixed by a `for` loop.** The 0x45C removal step is a
  `flag = 1; for (i = 0; i < count && flag; i++) { ... }` loop over `gCardListView` alone (no `gCardListViewCards`).
  A `for` loop gets a loop-top note (`NOTE_INSN_LOOP_VTOP`) before its duplicated bottom test. loop.c therefore hoists the
  latch's GCSE re-set of the list base after the inner `CopyDuelCard` call. With the old `if (...) do {} while`
  form the copy stayed in the loop and the list base landed in r9. After the fix the list base is in `sl`, `&count` in `ip`, `i` in
  r8 and `number` in r4, as in the ROM (2802 → 2597).
- **One function-level `j`** serves as the 0x3F0 player index, the removal step's inner index and the tail's inner
  indices (r6 everywhere in the ROM).
- **Natural scan loops.** Most cases are `for (i = 0; i < gDuelPlayers[player & 1].w.X; i++)` with a
  grouped-offset word pointer, `word = (u32 *)((u8 *)gDuelPlayers + 0x904 + (i * 4 + (player & 1) * 0xD64))`
  (deck reads via `+ 0x7C4` or `gDuelDecks`, hand reads via `+ 0x684`). This made 0x65, 0x1AB, 0x3F3, 0x443, 0x4D9,
  0x5E7, 0x5E9, 0x5EA, 0x410, 0x45A, 0x4D8, 0x13D, 0xF, 0x191, 0x1A3, 0x3C6, 0x3EB, 0x3F0 and 0x3B1 exact. Only 0x58D/0x59F keep
  a guarded `if (...) do { } while` loop.
- **Bitfield card reads.** `struct TargetCard c = *(struct TargetCard *)(...)`, with `c.id` passed to both helpers,
  gives the ROM's single `lsl #20` with two `lsr #20` (0x2F, 0x23D, 0x526). The shift macro is CSE'd and gives one of each. `gDuelPlayers`
  is a `union` of a raw-word view and a bitfield view, so `gDuelPlayers[pl].s.graveyard[i].flag20` keeps the ROM's
  `(X + base) + 0x906` address in 0x60A.
- **`ADD_TARGETB`** (a plain `{ }` block) instead of `do { } while (0)` in the simple loops: the do-while adds loop
  notes, which weight the refs inside it one loop level deeper. The do-while `ADD_TARGET` remains only in the
  0x454, 0x447 and 0x5EB groups.
- **cse2 path reachability** decides whether a case's loop preheader copies the dispatch's list/count registers
  (`mov rX, sl`) or reloads the constant (`ldr =list`). 0x3FA's `&& i <= 4` loop ending makes 0x400 reachable
  and 0x3FA not, as in the ROM.

### FAKEMATCH forms in the final source
| Case | Form | Why |
|---|---|---|
| 0x2F | `asm volatile("" ::: "r4", "r5", "r6", "r7");` right after `i = 0;` | Global allocation then gives `i` r8, while `i` stays a biv (a `register ... asm("r8")` pin stopped strength reduction). **No `/* FAKEMATCH */` comment in the source.** |
| 0x3FA | one `asm("");` in the loop | loop.c pass 2 counts 27 insns, so `list + 12` stays in the loop |
| 0x400 | three `asm("");` in each of the two loops | as 0x3FA (24 → 27 insns; two are not enough) |
| 0x439 | dead stores `j = i; flag = i; fieldCount = i;` | as 0x3FA, but flow deletes them before register allocation, so live lengths are unchanged (three `asm volatile("")` also fix the loop but shift registers) |
| 0x58D, 0x59F | `register u32 t asm("r0") = off + (u32)g;` | a hard register is never a loop movable, so `off + g` is not hoisted and the read does not become a giv |

The dead stores, the asm padding and the r0 pins are candidates for the FAKEMATCH cleanup pass ([[overview]]).

### What the JP counterpart showed
[[jpmap]] mapped the function to JP `0x08061840` (0x18CC bytes, certain). `build/jp/sub_08044224_diff.md` aligns
the two versions case by case.
- JP has 35 numbers in 27 bodies. Every case both versions share comes in the same order. USA inserted its new
  cases next to related ones and appended the rest after `0x4B2`, so the ROM's body order is source order.
  `0x45C` was split out of JP's `0x447` body (USA has a copy-pasted case block), and JP's `0x48A` was dropped.
- Idioms present in JP too, so original source rather than tricks: the `player & skipFilter` reload after
  `skipFilter = 1` (r1C's 0x439 fix uses plain `player & 1` with a plain `int` flag), the absolute aliases in the
  fusion-copy group, and destination before source in list copies (`cards[count] = src`).
- USA changes: the tail calls `HasFaceUpToonWorld(player)` unconditionally and narrows it to `u16` before the flag
  tests (JP calls its counterpart inside the `||`). The third filter pass (`IsCardProhibited`) is new. The type/ATK
  helper has an extra `type == 24 → 4000` arm and reloads the stats word (JP loads it once), which fits the
  volatile `TARGET_STATS` reads kept in the USA helpers. Each deck word is read through two different expressions.
- JP's frame is 0x1C with `player` in `sl`. USA's is 0x28, with `player` spilled to `[sp+8]` and the list base in
  `sl` (hypothesis: the extra parameter and the list base displaced `player`).
- In JP's deck loops the tag constant 1 is hoisted into a high register, but tags 2 and 4 are never hoisted. So the
  hoist follows the value 1, not the hand area.

### Verification
- 2026-10-02, for this page: `tools/dr python3 tools/check.py effect_target_collect` reports 1/1 functions, `unit bytes
  MATCH` (0x2514). `python3 tools/progress.py` reports 1976/1976 functions and 0x7EC70/0x7EC70 bytes.
  `tools/regoracle.py --verify-compiler` gives identical code from the stock and patched compilers for all 112 C
  units.
- Reported by the coordinator for `d77fcef` and not re-run here: `check_all.py` 112/112 units, and
  `make compare` printing `eds.gba: OK`.

## Findings
- Signature: `u16 CollectEffectTargets(int player, u16 number, int arg)`. The prologue verifies the
  parameter narrowing; the return value is the zero-extended halfword count.
  - `ListView_Open` in [[card-list-viewer-c]] calls it for area −1 instead of copying a fixed card list.
  - [[effect-prepare1-c]] tests `CollectEffectTargets(p, number(ref), 0) > 0`.
  - It resets the overlay count, dispatches by card/effect number, appends selected card words and area tags,
    applies three filtering passes, and returns the final count. The low bit of `player` selects a player's
    lists; comparisons and callee arguments sometimes retain the full signed player value.
- It contains 132 `bl` instructions; most are agbcc far jumps inside the function itself, the usual sign of a very
  large function ([[decomp-workflow]]). Direct callees are `CopyDuelCard`, `IsToonMonster`, `IsEffectMonster`,
  `IsSpecialSummonOnly`, `HasFaceUpToonWorld`, `IsCardProhibited`, `IsMaterialOfFusion`, `FindFusionMaterials`, and `CanReviveGraveyardCard`.
- The generated [[m2c]] draft is useful for predicates, but requires repairs: some case values wrap by
  −2³², shared cases appear inside unrelated nested switches, and pointer offsets can remain byte offsets
  despite their inferred C pointer types. It must not be enabled as generated.

## Overlay layout used by this function

| Address | Offset from `0x0201D810` | Interpretation |
|---|---|---|
| `0x0201D81C` | `+0x00C` | 128 complete 32-bit card words |
| `0x0201DA1C` | `+0x20C` | 128 halfword area tags |
| `0x0201DB1C` | `+0x30C` | Halfword count |

This is a local view of the same overlay that [[card-list-viewer-c]] exposes as 192 card words. Its first 128 card
words occupy 0x200 bytes; the remaining 0x100 bytes are interpreted as area tags by this search. No separate
buffer is allocated. Compile-time layout checks verify the count/area offsets and the 0xD64 player stride.

The tags written here are 1 for hand, 2 for deck, 4 for graveyard, 8 for fusion deck, and 0x10 for the list at
player `+0xB84`. These are masks rather than the viewer's area numbers 12–15.

## Dispatch map

Symbolic execution of the assembly's initial decision tree for every one of the 65,536 possible `u16` values
found **60 supported numbers in 42 shared bodies**. The other 65,476 numbers reach the default tail with an
empty list. Body order below follows the ROM, rather than sorting the numbers.

Here, "monster" means the packed card type is ≤20; it does not also exclude card ID zero. ATK, DEF,
level, kind and attribute come from [[card-table]]. "Self" selects `player & 1`, and "other" selects
`(1 - player) & 1`. Helper-test meanings are deliberately left unnamed.

| Numbers | Selection |
|---|---|
| `0x002F` | Self deck monsters with ATK ≤1500 |
| `0x0065` | Self graveyard Traps |
| `0x013D` | Self hand then deck; card numbers `0x003D`, `0x003E`, `0x04E1` |
| `0x000F` | Self hand then deck; card number `0x04B2` |
| `0x0191` | Self graveyard; card numbers `0x03EB`, `0x040A` |
| `0x01A3`, `0x01F9`, `0x05E8` | All self fusion-deck words; no area tags written |
| `0x01A8` | Self deck; card number equals the input number |
| `0x01AB` | Self graveyard Magic cards |
| `0x023D` | Self deck monsters with DEF ≤1500 |
| `0x03B1` | Self deck cards with type >20 |
| `0x03C6` | Test nonzero hand IDs for Dragon type, then copy the corresponding **deck** word, tag 1 |
| `0x03EB`, `0x040A`, `0x060B` | Self fusion deck; `FindFusionMaterials(player, id, materials)` succeeds |
| `0x03F0` | Both graveyards, player 0 then 1; monsters with nonzero low halfword from `CanReviveGraveyardCard` |
| `0x03F3` | Other graveyard monsters; skip the first filter |
| `0x03FA` | Other deck's first up to five words; skip the first filter |
| `0x0400` | All self graveyard then other graveyard words; skip the first filter |
| `0x0410` | Self deck; card numbers `0x03EB`, `0x040A` |
| `0x041E` | Self deck monsters with ATK ≤1500 and `IsSpecialSummonOnly(id) == 0`; force the first filter |
| `0x0439` | All self deck words; skip the first filter |
| `0x0443` | Other graveyard Magic cards |
| `0x0454`, `0x0456`, `0x045D`, `0x045F`, `0x0460`, `0x0463` | Self deck monsters with ATK ≤1500 and attribute EARTH, FIRE, LIGHT, WATER, WIND, DARK respectively; exclude fusion/ritual kinds and nine specific card numbers |
| `0x0455` | Self deck monsters of ritual kind |
| `0x045A`, `0x045B`, `0x051B` | Self deck; card number equals the input number |
| `0x0462` | Self deck ritual Magic cards (type 22, subtype 6) |
| `0x0447`, `0x0487`, `0x0488`, `0x05F0` | Self graveyard monsters with nonzero low halfword from `CanReviveGraveyardCard(player, index)` |
| `0x045C` | Same graveyard test, then remove the first result numbered `0x045C` when `player == arg` |
| `0x047B` | Self graveyard monsters with ATK ≤1500 and `IsEffectMonster(id) == 0` |
| `0x04B2` | Self deck Magic cards |
| `0x04BC` | Self deck; card numbers `0x0022`, `0x04BA`, `0x07F2` |
| `0x04D8` | Self deck; card number `0x02EA` |
| `0x04D9` | Self graveyard; card numbers `0x02EA`, `0x04D8` |
| `0x0526` | Self deck Insects with level equal to `arg` and `IsSpecialSummonOnly(id) == 0` |
| `0x058D` | Self graveyard monsters whose card-word bit 21 is set |
| `0x059F` | Self graveyard Magic cards whose card-word bit 22 is set |
| `0x05E7` | Other graveyard monsters |
| `0x05E9` | Self graveyard monsters; skip the first filter |
| `0x05EA` | Self graveyard Fiends |
| `0x05EB`–`0x05EF` | Self graveyard monsters with attribute LIGHT, FIRE, WATER, EARTH, WIND respectively |
| `0x05F4` | Self graveyard words whose bit 20 is set |
| `0x05FC` | Self graveyard monsters |
| `0x060A` | Self graveyard words whose bit 20 is set and `IsMaterialOfFusion((u16)arg, id)` succeeds |
| `0x060D` | Self `+0xB84` monsters; low byte of the corresponding `+0xCC4` halfword is not 2 |

The six attribute-search cases exclude card numbers `0x0037`, `0x0038`, `0x003E`, `0x0042`, `0x0170`,
`0x0175`, `0x0187`, `0x02E5`, and `0x034D`. Their kind classifier handles numbers `0x0776`–`0x0778`
before the packed kind bits, as in [[duel-ritual-c]].

## Filtering and copying details

When the collected count is nonzero, the function always calls `HasFaceUpToonWorld(player)` and narrows its result
to `u16`. It applies the `IsToonMonster(number)` rejection pass when that halfword is zero or the case forces
it, unless the case skips that pass. It then rejects card-word bit 17, followed by cards for which
`IsCardProhibited(low_12_bit_id)` is nonzero.

The matched source keeps these ROM details:

- The three final filters compact **only card words**, leaving area tags untouched. Each compaction copies
  through the old count, including `cards[old_count]`, before decrementing the count.
- The `0x045C` special removal decrements the count first and compacts both words and area tags through the
  new count. It removes at most one occurrence.
- The unfiltered fusion-copy group writes through the absolute aliases `gCardListViewCards` and `gUnk_0201DB1C`
  and leaves all area tags as they were.
- The `0x03C6` hand-test/deck-copy discrepancy is present in the assembly and retained in the matched source.
- Helper calls to `CanReviveGraveyardCard` test its low halfword, rather than the full return word.

## History of the matching attempts (before 2026-10-02)

> [!warning] Contradiction: superseded status claims
> The sections below were written while the function was unmatched. Their status claims are superseded by
> the match ([How it matched](#how-it-matched-2026-10-02), commit `d77fcef`, checked with `check.py` on
> 2026-10-02). This covers "Status: nonmatching", "one of the last two giants", "still nonmatching", "contributes
> no decompilation progress", "Still open", and the parked scores 4321 and 338. Their
> source-level findings are kept as history. Where a later round contradicted one, a callout says so in place.

### Validation and remaining matching work (first draft, 2026-10-01)

The active assembly fallback passed `tools/dr python3 tools/check.py effect_target_collect`: **1/1 functions,
unit bytes MATCH, 0x2514 bytes**. The complete C candidate compiled with the unit's `old_agbcc -O2` flags
to **0x2148 bytes**, so it is still nonmatching and contributes no decompilation progress.

A local finite differential experiment in `build/decomp_large/verify_behavior.py` executed both the ROM
function and that compiled Thumb candidate. **7,609 samples passed**, covering all 60 dispatch numbers,
two default values, all 821 card IDs (measured table-read coverage), flag combinations, signed player values,
0/1/40-card boundary lists, and eight repeating argument/callee-return patterns. It compared complete EWRAM snapshots, return values,
and callee argument order, with caller-save registers clobbered at each external call. Callees were deterministic synthetic hooks, not their real implementations,
so the result is behavioral evidence. It does not prove equivalence or replace an exact byte match. No host
dependencies were installed, and the experiment uses the existing Docker image's Capstone library.

The initial candidate reproduces the main switch's decision tree, including agbcc far branches. The ROM
keeps the narrowed number in `r4`, overlay base in `sl`, and count pointer in `ip`; the candidate uses
`r6`, `r9`, and `r3`, and its frame is 0x24 bytes instead of 0x28. Loop pointer formation, table hoisting,
field extraction, and the resulting register lifetimes still differ throughout the bodies. Explicit
list/count pointer locals, raw-address table aliases, card bitfield reads, and guarded do-loops have been
examined in scratch compiler variants without obtaining a match. Volatile reads were tested only in
scratch and are not retained in the behavioral draft.

Next work should preserve the verified dispatch/body order, recover the original byte-offset induction
variables and pointer lifetimes per body, and recheck the final filter loops. Keep all near misses parked
until the entire 0x2514-byte unit matches; instruction similarity alone is insufficient.

### Larger-coverage pass (private frontier)

The larger-coverage pass kept the active assembly and the original parked draft intact. Private experiments are in
`build/bigguns-targets/`, with a source snapshot and per-candidate `source.c`, `unit.s`, `text.bin`, and
whole-unit comparison log. All use the existing Docker compiler and the unchanged ROM.

Useful source-level findings, rather than accepted matches:

- The ATK/DEF/level helpers need a signed switch on types 21–24. A `u16` ID parameter restores several
  of the original repeated ID masks and table reads. Moving the same getter behind an additional inline
  helper can make agbcc merge those reads again; helper boundaries matter even when the expressions agree.
- An index-first deck read through `gDuelDecks` and a copy through player base `+0x7C4` recover a shared
  byte-offset induction variable. Using one identical array expression for both lets agbcc reuse the word,
  although the ROM reloads it. The other-player top-five case must retain `1 - player` for the copy.
- The ROM computes the destination before reloading a selected source word. An inline word-returning
  copy helper instead tends to evaluate the source first. Pointer-returning helpers, direct expressions,
  and explicit destination locals were compared; none resolved the whole function.
- Guarding pointer initialization and then using a `do` loop puts pointer increments before index updates
  in several list bodies. This improves local instruction shape but does not fix global register allocation.

The `guard-typed32-wordfirst` candidate builds to **0x2340 unit bytes** (symbol size 0x233E), versus the
original draft's 0x2148 and the required 0x2514. Its frame is **0x20**, still wrong against the ROM's 0x28,
so the smaller size gap must not be read as a near match. A diagnostic normalized-assembly
sequence comparison increased from 1,209 to 1,551 aligned lines out of 4,232 target lines. This metric
ignores label values and is not an exact-match percentage. Mixed symbol/constant table-view candidates
got closer in length but had worse spills and were not promoted.

`tools/dr python3 build/bigguns-targets/verify_candidate.py` reruns the existing finite behavioral suite
against that candidate: **7,609/7,609 samples passed, with all 821 card IDs visited**. The log is
`build/bigguns-targets/behavior-check.log`. Candidate behavior and exact
matching are separate checks; no new function or byte coverage is credited for this private frontier.

Next targeted repairs are the destination/source evaluation order, loop-local player-base lifetimes,
and the original two stack slots that currently remain in registers. Do not repeat the recorded broad
helper-type, pointer-form, or table-view combinations without new assembly evidence.

### Region-diff pass (2026-10-01)

The parked draft in `src/effect_target_collect.c` was replaced by a much closer candidate. Tooling lives in
`build/fable/CollectEffectTargets/` (untracked): `w.c` (working copy of the whole unit), `rdiff.py`/`sn.sh` (per-case-region diff,
optionally register-masked), `rs.sh REGION` (side by side), `tryv.py`/`try_block.py` (variant tests), `publish.py` (write `w.c` into `src/`).
Metric: register-masked differing lines over the 44 regions, **4667 -> about 1300**; built size 0x24A0-0x24EC against 0x2514. Still nonmatching.

C idioms confirmed by region diffs (each reduced the diff when applied):
- Locals are per case (`{ ... }`), `i` stays function-wide (r8 in the ROM).
- The type lookup goes through an inline taking `u16 id`; this creates the HImode/SImode constant pair that makes the `0x7FF` mask hoist like the ROM. ATK/DEF take `u32` and return `u16` (this hoists the 1500 compare constant). `TARGET_NUMBER` is a plain macro on a `u32` (folds to `lsls 21; lsrs 20`).
> [!warning] Contradiction
> The next bullet (region-diff pass, 2026-10-01) says the ROM never CSEs its repeated stats lookups. The workflow round (2026-10-02, `build/wf/CollectEffectTargets/NOTES.md`) found that cases 0x454 and 0x5EB do share one **non-volatile** stats load between the type and the `>> 29` attribute. The parked draft now has both a volatile `TARGET_STATS` and a non-volatile form, chosen per case. Resolved: the volatile read is per case, not a whole-function rule.

- The stats table is read through `volatile const u32 *`: the ROM does not CSE its repeated lookups.
- Pointer formation: `b=(u8*)gDuelPlayers; off=(player&1)*0xD64; if (i < ((struct TargetPlayer*)(b+off))->xCount) { do {...} while (i < gDuelPlayers[player&1].xCount); }`. There is no named `p` in the explicit guard/do-while loops (the recomputed expression produces the ROM's `p` copy); `for` loops keep a named `p`.
- Two list-read styles: a `word++` pointer (offset constants `0x904`, `0x684`, ...), or an integer `off += 4` induction variable added to a base (`gDuelDecks`/`gDuelGraveyards` symbols or `b+ARR`). The pool in each case tells which.
- Number compares: `int cardNo` variable (the constant is loaded before the `ldrh`); several-value compares are `switch` (gives the `blt` tree).
- Tail: flag bit 17 is `(s32)(word << 14) < 0`; lookup is `TARGET_NUMBER(TARGET_ID(word))`; the three filter loops read through a `word++` pointer.
- Case `0x400`: `skipFilter = 1` is CSE'd into the `player & 1` constant (the ROM reloads it from its stack slot).

> [!warning] Contradiction: pointer formation and the tail loops
> The pointer-formation and tail bullets above are from the region-diff pass (2026-10-01). Later rounds
> (`build/wf/CollectEffectTargets/NOTES.md`, rounds 4–5) and the matched source (`d77fcef`) disagree with them:
> - Most scan cases match as natural `for (i = 0; i < gDuelPlayers[player & 1].w.X; i++)` loops with a
>   grouped-offset word pointer. Only 0x58D/0x59F keep the guarded `if (...) do { } while` form.
> - The three tail filter loops index `u32 *t = gCardListView.cards; ... t[i]` inside each loop, not a `word++`
>   pointer.
>
> Resolved in favour of the matched source.

Allocation findings (the remaining gap): the set of cases that use the PRE'd list pseudo (gcse `ldr; mov sl` re-insertions after calls) is identical in the ROM and the draft, so the structure agrees. What differs is the registers: the ROM keeps the list base in `sl`, hoisted constants in `r9`, the count pointer in `ip`, and spills `player`; the draft swaps `sl`/`r9` and its count pointer is spilled and rematerialised. Priority numbers (n_refs, live length, from `-dg`) suggest the ROM's tail loops hold a call-crossing induction variable in `r9` (the tail's `i*4` giv), forcing the list into `sl`. The tail loops therefore still need their exact source form. Unresolved: why only the tag-1 (hand) loops hoist their area constant, and the second table lookup in `0x2F` being recomputed from the shifted word in the ROM.

> [!note] Resolved later
> - **Allocation:** fixed by writing the 0x45C removal step as a `for` loop
>   ([Earlier rounds](#earlier-rounds-that-the-final-round-built-on)).
> - **Recomputed lookup in 0x2F:** fixed by the bitfield card read (`c.id` passed to both helpers).
> - **Tag-1 hoist:** the JP counterpart shows that the hoist follows the value 1, not the hand area. JP's deck tag
>   is 1 and is hoisted, while tags 2 and 4 never are.

### First workflow round (2026-10-01/02)

The giants loop worked on this function after the waves without matching it. Working notes:
`build/wf/CollectEffectTargets/NOTES.md`, with the older pass in `build/fable/CollectEffectTargets/NOTES.md`. The parked draft
in `src/effect_target_collect.c` was refreshed at the wave 3 checkpoint (commit `f7c9206`). Its `#if 0` note reads
"score 4321".

**Scores.** The wf score counts normalised differing lines (`python3 tools/wf.py check CollectEffectTargets`). It went from 6266
(prep) and 6230 (round start) through 5952, 5549, 5053, 4668, 4588 and 4506 to **4321** (parked). The
register-masked region total of `sc.py` went from 1624 to 863 at the 5549 checkpoint, and the register-sensitive
region total went from 4360 to 3513. These metrics are not comparable with the "4667 -> about 1300" masked metric of the region-diff pass above.

**What helped (in the parked 4321 draft):**
- **`i` in r8 without a pin.** `register int i asm("r8")` (score 5952) hurt: a hard-register `i` is not a
  biv, so loop.c could not strength-reduce the i-indexed addresses in the tail loops. What works is a plain `int i` and
  one empty clobber `asm volatile("" ::: "r4", "r5", "r6", "r7");` right after `i = 0;` in case 0x2F (FAKEMATCH).
  At that point `i` is live and nothing else wants r4-r7, so global allocation gives `i` r8 as in the ROM.
- **Card words as a bitfield struct with signed 1-bit fields.** The flag-20 test in 0x5F4/0x60A is
  `ldrb [p+0x906+4i]; lsl #27; cmp; bge`, which only comes from an `s32 flag20:1` field read straight from a
  struct-typed array element (`PS->graveyard[i].flag20`). A `u32` field gives a word load and `lsl #11`; a `u8` field or
  `& 0x10` gives `movs/ands/beq`. 0x58D/0x59F use the same approach: copy the element
  `struct TargetCard w = ((struct TargetCard *)((u8 *)g + off))[i];`, test `w.flag21` / `w.flag22`
  (`lsl #10` / `lsl #9` sign tests), and copy the word through a different expression, `*(u32 *)((u8 *)g + (off + i * 4))`,
  so that CSE keeps the separate copy giv.
- **Deck and graveyard reads through their own symbols:** `*(u32 *)((u8 *)gDuelDecks + i * 4 + (player & 1) * 0xD64)`
  gives the ROM's "and, lsl, mul" order. Graveyard scans (0x447, and probably 0x3F0/0x45C) read and copy through
  `gDuelGraveyards`, which is player 0's graveyard as a separate symbol (5053 -> 4652). The copy inside `ADD_TARGET` must
  not go through an inline helper, because inline calls in the right-hand side are expanded before the slot address and the ROM computes the slot first.
- Loops that contain a jump-table switch (0x454, 0x5EB) are not loop-optimised in the ROM. They are natural
  `for (i = 0; i < gDuelPlayers[player & 1].X; i++)` loops.
- Two-array scans (0x3C6 hand->deck, 0x60D): `p = (P *)(b + off)` for the loop test, then a guarded
  `do { u32 *t = (u32 *)((u8 *)g + off); u32 *td = (u32 *)(b + K2 + off); ... } while (i < p->X);`
  (regions 28 -> 8 and 27 -> 10). Superseded: in the matched source, 0x3C6 and 0x60D are natural `for` loops
  with grouped-offset reads (`+ 0x684`/`+ 0x7C4`, `+ 0xB84`).
- Superseded in part: in 0x41E and 0x47B the matched source reads through `(u8 *)gDuelPlayers + 0x7C4`/`+ 0x904`
  and copies through the array reference `gDuelPlayers[player & 1].w.deck[i]` / `.graveyard[i]` (workers r1C
  and r1E). The mirror symbols `gDuelDecks`/`gDuelGraveyards` are still used elsewhere (0x2F, 0x447, 0x45C,
  0x5EB).
- `TargetKind` takes a `u32` id and switches on the `(s32)` type, which gives the signed `bgt` tree. In 0x454, `u8 ok = attribute == K;`
  gives the ROM's `mov r1,#0` after the load.

**Compiler facts established** (merged into [[matching-tricks]]):
- loop.c's movable threshold is `(call ? 1 : 2) * (1 + n_non_fixed_regs)` (26 or 13 here), minus 3 after every
  moved movable. Loop runs twice (`-frerun-loop-opt`): pass-1 movables, then the strength-reduction giv inits, then pass-2 movables.
  So the ROM's preheader order "g, list copy, count copy, p copy, giv init, 0x7FF, areas" means the `0x7FF` mask moved in
  pass 2. If it moves in pass 1, the loop body is too small or fewer movables came before it.
- GCSE treats constant-pool symbol loads as memory killed by calls. That is why `ldr rX,=list; mov sl,rX` is
  re-inserted after calls and at case ends.
- Simple scan cases do not share one shape in the ROM: the preheader order differs from case to case, so a single
  template only helped some of them (0x65, 0x4BC).

**Still open:**
- Global allocation: tail loop 1 written as `u32 *t = gCardListView.cards;` inside the loop with `t[i]` gives the
  ROM's two givs (tail masked diff 78 -> 52) but flips the global allocation: the count pointer is spilled and the score goes from 5239 to 5722.
  Revisit once the other regions are closer (`e43.txt`).
- In 0x2F/0x23D/0x462/0x526 the ROM computes `w << 20` once but `>> 20` twice, deriving the second id again for the
  second helper. Mini tests (`mini/m1.c`, `m2.c`) with u16/u32/s16 parameters, bitfields and `& 0xFFF` did not reproduce it.
- Pointer formation, the `0x7FF` hoist in the remaining cases, and global register allocation (list base `sl` vs `r9`,
  count pointer `ip`) still differ, as in the region-diff pass.

> [!note] All three were resolved before or during the final round
> - **Tail loop 1:** uses the `t[i]` form, which became possible once the `for`-loop removal step had fixed the
>   global allocation.
> - **`w << 20` once, `>> 20` twice:** reproduced by the bitfield card read.
> - **Pointer formation and the `0x7FF` hoist:** fixed by the natural loops, grouped offsets and `TargetKind16`
>   (r1D).
>
> See [How it matched](#how-it-matched-2026-10-02).

Tools in the work directory: `sc.py` (per-region table and wf score, about 1.2 s), `rs.sh REGION` (side by side),
`xp.py EDITFILE` (apply `@@OLD/@@NEW/@@END` edit blocks and score them), `greg.py` (pseudos per hard register with
priorities, after `wf.py dump`), and `park.sh` (parks only when `src/` still equals the last copy, then syncs `base.c`).

