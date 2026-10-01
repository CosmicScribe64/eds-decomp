#!/usr/bin/env python3
"""Find already-matched functions that look like an unmatched one (siblings / near-copies).

  python3 tools/similar.py <func> [-n 5]      # closest matched functions to <func>
  python3 tools/similar.py --all              # write build/similar.tsv for every unmatched function
                                              # (and a "closest matched siblings" note into build/m2c drafts)

Similarity = difflib ratio over the functions' instruction sequences, with registers, immediates and labels
normalised away (so `add r4, r5, #0x10` and `add r1, r2, #0x3C` look the same). Only functions of similar size
are compared. A matched sibling's C (shown as src/<unit>.c:<line>) is usually a much better starting point than
a decompiler draft.
"""
import difflib
import glob
import os
import re
import sys

NORM = [
    (re.compile(r'\b(r\d+|sb|sl|fp|ip|sp|lr|pc)\b'), 'R'),
    (re.compile(r'#-?(0x[0-9A-Fa-f]+|\d+)'), '#N'),
    (re.compile(r'\b_[0-9A-F]{8}\b'), 'L'),
    (re.compile(r'\b(sub|gUnk)_[0-9A-F]{8}\b'), 'S'),
    (re.compile(r'0x[0-9A-Fa-f]+'), 'N'),
    (re.compile(r'@.*$'), ''),
]


def tokens(path):
    out = []
    for line in open(path):
        line = line.strip()
        if not line or line.endswith(':') or line.startswith(('thumb_func', 'arm_func', 'non_word', '.align', '.syntax')):
            continue
        line = re.sub(r'^_[0-9A-F]{8}:\s*', '', line)
        for rx, rep in NORM:
            line = rx.sub(rep, line)
        out.append(' '.join(line.split()))
    return out


def load():
    info = {}
    for line in open('config/functions.tsv'):
        if line.startswith('#'):
            continue
        f = line.rstrip('\n').split('\t')
        info[f[3]] = (f[4], int(f[2], 16))
    asm_left = set()
    where = {}
    for p in glob.glob('src/*.c') + glob.glob('src/*/*.c'):
        lines = open(p, errors='replace').read().split('\n')
        for i, l in enumerate(lines):
            m = re.match(r'^\s*INCLUDE_ASM\([^,]*,\s*(\w+)\)', l)
            if m:
                asm_left.add(m.group(1))
                continue
            m = re.match(r'^[A-Za-z_][\w \*]*\b(sub_[0-9A-F]{8})\s*\(', l)
            if m and not l.rstrip().endswith(';'):
                where[m.group(1)] = f'{p}:{i + 1}'
    units_with_c = {os.path.splitext(os.path.relpath(p, 'src'))[0]
                    for p in glob.glob('src/*.c') + glob.glob('src/*/*.c')}
    matched, unmatched = {}, {}
    for name, (unit, size) in info.items():
        asm = f'asm/nonmatching/{unit}/{name}.s'
        if not os.path.exists(asm):
            continue
        if unit in units_with_c and name not in asm_left and name in where:
            matched[name] = (unit, size, asm)
        elif name in asm_left or unit not in units_with_c:
            unmatched[name] = (unit, size, asm)
    return matched, unmatched, where


def closest(name, matched, unmatched, toks, n=5):
    unit, size, asm = unmatched[name]
    a = toks.setdefault(name, tokens(asm))
    res = []
    for m, (mu, msize, masm) in matched.items():
        if not (0.6 * size <= msize <= 1.6 * size):
            continue
        b = toks.setdefault(m, tokens(masm))
        sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
        if sm.real_quick_ratio() < 0.6 or sm.quick_ratio() < 0.6:
            continue
        r = sm.ratio()
        if r >= 0.6:
            res.append((r, m))
    res.sort(reverse=True)
    return res[:n]


def main():
    matched, unmatched, where = load()
    toks = {}
    if len(sys.argv) >= 2 and sys.argv[1] != '--all':
        name = sys.argv[1]
        n = int(sys.argv[3]) if len(sys.argv) > 3 and sys.argv[2] == '-n' else 5
        if name not in unmatched:
            sys.exit(f'{name} is not an unmatched function')
        for r, m in closest(name, matched, unmatched, toks, n):
            print(f'{r:.0%}  {m}  ({matched[m][0]}, 0x{matched[m][1]:X} bytes)  {where[m]}')
        return
    rows = []
    for name in sorted(unmatched):
        best = closest(name, matched, unmatched, toks, 3)
        rows.append((name, unmatched[name][0], best))
        draft = f'build/m2c/{unmatched[name][0]}/{name}.c'
        if best and os.path.exists(draft):
            t = open(draft).read()
            t = re.sub(r'^/\* closest matched siblings:.*?\*/\n', '', t, flags=re.S)
            note = '/* closest matched siblings (tools/similar.py): ' + '; '.join(
                f'{m} {r:.0%} at {where[m]}' for r, m in best) + ' */\n'
            open(draft, 'w').write(note + t)
    os.makedirs('build', exist_ok=True)
    with open('build/similar.tsv', 'w') as f:
        for name, unit, best in rows:
            f.write('\t'.join([name, unit] + [f'{m}:{r:.2f}' for r, m in best]) + '\n')
    hits = [r for r in rows if r[2] and r[2][0][0] >= 0.8]
    print(f'{len(rows)} unmatched functions; {sum(1 for r in rows if r[2])} have a matched sibling >= 60%, '
          f'{len(hits)} >= 80%')


if __name__ == '__main__':
    main()
