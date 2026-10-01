---
title: SoundMixAll / SoundMixFifo / SoundMixChannel (ARM mixer, proposed names)
type: function
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# ARM PCM mixer

| Function | Address | Size | Mode | Unit | Match |
|---|---|---|---|---|---|
| `SoundMixAll` | `0x0807EAD0` | 0x20 | ARM | `src/sound_mixer_arm.s` | **matching** (authored asm) |
| `SoundMixFifo` | `0x0807EAF0` | 0x114 + pool `0x0807EC04`–`0x0807EC1B` | ARM | same | **matching** (authored asm) |
| `SoundMixChannel` | `0x0807EC1C` (ROM image) / **runs at `0x03005A54`** | 0xD4 + pool `0x0807ECF0`–`0x0807ECF7` | ARM | same | **matching** (authored asm) |
| veneer | `0x08080A18` | 8 | Thumb→ARM | `veneer` | linker-generated `__sub_0807EAD0_from_thumb` |

This is hand-written ARM assembly (hypothesis: there are no agbcc ARM-mode patterns, and `SoundMixChannel` returns through `bx r1` into a computed label). It's called once per VBlank from `SoundVBlank` (`0x0807E3B0`) after the sequencer tick. See [[sound-engine]].

## Behaviour (verified)
`SoundMixAll`: `r7 = gSoundPcmChannels (0x030053AC)`; `SoundMixFifo(r1=0)` (FIFO A, voices 0–2); `SoundMixFifo(r1=4)` (FIFO B, voices 3–5). `r7` carries across the two calls.

`SoundMixFifo(r1 = 0|4)`:
1. `cur = gSoundDmaPos[r1]`, `prev = gSoundDmaPos[r1+2]`, then `gSoundDmaPos[r1+2] = cur`.
2. `buf = 0x03005414 + r1*0xC8` (A = `0x03005414`, B = `0x03005734`).
3. It refills the span that the DMA has consumed since the last frame: `[prev, cur)`, or `[prev, 0x2C0) + [0, cur)` on wrap. Each span is first cleared with a **DMA0** (`0x040000B0`) 32-bit fill from a zero on the stack (`0x85000000 | words`, busy-wait on the enable bit).
4. For each of 3 voices: skip it if `flags` bit 7 (active) is clear or `volume` (+0xF) is 0. Otherwise call the IWRAM routine at `0x03005A54` for each span with `r5 = dataPtr`, `ip = remaining`, `r2 = step`, `r3 = frac`, `r8 = volume+1`, `r6 = dst`, `r4 = count`. Then write back `dataPtr`, `remaining`, `frac`.

`SoundMixChannel` (IWRAM) computes, per output byte, `out += (s8 sample * (vol+1)) >> 4`. `frac += step`, and the whole part (`frac >> 12`) advances the sample pointer and decrements `remaining`. `step == 0x800` has a special path that writes each sample twice. At the end of a sample, if `flags` bit 6 (loop) is set, it reloads from the sample header. The header comes from the table at `0x0811B420` if `sampleId` bit 15 is clear, else `0x08088A20`, indexed by `id & 0x7FFF` (the ARM code shifts `id << 17 >> 15`, and the Thumb driver masks the bank-1 index with `0x3FFF`). The new pointer is `hdr+0xC+loopStart` and the remaining count is `length − loopStart`. Otherwise it clears `flags`.

## Notes
- The mix adds into signed 8-bit samples with **no saturation**. Clipping is avoided only by keeping the voice volumes low (hypothesis).
- The IWRAM copy is exactly 0x38 words from `0x0807EC1C`. It includes the literal pool at `0x0807ECF0` **and** the first two instructions of `CpuFastSet` (`0x0807ECF8`), which the copy count over-reaches into. That's harmless.

## Matching notes
- **Done (2026-10-01):** `src/sound_mixer_arm.s` is an authored source that replaces `asm/sound_mixer_arm.s` in the link. It uses named struct offsets (`PCM_*`), labelled literal-pool words (the duplicated `0x03005A54` pool entry is kept), `adr` for both computed restart points (GAS emits the original `sub r1, pc, #0x48/#0x3C`) and RAM symbols resolved by autosyms. `make compare` gives `eds.gba: OK`. objdiff treats it like the SDK: target = base = `build/srcasm/sound_mixer_arm.o`.
- Corrections found while authoring: the span clear uses DMA0, not DMA3, and the loop-sample index is 15 bits wide in the ARM code.
- Hand asm. It must be reproduced byte for byte. The `sub r1, pc, #0x48` / `bx r1` return trick and the `ldrsb … !` pre-index forms must be kept.
- Unit: all three routines are one unit, `sound_mixer_arm` (`units.txt`). Earlier notes naming `asm/code_0807DB58.s` / `code_0807EC1C` were stale.

Related: [[sound-engine]], [[interrupt-handlers]], [[thumb-and-arm]].
