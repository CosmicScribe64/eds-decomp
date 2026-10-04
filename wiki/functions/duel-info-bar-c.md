---
title: duel_info_bar (duel text box / menu UI) decompilation status
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Duel text box, 2/3-choice menus and UI graphics loaders in duel_info_bar (`0x0805F96C`-`0x080609C4`)

The unit is `src/duel_info_bar.c` (21 functions, 0x1058 bytes), compiled with `old_agbcc -O2`. **21/21 functions in C** after workflow wave 3 (2026-10-01/02: `0x0805FEA4`, `0x0805FD28` and `0x0805F96C` in wave 3, see [Wave 3 matches](#wave-3-matches-2026-10-0102)); none stay `INCLUDE_ASM`. Before wave 3: 18/21. The unit links to the exact target bytes. Names are proposals, and the code keeps `sub_08XXXXXX`. The text-box opener `TextBoxOpen` (`TextBoxOpen`) is called by the card-effect selectors in [[effect-targets3-c]] / [[duel-turn-end-c]]; the menu block at `0x0201AE60` is also described in [[duel-phases-c]] (`TextBoxDrawChoiceCursor` is byte-identical to `PhaseMenu_DrawCursor` there).

## Functions

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x0805F96C` | 0x238 | **matching** (wave 3, 2026-10-02) | `DuelScreenInfoStep` | `DuelCursor_GetCardId`/`TextCellsClear`, then a 16-way jump table on the mode `0x0201CFB0+0x828` with player/zone in `+0x824`/`+0x82C`: modes 0, 5, 10 test the zone's face-down bit (`flags6 >> 1 & 1`, zone arrays at `0x0201930C`, `0x020195F0`, `0x020198D4`), 11 calls `IsHandRevealed`, 12-15 print a text (`gStrMyFusionDeck..0x08086538`) with `DuelInfo_DrawLabelNumber(10, txt, playerByte, strlen)` |
| `0x0805FBA4` | 0x74 | matching | `TextMenuDrawCursor` | draws the 3-choice cursor sprite (attr `0x431F`), blinking while `state == 1` |
| `0x0805FC18` | 0xDC | matching | `TextMenuInput2` | input of the 2-choice menu with cursor (up/down toggle `sel`, A sets state 1 = blink timer, B plays sound 3); state 2 = done, returns 1 |
| `0x0805FCF4` | 0x34 | matching | `TextBoxLoadBorderGfx` | copies 216 x 0x20 bytes from VRAM `0x06009A40` into the UI buffer `0x0201AE84` and sets `gfx_ok` |
| `0x0805FD28` | 0x17C | **matching** (wave 3, 2026-10-01) | `TextBoxLayout(a, b, text)` | sets x/y = a bytes, w/h = b bytes (clamped: w 5..0x18, h 2..0xB), `TextBoxClearTiles`, `TextCanvasInit(w,h)`, then draws the string: `\n` new line, `@d` colour, 2-byte chars (bit 7 of `0x02011C20+4`) via `TextDrawSjisGlyph`, else 1-byte via `TextDrawLatinGlyph` (drop shadow drawn first at +1,+1), wrapping at `w*8` |
| `0x0805FEA4` | 0x1D8 | **matching** (wave 3, 2026-10-01) | `TextBoxDrawRow(row)` | draws one scan row of the frame into the BG map at `0x03002C5C`: blank row, top border tiles `0x82CE..0x82D0`, h text rows (`0x82D1`, ascending text tiles from `0x82D7`, `0x82D3`), bottom border `0x82D4..0x82D6`, blank row; clipped to rows 0..0x13 |
| `0x0806007C` | 0x30 | matching | `TextBoxScrollDown` | `DrawRow(row)`; 1 when `row >= y+h+2`, else `row++` |
| `0x080600AC` | 0x2C | matching | `TextBoxScrollUp` | `DrawRow(--row)`; 1 when row reaches 0 |
| `0x080600D8` | 0x88 | matching | `TextMenuInput1` | input of the 1/2-choice menu by mode (0: A confirms with sound 2; 1: A confirms and sets `sel` from `sel2`, B -> `sel = 0`, left/right toggles `sel2`) |
| `0x08060160` | 0x144 | matching | `TextBoxDrawMenuCursor` | cursor sprites for mode 0 (single arrow, blinking tile via the frame counter `gMain+0x485E >> 2`) and mode 1 (two markers `0x430C`/`0x4314` plus selection marker, `AddAffineSprite` / `AddSprite`) |
| `0x080602A4` | 0x64 | matching | `TextBoxOpen(a, b, c, text)` | the prompt helper: `TextBoxLayout(a, b, text)`, resets the state (`w2`, `mode`, callbacks, `+0x20..+0x23`, `sel`), sets `active`, clears bit 3 of `0x0201CFB0+0x808` |
| `0x08060308` | 0x3C | matching | `TextBoxSetMenu(mode, cb1, cb2)` | stores mode and the two callbacks; mode 2 installs `TextBoxDrawChoiceCursor` / `TextBoxHandleChoiceInput`, modes 0/1 clear them |
| `0x08060344` | 0xBC | matching C | `TextBoxUpdate` | per-frame driver: state byte `+0x20`: 0 runs scroll-down when opening flag 1 is set, 1 runs the menu callback (`cb2`, else `TextMenuInput1`) when flag 8 is set, 2 runs scroll-up when flag 2 is set; a clear flag advances the state immediately. Other states clear `active`; returns 1 when active on entry |
| `0x08060400` | 0x1C | matching | `ScrollBgUp` | `--0x0201CFB0.scroll`, written to BG0VOFS/BG1VOFS (`0x04000010/12`) |
| `0x0806041C` | 0x30 | matching | `SetBgControl` | BG0CNT = 7, BG1CNT = 0x8106, BG2CNT = 0x8305, BG3CNT = 0x0504 |
| `0x0806044C` | 0x12C | matching | `FillFrameMap(style)` | loads the frame graphics (`LoadBgImage4bpp(0, 0x50, 0x60, gFieldBackgroundImages[style])`), fills the 32x32 BG map (`0x0300045C`) with 4x4 blocks of tiles `0x5060..0x506F`, stores `style` in the high nibble of `0x0201CFB0+7` |
| `0x08060578` | 0x1E4 | matching | `LoadDuelUiGfx` | `DISPCNT |= 0x40`, 7 palettes to `0x05000200..`, 17 tile blocks to OBJ VRAM `0x06010000..`, sets bit 1 of `0x0201CFB0` |
| `0x0806075C` | 0x160 | matching | `LoadTextBoxBgGfx` | `LoadSystemGfx`, palettes `0x05000020..0x05000100`, tile blocks `0x06004E00..0x060099C0` |
| `0x080608BC` | 0x78 | matching C | `DrawNumber5(block, cell, palOff, val)` | prints `val` as 5 digits (leading zeros blank) right-to-left from `cell + 4`: tile = `0x3244 + c*10 + digit`, map `0x0300045C + block*0x800` |
| `0x08060934` | 0x30 | matching | `DrawCounter(kind, val)` | switch: kind 0 -> `DrawNumber5(3, 0x267, 0, val)`, kind 1 -> `(3, 0x294, 0, val)` |
| `0x08060964` | 0x60 | matching | `DrawSelector2x6(a, b)` | 2 x 6 selector tiles at map `0x03001C5C` (rows `0x286`, `0x273`), tile `0x288 + j + 0x4000`, entry (a, b) highlighted by `-6` |

## UI block `struct Ui` at `0x0201AE60` (verified field offsets)

`+0` bit 0 `active`, bit 1 `gfx_ok`; `+2` u16 (cleared on open); `+4` u16 `mode` (menu kind; 2 = 3-choice with cursor callbacks); `+6` u16 (third argument of `TextBoxOpen`); `+8` x, `+0xA` y, `+0xC` w, `+0xE` h (u16, cells); `+0x12` `sel2` (2-choice selection), `+0x14` `sel` (result); `+0x18` cursor callback, `+0x1C` input callback; `+0x20` u8 step (0/1/2), `+0x21` u8 current row, `+0x22` u8 blink state, `+0x23` u8 blink timer; `+0x24` 216 x 0x20 bytes border tile buffer.
`0x0201CFB0`: `+0` flags (bit 1 = gfx loaded), `+2` u16 scroll, `+7` high nibble = frame style, `+0x808` bit 3 cleared on open, `+0x824/828/82C` screen state for `DuelScreenInfoStep`.
`gMain` (`0x03000040`): `+4` held keys, `+6` pressed keys (bit 0 A, 1 B, 3 Start, 6 Up, 7 Down; `0x30` = left/right), `+0x485E` frame counter.

## Tricks learned (old_agbcc)

- **Consecutive stores to `sym[k + n]`** only keep the `(n+k)<<1; add base` form per element when indexed through an **`asm("...")` u16-array alias of the address symbol** (`DuelScreen_LoadFieldBackground`); a local `u16 *map` pointer folds them into `[r, #imm]` offsets.
- **Plain constant stores** (`u->mode = 0; u->cb1 = 0; ...`) reproduce the target's separate QI/HI zero registers; naming a `u8 z8`/`u16 z16` made it worse.
- **`TextBoxUpdate` switch-copy and store sharing (resolved):** retain a word original `st`, a signed switch copy `sw`, and a separately formed state-byte pointer. The target reloads `*p` after each successful callback, but a clear opening flag computes `st+1` from the original byte. Three initialized register hints (`st` r2, callback r1, next-value r0) retain those paths. An empty input on the initialized next-value before the default menu-input store and two empty clobbers after the menu and scroll-out stores prevent cross-jumping separate case store tails. They emit no instructions and are marked FAKEMATCH. The callback branch explicitly jumps to `menu_store`, retaining both callback/default reload sequences. Within case 2, `w6 & sw` uses the existing value 2 instead of materializing a fresh mask. Pointer/switch pins, the initial state read/write barrier, all case-0 barriers, and case-2 pre-store barrier were removed with whole-unit checks. See [[matching-tricks]] for the related `Chain_WaitPartnerReply` state-copy case.
- **Bitfield flags** (`u8 active:1; u8 gfx_ok:1`) give the `mov r0,#2; ldrb; orr; strb` form for `flags |= 2` with the constant first, and `u8 f3:1 = 0` the `movs #9; negs` mask.
- **Operand order of sums of two struct fields** follows source order (`u->x + u->w` loaded w/x in that order), `pos -= 0x30` as a separate statement keeps `lsls r5,r0,#3` in the destination.
- **`(u8)a` for the low byte** of a `u16` parameter gives `lsl 8; lsr 24` (target) instead of `movs #255; ands`.
- **Helpers with `u16` return** (`lsl r0,#16; cmp r0,#0` in the caller) need a separate prototype through an `asm("sub_XXXX")` alias when the definition returns `int`.
- **An integer that is only a byte/lane value should often be declared `u16`/`u8`**, not `int`: in `DrawPhaseIndicator` changing `int t` to `u16 t` (loop tile base) turned a 16-line register diff into a byte match (`val`/`cell` r4/r5 went from swapped to correct). Small type changes here re-run the register allocator, so try them before giving up.
- **A dead byte copy can steer the switch-value register**: `TextBoxHandleChoiceInput` only matched (via permuter) after adding `u8 new_var;` + `new_var = st;` used in `0x0201CFB0.b0 & new_var`. The copy moves the loaded state byte into r0 and the switch copy into r2. Marked `/* FAKEMATCH */`.

## Nonmatching notes

- Historical (matched in wave 3): `0x0805F96C`: draft loads the same fields but the register allocation and prologue differ throughout (built uses one fewer callee-saved push), so it is not a small diff; the permuter did not help.
- Historical (matched in wave 3): `0x0805FD28`: despite the earlier note, the built body differs across the whole function (extra `sl`/register pressure); not a near-miss.
- Historical (matched in wave 3): `0x0805FEA4`: built allocates a different stack frame and register set; not a near-miss.
- `0x08060344` now matches all 0xBC bytes. The former corrected private near miss was four bytes short because the final stores for cases 1 and 2 shared case 0's store. Placing distinct empty compiler clobbers after each explicit case store retained the required intra-case sharing and separate inter-case stores. A first pass with barriers after every inner branch store was eight bytes too long; moving them to the single explicit case store solved that.

> [!warning] Corrected semantic discrepancy
> The older parked `TextBoxUpdate` draft and function description interpreted the tests as `gMain.keys`. The ROM retains the Ui base in r1 and loads `[r1,#6]`, so the tested halfword is `Ui.w6` at `0x0201AE66`, set by the third argument to `TextBoxOpen`. The matching C reads that opening-flag field. Other menu input functions on this page still read the actual main key field.

- Verification: `tools/dr python3 tools/check.py duel_info_bar` returns 0 with all 0x1058 unit bytes MATCH. Its 21/21 byte-matching functions included three assembly fallbacks; the actual C count was 18/21 (historical: all 21 are C since wave 3).
- `0x080608BC` matches. The column is a full ARM word argument in the C interface, narrowed explicitly by `((b << 16) + 0x40000) >> 16`, which equals `(b + 4) & 0xFFFF` for every unsigned 32-bit input. This preserves the four register-word argument ABI and the ROM's 16-bit column wrap while avoiding a premature parameter-type narrowing. An empty input barrier on `v` keeps the value in r4 and the cell in r5; it emits no instruction and is marked `FAKEMATCH`. The former draft's `u16 b` type placed the shift before the value copy, and its `b + 0x40` comment was incorrect, because the ROM adds 4. Verified with `tools/check.py duel_info_bar`: 21/21 functions and all 0x1058 unit bytes MATCH.

Related: [[duel-phases-c]], [[effect-targets3-c]], [[decomp-workflow]], [[compiler-flags]].

## Wave 3 matches (2026-10-01/02)

All three match in ordinary C. Working notes: `build/wf/DuelScreen_DrawCursorInfo/NOTES.md`, `build/wf/TextBoxDrawText/NOTES.md`, `build/wf/TextBoxDrawTilemap/NOTES.md`.

### `DuelScreen_DrawCursorInfo` (0x238, start score 227; ordinary C)

- `c` (`+0x82C`) is `u32`, not `s16`: the ROM loads a full word and multiplies it without an extension.
- The ROM keeps two extra callee-saved values that only the case-0 call uses: r8 = a copy of `a` and r9 = `b + c`, both computed before `TextCellsClear()`. Fix: `a2 = a; sum = b + c;` before that call; cases 5/10 pass a freshly computed `b + c`.
- Multiply order: case 0 uses `ZB2(a & 1, c)` (`p*0xD64 + z*0x94`), case 5 the explicit `c * 0x94 + (a & 1) * 0xD64 + base`, because agbcc emits these operands in the reverse of the source order (see [[matching-tricks]], zone forms).
- Modes 12-15 are nested `switch (a) { case 0: ... case 1: ... }` (ROM `cmp #0; beq; cmp #1; beq; b end`) reading `gDuelPlayers[a].bN`; CSE substitutes the known `a` = 0/1, which gives the `+0xD64` literal form.
- Last swap (txt r4 / p r5): `const u8 *txt` and `u8 p` declared block-local in each case block. Function-wide variables became shared pseudos whose priorities put `p` first.

### `TextBoxDrawText` (0x17C, start score 180; ordinary C)

1. Rewritten from the asm with direct `gTextBox.field` accesses (no local struct pointer) and `int col, px, py`: score 62. The old draft had the wrong wrap test, a swapped call order and an if-chain.
2. `switch (*s) { case '@': ...; case '\n': ...; default: ... }` with case `'@'` first. This gives the ROM layout (it tests 0xA first but places the `'@'` body first), and the switch load is a separate SImode zero-extend that is not merged with the loop-condition load.
3. The real callee prototypes from `src/text_canvas.c`, called through casts: `TextDrawSjisGlyph(u16, s32, s32, u16)`, `TextDrawLatinGlyph(u8, s32, s32, u16)`, and `TextWordLength` returning `int` (signed `ble`, no extension). The `u16` fourth parameter makes `0xA00` an HImode constant, which explains the ROM's `movs; lsls; adds r5, r0, #0` hoist copy; it fixed the table/`0xA00` r5/r7 swap and every later reload register (62 -> 0).
- Tool issue found here: while Docker was down for about a minute, `wf.py check` printed the connection error and then `score: 0 (MATCH)`, so a failed compile counted as a match. `apply` re-verifies the whole unit, so a false check result should not reach `src/`, but checks run by other agents at that time were not re-confirmed; see [[agent-tooling]].

### `TextBoxDrawTilemap` (0x1D8, start score 124; ordinary C)

Score 124 -> 0 in six experiments, each change found by reading the `.greg` dump:

1. `base = 0x82CE; text = 0x82D7;` assigned after `idx = q`, not as declaration initialisers. The GCSE reaching register for `&gTextBox` (12 refs, live 302, priority 0.119) and `base` (11 refs, live 278, 0.1187) were nearly tied. The shorter live range put `base` in sl; the Ui pointer then got no register and every use became a fresh `ldr =0x0201AE60` from its REG_EQUIV, as in the ROM.
2. `u16 text` (not `s16`): `adds r0,r1,#1; lsl; lsr`.
3. The text-row block indexes `gUnk_03000040_m2.map[...]` like the other blocks, not the `gUnk_03002C5C` symbol. loop.c hoists the invariant through its REG_EQUAL constant (the single literal `0x03002C5C`); the inner loop's PRE copy then has no REG_EQUIV and is spilled to `[sp+4]` instead of rematerialised, as in the ROM.
4. The outer text-row loop reuses `j` (the counter of the other blocks' loops) and the inner loop uses a separate `k`, which puts the outer counter in r3 (it was r0).
- Failed: `((u16 *)0x03002C5C)[...]` in the middle block (259), the `gUnk_03002C5C` symbol directly (201), a top-level `u16 *m` local (124), one counter shared by blocks 1/5 and the outer loop (108).
