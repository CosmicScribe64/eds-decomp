---
title: Emulator harness (tools/emu.py)
type: tool
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Emulator harness (`tools/emu.py`)

`tools/emu.py` runs EDS in mGBA 0.10.5 without a screen and drives it from the command line. It answers
run-time questions: which functions run in a scene, who writes a RAM address, and what arguments a function
receives. It is the dynamic half of the readability pass. The static half is [[xref]]. Added in commit
`c441483`. The author's full notes are in `build/emu/README.md`, which exists only locally because `build/` is
gitignored.

> [!warning] The ROM and all outputs stay local
> The ROM is only read from the repo: `baserom.gba` by default, or `--rom eds` for the `eds.gba` rebuilt by
> `make compare`. It never leaves the local machine. Save data stays in memory, so nothing is written next to
> the ROM. Every output (savestates, RAM dumps, screenshots, traces) is written to `build/emu/`, which git
> ignores. These files contain game data: never commit, upload or paste them.

## Setup
```
docker build -t eds-emu docker/emu/   # once, about 5 min (builds mGBA from source)
tools/emu.py selftest                 # optional, about 20 s, PASS/FAIL per check
```
- On the host, `tools/emu.py` re-runs itself in an `eds-emu` container with the repo mounted at `/work`, as
  `tools/dr` does. With `EDS_NATIVE=1` it skips Docker and uses an mGBA installed under `$MGBA_PREFIX`.
- `docker/emu/Dockerfile`: Debian trixie, mGBA 0.10.5 built from source (libmgba with Lua 5.4, debuggers, GDB
  stub, PNG), `gdb-multiarch` and Pillow.
- `tools/emu/emuh.c` is the libmgba harness. Its job-file directives are documented at the top of the file. It
  is compiled on first use into `build/emu/.bin/emuh-<hash>`, and again when the source or mGBA version changes.
- `tools/emu/states/*.txt` holds the savestate recipes, and `tools/emu/examples/lp_monitor.lua` is a Lua example.

## Basics
- **Names for addresses.** Any address argument can also be a symbol, optionally with an offset
  (`DuelMainStep+0x218`). Symbols come from `config/functions.tsv`, from `build/eds.map` (libgcc and crt0) and
  from the proposed names in `config/names.txt`. `tools/emu.py sym NAME|ADDR` shows how a name resolves.
- **Frames.** Frames count from 0, at boot or at the loaded state. A frame ends at the start of VBlank.
  - Keys given for frame F are held while F runs. The game reads KEYINPUT once per frame, in
    [[frame-sync-update]].
  - `--shot`, `--dump` and `--savestate` at F happen after F frames have run. `--shot 0` shows the loaded
    state, and an event during frame F first shows in `--shot F+1`.
  - Frame lists look like `60,120`, `0-600:50` (every 50 frames) or `end`.
- **Input scripts** (`--input FILE`, or inline as `--keys "LINE; LINE"`). Each line is `<when> <keys>`.
  - `when` can be `120` (a one-frame tap), `130-140` (hold), `200:5` (5 frames from 200), `+20` or `+20:5`
    (relative to the previous line), or `40-900/15` (tap every 15 frames, useful for dialogue).
  - Keys are `A B SELECT START RIGHT LEFT UP DOWN R L`. Join them with `+`, and use `-` for no key.
  - A file may also contain `frames N`, `stopat`, `until` and `settle` lines. Command-line options take
    precedence over them.
- **Stop conditions.**
  - `--stopat FUNC[:COUNT]` stops at the end of the frame in which FUNC ran for the COUNT-th time.
  - `--until ADDR[.b|.h|.w]=VALUE[:COUNT]` (or `!=`) is checked at the end of each frame. A condition that is
    already true at the start does not count.
  - `--settle N` runs N more frames with no keys after either condition fires.
- **Common options:** `--rom base|eds|PATH`, `--state NAME|PATH.ss`, `--sram FILE` (default: blank, never
  written back), `--frames`, `--out NAME` (an existing run directory is replaced unless you pass `--keep`),
  `--shot`, `--sheet` (contact sheet), `--dump FRAMES --regions ewram,iwram,io,pal,vram,oam,sram|all`,
  `--dump-range ADDR:LEN[:NAME]`, `--savestate FRAME[:NAME]`, `--idle ignore|detect|remove`.

## Commands
Each command prints a short summary. The full data is in `build/emu/<run>/` (the `--out` name, or
`<command>-<timestamp>`).

| Command | What it does | Outputs |
|---|---|---|
| `run` | Runs frames; takes screenshots, dumps and savestates | `shots/fNNNNN.png`, `sheet.png`, `dumps/fNNNNN_<region>.bin`, `<name>.ss`, `info.json`, `job.txt` (the exact job, which can be re-run) |
| `trace` | Every function that ran, with call counts | `functions.tsv` (addr, name, proposed, unit, hits, first/last frame, frames active), `units.tsv`, `calls.tsv` with `--calls` (caller, callee, callsite, count), `info.json` (instructions per memory region) |
| `watch` | Every read or write of the given addresses | `watch.tsv` (frame, pc, function+off, access, addr, old, new, lr_function, backtrace), `watch_summary.tsv` (one row per instruction) |
| `break` | Registers each time an address executes | `break.tsv` (frame, function, callsite, pc, lr, sp, r0–r12, cpsr, `--deref` dumps, backtrace) |
| `peek` | Memory right after a state loads, or after `--frames N` | stdout. The run directory is deleted unless `--out` is given |
| `lua` | Runs an mGBA 0.10 Lua script | `stdout.txt` (`[lua]` lines) and whatever the script writes to `OUTDIR` |
| `gdb` | mGBA's GDB stub driven by `gdb-multiarch`, with `build/eds.elf` symbols | `gdb.log` |
| `compare` | Runs the same job on two ROMs, comparing every dump, screenshot and (with `--trace`) the trace. Exits 1 on any difference | one run directory per ROM |
| `states` | Savestate library: `list`, `build`, `show`, `diff` | `build/emu/states/` |
| `tracediff` | Functions that ran in only one of two trace runs | stdout |
| `sym` | Resolves names and addresses | stdout |
| `selftest` | Checks determinism, and that no tool changes emulation | stdout |

One example of each:
```
tools/emu.py run   --state title --frames 120 --shot 0,60,120 --sheet
tools/emu.py trace --state duel_main1 --input tools/emu/states/duel_turn2.txt --calls     # one full duel turn
tools/emu.py watch 0x020192E4:2 --state duel_main1 --input tools/emu/states/duel_turn2.txt --changes
tools/emu.py break SubtractLifePoints --state duel_main1 --input tools/emu/states/duel_turn2.txt --deref r0:4
tools/emu.py peek  --state duel_turn2 0x020192E4:8 --fmt u16 --decimal
tools/emu.py lua   tools/emu/examples/lp_monitor.lua --state duel_main1 --input tools/emu/states/duel_turn2.txt
tools/emu.py gdb   --state duel_start --ex "break *0x0804E420" --ex continue --ex "info registers" --ex bt
tools/emu.py compare --state duel_main1 --input tools/emu/states/duel_turn2.txt --rom-b eds --trace
tools/emu.py states build --all --rom eds --dir build/emu/states-eds
tools/emu.py tracediff verify-trace-title verify-trace-deck-edit
tools/emu.py sym DuelMainStep 0x020192E4
tools/emu.py selftest --rom eds
```

How the commands work:
- **`trace` is exact, not sampled.** The harness steps one instruction at a time, in the same loop structure
  as mGBA's `runFrame`. Before each instruction runs, its address is checked against a bitmap of all 2044
  known function starts. A "hit" means the first instruction ran, so tail calls count too.
  - Call edges come from LR at function entry. Calls through `_call_via_rN` are attributed to the real
    caller, and IRQ handlers show `IntrMain` as their caller.
  - `--trace-from F` runs the frames before F at full speed.
- **`watch` sees every data access.** The CPU's load and store functions are wrapped, so LDM/STM is
  included.
  - DMA writes are reported as `via=dmaN`, with the PC of the store that started the channel.
  - Accesses inside HLE BIOS calls (CpuSet, CpuFastSet) are attributed to the SWI caller.
  - Instruction fetches are not watched. `--changes` keeps only writes that change the value.
- **Backtraces** come from a shadow call stack. After a state load the stack starts empty, so `AgbMain`
  appears only in runs from boot. `lr_function` is meaningful only in leaf functions.
- **`break`** at a function start shows the arguments in r0–r3. For example, `SubtractLifePoints` gets
  r0 = `&players[0]`, r1 = player and r2 = damage.
- **`lua`** provides `emu`, `callbacks` (`frame`, `keysRead`), `console:log` and `util`. Setting `EMU_STOP = 1`
  ends the run after the current frame. To inject keys from Lua, use the `keysRead` callback with
  `emu:addKeys`.
- **`gdb`** ignores input scripts because the target runs freely, so start it from a state. `break NAME` stops
  after the prologue, and `break *ADDR` stops at the exact entry. `-x FILE` and `--interactive` (which needs a
  TTY) are also available.

## Savestate library
A recipe in `tools/emu/states/<name>.txt` is an input script with a `from <parent>` line. `from boot` means
power-on with blank SRAM. `--state NAME` builds the state, and its parents, if it is missing or its recipe has
changed. A build writes `build/emu/states/<name>.ss` (an mGBA PNG state, with SRAM), a `.png` screenshot and
a `.json` file. The JSON records the recipe and ROM SHA-1, the frame count from boot and the current scene
callback. `sheet.png` shows every state. The states carry no timestamp, so a rebuild gives identical bytes.

| State | From | Frame from boot | Screen |
|---|---|---|---|
| `title` | boot | 860 | Title screen, cursor on New Game. Scene `CB_Title` (`0x080057BC`, [[title-screen]]). |
| `main_menu` | title | 1817 | Main menu after New Game: A is tapped through Tea's intro (the deck choice is the first option) until `CB_MainMenu` runs, then 60 settle frames. Cursor on Campaign. Scene `CB_MainMenu` (`0x08003AA4`, [[main-menu]]). |
| `deck_edit` | main_menu | 2017 | Deck Edit showing the 40-card main deck list. Scene `CB_DeckEdit` (`0x0806EF04`). |
| `duel_start` | main_menu | 2392 | First Campaign duel (vs Yugi), on the first frame `DuelMainStep` (`0x08021A48`) runs, before the shuffle and deal. The screen is black (fade-in). |
| `duel_main1` | duel_start | 3163 | Player's turn 1, Main Phase 1 (`until 0x02015EE8=5`, then 120 settle frames). Hand cursor shown, LP 8000/8000. |
| `duel_turn2` | duel_main1 | 5594 | Player's turn 2, Main Phase 1. On turn 1 the player summoned Winged Dragon #1 (1400) and ended the turn, and Yugi's Gazelle (1500) destroyed it. LP 7900/8000. |

Replaying `duel_turn2.txt` from `duel_main1` makes a good "one full turn" workload: a summon, the end of the
turn, the CPU's whole turn with a battle, then the Draw and Standby Phases. To add a state, write a recipe,
run `states build NAME`, and check the printed scene and `<name>.png`. `states diff DIR_A DIR_B` loads each
pair of states and compares all memory and the screen at +0 and +60 frames.

Duel controls found while building the library:
- In the Draw Phase, press A on the deck to draw.
- In Main Phase 1, A on a hand card shows Card View, Set and Summon; move between them with LEFT and RIGHT.
- B in the Main Phase asks "End your turn?" on turn 1, and "End your Main Phase?" on later turns.
- Banners take about 90 frames, and input is ignored while they show.

## Verification (2026-10-02)
The author's runs are kept in `build/emu/verify-*`.
- **State library:** all six states build from boot. Rebuilt with `eds.gba` (`make compare` OK at `350cb09`),
  the `.ss` files are byte-identical to the baserom ones, and `states diff` reports all six identical.
- **`compare`:** 3000 frames from boot with input (25 files, including the trace), and one duel turn (49
  files), are identical between `eds.gba` and the baserom.
- **`selftest`** (baserom and `--rom eds`): all PASS. It checks that plain runs are deterministic, and that
  trace, watch (with backtraces and DMA tracking), break and Lua leave EWRAM, IWRAM, IO, palette, VRAM, OAM,
  SRAM and the screen byte-identical to a plain run. It also checks that the LP write is found with its
  backtrace, and that the GDB stub stops at a breakpoint.
- **Traces:** idle title, 600 frames: 26 functions. Boot to title, 900 frames: 79. Scrolling the deck list,
  300 frames: 98. A full turn cycle, 2431 frames: 323.
- **Speed** (M-series Mac, arm64 Docker): runs about 1400 frames/s, traces about 1000, watch with backtraces
  about 1100. Docker start-up adds about 1.5 s per command. The whole library (5600 frames) takes about 5 s.
- Re-checked by the scribe on 2026-10-02: `sym` and `states list` run, and all six states report `ok`.

## Findings from the verification runs
These are verified by the runs above, except where marked as a hypothesis.
- **LP damage.** `players[0].lifePoints` (`0x020192E4`) goes from 8000 to 7900 at frame 1723 of the turn
  workload.
  - The write is `SubtractLifePoints+0x1E` (`LP -= amount`, clamped at 0), reached through `DuelCmd_ChangeLifePoints <
    DuelCmd_Dispatch < DuelCmdQueue_Run < DuelMainStep < CB_Campaign`. A `break` shows r1 = 0 (player) and
    r2 = 100 (damage).
  - The same addresses also get DMA3 copies of the same value, started by `AiRestoreDuelState` from the CPU AI.
    > [!question] Hypothesis: the AI snapshots and restores the duel state while it simulates moves.
- **Duel phase byte** `0x02015EE8` ([[ram-map]], [[program-flow]]). On the player's turns it goes 1
  (setup/deal), 2, 3 (Draw, which waits for A on the deck), 4 (Standby), then 5 (Main 1). The GDB run stops in
  `0x0804E420`, entry 3 of the phase table, during the Draw Phase.
  - Ending turn 1 goes 5 → 6 → 7 → 8 within about 150 frames.
  - The byte stays 8 for the CPU's whole turn, then returns to 2.
  - > [!question] The meanings of phases 6, 7 and 8 are hypotheses. 8 may be "opponent's turn".
- **Scene timeline from boot:** the license sequence runs during frames 3–567, then installs Title directly
  ([[license-sequence]]). The title accepts input by about frame 850.
- **Interrupts** ([[interrupt-handlers]]). Each frame has 1 VBlank, about 21 sound DMA1 IRQs (`SoundDma1Intr`)
  and about 0.09 Timer2 IRQs. HBlank handlers appear only during effects: `Title_HBlank` while the title
  appears, and `BattleScene_HBlank` for 19 frames of the duel turn.

## Limitations
- mGBA's HLE BIOS is used (no BIOS file). There is no boot logo, and SWI timing differs from hardware. Game
  logic is unaffected.
- There is no audio output and no link cable, so Link Battle and Card Trading wait for a partner.
- Code copied to RAM maps back to ROM only for the known copies, `IntrMain` (`0x0300004C` ← `0x080000FC`) and
  the mixer loop (`0x03005A54` ← `0x0807EC1C`) ([[crt0]], [[sound-mixer]]). These are listed in `RAM_ALIASES`.
  Other RAM code, such as the AgbSram `*_Core` routines copied to the stack ([[agb-sram]]), shows up only in the
  per-region instruction counts.
- Names are read at run time. After a rename pass, re-run the job instead of reusing old TSVs.
- `--idle detect` is about twice as fast for plain runs, but it changes how often the VBlank wait loop reads
  `gMain.intrCheck`. The default, `ignore`, is exact.
- Recipes depend on frame timing. That timing cannot change while the ROM matches. If a recipe ever stops
  reaching its screen (check `states/sheet.png`), fix its inputs.
- Every command uses its own container and run directory, so concurrent use is safe, and library states are
  installed atomically. Reusing an `--out` name replaces that directory.

Related: [[xref]], [[agent-tooling]], [[decomp-workflow]], [[toolchain]].
