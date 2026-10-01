#!/usr/bin/env python3
"""Report where a built ROM differs from the baserom, mapped to the nearest symbol.
Usage: romdiff.py built.gba baserom.gba [map_file]"""
import re
import sys

a = open(sys.argv[1], 'rb').read()
b = open(sys.argv[2], 'rb').read()
syms = []
if len(sys.argv) > 3:
    for line in open(sys.argv[3]):
        m = re.match(r'^\s+0x0*(8[0-9a-f]{7})\s+([A-Za-z_.$][\w.$]*)\s*$', line)
        if m:
            syms.append((int(m.group(1), 16), m.group(2)))
    syms.sort()


def where(addr):
    best = None
    for s_addr, name in syms:
        if s_addr <= addr:
            best = (s_addr, name)
        else:
            break
    return f'{best[1]}+0x{addr - best[0]:X}' if best else '?'


if len(a) != len(b):
    print(f'size differs: built 0x{len(a):X} vs base 0x{len(b):X}')
diffs = [i for i in range(min(len(a), len(b))) if a[i] != b[i]]
print(f'{len(diffs)} differing bytes')
shown = 0
last = -100
for i in diffs:
    if i - last > 16:
        print(f'  0x{0x08000000 + i:08X} ({where(0x08000000 + i)}): built {a[i:i+8].hex()} base {b[i:i+8].hex()}')
        shown += 1
        if shown >= 20:
            break
    last = i
