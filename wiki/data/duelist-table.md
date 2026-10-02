---
title: Duelist table
type: data
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Duelist table

**Verified 2026-09-29.** This table holds each opponent's (and helper character's) ID, full name, and short name.

| Field | Value |
|---|---|
| Address | `0x08139F64`–`0x0813ADD4` (`ROM+0x139F64`) |
| Entry size | 0x84 |
| Count | 28 (index 0 is a placeholder with names `" "`) |
| Refs | `0x08139F64` (from `0x08000232`) and `0x08139F68` (from `0x08000264`). The next object, at `0x0813ADD4`, is referenced separately. The preceding `0x08139F5C` is a different object (two pointers to `0x0874C650`) with its own 3 references. |

```c
struct Duelist {            /* size 0x84 */
    u32  id;                /* 0x00: duelist id (matches deck-table index for 1..24) */
    char name[0x40];        /* 0x04: full name, e.g. "Yugi Muto" */
    char shortName[0x40];   /* 0x44: short name, e.g. "Yugi" */
};
const struct Duelist gDuelists[28];   /* proposal */
```

## Entries
| idx | id | Name | Short name | idx | id | Name | Short name |
|---|---|---|---|---|---|---|---|
| 1 | 1 | Yugi Muto | Yugi | 15 | 15 | Marik Ishtar | Marik |
| 2 | 2 | Tea Gardner | Tea | 16 | 16 | Kaiba Seto | Seto |
| 3 | 3 | Joey Wheeler | Joey | 17 | 17 | Ishizu Ishtar | Ishizu |
| 4 | 4 | Tristan Taylor | Tristan | 18 | 18 | Shadi | Shadi |
| 5 | 5 | Bakura Ryou | Ryou | 19 | 19 | Yami Bakura | Bakura |
| 6 | 6 | Rex Raptor | Rex | 20 | 20 | Yami Yugi | Yami Yugi |
| 7 | 7 | Espa Roba | Roba | 21 | 21 | Duel Computer | Duel Computer |
| 8 | 8 | Weevil Underwood | Weevil | 22 | 22 | Simon | Simon |
| 9 | 9 | Mako Tsunami | Mako | 23 | 23 | Maximillion Pegasus | Pegasus |
| 10 | 10 | Mai Valentine | Mai | 24 | 24 | Trusdale | Grandpa |
| 11 | 11 | Rare Hunter | R.Hunter | 25 | 38 | Umbra | Umbra |
| 12 | 12 | Arkana | Arkana | 26 | 39 | Lumis | Lumis |
| 13 | 13 | Strings | Strings | 27 | 40 | Ghouls | Ghouls |
| 14 | 14 | Umbra & Lumis | Umbra & Lumis | | | | |

IDs 1–24 are the opponents. Their decks sit in the main deck table at the same index; see [[deck-lists]]. IDs 38–40 (Umbra, Lumis, and "Ghouls") have no deck entry. They're probably used for story dialogue or the tag duel (hypothesis).

## Related data
- A separate string pool at `0x08081260`–`0x080813A4` lists the same 24 full names in **reverse** order ("Duel Computer", "Trusdale", "Maximillion Pegasus", … "Yugi Muto"), followed by "Draw", "Lose", "Win", "[ Unknown ]". It's probably a `const char *[]` for a records/statistics screen (hypothesis).
- `0x0808180E`–`0x0808198C`: 24 groups of 7 `u16`s built around `duelistId*1000` (1014, 1013, 1012, 1011, 1010, 1004, 1017, then 2014, …). These may be per-duelist script/portrait/text IDs (hypothesis).
- Story dialogue with `$`-codes (for example `$r5`, `$c`, `$p`) starts right after this table.
- **Portraits.** `GetBustupSet` (`sub_08001C78`) maps a character ID to its dialogue scene set. Every opponent 1–24 and Umbra/Lumis (38, 39) has one; Yugi's set 6 is also the default. The IDs 32–35 and 37, which are not in this table, select the five background scenes 0–4. Full table: [[scene-sets]].
- **Duel BGM.** `0x08198F20` maps each opponent ID to a song (`{duelist; u16 bgm}`, read by [[code-0801e260]]). See [[sound-engine]].
- The table extracts to `text/duelists.json` ([[assets]]).

## Method
- A string search for "Yugi Muto" found the two copies at `ROM+0x139FEC` and `ROM+0x13A02C`.
- The `u32` 1 at `ROM+0x139FE8` and the next `u32` 2 at `ROM+0x13A06C` gave the 0x84 stride.
- Walking backwards found the placeholder entry 0 at `0x08139F64`. Code references `0x08139F64` and `0x08139F68`, which is `&gDuelists[0].name`.
- Walking forwards stopped at index 28, where dialogue text starts. `0x08139F64 + 28*0x84 = 0x0813ADD4` is a separately referenced symbol.
- Reproduce: `python3 tools/extract_cards.py duelists`.

Related: [[deck-lists]], [[game-overview]].
