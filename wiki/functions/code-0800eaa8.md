---
title: Unit code_0800EAA8 (duel script-command handlers)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit code_0800EAA8

`0x0800EAA8`–`0x0800FB0F`, Thumb, `old_agbcc -O2`. Source: `src/code_0800EAA8.c`.
Duel "script command" handlers. The dispatcher `sub_0801ECA8` switches on
`(gUnk_020185C0.cmd & 0xFFF) - 1` and calls one of these. Each handler reads its operands from the
command block at `0x020185C0` and clears the "command running" flag (bit 5 of byte `0x020185C0+0x80D`)
when it is done. See [[code-08010bdc]] (sibling handlers), [[code-0800d8a4]].

Unit status: `unit bytes MATCH`, **22/22 functions in C** after workflow wave 2 (2026-10-01: `0x0800EE50` in wave 2); none stay `INCLUDE_ASM`. Before wave 2: 21/22. Verified with `tools/check.py code_0800EAA8`.

## Shared headers

The unit uses `include/duel.h`. It does **not** need `include/main.h`, because it never touches
`gUnk_03000040`. `struct DuelCard`, `DuelZone`, `DuelPlayer`, `DuelState`, `DuelZonesPlayer` and the
globals `gUnk_020192E0` / `gUnk_020192E4` / `gUnk_0201930C` now come from that header.

Deleted from the unit: four local struct definitions (`DuelCard`, `DuelZone`, `DuelPlayer`,
`DuelState`) and three local `extern`s (`gUnk_020192E0`, `gUnk_020192E4`, `gUnk_0201930C`). Canonical
field names adopted: `DuelZone.numLinks`, `DuelState.queueCount/queueZone/queueArg`,
`DuelState.linkSkip`, `DuelPlayer.handCount/deckCount`. The unit-local `struct DuelCmd`, `CardLoc`,
`Cmd80C`, `DuelScreen` and `Unk02017FB0` are not in a shared header and were kept.

Local views kept (canonical declaration differs; see the comments in `src/code_0800EAA8.c`):

- `struct DuelZoneWord4`: canonical `DuelZone` declares `+0x04` as `u16 serial` and `+0x06` as the u8
  bitfields `flag6_0`/`flag6_1`/`counter6`. This unit reads and writes `+0x04` as one u32 bitfield
  container whose counter sits at bits 18–21 (`+0x06` bits 2–5). Needed by `sub_0800EBF4`,
  `sub_0800EC54` and `sub_0800ECB0`.
- `struct DuelCardBit22`: canonical `DuelCard` has `flag20` at bit 20 and `unk21:11` for bits 21–31,
  so the bit-22 flag that `sub_0800EBA0` writes has no canonical field name.
- `struct DuelZone90`: canonical `DuelZone` stops at `+0x8C` (`unk8C[8]`), but this unit writes the u32
  bitfield at `+0x90` bits 13–17. Needed by `sub_0800EB44` and `sub_0800ECB0`.
- `struct DuelPlayerFlags`: canonical `DuelPlayer` declares `+0x07` as `deckOut`/`winA`/... bitfields.
  `sub_0800F0F8` sets it with a plain whole-byte `|= 1` (`ldrb`/`orrs`/`strb`), which only matches as a
  byte view.

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x0800EAA8` | 0x34 | matching | `sub_0800935C(arg4, arg2, 1)` + screen setup, clear running | |
| `0x0800EADC` | 0x34 | matching | Same with third operand `arg6` | |
| `0x0800EB10` | 0x34 | matching | `sub_08009424(arg4, arg2, arg6)` + screen setup | |
| `0x0800EB44` | 0x5C | matching | Set zone `+0x90` bits 13–17 to `arg4` | |
| `0x0800EBA0` | 0x54 | matching | Set card-word bit 22 to `arg4` | |
| `0x0800EBF4` | 0x60 | matching | Add `arg4` to the zone counter if the zone is occupied | |
| `0x0800EC54` | 0x5C | matching | Set the zone counter to `arg4` if occupied | |
| `0x0800ECB0` | 0x6C | matching | If occupied: clear counter, set `+0x90` bits 13–17 | |
| `0x0800ED1C` | 0x3C | matching | Clear a zone's `numLinks` | |
| `0x0800ED58` | 0x74 | matching | Transfer link arrays/count between command zones; clear source count and running bit | |
| `0x0800EDCC` | 0x84 | matching | Append `(arg2<<8)|player` and `arg4` to the marked-card queue | |
| `0x0800EE50` | 0xE8 | **matching** (wave 2, 2026-10-01; FAKEMATCH) | Remove a marked-card queue entry matching `(arg2<<8)|player` | |
| `0x0800EF38` | 0x1C0 | matching | Multi-frame place-card state machine (areas 11→14, link-duel check) | |
| `0x0800F0F8` | 0x19C | matching | Draw `arg4` cards (deck→hand animation); empty deck sets player `+0x07` bit 0 | |
| `0x0800F294` | 0x13C | matching | Take `arg2` cards, animate area 13→14, commit | |
| `0x0800F3D0` | 0x174 | matching | Take up to `counter` cards, animate area 13→15, commit | |
| `0x0800F544` | 0x134 | matching | Return card word from area 13 to hand | |
| `0x0800F678` | 0x38 | matching | `sub_08007F48(player, arg4<<16|arg2)` | |
| `0x0800F6B0` | 0x140 | matching | Move card word area 13→monster zone `arg6` | |
| `0x0800F7F0` | 0x108 | matching | Move card word area 13→14 | |
| `0x0800F8F8` | 0x10C | matching | Move card word area 13→15 | |
| `0x0800FA04` | 0x10C | matching | Move card word area 12→14 | |

## Data layout

- Command block `gUnk_020185C0` (`0x020185C0`): `+0x000` `u16 cmd` (bits 0–11 id, bit 15 acting
  player), `+0x002/4/6` operands, `+0x80A` `u16 step:7`/`counter:7`, `+0x80C` animation counter
  (accessed as a padded `struct Cmd80C` halfword), `+0x80D` bit 5 command-running, `+0x814` card word.
- `gUnk_020192E0` `struct DuelState` (`duel.h`): queue count at `+0x1ACC` bits 15–18, `queueZone[16]`
  at `+0x1AD0`, `queueArg[16]` at `+0x1AF0`, `linkSkip` at `+0x1B12` bit 1.
- `gUnk_020192E4` `struct DuelPlayer[2]` (`duel.h`): `handCount` `+0x02`, `deckCount` `+0x03`,
  flags byte `+0x07`.
- `gUnk_0201930C` `struct DuelZonesPlayer[2]` (`duel.h`); `ZONE(p, s)` computes
  `s*0x94 + p*0xD64 + (u8*)gUnk_0201930C`.

## Matching tricks

- `ZONE_CARD_ID(z)` reads the zone's first word raw (`(*(u32*)z << 20) >> 20`) rather than through a
  `struct DuelCard` member, which is what the ROM does.
- Zone bitfields the canonical header does not model (`+0x04` as a u32 container, card bit 22, `+0x90`)
  go through the unit-local view structs listed above; casting the zone pointer at the access site keeps
  the same base+offset codegen.
- `sub_0800F0F8`'s player flags write must be a byte view (`flags7 |= 1`). The canonical 1-bit
  `deckOut` bitfield changes register allocation for the whole function and no longer matches.
- `sub_0800EC54`: the ROM reads the zone counter byte (`+0x06`) *before* the card-id test. An explicit
  unused-looking read `u8 old = ((struct DuelZoneWord4 *)zone)->unk4_18;` before the `if` forces the
  load into the entry block. The compiler keeps it because the later `gUnk_020185C0.running`
  store may alias the zone.

## Verified C conversions (2026-09-30)

- `sub_0800ED58`, Thumb, `0x74` bytes, matching C. It transfers the two 32-halfword link arrays and link count between the acting player's source and destination zones, then clears the source count and command-running bit. An empty r6 clobber keeps the source in r5 and the destination in r6.

The compiler hints emit no instructions. Each conversion passed a whole-unit byte comparison with the baserom; remaining assembly functions retain their original bytes. Earlier notes describing these functions as allocation near misses are resolved by the matching C above.

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/sub_0800EE50/NOTES.md`.

### `sub_0800EE50` (0xE8, start score 86; FAKEMATCH)

The parked attempt hoisted the masked player bit out of the queue loop and was 4 bytes longer; the ROM hoists only the `cmd` load and keeps `mov #0x80; lsl #8; and` inside the loop. Three fixes, in order:

1. The else arm recomputes `lsl r0, r2, #8`. Wrapping the whole key in `(u16)(((u8)arg2 << 8) | (cmd & 0x8000 ? 1 : 0))` produces this: fold distributes the compare into both arms of the conditional, each arm zero-extends its value, and combine folds that extension into a second shift (86 -> 61).
2. The AND must stay in SImode. A plain `cmd & 0x8000` on the u16 field was shortened to HImode (extra `lsl/lsr #16`); an int-typed mask variable, or `(s16)cmd & 0x8000`, keeps it in SImode (42).
3. FAKEMATCH: the mask is a statement expression `({ int mask = 0x8000; asm volatile("" : "+r"(mask)); mask; })` inside the condition. In its first pass loop.c judged the constant not worth hoisting; its second pass (on the shorter loop) hoisted the constant and the AND. The volatile asm blocks the second-pass hoist (score 0).

Failed: a non-volatile asm (42; loop.c treats it as an invariant set), the mask+asm as a statement at the top of the loop body (36; the constant then lands before the arg2 load), a plain mask variable in the loop (52), `(s16)cmd < 0` / `cmd >> 15` (61), `cmd & ~0x7FFF` (43), bitfield player views (44-50), a separate `u16 key` local (104-135), an int `cmd` local before the loop (99).
