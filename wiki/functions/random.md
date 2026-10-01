---
title: Random (proposed)
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-09-29
---
# Random `sub_08076F9C`

| Field | Value |
|---|---|
| Address | `0x08076F9C` |
| Size | 0x1A (+ a 3-word pool) |
| Mode | Thumb |
| Unit | `asm/code_08076144.s` |
| Match | nonmatching |

## Purpose (verified)
```c
u16 Random(void) {
    u32 x = gMain.rngState * 0x343FD + 0x269EC3;   // 0x03000040 (gMain+0)
    x = (x << 16) | (x >> 16);                      // rotate by 16
    gMain.rngState = x;
    return (x << 1) >> 17;                          // bits 16..30 → 0..0x7FFF
}
```
- The multiplier and increment are the Microsoft Visual C++ `rand()` LCG constants. The rotation step is Konami's own.
- **It's called once per frame** by [[frame-sync-update]], so the RNG advances even when nothing asks for a number. There are 76 `bl` call sites in total, including the Campaign pre-duel code (`0x0801BC6A`), the Password scene and the duel engine.
- The seed is 0 at boot (IWRAM is cleared) and isn't seeded from anything else found so far. Variation therefore comes from the number of frames the player takes (hypothesis).

## Matching notes
- The return value is computed with `lsls #1; lsrs #0x11`. Try `return (x >> 16) & 0x7FFF` or `(u16)(x >> 16) & 0x7FFF`.

Related: [[ram-map]].
