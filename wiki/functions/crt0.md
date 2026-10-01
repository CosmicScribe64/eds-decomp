---
title: crt0 and IntrMain (start-up and IRQ dispatcher)
type: function
status: verified
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# crt0 / IntrMain

| Field | Value |
|---|---|
| Address | `0x080000C0`–`0x08000228` (0x168 bytes, including 2 stack words and a 3-word literal pool) |
| Mode | ARM |
| File | `asm/crt0.s` (future `src/crt0.s`) |
| Match | **matching** |

| Symbol | Address | Notes |
|---|---|---|
| `crt0` | `0x080000C0` | ROM entry target. Sets IRQ stack `0x03007FA0` and SYS stack `0x03007B00`, stores `IntrMain` at `INTR_VECTOR` (`0x03007FFC`), and calls `AgbMain` (`0x08075F64`, Thumb) with `bx`. Loops back if it ever returns. |
| `IntrMain` | `0x080000FC` | Nintendo SDK multiple-interrupt dispatcher. Jumps through `IntrTable` at `0x03000000`. |

## Purpose
This is the Nintendo SDK `crt0.s` template, the same one as fireemblem8u's `src/crt0.s`. EDS built it with its own configuration:
- **Priority / IntrTable slot order:** SERIAL, HBLANK, VBLANK, VCOUNT, TIMER0, TIMER1, TIMER2, TIMER3, DMA0, DMA1, DMA2, DMA3, KEYPAD (13 slots). After those, GAMEPAK goes to an infinite loop. The **link cable (SIO) has top priority**, and there's no leading GAMEPAK check as in FE8.
- **Nested interrupts:** while a handler runs, `IE = 0x2280` (SERIAL | DMA1 | GAMEPAK), and handlers run in SYS mode with IRQs enabled, so only SIO, sound FIFO DMA1 and cart removal can pre-empt a handler. The original IE and SPSR are restored afterwards. Unlike pokeemerald's variant, IME isn't saved.
- **IntrMain is moved to IWRAM at boot.** The boot init `sub_08075DF4`, which AgbMain calls, uses DMA3 to copy `0x400` bytes from `0x080000FC` to `0x0300004C` (DMA3CNT `0x80000200`). It then sets `INTR_VECTOR = 0x0300004C`. So the ROM copy installed by `crt0` runs only until the init code executes, and after that the dispatcher runs from IWRAM. It still works there, because its literal pool is copied along with it and all its accesses are PC-relative.
- `sub_08075DF4` also fills `IntrTable` (`0x03000000`, 16 words, `0x40` bytes). Every slot starts as 0 except these (verified from the stores at `0x08075E2C`–`0x08075E54`):
  - slot 2 (VBlank) = `0x0807569D`
  - slot 6 (Timer2) = `0x0807570D`
  - slot 9 (DMA1) = `0x0807E325` (inside the Konami sound driver)
  - slot 13 = `0x08075741`. `IntrMain` never dispatches slot 13, because GAMEPAK loops before dispatch. The table is probably declared with more slots than the dispatcher uses (hypothesis).

## Callers / Callees
- Entry: the ROM header branch at `0x08000000` (see [[rom-header]]).
- Calls: `AgbMain` (`0x08075F64`), and `IntrTable[n]`.

## Matching notes
- Hand-written ARM assembly. It was assembled with `arm-none-eabi-as -mcpu=arm7tdmi`, linked at `0x080000C0` with `AgbMain`/`IntrTable` defined, and matched byte-for-byte. The literal pool order is `INTR_VECTOR`, `AgbMain+1`, `IntrTable`.
- `lsl r1, r2, #16; lsr r1, r1, #16` isolates IE. Keep the exact `push {r0, r1, r3, lr}` register set.

See [[nintendo-sdk-libraries]], [[thumb-and-arm]].

The IntrTable handlers (VBlank, Timer2, DMA1 sound, serial/Timer3 link, per-scene HBlank) are on [[interrupt-handlers]]. `AgbMain` and the boot init are on [[agb-main]]. The final IE after sound init is `0x2229` (VBlank, Timer0, Timer2, DMA1, Gamepak); scenes add HBlank and the link code adds Serial and Timer3 (see [[program-flow]]).
