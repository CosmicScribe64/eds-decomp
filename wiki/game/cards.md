---
title: Cards (game-level overview)
type: game
status: draft
confidence: high
sources: [rom-analysis, web-card-references]
updated: 2026-10-02
---
# Cards in EDS

This page gives the big picture of how EDS represents cards. Addresses and layouts live on the linked data pages.

## Card identity
- There are 820 cards, with IDs 1–820. That's 819 collectable cards plus the Insect Monster Token (ID 820). The public figure is 819 cards; see [[web-card-references]].
- Two numberings (see [[card-id-map]]):
  - The **card ID** is the alphabetical index into the English tables. It's what code and duel state carry around, always masked `& 0x7FF`.
  - The **card number** is the Konami number (DM-series number − 1). Decks, packs, special lists, and hard-coded card checks use it. Alternate-art copies are numbered base + 2000.
- There are 9 alternate-art duplicates: Blue-Eyes White Dragon, Celtic Guardian, Dark Magician, Flame Swordsman, Gaia The Fierce Knight, Launcher Spider, Pendulum Machine, Thousand Dragon, and Tiger Axe. Each has its own ID, number, and (except Thousand Dragon) password.

## What a card is made of (per card ID)
| Data | Page |
|---|---|
| name (`char[0x40]`) | [[card-name-table]] |
| lore/effect text (`char[0x1E0]`) | [[card-descriptions]] |
| 72×80 6bpp art + 64-colour palette | [[card-art]] |
| packed stats `u32`: ATK/DEF (÷10), level, type, attribute, monster kind / spell subtype | [[card-table]] |
| card number | [[card-id-map]] |
| 8-digit password (BCD) | [[password-table]] |
| JP kana sort key (probably vestigial) | [[card-id-map]] |

Card effects are code, dispatched by card number:
- The handler table at `0x0819A9D4` (426 × 0x18 `{u16 number; u16 flags; fn resolve, check, prepare, chainA, chainB}`, found by the ROM-map pass; see [[rom-map]]) has 290 keys that are EDS card numbers. `flags` is 0 in every row. All 867 non-NULL pointers are Thumb entry points of known functions. The slot names come from the callers (hypothesis about their roles): `resolve` (+4) is read by [[duel-main-c]], [[effect-resolve5-c]] and [[duel-turn-end-c]]; `check` (+8) and `prepare` (+0xC) by [[effect-activation-c]]; `chainA`/`chainB` (+0x10/+0x14) by [[duel-setup-c]] and [[duel-turn-end-c]]. `FindCardEffect` finds a row by binary search over a hard-coded 0..0x1A9 range, so the table must stay sorted and exactly 426 rows long. It extracts to `tables/card_effect_handlers.json`, with function names ([[assets]]). That covers all 148 Magic cards, all 72 Traps, 69 effect monsters, and Relinquished. The other 136 keys (1211–1552) aren't EDS card numbers. They may be sub-effect/trigger IDs, or leftovers from the larger shared database (hypothesis). Checked by script against `0x08622AB4`.
- Elsewhere, code tests specific card numbers directly (`&gCardNumberToId[N]` literals, and constants like 812, 730, and 1910).
- The remaining 76 effect monsters have no key equal to their card number. They're probably handled through the 1211+ keys or inline checks (hypothesis).

## Classification
- 594 monsters: 382 normal, 145 effect, 52 fusion, 15 ritual.
- 148 Magic cards: 66 normal, 41 equip, 15 ritual, 13 field, 7 continuous, 6 quick-play.
- 72 Traps: 45 normal, 20 continuous, 5 counter, 2 equip.
- 3 Ticket cards (type 23): The Monarchy, Set Sail for the Kingdom, and Glory of the King's Hand. They have no text or password; possibly story/key items (hypothesis).
- 3 Divine cards (type 24): Obelisk, Slifer, and Ra. They have 0 ATK/DEF, no password, and are probably unplayable (hypothesis).
- 20 monster types plus Trap, Magic, Ticket, and Divine; 6 attributes. Enums are on [[card-table]].

## Getting cards
- **Initial deck**: one of 3 randomly generated 40-card decks. See [[deck-lists]].
- **Booster packs**: 28 pack definitions with 8 rarity slots, plus 3 special random packs. See [[booster-packs]].
- **Passwords**: 757 cards can be obtained by password. See [[password-table]].
- **Opponents**: 24 duelists, each with a fixed deck, and 5 with alternate decks. See [[duelist-table]] and [[deck-lists]].

## Tooling
`tools/extract_cards.py` (stdlib only) dumps all of the above as JSON/CSV from the baserom and self-checks with `--verify`.

Related: [[game-overview]], [[special-card-lists]], [[card-data-functions]].
