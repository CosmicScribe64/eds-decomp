---
title: Unit code_0802BAD0 (duel field-target checks, part 2)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit code_0802BAD0

`0x0802BAD0`–`0x0802CAE7`, Thumb, `old_agbcc -O2`. Source: `src/code_0802BAD0.c`. Continues [[code-0802aac0]] (`0x0802B1B8`–`0x0802BA68`, same signature and zone helpers).

Unit status: `unit bytes MATCH`, 32/33 functions in C (complete 0x1018 bytes, verified after enabling `sub_0802C3C0` and `sub_0802C080` with `tools/check.py code_0802BAD0`). Only `sub_0802BBDC` remains `INCLUDE_ASM` with its attempt in `#if 0`. Latest log: `build/middle_experiments/sub_0802C080/unit-check.log`.

Nearly every function is a target check `int f(struct CardRef *ref, u16 pos)`, where `pos` is `player | zone << 8` with the player in the low byte. It returns 0 or 1 (often a face-up flag). They are probably the per-card "can `ref` target the card in (player, zone)" predicates dispatched from a table (hypothesis; see [[code-0802aac0]]). The functions from `0x0802C77C` on take only `ref` and are card-effect steps. They queue duel events (`sub_0801EC58`) or ask the player to pick a card, and return 1 when done.

## Shared headers

Since 2026-09-30 this unit uses the canonical shared layouts instead of its own copies:

- `#include "duel.h"` provides `struct DuelCard`, `struct DuelZone`, `struct DuelZonesPlayer`, `struct DuelPlayer` and the externs `gUnk_020192E0`, `gUnk_020192E4`, `gUnk_0201930C`.
- `#include "main.h"` provides `struct Main` and the extern `gUnk_03000040` (read as `.newKeys`, the u16 at `+0x06`).
- Field renames: the player `+0x00` u16 is now `lifePoints`; `gMain.keys` is now `gMain.newKeys`.
- Removed local struct definitions: `struct DuelCard`, `struct DuelZone`, `struct DuelZonesPlayer`, `struct DuelPlayerHead` (replaced by `struct DuelPlayer`), `struct MainHead` (replaced by `struct Main`). Five in total.
- No local views are kept. The canonical bitfield splits (for example `DuelCard.id:12` versus the old `id:12`/`unk12:20`) and types produce identical code. The card word is only ever read as a whole `u32` (`CARD_WORD`), and the zone flags are read through the raw-byte `ZFLAGS` macro. `struct CardRef` and `struct CardInfo` are unit-local and stay here.

## Functions

| Address | Size | Status | Purpose (target checks unless noted) |
|---|---|---|---|
| `0x0802BAD0` | 0x70 | matching | zone 0-4, occupied, face-up, `sub_0802B1B8` ok, and `sub_0800C8BC == 1` |
| `0x0802BB40` | 0x54 | matching | zone 0-4, `sub_0802B1B8` ok, occupied (reusing `zone` keeps `ret` in r3) |
| `0x0802BB94` | 0x48 | matching | other player's occupied face-down zone |
| `0x0802BBDC` | 0x14C | **nonmatching** | face-up allowed occupied zone; `switch (ref number)` 0x28C/0x293/0x295/0x296/0x297/0x40C/0x40D compare `sub_0800C8BC` with 0xF/7/0xA/6/8/0x12/3; 0x28F needs `sub_0800A668(...) > 0`. Logic is right; register allocation differs |
| `0x0802BD28` | 0x70 | matching | no card 0x58A on either side (`sub_08008524`), occupied, face-down, bit 0 set |
| `0x0802BD98` | 0x58 | matching | occupied, allowed: returns face-up flag |
| `0x0802BDF0` | 0x80 | matching | occupied, card number not in 0x780-0x7CF, allowed: face-up flag (a redundant `u16` copy of the id reproduces the ROM's register move) |
| `0x0802BE70` | 0x78 | matching | other player's, allowed, `pos != ref->unk6` (u16 at +6): face-up flag |
| `0x0802BEE8` | 0x50 | matching | other player's occupied zone: face-up flag |
| `0x0802BF38` | 0x6C | matching | spell/trap zone 5-10, occupied, face-up: is it a Trap (type 21) (a redundant `u16` copy of the id reproduces the ROM's register move) |
| `0x0802BFA4` | 0x60 | matching | other player's occupied allowed zone: NOT bit 0 (`1 & ~flags`) |
| `0x0802C004` | 0x7C | matching | other player's occupied allowed zone, number != 0x547: face-up flag |
| `0x0802C080` | 0x1B0 | matching | zone 5+, face-up, card number in a 27-entry set (0x12C-0x13C, 0x13E, 0x140-0x147, 0x28A...0x60E) and its stat-bits 17-19 == 3 (type 21/22) and `sub_0800CC18` and `sub_0800CD24 > 1`. An explicit `default: return 0` placed before the allowed case group joins the final 0x604/0x60E comparisons. Ordinary C, no hints; complete unit exact |
| `0x0802C230` | 0x44 | matching | other player's occupied zone 5-10 |
| `0x0802C274` | 0x38 | matching | occupied zone 5-10 |
| `0x0802C2AC` | 0x40 | matching | occupied zone 0-4: NOT face-up (`1 & ~(flags >> 1)`) |
| `0x0802C2EC` | 0x48 | matching | occupied zone 5-9: NOT face-up |
| `0x0802C334` | 0x8C | matching | own occupied face-up allowed zone 0-4 with number 0x15 or `sub_0800C8BC == 0x13` (a redundant `u16` copy of the id reproduces the ROM's register move) |
| `0x0802C3C0` | 0x11C | matching | own occupied zone 0-4, no card 0x58A, "level" (0 for types 21-23, 10 for 24, else stat bits 25-28) non-zero and `sub_08044224(player, 0x526, level + 1) > 0`. One initialized level-result read/write constraint preserves the shared zero test; a separately initialized count and named r0 argument copy retain the original call setup order. Complete unit exact |
| `0x0802C4DC` | 0x5C | matching | `sub_0802B1B8` ok, own zone 0-4, occupied |
| `0x0802C538` | 0x64 | matching | zone 5-10, occupied, face-up, type 22 (Magic) |
| `0x0802C59C` | 0x48 | matching | zone > 4, occupied, other player's |
| `0x0802C5E4` | 0x4C | matching | zone 0-4, occupied, face-up: bit 16 of the card word |
| `0x0802C630` | 0x44 | matching | zone 5-9, occupied, face-down |
| `0x0802C674` | 0x108 | matching | occupied face-up allowed monster (type <= 20) whose "kind" == 2: number 0x776 -> 3, 0x777-0x778 -> 1, else type 22/21/23 -> 7/8/9, else stat bits 18-19 |
| `0x0802C77C` | 0xF8 | matching | effect: by `ref` number queue event `0x43`/`0x8043` (`sub_0801EC58`) with a value 500/800/1000/3000/5000, or (0x404) `sub_080754A4(player LP word)`. Returns 1 |
| `0x0802C874` | 0x1C | matching | effect: `sub_08017FF4(ref->player, ref->zone)`, return 1 |
| `0x0802C890` | 0x24 | matching | same, only if `ref->unk2_10 == 2` |
| `0x0802C8B4` | 0xC0 | matching | 2-step prompt (`0x02017A40+0x3E4` = step): text `0x0808277C`, `sub_08052F38(0xF0)`; accepts the hand card at `0x0201CFB0+0x82C` if `sub_0800ABC8` gives attribute 2 and value <= 1000 (SE 3 otherwise); B (`gMain+6` bit 1) goes back a step |
| `0x0802C974` | 0x50 | matching | effect: events `0x43`/`0x8043` (1000) and `0xB3`/`0x80B3` (zone) |
| `0x0802C9C4` | 0x6C | matching | effect: by `ref` number (0x3FF/0x405 -> 1, 0x430 -> 2, 0x42B -> 5) `sub_08022758(player, n, 0, 0)` |
| `0x0802CA30` | 0x6C | matching | 2-step prompt (text `0x080827C4`) then `sub_08017FF4(0x0201CFB0+0x824, +0x82C)` |
| `0x0802CA9C` | 0x4C | matching | effect: event `0x43`/`0x8043` with `sub_080754A4(0x020192E4[player].u16)` |

## Data

- Duel player block: `0x020192E4 + (player & 1) * 0xD64`; `+0x00` u16 `lifePoints` (input of `sub_080754A4`, hypothesis); zones start at `+0x28` (`0x0201930C`), 0x94 bytes each, see [[code-0802aac0]]. Layouts now from `include/duel.h`/`include/main.h`.
- `struct CardRef` (0x14): `+0` u16 card id, `+2` bit 0 player, bits 4-9 zone, bits 10-15 `unk2_10`, `+6` u16 `pos` (player | zone << 8) used by `0x0802BE70`.
- `0x02017A40 + 0x3E4`: step byte of the two prompts.
- Card word: bits 0-11 id, bit 16 tested by `0x0802C5E4`.

## Matching tricks

- `int one = 1; int p = player & one; ... (ZFLAGS(z) >> 1) & one` gives the ROM's shared `movs r7,#1`.
- **Redundant `u16` copy of an extracted id:** `int id = CARD_ID(...); u16 copy = id;` then use `copy` in `CARD_NUMBER(copy)`/`CARD_TYPE(copy)` while testing the original `id`. old_agbcc then emits the ROM's `lsrs r0,#20; adds rX,r0,#0` register move (`0x0802BDF0`, `0x0802BF38`, `0x0802C334`; found with decomp-permuter). The copy is not used until later and looks redundant, so mark it `/* FAKEMATCH */`.
- `one & ~flags` is `bic` (`0x0802BFA4`, `0x0802C2AC`).
- `(u32)(zone - 5) <= 5` for zone 5-10 (`sub; cmp; bhi`); `zone > 4` for 5+; `zone <= 4` then `bgt`.
- `0x7FF & id` order does not matter, but a `u32`/`int` id does not fix the id copy (below).
- **Non-folded `symbol + big_offset`:** `u8 *base = gUnk_02017A40; u8 *step = base + 0x3E4;` keeps `ldr r0,=sym; mov r1,#0xF9; lsl; add` (a direct `&sym[0x3E4]` folds into the literal pool). Also `u8 *cbase = gUnk_0201CFB0; *(int *)(cbase + 0x82C)`; compute `int pl = ref->player;` before it to get the ROM's order.
- **Early returns / `default: return 0`:** see [[code-0802cae8]] for the rules on where agbcc places shared `return` blocks; an inlined `static inline` helper with several `return`s reproduces the ROM's result copy.
- `switch ((int)CARD_TYPE(id))` (int cast) gives the signed `bgt` tree; a plain `if (a == X) ... else if (a >= X && a <= Y)` merges into an unsigned range and CSE's later reads, but a `switch (number) { case 0x776: ...; case 0x777: case 0x778: ...; default: }` reproduces the ROM (`0x0802C674`). Body order follows source order (`0x0802C77C`: 500, 800, 1000, 3000, 5000, special). GNU case ranges (`case 0x12C ... 0x13C:`) work.
- `ref->player ? 0x8043 : 0x43` reproduces `mov r2,#0x43; ... ldr r2,=0x8043` (`mov #1; and` test of the 1-bit field).
- `sub_0800C8BC` result assigned to `u16 a` gives `lsl #16; lsr #16`.

## Remaining and historical allocation findings

- **Resolved `u16 pos` copy in `0x0802BE70`** (`lsrs r0,r1,#16; adds r7,r0,#0`): initialize a u16 normalization local bound to `r0` and use an empty input constraint before assigning the preserved copy. Computing the player from that copy keeps the normalization and r7 copy separate while the original pos supplies the zone. This FAKEMATCH hint emits no instructions, introduces no unset values and preserves the original `(ref,u16 pos)` ABI.
- `0x0802BBDC`: register allocation (`p*0xD64` in `r9`, zone ptr in `sl`, base reloaded). The permuter's best is score 270.
- `0x0802C3C0`'s historical permuter "score 0" was a false positive: its old metric ignored branch targets. The current accepted source is independently exact with strict branch targets and all unit bytes; see the resolved technique below.
- `0x0802C080`'s historical permuter match read an unset `new_var` and was rejected. The current accepted ordinary C uses no such scratch and reproduces the shared comparison by putting an explicit default return before the allowed cases, as described below.
- Historical id/pos-copy observations are related to `0x0802B48C`/`0x0802BA68` in [[code-0802aac0]]; this unit now has only the `sub_0802BBDC` fallback listed above.

## Resolved level predicate

Fresh `sub_0802C3C0` baseline was 0x108 versus ROM 0x11C. `CARD_LEVEL(level,id)` followed by one empty `+r(level)` retains the initialized result through the shared `level != 0` test, including the original zero/ten case blocks. An inline level helper alone did not stop the jump-threading. A separate initialized `count = level2 + 1` narrows the remaining call-order miss to four bytes; initializing a local argument copy bound to r0 before `sub_08044224(arg0,0x526,count)` fixes them. The argument input constraint was removed and bytes stayed exact. The remaining read/write constraint emits no instruction, no value is unset, and the original `(ref,u16 pos)` and called-helper ABIs remain. Both original level computations and their table rereads are retained. Source documents the compiler choices as FAKEMATCH. The function is exact at 0x11C and the entire unit at 0x1018, with C coverage now 31/33; log `build/middle_experiments/sub_0802C3C0/unit-check.log`. Private scripts `card_level_boundaries.py` and `card_level_call_order.py` retain the bounded failed variants and successful branch/call-order stages.

`sub_0802C080` was then resolved in ordinary C. Fresh baseline was 0x1B4 versus ROM 0x1B0, with the correct enum tree except for a duplicated final comparison. Placing `default: return 0;` before the entire allowed case group makes the 0x604 arm branch to the same final `cmp r2,r0; bne fail` as 0x60E. A default after the group, a common failure goto, explicit case-body return and added success labels did not settle it. The fix uses no hint, unset variable, changed card set, extra access or ABI change. `card_set_tails.py` records the bounded layout grid. The clean body and the complete 0x1018 unit are exact, with C coverage now 32/33. Log `build/middle_experiments/sub_0802C080/unit-check.log`.
