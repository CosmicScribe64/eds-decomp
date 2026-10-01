#!/usr/bin/env python3
"""Create src/<unit>.c as an all-INCLUDE_ASM skeleton for an asm unit.
Usage: tools/dr python3 tools/mkunit.py <unit>
Then replace INCLUDE_ASM lines with C one function at a time, checking with tools/check.py."""
import os
import sys

unit = sys.argv[1]
path = f'src/{unit}.c'
if os.path.exists(path):
    sys.exit(f'{path} already exists')
funcs = []
for line in open('config/functions.tsv'):
    if line.startswith('#'):
        continue
    f = line.split('\t')
    if f[4] == unit:
        funcs.append((f[0], f[3], f[2]))
if not funcs:
    sys.exit(f'unknown unit {unit}')
os.makedirs(os.path.dirname(path), exist_ok=True)
with open(path, 'w') as fh:
    fh.write('#include "global.h"\n\n')
    for addr, name, size in funcs:
        fh.write(f'INCLUDE_ASM("asm/nonmatching/{unit}", {name}); /* {addr} size {size} */\n')
print(f'wrote {path} ({len(funcs)} functions)')
