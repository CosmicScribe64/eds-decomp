---
title: RAM Map (globals)
type: rom
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# RAM Map

This page lists the known EWRAM (`0x02xxxxxx`) and IWRAM (`0x03xxxxxx`) globals. Names are **proposals** (pret-style `gName`). They are also listed in `config/names.txt`.

**Method.** A scratch script ran constant propagation over every Thumb function it found (bl targets plus pointer tables, about 2100 functions). For each load or store it resolved `base literal + offset` and recorded the address, access width and function. The "verified" rows were also read by hand in the disassembly. Offsets inside `gMain` come from agbcc's `ldr rB,=0x03000040; ldr rO,=off; add` pattern.

## IWRAM (`0x03000000`–`0x03008000`)

| Address | Size | Name (proposed) | Status | Meaning / evidence |
|---|---|---|---|---|
| `0x03000000` | 0x40 | `IntrTable` | verified | 16 IRQ handler pointers in the dispatcher's priority order (Serial, HBlank, VBlank, VCount, Timer0–3, DMA0–3, Keypad, 13 = Gamepak stub). Filled by `GameInit`. See [[interrupt-handlers]]. |
| `0x03000040` | ≈0x488C | `gMain` | verified (it is one struct) | The big system struct. `ReadKeys`, `MainLoop` and the VBlank handler all use base `0x03000040` plus offsets, so it's one object rather than separate globals. Fields are below. |
| `0x0300004C` | 0x400 | `gMain.intrMainBuf` | verified | IWRAM copy of `IntrMain`. `INTR_VECTOR` points here after `GameInit`. |
| `0x030049D0` | ? | `gUnk_030049D0` | hypothesis | Base of a struct used by the link/VBlank code (`0x08071FA0`–`0x080723B4`, `0x08023228`). `+0x82C` (`0x030051FC`, u16) is incremented by `Timer2Intr`. |
| `0x03005210` | 0x198 | `gSoundDriver` | verified (layout partly) | Konami sound-driver state: 10 BGM track slots at +0x08, 6 SE track slots at +0xF8 (0x18 bytes each), flags at +0x188, requests at +0x18A/+0x18C. See [[sound-engine]]. |
| `0x030053AC` | 0x60 | `gSoundPcmChannels` | verified | 6 PCM voices × 0x10 bytes. Voices 0–2 mix into FIFO A and 3–5 into FIFO B. |
| `0x0300540C` | 8 | `gSoundDmaPos` | verified | u16 FIFO-A position/prev and FIFO-B position/prev. The DMA1 IRQ advances it by 16 each time. |
| `0x03005414` | 0x640 | `gSoundPcmBuffer` | verified | 2 × 0x320-byte signed-8-bit ring buffers (A at `0x03005414`, B at `0x03005734`). 0x2C0 bytes of each are used. |
| `0x03005A54` | 0xE0 | `gSoundMixCodeRam` | verified | IWRAM copy of the ARM inner mixing loop (`0x0807EC1C`, 0x38 words). |
| `0x03005B38`, `0x03005B50` | ? | (unnamed) | unknown | Only referenced by libc-area code (`0x0807F404`, `0x080800B4`). |
| `0x03005B60` | ≈0xB18 | `gLinkSio` (hypothesis) | verified used by link code | Link-cable state for `sub_080735D4`/`0x08075F74` and friends. `+0xA2C` holds a counter/state (−3…10), and there are send/receive buffers. |
| `0x03006644` | 8 | `gSioMultiRecv` (hypothesis) | verified | Snapshot of `SIOMULTI0`–`3` taken at the top of the serial IRQ. |
| `0x03007B00` | n/a | (SYS stack top) | verified | crt0 `sp_sys`. The user stack grows down from here. |
| `0x03007FA0` | n/a | (IRQ stack top) | verified | crt0 `sp_irq`. |
| `0x03007FFC` | 4 | `INTR_VECTOR` | verified | BIOS IRQ vector, set to `0x0300004C`. |

### `gMain` fields (base `0x03000040`)

| Addr | Off | Type | Proposed field | Status | Evidence |
|---|---|---|---|---|---|
| `0x03000040` | +0x000 | u32 | `rngState` | verified | `Random` (`0x08076F9C`): `x = x*0x343FD + 0x269EC3`, then rotate by 16. Stepped once per frame. See [[random]]. |
| `0x03000044` | +0x004 | u16 | `heldKeys` | verified | `~KEYINPUT`. Read by 21 functions. |
| `0x03000046` | +0x006 | u16 | `newKeys` | verified | held & ~previous, plus D-pad auto-repeat. Read by about 99 functions. |
| `0x03000048` | +0x008 | u16 | `prevKeys` | verified | the last different key state (for repeat) |
| `0x0300004A` | +0x00A | u16 | `keyRepeatTimer` | verified | counts to 20, then repeats every 2 frames. See [[read-keys]]. |
| `0x0300004C` | +0x00C | u8[0x400] | `intrMainBuf` | verified | IntrMain RAM copy |
| `0x0300044C` | +0x40C | u16 | `intrCheck` | verified | bit 0 set by `VBlankIntr`, cleared and polled by `MainLoop` |
| `0x0300044E` | +0x40E | u16 | `vblankFlags` | verified | bit0: copy the OAM buffer; bit1: copy the BG map buffer; bits 4–7: write BG0–3 HOFS; bits 8–11: write BG0–3 VOFS. Set by 31 functions. |
| `0x03000450` | +0x410 | fnptr | `callback` | verified | the current scene (main callback). Returns u16 "done". |
| `0x03000454` | +0x414 | fnptr | `vblankCallback` | verified | called at the end of `VBlankIntr`. Set by about 19 scene functions. |
| `0x03000458` | +0x418 | fnptr | `vblankCallbackEarly` | verified | called before the sound mixer in `VBlankIntr`. Set to `0x08072055` by the link code (`sub_08072510`). |
| `0x0300045C` | +0x41C | u16[8][0x400] | `bgMapBuffer` | verified | 8 screenblocks (16 KiB) copied to VRAM `0x06000000` when `vblankFlags & 2`. Cleared by `ClearBgMapBuffers`. |
| `0x0300445C` | +0x441C | u16, u16 | ? | verified written | set by `sub_0807289C(a,b)`, which `ResetVideo` calls with (0, 0x27E) |
| `0x03004460` | +0x4420 | u16[4] | `bgVofs` | verified | shadow BG0–3 VOFS |
| `0x03004468` | +0x4428 | u16[4] | `bgHofs` | verified | shadow BG0–3 HOFS |
| `0x03004470` | +0x4430 | u8[0x400] | `oamBuffer` | verified | 128 OAM entries, copied to OAM when `vblankFlags & 1`, then reset. `AddSprite` appends here. |
| `0x03004870` | +0x4830 | u8 | `oamCount` | verified | next free OAM slot (max 0x80) |
| `0x03004871` | +0x4831 | u8 | ? | verified | reset together with `oamCount` (affine count? hypothesis) |
| `0x03004872` | +0x4832 | u8 | `brightness` | verified | low 6 bits: current fade level 0–0x1F, mirrored to `BLDY` by the fade helpers. See [[fade-functions]]. |
| `0x03004874` | +0x4834 | u16 | ? | seen | used by `0x0800696C`, `0x08006D08` |
| `0x03004897` | +0x4857 | u8 | `seqIndexCampaign` | verified | step index of the Campaign and debug-menu runners |
| `0x03004898` | +0x4858 | u8 | `seqState0` | verified | sub-state of top-level steps and index of some runners |
| `0x03004899` | +0x4859 | u8 | `seqIndex1` | verified | index of the Menu, Password, Trading and Deck Edit runners; also a frame timer in the License steps |
| `0x0300489A` | +0x485A | u8 | `seqState1` | verified | sub-state for steps of the `+0x4859` runners |
| `0x0300489B` | +0x485B | u8 | `seqState2` | verified | index or sub-state (Record runner index) |
| `0x0300489C` | +0x485C | u16 | `currentBgm` | verified | the last BGM started through `PlayBGM`. 0xFFFF = none. |
| `0x0300489E` | +0x485E | u16 | `frameCounter` | verified | +1 per main-loop iteration |
| `0x030048A0` | +0x4860 | u8 | `frameCounter8` | verified | +1 per main-loop iteration |
| `0x030048A1` | +0x4861 | u8 | `vblankCounter8` | verified | +1 per VBlank IRQ |
| `0x030048A2` | +0x4862 | u16 | ? | seen | `0x08005818`, `0x0805DC38` |
| `0x030048A4` | +0x4864 | u16 | `vblankCounter` | verified | +1 per VBlank IRQ |
| `0x030048A6` | +0x4866 | u16 | `lagCounter` | verified | +1 per VBlank, zeroed each main-loop iteration |
| `0x030048A8` | +0x4868 | u16 | `lastSeFrame` | verified | `PlaySE` allows only one SE per `frameCounter` value |
| `0x030048B0` | +0x4870 | u8 | ? | seen | duel/link code (11 functions) |
| `0x030048B4` | +0x4874 | u8 | `deckEditMode` (hypothesis) | verified | low 2 bits: Deck Edit clears them, Card Trading sets them to 1 |
| `0x030048B8` | +0x4878 | u8 | `seqIndexTop` | verified | step index of the License and Title runners |
| `0x030048B9`,`BA` | +0x4879/+0x487A | u8 | ? | verified cleared | zeroed by `SetMainCallback` |
| `0x030048BC` | +0x487C | u32 | ? | seen | Campaign |
| `0x030048C4`/`C6` | +0x4884/+0x4886 | u16 | dialogue state | seen | `StartDialogue` `0x08001C10` (+0x4886 = event id, hypothesis), `0x080017E8` |
| `0x030048C8` | +0x4888 | u8 | ? | seen | Campaign flags |
| `0x030048CA` | +0x488A | u16 | ? | verified | bits 4–11 cleared on every Campaign/Link/Menu step change (mask `0xF00F`) |

## EWRAM (`0x02000000`–`0x02040000`)

The whole of EWRAM is zeroed at boot. The addresses below are **base literals** as the code loads them; field offsets hang off them. "Users" gives the code address range of the referencing functions, which hints at the owning subsystem.

| Address | Name (proposed) | Status | Users | Meaning / evidence |
|---|---|---|---|---|
| `0x02000000` | `gTextWork` (hypothesis) | verified (writes) | `0x08074B08`–`0x08075114` (text renderer) | 64 KiB text/glyph work area. `sub_08074B08(a, b)` stores `a`/`b` at `0x02010000`/`0x02010001`, zeroes `0x02010004`, and clears `0x02000000`–`0x0200FFFF`. The License screen calls it with (0x20, 3). The parameters' meaning is unknown. |
| `0x02010014` | (unnamed) | verified | `ClearBgMapBuffers` | 0x1C00 bytes, cleared along with the BG map buffers. The u16 at `0x02010010` is cleared too. |
| `0x02011C20` | `gSaveData` | verified | 82 functions | 0x2170-byte save image, mirrored to SRAM `0x0E000000`. See [[save-game]]. `+0x8` = card trunk, `u32` per card ID with the owned count in bits 0–9 (per [[special-card-lists]]). |
| `0x02011C24` | `gSaveData+4` | verified | text renderer | u8. Bit 7 selects between two text-render paths (`0x08074E60` / `0x08074F50`). Set to 1 at boot by `sub_080770BC(1)`. |
| `0x02013D72` | `gSaveData+0x2152` | verified | sound API | u16 option flags: bit0 = SE on, bit1 = BGM on |
| `0x02013D86` | `gSaveData+0x2166` | verified | save code | 8-byte signature `"DMEX1INT"` (from `0x081A78A8`) |
| `0x02013D8E` | `gSaveData+0x216E` | verified | save code | u16 checksum = −Σ(u16 words 0…0x10B5) |
| `0x02013D90`, `0x02013DE0` | (unnamed) | seen | `0x08000420`–`0x0807093C` | right after the save image. Used by the script/text code. |
| `0x02014888` | `gTextBox` (hypothesis) | seen | `0x080002C0`–`0x080017E8` | text-box / dialogue module state ([[text-system]]). Debug print "DM5:Script=%3d(%5d) / BG = %d". |
| `0x0201527C` | `gTitleState` | verified | title screen | bit0 = save present (checksum valid), bit1 = Continue selected |
| `0x02015ED8` | `gMainMenuCursor` | verified | main menu | u16 0–6. See [[main-menu]]. |
| `0x02015EE8` | `gDuelCtrl` (hypothesis) | verified | 49 functions, `0x0800D524`–`0x0805B884` | byte +0 = duel-phase index into the table at `0x08198F80`. Bit 0 of byte +1 is tested on Link Battle paths (link flag? hypothesis). |
| `0x02017A40` | (unnamed) | seen | 190 functions (duel) | duel data |
| `0x020185C0` | (unnamed) | seen | 166 functions (duel) | duel data |
| `0x020192E0` / `0x020192E4` / `0x0201930C` | (unnamed) | seen | 163 / 270 / 363 functions (`0x08007xxx`–`0x08062xxx`) | the most-referenced duel structures (field/card state, hypothesis). `+0x1B12` bit 5 is a link-error flag checked by Link Battle. |
| `0x0201AE60` | (unnamed) | seen | 85 functions (duel UI) | |
| `0x0201CFB0` | (unnamed) | seen | 167 functions (duel) | |
| `0x0201DB20` | `gDeckEdit` (hypothesis) | seen | 68 functions, `0x08064F90`–`0x08070F18` | Deck Edit state. `+0x1C5A` flags. |
| `0x0201F780` | `gCardTrading` (hypothesis) | verified | `0x0807CCAC`–`0x0807D348` | Card Trading state |
| `0x0201F7B0` | `gPassword` (hypothesis) | verified | `0x0807BF68`–`0x0807CB74` | Password scene state |
| `0x0201F7D0` / `0x0201F7E0` | `gCalendar` (hypothesis) | verified | `0x08001F08`–`0x080036FC` | Calendar scene state |
| `0x0201F814` | `gRecord` (hypothesis) | verified | `0x08003AF4`–`0x08003F88` | Record scene state |
| `0x02020310` | (unnamed) | seen | 51 functions, `0x08026194`–`0x08064BA0` | a card-list / detail viewer (it can return to the menu with B: `0x08029DCC`) |
| `0x02030000`, `0x02031014`, `0x0203A614` | (unnamed) | seen | few | large buffers |

## Open questions
- What is `gSaveData+4` bit 7 (two text paths)? A leftover from a language or font switch?
- What is the Timer2 counter at `0x030051FC` for?
- Where does `gMain` end exactly? The last seen field is `+0x488A`, and nothing is referenced between `0x030048CC` and `0x030049D0`.

Related: [[program-flow]], [[gba-memory-map]], [[save-type]], [[sound-engine]].
