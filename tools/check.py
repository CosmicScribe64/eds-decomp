#!/usr/bin/env python3
"""Check a C unit against the baserom without doing a full link.

  tools/dr python3 tools/check.py <unit>                 # per-function MATCH/DIFF summary
  tools/dr python3 tools/check.py <unit> --diff <func>   # instruction diff for one function
  tools/dr python3 tools/check.py <unit> --asm <func>    # just print the target disassembly

<unit> is a name from units.txt (e.g. code_08000228); its source is src/<unit>.c.
The unit is compiled with the same flags as the Makefile (via `make build/src/<unit>.o`),
linked alone at its ROM address with external symbols taken from build/eds.elf (when a full build
exists), config/symbols.txt, config/functions.tsv, symbols.ld or their _XXXXXXXX address suffix, and
compared. Without baserom.gba, the target bytes come from assembling the unit's original
assembly (tools/target.py). Safe to run concurrently for different units.
"""
import argparse
import difflib
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from target import ROM, known_symbols  # noqa: E402  (the real ROM, or assembled originals without it)

BASE = 0x08000000
AGBCC_DIR = os.environ.get('AGBCC_DIR', '/opt/agbcc')


def run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


def unit_range(unit):
    lo, hi = None, None
    funcs = []
    for line in open('config/functions.tsv'):
        if line.startswith('#'):
            continue
        f = line.rstrip('\n').split('\t')
        if f[4] != unit:
            continue
        a, size = int(f[0], 16), int(f[2], 16)
        funcs.append((a, size, f[3]))
        lo = a if lo is None else min(lo, a)
        hi = a + size if hi is None else max(hi, a + size)
    if lo is None:
        sys.exit(f'unknown unit {unit} (not in config/functions.tsv)')
    return lo, hi, funcs


def baseline_syms():
    """name -> (address, is_thumb_func): committed symbol tables, overridden by the last full build."""
    syms = known_symbols()
    if not os.path.exists('build/eds.elf'):
        return syms
    out = run(['arm-none-eabi-readelf', '-sW', 'build/eds.elf']).stdout
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 8 and p[0].endswith(':') and p[7] and p[3] in ('FUNC', 'NOTYPE', 'OBJECT'):
            v = int(p[1], 16)
            thumb = p[3] == 'FUNC' and v & 1
            syms[p[7]] = (v & ~1 if thumb else v, thumb)
    return syms


def objdump_bin(data, vma, lo, hi, mode):
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as f:
        f.write(data)
        path = f.name
    args = ['arm-none-eabi-objdump', '-D', '-b', 'binary', '-m', 'arm', f'--adjust-vma=0x{vma:X}',
            f'--start-address=0x{lo:X}', f'--stop-address=0x{hi:X}']
    if mode == 't':
        args += ['-M', 'force-thumb']
    out = run(args + [path]).stdout
    os.unlink(path)
    lines = []
    for line in out.splitlines():
        m = re.match(r'^\s*([0-9a-f]+):\s+([0-9a-f ]+?)\s{2,}(.*)$', line)
        if m:
            lines.append(f'{int(m.group(1), 16):08X}: {m.group(3).strip()}')
    return lines


def unit_cflags(unit):
    """(compiler binary, flags) for a unit, from config/cflags.txt or the Makefile defaults."""
    mk = open('Makefile').read()
    cc = re.search(r'^DEFAULT_CC1\s*:=\s*(\S+)', mk, re.M).group(1)
    flags = re.search(r'^DEFAULT_CFLAGS\s*:=\s*(.*)$', mk, re.M).group(1).split()
    for line in open('config/cflags.txt'):
        p = line.split('#')[0].split()
        if p and p[0] == unit:
            return p[1], p[2:]
    return cc, flags


def compile_unit(unit, src, tmp):
    """cpp -> agbcc -> as, same as the Makefile, into tmp. Returns the object path."""
    i, s, o = f'{tmp}/unit.i', f'{tmp}/unit.s', f'{tmp}/unit.o'
    steps = [
        ['cpp', '-nostdinc', '-undef', '-I', 'include', '-I', f'{AGBCC_DIR}/include', '-iquote', '.', src, '-o', i],
        [f'{AGBCC_DIR}/bin/{unit_cflags(unit)[0]}'] + unit_cflags(unit)[1] + [i, '-o', s],
    ]
    for cmd in steps:
        r = run(cmd)
        if r.returncode != 0 or r.stderr.strip():
            sys.stderr.write(r.stdout + r.stderr)
        if r.returncode != 0:
            sys.exit(f'compile failed: {cmd[0]}')
    with open(s, 'a') as f:
        f.write('\t.text\n\t.align 2, 0\n')
    r = run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-mthumb-interwork', '-I', '.', '-o', o, s])
    if r.returncode != 0:
        sys.exit('assemble failed:\n' + r.stderr)
    return o


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('unit')
    ap.add_argument('--diff', metavar='FUNC')
    ap.add_argument('--ctx', type=int, default=3, help='with --diff: context lines')
    ap.add_argument('--norm', action='store_true', help='with --diff: hide branch targets, pc offsets and comments')
    ap.add_argument('--asm', metavar='FUNC')
    ap.add_argument('--keep', action='store_true', help='keep temp dir')
    ap.add_argument('--src', metavar='FILE', help='compile FILE instead of src/<unit>.c (a private working copy)')
    a = ap.parse_args()

    lo, hi, funcs = unit_range(a.unit)
    fmode = {}
    for line in open('config/functions.tsv'):
        if not line.startswith('#'):
            f = line.split('\t')
            fmode[f[3]] = f[1]

    if a.asm:
        f = next((x for x in funcs if x[2] == a.asm), None)
        if not f:
            sys.exit(f'{a.asm} not in unit {a.unit}')
        print('\n'.join(objdump_bin(ROM[f[0] - BASE:f[0] - BASE + f[1]], f[0], f[0], f[0] + f[1], fmode.get(a.asm, 't'))))
        return

    src = a.src or f'src/{a.unit}.c'
    if not os.path.exists(src):
        sys.exit(f'{src} does not exist')
    tmp = tempfile.mkdtemp(prefix='check_')
    obj = compile_unit(a.unit, src, tmp)
    # external symbols
    nm = run(['arm-none-eabi-nm', obj]).stdout
    undef = [l.split()[-1] for l in nm.splitlines() if l.split()[0] == 'U']
    base = baseline_syms()
    lds = [f'SECTIONS {{ . = 0x{lo:08X}; .text : {{ {obj}(.text) }} .rodata : {{ {obj}(.rodata) }} '
           f'.data : {{ {obj}(.data) }} .bss 0x02000000 (NOLOAD) : {{ {obj}(.bss) {obj}(COMMON) }} }}']
    # externals: thumb functions via .thumb_set (so ld does not add interworking glue)
    sasm = ['\t.text']
    missing = []
    for s in undef:
        if s in base:
            addr, thumb = base[s]
        else:
            m = re.search(r'_([0-9A-Fa-f]{8})$', s)
            if not m:
                missing.append(s)
                continue
            addr = int(m.group(1), 16)
            thumb = s.startswith('sub_') and BASE <= addr < 0x08080A20
        sasm.append(f'\t.global {s}')
        sasm.append(f'\t.thumb_set {s}, 0x{addr:08X}' if thumb else f'\t.set {s}, 0x{addr:08X}')
    if missing:
        sys.exit(f'unresolved external symbols (not in build/eds.elf, no address suffix): {missing}')
    with open(f'{tmp}/link.ld', 'w') as f:
        f.write('\n'.join(lds) + '\n')
    with open(f'{tmp}/syms.s', 'w') as f:
        f.write('\n'.join(sasm) + '\n')
    r = run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-o', f'{tmp}/syms.o', f'{tmp}/syms.s'])
    if r.returncode != 0:
        sys.exit('symbol stub assembly failed:\n' + r.stderr)
    elf = f'{tmp}/unit.elf'
    r = run(['arm-none-eabi-ld', '-T', f'{tmp}/link.ld', '-o', elf, obj, f'{tmp}/syms.o'])
    if r.returncode != 0:
        sys.exit('link failed:\n' + r.stderr)
    r = run(['arm-none-eabi-objcopy', '-O', 'binary', '-j', '.text', elf, f'{tmp}/text.bin'])
    built = open(f'{tmp}/text.bin', 'rb').read()
    # section sizes (rodata/data/bss should be empty for now)
    hdr = run(['arm-none-eabi-objdump', '-h', obj]).stdout
    for m in re.finditer(r'^\s*\d+\s+(\.\S+)\s+([0-9a-f]{8})', hdr, re.M):
        if m.group(1) in ('.rodata', '.data', '.bss') and int(m.group(2), 16):
            print(f'warning: unit has {m.group(1)} of 0x{int(m.group(2), 16):X} bytes '
                  f'(needs placement in the rodata/RAM layout; `make compare` will fail)')

    target = ROM[lo - BASE:hi - BASE]
    # function symbols in the built object
    syms = {}
    out = run(['arm-none-eabi-nm', '-S', elf]).stdout
    for line in out.splitlines():
        p = line.split()
        if len(p) == 4 and p[2] in 'Tt':
            syms[p[3]] = (int(p[0], 16), int(p[1], 16))

    if a.diff:
        f = next((x for x in funcs if x[2] == a.diff), None)
        if not f:
            sys.exit(f'{a.diff} not in unit {a.unit}')
        fa, fsize, name = f
        bstart, bsize = syms.get(name, (fa, fsize))
        mode = fmode.get(name, 't')
        want = objdump_bin(ROM[fa - BASE:fa - BASE + fsize], fa, fa, fa + fsize, mode)
        have = objdump_bin(built[bstart - lo:bstart - lo + max(bsize, fsize)], bstart, bstart,
                           bstart + max(bsize, fsize), mode)
        strip = lambda ls: [re.sub(r'^[0-9A-F]+: ', '', x) for x in ls]
        if a.norm:
            norm = lambda x: re.sub(r'\s*@.*$', '', re.sub(r'\[pc, #\d+\]', '[pc]', re.sub(r'^(b\S*|bl|blx)\s+0x[0-9a-f]+.*$', r'\1', x)))
            strip0 = strip
            strip = lambda ls: [norm(x) for x in strip0(ls)]
        sw, sh = strip(want), strip(have)
        d = list(difflib.unified_diff(sw, sh, 'target', 'built', lineterm='', n=a.ctx))
        print('\n'.join(d) if d else f'{name}: MATCH')
        if d:
            # Progress signal for big functions: where the first difference is and how many lines differ.
            k = next((i for i, (x, y) in enumerate(zip(sw, sh)) if x != y), min(len(sw), len(sh)))
            off = int(want[k].split(':')[0], 16) - fa if k < len(want) else fsize
            changed = sum(1 for x in d if x[:1] in '+-' and x[:3] not in ('---', '+++'))
            print(f'summary: first difference at +0x{off:X} of 0x{fsize:X} '
                  f'({100 * off / fsize:.1f}% matching prefix), {changed} differing lines')
        if bstart != fa:
            print(f'note: built {name} starts at 0x{bstart:08X}, target 0x{fa:08X} (earlier code differs in size)')
        if name in syms and bsize != fsize:
            print(f'size: built 0x{bsize:X} vs target 0x{fsize:X} ({bsize - fsize:+d} bytes)')
        elif name not in syms:
            print(f'note: {name} is not defined by the build')
        return

    ok = 0
    for fa, fsize, name in funcs:
        if name not in syms:
            print(f'  {name:32s} MISSING from build')
            continue
        bstart, bsize = syms[name]
        want = ROM[fa - BASE:fa - BASE + fsize]
        have = built[bstart - lo:bstart - lo + fsize]
        if bstart == fa and have == want:
            ok += 1
            status = 'match'
        else:
            first = next((i for i in range(min(len(have), len(want))) if have[i] != want[i]), min(len(have), len(want)))
            status = f'DIFF (starts 0x{bstart:08X}, first diff +0x{first:X})'
        print(f'  {name:32s} {status}')
    whole = built == target
    print(f'{a.unit}: {ok}/{len(funcs)} functions match; unit bytes {"MATCH" if whole else "DIFFER"} '
          f'(built 0x{len(built):X} vs target 0x{len(target):X})')
    if a.keep:
        print(f'kept {tmp} (unit.s = agbcc output)')
    else:
        subprocess.run(['rm', '-rf', tmp])
    sys.exit(0 if whole else 1)


if __name__ == '__main__':
    main()
