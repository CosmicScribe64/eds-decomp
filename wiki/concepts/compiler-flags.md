---
title: Compiler and Flags
type: concept
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Compiler and Flags

EDS was built with **agbcc** (GCC 2.95 for Thumb, see [[agbcc]]). It was **not** built with one compiler, and three builds are visible in the ROM. Each row below comes from compiling C and comparing it byte-for-byte against the baserom (53 functions, see [Evidence](#evidence)).

| Region | Range (Thumb `.text`) | Compiler | Flags | Status |
|---|---|---|---|---|
| **Game code** (Konami) | `0x08000228`–`0x0807D3CF` | **`old_agbcc`** | `-mthumb-interwork -O2` | verified (35 functions, 9 of them *only* match `old_agbcc`) |
| **Konami sound driver** ([[sound-engine]]) | `0x0807D3D0`–`0x0807EACF` | **`agbcc`** | `-mthumb-interwork -O2 -fprologue-bugfix` | verified (12 functions: 6 match *only* `agbcc`, and 1 of those, `SoundRequestBGMIfNotPlaying`, only with `-fprologue-bugfix`). The start address is a hypothesis, see [Open points](#open-points). |
| **AgbSram** SDK library ([[agb-sram]]) | `0x0807ED04`–`0x0807EE97` | `agbcc` or `old_agbcc` (identical here) | `-mthumb-interwork -O1` | verified (6/6 functions) |
| libgcc / libc | `0x0807EE98`–`0x08080A18` | prebuilt `libgcc.a` / `libc.a` from agbcc | none | byte-identical ([[nintendo-sdk-libraries]]) |

`-fhex-asm` only changes how numbers are printed in the `.s` file, so it never affects the bytes. Keep it for readable output.

> [!warning] Contradiction
> [[decomp-workflow]] and the `Makefile` (`DEFAULT_CFLAGS`, 2026-09-29) say game code is `agbcc -O2 -fprologue-bugfix`, based on leaf functions such as `Bustup_ShowPage`. That function matches both `agbcc -fprologue-bugfix` **and** `old_agbcc`, so it can't tell them apart. The discriminating functions below (`Bustup_ClearHiddenBox`, `GetDayOfWeek`, `EffectJigenBakudanChainA`, `EffectDiscardMagicDamageResolve`, `TextBoxClearTiles`, `IsSeEnabled`, `SetSeEnabled`, `Ease_Tick`, `CardTrading_SelectCard`) match **only** `old_agbcc`. None of the 35 matched game-region functions needs `agbcc`. A re-check of the work-in-progress `src/` units (2026-09-29) agrees. Six functions there (`Record_HasPrevPage`, `Title_HBlank`, `ClearCardStatusFlags`, `DuelCmd_ZeroAttackerAtk`, `DuelCmd_AddProhibition`, `DuelCmd_RemoveProhibition`) match with `old_agbcc` but not with the Makefile default, and no function matches only with the default. **Resolution (verified):** game units should use `old_agbcc -mthumb-interwork -O2`. Only the sound-driver units need `agbcc … -fprologue-bugfix`. Unit `password_trade` crosses the boundary and must be split at `0x0807D3D0`.

## Why `old_agbcc` and not `agbcc -fprologue-bugfix`?

pret's agbcc repo builds two cc1 binaries from the same GCC 2.95 tree. Both print `gcc 2.9-arm-000512`. They represent two different SDK compiler releases. The differences that matter for EDS, all observed while matching:

| Behaviour | `old_agbcc` | `agbcc` | `agbcc -fprologue-bugfix` |
|---|---|---|---|
| Read-modify-write / bit test of **non-volatile** memory with a constant (`x \|= 4`, `if (p->f & 0x80)`) | constant first: `mov r1,#4; ldrh r2,[r0]; orr r1,r2` | load first: `ldrh r2,[r0]; mov r1,#4; orr r1,r2` | load first |
| Leaf function that has a branch but saves no registers | `bx lr` | **spurious** `push {lr}` … `pop {r0}; bx r0` | `bx lr` |
| Register allocation | "old" choices (e.g. `GetDayOfWeek`, `Ease_Tick`) | different | same as `agbcc` |
| `-fprologue-bugfix` option | rejected (`Invalid option`) | accepted | n/a |

- **Volatile I/O accesses** (`REG_SIOCNT & 0x40` and similar) come out load-first with **both** compilers, because volatile loads aren't reordered. Don't use them to tell the compilers apart.
- **ROM-wide scan** (scratch classifier over 1987 functions found by a linear-sweep splitter): the game region has 1298 constant-first RMW sites against 132 load-first ones. The load-first sites are mostly from constant reuse, where both compilers agree. The sound region has 0 constant-first and 15 load-first. Across the whole ROM, **0** leaf functions with branches use a bare `push {lr}`, and 111 return with `bx lr` (2 of them in the sound driver). So *plain* `agbcc` without `-fprologue-bugfix` was never used.
- This fits the sound driver being a separately maintained Konami module that was built with a newer SDK compiler and linked between the game objects. It's placed right after the Card Trading code, see [[sound-engine]]. The game code used the older compiler. (The interpretation is a hypothesis. The compiler assignment itself is verified.)

## Other flags (verified)

- **`-O2` is required** for game and sound code. `-O1` breaks `GetDuelistName`, `CopyBitmapRows`, `IsLeapYear`, `GetDayOfWeek`, `DestinyBoardScene_HBlank`, `TextBoxClearTiles`, `IsSeEnabled/74`, `SoundLoadWaveRam` and others. The changes are strength reduction, loop reversal and register choice. `-O3` looks the same as `-O2` in this sample. It only adds automatic inlining, and nothing here depends on that.
- **`-O1` is required** for AgbSram. At `-O2`, `while (size--)` and the ReadSram copy loop compile differently. This matches how Nintendo's backup libraries are built elsewhere. pret's pokeemerald compiles its `agb_flash*.c` with `-O -mthumb-interwork` (general knowledge, not re-checked here).
- **`-mthumb-interwork` is required.** Without it, non-leaf and register-saving functions return with `pop {pc}` / `pop {r4,pc}` instead of `pop {r0}; bx r0` (void) or `pop {r1}; bx r1` (value in `r0`). A linear sweep of all Thumb code finds 14 `pop {…, pc}` decodes. All are jump-table words that were misread as code (`0x0800AFA0`…, `0x0805BC68`…, `0x0807BD38`…), so no real function uses a non-interworking return.
- **No extra optimization flags.** Calls are plain `bl`, so there's no `-mlong-calls`. The default GCC 2.95 `-O2` pass set reproduces all 53 functions. Toggling `-fcaller-saves`, `-fno-gcse`, `-fno-strength-reduce`, `-fno-regmove` and similar on `Ease_Tick` never produced a closer match.
- **Assembly:** `arm-none-eabi-as -mcpu=arm7tdmi`. Append `.text; .align 2, 0` to every compiler `.s` (the pret convention), so section padding is `0x0000` and not `mov r8, r8` (`0x46C0`). The ROM pads with `0x0000` everywhere.

## Codegen quirks worth knowing when matching

These are all observed in matched functions. Also see the tips in [[decomp-workflow]].

- **Function alignment** is 4 bytes. The padding before every function and every literal pool is `0x0000`.
- **Literal pools** go after the function, or in the middle after an unconditional `b` (`SoundSeekBGM`, `GetDialogueEventId`).
- **Epilogues:** `pop {r0}; bx r0` for void functions and `pop {r1}; bx r1` when `r0` holds a return value. High registers are saved with `mov r7,r8; push {r7}` and restored with `pop {r3}; mov r8,r3`.
- **Moves** are `add rD, rS, #0`. Returning a local is `add r0, rX, #0` before the epilogue.
- **Large struct offsets** load base and offset as two literals and add them: `ldr r1,=gSaveData; ldr r0,=0x2152; add r1,r0` (`IsSeEnabled`, `DestinyBoardScene_HBlank`, `CardTrading_SelectCard`). Small offsets such as `0x188` are built with `mov r1,#0xC4; lsl r1,#1; add`. Write plain `gStruct.field` C for both.
- **Branch layout follows the source.** `if (id <= 490) return tbl[id].x; else return 0;` puts the `return 0` block first (`bls`). `if (id > 490) return 0; return tbl[id].x;` gives the opposite layout (`GetDialogueEventId`). The leap-year test (`IsLeapYear`) only matches as `if (y % 4 != 0 || (y % 100 == 0 && y % 400 != 0)) return 0; return 1;`.
- **Ternary versus if-assign:** `f(p->unkC, p->unk20 == 1 ? A : B, n)` matches `Bustup_ClearHiddenBox`. The equivalent `dest = B; if (...) dest = A;` does not.
- **Cross-jumping leaves a saved but unused register:** `Ease_Tick` saves `r4` without using it. The source repeats the same tail (`r->state = 2; r->cur = r->target;`) in both arms of an `if`. The optimizer merged the copies after register allocation had already reserved `r4` (seen with `old_agbcc`).
- **Loop reversal:** a `for (i = 0; i < N; i++)` whose body doesn't use `i` becomes a count-down `sub rX,#1; cmp rX,#0; bge` (`TextBoxClearTiles`). `while (--n >= 0)` gives the same shape (`SoundSeekBGM`).
- **Post-increment in an argument** (`f(..., p++)` with `sizeof(*p)==0x14`) puts the `add` *before* the `bl` (`ExodiaScene_StartPieceFlight`). A separate `p += 0x14;` puts it after.
- **Bitfields:** ARM GCC rounds every struct up to a multiple of 4 (STRUCTURE_SIZE_BOUNDARY 32). If the struct is exactly 4 bytes, bitfields are read as a whole word (`ldr` + shifts). If it's larger, they're read with `ldrb`/`ldrh` + `lsl/lsr` (`EffectJigenBakudanChainA`, `EffectDiscardMagicDamageResolve`). Mixed `u8`/`u16` bitfield declarations pick the byte or halfword container.
- **Signed 16-bit compares** of two values held in registers are done as `lsl #16` on both sides, then a signed branch, with no `asr` (`Ease_Tick`).
- **`% 64` of a provably non-negative int** comes out as `asr #6; lsl #6; sub` with no sign fix-up (`DestinyBoardScene_HBlank`). `(i * stride) >> 2` used as a `u32 *` index gives `asr #2; lsl #2`.
- **64-bit:** `s64 r = (s64)a * (s64)b; return r >> 8;` matches `MulFix8Wide`. The same expression without the named `s64` temporary allocates different registers.
- **`--x == 0` on a `u16`** is `sub; strh; lsl #16; cmp #0` (`Timer_Tick`).
- **AgbSram idioms** at `-O1`: function addresses in the size computation (`(u32)ReadSram - (u32)ReadSram_Core`) emit dead `.rodata` constant-pool entries. See [[agb-sram]] and [[save-type]].

## Evidence

The ✓ marks mean a byte-identical `.text`, with relocations resolved from `_XXXXXXXX` name suffixes and the libgcc/libc addresses. All configurations use `-mthumb-interwork` except the last column. Columns: **A** `old_agbcc -O2` · **B** `agbcc -O2 -fprologue-bugfix` · **C** `agbcc -O2` · **D** `old_agbcc -O1` · **E** `agbcc -O1 -fprologue-bugfix` · **F** `agbcc -O1` · **G** `old_agbcc -O2` without `-mthumb-interwork`.

| Function | What | A | B | C | D | E | F | G |
|---|---|---|---|---|---|---|---|---|
| `GetDuelistName` | struct-array search loop (leaf) | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `CopyBitmapRows` | CpuSet row loop; 5th arg on stack; r8/sb | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `Bustup_ShowPage` | DISPCNT bit 4 set/clear (leaf, branch) | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ | ✓ |
| `Bustup_ClearHiddenBox` | CpuSet with ternary dest | ✓ | ✗ | ✗ | ✓ | ✗ | ✗ | ✗ |
| `GetDialogueEventId` | bounds-checked table getter | ✓ | ✓ | ✗ | ✓ | ✓ | ✗ | ✓ |
| `GetDialogueSpeaker` | bounds-checked table getter | ✓ | ✓ | ✗ | ✓ | ✓ | ✗ | ✓ |
| `IsLeapYear` | leap-year test (__umodsi3) | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `GetDaysInMonth` | days in month | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `GetDayOfWeek` | day of week (r8, loop, __modsi3) | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `GetHolidayFlags` | 12-way jump table + nested switches | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `GetZoneCardAtk` | struct out-param on stack (sub sp) | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `ExodiaScene_StartPieceFlight` | loop, 5th arg on stack, ldrsh | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `DestinyBoardScene_HBlank` | s8 wave table, % 64, volatile I/O | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `EffectJigenBakudanChainA` | bitfield extract (ldrb/ldrh) | ✓ | ✗ | ✗ | ✓ | ✗ | ✗ | ✗ |
| `EffectDiscardMagicDamageResolve` | bitfield test + call | ✓ | ✗ | ✗ | ✓ | ✗ | ✗ | ✗ |
| `TextBoxClearTiles` | count-down loop + |= 2 | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `IsSeEnabled` | flag getter, big struct offset | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✓ |
| `IsBgmEnabled` | flag getter | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `SetSeEnabled` | flag set/clear (leaf, branch) | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✓ |
| `Timer_Reset` | timer reset | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `Timer_Start` | timer start | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `Timer_Tick` | timer tick (leaf, branch) | ✓ | ✓ | ✗ | ✓ | ✓ | ✗ | ✓ |
| `Ease_Init` | ramp init | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `Ease_Tick` | ramp tick (cross-jumped tail) | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `SetBldAlpha` | BLDALPHA | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `SetBldY` | BLDY | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `MulFix8` | 8.8 fixed mul | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `MulFix8Wide` | s64 mul >> 8 (__muldi3) | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `DivFix8` | fixed div (SWI Div) | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `ReciprocalFix8` | fixed reciprocal | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `SioGetMultiPlayerId` | SIOCNT field getter | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✓ |
| `SioSetMultiSend` | SIOMLT_SEND setter | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `SioGetMultiRecv` | SIOMULTI[n] getter | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `SioGetError` | SIOCNT bit getter | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✓ |
| `CardTrading_SelectCard` | card-trade state (last pre-driver fn) | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `SoundLoadWaveRam` | SoundLoadWaveRam | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `SoundVBlank` | SoundVBlank | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `SoundRequestBGM` | BGM request | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `SoundRequestBGMFadeIn` | BGM request + arg | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `SoundRequestBGMIfNotPlaying` | BGM request if changed (leaf, branch) | ✗ | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `SoundFadeOutBGM` | volume set | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `SoundPauseBGM` | volume set + flag | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `SoundResumeBGM` | volume set, clear flag (uses ip) | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `SoundGetBGMTick` | driver getter | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `SoundSeekBGM` | driver pause/skip loop (r8) | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `SoundSeekBGMFadeIn` | fade start | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `SoundStopAllSE` | flags |= 4 | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_0807ED04` | ReadSram_Core | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807ED28` | ReadSram | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807ED8C` | WriteSram | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807EDCC` | VerifySram_Core | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807EDFC` | VerifySram | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807EE60` | WriteSramEx | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |

Reading the table:
- **Game code, `old_agbcc` only (A):** `Bustup_ClearHiddenBox`, `GetDayOfWeek`, `EffectJigenBakudanChainA`, `EffectDiscardMagicDamageResolve`, `TextBoxClearTiles`, `IsSeEnabled`, `SetSeEnabled`, `Ease_Tick`, `CardTrading_SelectCard`. These span `0x08000xxx`–`0x0807Dxxx`.
- **Sound driver, `agbcc` only (B and C):** `SoundVBlank` (SoundVBlank), `SoundPauseBGM`, `SoundResumeBGM`, `SoundSeekBGM`, `SoundStopAllSE`. **`SoundRequestBGMIfNotPlaying` matches only B**, so the driver needs `-fprologue-bugfix`.
- **AgbSram, `-O1` only (D, E and F):** `sub_0807ED04`–`sub_0807EDFC`. `sub_0807EE60` (WriteSramEx) is insensitive to the flags.
- Column G fails for every function that saves registers, so `-mthumb-interwork` is required.

**Method.** A scratch compare script (now `tools/compiler_tests/cmp.py`) runs `cpp -nostdinc -undef -I/opt/agbcc/include | <cc1> <flags>`. It appends `.text; .align 2, 0`, assembles, resolves `R_ARM_ABS32` / `R_ARM_THM_CALL` relocations from symbol names, and compares against `baserom.gba`. Every function is stored as a standalone file in `tools/compiler_tests/sub_08XXXXXX.c`, with the flags in its header. Re-verify with `tools/dr sh tools/compiler_tests/check.sh`, or add `MATRIX=1` to re-run the flag matrix.

## Open points

- **The exact start of the `agbcc` region.** The last function verified as `old_agbcc` is `CardTrading_SelectCard` (Card Trading). The first discriminating `agbcc` function is `SoundVBlank`. `SoundLoadWaveRam` matches with both compilers. `0x0807D3D0` (`SoundDmaInit`, the first function that touches sound hardware, and where [[sound-engine]] starts the driver) is assumed to be the boundary. The next check is to match something in `0x0807D3D0`–`0x0807E3AF` that tells the compilers apart.
- **Other middleware islands** built with a different compiler may exist and weren't sampled one by one. Candidates are the Konami link code "BASICSIO" (`0x08072054`–`0x08074300`) and the long `0x0807A6AC`–`0x0807C7C8` utility stretch. The utility functions sampled there (`Timer_Reset`–`SioGetError`) are `old_agbcc`, or can't tell the compilers apart. A per-unit re-check with both compilers catches any outliers.
