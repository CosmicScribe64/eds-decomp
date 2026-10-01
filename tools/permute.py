#!/usr/bin/env python3
"""Set up a decomp-permuter job for one near-miss function.

  tools/dr python3 tools/permute.py <unit> <func>              # set up build/permuter/<func>/
  tools/dr python3 tools/permute.py <unit> <func> --from F.c   # use the function text in F.c instead of the draft
  tools/dr python3 tools/permute.py <unit> <func> --run [-j N] [--minutes M]

The C comes from the `#if 0 /* NONMATCHING ... */` draft directly above the function's
INCLUDE_ASM line in src/<unit>.c (or from --from). The rest of the unit stays as context, and other
functions become prototypes (static inline helpers are kept so they still inline).

Both the target (asm/nonmatching/<unit>/<func>.s) and every candidate are linked alone at the
function's ROM address with the same external symbols, so the permuter compares final bytes.
Results land in build/permuter/<func>/output-<score>-<n>/source.c; score 0 = match.
"""
import argparse
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(__file__))
from check import baseline_syms, unit_cflags, unit_range, run  # noqa: E402
from match_drafts import BLOCK, draft_body  # noqa: E402

BASE = 0x08000000
AGBCC_DIR = os.environ.get('AGBCC_DIR', '/opt/agbcc')

# decomp-permuter weights for register-allocation near-misses (default weights: /opt/permuter/default_weights.toml)
REGALLOC_WEIGHTS = {
    'perm_temp_for_expr': 80, 'perm_reorder_decls': 60, 'perm_reorder_stmts': 40, 'perm_commutative': 40,
    'perm_randomize_internal_type': 30, 'perm_chain_assignment': 25, 'perm_refer_to_var': 25,
    'perm_split_assignment': 25, 'perm_remove_var': 20, 'perm_struct_ref': 15, 'perm_expand_expr': 10,
    'perm_duplicate_assignment': 10, 'perm_compound_assignment': 10, 'perm_cast_simple': 10,
    'perm_long_chain_assignment': 5, 'perm_add_sub': 5,
    'perm_add_mask': 1, 'perm_xor_zero': 0.5, 'perm_float_literal': 0.1, 'perm_sameline': 0.1,
    'perm_empty_stmt': 1, 'perm_condition': 3, 'perm_mult_zero': 0.5, 'perm_dummy_comma_expr': 0.5,
    'perm_ins_block': 3, 'perm_inline': 2, 'perm_var_cond_block': 2, 'perm_factor_mult': 1, 'perm_factor_shift': 1,
    'perm_inequalities': 2, 'perm_remove_ast': 1, 'perm_pad_var_decl': 0.5, 'perm_alias_array': 0.5,
    'perm_randomize_external_type': 1, 'perm_randomize_function_type': 0.1, 'perm_add_self_assignment': 1,
}


def draft_source(unit, func, from_file):
    source = open(f'src/{unit}.c').read()
    lines = source.split('\n')
    inc = re.compile(r'^\s*INCLUDE_ASM\(\s*"[^"]*"\s*,\s*' + re.escape(func) + r'\s*\)')
    idx = next((i for i, l in enumerate(lines) if inc.match(l)), None)
    if idx is None:
        sys.exit(f'{func}: no INCLUDE_ASM line in src/{unit}.c (already matched?)')
    block = next((m for m in BLOCK.finditer(source) if m['name'] == func), None)
    if block is not None:
        body = open(from_file).read() if from_file else draft_body(block)
        source = source[:block.start()] + body.rstrip() + '\n' + source[block.end():]
        new = source.split('\n')
    elif from_file:
        new = lines[:idx] + open(from_file).read().split('\n') + lines[idx + 1:]
    else:
        sys.exit(f'{func}: no `#if 0` draft directly above its INCLUDE_ASM line; use --from F.c')
    # other functions stay asm: drop their INCLUDE_ASM lines
    return '\n'.join(l for l in new if not re.match(r'^\s*INCLUDE_ASM\(', l)) + '\n'


def skip_c_literal_or_comment(src, i):
    """Return the first offset after a string/character/comment, or None."""
    if src.startswith('/*', i):
        end = src.find('*/', i + 2)
        return len(src) if end < 0 else end + 2
    if src.startswith('//', i):
        end = src.find('\n', i + 2)
        return len(src) if end < 0 else end
    if src[i] in ('"', "'"):
        quote = src[i]
        j = i + 1
        while j < len(src):
            if src[j] == '\\':
                j += 2
            elif src[j] == quote:
                return j + 1
            else:
                j += 1
        return len(src)
    return None


def strip_other_fns(src, keep):
    """Replace every top-level function body except `keep` (and inline helpers) with `;`."""
    kr_header = re.compile(
        r'(\w+)\s*\((\s*\w+(?:\s*,\s*\w+)*\s*)\)\s*'
        r'((?:[^{};]+;\s*)+)$'
    )
    out, i, n = [], 0, len(src)
    stmt_start = 0
    depth = 0
    while i < n:
        skipped = skip_c_literal_or_comment(src, i)
        if skipped is not None:
            i = skipped
            continue
        c = src[i]
        if c == '{':
            if depth == 0:
                head = re.sub(r'/\*.*?\*/|//[^\n]*', '', src[stmt_start:i], flags=re.S)
                m = re.search(r'(\w+)\s*\([^;{}]*\)\s*$', head)
                kr = kr_header.search(head) if not m else None
                if kr:
                    m = kr
                is_fn = m and '=' not in head.split('(')[0]
                end = i + 1
                d = 1
                while end < n and d:
                    skipped = skip_c_literal_or_comment(src, end)
                    if skipped is not None:
                        end = skipped
                        continue
                    if src[end] == '{':
                        d += 1
                    elif src[end] == '}':
                        d -= 1
                    end += 1
                if is_fn and m.group(1) != keep and 'inline' not in head:
                    # A K&R definition must become an unprototyped declaration.
                    # Keeping its parameter declarations leaves invalid C;
                    # supplying a typed prototype changes caller narrowing.
                    declaration = (head[:kr.start()] + kr.group(1) + '();\n'
                                   if kr else src[stmt_start:i].rstrip() + ';\n')
                    out.append((stmt_start, end, declaration))
                i = end
                stmt_start = end
                continue
            depth += 1
        elif c == '}':
            depth -= 1
        elif c == ';' and depth == 0:
            head = re.sub(r'/\*.*?\*/|//[^\n]*', '', src[stmt_start:i + 1], flags=re.S)
            # Parameter declarations belong to the pending K&R function header,
            # so they must not reset the start before its opening brace.
            if not kr_header.search(head):
                stmt_start = i + 1
        i += 1
    res, pos = [], 0
    for s, e, text in out:
        res.append(src[pos:s])
        res.append(text)
        pos = e
    res.append(src[pos:])
    return ''.join(res)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('unit')
    ap.add_argument('func')
    ap.add_argument('--from', dest='from_file')
    ap.add_argument('--run', action='store_true', help='run the permuter after setup')
    ap.add_argument('-j', type=int, default=4)
    ap.add_argument('--minutes', type=float, default=30, help='time limit for --run')
    ap.add_argument('--fresh', action='store_true', help='delete earlier outputs, even a score-0 one')
    ap.add_argument('--profile', choices=['default', 'regalloc'], default='default',
                    help='regalloc: weight the passes that move register allocation (declaration/statement order, '
                         'commutative swaps, temporaries, chained assignments, local types) and damp the rest')
    a = ap.parse_args()

    lo, hi, funcs = unit_range(a.unit)
    f = next((x for x in funcs if x[2] == a.func), None)
    if not f:
        sys.exit(f'{a.func} not in unit {a.unit}')
    fa, fsize, _ = f
    d = f'build/permuter/{a.func}'
    os.makedirs(d, exist_ok=True)
    olds = [o for o in os.listdir(d) if o.startswith('output-')]
    if any(o.startswith('output-0-') for o in olds) and not a.fresh:
        sys.exit(f'{d} already has a score-0 output; apply it, or pass --fresh to start over')
    for old in olds:
        if True:
            subprocess.run(['rm', '-rf', f'{d}/{old}'])

    # base.c: preprocessed unit with only the target function's body
    with open(f'{d}/unit.c', 'w') as fh:
        fh.write(draft_source(a.unit, a.func, a.from_file))
    r = run(['cpp', '-P', '-nostdinc', '-undef', '-I', 'include', '-I', f'{AGBCC_DIR}/include', '-iquote', '.',
             '-iquote', 'src', f'{d}/unit.c', '-o', f'{d}/unit.i'])
    if r.returncode:
        sys.exit('cpp failed:\n' + r.stderr)
    base = strip_other_fns(open(f'{d}/unit.i').read(), a.func)
    base = re.sub(r'^\s*asm\s*\(".*"\)\s*;\s*$', '', base, flags=re.M)  # stray top-level asm
    with open(f'{d}/base.c', 'w') as fh:
        fh.write(base)
    os.remove(f'{d}/unit.i')
    os.remove(f'{d}/unit.c')

    cc, flags = unit_cflags(a.unit)
    ab = os.path.abspath(d)
    with open(f'{d}/link.ld', 'w') as fh:
        fh.write(f'SECTIONS {{ . = 0x{fa:08X}; .text : {{ *(.text) }} .rodata : {{ *(.rodata) }} '
                 f'.data : {{ *(.data) }} .bss 0x02000000 (NOLOAD) : {{ *(.bss) *(COMMON) }} '
                 f'/DISCARD/ : {{ *(.comment) }} }}\n')
    with open(f'{d}/compile.sh', 'w') as fh:
        fh.write(f'''#!/bin/bash
# usage: compile.sh in.c -o out.o   (in.c is already preprocessed)
set -e
T=$(mktemp -d); trap 'rm -rf "$T"' EXIT
{AGBCC_DIR}/bin/{cc} {' '.join(flags)} "$1" -o "$T/a.s" 2>/dev/null
printf '\\t.text\\n\\t.align 2, 0\\n' >> "$T/a.s"
arm-none-eabi-as -mcpu=arm7tdmi -mthumb-interwork -o "$T/a.o" "$T/a.s"
arm-none-eabi-ld -T {ab}/link.ld -o "$3" "$T/a.o" {ab}/syms.o
''')
    os.chmod(f'{d}/compile.sh', 0o755)

    # target: the function's asm, assembled and linked the same way
    with open(f'{d}/target.s', 'w') as fh:
        fh.write(f'\t.include "asm/macros.inc"\n\t.text\n\t.include "asm/nonmatching/{a.unit}/{a.func}.s"\n')
    r = run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-mthumb-interwork', '-I', '.', '-o', f'{d}/target_raw.o',
             f'{d}/target.s'])
    if r.returncode:
        sys.exit('target assembly failed:\n' + r.stderr)

    # the candidate's externals + the target's externals -> one symbol stub
    undef = set()
    raw_cand = f'{d}/cand.o'
    rr = run(['bash', '-c', f'''set -e; T=$(mktemp -d); {AGBCC_DIR}/bin/{cc} {' '.join(flags)} {d}/base.c -o $T/a.s;
        printf '\\t.text\\n\\t.align 2, 0\\n' >> $T/a.s; arm-none-eabi-as -mcpu=arm7tdmi -mthumb-interwork -o {raw_cand} $T/a.s'''])
    if rr.returncode:
        sys.exit(f'the draft does not compile:\n{rr.stderr}\n(see {d}/base.c)')
    for obj in (raw_cand, f'{d}/target_raw.o'):
        for l in run(['arm-none-eabi-nm', obj]).stdout.splitlines():
            p = l.split()
            if p[0] == 'U':
                undef.add(p[-1])
    bsyms = baseline_syms()
    lines = ['\t.text']
    for s in sorted(undef):
        if s in bsyms:
            addr, thumb = bsyms[s]
        else:
            m = re.search(r'_([0-9A-Fa-f]{8})$', s)
            if not m:
                sys.exit(f'unresolved external symbol {s} (not in build/eds.elf, no address suffix)')
            addr = int(m.group(1), 16)
            thumb = s.startswith('sub_') and BASE <= addr < 0x08080A20
        lines.append(f'\t.global {s}')
        lines.append(f'\t.thumb_set {s}, 0x{addr:08X}' if thumb else f'\t.set {s}, 0x{addr:08X}')
    with open(f'{d}/syms.s', 'w') as fh:
        fh.write('\n'.join(lines) + '\n')
    r = run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-o', f'{d}/syms.o', f'{d}/syms.s'])
    if r.returncode:
        sys.exit('symbol stub failed:\n' + r.stderr)
    r = run(['arm-none-eabi-ld', '-T', f'{d}/link.ld', '-o', f'{d}/target.o', f'{d}/target_raw.o', f'{d}/syms.o'])
    if r.returncode:
        sys.exit('target link failed:\n' + r.stderr)
    for tmp in ('target_raw.o', 'cand.o', 'target.s'):
        if os.path.exists(f'{d}/{tmp}'):
            os.remove(f'{d}/{tmp}')
    with open(f'{d}/settings.toml', 'w') as fh:
        fh.write(f'func_name = "{a.func}"\ncompiler_type = "gcc"\n')
        if a.profile == 'regalloc':
            fh.write('\n[weight_overrides]\n' + '\n'.join(f'{k} = {v}' for k, v in REGALLOC_WEIGHTS.items()) + '\n')

    # sanity: the target bytes must equal the ROM
    run(['arm-none-eabi-objcopy', '-O', 'binary', '-j', '.text', f'{d}/target.o', f'{d}/target.bin'])
    tb = open(f'{d}/target.bin', 'rb').read()
    os.remove(f'{d}/target.bin')
    if os.path.exists('baserom.gba'):
        rom = open('baserom.gba', 'rb').read()[fa - BASE:fa - BASE + fsize]
        if tb[:fsize] != rom:
            sys.exit('internal error: assembled target does not match the ROM')
    else:
        print('note: no baserom.gba; target taken from the original assembly without a ROM cross-check')
    print(f'set up {d} (target 0x{fa:08X}, 0x{fsize:X} bytes)')

    if a.run:
        cmd = ['timeout', f'{int(a.minutes * 60)}', 'python3', os.path.join(os.environ.get('PERMUTER_DIR', '/opt/permuter'), 'permuter.py'), d, '-j', str(a.j),
               '--stop-on-zero', '--best-only', '--no-ignore-branch-targets']
        print(' '.join(cmd))
        subprocess.run(cmd)
        outs = sorted((o for o in os.listdir(d) if o.startswith('output-')),
                      key=lambda o: int(o.split('-')[1]))
        if outs:
            print(f'best: {d}/{outs[0]}/source.c (score {outs[0].split("-")[1]})')


if __name__ == '__main__':
    main()
