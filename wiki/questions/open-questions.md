---
title: Open questions
type: question
status: draft
confidence: low
sources: []
updated: 2026-10-01
---
# Open questions

Add a question when it comes up. When one is answered, strike it through, then link the page that answers it.

## Toolchain
- [x] ~~Which compiler built EDS: agbcc or ARM SDT/ADS? Which flags?~~ agbcc family: libgcc/libc match byte for byte. Game code is `old_agbcc -O2`, the sound driver `agbcc -O2 -fprologue-bugfix`, and AgbSram `-O1`. See [[compiler-flags]], [[agbcc]].
- [x] ~~Is there prior work (an EDS disassembly or decomp) we can build on?~~ None found (2026-09-29 web search). We started from scratch (see [[toolchain]]).
- [ ] Were the ARM sound mixer (`0x0807EAD0`) and the Konami link-cable library written in asm or C (`agbcc_arm`)? See [[sound-mixer]].
- [ ] Where are the real translation-unit boundaries? The current units are 4 KiB chunks. Hints: rodata grouping, and static functions called from only one place.
- [ ] Were some functions built without GCSE? `sub_0802F5F8`, `sub_08030620` and `sub_0802FEA4` match more naturally with `-fno-gcse`, but that flag breaks most other functions. That would suggest per-file flags or the original's TU boundaries. See [[code-0802fb64]], [[compiler-flags]].

## ROM structure
- [x] ~~Where does code end and data begin?~~ `.text` ends at `0x08080A20`, after the 8-byte linker veneer. See [[rom-map]].
- [x] ~~Which sound engine: m4a or a custom Konami one?~~ Custom Konami driver. See [[sound-engine]].
- [x] ~~Are graphics compressed?~~ No BIOS compression. A custom LZSS is used for dialogue scenes only; card art is 6bpp-packed. See [[graphics-formats]].
- [ ] About 30% of the uncompressed graphics banks is unlabelled, and so are the three 0x2800-byte blocks at `0x0819DD94`. See [[rom-map]].
- [ ] Which function loads the 4bpp image packs?
- [ ] How is the song sequence bytecode encoded, and how are tracks assigned to PSG or PCM? See [[sound-engine]].

## Program structure
- [ ] What is the Timer2 counter at `0x030051FC` for? See [[interrupt-handlers]].
- [ ] Is `SetMainCallback(NULL)` at `0x0801BCBA` a deliberate soft reset? See [[set-main-callback]].
- [ ] Boot forces SE/BGM on after loading the save. Do the saved sound flags (`gSaveData+0x2152`) do anything?
- [ ] Nothing checks the `"DMEX1INT"` save signature, and the title screen only tests the checksum. What happens with a corrupt save? What are the full save layout and checksum algorithm? See [[save-game]].
- [ ] What does bit 7 of `gSaveData+4` do? It switches between two text renderers (`0x08074E60` and `0x08074F50`).
- [ ] Which duel phase is which in the 10-entry table at `0x08198F80`?
- [ ] Is the link code Nintendo's MultiSio sample (sync word `0xFEFE`)? A byte-for-byte comparison would settle it.
- [ ] Where does `gMain` end (about `0x030048CC`), and what is the struct at `0x030049D0`? See [[ram-map]].
- [ ] Do the deck-edit 20-byte panel cells at `0x0201DB20` start at `+0x1724` (flag bytes at cell +2/+3) or at `+0x1726`? The matched C view has to start at the 4-aligned `+0x1724` because agbcc aligns structs to 4; the ROM accesses alone do not decide the original layout. See [[code-0806704c]].

## Game data
- [x] ~~Where are the card stats?~~ `0x08621DE0`. See [[card-table]].
- [x] ~~How do card IDs map to the alphabetical name table?~~ They're the same index, since every per-card table uses the 1-based alphabetical ID. See [[card-id-map]].
- [x] ~~What are the other name hits at `ROM+0x24F980` and `ROM+0x257ED3`?~~ They're inside card descriptions. See [[card-descriptions]].
- [x] ~~Where are the password table, deck lists and duelists?~~ See [[password-table]], [[deck-lists]], [[duelist-table]].
- [ ] How is "has an effect" known for fusion and ritual monsters? Only numbers 812 and 730 are special-cased.
- [ ] What are the 136 effect-handler keys from 1211 to 1552 that aren't card numbers? How are the 76 effect monsters without a key dispatched?
- [ ] Is the kana sort-key table at `0x08624DF4` ever used? What is the all-zero second name bank at `0x08239460` (Japanese names blanked out?)?
- [ ] Which on-screen rarity is each pack slot? What are packs 801/802/901–903? What are the shop names of the random packs `0x66`/`0x67`/`0x6E`? See [[booster-packs]].
- [ ] When are the alternate decks for duelists 1–5 used? See [[deck-lists]].
- [ ] Are the 13- and 26-card lists really AI priority lists? What is the 60-card list for? See [[special-card-lists]].
- [ ] What do the attribute bits hold for non-monsters? Are the Gods and Tickets unplayable story items?
- [ ] What do the text codes `$c`, `$p`, `$h` and `$k` do, and which colours are `@2`/`@3`? See [[text-system]].
- [ ] Is special booster-pack type 21 Trap and 22 Magic ([[booster-packs]]), or the reverse ([[code-08006878]])? The pages may use different encodings; compare the pack code with the stat-word `type` field ([[card-table]]: 21 Trap, 22 Magic).

