---
title: ROM versions (USA, Japan) and the multi-version plan
type: rom
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# ROM versions

The project targets the **USA** release first. On 2026-10-01 the user supplied every dump they could find, and they contain two distinct game builds. Both live in `roms/` and are never committed (`.gitignore` covers `roms/`, `*.gba`, `*.zip`).

| File | Header title | Code | SHA-1 | Notes |
|---|---|---|---|---|
| `roms/base_eng.gba` | `YU-GI-OH!EDS` | `AY5E` | `510fbba212aca9bab95ea12f8fd933e62ee34dea` | The baserom; `baserom.gba` is a symlink to it. Used size ≈ `0x7F8568`. |
| `roms/base_jp.gba` | `KCEJDM5EX1` | `AY5J` | `dc25f733cee913afdf187ec03df30909fe28b03c` | *Yu-Gi-Oh! Duel Monsters 5 Expert 1* (Japan). Used size ≈ `0x7FFFFC`. |

**Verified (2026-10-01, `hashlib` and a header read):** three of the supplied USA copies were byte-identical to `base_eng.gba`. A fourth `[Eng]` dump (SHA-1 `e092d4d3…`) differs in only two places. Its entry branch at `0x08000000` jumps to about 0x3F03 bytes of injected ARM code at `0x087FC000`, in the padding, which is a cracktro or a repro-cart save patch. It isn't a separate release and was discarded.

## Japanese build compared with USA
Two analyses exist. The first (2026-10-01, `build/solo-s49/jpcompare.py`) aligned masked USA function bytes against the start of the JP image. The second (2026-10-02, [[jpmap]], commit `446546b`) disassembles JP with the project's own analyzer, pairs every USA function with at most one JP function, and checks the result by recompiling USA C at JP addresses. The second supersedes the first. Its reports are in `build/jp/` (local only; `build/` is gitignored), and the staged plan below comes from its write-up `build/jp/PLAN.md`.

**Summary:** JP is not a localisation of EDS. Both builds share the compiler, the SDK, the system and library layer and about 200 functions. The duel engine was reworked (a different card and player data model and 928 cards instead of 821). JP also links a 44 KB Mobile Adapter GB library that USA lacks. A matching JP build is a separate effort comparable in size to the USA one, with USA C available as a template for about a third of it.

### Layout
| | USA (AY5E) | JP (AY5J) |
|---|---|---|
| Functions (Thumb + ARM) | 1976 | 2136 |
| `.text` | `0x08000228`–`0x08080A20` (0x807F8) | `0x0800022C`–`0x08089B50` (0x89924) |
| After the SDK (SWI stubs, AgbSram) | libgcc, libc | **Mobile Adapter GB library**, 195 functions at `0x0807D2E4`–`0x08087F80`, then libgcc, libc |
| ARM code | the sound mixer ([[sound-mixer]], 3 functions) and its Thumb-to-ARM veneer | no mixer and no veneer; one ARM HBlank routine with an APCS frame at `0x0805CA00`, reached only through pointers |
| libc members | `memcpy`, `memset`, `strcpy` | `memcpy`, `strcat`, `strcmp`, `strcpy`; in libgcc, `__muldi3` follows `__lshrdi3` |
| [[crt0]] | `IntrMain` builds the nested-IRQ mask with `mov r1,#0x2280` | `ldr r1,=0x2008`, one more pool literal, so code starts 4 bytes later |
| Cards | 821, IDs sorted by English name ([[cards]]) | 928, IDs sorted by Japanese name; 819 card numbers in common, 816 of them with identical stats words |

- **The Mobile Adapter GB library** (identifier `MAGB`) is an HTTP/SMTP client for the Mobile Adapter GB service at `gameboy.datacenter.ne.jp`. It is Nintendo SDK code ([[nintendo-sdk-libraries]]) and has no USA counterpart.
- **Library symbols:** jpmap located JP's libgcc/libc functions by searching for the leading bytes of each USA library function (`memset` is not in JP). `strcat` and `strcmp`, which USA lacks, were identified by reading them (newlib word-at-a-time versions). No JP call into the library area is unidentified (`build/jp/summary.txt`).
- **Verified for this page (2026-10-02, a read-only Python read of both ROMs):** the JP SHA-1 and header; the literal `0x00002008` at `0x08000224` in JP and `mov r1,#0x2280` (`0xE3A01D8A`) at `0x080001C8` in USA; `MAGB` at `0x0807FFF8` and `gameboy.datacenter.ne.jp` at `0x0809ED97` in JP, with neither string in USA; `mov ip, sp` (`0xE1A0C00D`) at `0x0805CA04`, the APCS prologue of the JP ARM routine.

> [!warning] Contradiction
> **Where JP code ends.** This page said (2026-10-01, from `jpcompare.py`): "The JP game code ends earlier, with its SWI stubs at `0x0807D144` (USA `0x0807ECF8`), about 0x1BB4 bytes less code." [[jpmap]] and `build/jp/PLAN.md` §6 show this is wrong. The SWI stubs and AgbSram are at `0x0807D144` in JP, but `.text` continues after them with the 44 KB Mobile Adapter GB library and libgcc/libc, and ends at `0x08089B50`. JP has about 37 KB *more* code than USA. `jpcompare.py` had stopped its search at `0x0807D144`. Resolved in favour of jpmap, whose function list is complete (0 analyzer messages) and whose library positions were found byte for byte.

### Function correspondence
Every USA function, classified by [[jpmap]] (`build/jp/map.tsv`; counts re-checked from the file for this page):

| Status | Functions | USA code bytes |
|---|---:|---:|
| identical modulo relocation | 158 | 2.9% |
| same shape, different constants | 41 | 0.9% |
| … only struct offsets, add immediates or pool constants differ ("offsets-only") | 32 | 0.8% |
| … other immediates differ | 9 | 0.1% |
| changed (counterpart found, similarity 0.2–0.99) | 647 | 35.8% |
| … high / medium / low confidence | 311 / 164 / 172 | |
| … similarity ≥ 0.9 / ≥ 0.75 / ≥ 0.5 | 68 / 168 / 498 | |
| no JP counterpart found | 1130 | 60.5% |
| **JP-only functions** | **1290** (0x56EB4 bytes), 195 of them in the Mobile Adapter library | |

706 of the 846 pairs have every aligned call and function pointer consistent with the map. "No counterpart" does not always mean absent: some USA code exists in JP in a form rewritten past recognition. For example, `CanReviveGraveyardCard` has its counterpart right before JP `sub_08061840` (the counterpart of `CollectEffectTargets`, [[effect-target-collect-c]]), but it scores only 0.36.

### The compiler is the same (verified)
Ten matched USA C functions whose JP counterparts are identical modulo relocation were compiled with the USA compiler and flags (`old_agbcc -O2`, [[compiler-flags]]), linked at their JP addresses with their relocations set to the JP values, and compared. **All ten are byte-identical** (`build/jp/compile_test.txt`). Every relocation's JP target agrees with the independently derived function and address map. The largest, `LinkSioRecvMultiBlock` (0x338 bytes), has 20 relocations, 11 of them to RAM that moved. The 2026-10-01 comparison had already found [[lzss-decompress]] (`LZSSDecompress`) byte-identical, literal pool included, at JP `0x08002C80`.

### The game logic was reworked
The duel engine's data model differs between the versions:

| | USA | JP |
|---|---|---|
| Card word in a list | `u32`, ID in bits 0–11 (`& 0xFFF`), flags above | `u16`, ID in bits 0–10 (`& 0x7FF`), flag bit 12 |
| Player block | `struct DuelPlayer` at `0x020192E4 + p*0xD64` ([[duel-engine]]) | `0x020078F0 + p*0x18`, card lists at `+0x746 + p*0x64` |
| Card stats / ID→number tables | `0x08621DE0` / `0x08622AB4` ([[card-table]], [[card-id-map]]) | `0x0837FCE0` / `0x0837ECE0` (same packed stats format) |

Effect executors, target prompts, the deck editor and the duel UI mostly have no counterpart: their best JP candidates score at chance level (0.3–0.4), and the constants that mark the USA executors (for example `0x206`, `0x712` and `0x613` in [[effect-resolve9-c]]) never occur together in one JP function. Shared code is concentrated in the system and library layer (`build/jp/units.tsv`):

| USA unit | Identical or same-shape bytes | What it is |
|---|---:|---|
| `sdk/libagbsyscall`, `sdk/agb_sram` | 100% | SDK ([[bios-swi-stubs]], [[agb-sram]]) |
| [[bg-image-c]] | 93% | link/SIO |
| [[gfx-util-c]] | 71% | LZSS and graphics helpers |
| [[sprite-c]] | 53% | main loop, video helpers |
| [[bitmap-text-c]], [[link-sio-c]], [[main-c]], [[title-screen-c]], [[text-canvas-c]], [[coin-toss-scene-c]], [[collection-c]], [[bustup-scene-c]] | 15–26% | |
| [[text-bg-c]], [[main-menu-c]], [[dice-scene-c]], [[turn-order-steps-c]] | 6–9% | |
| the other 97 units | under 5% | game logic |

For one worked pair, `build/jp/sub_08044224_diff.md` compares `CollectEffectTargets` with JP `sub_08061840` case by case. It was written as matching hints for [[effect-target-collect-c]].

> [!warning] Contradiction
> **Localisation or rework?** This page held the hypothesis (2026-10-01): "The low exact-match share comes mostly from localization changing RAM and struct layouts (text buffers, save data), which alters the immediate field offsets in most functions, rather than from different logic." [[jpmap]] and `build/jp/PLAN.md` §6 refute it. The card and player data model differs, JP has 928 cards against 821, and 60% of USA code bytes have no counterpart even at the similarity level. RAM did move (below), and those deltas are verified for the system layer. But a pure layout shift leaves a function's instruction shape unchanged, and only 41 pairs are like that. Resolved: the hypothesis is withdrawn.

### Link order
Matched functions form 428 runs that keep their relative order in both builds (`build/jp/blocks.tsv`). Only 64 runs hold three or more functions, and consecutive runs are ascending in JP only 218 of 427 times, which is chance level. So the object files were linked in a different order. The longest shared runs:

| USA range | JP start | Functions | USA units |
|---|---|---:|---|
| `0x08027C58`–`0x08029EC4` | `0x0805DB48` | 37 | [[destiny-board-scene-c]], [[turn-order-scene-c]], [[turn-order-steps-c]] |
| `0x0807A490`–`0x0807ADE8` (LZSS library) | `0x08007508` | 19 | [[bitmap-text-c]], [[gfx-util-c]] |
| `0x0807B9D4`–`0x0807BF54` | `0x0807AB04` | 16 | [[link-sio-c]] |
| `0x08072C0C`–`0x08073574` | `0x08067A80` | 15 | [[text-bg-c]], [[bg-image-c]] |
| `0x08026194`–`0x08026C90` | `0x0807662C` | 15 | [[dice-scene-c]] |

The link code from `0x080735D4` forms an 11-function run. The sound driver, the SWI stubs and AgbSram come last in both builds. `build/jp/link_order.tsv` lists the JP order in terms of USA code. The breaks between runs are the best evidence so far for the original object-file boundaries in both versions (see [[open-questions]]).

### RAM
Address pairs come from aligned literal pools of matched pairs (`build/jp/ram_map.tsv`). *Strong* pairs come from identical or same-shape functions, where the alignment is exact. *Weak* pairs come from changed functions. EWRAM: 59 USA addresses paired, 9 strong. IWRAM: 26 paired, 20 strong. See [[ram-map]] for the USA names.

| USA | JP | Delta | Evidence |
|---|---|---|---|
| `gMain` `0x03000040` | `0x03000040` | 0 | strong, 20 votes |
| `bgMapBuffer` `0x0300045C` (and `0x03000C5C`, `0x0300245C` inside it) | same | 0 | strong |
| `oamBuffer` `0x03004470` | `0x03004460` | −0x10 | strong; something between `0x0300245C` and `0x03004470` is 0x10 bytes shorter in JP |
| link/SIO block `0x03005B60`–`0x03006676` | `0x03004890`–`0x030053A6` | −0x12D0 | strong, 12 addresses, all consistent |
| `gSoundDriver` `0x03005210` | `0x030053C8` | +0x1B8 | strong |
| `0x03005204` (IWRAM) | `0x02000938` (EWRAM) | | strong, 1 vote |
| `gSaveData` `0x02011C20` ([[save-type]]) and `0x02010014` | `0x02002C50`, `0x02001044` | −0xEFD0 | strong |
| duel-state aliases `0x020192E0`/`E4`, `0x0201930C`, `0x02019AA8`, … | one base `0x020078F0` plus offsets | | weak, many USA addresses to one JP base |

ROM data: 239 USA addresses paired, 16 of them strong (`build/jp/romdata_map.tsv`).

### Data and assets
Of the 83 USA asset ranges in `config/assets.tsv` ([[assets]]), JP contains (`build/jp/assets.tsv`, sampled 64-byte chunks):

| Status | Ranges | Which |
|---|---:|---|
| identical somewhere in JP | 5 | the noise preset table, the dialogue terminator record, scene sets 00–02 |
| mostly shared | 21 | the header, the deck lists (85%, [[deck-lists]]), 19 dialogue scene sets ([[scene-sets]]) |
| partly shared | 3 | the scene scripts and lists, the system tiles, the dialogue box |
| zero fill | 3 | padding and the empty name bank |
| not found | 51 | fonts ([[font]]), sound data ([[sound-engine]]), card names, descriptions and art, `.rodata` strings, most tables |

The "not found" data differs in content or format. JP needs its own asset manifest and probably its own converters for text (Shift-JIS), fonts, card art and sound.

### Earlier measurement (2026-10-01, superseded)
`jpcompare.py` masked call and branch offsets, PC-relative load immediates and every address-like pool word, then aligned each USA function to JP bytes below `0x0807D144` on its own:

| Identical modulo relocation | Functions | Share of code bytes |
|---|---:|---:|
| 100% | 166 | 3.1% |
| ≥ 98% | 174 | 3.6% |
| ≥ 90% | 205 | 4.4% |
| ≥ 75% | 247 | 6.1% |
| ≥ 50% | 474 | 13.8% |

jpmap's one-to-one pairing finds 158 identical functions. The likely reason for the difference (hypothesis) is that `jpcompare.py` let several USA functions align to the same JP bytes, which inflates the counts for small, generic functions.

## Multi-version handling in other decomps
Single source tree with a build-time version switch:
- **pret pokeruby / pokefirered:** Makefile targets per version/revision (each with its own baserom, SHA-1 and build dir). `GAME_VERSION` / `GAME_REVISION` / `GAME_LANGUAGE` become `-D` defines, and the code uses `#if`.
- **zeldaret/tmc (Minish Cap, agbcc GBA):** `make usa|eu|jp|demo_*`, per-version baseroms, `#ifdef` code differences, and assets extracted per version from a manifest with per-version offsets. The closest model for EDS.
- **N64 (sm64, oot, mm, papermario) and GameCube dtk-template projects:** `VERSION=` / `--version`, with per-version splits, symbol addresses and asset configs.

## Plan (agreed with the user, 2026-10-01)
1. **Finish USA to 100%** matching source.
2. **Global rename to semantic names.** Address-suffixed placeholders (`gDuelZones`, `sub_08012345`) resolve through `tools/autosyms.py` from the USA address, so they can't serve two versions. Each name needs a per-version address table.
3. **`make jp` skeleton:** its own baserom (`roms/base_jp.gba`), SHA-1, link order, symbol address table, rodata splits and asset offsets. Bootstrap JP as an all-assembly byte-matching build, the way USA started ([[decomp-workflow]]).
4. **Share the C.** Replace JP assembly with the USA C function by function, putting struct layouts and constants behind `#if VERSION_JP`. The JP-only functions get decompiled separately.

The order still holds. The 2026-10-02 measurements change step 4's scope: the C that can be shared as is covers about 200 functions, not most of the game (see the contradiction above).

## Staged plan for a matching JP build (2026-10-02)
From `build/jp/PLAN.md`, a research result that has not been started. **Scope:** one repo with shared tooling, headers, SDK and libraries; a shared core of 199 functions (158 identical + 41 same-shape); about 650 functions as `#if VERSION_JP` variants or JP copies derived from USA C; about 1300 JP-only functions, including the Mobile Adapter library, as a new decompilation. That is about 1940 functions of new matching work, on the order of the USA effort, with USA templates for a third of it.

**Layout:** USA keeps its paths, so the matched USA code, CI and objdiff do not churn. JP adds `config/jp/` (functions, units, splits, `disasm.json`, assets, `symbols.ld`, per-unit cflags), `asm/jp/`, `data/jp/`, `src/jp/`, `assets/jp/`, and `src/shared/<group>.inc.c` fragments that both versions `#include`. The research folder `build/jp/` should be renamed (for example to `build/jp-research/`) before the JP build output takes that path.

Effort is in this project's units: the USA run took 1976 functions from assembly to matched C in about four days of agent waves (2026-09-29 to 2026-10-02, [[log]]).

| Stage | Work | Estimate | Depends on |
|---|---|---|---|
| 1. Version plumbing | `VERSION ?= usa` in the Makefile (`make jp`, `compare-jp`, `setup-jp`; `-DVERSION_JP`); a `--version` flag (or `EDS_VERSION`) for `check.py`, `check_all.py`, `permute.py`, `wf.py`, `similar.py`, `target.py`, `m2c_draft.py`; per-version config dirs for `mkld.py`, `autosyms.py`, `setup.py`, `assets.py`; `INCLUDE_ASM` takes its directory from a macro | about 0.5 day | nothing |
| 2. All-assembly JP build | parameterise `tools/disasm.py` with `config/<ver>/disasm.json` (jpmap already runs it on JP: 2136 functions, 0 messages); `.ifdef VERSION_JP` in `asm/crt0.s`; `sdk/libmobile` as an asm unit after `sdk/agb_sram`; the ARM HBlank routine as an ARM asm unit; check `ld` reproduces JP's libgcc/libc member order; `.rodata` as `.incbin` chunks. Done when `make jp compare` prints `dm5ex1.gba: OK` | about 1 day | 1 |
| 3. JP assets | a raw-range `config/jp/assets.tsv` first (hours); then card tables, scene sets (shared with USA), Shift-JIS text, fonts, card art and sound, reusing USA converters where the format is the same (days) | hours, then days | 2 |
| 4. Names for both versions | JP placeholders get their own spelling (for example `sub_JP_0806172C`, `gUnkJP_020078F0`) so a USA-style name never resolves to the wrong address; shared code needs named symbols with per-version addresses. The 199 shareable functions reference 78 functions, 29 RAM and 16 ROM-data addresses: a scoped rename of about 320 symbols, with the JP column generated from `map.tsv`/`ram_map.tsv` and reviewed | about 0.5 day (scoped) | the global rename is its own project |
| 5. Shared core | 158 identical functions compile unchanged; 32 offsets-only take `#if VERSION_JP` layouts or sizes; 9 take `#if` constants. Shared groups move into `src/shared/*.inc.c` fragments included at the right place in each version's units (agbcc emits functions in source order). Long common runs are candidates for real shared units | about 1 day | scoped part of 4 |
| 6. Changed functions with USA templates (647) | `tools/wf.py` waves on JP units, each agent starting from the USA counterpart's C instead of an m2c draft; high-confidence pairs first (311; 168 with similarity ≥ 0.75), low-confidence pairs only as hints. Small differences go under `#if VERSION_JP` at statement granularity; a different data model (most duel code) becomes a JP copy in `src/jp/` | about 1.5–2 days of waves | global rename |
| 7. JP-only code (1290) | the USA pipeline (m2c drafts, `similar.py` across both versions, waves, the permuter). Find the Mobile Adapter library's compiler and flags first (`agbcc`/`old_agbcc`, `-O1`/`-O2`, with and without `-fprologue-bugfix`) or leave it as assembly; try `agbcc_arm` for the ARM HBlank routine | about 2–4 days of waves | global rename |
| 8. CI and progress | `check_all.py` and the no-regression rule per version; objdiff report per version; a version column on wiki function and unit pages | about 0.5 day | 2 |

Summing the estimates gives about 7–9.5 days, not counting stage 3's converters or the global rename (derived here from the per-stage figures, not stated in `PLAN.md`).

```
USA 100% ──► global rename ──────────────────────────────► stage 6 ─► stage 7
   │                 ▲ (scoped part first)                  ▲
   ▼                 │                                      │
stage 1 ─► stage 2 ─► stage 3                stage 4 ─► stage 5
```

Stages 1–3 do not depend on the rename and could start at any time; they give a byte-matching JP build and a JP ROM map. (2026-10-02: after the USA decompilation reached 100%, the user paused JP work. The order is the rename pass first, then JP; see [[overview]].) Stage 5 needs only the scoped part of stage 4. Stages 6–7 need the global rename, because otherwise every shared or derived function carries USA-address names the JP build cannot resolve. The duel code reaches struct fields through absolute aliases (`gDuelZones`, `gDuelDecks`, …) where JP uses one base plus offsets, so those aliases have to become struct globals with member accesses first. agbcc still folds `&gDuel.x` into one relocated literal, so USA keeps matching.

**Risks and open points** (from `PLAN.md` §5):
- Identical and same-shape pairs are exact; the 172 low-confidence changed pairs are similarity guesses. Re-run [[jpmap]] after stage 2, when the JP function list is final.
- Pairing by position (neighbours of matched pairs, shared callers) would recover some of the 1130 rewritten USA functions, such as `CanReviveGraveyardCard`. The total stays far below what a localisation would give.
- Object-file boundaries are unknown in both versions.
- JP data formats (font, card art, sound) still need analysis, so assets will take longer than they did for USA.
- A JP placeholder spelled like a USA one (`sub_0806172C` exists in both address spaces) would silently resolve to the wrong address; hence stage 4's distinct spelling.

Related: [[jpmap]], [[rom-header]], [[rom-map]], [[overview]].
