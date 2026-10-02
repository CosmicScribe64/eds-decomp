---
title: Unit code_0804EFF0 (duel confirmation menu and turn-end effect helpers)
type: function
status: draft
confidence: low
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit code_0804EFF0

The unit covers `0x0804EFF0`-`0x08050A6F` (Thumb, `old_agbcc -O2`), and its source is `src/code_0804EFF0.c`. It follows [[code-0804b640]] and uses the duel global `0x020192E0` (step byte `+0x1B20`, current player = bit 1 of `+0x1B12`), the player state array `0x020192E4` (0xD64 per player), the zone array `0x0201930C` and the UI block `0x0201AE60`.

Unit status: `unit bytes MATCH`, **9/10 functions in C** after workflow waves 2-3 (2026-10-02: `0x0804F168` in wave 2, `0x0804EFF0` and `0x0804F7A4` in wave 3); only the 3.6 KB turn-end machine `sub_0804FC4C` stays `INCLUDE_ASM`, one of the last two giants.

> [!warning] Contradiction: `sub_0804FC4C` now matches
> The status line above (waves 2-3) says `sub_0804FC4C` stays `INCLUDE_ASM`. It matched later on 2026-10-02 (commit `f637be2`,
> "Match sub_0804FC4C (3.6 KB giant)"), so the unit is **10/10 functions in C** and `src/code_0804EFF0.c` has no
> `INCLUDE_ASM` left (checked 2026-10-02). The [workflow-round section](#sub_0804fc4c-workflow-round-2026-10-0102-still-nonmatching)
> below is history; the tricks of the final match are not written up yet. See [[overview]]. Before wave 2: 6/10 (0x494 bytes). The exact whole-unit check is 10/10 including the fallback, 0x1A80 bytes.

| Address | Size | Status | Purpose (hypotheses) |
|---|---|---|---|
| `0x0804EFF0` | 0x178 | **matching** (wave 3, 2026-10-01) | `(player)`: for zones 0-4 of `player` with a face-down card (flag `+6` bit 1): clear bit 4 of byte `+0x8C`; card number 0x52 (no `+7` bit 5) sets a flag; 0x62 with `(f6 >> 2) & 15 <= 3` calls `sub_08046738(player, i)`; on the flag, for every face-down monster zone of both players with `sub_0800C8BC(p,j) == 2` and `w4 <=` this zone's `w4`, `sub_08017AB4(player, player \| i<<8, p \| j<<8, 2)`. ROM allocation: i in r8, i+1 in r7, `(u8)player` in r9 |
| `0x0804F168` | 0x1A8 | **matching** (wave 2, 2026-10-01) | 4-step machine on `0x020192E0+0x1B20`: 0 `sub_080240A8(cur, 0xB)`; 1 if player-state byte `+9` bit 3 set: clear it, maybe `sub_0802297C(0xF002)`, `0x02015EE8 += 5`, return 1, else message 1/0x8001; 2 `sub_0804EFF0(cur)` and `(1-cur)`; else clear bit flags in `+8/+9/+0xB/+0xC` of the current player and return 1. The ROM reloads `+9` before the RMW and keeps `&step` in r6 |
| `0x0804F310` | 0x74 | matching | draws the 3-choice menu cursor sprite (`sub_080761F0`, attr 0x431F) at a position from `0x0201AE60` fields (+8 x, +0xA y, +0xE h, +0x14 selection, +0x21); blinks while state (+0x22) is 1 (timer +0x23 bit 1) |
| `0x0804F384` | 0xDC | matching | `int f(void)`: input handling of the 3-choice menu: state 1 counts the timer to 0x3C then state 2; state 2 returns 1; state 0 handles Down (0x80: sel+1 wrapping at 3), Up (0x40), A (1: state 1) and B (2: sel = 2, state 1); sounds via `sub_08077AEC` |
| `0x0804F460` | 0x1F4 | matching C | step machine of the confirmation dialog on `0x020192E0+0x1B20` (0: message 0x52, clear `SelMask` flag0/active; 10: `sub_0804A92C(0)` -> text dialog with menu callbacks `sub_0804F310`/`sub_0804F384`, step 20, else plain text and step 11; 11/20/21 evaluate the menu result `0x0201AE60+0x14`, `sub_0804E31C(0)`; default: run `sub_0804A1C8`, B (2) -> step 10). An empty memory barrier after the case-20 increment keeps its completed store separate from the shared `step = 1` tail |
| `0x0804F654` | 0x54 | matching | `int f(u16 card)`: life-point amount by card number: 0x44B -> 2000, 0x482 -> 700, 0x58C -> 1000, 0x3BA/0x590 -> 500, else 0 |
| `0x0804F6A8` | 0x44 | matching | `int (a, b)`: count of `(i, j)` with i 0-1, j 0-4, `(i,j) != (a,b)` and `sub_0802B9EC(i, j) != 0` |
| `0x0804F6EC` | 0xB8 | matching C | `(player) -> 0/1`: zones 0-9: cards 0x243/0x1A0/0x2DB (face-down, zone <= 4) or 0x428 (not face-down, zone >= 5); returns 1 if `sub_0802CFD0(player, i, 2)`. A `u16` table-index boundary, split rejection guards and an initialized player-offset local in r6 reproduce the ROM |
| `0x0804F7A4` | 0x4A8 | **matching** (wave 3, 2026-10-02; FAKEMATCH) | zone scans (0-4, own 5-10, opponent 5-9) with life-point effects per card number |
| `0x0804FC4C` | 0xE24 | nonmatching (asm; parked draft, wf score 656) | turn-end state machine, queued card effects, random level destruction, maintenance costs and sacrifice selection |

## Structs and globals
- `0x0201AE60` (menu block): `+8` u16 x, `+0xA` u16 y, `+0xE` u16 h, `+0x14` u16 selection (0-2), `+0x21` u8, `+0x22` u8 state, `+0x23` u8 timer. `gMain +6` = pressed keys.
- `0x020192E0+0x1B20`: step byte of the dialog machines; `+0x1B26` bit 0; `+0x1B14` word with `stage` in bits 9-16.
- `0x02015EE8` (`+0` byte counter, `+1` byte flags bit 0), `0x02015EF0` two bytes cleared together.
- Zone at `0x0201930C + p*0xD64 + z*0x94`: `+0` card (12-bit), `+4` u16 (`w4`, compared between zones), `+6` bit 1 face-down and bits 2-5 an index, `+7` flags (bit 5), `+0x8C` bit 4.

## Matching tricks
- **`switch ((u8)st)` with `u32 st = state;` and `state = st + 1`** keeps `adds r0, r2, #1` (a plain `u8 st` or `state++` constant-folds the value to 2).
- **Bitfield zone bytes**: `z->b8C_4 = 0` gives the ROM's `mov r5,#0x11; neg` int mask.
- **Case order in `switch`** dictates the block layout; sort the source cases in the ROM order (`0x44B, 0x482, 0x58C, 0x3BA/0x590`) to match the compare chain of `0x0804F654`.
- **Struct pointer local** `struct Ui *u = &gUnk_0201AE60;` gives the ROM's r3 base in `0x0804F310`.

## Helper pass validation (2026-09-30)

Accepted `sub_0804F6EC` and `sub_0804F460`, adding 2 functions and 0x2AC (684) C bytes.
Both retain their original ABI, initialized values and ROM call/control order. `F6EC` pins only the
computed player-byte offset in callee-saved r6. `F460` uses one empty memory barrier after an actual
state store; it introduces no emitted instructions or additional runtime operation.
`tools/dr python3 tools/check.py code_0804EFF0` reports **10/10, unit bytes MATCH, 0x1A80**.
Private candidates are under `build/bigguns-effects/` (`f6ec-pin`, `f460-tail`).

Historical (`F7A4` and `F168` matched in waves 2-3; see [Waves 2-3 matches](#waves-2-3-matches-2026-10-0102)): the next four paragraphs describe the parked drafts before the match.

The first complete private `F7A4` reconstruction was audited against the assembly, including the
otherwise surprising own-side field-zone iteration through **10** and the **player > 99** comparison
in card-number 0x46B. These are actual ROM behaviors and must not be corrected to presumed game
intent. Its current matching experiments are in `build/bigguns-effects/f7a4*.py`; no exact replacement
or executed behavioral equivalence is claimed.

The saved whole-unit checkpoint log is `build/bigguns-effects/code_0804EFF0-checkpoint.log`
(10/10 including fallbacks, exact 0x1A80 bytes). Continued private F7A4 work reached an exact-size
0x4A8 draft (`f7a4-numbers/best.body.c`), but it still lacks the ROM's four-byte frame and hoists the
effect-card table into the register occupied by constant 1 in the ROM. A signed or unsigned 16-bit
masked player stride lowers the positional byte-difference score from 926 to 712 at size 0x4AC
(`f7a4-offsets/best.body.c`); this metric is only a matching diagnostic. Full card-number conversion
logic from a matched sibling, separately staged table pointers, and initialized number/constant
constraints have not recovered the original lifetimes. These are failed experiments, not accepted
changes or executed semantic validation. `f7a4.c` retains the first complete source-audited draft.

The private F168 audit (`f168_views.py`) moves casts before left shifts to make the operations
unsigned before the signed sign-bit test. This preserves the ROM interpretation without relying on
signed-shift overflow. An initialized current-player constraint restores the duplicate mask/address
calculation and flag-byte reload, reaching 0x1A4 versus 0x1A8, but case-1 register roles remain different;
explicit pointer/register experiments were not accepted. The active assembly fallback is unchanged.

The ordinary F7A4 draft is now parked under `#if 0` in the source (0x4A0), and only the defined-shift
correction was applied to F168's existing parked draft (0x198). Their earlier source is retained as
`build/bigguns-effects/code_0804EFF0.pre-frontiers.c`. `check_parked.py` confirms both drafts compile
independently; it does not establish behavioral equivalence. The latest whole-unit log is
`code_0804EFF0-parked-check.log`, still exact 0x1A80 including the unchanged fallbacks.

## Turn-end state machine (`sub_0804FC4C`)

`sub_0804FC4C` has a complete private ordinary-C reconstruction in
`build/bigguns-effects/fc4c.c`, with improving candidates under `fc4c-views`,
`fc4c-lifetimes`, `fc4c-players` and `fc4c-tails`. The assembly audit covers every
switch case and confirms the two 128-byte text buffers, a card word at stack +0x104,
and a total frame of 0x10C. The initial byte-array view compiled to 0xC9C with a
0x110 frame. Restoring structured state fields and a constant-address card-level
table view recovered the exact frame. The `fc4c-tails/pending-label.body.c`
frontier is 0xE2C versus ROM 0xE24, still with substantial register allocation,
constant-lifetime and shared-tail differences. These sizes are compilation evidence,
not behavioral-equivalence evidence, and the assembly fallback remains active.

Verified against the original assembly: case 20 uses signed remainder (`__modsi3`)
for the die roll; the exclusion argument for card 0x47A in case 101 is **-1**, not
255; case 4 returns after processing a matching list entry without incrementing its
cursor; and all eight AI support-card probes in case 110 execute without short
circuiting. The temporary card word is passed onward only when `sub_080195D0`
returns success; its matched implementation writes that output before returning 1.
The reconstructed return and state-update ordering was source-audited; no executed
differential cases or complete behavioral proof are claimed for the private draft.

The later ordinary draft is parked in `src/code_0804EFF0.c`. Restoring state
accesses relative to **0x020192E4 + 0x1B1C** in cases 5/101 retains the player-state
base across calls and recovers the ROM's r9 global/r8 card-constant lifetimes.
`fc4c-statealias/both-player-base.body.c` compiles to 0xE68 with the correct 0x10C
frame; it improves register correspondence despite a larger size caused by remaining
branch-tail differences. The closely sized 0xE28 ordinary variant remains preserved
as `fc4c-scopes/found-branch.body.c`. Full card-number-to-ID conversion logic and an
explicit zero-initialized AI flag recover the constant lifetime in case 3 and the
ROM's first-probe Boolean conversion. All AI probes remain independently executed.

The parked draft also corrects callee argument widths from their reconstructed
implementations (`sub_08055F70` and `sub_08022678` use u16 values); the function's
own ABI remains `int(void)`. Its player-record stride is checked at compilation.
The reviewed copy is `build/bigguns-effects/fc4c-reviewed.c`; the prior live source
is `code_0804EFF0.pre-fc4c.c`. The post-edit whole-unit check remains **10/10 including
fallbacks, exact 0x1A80** in `code_0804EFF0-fc4c-parked-check.log`. This check validates
the active fallback, not the parked draft's behavior.

## Waves 2-3 matches (2026-10-01/02)

Working notes: `build/wf/sub_0804F168/NOTES.md`, `build/wf/sub_0804EFF0/NOTES.md`, `build/wf/sub_0804F7A4/NOTES.md`.

### `sub_0804F168` (0x1A8, wave 2, start score 66; ordinary C)

A rewrite from scratch (66 -> 46 -> 0) replaced the old draft with its r12/r5 pins and asm barriers.
- A local `struct F168Duel *e = (struct F168Duel *)&gUnk_020192E0` with a `players[2]` member at +4, and `struct PS *ps = e->players` in case 1. This gives the ROM's `adds r7, r0, #4` base and r6 = `&step`.
- Each case ends with its own `e->step++; return 0;` or `return 1;`. Cross-jumping merges the tails into case 2 and the default, as in the ROM. A single increment after the switch puts the tail after the default instead.
- The current player is read through a padded `u8` bitfield view (`cur:1` at bit 1), with `& 1` on the index in case 1.
- The bit-3 test is `(s8)(byte9 << 4) < 0` (`lsl #28; bge`). A signed 4-bit field `s8 b9_0:4` tested `< 0` also matches. The `.b9_3` bitfield test gives `movs #8; ands; beq` and loses the duplicate address computation. `== 1` on the 1-bit field failed too.
- The default case addresses the flags byte as `gUnk_020192E4 + 0x1B0E`, with a fresh `ldr` of `0x020192E4`.

### `sub_0804EFF0` (0x178, wave 3; ordinary C)

Score path 249 -> 163 -> 121 -> 34 -> 8 -> 0. The draft was parked at 8 for a while, and the wave-3 queue started there.
1. `int i`, not `u8`: the ROM compares `i` signed without narrowing (`cmp r4,#4; bgt`).
2. The inner zone address is written `(p & 1) * 0xD64 + j * 0x94 + base` (macro `ZB2`). fold swaps the two terms, so `0x94` becomes the first movable of the j loop. loop.c's threshold (13, minus 3 per move) then moves the `(p&1)*0xD64` chain in pass 1. Pass 2 moves `base`, `me + base` and then `(u8)player`, which puts `(u8)player` in r9 in the j preheader and pushes `player` itself to the stack, as in the ROM. With the old `j*0x94 + (p&1)*0xD64`, pass 2 used up the threshold on the `me` chain and `(u8)player` stayed in the loop (249 -> 163).
3. The same `ZB2` order for the i-level accesses gives the `(i*0x94) + poff` operand order (163 -> 121).
4. `i` (r8) and `i+1` (r7, the PRE copy of the loop increment) had nearly equal global-alloc priorities (0.2897 and 0.2892). One extra pre-combine narrowing in the region where only `i` is live tipped them: the `card << 20 >> 20 & 0x7FF` index form (121 -> 34 -> 8).
5. The card table goes through a constant address, `((const u16 *)0x08622AB4)[(card << 20 >> 20) & 0x7FF]`. The table base then becomes a reload instead of a local pseudo, which advances reload's round-robin by one slot. As a result the `poff` reload lands in r4 and the constant 1 in the p preheader in r5 (8 -> 0).
- Failed: an `off` variable in the p body (204), `*(u16 *)` loads for `w4` (177, 241), `TBL(card)` without `<< 20 >> 20` (58), `[card & 0x7FF]` (50). A 2D-array struct view `&zp[p&1].zones[j]` gave the same code as the macro.

### `sub_0804F7A4` (0x4A8, wave 3, start score 237; FAKEMATCH)

End-of-turn zone scans: loop 1 handles the empty-zone mask message, loop 2 the own monster-zone life effects, loop 3 own zones 5-10, loop 4 opponent zones 5-9, and a final 0x5A6 check.
- The zone base is relative to the player block: `gEndPS_020192E4 + 0x28`, an alias of `gUnk_020192E4` typed as an array of a 0xD64 struct with `life` at +0 and `zones[]` at +0x28. CSE then derives `base28` from the base (`movs r1,#40; adds`, and `r9 - 40` in case 0x46B).
- Effect card ids use a block-scoped `int number = 0x58B;` with `((const u16 *)0x08623DF4)[number]`, one variable per block. The constant-int table reloads each time, while the extern symbol was hoisted into `sl`. The separate scopes give r4 for 0x58B and r6 for the others.
- Case 0x46B is `gEndPS_020192E4[player > 99].life`. Only a real ARRAY_REF keeps `cond ? 0xD64 : 0` separate from the base. fold() turns pointer-cast and `(u8 *)base + cond` forms into `cond ? base + 0xD64 : base`.
- Loop 3: `int p = player & 1;` as the first statement, so `p` is hoisted and spilled to `[sp]`, which gives the 4-byte frame. **FAKEMATCH:** the zone reads are wrapped in `do { ... } while (0)`. The LOOP_END note ends CSE1's extended basic block there, so `0xD64` and `base28` are not long-lived at loop time and stay in the loop. CSE2 (after loop) then reuses them for case 0x46B as `sl`/`r9`.
- Loop 1: `u32 id1 = 0x527;` at the function top with `((const u16 *)0x08623DF4)[id1]`. loop.c hoists the shift/add group and combine folds it into the `0x08624842` literal in r9. A direct `gUnk_08624842[0]` was not hoisted. `int lo = ...bB >> 4;` followed by `((bC & 1) << 4) | lo` reproduces the split-bitfield load and OR order.
- Diagnosis: compiling with `-fno-cse-skip-blocks` showed that the 0x46B merge came from CSE path following.
- Failed: the old exact-size draft with `asm("" : "+r"(number))` (389). Wrapping the 0x589 count block in `do/while(0)` left the skip label after LOOP_END, so CSE1 still merged (298).

## `sub_0804FC4C`: workflow round (2026-10-01/02, still nonmatching)

Working notes: `build/wf/sub_0804FC4C/NOTES.md` (tools `cmp.py`, `casecmp.py` per-case region table, `try.sh`, `mk.py`, `rtlsum.py`). The wf score went from **1026 to 656** (698 at the end of round 1). The per-case region total (`casecmp`) went from 547 to 189, and the built size is 10 bytes short (0xE1A vs 0xE24). The parked draft in `src/code_0804EFF0.c` matches the prologue and cases 0-3, 5-11, 21, 100, 102 and 120-122. The remaining region diffs are case 4 (27), 20 (36), 101 (38), 110 (65) and 111 (19).

What worked (kept in the draft):
- **Shared tails:** every case ends with its own `return 0;` and no `break`, with `default: goto done;` and `return 0; done: return 1;` after the switch. Case 121's if-branch has its own `return 0`, because it is the survivor of the `step++` cross-jump tails. This reproduces all the ROM's cross-jumped tails.
- Prologue: `u8 *base = FC_E; u8 *b4 = base + 4; ps = (struct FcFlagsS *)(b4 + player * 0xD64);`, with `b4` computed before the multiply. A **signed 1-bit bitfield** tested `if (ps->bit1 < 0)` gives `lsl #30; bge` and the `and r0, r2` tie for the clear. `player` is made opaque (set twice) so that `player & 1` survives.
- Cases 6 and 8: the event-word constant must not be folded into `other << 31`. Case 6 declares `u32 kind = 0x6200000;` first, and case 8 assigns `(kind = 0x6400000)` inside the expression.
- Card-number compares go through `FcNum(u32 id)`, an inline `return table[id & 0x7FF];` returning `int`, with no `(u16)` cast. The cast creates an HImode AND with a long-lived HImode `0x7FF` constant, which adds copies and shifts the reload rotation. In case 4, `u32 c = ...; if (c == 0x402)` keeps 0x402 in the loop.
- Case 9: `u32 zone = FC_ZONE; u8 *zp = FC_E + 0x2C; z = (struct Zone *)(zone * 0x94 + player * 0xD64 + zp);` gives the ROM's preheader order (`&zone`, `E0 + 0x2C`, `0x94`, mult) and allocation (r5, r3, r4, r8). Through the reload rotation this also fixed cases 10 and 102.
- Case 20: `int count; if (count > 0) do { ... } while (--count);` (`bgt` at the top, `beq` at the bottom, early decrement). Case 111's second message uses its own `u16 msg2`. Case 122 uses a `struct FcCfb0` field view, which gives separate symbol + 0x824/0x82C loads instead of one `sym+0x824` pool word.

Remaining (hypotheses from the notes):
- Case 4: the ROM recomputes `player * 0xD64` in the loop preheader and copies the top-test base, but cse2 merges the hoisted multiply with the top-test one in the build. A pointer variable for the list count made it worse.
- Case 9: the ROM's preheader order suggests that pass 1 of loop.c hoisted only `&zone` and `E0 + 0x2C`, and pass 2 hoisted the rest. In the build, CSE1 shares `0x94` and the multiply between the loop top and the found-branch recomputation, so they are hoisted in pass 1.
- Cases 101/102/110/111/120: probably the same `FcNum` compare issue plus allocation. Case 102's 0x416 constant is hoisted in the build but not in the ROM.
- Case 10's final `FC_STEP++` uses `mov r6,#0xD9` and needs r1 to merge into case 121's copy, which is a reload-register choice.
- Gotcha: `wf.py park` merges three ways against `base.c`, and other agents edit this unit. After a park, run `syncbase.py` (or copy `src/` to `base.c` once the diff shows only this draft).

