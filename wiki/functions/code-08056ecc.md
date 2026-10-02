---
title: Unit code_08056ECC (duel AI: hand pick, zone scoring, attack selection, state save/restore)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# code_08056ECC: duel AI helpers (`0x08056ECC`-`0x08057EE0`)

`src/code_08056ECC.c` (19 functions, 0x1014 bytes). **18/19 functions in C** after workflow waves 2-3 (2026-10-02: `0x08057C94` in wave 3); 1 stays `INCLUDE_ASM` (`sub_08056ECC`, best attempt under `#if 0 /* NONMATCHING */`). Before wave 3: 17/19. The unit links to the exact target bytes. Compiler `old_agbcc -O2`. Names are proposals, and the code keeps `sub_08XXXXXX`. All of this is probably the CPU opponent's decision code (hypothesis). Neighbours: [[code-080609c4]] (board drawing) and [[code-08009a68]] (per-player lists/zones). See also `src/code_0800C894.c`: `sub_0800C894` is zone ATK, `sub_0800C8A8` is zone DEF, and `sub_0800ABC8` is card info.

> [!warning] Contradiction: the unit is now 19/19
> The count above (18/19) predates later matches. `src/code_08056ECC.c` has no `INCLUDE_ASM` left (checked 2026-10-02), so all
> 19 functions are in matching C. The decompilation reached 100% at commit `d77fcef` ([[overview]]). Resolved in favour of
> the source. Text below that calls a function nonmatching, parked or `INCLUDE_ASM` is history. Some of the later matches
> are recorded only in git (`git log`) and not yet written up here.

## Functions

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x08056ECC` | 0x504 | nonmatching | `AiPickHandCard(id)` | picks an index in the list `0x0201D81C` (`n = sub_08044224(1, number(id), 0)` entries, card words): with `0x02015EE8+4 & 0x200` first looks for a forced card (list card number 0x10-0x14 with own number 0x2F/0x23D/0x47B, or list card 0x2F/0x23D with own number 0x463) and returns it; own number 0x463 otherwise returns -1; for own numbers 0x1AB/0x65/0x443 scans the priority list `gUnk_0819D2FC` (13 card numbers); else scores every list card (`CardValue` = ATK-like stat * 10, 0 for types 0x15-0x17, 4000 for type 0x18; `CardDefValue` uses the low 9 bits) against the strongest opponent ATK (`sub_0800ABC8` `[1]` max over the 5 zones), falls back to the highest ATK value, then to `rand() % n`; the result is stored at `0x02015F00 + 0x1B22` (u16) |
| `0x080573D0` | 0xBC | matching | `BestZoneScore(p, skip, useAtk, useDef)` | max over occupied zones of player `p` (except `skip`) of `[useAtk ? info[1] : 0] + [useDef ? info[2] : 0]` (`sub_0800ABC8`; the zone's flag bit 1 is set around the call when clear); -1 if none |
| `0x0805748C` | 0xC4 | **matching** | `BestZoneIndex` | same, returns the index of the maximum |
| `0x08057550` | 0xCC | **matching** | `WorstZoneIndex` | same, index of the minimum (starts at 99999) |
| `0x0805761C` | 0x20 | matching | `SumZoneAtk(p)` | sum of `sub_0800C894(p, i)` for zones 0-4 |
| `0x0805763C` | 0x80 | matching | `CanUseZone(p, zone)` | occupied, `flags7` bits 2/3 clear, `sub_0800A78C(p, zone, 0x15C) == 0`, and not (card 0x148 on either side and `sub_0800C8BC == 1`) |
| `0x080576BC` | 0x140 | matching | `AiShouldPlayCard(id, flag)` | 1 if either list test `sub_08007590(number, 1/0)` holds (only when `flag == 0`), number 0x16D, number 0x2DA with no own monsters, or value <= best opponent score (`BestZoneScore(0, -1, 1, 0)`); or (value <= 1000, own deck > 4, life > 999) |
| `0x080577FC` | 0x58 | **matching** | `CountDeckIn10to14` | count cards of player 1's deck (`deckCount`, list `+0x7C4`) with card number 0x10-0x14 |
| `0x08057854` | 0x58 | **matching** | `CountList904In10to14` | same for list `+0x904` (`count904`) |
| `0x080578AC` | 0x48 | matching | `CountZonesIn10to14` | same for player 1's monster zones (forward `i <= 4` loop + `switch` range test) |
| `0x080578F4` | 0x94 | matching | `PickHandByPriority` | first hand index (player 0) whose card number matches the 26-entry priority list `gUnk_0819D316`, else `rand() % handCount` |
| `0x0805797C` | 0x104 | matching | `EvalAttack(a, b, out)` | fills an `AttackPlan` for player 1's zone `a` attacking player 0's zone `b` (`x = ATK(1, a)`; `y` = ATK or scaled DEF of the target): `lose` if y < x, `gt`/`le` flags, `diff = x - y` |
| `0x08057A80` | 0x12C | matching | `BestAttackTarget(a, out)` | direct attack (return 1, `f1 \| f2`, `diff = ATK`) when `sub_0804A3D8(1, a)` or no defenders and the card is not 0x5F3; otherwise scans zones 0-4 with `EvalAttack`, keeping the plan with the larger `diff` (tie: larger DEF) |
| `0x08057BAC` | 0xA4 | matching | `ZoneWeakerThan(value, zone)` | compares player 0's zone value (ATK if face-down, else DEF scaled by `(handicap + 5) / 16` when `handicap <= 10` and the zone flag bit 1 is clear) with `value`; ties favour the side with more monsters |
| `0x08057C50` | 0x44 | matching | `AnyZoneWeakerThan(value)` | 1 if player 0 has no monsters or some occupied zone satisfies `ZoneWeakerThan` |
| `0x08057C94` | 0x174 | **matching** (wave 3, 2026-10-01) | `ChooseAttacker` | collects player 1's eligible zones (`sub_0804A528`), bubble-sorts them by ATK ascending (swap blocked when the later card is number 0x226/0x2E8/0x2F9), then takes the first whose `BestAttackTarget` succeeds and stores the plan at `0x02015F0C` (`f2 = 1`) |
| `0x08057E08` | 0x34 | matching | `DuelStateSave` | DMA word copy `0x020192E4` -> `0x02015F14`, 0xD86 words (both `DuelPlayer`s + globals) |
| `0x08057E3C` | 0x34 | matching | `DuelStateRestore` | the reverse copy |
| `0x08057E70` | 0x70 | matching | `DropUnplayableZones` | player 1 zones whose card fails `sub_08007590(number, 0)`: `f6_0 = 0, f6_1 = 1, f7_2 = 0` and their bit cleared in the zone mask `+0x26` |

## Structures

- `AttackPlan` (8 bytes, `struct AttackPlan` in the unit): `u16` bitfield `f0:1 f1:1 f2:1 lose:1 src:3 dst:3 gt:1 le:1 rest:4`, `u16 unk2`, `s16 diff`. Stack copies are moved with `sub_08075294(dst, src, 8)`; the chosen plan lives at `0x02015F0C` (the `AiWork` area `0x02015F00`, whose `+0x1B22` u16 holds the chosen hand index).
- `DuelZone` bytes used: `+6` bit 0 (`f6_0`, face-up?), bit 1 (`f6_1`, temporary evaluation flag set around `sub_0800ABC8`), `+7` bit 2/3; `DuelPlayer.zoneMask` (`+0x26`, bit i = zone i). Other fields as in [[code-08009a68]].
- `gMain+0x4870` bits 1-5 (`handicap`, 0-10, hypothesis: CPU difficulty): DEF values of face-up monsters are scaled by `(handicap + 5) / 16` in `ZoneWeakerThan` / `EvalAttack`.

## Matching tricks (old_agbcc)

- **`static inline` helper for a repeated value expression** (`CardValue(id)`: switch on the card type, 0 / 4000 / `field * 10`) reproduces the ROM's repeated inline expansions with the result in r0 (`0x080576BC`). The second use needs `(u32)v > 1000` (unsigned compare).
- **Switch for sparse comparisons**: three card numbers `0x226/0x2E8/0x2F9` as `switch` cases give the ROM's binary tree of `cmp/beq/bgt` with the middle constant kept in a high register.
- **Consecutive bitfield stores to the same byte are merged into one read-modify-write** by old gcc (`0x0805797C`: `f1 = 0; lose = 0; src = a;` and `0x08057E70`: `f6_0 = 0; f6_1 = 1;`), and the mask constants then appear as `mov #3; neg` (int `~2`), unlike a plain `u8 x &= ~2` (which gives `mov #253`). So zone flags are declared as bitfields.
- A `u16` local for a masked id (`u16 id = CARD_ID(word)` then `CARD_NUMBER(id)`) changes how the `& 0x7FF` mask is materialised (`ldr r2,=0x7FF; adds r0,r2,#0; ands r1,r0`) and matched `0x08057E70`.
- **`int y = 0;` initial value shared with the constant-zero stores** (`strh r3,[r5,#2]`, `strh r3,[r5,#4]`) in `0x0805797C`: the zero register is the local's register.
- **`int pl = p & 1;` first**, then `ZB(pl, zone)` gives the ROM's evaluation order (`ands` before the `0x94` multiply) in `0x0805763C`.
- Reading a card id as a word, `(CARD_WORD(zone->card) << 20) == 0`, gives `ldr; lsls #20; cmp #0`, while testing the bitfield `z->card.id` gives `ldrh`.
- **Zone address `z*0x94 + p*0xD64 + base`** (macro `ZB`, base added last) is the ROM's association; with the base first the `add` order is wrong.
- **Table access**: `gUnk_08622AB4[...]` through an integer-constant pointer (`((const u16 *)0x08622AB4)[...]`) is used for the card-number/type tables; `0x080578F4` needs the priority list as an `extern const u16 gUnk_0819D316[]` indexed by an unsigned counter so gcc strength-reduces it like the ROM.
- **Type of the loop counter**: an outer `u32 k` (`cmp #25; bls`) in `0x080578F4`.
- **`switch` for a small contiguous range** (`0x080578AC`): the ROM's `cmp #0x14; bgt; cmp #0x10; blt` is old_agbcc's code for `switch (t) { case 0x10: ... case 0x14: }`, *not* nested `if (t <= 0x14) { if (t >= 0x10) ... }`. The nested form is canonicalised to `cmp #0xF; ble` (and `t <= 0x14 && t >= 0x10` becomes a `subs/cmp/bhi` range check), so only the switch reproduces the two separate signed compares.
- **`do { } while` to keep a variable-bound loop counting up** (`0x080578AC`): a `for (i = 0; i < count; i++)` whose index is only used for addressing is reversed by old_agbcc into a decrementing counter. Writing it as `if (i < count)` + `do { ...; i++; } while (i < count)` prevents the reversal and keeps `i` in r2 with the unused pointer in r1, matching the ROM.
- **Separate guarded loop bound and count load** (`0x080577FC`/`0x08057854`): load the byte count into a word local, then assign a word `bound = count` inside the nonempty guard. `577FC` also needs the initialized count constraint to r0; `57854` matches with a normal word local. This gives the post-guard copy to r4 without changing the unsigned byte count semantics.

## Nonmatching notes

- `0x080577FC` / `0x08057854` now match together. Keep the `if` + forward `do/while` and switch range check, scope the integer base pointer to the nonempty branch, then use `base += offset` before converting to the advancing card pointer. This reproduces the in-place address-add operand order; declaring the base at function scope adds an unwanted count-address copy and swaps the loop locals. Initialized base r1, offset r6, and per-iteration table r6 constraints remain; an input-only barrier on the initialized list offset fixes its literal order. The table read/write barrier and explicit mask, bound, and card-id constraints were removed after complete-unit checks. This is documented as `FAKEMATCH`; no assembly instructions are emitted. Checked with `tools/check.py code_08056ECC`: all 0x1014 bytes MATCH, with 17 C functions and 2 assembly fallbacks. Private minimized evidence: `build/manual_late_next/sub_080577FC.min.exact.c`, `sub_08057854.min.exact.c`, and `sub_08057854.pair.unit.c`.
- Earlier `s16 count` attempts reproduced the forward loop but hoisted the table; a two-minute register-allocation permuter found no improvement from that draft. A table-pointer read/write barrier reached an eight-line miss. The scoped base/offset approach above resolved the remaining count-copy, literal-order, and commutative-add differences.
- `0x0805748C` is matching: reuse the verified `0x08057550` sibling, with `bestIdx = -1`, then `best = bestIdx`, and the greater-than comparison. Assigning the initial score through `bestIdx` reproduces the ROM high-register copy; separate `best = -1` copies from r0 instead. Initialize the iterator before `pl = 1; pl &= p`. Only the initialized parity constraint to r8 is needed (documented `FAKEMATCH`); both accumulator constraints were removed after whole-unit checks. No empty barriers or assembly instructions are used. Checked with `tools/check.py code_08056ECC`: 19/19 functions and all 0x1014 unit bytes MATCH.
- `0x08057550` (solved): the product-hoist is defeated by pinning both the accumulators and the parity: `register int bestIdx asm("r10") = -1; register int best asm("r9") = 99999; register int pl asm("r8");` then `int i = 0;` (init before `pl`) and `pl = 1; pl &= p;` (`pl = p & 1` emits a shorter/shifted form). With every callee-saved reg spoken for, LICM can no longer hoist `pl * 0xD64` and the loop body matches exactly. The sibling now matches using the same initialization pattern and only the parity constraint, as described above.
- Historical (matched in wave 3, see below): `0x08057C94`: whole structure matches (bubble sort, switch tree, plan copy). The ROM spills `n`, `done`, `last` and keeps the `0x2E8` switch constant hoisted in `sl`, while the build keeps `n`/`last` in `sl`/`r9`.
- `0x08056ECC`: control flow reproduced (two nested `switch`es, loops, inlined `CardValue` / `CardDefValue`); the build is 0x48 bytes shorter because the `0x7FF` mask and the tables stay in hoisted registers in the ROM (so `(w << 20 >> 20) & 0x7FF` is not fused) and the allocation differs.

## Wave 3 matches (2026-10-01/02)

Working notes: `build/wf/sub_08057C94/NOTES.md`.

### `sub_08057C94` (0x174, start score 116; ordinary C)

The parked draft had the whole structure, with three differences. The `0x2E8` switch constant was not hoisted into `sl`, because loop.c's `13 * savings * life >= insn_count` test failed (life 4 against 57+ insns). The registers differed: the ROM keeps i, j and k all in r5, the PRE copy of j+1 in r1, the switch value in r2, `ok` in r3 and `a` in r8, and spills n, done and last. And the zone address came out as `mov; mul; ldr` instead of the ROM's `mov r0,#0x94; ldr r1,=0x0201A070; mul`. Fixes, in order:
1. A `u16 idb` card id gives the HImode `0x7FF` constant (`ldr rX; add r0,rX,#0; and`).
2. In the sort's inner loop `for (i = 0; i < last; i++)`, write `next = i + 1; b = cand[next];` and store `cand[i + 1] = a`. GCSE PRE then makes the ROM's end-of-block copies (`add r1,r5,#0` / `add r5,r1,#0`), and the constant's lifetime becomes 5, so loop.c hoists `0x2E8`.
3. Write the collecting loop as `cand[n] = i; n++` (a giv-reduced pointer, which initialises i first).
4. **Use one counter variable `i` for all three loops** (score 147 -> 2). There is then one call-crossing pseudo, which lands in r5, and the rest of the allocation falls into place.
5. **Add an unused `ida = CARD_ID(CARD_WORD(ZB(1, a)->card));` before idb** (2 -> 0). Its dead load is deleted, but CSE has already bound idb's `0x94` and zone-base loads to the registers set for ida, which gives the order `0x94`, base, `mul`. The source comments this dead local but does not mark it FAKEMATCH.

Failed: struct-indexed, constant-pointer and reordered `ZB` forms, and `->card.id` bitfield access, did not change the mov/ldr/mul order. `asm volatile("" ::: "r1")` / `"r5"` clobbers to steer the index and the copy broke the hoisting (116).

## Open questions

> [!question] `sub_08044224(player, number, flag)` (used as a count) and `sub_0804A528(player, zone, flag)` / `sub_0804A3D8(player, zone)` (zone eligibility) are only known from these callers; names above are guesses.

Related: [[code-080609c4]], [[code-08009a68]], [[compiler-flags]], [[decomp-workflow]].
