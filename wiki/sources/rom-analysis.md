---
title: "Direct ROM analysis (source)"
type: source
status: solid
confidence: high
sources: []
updated: 2026-10-01
---
# Direct ROM analysis (source)

This is a pseudo-source for facts checked directly against the baserom with scripts. Each page that cites it records its own method.

Baserom: `Yu-Gi-Oh! - The Eternal Duelist Soul (USA).gba` (SHA-1 `510fbba212aca9bab95ea12f8fd933e62ee34dea`).

Pages derived from it: [[rom-header]], [[save-type]], [[card-name-table]], [[card-table]], [[card-descriptions]], [[card-id-map]], [[password-table]], [[card-art]], [[duelist-table]], [[deck-lists]], [[booster-packs]], [[special-card-lists]], [[cards]], [[card-data-functions]].

Reusable extraction and verification script: `tools/extract_cards.py`. It is stdlib-only, reads the baserom, and prints JSON/CSV to stdout. `--verify` runs the spot checks.
