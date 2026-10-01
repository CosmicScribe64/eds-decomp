---
title: Function pointer tables
type: data
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-09-30
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
