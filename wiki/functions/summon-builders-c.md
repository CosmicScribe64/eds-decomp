---
title: Unit summon_builders (duel AI: action records, zone/hand pick helpers)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit summon_builders

`0x08055EB0`-`0x08056ECB`, Thumb, `old_agbcc -O2`. Source: `src/summon_builders.c`. It directly precedes the CPU opponent AI ([[ai-picks-c]]) and shares its helpers (`BestZoneScore`, card tables `0x08621DE0` stats / `0x08622AB4` card number). Unit status: `unit bytes MATCH`, **14/14 functions in C** since workflow wave 1 (2026-10-01), when the last fallback `AiPickWeakestHandCard` matched.

| Address | Size | Status | Purpose (hypotheses) | Proposed name |
|---|---|---|---|---|
| `0x08055EB0` | 0xC0 | matching | `(player, zone)`: fills the action record `0x0201CF90` for the card in a field zone (kind 3, h0C = 3) and calls `SummonAction_Start` | `QueueZoneAction` |
| `0x08055F70` | 0x124 | matching | `(player, card*, c, d, e)`: same record from a card word (kind 4): `f14 = c` (forced to 1 when card 0x47F is on either side or `IsSpecialSummonOnly(id) != 0`), `f15 = d`, copies the card with `CopyDuelCard`, h0C = e then `e \| 4` | `QueueCardAction` |
| `0x08056094` | 0x10C | matching | `(player, card*, c, d)`: like the above with kind 5, `f15 = 0`, h0C = `d \| 4` | `QueueCardAction2` |
| `0x080561A0` | 0x160 | matching | `(player, zone, x, y, z)`: record for hand card `zone` (kind 6, h0C = 5), `f15 = (z == 0)`, unpacks the u16 `y` into two bytes of 3-bit + bit 4 + bit 7 flags; calls `PayChainEnergyCost(player)` first. The hand-card address uses `zone*4 + (player&1)*0xD64 + 0x02019968` | `QueueHandAction` |
| `0x08056300` | 0xB8 | matching | `(a, number)`: predicate on a card number: always true for 0x10-0x14; with `a > 0` also for 0x14F, 0x150, 0x15B, 0x29F, 0x2DA, 0x3BA, 0x3F0, 0x3F2, 0x403, 0x405, 0x406, 0x420, 0x425, 0x42C, 0x49B; with `a > 1` also 0x39, 0x1A3 (compiled from two `switch`es) | `IsSpecialCardNumber` |
| `0x080563B8` | 0xF0 | matching | `(skip, flag)`: pick an opponent (player 1) zone: first a zone holding number 0x2F, then 0x23D; else the zone with the smallest `info[1]*2 + info[2]` (`GetZoneCardStats`), cards failing `IsFusionMonster` only when `flag`; -1 if none | `AiPickTargetZone` |
| `0x080564A8` | 0x9C | matching | `(id)`: level test of a card (types 0x15-0x17 count as level 0, 0x18 as 10): level <= 4 -> 1; 5-6 -> `AiPickTributeMonster(-1,0) != -1`; otherwise needs two distinct picks (`AiPickTributeMonster(-1,0)` then `AiPickTributeMonster(r,0)`) -> tribute-summon check | `AiCanSummon` |
| `0x08056544` | 0x108 | matching | `(skip)`: zone pick for player 1: first zone with card number in 0x780-0x7CF, else the first other zone, where it **writes 0x2F / 0x23D into the ROM card-number table entry** (`strh` to `0x08622AB4 + id*2`, a no-op on ROM; likely an original bug or leftover) and returns it; else the smallest `info[1] + info[2]` | `AiPickZone2` |
| `0x0805664C` | 0x448 | **matching** (wave 1, 2026-10-01) | `(duel, player)`: hand pick, 3 passes: min of `CardValue + CardDefValue` over hand cards (type <= 0x14, `AiIsKeyCard(1, number) == 0`; pass 1 also needs `CardKey == 0`); pass 3 also empty slots; returns index or -1. Plain C rewrite modelled on `AiPickStrongestHandMonster` (see below) | `AiPickHandWeakest` |
| `0x08056A94` | 0x254 | matching | `(duel, player)`: hand pick, strongest `CardValue` among eligible cards (type <= 0x14, not `IsSpecialSummonOnly`, not `AiIsKeyCard(1, n)`), first pass only when level > 4, second pass without the level test | `AiPickHandStrongest` |
| `0x08056CE8` | 0xB0 | matching | `()`: top-level AI hand pick for player 1: with card 0x3F0/0x488 in hand or `AiHasUsableSpellTrap(0x447)` and `CountFreeMonsterZones(1) > 0` tries `AiPickStrongestHandMonster`; else a card with number 0x1DA; else `AiPickWeakestHandCard` | `AiChooseHandCard` |
| `0x08056D98` | 0x6C | matching | `(player, number, limit)`: index of the first deck card (`deck[]`, `+0x7C4`, at most `limit`) with that card number, or -1 | `FindDeckByNumber` |
| `0x08056E04` | 0x64 | matching | same in the hand (`+0x684`) | `FindHandByNumber` (= `FindHandCardByNumber` twin) |
| `0x08056E68` | 0x64 | matching | same in the fusion deck (`+0xA44`, `fusionCount`) | `FindFusionByNumber` |

## Structs and globals
- **Action record** `0x0201CF90` (16 bytes, `struct ActRec` in the unit), consumed by `SummonAction_Start`: bit 0 player, bits 1-5 and 6-13 zone (same value twice), bit 14/15 flags, bits 16-18 and 19-21 two 3-bit values, bits 25/26/28/29 flags, a 16-bit card id at **bit 31** (straddles the word boundary: gcc emits a `strb [+3]` of `id & 1` at bit 7 plus a `strh [+4]` of `id >> 1`), a `DuelCard` copy at `+8`, `+0xC` u16 (3 / `e` / `x | 4` / 5), `+0xE` bits 2-4 "kind" (3 zone, 4 card+, 5 card, 6 hand).
- `DuelPlayer` layout as in [[ai-picks-c]] / [[duel-piles-c]]; player 1 zones start at `0x0201A070`, hand of player p at `0x020192E4 + p*0xD64 + 0x684`.
- Card tables: `0x08621DE0` u32 stats (type bits 20-24, ATK-like bits 9-17, DEF-like bits 0-8, level bits 25-28, bits 18-19 "class"), `0x08622AB4` u16 card number.

## Matching tricks
- **`u16 id` locals change the loop codegen**: `u16 id = CARD_ID(word)` (instead of `u32`) makes old_agbcc hoist the `0x7FF` mask, *not* strength-reduce `i * 0x94` (keeps `mov #0x94; mul`), and fold the zero test into the same register (`AiPickTributeMonster`, `AiPickEffectTribute`).
- **Zone base through an address-suffixed extern** (`extern u8 gDuelZonesP1[]`, `(u32)gDuelZonesP1 + i * 0x94`) makes the base a hoistable pseudo; the plain integer literal `0x0201A070` is loaded after the `mul` inside the loop.
- **Constant-arm `switch` followed by a zero/sign test** gets jump-threaded by gcc (ROM keeps the shared `cmp`). Declaring the result `u8`/`s8` (`CardKey`, level in `AiHasTributesFor`) blocks the threading and gives the ROM's layout.
- **Two separate `switch`es** (`case 0x10..0x14` first, then `if (a > 0) switch`) reproduce `AiIsKeyCard`'s decision tree; `if (n >= a && n <= b)` is folded to an unsigned range test and does not.
- **`switch (number) { case 0x776: return 3; case 0x777: case 0x778: return 1; }`** inside a `static inline` keeps the ROM's un-folded `cmp/blt/cmp/bgt` pair.
- **Field order of `&&` and loop-invariant reads:** `for (i = 0; i < gDuelPlayers[p & 1].handCount; i++)` (count re-read in the condition) is what the ROM hoists; a cached `int n` gives different registers.
- **Address sums:** `(u32)pd + off` with `off` a separate statement (`i * 4 + 0x684`) keeps the ROM's `(pd) + (i*4 + 0x684)` order; `player * 0xD64 + zone * 0x94` written that way evaluates the zone term first.
- Two passes returning through `if (bestIdx >= 0) return bestIdx;` (two returns) avoid a reload jump in `AiPickStrongestHandMonster`.
- `0x080561A0` now matches: split the hand-card address into parity, byte offset and stride, accumulate the offset, and reuse the parity temporary for the initialized hand-buffer base. An empty input barrier on that base retains the ROM r0 literal load. The r0 parity/base and r2 stride constraints plus the barrier are documented `FAKEMATCH`; the offset register constraint was removed after exact whole-unit checks. The five-argument ABI and explicit u16 y/z behavior are unchanged. Verified `tools/check.py summon_builders`: 14/14 functions and all 0x101C unit bytes MATCH.
- Resolved in wave 1: `AiPickWeakestHandCard` register allocation (see [the wave 1 section](#ai-hand-pick-matched-wave-1-2026-10-01)).

## Explicit halfword decoding at the word interface (2026-10-01)

The matching `AiStrategyToonWorld` caller in [[ai-strategy-c]] passes its packed tribute word directly in r3. `QueueSpecialSummonFromHand` now accepts fourth/fifth parameters as words and initializes local `u16 y = packed; u16 z = faceUp;` in that order. This supersedes the older formal-u16 declaration above while preserving its low-halfword behavior. Both conversions reproduce the original prologue; widening only the fourth parameter moved the fifth-argument shift too early. The complete 0x101C-byte unit remains exact, and the full ROM check passes. No instructions or argument locations changed.

## 2026-10-01 card-scan interface reconciliation

`AiFindHandCardByNumber(int player, int numberWord)` explicitly decodes `u16 number = numberWord` for its hand-card comparison. This agrees with the signed 16-bit table load passed as a word by [[ai-turn-steps-c]]. The complete unit bytes are unchanged. These declaration repairs add no coverage; per-unit artifacts are under `build/bigguns-lead2/` and the full ROM passes at `build/lead-pass27/`.

## `AiPickWeakestHandCard` hand-selection drafts, first batch (2026-10-01)

> Superseded: `AiPickWeakestHandCard` matches since the wave 1 rewrite below; the batches here record the earlier pinned drafts.

`AiPickWeakestHandCard` remains ASM. Fresh staging fixes its second/third hand addresses, explicit ATK/DEF intermediates and return reloads. `solo-weakest-branch-1` has the correct 0x448 size, 34 differing bytes / 27 normalized differences; the older `solo-weakest-reduce-71` has 29 normalized differences. Both pass **2,666 finite differential fixtures** and an independent three-pass selection oracle, covering every valid ID, all types, special number/class boundaries, sampled counts 0..80, parity and separate global/passed hand buffers. The original card-number predicate executes. All paths (first/second/third scan and no result), ordered calls, EWRAM/live IWRAM, saved registers and SP agree. Passing the fixtures does not make a draft byte-identical.

Remaining differences are initial mask reloads, one type-mask scratch copy, a commuted pointer ADD and final conditional-branch expansion. `solo-weakest-preheader-4` reaches 21 normalized differences, but changes reload order and is not behavior-tested. Fixed mask seeds and broad clobber variants generally worsened allocation; combining initialized base/scaled inputs preserves the closer draft but did not close it. The bounded permuter rejected GNU statement-expression syntax before searching; it found no candidate. Evidence: `ai_weakest_*.py`, `verify_ai_weakest.py` and `AiPickWeakestHandCard/` under `build/bigguns-lead2/`. No coverage added.

## `AiPickWeakestHandCard` hand-selection drafts, second batch (2026-10-01)

`solo-weakest-def-form-4` improves the 1,096-byte draft to **13 differing bytes / 26 normalized instruction differences**, with the exact 0x448 size, and all **2,666 behavior fixtures pass**. The final loop's short backward branch is restored by removing two redundant empty constraints. Using the existing stats-table symbol for its DEF load restores the r4 literal without reintroducing a constraint. The remaining differences are three mask preheader scratch registers, the first type-mask setup/copy, and the last hand-pointer load/ADD.

Joint release of all subsets of six r4 table bindings, live-mask rewrites with guarded for/do loops, typed DEF address forms and staged pointer forms produced no exact result. `solo-weakest-pointer-stage-9` restores the pointer load register but reintroduces the long conditional branch and leaves 33 differing bytes. The 13-byte candidate is the closest so far; no coverage was added. Evidence: `ai_weakest_{joint_release,live_mask,for_mask,tail_hints,tail_stage,def_forms,pointer_stage}.py`, `verify_ai_weakest.py` and `AiPickWeakestHandCard/solo-weakest-def-form-4/` under `build/bigguns-lead2/`.

## AI hand pick matched (wave 1, 2026-10-01)

`AiPickWeakestHandCard` (0x448) matches in plain C, with no FAKEMATCH. Working notes: `build/wf/AiPickWeakestHandCard/NOTES.md`.

- The old draft was heavily pinned (register asm on r4/r2/r1/r3/r0 plus empty constraints) and still scored 26. The ROM's erratic table-literal registers (r2, r4, r1, r3...) and the `ldr r4,=0x7FF; adds r6,r4` copies are **reload** choices: reload takes spill registers in rotation (`last_spill_reg`), so one extra or missing reload anywhere shifts every later choice.
- Steps: (1) a clean rewrite modelled on sibling `AiPickStrongestHandMonster`, with constant-pointer `CARD_*` macros and `u16 id`, scored 229 (registers only). (2) `CardDefValue`'s default written `((u16)CARD_STATS(id) & 0x1FF) * 10`: the front end shortens the AND to HImode, so 0x1FF becomes an HImode constant that needs a reload (`ldr r3,=0x1FF; adds r0,r3`), and that reload puts every later rotation choice back in step (58). (3) `v = CardDefValue(id) + atk` (DEF first) fixes the `adds r0, r0, r2` operand order (52). (4) The ATK value as a switch-statement macro `CARD_ATK_VALUE(id, atk)` instead of the `CardValue` inline: every arm writes `atk` (r2) directly, with no return-value copy (0).
- The unused `WeakestValue`/`WeakestKey` helpers used only by the parked draft were removed.
- Tip: `cc.sh -dl -dg` in `build/wf/AiPickWeakestHandCard/` shows the reload insns (`set (reg:SI rN) (const_int ...)` before an HImode move). See [[matching-tricks#Register allocation priority and reload rotation]].
