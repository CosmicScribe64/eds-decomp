#!/usr/bin/env python3
"""Search other agbcc decomps for functions that look like one of ours.

Other GBA projects (pret, tmc, sa2, mzm, Fire Emblem 8, Kingdom Hearts: CoM, Kirby, Klonoa) have thousands of
matched C functions built with the same compiler. Compiling their C with our agbcc gives C/asm pairs; an unmatched
EDS function whose instruction pattern resembles one of those shows which C shape produces it.

  tools/dr python3 tools/corpus.py build          # compile build/corpus/<project>/src/**/*.c to build/corpus/_s/
  python3 tools/corpus.py index                   # parse the .s files into build/corpus/index.json
  python3 tools/corpus.py near <func> [-n 5]      # closest corpus functions to an EDS function

Projects are sparse clones (C and headers only) in build/corpus/<project>; see wiki/tools/corpus.md for setup.
Their C is compiled with generic flags (-O2, IDE stubs for INCBIN), so files that need generated headers are
skipped, and the asm of a few files may differ slightly from that project's real build.
"""
import difflib
import glob
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

ROOT = 'build/corpus'
OUT = f'{ROOT}/_s'
AGBCC = os.environ.get('AGBCC_DIR', '/opt/agbcc')
DEFINES = ['-D__INTELLISENSE__', '-DMODERN=0', '-DNDEBUG', '-DENGLISH', '-DUSA', '-DREVISION=0',
           '-DGAME_VERSION=0', '-DFIRERED', '-DBUGFIX=0', '-DPLATFORM_GBA=1', '-DNON_MATCHING=0']


def projects():
    return sorted(d for d in os.listdir(ROOT) if not d.startswith('_') and os.path.isdir(f'{ROOT}/{d}'))


def compile_one(args):
    proj, src, incs = args
    out = f'{OUT}/{proj}/{os.path.relpath(src, f"{ROOT}/{proj}")[:-2]}.s'
    if os.path.exists(out):
        return True
    os.makedirs(os.path.dirname(out), exist_ok=True)
    # Headers a project generates during its own build are replaced by empty stubs, one at a time.
    stub = f'{ROOT}/_stub/{proj}'
    for _ in range(8):
        cpp = subprocess.run(['cpp', '-nostdinc', '-undef', '-Wno-trigraphs', *DEFINES, '-I', f'{AGBCC}/include',
                              *sum((['-I', i, '-iquote', i] for i in incs + [stub]), []), src],
                             capture_output=True, text=True, errors='replace')
        missing = re.search(r'fatal error: (\S+): No such file', cpp.stderr)
        if cpp.returncode == 0 or not missing:
            break
        os.makedirs(os.path.dirname(f'{stub}/{missing.group(1)}'), exist_ok=True)
        open(f'{stub}/{missing.group(1)}', 'a').close()
    if cpp.returncode != 0:
        return False
    i = out[:-2] + '.i'
    open(i, 'w').write(cpp.stdout)
    cc = subprocess.run([f'{AGBCC}/bin/agbcc', '-mthumb-interwork', '-O2', i, '-o', out],
                        capture_output=True, text=True, errors='replace')
    os.remove(i)
    if cc.returncode != 0:
        if os.path.exists(out):
            os.remove(out)
        return False
    return True


def cmd_build():
    jobs = []
    for proj in projects():
        base = f'{ROOT}/{proj}'
        incs = [base, f'{base}/src'] + sorted(d for d in glob.glob(f'{base}/*') + glob.glob(f'{base}/*/*')
                                              if os.path.basename(d) in ('include', 'gflib', 'constants'))
        for src in glob.glob(f'{base}/src/**/*.c', recursive=True):
            jobs.append((proj, src, incs))
    with ThreadPoolExecutor(os.cpu_count()) as ex:
        res = list(ex.map(compile_one, jobs))
    per = {}
    for (proj, _, _), ok in zip(jobs, res):
        per.setdefault(proj, [0, 0])[0 if ok else 1] += 1
    for proj, (ok, bad) in per.items():
        print(f'{proj:16s} {ok:4d} compiled, {bad:4d} skipped')


INSN = re.compile(r'^\s+([a-z][a-z0-9]*)\b(.*)$')


def norm(op, rest):
    """Normalize one instruction so our disassembly and agbcc -S output compare equal."""
    rest = re.sub(r'@.*$', '', rest)
    if op in ('bl', 'blx'):
        return 'bl S'
    rest = re.sub(r'\b(r\d+|sb|sl|fp|ip|sp|lr|pc)\b', 'R', rest)
    rest = re.sub(r'#-?(0x[0-9A-Fa-f]+|\d+)', '#N', rest)
    rest = re.sub(r'\.L\w+|\b_[0-9A-F]{8}\b', 'L', rest)
    rest = re.sub(r'L[+-]\d+', 'L', rest)
    rest = re.sub(r'\[R\]', '[R, #N]', rest)
    rest = re.sub(r'\b[A-Za-z_]\w*\b(?<!R)(?<!L)', 'S', rest) if op.startswith('b') else rest
    return ' '.join((op + ' ' + rest).split())


def asm_tokens(lines):
    out = []
    for line in lines:
        m = INSN.match(line)
        if m and not line.strip().startswith('.'):
            out.append(norm(m.group(1), m.group(2)))
    return out


def cmd_index():
    index = []
    for path in glob.glob(f'{OUT}/**/*.s', recursive=True):
        rel = path[len(OUT) + 1:]
        proj, src = rel.split('/')[0], f'{ROOT}/{rel[:-2]}.c'
        name, body, pending = None, [], False
        for line in open(path, errors='replace'):
            stripped = line.strip()
            if stripped == '.thumb_func':
                pending = True
                continue
            m = re.match(r'^(\w+):$', line.rstrip())
            if pending and m:
                name, body, pending = m.group(1), [], False
            elif name and stripped.startswith('.size'):
                toks = asm_tokens(body)
                if len(toks) >= 8:
                    index.append({'proj': proj, 'src': src, 'func': name, 'toks': toks})
                name = None
            elif name:
                body.append(line)
    json.dump(index, open(f'{ROOT}/index.json', 'w'))
    by = {}
    for x in index:
        by[x['proj']] = by.get(x['proj'], 0) + 1
    print(f'{len(index)} functions indexed: ' + ', '.join(f'{p} {n}' for p, n in sorted(by.items())))


def eds_tokens(func):
    for line in open('config/functions.tsv'):
        f = line.rstrip('\n').split('\t')
        if not line.startswith('#') and f[3] == func:
            path = f'asm/nonmatching/{f[4]}/{func}.s'
            if os.path.exists(path):
                return asm_tokens(open(path).read().split('\n'))
    sys.exit(f'no asm/nonmatching file for {func}')


def find_line(src, func):
    try:
        for i, line in enumerate(open(src, errors='replace')):
            if re.match(r'^[A-Za-z_][\w \*]*\b' + re.escape(func) + r'\s*\(', line) and not line.rstrip().endswith(';'):
                return f'{src}:{i + 1}'
    except OSError:
        pass
    return src


def cmd_near(func, n=5):
    a = eds_tokens(func)
    index = json.load(open(f'{ROOT}/index.json'))
    res = []
    for x in index:
        b = x['toks']
        if not (0.5 * len(a) <= len(b) <= 2 * len(a)):
            continue
        sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
        if sm.real_quick_ratio() < 0.5 or sm.quick_ratio() < 0.5:
            continue
        r = sm.ratio()
        if r >= 0.5:
            res.append((r, x))
    res.sort(key=lambda t: -t[0])
    print(f'{func}: {len(a)} instructions')
    for r, x in res[:n]:
        print(f'{r:.0%}  {x["proj"]:12s} {x["func"]:32s} {len(x["toks"]):5d} insns  {find_line(x["src"], x["func"])}')
    if not res:
        print('no corpus function at 50% or more')


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    if sys.argv[1] == 'build':
        cmd_build()
    elif sys.argv[1] == 'index':
        cmd_index()
    elif sys.argv[1] == 'near':
        cmd_near(sys.argv[2], int(sys.argv[4]) if len(sys.argv) > 4 and sys.argv[3] == '-n' else 5)
    else:
        sys.exit(__doc__)


if __name__ == '__main__':
    main()
