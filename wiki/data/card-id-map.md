---
title: Card ID and card number maps
type: data
status: solid
confidence: high
sources: [rom-analysis, web-card-references]
updated: 2026-10-01
---
# Card ID and card number maps

EDS uses **two card numberings**. Keep them apart on every page:

| Term | Range | Meaning | Used for |
|---|---|---|---|
| **card ID** | 1..820 (0 = none) | 1-based index into the alphabetical English tables | indexing [[card-name-table]], [[card-descriptions]], [[card-art]], [[card-table]], and [[password-table]]; the in-memory card identifier (for example, the low 11–12 bits of duel zone words, always masked `& 0x7FF`) |
| **card number** | 0..1205, 1901–1920, and 2000+ for alternate art (`0xFFFF` = none) | language-independent Konami number | [[deck-lists]], [[booster-packs]], [[special-card-lists]], the effect-handler table at `0x0819A9D4` (290 of its 426 keys are card numbers; see [[cards]]), and code that tests for specific cards |

The card number equals the classic *Duel Monsters* list number minus one: 0 = Blue-Eyes White Dragon, 1 = Mystical Elf, 2 = Hitotsu-Me Giant, 3 = Baby Dragon, 4 = Ryu-Kishin, 5 = Feral Imp, 10 = Sword Arm of Dragon, 21 = Summoned Skull. That matches the familiar DM1/Forbidden Memories order "#001 Blue-Eyes, #002 Mystical Elf, …" (hypothesis on the exact lineage, but the first ~20 match).

## ID to number table at `0x08622AB4`
| Field | Value |
|---|---|
| Address | `0x08622AB4`–`0x0862311E`, plus 2 bytes of zero padding up to `0x08623120` |
| Entry | `u16` |
| Count | 821; entry 0 = `0xFFFF` |
| Refs | 513 literal-pool copies; the end pointer `0x0862311E` is also referenced once, at `0x08063D54` |

```c
const u16 gCardIdToNumber[821];   /* proposal */
```

## Number to ID table at `0x08623DF4`
| Field | Value |
|---|---|
| Address | `0x08623DF4`–`0x08624DF4` |
| Entry | `u16` card ID; 0 = card not in EDS |
| Count | 2048 (indexed `number & 0x7FF`) |
| Refs | 107 literal-pool copies, many of them `&table[CONST]` for specific cards (for example `0x08623E66` = entry 57, and `0x08624CCE`–`0x08624CD2` = entries 1901–1903) |

```c
const u16 gCardNumberToId[2048];  /* proposal */
```

The canonical lookup is at `0x08000FC0` (inside `Bustup_UpdateTextBox`):
```c
u16 CardNumberToId(u16 no) {
    if (no == 0xFFFF) return 0;
    if (no <= 1999)   return gCardNumberToId[no & 0x7FF];
    return gCardNumberToId[(no - 2000) & 0x7FF] + 1;   /* alternate art */
}
```
Round trip: `CardNumberToId(gCardIdToNumber[id]) == id` for all 820 IDs.

### Alternate-art cards (number = base + 2000)
The alternate copy always sorts right after the original, so its ID is the base ID plus 1.

| ID | Card | Number | Password |
|---|---|---|---|
| 83 | Blue-Eyes White Dragon | 2000 | 80906030 |
| 105 | Celtic Guardian | 2040 | 90101050 |
| 151 | Dark Magician | 2034 | 40609080 |
| 236 | Flame Swordsman | 2014 | 40502030 |
| 253 | Gaia The Fierce Knight | 2037 | 00603060 |
| 406 | Launcher Spider | 2389 | 80703020 |
| 544 | Pendulum Machine | 2387 | 20404030 |
| 716 | Thousand Dragon | 2068 | (none) |
| 721 | Tiger Axe | 2063 | 40907090 |

Yugipedia lists 80906030 as a second Blue-Eyes White Dragon passcode, which confirms that these are the alternate-print passwords.

### Special numbers
- 1901–1903: the 3 Ticket cards (IDs 814–816).
- 1910–1912: Obelisk, Slifer, and Ra (IDs 817–819).
- 1920: Insect Monster Token (ID 820).
- 1201–1205: Big Shield Gardna, Dark Sage, Graceful Dice, Skull Dice, and Exchange. These may be promo/game-exclusive cards numbered after the main list (hypothesis).

Pack generation excludes numbers 1920–1999 (`number - 1920 < 80`). See [[booster-packs]]. Numbers missing below 1200 (for example 60, 102, 105, and 763–899) are cards from the wider Konami database that aren't in EDS.

> [!warning] Contradiction with [[rom-map]] (2026-09-29)
> The Card bank section of [[rom-map]] listed "one LZSS blob at `0x08624CF4` (unpacks to 0x78E)" and "u16 card lists at `0x0862502C`" (marked H).
> - `0x08624CF4` is `&gCardNumberToId[1920]`, which falls inside the 2048-entry table above. Its value is `0x0334` = ID 820, Insect Monster Token. The only code reference, at `0x08014686`, does `ldrh r1, [r0]` and passes the ID to `0x0801EC58`. This is the same constant-card-lookup pattern as the `&table[1901..1903]` literals at `0x0801C0B0`, `0x0801C1FA`, and `0x0801C754`. That's a halfword read, not a compressed blob.
> - `0x0862502C` is inside the 821-entry JP sort-key table below (entry 284). No code reference to it was found.
>
> Resolution: the table boundaries on this page are exact and consistent with the rest of the [[card-table]] block. [[rom-map]] has since been corrected.

## JP sort key table at `0x08624DF4` (hypothesis)
| Field | Value |
|---|---|
| Address | `0x08624DF4`–`0x0862545E`, plus 2 bytes of padding up to `0x08625460` |
| Entry | `u16`; entry 0 = `0xFFFF` |
| Count | 821 (indexed by card ID) |
| Refs | no direct literal-pool reference found. It may be reached as `0x08623DF4 + 0x1000`, or it may be dead. |

The values are 820 distinct numbers in the range 2..1112. **Sorting cards by this key gives Japanese gojūon (kana) order.** The Little Swordsman of Aile (アイルの小剣士) and Rhaimundos of the Red Sword (赤き剣のライムンドス) come near the start. The JP names here are from general knowledge. Remove Trap (罠はずし), Waboku (和睦の使者), and Laughing Flower (笑う花) come at the end, followed by the Tickets, Gods, and Token at 1106–1112. So this looks like a leftover of the Japanese build's name-sort order, with gaps where JP cards are missing from EDS.

> [!question] Is the JP sort key used?
> Is it still used anywhere (a trunk "sort" option?) or is it dead data? Search for `0x08623DF4` loads followed by `+0x1000`.

## Method
- The lookup code at `0x08000FC0` shows the three branches, the mask `0x7FF`, the `+1` for numbers ≥ 2000, and the `(result)*0x40 + 0x0822C720` name access.
- A script checked that `B[A[id]] == id` for every non-alternate ID and that the inverse table has no inconsistencies (`tools/extract_cards.py --verify` repeats this).
- `CardDetail_DrawInfo` compares ID-to-number results against 812, 730, and 1910–1912. The code at `0x080541D4` subtracts 1514 and tests `<= 5`. Both show game logic keying off card numbers.

Related: [[card-table]], [[card-name-table]], [[cards]], [[deck-lists]].
