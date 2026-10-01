---
title: Special card lists
type: data
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Special card lists

These are small fixed `u16` lists of **card numbers** (see [[card-id-map]]) used by game logic. The addresses and contents are verified. What each list is *for* is a hypothesis unless noted otherwise.

| Address | Count | Used at | Contents | Purpose |
|---|---|---|---|---|
| `0x0819D2FC` | 13 | `0x08057000` | Raigeki, Dark Hole, Change of Heart, Pot of Greed, Harpie's Feather Duster, Monster Reborn, Snatch Steal, Graceful Charity, Mirror Force, Magic Jammer, Seven Tools of the Bandit, Swords of Revealing Light, Heavy Storm | Used by the AI. The code loops `i = 0..12` and checks whether any card in a card-ID list at `0x0201D81C` has this number (hypothesis: an AI "power card" check). The contents resemble the TCG "Limited" list of that era (from general knowledge; unverified). |
| `0x0819D316` | 26 | `0x0805790C` | Change of Heart, Mirror Force, Raigeki, Monster Reborn, Snatch Steal, Jinzo, Dark Hole, Harpie's Feather Duster, Pot of Greed, Witch of the Black Forest, Sangan, Dimensional Warrior, Penguin Soldier, Man-Eater Bug, Magician of Faith, Wall of Illusion, Needle Worm, Hane-Hane, Royal Decree, Imperial Order, Magic Jammer, Seven Tools, Gemini Elf, Vorse Raider, Summoned Skull, Cyber-Tech Alligator | A second AI priority list (hypothesis). The next word, at `0x0819D34A`, is 0; the first deck list starts at `0x0819D34C`. |
| `0x08081A6C` | 60 | `sub_0801B640`, `0x0801B6E0` | notable/rare cards: Blue-Eyes White Dragon, Flame Swordsman, the 5 Exodia pieces, Gaia, Harpie Lady Sisters, PUGM, Red-Eyes, … and the 3 Gods (numbers 1910–1912) at the end | `sub_0801B640(n)` counts how many of these 60 the player owns (trunk word bits 0-9 at `0x02011C28 + id*4`) and returns whether that count is at least n. `0x0801B6D4` picks one at random (`rand % 60`). Possibly a progress or unlock check and a "wanted card" pick (hypothesis). |
| `0x08081A28` … `0x08081A62` | small | `0x0801B868`… | `u16` index tables 1..20 and permutations of them | not card numbers; menu/ordering data (unmapped) |

## Related player-state fact
The code around the 60-card list reads the player's **trunk** (card collection) at `0x02011C20 + 8 + id*4`, a `u32` per card ID whose bits 0-9 hold the owned count (verified from `sub_0801B640`). The rest of the save/trunk layout is unmapped.

## Method
- Found the lists by scanning for `u16` runs of valid card numbers, then confirmed them from their code references (`ldr rX, =addr`).
- The loop bounds come from the disassembly: `cmp r6, #0xC` at `0x08057030` and `cmp r3, #0x3B` at `0x0801B6B4`.
- Reproduce: `python3 tools/extract_cards.py lists`.

Related: [[deck-lists]], [[card-id-map]], [[cards]].
