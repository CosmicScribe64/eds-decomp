---
title: Special card lists
type: data
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Special card lists

These are small fixed `u16` lists of **card numbers** (see [[card-id-map]]) used by game logic. The addresses and contents are verified. What each list is *for* is a hypothesis unless noted otherwise.

| Address | Count | Used at | Contents | Purpose |
|---|---|---|---|---|
| `0x0819D2FC` | 13 | `0x08057000` | Raigeki, Dark Hole, Change of Heart, Pot of Greed, Harpie's Feather Duster, Monster Reborn, Snatch Steal, Graceful Charity, Mirror Force, Magic Jammer, Seven Tools of the Bandit, Swords of Revealing Light, Heavy Storm | Used by the AI. The code loops `i = 0..12` and checks whether any card in a card-ID list at `0x0201D81C` has this number (hypothesis: an AI "power card" check). The contents resemble the TCG "Limited" list of that era (from general knowledge; unverified). |
| `0x0819D316` | 26 | `0x0805790C` | Change of Heart, Mirror Force, Raigeki, Monster Reborn, Snatch Steal, Jinzo, Dark Hole, Harpie's Feather Duster, Pot of Greed, Witch of the Black Forest, Sangan, Dimensional Warrior, Penguin Soldier, Man-Eater Bug, Magician of Faith, Wall of Illusion, Needle Worm, Hane-Hane, Royal Decree, Imperial Order, Magic Jammer, Seven Tools, Gemini Elf, Vorse Raider, Summoned Skull, Cyber-Tech Alligator | A second AI priority list (hypothesis). The next word, at `0x0819D34A`, is 0; the first deck list starts at `0x0819D34C`. |
| `0x08081A6C` | 60 | `HasEnoughRareCards`, `0x0801B6E0` | notable/rare cards: Blue-Eyes White Dragon, Flame Swordsman, the 5 Exodia pieces, Gaia, Harpie Lady Sisters, PUGM, Red-Eyes, … and the 3 Gods (numbers 1910–1912) at the end | `HasEnoughRareCards(n)` counts how many of these 60 the player owns (trunk word bits 0-9 at `0x02011C28 + id*4`) and returns whether that count is at least n. `0x0801B6D4` picks one at random (`rand % 60`). Possibly a progress or unlock check and a "wanted card" pick (hypothesis). |
| `0x08081A28` … `0x08081A62` | small | `0x0801B868`… | `u16` index tables 1..20 and permutations of them | not card numbers; menu/ordering data (unmapped) |
| `0x0819DD64` | 4 | `AiActivateMonsterEffects` | Time Wizard, Cannon Soldier, Relinquished, Barrel Dragon | cards the AI looks for in its spell/trap zones (`ai_scan_cards`; role from the converter's reading of the code) |
| `0x081A78B4` | 47 | `GetCardCopyLimit` (`GetCardCopyLimit`, [[collection-c]]) | `{u16 card; u16 limit}` pairs | **the Forbidden/Limited list** (verified, below) |
| `0x0819A7C8` | 52 + end | [[effect-fusion-c]] | `{result, a, b, pad}` card numbers, ended by 999 | **two-material fusion recipes**, e.g. Flame Swordsman = Flame Manipulator + Masaki the Legendary Swordsman |
| `0x0819A970` | 3 + end | [[effect-fusion-c]] | `{result, a, b, c}` card numbers, ended by 999 | **three-material fusions**: Blue-Eyes Ultimate Dragon (3 × Blue-Eyes White Dragon), Aqua Dragon, Man-eating Black Shark |

## Forbidden/Limited list (`0x081A78B4`)
`GetCardCopyLimit` looks a card's number up in these 47 pairs and returns its limit, or 3 when the number is not listed.
- **Limit 0 (10):** the three Tickets (1901–1903), the three Gods (1910–1912), Insect Monster Token (1920), and numbers 1921–1923, which have no EDS card.
- **Limit 1 (26):** the five Exodia pieces, Dark Hole, Raigeki, Sinister Serpent, Megamorph, Harpie's Feather Duster, Jinzo, Monster Reborn, Pot of Greed, Change of Heart, Mirror Force, Snatch Steal, Confiscation, The Forceful Sentry, Painful Choice, Call of the Haunted, Cyber Jar, Ceasefire, Imperial Order, and three numbers with no EDS card (1214, 1314, 1449).
- **Limit 2 (11):** Sangan, Swords of Revealing Light, Witch of the Black Forest, Bell of Destruction, Graceful Charity, Heavy Storm, Delinquent Duo, Backup Soldier, Nobleman of Crossout, Morphing Jar #2, Riryoku.

This is the game's own ban list. The 13-card AI list at `0x0819D2FC` overlaps it heavily but is not the same list: for example, Seven Tools of the Bandit and Magic Jammer are there but not limited.

## Editing
Every list on this page extracts to JSON with `[number, "name"]` entries ([[assets]]):
- the AI lists and the ban list go to `tables/pointer_tables/ai_*.json` and `tables/rodata2/*.json`;
- the 60-card list and the fusion lists go to the `tables.json` files of `rodata/strings_and_tables` and `tables/scene_scripts_and_lists`.

The build reads only the number. Each list keeps its entry count.

## Related player-state fact
The code around the 60-card list reads the player's **trunk** (card collection) at `0x02011C20 + 8 + id*4`, a `u32` per card ID whose bits 0-9 hold the owned count (verified from `HasEnoughRareCards`). The rest of the save/trunk layout is unmapped.

## Method
- Found the lists by scanning for `u16` runs of valid card numbers, then confirmed them from their code references (`ldr rX, =addr`).
- The loop bounds come from the disassembly: `cmp r6, #0xC` at `0x08057030` and `cmp r3, #0x3B` at `0x0801B6B4`.
- Reproduce: `python3 tools/extract_cards.py lists`.
- The ban list, the AI scan list and the fusion lists were decoded by the `tables_game` and `tables_code` converters (2026-10-02). The fusion terminators and the limit groups were re-checked against the ROM for this page.

Related: [[deck-lists]], [[card-id-map]], [[cards]].
