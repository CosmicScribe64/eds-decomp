---
title: Unit battle_scene (duel screen: card-flip transition, text-cell buffer, card info panels)
type: function
status: solid
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit battle_scene

The unit covers `0x0805E788`-`0x0805F96B` (Thumb, `old_agbcc -O2`), and its source is `src/battle_scene.c`. It sits between the duel UI animation helpers ([[duel-card-anim-c]]; `BattleScene_DrawValues`, `BattleScene_DrawDamage` and `BattleScene_ResetBgAffine` are called from here) and the text box UI ([[duel-info-bar-c]], which calls `DuelCursor_GetCardId` / `TextCellsClear` / `DuelInfo_DrawLabelNumber`). Unit status: `unit bytes MATCH`, **14/14 functions in C** after workflow waves 2-3 (2026-10-02: `0x0805E788`, `0x0805EF00`, `0x0805F270` in wave 3); none stay `INCLUDE_ASM`. Before wave 3: 11/14, with 3 `INCLUDE_ASM` (2 had near-miss attempts under `#if 0`, and the big transition state machine `BattleScene_Update` had not been attempted).

The unit has two parts. The first is a fade/transition state machine and a screen-cursor helper. The second is a "text cell buffer" API, a buffer of 64 cells of 0x20 bytes (8x8 4bpp tiles) at `0x0201CFB8`. Strings and numbers are rendered into it (`RenderBoldGlyphTile` draws one char into a cell and `RenderSjisGlyphTile` a 2-byte char) and then copied to VRAM. The card info panels are built on it.

> [!warning] Contradiction
> The `0x0805EF00` row below (2026-10-01) said `LoadIconPack` "clears bits 0-1 of `0x0201D7B8`", and the "Structs and globals" list said "low bits cleared". The original assembly ends with `mov r0,#3; neg r0,r0; ldrb; and; strb` at `0x0201CFB8+0x800`, which clears bit 1 only, and the wave 3 match (2026-10-01, `build/wf/TextCellsLoadIcon/NOTES.md`) writes it as the single bitfield store `->f1 = 0`. Resolved in favour of the matched source; both places are corrected.

| Address | Size | Status | Purpose (hypotheses) | Proposed name |
|---|---|---|---|---|
| `0x0805E788` | 0x574 | **matching** (wave 3, 2026-10-01) | `(u16 a, u16 b, u16 mask)`: state machine on the byte `0x02018450+0x15C` (5 states) with a sub-step byte `+0x15D` (0-4) and a frame counter `+0x15F`: writes BLDCNT/BLDALPHA (`0x04000050/54`, fade 0xF-n, 0x3FFF, 0x0404/0x0808 target bits), DISPCNT `\|= 0x1C00`, clears IME/IE bits (`0x04000208/200`), calls `ClearBlend`, `BattleScene_ResetBgAffine`, `BattleScene_DrawValues`, `BattleScene_DrawDamage`, `Random` (random, shake offsets into `gBattleSceneShakeRegs` tables), `FadeToBlack`; held B (`gMain+4` bit 1) or the flag `0x0201CFB0 & 1` speeds the timers up (+8 per frame instead of +1; +4 for the first fade), and pressed B (`gMain+6` bit 1) ends the hold of sub-step 4; returns 1 when done (state > 4) | `DuelTransitionStep` |
| `0x0805ECFC` | 0x7C | matching | card id (12 bits) under the screen cursor: `0x0201CFB0+0x824` player, `+0x828` mode, `+0x82C` index: modes 0/5/10 = zone `mode + index` of the player, 11 = hand slot `index` (`0x02019968`), else 0. FAKEMATCH: the base and byte offset are pinned to r2/r0 so agbcc emits `adds r0, r2, r0` | `ScreenCursorCardId` |
| `0x0805ED78` | 0x24 | matching | fills the 64 halfwords at `gMain+0x309C..0x311A` with `0x28E..0x2CD` (a BG map index table) | `InitCellMapTable` |
| `0x0805ED9C` | 0x44 | matching | clears the 64 text cells (`RenderBoldGlyphTile(0x20, cell, 0, 9)`), sets bits 0,1 of `0x0201CFB0+0x808` (dirty), then `TextCellsResetMap` | `TextCellsClear` |
| `0x0805EDE0` | 0x50 | matching | `(first, u16 *str, pal)`: draws a halfword string (bytes swapped: `(c>>8) \| (c<<8)`) into cells from `first`, stops at a zero low byte; sets the dirty bits | `TextCellsPutString16` |
| `0x0805EE30` | 0x48 | matching | `(first, u8 *str, pal)`: same for a byte string | `TextCellsPutString` |
| `0x0805EE78` | 0x64 | matching | `(first, val, pal, digits)`: prints `val` as decimal, right-aligned ending at cell `first + digits - 1` (digit + 0x30); stops early (without the dirty bits) when the quotient becomes 0 | `TextCellsPutNumber` |
| `0x0805EEDC` | 0x24 | matching | `(cell, tile)`: `MemCopy16(buf + cell*0x20, 0x06004000 + tile*0x20, 0x20)` (copies VRAM tile into the cell; hypothesis on direction) | `TextCellFromTile` |
| `0x0805EF00` | 0x10C | **matching** (wave 3, 2026-10-01) | `(a, b, hdr)`: loads an icon pack (`hdr`: u16 palette length n at +0, palette at +8, tile count at +8+2n, tiles after) into cell `a` (two tile rows, +0x400 apart), palette slot `b`, and sets palette `b & 15` in the four BG map entries of that cell (`gMain+0x2C1C`, indices `a+0x240/0x260`, `a+1+...`); clears bit 1 of `0x0201D7B8` (= `0x0201CFB0+0x808`, the redraw bit; corrected from "bits 0-1", see the contradiction note above) | `LoadIconPack` |
| `0x0805F00C` | 0x68 | matching | `(u16 n)`: a 2-row name box for string `n` of the table `gCardNames` (64 bytes each): width 12, or 10 when the text is longer than 15, `TextCanvasInit(0x20,2)`, two `TextDrawString` calls (shadow and text) | `DrawNameBox` |
| `0x0805F074` | 0x1FC | matching | `(u16 id, u16 flag)`: name box (like above, threshold 12) plus, for monster cards (type <= 0x14) when `flag`: labels (`gInfoAtkLabelJp/78` or `..80/88` by `0x02011C20+4 & 0x7F`), ATK, DEF, level numbers, cell block `0x18` tile 3. FAKEMATCH: the byte offset of the name string is pinned to r0 so agbcc emits `lsls` before the table `ldr` | `DrawCardInfoHeader` |
| `0x0805F270` | 0x3DC | **matching** (wave 3, 2026-10-01; FAKEMATCH) | `(u16 id, u16 flag, player, slot)`: name box; for a zone flagged bit 1 holding card number 0x47/0x15B/0x4CE a counter box (`byte6 >> 2 & 15`, `gTurnCounterLabel` label, x shifted by 0x3C/0x46); the ATK/DEF/level block of `DuelInfo_DrawCard` when no counter box was shown; cards 0x479/0x5A8 add an icon pack `gCardTypeIcons/81A41F8[(zone+0x90)>>18 & 31]`. | `DrawZoneCardDetail` |
| `0x0805F64C` | 0xDC | matching | `(a, str, val, cnt)`: frame + number box: `TextCanvasInit(0x20, 2)`, two text draws with attr `(a<<8) \| 0xD/5`, then if `cnt > 0` the number `val` via `TextDrawNumber` centred (`a * cnt' / 2 + 3`, `cnt'` grows by one per digit, by two when `0x02011C20+4` bit 7 (2-byte chars) is set) and finally `TextCanvasToTiles(buf, 9)` | `DrawNumberBox` |
| `0x0805F728` | 0x244 | matching | `(player, slot)`: `GetZoneCardStats` card info (12 bytes: id u16, attr/kind bitfields in byte 2, ATK, DEF), `DuelInfo_DrawCard(id, 0)`, labels, ATK/DEF with palette 7 when equal to the printed value else 6, level, attribute icon (`gCardTypeIcons[info.attr]`, attr 1-0x14) and kind icon (`gCardAttributeIcons[info.kind]`, kind 1-6) via `TextCellsLoadIcon` | `DrawCardDetail` |

## Structs and globals
- `0x0201CFB0` duel screen block (see [[duel-prompt-handlers-c]], [[duel-card-anim-c]]): `+0x808` bits 0-1 dirty flags of the text cells (bit 1 = redraw), `+0x824/828/82C` cursor player/mode/index.
- `0x0201CFB8` text cell buffer: 64 cells x 0x20 bytes; byte `+0x800` (`0x0201D7B8`, the same byte as `0x0201CFB0+0x808`) has bit 1 cleared by `LoadIconPack` (corrected from "low bits", see the contradiction note above).
- `0x02018450+0x15C..0x15F`: transition state / sub-step / frame counter (`struct Blk18450` in [[duel-card-anim-c]] has `+0x15D`: the sub-step byte is also the table row of the HBlank warp `BattleScene_HBlank`).
- `0x02011C20+4` bit 7: 2-byte character mode; `& 0x7F` selects the label set (language?).
- Card info block from `GetZoneCardStats` (12 bytes): `u16 id; u8 attr:5; u8 kind:3; u32 atk; u32 def`. Keep `attr`/`kind` in a `u8` storage unit, not `u32`: agbcc then emits `ldrb` + `and #0x1F`/`and #0xE0` for the `!= 0` tests (a `u32` unit gives `lsls`/`lsrs` instead and breaks `DuelInfo_DrawMonsterZone`).
- Card stats table `0x08621DE0`: ATK-like bits 9-17 (x10), DEF-like bits 0-8 (x10), level bits 25-28, type bits 20-24 (0x15-0x17 Magic/Trap/Ritual, 0x18 special).

## Matching tricks
- **Inline helpers with `u16` return type** (`CardAtk/CardDef/CardLevel` = the `switch (CARD_TYPE)` value, 0 / 4000 / (stats field * 10), level 0 / 10 / field) make the call argument a copy (`adds r1, r0, #0`) as in the ROM; `int` results are allocated straight into the argument register.
- **`x = (int)(len * w) >> 1;` into a fresh variable** (not `len *= w; len >>= 1`) gives the ROM register assignment in `DuelInfo_DrawCardNameCentered` (len dies at the multiply and the result reuses r4).
- **Post-increment of the cell index in the argument** (`buf + first++ * 0x20`) puts the `adds r5, #1` before the `bl` (`TextCellsPutSjisString`, `TextCellsPutString`); `first += digits;` as a statement (not a new `pos = first + digits`) keeps the extra `adds r6, r0, #0` copy in `TextCellsPutNumber`.
- **Byte swap** `u32 hi = c >> 8; u32 lo = (u8)c << 8; f(lo | hi, ...)` reproduces the shift order; a single expression reloads the halfword.
- **Sibling of `QueueFlipSummon`'s lesson**: a `sym + 0x311A` pointer that the ROM adds at run time is written `(u16 *)((u8 *)&gMain + 0x311A)` (a `&gMain.field[n]` folds into one literal); the declaration order `b, i, v, p` reproduces the ROM's load order (`TextCellsResetMap`).
- **Bitfield flags** `f808_0 = 1; f808_1 = 1;` give two `orr`s (`mov r1,#1; ldrb; orr; mov r2,#2; orr; strb`).
- **Nested switch for `n == a || n == b || n == c`** in `DuelInfo_DrawSpellZone` (`switch (n) { case 0x47: case 0x15B: case 0x4CE: ... }`) gives the ROM's binary decision tree; the `||` chain does not.
- **Local register-variable pins** (`register u8 *p asm("r2"); register u32 n asm("r0");`) fix agbcc's operand/schedule choice where it would otherwise differ: in `DuelCursor_GetCardId` pinning the base to r2 and the offset to r0 makes the final add `adds r0, r2, r0` instead of `adds r0, r0, r2`; in `DuelInfo_DrawCard` pinning the name-string byte offset to r0 makes agbcc emit `lsls` before the table `ldr`. Both are marked `/* FAKEMATCH */` (harmless, because they only steer register allocation). `DuelInfo_DrawSpellZone` (wave 3) reuses the `DuelInfo_DrawCard` pin.
- **Compare against the memory operand, not a cached local:** in `DuelInfo_DrawMonsterZone` write `CardAtk(id) != info.atk ? 6 : 7` (and `info.def` for DEF) while keeping `atk`/`def` locals for the call argument; this reproduces the ROM's extra copy / stack re-read.
- **Historical (both matched in wave 3, see below): open problems:** broad register allocation in `TextCellsLoadIcon` (ROM: `cnt` r6 / `cell` r5, map base r8, map-entry pointers r5/r4/r3/r1; the draft swaps `cnt`/`cell` and spills the pointers) and in `DuelInfo_DrawSpellZone` (name-string address and the zone-address products use swapped temporaries). Both have parked drafts as permuter bases; simple register pinning did not converge (the intermediate address temporaries reuse the same registers).

## Wave 3 matches (2026-10-01/02)

Working notes: `build/wf/BattleScene_Update/NOTES.md`, `build/wf/TextCellsLoadIcon/NOTES.md`, `build/wf/DuelInfo_DrawSpellZone/NOTES.md`.

### `BattleScene_Update` (0x574, start score 155; ordinary C)

1. **r6 as a reload register for constants.** The ROM loads constants through r6 (`ldr r6,=0x3FFF; adds r0,r6,#0`, `movs r6,#0xAE; lsls`). Reload's per-insn spill set is the function-wide union of spill registers minus the registers live at that insn, and `allocate_reload_reg` round-robins over it (`last_spill_reg`). r6 joins the set only if some insn needs a reload while r0-r5 are all live. In the ROM that insn is the `0x404`/`0x808` constant in sub-step 3, where the old timer value stays live in r3 across the BLDCNT ORs. The draft re-read the timer (`gUnk.t++`), so r3 was free and r6 never became a spill register. Fix: `u32 t3 = gUnk.t; if (t3 <= 0x1F) { ...; BLDY = gUnk.t; gUnk.t = t3 + 1; ... }`. A `u8 t3` added a copy pseudo that landed in ip, and `u16` was worse; `u32`/`int` gave the ROM's base r2 / timer r3 split.
2. **Cross-jumped tails.** The `step++` / `t += 7` tails and the single `movs r0,#0; b epilogue` block (which the ROM places after sub-step 3's loop) need an explicit `return 0;` at the end of nearly every path. That covers both exits of case 1 (`if (v <= 0xE) { ...; return 0; } step++; return 0;`), every exit of sub-steps 0-3, and the `pressed & 2` branch of sub-step 4. Only sub-step 4's timer branches still `break` to the shared `return 0`. The reason is in jump2, after reload. `find_cross_jump` stops the jump-side stream at a CODE_LABEL, so a `step++` that falls into a join label before `return 0` can never merge. The "other jump to the same label" matching (minimum 2 insns) runs only for labels whose UID is below max_uid, so it never runs for labels that cross-jumping creates. Paths that `break` to the shared after-switch label therefore merge with each other (sub-step 1's `adds #8; strb` tail merged with sub-step 4's), while `return 0` paths jump to the new label and stay separate. The label-preceded `set r0 0; jump ret` copies merge into the first matching jump in jump_chain order, which is the last one in insn order.
3. **Tools:** `build/wf/BattleScene_Update/rtl2txt.py <dump> [func]` condenses an RTL dump to one line per insn (uid, pattern). Diffing `.greg` (before jump2) against `.jump2` shows exactly what cross-jumping did. `getasm.sh [-dX ...]` dumps and extracts the built asm.

### `TextCellsLoadIcon` (0x10C, start score 184; ordinary C)

- `u16 off = hdr[0]*2` added a truncation. `int off` with `(u8 *)hdr + (off + 8)` matches, whereas `hdr + hdr[0] + 4` folds into `ldrh [r4,#8]`. Likewise `u8 cell = a << 5` truncated, so the source uses `int cell`.
- Map indices: precomputed `x0`/`x1` variables (or `s16`) compute the index too early or with `asr`. Writing `map[(u16)a + 0x240]` and `map[(u16)(a + 1) + 0x240]` inline in each statement gives the ROM's interleaving (the index is computed after the gMain base load, and `a + 1` after the first store). It also fixed the a/b register swap (r9/r8).
- Second tile set: any `buf + 0x400 + cell` form through a local pointer gives `(cell + 0x400) + sl`. Naming the global itself (`&((struct TileBufEF00 *)gDuelTextTiles)->b[cell]`) makes CSE's use_related_value rebuild `sym + 0x400` from `sl` first: `movs r0,#0x400; add r0,sl; adds r0,r5,r0`.
- The flag clear is a bitfield store (`->f1 = 0` gives `movs #3; negs`), not `&= ~2`, which gives `movs #253`.
- `wf.py` validate rejects `extern T x asm("gUnk_...")` symbol aliases inside the region, because its ASMSTR regex treats the label as an instruction. The struct is therefore reached by casting the `u8[]` extern ([[agent-tooling]]).

### `DuelInfo_DrawSpellZone` (0x3DC, start score 126; FAKEMATCH)

- `s16 len` added a sign extension before `cmp r0,#15`. The callee returns `int`, so the local is `int len`.
- Name-string address: the same FAKEMATCH as `DuelInfo_DrawCard`, `register u32 off asm("r0") = id << 6; tbl + off`.
- The three zone-address computations used one shared `z`/`pl` local, so all three got the same pseudos and registers, while the ROM uses separate pseudos per use (r1, r0, r2). The fix is a `static inline` helper, `ZoneF270(player, slot) { int pl = player & 1; return slot*0x94 + pl*0xD64 + base; }`. Each inline expansion gets fresh pseudos, and the separate `pl` statement keeps the ROM's evaluation order (pl first, then `slot*0x94`, then `0xD64*pl`). A macro without the `pl` temporary swapped the player/slot registers and the multiply order (138).
- `x -= 0x3C` must come before the second zone read.
- The final `if (n == 0x479) ... else if (n == 0x5A8)` must be a `switch` with two cases (ROM: `beq, beq, b default`).
