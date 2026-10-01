---
title: Card Table (stats)
type: data
status: solid
confidence: high
sources: [rom-analysis, web-card-references]
updated: 2026-10-01
---
# Card Table (stats) and the per-card data block

**Verified 2026-09-29.** Card stats are one packed little-endian `u32` per card at `0x08621DE0` (`ROM+0x621DE0`). The table is indexed by **card ID**, the 1-based alphabetical index, with slot 0 empty. See [[card-id-map]].

| Field | Value |
|---|---|
| Address | `0x08621DE0`–`0x08622AB4` (`ROM+0x621DE0`) |
| Entry size | 4 bytes (`u32`, bitfield) |
| Count | 821 (IDs 0..820; ID 0 = `0x00000000`) |
| Index | card ID, masked with `& 0x7FF` by every reader |
| Code references | 499 literal-pool copies of `0x08621DE0` in `0x08000000`–`0x08080A24` |

## Layout
```c
/* 0x08621DE0: const u32 gCardStats[821]  (name is a proposal) */
/* Field positions verified against the ROM; the bitfield syntax is a guess at the source. */
struct CardStats {
    u32 def   : 9;  /* bits 0-8   DEF / 10                       (monsters) */
    u32 atk   : 9;  /* bits 9-17  ATK / 10                       (monsters) */
    u32 kind  : 2;  /* bits 18-19 0 normal, 1 effect, 2 fusion, 3 ritual (monsters) */
    u32 type  : 5;  /* bits 20-24 see enum below                 (all cards) */
    u32 level : 4;  /* bits 25-28 stars                          (monsters) */
    u32 attr  : 3;  /* bits 29-31 1 LIGHT .. 6 WIND               (monsters) */
};
/* Magic (type 22) and Trap (type 21): bits 0-16 are 0 and level is 0, and
   bits 17-19 hold the spell/trap subtype (mask 0xE0000). This overlaps atk's MSB and kind. */
```

The masks and shifts the code applies to words loaded from this table (counted in a linear disassembly):

| Field | Code idiom | Occurrences |
|---|---|---|
| type | `movs #0xF8; lsls #0x11` (mask `0x1F00000`), then `lsrs #0x14` | ~346 |
| level | mask `0x1E000000`, then `lsrs #0x19` | ~56 |
| atk | `lsls #0xE; lsrs #0x17` | ~41 |
| kind | mask `0xC0000`, then `lsrs #0x12` | ~40 |
| attr | `lsrs #0x1D` | ~14 |
| spell/trap subtype | mask `0xE0000` | 1 |

### Enums
**type** (bits 20-24). The ROM's own name array sits at `0x081988D0`: `const char *[25]`, where index 0 is `""`.

| # | Type | # | Type | # | Type |
|---|---|---|---|---|---|
| 1 | Dragon | 9 | Dinosaur | 17 | Fairy |
| 2 | Zombie | 10 | Insect | 18 | Spellcaster |
| 3 | Fiend | 11 | Beast | 19 | Thunder |
| 4 | Pyro | 12 | Beast-Warrior | 20 | Reptile |
| 5 | Sea Serpent | 13 | Plant | 21 | **Trap** |
| 6 | Rock | 14 | Aqua | 22 | **Magic** |
| 7 | Machine | 15 | Warrior | 23 | **Ticket** (3 cards) |
| 8 | Fish | 16 | Winged Beast | 24 | **Divine** (3 Egyptian Gods) |

**attr** (bits 29-31, monsters): 1 LIGHT, 2 DARK, 3 WATER, 4 FIRE, 5 EARTH, 6 WIND. The name array at `0x0819D264` holds these six strings in that order (index = attr - 1). Non-monsters carry fixed raw values here: Magic 0, Trap 1, Divine 2, Ticket 0. Treat those as card-class markers, not as attributes (hypothesis).

**kind** (bits 18-19, monsters): 0 normal, 1 effect, 2 fusion, 3 ritual. The card-info screen `sub_08005A70` appends `/Effect`, `/Fusion`, or `/Ritual`. It special-cases a few monsters by **card number** (see [[card-id-map]]):
- Card number 812, *Alligator's Sword Dragon*, displays as `/Fusion/Effect`. The code also checks numbers 1241, 1334, and 1526, which don't exist in EDS (they're probably left over from a larger shared card database).
- Card number 730, *Relinquished*, displays as `/Ritual/Effect`.
- Card numbers 1910–1912 (the Gods) take special paths.

**subtype** (bits 17-19, Magic/Trap). The suffix array at `0x08198934` is `{NULL, "/Counter", "/Field", "/Equip", "/Continuous", "/Quick", "/Ritual", NULL}`.

| Value | Subtype | Magic count | Trap count |
|---|---|---|---|
| 0 | Normal | 66 | 45 |
| 1 | Counter | – | 5 |
| 2 | Field | 13 | – |
| 3 | Equip | 41 | 2 (Kunai with Chain, Metalmorph) |
| 4 | Continuous | 7 | 20 |
| 5 | Quick-Play | 6 | – |
| 6 | Ritual | 15 | – |

### Examples
| ID | Card | Raw | Decoded |
|---|---|---|---|
| 82 | Blue-Eyes White Dragon | `0x301258FA` | 3000/2500, L8, Dragon, LIGHT, normal |
| 150 | Dark Magician | `0x4F21F4D2` | 2500/2100, L7, Spellcaster, DARK, normal |
| 391 | Kuriboh | `0x42343C14` | 300/200, L1, Fiend, DARK, effect |
| 235 | Flame Swordsman | `0x8AF968A0` | 1800/1600, L5, Warrior, FIRE, fusion |
| 147 | Dark Hole | `0x01600000` | Magic, Normal |
| 476 | Mirror Force | `0x21500000` | Trap, Normal (attr field = 1) |
| 817 | Obelisk the Tormentor | `0x41800000` | Divine, 0/0, L0. Hypothesis: the Gods are collect-only; the string "This is not able to play." sits at `0x0808158C` |
| 820 | Insect Monster Token | `0xA2A0140A` | 100/100, L1, Insect, EARTH |

### Totals (all 820 cards)
- 594 monsters: 382 normal, 145 effect, 52 fusion, 15 ritual.
- 148 Magic, 72 Trap, 3 Ticket, 3 Divine.
- Level range 1–12 (Blue-Eyes Ultimate Dragon is 12). Maximum ATK is 4500.

## The per-card data block
All per-card tables form one contiguous block, and each is indexed by card ID with an empty slot 0. Because the boundaries line up exactly, the entry count of 821 is confirmed several times over.

| Address | Table | Entry | Page |
|---|---|---|---|
| `0x0822C720` | English names | `char[0x40]` | [[card-name-table]] |
| `0x08239460` | second name bank, **all zero** (unused) | `char[0x40]` | [[card-name-table]] |
| `0x082461A0` | descriptions | `char[0x1E0]` | [[card-descriptions]] |
| `0x082A6500` | card art, 6bpp packed, 72×80 px | `u8[0x10E0]` | [[card-art]] |
| `0x08608360` | card art palettes, 64 colours | `u16[64]` | [[card-art]] |
| `0x08621DE0` | **stats (this page)** | `u32` | |
| `0x08622AB4` | ID to card number | `u16` (+2 pad) | [[card-id-map]] |
| `0x08623120` | passwords (BCD) | `u8[4]` | [[password-table]] |
| `0x08623DF4` | card number to ID | `u16[2048]` | [[card-id-map]] |
| `0x08624DF4` | JP (kana) sort key (hypothesis) | `u16` (+2 pad) | [[card-id-map]] |
| `0x08625460` | (graphics follow) | | |

## Used by (sample)
- `sub_08005A70` (`0x08005A70`–`0x080063D0`): card info/detail text. It reads the name (`0x0822C720 + id*0x40`), the description (`0x082461A0 + id*0x1E0`), the stats, the type/suffix name arrays, and the ID-to-number table.
- `sub_0800ABC8`: duel code. It reads the card ID from the low 12 bits of a zone word in per-player duel state at `0x0201930C + (player&1)*0xD64 + slot*0x94`, then extracts type and attr into an output struct. The duel-state layout has not been mapped yet. See [[card-data-functions]].
- `sub_08062AF4`: pack generation. Its random Magic/Trap packs filter on `type`. See [[booster-packs]].

## Method
1. **Code path.** Literal pools in the code hold `0x0822C720` (names − 0x40) 58 times and `0x082461A0` (descriptions − 0x1E0) once. The function that uses them (`sub_08005A70`) also loads `0x7FF` and `0x08621DE0`, then indexes `(id & 0x7FF) * 4`.
2. **Correlation.** The low 9 bits of Blue-Eyes White Dragon's word are `0xFA` = 250 (DEF/10), and bits 9-17 are `0x12C` = 300 (ATK/10). Checking the other fields against about 30 well-known cards pinned down level, type, attribute, and kind.
3. **Script.** `python3 tools/extract_cards.py --verify` checks 13 monsters (ATK, DEF, level, type, attribute, kind, and 4 passwords), 8 spell/trap subtypes, and the ID-to-number round trip for all 820 IDs. All pass.
4. **Sanity.** All 594 monsters have level 1-12 and attr 1-6. Every Magic/Trap word has bits 0-16 zero and level 0.

> [!question] Open
> - How is "has an effect" known for fusions and rituals other than the two special-cased by number? Is it a separate effect table keyed by card number (hypothesis)?
> - Do the non-monster attr values (Trap = 1, Divine = 2) have a meaning in code, for example as an icon index?

Related: [[card-name-table]], [[card-id-map]], [[cards]], [[game-overview]].
