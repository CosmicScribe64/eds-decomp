---
title: Unit duel_zones (field zones, graveyard, banished list)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit duel_zones

`0x08008A1C`–`0x08009A67`, Thumb, `old_agbcc -O2`. Source: `src/duel_zones.c`.
Duel field helpers: queries over a player's 11 field zones (0–4 monster, 5–9 magic/trap, 10 field), zone link lists, and moving zone cards to the graveyard or banished list. Related: [[duel-card-lists-c]] (deck lists, zone placement), [[card-detail-c]].

Unit status: `unit bytes MATCH`, **34/34 functions in C** after workflow waves 2-3 (2026-10-02: `0x08008D3C`, `0x08009424` in wave 2, `0x0800935C`, `0x08009538`, `0x080097F0`, `0x080098C0` in wave 3); none stay `INCLUDE_ASM` (verified with `tools/check.py duel_zones`). Before wave 2: 28/34. All six match in ordinary C.

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x08008A1C` | 0x28 | matching | Count monster zones for which `IsMonsterZoneFree` is true | |
| `0x08008A44` | 0x28 | matching | First such monster zone, or -1 | |
| `0x08008A6C` | 0x8C | matching | Zone holds a non-token monster and card 1418 is on neither field | |
| `0x08008AF8` | 0x78 | matching | Count monster zones other than `exclude` passing `IsTributableMonster` | |
| `0x08008B70` | 0xB4 | matching | Count occupied M/T zones (optionally + field zone), all / face-up / face-down | |
| `0x08008C24` | 0x48 | matching | Zone empty and not locked (player +0x08 bits 14–23) | |
| `0x08008C6C` | 0x28 | matching | First free magic/trap zone, or -1 | |
| `0x08008C94` | 0x68 | matching | Room to play a card: always for a Field magic, else a free M/T zone | |
| `0x08008CFC` | 0x40 | matching | Zone card → banished (flag) or graveyard, clear zone | |
| `0x08008D3C` | 0x108 | **matching** (wave 2, 2026-10-01) | Drop every link (kinds 1,2,5,7,10) whose target is the given zone | `DropLinksToZone` (hyp.) |
| `0x08008E44` | 0x3C | matching | Zone card → graveyard, drop links, clear zone | `SendZoneToGrave` |
| `0x08008E80` | 0x34 | matching | Zone card → banished, clear zone | `BanishZoneCard` |
| `0x08008EB4` | 0x60 | matching | Zone card → fusion deck if `IsFusionMonster`, else hand; drop links; clear | `ReturnZoneToHand` |
| `0x08008F14` | 0x60 | matching | Same, but to the top of the deck | `ReturnZoneToDeck` |
| `0x08008F74` | 0x68 | matching | Any face-up monster whose number passes `IsToonMonster` | |
| `0x08008FDC` | 0x5C | matching | False if a face-up monster has `GetZoneCardAttribute` state 1, 2 or 6 | |
| `0x08009038` | 0x90 | matching | Count face-up cards (all zones, +0x91 bit 3 clear) with a card number | |
| `0x080090C8` | 0x18 | matching | `CountActiveCardsOnFieldIn(gDuelPlayers, player, no)` | |
| `0x080090E0` | 0x70 | matching | Count face-up M/T zone cards of a card type | |
| `0x08009150` | 0x64 | matching C | Count graveyard cards of a card type | |
| `0x080091B4` | 0x3C | matching | Count occupied M/T zones | |
| `0x080091F0` | 0x90 | matching | Count face-down cards (+0x91 bits 2–3 == 1) with a card number | |
| `0x08009280` | 0x18 | matching | `CountActivatableSetCardsIn(gDuelPlayers, player, no)` | |
| `0x08009298` | 0xC4 | matching C | Count other face-up monsters (both sides, opponent first) that are the same card (`IsSameCardName`) as a zone's face-up card | |
| `0x0800935C` | 0xC8 | **matching** (wave 3, 2026-10-01) | Add a link `(zone << 8) \| player` → target with a kind; unless kind 10, an existing link just gets its count (high byte of `linkInfo`) incremented | `AddZoneLink` |
| `0x08009424` | 0xC0 | **matching** (wave 2, 2026-10-01) | Remove one link of a zone: first link whose kind is `kind` and (unless kind 4-7/9-12) whose target matches | `RemoveZoneLink` (hyp.) |
| `0x080094E4` | 0x54 | matching | Card number of the face-up Field card (zone 10) of either player, or 0 | `GetActiveFieldCard` |
| `0x08009538` | 0x134 | **matching** (wave 3, 2026-10-01) | If the given zone holds a card: first occupied face-up monster zone holding a link (kinds 1,2,5,7,10) to it; its loc `(zone<<8)\|player`, else 0xFFFF | `FindLinkSource` (hyp.) |
| `0x0800966C` | 0x88 | matching | Marked-zone list (`gDuel` +0x1ACC..): is card `id` (`IsSameCardName`) still on its recorded zone | |
| `0x080096F4` | 0x74 | matching | Put a non-token card into its owner's graveyard | `AddToGrave` |
| `0x08009768` | 0x88 | matching | Put a non-token card into its owner's banished list (kind 0) | `AddToBanished` |
| `0x080097F0` | 0xD0 | **matching** (wave 3, 2026-10-01) | Temporarily banish a monster: banished entry kind 1 with its zone, set the zone's bit in `removedMask` | |
| `0x080098C0` | 0x128 | **matching** (wave 3, 2026-10-01) | Return a kind-1 temporarily-banished monster of a zone to that zone, compact `banished[]`, clear the zone's bit in `removedMask` | `ReturnBanishedToZone` (hyp.) |
| `0x080099E8` | 0x80 | matching | Banished list, kind 2 | |

## Data layout (this unit's view)

- Player (0xD64, `0x020192E4`): +0x04 graveyard count, +0x06 banished count, +0x08 u32 bitfields (bits 14–23 per-zone lock, bits 28–32 "removed" mask for monster zones, straddling into +0x0C), +0x28 zones, +0x904 graveyard (u32 card words), +0xB84 banished list, +0xCC4 u16 banished info (low byte kind, high byte zone).
> [!warning] Contradiction
> [[duel-card-lists-c]] calls player +0x004 "count of list904" and +0xA44 "fusion deck" (from `0x08007D18`/`0x0800817C`, which use +0x005/+0xA44 as the save's second deck). This unit's struct puts `grave[160]` at +0x904 (160 entries would overlap +0xA44). Both agree on +0x904 = graveyard with count at +0x004. The array length is unresolved (80 is more likely, so the graveyard would end at +0xA44).

- `gDuel` (`0x020192E0`) after the two players: +0x1ACC u32 bitfield with a 4-bit count at bits 15–18, +0x1AD0 u16 `(zone << 8) | player`[16], +0x1AF0 u16 card IDs[16] (a "marked zones" list, name is a hypothesis).

## Matching tricks

- **`u16 id` locals** for card IDs read from zones (`u16 id = ZONE_CARD(p, i).id;`) make GCC hoist the `0x7FF` mask instead of the table address (or copy it through a spare register), as the ROM does. This one change fixed `0x08008F74`, `0x08009038`, `0x080090E0`, `0x080091F0` and `0x080094E4`.
- Card tables are indexed through integer-constant pointers (`((const u16 *)0x08622AB4)[id & 0x7FF]`).
- Zone links (`(zone << 8) | player`) are read in two parts: the player with `ldrb` on the low byte and the zone with `ldrh; lsr #8`. Load both into locals before building the address (`0x0800966C`).
- `int end` (not `u16`) for a loop bound that is compared with a `u16` counter gives the ROM's signed `bge`/`blt` (`0x08008B70`).
- **Zone address shape (resolved for `0x08009298`):** assign an `int pp = player & 1` before forming `(u32)gDuelPlayers[0].zones + (zone * 0x94 + pp * 0xD64)`. Repeat with a distinct initialized `int pp2 = p & 1` inside the second scan before `i * 0x94 + pp2 * 0xD64`. Both stages matter: changing only the first restores the target size but leaves register differences. This ordinary C shape needs no hints. The old claim that a precomputed masked-player local could not reproduce the order is superseded by the complete two-scan match; `0x0800935C` remains unresolved (historical: it matched in wave 3, see below).
> [!warning] Contradiction
> The next bullet (2026-09-30/10-01) presents `ZB(p,z)` = `z*0x94 + p*0xD64 + base` as the address form that matches "the link functions below". The wave 2/3 matches of `RemoveLinksToZone`, `RemoveZoneLink` and `FindMonsterWithLinkTo` (2026-10-01, `build/wf/RemoveLinksToZone/NOTES.md`, `build/wf/RemoveZoneLink/NOTES.md`, `build/wf/FindMonsterWithLinkTo/NOTES.md`) found that `ZB` makes agbcc emit the player term first, while these ROM functions emit `zone * 0x94` first; they need the reversed operand order `ZB_PZ(p,z)` = `p*0xD64 + z*0x94 + base`, still read per access. Resolved in favour of the matched source: the per-access reading is right, the operand order is `ZB_PZ` for these three.

- **`#define ZB(p,z) ((struct DuelZone *)((z)*0x94 + (p)*0xD64 + (u32)gDuelZones))`** (declared locally; `extern u8 gDuelZones[]`) is the address form that already-matched link walkers elsewhere use (`duel_piles.c`). Reading each field where it is used, with no shared pointer local, matches the ROM's per-access recomputation for the link functions below.
- Historical (`0x08009424` matched in wave 2 with `int player` and a duplicated call, see below): `0x08009424` was tried with: inline macro, `z` pointer local (in and out of the loop), `link`/`kind` locals, a masked-player local, `PLAYER(...).zones[z]` indexing, and a boolean `kind`-range test. The switch decision tree (`cmp #4/blt`, `cmp #7/ble`, `cmp #12/bgt`, `cmp #9/bge`) only comes out of a `switch (kind)` over cases 4-7 and 9-12. What still differs is pure register allocation (ROM: target in `sl`, player in `r8`; GCC swaps them); the advisor confirmed the shape is right.
- `0x08008D3C`'s jump table maps `kind - 1` in {0,1,4,6,9} (i.e. kinds 1,2,5,7,10) to the compare; the default case does **two** `i++` (skips a link). Reproduce it verbatim. `0x08009538` uses the same kind set and returns `(zone << 8) | player`.
- `0x080098C0` is a bitfield round-trip: `pl->removedMask &= ~(1 << zone)` on the 5-bit straddling field generates the byte +0x0B/+0x0C extract/clear/insert. The compaction loop shifts only `banished[]` card words, not `banishedInfo[]`.

## Graveyard type count match (2026-09-30)

`CountGraveyardCardsOfType` now matches all `0x64` bytes as ordinary C. Reusing [[duel-piles-c]]'s inline card-type helper with a `u16` ID argument preserves the extraction/narrowing boundary and reloads the stats-table literal inside the loop, while keeping the `0x7FF` mask hoisted. The loop declares its index before its zero-initialized count, following the matched sibling. `GetGraveCardType` needs no register pins or empty constraints. The whole `duel_zones` unit matches all `0x104C` bytes, with 27/34 functions in C and seven assembly fallbacks.

Prior direct-macro, explicit ID-local and table-pointer constraints stayed nonmatching. A private pinned-pointer candidate omitted the original `r7` save/restore and was rejected despite a two-byte isolated score. The inline-helper match resolves this earlier table-hoisting diagnosis without that ABI problem.

## Same-monster scan match (2026-09-30)

`CountOtherFaceUpSameNameMonsters` now matches all `0xC4` bytes as ordinary C. Separating the masked-player local before each explicit zone-first stride sum fixes both address scheduling and register allocation. Removed unused experiment locals still match, and the restored `CARD` macros preserve the same preprocessed code. The whole unit matches all `0x104C` bytes with 28/34 C bodies and six assembly fallbacks; log: `build/codex-continue/same-monster-unit-check.log`. The first grid using a new `gDuelZones` extern did not link in its isolated context and is not evidence of a code-generation failure. Using the existing player-array member gives the same actual zone base, `0x0201930C`.

Historical (`AddCardToBanishedTemporarily` matched in wave 3, see below): the separate `AddCardToBanishedTemporarily` halfword-mask/staged-address grid remains partial: its best private candidate is `0xC8` versus the ROM's `0xD0`. Do not enable it or repeat that unchanged family.

### Adjacent-helper follow-up

Historical: all three helpers below matched in workflow waves 2-3 (2026-10-01); see [Wave 2 matches](#wave-2-matches-2026-10-01) and [Wave 3 matches](#wave-3-matches-2026-10-0102).

- `AddZoneLink`: 43 fresh masked-player/address/low-byte variants improve the private baseline from `0xB4` to `0xC0` versus target `0xC8`, but remain nonmatching. A raw `*(u8 *)&linkInfo[i]` read preserves the ROM's separate low-byte load; source address staging alone does not restore all duplicated address arithmetic. Results: `build/codex-continue/add-link-grid/`.
- `RemoveZoneLink`: the current private baseline is `0xB8` versus target `0xC0`, so the earlier "only registers differ" diagnosis does not describe this source/context. Three finite grids tested initialized target/player copies, per-access recomputation and sibling byte-pointer association. The exact-size variants still retain an unwanted link-base precomputation and different base lifetimes. No C accepted; results: `remove-link-grid/`, `remove-link-recompute/`, `remove-link-association/` under `build/codex-continue/`.
- `AddCardToBanishedTemporarily`: the narrow-field/initialized-one grids reach target size `0xD0`, with 58 differing bytes. A private `u16 removedMask` view and initialized r9 `one` produce the missing halfword narrowing, but the implicit field masks, first constant scheduling and saved-register roles still differ. This is a code-generation observation, not a verified layout change; the active `u32` view and assembly fallback remain intact. Results: `build/codex-continue/banish-field-view/` and `banish-one-width/`.

## Word-return declaration reconciliation (2026-10-01)

`CountSpellTrapsFiltered` now declares its verified zero-extended result as `int`, agreeing with the full-register consumers in [[ai-strategy-c]]. Explicit halfword casts preserve nonconstant results. Its complete unit and the full ROM remain byte-exact; this adds no coverage by itself.

`GetFaceUpFieldMagicNumber` also now declares its verified zero-extended field-card number as a word result, retaining `(u16)CARD_NUMBER(id)` and exact unit bytes. The complete Strategy 3 caller in [[ai-strategy-c]] consumes that word directly. Both units were verified.

## 2026-10-01 card-scan interface reconciliation

`CountActivatableSetCards(int player, int numberWord)` explicitly decodes `u16 number = numberWord` before calling its existing search. This agrees with the signed 16-bit table load passed as a word by [[ai-turn-steps-c]]. The complete unit bytes are unchanged. These declaration repairs add no coverage; per-unit artifacts are under `build/bigguns-lead2/` and the full ROM passes at `build/lead-pass27/`.

## Wave 2 matches (2026-10-01)

Both match in ordinary C. Working notes: `build/wf/RemoveLinksToZone/NOTES.md`, `build/wf/RemoveZoneLink/NOTES.md`.

### `RemoveLinksToZone` (0x108, start score 92; ordinary C)

- What differed: register allocation (p/z/base swapped) and the address arithmetic order. The parked draft used `ZB()`, which emits `(p & 1) * 0xD64` first; the ROM emits `z * 0x94` first.
- Two edits, straight to score 0: the reversed operand order `ZB_PZ(p, z)` = `(p) * 0xD64 + (z) * 0x94 + base` (the macro is defined further down for `FindMonsterWithLinkTo`, so it is re-defined identically above this function), and the packed target compare `link == (u16)((u8)player | ((u8)zone << 8))`, which gives the ROM's `lsl #24; lsr #8; orr; lsr #16`. The register swap came from the operand order alone; no pins.

### `RemoveZoneLink` (0xC0, start score 101; ordinary C)

- The draft cached a `struct DuelZone *z` with ZB's player-first order; agbcc CSE'd the `links[i]` address and the function came out 8 bytes short. Per-access `ZB_PZ` gave the right size (score 85), but loop.c's second pass hoisted the default case's `links[i]` address (`0xD64` constant, mult, adds) out of the loop, where the ROM recomputes it in place.
- Mechanism, from the `-dL` loop dump: a movable is hoisted when `threshold * savings * life >= insn_count`, with threshold 13 in a loop with a call, dropping by 3 after each move. Writing the call twice (`case 4..12: call; return;` and `default: if (links[i] == target) { call; return; }`) adds loop insns; cross-jumping merges the two calls only later, so pass 2 counts 54 insns, above the `13*2*2 = 52` limit, and the default-case constant stays in the loop.
- `int player = (u8)loc` instead of `u8 player`: `player & 1` loses its QImode zero-extension insns (2 fewer movables), so pass 1 moves `zone*0x94` at threshold 7 instead of 1. That gives the ROM's pre-loop copy order (`p&1` copy, zone94 copy, then the `i*2` giv init) and fixed all register differences.
- Failed: 1 to 3 empty `asm("")` before the call (right structure, wrong registers, score 70, the same as the duplicated call with `u8 player`), a goto to a shared call (32 bytes short), a found flag (16 bytes long).

## Wave 3 matches (2026-10-01/02)

All four match in ordinary C. Working notes: `build/wf/AddZoneLink/NOTES.md`, `build/wf/FindMonsterWithLinkTo/NOTES.md`, `build/wf/AddCardToBanishedTemporarily/NOTES.md`, `build/wf/ReturnTemporarilyBanishedCard/NOTES.md`.

### `AddZoneLink` (0xC8, start score 124; ordinary C)

1. Loop preheader: the ROM strength-reduces `links[i]` from the zone pointer (`z + 0xA`) but `linkInfo[i]` from a separately derived address `((pp*0xD64 + 0x4A) + zn*0x94) + base`. That comes from writing the `linkInfo` access inside the loop as a fresh `(u32)gDuelPlayers[0].zones + zn * 0x94 + pp * 0xD64` (left-associated, base first); loop.c expands the movables in the giv and reverses the leaf order. Array indexing, `ZB`, `ZONE_PTR` or base-last give other orders, or CSE the whole sum with `z`.
2. The final block uses its own pointer `z2` (not `z` again), so `z` has only 3 refs.
3. `z` and `pp*0xD64` then tied in global-alloc priority (3 refs, live length 13) and the tie went to `z` (lower pseudo number). Fix: `zoneOfs = zn * 0x94` and `playerOfs = pp * 0xD64` as user variables declared before `z`, so `pp*0xD64` gets the lower pseudo number and r4.
4. From the older draft: the low byte re-read as `*(u8 *)info` (`ldrb`), and `pp = p & 1` recomputed before the final address.
- Failed: the `ZONEP`/`ZB` macros everywhere (score 124-263), a single `z` for both blocks (58).

### `FindMonsterWithLinkTo` (0x134, start score 142; ordinary C)

1. `ZB_PZ` for every access, the entry check included (zone term emitted first, as in `RemoveLinksToZone`).
2. Compare value `(u16)((u8)player | (u8)zone << 8)`: the u16 cast gives the ROM's `lsl 24; lsr 8; orr zone<<24; lsr 16`.
3. The jump table maps kinds 1/5/10 and 2/7 to two different labels, so the source has two case groups with identical bodies; cross-jumping turns the first into a 2-insn stub.
4. No early return: `if (id != 0) { loops } return 0xFFFF;`. With `if (id == 0) return 0xFFFF;` the barrier before the loop lets loop.c (`find_and_verify_loops`) move the last case's return block out of the loop to that barrier; cross-jumping then goes the other way and `p` and `0xD64` swap r6/r7.

### `AddCardToBanishedTemporarily` (0xD0, start score 134; ordinary C)

- What differed: the ROM keeps the constant 1 in r9 from the prologue on, and narrows the new `removedMask` value to 16 bits (`lsl/lsr #16`) before splitting it over bytes `+0x0B`/`+0x0C`.
- `u32 owner = card->owner & 1;`: the `& 1` emits an SImode constant 1 before the call. combine later deletes the redundant AND (the bit-extract is already 0/1), but CSE had already reused that register for the later bitfield `& 1` and `1 << zone` constants, so it lives across the call in r9 (score 134 -> 50). The `banishedInfo` `| 1` stays a fresh `movs` because it is HImode (CSE tracks constants per mode).
- `removedMask = (u16)(removedMask | 1 << zone);` gives the halfword narrowing (50 -> 0). The earlier private `u16 removedMask` view and initialized `one` grids (follow-up above) were not needed.

### `ReturnTemporarilyBanishedCard` (0x128, start score 185; ordinary C)

- The parked draft (`u16 p` local, `s8 info`, `i` reused in the compaction loop) was structurally off. `PLAYER(player)` macros everywhere (player spilled, `player & 1` recomputed after the call) fixed the structure (score 132), but reusing `i` for the compaction loop let the second loop.c pass (rerun-loop-opt) strength-reduce `i*4` in the outer loop (+8 bytes). A separate `j` fixed the size (109), leaving only global-alloc order: `i*4` (pseudo 70) got r4 ahead of `p*0xD64`, so the banished base went to r9 and `zone` was spilled instead of kept in sl.
- What matched: `u32 info` instead of `u16 info`. The u16 zero-extension insn disappears from the flow RTL, so `p*0xD64`'s live length drops from 27 to 25; its priority then ties `i*4`'s (1.200) and wins on the lower pseudo number: `p*0xD64` r4, low byte r5, `i*4` r7, banished base r2, `zone` sl. `int info` with `(info & 0xFF) == 1` scored 2.
- Failed: a `register int off asm("r7") = i * 4` pin (old_agbcc then hands r7 to another pseudo too and miscompiles; do not pin compiler temporaries this way); removing the `info` temporary, `-= 1`, break vs return, declaration order, a temporary destination pointer (all 109).
