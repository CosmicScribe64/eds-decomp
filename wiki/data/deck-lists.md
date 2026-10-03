---
title: Deck Lists (opponent decks and initial-deck pools)
type: data
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Deck Lists

Every deck is stored as a **sorted `u16[]` of card numbers** (not card IDs; see [[card-id-map]]), with a separate `{pointer, count}` header table. The card lists are packed back-to-back in `0x0819D34C`–`0x0819DC6C`, with no terminators.

```c
struct DeckRef {                 /* 8 bytes; `struct DeckList` in src/ai_deck.c */
    const u16 *cards;            /* card numbers, ascending */
    u16 count;                   /* 40..42 */
    u16 pad;                     /* always 0 */
};
```

> [!warning] Contradiction
> This page declared `u32 count`. The matched source `src/ai_deck.c` declares `struct DeckList { const u16 *ids; u16 n; u16 pad; }`, and the deck converter writes that layout back byte-identical. A ROM check on 2026-10-02 found every pad halfword zero in both header tables, which is why the old `u32` reading gave the same counts. Resolved in favour of the matched source.

**Editing.** The deck lists extract to `tables/deck_lists.json` ([[assets]]), with one `[number, "name"]` list per deck. The build re-packs the lists back to back in ROM order and rewrites both header tables. Decks may grow or shrink as long as all 31 lists fit in the 0x920-byte region (1,168 cards, all used by the original data). The number of header entries (25 and 6) is fixed. The ROM lists are sorted, but the game doesn't seem to need that, and the build does not sort.

## Opponent decks at `0x0819DC6C`
| Field | Value |
|---|---|
| Address | `0x0819DC6C`–`0x0819DD34` |
| Entry | `struct DeckRef` (8 bytes) |
| Count | 25; index = duelist ID (0 = `{NULL, 0}`) |
| Refs | code at `0x08059308` |

Each deck's index matches the duelist's ID in the [[duelist-table]]:

| # | Duelist | Cards | Theme / signature cards |
|---|---|---|---|
| 1 | Yugi Muto | 40 | Exodia pieces, Dark Magician, Celtic Guardian, Buster Blader |
| 2 | Tea Gardner | 40 | fairies and elves (Dancing Elf, Gemini Elf, Petit Angel) |
| 3 | Joey Wheeler | 41 | Red-Eyes B. Dragon, Alligator's Sword, Baby Dragon, Polymerization |
| 4 | Tristan Taylor | 40 | warriors (M-Warrior #1/#2, Masaki, Swordstalker) |
| 5 | Bakura Ryou | 40 | zombies and fiends (Dark Elf ×3, Dokurorider) |
| 6 | Rex Raptor | 40 | dinosaurs (Two-Headed King Rex ×3) |
| 7 | Espa Roba | 40 | machines (Jinzo, Machine King, Cyber Falcon) |
| 8 | Weevil Underwood | 40 | insects (Insect Queen, Parasite Paracide) |
| 9 | Mako Tsunami | 40 | water (Fortress Whale, 7 Colored Fish, Umi) |
| 10 | Mai Valentine | 41 | Harpie Lady ×3, Harpie's Pet Dragon, Rising Air Current |
| 11 | Rare Hunter | 40 | Exodia |
| 12 | Arkana | 40 | Dark Magician ×3, Dark Magician Girl |
| 13 | Strings | 40 | Morphing Jar ×3, Tremendous Fire |
| 14 | Umbra & Lumis | 40 | Millennium Shield, Labyrinth Wall, Millennium Golem |
| 15 | Marik Ishtar | 40 | White Magical Hat, Dream Clown |
| 16 | Kaiba Seto | 42 | Blue-Eyes White Dragon ×3, Blue-Eyes Ultimate Dragon ×2 |
| 17 | Ishizu Ishtar | 40 | Shining Fairy, Banisher of the Light, Luminous Spark |
| 18 | Shadi | 40 | Orion the Battle King, La Jinn, Lord of Zemia |
| 19 | Yami Bakura | 41 | Summoned Skull, Masked Sorcerer, Dimensional Warrior |
| 20 | Yami Yugi | 41 | Magnet Warriors and Valkyrion, Dark Magician Girl ×2 |
| 21 | Duel Computer | 40 | Mask of Darkness, Gravekeeper's Servant, Morphing Jar |
| 22 | Simon | 40 | Total Defense Shogun, Muka Muka |
| 23 | Maximillion Pegasus | 41 | Toons (Toon World, Blue-Eyes Toon Dragon, Relinquished) |
| 24 | Trusdale (Grandpa) | 40 | Exodia, Mystical Elf ×3 |

## Alternate decks at `0x0819DD34`
| Field | Value |
|---|---|
| Address | `0x0819DD34`–`0x0819DD64` |
| Entry | `struct DeckRef` |
| Count | 6; index = duelist ID 1..5 (0 = `{NULL, 0}`) |
| Refs | code at `0x080592D0` |

These are second, stronger decks for **Yugi, Tea, Joey, Tristan, and Bakura** (40, 40, 41, 40, and 40 cards). They add staples such as Raigeki, Mirror Force, Change of Heart, and Magnet Warriors. Their lists are interleaved in ROM with the main decks of the same duelists (main 1, alt 1, main 2, alt 2, …).

> [!question] When are the alternate decks used?
> Perhaps after the player reaches some progress level, or for rematches (hypothesis).

## Initial ("starter") deck pools at `0x08198744`
At the start of a game the player picks one of three face-down decks (dialogue: "You have 3 choices…"). Each deck is **generated randomly** from 11 card pools.

| Field | Value |
|---|---|
| Address | `0x08198744`–`0x0819879C` |
| Entry | 8 bytes: `const u16 *pool; u32 packed` |
| Count | 11 groups; the pools themselves sit at `0x08198634`–`0x08198744` (136 card numbers) |
| Builder | `BuildStarterDeck` (`0x0800495C`–`0x08004ABC`) |

```c
struct StarterPool {
    const u16 *pool;          /* card numbers */
    u32 poolSize : 10;        /* bits 0-9  */
    u32 pickA    : 5;         /* bits 10-14 cards taken for choice 0 */
    u32 pickB    : 5;         /* bits 15-19 choice 1 */
    u32 pickC    : 5;         /* bits 20-24 choice 2 */
    u32 flag     : 7;         /* bits 25-31: 1 on groups 0 and 4, 0 elsewhere (meaning unknown) */
};
```
The builder copies the pool into a stack buffer, shuffles it (`poolSize*4` random swaps), and takes the first `pick[choice]` cards, where `choice = arg % 3`. **Each choice sums to exactly 40 cards**, which confirms the field split. Groups 0–4 and 8–10 use the same pick counts for all three choices. The choices differ only in which level-4 monster pool (groups 5–7) contributes 6 cards instead of 3.

| Group | Pool size | Picks (A/B/C) | Contents (summary) |
|---|---|---|---|
| 0 | 11 | 11/11/11 | fixed spells/traps (Black Pendant, SoRL, Monster Reborn, MST ×2, Pot of Greed, Change of Heart, Trap Hole ×2, Magic Jammer, Seven Tools) |
| 1 | 2 | 1/1/1 | Dark Hole or Raigeki |
| 2 | 3 | 1/1/1 | Megamorph / Snatch Steal / Premature Burial |
| 3 | 3 | 1/1/1 | Bell of Destruction / Mirror Force / Call of the Haunted |
| 4 | 2 | 2/2/2 | Mystical Elf, Summoned Skull |
| 5 | 13 | 3/6/3 | level-4 normal monsters, set A |
| 6 | 13 | 3/3/6 | level-4 normal monsters, set B |
| 7 | 13 | 6/3/3 | level-4 normal monsters, set C |
| 8 | 57 | 9/9/9 | level 1–3 normal monsters |
| 9 | 12 | 2/2/2 | level 5–6 normal monsters (Battle Steer, Curse of Dragon, …) |
| 10 | 7 | 1/1/1 | level 1–4 effect monsters (Sangan, Magician of Faith, Man-Eater Bug, …) |

## Method
- Scanned the ROM for runs of ≥20 ascending `u16` values that are all valid card numbers (the value set of `0x08622AB4`). That found 30 back-to-back runs at `0x0819D34C`–`0x0819DC6C`.
- Searched for pointers into that range. They all come from the two header tables above, and each header's count field (40–42) matches the run lengths.
- Deck themes were checked by eye against the duelists' well-known anime/manga decks: Kaiba's Blue-Eyes, Mai's Harpies, Weevil's insects, Pegasus's Toons, and so on.
- The starter pools were found through code references (the reference at `0x080049F8` points to `0x08198744`), and the three 40-card sums confirmed the field split.
- Reproduce: `python3 tools/extract_cards.py decks` and `python3 tools/extract_cards.py starter`, or read `assets/tables/deck_lists.json` and the `starter deck pool` items in `assets/tables/scene_scripts_and_lists/tables.json` after `make setup`.
- The header layout and the pack order were confirmed on 2026-10-02 by the exact re-pack in `tools/assetfmt/tables_game.py`.

Related: [[duelist-table]], [[booster-packs]], [[special-card-lists]], [[card-id-map]].
