---
title: Password table
type: data
status: solid
confidence: high
sources: [rom-analysis, web-card-references]
updated: 2026-10-01
---
# Password table

**Verified 2026-09-29.** Each card's 8-digit password is stored as 4 packed-BCD bytes in reading order. The first digit pair goes in the first byte, so the bytes look "big-endian". The table is indexed by card ID.

| Field | Value |
|---|---|
| Address | `0x08623120`–`0x08623DF4` (`ROM+0x623120`) |
| Entry size | 4 bytes (`u8[4]`, BCD) |
| Count | 821 (ID 0 = `00 00 00 00`) |
| No password | `FF FF FF FF` (63 cards) |
| With password | 757 cards |

```c
/* 0x08623120 */
const u8 gCardPasswords[821][4];   /* proposal; e.g. BEWD (ID 82) = {0x89,0x63,0x11,0x39} */
```

## Examples
| ID | Card | Bytes | Password |
|---|---|---|---|
| 1 | 7 Colored Fish | `23 77 17 16` | 23771716 |
| 82 | Blue-Eyes White Dragon | `89 63 11 39` | 89631139 |
| 83 | Blue-Eyes White Dragon (alt art) | `80 90 60 30` | 80906030 |
| 150 | Dark Magician | `46 98 64 14` | 46986414 |
| 579 | Red-Eyes B. Dragon | `74 67 74 22` | 74677422 |
| 675 | Summoned Skull | `70 78 10 52` | 70781052 |

Cards without a password include most fusion and ritual monsters, ritual spells, Dark Magician Girl, Kazejin, Sanga of the Thunder, Gate Guardian, the Tickets, the Gods, and the Token.

## Lookup: `sub_0807C304`
`sub_0807C304` (`0x0807C304`–`0x0807C374`, Thumb) handles password entry:
1. It packs 8 entered digits from `0x0201F7B0 + 8` into 4 BCD bytes on the stack (`hi<<4 | lo`).
2. It scans IDs 0..820 (`cmp r3, #0x334` inclusive) and compares all 4 bytes.
3. It returns the matching **card ID**, or 0 if nothing matches.

`FF` entries can never match, because every digit is 0–9.

## Method
- Searched for each known password in three encodings: BCD big-endian, BCD little-endian, and binary u32. Only BCD big-endian hit, once per card: BEWD at `ROM+0x623268` = `0x08623120 + 82*4`, Dark Magician at `ROM+0x623378` = `+150*4`, and so on.
- The table's boundaries meet the ID-to-number table (after 2 bytes of padding) and the number-to-ID table exactly. See the per-card data block section of [[card-table]].
- The disassembly of `sub_0807C304` confirms the byte order and the 821-entry bound.
- `python3 tools/extract_cards.py --verify` checks 4 passwords.

Related: [[card-id-map]], [[card-table]], [[cards]].
