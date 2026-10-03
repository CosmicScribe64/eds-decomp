---
title: Unit card_detail (Card Detail viewer, duel helpers)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit card_detail

`0x08006878`–`0x08007993`, Thumb, `old_agbcc -O2`. Source: `src/card_detail.c`.
Contents: the **Card Detail** viewer (debug menu items "Card Detail" / "Auto Detail", see [[debug-menu]]) and small duel/card helpers.

Unit status: `unit bytes MATCH`, **23/23 functions in C**; no `INCLUDE_ASM` remains (verified with `tools/dr python3 tools/check.py card_detail`, 0x111C bytes).

## Shared headers

This unit uses the canonical layouts from `include/main.h` (`struct Main`, `gMain`)
and `include/duel.h` (`struct DuelPlayer`, `struct DuelZonesPlayer`, `gDuelZones`).
It does **not** include `duel_ui.h` (no `DuelCmd`/`DuelScreen` state here).
The local `struct Main`, `struct DuelPlayerLp`, `struct DuelSlot` and `struct DuelSlotsPlayer`
definitions plus the local `gMain`/`gDuelZones` externs were removed; field accesses
were renamed (`hblankPal` to `hblankScroll`, `lp` to `lifePoints`, `slots[]` to `zones[]`).

One local view is kept:
- `struct DuelSlotFlags` (bytes 1..3 bitfields) for `ClearCardStatusFlags`/`ClearZoneCardStatusFlags` only. The
  canonical `struct DuelZone` puts a `DuelCard` word at +0x00 and `serial` at +0x04. These
  functions clear status-flag bits in byte offsets 1..3 of each 0x94-byte slot, so the header
  cannot express the flag accesses. `ClearZoneCardStatusFlags` still indexes the canonical
  `struct DuelZonesPlayer::zones[]` (same 0x94 stride / 0xD64 player stride) and casts the
  element to `struct DuelSlotFlags *`.

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x08006878` | 0x14 | matching | Clear Card Detail state (`MemClear16(&gCardDetail, 0x44)`) | `CardDetail_Clear` |
| `0x0800688C` | 0xE0 | matching | Init viewer for a card: displayed ATK/DEF, timer, HBlank flag | `CardDetail_Init` |
| `0x0800696C` | 0x12C | matching | Step 0: video setup (BGxCNT, font, sprite gfx), optional HBlank handler `CardDetail_HBlank` | `CardDetail_SetupVideo` |
| `0x08006A98` | 0x24 | matching | Fade in (DISPCNT \|= 0x1F00) | |
| `0x08006ABC` | 0x2C | matching | Fade out, then hide layers | |
| `0x08006AE8` | 0x98 | matching | Main step: A/B/timer close, Up/Down scroll | `CardDetail_Main` |
| `0x08006B80` | 0x188 | matching | Draw card frame (by type / subtype; none for tokens) and the card | `CardDetail_DrawCard` |
| `0x08006D08` | 0x18C | matching | Card Detail callback: setup, fade in (HBlank variant: mosaic + BLDY ramp, palette rows from `0x08198A50`), main, fade out; state in `gCardDetail` bits 1–15 | `CardDetail_Callback` |
| `0x08006E94` | 0x118 | matching | "Auto Detail" step: init for card 800, then run setup | |
| `0x08006FAC` | 0x24C | matching | Interactive card browser: Left/Right ±1, held L/R ±10, card ID 1..0x334 | `CardDetail_Browse` |
| `0x080071F8` | 0x174 | matching | Slideshow: every card ID 1..0x334 in turn (hypothesis: debug) | |
| `0x0800736C` | 0x50 | matching | Card Detail step runner (`gDebugCardDetailSteps`) | |
| `0x080073BC` | 0x5C | matching | Auto Detail step runner (`gDebugAutoDetailSteps`) | |
| `0x08007418` | 0x24 | matching | Subtract LP, clamp at 0 | `DuelSubtractLp` |
| `0x0800743C` | 0x40 | matching | Clear slot status flag bits | |
| `0x0800747C` | 0x24 | matching | Same, for (player, slot) | |
| `0x080074A0` | 0xB8 | matching | Same-card test (alt art +2000 folded, 3 equivalent pairs) | `IsSameCard` |
| `0x08007558` | 0x8 | matching | `*dst = *src` (u32) | |
| `0x08007560` | 0xC | matching | swap two u32 | |
| `0x0800756C` | 0x24 | matching | card number in {726,727,728,766} | |
| `0x08007590` | 0x1A0 | matching | Card-number list test (40 numbers → 1; 640 → `!flag`; 735/1170 → `flag`) | |
| `0x08007730` | 0x104 | matching | Monster with subtype 1, or card number in {730,812,1241,1334,1526} | |
| `0x08007834` | 0x160 | matching | Monster with subtype 2/3, or card number in a list (55, 56, 62, 66, 368, 373, 391, 726–728, 741, 766, 845, 1202, 1257, 1514–1519); never subtype 0 | |

## Data

- `gCardDetail` = `0x02013D90` (0x44 bytes): +0x00 bit0 useHBlank (u8 container), bits 1–15 state of `0x08006D08` (u16 container), +0x02 cardId, +0x04 timer, +0x2C atk, +0x30 def, +0x34 scrollPos, +0x38 scrollTarget, +0x3C scrollMax, +0x40 card ID used by Auto Detail.
- Card stats word `0x08621DE0[id & 0x7FF]`: DEF = bits 0–8, ATK = bits 9–17, subtype = bits 18–19 (`& 0xC0000`), type = bits 20–24. Types 21, 22 and 23 are Magic, Trap and Ritual in some order (hypothesis: 21 Magic, 22 Trap, 23 Ritual), and 24 is Divine (ATK/DEF shown as 4000).
- Card "subtype" helper (inlined into `0x08006B80`, `0x08007730`): card number 1910 gives 3, 1911–1912 give 1, type 22/21/23 give 7/8/9, and anything else uses stats bits 18–19. Frame graphics per subtype 1/2/3/other: `0x08627AF8`/`0x0862A190`/`0x0862C828`/`0x08625460`; per type 21/22/23: `0x08631558`/`0x0862EEC0`/`0x08633BF0`.

## Matching tricks

- **Card tables through constant addresses.** Write `((const u32 *)0x08621DE0)[id & 0x7FF]`, not the `extern` symbol. GCC then reloads the table address at every use instead of keeping it in a register. This is needed for `0x0800688C` and `0x08007730`.
- **Range tests that are really `switch`es:** `cmp 1910; bne; ...; cmp 1910; blt; cmp 1912; bgt` is `switch (no) { case 1910: ...; case 1911: case 1912: ... }` (a case-range tree), not `if (no >= 1910 && no <= 1912)`.
- Inline helpers returning values (`GetCardAtk`, `GetCardSubtype`, `GetCardFrameGfx`) reproduce the `b join` layout of the switches.
- `LoadBgImageMap1`'s first parameter is `u16`; declaring it `u32` changes how `pal | 0x440` is built.
- `IsSpecialSummonOnly`: assign `GetCardSubtype(id)` to an initialized `int subtype`, then consume it with `asm volatile ("" : : "r"(subtype))` before the following switch. This empty input-only barrier keeps jumps from constant subtype values (`mov r0,#3; b join`) entering the common switch head; without it GCC threads them past the `== 0` / `< 0` tests. The barrier emits no instructions or value changes. The complete 0x160-byte function and complete unit match.
- Step functions returning `0`/`1`: write `return 0;` at the end of every case and a final `return 1;` (not `default: return 1;` plus a trailing `return 0;`), otherwise the `mov r0,#0` / `mov r0,#1` blocks come out in the wrong order (`0x08006D08`, `0x08006FAC`, `0x080071F8`).
- `gMain` +0x4834 is an `s16` line counter for the HBlank effect (`ldsh`, set to -0x20), +0x4836 a 0x20-byte buffer filled by `MemCopy16` (hypothesis: a palette row).

## Property flag interface (2026-10-01)

`HasFlipEffect(u16 cardNo, int flags)` decodes `u16 flag = flags` explicitly. The complete 0x111C-byte unit stays exact, with all 23 functions in C. This models the observed [[ai-steps-c]] scanner call, which passes `0x08622AB4` in r1 (low half 0x2AB4). Numbers 735 and 1170 return that flag value instead of normalizing it to one, and number 640 tests for zero. Existing narrow callers pass the same low-half values in the same argument locations. Evidence: `build/bigguns-lead2/HasFlipEffect/solo-zone-accepted/` and the full-ROM run in `build/lead-pass24/`.
