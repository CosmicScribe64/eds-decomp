---
title: Konami sound driver decompilation status
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Konami sound driver (`0x0807D3D0`–`0x0807EACF`)

> [!note] Status update (2026-10-02)
> Both remaining functions now match: `sub_0807D6B4` (commit `acd0f2c`) and `sub_0807DB58` (commit `01c08ce`). `src/sound_driver.c` has no `INCLUDE_ASM` left, so it is **30/30 in C**. Their match notes are not written up on this page yet. The bytecodes they decode are on [[sound-sequence-format]]. The rest of this page describes the state before those matches.

`src/sound_driver.c` contained **28 of 30 functions in byte-matching C**, with two assembly fallbacks. The complete unit is 0x1700 bytes and passes `tools/dr python3 tools/check.py sound_driver`. The per-function checker reports 30/30 because it includes those two fallbacks; C coverage is 28/30. The compiler is `agbcc -O2 -mthumb-interwork -fhex-asm -fprologue-bugfix`, as configured in `config/cflags.txt` ([[compiler-flags]]).

The source replaces the previously all-assembly driver unit. Proposed descriptive names below are documentation names; the linked symbols retain their original `sub_08XXXXXX` names. The separate ARM mixer remains in `asm/sound_mixer_arm.s`; see [[sound-mixer]]. The game's wrappers are described in [[sound-api]].

## Functions

All entries are Thumb functions. Sizes include each function's literal pool and padding.

| Address / symbol | Size | Status | Verified behavior |
|---|---|---|---|
| `sub_0807D3D0` | 0x148 | matching C | Configures Timer0, FIFO DMA and interrupts; copies the inner mixer to IWRAM and clears PCM voices/buffers. |
| `sub_0807D518` | 0x60 | matching C | Loads four wave-RAM words, toggles driver flag 0x200, and selects the next wave bank. |
| `sub_0807D578` | 0x13C | matching C | Initializes sound hardware and driver state, optionally installs the DMA1 handler, clears tracks, and calls DMA setup. |
| `sub_0807D6B4` | 0x4A4 | matching C (2026-10-02) | Updates an SE track and decodes its bytecode ([[sound-sequence-format#SE track bytecode]]). |
| `sub_0807DB58` | 0x7CC | matching C (2026-10-02) | Advances the sequencer and programs PSG/PCM channels ([[sound-sequence-format#BGM track bytecode]]). |
| `sub_0807E324` | 0x8C | matching C | DMA1 IRQ: advances the read position by 16 and restarts both FIFO DMAs at the 0x2C0-byte wrap. |
| `sub_0807E3B0` | 0x28 | matching C | Calls the sequencer and ARM mixer unless driver flag 0x2000 pauses them. |
| `sub_0807E3D8` | 0x17C | matching C | Starts the pending SE, arbitrating slot masks and priorities. |
| `sub_0807E554` | 0x120 | matching C | Processes the pending SE, handles BGM fade-out, loads a requested song, and initializes its ten tracks. |
| `sub_0807E674` | 0x1C | matching C | Sets a BGM request and clears its requested fade speed. |
| `sub_0807E690` | 0x20 | matching C | Sets a BGM request and supplied fade speed. |
| `sub_0807E6B0` | 0x38 | matching C | Tests whether BGM flag 0x80 is set and the current song equals the supplied ID. |
| `sub_0807E6E8` | 0x3C | matching C | Requests a song if it is not already playing. |
| `sub_0807E724` | 0x40 | matching C | Sets the playing BGM's target volume and fade speed 0x40; returns its ID, or −1 if inactive. |
| `sub_0807E764` | 0x1C | matching C | Sets target volume to zero and the supplied fade speed. |
| `sub_0807E780` | 0x38 | matching C | Sets flag 0x100, target volume zero, and fade speed. |
| `sub_0807E7B8` | 0x30 | matching C | Clears flag 0x100 and fades toward volume 0x10. |
| `sub_0807E7E8` | 0x2C | matching C | Tests whether current and target volume are equal. |
| `sub_0807E814` | 0x5C | matching C | Queues an SE with full volume and variant zero; bit 15 suppresses requests for an already active identical ID. |
| `sub_0807E870` | 0x28 | matching C | Requests an SE, sets variant `& 3`, and immediately invokes pending-SE processing. |
| `sub_0807E898` | 0x44 | matching C | Marks matching active SE tracks for stopping; wakes delayed tracks by clearing flag 1 and setting delay 1. |
| `sub_0807E8DC` | 0x3C | matching C | Applies the same stop operation to every active SE track. |
| `sub_0807E918` | 0x78 | matching C | Starts a PCM voice from `(voice, sampleId, volume, note)`, calculating pitch and loop flags from the sample header. |
| `sub_0807E990` | 0x2C | matching C | Counts the six PCM voices whose active flag is set. |
| `sub_0807E9BC` | 0xC | matching C | Returns the driver status word. |
| `sub_0807E9C8` | 0x84 | matching C | Pauses normal ticks, optionally starts a song, advances a requested number of sequencer ticks, and restores target volume. |
| `sub_0807EA4C` | 0x3C | matching C | Calls the manual-tick routine, then starts a fade-in from volume zero. |
| `sub_0807EA88` | 0x18 | matching C | Sets driver flag 4. |
| `sub_0807EAA0` | 0x18 | matching C | Sets driver flag 1. |
| `sub_0807EAB8` | 0x18 | matching C | Sets driver flags 1 and 4. |

## Layouts and calling conventions

`include/sound.h` provides the shared layouts: `SoundTrack` is 0x18 bytes; `SoundDriver.flags` is at +0x188 and `seVariant` at +0x197; `SoundPcmVoice` is 0x10 bytes. Compile-time assertions check those offsets and sizes. Unknown track fields remain unnamed. The same track layout serves BGM and SE slots: +0x10 is a BGM flag byte and an SE priority byte.

PCM voices are at `0x030053AC`. The +8 word contains a low-halfword step and a high-halfword mixer fraction. PCM start writes the full word. `SoundPcmStart` takes volume in **r2** and note in **r3**, verified against the matched C and its caller. See the correction recorded in [[sound-engine]].

Driver initialization optionally writes `sub_0807E324` through the DMA1 callback pointer. VBlank processing calls `sub_0807DB58` and the linker veneer `__sub_0807EAD0_from_thumb`. Song requests and manual ticking call the matched pending-song loader; variant SE requests call the matched SE allocator.

## Matching details

- MMIO reads and writes retain their original widths and ordering. DMA control-halfword read/modify/write uses `vu16 *` accesses; expressing that operation through a volatile struct field causes agbcc to emit an additional load before the store.
- DMA initialization keeps its initialized voice-array base live through the six-voice reset loop using one empty input constraint after the loop. This emits no instructions or writes and produces the original three hoisted pointer registers. A `do ... while (0)` scope around the first FIFO-DMA setup preserves the final pointer arithmetic and allocation.
- Initialization assigns the reverse SE-loop counter before its track pointer. Swapping those statements changes register allocation and the later PSG-reset sequence.
- The pending-song loader re-reads `pendingBgm` in its current-song comparison, while retaining the first signed request in `song` for table indexing. Comparing the cached `song` produces different register lifetimes. Its ten-track loop reuses that dead local as a countdown.
- The pending-SE allocator uses distinct first and second signed request locals, a cursor bound to `r3`, and a separate shifted slot-mask temporary. Five empty constraints preserve the first request through metadata loading, the variant through cursor setup, and the second signed request before masking, inside allocation, and after final flag writes. They emit no instructions, preserve initialized values, and reproduce the original register assignments and volume-pointer spill. The final overlap loop repeatedly marks slot 5 without advancing its cursor; matched C preserves that behavior.
- PCM start retains pitch-pointer calculation and reuses `note` for the calculated step. Multiplication operand order affects the instruction scheduling.
- Six-track stop/request loops use an explicit countdown after advancing the pointer. The active-PCM count uses the opposite source-loop form, a forward `for` loop, to obtain the original allocation.
- No emitted inline assembly, ABI changes, or uninitialized scaffold variables are used in these 28 C functions.

## Remaining work

The two sequencer functions retain assembly, with complete typed C drafts parked under `#if 0` in source. Both have scoped differential checks. The next bottleneck is matching their cursor/register lifetimes and instruction scheduling. Raw m2c references remain in `build/m2c/sound_driver/`.

Related: [[sound-engine]], [[sound-mixer]], [[sound-api]], [[compiler-flags]], [[decomp-workflow]].

### Saved sound drafts

The DMA initializer is now enabled in exact C after its three hoisted pointer
registers were matched with the empty voice-base input described above.
`sub_0807D6B4` has a preliminary typed SE decoder draft in source,
with explicit 0x18-byte track and 8-byte channel-output views. It compiles after
fixing recovered pointer scaling and narrowing errors. Its channel state at
track +0x0A has both a halfword view and separate flag/volume byte views; commands
0xA0–0xEF read and write the full halfword. The previous byte-only draft lost
volume updates in those commands. This is corrected in the disabled source.
The first corrected candidate was 0x4C8 bytes against the original 0x4A4. A
bounded refinement widened linked-track counters and arithmetic temporaries,
used a single advancing bytecode cursor, and retained the driver base through
a local pointer. The current parked candidate is **0x4AC**, eight bytes larger
than the ROM range, and remains nonmatching. No inline-assembly hints are used
in this decoder draft. The active unit still matches
all `0x1700` ROM bytes with 28 C functions and two assembly fallbacks. The pending-SE allocator is also enabled in exact C using the request and cursor lifetimes described above.

The corrected decoder passed **13,440 finite differential cases** using
`build/decomp_large/sound/verify_decoder.py`, an existing Capstone-based Thumb
interpreter in Docker. Fixtures cover commands 0x20–0xFF, all six SE indices,
ten track-flag profiles, random channel state, volume scaling, positive linked
track clearing, initialization, waits, loops, calls, returns, and global stops.
Each pair compares the complete 0x40000-byte EWRAM image and the sound-state
IWRAM region 0x03005200–0x03005BFF after return. It excludes stack scratch,
caller-save registers, and the unspecified return register of this void
function. Commands 0x00–0x1F retain their command and repeat indefinitely in
the original control flow and are excluded; negative linked-track counts are
excluded from global-stop fixtures because that branch bypasses the normal
negative-count guard. This finite experiment provides behavioral evidence for
the tested inputs, not proof of equivalence or a byte match. The same 13,440-case
experiment was repeated successfully for the current 0x4AC candidate, using
`SOUND_DECODER_CHECK_PATH=build/decomp_large/sound/decoder-manual/wide-stream-driver-none`.

### Paired main-sequencer draft

The complete `sub_0807DB58` draft shares the eight-byte `SoundChannelParams`
view with the SE decoder. BGM tracks use a separate 0x18-byte local view: pitch
at +0, instrument/volume at +2/+3, song offset and position at +4/+6, return
position/offset at +8/+0xA, delay at +0xC, vibrato phase/depth at +0xE/+0xF,
flags at +0x10, fade counter/volume at +0x11/+0x12, loop counter at +0x13, and
routing at +0x14. Source-local assertions check the BGM track size, the saved
song/track alias offsets and the current 0x6C-byte frame object. The compiler
adds a four-byte cursor spill for the original 0x70-byte total stack frame;
no shared-header changes were needed.

Recovered width corrections include whole halfword resets for the paired
instrument/volume and vibrato fields, signed byte note offsets for commands
0xB0–0xBF, and a wide extended-duration temporary. PCM note changes update
only the low halfword of `stepAndFraction`, preserving the mixer fraction.

Command 0xF0 on PCM tracks writes output +0x0B/+0x0C, which is the next
record's volume/dirty bytes. At track 9 those addresses reach the saved song
pointer's high byte and saved last-track pointer's low byte. The draft uses a
real frame struct containing the ten outputs and adjacent saved fields, so
these byte aliases are represented. It does not assume a bounds-safe output
array for that command.

The initial main-sequencer candidate was **0x7FC** against the original 0x7CC,
with stack allocation 0x78 against the ROM's 0x70. Twenty-five
bounded local-base, cursor/counter reuse, width, and register-binding variants
did not match; forcing all cursors/counts to the ROM registers increased code
size. Scratch variants and results are in `build/decomp_large/sound/tick-manual/`.

`build/decomp_large/sound/verify_tick.py` passed **26,860 finite cases** for the
initial 0x7FC candidate: 15,350 ordinary-duration cases across all byte commands,
ten BGM indices and six flag profiles; 10,230 zero/extended-duration cases
across four profiles; and 1,280 fade/mute/stop/reset prefix cases. The test
executes the original SE decoder, wave-RAM loader and PCM-start helpers. It
compares all 0x40000 bytes of EWRAM, sound IWRAM 0x03005200–0x03005BFF,
the MMIO byte image and ordered register writes, and normalized helper
arguments/channel records. It excludes stack scratch, caller-save registers
and the unspecified void return. MMIO is a register-write model, not audio
hardware emulation. The SE helper used by this main-sequencer experiment is
the ROM assembly; the current C SE draft was checked separately.

The 0xFE/reset cases with the 0x40 track flag are excluded because the original
replays reset indefinitely. Track-9 0xF0 fixtures use high nibble 2 to keep the
saved EWRAM song pointer valid; arbitrary corrupted pointers are not covered.
Other operands/table indices use controlled finite records. These results
support the tested behavior and do not replace the required exact-byte check.

### Follow-up: correct sequencer stack size, no new C conversions

The first follow-up main-sequencer draft was **0x7F0** with the original **0x70**
stack allocation. The frame object keeps the outputs and their adjacent alias
fields, but omits the explicitly modeled `resumePosition` spill: the cursor
already lives across the wave helper call, and agbcc supplies that spill.
The original explicit slot duplicated compiler-generated storage. Referring
directly to the driver's BGM tracks also removes a saved base, while wide
delay/loop/priority temporaries avoid premature truncation. The fade-prefix
masked flags local is a word. No register constraints or emitted assembly
were added.

`build/bigguns-sound/tick-final/` contains the enabled private candidate and
complete check/disassembly output. It is still **36 bytes larger** than the
0x7CC target. Its cleaned source produces identical bytes to the candidate
checked by `build/bigguns-sound/verify_tick.py`: **15,350 ordinary-duration
finite cases passed**, recorded in `tick-verification-base.txt`. The earlier
extended-duration and fade-prefix batches were **not rerun for this revision**.
The same table/index, reset-loop, track-9 F0 and MMIO-model limitations above
apply. Only the disabled draft changed in `src/sound_driver.c`.

The decoder's new loop-shape, ordinary pointer-lifetime and temporary-reuse
grids did not match. A three-minute, one-job register-allocation permuter
run ended at score 9452; it supplied no accepted conversion. Its outputs are
unverified candidates, not evidence of equivalence. Reusing every temporary
that once occupied r2 is invalid when the opcode must remain live while an
operand replaces it; split those live ranges before considering such a form.

**Rejected historical pin candidate:**
`build/decomp_large/game-batch/sub_0807D6B4/lifetime-out-entry-stream-index/`
initializes the output alias in r7, then agbcc reuses r7 for the track pointer.
The existing decoder interpreter confirms a failure at flags 0x80, opcode
0x20, SE index 0: both output bytes and track state differ. The counterexample
is saved in `build/bigguns-sound/rejected-pins-verification.txt`. Named-register
declarations plus empty constraints do not by themselves establish safe
pointer lifetimes in this compiler.

The follow-up accepted **zero new functions/bytes**. Both fallbacks remain;
`build/bigguns-sound/active-unit-check.txt` records **30/30 functions and all
0x1700 unit bytes matching**, still 28/30 functions in C.

### Second follow-up: smaller sequencer draft and decoder-loop counterexample

The current parked sequencer draft is **0x7DC**, **16 bytes above** the 0x7CC
target, with the original **0x70** actual stack frame. It introduces an
ordinary `u16 *flags = &p->flags` view for the initial masked-flags update,
fade flags, and mute test. Later flag accesses retain their driver-field
form. The frame object, adjacent F0 alias fields, function ABI, and all
other source operations are unchanged. No constraints or assembly were
added. The complete enabled source is identical to the private candidate in
`build/bigguns-sound2/fade-scope-False-through-test/`.

That candidate passed **15,350 ordinary-duration cases** and **1,280
fade/mute/stop/reset prefix cases** using the existing interpreter and ROM
helpers. Logs are `build/bigguns-sound2/tick-flags-base.txt` and
`tick-flags-prefix.txt`; `verify_tick_flags.py` reproduces the runs. The
10,230 zero/extended-duration cases were not rerun for this revision. The
previously documented fixture exclusions and MMIO-model limits apply.
The smaller size is useful evidence, but register allocation and block
placement still differ substantially; it is not an exact match.

A structured decoder experiment exposed an important transfer distinction:
FA/FB/FC/FF cursor jumps go straight to the next record, even when the current
delay is nonzero. A `while (delay == 0)` loop with ordinary `continue` on
those paths is wrong. The interpreter rejected
`decoder-reducible-while-int-2/` at flags 0x80, opcode FA, SE index 0: the
candidate left delay 5, while ROM consumed the destination record and left
delay 7. The misleading natural r7/r5/r6 output/stream/track allocation does
not make that draft valid. The counterexample is saved as
`build/bigguns-sound2/decoder-reducible-verification.txt`.

The corrected private `decoder-baseline-for-int-1/` uses a real `for (;;)`
loop, checks delay only at command completion, preserves unconditional
cursor jumps, and widens the initially loaded track-flags temporary to a
word. It adds no compiler hints, compiles to **0x4A0** against target 0x4A4,
and passes **13,440 finite decoder cases**, recorded in
`decoder-for-verification.txt`. Its register allocation differs more than
the existing 0x4AC source draft, so it remains a private alternative.

`build/bigguns-sound2/README.md` records exact reproduction commands and the
other bounded experiments. This follow-up accepted **zero new functions or
bytes**. Both assembly fallbacks remain active, and
`build/bigguns-sound2/active-unit-check.txt` confirms **30/30 functions and all
0x1700 bytes matching**, with C coverage still **28/30**.
