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
| `0x081A723C` | 77 | `sub_0806A92C`, `sub_0806A96C` | `sub_08069A90`, `sub_08069AE0`, `sub_08069FE4` |
| `0x08198E7C` | 40 | `sub_0801AD18` | `sub_0801A7F4`, `sub_0801A8A4`, `sub_0801A8CC` |
| `0x0819A6B0` | 39 | `sub_080297B4` | `sub_08028D9C`, `sub_08028FD0`, `sub_080292C8` |
| `0x081A7970` | 29 | `sub_0807CC28` | `sub_0807C374`, `sub_0807C46C`, `sub_0807C4A8` |
| `0x08199A2C` | 22 | (no literal-pool reference found) | `sub_08025DF0`, `sub_080256D8` |
| `0x08198F80` | 14 | `sub_08021A48` | `sub_08021834`, `sub_0801F97C`, `sub_0804F168` |
| `0x0819D1D8` | 14 | `sub_0804E31C` | `sub_0804AB90`, `sub_0804AC18`, `sub_0804AE64` |

> [!warning] Contradiction
> Three of the "largest tables" above are runs that span several tables. The scanner allows single NULL gaps, so it joins NULL-terminated step tables that sit back to back. The asset converters, which split the data at code labels (2026-10-02), give these boundaries:
> - **`0x081A723C` (77)** is six step tables ending at `0x081A7374`: `transfer_steps` (`0x081A723C`, the one `sub_0806A92C`/`sub_0806A96C` load), `statistics_steps` (`0x081A724C`, `sub_0806D198`), `deck_edit_steps` (`0x081A725C`, `sub_0806EF04`), `deck_edit_select_steps` (`0x081A72A0`, `sub_0806EF74`), `deck_edit_sub_steps` (`0x081A72E4`, `sub_0806F01C`) and `deck_edit_popup_steps` (`0x081A7330`, `sub_0806F05C`).
> - **`0x081A7970` (29)** is `password_steps` (`0x081A7970`, `sub_0807CC28`) followed by `card_trading_steps` (`0x081A79A4`, `sub_0807D348`).
> - **`0x08199A2C` (22, "no literal-pool reference")** starts one word into `gUnk_08199A28`, whose first entry is NULL, and continues into `gUnk_08199A40` and further tables of [[code-08025108]]. The references are to the labels, which is why the scanner found none for `0x08199A2C`.
>
> Resolved in favour of the label-split tables. The scanner's runs are still valid groupings of related functions.

## Named tables from the asset converters
The converters write every pointer table in the data area as JSON with function names ([[assets]]), so these files are now the full listing:
- `tables/rodata2/*_steps.json`, `tables/pointer_tables/duel_step_handlers.json` (`0x0819D1D8`) and `ai_turn_steps.json` (`0x0819DD6C`);
- `tables/dialogue_box_steps.json` (`0x0813ADD4`, the bust-up runner, [[scene-sets]]);
- about 40 step tables in `tables/scene_scripts_and_lists/tables.json` (type `fn`, each named after the unit that reads it);
- the card-effect handler table `tables/card_effect_handlers.json` (`0x0819A9D4`): 426 rows of five slots (`resolve`, `check`, `prepare`, `chain_a`, `chain_b`). All 867 non-NULL entries are Thumb entry points of known functions ([[cards]]).
