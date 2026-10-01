#!/usr/bin/env python3
"""Draft a canonical struct for a shared global from how all units declare it (tools/structmap.py).

  python3 tools/mkheader.py --name Main --base 0x03000040 --size 0x4A00 [--view 0xADDR[:elem][@lo-hi]] ...
  python3 tools/mkheader.py --name DuelPlayer --base 0x020192E4 --size 0xD64 --elem \\
        --view 0x020192E0 --view 0x0201930C:elem --nest zones:DuelZone:11:0x28:0x94

Every view's field offsets are translated to offsets inside one struct of --size bytes (starting at --base; with
--elem, modulo --size for array globals). At each offset the most-used meaningful name wins (generic names such as
b5, w0, unk*, filler*, rest are ignored); the alternatives are listed in a comment. Gaps become `u8 unk_XXX[n]`.
--nest NAME:TYPE:COUNT:OFFSET:STRIDE emits `struct TYPE NAME[COUNT]` at OFFSET and drops the fields inside it.
The draft is re-parsed and its layout checked against the intended offsets.
"""
import argparse
import collections
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(__file__))
import structmap  # noqa: E402

GENERIC = re.compile(r'^(unk|pad|filler|skip|rest|u\d|b\d|w\d|h\d|f\d|s\d|p\d|x\d|count\d|flags?\d|field|_|$)', re.I)
BITS = {1: 'u8', 2: 'u16', 4: 'u32'}


def collect(base, size, views, elem):
    """views: [(addr, elem, lo, hi)]: only offsets lo <= off < hi of that view (in its own coordinates) count."""
    recs = collections.defaultdict(lambda: collections.defaultdict(set))
    for f in sorted(glob.glob('src/code_*.c')):
        unit = f[4:-2]
        for vaddr, velem, vlo, vhi in views:
            try:
                _, leaves = structmap.unit_fields(f, vaddr, velem)
            except Exception:  # noqa: BLE001
                continue
            for path, bit, w, fsize, ty in leaves:
                if '[' in path.split('.')[-1] and not path.endswith(']'):
                    pass
                name = re.sub(r'\[0\]', '', path.split('.')[-1])
                if GENERIC.match(name) or ty.startswith('struct') or ty.startswith('union'):
                    continue
                if not vlo <= bit // 8 < vhi:
                    continue
                off = vaddr - base + bit // 8
                if elem:
                    off %= size
                if not 0 <= off < size:
                    continue
                key = (off, bit % 8 if w is not None else None, w, fsize)
                recs[key][(name, ty)].add(unit)
    return recs


def choose(recs, nests):
    covered = [(o, o + c * st) for _, _, c, o, st in nests]
    cands = []
    for (off, bit, w, fsize), names in recs.items():
        if any(a <= off < b for a, b in covered):
            continue
        best = sorted(names.items(), key=lambda kv: (-len(kv[1]), kv[0][0]))
        n_units = len(set().union(*names.values()))
        cands.append((n_units, off, bit, w, fsize, best))
    # greedy: most-used first; skip anything overlapping an accepted field
    taken, out = [], []
    for n_units, off, bit, w, fsize, best in sorted(cands, key=lambda c: (-c[0], c[4], c[1])):
        lo = off * 8 + (bit or 0)
        hi = lo + (w if w is not None else fsize * 8)
        if any(lo < b and a < hi for a, b in taken):
            continue
        taken.append((lo, hi))
        out.append((lo, hi, n_units, off, bit, w, fsize, best))
    return sorted(out)


def ctype(ty):
    t = ty.replace('unsigned char', 'u8').replace('unsigned short', 'u16').replace('unsigned int', 'u32')
    t = t.replace('FuncDecl *', 'void *')
    return t


def render(name, size, fields, nests):
    lines = [f'struct {name} {{']
    items = [(x[0], x[1], 'field', x) for x in fields]
    items += [(o * 8, (o + c * st) * 8, 'nest', (nm, ty, c)) for nm, ty, c, o, st in nests]
    items.sort()
    pos = 0
    used = collections.Counter()
    for lo, hi, kind, data in items:
        if lo // 8 > (pos + 7) // 8 and (lo % 8 == 0 or kind == 'nest'):
            start = (pos + 7) // 8
            lines.append(f'    u8 unk_{start:X}[0x{lo // 8 - start:X}];')
            pos = start * 8
        if kind == 'nest':
            nm, ty, c = data
            lines.append(f'    struct {ty} {nm}[{c}];' + f'  /* +0x{lo // 8:X} */')
            pos = hi
            continue
        _, _, n_units, off, bit, w, fsize, best = data
        (fname, fty), units = best[0]
        used[fname] += 1
        if used[fname] > 1:
            fname = f'{fname}_{off:X}'
        alts = ', '.join(f'{n} x{len(u)}' for (n, _), u in best[1:4])
        comment = f'/* +0x{off:X}' + (f' bits {bit}..{bit + w - 1}' if w is not None else '') + \
                  f', {n_units} unit(s)' + (f'; also: {alts}' if alts else '') + ' */'
        if w is not None:
            base_t = ctype(fty).split('[')[0].strip() or 'u8'
            want = off * 8 + bit
            if want > pos:
                gap = want - pos
                lines.append(f'    {base_t} :{gap};')
            lines.append(f'    {base_t} {fname}:{w};  {comment}')
            pos = want + w
        else:
            t = ctype(fty)
            m = re.match(r'^(.*?)((?:\[[^\]]*\])+)$', t)
            decl = f'{m.group(1).strip()} {fname}{m.group(2)}' if m else f'{t} {fname}'
            decl = decl.replace('[...]', f'[0x{fsize:X} / sizeof(u8)]')
            lines.append(f'    {decl};  {comment}')
            pos = (off + fsize) * 8
    end = (pos + 7) // 8
    if end < size:
        lines.append(f'    u8 unk_{end:X}[0x{size - end:X}];')
    lines.append(f'}}; /* size 0x{size:X} */')
    return '\n'.join(lines)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--name', required=True)
    ap.add_argument('--base', required=True, type=lambda s: int(s, 0))
    ap.add_argument('--size', required=True, type=lambda s: int(s, 0))
    ap.add_argument('--elem', action='store_true')
    ap.add_argument('--view', action='append', default=[])
    ap.add_argument('--nest', action='append', default=[])
    a = ap.parse_args()
    def view(v):
        rng = (0, 1 << 30)
        if '@' in v:
            v, r = v.split('@')
            rng = tuple(int(x, 0) for x in r.split('-'))
        return (int(v.split(':')[0], 0), v.endswith(':elem')) + rng
    views = [(a.base, a.elem, 0, a.size if not a.elem else 1 << 30)] + [view(v) for v in a.view]
    nests = []
    for n in a.nest:
        nm, ty, c, o, st = n.split(':')
        nests.append((nm, ty, int(c, 0), int(o, 0), int(st, 0)))
    recs = collect(a.base, a.size, views, a.elem)
    fields = choose(recs, nests)
    print(render(a.name, a.size, fields, nests))


if __name__ == '__main__':
    main()
