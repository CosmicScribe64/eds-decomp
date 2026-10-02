---
title: Unit code_0802AAC0 (duel card-list viewer, field target checks)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit code_0802AAC0

`0x0802AAC0`–`0x0802BACF`, Thumb, `old_agbcc -O2`. Source: `src/code_0802AAC0.c`.

Unit status: `unit bytes MATCH`, **14/14 functions in C** after workflow wave 2 (2026-10-01: `0x0802B2FC` in wave 2); none stay `INCLUDE_ASM`. Before wave 2: 13/14 (after enabling `sub_0802B558` on 2026-09-30). Verified with `tools/check.py code_0802AAC0` (complete 0x1010-byte unit).

## Shared headers

The unit includes `include/main.h` and `include/duel.h` and uses their canonical layouts instead of its own
copies. Five local struct definitions and three local `extern` declarations were removed:

| Removed local definition | Replaced by | Notes |
|---|---|---|
| `struct Main` + `extern struct Main gUnk_03000040` | `main.h` | field `bgScroll4422` (+0x4422) renamed to `bgVofs[1]`; `newKeys` keeps its name |
| `struct DuelCard` | `duel.h` | the unit's split (`id:12`, `unk12:20`) is never read as a bitfield, only as a whole `u32` via `CARD_WORD`/`CARD_ID` |
| `struct DuelZone` | `duel.h` | fields used (`card`, `counter6`, `links`, `linkKinds`, `numLinks`) all match; the unit's `unk4`/`unk5` (`u8`s) are the header's `serial` (`u16`) but are never read |
| `struct DuelZonesPlayer` + `extern ... gUnk_0201930C[2]` | `duel.h` | only `zones[]` is used |
| `struct DuelPlayer` + `extern struct DuelPlayer gUnk_020192E4[2]` | `duel.h` | `count904` (+0x4) → `graveCount`; `list904` (+0x904) → `graveyard`; other fields already had canonical names |

Kept local (not in either header): `struct CardRef`, `struct ListView` (`gUnk_0201D810`) and the `gListView` /
`gMain` macros.

No local field views are kept. Every function still matches with the canonical declared types and bitfield
splits. The unit never reads a shared field whose canonical type or bitfield layout differs from its old copy,
and the two differences noted above (`DuelCard`'s upper bits and `DuelZone`'s `+0x4`) are never accessed.

The unit has two halves:
- `0x0802AAC0`–`0x0802AF34`: the duel **card-list viewer** at `0x0201D810` (`struct ListView`). It shows one of a player's card lists (graveyard, fusion deck, deck, list `+0xB84`) as a scrolling list with up to four buttons. The helpers it calls (`sub_0802A45C`, `sub_0802A47C`, `sub_0802A4A4`, `sub_0802A4CC`, `sub_0802A658`, `sub_0802A6DC`) are in [[code-08029750]].
- `0x0802B1B8`–`0x0802BA68`: **target checks** `(struct CardRef *ref, u16 pos)` with `pos` = player (low byte) | zone << 8. They answer "can card `ref` select / affect the card in (player, zone)?". `sub_0802B558` (big, called from [[code-08009a68]] `sub_0800A480`) is presumably the dispatcher over them (hypothesis).

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x0802AAC0` | 0x4C | matching | Viewer close step: wait for `sub_08075A6C(4)`, clear `drawList`; then `sub_080609C4`, `sub_0805F96C`; then return `sub_08060B2C()` | `ListView_CloseStep` |
| `0x0802AB0C` | 0x120 | matching | Per-frame update: clear VRAM `0x06004200` once, scroll animation (`gMain+0x4422` = `gUnk_0819A788[dir][timer].y − row·16`), row ± 1 at the end, redraw (`sub_0802A45C/4A4`), buttons (`sub_0802A658`), card preview of the selected entry (`sub_0802A4CC`) | `ListView_Update` |
| `0x0802AC2C` | 0x2AC | matching | Input step. States 1–4: fade, open Card Detail (`sub_0800688C(id, 0, 0)`), wait `sub_08006D08`, wait `sub_0802A6DC`. Otherwise Up/Down move the cursor or scroll (SE 0 / SE 3 at the ends), Right/Left cycle through the enabled buttons, B closes (if button 1 is enabled, SE 2), A runs button 0–3 (0 = detail view, refused with SE 3 in mode 3 when the entry's `arrCC4` kind is 2 and `player` is 1; 1 = close; 3 = return 1) | `ListView_Input` |
| `0x0802AED8` | 0x5C | matching | Step runner over `gUnk_0819A7B8[step]` (calls `sub_0802AB0C` first); clears `active` at the NULL terminator. Returns 1 while active | `ListView_Run` |
| `0x0802AF34` | 0x284 | matching | Open: `(player, area, a2, a3)`. Copies the player's list for area 14 (`+0x904`), 12 (`+0xA44`), 15 (`+0xB84`), 13 (`+0x7C4`, deck) into `cards[]`; area −1 calls `sub_08044224(player, a2, a3)` instead. Sets `mode`/`buttonMask`, resets the cursor, `active = 1` | `ListView_Open` |
| `0x0802B1B8` | 0xD4 | matching | `(u16 id, player, zone)`: 0 if the zone is empty, 1 if face-down; face-up: refused when `sub_0800C8BC == 1` and a card number 0x2E4 is on the field (`sub_080086CC`), or when the zone holds card 0x52E/0x531, `sub_080094E4() == 0x14D` and `id` is a Magic (type 22) of subtype ≠ 3 | `CanTargetZone` (hypothesis) |
| `0x0802B28C` | 0x70 | matching | `(player, zone)`: occupied, and not (card 0x52E/0x531 with `sub_080094E4() == 0x14D` and face-up) | |
| `0x0802B2FC` | 0x190 | **matching** (wave 2, 2026-10-01) | Card-specific: `ref` number 0x37/0x38/0x42 (limit 1/3/5, needs zone card 0x115 and link card 0x47) or 0x170 (limit −1, 0x16D / 0x28B); no card 0x58A on the field; true when a kind-1 link of the zone holds `wantNo` with counter > limit | |
| `0x0802B48C` | 0x74 | matching | Occupied spell/field zone (5–10); if face-up, returns `type == 21` (Trap) | |
| `0x0802B500` | 0x58 | matching | Opponent's occupied monster zone (0–4) that `sub_0802B1B8` allows | |
| `0x0802B558` | 0x434 | matching | Main check for a face-up, allowed monster target. `switch` on `ref`'s card number (about 60 numbers): return 1; compare `a = sub_0800C8BC(target)` (0x130/0x131 → 10, 0x144/0x3C2/0x522 → 7, 0x416/0x417 → ≠ 7, …) or `b = sub_0800CAF0(target)` (0x132/0x29B → 1, 0x28D/0x3F4 → 4, …) with a constant; 0x28A/0x422 own monster only; 0x47 needs zone card 0x115 and `sub_0800A78C(player, zone, 0x47) == 0`; 0x13C/0x28B/0x604 need specific zone cards. Matching ordinary C: staged first-zone pointer, explicit failure returns, shared true label inside case 0x604, and initialized case-0x47 result preserve the original address operands and all branch destinations | `CanCardTargetMonster` (hypothesis) |
| `0x0802B98C` | 0x60 | matching | Opponent's monster zone, occupied and allowed: returns zone byte 6 bit 0 | |
| `0x0802B9EC` | 0x7C | matching | Face-up occupied zone with `sub_0800C894 ≤ 1000` (attack ≤ 1000, hypothesis), allowed, and not `ref`'s own zone | |
| `0x0802BA68` | 0x68 | matching | Occupied zone 5–10 that is not a face-up Trap (type 21) | |

## Data

### List viewer `0x0201D810` (`struct ListView`, 0x310 bytes)
- `+0x0` bit 0 `active`, bit 1 `player`, bit 2 clear VRAM, bit 4 `drawList`, bits 5–7 `mode` (2 fusion deck, 3 list `+0xB84`, 4 area −1, 5 deck; area 14 stores the *player* number here, as in the ROM).
- `+0x1` step (index into `gUnk_0819A7B8`), `+0x2` state, `+0x3`, `+0x4`.
- `+0x5` bits 0–1 `row`, bits 2–4 `scrollTimer` (counts from 4 down to 0), bits 5–6 `scrollDir` (1 up, 2 down). `+0x6` u16 `top`.
- `+0x8` bits 0–1 `button`, bits 2–5 `buttonMask` (2 = button 1 only; 8 = button 3 only).
- `+0xC` `cards[]` (card words), `+0x30C` u16 `count`.
- `gUnk_0819A788[dir][4]`: 4-byte scroll steps, `+0` = BG y offset.

### Duel zone (0x94 bytes, `0x0201930C + player*0xD64 + zone*0x94`)
`+0x0` card word, `+0x6` bit 0, bit 1 face-up (hypothesis), bits 2–5 counter, `+0xA` links[32] (player | zone << 8), `+0x4A` link kinds, `+0x8A` numLinks. Same layout as [[code-08009a68]].

## Matching tricks

> [!warning] Contradiction
> The next bullet (page text before 2026-10-01) prescribes `ZB(p, z)` with `int p = player & 1;` as its own statement to get the ROM's order. The wave 2 match of `sub_0802B2FC` (2026-10-01, `build/wf/sub_0802B2FC/NOTES.md`) needs the opposite source order, `ZR(player & 1, zone)` = `p*0xD64 + z*0x94 + base` with the `and` inline, for the first zone pointer, the loop condition and `links[i]`: agbcc then emits the zone multiply first, as the ROM does there. Only the linked-zone pointer uses `ZB(pp, lz)` with a separate `int pp = lp & 1`. Resolved in favour of the matched source: the right macro is per address, not per unit.

- **Zone address.** Write `ZB(p, z) = (z)*0x94 + (p)*0xD64 + (u32)gUnk_0201930C` and compute `int p = player & 1;` as its **own statement** before it. This gives the ROM's order (`and` first, then `zone*0x94`, then `p*0xD64`). With `player & 1` inside the macro, the `and` is scheduled after the first multiply, and the input register choice changes (`sub_0802B1B8`).
- **A named zone pointer even for a single use.** `struct DuelZone *z = ZB(p, zone); if (CARD_ID(CARD_WORD(z->card)))` matched `sub_0802B500`, and also fixed a player/zone register swap earlier in the function.
- `pos` unpacking: `int player = (u8)pos; int zone = pos >> 8;` gives `lsl #8; lsr #24` and signed `bgt` compares on zone. `u8` locals give `bhi`; `pos & 0xFF` gives a `mov #0xFF; lsl #16; and`.
- Bitfields (not masks) for the viewer flags: `v->active = 0` gives `mov #2; neg; and` (the ROM's form); `&= ~1` on a `u8` gives `mov #0xFE`.
- **Flag variable left in a callee-saved register (`sub_0802AF34`).** The ROM keeps a zero in `sl`, uses it for `count = 0`, and tests it only after the area −1 case. That is `int copy = 0;` set to 1 in each list case and `if (copy) { copy loop }` after the switch: old_agbcc threads the known-1 paths straight into the loop but keeps the test on the −1 path.
- `sub_0802AC2C`: `gListView.x` directly (not a `v` pointer) reproduces the ROM's three copies of the struct address. `int i = top + row;` as a separate local inside case 0 fixes the literal-load order of `gUnk_020192E4`. The Right/Left button cycling loops `do { button++; } while (!((buttonMask >> button) & 1));` matched as written.
- `sub_0802B558`: the first zone read first takes `pz = &gUnk_0201930C[player & 1]`, then `first = (struct DuelZone *)pz; first = (struct DuelZone *)((u32)first + zone * 0x94);`. This retains separate reusable player/zone stride products and the ROM's `add r0,r0,r7`; direct `pz->zones[zone]` indexing reverses those add operands. Its binary-search dispatch case sets were recovered with a small scratch interpreter that maps each value to its case label.
- `sub_0802B558`'s return layout is ordinary C with no asm hint. Cases 0x13C/0x28B/0x604 explicitly return zero on failure. The unconditional true case group jumps to a shared label *inside* case 0x604's successful condition. Case 0x47 assigns the helper result to `int result`, tests it, then assigns/returns one; this retains the ROM's short-success trampoline before the shared true block. A broad shared label outside the conditional changed comparison-tail merging; merely adding the inside label left five differing bytes, and the local result reduced that to the two address-add bytes before staged pointer assignment matched. Empty barriers and direct whole-address casts did not settle those bytes. Complete-unit check: `build/middle_experiments/sub_0802B558/unit-check.log`.
- `sub_0802B48C` / `sub_0802BA68`: the ROM extracts the card id into `r0` and copies it to `r2`
  (`lsrs r0,#20; adds r2,r0,#0`), then masks it non-destructively. A FAKEMATCH dead store
  `u16 copy; copy = id;` (as in sibling `sub_0802BF38` in [[code-0802bad0]]) forces the extra copy;
  the id is then used only through `copy` in `CARD_TYPE`.

## Bounded follow-up for the remaining predicate

Historical (`sub_0802B2FC` matched in wave 2, see below).

`sub_0802B2FC` remains parked after the 2026-09-30 post-1589 batch. Separate initialized player/zone barriers remove some first-index CSE; reversing the first and linked-zone expression grouping restores the target's zone-first multiplication. A player barrier after initializing `i=0` reaches the correct 0x190 size (`build/middle_experiments/sub_0802B2FC/correct-size-156.c`), but still differs in 156 bytes: player/zone/counter homes and loop stride/base invariants rotate, the initial table scratch is wrong, and one loop copy is absent. A zone-only barrier reaches 0x198 and adds a spill. Narrow first-player locals and equivalent initial masks do not settle this. No isolated candidate was enabled or counted as C.

The verified `sub_08009298` address shape was also tried here: separately initialized `pp2 = lp & 1`, explicit zone-first offset grouped after the global zone base, and the equivalent `gUnk_020192E4[0].zones` view. Across first/linked/retained-loop pointer combinations, it did not improve the retained correct-size 156-byte near miss. The best new ordinary-C shape was 0x194 with 367 differing bytes; it remains private (`build/middle_experiments/linked_target_staged.py`). This is a recorded failed transfer of the sibling pattern, not evidence against that sibling's exact match.

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/sub_0802B2FC/NOTES.md`.

### `sub_0802B2FC` (0x190, start score 82; ordinary C)

- Types first: the old draft truncated `u8 needNo = 0x115` and read the link with `s8` (`ldrb`); fixing both raised the score to 184, as expected, before the real fixes.
- Address term order: `ZR(p, z)` = `p*0xD64 + z*0x94 + base` makes agbcc emit the zone multiply first, as the ROM does for the first zone pointer, the loop condition and `links[i]`. The linked-zone pointer uses the opposite form, `ZB(pp, lz)` with a separate `int pp = lp & 1`: `and`, `lz*0x94`, then `pp*0xD64`, with the `and` reusing the kind register.
- Last swap (`0xD64` in ip and `zone*0x94` in r7; the ROM has them the other way round), diagnosed from global-alloc priorities: the `0xD64` pseudo is set twice with the same constant and `update_equiv_regs` doubles its REG_LIVE_LENGTH once per set (4 x 44 = 176, priority 24/176 = 0.1364), below the `zone*0x94` invariant (8/56 = 0.1429). The fix is `u16 cid = CARD_ID(CARD_WORD(l->card));` before `CARD_NUMBER(cid)`: its zero-extend insns sit after the last use of `0xD64` in the body, so they lengthen only the `zone*0x94` life (61, priority 0.1311), and `0xD64` is allocated first.
- Failed: `ZR(lp & 1, lz)` for the linked zone (wrong multiply order), `ZB` for `links[i]` (the giv init becomes `(pD + zone94) + (base + 74)`, 22), no `l` local, u8 `lp`/`lz`/`pp`, a `u16 no` temp, nested ifs, `limit < counter6`, int `wantNo`/`needNo`.
