---
title: Asset extraction and the data build
type: tool
status: verified
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Asset extraction and the data build

The repository holds no game data. `make setup` (`tools/setup.py`) finds the user's ROM in `roms/`, checks its SHA-1 and extracts every non-code byte into `assets/`, which is gitignored. From then on the build reads `assets/` and never the ROM.

**State (2026-10-02, commit `2cf1b05`).** `config/assets.tsv` has 83 assets. Every one extracts to an editable format (JSON, CSV, text, PNG, WAV or JASC-PAL) and builds back byte-identical. `assets/fallback.txt` is empty, and no manifest row uses the raw `bin` type. The only raw file is `header.logo.bin`, the 156-byte Nintendo logo, which the BIOS checks. A fresh extraction writes 1,751 files.

## Pieces
- **Manifest.** `config/assets.tsv` has one row per asset: start and end address (end exclusive), type, output path and `key=value` options. Every byte of both data units (`0x08080A20`–`0x08087FB4` and `0x08087FD0`–`0x08800000`) belongs to exactly one asset. The cartridge header (`0x08000004`–`0x080000C0`) is one more. `tools/assets.py check` verifies the coverage.
- **`extract`.** Converts each range. Before writing a converted file it builds the bytes back from the in-memory files and compares them with the ROM. If they differ, or the converter raises `ValueError`, `KeyError` or `AssertionError`, the asset is written as a raw `.bin` and listed in `assets/fallback.txt`. A build from extracted assets therefore always matches.
- **`build`.** Writes `build/assets/<path with / replaced by __>.bin`. The Makefile runs it when `config/assets.tsv`, `config/functions.tsv`, `tools/assets.py`, any `tools/assetfmt/*.py` or any file in `assets/` changes.
- **`gen-data`.** Regenerates `data/rodata_*.s` from the manifest. They `.incbin` the built files, split at every `gUnk_08XXXXXX` label the code refers to. Run it after changing a row's range, type or path, then run `make setup` again. `asm/crt0.s` takes the header from `build/assets/header.bin`.
- **`verify`.** Builds every asset from `assets/` and compares it with the ROM.

## Plugins
The 11 core types live in `tools/assets.py`. Every other format family is a module in `tools/assetfmt/<family>.py`, loaded at startup in file-name order. Files that start with `_` are skipped.

```python
def register(A):              # A is the tools/assets.py module (png_read/png_write, enc/dec, bgr555 helpers, MANIFEST, ...)
    return {'family_type': (extract, build)}

def extract(data, p, rom):    # data = the asset's ROM bytes, p = the manifest row as a dict, rom = the whole image
    return {'rel/path.json': b'...', ...}        # files to write under assets/
def build(read, p):           # read(rel) returns the bytes of one asset file
    return b'...'                                 # must be exactly end - start bytes
```
A type name may be defined only once. Two plugins defining the same name stop the tool.

**Developing a converter privately.** Three environment variables redirect the tool, so a converter can be written and tested without touching the shared manifest or `assets/`:

| Variable | Default | Overrides |
|---|---|---|
| `EDS_ASSET_MANIFEST` | `config/assets.tsv` | the manifest |
| `EDS_ASSETS_DIR` | `assets` | where `extract` writes and `build`/`verify` read |
| `EDS_ASSETS_OUT` | `build/assets` | where `build` writes |

`extract --only PREFIX` and `verify --only PREFIX` process only the assets whose path starts with `PREFIX` (for example `--only sound/`). The six plugins below were each written this way, with a private manifest under `build/assetwf/<family>/`, and then merged.

## Formats
Each family's own docstring and its file headers (`"_format"`, `doc`, `;` comments) describe the format in full. Keys that start with `_` are notes, and the build ignores them.

### Core types (`tools/assets.py`)
| Type | Assets | Files | How to edit | Constraints |
|---|---|---|---|---|
| `strings` | card names (821 × 0x40), descriptions (821 × 0x1E0) | JSON list | edit text | must fit the record; a record with a non-zero tail is `{"text", "raw_tail"}` |
| `dialogue`, `duelists` | [[text-system]], [[duelist-table]] | JSON list | edit text | fixed record size and count |
| `card_stats` | [[card-table]] | CSV, raw bitfields `def atk kind type level attr` | edit cells | the name column is a note |
| `card_art` | 821 card images ([[card-art]]) | `NNN.png`, 72×80, 64-colour indexed | edit pixels and palette | sidecar `palette_bit15.json` |
| `font1bpp` | 8 fonts ([[font]]) | PNG sheet; kanji sheets have one Shift-JIS lead byte (192 glyphs) per row | edit pixels | fixed glyph grid |
| `palette`, `tiles4bpp` | system palette and tiles | JASC-PAL, PNG sheet | edit | sidecar `system.pal.bit15.json` |
| `u16` | card ID maps ([[card-id-map]]) | JSON list | edit values | fixed count |
| `zero` | 3 padding ranges | none | none | checked to be zero |
| `bin` | (none now) | `.bin` | hex editor | the per-asset fallback |

### Sound samples (`sound_samples.py`)
| Asset | Files | How to edit | Constraints |
|---|---|---|---|
| `sound/pcm_bank0_samples`, `sound/pcm_bank1_samples` (`sound_samples_pcm`) | `NN.wav` (mono 8-bit, named after the first table index pointing to the sample) + `index.json` (`address`, `rate`, `loop_start`, `sample_ids`) | replace or edit a WAV; mono 8- or 16-bit PCM is accepted (16-bit is rounded) | each sample keeps its `address` (16-aligned), and a longer sample must end before the next one. Spare space: 1 byte in bank 1, 14 in bank 0. A WAV rate that disagrees with `rate` stops the build; `"rate": null` takes it from the WAV |
| `sound/pcm_bank0_table.json`, `sound/pcm_bank1_table.json` (`sound_samples_table`) | JSON, one `ptr` per entry | repoint `ptr` to another sample's `address` | fixed entry count (36, 28) |
| `sound/wave_ram_patterns.json` (`sound_samples_waveram`) | 160 records `{wave, level, samples}`, 32 hex nibbles each | change digits | every (wave, level) slot exactly once |
| `sound/noise_table.json` (`sound_samples_noise`) | 6 `SOUND4CNT_H` values split into fields | edit fields | 6 entries |

WAV byte = ROM signed byte XOR 0x80. The WAV sample rate is `rate * 4096 / 839` Hz, the playback rate at pitch index 0, so every game sample is 9,998 Hz. Formats and data: [[sound-engine]].

### Sound sequences (`sound_seq.py`)
| Asset | Files | How to edit | Constraints |
|---|---|---|---|
| `sound/se_tracks` (`sound_seq_se_tracks`) | `index.txt` + 49 `.txt` (`se<ID>_<channel>.txt`, one called subroutine) | one event per line, `<ticks> <command> [args]`; labels resolve across files | `.org ADDRESS` fixes each track; a track may shrink (zero-filled) but not grow past the next one |
| `sound/se_table.json` (`sound_seq_se_table`) | 48 effects: `priority`, `slot_mask`, `lock_ticks`, `tracks` | edit values, repoint tracks | exactly 48 entries |
| `sound/song_tracks` (`sound_seq_song_tracks`) | `index.txt` + `song_NN.txt`, ten tracks per song | edit events; notes by name on PSG tracks | `.song ADDRESS` / `.track OFFSET` fixed; same growth rule |
| `sound/song_table.json` (`sound_seq_song_table`) | 58 songs: `data` + ten track offsets | repoint | 58 entries |
| `sound/lookup_tables.json` (`sound_seq_lookup`) | the driver's four lookup tables, rows labelled by volume, semitone offset or note name | edit values | fixed sizes |

The command set is on [[sound-sequence-format]]. Moving a track means editing both its `.org`/`.track` line and the table entry that points to it.

### Scene graphics (`gfx_scenes.py`)
| Asset | Files | How to edit | Constraints |
|---|---|---|---|
| `gfx/scenes/setNN` (31 rows, `gfx_scenes_set set=N`) | `bitmap.png` (PNG palette = the BG palette), `sprites.png` (sets 5–30, 128×128, PNG palette = the OBJ palette), `scene.json` | edit pixels and colours in an image editor | each LZSS blob must fit its slot; image size fixed; a sprite tile may use only one 16-colour bank; sidecars `trailing_bytes`, `*_bit15` |
| `gfx/dialogue_box` (`gfx_scenes_box`) | `box.png`, `header.png`, `box.pal`, `text.pal`, `dialogue_box.json` | edit pixels; recolour through `box.pal` (the PNG palettes are previews) | same slot rule |
| `gfx/scene_sets.json` (`gfx_scenes_table`) | 31 descriptors, each value `"0x08XXXXXX <note>"` | repoint | 31 entries; must agree with each set's `layout` |
| `tables/dialogue_box_pointers.json`, `tables/dialogue_box_steps.json` (`gfx_scenes_ptrs`) | u32 pointer lists; `kind=func` writes function names with the Thumb bit | repoint | fixed count |

The LZSS streams are re-encoded with a reconstruction of the original compressor, which reproduces all 59 streams byte for byte ([[graphics-formats#The original compressor]]). If an edited image no longer fits, the build retries with an optimal parse (about 1.6% smaller), then stops with both sizes, for example `compresses to 20894 bytes but its slot holds 16616`. Formats: [[scene-sets]].

### Graphics banks (`gfx_banks.py`)
One row per bank (`gfx/card_frames`, `gfx/bank_a`, `gfx/small_graphics`, `gfx/mode4_bitmaps`, `gfx/bank_b`; 562 items). A `layout=` table in the plugin cuts each bank into items. The build reads only `<bank>/index.json`, which lists every item with its address, slot size, kind, file, the code labels inside it and a note.

| Kind | File | How to edit | Constraints |
|---|---|---|---|
| `pal` | `<addr>.pal` (JASC-PAL), or the PNG palette of the bitmap it belongs to | edit RGB lines (rounded to 5 bits) | colour count = slot / 2; `bit15` list |
| `bitmap` | `<addr>.png`, 240×160 8-bit indexed | edit pixels and the PNG palette | size fixed |
| `tiles` | `<addr>.png` sheet, 4 or 8 bpp, arranged in `frame`s | edit pixels (indices 0–15 for 4bpp) | the PNG palette is a preview only; colours come from the named `pal` item |
| `map` | `<addr>.json`, rows of 4-digit hex entries (`tile \| hflip<<10 \| vflip<<11 \| palette<<12`) | edit entries | entry count fixed |
| `pack` | `<addr>.png`, the picture the image pack draws | edit freely | re-encoded from the picture; may shrink, not grow |
| `sprite` | `<addr>.png` (one cell per graphic) + the frame script in `index.json` | edit pixels, palette, frames | must fit its slot |

Every item is built to exactly its slot size: shorter is zero-padded, longer stops the build with `builds to 0x.. bytes, its ROM slot holds 0x..`. An item that does not round-trip falls back to `.bin` on its own, listed in the bank index's `fallback` (all are empty now). Formats and bank contents: [[graphics-formats]], [[rom-map]].

### Game tables (`tables_game.py`)
| Asset | Files | How to edit | Constraints |
|---|---|---|---|
| `tables/deck_lists.json` | 25 opponent decks + 6 alternate decks, `[number, "name"]` lists | add, remove or change cards | decks are re-packed; the total must fit 0x920 bytes (1,168 cards, all used now); entry counts fixed |
| `cards/passwords.csv` | `id,password,number,name` | edit the 8-digit password, or empty for none | keep leading zeros (spreadsheets strip them) |
| `tables/sine.json` | 256 × s16 | edit values | fixed size |
| `text/dialogue_end.json` | the dialogue terminator record | as `dialogue` | fixed size |
| `tables/pointer_tables/*.json`, `tables/rodata2/*.json` (`tables_game_layout`) | one JSON per named sub-table (62 files) | edit values, pointers, OAM entries, pack slots | each sub-table keeps its size; booster-pack slot lists may change size within their region; OAM frames cannot be added or removed |

Card numbers are authoritative everywhere: the build reads the number and ignores the name. Pointers are `"0x08XXXXXX"` strings with a note. Sub-table list: [[rom-map]]; content pages: [[deck-lists]], [[booster-packs]], [[password-table]], [[special-card-lists]].

### Code tables and header (`tables_code.py`)
| Asset | Files | How to edit | Constraints |
|---|---|---|---|
| `header` (`tables_code_header`) | `header.json` + `header.logo.bin` | edit title, codes, version | `checksum: "auto"` is recomputed at build time; the logo stays raw ([[rom-header]]) |
| `rodata/strings_and_tables`, `tables/scene_scripts_and_lists` (`tables_code_layout`) | `strings.json`, `tables.json`, `anims.json`: lists of items placed at `addr` | edit strings, values, pointers; add items in zero gaps | an item may grow only into zero bytes before the next item; the build names the overlap |
| `tables/card_effect_handlers.json` (`tables_code_effects`) | 426 rows `{number, card, resolve, check, prepare, chain_a, chain_b}` | change function names | rows sorted, unique, exactly 426 (the search bound is hard-coded) |

Function pointers are written as names. They resolve through `config/functions.tsv`, and `sub_XXXXXXXX` works without it. The type language of `tables.json` (`u16`, `ptr`, `{a:T}`, `bitsN(...)`, `T[N]`, `AnimStep`, `OamTemplate`) is in the plugin docstring.

## Text and palettes
Text uses Windows-1252, with Latin-1 for the five undefined bytes, so every byte value maps to one character and back. Shift-JIS appears only in the three debug `printf` strings and the `sjis16` table entries.

**Palette bit 15.** The hardware ignores bit 15 of a BGR555 colour, but many palettes set it on some entries, with no visible pattern: 820 of the 821 card palettes, the system palette, about half the scene BG palettes, and never an OBJ palette. PNG and JASC palettes can't hold it, so each converter keeps a list of the affected indices (`*_bit15` keys or `.bit15.json` sidecars).

## Verification (2026-10-02)
- `check`: manifest OK, 83 assets. A full `extract` leaves `fallback.txt` empty, and `verify` reports 83/83 (rerun for this page: about 8 s).
- A fresh clone with only the ROM in `roms/` ran `make setup` and then `make compare` (`eds.gba: OK`). With the ROM deleted afterwards, a clean build still matched.
- Each plugin was verified with a private manifest first (54/54 assets at the time) and with edit tests, which changed a file and checked that exactly the expected bytes changed, then reverted. Examples:
  - flipping one WAV byte changed ROM `0x08120610`;
  - `freq 0x744` → `0x745` in `se00_sq2.txt` changed `0x080885A0`;
  - one bitmap pixel of set 5 changed only that pixel, and the re-encoded stream still fit;
  - one map entry at `0x086E21D0`;
  - a deck change and a password change;
  - the header title, with the checksum recomputed;
  - a debug string grown into its padding.
- Fuzzing: `sound_seq` round-trips 300 synthetic SE and 300 BGM blobs with every opcode, odd tick encodings and junk after terminators.
- Per-plugin notes, edit-test scripts and logs: `build/assetwf/<family>/NOTES.md`.

## Limitations
- **Fixed ROM layout.** Code and data refer to each other by absolute address, so every edited item has to fit its original slot. This applies to samples, tracks, LZSS blobs, packs, strings and tables. Only the deck lists and the booster-pack slot lists are re-packed by the build. An item that grows too large stops the build with the sizes. Growing a sample or a track means moving its neighbours by hand: the item's `address`/`.org` and every pointer to it.
- **Cross-asset pointers are raw values.** A table and the data it points to are separate assets: the SE table and its tracks, the scene descriptors and their blobs, animation steps and their sprite templates. A converter cannot read another asset's files during extraction, so the pointers stay `"0x08XXXXXX"` with a note.
- **Tool gaps (reported, not fixed).**
  - `extract --only` rewrites `fallback.txt` with only the subset's entries.
  - `extract` does not delete files left from an earlier extraction.
  - `out_path` keeps the file extension (`tables__deck_lists.json.bin`), so a row that gains a converter changes the generated `data/*.s` names.
  - Plugins that annotate pointers parse `A.MANIFEST` themselves, because `manifest()` applies `--only`.
  - The LZSS re-encode takes about 4 s per build of the scene assets; nothing caches the compressed streams.

## Future work
- **Relocatable data.** Pointers would become symbolic (for example `"gfx/scenes/set05:sprites"`, track labels), and a layout pass would place each group's items and rewrite the tables that point to them. The scene descriptor table, each set's `layout`, and the SE and song tables are where it would plug in. Merging each sound table with its tracks into one asset would also let tracks grow and move. Shifting data across asset boundaries needs the code's literal pools to come from symbols, as they already do through the `gUnk_` labels.
- A `smpl` chunk with the loop points in the WAVs, for samplers (not written now, so `index.json` stays the only source).
- `tools/extract_cards.py decks/packs/lists` duplicates what the extracted JSON now shows, and could read it.

Related: [[rom-map]], [[rom-free-workflow]], [[decomp-workflow]], [[sound-sequence-format]], [[scene-sets]].
