---
title: ROM Map (data area)
type: rom
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# ROM Map

This page maps the whole 8 MiB image, with the data area after `.text` (`0x08080A20`–`0x08800000`) in detail. Each row gives the half-open range `[start, end)`, what the segment holds, and whether the claim is **V** (verified against the ROM with a script, or by decoding and looking at the result) or **H** (hypothesis). Format details live on the linked pages.

To re-check the main **V** boundaries, run `python3 tools/verify_rom_map.py`. It checks 29 facts and should print `0 failure(s)`.

## Big picture

| Range | Size | What |
|---|---|---|
| `0x08000000`–`0x080000C0` | 0xC0 | Cartridge header. See [[rom-header]]. |
| `0x080000C0`–`0x08080A20` | 0x80960 | `.text`: game code, then the SDK/libgcc tail. See [[nintendo-sdk-libraries]]. |
| `0x08080A20`–`0x08087FD0` | 0x75B0 | `.rodata` 1: C string literals and small tables, then AgbSram rodata. |
| `0x08087FD0`–`0x08139F5C` | 0xB1F8C | **Sound data** (Konami driver): SE table, two PCM banks, 58 songs. See [[sound-engine]]. |
| `0x08139F5C`–`0x081ABE4C` | 0x71EF0 | Game tables: duelists, **dialogue** (490 × 0x304), scene sets, decks, `.rodata` 2. |
| `0x081ABE4C`–`0x081C0000` | 0x141B4 | Zero padding. The next bank is placed at a round address. |
| `0x081C0000`–`0x0822C720` | 0x6C720 | **Fonts**: 3 Shift-JIS kanji fonts and 5 CP1252 fonts. See [[font]]. |
| `0x0822C720`–`0x08625460` | 0x3F8D40 | **Per-card block**: names, descriptions, art, palettes, stats, ID maps. All indexed by card ID 0..820. See [[card-table]]. |
| `0x08625460`–`0x087F8568` | 0x1D3108 | **Graphics**: image packs, Mode-4 bitmaps, LZSS scene sets. See [[graphics-formats]]. |
| `0x087F8568`–`0x08800000` | 0x7A98 | Zero padding (end of ROM). |

About 45% of the ROM (3.5 MiB) is card art. There is **no BIOS compression** anywhere (see [[bios-swi-stubs]]). The only LZ decoder is [[lzss-decompress]], and it is used only for the dialogue-scene bitmaps (about 0.7 MiB). Card art uses its own 6bpp packing ([[card-art]]). Everything else is stored raw.

## Detailed map

### `.rodata` 1 and SDK rodata
| Start | End | Contents | V/H |
|---|---|---|---|
| `0x08080A20` | `0x08087FB4` | Game `.rodata`: debug `printf` strings (`"Change BG:%d\n"`, `"DM5:Script=..."`); about 280 duel prompts with `@n` colour codes (`0x0808275C`–`0x08086391`); the duelist-name pool (`0x08081260`); the notable-card list `0x08081A6C` ([[special-card-lists]]); the booster-pack info table `0x080865DC` ([[booster-packs]]); calendar/event names (`0x08087724`); animation-frame tables (`0x08080B08`…). Code refers to each string directly from a literal pool. See [[text-system]] | V (named items), H (rest) |
| `0x08087FB4` | `0x08087FD0` | AgbSram rodata: `"SRAM_V112"` plus 4 dead `.LC` words. See [[agb-sram]] | V |

### Sound data (`0x08087FD0`–`0x08139F5C`)
The driver and the formats are on [[sound-engine]]. This table only pins the extents.

| Start | End | Contents | V/H |
|---|---|---|---|
| `0x08087FD0` | `0x08088510` | SE table: 48 × 0x1C. Only the first 3 of the 6 track pointers are ever non-NULL, and all 49 non-NULL pointers land in the next row | V (extent) |
| `0x08088510` | `0x08088A20` | SE track bytecode | V (extent) |
| `0x08088A20` | `0x08088A88` | PCM table (the id bit 15 = 1 bank), 26 pointers | V |
| `0x08088A90` | `0x080E09D0` | 26 PCM samples. Each is `{u32 rate = 0x800; u32 len; s32 loop (−1 = none)}` + s8 data, padded to 16 | V |
| `0x080E09D0` | `0x080E0F40` | Song table: 58 × 0x18 `{u32 songData; u16 trackOffset[10]}` | V |
| `0x080E0F40` | `0x0811B420` | Song track data (58 songs, byte-packed; the last ends at `0x0811B415`) | V (extent) |
| `0x0811B420` | `0x0811B4B0` | PCM table (the id bit 15 = 0 bank), 36 pointers | V |
| `0x0811B4B0` | `0x08139542` | 36 PCM samples (3 of them loop) | V |
| `0x08139550` | `0x08139F50` | PSG channel-3 wave-RAM patterns, 160 × 16 bytes | V (per [[sound-engine]]) |
| `0x08139F50` | `0x08139F5C` | Small struct read by the sound code at `0x0807E260` | H |

### Game tables (`0x08139F5C`–`0x081ABE4C`)
| Start | End | Contents | V/H |
|---|---|---|---|
| `0x08139F5C` | `0x08139F64` | 2 pointers to the LZSS dialogue-box bitmap `0x0874C650` | V |
| `0x08139F64` | `0x0813ADD4` | Duelist/character table: 28 × 0x84 `{u32 id; char name[0x40]; char shortName[0x40]}`. See [[duelist-table]] | V |
| `0x0813ADD4` | `0x0813ADF4` | 8-word function-pointer (step) table of the dialogue text-box runner (`0x08001365`, `0x080016A9`, …; see [[debug-menu]] "Bustup") | V (pointers), H (use) |
| `0x0813ADF4` | `0x0819739C` | **Dialogue table**: 490 × 0x304 `{u16 eventId; u16 speakerId; char text[0x300]}`. See [[text-system]] | V |
| `0x0819739C` | `0x081976A0` | Terminator `0xFFFFFFFF`, then zeros | V |
| `0x081976A0` | `0x0819790C` | **Scene-set descriptors**: 31 × 0x14 `{lz bitmap; bgPal; objPal; lz objTiles; anim}`. See [[graphics-formats]] | V |
| `0x0819790C` | `0x0819A9D4` | Scene animation scripts (the descriptors' `anim` targets, e.g. `0x08197954`); event/tournament names (`0x081980D8`); starter-deck pools `0x08198634`–`0x0819879C` ([[deck-lists]]); the Mode-4 bitmap table `0x08198440` (5 × `{pal, bitmap}`); image-pack pointer lists (`0x081985A0`, `0x08198604`) | V (named items), H (rest) |
| `0x0819A9D4` | `0x0819D1C4` | Handler table: 426 × 0x18 `{u32 key; fn[5]}`, with keys ascending 15..1552. 290 keys are card numbers, which makes this the card-effect dispatch table. See [[cards]] | V |
| `0x0819D1C4` | `0x0819D34C` | Pointer/jump tables (e.g. `0x0819D1C4` points to duel prompt strings) and the AI card lists `0x0819D2FC`/`0x0819D316` ([[special-card-lists]]) | V (lists), H (rest) |
| `0x0819D34C` | `0x0819DD64` | Deck lists (card-number arrays + header tables). See [[deck-lists]] | V |
| `0x0819DD64` | `0x081A7A0C` | `.rodata` 2: three 0x2800 blocks at `0x0819DD94`/`0x081A0594`/`0x081A2D94` (mostly zero, probably tilemaps); UI tables `0x081A4xxx`–`0x081A73xx` (including rarity thresholds `0x081A570C`, [[booster-packs]]); the **debug-menu item table** at `0x081A73A0` (`{char name[0x40]; callback}` × 10: `"Get all card"`, `"Get a pack"`, …; see [[debug-menu]]); the **ASCII-to-Shift-JIS table** at `0x081A76A0` | V (strings, SJIS table), H (rest) |
| `0x081A7A0C` | `0x081ABC4C` | Sound lookup tables: SE variant table (`0x081A79F9`, just before this row), envelope/volume tables, the pitch table (`0x081A8D48`–`0x081AA20C`, used from its middle at `0x081A960C`), and a PSG frequency table (`0x081AA20C`–`0x081ABC4C`, u16 with bit 15 as the trigger bit) | V (pitch table per [[sound-engine]]), H (boundaries) |
| `0x081ABC4C` | `0x081ABE4C` | Sine table: 256 × s16 in 4.12 fixed point (`[64] = 0x1000`) | V |

### Font bank (`0x081C0000`–`0x0822C720`)
See [[font]].

| Start | End | Contents | V/H |
|---|---|---|---|
| `0x081C0000` | `0x081D0200` | Shift-JIS kanji font 8×8: 8256 glyphs × 8 bytes | V |
| `0x081D0200` | `0x081F8700` | Shift-JIS kanji font 10×10: 8256 × 20 bytes (16-bit big-endian rows) | V |
| `0x081F8700` | `0x08228D00` | Shift-JIS kanji font 12×12: 8256 × 24 bytes | V |
| `0x08228D00` | `0x0822C300` | 5 CP1252 fonts, 256 glyphs each, 8 px wide: 8×8, 8×10, 8×12, 8×16, and 8×8 bold | V |
| `0x0822C300` | `0x0822C320` | 16-colour "system" palette (index 0 = `0xFE3F` key colour; 1–15 = blue, red, magenta, green, cyan, yellow, white, black, then the dark variants and grey) | V (values), H (used for text colours) |
| `0x0822C320` | `0x0822C720` | 32 4bpp "system" tiles (tiles 0–1 blank): level star, cursor, gem icons. `LoadSystemFontGfx` (`0x08075630`, see [[video-helpers]]) loads the first 16 with the palette above. `0x0822C360` is also referenced on its own (`0x08061F54`) | V |

### Per-card block (`0x0822C720`–`0x08625460`)
Every table is indexed by the **card ID (1..820, alphabetical order)**, with a blank slot 0. The same slot number gives the name, the art and the description (art 1 = 7 Colored Fish, 2 = 7 Completed, 100 = Castle of Dark Illusions, all decoded). The owner of these pages is [[card-table]], and the rows below follow its block map.

| Start | End | Contents | V/H |
|---|---|---|---|
| `0x0822C720` | `0x08239460` | Names, 821 × 0x40. See [[card-name-table]] | V |
| `0x08239460` | `0x082461A0` | **All zero**, exactly 821 × 0x40: a second name bank that is unused here | V |
| `0x082461A0` | `0x082A6500` | Descriptions, 821 × 0x1E0, plain ASCII. See [[card-descriptions]] | V |
| `0x082A6500` | `0x08608360` | **Card art**, 821 × 0x10E0: 72×80 px, 6 bits per pixel. See [[card-art]] | V |
| `0x08608360` | `0x08621DE0` | Card art palettes, 821 × 0x80 (64 colours each). See [[card-art]] | V |
| `0x08621DE0` | `0x08622AB4` | Stats, 821 × u32. See [[card-table]] | V |
| `0x08622AB4` | `0x08623120` | ID to card number (u16, plus pad). See [[card-id-map]] | V |
| `0x08623120` | `0x08623DF4` | Passwords (BCD). See [[password-table]] | V |
| `0x08623DF4` | `0x08624DF4` | Card number to ID, u16[2048]. Used by the dialogue code `$iNNNN` ([[text-system]]). See [[card-id-map]] | V |
| `0x08624DF4` | `0x08625460` | u16 per card (probably a Japanese sort key). See [[card-id-map]] | H |

> [!warning] Correction (2026-09-29): resolves the contradiction raised on [[card-id-map]]
> An earlier draft of this page listed an "LZSS blob at `0x08624CF4`" and "u16 card lists at `0x0862502C`". Both were wrong. `0x08624CF4` is `&numberToId[1920]` (the literal is at `0x08014700` and is read with `ldrh` at `0x08014686`). Its value `0x00000334` only happened to pass the LZSS size test. `0x0862502C` is inside the sort-key table. There is no LZSS data in the per-card block.

### Graphics (`0x08625460`–`0x087F8568`)
See [[graphics-formats]]. Almost everything here is uncompressed. The only exception is the LZSS scene rows.

| Start | End | Contents | V/H |
|---|---|---|---|
| `0x08625460` | `0x08636288` | 7 **card-frame** image packs (32 colours, 144 8bpp tiles, 144 cells, 0x2698 each), chosen by card type in the code at `0x08006BB4`–`0x08006CD6` | V (decoded) |
| `0x08636288` | `0x0870C640` | Uncompressed bank A: 16-colour palettes; about 70 small 4bpp icon packs (`0x08636828`–`0x0863840C`, `0x0868B670`–`0x0868C9F8`); 4bpp and 8bpp BG packs (`0x0863862C`, `0x0863D12C`, …); **booster-pack cover art** `0x0864073C`–`0x08677930` (mostly 0x1880 = 98 8bpp tiles each, see [[booster-packs]]); 0x800 and 0x2000 tile/map blocks; Mode-4 bitmaps `0x086A12EC` and `0x086B8568` | V (named items), H (rest) |
| `0x0870C640` | `0x0871B650` | LZSS scene sets 0–4: four 240×80 bitmaps and one 240×96 bitmap, each followed by a raw 256-colour palette | V |
| `0x0871B650` | `0x0871CE50` | Small raw graphics: palettes, 4bpp digit tiles | H |
| `0x0871CE50` | `0x0874C650` | 5 **Mode-4 bitmaps**, 240×160 8bpp (0x9600) plus a 256-colour palette each (table `0x08198440`). One is a character-select screen | V (decoded) |
| `0x0874C650` | `0x0874E324` | LZSS dialogue-box bitmap 240×64 (`0x0874C650`), LZSS 240×24 strip (`0x0874D5B0`), palettes `0x0874E104` and `0x0874E304` | V |
| `0x0874E324` | `0x087BDAA8` | LZSS scene sets 5–30: 26 × `{240×96 bitmap (LZSS 0x5A00), BG palette, OBJ palette, OBJ tiles (LZSS 0x2000)}`. These are the duelist "bust-up" dialogue scenes | V |
| `0x087BDAA8` | `0x087F8568` | Uncompressed bank B: image packs (`0x087BDAA8` with 128 colours; `0x087C056C`; `0x087C0CD4`, a coin); Mode-4 bitmap `0x087C29D4`; full-screen 8bpp BG packs (`0x087D01F4`, `0x087D292C`, `0x087D4B24` sky); 24 small 4bpp packs `0x087E795C`–`0x087EA716` (list at `0x081985A0`); title/menu assets `0x087EA718`–`0x087F7E18` (loaded from `0x080026B8`…) | V (named items), H (rest) |

## Method
- **Block statistics:** `python3 tools/romstats.py <start> <end> <blocksize>` prints the zero, 0xFF and ASCII fractions, entropy, and pointer density per block. Card art shows up as entropy 7.6. Dialogue and name slots show up as 80% zeros.
- **Anchors from code:** every aligned u32 in `.text` (`0x08000228`–`0x08080A20`) with a value in `[0x08080A20, 0x08800000)` gives about 1050 literal-pool targets. Pointer tables inside the data (scene descriptors, pack lists, sample tables) add the rest. Most segment boundaries above are one of these targets.
- **Decoding:** a boundary is only marked V if the table parses to its exact end: the sample headers chain, the dialogue records have zero padding, the LZSS blobs decode to the expected size, and the image packs parse. Images were rendered with `tools/render_gfx.py` (Pillow in a throwaway container, output kept outside the repo) and looked at.
- **Caveat:** an LZSS size test on its own gives false positives (see the correction above). A blob only counts as LZSS here if a pointer table or the loader code confirms it.
- **Helper tools:** `tools/lzss.py`, `tools/scan_objpack.py`, `tools/blrefs.py` (finds Thumb `bl` callers), and `tools/verify_rom_map.py` (the regression check).

## Open questions
- What is the zero bank at `0x08239460` (821 × 0x40)? Probably Japanese names blanked out for the USA build.
- About 30% of uncompressed graphics banks A and B is still unlabelled. Walking every pointer table in `.rodata` 2 would label most of it.
- What are the `.rodata` 2 blocks at `0x0819DD94`, `0x081A0594`, `0x081A2D94` (0x2800 each)?
