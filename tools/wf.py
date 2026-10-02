#!/usr/bin/env python3
"""Private per-function working copies, so several agents can work on functions of the same unit at once.

Run on the host (it calls tools/dr for compiling):
  python3 tools/wf.py prep  <func>          # build/wf/<func>/unit.c: src/<unit>.c with <func>'s draft enabled
                                            # between WF-BEGIN/WF-END markers; prints the starting score
  python3 tools/wf.py check <func> [--ctx N] [--full]
                                            # compile the working copy; normalized diff of <func> + score
  python3 tools/wf.py score <func>          # just the score line
  python3 tools/wf.py perm  <func> [--minutes M] [-j J]
                                            # permuter on the working copy (outputs in build/permuter/<func>/)
  python3 tools/wf.py apply <func>          # merge the working copy into src/<unit>.c (3-way, under a lock),
                                            # keep it only if the whole unit matches; else revert
  python3 tools/wf.py park  <func> "<note>" # store the working copy's function as the unit's
                                            # `#if 0 /* NONMATCHING: note */` draft, if it scores better than
                                            # the draft it started from

Edit only build/wf/<func>/unit.c. Put new structs, externs, prototypes and static inline helpers between the
markers, above the function; edits elsewhere in the copy are merged too, but keep them minimal.
Score = differing normalized lines + 4 * |size delta| (0 = the function matches).
"""
import json
import os
import re
import subprocess
import sys
import textwrap
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match_drafts import BLOCK  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)
DR = os.path.join(ROOT, 'tools', 'dr')
FUNCS = {}
for line in open('config/functions.tsv'):
    if not line.startswith('#'):
        f = line.rstrip('\n').split('\t')
        FUNCS[f[3]] = dict(addr=int(f[0], 16), mode=f[1], size=int(f[2], 16), unit=f[4])


def die(msg):
    sys.exit(msg)


def wdir(func):
    return f'build/wf/{func}'


def markers(func):
    return (f'/* WF-BEGIN {func}: work between these markers */', f'/* WF-END {func} */')


def include_re(func):
    return re.compile(r'^[ \t]*INCLUDE_ASM\(\s*"[^"]*"\s*,\s*' + re.escape(func) + r'\s*\);[^\n]*\n?', re.M)


def find_block(source, func):
    """(start, end, draft body or None, header note) of func's `#if 0` draft + INCLUDE_ASM, or its bare INCLUDE_ASM."""
    for m in BLOCK.finditer(source):
        if m['name'] == func:
            header = m['header'].lstrip()
            body = m['body']
            note = ''
            if header.startswith('/*'):
                text = header + '\n' + body
                end = text.find('*/')
                note = text[2:end].strip()
                body = text[end + 2:].lstrip('\n')
            end = m.end()
            if end < len(source) and source[end] == '\n':
                end += 1
            return m.start(), end, body, note
    m = include_re(func).search(source)
    if not m:
        return None
    return m.start(), m.end(), None, ''


def run_check(unit, src, func=None, ctx=3):
    cmd = [DR, 'python3', 'tools/check.py', unit, '--src', src]
    if func:
        cmd += ['--diff', func, '--norm', '--ctx', str(ctx)]
    r = subprocess.run(cmd, capture_output=True, text=True)
    return r.returncode, r.stdout + r.stderr


def score_text(out, func):
    if 'compile failed' in out or 'assemble failed' in out or 'link failed' in out or 'unresolved external' in out:
        return None
    if f'{func}: MATCH' in out and 'size:' not in out and 'not defined' not in out:
        return 0
    m = re.search(r'(\d+) differing lines', out)
    lines = int(m.group(1)) if m else 0
    m = re.search(r'\(([+-]\d+) bytes\)', out)
    delta = int(m.group(1)) if m else 0
    if 'not defined by the build' in out:
        return 100000
    return lines + 4 * abs(delta)


def working_score(func, ctx=3):
    u = FUNCS[func]['unit']
    code, out = run_check(u, f'{wdir(func)}/unit.c', func, ctx)
    return score_text(out, func), out


def lock(unit):
    path = f'build/wf/.locks/{unit}'
    os.makedirs('build/wf/.locks', exist_ok=True)
    t0 = time.time()
    while True:
        try:
            os.mkdir(path)
            return path
        except FileExistsError:
            try:
                if time.time() - os.path.getmtime(path) > 900:
                    os.rmdir(path)
                    continue
            except FileNotFoundError:
                continue
            if time.time() - t0 > 1200:
                die(f'could not lock {unit} (held by another agent for 20 min)')
            time.sleep(2)


def unlock(path):
    try:
        os.rmdir(path)
    except FileNotFoundError:
        pass


def merge(current, base, theirs):
    """3-way merge of theirs (derived from base) onto current. Returns (text, conflicts)."""
    tmp = f'build/wf/.merge-{os.getpid()}'
    os.makedirs(tmp, exist_ok=True)
    paths = []
    for n, t in (('cur', current), ('base', base), ('theirs', theirs)):
        p = f'{tmp}/{n}.c'
        open(p, 'w').write(t)
        paths.append(p)
    r = subprocess.run(['git', 'merge-file', '-p', '--diff3'] + paths, capture_output=True, text=True)
    subprocess.run(['rm', '-rf', tmp])
    return r.stdout, r.returncode


def strip_markers(text, func):
    b, e = markers(func)
    return '\n'.join(l for l in text.split('\n') if l.strip() not in (b, e))


def marked_region(text, func):
    b, e = markers(func)
    lines = text.split('\n')
    try:
        i = next(k for k, l in enumerate(lines) if l.strip() == b)
        j = next(k for k, l in enumerate(lines) if l.strip() == e)
    except StopIteration:
        die(f'the WF-BEGIN/WF-END markers for {func} are missing from {wdir(func)}/unit.c; put them back')
    return i, j, lines


FORBIDDEN = re.compile(r'(?:__asm__|\basm)\s*(?:volatile|__volatile__)?\s*\(\s*"[^"]*\S[^"]*"')


def validate(text, func):
    i, j, lines = marked_region(text, func)
    region = '\n'.join(lines[i + 1:j])
    if include_re(func).search(region + '\n'):
        die(f'{func} is still INCLUDE_ASM between the markers')
    if not re.search(r'\b' + re.escape(func) + r'\s*\([^;{]*\)\s*\{', region):
        die(f'no C definition of {func} between the markers')
    if FORBIDDEN.search(region) or re.search(r'\.(incbin|byte|hword|word|2byte|4byte|inst)\b', region):
        die('hand-written instructions/data inside asm() are not accepted (empty asm("" : ...) is fine)')
    return region


def cmd_prep(func):
    if func not in FUNCS:
        die(f'unknown function {func}')
    unit = FUNCS[func]['unit']
    src = f'src/{unit}.c'
    source = open(src).read()
    blk = find_block(source, func)
    if blk is None:
        die(f'{func}: no INCLUDE_ASM in {src} (already matched?)')
    start, end, body, note = blk
    b, e = markers(func)
    if body is None:
        body = f'/* no draft yet: write {func} here (target: asm/nonmatching/{unit}/{func}.s) */\n'
    os.makedirs(wdir(func), exist_ok=True)
    work = source[:start] + b + '\n' + body.rstrip('\n') + '\n' + e + '\n' + source[end:]
    if os.path.exists(f'{wdir(func)}/unit.c') and '--force' not in sys.argv:
        print(f'{wdir(func)}/unit.c already exists (pass --force to start over); rescoring it')
    else:
        open(f'{wdir(func)}/base.c', 'w').write(source)
        open(f'{wdir(func)}/unit.c', 'w').write(work)
    s, out = working_score(func)
    meta = dict(func=func, unit=unit, note=note, start_score=s, size=FUNCS[func]['size'])
    if not os.path.exists(f'{wdir(func)}/prep.json') or '--force' in sys.argv:
        json.dump(meta, open(f'{wdir(func)}/prep.json', 'w'), indent=1)
    print(f'{func} ({unit}, 0x{FUNCS[func]["size"]:X} bytes). Working copy: {wdir(func)}/unit.c')
    print(f'target asm: asm/nonmatching/{unit}/{func}.s')
    if note:
        print(f'draft note: {note}')
    print(f'score: {s}' + (' (does not compile)' if s is None else ''))
    if s is None:
        print(out[-3000:])


def cmd_check(func, ctx, full):
    s, out = working_score(func, ctx)
    lines = out.rstrip('\n').split('\n')
    if not full and len(lines) > 400:
        out = '\n'.join(lines[:400]) + f'\n... ({len(lines) - 400} more lines; pass --full)'
    print(out.rstrip('\n'))
    print(f'score: {s}' + (' (does not compile)' if s is None else ' (MATCH)' if s == 0 else ''))


def cmd_perm(func, minutes, j):
    unit = FUNCS[func]['unit']
    src = f'{wdir(func)}/unit.c'
    text = strip_markers(open(src).read(), func)
    p = f'{wdir(func)}/perm-input.c'
    open(p, 'w').write(text)
    r = subprocess.run([DR, 'python3', 'tools/permute.py', unit, func, '--src', p, '--fresh', '--run', '-j', str(j),
                        '--minutes', str(minutes), '--profile', 'regalloc'], capture_output=True, text=True)
    out = r.stdout + r.stderr
    print('\n'.join(l for l in out.split('\n') if not l.startswith('iteration') and 'score =' not in l)[-3000:])
    d = f'build/permuter/{func}'
    outs = sorted((o for o in os.listdir(d) if o.startswith('output-')), key=lambda o: int(o.split('-')[1])) if os.path.isdir(d) else []
    if outs:
        print(f'best permuter output: {d}/{outs[0]}/source.c (permuter score {outs[0].split("-")[1]}); '
              f'diff it against {d}/base.c to see what changed')


def cmd_apply(func):
    unit = FUNCS[func]['unit']
    src = f'src/{unit}.c'
    work = open(f'{wdir(func)}/unit.c').read()
    validate(work, func)
    s, out = working_score(func)
    if s != 0:
        die(f'the working copy does not match yet (score {s}); nothing applied')
    base = open(f'{wdir(func)}/base.c').read()
    lk = lock(unit)
    try:
        cur = open(src).read()
        merged, conflicts = merge(cur, base, work)
        if conflicts:
            die(f'merge conflict with changes made to {src} since prep; re-run prep --force on a copy, or resolve by hand')
        merged = strip_markers(merged, func)
        if include_re(func).search(merged):
            die('merge kept the INCLUDE_ASM line; nothing applied')
        open(src, 'w').write(merged)
        code, out = run_check(unit, src)
        if 'unit bytes MATCH' not in out:
            open(src, 'w').write(cur)
            die(f'merged unit does not match; {src} restored:\n{out[-2500:]}')
        print(out.rstrip('\n').split('\n')[-1])
        print(f'APPLIED: {func} now matches in {src}')
        json.dump(dict(func=func, unit=unit, applied=time.strftime('%Y-%m-%d %H:%M')),
                  open(f'{wdir(func)}/applied.json', 'w'))
    finally:
        unlock(lk)


def cmd_park(func, note):
    unit = FUNCS[func]['unit']
    src = f'src/{unit}.c'
    meta = json.load(open(f'{wdir(func)}/prep.json'))
    work = open(f'{wdir(func)}/unit.c').read()
    s, out = working_score(func)
    if s is None:
        die('the working copy does not compile; nothing parked')
    start = meta.get('start_score')
    if start is not None and s >= start:
        die(f'score {s} is not better than the starting draft ({start}); nothing parked')
    i, j, lines = marked_region(work, func)
    region = '\n'.join(lines[i + 1:j]).strip('\n')
    note = ' '.join(note.replace('*/', '* /').split())
    head = '\n       * '.join(textwrap.wrap(f'#if 0 /* NONMATCHING (score {s}): {note} */', 112,
                                            break_long_words=False, break_on_hyphens=False))
    inc = f'INCLUDE_ASM("asm/nonmatching/{unit}", {func}); /* 0x{FUNCS[func]["addr"]:08X} size 0x{FUNCS[func]["size"]:X} */'
    parked = '\n'.join(lines[:i] + [head, region, '#endif', inc] + lines[j + 1:])
    base = open(f'{wdir(func)}/base.c').read()
    lk = lock(unit)
    try:
        cur = open(src).read()
        merged, conflicts = merge(cur, base, parked)
        if conflicts:
            die(f'merge conflict with changes made to {src} since prep; nothing parked')
        open(src, 'w').write(merged)
        code, out = run_check(unit, src)
        if 'unit bytes MATCH' not in out:
            open(src, 'w').write(cur)
            die(f'parking broke the unit; {src} restored:\n{out[-2500:]}')
        print(f'PARKED: {func} draft (score {s}, was {start}) in {src}')
        meta['parked_score'] = s
        json.dump(meta, open(f'{wdir(func)}/prep.json', 'w'), indent=1)
    finally:
        unlock(lk)


def main():
    a = sys.argv[1:]
    if len(a) < 2:
        die(__doc__)
    cmd, func = a[0], a[1]
    if func not in FUNCS:
        die(f'unknown function {func}')
    opt = lambda k, d: a[a.index(k) + 1] if k in a else d
    if cmd == 'prep':
        cmd_prep(func)
    elif cmd == 'check':
        cmd_check(func, int(opt('--ctx', 3)), '--full' in a)
    elif cmd == 'score':
        s, _ = working_score(func)
        print(f'score: {s}')
    elif cmd == 'perm':
        cmd_perm(func, float(opt('--minutes', 15)), int(opt('-j', 2)))
    elif cmd == 'apply':
        cmd_apply(func)
    elif cmd == 'park':
        if len(a) < 3:
            die('park needs a note')
        cmd_park(func, a[2])
    else:
        die(__doc__)


if __name__ == '__main__':
    main()
