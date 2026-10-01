---
title: LZSS decompressor (sub_0807A1A8)
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# LZSS decompressor (`sub_0807A1A8`)

| Field | Value |
|---|---|
| Address | `0x0807A1A8`–`0x0807A298` (0xF0 bytes, including literal pools at `0x0807A21C` and `0x0807A290`) |
| Mode | Thumb |
| Signature | `void LZSSDecompress(const u8 *src, u8 *dst, s32 packedSize)` (proposed name) |
| File | not split yet |
| Match | **matching** (2026-10-01, `src/code_0807960C.c`) |

## Purpose
This is the game's **only** LZ-family decoder. It implements Okumura LZSS with a 4 KiB ring, a first write position of `0xFEE`, a flag byte read LSB first (1 = literal), and 2-byte references `pos = b0 | (b1&0xF0)<<4`, `len = (b1&0xF)+3`. The ring buffer is EWRAM `0x02030000`. The stream format is on [[graphics-formats]].

Callers read the blob header themselves, as two halfwords (`ldrh [p,#2] << 16 | ldrh [p]`), and pass `src = blob + 4`. The decoder then consumes exactly `packedSize` bytes. It never checks the output size.

## Callers / Callees
- Called by 10 `bl` sites, all in the three dialogue-scene loaders `0x08000420`, `0x08000570` and `0x08000708` (`0x0800044C`, `0x0800047A`, `0x080004B2`, `0x0800059C`, `0x080005CC`, `0x080005F8`, `0x08000628`, `0x08000730`, `0x08000766`, `0x08000786`).
- Calls nothing.

## Equivalent C (sketch, not matched)
```c
void LZSSDecompress(const u8 *src, u8 *dst, s32 size)
{
    u8 *ring = (u8 *)0x02030000;
    u32 flags = 0;          /* kept on the stack in the asm */
    u8  mask = 0;           /* sl; advanced as (mask << 25) >> 24 */
    u16 r = 0xFEE, i;       /* write position, r7 */

    for (i = 0; i <= 0xFED; i++)        /* note: 0xFEE..0xFFF are not cleared */
        ring[i] = 0;
    do {
        mask <<= 1;
        if (mask == 0) { flags = *src++; size--; mask = 1; }
        if (flags & mask) {
            u8 c = *src++;
            *dst++ = c; ring[r++] = c; r &= 0xFFF; size--;
        } else {
            u16 p = *src++; u8 b = *src++;
            u16 len;
            p |= (b & 0xF0) << 4; len = (b & 0x0F) + 3; size -= 2;
            for (i = 0; i < len; i++) {
                *dst++ = ring[p];
                ring[r++] = ring[p++];      /* the asm reloads ring[p] after the dst store */
                r &= 0xFFF; p &= 0xFFF;
            }
        }
    } while (size != 0);
}
```
`u16` counters show up as `lsls #16; lsrs #16` pairs. The mask update is `lsls #25; lsrs #24`, which suggests a `u8 mask` or an explicit `(mask << 1) & 0xFF`. The first iteration always reads a flag byte, because the mask starts at 0.

## Matching notes
The things to watch were the stack slot for `flags`, the `size--` placement before the literal branch, and the ring re-read in the copy loop. The details that decided the match are listed under Matching source below.

## Method
The only `0xFEE` literal in `.text` is at `0x0807A220`. Disassembled with `tools/dr python3 tools/cs.py t 0x0807A1A8 0xF0`, and callers found with `python3 tools/blrefs.py 0x0807A1A8`. The reference decoder `tools/lzss.py` reproduces the expected sizes for all 59 scene blobs.

Related: [[graphics-formats]], [[rom-map]].

## Matching source (2026-10-01)
`sub_0807A1A8` is byte-matching C; see [[code-0807960c]] for the three details that mattered: `u16` ring/position/byte locals, declaration order, and the match length kept in the loop condition. The "Equivalent C (sketch)" above is behaviourally the same as the matched source.
