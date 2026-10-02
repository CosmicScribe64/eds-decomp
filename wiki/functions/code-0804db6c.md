---
title: Unit code_0804DB6C (battle-phase effect steps and per-zone scans)
type: function
status: draft
confidence: low
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit code_0804DB6C

`0x0804DB6C`-`0x0804EFEF`, Thumb, `old_agbcc -O2`. Source: `src/code_0804DB6C.c`. Follows [[code-0804cb58]]. A mix of battle step machines (step byte at `0x020192E0+0x1B16`, bits 1-8, as in [[code-0804cb58]]; secondary step byte at `0x020192E0+0x1B22`/`+0x1B20`) and per-zone scans of both players' field zones (`0x0201930C + p*0xD64 + z*0x94`, zone word `w0 & 0xFFF` = card number). The unit ends with the effect dispatcher `sub_0804E948`, which calls the scans `sub_0804E538`, `sub_0804E5B4`, `sub_0804E780` and `sub_08046D3C`/`sub_08046CB0` from [[code-08046738]].

Unit status: `unit bytes MATCH`, **9/11 functions in C** after workflow waves 2-3 (2026-10-01/02: `0x0804E420` in wave 2, `0x0804E780` in wave 3); 2 stay `INCLUDE_ASM` (`0x0804DC88`, `0x0804E948`, both with complete C drafts under `#if 0`). After wave 1: 7/11 (2026-10-01: `sub_0804E240`, `sub_0804E5B4` added); before wave 1: 5/11 (0x29C bytes of C). The exact whole-unit check is 11/11 including assembly fallbacks, 0x1484 bytes.

> [!warning] Contradiction
> The table row for `0x0804E420` (before 2026-10-02) gave its step byte as `0x0201AF00`. The wave 2 match (2026-10-01, `build/wf/sub_0804E420/NOTES.md` and the matched source) uses `0x020192E4 + 0x1B1C` = `0x0201AE00` (the ROM literals are `0x020192E4` and `0x1B1C`; the default case increments the same byte as `0x020192E0[0x1B20]`). Resolved in favour of the matched source; the row is corrected below.

| Address | Size | Status | Purpose (hypotheses) |
|---|---|---|---|
| `0x0804DB6C` | 0x11C | matching C | `int (player)`: bails out (returns 1) while `sub_08021628()`; calls `sub_08046B54(1-p, defSlot)`; if the defender zone holds a card, `sub_08008A1C(p) > 0` and `sub_0800A78C(1-p, def, 0x4DB)`, calls `sub_08019078(1-p, pos, p \| sub_08008A44(p)<<8)`; if card 0x602 is present on either side, `sub_080197E0(p, gUnk_086249F8[0])` and returns 1, else resets the step and returns 0. An initialized `+r` copy constraint retains the ROM's `q` copy before `(u8)` narrowing |
| `0x0804DC88` | 0x5B8 | complete C draft in `#if 0`; asm active | step machine (9-entry jump table on the halfword step); processes linked cards, flagged graveyard entries and prompts, then sets the player's byte-9 bit 4 |
| `0x0804E240` | 0xDC | **matching** (wave 1, 2026-10-01) | `int (player)`: scan both players' zones 0-4 for a card with zone byte `+0x8C` bit 1 (destroy via `sub_08018544`) or bit 2 (message 0xA2, `sub_08019078`, or destroy if `sub_08008A44(1-i) < 0`); returns 1 when none. Ordinary C (see below) |
| `0x0804E31C` | 0xA0 | matching C | `int (a)`: table `gUnk_0819D1D8` of handler pointers indexed by the step (bits 9-16 of the word at `0x020192E0+0x1B14`); if present call it with `a`, on nonzero result reset the halfword step and advance the word step (return 0); if absent send message 0x54 (0x8054) and return 1. An initialized shifted-step constraint preserves the second index extraction; the player flag is evaluated before the message value |
| `0x0804E3BC` | 4 | matching | `void`, empty |
| `0x0804E3C0` | 0x60 | matching | queues a sprite draw `sub_08076714(v, 0x40C0, 0xF364, tab[(gMain+0x485E >> 1) & 15] << 16)` where `v` = 0x2800A0 or 0x7000A0 depending on `*(0x0201CFB0+0x810) - byte(0x0201CFB0+4) <= 0x47` |
| `0x0804E420` | 0x118 | **matching** (wave 2, 2026-10-01) | multi-step routine (step byte `0x020192E4+0x1B1C` = `0x0201AE00`; formerly given as `0x0201AF00`, see the callout above), keyed on the active player (bit 1 of `0x020192E4+0x1B0E`): if that player's byte-9 bit 2 is set it is cleared (return 1); else step 0 message 0x50 (0x8050), step 1 `sub_080199E0(1, 1)` (return 1) when the bit is set or `sub_08024134(who, 0xD, 0)`, step 2 sets `0x0201CFB0+0x808` bit 3, step 4 clears `0x0201CFB0+0x85C` and `sub_080199E0(0, 1)` (return 1); other steps advance on key 0x100 when `sub_0804A1C8() == 0`. See Wave 2 matches below (historical: the ROM keeps constant 1 in a register and ANDs it with a re-extracted bit, `PS[1 & who]`, which gcc folded) |
| `0x0804E538` | 0x7C | matching | for zones 0-4 of `player` with a face-down card (`flags6 & 2`) whose card key (`0x08622AB4`) is 0x16: message 0x73 (0x8073 for player 1) then `sub_08018AE8(player, i, 0)` |
| `0x0804E5B4` | 0x1CC | **matching** (wave 1, 2026-10-01) | 3-step routine on the step byte `0x020192E0+0x1B22` with zone counter `+0x1B23`: scan for a face-down card with key 0x228, then card-name prompt, then message 0x43 + `sub_08019078` per found zone. Ordinary C (see below) |
| `0x0804E780` | 0x1C8 | **matching** (wave 3, 2026-10-02) | two passes over the face-down zones of `player`: set `found` if a level <= 3 monster (level from the `0x08621DE0` table, as in [[code-08046738]] `sub_08046F20`) with byte `+8` bit 0 clear exists and card 0x52A is on the field (message 0x73), then destroy them / send message 0xA6. See Wave 3 matches below (historical: the ROM steps a zone pointer while counting `i` up; the old draft got a multiply per iteration or a counted-down loop) |
| `0x0804E948` | 0x6A8 | complete C draft in `#if 0`; asm active | effect dispatcher: calls the scans above, `sub_0801FBCC`, `sub_080088A4`, `sub_0800AA40`; handles linked zones and graveyard notifications |

## Structures (hypotheses)
- Duel step: the word at `0x020192E0+0x1B14` has bits 9-16, immediately followed by the halfword step at `+0x1B16` (bits 1-8, or word bits 17-24); these fields are adjacent, not aliases; `sub_0804DB6C` writes 1 through the word view and 0 through the halfword view (ROM does both stores).
- Player flag byte at `0x020192E4 + p*0xD64 + 9`: bit 2 cleared by `sub_0804E420`; `+8` bits 0-1 in [[code-0804cb58]].
- Zone: `+6` bit 1 face-down, `+8` bit 0 (tested by `sub_0804E780`), `+0x8C` bits 1-2 (`sub_0804E240`).

## Matching tricks
- **`u32 id = ID(zn); u32 n = id;` and index `[(u16)id & 0x7FF]`** reproduces both the copy of `id` into a second register (`adds r1,r2,#0`) and the `ldr r3,=0x7FF; adds r0,r3,#0; ands r2,r0` mask form (`sub_0804E538`). Without the `(u16)` cast the mask is loaded straight into r0.
- **Constant loaded in argument order** (`sub_0804E3C0`): a comma expression `(t = tab, m = (u8 *)&gMain, t[(*(u16 *)(m + 0x485E) >> 1) & 0xF] << 16)` inside the call gives constants first, then the table literal, then `gMain` unfolded (`ldr r0,=gMain; ldr r3,=0x485E; add r0,r0,r3`); a plain pointer local set before the call loads `gMain` too early, and a folded `gMain + 0x485E` needs no local.
- **Function-pointer table entry loaded twice** (`sub_0804E31C`): retain `packed = *(u32 *)step << 15` and add an empty initialized `+r(packed)` constraint after the null check. Both table accesses use `packed >> 24`. The volatile table retains both reads, and the constraint retains both index extractions. Extract the player flag before initializing the message to preserve the final argument register ordering.
- **Bitfield struct of 4 bytes or less** must be padded (`u8 pad[6]`) to be read with `ldrb`/`ldrh` (`struct BHdr`, `struct StepHalf`).

## Accepted conversions (2026-09-30)

This pass accepted `sub_0804DB6C` and `sub_0804E31C`, adding two functions and 0x1BC (444) bytes of C.
Both keep their original `int(int)` ABI and reviewed call order. The documented empty asm
constraints use initialized, meaningful values and emit no instructions; neither pins a register.
`tools/dr python3 tools/check.py code_0804DB6C` reports `11/11, unit bytes MATCH, 0x1484`.
Private baselines/candidates are under `build/bigguns-effects/` (`db6c-copy`, `e31c-tail`).

## Continued private frontiers (2026-10-01)

No further C function is accepted in this section. The complete original fallbacks remain active.
The saved checkpoint log is `build/bigguns-effects/code_0804DB6C-checkpoint.log` (11/11 and
0x1484 whole-unit bytes exact, including fallbacks). Private candidates below were compiled and
compared with ROM bytes; their behavior was audited against source assembly, without executed
differential cases.

- **E240** (matched in wave 1, see below): `build/bigguns-effects/e240-final/best.c` is 0xDC, with only 8 differing bytes.
  The remaining difference is the order of three invariant calculations at loop entry: ROM computes
  `i & 1`, `(u8)(1-i)`, `(u8)i`; C computes the last, the middle, then the first. All subsequent bytes
  match. The useful changes are a `u16` message, byte bitfield view for zone flags, and an explicit
  shared return label after the first destroy call; an empty memory barrier keeps the two destroy
  calls distinct. `e240_final.py` reproduces the frontier from `e240-loops/best.c`.
  Explicit outer-loop masked/opponent/current locals, an input-only mask constraint, packed-position
  types/forms, signed-16 player strides, and equivalent address trees did not remove the final order
  difference. Outer-loop staging loses the opponent's strength reduction and the original stack spill.
- **DC88:** `build/bigguns-effects/dc88.c` is the complete ordinary-C reconstruction; reproduce with
  `tools/dr python3 build/bigguns-effects/dc88_build.py`. First build is 0x574 versus 0x5B8.
  Source audit covers the cross-byte selected-player field at +0x1B17/+0x1B18, graveyard bit-24 and
  bit-28 scans, the linked opponent zone in case 1, case-7 halfword message arguments, and adjacent
  step resets in the default branch. Structured player records, physical-address views and a narrow
  card-number helper change hoisting/register use but do not match. Explicit state-register binding
  substantially enlarges the result and is not a useful direction.
- **E948:** `build/bigguns-effects/e948.c` is a complete ordinary-C reconstruction; reproduce with
  `tools/dr python3 build/bigguns-effects/e948_build.py`. First build is 0x6C0 versus 0x6A8, frame 4
  instead of 8. `e948_views.py` recovers typed link/disabled fields and a prechecked case-4 scan,
  reaching 0x6AC; `e948_one.py` reaches the target size with an initialized mask constraint but still
  has frame 4 and substantial register/hoisting differences. The m2c draft's case-4 unrecognized-card
  path was misleading: the ROM increments the zone and continues scanning. Case 10 resets its zone
  cursor on every invocation; case 11's increment therefore must not be used to infer resume behavior.
  The source audit also preserves case 0's fallthrough and case 20's graveyard bit-23 notifications.

The ordinary DC88/E948 drafts and cleaned E240 near miss are now parked in the source under
`#if 0`; previous source is retained in `build/bigguns-effects/code_0804DB6C.pre-frontiers.c`.
`check_parked.py` confirms all three compile independently at the sizes above, and the cleaned E240
draft still differs by exactly 8 bytes. A masked-player-only initializer fixes its first calculation's
placement but exchanges the remaining two byte conversions (`e240-maskonly`); explicitly scoping
the opponent byte loses the desired strength reduction. E948's phase-10 signed masked stride
recovers the target frame of 8, still nonmatching at 0x6A0 (`e948-frame/u32-with-frame.body.c`).
The `e948-roles` pinned-player experiment is rejected: its generated code reuses the player register
as an induction value before later calls. Do not promote that diagnostic candidate on a score alone.
The latest active fallback-inclusive check is `code_0804DB6C-parked-check.log`, still exact 0x1484.

## `sub_0804E240` hoist order (2026-10-01)

> Superseded: the wave 1 match below found the actual mechanism (GCSE PRE plus loop.c's two passes), which refines the source-order explanation here.

The inner loop's hoisted invariants follow the source order of their first use within one loop pass. The parked draft now declares `u8 opp = 1 - i` after the zone pointer (with `iu` first) and is down to 4 diff lines: the ROM hoists `(i & 1)`, `(u8)(1 - i)`, `(u8)i` while the build puts `(u8)i` first. Placing `iu` after `opp`, or computing `side = i & 1` in the outer loop, is worse.

## Wave 1 matches (2026-10-01)

Both scans matched in ordinary C with no FAKEMATCH; the unit check reports 11/11, bytes exact. Working notes: `build/wf/sub_0804E240/NOTES.md`, `build/wf/sub_0804E5B4/NOTES.md`.

### `sub_0804E240` (0xDC)

Only the order of the inner-loop invariants in the preheader differed (ROM: `i&1`, `(u8)(1-i)` strength-reduced as `r4>>24`, `(u8)i`; draft: `(u8)i` first). Root cause, from `-dG`/`-dL` dumps and the agbcc `loop.c` / `gcse.c` sources:
- `u8 iu = i` at the top of the loop body is computed on every path, so GCSE PRE (LCM) hoists `i<<24` to the end of the outer block, ahead of everything loop.c moves.
- loop.c runs twice. Pass 1 moves the constant 1, then the forced `1-i` / `<<24` / `>>24` chain. `i&1` (life 3) is "not desirable" in pass 1 and only moves in pass 2, so it lands after `opp`. The threshold drops by 3 after each move (T0 is 12-13).
- Fix 1: `int side = i & 1;` as the first statement of the inner body. Its lifetime now covers the `j*0x94` computation, so pass 1 finds it desirable and moves it before the opp chain.
- Fix 2: write `(u8)i` inline as the first OR operand, `pos = (u8)i | (u8)j << 8;`. It is conditional, so PRE leaves it alone; its temporaries stay inside one basic block, so loop.c may move them, and the longer temp life makes pass 2 move them last.
- Required: `int i = 0` with `for (; i <= 1; i++)` (`for (i = 0; ...)` scores 10), the `u8 *zb` base (a literal `0x0201930C` scores 46), and the shared `found: return 0;` label (plain returns score 72). The empty memory barrier of the older frontier is no longer needed.
- Failed: `u8 iu` after opp at the loop top (18); `iu` inside the b2 block (84, never hoisted); `(u8)i` as the second OR operand (93, combine merges it).
- Tooling note: this function was byte-exact before `wf.py` could apply it; the 2-byte `.align 2, 0` pad after the final `bx r1` made the size delta nonzero. `check.py` now ignores a delta that is only that pad (see [[agent-tooling]]).

### `sub_0804E5B4` (0x1CC, start score 40)

The ROM preheader hoists the constants `1` (r8), `g = e` (a copy), `st` (a copy), 0x7FF (ip) and 0x228 (r7) in that order, keeps `e+0x2C` inside the loop, and caches the step byte across the `h14` store (`adds r0, r3, #1`).
- Inside the loop, access the duel state through the global `gUnk_020192E0[...]` instead of the local `e`. The SYMBOL_REF base lets alias analysis keep the step value across the store to `gUnk_0201AE60.h14`; the loop's symbol loads are hoisted, and cse2 turns them into the copies `g = e` and `st = r0`.
- `int side = player & 1;` at the top of the loop body, used as `idx * 0x94 + side * 0xD64`. The constant 1 is then the first movable, and loop.c matches it with the `h14` constant 1; the `& 1` and the multiply are CSE'd back onto the base register.
- Failed: an explicit cached `int v = *s2 + 1` (95); `(player & 1) * 0xD64` inline after the idx term (8: the 1 is hoisted after g and st); base term first (30); `int side = (player & 1) * 0xD64` (30); an extern `gUnk_08622AB4[]` instead of `((const u16 *)0x08622AB4)` (41).
- Debug aid: `build/wf/sub_0804E5B4/dump.sh` with `-dL` dumps loop.c's movable decisions. See [[matching-tricks#Loops, scope, escapes and live ranges]].

## Wave 2 matches (2026-10-01)

Working notes: `build/wf/sub_0804E420/NOTES.md`.

### `sub_0804E420` (0x118, start score 61; ordinary C)

Score 0 after about 10 experiments:
1. **`PS[1 & who]` with the constant 1 kept in r1.** Write both player indexes as `gUnk_020192E4[sb->who & 1]`. CSE replaces the second `1` with the first block's pseudo; combine folds the AND in the first block (`nonzero_bits`) but cannot reach the second one across the branch (LOG_LINKS are per basic block), so `mov r1,#1` stays in block 0 and `and r1,r0` in the second. The flags byte must be read as `u8` (the old `s8` gave `lsl/asr`).
2. **Three separate `ldrb/add/strb; b` increments.** The default case must be `if (sub_0804A1C8() != 0 || !(keys & 0x100)) return 0; inc; return 0;`. Then no label sits before the final `r0 = 0`, jump2's cross-jump creates a new label there, and jumps to new labels (uid >= `max_uid`) are never cross-jumped with each other. With `if (A && B) inc; return 0;` the if's end label is reused and the increments merge (score 108, 20 bytes short); volatile or odd increment forms give the same post-reload RTL.
3. **`0x0201CFB0+0x85C` as `ldr sym; ldr 0x85C; add`**: a struct member through a cast, `((struct E420Screen *)gUnk_0201CFB0)->unk85C = 0`. `*(u32 *)(arr + 0x85C)`, `&arr[0x85C]` and `((u32 *)arr)[0x217]` all fold into one literal.
4. Case 4 stores before the call; case 1's `sub_080199E0(1, 1); return 1;` then cross-jumps into case 4's tail by itself.

## Wave 3 matches (2026-10-01/02)

Working notes: `build/wf/sub_0804E780/NOTES.md` (short: rewritten after the workflow session was accidentally closed; variants in `exp/`, `try.sh` prints the score and loop.c's loop-1 decisions from the `-dL` dump). The tricks are also in the comment above the function in `src/code_0804DB6C.c`.

### `sub_0804E780` (0x1C8, start score 194; ordinary C)

- Zone addresses go through the symbol `gUnk_0201930C` (cast to `int`), so GCSE keeps one copy of the base in sl as in the ROM.
- Loop 1 computes the zone address twice, both times with the player term first: `(player & 1) * 0xD64 + i * 0x94 + (int)gUnk_0201930C`. loop.c hoists the 0x94 only in its second pass, so the zone pointer is strength-reduced (`add r3,#0x94`) while `i` still counts up. A hand-stepped pointer gives a reversed (counted-down) counter, and the old draft got a multiply per iteration or a counted-down loop.
- Loop 2 writes the first address as `i * 0x94 + player term` and the second as `player term + i * 0x94`.
- How the ROM's other registers (constant 1 in r7, 0x7FF in r6, per the old draft note) came out is not recorded in the NOTES. See [[matching-tricks#Loops, scope, escapes and live ranges]].
