---
title: ROM versions (USA, Japan) and the multi-version plan
type: rom
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# ROM versions

The project targets the **USA** release first. On 2026-10-01 the user supplied every dump they could find, and they contain two distinct game builds. Both live in `roms/` and are never committed (`.gitignore` covers `roms/`, `*.gba`, `*.zip`).

| File | Header title | Code | SHA-1 | Notes |
|---|---|---|---|---|
| `roms/base_eng.gba` | `YU-GI-OH!EDS` | `AY5E` | `510fbba212aca9bab95ea12f8fd933e62ee34dea` | The baserom; `baserom.gba` is a symlink to it. Used size ≈ `0x7F8568`. |
| `roms/base_jp.gba` | `KCEJDM5EX1` | `AY5J` | `dc25f733cee913afdf187ec03df30909fe28b03c` | *Yu-Gi-Oh! Duel Monsters 5 Expert 1* (Japan). Used size ≈ `0x7FFFFC`. |

**Verified (2026-10-01, `hashlib` and a header read):** three of the supplied USA copies were byte-identical to `base_eng.gba`. A fourth `[Eng]` dump (SHA-1 `e092d4d3…`) differs in only two places. Its entry branch at `0x08000000` jumps to about 0x3F03 bytes of injected ARM code at `0x087FC000`, in the padding, which is a cracktro or a repro-cart save patch. It isn't a separate release and was discarded.

## Japanese build compared with USA
Verified by direct comparison (`build/solo-s49/jpcompare.py`):
- **Same compiler and source lineage.** `sub_0807A1A8` ([[lzss-decompress]]) is byte-identical in JP, literal pool included, at `0x08002C80` (USA `0x0807A1A8`).
- **Different link order.** Units appear in a different order (the LZSS/tile library sits near the start of JP's code). The JP game code ends earlier, with its SWI stubs are at `0x0807D144` (USA `0x0807ECF8`), about 0x1BB4 bytes less code.
- **Mostly shifted offsets.** With call and branch offsets, PC-relative load immediates and address-like pool words masked, then aligning each USA function to JP:

| Identical modulo relocation | Functions | Share of code bytes |
|---|---:|---:|
| 100% | 166 | 3.1% |
| ≥ 98% | 174 | 3.6% |
| ≥ 90% | 205 | 4.4% |
| ≥ 75% | 247 | 6.1% |
| ≥ 50% | 474 | 13.8% |

> [!question] Hypothesis
> The low exact-match share comes mostly from localization changing RAM and struct layouts (text buffers, save data), which alters the immediate field offsets in most functions, rather than from different logic. This still needs to be checked per subsystem. The masking is conservative, so the real "same logic" share is higher.

## Multi-version handling in other decomps
Single source tree with a build-time version switch:
- **pret pokeruby / pokefirered:** Makefile targets per version/revision (each with its own baserom, SHA-1 and build dir). `GAME_VERSION` / `GAME_REVISION` / `GAME_LANGUAGE` become `-D` defines, and the code uses `#if`.
- **zeldaret/tmc (Minish Cap, agbcc GBA):** `make usa|eu|jp|demo_*`, per-version baseroms, `#ifdef` code differences, and assets extracted per version from a manifest with per-version offsets. The closest model for EDS.
- **N64 (sm64, oot, mm, papermario) and GameCube dtk-template projects:** `VERSION=` / `--version`, with per-version splits, symbol addresses and asset configs.

## Plan (agreed with the user, 2026-10-01)
1. **Finish USA to 100%** matching source.
2. **Global rename to semantic names.** Address-suffixed placeholders (`gUnk_0201930C`, `sub_08012345`) resolve through `tools/autosyms.py` from the USA address, so they can't serve two versions. Each name needs a per-version address table.
3. **`make jp` skeleton:** its own baserom (`roms/base_jp.gba`), SHA-1, link order (`units_jp.txt`), symbol address table, rodata splits and asset offsets. Bootstrap JP as an all-assembly byte-matching build, the way USA started ([[decomp-workflow]]).
4. **Share the C.** Replace JP assembly with the USA C function by function, putting struct layouts and constants behind `#if VERSION_JP` where localization changed them. The JP-only functions get decompiled separately.

Related: [[rom-header]], [[rom-map]], [[overview]].
