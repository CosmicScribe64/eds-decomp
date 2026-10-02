---
title: Sound sequence format (SE and BGM bytecode)
type: data
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Sound sequence format

The Konami driver ([[sound-engine]]) plays two kinds of sequence: sound effects (SE) and songs (BGM). Each has a table and a track bytecode. Both bytecodes were read from the matched decoders: `sub_0807D6B4` decodes SE tracks, and `SoundSequencerTick` (`sub_0807DB58`) decodes BGM tracks ([[sound-driver]]). The formats are **verified**: `tools/assetfmt/sound_seq.py` decodes all SE and BGM data to text and assembles it back byte-identical, and it round-trips 300 synthetic SE and 300 BGM blobs that use every opcode. The text syntax shown here is the one of the extracted files ([[assets]]).

## Timing
A track event is preceded or followed by a tick count. One tick is one VBlank, so 59.73 per second. Counts 0–0xEF take one byte. Counts 0xF0–0xFFF take two bytes, `0xF0 | hi, lo`. A count of 0 runs the next event in the same frame.

## SE table (`0x08087FD0`, 48 × 0x1C)
```c
struct SoundEffect {        /* 0x1C bytes */
    const u8 *track[6];     /* sq2, noise, pcm5, pcm4, pcm3, pcm2; NULL = unused */
    u8  priority;           /* refused if a needed channel plays an SE of higher priority */
    u8  slotMask;           /* 0x20 sq2, 0x10 noise, 0x08 pcm5, 0x04 pcm4, 0x02 pcm3, 0x01 pcm2 */
    u16 lockTicks;          /* starting the SE sets the driver's SE lock counter to this */
};
```
- The table has exactly 48 entries. It ends where the track data starts (`0x08088510`, SE 37's track).
- The SE lock counter counts down once per frame. An SE with non-zero `lockTicks` is refused while the counter is non-zero.
- `SoundRequestSE` variants 1–3 remap the four PCM tracks through `se_variant_channel_order` (`0x081A79F4`, `u8[4][6]`). The code addresses that table through `gUnk_081A79F9`, which is `&row[0][5]`.
- Only the first three track slots (sq2, noise, pcm5) are used. There are 48 track-entry files plus one called subroutine (`sub_0808866A`). SE 4 and SE 5 share a track.

## SE track bytecode
Each event is `[ticks][opcode][operands]`, with the ticks first. The low nibble of the tone opcodes holds flags: `env` (bits 0–1, the low bits of the envelope byte), `temp` (bit 2: don't store as the new base pitch) and `silent` (bit 3: store only, don't send). Pointers are absolute ROM addresses.

| Text | Opcode | Meaning |
|---|---|---|
| `freq F vol=V` | 0xB0 | raw 12-bit frequency register value F, volume V; the channel restarts only when volume or envelope change |
| `note N vol=V` | 0xE0 | pitch N on the PSG frequency table (1/32 semitones from C2) |
| `freq_rel delta=D vol_delta=W` | 0xA0 | frequency = base + D (−1024..1023); volume = current + W (−16..15, clamped 0..15) |
| `note_rel delta=D vol_delta=W` | 0xC0 | the same, on the PSG-table pitch |
| `note_add delta=D vol_delta=W` | 0xD0 | base pitch + D (s16) |
| `noise vol=V nr43=B` | 0x90 | noise channel at volume V, `SOUND4CNT_H` byte B, restarted |
| `setenv vol=V env=B` | 0x80 | store volume and envelope byte without sending |
| `pcm sample=S vol=V link=L [pitch=P] [wait]` | 0x60 | start bank-1 PCM sample S. The volume byte is `L<<4 \| V`, and the next L SE tracks are held silent until this one ends. `pitch` (bit 2) adds a u16 pitch-table index. `wait` (bit 3) with ticks 0 waits for the sample to end |
| `pitch P` | 0x30 | base pitch = P (12 bits), sent |
| `vol V` / `vol_add D [temp]` | 0x50 / 0x40 | set / add to the volume (`vol_add` clamps to 0..63; `temp` sends without storing) |
| `break_loop track=T` | 0x70 | sets SE track T's loop counter to 1 |
| `call L` / `jump L` | 0xFA / 0xFB | subroutine call (returns at `end`) / jump |
| `loop N L` | 0xFC | jump back to L N more times, so the block plays N+1 times. N = 0 jumps unless the SE is being stopped, which gives a release section |
| `waitpcm` | 0xF9 | wait until the track's PCM voice stops |
| `silence` / `keyoff` / `end` | 0xFD / 0xFE / 0xFF | note off / PCM key-off / return from a call, or end of track |
| `nop` | 0xF0–0xF8 | nothing |
| `raw` | 0x00–0x2F | unused by the game: 0x00–0x1F re-dispatch forever (the driver hangs); 0x20–0x2F read the next byte as a pitch low byte without consuming it, so it is read again as the next tick count |

Use in the game: `freq` 189, `noise` 77, `pcm` 27, `freq_rel` 10, `loop` 2, `call` 1, `end` 49.

## Song table (`0x080E09D0`, 58 × 0x18)
`{u32 songData; u16 trackOffset[10]}`, with offsets relative to `songData` (the first is 0). A song has ten tracks: `sq1`, `sq2`, `wave`, `noise` (PSG channels 1–4), then `pcm0`–`pcm5` (the six PCM voices). Tracks are therefore assigned to channels by position, not per song. A song's unused tracks all point to one empty track of that song (`00 FD`, "wait 0, end").

## BGM track bytecode
A track is `[ticks]`, then `[opcode][operands][ticks]` repeated, then a terminator with no ticks. The text writes the first count as `<ticks> wait`.

| Text | Opcode | Meaning |
|---|---|---|
| `note N vol=V` | 0xD0 | pitch = N semitones (on `sq1`/`sq2`/`wave` written as a name, C2 = 0 … B8 = 83). Restarts the note only if V differs from the current volume |
| `vol V` | 0xC0 | set the volume. On PSG tracks this restarts the note at the new volume; songs use runs of `vol` as envelopes |
| `play inst=I vol=V [note=S]` | 0xA0 / 0xB0 | start instrument I at volume V (on PCM tracks: bank-0 sample I). `note=` (signed semitones) selects the 0xB0 form |
| `off` | 0xE0 | note off (volume 0) |
| `vibrato D` | 0xF1 (0xF4–0xFC alias) | vibrato depth D>>1, using the sine table at `0x081ABC4C`; 0 stops it |
| `loopstart` | 0xF3 | this track's loop point |
| `loop` / `stop` / `end` | 0xFE / 0xFF / 0xFD | every track restarts from its loop point / the song stops / this track ends |
| `bend D` | 0xF2 | temporary pitch offset of D/32 semitones |
| `pan B` | 0xF0 | PSG: the channel's NR51 output bits. PCM: volume `B & 15`, and the next voice's volume `B >> 4` |
| `wave N` | 0x80 | PSG instrument; N > 3 loads wave-RAM pattern bank N−4 |
| `call 0xPPPP count=C` | 0x90 | play C events from position P (relative to the track's table offset), then return |
| `rest` | 0x00–0x7F | nothing |

Use in the game: `play` 35,576, `off` 21,016, `note` 19,672, `vol` 6,909, `vibrato` 1,357, `loopstart` 300, `end` 325, `loop` 54, `stop` 4. `call`, `wave`, `pan`, `bend` and `rest` never occur. `sq1` carries the `loop`/`stop`. PCM tracks 4–5 play drums with `play inst=` at base pitch, and tracks 6–9 use `play inst=31-35 note=`.

**Loop mechanism (verified from the matched `sub_0807DB58`).** `0xF3` adds the current position to the track's base offset. `0xFE` resets every track's position to 0 without restoring the base, so each track restarts at its own `0xF3`. 54 of the 58 songs loop this way. Songs 0 (title, 22.6 s), 20 (7.9 s), 22 (4.0 s) and 23 (3.2 s) end with `0xFF`.

**Quirks (unused by the game).**
- `0x9X` counts *events*, not repeats, and never clears the return position. After returning, the counter keeps decrementing and would jump back again 256 events later.
- On the noise track, `note N` indexes the 6-entry noise table at `0x08139F50` without a bound. `note 1` reads `gUnk_08139F50[32]`, 64 bytes past it, inside the duelist table at `0x08139F90` (value 0x0000). `note 0` reads 0x8000. 13 songs use `note 1` on noise and 2 use `note 0`. The effect is the noise register written without the restart bit (observation).

## Driver lookup tables
The four tables fill `0x081A7A0C`–`0x081ABC4C`. Each formula was checked against every value.

| Name | Address | Size | Content |
|---|---|---|---|
| `volume_nibble_scale` | `0x081A7A0C` | 16 × 256 u8 | `table[k][x]` scales both nibbles of x by k/15 (floor). No code reference found. Hypothesis: it generated pre-scaled wave patterns, but `wave_ram_patterns` doesn't match it |
| `pcm_pitch` | `0x081A8A0C` | 3,072 u16 | `round(4096 * 2^((i - 1536) / 384))`: the PCM step multiplier per 1/32 semitone, −48..+48 semitones. Code uses its middle, `gUnk_081A960C` (= 0x1000) |
| `psg_frequency` | `0x081AA20C` | 2,688 u16 | `0x8000 \| int(2048 - 131072 / f)`, with f = C2 × 2^(i/384) and C2 = 440 × 2^(−33/12) ≈ 65.406 Hz: the GB frequency register per 1/32 semitone, C2..B8 |
| `psg_vibrato_steps` | `0x081AB70C` | 84 × 8 u16 | per semitone: value, −1..−3 steps, value, +1..+3 steps. No code reference found |

## Method
- Opcode semantics: read from the matched decoders in `src/sound_driver.c`, then confirmed by the exact round trip of all game data and the fuzz test (`build/assetwf/sound_seq/fuzz_test.py`, `edit_test.py`).
- The lookup-table formulas were re-checked against the ROM on 2026-10-02 with a short script: all 3,072 `pcm_pitch` values and all 2,688 `psg_frequency` values match. With C2 rounded to 65.406 Hz instead of the exact value, three frequency entries differ.
- Use counts: from the extracted files.

## Open questions
- Is `volume_nibble_scale` read by any code? No reference by address was found.
- Does the wave channel sound an octave below the note names? That depends on the wave patterns ([[sound-engine]]).
- What is the audible role of the 2-bit `env` field of the SE tone commands? It goes to bits 0–1 of SOUNDxCNT_L (sound length).

Related: [[sound-engine]], [[sound-driver]], [[sound-api]], [[assets]].
