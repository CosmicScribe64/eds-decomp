---
title: Yu-Gi-Oh! The Eternal Duelist Soul
type: game
status: draft
confidence: medium
sources: [general-knowledge, rom-analysis]
updated: 2026-10-02
---
# Yu-Gi-Oh! The Eternal Duelist Soul (EDS)

- Platform: Game Boy Advance. Publisher: Konami (maker code `A4`, see [[rom-header]]).
- A card game that follows the official OCG/TCG rules of its time fairly closely: Life Point duels, a Main Phase and a Battle Phase, and Normal Summons with Tributes.
- Released in 2002. The Japanese release is *Yu-Gi-Oh! Duel Monsters 5 Expert 1*. It was built with the same compiler and shares the SDK and system code, but its duel engine was reworked: a different card and player data model, 928 cards, and only about 200 functions that are the same apart from addresses and constants ([[rom-versions]], [[jpmap]]).

> [!warning] Contradiction
> Until 2026-10-02 this page said JP "was built from the same source with a different link order and shifted data layouts", from the 2026-10-01 byte comparison. The full function mapping ([[jpmap]], `build/jp/PLAN.md` §6) shows the game logic itself differs; see [[rom-versions]]. Resolved in favour of the mapping.

## What the ROM has shown
- The game has 821 card IDs (0 to 820). The per-card tables start at `0x0822C720` ([[card-table]], [[cards]]).
- Sound uses a custom Konami driver, not m4a/MP2K ([[sound-engine]], [[sound-driver]]).
- Text is English-only ASCII on top of the Japanese engine ([[text-system]]).

## Major systems
- [[duel-engine]]: duel state, player and zone layout, command runner and screen state. Turn structure and AI are still being mapped.
- [[card-table]]: card stats and types. Card effects dispatch through a table keyed by card number ([[cards]]).
- Menus, the deck editor, the password system ([[password-table]]) and link-cable trading.

Open questions are collected in [[open-questions]].
