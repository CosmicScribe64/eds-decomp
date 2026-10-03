---
title: Card descriptions
type: data
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Card descriptions (English)

**Verified 2026-09-29.** Each card's lore or effect text is a NUL-padded ASCII string in a fixed 0x1E0-byte slot, indexed by card ID. Slot 0 is empty.

| Field | Value |
|---|---|
| Base (slot 0, empty) | `0x082461A0` (`ROM+0x2461A0`); one literal-pool reference, at `0x080063A0` |
| First text (ID 1) | `0x08246380` |
| End | `0x082A6500` (821 × 0x1E0 = `0x60360` bytes); [[card-art]] starts right after |
| Entry size | 0x1E0 (480 bytes; the longest text is 473 chars) |
| Count | 821 |

```c
/* 0x082461A0 */
const char gCardDescriptions[821][0x1E0];   /* name is a proposal */
```

## Content notes
- Plain printable ASCII only: letters, digits, space, and `" # % ' ( ) + , - . / :`. There are no control codes and no newlines. Line wrapping happens at render time. The text-script codes such as `$c`, `$p`, and `$r5` used in dialogue don't appear here.
- A fusion monster's text is its material list, for example "Blue-Eyes White Dragon + Blue-Eyes White Dragon + Blue-Eyes White Dragon" for ID 81. That explains the stray name hit at `ROM+0x24F980` noted in [[card-name-table]].
- Empty texts: ID 0, the 3 Ticket cards (IDs 814–816), and Insect Monster Token (ID 820).
- Alternate-art duplicates (for example IDs 82 and 83, Blue-Eyes White Dragon) have identical text in both slots.

## Used by
- `CardDetail_DrawInfo` (card detail screen). It computes the slot address with `id*15*32 + 0x082461A0` (`lsls r0,r7,#4; subs r0,r0,r7; lsls r0,r0,#5`), then passes it to a text-box routine (`bl 0x080059B4`).

## Method
- Stride and alignment: consecutive texts found by a string scan start 0x1E0 apart ("A rare rainbow fish…" at `ROM+0x246380` belongs to ID 1, 7 Colored Fish).
- Walking the 0x1E0 slots from ID 1 showed that every slot is printable ASCII up to its NUL, with no stray bytes after the terminators.
- Indices were matched to [[card-name-table]] using several distinctive texts. ID 81 holds the Blue-Eyes Ultimate Dragon materials, and ID 152 holds Dark Magician Girl's text, which mentions "Dark Magician".
- Reproduce: `python3 tools/extract_cards.py cards --desc`. The text is copyrighted, so the script prints it to stdout only and it is never stored in the repo.

Related: [[card-table]], [[card-name-table]], [[cards]].
