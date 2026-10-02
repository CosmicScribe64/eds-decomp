---
title: code_08072FAC (image-pack loaders, BG map helpers, BASICSIO link install/send/receive) decompilation status
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# code_08072FAC: image-pack loaders, map helpers and the multi-player link layer (`0x08072FAC`-`0x080740BC`)

`src/code_08072FAC.c` (19 functions, 0x1110 bytes). **14 / 19 are C and byte-matching** after 2026-10-01 (`sub_0807332C` by the permuter just before workflow wave 1, `sub_080730A8` in wave 1); 5 stay `INCLUDE_ASM` (3 with a near-miss attempt under `#if 0 /* NONMATCHING */`: `sub_080735D4`, `sub_08073784`, `sub_08073F04`; 2 not attempted: `sub_0807382C`, `sub_08073C10`). The unit links to the exact target bytes. Compiler `old_agbcc -O2`. Names are proposals; code keeps `sub_08XXXXXX`. Continues [[code-08071f40]] (whose `sub_08072EB0` is the same loader with map buffer `0x03000C5C`); the link state is the `LinkSio` block of [[code-080740bc]] / [[code-080750e0]] (`0x03005B60`), and `sub_080740BC` (link step, in [[code-080740bc]]) is the receive pump these functions call.

## Functions

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| `0x08072FAC` | 0xFC | **matching** | `LoadImagePack8(mapBase, palIdx, tileBase, img)` | 8bpp image pack: adds `palIdx` to every non-zero pixel byte while copying the tiles (64 bytes each) to `0x06004000 + tileBase*32`, copies the palette to `0x05000000 + palIdx*2`, writes the cells to the map buffer `0x0300045C`, cell = `tile + tileBase/2`. Returns the tile count |
| `0x080730A8` | 0xDC | **matching** (wave 1, 2026-10-01) | `LoadImagePackRel(mapBase, palIdx, tileBase, img)` | 4bpp pack; cell positions are taken relative to the first cell (`col - col0`, `(row - row0) << 5`), map `0x0300245C` (BG map bank 4) |
| `0x08073184` | 0x4C | **matching** | `LoadImagePackGfx(palIdx, tileBase, img)` | copies tiles (32 bytes each) to `0x06004000 + tileBase*32` and the palette to `0x05000000 + palIdx*2`; returns the tile count |
| `0x080731D0` | 0x9C | **matching** | `LoadImagePack(mapBase, palIdx, tileBase, img)` | `LoadImagePackGfx` + cell list to `0x03000C5C`, cell = `(tile + tileBase) \| (palIdx >> 4) << 12` |
| `0x0807326C` | 0xC0 | **matching** | `LoadImagePackInline` | same as above, map `0x0300045C`, copies inlined |
| `0x0807332C` | 0xC8 | **matching** (2026-10-01, permuter; FAKEMATCH) | `LoadImagePackRow(row, mapBase, palIdx, tileBase, img)` | as `0x0807326C` into map buffer number `row` (`0x0300045C + row*0x800`); `img` is the stack argument |
| `0x080733F4` | 0xA4 | **matching** | `LoadImagePackRel2` | `LoadImagePackGfx` + cells relative to the first cell's position, map `0x03000C5C` |
| `0x08073498` | 0x3C | **matching** | `ClearBgMaps` | clears the 8 BG map buffers (`0x0300045C`, 0x800 each) and the 0x1C00-byte text canvas at `0x02010014`, `*(u16 *)0x02010010 = 0` |
| `0x080734D4` | 0x2C | **matching** | `ClearBgMap0` | same for map buffer 0 only |
| `0x08073500` | 0x58 | **matching** | `FillMapRect(row, col, w, h)` | fills a `w x h` block of the map buffer with the halfword at `gUnk_081A7760` (row stride 0x40 bytes) |
| `0x08073558` | 0x1C | **matching** | `CopyCanvasToVram` | `CopyDoubleWords(0x06004400, 0x02010014, 0x1C00)` |
| `0x08073574` | 0x60 | **matching** | `ResetTextBgs` | `ClearBgMaps`, `sub_08075630`, `SetTextLimits(0, 0x27E)`, BG2/BG3 reference points 0, affine matrices = identity (`0x100`), `BG0CNT = 4` |
| `0x080735D4` | 0x168 | nonmatching (CSE/regalloc) | `LinkInstall(slotA, slotB)` | `IME=0; IE &= 0xFF3F; IME=1`, clears `LinkSio` (CpuSet fill, 0xB38 bytes), sets the 3 buffer pointers (`+0xA30/A34/A38 = +0xA54/A84/AB4`), `+0xA28[2] = 0xFF`, `RCNT = 0xC000`, SIOCNT = 0x1000 -> 0 -> 3 -> `\|= 0x2000`, `RCNT = 0`, `state = 0xC`, `+0xA40 = 0x1000`, stores `slotA/slotB` at `+0/+4`, `IE \|= 0x80` (SERIAL), `*slotA = *slotB = sub_08075F74 \| 1`, `SIOCNT \|= 0x4000` (IRQ enable), and if `+0xB0C` bit 2 is clear also enables Timer3 (`IE \|= 0x40`) |
| `0x0807373C` | 0x48 | **matching** | `LinkDisableIrq` | `IME=0; IE &= 0xFF3F; *slotB = 0; *slotA = 0; IME=1; SIOCNT = 0x2000; IF = 0xC0` (used by `LinkInit` / `LinkShutdown` / `LinkSyncFinish`) |
| `0x08073784` | 0xA8 | nonmatching (regalloc) | `LinkSend(src, n)` | `slot = (SIOCNT & 0x30) != 0` (player id); returns 0 if `+0xAF8[slot]` (busy) is set; if `1 <= n <= 0x100`: `+0xAF0[slot] = (n+0x10)/16` (block count), `+0xAF8[slot] = n + 1`, `+0xAF4[slot] = 0`, header `0x03005B68 = n \| (n <= 14 ? 0x3000 : 0x2000)`, `CpuSet(src, 0x03005B6A, n/2)`, `sub_08074218(0x03005B68)` (checksums/builds the packet); returns 1 |
| `0x0807382C` | 0x338 | not attempted | `LinkPoll` (hypothesis) | per-frame link state machine: reads `sub_080740BC`, dispatches on the packet type nibble (`0x1000/0x2000/0x3000/0x8000/0xA000...`), reassembles multi-block messages into `0x03006650..` |
| `0x08073B64` | 0xAC | **matching** | `LinkRecvSlot(id, dst)` | runs `sub_080740BC`; if slot `id` has data and its type is `0x3000` copies the 7-halfword packet to `0x03005D6C + id*0x202`, then `(len & 0x1FF)/2` halfwords to `dst`; clears `+0xAF8[id]`/`+0xAF4[id]`; returns the length |
| `0x08073C10` | 0x2F4 | not attempted | `LinkRecvAll` (hypothesis) | receive with debug prints (`gUnk_080876B4/D4/F8` via `sub_0801A7DC`) for each of 2 slots; size-driven CpuSet copies |
| `0x08073F04` | 0x1B8 | nonmatching (draft) | `LinkRecv(id, dst)` (hypothesis) | the public receive: `sub_080740BC(0x03006676)`, loops the 2 slots and copies packets of type `0x3000`; `SIOCNT` player-id compare; returns the byte count from `+0xB12` |

The `sub_08073784` / `sub_08073B64` / `sub_08073F04` triple is the send/receive API used by [[code-08071f40]] (`LinkSendPacket` calls `sub_08073784`) and the handshake in [[code-0807b6b8]] (`sub_08073784`, `sub_08073F04`).

## Image pack format (verified from the loaders)

```
+0x00 u16 nColors
+0x02..0x07 (unused by the loaders)
+0x08 u16 palette[nColors]                      -> 0x05000000 + palIdx*2 (nColors*2 bytes)
+0x08+2n u16 nTiles, +2..+7 unused
+0x10+2n tile data: nTiles * 32 bytes (4bpp)    (nTiles * 64 bytes for LoadImagePack8, 8bpp)
+(after tiles) u16 nCells, +2..+7 unused
+8  cells: { u16 pos; u16 tile; } * nCells
      pos: column = pos & 0x3F, row = pos >> 8 (x 0x20 map cells per row);  map index = (col | (pos & 0xFF00) >> 3) + mapBase
      map entry = (tile + tileBase) | (palIdx >> 4) << 12        (palette bank from palIdx)
```
Tiles go to `0x06004000 + tileBase*32` (charblock 1). The 6 variants differ only in (a) which map buffer they write (`0x0300045C`, `0x03000C5C`, `0x0300245C`), (b) 4bpp copy vs 8bpp with palette-base added, (c) absolute vs first-cell-relative positions.

## `LinkSio` fields seen here (`0x03005B60`, offsets verified against the asm)

| Offset | Meaning |
|---|---|
| `+0x000`, `+0x004` | the two IRQ vector slots (`0x03000000`, `0x0300001C`) passed to `LinkInstall`; `LinkDisableIrq` clears `*(+4)` then `*(+0)` |
| `+0x008` | send header `u16` (type nibble `0x2/0x3` << 12 \| length); `+0x00A..` send payload (`0x03005B6A`) |
| `+0xA18[2]`, `+0xA1A[2]`, `+0xA28[2]` | per-slot byte counters; `+0xA28` set to `0xFF` at install |
| `+0xA26` | u16 cleared at install |
| `+0xA2C` | `s32 state` (install sets `0xC`) |
| `+0xA30/A34/A38` | pointers to the three 0x30-byte packet buffers at `+0xA54`, `+0xA84`, `+0xAB4` (rx current / rx done / spare) |
| `+0xA40` | u16 = 0x1000 at install |
| `+0xAF0[2]`, `+0xAF4[2]`, `+0xAF8[2]` | per-slot u16: block count of the pending send, progress, busy/remaining (set to `n+1` by `LinkSend`) |
| `+0xB0C` | u8 flags; bit 2 decides whether Timer3 is enabled at install |
| `0x03005D6C + id*0x202` | per-slot receive buffer (u16[257]) |

## Matching tricks learned (old_agbcc)

- **Rows of a buffer**: `extern u8 gBgMaps[8][0x800] asm("gUnk_0300045C")` and `for (i = 0; i < 8; i++) f(gBgMaps[i], 0x800)` matches `ClearBgMaps`; a pointer advanced by `0x800` makes old_agbcc hoist the `0x800` constant into a register.
- **Nested counted loop with moved decrement** (`FillMapRect`): `u16 *dst = base + col; for (; h != 0; dst += 0x20, h--) for (x = 0; x < w; x++) dst[x] = *src;` with `dst` computed as `(u16 *)(gUnk_0300045C + row * 0x800); dst += col;` reproduces the target's loop-inverted `sub r3,#1` placement (the `while (h != 0) {... h--}` forms do not).
- **`idx += mapBase;` as a separate statement** gives `adds r0,r0,rX` (expression first); the one-line `(a|b) + mapBase` swaps the operands of the add.
- **Argument order**: when the tile destination must be computed before `hdrT[0]`, hoist it into a named pointer declared between the `tiles` and `hdrC` locals.
- **Global `gUnk_03005B60` struct** reached through a `asm("gUnk_03005B60")` alias of a struct-typed extern (`gUnk_03005B60_s`) so it doesn't clash with other units' declarations.
- **Bitmask arithmetic on `SIOCNT`**: `slot = (u32)(-id | id) >> 31` reproduces the target's `neg; orr; lsr #31` (the plain `id != 0` compiles to a branch).
- **`(int)x >> id`** for `(((v << 16) & 0xF0000) >> 16) >> id` needs `int f = (v & 0xF0000) >> 16; (f >> id)` to get `asrs` (signed shift). `sub_08073B64` needed this split as three statements (`f = 0xF0000; f = f & v; f = (u32)f >> 16;`) so the AND writes in place (`ands r1,r0`) and the narrowing shift is logical (`lsrs`) while `f >> id` stays arithmetic (`asrs`).
- **Halfword load register + literal schedule** (`sub_08073B64`): to load `ret = *q & 0x1FF` as the target does (`ldr r2,=0x1FF; ldrh r0,[r4]; ands r2,r0`), dereference through a `volatile u16 *` and put the mask in a local first: `{ u16 m = 0x1FF; ret = *(vu16 *)q & m; }`. A plain `*q` puts the loaded value in r3 after the literal.
- **Load/copy register roles** (`LoadImagePack8` tile loop): for `w = *tiles; v = w; if (w & 0xFF00)…` the target keeps `w` in the *loaded* register and `v` in the copy; old_agbcc always merges the load into `v` instead (~15 declaration/assignment orders, u16/int/u32 types and `register` all give the same flip). An empty volatile asm taking `w` as an input right after the copy pins the roles and emits no bytes: `__asm__ __volatile__("" : : "r"(w));`.

## Nonmatching notes

- `0x080731D0`, `0x0807326C`, `0x080733F4` are now matching (the split shift `((pos & 0xFF00) >> 1) >> 2` plus routing the map base through a pointer/assignment makes old_agbcc hoist the literal ahead of the palette bank and keep `0xFF00` in `ip`; see the `FAKEMATCH` comments in the source).
- Historical (both matched 2026-10-01, see below): `0x080730A8`, `0x0807332C`: the target re-loads the map literal inside the loop (no hoist) and keeps `0xFF00` in `ip`; with `(u16 *)0x0300045C` (integer literal) the hoist disappears but the mask lands in r5. `0x0807332C`: target keeps `row` in `sl` and hoists `row*0x800 + 0x0300045C`; built spills it to the stack.
- Historical `0x0807332C` fresh sibling batch: an initialized row constraint to sl corrects all other callee-saved register roles. Reuse that row variable for the map base, keep a named 0xFF00 mask in ip, read the initial cell count into r5, and use a single guarded do/while loop. The best private candidate has eight diff lines. The initial `mov sl,r0` occurs after argument narrowing, and the mask is built in r5 instead of r0. Attempts with explicit word-argument narrowing and scratch staging were worse, so none were activated. Evidence: `build/manual_late_next/sub_0807332C.rowdo.best.c` / `.rowdo.diff`. Runtime narrowing and five-argument ABI must remain intact in future experiments.
- `0x080735D4`: structure identical; the target derives `0xA84`/`0xAB4` as `r2 + 0x30` from the `0xA54` literal and keeps the base in r4 (copy in r5), while the built version uses separate literals.
- `0x08073F04`: first C attempt (from the m2c draft). The target keeps the packet base `0x03006676` in r4 and derives the `LinkSio` base as `r4 - 0xB16`; the attempt materialised `0x03005B60` and other constants separately and spilled them to r8/sl, so the loop body and header field accesses still differ.
- `0x08073784`: the target keeps the source pointer in `ip` and has a dead `movs r7,#0`; mid-function literal pool. The `ip`-vs-low-reg choice for `src` and the SIOCNT temp/id register roles did not change across sio-temp, declaration-order, `unkAF4 = 0` and direct-index shapes.

Related: [[code-08071f40]], [[code-080740bc]], [[code-080750e0]], [[code-0807b6b8]], [[graphics-formats]], [[decomp-workflow]], [[compiler-flags]].

## Image-pack loaders matched (2026-10-01)

### `sub_0807332C` (0xC8; permuter, FAKEMATCH)

Matched by the permuter in commit `1776627`, just before workflow wave 1. The old near miss had `mov sl,r0` after the u16 parameter narrowing, while the ROM copies `rowArg` to sl first. The permuter moved the row binding `register u32 row __asm__("r10") = rowArg;` **inside** the guarded `if (i < n)` block; with the existing r12 mask pin and the empty `asm volatile("" : "+r"(row))` after the base computation, row stays in sl and the mask in ip as in the ROM. All three are FAKEMATCH devices, commented in the source.

### `sub_080730A8` (0xDC, start score 36; wave 1, ordinary C)

Working notes: `build/wf/sub_080730A8/NOTES.md`.
- The tile destination was computed as a call argument; a named `u8 *dst` local between `tiles` and `hdrC`, exactly as in matched `sub_0807326C`, fixes it.
- The index was built as `((dc << 16) | (dr << 21)) >> 16` with ints, giving `asrs` instead of `lsrs`. What matched: int `col`/`row`/`col0`/`row0`, then u16 temporaries `u16 dc = col - col0; u16 dr = row - row0; idx = dc | (dr << 5);`. Combine turns the two zero-extends into the target's `lsl 16 / lsl 21 / orr / lsr 16`. With the u16 temporaries the named `dst` no longer spills `palIdx<<16` (it stays in sl). The map base is the integer literal `(u16 *)0x0300245C`.
- Failed: u16 `col/row/col0/row0` (90); an int index expression without the u16 temporaries (`lsl 5 / orr / lsl16 / lsr16`, 90).
