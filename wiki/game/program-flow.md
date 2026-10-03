---
title: Program Flow (boot, main loop, scenes)
type: game
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Program Flow

This page covers how EDS gets from reset to gameplay: the boot code, the one-time init, the per-frame main loop, and the "scene" system that the main loop drives. Function-level detail lives on the linked function pages. The globals are in [[ram-map]].

**Method.** Everything marked *verified* was read directly from the baserom with capstone (Thumb and ARM), using scratch scripts that resolve literal pools, `bl` targets and pointer tables. The scene names come from the main-menu sprite sheet `0x087DE878` (rendered to an image: the labels are *Campaign, Link Battle, Deck Edit, Record, Calendar, Card Trading, Password*) and from the debug-menu name table at `0x081A73A0`.

## 1. Boot (verified)

```
ROM entry 0x08000000 ─► crt0 0x080000C0 (ARM)
   sp_irq = 0x03007FA0, sp_sys = 0x03007B00
   INTR_VECTOR (0x03007FFC) = IntrMain 0x080000FC (ROM copy)
   bx AgbMain 0x08075F64 (Thumb)     ; loops back to crt0 if it ever returns
AgbMain:
   GameInit      GameInit        ; one-time init
   MainLoop      MainLoop        ; never returns
```
See [[crt0]] and [[agb-main]].

### GameInit `GameInit` (verified)
crt0 doesn't initialise `.data` or `.bss`. This function clears RAM instead:
1. DMA3 32-bit fill of 0 over **all of EWRAM** (`0x02000000`, 0x10000 words) and over **IWRAM `0x03000000`–`0x03007A00`** (0x1E80 words). That leaves the stacks alone.
2. `ReadSram(0x0E000000, 0x02011C20, 0x2170)` loads the save into `gSaveData` (see [[save-type]], [[agb-sram]]).
3. Fills `IntrTable` (`0x03000000`): VBlank=`0x0807569D`, Timer2=`0x0807570D`, DMA1=`0x0807E325`, slot 13=`0x08075741`, and every other slot 0.
4. DMA3 copies `IntrMain` (0x400 bytes) to IWRAM `0x0300004C` and points `INTR_VECTOR` there.
5. Interrupts: `IME=0`, `IE = VBLANK|DMA1|DMA2|GAMEPAK|TIMER2`, `DISPSTAT = 0x18` (VBlank and HBlank IRQ enable), `IME=1`.
6. `WAITCNT = 0x4014` (WS0 3/1 wait states, prefetch on).
7. `gMain.callback` (`0x03000450`) = `0x08004EAD`, the **License** sequence. `gMain.vblankCallback` = NULL.
8. `SoundInit(NULL)` (`0x0807D578`). This also rewrites IE: it clears DMA1/DMA2/Timer0, then sets DMA1|Timer0. **Final IE = `0x2229`** (VBlank, Timer0, Timer2, DMA1, Gamepak). Timer0 never raises an IRQ because its control has no IRQ bit.
9. Timer2: reload `0xF400`, prescaler /1024, IRQ on. That's one tick every 3072×1024 cycles, about 0.1875 s. The handler increments a u16 at `0x030051FC`; its purpose is unknown.
10. Sets `gMain+0x40E` (u16) = 3, then `ResetBgScroll`, `SetBrightnessWhite`, `SetSeEnabled(1)`, `SetBgmEnabled(1)`, and `SetTextModeLatin` (writes `gSaveData+4` = 1).

> [!note] Sound on by default
> Step 10's `SetSeEnabled`/`SetBgmEnabled` run **after** the save is loaded. That means the SE/BGM flags stored in the save (`gSaveData+0x2152`) are forced on at every boot (hypothesis about intent; the behaviour itself is verified).

## 2. Main loop (verified)
`MainLoop` (`0x08075D70`) runs once per frame:
```c
for (;;) {
    gMain.intrCheck &= ~1;                  // 0x0300044C
    while (!(gMain.intrCheck & 1)) ;        // busy-wait: VBlankIntr sets bit 0
    FrameSyncUpdate();                      // 0x08075CB4
    gMain.lagCounter = 0;                   // 0x030048A6
    if (gMain.callback())                   // 0x03000450, called via _call_via_r0
        SetMainCallback(CB_MainMenu);       // 0x080754F8(0x08003AA5)
    DebugHook_Nop();                         // empty (bx lr); stripped debug hook?
    gMain.frameCounter++;                   // u16 0x0300489E
    gMain.frameCounter8++;                  // u8  0x030048A0
}
```
- **Frame wait.** There is **no `VBlankIntrWait`/`Halt`**. The game spins on a flag that the VBlank IRQ sets. The only SWIs in the ROM are CpuFastSet, CpuSet and Div ([[bios-swi-stubs]]).
- **`FrameSyncUpdate` runs in the main thread right after VBlank**, not in the IRQ. It writes BG scroll registers from shadow copies, CPU-copies the 16 KiB BG tilemap buffer and the OAM buffer to VRAM/OAM when flagged, reads keys, runs the sound driver's main-thread half, and advances the RNG. See [[frame-sync-update]], [[read-keys]], [[random]].
- **The main callback returns a u16.** Nonzero means "this scene is finished": the loop then switches to the **main menu**. Every top-level scene ends this way unless it installs a successor itself.
- `SetMainCallback(NULL)` is called at `0x0801BCBA` (inside Campaign step 0). With a NULL callback the loop executes `bx r0` with r0=0, which jumps to the BIOS reset vector. That would be a deliberate **soft reset** (hypothesis, unverified at runtime).

## 3. Interrupts (verified)
| Source | Handler | What it does |
|---|---|---|
| VBlank | `VBlankIntr` `0x0807569C` | sets `gMain.intrCheck` bit 0, increments counters, calls `gMain.vblankCallbackEarly` (`0x03000458`), runs the **sound sequencer + ARM mixer** (`SoundVBlank` `0x0807E3B0`), calls `gMain.vblankCallback` (`0x03000454`) |
| HBlank | per scene, `IntrTable[1]` | installed and cleared by scenes (for example `0x08004ABD` on the title screen), and cleared by `SetMainCallback` |
| Timer2 | `Timer2Intr` `0x0807570C` | reloads, increments u16 `0x030051FC` |
| DMA1 | `SoundDma1Intr` `0x0807E324` | tracks the Direct Sound FIFO ring-buffer position and restarts DMA1/DMA2 on wrap |
| Serial (+Timer3) | `0x08075F74` | link-cable multiplayer handler, installed by the link code (`LinkSioInit`); sync word `0xFEFE` |
| Gamepak | loops forever inside `IntrMain` | cartridge pulled |

Details: [[interrupt-handlers]].

## 4. Scene system (verified mechanism)
A "scene" is the current **main callback** `gMain.callback`. Almost every callback is a **step sequencer**: a function with a NULL-terminated table of step functions and a one-byte step index in `gMain`. Each frame it calls `table[index]()`. When the step returns nonzero it advances `index` and clears the step's sub-state bytes. When it reaches NULL it returns 1, so the main loop goes back to the menu. Steps are themselves usually small `switch (subState)` machines. The same pattern nests: a step can be another runner with a deeper index byte. See [[set-main-callback]] for the runner template and the index bytes (`gMain+0x4857`…`+0x485B`, `+0x4878`).

`SetMainCallback(fn)` (`0x080754F8`) **saves the game to SRAM first** ([[save-game]]). It then clears both VBlank callbacks, disables and clears the HBlank IRQ, zeroes the sequencer bytes (`gMain+0x4857`–`+0x485B` and `+0x4878`–`+0x487A`), and installs `fn`. So every scene change writes the save.

### Top-level scene graph
```
CB_License 0x08004EAC ──(installs directly)──► CB_Title 0x080057BC ──(returns 1)──► CB_MainMenu 0x08003AA4
CB_MainMenu ──SetMainCallback(gMainMenuTable[cursor])──► one of the 7 menu scenes
each menu scene ──(returns 1)──► MainLoop ──► SetMainCallback(CB_MainMenu)
```

| Scene | Callback | Evidence | Notes |
|---|---|---|---|
| License / boot logos | `0x08004EAC` | debug name "License"; string "LICENSED BY NINTENDO" | 4 steps at table `0x0819879C`, index `gMain+0x4878`. See [[license-sequence]]. |
| Title | `0x080057BC` | debug name "TITLE"; strings "New Game"/"Continue" | 7 steps at `0x081988B0`. See [[title-screen]]. |
| **Main menu** | `0x08003AA4` | debug name "Menu"; the main loop's fallback | 4 steps at `0x081984F4`. Launch table `0x081984D8`. See [[main-menu]]. |
| Campaign | `0x0801D1E8` | menu slot 0 | 9 steps at `0x08198EAC`, index `gMain+0x4857`; step 4 is the duel |
| Link Battle | `0x0801AD18` | menu slot 1; uses the SIO registers | 5 steps at `0x08198E7C` (plus an error path at +6); step 3 is the duel |
| Deck Edit | `0x0806EF04` | menu slot 2; "There are no cards." | steps at `0x081A725C`; mode bits in `gMain+0x4874` |
| Record | `0x08003E94` | menu slot 3 | 5 steps at `0x08198588`, index `gMain+0x485B`; state `0x0201F814` |
| Calendar | `0x08002940` | menu slot 4 | 4 steps at `0x08198338` (`0x0800257C`, `0x08002704`, `0x08002728`, `0x08002930`), index `gMain+0x485B`; state `0x0201F7D0` |
| Card Trading | `0x0807D348` | menu slot 5; reaches the link library (`LinkSyncStep`); debug string "Throw it in now !" | steps at `0x081A79A4` (the menu's A/B choice jumps to index 4, 6 or 8) |
| Password | `0x0807CC28` | menu slot 6; strings "Password:", "->%4X" | 6 steps at `0x081A7970`; state `0x0201F7B0`. Lookup: [[password-table]]. |
| Debug menu | `0x08074A34` | table of `{char name[0x40]; callback}` at `0x081A73A0` | **Never referenced**, so this is dead code. See [[debug-menu]]. |

**Shared duel driver.** Step `0x08021A48` appears in both the Campaign and Link Battle tables. Each frame it dispatches through a 10-entry duel-phase table at `0x08198F80`, indexed by the byte at `0x02015EE8`: `0x08021835, 0x0801F97D, 0x0804F169, 0x0804E421, 0x0804FC4D, 0x0804F461, 0x0804E949, 0x08050A71, 0x0805146D, 0x080218AD`. Entry 5 (`0x0804F460`) owns the strings "End your Main Phase?" and "End your turn?" (hypothesis: Main Phase). The other phases aren't mapped yet.

Campaign steps (hypothesis, from their callees), in order:

1. `0x0801B75C` pre-duel (opponent, scripts, BGM)
2. `0x0801BCFC` (runs the sub-runner `0x08002FD0` and dialogue `0x0801AE2C`)
3. `0x0801BE0C` deck setup
4. `0x0801BE58` duel init
5. `0x08021A48` **duel**
6. `0x0801C938` post-duel (debug print "Changed!! %d -> %d")
7. `0x0801BF80`
8. `0x0801CE68` reward (pack code near `0x08063B48`, see [[booster-packs]])
9. `0x0801D140`

When the table ends, `FadeToBlack(8)` runs and the game goes back to the menu.

## 5. Call graph (depth ≤ 3 from AgbMain)
```
AgbMain 0x08075F64
├─ GameInit 0x08075DF4
│  ├─ ReadSram 0x0807ED28                       (AgbSram)
│  ├─ SoundInit 0x0807D578
│  │  ├─ SoundLoadWaveRam 0x0807D518
│  │  └─ SoundDmaInit 0x0807D3D0               (Timer0 + DMA1/DMA2 → FIFO A/B; copies mixer loop to IWRAM)
│  ├─ ResetBgScroll 0x080757AC ─ ResetBgHofs 0x08075744, ResetBgVofs 0x08075778
│  ├─ SetBrightnessWhite 0x08075A30
│  ├─ SetSeEnabled 0x08077A74, SetBgmEnabled 0x08077AB0
│  └─ SetTextModeLatin ─ SetTextMode              (gSaveData+4)
│  (installs: VBlankIntr 0x0807569C, Timer2Intr 0x0807570C, SoundDma1Intr 0x0807E324,
│             GamepakIntr 0x08075740, callback CB_License 0x08004EAC)
└─ MainLoop 0x08075D70
   ├─ FrameSyncUpdate 0x08075CB4
   │  ├─ CopyDoubleWords 0x080752B0             (BG map buffer → VRAM)
   │  ├─ FlushOamBuffer 0x08075C44
   │  ├─ ReadKeys 0x08075228
   │  ├─ SoundMain 0x0807E554 ─ SoundStartPendingSE 0x0807E3D8
   │  └─ Random 0x08076F9C
   ├─ _call_via_r0 0x0807EE98 → gMain.callback (current scene)
   ├─ SetMainCallback 0x080754F8 ─ SaveGame 0x080754BC ─ UpdateSaveChecksum, WriteSram, VerifySram
   └─ DebugHook_Nop (empty)
VBlankIntr 0x0807569C
   ├─ gMain.vblankCallbackEarly (0x03000458)
   ├─ SoundVBlank 0x0807E3B0 ─ SoundSequencerTick 0x0807DB58, veneer 0x08080A18 → SoundMixAll 0x0807EAD0 (ARM)
   └─ gMain.vblankCallback (0x03000454)
```

## 6. Per-frame timeline (summary)
1. **VBlank IRQ.** Sets the flag, runs the music sequencer and mixes PCM into the FIFO ring buffers. Nested DMA1/Serial IRQs are allowed while it runs (see [[crt0]]).
2. **Main thread wakes.** Scroll registers, 16 KiB BG map copy (if `gMain.vblankFlags & 2`), OAM copy (if `& 1`), key read, start pending BGM/SE, RNG step.
3. **Scene callback** builds the next frame: sprites go into the OAM buffer through `AddSprite` (`0x080761F0`), tiles into the BG map buffer.

## Open questions
- What is the Timer2 tick counter (`0x030051FC`) used for? Link timeouts? Play time?
- Is `SetMainCallback(NULL)` at `0x0801BCBA` really an intentional soft reset?
- What are the two u16 values that `ResetVideo` sets through `SetTextArea(0, 0x27E)` (`gMain+0x441C/+0x441E`)?

Related: [[ram-map]], [[sound-engine]], [[interrupt-handlers]], [[set-main-callback]], [[main-menu]], [[game-overview]].
