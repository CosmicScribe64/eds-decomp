---
title: AgbMain, GameInit (proposed) and MainLoop (proposed)
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# AgbMain / GameInit / MainLoop

| Function | Address | Size | Mode | Unit (current asm) | Match |
|---|---|---|---|---|---|
| `AgbMain` | `0x08075F64` | 0x0E | Thumb | `asm/code_080750E0.s` | nonmatching (not attempted) |
| `GameInit` (proposed; `sub_08075DF4`) | `0x08075DF4` | 0x114 (code 0x08075DF4–0x08075F08, then the pool to `0x08075F64`) | Thumb | same | nonmatching |
| `MainLoop` (proposed; `sub_08075D70`) | `0x08075D70` | 0x68 (code up to `0x08075DD8`, then the pool) | Thumb | same | nonmatching |

## Purpose
- **`AgbMain`** is `push {lr}; bl GameInit; bl MainLoop; pop {r0}; bx r0`. `MainLoop` never returns.
- **`GameInit`** does the whole one-time boot init. It clears EWRAM and IWRAM with DMA3 (crt0 doesn't do this), loads the save, fills `IntrTable`, copies `IntrMain` to IWRAM, sets up IE/DISPSTAT/IME/WAITCNT, installs the first scene (`CB_License` `0x08004EAC`), initialises sound, starts Timer2, and resets scroll and brightness. The step-by-step list is in [[program-flow]] §1.
- **`MainLoop`** runs once per frame: busy-waits for the VBlank flag, calls `FrameSyncUpdate`, calls the scene callback (and returns to the main menu if it reports done), then bumps the frame counters. The pseudo-C is in [[program-flow]] §2.

## Verified details
- RAM clear: `DMA3SAD = &zero (stack)`, `DMA3CNT = 0x85010000` over `0x02000000` (0x10000 words = 256 KiB) and `0x85001E80` over `0x03000000` (0x1E80 words = 0x7A00 bytes). The fill stops below the SYS stack (`0x03007B00`).
- `ReadSram(0x0E000000, 0x02011C20, 0x2170)` at `0x08075E28` ([[agb-sram]]).
- IntrMain copy: `DMA3 0x080000FC → 0x0300004C`, `CNT 0x80000200` (0x200 halfwords). Then `*(u32*)0x03007FFC = 0x0300004C`.
- `IME=0; IE=1; IE|=0x200; IE|=0x400; IE|=0x2000; IE|=0x20; DISPSTAT=8; DISPSTAT|=0x10; IME=1; WAITCNT=0x4014`.
- `gMain.callback = 0x08004EAD; *(u32*)0x03000454 = 0; SoundInit(0); TM2CNT_L=0xF400; TM2CNT_H|=0xC3; *(u16*)0x0300044E = 3; ResetBgScroll(); SetBrightnessWhite(); SetSeEnabled(1); SetBgmEnabled(1); sub_080770DC();`
- The main loop's volatile pattern (`ldrh; and; ldrh(discarded); strh`) shows `gMain.intrCheck` is declared `volatile u16`.

## Callers and callees
- `AgbMain` is called from crt0 (`0x080000EC`, `bx r1`). See [[crt0]].
- `GameInit` calls `ReadSram` `0x0807ED28`, `SoundInit` `0x0807D578`, `ResetBgScroll` `0x080757AC`, `SetBrightnessWhite` `0x08075A30`, `SetSeEnabled` `0x08077A74`, `SetBgmEnabled` `0x08077AB0`, `sub_080770DC`.
- `MainLoop` calls `FrameSyncUpdate` `0x08075CB4`, `_call_via_r0` `0x0807EE98` (the callback), `SetMainCallback` `0x080754F8` (with `0x08003AA5` = main menu), `sub_08075D6C` (empty).

## Matching notes
- Not attempted yet. `GameInit` uses a local `u32 zero` on the stack as the DMA source (`str r5,[sp]; mov r0,sp`). That's typical of a `DmaFill32`-style macro that takes the address of a local (compare pret's `DmaFill32`).
- The IntrTable stores are 16 consecutive `str` instructions with the constant 0 in r5, so the source is probably `IntrTable[i] = …` written out slot by slot, not a loop.
- `MainLoop`'s callback call goes through `_call_via_r0`, the normal agbcc code for an indirect call through a function pointer.

Related: [[program-flow]], [[interrupt-handlers]], [[set-main-callback]], [[frame-sync-update]].
