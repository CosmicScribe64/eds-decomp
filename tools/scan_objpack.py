#!/usr/bin/env python3
"""Scan the ROM for EDS "image packs" (palette + tiles + sparse BG map; stdlib only).

Format (reverse-engineered from the loaders sub_08072EB0 / sub_08072FAC,
see wiki/data/graphics-formats.md):
  u16 nColors  x4 (the value is stored four times)
  u16 palette[nColors]                 BGR555 (bit 15 often set, ignored)
  u16 nTiles   x4
  u8  tiles[nTiles*64 or nTiles*32]    8bpp (or 4bpp variant) 8x8 tiles, index 0 = transparent
  u16 nCells   x4
  struct {u16 pos; u16 tile;} cell[nCells]   pos = x | y<<8 on a 32-wide BG map
NOTE: palettes in this ROM often have bit 15 set, so no palette filter is applied;
expect a few false positives inside the 1bpp font bank (0x081C0000-0x0822C300).
Usage: python3 tools/scan_objpack.py [start end]
"""
import sys, struct
ROM = open(__file__.rsplit('/tools/', 1)[0] + '/baserom.gba', 'rb').read()

def rep4(off):
    if off + 8 > len(ROM): return None
    v = struct.unpack_from('<4H', ROM, off)
    return v[0] if v[0] == v[1] == v[2] == v[3] else None

def parse(off, tilesize):
    nc = rep4(off)
    if not nc or nc < 2 or nc > 256: return None
    p = off + 8 + nc * 2
    nt = rep4(p)
    if not nt or nt > 1024: return None
    p += 8 + nt * tilesize
    no = rep4(p)
    if no is None or no > 1024: return None
    p += 8 + no * 4
    return nc, nt, no, p - off

def main():
    s = int(sys.argv[1], 16) - 0x08000000 if len(sys.argv) > 1 else 0x80A24
    e = int(sys.argv[2], 16) - 0x08000000 if len(sys.argv) > 2 else len(ROM)
    off = s & ~3; n = 0
    while off < e:
        r = parse(off, 64) or parse(off, 32)
        if r:
            nc, nt, no, size = r
            bpp = 8 if parse(off, 64) else 4
            print(f"{0x08000000+off:08x} colors={nc:3d} {bpp}bpp tiles={nt:4d} cells={no:3d} size={size:#x}")
            n += 1; off += (size + 3) & ~3
        else:
            off += 4
    print(n, 'packs', file=sys.stderr)

if __name__ == '__main__':
    main()
