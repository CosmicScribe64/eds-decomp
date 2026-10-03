---
title: Function pointer tables
type: data
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Function pointer tables

`tools/fnptr_tables.py` scans the ROM data after the code (from `0x08080A20`) for runs of at least 3 words that
are known function addresses (+1 for Thumb), with single NULL gaps allowed. It also lists the functions that load
each table's address from a literal pool. The full list is in `config/fnptr_tables.tsv`: 88 tables with 636 entries
(2026-09-30). The idea comes from the khcom decomp's `function_pointer_evidence.py`.

Functions in one table usually share a signature and a role (state-machine steps, per-card handlers, menu
callbacks), so a matched member is a good model for the others (see also `tools/similar.py`).

## Largest tables (verified as pointer runs; roles are hypotheses)
| Table | Entries | Indexed by | First members |
|---|---|---|---|
| `0x081A723C` | 77 | `DeckEdit_RunListFilter`, `ProhibitCardSelect_RunListFilter` | `ListFilter_Reset`, `ListFilter_Init`, `ListFilter_Update` |
| `0x08198E7C` | 40 | `CB_LinkBattle` | `LinkBattle_Init`, `LinkBattle_DecideTurnOrder`, `LinkBattle_Connect` |
| `0x0819A6B0` | 39 | `TurnOrder_RpsMain` | `TurnOrder_ChooseHand`, `TurnOrder_ShowResult`, `TurnOrder_ChooseTurn` |
| `0x081A7970` | 29 | `CB_Password` | `Password_InitVideo`, `Password_InitState`, `Password_FadeIn` |
| `0x08199A2C` | 22 | (no literal-pool reference found) | `DiceScreen_HoldDie`, `DiceScreen_ThrowDie` |
| `0x08198F80` | 14 | `DuelMainStep` | `DuelPhase_Init`, `DuelPhase_Opening`, `DuelPhase_TurnStart` |
| `0x0819D1D8` | 14 | `BattlePhase_Run` | `BattleStage_Start`, `BattleStage_SelectAttacker`, `BattleStage_SelectTarget` |

> [!warning] Contradiction
> Three of the "largest tables" above are runs that span several tables. The scanner allows single NULL gaps, so it joins NULL-terminated step tables that sit back to back. The asset converters, which split the data at code labels (2026-10-02), give these boundaries:
> - **`0x081A723C` (77)** is six step tables ending at `0x081A7374`: `transfer_steps` (`0x081A723C`, the one `DeckEdit_RunListFilter`/`ProhibitCardSelect_RunListFilter` load), `statistics_steps` (`0x081A724C`, `DeckEdit_RunStatistics`), `deck_edit_steps` (`0x081A725C`, `CB_DeckEdit`), `deck_edit_select_steps` (`0x081A72A0`, `SideDeckSwap_Run`), `deck_edit_sub_steps` (`0x081A72E4`, `TradeCardSelect_Run`) and `deck_edit_popup_steps` (`0x081A7330`, `ProhibitCardSelect_Run`).
> - **`0x081A7970` (29)** is `password_steps` (`0x081A7970`, `CB_Password`) followed by `card_trading_steps` (`0x081A79A4`, `CB_CardTrading`).
> - **`0x08199A2C` (22, "no literal-pool reference")** starts one word into `gPlainDieScreenSteps`, whose first entry is NULL, and continues into `gSkullDiceSceneSteps` and further tables of [[coin-toss-scene-c]]. The references are to the labels, which is why the scanner found none for `0x08199A2C`.
>
> Resolved in favour of the label-split tables. The scanner's runs are still valid groupings of related functions.

## Named tables from the asset converters
The converters write every pointer table in the data area as JSON with function names ([[assets]]), so these files are now the full listing:
- `tables/rodata2/*_steps.json`, `tables/pointer_tables/duel_step_handlers.json` (`0x0819D1D8`) and `ai_turn_steps.json` (`0x0819DD6C`);
- `tables/dialogue_box_steps.json` (`0x0813ADD4`, the bust-up runner, [[scene-sets]]);
- about 40 step tables in `tables/scene_scripts_and_lists/tables.json` (type `fn`, each named after the unit that reads it);
- the card-effect handler table `tables/card_effect_handlers.json` (`0x0819A9D4`): 426 rows of five slots (`resolve`, `check`, `prepare`, `chain_a`, `chain_b`). All 867 non-NULL entries are Thumb entry points of known functions ([[cards]]).
