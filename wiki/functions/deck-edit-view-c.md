---
title: Unit deck_edit_view
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit deck_edit_view

Card-list / Deck Edit screen code (`0x0806D51C`–`0x0806ED44`), between [[deck-edit-stats-c]] and [[deck-edit-c]]. All functions are Thumb.

- `DeckEdit_InitListView` now matches in C, including its unconditional first-loop increment and object/list register allocation.
- `DeckEdit_Update` (0x1194 bytes) matched in workflow wave 1 (2026-10-01); the unit is now complete (5/5 C).

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x0806D51C` | 0x118 | matching | ResetCardListView (hyp.) | Reset scroll, animation and view-state fields. |
| `0x0806D634` | 0x10 | matching | ClearSpriteTile (hyp.) | Clear one 0x20-byte tile at an indexed destination. |
| `0x0806D644` | 0x560 | matching | InitCardListView (hyp.) | Load graphics/palettes, draw visible cards and configure windows/backgrounds. |
| `0x0806DBA4` | 0xC | matching | EnterCardListView (hyp.) | Run the initializer and complete the scene step. |
| `0x0806DBB0` | 0x1194 | **matching** (wave 1, 2026-10-01; FAKEMATCH) | UpdateCardListView (hyp.) | Animate scrolling, handle list/menu input, draw and fade. |

Actual C count is 5/5 (all 0x1828 bytes) since wave 1; before that 4/5 (0x694 of 0x1828 bytes), with only the frame handler in assembly.

## State and calls

Field offsets below are relative to `gDeckEdit`; the accesses are verified from assembly, while descriptive field names remain hypotheses.

| Offset | Shape | Use |
|---|---|---|
| `+0x618`, `+0x61E` | transition record, byte state | Enter/leave the view; state 2 completes this frame handler. |
| `+0x620` | `u16[3]` | Current row for each card list. |
| `+0x628`, `+0x62A` | byte, signed halfword | Scroll tween state and step; also based at `gDeckEditScrollEase`. |
| `+0x630`–`+0x63E` | scroll halfwords and two bytes | BG positions, artwork buffer selector and direction (1/2 vertical, 3/4 horizontal). |
| `+0x1494`, `+0x14A0` | `u16[2][3]`, `u8[3]` | Per-list counts and active row/category. |
| `+0x1710` bit 0 | flag | Redraw the list as a horizontal slide crosses its midpoint. |
| `+0x1718`, `+0x18B0` | object and row-object areas | Graphics/object initialization and animation. |
| `+0x18AC` | signed halfword | Brightness ramp from -0x400 toward +0x400. |
| `+0x1BB4`–`+0x1BB7` | four bytes | Previous/next arrow dirty and animation states. |
| `+0x1BB8` | row records with 0x10 stride | Clear a row's byte `+0xC` when the corresponding card is out of range. |
| `+0x1C14`, `+0x1C20` | bytes | Row/menu animation states. |
| `+0x1C1C` | byte | Current list selector; absolute alias `gDeckEditCurList`. |
| `+0x1C3C` bits 15–17 | three-bit selector | Seven menu choices, skipping current-list choice. Spans bytes `+0x1C3D`/`+0x1C3E`. |
| `+0x1C48` bits 0, 1–4 | flag and mode | Browse/menu selection and outgoing screen mode. |
| `+0x1C58` | halfword | Row animation timer, initialized to 30. |

`DeckEdit_EnterListView` directly calls D644; D644 calls D634 to clear four tile slots. Both large functions call `DeckEdit_GetListCard` for card lookup and the `DeckEdit_DrawListRowName`/`DeckEdit_DrawCursorRowName`/`DeckEdit_InitFrameSlot` rendering helpers. DBB0 also drives the scroll tween, arrow updates, menu actions and frame drawing. Input comes from `gMain + 6`, masked to ten bits; switches compare complete key combinations rather than independent button tests. Card classification uses `gCardIdToNumber` and `gCardStats`.

## Semantic repair and matching attempts

> [!warning] Contradiction resolved: first-loop increment
> The earlier parked C draft incremented k only inside the visible-row condition. In `asm/nonmatching/deck_edit_view/DeckEdit_InitListView.s`, the negative-row branch lands at `0x0806D846` before the shared `k = (u16)((s16)k + 1)` update. The parked source now increments on both paths. The similar `ProhibitCardSelect_InitListView` ([[deck-edit-c]]) deliberately differs: its skip target `0x0806F5F2` comes after the k update, so its conditional increment must remain unchanged. Transplanting this repair to that sibling would be incorrect.

- The original D644 draft had 194 changed disassembly lines; the faithful unconditional-increment draft has 192. A small score improvement does not measure the significance of a behavior correction. The active assembly preserves the original behavior throughout.
- Eight initialized object-base/signed-loop/index variants did not improve the older draft. Explicit object pointer r5, loop index r7, or a separate signed-k cache changed other allocation and literal placement; none was enabled. After semantic repair, separately caching signed k also worsened the diff. Current private reference: `build/manual_late_next/DeckEdit_InitListView.increment.best.c` and `.diff`.
- The related initializer and view drafts in [[deck-edit-c]] and [[deck-edit-prohibit-c]] share palette/object/state layouts, but their guards, index updates, and caller argument narrowing must be checked independently.

## Large-function pass

- Baseline whole-unit check: `unit bytes MATCH`, 0x1828 bytes; actual C coverage was 3/5 at the start of this pass.
- `DeckEdit_InitListView`: current baseline measured 163 normalized changed lines at 0x560 bytes. The older `u16 k` candidate is worse (190 lines). Testing local widths, declaration order, and explicit next-index scheduling reduced the parked draft to **20 normalized changed lines, still 0x560 bytes**.
- The useful form has signed-halfword `k`, keeps its update outside the visibility guard, and writes the slot argument as `-k + 2` to retain the target NEG/ADD. In the second loop, calculate the current position and count-minus-one first, then `next = j + 1`; use `next` in the first lookup and loop update, while retaining fresh state reads after calls. This also fixes object-base allocation without a binding. Remaining differences are scratch-register choices at the second-loop preheader and initial address loads.
- Reusing a pointer for sprite VRAM and the object array, explicitly binding that pointer to r5, and the older unsigned-halfword update variants did not improve the draft. No failed candidate was enabled.
- A complete readable `DeckEdit_Update` draft was checked against assembly and compiled in private scratch space. The first version compiled at 0x1074 bytes, before two omitted fifth draw-call arguments were corrected.
- The D644 permuter reached score zero after 3,200 iterations. Its initialized constant temporaries and equivalent expressions were applied with `FAKEMATCH` annotations, and **whole-unit bytes MATCH, 0x1828 bytes**, with D644 active in C. No register binding or inline assembly was needed.
- A 16-combination minimization retained only one permuter change: initialize `rowObjects = gDeckEditObjAffine` immediately after the first object flag update and use it in both row loops. The extra integer temporary, expanded OR assignment and commuted increment are unnecessary and were removed. This one behavior-neutral pointer-staging `FAKEMATCH` resolves the final 20 normalized lines; the complete unit still matches.
- DBB0 now has a complete readable, compiling draft parked under `#if 0`. Independent assembly review checked all cases and corrected a missing repeated `-1` argument in both `OamListAddSpriteGroup` calls. The draft started at 0x1074 bytes with 1,839 normalized changed lines. Typed field overlays and menu assignment expressions brought it to **0x1190 bytes with 196 lines**. The target is 0x1194; remaining differences are menu bitfield updates and card classification.
- DBB0's prefix up to the menu handling and its final draw/fade tail now match normalized assembly. A `volatile u32` fill local reproduces original stack slots and scheduling; 25 aggregate/alignment/scope alternatives did not improve it. Assignment of pointers inside draw-call arguments and explicit complete/fade labels recover the tail. Two empty `asm("")` barriers after menu transition calls prevent tail merging; they emit no instructions and are marked `FAKEMATCH`. A 30-minute, two-worker permuter run was started from this state.
- DBB0 reached its **exact 0x1194 target size**, first with a table-base constraint (174 normalized lines), then with an `int` card-number local and absolute pointer views of the existing number/stat tables (**108 normalized lines**). These views avoid sharing the statistics-table base with the fallback lookup. No ROM data is defined or changed. Ordinary extern-preserving cast/offset/wide-pointer variants did not retain the exact classifier. Raw byte comparison confirms `0x0806DBB0`–`0x0806E6E4` and `0x0806E8E8`–`0x0806ED44` already match; all remaining changes are in the intervening menu section. The parked draft retains the stronger 108-line version, and the earlier permuter was stopped and reseeded from it.
- Later DBB0 state: **0x1194 bytes, 66 normalized changed lines**. Declaring the three-bit menu selector as `u16` inside its mixed-width word preserves bits 15–17 while recovering the target's separate store/compare masks and 16-bit decrement narrowing. Staging a shared phase-byte pointer through empty constraints recovers the common update block. Remaining differences are register allocation within menu handling; both whole-unit assembly fallback and the four active C functions remain exact.
- Permuter preparation strips standalone `asm("")` lines, including the transition barriers. Their equivalent explicit immediate-input form `asm("" : : "i"(mode))` survives preparation and preserves the same bytes. The private search uses `#pragma _permuter randomizer start/end` around only the two menu-direction cases, keeping the full function as the compilation/scoring target; the pragmas are absent from the unit source. The prepared 66-line candidate starts at permuter score 205.

## Card-list frame handler matched (wave 1, 2026-10-01)

`DeckEdit_Update` (0x1194, 4.5 KB) matches, starting from the parked v4 draft (score 43). Working notes and dump recipe: `build/wf/DeckEdit_Update/` (`dump.sh`, run with `tools/dr sh`).

- The only remaining difference was the menu DPAD_LEFT tail (`phase = 3; PlaySE(0)`). The ROM forms the pointer as `ldr r2,=gDeckEdit; ldr r4,=0x1C3D; adds r2,r2,r4` and loads the byte into r5, so the tail is identical to the DPAD_RIGHT one from `movs r0,#8` on and the two are cross-jumped.
- Why: the `0x1C3D` offset and the byte load are **reload registers** (the `and` takes the byte as a `(subreg (mem:QI))` operand). Reload picks spill registers round-robin from `last_spill_reg` among the registers free at that insn. The previous reload was r2 (0x1C1C), so a plain version gets r3/r4; in the ROM r3 is not free at the add, so the offset goes to r4 and the byte to r5.
- Fix (FAKEMATCH): base pinned to r2, plus an empty asm pair that keeps r3 live across the add:

      register u8 *b asm("r2") = (u8 *)&gDeckEdit;
      register u32 busy asm("r3");
      struct DeckPhaseByte *p;
      asm("" : "=r"(busy));
      p = (struct DeckPhaseByte *)(b + 0x1C3D);
      asm("" : : "r"(busy));
      p->phase = 3;

  The asm insns sit before the cross-jump point, so the merge from `movs r0,#8` still happens.
- Failed: plain `gDeckEdit.phase = 3` (local-alloc gives the pointer r0, 78 lines); a folded `register p asm("r2") = sym + 0x1C3D` (43); base r2 plus offset r4 pinned (54); duplicating the tail in both LEFT arms (224, CSE then reuses the hoisted base).
- Its two 4.1 KB twins, `TradeCardSelect_Update` ([[deck-edit-prohibit-c]]) and `ProhibitCardSelect_Update` ([[deck-edit-c]]), matched in wave 2 with a refined version of this tail trick. See [[matching-tricks#Register allocation priority and reload rotation]].
