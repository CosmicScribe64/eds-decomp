---
title: Booster packs
type: data
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Booster packs

Pack data lives in two tables, a display/info table (ID, cover image, name) and a contents table (ID and rarity-slot card lists). Card lists hold **card numbers**, which [[card-id-map]] explains.

## Pack info at `0x080865DC`
| Field | Value |
|---|---|
| Address | `0x080865DC`–`0x08086C54` |
| Entry size | 0x48 |
| Count | 23 |
| Refs | 5, all in the `0x08063000`–`0x08064D60` shop/pack UI |

```c
struct PackInfo {           /* 0x48 bytes */
    u32 id;                 /* 0x00 */
    const u8 *image;        /* 0x04: cover art, 8bpp tiles, 0x1880 bytes (98 tiles) each */
    char name[0x40];        /* 0x08 */
};
```
| ID | Name | ID | Name |
|---|---|---|---|
| 1–7 | Vol.1 … Vol.7 | 501 | Expert Pack 1 |
| 11 | LOBEWD | 502 | Expert Pack 2 |
| 12 | Phantom of G | 503 | Expert Pack 3 |
| 21 | MagicRuler | 504 | Duelist Pack |
| 22 | Pharao'sSurvant | 505 | The Final Duelist |
| 23 | CursrOfAnubis | 506 | Rare Selections |
| 33 | Premium 3 | 507 | Expert Pack 4 |
| 41 | Celemony | 508 | Expert Pack 5 |
| | | 509 | Limited Collection |

The cover images run from `0x0864073C` (Vol.1) in steps of `0x1880`. The names are the original OCG set abbreviations, typos included ("Survant", "CursrOfAnubis", "Celemony").

**36 cover slots, 23 used.** The cover block holds 36 covers (`0x0864073C`–`0x0867793C`). Each is 56×112 px, 8bpp, 7 tiles wide, and is drawn with the shop BG palette `0x0863CC7C`. The 23 PackInfo entries use slots 0–8, 11–13, 17 and 21–30. Slots 9, 10, 14–16, 18–20 and 31–35 are covers that no table points to (gfx_banks converter, slot numbers re-checked against the PackInfo pointers on 2026-10-02). They may belong to packs 801/802/901–903 or to cut packs (hypothesis). They extract as PNGs with the rest of bank A ([[assets]]).

## Pack contents at `0x081A562C`
| Field | Value |
|---|---|
| Address | `0x081A562C`–`0x081A570C` |
| Entry size | 8: `{const struct PackSlots *p; u16 id; u16 pad}` |
| Count | 28 (the 23 above plus IDs 801, 802, 901, 902, 903, which have no info entry) |
| Data | slot lists and `PackSlots` structs, packed in `0x081A452C`–`0x081A562C` |

```c
struct PackSlots {                 /* 0x40 bytes */
    struct { const u16 *cards; s32 count; } slot[8];   /* slot 7 = common ... 0 = rarest */
};
```

Display order of all 28 IDs: `u16[28]` at `0x080819BE` (1–7, 41, 21, 11, 22, 12, 23, 33, 501–509, 801, 802, 901–903). A second list of 27 pack IDs, `u16[27]` at `0x081A5758`, is called `pack_unlock_ids` by the table converter (the role is a hypothesis).

**ROM layout (verified by an exact re-pack, 2026-10-02).** `0x081A452C`–`0x081A562C` holds, for each pack in table order, its non-empty slot lists, then 0 or 2 bytes of padding to a 4-byte boundary, then its `PackSlots`. An empty slot is `{NULL, 0}`. Packs 11, 12, 501, 502, 503, 505, 507, 802, 901 and 902 have the 2-byte pad. The contents extract to `tables/rodata2/booster_packs.json` ([[assets]]). Slot lists may change size if everything still fits before `0x081A562C`, but the number of packs is fixed at 28.

### Rarity roll (`RollPackRarity`)
- It draws `r = rand() % 180`, or `% 270` when buying the same pack again.
- It walks slots 0..6 using the cumulative thresholds `s32[8]` at `0x081A570C`, which hold `{1, 3, 6, 10, 16, 28, 64, 180}`, and takes the first *non-empty* slot with `r < thresh[i]`. Otherwise it falls back to the highest non-empty slot (`GetPackCommonSlot`, normally 7 = commons).
- A pity counter at `0x02013D76` counts commons-only packs. Once it exceeds 5 (or 10 when re-buying the same pack), the roll becomes `rand() % 12`, which forces one of the rare slots. This is an approximate reading.

### Pack generation (`GeneratePackCards(u16 *out5, u16 packId)`)
- **Normal packs:** the output holds 1 random card from the rolled slot plus 4 cards taken in order from a shuffled copy of the common slot, skipping copies of the rolled card. The 5 cards are then shuffled (25 random swaps).
- **Pack IDs 0x66 (102), 0x67 (103), and 0x6E (110)** are special random packs that draw 5 distinct random card IDs from 0..820, excluding card numbers 1920–1999. Pack 0x66 accepts only Traps (type 21), 0x67 only Magic (type 22), and 0x6E anything. These IDs have no table entry, and their shop names haven't been found yet.

### Slot counts per pack (slots 0..7)
| ID | Name | Counts | ID | Name | Counts |
|---|---|---|---|---|---|
| 1 | Vol.1 | 0,0,0,0,2,3,5,30 | 501 | Expert Pack 1 | 0,0,0,1,1,1,4,26 |
| 2 | Vol.2 | 0,0,0,0,2,3,5,30 | 502 | Expert Pack 2 | 0,0,0,0,0,4,6,21 |
| 3 | Vol.3 | 0,0,0,1,2,3,5,39 | 503 | Expert Pack 3 | 0,0,0,0,0,0,5,38 |
| 4 | Vol.4 | 0,0,0,1,3,3,5,38 | 504 | Duelist Pack | 0,0,1,9,6,6,3,11 |
| 5 | Vol.5 | 0,0,1,1,3,3,5,37 | 505 | The Final Duelist | 0,0,1,10,5,2,0,15 |
| 6 | Vol.6 | 0,0,1,3,3,3,5,37 | 506 | Rare Selections | 0,0,0,16,17,1,0,16 |
| 7 | Vol.7 | 0,0,1,1,3,4,5,38 | 507 | Expert Pack 4 | 0,0,0,0,0,0,0,47 |
| 11 | LOBEWD | 0,0,0,1,4,6,11,39 | 508 | Expert Pack 5 | 0,0,0,0,0,0,0,42 |
| 12 | Phantom of G | 0,0,0,1,8,6,5,45 | 509 | Limited Collection | 0,0,1,6,6,7,6,14 |
| 21 | MagicRuler | 0,0,0,0,3,4,4,39 | 801 | ? | 0,0,3,21,17,15,10,82 |
| 22 | Pharao'sSurvant | 0,0,1,2,3,4,6,36 | 802 | ? | 0,0,1,10,7,5,3,25 |
| 23 | CursrOfAnubis | 0,0,1,1,3,4,5,38 | 901 | ? | 0,0,1,2,8,4,20,16 |
| 33 | Premium 3 | 0,0,0,0,0,0,0,10 | 902 | ? | 0,0,0,1,0,0,0,14 |
| 41 | Celemony | 0,0,0,0,0,0,0,16 | 903 | ? | 0,0,0,1,2,1,1,5 |

Example: Vol.1 slot 4 holds {Dark Magician, Gaia The Fierce Knight}, and slot 5 holds {Dark Hole, Fissure, Trap Hole}.

> [!question] Open
> - Which rarity (Common, Rare, Super, Ultra, …) does each slot 0–6 correspond to on screen?
> - What are packs 801/802/901–903 (tournament prizes? the Grandpa/"Duelist" reward sets?), and where are their names?
> - What are the names and prices of the special random packs 0x66/0x67/0x6E?

## Method
- The pack names were found by a string search for "Expert Pack". Stepping back and forth in 0x48 increments then located `0x080865DC`, which has 5 code references.
- The `image` pointers were checked and are 8bpp tile data 0x1880 apart. An earlier guess that they pointed to pack contents was wrong.
- `GeneratePackCards` loads `0x081A562C` and scans 28 8-byte entries for a matching `u16` ID at +4. Disassembling it and its helpers `RollPackRarity`, `GetPackCommonSlot`, and `PickPackSlotCard` gave the slot layout and the thresholds.
- Reproduce: `python3 tools/extract_cards.py packs`, or read `assets/tables/rodata2/booster_packs.json` after `make setup`.

Related: [[deck-lists]], [[card-id-map]], [[card-data-functions]].
