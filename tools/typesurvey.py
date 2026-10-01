#!/usr/bin/env python3
"""Survey how the C units declare shared globals and structs (input for consolidating headers).

  python3 tools/typesurvey.py globals [--min 3]    # each global address: how many units use it, with which types
  python3 tools/typesurvey.py structs [--min 2]    # struct tags defined in several units, and whether they agree
  python3 tools/typesurvey.py macros               # address-cast #defines (e.g. CARD_STATS) across units
"""
import collections
import glob
import re
import sys

FILES = sorted(glob.glob('src/code_*.c'))


def strip_comments(t):
    t = re.sub(r'/\*.*?\*/', ' ', t, flags=re.S)
    return re.sub(r'//[^\n]*', ' ', t)


def active(t):
    """Drop #if 0 ... #endif blocks (drafts)."""
    return re.sub(r'^#if 0\b.*?^#endif[^\n]*$', '', t, flags=re.S | re.M)


def globals_(minimum):
    uses = collections.defaultdict(lambda: collections.defaultdict(list))
    for f in FILES:
        t = strip_comments(active(open(f, errors='replace').read()))
        unit = f[4:-2]
        for m in re.finditer(r'^\s*extern\s+([^;()]*?)\b((?:gUnk|g[A-Z]\w*?)_?([0-9A-F]{8})?)\s*(\[[^\]]*\])?\s*;', t, re.M):
            typ = ' '.join(m.group(1).split()) + (' ' + m.group(4) if m.group(4) else '')
            uses[m.group(2)][typ].append(unit)
    rows = sorted(uses.items(), key=lambda kv: -sum(len(v) for v in kv[1].values()))
    for name, types in rows:
        n = sum(len(v) for v in types.values())
        if n < minimum:
            continue
        print(f'{name}: {n} units, {len(types)} distinct type(s)')
        for typ, units in sorted(types.items(), key=lambda kv: -len(kv[1])):
            print(f'    {len(units):3d}  {typ}   e.g. {", ".join(units[:3])}')


def structs(minimum):
    defs = collections.defaultdict(lambda: collections.defaultdict(list))
    for f in FILES:
        t = strip_comments(active(open(f, errors='replace').read()))
        for m in re.finditer(r'\bstruct\s+(\w+)\s*\{', t):
            i, d = m.end(), 1
            while d and i < len(t):
                d += {'{': 1, '}': -1}.get(t[i], 0)
                i += 1
            body = ' '.join(t[m.end():i - 1].split())
            defs[m.group(1)][body].append(f[4:-2])
    for tag, bodies in sorted(defs.items(), key=lambda kv: -sum(len(v) for v in kv[1].values())):
        n = sum(len(v) for v in bodies.values())
        if n < minimum:
            continue
        print(f'struct {tag}: defined in {n} units, {len(bodies)} distinct layout(s)')
        for body, units in sorted(bodies.items(), key=lambda kv: -len(kv[1])):
            print(f'    {len(units):3d}  {{ {body[:160]}{"..." if len(body) > 160 else ""} }}   e.g. {", ".join(units[:3])}')


def macros():
    uses = collections.defaultdict(lambda: collections.defaultdict(list))
    for f in FILES:
        for m in re.finditer(r'^#define\s+(\w+)(\([^)]*\))?\s+(.*0x0[2389][0-9A-Fa-f]{6}.*)$', open(f, errors='replace').read(), re.M):
            addr = re.search(r'0x0[2389][0-9A-Fa-f]{6}', m.group(3)).group(0).upper().replace('0X', '0x')
            uses[addr][f'{m.group(1)}{m.group(2) or ""} = {" ".join(m.group(3).split())[:110]}'].append(f[4:-2])
    for addr, defs in sorted(uses.items(), key=lambda kv: -sum(len(v) for v in kv[1].values())):
        n = sum(len(v) for v in defs.values())
        if n < 2:
            continue
        print(f'{addr}: {n} units')
        for d, units in sorted(defs.items(), key=lambda kv: -len(kv[1]))[:6]:
            print(f'    {len(units):3d}  {d}')


if __name__ == '__main__':
    what = sys.argv[1] if len(sys.argv) > 1 else 'globals'
    mn = int(sys.argv[3]) if len(sys.argv) > 3 and sys.argv[2] == '--min' else (3 if what == 'globals' else 2)
    {'globals': lambda: globals_(mn), 'structs': lambda: structs(mn), 'macros': macros}[what]()
