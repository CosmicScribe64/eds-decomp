#!/usr/bin/env python3
"""Find Thumb BL call sites targeting an address (stdlib only).
Usage: python3 tools/blrefs.py 0x0807A1A8 [0x...]"""
import sys, struct
ROM = open(__file__.rsplit('/tools/', 1)[0] + '/baserom.gba', 'rb').read()
TEXT_END = 0x80A24
def bl_targets():
    for off in range(0, TEXT_END - 2, 2):
        hi, lo = struct.unpack_from('<HH', ROM, off)
        if (hi & 0xF800) == 0xF000 and (lo & 0xF800) == 0xF800:
            o = ((hi & 0x7FF) << 12) | ((lo & 0x7FF) << 1)
            if o & 0x400000: o -= 0x800000
            yield 0x08000000 + off, 0x08000000 + off + 4 + o
if __name__ == '__main__':
    want = {int(a, 16) for a in sys.argv[1:]}
    for site, tgt in bl_targets():
        if tgt in want:
            print(f"{site:08x} -> {tgt:08x}")
