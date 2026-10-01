---
title: Sound engine (Konami driver)
type: game
status: draft
confidence: medium
sources: [rom-analysis, gbatek]
updated: 2026-10-01
---
# Sound engine

EDS does not use Nintendo's m4a/MP2K ("Sappy"). It uses a custom Konami driver: a Thumb sequencer that drives the four GB PSG channels, plus six software PCM voices mixed by a small ARM mixer into two Direct Sound FIFOs. This answers the m4a question in [[nintendo-sdk-libraries]] and [[open-questions]].

**Method.** Hand disassembly (capstone) of `0x0807D3D0`–`0x0807ECF0`, the game-side wrappers at `0x08077A44`–`0x08077C10`, and the interrupt setup. Rows marked verified are read from code. Anything about *what* the tracks play is hypothesis.

Hardware interpretations were cross-checked against the reviewed [[gbatek]] sound, DMA, timer and interrupt sections. FIFO timing supplies 16-byte transfers regardless of the programmed count; the driver's DMA1 IRQ position increment follows that hardware behavior.

## Code layout
| Range | Mode | Contents |
|---|---|---|
| `0x08077A44`–`0x08077C10` | Thumb | game-side API (option flags, one-SE-per-frame throttle, current-BGM tracking). See [[sound-api]]. |
| `0x0807D3D0`–`0x0807EACF` | Thumb | driver: init, sequencer, request/stop/fade, DMA1 IRQ; 28/30 functions now matching C ([[sound-driver]]) |
| `0x0807EAD0`–`0x0807EC1C` | ARM | `SoundMixAll` / `SoundMixFifo`. Called through the Thumb-to-ARM veneer at `0x08080A18` (`bx pc; nop; b 0x0807EAD0`). |
| `0x0807EC1C`–`0x0807ECF0` | ARM | the inner mix loop. It is copied to IWRAM `0x03005A54` (0x38 words) by `SoundDmaInit` and executed there. See [[sound-mixer]]. |
| `0x0807ECF0`–`0x0807ECF8` | data | literal pool: sample tables `0x0811B420`, `0x08088A20` |

The driver starts right after the Card Trading scene code (which ends at `0x0807D3C0`). The current unit split is `sound_driver` (`0x0807D3D0`–`0x0807EACF`) and `sound_mixer_arm` (`0x0807EAD0`–`0x0807ECF7`); the older disassembler unit names are historical. See [[sound-driver]] for C status and matching details.

## Hardware setup (verified)
`SoundInit(IntrFunc *dma1Slot)` (`0x0807D578`) is called once from `GameInit` with NULL, because `GameInit` already put `SoundDma1Intr` into `IntrTable[9]`:
- `SOUNDCNT_X = 0x80` (master on); `SOUNDCNT_L = 0xFF77` (all 4 PSG channels on L+R at volume 7); `SOUNDCNT_H = 0x0E` (PSG 100%, DMA A and B 100%).
- `SOUNDBIAS` resolution bits = 01 (8-bit / 65.536 kHz PWM).
- Clears the PSG registers, loads a wave-RAM pattern (`SoundLoadWaveRam` `0x0807D518`, patterns at `0x08139550`, 16 bytes each), and initialises the driver struct at `0x03005210`.
- `SoundDmaInit` (`0x0807D3D0`):
  - `IE &= ~(DMA1|DMA2|TIMER0)`. Copies the inner mix loop to `0x03005A54`. Clears the 6 PCM voices and the 0x640-byte ring buffers.
  - `SOUNDCNT_H` high byte = `0xBB`: both FIFO A and FIFO B go to both speakers, both clocked by Timer0, both reset. So A and B are two mono buses of 3 voices each, not a stereo pair.
  - DMA1 copies from `0x03005414` to `FIFO_A`, and DMA2 copies from `0x03005734` to `FIFO_B`. Both use control `0xF600` (enable, IRQ, FIFO timing, 32-bit, repeat), count 4.
  - `IE |= DMA1|TIMER0`. `TM0CNT = 0x0080FCB9`: reload `0xFCB9` (839 cycles), no IRQ. That gives a sample rate of about 19 997 Hz (2²⁴/839), about 335 samples per frame.
- `SoundDma1Intr` (`0x0807E324`) fires every 16 samples (4 words). It adds 16 to `gSoundDmaPos`. Once the position passes `0x2BF` it stops DMA1/DMA2, rewinds both to the buffer starts, and resets the position to 0. The ring buffer is therefore 0x2C0 = 704 bytes per FIFO (≈ 35 ms ≈ 2.1 frames), even though 0x320 bytes are reserved.

## Per-frame operation (verified)
- **Main thread**, in [[frame-sync-update]]: `SoundMain` (`0x0807E554`) first calls `SoundStartPendingSE` (`0x0807E3D8`). Then, if a BGM request is pending (`+0x18A ≥ 0`) and it differs from the current song (or nothing is playing), it loads the song header and starts it.
- **VBlank IRQ**: `SoundVBlank` (`0x0807E3B0`). Unless flag `0x2000` is set (paused), it runs `SoundSequencerTick(&gSoundDriver)` (`0x0807DB58`, 0x7C4 bytes). That advances all 16 track slots, writes the PSG registers `0x04000062`–`0x0400007C`, and starts PCM voices through `SoundPcmStart` (`0x0807E918`). It then calls the ARM mixer. `SoundTrackUpdate` (`0x0807D6B4`, called 6× from the tick) is the per-channel update (hypothesis).
- **Mixing** runs inside the VBlank IRQ. IntrMain allows nested DMA1/Serial IRQs, so the FIFO refills keep going while the mixer runs. For each FIFO the mixer refills the region between the previous and current DMA read position (wrapping at 0x2C0): it clears it with DMA3, then adds 3 voices. See [[sound-mixer]].

## Data (verified addresses; formats partly hypothesis)
| Table | Address | Format |
|---|---|---|
| Song table | `0x080E09D0` | 58 entries × 24 bytes: `u32 songData; u16 trackOffset[10]` (offsets relative to `songData`; the first is 0). The table ends where the first song's data starts (`0x080E0F40`). |
| SE table | `0x08087FD0` | 28-byte entries: `u32 track[6]` (one per SE slot, NULL = unused), then `u8 priority; u8 slotMask; u16 flags`. About 48 entries before `0x08088510` (heuristic count). It immediately follows the AgbSram rodata (see [[save-type]]). |
| PCM sample tables | `0x0811B420` (id bit15 = 0), `0x08088A20` (id bit15 = 1, index = id & 0x3FFF) | pointers to sample headers `{u32 rate; u32 length; s32 loopStart (−1 = none); s8 data[]}` |
| Pitch table | `0x081A960C` | u16 per note. PCM step = `rate * pitch >> 12` (20.12 fixed point in the mixer). |
| Wave patterns | `0x08139550` | 16-byte wave-RAM images for PSG channel 3 |

## Driver state `gSoundDriver` @ `0x03005210` (partly verified)
| Off | Type | Meaning |
|---|---|---|
| +0x000 | u32 | init −1 |
| +0x004 | u32 | the current song's `songData` pointer |
| +0x008 | track[10] | BGM tracks, 0x18 bytes each (+0x10 = active flag byte) |
| +0x0F8 | track[6] | SE tracks, 0x18 bytes each (+0x0E s16 SE id, +0x10 priority, +0x11 slot mask, +0x13 flags: 0x80 active, 0x08, 0x04 stop, 0x01) |
| +0x188 | u16 | flags: 0x80 BGM playing, 0x40 SE started, 0x01 stop request, 0x04, 0x100, 0x200 wave-RAM bank, 0x2000 sequencer paused, 0x4000 |
| +0x18A | s16 | pending BGM request (−1 = none) |
| +0x18C | s16 | pending SE request (−1 = none) |
| +0x18E | s16 | current BGM id |
| +0x190…+0x197 | u8 | volume/fade bytes (+0x191/+0x193 = fade current/target, 0x10 = full), +0x195 SE lock priority, +0x197 SE variant (0–3, from the table at `0x081A79F9`) |

The PCM voices are at `0x030053AC` (6 × 0x10): `+0 u32 dataPtr; +4 u32 remaining; +8 u16 step; +0xA u16 frac; +0xC s16 sampleId; +0xE u8 flags (0x80 active, 0x40 loop); +0xF u8 volume`.

## Driver API (verified behaviour, names proposed)
| Address | Proposed name | Behaviour |
|---|---|---|
| `0x0807D578` | `SoundInit` | see above |
| `0x0807D3D0` | `SoundDmaInit` | Timer0 / DMA1 / DMA2 / FIFO setup, IWRAM code copy |
| `0x0807D518` | `SoundLoadWaveRam` | (drv, bank, idx) → WAVE_RAM, toggles bank flag 0x200, writes SOUND3CNT_L |
| `0x0807DB58` | `SoundSequencerTick` | the per-VBlank sequencer (0x7C4 bytes) |
| `0x0807E324` | `SoundDma1Intr` | FIFO ring-position IRQ |
| `0x0807E3B0` | `SoundVBlank` | tick + mix, unless paused |
| `0x0807E3D8` | `SoundStartPendingSE` | allocates SE slots by mask and priority |
| `0x0807E554` | `SoundMain` | main-thread request processing |
| `0x0807E674` | `SoundRequestBGM(id)` | `+0x18A = id`, `+0x194 = 0` |
| `0x0807E690` | `SoundRequestBGMEx(id, b)` | same, but `+0x194 = b` |
| `0x0807E6B0` | `SoundIsBGMPlaying(id)` | |
| `0x0807E764` | `SoundFadeOutBGM(speed)` | `+0x193 = 0`, `+0x192 = speed` |
| `0x0807E780` | — | fade plus flag 0x100 (fade then stop? hypothesis) |
| `0x0807E7E8` | `SoundIsFadeDone` (hypothesis) | `+0x191 == +0x193` |
| `0x0807E814` | `SoundRequestSE(id)` | if id bit 15 is set and the same SE is already playing, ignore it |
| `0x0807E870` | — | `SoundRequestSE` + variant + immediate start |
| `0x0807E898` | `SoundStopSE(id)` (hypothesis) | flags matching SE slots with 0x04 |
| `0x0807E918` | `SoundPcmStart` | (voice, sampleId, volume, note) |
| `0x0807E990` | `SoundCountActivePcm` | counts voices with active flag 0x80 |
| `0x0807E9C8` | — | pauses the tick and fast-forwards a song by n ticks (hypothesis: seek) |
| `0x0807EA88` / `0x0807EAA0` | — / `SoundStopBGM` | set flag 0x04 / 0x01 |

> [!warning] Resolved correction
> Earlier disassembly notes listed `SoundPcmStart(voice, sampleId, note, volume)` and placed the PCM-count entry at `0x0807E984`. The byte-matching C in [[sound-driver]] establishes volume in r2, note in r3, and the count entry at `0x0807E990`; the earlier labels were incorrect.

Game-side wrappers: [[sound-api]]. Known song ids are 0 (title, `PlayBGMNoTrack(0)`), 1 (New Game intro script), 3 (main menu) and 0x1B (a Campaign pre-duel BGM). SE ids are 0 (cursor move), 1 (confirm), 2 (cancel) and 3 (error buzz), from menu code (hypothesis).

## Open questions
- The track bytecode format (the command set of `SoundSequencerTick`).
- Which tracks go to PSG and which to PCM, and is it decided per song or per track?
- Why reserve 0x320 bytes per FIFO buffer when only 0x2C0 are used?

Related: [[program-flow]], [[interrupt-handlers]], [[ram-map]], [[sound-mixer]], [[sound-api]].
