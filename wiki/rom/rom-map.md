---
title: ROM Map (data area)
type: rom
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# ROM Map

This page maps the whole 8 MiB image, with the data area after `.text` (`0x08080A20`–`0x08800000`) in detail. Each row gives the half-open range `[start, end)`, what the segment holds, and whether the claim is **V** (verified against the ROM with a script, or by decoding and looking at the result) or **H** (hypothesis). Format details live on the linked pages.

To re-check the main **V** boundaries, run `python3 tools/verify_rom_map.py`. It checks 29 facts and should print `0 failure(s)`.

Since 2026-10-02 every data row below is also an asset in `config/assets.tsv` that extracts to an editable format and builds back byte-identical ([[assets]]). The converters decode each range completely, which turned most remaining **H** rows into **V** and corrected several boundaries (callouts below).

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
| `0x08080A20` | `0x08087FB4` | Game `.rodata`. It contains:<br>• debug `printf` strings (`"Change BG:%d\n"`, `"DM5:Script=..."`), three of them Shift-JIS (`0x080876B4`…);<br>• about 280 duel prompts with `@n` colour codes (`0x0808275C`–`0x08086391`);<br>• the duelist-name pool (`0x08081260`);<br>• the booster-pack ID list `0x080819BE` and the notable-card list `0x08081A6C` ([[special-card-lists]]);<br>• the booster-pack info table `0x080865DC` ([[booster-packs]]);<br>• calendar/event names (`0x08087724`);<br>• the sprite-animation step arrays (`AnimStep[]`: scene sets `0x08080B08`–`0x08081260`, die toss `0x08081FD4`–`0x080822FC`, `0x0808240C`–`0x080826DC`, `0x08086C54`–`0x0808733C`; see [[scene-sets]]);<br>• the s16 sine table `0x08087BA4`–`0x08087E24` (320 values: 256 per turn plus 64 for cosine, 0x100 = 1.0, split by the label `0x08087D8C`);<br>• 86 Shift-JIS hiragana with voiced marks folded (が→か) at `0x080874A8`, with no label (hypothesis: a sort map).<br>Code refers to each string directly from a literal pool. The `tables_code` converter types every byte as a string or a table, and no raw bytes are left. Unreferenced data (no label, no pointer): `0x08080AFA`, `0x080813C4`, `0x08081F7C`, `0x080875C4` (bit masks), `0x0808769C`, `0x0808771C`. See [[text-system]] | V (layout and types), H (some roles) |
| `0x08087FB4` | `0x08087FD0` | AgbSram rodata: `"SRAM_V112"` plus 4 dead `.LC` words. See [[agb-sram]] | V |

### Sound data (`0x08087FD0`–`0x08139F5C`)
The driver and the formats are on [[sound-engine]]. This table only pins the extents.

| Start | End | Contents | V/H |
|---|---|---|---|
| `0x08087FD0` | `0x08088510` | SE table: exactly 48 × 0x1C `{const u8 *track[6]; u8 priority; u8 slotMask; u16 lockTicks}`. Only the first 3 of the 6 track pointers are ever non-NULL, and all 49 non-NULL pointers land in the next row. See [[sound-sequence-format]] | V |
| `0x08088510` | `0x08088A20` | SE track bytecode, starting with SE 37's track; the data ends at `0x08088A12`, followed by 14 zero bytes | V |
| `0x08088A20` | `0x08088A90` | PCM table (the id bit 15 = 1 bank): 28 entries, 26 pointers and then 2 NULL (ids `0x801A`, `0x801B`) | V |
| `0x08088A90` | `0x080E09D0` | 26 PCM samples. Each is `{s32 rate = 0x800; u32 len; s32 loop (−1 = none)}` + s8 data, padded to 16 | V |
| `0x080E09D0` | `0x080E0F40` | Song table: 58 × 0x18 `{u32 songData; u16 trackOffset[10]}` | V |
| `0x080E0F40` | `0x0811B420` | Song track data (58 songs, byte-packed). The data ends at `0x0811B417`; 9 zero bytes follow | V |
| `0x0811B420` | `0x0811B4B0` | PCM table (the id bit 15 = 0 bank), 36 pointers | V |
| `0x0811B4B0` | `0x08139550` | 36 PCM samples (3 of them loop), ending at `0x08139542`, then 14 zero bytes | V |
| `0x08139550` | `0x08139F50` | PSG channel-3 wave-RAM patterns, 10 waves × 16 levels × 16 bytes | V |
| `0x08139F50` | `0x08139F5C` | PSG noise presets: 6 × u16 `SOUND4CNT_H`, all `0x8000` (read by the sequencer; see [[sound-engine]]) | V |

> [!warning] Contradiction
> Three sound rows changed when the sound converters decoded them (2026-10-02; re-checked against the ROM bytes for this page).
> - The bank-1 PCM table was given as `0x08088A20`–`0x08088A88` with 26 pointers. The driver's table runs to `0x08088A90`: 28 entries, the last two NULL.
> - The song data was said to end at `0x0811B415`. The last two bytes, `00 FD` at `0x0811B415`, are song 57's empty track, which four of its ten track offsets point to, so the data ends at `0x0811B417`.
> - `0x08139F50` was "a small struct read by the sound code" (H). It is the noise preset table.
>
> Resolved in favour of the decoded data.

### Game tables (`0x08139F5C`–`0x081ABE4C`)
| Start | End | Contents | V/H |
|---|---|---|---|
| `0x08139F5C` | `0x08139F64` | 2 pointers to the LZSS dialogue-box bitmap `0x0874C650` | V |
| `0x08139F64` | `0x0813ADD4` | Duelist/character table: 28 × 0x84 `{u32 id; char name[0x40]; char shortName[0x40]}`. See [[duelist-table]] | V |
| `0x0813ADD4` | `0x0813ADF4` | 8-word step table of the bust-up runner `sub_08001AE4`/`sub_08001B34` (`0x08001365`, `0x080016A9`, …; see [[debug-menu]] "Bustup", [[scene-sets]]) | V |
| `0x0813ADF4` | `0x0819739C` | **Dialogue table**: 490 × 0x304 `{u16 eventId; u16 speakerId; char text[0x300]}`. See [[text-system]] | V |
| `0x0819739C` | `0x081976A0` | Terminator: one complete 0x304-byte dialogue record with event = speaker = 0xFFFF and empty text | V |
| `0x081976A0` | `0x0819790C` | **Scene-set descriptors**: 31 × 0x14 `{lz bitmap; bgPal; objPal; lz objTiles; anim}`. See [[scene-sets]] | V |
| `0x0819790C` | `0x0819A9D4` | Code tables, typed item by item by the `tables_code` converter. It contains:<br>• the scene sets' animation track lists (the descriptors' `anim` targets, e.g. `0x08197954`) and their OAM sprite templates;<br>• about 40 NULL-terminated step-function tables;<br>• calendar events `{u32 flags; char name[0x40]}` (`0x081980D4`);<br>• the Mode-4 bitmap table `0x08198440` (5 × `{pal, bitmap}`) and image-pack pointer lists (`0x081985A0`, 25 entries; `0x08198604`, 5 entries);<br>• starter-deck pools `0x08198634`–`0x0819879C` ([[deck-lists]]);<br>• chain banner texts (`0x08198DE4`) and the BGM per opponent (`0x08198F20`);<br>• HBlank scroll waves;<br>• the fusion lists `0x0819A7C8` (52 two-material recipes + a 999 terminator) and `0x0819A970` (3 three-material recipes + terminator) ([[special-card-lists]]).<br>Unreferenced: `0x081979DC` (4 sprite templates), `0x081987B0` (4 scroll waves), the track list `0x081999C4` | V (layout and types), H (some roles) |
| `0x0819A9D4` | `0x0819D1C4` | Card-effect handler table: 426 × 0x18 `{u16 number; u16 flags (always 0); fn resolve, check, prepare, chainA, chainB}`, with numbers ascending 15..1552. 290 numbers are card numbers. All 867 non-NULL pointers are Thumb entries of known functions. See [[cards]] | V |
| `0x0819D1C4` | `0x0819D34C` | Tribute-summon prompts `char *[5]` (`0x0819D1C4`); duel step handlers (`0x0819D1D8`, 14 + NULL, `sub_0804E31C`); monster type names `char *[20]` (`0x0819D214`) and attribute names `char *[6]` (`0x0819D264`); the card jump arc `{s32 dx, dy}[16]` (`0x0819D27C`); the AI card lists `0x0819D2FC`/`0x0819D316` ([[special-card-lists]]) | V |
| `0x0819D34C` | `0x0819DD64` | Deck lists (card-number arrays + `{const u16 *cards; u16 count; u16 pad}` header tables). See [[deck-lists]] | V |
| `0x0819DD64` | `0x081A7A0C` | `.rodata` 2 (sub-tables below): AI card and step tables; the **HBlank warp tables** `0x0819DD94`–`0x081A4194`; UI tables from `0x081A4194` (including the booster-pack slot lists and rarity thresholds, [[booster-packs]]); OAM sprite frames `0x081A5790`–`0x081A70FC`; scene step tables; the **debug-menu item table** `0x081A73A0` ([[debug-menu]]); the **ASCII-to-Shift-JIS table** `0x081A76A0`; the Forbidden/Limited list `0x081A78B4`; sound channel maps | V |
| `0x081A7A0C` | `0x081ABC4C` | Sound lookup tables ([[sound-sequence-format#Driver lookup tables]]): nibble volume scale `0x081A7A0C` (16 × 256 u8); PCM pitch `0x081A8A0C`–`0x081AA20C` (3,072 u16, one per 1/32 semitone, used from its middle `0x081A960C`); PSG frequency `0x081AA20C`–`0x081AB70C` (2,688 u16, bit 15 = trigger); PSG vibrato steps `0x081AB70C`–`0x081ABC4C` (84 × 8 u16) | V |
| `0x081ABC4C` | `0x081ABE4C` | Sine table: 256 × s16 in 4.12 fixed point (`[64] = 0x1000`), `int(4096 * sin(2πi/256))` truncated; used by the sound driver's vibrato | V |

> [!warning] Contradiction
> Two `.rodata` 2 claims changed when the table and sound converters decoded the range (2026-10-02).
> - **The "three 0x2800 blocks, probably tilemaps"** at `0x0819DD94`/`0x081A0594`/`0x081A2D94` are the HBlank warp tables of `sub_0805DC38` ([[code-0805d58c]]): BG2X and BG2Y as `s32[16][160]` (0x2800 each), then BG2PA as `s16[16][160]`. The third table is 0x1400 bytes, not 0x2800, so the UI tables start at `0x081A4194`.
> - **The sound lookup row** listed "envelope/volume tables", a pitch table starting at `0x081A8D48`, and one PSG frequency table up to `0x081ABC4C`. The decoded layout is the four tables above. The pitch table starts at `0x081A8A0C`, and the old frequency range holds two tables.
>
> Both were re-checked against the ROM for this page: the block sizes, and every value of the pitch and frequency formulas. Resolved in favour of the decoding.

#### `.rodata` 2 sub-tables
The `tables_game` converter names each sub-table and writes it as `tables/rodata2/<name>.json`. Each one ends where the next begins.

| Address | Name | Contents |
|---|---|---|
| `0x0819DD64` | `ai_scan_cards` | 4 card numbers: Time Wizard, Cannon Soldier, Relinquished, Barrel Dragon |
| `0x0819DD6C` | `ai_turn_steps` | CPU-turn step handlers (10, with NULLs; `sub_0805BBE0`) |
| `0x0819DD94` / `0x081A0594` / `0x081A2D94` | `warp_bg2x` / `warp_bg2y` / `warp_bg2pa` | HBlank warp: s32[16][160], s32[16][160], s16[16][160] |
| `0x081A4194` | `shake_offset_regs` | BG2X/BG2Y/BG3X/BG3Y register pointers |
| `0x081A41A4`, `0x081A41F8` | `attribute_icons`, `kind_icons` | icon graphics pointers into bank A |
| `0x081A4214`–`0x081A451C` | duel UI | text colours, icon and cursor animation frames, lerp weights, zone positions and scroll targets, scale curves (0x100 = 1.0), card-move animation, banner offsets, zone-border colours |
| `0x081A452C` | `booster_packs` | slot lists, `PackSlots`, and the 28-entry table at `0x081A562C` ([[booster-packs]]) |
| `0x081A570C` | `pack_rarity_thresholds` | s32[8] {1, 3, 6, 10, 16, 28, 64, 180} |
| `0x081A572C`, `0x081A5758` | `pack_opening_steps`, `pack_unlock_ids` | debug "Get a pack" steps; u16[27] pack IDs |
| `0x081A5790` | `sprite_oam_a` | 305 OAM entries in 86 frames |
| `0x081A6118` | `card_list_menu_anims` | 14 animation track pointers + NULL |
| `0x081A6154` | `sprite_oam_b` | 501 OAM entries in 88 frames; `0x081A6154`–`0x081A6434` (92 entries) has no pointer to it |
| `0x081A70FC` | `deck_edit_anims` | 17 animation track pointers + NULL |
| `0x081A7144`–`0x081A71CC` | `slot_sprite_frames`, `filter_menu_sprites_a/b` | OAM frame pointers; `{OAM *oam; u8 count; pad[3]}`[14] × 2 |
| `0x081A723C`–`0x081A7374` | step tables | `transfer_steps`, `statistics_steps`, `deck_edit_steps`, `deck_edit_select_steps`, `deck_edit_sub_steps`, `deck_edit_popup_steps` ([[function-pointer-tables]]) |
| `0x081A7374`, `0x081A7382`, `0x081A7390` | link packets | 12-byte ACK, duplicate-ACK and NAK packets (types 0xF0/0xD0/0xE0) |
| `0x081A73A0` | `debug_menu_items` | `{char name[0x40]; callback}` × 10 + an empty end entry |
| `0x081A768C` | `debug_menu_steps` | 4 steps + NULL |
| `0x081A76A0` | `ascii_to_sjis` | u16[96]: full-width Shift-JIS for ASCII 0x20–0x7F |
| `0x081A7760`–`0x081A7784` | `map_fill_tile`, `bg_hofs_regs`, `bg_vofs_regs` | `{vu16 *reg; u16 mask; u16 pad}`[4] each |
| `0x081A77A8` | `sine_table_128` | s16[128], 0x100 = 1.0, `int(256 * sin(2πi/128))` truncated |
| `0x081A78A8` | `save_signature` | `"DMEX1INT"` in a 12-byte field |
| `0x081A78B4` | `card_copy_limits` | `{u16 card; u16 limit}`[47]: the Forbidden/Limited list ([[special-card-lists]]) |
| `0x081A7970`, `0x081A79A4` | `password_steps`, `card_trading_steps` | scene step tables |
| `0x081A79E8`, `0x081A79F4` | `sound_channel_map`, `se_variant_channel_order` | u8[12]; u8[4][6] (read through `gUnk_081A79F9`), ending exactly at `0x081A7A0C` |

The OAM frames are 8-byte OAM attribute entries (attr3 always 0). The animation scripts that use them are in `.rodata` 1 at `0x08086C54`–`0x08087338`. Each step is `{u8 frames; u8 count; u16 pad; const OAM *data}`. In `src/code_080784E4.c` the second byte is still `unk1`. The `tables_code` reading of the animation code (`sub_08078670`, `sub_080786D0`, `sub_08077EF4`) treats it as the number of templates drawn, and it matches each frame's OAM count ([[scene-sets#Animation]]).

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
| `0x08625460` | `0x08636288` | 7 **card-frame** image packs (32 colours, 144 8bpp tiles, 144 cells, 0x2698 each; 104×144 px with a blank art window): normal, effect, fusion, ritual, magic, trap, type 23. Chosen by card type in the code at `0x08006BB4`–`0x08006CD6` | V |
| `0x08636288` | `0x0870C640` | Uncompressed bank A, 476 items: 123 palettes, 237 tile blocks, 85 image packs, 23 BG maps, 6 sprite animation streams and 2 Mode-4 bitmaps. Contents:<br>• attribute palettes and icons (tables `0x08198950`, `0x0819897C`);<br>• 4bpp icon packs (tables `0x081989AC`, `0x081989F0`, `0x081A41A8`, `0x08086550`);<br>• the password keypad and the shop BG palette `0x0863CC7C`;<br>• **36 booster-pack covers** from `0x0864073C` (56×112 8bpp, 0x1880 each; 23 referenced, see [[booster-packs]]);<br>• the duel field, phase banners, "YOU WIN/LOSE/DRAW" and command buttons;<br>• the sprite streams `0x0868CAC0`…`0x0869771C`;<br>• Mode-4 bitmaps `0x086A12EC` (corridor) and `0x086B8568` (Millennium eye);<br>• dice, coin and card sprites;<br>• BG charblocks and maps for the deck editor, filter menu, trade and reel screens;<br>• interleaved 16×16 icons with palettes `0x08704D48`–`0x08706F28`;<br>• the "Card Trading" screen | V (bytes, kinds), H (some sheet arrangements) |
| `0x0870C640` | `0x0871B650` | LZSS scene sets 0–4, in the order 2, 0, 3, 4, 1: four 240×80 bitmaps and one 240×96 bitmap (set 2), each followed by a raw 256-colour palette. See [[scene-sets]] | V |
| `0x0871B650` | `0x0871CE50` | Small graphics: the opponent-select OBJ palette `0x0871B650` and **8bpp** OBJ tiles `0x0871B850`, a second palette, and two 4bpp digit sets | V |
| `0x0871CE50` | `0x0874C650` | 5 **Mode-4 bitmaps**, 240×160 8bpp (0x9600) plus a 256-colour palette each (table `0x08198440`): the duelist-select screens | V |
| `0x0874C650` | `0x0874E324` | LZSS dialogue-box bitmap 240×64 (`0x0874C650`), LZSS 240×24 header strip (`0x0874D5B0`, top 16 rows used), box palette `0x0874E104` and text palette `0x0874E304`. See [[scene-sets]] | V |
| `0x0874E324` | `0x087BDAA8` | LZSS scene sets 5–30: 26 × `{240×96 bitmap (LZSS 0x5A00), BG palette, OBJ palette, OBJ tiles (LZSS 0x2000)}`, the duelist "bust-up" dialogue scenes. See [[scene-sets]] | V |
| `0x087BDAA8` | `0x087F8568` | Uncompressed bank B, 64 items (41 packs, 11 palettes, 10 tile blocks, 2 bitmaps). Contents:<br>• the title logo (8bpp pack, 128 colours), copyright line, coin and flame;<br>• the delete-save prompt: stone-frame bitmap `0x087C29D4` and its text OBJ sheet `0x087CC1D4`;<br>• the Konami and KCEJ logos; the sky (`0x087D4B24`); the main-menu OBJ sheet; duel-score packs;<br>• 24 duelist face strips `0x087E795C`–`0x087EA716` (the first 24 pointers of `0x081985A0`) and 6 duelist-name list packs (the 6 pointers at `0x08198600`–`0x08198618`, which overlap the 25th entry of `0x081985A0` and the 5-entry list `gUnk_08198604`);<br>• the calendar bitmap `0x087EA718`, with day numbers, month names (one 0x800 block per season) and weekday names | V |

## Method
- **Block statistics:** `python3 tools/romstats.py <start> <end> <blocksize>` prints the zero, 0xFF and ASCII fractions, entropy, and pointer density per block. Card art shows up as entropy 7.6. Dialogue and name slots show up as 80% zeros.
- **Anchors from code:** every aligned u32 in `.text` (`0x08000228`–`0x08080A20`) with a value in `[0x08080A20, 0x08800000)` gives about 1050 literal-pool targets. Pointer tables inside the data (scene descriptors, pack lists, sample tables) add the rest. Most segment boundaries above are one of these targets.
- **Decoding:** a boundary is only marked V if the table parses to its exact end: the sample headers chain, the dialogue records have zero padding, the LZSS blobs decode to the expected size, and the image packs parse. Images were rendered with `tools/render_gfx.py` (Pillow in a throwaway container, output kept outside the repo) and looked at.
- **Caveat:** an LZSS size test on its own gives false positives (see the correction above). A blob only counts as LZSS here if a pointer table or the loader code confirms it.
- **Helper tools:** `tools/lzss.py`, `tools/scan_objpack.py`, `tools/blrefs.py` (finds Thumb `bl` callers), and `tools/verify_rom_map.py` (the regression check).
- **Asset converters (2026-10-02):** `tools/assets.py` and its plugins in `tools/assetfmt/` decode every data row. Each one is checked by building it back to the exact ROM bytes (`tools/assets.py verify`: 83/83). See [[assets]].

## Open questions
- What is the zero bank at `0x08239460` (821 × 0x40)? Probably Japanese names blanked out for the USA build.
- [x] ~~About 30% of uncompressed graphics banks A and B is still unlabelled.~~ The gfx_banks converter itemises all five banks (562 items, each typed and rendered). Some tile sheets' preview palettes and arrangements are still guesses ([[graphics-formats]]).
- [x] ~~What are the `.rodata` 2 blocks at `0x0819DD94`, `0x081A0594`, `0x081A2D94`?~~ HBlank warp tables (see the callout above).
- Several items have no reference from code or data: `sprite_oam_b` entries `0x081A6154`–`0x081A6434`, the sprite templates `0x081979DC`, the scroll waves `0x081987B0`, the track list `0x081999C4`, and the `.rodata` 1 items listed above. Are they leftovers or reached through computed addresses?
