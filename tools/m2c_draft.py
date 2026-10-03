#!/usr/bin/env python3
"""First-draft C for not-yet-decompiled functions, using the m2c decompiler (ARM target).

  tools/dr python3 tools/m2c_draft.py <unit> [func ...]   # drafts for those functions (default: all
                                                          # INCLUDE_ASM functions of the unit that have no draft yet)
  tools/dr python3 tools/m2c_draft.py --all               # every function with no draft, in every unit

Output: build/m2c/<unit>/<func>.c (src/ is never touched). The draft is only a starting point and usually
does not compile as-is, because m2c writes `?` for unknown types and `unkXX` for unknown struct fields. Refine
it by hand, then check it with tools/check.py.

Preparation of the asm (in a temp copy):
  - pool words pointing at a label in the same function (agbcc's Thumb switch tables) become that label,
    which is how m2c finds jump tables;
  - divided-syntax `ldsh`/`ldsb` are renamed to `ldrsh`/`ldrsb` (m2c does not know the old spellings);
  - literal-pool words holding RAM / ROM-data addresses are replaced by symbol names (a real name from
    build/eds.elf when there is one, else gUnk_XXXXXXXX), so the draft reads `gAiState.unkA` instead of
    `((void *)0x02015EF0)->unkA`.
The unit's own C (types, structs, prototypes) is passed to m2c as context when it parses; otherwise the
draft is made without context.
"""
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(__file__))
from check import baseline_syms  # noqa: E402
from permute import strip_other_fns  # noqa: E402

M2C = ['python3', '/opt/m2c/m2c.py', '-t', 'arm-gcc-c']


def functions():
    out = {}
    for line in open('config/functions.tsv'):
        if line.startswith('#'):
            continue
        f = line.rstrip('\n').split('\t')
        out.setdefault(f[4], []).append(f[3])
    return out


def undrafted(unit, funcs):
    src = f'src/{unit}.c'
    if not os.path.exists(src):
        return funcs
    lines = open(src).read().split('\n')
    out = []
    for i, l in enumerate(lines):
        m = re.match(r'^\s*INCLUDE_ASM\([^,]*,\s*(\w+)\)', l)
        if m and not (i and lines[i - 1].strip() == '#endif'):
            out.append(m.group(1))
    return out


_names = None


def addr_names():
    global _names
    if _names is None:
        _names = {}
        for name, (addr, thumb) in baseline_syms().items():
            if name.startswith(('gUnk_', 'sub_', '_', '$', '.')) or thumb:
                continue
            _names.setdefault(addr, name)
    return _names


def prep_asm(path):
    t = open(path).read()
    t = re.sub(r'\bldsh\b', 'ldrsh', t)
    t = re.sub(r'\bldsb\b', 'ldrsb', t)

    labels = set(re.findall(r'^(_[0-9A-F]{8}):', t, re.M))

    def lit(m):
        a = int(m.group(2), 16)
        if f'_{a:08X}' in labels:  # pointer to a local label (a jump table): m2c needs the symbol
            return m.group(1) + f'_{a:08X}'
        if 0x02000000 <= a < 0x04000000 or 0x08080A20 <= a < 0x08800000:
            return m.group(1) + addr_names().get(a, f'gUnk_{a:08X}')
        return m.group(0)
    return re.sub(r'(\.4byte )0x([0-9A-Fa-f]{8})\b', lit, t)


def context(unit, tmp):
    src = f'src/{unit}.c'
    ctx = f'{tmp}/ctx.c'
    inp = src if os.path.exists(src) else 'include/global.h'
    r = subprocess.run(['cpp', '-P', '-nostdinc', '-undef', '-I', 'include', '-I', os.environ.get('AGBCC_DIR', '/opt/agbcc') + '/include',
                        '-iquote', '.', inp], capture_output=True, text=True)
    if r.returncode:
        return None
    s = strip_other_fns(r.stdout, '__none__')
    s = re.sub(r'^\s*asm\s*\(.*\)\s*;\s*$', '', s, flags=re.M)
    open(ctx, 'w').write(s)
    return ctx


def draft(unit, func, ctx, tmp):
    asm = f'asm/nonmatching/{unit}/{func}.s'
    if not os.path.exists(asm):
        return f'/* no asm file {asm} */\n'
    a = f'{tmp}/{func}.s'
    open(a, 'w').write(prep_asm(asm))
    for c in ([ctx] if ctx else []) + [None]:
        cmd = M2C + (['--context', c] if c else []) + [a]
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
        if r.returncode == 0 and r.stdout.strip():
            note = '' if c else ' (made without the unit context: it did not parse)'
            return f'/* m2c draft of {func} ({unit}){note}. Starting point only; refine, then tools/check.py. */\n' + r.stdout
    return f'/* m2c failed on {func}:\n{r.stderr[-1500:]}\n*/\n'


def main():
    args = sys.argv[1:]
    fmap = functions()
    if args == ['--all']:
        jobs = [(u, undrafted(u, fs)) for u, fs in sorted(fmap.items()) if u.startswith('code_')]
    else:
        unit = args[0]
        jobs = [(unit, args[1:] or undrafted(unit, fmap.get(unit, [])))]
    n = 0
    for unit, funcs in jobs:
        if not funcs:
            continue
        os.makedirs(f'build/m2c/{unit}', exist_ok=True)
        with tempfile.TemporaryDirectory() as tmp:
            ctx = context(unit, tmp)
            for func in funcs:
                open(f'build/m2c/{unit}/{func}.c', 'w').write(draft(unit, func, ctx, tmp))
                n += 1
    print(f'wrote {n} drafts to build/m2c/')


if __name__ == '__main__':
    main()
