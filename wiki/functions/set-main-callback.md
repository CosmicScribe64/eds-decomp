---
title: SetMainCallback (proposed) and the scene step-runner pattern
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# SetMainCallback `SetMainCallback` and step runners

| Field | Value |
|---|---|
| Address | `0x080754F8` |
| Size | 0x90 (code to `0x08075588`, then the pool) |
| Mode | Thumb |
| Unit | `asm/main.s` |
| Match | nonmatching (not attempted) |

## Purpose (verified)
```c
void SetMainCallback(u16 (*cb)(void)) {
    SaveGame();                               // 0x080754BC: checksum + WriteSram/VerifySram ×≤32
    gMain.vblankCallbackEarly = NULL;         // 0x03000458
    gMain.vblankCallback = NULL;              // 0x03000454
    REG_IME = 0; REG_IE &= ~INTR_FLAG_HBLANK; REG_IME = 1;
    REG_IME = 0; REG_IE &= ~INTR_FLAG_HBLANK; IntrTable[1] = NULL; REG_IME = 1;
    gMain.seq[0x4878..0x487A] = 0;            // top-level runner index + 2 bytes
    gMain.seq[0x4857..0x485B] = 0;            // runner indices / step sub-states
    gMain.callback = cb;                      // 0x03000450
}
```
**Every scene change writes the save to SRAM.** `SaveGame` is also called directly from `0x0801C02E` (×3), `0x08021628`, `0x08062AF4`, `0x08064604` and `0x0807D1F4` (Card Trading), and from the uncalled `SaveAndResetSceneState`. The same inline sequence (without the save) appears in `License_ShowKcejLogo` (the last License step installs `CB_Title` directly), and `SaveAndResetSceneState` is a variant that saves and clears the state but keeps the callback and returns 1. It has **no callers** (dead code).

Callers (6): `MainLoop` (fallback to the main menu), `OpponentSelect_ExitToMainMenu` (goes to the menu), main-menu launch `0x08003A84` (goes to `gMainMenuTable[cursor]`), `0x0801BCBA` (Campaign: `SetMainCallback(NULL)`, a probable soft reset), `0x08029DF8` (card viewer, B goes to the menu), debug menu `0x08074A08`.

## The step-runner pattern (verified)
Almost every main callback (and many steps) is a copy of this template, with a different table and index byte:
```c
u16 CB_Something(void) {
    u8 *idx = &gMain.<indexByte>;
    if (sTable[*idx] == NULL)
        return 1;                       // done → MainLoop goes back to the main menu
    if (sTable[*idx]()) {               // step returns nonzero when finished
        (*idx)++;
        /* zero the deeper sub-state bytes, e.g. gMain+0x4858..0x485B */
    }
    return 0;
}
```
Steps are usually `switch (gMain.<subState>)` machines that return 0 until done. Runners found by scanning for the template (`push {r4,r5,lr}; ldr r1,=table; ldr r5,=gMain; ldr r0,=off; …`):

| Runner | Table | Index byte | Steps | Role |
|---|---|---|---|---|
| `0x08004EAC` | `0x0819879C` | +0x4878 | `08004AF8 08004B84 08004CBC 08004D60` | License / boot logos ([[license-sequence]]) |
| `0x080057BC` | `0x081988B0` | +0x4878 | `0800527C 08005310 08005368 0800548C 0800545C 0800553C 08005718` | Title ([[title-screen]]) |
| `0x08003AA4` | `0x081984F4` | +0x4859 | `08003920 080039C0 080039E0 08003A58` | Main menu ([[main-menu]]) |
| `0x0801D1E8` | `0x08198EAC` | +0x4857 | `0801B75C 0801BCFC 0801BE0C 0801BE58 08021A48 0801C938 0801BF80 0801CE68 0801D140` (+ `0801D158` after the NULL) | Campaign |
| `0x0801AD18` | `0x08198E7C` | +0x4858 | `0801A7F4 0801A8A4 0801A8CC 08021A48 0801ABA0`, then an error path at index 6: `0801ABCC 0801AC48 0801AC7C` | Link Battle (jumps to 6 when `0x020192E0+0x1B12` bit 5 is set) |
| `0x08002FD0` | `0x08198380` | +0x4859 | `08002980 08002A48 08002AA0 08002CE8` | sub-runner called with `bl` from Campaign step 1 |
| `0x0800736C` | `0x08198D50` | +0x4859 | `08006E94 08006FAC` | debug "Card Detail" |
| `0x08063AF8` | `0x081A572C` | +0x4859 | `08062F6C 08063040 08063330 08063354 0806347C 0806360C` | debug "Get a pack" (the pack-opening screen) |
| `0x0807CC28` | `0x081A7970` | +0x4859 | `0807C374 0807C46C 0807C4A8 0807C4CC 0807C7C8 0807C924` | Password |
| `0x08074A34` | `0x081A768C` | +0x4857 | `08074554 08074794 08074868 080749E8` | debug menu ([[debug-menu]]) |

Variants that don't fit the exact byte pattern: Deck Edit `0x0806EF04` (table `0x081A725C`, index +0x4859, with mode-dependent tables at `0x081A7270`…`0x081A72E4`), Record `0x08003E94` (`0x08198588`, +0x485B), Calendar `0x08002940` (`0x08198338`, +0x485B), Card Trading `0x0807D348` (`0x081A79A4`, +0x4859).

The duel has its own dispatcher: `0x08021A48` indexes the phase table `0x08198F80` with the byte at `0x02015EE8` (see [[program-flow]]).

## Matching notes
- The IE manipulations are repeated twice with IME toggling. That's probably two inline helpers or macros, something like `DisableHBlankIntr()` and `SetHBlankCallback(NULL)`.
- The sequencer resets are 8 separate `strb` instructions (0x4878, 0x4879, 0x487A, 0x4857…0x485B). Either the fields are individual bytes, or two byte arrays cleared by unrolled code.

Related: [[program-flow]], [[save-game]], [[agb-main]], [[ram-map]].
