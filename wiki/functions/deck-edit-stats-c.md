---
title: deck_edit_stats card composition statistics screen
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# deck_edit_stats

Thumb, `old_agbcc -O2`, 0x0806C4E4–0x0806D51C. **8/9 functions in C** after workflow wave 2 (2026-10-01: `0x0806D1D8` in wave 2, see [Wave 2 matches](#wave-2-matches-2026-10-01)); 1 stays `INCLUDE_ASM` (`0x0806C590`, C attempt parked). Before wave 2: 7/9. Whole unit bytes MATCH. This unit computes and draws card-list composition statistics (screen name is a hypothesis), sharing state with [[deck-edit-cards-c]], [[deck-edit-filter-steps-c]] and [[deck-edit-c]].

> [!warning] Contradiction: the unit is now 9/9
> The count above (8/9) predates later matches. `src/deck_edit_stats.c` has no `INCLUDE_ASM` left (checked 2026-10-02), so all
> 9 functions are in matching C. The decompilation reached 100% at commit `d77fcef` ([[overview]]). Resolved in favour of
> the source. Text below that calls a function nonmatching, parked or `INCLUDE_ASM` is history. Some of the later matches
> are recorded only in git (`git log`) and not yet written up here.

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| 0x0806C4E4 | 0x50 | matching C | ResetStatisticsAnimation (hyp.) | clears halfwords +0x1C4E/+0x1C4C and bytes +0x1C49/+0x1C4A/+0x1C52/+0x1C53/+0x1C4B |
| 0x0806C534 | 0x5C | matching C, initialized binding | GetCopiesForList (hyp.) | list 0 owned 10-bit count, list 1 main+extra copies, list 2 side copies; trunk +8/+9 |
| 0x0806C590 | 0x5D8 | asm; C attempt parked | CountListCategory (hyp.) | counts copies in categories 1–6: normal/effect/fusion monsters, Magic, Trap, ritual monsters; card numbers 1910/1911/1912 treated as ritual/effect/effect |
| 0x0806CB68 | 0x1AC | matching C, initialized hints | BuildCompositionStatistics (hyp.) | rebuilds lists; computes six counts, their sum and rounded percentages at 0x02030000 |
| 0x0806CD14 | 0xC0 | matching C | DrawCompositionNumbers (hyp.) | draws six count/percentage pairs and total with DrawNumberSprites |
| 0x0806CDD4 | 0x23C | matching C, initialized hints | InitStatisticsGraphics (hyp.) | clears VRAM, loads maps/tiles/palettes, sets BG control, initializes opening animation and computes statistics |
| 0x0806D010 | 0x188 | matching C | TickStatisticsScreen (hyp.) | scrolls BG3, accepts A/B when idle, handles animation completion and alpha transition |
| 0x0806D198 | 0x40 | matching C | RunStatisticsSteps (hyp.) | runs table 0x081A724C on gMain+0x485A |
| 0x0806D1D8 | 0x344 | **matching** (wave 2, 2026-10-01) | InitStatisticsLists (hyp.) | clears shared state and builds owned/main+extra/side lists, excluding card numbers 1920–1999 |

## State and matching notes

Scratch 0x02030000 is six `{u16 count, percent}` rows (4-byte stride), followed by total/count 100 at +0x18/+0x1A. Percentages use `(u16)DivFix8(count*4,total)*25`, shift by 8 and round up if fraction exceeds 0x7F. The zero-total path is delegated to the division helper.

Shared state 0x0201DB20: animation block +0x618, state byte +0x61E; menu phase +0x1C3D low 3 bits; mode +0x1C48 bits 1–4; scroll halfwords +0x1C4C/+0x1C4E; alpha +0x1C50, signed direction +0x1C51, busy byte +0x1C52. Animation base 0x0201E138 exposes the same scroll fields at +0x1634/+0x1636, close flag +0x1639 and busy +0x163B.

The matching input handler requires `switch` on a full-width unsigned key value for cases 1/2: it produces `bhi`/`bcc` range tests. A u16 switch promotes to signed int (`bgt`/`blt`), and an explicit `keys >= 1` test is optimized to `keys != 0`. Direct struct field accesses preserve large-offset literals. Two unmatched functions retained readable C under `#if 0` (historical: since wave 2 only `DeckStats_CountCategory` is left). The following earlier frontier notes include the percentage routine (`DeckStats_Compute`), which has since been resolved:

- **`GetCardCopiesInList`**: the switch-value copy lands in `r2` (ROM `r3`) and the card's narrowed index in `r1` (ROM `r2`), and the case-body base/pointer registers follow. Named locals, pointer caches, `int r` return vars and `if/else` all fail to move the copy.
- **`DeckStats_Compute`**: case 1 is `main + extra`. The ROM derives `extra` as `main+4` (one offset literal, `adds r2,#4`), while agbcc loads a second offset literal (4 bytes longer). The sum `last + rows[4] + … + rows[0]` is coalesced to `r1` by agbcc but cycles `r1/r2/r3/r1` in the ROM.
- **`DeckStats_Init`**: the *discarded* `CardType(0x439)` computation is kept in the ROM because `gCardStats` is read as a side effect. Declaring it `volatile` (`extern volatile ... gUnk_08621DE0_vol[] asm("gCardStats")`, /* FAKEMATCH */) restores the load, but agbcc still drops the mask/shift and canonicalises `number<0x776` to `<=0x775`, leaving the tail 8 bytes short.
- **`DeckStats_CountCategory`**: `StatisticsKind` must spell the range test as `if (number < 0x776) goto …; if (number > 0x778) goto …; return 1;` to stop agbcc merging it into a subtract/compare. It is still 0x34 bytes short because agbcc shares the `gCardStats` base register and CSEs the two `CARD_TYPE(card)` evaluations per card.

> [!warning] Contradiction
> The next note (2026-10-01) prescribes three `u16 *` column pointers (`count0/1/2[row*3]++`) as the form that reproduces the size of `DeckEdit_Init`, and the later checkpoint-43 paragraph a named `counts` union for count-column indexing. The wave 2 match (2026-10-01, `build/wf/DeckEdit_Init/NOTES.md`) uses neither: the draft with those column pointers (plus a trunk-entry pointer and the symbol card-number table) hoisted `&row[k]` instead of the count bases, and the match increments `INIT.count[INIT.row[k]][k]++` directly, with the loop copied from the matched twin `TradeCardSelect_Init`. Resolved in favour of the matched source.

- Historical (matched in wave 2): **`DeckEdit_Init`**: the byte size matches (0x344) but the registers differ. The ROM keeps `INIT` in `r5` and the three count-column bases (`&count[0][0..2]`) in `sl/r9/r8`, recomputing `&INIT.row` per use. Expressing the increments as three `u16 *` column pointers (`count0/1/2[row*3]++`) reproduces the size, while agbcc hoists `&INIT.row` and tests the `owned` bitfield with `ands` instead of `lsls`.

No `build/permuter/<func>/output-0-*` result existed for any still-asm function here, so nothing could be applied from the permuter. Its best scores were 160 (`GetCardCopiesInList`) and 676/881 (`DeckStats_Compute`). Every parked `#if 0` draft above is left for a later permuter run.

**Matching trick (`DeckStats_DrawNumbers`):** the ROM hoists loop constants `1`/`8` into `sl`/`r9` but reloads the `0x02030000` base each iteration. Wrapping the `for` loop in `do { ... } while (0);` (found by the permuter, marked `/* FAKEMATCH */`) changes agbcc's loop-invariant hoisting to reproduce that split.

## Bounded experiments (2026-10-01)

`DeckStats_CountCategory`: 54 combinations of ordinary/fixed/64-bit table-address views and 16 distinct symbol-alias combinations produced no match. Bodies remained 0x5A4 or 0x5A8, below the original 0x5D8; the older CSE/register-lifetime frontier remains. `GetCardCopiesInList`: 27 initialized list/card local-binding and trunk-address combinations, followed by a bounded one-worker search with empty branches and ABI mutations disabled, also produced no exact result. Live ASM is unchanged. Evidence: `build/bigguns-lead2/statistics_wide.py`, `statistics_aliases.py`, `copy_count_roles.py`, and their logs. These unchanged grids should not be repeated.

## Statistics percentages matched (2026-10-01, checkpoint 41)

`DeckStats_Compute` is enabled: all **428 bytes** and the complete **0x1038-byte unit** match. This brings the unit to **5/9 matching C**, and the full ROM passes. Earlier ASM/near-match notes for this routine are historical.

- Case 0 and case 2 share their final halfword read. Case 1 derives the extra-copy address by adding four to the main offset, preserving literal loads.
- The list cursor is reloaded before every category call. The first five stored counts are reloaded and summed in the original order before adding the full sixth return word and storing the low halfword total. Named CountRow fields avoid indexing a u16 pointer across row subobjects.
- Each percentage uses the low halfword quotient multiplied by 25, rounded upward only when the low fraction exceeds 127. All arithmetic bounds fit signed int; address arithmetic is unsigned. Seven initialized bindings and eleven empty constraints remain after removing 10 bindings and 3 constraints. Caller-saved bindings die before calls.
- ABI audit found the helper definition used narrow signed parameters while callers passed words. `DivFix8` in [[gfx-util-c]] explicitly decodes both word arguments to s16 and returns the sign-extended s16 quotient. Signed multiplication by 256 replaces a potentially negative left shift. Its definition and both caller declarations agree, and all three complete units match unchanged ROM bytes.

**1,024 finite differential fixtures PASS**, covering valid/default selectors, helper mutations between cursor/count loads, full-word count and quotient values, signed division with nonzero denominators and both rounding-boundary sides. Divide-by-zero returns are synthetic; BIOS zero-denominator behavior is not claimed. Full EWRAM, live IWRAM, ordered calls, saved registers and SP agree.

Evidence: `build/bigguns-lead2/deck_percent_{tails,minimize,clean,accept}.py`, `verify_deck_percent.py`, `DeckStats_Compute/solo-percent-clean/`, ABI checks under `DivFix8/solo-percent-word-abi/` and `DeckEdit_DrawLevelStars/solo-percent-word-proto/`, and `build/lead-pass31/`.

Additional bounded selector drafts (`deck_count_branch.py`) with explicit default return and signed selector locals still do not match `GetCardCopiesInList`; its ASM remains active.

## Statistics graphics initialization matched (checkpoint 42)

`DeckStats_Init` is enabled: **572 bytes**, complete unit exact, bringing the unit to **6/9 matching C**. The full ROM passes. Earlier ASM/volatile near-match notes for this function are historical. The accepted source uses ordinary table memory plus three empty initialized constraints to retain the discarded card-type calculation, its signed threshold tree, and its masked result. No instructions or uninitialized values are supplied.

The shared cropped-map tile source is bound to r10 and the fade speed to r0; all other tested bindings and allocation hints were minimized away (3 bindings and 3 constraints removed). Callee-saved state is restored. Prototypes for enabled graphics/fade callees match their definitions; the still-ASM crop helper explicitly accepts word scalars and narrows them on entry, as shown by its original instructions. Using the old narrow sy draft prototype added caller narrowing absent from the ROM and was rejected.

**256 finite differential fixtures PASS**: every cursor byte, cursor mutation between the two crop calls, card-number range boundaries and all 32 card-type values. Modeled BIOS fill/copy operations and synthetic graphics callees agree on complete EWRAM, live IWRAM, VRAM, palette, IO, ordered writes/calls, card-table reads, return, saved registers and SP. These are finite software fixtures, not hardware emulation or a proof.

Evidence: `build/bigguns-lead2/statistics_graphics_{tail,order,refine,minimize,clean,accept}.py`, `verify_statistics_graphics.py`, `DeckStats_Init/solo-graphics-clean/`, and `build/lead-pass32/`.

## Copy-count selector matched (checkpoint 43)

`GetCardCopiesInList` is enabled: **92 exact bytes**, complete unit exact, bringing the unit to **7/9 matching C**. The full ROM passes. Earlier near-match status and failed selector grids are historical. Word parameters are explicitly decoded to u16. The default returns the narrowed selector, matching the untouched r0 in the original; the parked draft's missing default return was not carried forward. Explicit above-one/owned/main/side labels retain branch layout. Only one initialized r3 selector binding remains after removing seven bindings and all four empty constraints.

**6,144 finite differential fixtures PASS**, without synthetic callees: every owned count from 0 through 1023, all packed main/extra/side combinations, all 821 valid card IDs, selectors 0/1/2 and three defaults, and nonzero upper argument halves. ROM and compiled C agree with an independent field-extraction oracle; EWRAM, live IWRAM, saved registers and SP also agree.

Evidence: `build/bigguns-lead2/deck_count_{explicit,layout,minimize,clean,accept}.py`, `verify_deck_count.py`, `GetCardCopiesInList/solo-count-clean/`, and `build/lead-pass33/`.

The two larger remaining routines stayed in ASM at this checkpoint (historical: `DeckEdit_Init` matched in wave 2). Private `DeckEdit_Init` trials add a named union of `count[2][3]` and `countFlat[6]` to make count-column indexing valid without changing layout. Baseline bytes are unchanged by that view. Scoped base/column bindings, shifted bitfield tests, and guarded-loop variants in `statistics_lists_{resume,shape}.py` do not match (best normalized difference 70), and no coverage is claimed. old_agbcc does not support the attempted anonymous union, so use the named `counts` union.

## Private continuation after checkpoint 43

Private category-counting frontier: `DeckStats_CountCategory/solo-category-word-stage-1/` has 66 normalized differences and size 0x5D0 against 0x5D8. Nested signed range/switch tests preserve CMP 22 / BGT followed by CMP 21 / BGE; an initialized ordinary masked-ID input constraint and late table-base assignment improve lookup. Remaining differences include commuted selector ADD, swapped card-load copies, reload scratch registers and two shortened type-mask sequences. No behavioral validation or added coverage. Invalid-list representation still needs review. Fixed u8 row r7 produced bogus stack reads and was rejected. A bounded one-worker permutation found no match. Experiments: `statistics_category_{conditions,word_stage,lifetimes}.py`. Evidence under `build/bigguns-lead2/`.

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/DeckEdit_Init/NOTES.md`.

### `DeckEdit_Init` (0x344, start score 95; ordinary C)

Trunk/deck-edit scene init: clears the `0x0201DB20` state block (0x1C5C bytes) and the BG offset registers, resets the three card lists, fills them from the trunk (owned / main+extra / side copies), skipping card numbers 0x780-0x7CF, and primes the slider and panel.
- The old draft used `u16 *count0/1/2` column pointers, a `struct TrunkEntry *entry` local over `gUnk_02011C20_words` and `gCardIdToNumber[...]`. That hoisted `&row[k]` instead of the count bases and tested `owned` with `ands` instead of `lsls #22`.
- What matched was the card loop of the matched twin `TradeCardSelect_Init` ([[deck-edit-prohibit-c]], its `case 0`):
  - `for (i = 1; i <= 0x334 && CARD_NUMBER(i) != 0xFFFF; i++)` with `CARD_NUMBER(id) = ((const u16 *)0x08622AB4)[(id) & 0x7FF]` (integer address).
  - Direct `trunk->e[i].owned`, `.f1 || .f3` and `.f2` tests on `struct { u16 owned:10; u8 f1:2; u8 f2:2; u8 f3:2; }` inside `{ u8 pad0[8]; entry e[1]; }`; the mixed u16/u8 bitfield base types give the `ldrh; lsls #22` and `ldrb [#9]` tests.
  - Direct `INIT.count[INIT.row[k]][k]++`, with no column-pointer locals.
- `INIT` and the trunk view are cast macros (`#define INIT (*(struct InitState *)&gDeckEdit)`, `((struct TrunkInit *)gSaveData)`), because `wf.py apply` rejects `extern T x asm("gUnk_...")` labels; they compile byte-identically ([[agent-tooling]]).
