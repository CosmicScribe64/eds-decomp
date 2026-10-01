#!/usr/bin/env python3
"""Decompilation progress: functions/bytes in C vs still in asm.
Counts a function as decompiled when its unit has src/<unit>.c and the function is not
pulled in with INCLUDE_ASM. Hand-written asm sources (src/*.s) count as done.
Usage: python3 tools/progress.py [--units]"""
import os
import re
import sys

funcs = []
for line in open('config/functions.tsv'):
    if line.startswith('#'):
        continue
    f = line.rstrip('\n').split('\t')
    funcs.append((int(f[0], 16), int(f[2], 16), f[3], f[4]))

inc_cache = {}


def included(unit):
    if unit not in inc_cache:
        path = f'src/{unit}.c'
        if os.path.exists(path):
            inc_cache[unit] = set(re.findall(r'INCLUDE_ASM\([^,]+,\s*(\w+)\)', open(path).read()))
        elif os.path.exists(f'src/{unit}.s'):
            inc_cache[unit] = set()
        else:
            inc_cache[unit] = None
    return inc_cache[unit]


per_unit = {}
tot_n = tot_b = done_n = done_b = 0
for addr, size, name, unit in funcs:
    inc = included(unit)
    done = inc is not None and name not in inc
    u = per_unit.setdefault(unit, [0, 0, 0, 0])
    u[0] += 1
    u[1] += size
    tot_n += 1
    tot_b += size
    if done:
        u[2] += 1
        u[3] += size
        done_n += 1
        done_b += size
if '--units' in sys.argv:
    for unit, (n, b, dn, db) in per_unit.items():
        if dn:
            print(f'{unit:24s} {dn:3d}/{n:3d} funcs  {100 * db / b:5.1f}% bytes')
print(f'Decompiled: {done_n}/{tot_n} functions ({100 * done_n / tot_n:.2f}%), '
      f'{done_b:#x}/{tot_b:#x} bytes ({100 * done_b / tot_b:.2f}%) of game+SDK code')
