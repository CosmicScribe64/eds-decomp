---
title: Unit effect_hooks (per-card duel effect handlers and effect-table lookup)
type: function
status: solid
confidence: low
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit effect_hooks

`0x08046738`-`0x08048FE0` (exclusive), Thumb, `old_agbcc -O2`. Source: `src/effect_hooks.c`. Follows [[effect-target-collect-c]]. The unit starts with a run of small **per-card effect handlers**. Each takes a player, checks `CountActiveCardsOnField(player, cardNumber)` / `CountFaceUpMonstersByNumber`, then queues a message via `DuelCmd_Push`, a life-point change via `LoseLifePoints/80/0802272C` or a destroy via `DestroyFieldCard`. After them come the **effect-table lookup** `FindCardEffect` and the summon/tribute selection state machine `CardMenu_SummonMonster` (0x1DF8 bytes, matched on 2026-10-02).

Unit status: `unit bytes MATCH`, **23/23 functions in C**, the whole unit (2026-10-02). The last one, the 7.7 KB state machine `CardMenu_SummonMonster`, matched in the giants loop after the workflow waves; see [Giant match](#giant-match-2026-10-02). Before that: 22/23, with C function extents of 0xAB0 (2,736) bytes of the unit's 0x28A8.

| Address | Size | Status | Proposed name | Purpose (hypotheses about role) |
|---|---|---|---|---|
| `0x08046738` | 0x78 | matching |  | `(player, zone)`: if the face-down zone card (flag 2) exists and card 0x52 is present for either side, `ShowActivatedCard(player, id)` then `QueueAddZoneLink(player, id, player \| zone<<8, 0xD)`. Zone address uses a separate symbol-backed byte base |
| `0x080467B0` | 0x58 | matching |  | life-point effect of card 0xA5: `GainLifePoints(p, 500 * n_p)` and `(1-p, 500 * n_other)` |
| `0x08046808` | 0x114 | matching C |  | if card 0x148 is present: for monster zones 0-4 of both players with face-down flag pattern `(flags6 & 3) == 2` and `GetZoneCardType(p,z)==1`, message 0x73 (`\|0x8000` for player 1) then `ChangeBattlePosition(p,z,0,0)` on each. FAKEMATCH next-player local in r6 |
| `0x0804691C` | 0xC0 | matching C |  | 2-step state machine on byte `0x020192E0+0x1B22` (step 0: needs `CountGraveyardCardsByNumber(p, 0x1DA)`, sets `0x0201AE60+0x14` or prints a card-name text via `FormatStr`+`TextBoxOpen`; step 1: `ShowCardEffect`, `ReturnGraveyardCardToHand(p, 0x1DA)`) |
| `0x080469DC` | 0x98 | matching C |  | for both players, spell/trap zones 5-9 holding a face-down card of type 0x15: send message 0xB1 (`\|0x8000` p1). Narrow ID local and inline lookup argument reproduce the allocation |
| `0x08046A74` | 0x5C | matching |  | card 0x436: `n` = count for both sides, message 0x43 (`\|0x8000`) with arg `(u16)(n*500)` |
| `0x08046AD0` | 0x84 | matching |  | card 0x464: if `CountOtherFaceUpSameNameMonsters(p,z)>0` for any monster zone, `ShowCardEffect(0, gCardNumberToId_Kotodama[0])` and `DestroyFieldCard(p,z,1)` for each |
| `0x08046B54` | 0x54 | matching |  | card 0x464 variant for one zone: `CountOtherFaceUpSameNameMonsters(player, zone) > 0` -> `DestroyFieldCard(player, zone, 1)` |
| `0x08046BA8` | 0x38 | matching |  | card 0x475: `n` copies, `DrawCards(player, 2*n)` |
| `0x08046BE0` | 0x40 | matching |  | card 0x476: `DuelPrompt_PostDiscard(1-player, mul*n, 0, 1)` |
| `0x08046C20` | 0x4C | matching |  | card 0x51A (either side): `LoseLifePoints(player, idx*300)` |
| `0x08046C6C` | 0x44 | matching |  | `(player, zone, flag)`: send message 0xD9 (flag) / 0xD8 (`\|0x8000` for p1) via `DuelCmd_Push(msg, zone, flag?1:0, 0)` |
| `0x08046CB0` | 0x8C | matching |  | `(a, b, c)`: zone card of player b/zone c with card number (via `0x08622AB4`) 0x5EA, `a != b`, card 0x453 absent, `c <= 4`: message 0x4C, `ShowActivatedCard` |
| `0x08046D3C` | 0x150 | matching C |  | picks a card number 0x605..0x608 from bits 2-5 of the zone byte +6, then either announces (`PlaceDeckCardOnField(p, num, FindFreeSpellTrapZone())` true -> `ShowCardEffect(p, cardId)`) or searches the hand for it (message 0xC5, `DuelCmd_Push(msg, id, (i&15)<<4 \| (x&15) \| 0x100, 0)`). Tagged FAKEMATCH register/lifetime constraints |
| `0x08046E8C` | 0x94 | matching C |  | card 0x5FD: count opponent's list cards (`+0xB84` of the player struct) of type <= 0x14; `LoseLifePoints(1-player, 100*n)`. Explicit back edge and FAKEMATCH register bindings preserve hoisting |
| `0x08046F20` | 0x130 | matching C |  | dice: card 0x600, roll = `Random() % 6 + 1`, messages 0xE4 / 0x12, then destroy every face-down zone monster whose level (0 for 0x15-0x17, 10 for 0x18, else `(stats & 0x1E000000) >> 25`) equals the roll (or >5 with roll 6) |
| `0x08047050`, `0x08047054` | 4 | matching |  | `return 0` stubs |
| `0x08047058` | 0x68 | matching |  | `int (id)`: binary search of `gCardEffects` (0x1AA entries of 24 bytes, key `u16` card-number index from `0x08622AB4[id & 0x7FF]`); -1 if absent |
| `0x080470C0` | 0x54 | matching |  | `u16 (ref, a, b)`: NULL -> 0; effect entry of `ref->id` via `FindCardEffect`; none or `fn == 0` -> 1; else call `fn(ref, a \| b<<8)` (u16 result) |
| `0x08047114` | 0x5C | matching |  | predicate: `!(gDuelPlayers[p&1].byte7 bit 3)` and cards 0x592 (p), 0x5F6 (both) absent |
| `0x08047170` | 0x78 | matching |  | same with bit 4 of byte 7 and also cards 0x5E6 (both) absent |
| `0x080471E8` | 0x1DF8 | **matching** (giants loop, 2026-10-02; FAKEMATCH) |  | summon/tribute and special-summon selection state machine; original ABI `void (u16 faceUp, u16 special)` |

## Structures and tables (hypotheses)
- **`gCardEffects`** effect table: 24-byte entries: `u16 cardIdx` (+0), handler function pointer at **+8** (`u16 fn(ref, u16 posBytes)`, 0 = none), rest unknown. Sorted by `cardIdx`, 0x1AA entries.
- **`0x08622AB4`** maps an internal card ID (low 11 bits) to a card number/key, as in [[duel-ritual-c]]. `0x08623DF4` performs the reverse lookup used for card names and announcements.
- Player struct at `0x020192E4` (0xD64 bytes): `+2` hand count, `+6` count of a second list at `+0xB84`, `+7` flag byte (bit 3, bit 4 tested), hand list at `+0x684` (as in [[duel-ritual-c]]).
- Zone at `0x0201930C + p*0xD64 + z*0x94`: `w0 & 0xFFF` card number, byte `+6` flags (bit1 face-down; bits 2-5 an index 0-3 in `PlaceNextSpiritMessage`).

## Matching tricks
- **Table lookup with a constant index kept in a register (`lsl r0,r6,1; ldr r1,=tab; add`)**: write the table as a **cast literal** `((const u16 *)0x08623DF4)[id]` with `u32 id = 0x475;` also passed as call argument. The symbol form `gCardNumberToId[id]` swaps the add operands. Same for `0x08622AB4` (index `(id << 21) >> 21`).
- **`(u8)a | (u8)b << 8`** with `int` params reproduces `lsl 24; lsl 24; lsr 8; orr; lsr 16` (a two-byte pos argument); the shift-and-mask forms give the wrong order.
- **Many `&&` tests on the same constants**: `if (a == 0 && b == 0 ...)` lets gcc rewrite `0x5F6` as `r4 + 16`; write early-`return 0` statements (`if (f() != 0) return 0;` chain) to force the separate literal loads.
- **Bitfield struct of 4 bytes or less is read as a whole word**: pad it to the real size (`u8 pad[6]; u8 lo:2, kind:4, hi:2; u8 rest[..]`) to get `ldrb`.
- **`switch` on a bitfield** gives the signed `cmp/bgt` decision tree; use an `int t = ...` local for the `switch (t)` of `RollDieDestroyMonstersByLevel` (unsigned expression gives `bcc/bls`).
- **Zone address, separate base (found in [[card-command-menu-c]])**: `int s1 = z * 0x94 + (p & 1) * 0xD64; zn = (T *)(s1 + 0x0201930C);` matches the loop form of `ApplyDragonCaptureJar` (only p+1/pofs registers still swapped). The prologue form of `ApplyPumpkingBoost`/`OnCardDestroyedByEffect` matches with a symbol-backed byte base (see below).
- **Zone address** `zone*0x94 + (p&1)*0xD64 + 0x0201930C` in byte arithmetic (inline `(p & 1)`, not a temporary) matches inside loops (`ApplyDragonCaptureJar` hoisting of `1`/`0xD64`), but as a standalone prologue (`ApplyPumpkingBoost`, `OnCardDestroyedByEffect`) the ROM copies `zone` before multiplying and puts `p&1` in r1; the separate symbol-backed base below reproduces it.
- **Standalone zone address (`ApplyPumpkingBoost`, `OnCardDestroyedByEffect`)**: compute `int s1 = zone * 0x94 + pi * 0xD64`, then use a separate **symbol-backed** byte base `u8 *zb = (u8 *)gDuelZones; z = (struct DuelZone *)(s1 + (int)zb);`. This reproduces the zone-copy multiply and register allocation; the literal address form does not.

## Cleanup verification (2026-09-30)

All eight saved drafts were activated and tested individually with `tools/dr python3 tools/check.py effect_hooks --diff <func>`, including variations of types, statement order, address forms, table access, and loop shape. Two converted: `ApplyPumpkingBoost` and `OnCardDestroyedByEffect`. The six remaining drafts were restored to `INCLUDE_ASM` with their original attempts retained; `CardMenu_SummonMonster` was excluded. Final verification: **23/23 functions match; unit bytes MATCH**.

`SinisterSerpentStandbyStep` matched later. A local base pointer constrained to r0 and an empty input barrier make the base load precede `r1 = 1`, followed by `strh r1,[r0,#0x14]`. The barrier emits no instruction and the constraint is marked `FAKEMATCH`. `tools/check.py effect_hooks` verifies all 23 functions and all 0x28A8 unit bytes. The earlier upper bound `0x0804857F` omitted most of the final dispatcher; the verified unit ends at `0x08048FE0`.

`RollDieDestroyMonstersByLevel` also matched later. An empty input barrier immediately after the dice-roll calculation keeps the roll in r6 and the outer player loop in r7. No fixed register declaration or instruction-producing assembly was needed. The source marks this compiler constraint `FAKEMATCH`; the full unit remains byte-exact.

## Ordinary C scan match and summon reconstruction (2026-09-30)

`DisableFaceUpTraps` matches in ordinary C, with no register constraints or assembly hints. Both the zone card-ID local and `EffectCardType`'s argument must be `u16`; the value is extracted from only 12 bits, so narrowing loses no information. The inline helper reads the stats through the literal pointer `((const u32 *)0x08621DE0)`. Narrowing only the helper left 13 mismatching bytes from an r1/r2 swap; narrowing both the local and helper fixed the full function and unit. Its original `void (void)` ABI, loop bounds, and message arguments are unchanged.

Verification: `tools/dr python3 tools/check.py effect_hooks` reports **23/23 function slices match; unit bytes MATCH (built 0x28A8, target 0x28A8)**. The new C contribution is **one function / 0x98 (152) bytes**. Evidence: `build/bigguns-effects46738/final-check.txt`, `siblings2.log`, and `siblings3.log` (`combo1` is the exact candidate). The full unit test includes the four remaining assembly fallbacks and does not mean those functions have C matches.

### Large state machine: verified structure, unfinished match

The caller in [[duel-cmd-queue-c]] invokes `CardMenu_SummonMonster` with `(0,0)`, `(1,0)`, `(1,1)`, or `(0,1)`. The callee explicitly narrows both incoming arguments to 16 bits. The two downstream placement routines `QueueNormalSummon` and `QueueSpecialSummonFromHand` take player, source index, destination zone, tribute encoding, and face-up flag. These observations support the summon/tribute interpretation; individual state names remain hypotheses.

The ROM has an 85-entry dispatch table (states 0 through 84), occupying 37 physical code chunks including the common default/prologue. The reconstructed stack objects reproduce the 0x118-byte frame: two 0x80-byte text buffers, a 0x14-byte card reference, and the outgoing argument word. The draft preserves all state bodies, the card-number conversion rules, and the two-argument `IsTributableMonster` calls omitted by the raw m2c output. State 81's observed zone lookup uses the cursor's **area** low bit as the player selector; the draft preserves that unusual ROM behavior.

Relevant fields relative to `0x020192E0`:

| Offset / bits | Draft field | Evidence |
|---|---|---|
| `+0x1B28`, halfword | card ID | mapped through `0x08622AB4` for the initial dispatch |
| `+0x1B2A`, halfword | selected/tribute encoding | built during zone selection |
| `+0x1B2C`, bit 1 | active | cleared on completion/cancel |
| `+0x1B2C`, bits 2..5 | command | selected command-menu entry; values 11/12 choose the special path |
| `+0x1B30`, bits 2..9 | step | 85-entry state-machine index |
| `+0x1B31`, bits 2..5 | sequence | three-card selection index in states 40/41 |
| `+0x1B30`, word bits 14..17 | choices | hand/field selection availability |
| `+0x1B33`, bit 1 | player | placement routine argument |
| `+0x1B34`, bits 1..8 | source index | placement routine argument |
| `+0x1B34`, word bits 9..16 | event field | message bits 16..20 after masking |

Several chunks reuse the already-loaded `0x020192E4` or `0x0201930C` base for these tail fields. Modeling those relative views fixed both literal selection and register allocation across related states. In the guarded parked draft, 14 of 37 chunks have identical instruction shape after normalizing PC-relative literals and branch destinations: states 2, 6, 50, 51/54/57, 52/55, 53, 56, 60, 61/64, 62, 63, 65, 72, and 4/82. This diagnostic **is not byte equality or a proof of matching control flow**. The draft remains 0x1E18 bytes versus 0x1DF8 and has 764 normalized instruction-line differences, down from 1,181 in the initial reconstruction. Evidence: `shapes.c`, `shapes.report.json`, `shapes.case*.diff`, and `repair_shapes.log` in `build/bigguns-effects46738/`.

### Earlier state-domain evidence and semantic blocker (2026-09-30)

The matched command-menu handler `CardMenu_Update` in [[campaign-c]] resets the command substate (its `SEL.unk34`, the draft's `step`) to zero when A confirms a command. Within `CardMenu_SummonMonster`, the only explicit entry to state 40 is card number `0x34D` in state 0, which first writes `sequence = 0`. State 40 preserves the sequence and advances to 41. State 41 increments it only after a matching selected card; it returns to 40 only while the new value is at most 2, and otherwise advances to 42. Thus the observed transition graph supports the valid domain 0..2. State 70 is likewise entered from state 0 only for card numbers `0x5EC`..`0x5EF`.

> [!warning] Unresolved invalid-state behavior (historical)
> Resolved by the 2026-10-02 byte match: the active C is byte-identical to the ROM, so it behaves the same on these paths as well. The rest of this callout describes the guarded parked draft of 2026-09-30.
>
> For sequence values 3..15, the state-40/41 assembly switches do not assign the required-card register and continue with its incoming value. The parked draft returns on this path, so it is **not semantically equivalent for arbitrary state memory**. For state 70 with an unrelated card number, the ROM consumes text it did not populate; the draft returns. The local transition graph does not by itself prove that every external writer/callee preserves the invariant. Keep the assembly fallback until the domain, original variable lifetimes, and whole-unit bytes are all resolved. No invented uninitialized register scaffolding was enabled.

A private experiment removed these guards to study the apparent original switch lifetimes, then represented the player flag through a field at offset 0x10. Its best diagnostic reached 692 normalized differences and 15 matching-shape chunks (adding state 58), but remained 0x1E20 bytes and unresolved on invalid inputs. It was **not installed**. Separate per-case required-card locals were worse (710 or 1,165 differences); simply changing signed/unsigned bit tests also regressed codegen. Evidence: `anchor_domains.log`, `anchor_lifetimes.log`, and `lifetime1.report.json`. The guarded source reconstruction is retained to make future work reviewable.

### Bounded same-unit attempts

- `ApplyDragonCaptureJar`: splitting player/zone loop-variable lifetimes in either or both passes, with an explicit player-mask temporary, retained the same 20 differing bytes. The outstanding differences are register choices for the increment and hoisted player stride, plus associated temporary registers.
- `PlaceNextSpiritMessage`: card-number width, card-ID width, symbol-backed zone base, and narrowed temporary experiments did not match. The best tested base-view variant still differed in 86 bytes; no draft was enabled.
- `DamageOpponentPerBanishedMonster`: literal/symbol stats pointers, narrow inline arguments/results, a list-reader helper, and player-index types changed hoisting but did not match. The closest reader experiment still differed in 47 bytes. The original fallback and draft remain.

These earlier failures are recorded in `siblings1.log` through `siblings4.log` under the private build directory. All three smaller functions were subsequently matched in the 2026-10-01 pass below.

## State-by-state matching pass (2026-10-01)

Starting verification: **unit bytes MATCH**, 19/23 functions in C. Scratch candidates are compiled through `tools/check.py` in `build/codex-scratch/codex-effect_hooks/`, and the assembly fallback stays active during experiments. The parked draft initially had 1,170 `check.py --norm` differing lines. Its `SummonDuelFromPlayer.cardId` had become `u8`, and state 80's `number1 = 0x582` had become `s8`. Neither narrowing represents the ROM, and restoring both widths recovers the halfword reads and card-number constant.

The 85-entry outer dispatch already has the correct state grouping. A player-header view with fields at +6 and +0x10 removes extra address arithmetic in states 0 and 70. An early cancellation `break` in state 10 reproduces its shared clear-active tail. Writing the event-position expression with the area operand first gives matching instruction shapes in states 3 and 11. State 71's human branch must increment the step through the main duel base, and its AI branch uses the player-relative view. These are intermediate shape comparisons, not credited C matches. Explicit command casts do not stop the compiler from merging its range tests, and an empty constraint in an inline predicate adds unwanted materialization without improving the result.

`ApplyDragonCaptureJar` matches all **0x114 bytes**. Both player loops keep an explicit next-player local in r6, assigned after `z = 0`, and advance `p` after the inner loop. The symbol-backed zone base preserves the desired address arithmetic. Both register bindings are marked `FAKEMATCH`, and there are no assembly instructions. The restored `u32` card-number local preserves 0x148, and the address offset is an `int`. A direct `tools/dr python3 tools/check.py effect_hooks` run confirms **23/23 slices match; unit bytes MATCH (0x28A8)** after enabling this function.

`DamageOpponentPerBanishedMonster` also matches all **0x94 bytes**. An explicit countdown and back edge keep the stats-table and type-mask loads in the loop. The ID mask alone stays hoisted in r6, the count uses r4, the player offset and remaining count reuse r2, and the list base is materialized in r0. Integer address sums reproduce the ADD operand order. These choices are marked `FAKEMATCH`. The count is an `int`, as in the ROM's unrestricted increment, and the list length is a byte. Enabling both new C functions together passes the full unit check, for **21/23 functions in C**.

The merged state-machine scratch draft reached **615 normalized diff lines**. Mixing literal and symbol-backed card-number table accesses in state 80 prevents an unwanted cached pointer and restores the main duel base to r6. The prologue, states 1, 12, 42, 73, 84, and default then match in normalized instruction shape. Direct switches for command values 11/12 reproduce state 0's signed range tests, and the inline predicate forms tested earlier do not.

`PlaceNextSpiritMessage` matches its **0x150-byte extent**, including the final two-byte alignment. The hand scan retains the full 12-bit card ID, which the parked draft's `s8` local had truncated. Explicit hand-base/offset arithmetic and tagged register and empty-assembly constraints recover the original table-load scheduling, saved position/mask, packed message, and loop-bound reload. The three smaller matches together pass **unit bytes MATCH**, adding **0x2F8 (760) C bytes** in this pass.

Further large-function trials reached **419 normalized lines**, with 32 of 37 physical chunks matching instruction shape. The remaining chunks were states 5, 30, 31, 40, and 80. States 0 and 70 require spelling their address addition as unsigned subtraction of a negated offset to retain the ROM's ADD operand order. State 20 matches by binding its duel base to r8 and player-field pointer to r6, with the pointer assignments embedded in the reference-field assignments.

The improved draft is saved in the source, still inactive. Its obsolete invalid-state return guards were removed to reproduce the original switch lifetimes, and byte equality is still required before activation. The draft is **0x1DF0 bytes** with **340 normalized diff lines** (many are shifted dispatch table addresses). States 5 and 30 match instruction shape. State 5 uses an explicit halfword mask with the mask operand first, preserving its shared update tail. States 30 and 70 prepare the same four prompt arguments before a shared call and step increment. State 31's cursor/table lifetimes line up except for one scratch offset load. The remaining differing chunks are 31, 40, and 80. All register bindings and empty constraints are tagged `FAKEMATCH`, and no assembly instructions were added. Whole-unit verification still reports **unit bytes MATCH**.

## Solo pass 2026-10-01: CardMenu_SummonMonster case 40 now matches; case 80 remains

Historical (matched in the giants loop, 2026-10-02; see [Giant match](#giant-match-2026-10-02)). The case 40 forms below are still in the matched source; the case 80 blocker was solved differently from the "next lever" suggested at the end.

Enabling the parked draft and diffing with `tools/check.py effect_hooks --diff CardMenu_SummonMonster --norm` now shows differences only in case 80 (about 255 differing lines, most of them jump-table words shifted by size; built 0x28B0 vs 0x28A8). The improved draft stays parked (`#if 0`); unit still `MATCH` with `INCLUDE_ASM`. A copy is in `build/fable/CardMenu_SummonMonster/tmp/base2.c`.

**Case 40 fix (verified by diff):** the three `choices` tests and the final `choices==0` test plus `active=0` tail match.
- Tests 1-3: read the word as `raw = (u32)&SD; asm("" : "+r"(raw)); off = 0x1B30; asm("" : "+r"(off)); raw = *(u32 *)(raw + off);` with `raw`/`shifted`/`choices` pinned to r0/r2/r1. This reproduces the ROM's `ldr r0; ldr r1; adds r0,r0,r1` (hard-register operands keep base-first order when both are ascending).
- Final test: use the natural struct-view read `((struct { u8 prefix[0x1B30]; u32 w; } *)&SD)->w & 0x3C000`, then `SD.active = 0`. To make reload choose the ROM spill registers, declare `register char *t3 asm("r3"); register int t5 asm("r5"); asm("" : "=r"(t3), "=r"(t5));` before the test and `asm("" : : "r"(t3), "r"(t5));` on the non-break path. Then the offsets are r0/r3 and the scratch r4, so the `active=0` tail cross-jumps into the shared default tail.

**Compiler mechanics learned (from the agbcc sources, pret/agbcc `reload1.c`, `cse.c`, `local-alloc.c`):**
- Large `base+constant` addresses are not pseudos: reload loads the constant into a *spill register*. `choose_reload_regs` allows only the function-wide set of spill regs (numeric order) minus regs live around the insn, and rotates from `last_spill_reg`. So the register of each `ldr rX,=0x1B30` depends on the previous reload pick plus liveness; keeping other hard regs live (empty `asm` outputs used later) blocks them.
- Local-alloc priority is `floor_log2(refs) * refs * size / live_length`; extra references on a pseudo (even in an `asm`) can swap which of two temporaries gets the lower register.
- `cse` puts a known-constant operand second in commutative ops, so an explicit `base + off` with a constant-valued base prints as `adds rd, off, base`, unlike the reload form `adds rd, base, off`. Hard-register variables sort first.
- Natural spill picks are what the ROM does for the `0x08623DF4` table loads in case 80.

**Case 80 blocker (not solved):** in the ROM the shared `0x08623DF4` table pointer is CSE'd but gets no hard register (r4 names, r5 buffer, r6 base, r7 first item number are all taken), so every use is a reload (`ldr r2`, `ldr r1`, ...), with one pool word. In the draft the CSE'd pseudo wins a register and pushes the item numbers to r8/r9 and the base to r7; mixing numeric and symbol forms avoids that but adds a 4-byte pool word and removes the reload picks that set the rotation for the later `SD.step++` offset registers (ROM: r2, r5, r5; `active=0`: r0 then scratch r2). Pinned-register forms of the table loads reproduce the bytes at those sites but not the following picks. Next lever: lower the table pseudo's allocation priority below the item numbers so it spills (lengthen its range or conflict it with r4-r7) without adding emitted instructions. `build/fable/CardMenu_SummonMonster/{cc.sh,tryh.py,NOTES.md}` hold the dump script (greg/lreg/cse dumps) and variant runner.

## Giant match (2026-10-02)

`CardMenu_SummonMonster` (0x1DF8 bytes, 7,672) matched on 2026-10-02 at 00:31 and was applied with `tools/wf.py apply`: 23/23 functions, `unit bytes MATCH`. It was one of the four "giants" left after the workflow waves (with `GetZoneCardStats` in [[card-stats-c]], [[effect-target-collect-c]] and `DuelPhase_Standby` in [[duel-phases-c]]). The wf queue score was 287 at the start of the waves and 28 when the solo pass above parked it.

Working notes: `build/fable/CardMenu_SummonMonster/NOTES.md` (solo pass: case 40, case 80 diagnosis) and `build/wf/CardMenu_SummonMonster/NOTES.md` (score 32 to the match).

1. **Case 80, blocks 2 and 3** (`first && !second`, `!first && second`): the ROM loads the name-table base by reload, with the rotation picking r4 and then r5 for the 0x1B30 offset. Writing it as `(const char *)0x0822C720 + id * 0x40`, with the integer constant inside the add, reproduces that. The earlier symbol form plus `asm("" : : "r"(gCardNames))` did not (score 28).
2. **Case 80, block 1:** the two `0x08623DF4` table constants must not be shared across the call. If they are, CSE keeps one pseudo, local-alloc gives it a callee-saved register and global allocation shifts. Routing the first lookup through an inline helper (`SummonCardId()`) ends CSE's block at the helper's folded branches, so each lookup gets its own const-in-add reload (r2, then r1). This replaced an `asm("" : "+r"(t))` hack.
3. **Score 0 was not a match.** `--norm` drops branch targets, and `wf.py apply` then refused the function because three branch targets differed. They showed up only in a raw diff (`tools/dr python3 tools/check.py <unit> --src <copy> --diff <func>` without `--norm`, or `rawtry.sh` in the work directory):
   - `SummonLevel` switch: the first jump pass threaded the `return 0` and `return 10` arms past the `cmp r0,#0` head test. Consuming the result with an empty `asm volatile("" : : "r"(level))` keeps them entering the head (FAKEMATCH, the same trick as `IsSpecialSummonOnly` in [[card-detail-c]]).
   - Case 30's far jump went to case 70's prompt tail; the ROM goes to case 4/82's tail. Case 30 now has its own `TextBoxOpen(...); SD.step++;` instead of `goto normal_prompt`, and the old prompt-argument register pins are gone. With the name-table base in integer form (a reload in r5), the 0x1B30 reload becomes r0 and cross-jumping merges the tail into case 4/82's.
   - Case 31 then needed one more reload in the rotation: `register struct SummonCursor *cursor asm("r8") = &SC;` with a plain `int *zonePtr = &cursor->zone;` (the symbol goes through reload r1 and the zone pointer through output reload r2) and a plain `&((const u16 *)0x08622AB4)[id & 0x7FF]` (reload r3). That gives the ROM's r0 for the 0x1B05 offset.

**FAKEMATCH use:** heavy. The function body carries 70 `FAKEMATCH` comments, 47 `register ... asm("rN")` bindings and 51 empty `asm("")` statements, mostly from the state-by-state passes and the case 40 fix above. Each pins a reload or temporary to the register the ROM's rotation chose. This is the opposite end from `GetZoneCardStats`, which matched with two FAKEMATCH forms and no register pins. A cleanup pass could try to replace some pins with ordinary C now that the whole function matches. A pin that was only needed to stabilise a neighbouring block may have become redundant.

