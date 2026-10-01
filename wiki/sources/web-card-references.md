---
title: "Source: Public card references (web)"
type: source
status: draft
confidence: medium
sources: []
updated: 2026-10-01
---
# Source: Public card references (web)

- Raw file: none. These are web lookups made on 2026-09-29 to cross-check ROM-derived card data.
- Use: only for publicly documented facts (card counts and passwords). Every structural claim on the card pages comes from [[rom-analysis]].

## Findings
- **Passcodes.** Yugipedia's passcode article (https://yugipedia.com/wiki/Passcode), found by searching for "80906030", lists both 80906030 and 89631139 for Blue-Eyes White Dragon. That supports reading the 9 duplicate-name cards in the ROM as alternate-print versions with their own passwords. See [[password-table]] and [[card-id-map]].
- **Card count.** The Yu-Gi-Oh! Fandom wiki's EDS article (https://yugioh.fandom.com/wiki/Yu-Gi-Oh!_The_Eternal_Duelist_Soul, as summarised by search; the page itself returned HTTP 402) gives **819 cards**. That matches the ROM's 820 IDs minus the Insect Monster Token. See [[cards]].
- **Initial deck.** Yugipedia has an "Initial Deck (EDS)" page (https://yugipedia.com/wiki/Initial_Deck_(EDS)), but fetching it returned HTTP 403, so it isn't used yet. It could be used to cross-check the initial deck pools in [[deck-lists]].
- The well-known ATK/DEF/level/type/attribute values used for spot checks (Blue-Eyes, Dark Magician, Kuriboh, and others) are general card knowledge, consistent with [[general-knowledge]].

## Pages updated
[[card-table]], [[password-table]], [[card-id-map]], [[cards]].
