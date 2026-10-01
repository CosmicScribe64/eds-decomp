---
title: Card-data functions (cluster)
type: function
status: stub
confidence: medium
sources: [rom-analysis]
updated: 2026-09-29
---
# Card-data functions (cluster)

These are the functions that read the card tables. They were identified by where they load the table bases. **None has been decompiled yet** (match status: `nonmatching` for all). All are Thumb. Sizes are measured from one `push {…, lr}` to the next, so they include each function's literal pool and are approximate.

| Function | Range | Purpose (from disassembly) | Tables touched |
|---|---|---|---|
| `sub_08000C54` | `0x08000C54`–`0x080011F0` | Large text/formatting routine. At `0x08000FC0` it contains the inline card number → ID → name lookup (`0xFFFF` → none, `≤1999` direct, `≥2000` alternate +1). | [[card-id-map]], [[card-name-table]] |
| `sub_08005A70` | `0x08005A70`–`0x080063D0` | Card detail screen text: name, `[Type/Kind]`, ATK/DEF, description box. Special-cases card numbers 812, 730, and 1910–1912. | [[card-table]], [[card-name-table]], [[card-descriptions]], [[card-id-map]] |
| `sub_0800495C` | `0x0800495C`–`0x08004ABC` | Builds one of the 3 initial decks (`choice = arg % 3`) from the pools at `0x08198744`. | [[deck-lists]] |
| `sub_0800ABC8` | `0x0800ABC8`–`0x0800C894`? | Duel: reads a zone word from per-player state `0x0201930C + (p&1)*0xD64 + slot*0x94`, takes the card ID (low 12 bits), and fills an output struct with `{u16 id; u8 type | attr<<5; …}`. | [[card-table]] |
| `sub_0801B640` | `0x0801B640`–`0x0801B6D4` | Counts how many of the 60 "notable" cards the trunk holds and returns `count >= n`. | [[special-card-lists]] |
| `sub_0805DF34` | `0x0805DF34`–`0x0805E054` | Loads a card picture: palette → PRAM, then unpacks 6bpp → 8bpp tiles in VRAM. | [[card-art]] |
| `sub_080629F0` | `0x080629F0`–`0x08062A0C` | Returns the highest non-empty rarity slot of a pack. | [[booster-packs]] |
| `sub_08062A0C` | `0x08062A0C`–`0x08062AD4` | Rarity roll (thresholds at `0x081A570C`, pity counter). | [[booster-packs]] |
| `sub_08062AD4` | `0x08062AD4`–`0x08062AF4` | Returns a random card from a pack slot. | [[booster-packs]] |
| `sub_08062AF4` | `0x08062AF4`–`0x08062EE8` | Generates a 5-card pack (normal packs, and the special random packs 0x66/0x67/0x6E). | [[booster-packs]], [[card-table]] |
| `sub_0807C304` | `0x0807C304`–`0x0807C374` (0x70) | Password → card ID (BCD compare over 821 entries). | [[password-table]] |

## Callees seen
- `0x08076F9C`: probably `rand()` (hypothesis). Its result feeds a modulo (`bl 0x0807EF6C`).
- `0x0807EF6C` / `0x0807F0AC` / `0x0807F124`: modulo/division helpers, probably libgcc `__umodsi3`/`__modsi3`-style routines (hypothesis).
- `0x080752E8`: strcat-like (appends a string to a stack buffer). `0x080753CC`: strlen-like (hypothesis).
- `0x080059B4`: draws a text box (called with the description pointer).

## Matching notes
None yet. The recurring idiom `ldr rX, =0x7FF; ands; lsls #2; ldr rY, =table; adds; ldr` suggests source like `gCardStats[id & 0x7FF]`, or a macro/inline accessor used at hundreds of sites.

Related: [[card-table]], [[cards]].
