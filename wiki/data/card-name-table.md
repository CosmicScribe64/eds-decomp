---
title: Card name table (English)
type: data
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Card name table (English)

Verified 2026-09-29. English card names are NUL-padded ASCII strings in fixed 0x40-byte slots, indexed by card ID (1-based). Slot 0 is empty. The code addresses the table from the slot-0 base `0x0822C720`, as `0x0822C720 + id*0x40`.

| Field | Value |
|---|---|
| Base (slot 0, empty) | `0x0822C720` (`ROM+0x22C720`); 58 literal-pool references |
| First name (ID 1) | `0x0822C760`: "7 Colored Fish" |
| End | `0x08239460` (821 × 0x40 = `0xCD40` bytes) |
| Entry size | 0x40 (`char[64]`; the longest name is 41 chars) |
| Count | 821 slots = 820 cards + empty slot 0 |

```c
/* 0x0822C720 */
const char gCardNames[821][0x40];    /* name is a proposal; gCardNames[0] = "" */
```

## Order
- **IDs 1–813 are alphabetical**, ending with "Zone Eater". The sort key is lower-cased and hyphens are dropped: `key = name.lower().replace('-', '')` gives zero inversions over IDs 1–813. Plain ASCII order gives 16 inversions, and plain case-folding gives 6. Digits sort first, so "7 Colored Fish" is ID 1.
- **IDs 814–820 are appended out of order:** The Monarchy, Set Sail for the Kingdom, Glory of the King's Hand (the 3 Ticket cards), Obelisk the Tormentor, Slifer the Sky Dragon, The Winged Dragon of Ra, and Insect Monster Token.
- **Duplicate names are separate IDs.** Nine cards have an alternate-artwork copy directly after the original. Blue-Eyes White Dragon is IDs 82 and 83, and Dark Magician is 150 and 151. See [[card-id-map]].
- The "Blue-Eyes" hits at `ROM+0x22DB20`–`0x22DBE0` are IDs 80–83: Blue-Eyes Toon Dragon, Blue-Eyes Ultimate Dragon, Blue-Eyes White Dragon, and Blue-Eyes White Dragon (alt art).

> [!warning] Contradiction (resolved 2026-09-29)
> The earlier draft of this page (from the 2026-09-29 setup scan) said the table had "820 slots" starting at `0x0822C760`, and that "slot index is probably not card ID". Both claims are superseded:
> - The code indexes with a slot-0 base (`0x0822C720`), so the table has 821 slots.
> - The alphabetical slot index is the primary in-memory card ID. Names, descriptions, stats, art, and passwords all share it. There is still a second numbering (the language-independent "card number"), with conversion tables. See [[card-id-map]].

## The second, empty name bank
`0x08239460`–`0x082461A0` is another 821 × 0x40 block of all zeros, running up to the description table. No code references it. It could be a slot for a second name set (for example, the reading/kana names the JP build used for sorting) that the US build left empty (hypothesis).

## The "other name copies" (resolved)
The extra "Blue-Eyes" and "Dark Magician" hits at `ROM+0x24F980` and `ROM+0x257ED3` are not other languages. They sit inside [[card-descriptions]]:
- `ROM+0x24F980` is ID 81's (Blue-Eyes Ultimate Dragon's) fusion-material text.
- `ROM+0x257ED3` is inside ID 152's (Dark Magician Girl's) effect text.

The US ROM contains no German, French, Italian, or Spanish card names; searches for common translations came up empty.

## Method
- Walked the slots from `0x0822C760` in 0x40 steps while each slot was printable ASCII. That gives 820 names, and the slot after "Insect Monster Token" is all zero.
- Literal pools hold `0x0822C720` in code, for example `sub_08005A70`: `lsls r0, r7, #6; ldr r1, =0x0822C720; adds r1, r1, r0`.
- The code at `0x08000FC0` (inside `sub_08000C54`) converts a card number to an ID with the table at `0x08623DF4`, then forms `0x0822C720 + id*0x40`.
- No slot has non-NUL bytes after its terminator.
- Reproduce with `python3 tools/extract_cards.py cards`.

Related: [[card-table]], [[card-descriptions]], [[card-id-map]], [[cards]].
