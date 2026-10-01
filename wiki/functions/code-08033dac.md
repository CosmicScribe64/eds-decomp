---
title: Unit code_08033DAC (duel card-effect step handlers, part 2)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit code_08033DAC

`0x08033DAC`-`0x08035197`, Thumb, `old_agbcc -O2`. Source: `src/code_08033DAC.c`. Continuation of [[code-08032cb0]]: more card-effect step handlers `int f(struct EffCtx *ctx)` (same `EffCtx` head as `CardRef` in [[code-0802cae8]]: `+0 id`, `+2 player/zone/kind`, `+4` flags (bit 2 = skip), `+6 u16` (player | zone << 8 of a card, "CardRef view"), `+0xA` bits 0-2 phase or target count, `+0xC u16 pos` (target), `+0xE u16`). Return value is the next step (0x64-0x80) or 0; the step byte is `0x02017A40+0x3E0`, sub-step `+0x3E1`.

Unit status: `unit bytes MATCH`, 14/17 functions in C; 3 remain `INCLUDE_ASM`. Verified with `tools/check.py code_08033DAC` (0x13EC bytes).

## Shared headers
The unit includes `include/duel.h` and `include/duel_ui.h` (the latter pulls in `duel.h`); `include/main.h` is not needed (`gUnk_03000040` is unused here). It removed four local struct definitions (`DuelCard`, `DuelZone`, `DuelZonesPlayer`, `DuelPlayerHead`) and the local externs for `gUnk_020192E0`/`gUnk_020192E4`/`gUnk_0201930C`/`gUnk_0201CFB0`, and switched to the canonical tags and field names. `flags6 & 2` became `flag6_1`, `w824`/`b828` became `player`/`zone`, and the `#if 0` drafts' hand entries go through `CARD_WORD` since `hand[]` is now `struct DuelCard[]`.

Local views kept:
- `struct DuelStateByteView` (`gDuelStateBytes asm("gUnk_020192E0")`): `duel.h`'s `struct DuelState` models `+0x1ACC` as a `u32` bitfield (`unk1ACC_0:15`) and has no byte-sized field at `+0x1ACD`. `sub_08033DAC` tests bit 6 of the byte at `+0x1ACD` (u32 bit 14) and only matches with a `u8` load, so the unit keeps a byte view for that one function.

## Functions

| Address | Size | Status | Purpose (hypotheses) |
|---|---|---|---|
| `0x08033DAC` | 0x48 | matching | log msg 0x1D with argument `!((byte 0x020192E0+0x1ACD >> 6) & 1)` |
| `0x08033DF4` | 0x50 | matching | step 0x80: `sub_080199E0(player, 3)` -> 0x7F; 0x7F: `sub_0802272C(player, 2, 0, 0)` -> 0x7E |
| `0x08033E44` | 0x138 | matching | CardRef-view handler: card at `pos6` zone; 0x80 (needs a card) resets sub-step, 0x7F scans the own hand for a card `sub_080074A0(hand id, zone card id)`, 0x7E `sub_08019E0C(player, number, 1)`, 0x7D log 0x60 -> 0x78 |
| `0x08033F7C` | 0x34 | matching | log msg 0x48 |
| `0x08033FB0` | 0x94 | matching | `(ctx, a)`: `sub_0802E784`, phase 1, `sub_0802BE70(ctx, pos)`: stores `player \| sub_08008A44(player) << 8` into `ctx+0xE`, logs 0xA2 and 0x38, `sub_08019078` |
| `0x08034044` | 0x118 | matching | among the opponent's zones 0-4 (face-up cards) picks the one with the lowest ATK (`sub_0800C894`, start 99999); if `sub_0802B28C` accepts it does `sub_08030028`/`sub_08046CB0`, else `sub_080197E0(player, card id)` |
| `0x0803415C` | 0xE4 | matching | CardRef-view: kind 5/6, zone card present and `sub_0802B1B8`; card numbers 0x2A8 (`sub_0800C8A8 <= 500`), 0x2A9 (ATK <= 500), 0x3EA (ATK > 999) gate `sub_08030028` + `sub_08046CB0` |
| `0x08034240` | 0x80 | matching | phase 1, target zone holds a face-up Magic (type 0x15): `sub_08018544(player, zone, 1)` |
| `0x080342C0` | 0x9C | matching | phase 3, `phaseA & 7` targets `list[0..n]` at `ctx+0xC` (u16 pos each): all must be occupied, then `sub_08030028` + `sub_08046CB0` for each |
| `0x0803435C` | 0xB8 | matching | card number 0x3F0 with card 0x402 on either side aborts; phase 2 and `sub_08008A1C`: builds a card word from `ctx+0xE:ctx+0xC`, `sub_08009C08`, logs 0xD3, `sub_08056094(..., 0x30)` |
| `0x08034414` | 0x68 | matching | by card number 0x21B/0x5A7 -> 1, 0x3F2 -> 2: `sub_080199E0(player, n)` |
| `0x0803447C` | 0x1C8 | matching | 6-step machine 0x7B-0x80 (prompt `gUnk_08082CA8`/`CE0`, selected card block `0x0201D810`, log 0xD4, sub-step countdown) |
| `0x08034644` | 0xC4 | nonmatching (`#if 0`) | own hand non-empty and `sub_08052F38(0x10000)`: logs 0x08 with the hand cursor `0x0201CFB0+0x824..0x82C`, `sub_08019788(player, card id of the opponent hand card at the cursor)`; else returns 0x80. Constants hoisted into r8/r9 in the ROM |
| `0x08034708` | 0x60 | matching | phase 1, target zone face-up and occupied: `sub_08017AB4(player, id, pos, 3)` |
| `0x08034768` | 0x440 | nonmatching (`#if 0`) | 29-entry jump table, steps 0x64-0x80: the hand-card selection wizard. 0x80 scans the own hand for a card with `sub_08054398 != 0 && sub_08007834 == 0` (like `0x0802E058` in [[code-0802db30]]) and shows prompt `gUnk_08082D24`; 0x7F/0x76/0x74/0x6C read the hand cursor (`0x0201CFB0+0x82C`, `sub_08008A6C(+0x824, cursor)`) and store picks into `ctx+0xC/0xE/0x10`; card type/level (0x15-0x17 -> 0, 0x18 -> 10, else stats bits 25-28) picks the next step 0x64 / 0x6E / 0x78; prompts are entries 0-4 of the pointer table `0x0819D1C4`; 0x64 finishes with `sub_08055D3C(player, ctx+0xC, +0xE, +0x10)` and returns 0xA. Control flow matches; only register allocation differs |
| `0x08034BA8` | 0x54 | matching | `sub_08044224(player, number, 0)` then `sub_0802AF34(player, -1, number, 0)` |
| `0x08034BFC` | 0x59C | not attempted | |

## Matching tricks
- `!((byte >> 6) & 1)` (not `1 & ~(byte >> 6)`) gives `bic r1, r0` with the shared constant 1 in `0x08033DAC`.
- `u16 id = CARD_ID(...)` gives the `adds r5, r4, #0` copy and avoids the `lsl #16; lsr #16` before a `u16` parameter (`0x08033E44`).
- A card-word bitfield struct (`u32 id:12; u32 flag12:1; u32 rest:19`) with `.flag12` reproduces `lsl #19; lsr #31` (`0x0803435C`).
- Switch with a shared tail (`0x0803415C`): order the cases as the ROM lays the blocks out (0x3EA, 0x2A8, 0x2A9) and agbcc cross-jumps the identical tails. Inline `Le500(value)` / `Gt999(value)` helpers keep their bounds inside the comparison, reproducing the ROM's zero-result setup before loading the comparison constant. An empty read/write constraint on initialized `ok = 1` prevents reusing it as the later player mask; an empty `r5` clobber at return gives ctx/zone registers r5/r4. Both FAKEMATCH hints emit no instructions and use no unset values.
- `sub_08034414`: `n = 0; switch { case A: case B: n = 1; break; case C: n = 2; } if (n > 0) call(n)` makes agbcc thread the `n = 1` cases straight to the call.
- Returning `0x80` from an `else` of the `sub_08052F38` test puts the return block last (`0x08034644`).
- Big state machines: `goto` labels reproduce the ROM's shared returns (`found: prompt(); ret7f: return 0x7F;` placed after the last case); a `switch ((int)v)` with cases 1-4 / 5-6 / default gives the separate `cmp #1; blt; cmp #4; bgt; cmp #6; bgt` compare chain instead of an unsigned range test (`0x08034768`).
- A `for (i = 0; i < arr[1 & ctx->player].handCount; i++)` loop shows the ROM's guard without the `& 1` (folded) and the loop-end test with it.
- The sign test of a byte bit as `((int)((u32)byte << 26) < 0)` works in [[code-08032cb0]]; `flag = 0; if (x) flag = 1;` gives the `negs; orrs; lsrs` setcc.
- **Recomputed list addressing through initialized offset/base constraints** (`sub_080342C0`): each iteration assigns `off = i * 2` in r1 and `loopbase = (u8 *)ctx` in r0 and passes both initialized values through an empty read/write constraint. It then adds `0xC` to the base and passes that initialized base through an empty input before adding `off`. This preserves the ROM's `lsl r1,index,#1; copy ctx to r0; add r0,#0xC; add r0,r0,r1` in both loops. The first constraint prevents strength reduction into a walking pointer; the second prevents folding `+0xC` into the index. Hints emit no instructions and are marked FAKEMATCH; original ABI remains unchanged. Index-only constraints changed allocation/length and did not match, while the two-value form without the second input differed at the two `+0xC` instructions. Complete unit `0x13EC` bytes verified exact.

## Unsolved

Remaining `INCLUDE_ASM`: `sub_08034644`, `sub_08034768`, `sub_08034BFC`. Each keeps its original assembly; decoded drafts and current mismatch notes remain guarded by `#if 0` in the unit source.

## Verified C conversions (2026-09-30)

- `sub_08034044` (Thumb, `0x118` bytes) is matching C. It scans occupied face-up opponent monster zones for the smallest ATK, then performs the selected action. An empty r1 clobber in the loop and reversing the player/zone terms in the final address sum resolve allocation and add operand order.

The compiler hints emit no instructions. Each conversion passed a whole-unit byte comparison with the baserom; remaining assembly functions retain their original bytes. The matching C above resolves earlier notes that described these functions as allocation near misses.

The source also contains the verified matching C for `sub_08033E44`. Status counts and function-table rows above reflect those recovered matches.
