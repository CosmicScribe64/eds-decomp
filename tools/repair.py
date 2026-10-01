#!/usr/bin/env python3
"""Park non-matching C functions of a unit back to INCLUDE_ASM until the unit matches again.

  tools/dr python3 tools/repair.py <unit>

For use after an agent was stopped mid-edit. Repeatedly runs check.py; the first function that
fails to compile, shows DIFF, or is MISSING gets its C definition wrapped in
`#if 0 /* NONMATCHING (auto-parked by tools/repair.py) */ ... #endif` followed by its INCLUDE_ASM line.
A copy of the original file is saved as build/repair/<unit>.c.orig.
"""
import os
import re
import shutil
import subprocess
import sys

unit = sys.argv[1]
src = f'src/{unit}.c'
os.makedirs('build/repair', exist_ok=True)
shutil.copy(src, f'build/repair/{unit}.c.orig')


def check():
    r = subprocess.run(['python3', 'tools/check.py', unit], capture_output=True, text=True)
    return r.stdout + r.stderr


def func_span(text, name):
    """(start, end) of the top-level definition of `name` (from the start of its line to after `}`)."""
    for m in re.finditer(r'^[^\n#;{}]*\b' + re.escape(name) + r'\s*\([^;{]*\)\s*\{', text, re.M):
        # must be at top level
        before = text[:m.start()]
        if before.count('{') - before.count('}') != 0:
            continue
        # skip if inside an #if 0 block
        if before.rfind('#if 0') > before.rfind('#endif'):
            continue
        i, depth = m.end(), 1
        while depth:
            depth += {'{': 1, '}': -1}.get(text[i], 0)
            i += 1
        return m.start(), i
    return None


def func_at_line(text, line):
    """Name of the top-level function containing 1-based line `line`."""
    lines = text.split('\n')
    off = sum(len(l) + 1 for l in lines[:line - 1])
    for m in re.finditer(r'^[^\n#;{}]*\b(sub_[0-9A-F]{8}|\w+)\s*\([^;{]*\)\s*\{', text, re.M):
        span = func_span(text, m.group(1))
        if span and span[0] <= off < span[1]:
            return m.group(1)
    return None


parked = []
for _ in range(40):
    out = check()
    if 'unit bytes MATCH' in out:
        print(f'{unit}: MATCH' + (f' (parked: {", ".join(parked)})' if parked else ''))
        sys.exit(0)
    text = open(src).read()
    name = None
    m = re.search(r'^src/' + re.escape(unit) + r'\.c:(\d+):', out, re.M)
    if 'compile failed' in out and m:
        name = func_at_line(text, int(m.group(1)))
    if not name:
        m = re.search(r'^\s+(\w+)\s+(DIFF|MISSING)', out, re.M)
        name = m and m.group(1)
    if not name or name in parked:
        print(out[-2000:])
        sys.exit(f'{unit}: cannot repair automatically (stuck on {name}); original in build/repair/{unit}.c.orig')
    span = func_span(text, name)
    if not span:
        print(out[-2000:])
        sys.exit(f'{unit}: no C definition found for {name}')
    s, e = span
    inc = f'INCLUDE_ASM("asm/nonmatching/{unit}", {name});'
    text = (text[:s] + '#if 0 /* NONMATCHING (auto-parked by tools/repair.py) */\n' + text[s:e] +
            '\n#endif\n' + inc + text[e:])
    open(src, 'w').write(text)
    parked.append(name)
sys.exit(f'{unit}: gave up after 40 rounds')
