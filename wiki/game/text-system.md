---
title: Text system
type: game
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Text system

**Summary (verified):** all game text is **English, single-byte ASCII**. The ROM holds no other language. No bytes ≥ 0x80 appear in any string, and a search for common French/German/Spanish/Italian words finds nothing. The rendering engine, though, is the Japanese one. It still handles **Shift-JIS** double-byte text, ships three kanji fonts, and has a table that maps ASCII to full-width characters. The Latin fonts cover **CP1252** (0x80–0xFF includes €, À–ÿ). There are two unrelated markup systems: `$` codes in dialogue and `@` codes in duel/system prompts.

> [!warning] Contradiction
> The summary above says no byte ≥ 0x80 appears in any string. The `tables_code` converter, which types every byte of `.rodata` 1 (2026-10-02), finds two exceptions, both re-checked in the ROM for this page:
> - **Three Japanese debug `printf` strings** in Shift-JIS at `0x080876B4`, `0x080876D4` and `0x080876F8`. They are buffer-save and buffer-output log messages, the only Japanese text in the ROM.
> - **Glyph strings** at `0x08086470`… for `sub_0805EE30`, which use bytes 0x81 and 0xC4 as custom glyphs.
>
> All player-visible text is still English ASCII. Resolved: the summary holds for game text, not for every string.

## Text locations

| What | Where | Layout | See |
|---|---|---|---|
| Menu, duel prompts, debug `printf` strings | `.rodata` 1, `0x08080A20`–`0x08087FB4` | NUL-terminated C strings. Code refers to each one directly from its literal pool (about 470 strings) | [[rom-map]] |
| Event/tournament names, debug menu | `0x081980D8` (calendar events in the scene/list range: `"Weekly Yu-Gi-Oh!"`, `"National Championship - 1st Round"`…), and in `.rodata` 2 `0x081A73A0` (`"Menu"`, `"Get all card"`, `"Get a pack"`, `"Next Level"`, `"License"`, `"TITLE"`) | C strings in fixed-size slots | [[debug-menu]] |
| **Dialogue** | `0x0813ADF4`–`0x0819739C`, then a terminator record (`0x0819739C`, 0x304 bytes, event = speaker = 0xFFFF, empty text) | 490 × 0x304 `{u16 eventId; u16 speakerCharId; char text[0x300]}`, zero-padded, longest text 341 bytes | below |
| **Character names** | `0x08139F64`–`0x0813ADD4` | 28 × 0x84 `{u32 id; char name[0x40]; char shortName[0x40]}`, record 0 blank | [[duelist-table]] |
| Card names | `0x0822C720` + id × 0x40 | fixed slots, id 1..820 | [[card-name-table]] |
| Card descriptions | `0x082461A0` + id × 0x1E0 | fixed slots, plain ASCII, **no control codes** | [[card-descriptions]] |

### Dialogue table
- **Event ID:** for duelist lines it is `duelist*1000 + n` (1000–24017; for example, 2002 is Tea and 14002 is Umbra & Lumis). Below 1000 are story/system events: 100/101 are the intro (speaker 2, Tea), 200–207 are tournament announcements (speaker 34), and there are also 300, 320, 350–352, 400–401, 500, 701–703, 800–803 and 900. Grouping those into shop, magazine and similar events is a hypothesis.
- **Speaker** (`u16` at +2) is the character ID of the portrait shown when the record opens. It ranges over 1–39.
- **Code:** the table base is read at `0x08001BC4`… (as `0x0813ADF4`) and at `0x080016F8`… (as `0x0813ADF8`, the text field). The text-box module is at about `0x08000930`–`0x08001E00`, and the function-pointer table at `0x0813ADD4` points into it. Its RAM state is `gTextBox` (see [[ram-map]]). The debug menu's "Bustup" item runs it directly ([[debug-menu]]).

### Character names
The table itself is on [[duelist-table]] (IDs 1–24 are the duelists, and 38–40 are Umbra, Lumis and Ghouls). Text code reaches it only through `sub_08000228(id, full)` (`0x08000228`–`0x0800026C`). That function does a linear search over records 1–27 for a matching `id`. It returns `&name` if `full != 0`, otherwise `&shortName`. An unknown ID returns record 0's name. This is verified from the disassembly.

## Dialogue control codes (`$`)
The dialogue text processor is a state machine around `0x08000DC4`–`0x08001076`. When it sees `$` it dispatches on the next character through a jump table at `0x08000E00` (index `c - 'Q'`, 34 entries). The handlers below were read from the code. The counts are occurrences across all 490 records, and the V/H column marks each meaning as verified (V) or hypothesis (H).

| Code | Count | Handler | Meaning | V/H |
|---|---|---|---|---|
| `$qNN` | 30 | `0x08001024` | Insert the **full** name of character NN: 2 decimal digits → `sub_08000228(NN, 1)` | V (code). Tea's intro "I am `$q02`" gives "I am Tea Gardner" |
| `$QNN` | 47 | `0x08001048` | Insert the **short** name of character NN (`sub_08000228(NN, 0)`), e.g. `$Q01` gives "Yugi" | V |
| `$iNNNN` | 15 | `0x08000FAC` | Insert a **card name by card number**: 4 digits → number → ID map at `0x08623DF4` ([[card-id-map]]) → name at `0x0822C720 + id*0x40`. Numbers ≥ 2000 use `map[n-2000]+1` | V (code, and every use gives the speaker's signature card: number 20 is Exodia the Forbidden One (Rare Hunter), 34 Dark Magician (Arkana), 61 Harpie Lady (Mai), 81 Red-Eyes B. Dragon (Joey), 751 Jinzo (Espa Roba), 761 Insect Queen (Weevil). Card number = classic list number − 1, per [[card-id-map]]) |
| `$rX` | 24 | `0x08000F10` | Text colour = hex digit X (`0`–`9`, `a`–`f`), stored at state+0x24. The data uses `$r3`, `$r4`, `$r5`, and `$r7` to reset | V (parse), H (7 = default white, via the palette at `0x0822C300`) |
| `$bNN` | 17 | `0x08000F64` | Switch the **speaker portrait** to character NN mid-record (clamped to ≤ 39). Umbra & Lumis records alternate `$b38`/`$b39`/`$b14` | V (parse), H (portrait) |
| `$n` | 163 | `0x08000EB4` | Line break | H (from usage) |
| `$c` | 374 | `0x08000EF0` | Always immediately followed by `$p`. Probably "wait for button" | H |
| `$p` | 374 | `0x08000EC8` | New page (clears the box) | H |
| `$h`, `$k` | 0 | `0x08000E88`, `0x08000E9C` | Clear or set a flag byte at `0x02014786`. Unused in the data | V (code) |
| `$$` | 0 | default | Falls to the default path, probably a literal `$` | H |

Other characters after `$` (`R`, `d`, `j`, `l`, `o`, …) take the default path. `sub_08079F10(p, n)` parses *n* decimal digits. `sub_08079F40` measures string width. Bytes > 0x7E count as a **2-byte SJIS** character, `$r?` skips 3 bytes, and any other `$x` skips 2.

## System/duel prompt codes (`@`, `%`)
The strings in `.rodata` 1 (`"Please select @3%s@0 as @2Tribute@0"`) use:
- `@0`, `@2`, `@3` (and rarely others) for colour switches, where `@0` is the default. Counts: `@0` 174, `@2` 103, `@3` 90. The measure/wrap routine at `0x08074AB4` treats `@0`–`@3` as zero-width, and also treats `\n` (0x0A) and the two-character sequence `\` `n` as line breaks. This is verified from the code.
- `%s`, `%d` (54 and 31 uses) are substituted by Konami helpers `0x080753F4` and `0x08075434` (see [[nintendo-sdk-libraries]]; there is no libc `printf`).
- Menu choices are written as `\n` plus 4 spaces plus the option (`"Coin-toss Selection:\n    @3Heads\n    @2Tails"`).

## Rendering
Text is drawn **pixel by pixel from 1bpp fonts** into a tile-ordered 8bpp canvas. Nothing is pre-rendered. Font formats are on [[font]].

| Function | Role |
|---|---|
| `sub_08074D48(u8 ch, x, y, size|colour)` | Draws a CP1252 glyph. Size 8/10/12/16 selects the font at `0x08228D00`/`0x08229500`/`0x08229F00`/`0x0822AB00`. 7 callers |
| `sub_08074C80(u16 sjis, x, y, size|colour)` | Draws a Shift-JIS glyph. Size 8/10/12 selects the kanji font at `0x081C0000`/`0x081D0200`/`0x081F8700`. 7 callers |
| `sub_08072584(u16 sjis)` | SJIS → glyph index: `(lead − 0x80 if lead ≤ 0x9F, else lead − 0xC0) × 192 + (trail − 0x40)`. 11 callers |
| `sub_08074B74` / `sub_08074BF8` | Plot one 8-px or 16-px glyph row as 8bpp pixels into the canvas (EWRAM `0x02000000`, tile-ordered, with width in tiles at `0x02010000`, H) |
| `sub_08078ED4`, `sub_08078FD4` | Tile-path variants for 8×8 ASCII (`0x08228D00`) and 8×8 SJIS, through the nibble writer `sub_080725B0` |
| `sub_08079FDC(dst, ch*, colour, bpp)` | Expands an 8×8 bold glyph (`0x0822BB00`) straight into a 4bpp or 8bpp tile, for HUD/number text |
| `sub_08074A90(ch)` | ASCII 0x20–0x7E → full-width SJIS through the table at `0x081A76A0` (95 × u16: `' '`→`0x8140`, `'A'`→`0x8260`, `'a'`→`0x8281`). Returns 0 otherwise |

## Method
- Collecting every byte of the dialogue, name and description tables shows that the set is printable ASCII plus `\n`. Code counts come from `re.findall(rb'\$[A-Za-z][0-9]*')`.
- The jump tables were decoded from `0x08000E00` (base loaded from the literal at `0x08000DFC`) and `0x08000F28`, then each handler was disassembled with `tools/cs.py`.
- The `$i` mapping was checked against the ten distinct numbers used (`tools/verify_rom_map.py` checks four of them).
- SJIS indexing was checked by rendering index 480 (`あ`, SJIS `0x82A0`) and 3834 (`日`, `0x93FA`) from all three kanji fonts.

## Open questions
- The exact semantics of `$c`, `$p`, `$h` and `$k` are unknown. Tracing the handlers' callee `sub_0800093C` would settle them.
- Is the palette at `0x0822C300` the one indexed by `$rX` and `@n`? Which colours are `@2` and `@3`?
- Why keep a blank 821-slot table (`0x08239460`) next to the names? Maybe a Japanese name table blanked for the USA build.
