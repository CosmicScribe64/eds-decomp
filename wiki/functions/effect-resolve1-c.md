---
title: Unit effect_resolve1 (duel card-effect executors, part 1)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit effect_resolve1

`0x08030B88`-`0x08031BC7`, Thumb, `old_agbcc -O2`. Source: `src/effect_resolve1.c`. Follows [[effect-prepare1-c]] (the condition predicates); these are the **effect executors** `int f(struct CardRef *ref)` (hypothesis, from the shape: they change life points, move cards, print messages and return a step code). Same `CardRef` as in [[effect-prepare1-c]] and [[card-list-viewer-c]].

Unit status: `unit bytes MATCH` (`tools/check.py effect_resolve1`), **20/20 functions in C** after workflow waves 2-3 (2026-10-02: `0x08030B88`, `0x08031550`, `0x08031780` in wave 3); none stay `INCLUDE_ASM`. After wave 1: 17/20 (2026-10-01, `EffectDarkHoleResolve` added). Complete unit size: `0x1040`.

## Shared headers (header migration, 2026-09-30)
This unit now uses the canonical shared layouts from **`include/duel.h`** (it does not touch `main.h` /
`gMain`): `struct DuelCard`, `struct DuelZone`, `struct DuelZonesPlayer`, `struct DuelPlayer` and the
externs `gDuelZones` / `gDuelPlayers`. Its four local struct definitions and two local externs were
removed; `struct PlayerLP` uses were switched to the canonical tag `struct DuelPlayer` (`count904` maps to the
canonical `graveCount`, which is not referenced here). **No local views were kept**: even the `+0x06` flag byte maps
onto the canonical `flag6_0`/`flag6_1` bitfields without changing codegen. `(ZFLAGS & 3) == 1` became
`z->flag6_0 && !z->flag6_1`, `(ZFLAGS & 3) == 2` became `z->flag6_1 && !z->flag6_0`, `ZFLAGS & 1`/`& 2`
became `z->flag6_0`/`z->flag6_1`, and `str % 0x94`/`str % 0xD64` zone addressing is kept via the local `ZB`
macro over the canonical `gDuelZones`.

## Return value and step machine (hypothesis)
Every function returns 0 ("nothing / done") or a step code that the caller stores and passes back in (`0x64`, `0x7C`-`0x80`). The current step is the byte at `0x02017A40 + 0x3E0` (`EFF_PHASE`); the player currently being processed is `+0x3E1` (`EFF_SIDE`). Multi-step effects run through `0x80` (start), `0x7F`, `0x7E`, `0x7D` and `0x7C` in that order, each case returning the next code. Nearly every function starts with `if (ref->skip4) return 0;` (`ref+4` bit 2, the "effect negated / skipped" flag, hypothesis).

## Struct CardRef (effect side, 0x14 bytes)
`+0 u16 id`, `+2 bit0 player`, `+2 bits 4-9 zone`, `+2 bits 10-15 kind`, `+4 bit 2 skip4`, `+0xA bits 0-2 numTargets`, `+0xC u16 targets[2]` (low byte player, high byte zone of each target). The byte at `+2` is also read raw (`1 & ((u8 *)ref)[2]`) to get the player as an `and` instead of a bitfield shift.

## Functions

> [!warning] Contradiction
> The `0x08031780` row below said (before 2026-10-02, from the old draft) that the deck search looks for card number 0x1A9. The wave 3 match (2026-10-01, `build/wf/EffectThunderDragonResolve/NOTES.md`) shows the ROM builds 0x1A8 (`mov #0xD4; lsl #1`), and the matched source compares `n == 0x1A8` and calls `AddDeckCardToHand(p, 0x1A8)`. Resolved in favour of the matched source.

| Address | Size | Status | Purpose (hypotheses about role, verified about logic) |
|---|---|---|---|
| `0x08030B88` | 0x2C4 | **matching** (wave 3, 2026-10-02) | 5-way step machine (0x7C-0x80) on the card-list viewer `0x0201D810`: step 0x80 checks for card numbers 0x3D/0x3E/0x4E1 on the field, then searches the list for them and opens the viewer (`AiPickCardListEntry`, `row = 0`, `top = i`) or shows a text box (`TextBoxOpen`); 0x7F `CardListView_Open(p, -1, no, 0)`; 0x7E prints message 0xC2/0x65 for the selected entry; 0x7D `QueueSpecialSummonChoosePosition`; 0x7C message 0x60 |
| `0x08030E4C` | 0xB8 | matching | one target; if its card number is 0x4B1 and it is face-down-set (`(ZFLAGS & 3) == 1`): message 0x7F and `ShowActivatedCard`; else if flag bit 0: `ChangeBattlePosition(tp, tz, 1, 1)` |
| `0x08030F04` | 0x80 | matching | for both players, zones 0-4: occupied, flags `& 3 == 2`, `GetZoneCardType == 1` -> `ChangeBattlePosition(p, i, 0, 0)` |
| `0x08030F84` | 0x78 | matching | if the field word at `0x020198D4 + p*0xD64` is set: message 0x11 with `GetFieldMagicIndex(number)`, `EventResponse_Request(1 - p, 0x18, 0)` |
| `0x08030FFC` | 0x98 | **matching** (wave 1, 2026-10-01; FAKEMATCH) | phase 0x80/0x7F: scan the other side's zones 0-4 with `IsZoneTargetable`, `DestroyFieldCardByEffect` + `OnCardDestroyedByEffect`; returns 0x7F |
| `0x08031094` | 0x58 | matching | first opponent zone 0-4 accepted by `IsZoneTargetable`: `DestroyFieldCardByEffect(opp, i)`, `OnCardDestroyedByEffect(p, opp, i)`, return 0x80 |
| `0x080310EC` | 0x94 | matching | by card number 0x151/0x153/0x154/0x3EE -> amount 200/600/800/400; target byte 0 = own life points, 1 = opponent's: `GainLifePoints(player, amount)` |
| `0x08031180` | 0x88 | matching | card number 0x152/0x155/0x523 -> `GainLifePoints(p, 500/1000/1000 (+ opponent 1000))` |
| `0x08031208` | 0x12C | matching | same idea for 0x156-0x15A, 0x3EF, 0x40F with `LoseLifePoints` (amounts 200/500/600/800/1000+500 own/300; 0x40F = 200 * opponent's hand count) |
| `0x08031334` | 0x88 | matching | phase 0x80: for the opponent's zones 0-4 with a face-down (flag bit 1 clear) card: `FlipFieldCard(opp, i, 1)`, `ShowRevealedCard(opp, id)`; returns 0x7F; other phases call `ApplyKotodama` |
| `0x080313BC` | 0x54 | matching | one target that holds a card -> `QueueAddZoneLink(player, player | zone << 8, target, 2)` |
| `0x08031410` | 0x7C | matching | for the opponent's zones 0-4 that hold a face-down card: `FlipFieldCard(opp, i, 1)` |
| `0x0803148C` | 0x30 | matching | `if (ReturnGraveyardCardToHand(p, 0x3EB) == 0) ReturnGraveyardCardToHand(p, 0x40A)`; returns 0 |
| `0x080314BC` | 0x94 | matching | `TributeMonster(p, zone)`, then for up to 2 targets that hold a card and pass `EffectBlastJugglerCheck(ref, target)`: `DestroyFieldCardByEffect` + `OnCardDestroyedByEffect` |
| `0x08031550` | 0x230 | **matching** (wave 3, 2026-10-01) | like `0x08030B88` for card numbers 0x1A3 / 0x1F9 (viewer at `0x0201D810`, `QueueSpecialSummon`, `LoseLpOnSendToGraveyard`); first checks `fusionCount != 0` (0x1A3 also `CountFreeMonsterZones(p)`); player 1 opens the viewer (returns 0x7E), player 0 shows text `gStrCyberSteinSelectPrompt` / `gStrGaleDograSelectPrompt` (returns 0x7F); 0x7E message 0xDC; 0x7D: 0x1A3 `QueueSpecialSummon(.., skip)`, 0x1F9 message 0x7C + `LoseLpOnSendToGraveyard(p, 1)` |
| `0x08031780` | 0x178 | **matching** (wave 3, 2026-10-01) | phase 0x80 sets `EFF_SIDE = 2`; 0x7F: deck search (`+0x7C4` list) for card number 0x1A8 (formerly given as 0x1A9, see the contradiction note above); on a hit builds a text from `0x08623DF4[n]` into a 0x100-byte stack buffer (`FormatStr`) and shows it (0x7E); else message 0x60 (0x64); 0x7E: `AddDeckCardToHand(p, 0x1A8)` and counts `EFF_SIDE` down |
| `0x080318F8` | 0x48 | matching | `GainLifePoints(p, 3000)`, message 0x92 (0x8092 for player 1) |
| `0x08031940` | 0xE4 | matching | one target holding a card: face-up -> `DestroyFieldCard(tp, tz, 1)` unless its type is 21 (Magic); face-down -> messages 0x7F, `ShowRevealedCard`, then same |
| `0x08031A24` | 0xC8 | matching | player 1: if own LP <= 999 and opponent LP > 2000: message 0x43, `LoseLifePoints(1 - p, 1000)`; player 0: phase 0x80 shows a text (`gStrNeedleBallPayLpPrompt`), 0x7F does the LP change if `0x0201AE60+0x14` is set |
| `0x08031AEC` | 0xDC | matching | player 0 only: phases 0x80 (needs hand count != 0, text `gStrYadoKaruReturnPrompt`), 0x7F (text `gStrYadoKaruSelectPrompt`), 0x7E (`DuelCursor_PickTarget(1)`, then `ReturnHandCardToDeck`) |

## Globals
- `0x02017A40`: effect state. `+0x3E0` step byte, `+0x3E1` current side.
- `0x020192E4 + p*0xD64`: player state (see [[effect-prepare1-c]]): `+0` LP, `+2` hand, `+3` deck, `+5` fusion counts, `+0x7C4` deck list, `+0x5F0` (`0x020198D4`) one card word.
- `0x0201D810`: card-list viewer ([[card-list-viewer-c]]); `+0x20C` is a `u16[]` per-entry kind (hypothesis), `+0xC` card words.
- `0x0201AE60 + 0x14` u16 flag; `0x0201CFB0 + 0x82C` word (passed to `ReturnHandCardToDeck`); `0x02015F00 + 0x1B22` u16 (saved list position, hypothesis).
- Helpers: `GainLifePoints` and `LoseLifePoints` change life points (two variants; hypothesis: one is instant, one animated), `DuelCmd_Push(msg, a, b, c)` shows a duel message (msg | 0x8000 = player 1), `TextBoxOpen(0x205, 0x914, 0xB, text)` opens a text box.

## Proposed names
`EffectRaigekiResolve` `Effect_DestroyFirstOpponentMonster`? (hypothesis), `EffectDamageOpponentResolve` / `EffectGainLpChosenPlayerResolve` / `EffectGainLpResolve` `Effect_ChangeLifePoints_ByCard`, `EffectDragonCaptureJarResolve` `Effect_FlipAllFaceUpMonsters` (hypothesis).

## Matching tricks
- **Named zone pointer even for one use.** `struct DuelZone *z = ZB(p, i); if (CARD_ID(CARD_WORD(z->card)))` with `int p = ...& 1;` as its own statement matched `EffectDarkPiercingLightResolve` and `EffectSwordsOfRevealingLightResolve`; reading the flag byte off `ZB(p, i)` folded inline did not (the offset `+6` merged into the address).
- **Two zone reads in the same function need two locals** (`p`, `p2`), each with its own `ZB`; that reproduces the ROM keeping the zone-array base literal in a callee-saved reg.
- **One target's player/zone**: `u8 tp = ref->targets[0]; int tz = ref->targets[0] >> 8; int p = tp & 1;` (`ldrb` then `ldrh`+`lsr`). `& 1`, not `& ref->numTargets`, when `numTargets == 1` was just tested.
- **Array of targets in a loop**: use `ref->targets[i]` three times, not a pointer local (a `u16 *t` local gets strength-reduced, the ROM's does not) (`EffectBlastJugglerResolve`).
- **Early return vs loop shape**: `if (skip) return 0; for (...)` (found-block after) matched `EffectRaigekiResolve`; `if (!skip) { for ... }` did not.
- **Switch layouts**: for a 2-3 case `switch` the case bodies appear in source order while the compare chain is ascending, so put `case 0x80:` first if the ROM has it first (`EffectNeedleBallResolve`, `EffectYadoKaruResolve`). Writing the last case as `if (call != 0) { ...; return 0x80; } return 0x7E;` let the `mov r0,#0x7E` be shared (`EffectYadoKaruResolve`).
- **`1 & ((u8 *)ref)[2]`** gives the ROM's `mov r0,#1; ldrb; and` where `ref->player` gives `lsl #31`.
- **Second argument computed first**: `int a = 1 - ref->player; ...; f(a, ...)` fixed operand order (`EffectDamageOpponentResolve`).
- **Card number from a card word**: `u16 id = CARD_ID(w); CARD_NUMBER(id)` keeps `& 0x7FF`; `CARD_ID11(w)` (`lsl 21; lsr 20`) is the form `gCardIdToNumber[...]` folds to when the id is used directly.
- **Don't pre-store a table lookup in a local** (`EffectStopDefenseResolve`): `u16 no = *(u16 *)((u8 *)gCardIdToNumber + (CARD_ID11(w) << 1)); if (no == 0x4B1 && ...)` came out one register off (the flags byte load took r1 instead of r2); writing the test straight as the macro `CARD_NUMBER(CARD_ID11(w)) == 0x4B1 && ...` matches. One pseudo fewer shifts what the local allocator hands the next temp.
- **Flat early returns defeat cross-jumping** (`EffectDestroyMagicTargetResolve`): the ROM keeps two separate identical
  `DestroyFieldCard(player, zone, 1)` call tails; with the nested `if (!skip && n == 1) { if (id) { ... } }`
  form old_agbcc merges them (8 bytes short). Rewriting in the sibling `EffectReaperOfTheCardsResolve` style keeps both tails and matches: two flat
  `if (...) return 0;` guards and a bare `if (id == 0) return 0;`. The
  `!=`/`==` polarity of the two type-21 tests must follow the ROM (`!=` for the face-up call, `!=`/`else`
  for the face-down side).
- **A redundant `1 &` on a 1-bit bitfield can place a hoisted constant** (`EffectFieldMagicResolve`): the ROM keeps the literal `1` in r6 and initialises it *between* the `lsl #31` and the `lsr #31` of the bitfield extract, i.e. where the constant's `SET` sits in the expand queue. Writing the index as `gDuelFieldZone[1 & ref->player]` (the `and` is folded away, but its constant stays there) matches; plain `ref->player` puts the init at the first use instead. Same trick when a constant must be hoisted early: give it a syntactic use next to the expression that ends up next to its `mov`.
- **Shared LP-call tail and opponent subtraction** (`EffectGainLpResolve`): use `u16 lp` to preserve the 155/523 shared tail that rematerializes 1000 in r1. After the first 523 call, assign `int other = ref->player`, pass that initialized value through `asm("" : : "r"(other) : "r0")`, then compute `p = 1 - other`. The empty constraint retains the reloaded player in r1 while r0 receives 1, matching the ROM subtraction; it emits no instructions and is marked FAKEMATCH. Original function and callee ABIs are unchanged. Verified complete `0x1040`-byte unit, 20/20 function bytes matching.

## Open problems
- Resolved `EffectDarkHoleResolve` (wave 1, 2026-10-01; see [the wave 1 section](#effect-executor-matched-wave-1-2026-10-01)): the ROM reloads the step byte after storing the side byte (reg+reg addressing), i.e. alias-unknown stores.
  A bounded 2026-09-30 isolated batch reused [[effect-resolve7-c]]'s volatile byte-reread pattern: local `base` plus `offset = 0x3E0`, volatile phase decrement, then a separately initialized tail base passed through an empty read/write constraint and a named r0 side pointer. Reading `ref->player` before the final volatile side read reproduces the whole tail. This candidate is `0x98` bytes and differs in only one instruction: ROM increments the existing r4 offset to `0x3E1`, while old_agbcc reloads the literal into r4. Offset input/read-write constraints before/after the phase pointer and before the side store, including named r4, did not match and often changed pointer allocation. Candidate remains isolated and the production fallback stays assembly. Scratch scripts: `build/middle_experiments/state_scan*.py`.
- Historical (matched in wave 3): `EffectElegantEgotistResolve`, `EffectCyberSteinResolve`, `EffectThunderDragonResolve`: logic decoded, register allocation / strength reduction differ. See [Wave 3 matches](#wave-3-matches-2026-10-0102).

## Effect executor matched (wave 1, 2026-10-01)

`EffectDarkHoleResolve` (0x98, start score 40) matches. Working notes: `build/wf/EffectDarkHoleResolve/NOTES.md`. Its twin `EffectHeavyStormResolve` in [[effect-resolve5-c]] was matched by porting this shape.

- Shape of the matched twin `EffectDestroySpecialSummonedResolve` ([[effect-resolve11-c]]): `switch (EFF_PHASE) { case 0x80: EFF_SIDE = ...; EFF_PHASE--; case 0x7F: ... }`. Storing the side **before** the phase decrement gives the ROM's phase reload, and the `0x3E0` to `0x3E1` `add r4, #1` comes for free from the post-reload move2add pass. This resolves the old one-instruction miss (literal reload instead of the increment). Score 40 to 20.
- Tail: `sd = ref->player; if (*(volatile u8 *)p == sd)`; reading `sd` first keeps the `lsl/lsr` before the `ldrb`.
- FAKEMATCH: the tail pointer `register u8 *p asm("r0"); u8 *b = gChain; p = b + 0x3E1;`. Pinning `&EFF_SIDE` directly folds base+0x3E1 into one pool literal; going through `b` keeps the add. Unpinned, the pointer always lands in r1, because local-alloc gives the constant 1 to r0 first (score 20).
- Failed for the unpinned tail: u8/int `sd`, operand order, volatile on every access, an explicit `int one`, `(u8)` casts, a struct view, break forms.
- Cleanup idea: ordinary C that makes the pointer quantity outrank the constant-1 quantity in local-alloc.

## Wave 3 matches (2026-10-01/02)

All three match in ordinary C. Working notes: `build/wf/EffectElegantEgotistResolve/NOTES.md`, `build/wf/EffectCyberSteinResolve/NOTES.md`, `build/wf/EffectThunderDragonResolve/NOTES.md`.

### `EffectElegantEgotistResolve` (0x2C4, start score 220; ordinary C)

- Loop addressing (score to 66): `gCardListView.cards[i]` (an ARRAY_REF) makes expand put the struct symbol in a register and add 12 afterwards. The ROM loads `sym+12` (`0x0201D81C`) as one pool constant, which `*(gCardListView.cards + i)` reproduces. Related-value CSE then rewrites the found blocks' `row`/`top` stores as `r7-12` / `ip-12`.
- List position (66 to 4): `*(u16 *)&gAiWork[0x1B22]` and `((u16 *)gAiWork)[0xD91]` both fold to one `sym+0x1B22` constant. A struct view with the field at `+0x1B22` (`S15F00_30B88->listPos`) gives the ROM's `ldr sym; ldr 0x1B22; add`.
- Hoist order (4 to 0): in loops 1 and 3 the ROM hoists the card table `0x08622AB4` after the strength-reduced pointer copy, i.e. in loop.c's second pass. `move_movables` starts with threshold 26 and lowers it by 3 per moved insn, so the third movable is rejected when the loop has more than 20 insns. Loops 1 and 3 had 19; a `u16 id = CARD_ID(...)` local adds truncate/extend insns (24, removed later by combine). Loop 2 already had 22 insns because it builds 0x4E1.
- Failed: the extern table `gCardIdToNumber[...]` (36), a local table pointer in the loop (hoisted first, 28), `u16 no = CARD_NUMBER(...)` (17 insns), a single-case switch.

### `EffectCyberSteinResolve` (0x230, start score 129; ordinary C)

- `u8 skip = ((u8 *)ref)[4] & 4;` instead of `s16`: the `lsl/lsr 24` extension lands in r3, which is also the fifth argument of `QueueSpecialSummon` in case 0x7D.
- Case 0x1A3: `AiPickCardListEntry(ref->id)` before `row = 0` (the draft had them swapped). List position through the same `+0x1B22` struct view as `EffectElegantEgotistResolve`.
- The key: the ROM's `mov r4, #1` sits between the `lsl` and `lsr` of the player extraction in case 0x80 and is reused across the `CountFreeMonsterZones` call by both `1 & ((u8 *)ref)[2]` tests. It comes from indexing `gDuelPlayers[1 & ref->player]` (the redundant-`1 &` trick of `EffectFieldMagicResolve` above): combine folds the `and` away, the constant-1 register stays and CSE hands it to the later tests (one r4, ref r5, byte2 r6).
- Failed: an explicit `int/u8 one = 1` with `one &= b` (right shape, but global-alloc priority swaps ref/one between r4/r5); pinning one or ref with `register asm` (breaks the CSE of byte2, and cross-jumping merges the two bodies).

### `EffectThunderDragonResolve` (0x178, start score 108; ordinary C)

- The card number is 0x1A8, not the draft's 0x1A9. The deck is read as the `DuelPlayer` field `gDuelPlayers[1 & ref->player].deck[i]` (ROM computes `base + 0x7C4`), not a separate `gDuelDecks` extern (106 to 98).
- Hoist order of the `0x7FF` mask, measured from `-dL` dumps: threshold 26 for this call-free loop, minus 3 per move; a movable is hoisted when threshold * savings * life >= insn count. A literal `& 0x7FF` (life 1) is never hoisted, a `u16 id` hoists it in pass 1 (too early). Fix: `u32 mask = 0x7FF;` declared after the deck-word read (life 3: 11*3 < 39 in pass 1, 26*3 >= 29 in pass 2).
- Found block: the cast constant `((const u16 *)0x08623DF4)[n]` computes `n << 1` before loading the base and keeps n in r0 (the extern-array form loaded the base first). This alone took the score from 119 to 0.
- Failed: a `deck[i].id` bitfield (`ldrh`, 94), a u32 bitfield view of `player` (the whole player*stride chain hoisted, 279), the extern `gCardIdToNumber` (table hoisted instead of the mask, 99), a mask local at the top of the loop body (life 17, hoisted in pass 1), dropping the `1 &`.
