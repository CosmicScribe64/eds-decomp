---
title: agbcc
type: tool
status: verified
confidence: high
sources: [rom-analysis, general-knowledge]
updated: 2026-09-29
---
# agbcc

agbcc is pret's reconstruction of the GCC 2.95-based C compiler from Nintendo's GBA SDK (banner: `gcc 2.9-arm-000512 for Thumb/elf`). pokeruby, pokeemerald, pokefirered and many other GBA decomps use it. The Docker image (`eds-decomp:latest`) builds it from `pret/agbcc` into `/opt/agbcc`:

| Binary | What it is | Used by EDS for |
|---|---|---|
| `agbcc` | cc1 (preprocessed C in, `.s` out) of the newer SDK compiler. Accepts `-fprologue-bugfix`. | the Konami **sound driver** (`0x0807D3D0`–`0x0807EACF`, start is a hypothesis) with `-O2 -fprologue-bugfix`, and optionally AgbSram |
| `old_agbcc` | cc1 of the older SDK compiler. Rejects `-fprologue-bugfix`, but never emits the spurious leaf `push {lr}` anyway. | **all game code** (`0x08000228`–`0x0807D3CF`) with `-O2` |
| `agbcc_arm` | ARM-mode cc1 | not needed. The only ARM code (crt0, sound mixer) is hand-written asm. |
| `libgcc.a`, `libc.a` | agbcc runtime | linked unchanged; byte-identical to the ROM ([[nintendo-sdk-libraries]]) |

## Verified for EDS (2026-09-29)

- **agbcc is confirmed.** 53 C functions from all parts of the ROM rebuild byte-for-byte (`tools/compiler_tests/`). libgcc/libc match too.
- **Flags:** game code is `old_agbcc -mthumb-interwork -O2`. The sound driver is `agbcc -mthumb-interwork -O2 -fprologue-bugfix`. AgbSram is `-mthumb-interwork -O1` (either binary). `-fhex-asm` is cosmetic. Full evidence and the flag matrix are in [[compiler-flags]].
- **Epilogues** (`-mthumb-interwork`): `pop {r0}; bx r0` for void functions and `pop {r1}; bx r1` when returning a value. Leaf functions that save no registers return with `bx lr`.
- **Telling the two binaries apart:** for non-volatile `x |= C` / `x & C`, `old_agbcc` materialises the constant before the load and `agbcc` loads first. Plain `agbcc` (without `-fprologue-bugfix`) also pushes `lr` in branchy leaf functions, which never happens anywhere in the EDS ROM.
- **Build detail:** append `.text` / `.align 2, 0` to each generated `.s` before `as`, as pret does, so that section padding is `0x0000` rather than `mov r8, r8`.

> [!warning] Contradiction
> The first build setup (`Makefile` `DEFAULT_CFLAGS`, [[decomp-workflow]]) compiled game code with `agbcc -fprologue-bugfix`. Matching shows the game code needs `old_agbcc`. See the callout in [[compiler-flags]].

Related: [[compiler-flags]], [[matching-decompilation]], [[toolchain]], [[decomp-workflow]].
