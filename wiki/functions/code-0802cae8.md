---
title: Unit code_0802CAE8 (duel target-rule dispatcher and card-effect steps)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit code_0802CAE8

The unit covers `0x0802CAE8`-`0x0802DB2F` (Thumb, `old_agbcc -O2`), and its source is `src/code_0802CAE8.c`. It follows [[code-0802bad0]] (whose `(ref, pos)` target checks it calls) and [[code-0802aac0]].

Unit status: `unit bytes MATCH`, **31/31 functions in C** since workflow wave 1 (2026-10-01), when the last fallback `sub_0802D058` matched (0x1048 bytes, verified with `tools/check.py code_0802CAE8`).

## Shared headers

This unit uses `include/duel.h` (`struct DuelCard`, `DuelZone`, `DuelZonesPlayer`, `DuelPlayer`, `DuelState`; `gUnk_020192E0`, `gUnk_020192E4`, `gUnk_0201930C`) and `include/duel_ui.h` (`struct DuelScreen`; `gUnk_0201CFB0`). It does not use `include/main.h`. Local definitions of `DuelCard`, `DuelZone`, `DuelZonesPlayer`, `DuelPlayerHead` and `DuelState` and the `extern` for `gUnk_0201930C`, `gUnk_020192E4`, `gUnk_020192E0` and `gUnk_0201CFB0` were removed; accesses were renamed to the canonical fields (`unk0` became `lifePoints`, `unk5` became `fusionCount`, `linkBit` became `linkSkip`, and byte `+0x824/+0x82C` became `gUnk_0201CFB0.player/.cursor`).

Local views kept (canonical field differs, so do not change the header):

- `struct DuelPlayerView` / `gUnk_020192E4_view` (`asm("gUnk_020192E4")`): the canonical `struct DuelPlayer` has a plain `u8 unk8` at `+0x08`, but `sub_0802D47C` tests bit 4 of that byte; the view keeps the `flag8_4:1` bitfield split. Reading canonical `(unk8 >> 4) & 1` compiles to `lsrs #4; and #1` instead of the ROM's `lsls #27; blt` and shifts the branch targets, so the view is required for a byte match.
- Deliberately not migrated: `struct CardRef`, `struct RefFlags`, `struct DuelHand`/`gUnk_02019968`, `struct ActLists`/`gUnk_02017A40`, `struct TargetRule`, `struct CardInfo`, and the `ZFLAGS`/`ZB`/`ZB2` macros; these are unit-local shapes the shared headers do not define.

The core of the unit is `sub_0802CE38(ref, other, x)`, a **"can this card effect be used" dispatcher**. It looks the card up in the rule table `gUnk_0819A9D4` (24-byte entries, index `sub_08047058(card id)`) and runs the entry's two callbacks. `prepare(ref, other, x)` (+0x0C) must accept first, then `checkZone(ref, pos)` (+0x08) is tried on every (player 0-1, zone 0-10) until one returns 1. The `checkZone` callbacks are the target checks of [[code-0802bad0]]. Around it: a card "kind" helper, wrappers that build a temporary `CardRef` on the stack for a field zone or a hand card, and the small `(ref, other, x)` rule callbacks (`prepare` entries) and prompt steps.

## Functions

| Address | Size | Status | Purpose (hypotheses for the intent) |
|---|---|---|---|
| `0x0802CAE8` | 0x24 | matching | set bit 4 of byte +9 of the player block `0x020192E4 + player * 0xD64`; return 1 |
| `0x0802CB0C` | 0xE0 | matching | 2-step prompt: step 0 formats a name (`sub_080753F4`, table `0x0822C720 + 0x40 * gUnk_08623E66[0]`) and shows it (`sub_080602A4`); step 1: `sub_08052F38(0xE0)`, chosen (player, zone) from `0x0201CFB0+0x824/+0x82C` must hold card number 0x39 face-up, else SE 3 |
| `0x0802CBEC` | 0xB4 | matching | same for a hand card: `0x02019968` (hand words) of the chosen player/index must be type 22 (Magic); `sub_080193D4(player, idx, 0, 1)` |
| `0x0802CCA0` | 0x18 | matching | `sub_08022758(ref->player, 1, 1, 0)`, return 1 |
| `0x0802CCB8` | 0x38 | matching | step 0: `sub_080227CC(ref->player, 2)`, step++; return 0; later return 1 |
| `0x0802CCF0` | 0x38 | matching | same with `sub_08022784(ref->player, 0, 1)` |
| `0x0802CD28` | 0x110 | matching | `(u16 id)` card class 0-3: type 21 (Trap): 3 if stat bits 17-19 == 1 else 2; type 22: 2 if == 5 else 1; otherwise 0, or (kind == 1, see `CardKindOf`) 1/2 by `sub_08007590(number, 0)`. Used to compare two cards (`>=`) | `CardClass` |
| `0x0802CE38` | 0x168 | matching | the dispatcher above. Also: fail if `sub_0800966C(id)`; with `other`, need `CardClass(ref) >= CardClass(other)` and `!= 1`; if `ref->unk2_10 == 0x11` the card number must be one of 15 (0x28A, 0x291, 0x2B0, 0x3F7/8, 0x433/4, 0x43A, 0x44B, 0x49A, 0x4B3/4, 0x522, 0x587, 0x5FE) | `CanUseCardEffect` |
| `0x0802CFA0` | 0x30 | matching | `(player, id, u16 x)`: builds a `CardRef` on the stack and calls `sub_0802CE38(&ref, 0, x)` |
| `0x0802CFD0` | 0x88 | matching | `(player, zone, kind)`: same for a field zone (id from the zone word, `zone`, `unk2_10 = kind`); 0 if the zone is empty |
| `0x0802D058` | 0x88 | **matching** (wave 1, 2026-10-01) | `u16 (player, zone)`: 1 if either action list of `0x02017A40` (list B `+0x280` count `+0x3C0`, list A `+0x000` count `+0x3C4`, 0x14-byte entries whose word at +2 is player bit 0 / zone bits 4-9) has an entry for (player, zone). Plain struct-array indexing through the global; see [the wave 1 section](#action-list-lookup-matched-wave-1-2026-10-01) |
| `0x0802D0E0` | 0x17C | matching | `u16 (ref, player, zone)`: field card check: temp `CardRef` copy (`sub_08075294` = memcpy 0x14), id from the zone; need type > 20, `CardClass(zone card) >= CardClass(ref)`, no pending action (`D058`), not face-up (numbers 0x52C/0x3F9/0x594/0x5FC count as face-down), zone byte +0x91 bit 2 set and bit 3 clear, and for a Trap no card 0x2EF on the field; then `sub_0802CE38(&tmp, ref, 0)` |
| `0x0802D25C` | 0xB0 | matching | `u16 (ref, player, idx)`: same for hand card `idx`: must be type 22 with stat bits 17-19 == 5, `CardClass >=`, `sub_08008C94(player, id)`, then the dispatcher |
| `0x0802D30C` | 0x108 | matching | `(ref, player)`: does `ref` have any usable target: field zones 5-9 via `D0E0`; if `player == duel+0x1B12 bit 1` also hand cards (count at player block +2) via `D25C`, else if `ref` is type 22, a face-up unlocked zone 0-4 holding card 0x5F5 |
| `0x0802D414` | 0x68 | matching | rule callback `(ref, ?, x)`: `x == 0`, own zone's byte +7 bit 5, and the opponent has a card in zones 0-4 |
| `0x0802D47C` | 0x2C | matching | callback: `x != 0` and bit 4 of player block byte +8 clear |
| `0x0802D4A8` | 0x94 | matching | `sub_08047170 && sub_08008A1C` for the player, cards 0x3D / 0x4E1 on the field, then `sub_08044224(player, card number, 0) > 0` |
| `0x0802D53C` | 0x44 | matching | any card in any zone 0-4 of either player |
| `0x0802D580` | 0x40 | matching | any card in the opponent's zones 0-4 |
| `0x0802D5C0` | 0x60 | matching | opponent has a face-down (byte 6 bit 1 clear) card in zones 0-4 |
| `0x0802D620` | 0x5C | matching | player's word at player block +0 > 999 (life points?) and `sub_08044224(...) > 0` |
| `0x0802D67C` | 0x58 | matching | `x == 0`, `unk2_10 == 2` and at least two (player, zone in 0-4) accepted by `sub_0802B9EC` |
| `0x0802D6D4` | 0xB8 | matching | card number 0x1A3 (LP >= 5000, fusion-count byte +5 non-zero, `sub_08047170` and `sub_08008A1C` succeed) or 0x1F9 (LP >= 3000, fusion-count byte +5 non-zero) |
| `0x0802D78C` | 0x40 | matching | `x != 0` and `sub_08044224(...) > 0` |
| `0x0802D7CC` | 0x34 | matching | `x == 0`: bit 5 of the own zone's byte +7 |
| `0x0802D800` | 0x2C | matching | `sub_08008B70(0,0,1,1) + sub_08008B70(1,0,1,1) > 1` |
| `0x0802D82C` | 0x70 | matching | `x == 0`; if `unk2_10 == 0x10` and the card at `ref->unk6` (player | zone << 8) is present, bit 0 of its byte 6 clear, and of the other player: 1; else `sub_080088A4(player, 1, 0) > 0` |
| `0x0802D89C` | 0xBC | matching | no card 0x58A on the field; own zone 0-4 holds a monster (type <= 20, number <= 0x76B) whose `sub_0800ABC8` info has value <= 1000 and attribute 2 |
| `0x0802D958` | 0x24 | matching | `sub_08008B70(1 - player, 0, 0, 1) > 0` |
| `0x0802D97C` | 0x158 | matching | `x == 0`, `other` of the other player; number set A (0x53, 0xDF, 0x3EC, 0x437, 0x46E/F): the card at `other->unkC` is a Trap and not `ref`'s zone; set B (0x29F, 0x425/6, 0x42B): `ref`'s player has a Trap in spell zones 5-9 other than `ref`'s |
| `0x0802DAD4` | 0x5C | matching | `x == 0`, `sub_0800A430 == 0xFFFF`, `sub_08008C6C != -1`: bit 5 of the zone's byte +7 |

## Data

- `struct TargetRule` `0x0819A9D4[]`, 0x18 bytes: `+8 checkZone`, `+0xC prepare` (function pointers; either may be NULL).
- `struct CardRef` (0x14): also `+6` u16 (player | zone << 8 of a second card) and `+0xC` u16 (same), see [[code-0802bad0]].
- Player block `0x020192E4 + p * 0xD64`: `+0` u16 (LP?), `+2` u8 hand count, `+5` u8, `+8` bit 4, `+9` bit 4 (set by `0x0802CAE8`), zones from `+0x28`, hand cards at `+0x684` (`0x02019968`). `0x020192E0` is the same block set seen from +4 (`gUnk_020192E0.players[]`), `+0x1B12` bit 1 compared with the player number.
- `0x02017A40`: action lists (see `0x0802D058`) and the prompt step byte `+0x3E4`.

## Matching tricks

- **Inlined function with several returns** (`CardKindOf`): old_agbcc copies the result out of the "return register" (`adds r2,r0,#0`) exactly as in the ROM, so a `static inline` helper with `return`s reproduces `0x0802CD28` (the plain local `v = ...` does not).
- `switch (t2) { case 21: case 22: w = ...; default: w = 0; }` gives the signed `bgt`/`blt` pair; `if (t <= 22 && t >= 21)` merges into an unsigned range test.
- **Early returns give the ROM's block order** when it emits the `return 1` block before the loop: `if (x != 0) return 0; if (!flag) return 0; for (...) { if (found) return 1; }` (`0x0802D414`, `0x0802D67C`).
- `for (...; i++)` with a pointer `p += 0x14` step: write `p += 0x14, i++` in that order. This was only a partial improvement for `0x0802D058`, whose match (wave 1) uses no hand-made pointer at all; see below.
- `(1 - ref->player) & 1` matched with `ZB2` (p term first) / literal 1 and `ref->player` used directly in the loop (its shift `lsl #31` is hoisted, the `lsr` stays).
- `u16 id = CARD_ID(...)` (instead of `int`) makes agbcc hoist the `0x7FF` mask constant out of the loop (`0x0802D97C`, `0x0802D30C`).
- `gUnk_0819A9D4[idx].checkZone` read directly into locals (not through a `rule` pointer) gives the ROM's `(base + 8) + idx * 24` address form.
- Reading a byte flag at 0x91 twice via `((u8 *)z)[0x91]` (no local) avoids agbcc merging `& 4` and `& 8` into one test (`0x0802D0E0`).
- `u8 *base = (u8 *)&gUnk_02017A40; u8 *step = base + 0x3E4;` for the prompt step byte (see [[code-0802bad0]]).
- `sub_0802D058` must be declared `u16` (callers `lsl #16` the result).

## Unsolved and resolved items

- Resolved `0x0802CFD0`: assign `&ref.id` to a local bound to `r5` before computing the id, preserve the initialized player argument with an empty read/write constraint, then mask it separately for the zone address. The player-first `ZB2` source expression emits the ROM's zone-first multiply sequence. These FAKEMATCH choices preserve the original ABI, emit no instructions and introduce no unset scratch values.
- Resolved `0x0802D058` (wave 1, 2026-10-01): register allocation of the base pointer; see below.
- Resolved `0x0802D6D4`: place the default/shared `return 0` before the two case bodies. Each case holds the initialized raw shifted player in `r3` and an initialized mask of one; empty constraints keep the explicit player mask and repeated extractions. A read/write constraint after the fusion-count comparison in the 0x1A3 case prevents hoisting the next extraction before that comparison. The initialized compiler hints are marked FAKEMATCH and emit no instructions. `sub_0802D058` was then the only remaining assembly function (matched in wave 1).

### Return declaration reconciliation (2026-10-01)

`sub_0802CFA0` now returns int and explicitly casts its `sub_0802CE38` result to u16. This agrees with its existing callers' int declarations while preserving the ROM's final LSL/LSR 16 and full zero-extended r0 value. Every unit byte remains exact; no count change. The new caller in [[code-08054e7c]] uses the same verified word return. Evidence: `build/bigguns-lead2/sub_0802CFA0/solo-word-return-clean/` and full-ROM checkpoint 29.

## 2026-10-01 card-scan interface reconciliation

`sub_0802CE38` now returns `int`; all its returns are full-register 0 or 1. Its existing argument types and predicate behavior are unchanged, and the word consumers in [[code-0805a30c]] agree with this declaration. The complete unit bytes are unchanged. These declaration repairs add no coverage; per-unit artifacts are under `build/bigguns-lead2/` and the full ROM passes at `build/lead-pass27/`.

## Action-list lookup matched (wave 1, 2026-10-01)

`sub_0802D058` (0x88, start score 32) matches in ordinary C on the first try, with no FAKEMATCH. Working notes: `build/wf/sub_0802D058/NOTES.md`.

- The old draft walked a hand-made `u8 *p` from a `base` local (`p + 0x282`, `base + 2`). agbcc then kept the base in r6 and the walking copy in r1; the ROM loads the base into r1 and copies it to r7.
- What matched: plain struct-array indexing through the global, with no locals besides `i`:

      for (i = 0; i < gUnk_02017A40.countB; i++)
          if (gUnk_02017A40.listB[i].player == player && gUnk_02017A40.listB[i].zone == zone)
              return 1;

  and the same loop over `listA` / `countA`. Loop strength reduction creates the pointer giv itself (base + i*0x14, with 0x282 in ip, then base+2), and the allocation comes out as in the ROM.
- Lesson: when the ROM's walking pointer looks compiler-made, try the indexed form before hand-writing the pointer.
