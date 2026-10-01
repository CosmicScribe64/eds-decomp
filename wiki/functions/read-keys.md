---
title: ReadKeys (proposed)
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# ReadKeys `sub_08075228`

| Field | Value |
|---|---|
| Address | `0x08075228` |
| Size | 0x50 (code to `0x08075276`, with the pool at `0x0807524E`–`0x08075257` in the middle) |
| Mode | Thumb |
| Unit | `asm/code_080750E0.s` |
| Match | nonmatching |

## Purpose (verified)
[[frame-sync-update]] calls this function once per frame. It is the only place that reads `KEYINPUT` (`0x04000130`).
```c
void ReadKeys(void) {
    u16 keys = ~REG_KEYINPUT;                 // active-high
    gMain.newKeys  = keys & ~gMain.heldKeys;  // 0x03000046 ("trg")
    gMain.heldKeys = keys;                    // 0x03000044
    if (keys != gMain.prevKeys) {             // 0x03000048
        gMain.keyRepeatTimer = 0;             // 0x0300004A
        gMain.prevKeys = keys;
    } else {
        u16 t = gMain.keyRepeatTimer;
        gMain.keyRepeatTimer = t + 1;
        if ((u16)(t + 1) > 20) {              // after 20 frames held…
            gMain.keyRepeatTimer = t - 1;     // …step back 2, so it repeats every 2 frames
            gMain.newKeys |= keys & 0xF0;     // auto-repeat for the D-pad only
        }
    }
}
```
- `newKeys` is the main input for menus. About 99 functions read `0x03000046`, and 21 read `heldKeys`.
- Key bits are the standard GBA ones: A=1, B=2, Select=4, Start=8, Right=0x10, Left=0x20, Up=0x40, Down=0x80, R=0x100, L=0x200.

## Matching notes
- A literal pool sits in the middle of the function (`0x0807524E`: `0x04000130`, `0x03000040`) because an unconditional `b` jumps over it. This is normal agbcc output for an if/else whose first arm ends in a branch.
- The expression `keys & ~held` compiles to `bics r2, r0`.

Related: [[ram-map]], [[program-flow]].
