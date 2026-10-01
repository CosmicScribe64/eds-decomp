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
| **Konami sound driver** ([[sound-engine]]) | `0x0807D3D0`–`0x0807EACF` | **`agbcc`** | `-mthumb-interwork -O2 -fprologue-bugfix` | verified (12 functions: 6 match *only* `agbcc`, and 1 of those, `sub_0807E6E8`, only with `-fprologue-bugfix`). The start address is a hypothesis, see [Open points](#open-points). |
| **AgbSram** SDK library ([[agb-sram]]) | `0x0807ED04`–`0x0807EE97` | `agbcc` or `old_agbcc` (identical here) | `-mthumb-interwork -O1` | verified (6/6 functions) |
| libgcc / libc | `0x0807EE98`–`0x08080A18` | prebuilt `libgcc.a` / `libc.a` from agbcc | none | byte-identical ([[nintendo-sdk-libraries]]) |

`-fhex-asm` only changes how numbers are printed in the `.s` file, so it never affects the bytes. Keep it for readable output.

> [!warning] Contradiction
> [[decomp-workflow]] and the `Makefile` (`DEFAULT_CFLAGS`, 2026-09-29) say game code is `agbcc -O2 -fprologue-bugfix`, based on leaf functions such as `sub_08000838`. That function matches both `agbcc -fprologue-bugfix` **and** `old_agbcc`, so it can't tell them apart. The discriminating functions below (`sub_08000854`, `sub_080042D8`, `sub_0802C890`, `sub_0803A7F4`, `sub_0805FCF4`, `sub_08077A44`, `sub_08077A74`, `sub_0807B114`, `sub_0807D0A0`) match **only** `old_agbcc`. None of the 35 matched game-region functions needs `agbcc`. A re-check of the work-in-progress `src/` units (2026-09-29) agrees. Six functions there (`sub_08003ED4`, `sub_08004ABC`, `sub_0800743C`, `sub_0800D6D4`, `sub_0800EDCC`, `sub_0800EE50`) match with `old_agbcc` but not with the Makefile default, and no function matches only with the default. **Resolution (verified):** game units should use `old_agbcc -mthumb-interwork -O2`. Only the sound-driver units need `agbcc … -fprologue-bugfix`. Unit `code_0807C7C8` crosses the boundary and must be split at `0x0807D3D0`.

## Why `old_agbcc` and not `agbcc -fprologue-bugfix`?

pret's agbcc repo builds two cc1 binaries from the same GCC 2.95 tree. Both print `gcc 2.9-arm-000512`. They represent two different SDK compiler releases. The differences that matter for EDS, all observed while matching:

| Behaviour | `old_agbcc` | `agbcc` | `agbcc -fprologue-bugfix` |
|---|---|---|---|
| Read-modify-write / bit test of **non-volatile** memory with a constant (`x \|= 4`, `if (p->f & 0x80)`) | constant first: `mov r1,#4; ldrh r2,[r0]; orr r1,r2` | load first: `ldrh r2,[r0]; mov r1,#4; orr r1,r2` | load first |
| Leaf function that has a branch but saves no registers | `bx lr` | **spurious** `push {lr}` … `pop {r0}; bx r0` | `bx lr` |
| Register allocation | "old" choices (e.g. `sub_080042D8`, `sub_0807B114`) | different | same as `agbcc` |
| `-fprologue-bugfix` option | rejected (`Invalid option`) | accepted | n/a |

- **Volatile I/O accesses** (`REG_SIOCNT & 0x40` and similar) come out load-first with **both** compilers, because volatile loads aren't reordered. Don't use them to tell the compilers apart.
- **ROM-wide scan** (scratch classifier over 1987 functions found by a linear-sweep splitter): the game region has 1298 constant-first RMW sites against 132 load-first ones. The load-first sites are mostly from constant reuse, where both compilers agree. The sound region has 0 constant-first and 15 load-first. Across the whole ROM, **0** leaf functions with branches use a bare `push {lr}`, and 111 return with `bx lr` (2 of them in the sound driver). So *plain* `agbcc` without `-fprologue-bugfix` was never used.
- This fits the sound driver being a separately maintained Konami module that was built with a newer SDK compiler and linked between the game objects. It's placed right after the Card Trading code, see [[sound-engine]]. The game code used the older compiler. (The interpretation is a hypothesis. The compiler assignment itself is verified.)

## Other flags (verified)

- **`-O2` is required** for game and sound code. `-O1` breaks `sub_08000228`, `sub_08000270`, `sub_08004280`, `sub_080042D8`, `sub_08026CC8`, `sub_0805FCF4`, `sub_08077A44/74`, `sub_0807D518` and others. The changes are strength reduction, loop reversal and register choice. `-O3` looks the same as `-O2` in this sample. It only adds automatic inlining, and nothing here depends on that.
- **`-O1` is required** for AgbSram. At `-O2`, `while (size--)` and the ReadSram copy loop compile differently. This matches how Nintendo's backup libraries are built elsewhere. pret's pokeemerald compiles its `agb_flash*.c` with `-O -mthumb-interwork` (general knowledge, not re-checked here).
- **`-mthumb-interwork` is required.** Without it, non-leaf and register-saving functions return with `pop {pc}` / `pop {r4,pc}` instead of `pop {r0}; bx r0` (void) or `pop {r1}; bx r1` (value in `r0`). A linear sweep of all Thumb code finds 14 `pop {…, pc}` decodes. All are jump-table words that were misread as code (`0x0800AFA0`…, `0x0805BC68`…, `0x0807BD38`…), so no real function uses a non-interworking return.
- **No extra optimization flags.** Calls are plain `bl`, so there's no `-mlong-calls`. The default GCC 2.95 `-O2` pass set reproduces all 53 functions. Toggling `-fcaller-saves`, `-fno-gcse`, `-fno-strength-reduce`, `-fno-regmove` and similar on `sub_0807B114` never produced a closer match.
- **Assembly:** `arm-none-eabi-as -mcpu=arm7tdmi`. Append `.text; .align 2, 0` to every compiler `.s` (the pret convention), so section padding is `0x0000` and not `mov r8, r8` (`0x46C0`). The ROM pads with `0x0000` everywhere.

## Codegen quirks worth knowing when matching

These are all observed in matched functions. Also see the tips in [[decomp-workflow]].

- **Function alignment** is 4 bytes. The padding before every function and every literal pool is `0x0000`.
- **Literal pools** go after the function, or in the middle after an unconditional `b` (`sub_0807E9C8`, `sub_08001BC8`).
- **Epilogues:** `pop {r0}; bx r0` for void functions and `pop {r1}; bx r1` when `r0` holds a return value. High registers are saved with `mov r7,r8; push {r7}` and restored with `pop {r3}; mov r8,r3`.
- **Moves** are `add rD, rS, #0`. Returning a local is `add r0, rX, #0` before the epilogue.
- **Large struct offsets** load base and offset as two literals and add them: `ldr r1,=gUnk_02011C20; ldr r0,=0x2152; add r1,r0` (`sub_08077A44`, `sub_08026CC8`, `sub_0807D0A0`). Small offsets such as `0x188` are built with `mov r1,#0xC4; lsl r1,#1; add`. Write plain `gStruct.field` C for both.
- **Branch layout follows the source.** `if (id <= 490) return tbl[id].x; else return 0;` puts the `return 0` block first (`bls`). `if (id > 490) return 0; return tbl[id].x;` gives the opposite layout (`sub_08001BC8`). The leap-year test (`sub_08004280`) only matches as `if (y % 4 != 0 || (y % 100 == 0 && y % 400 != 0)) return 0; return 1;`.
- **Ternary versus if-assign:** `f(p->unkC, p->unk20 == 1 ? A : B, n)` matches `sub_08000854`. The equivalent `dest = B; if (...) dest = A;` does not.
- **Cross-jumping leaves a saved but unused register:** `sub_0807B114` saves `r4` without using it. The source repeats the same tail (`r->state = 2; r->cur = r->target;`) in both arms of an `if`. The optimizer merged the copies after register allocation had already reserved `r4` (seen with `old_agbcc`).
- **Loop reversal:** a `for (i = 0; i < N; i++)` whose body doesn't use `i` becomes a count-down `sub rX,#1; cmp rX,#0; bge` (`sub_0805FCF4`). `while (--n >= 0)` gives the same shape (`sub_0807E9C8`).
- **Post-increment in an argument** (`f(..., p++)` with `sizeof(*p)==0x14`) puts the `add` *before* the `bl` (`sub_080261A8`). A separate `p += 0x14;` puts it after.
- **Bitfields:** ARM GCC rounds every struct up to a multiple of 4 (STRUCTURE_SIZE_BOUNDARY 32). If the struct is exactly 4 bytes, bitfields are read as a whole word (`ldr` + shifts). If it's larger, they're read with `ldrb`/`ldrh` + `lsl/lsr` (`sub_0802C890`, `sub_0803A7F4`). Mixed `u8`/`u16` bitfield declarations pick the byte or halfword container.
- **Signed 16-bit compares** of two values held in registers are done as `lsl #16` on both sides, then a signed branch, with no `asr` (`sub_0807B114`).
- **`% 64` of a provably non-negative int** comes out as `asr #6; lsl #6; sub` with no sign fix-up (`sub_08026CC8`). `(i * stride) >> 2` used as a `u32 *` index gives `asr #2; lsl #2`.
- **64-bit:** `s64 r = (s64)a * (s64)b; return r >> 8;` matches `sub_0807B4E0`. The same expression without the named `s64` temporary allocates different registers.
- **`--x == 0` on a `u16`** is `sub; strh; lsl #16; cmp #0` (`sub_0807B0D0`).
- **AgbSram idioms** at `-O1`: function addresses in the size computation (`(u32)ReadSram - (u32)ReadSram_Core`) emit dead `.rodata` constant-pool entries. See [[agb-sram]] and [[save-type]].

## Evidence

The ✓ marks mean a byte-identical `.text`, with relocations resolved from `_XXXXXXXX` name suffixes and the libgcc/libc addresses. All configurations use `-mthumb-interwork` except the last column. Columns: **A** `old_agbcc -O2` · **B** `agbcc -O2 -fprologue-bugfix` · **C** `agbcc -O2` · **D** `old_agbcc -O1` · **E** `agbcc -O1 -fprologue-bugfix` · **F** `agbcc -O1` · **G** `old_agbcc -O2` without `-mthumb-interwork`.

| Function | What | A | B | C | D | E | F | G |
|---|---|---|---|---|---|---|---|---|
| `sub_08000228` | struct-array search loop (leaf) | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_08000270` | CpuSet row loop; 5th arg on stack; r8/sb | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_08000838` | DISPCNT bit 4 set/clear (leaf, branch) | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ | ✓ |
| `sub_08000854` | CpuSet with ternary dest | ✓ | ✗ | ✗ | ✓ | ✗ | ✗ | ✗ |
| `sub_08001BC8` | bounds-checked table getter | ✓ | ✓ | ✗ | ✓ | ✓ | ✗ | ✓ |
| `sub_08001BEC` | bounds-checked table getter | ✓ | ✓ | ✗ | ✓ | ✓ | ✗ | ✓ |
| `sub_08004280` | leap-year test (__umodsi3) | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_080042B4` | days in month | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `sub_080042D8` | day of week (r8, loop, __modsi3) | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `sub_08004358` | 12-way jump table + nested switches | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `sub_0800C894` | struct out-param on stack (sub sp) | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `sub_080261A8` | loop, 5th arg on stack, ldrsh | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `sub_08026CC8` | s8 wave table, % 64, volatile I/O | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_0802C890` | bitfield extract (ldrb/ldrh) | ✓ | ✗ | ✗ | ✓ | ✗ | ✗ | ✗ |
| `sub_0803A7F4` | bitfield test + call | ✓ | ✗ | ✗ | ✓ | ✗ | ✗ | ✗ |
| `sub_0805FCF4` | count-down loop + |= 2 | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `sub_08077A44` | flag getter, big struct offset | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✓ |
| `sub_08077A5C` | flag getter | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_08077A74` | flag set/clear (leaf, branch) | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✓ |
| `sub_0807B0C0` | timer reset | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_0807B0C8` | timer start | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_0807B0D0` | timer tick (leaf, branch) | ✓ | ✓ | ✗ | ✓ | ✓ | ✗ | ✓ |
| `sub_0807B0EC` | ramp init | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807B114` | ramp tick (cross-jumped tail) | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `sub_0807B4A8` | BLDALPHA | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_0807B4C0` | BLDY | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_0807B4D0` | 8.8 fixed mul | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_0807B4E0` | s64 mul >> 8 (__muldi3) | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807B504` | fixed div (SWI Div) | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807B51C` | fixed reciprocal | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807BED8` | SIOCNT field getter | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✓ |
| `sub_0807BEE8` | SIOMLT_SEND setter | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_0807BEF8` | SIOMULTI[n] getter | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_0807BF08` | SIOCNT bit getter | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✓ |
| `sub_0807D0A0` | card-trade state (last pre-driver fn) | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `sub_0807D518` | SoundLoadWaveRam | ✓ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_0807E3B0` | SoundVBlank | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_0807E674` | BGM request | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_0807E690` | BGM request + arg | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807E6E8` | BGM request if changed (leaf, branch) | ✗ | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ |
| `sub_0807E764` | volume set | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_0807E780` | volume set + flag | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_0807E7B8` | volume set, clear flag (uses ip) | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_0807E9BC` | driver getter | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `sub_0807E9C8` | driver pause/skip loop (r8) | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_0807EA4C` | fade start | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807EA88` | flags |= 4 | ✗ | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ |
| `sub_0807ED04` | ReadSram_Core | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807ED28` | ReadSram | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807ED8C` | WriteSram | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807EDCC` | VerifySram_Core | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807EDFC` | VerifySram | ✗ | ✗ | ✗ | ✓ | ✓ | ✓ | ✗ |
| `sub_0807EE60` | WriteSramEx | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✗ |

Reading the table:
- **Game code, `old_agbcc` only (A):** `sub_08000854`, `sub_080042D8`, `sub_0802C890`, `sub_0803A7F4`, `sub_0805FCF4`, `sub_08077A44`, `sub_08077A74`, `sub_0807B114`, `sub_0807D0A0`. These span `0x08000xxx`–`0x0807Dxxx`.
- **Sound driver, `agbcc` only (B and C):** `sub_0807E3B0` (SoundVBlank), `sub_0807E780`, `sub_0807E7B8`, `sub_0807E9C8`, `sub_0807EA88`. **`sub_0807E6E8` matches only B**, so the driver needs `-fprologue-bugfix`.
- **AgbSram, `-O1` only (D, E and F):** `sub_0807ED04`–`sub_0807EDFC`. `sub_0807EE60` (WriteSramEx) is insensitive to the flags.
- Column G fails for every function that saves registers, so `-mthumb-interwork` is required.

**Method.** A scratch compare script (now `tools/compiler_tests/cmp.py`) runs `cpp -nostdinc -undef -I/opt/agbcc/include | <cc1> <flags>`. It appends `.text; .align 2, 0`, assembles, resolves `R_ARM_ABS32` / `R_ARM_THM_CALL` relocations from symbol names, and compares against `baserom.gba`. Every function is stored as a standalone file in `tools/compiler_tests/sub_08XXXXXX.c`, with the flags in its header. Re-verify with `tools/dr sh tools/compiler_tests/check.sh`, or add `MATRIX=1` to re-run the flag matrix.

## Open points

- **The exact start of the `agbcc` region.** The last function verified as `old_agbcc` is `sub_0807D0A0` (Card Trading). The first discriminating `agbcc` function is `sub_0807E3B0`. `sub_0807D518` matches with both compilers. `0x0807D3D0` (`SoundDmaInit`, the first function that touches sound hardware, and where [[sound-engine]] starts the driver) is assumed to be the boundary. The next check is to match something in `0x0807D3D0`–`0x0807E3AF` that tells the compilers apart.
- **Other middleware islands** built with a different compiler may exist and weren't sampled one by one. Candidates are the Konami link code "BASICSIO" (`0x08072054`–`0x08074300`) and the long `0x0807A6AC`–`0x0807C7C8` utility stretch. The utility functions sampled there (`sub_0807B0C0`–`sub_0807BF08`) are `old_agbcc`, or can't tell the compilers apart. A per-unit re-check with both compilers catches any outliers.
