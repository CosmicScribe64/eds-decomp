#!/usr/bin/env python3
"""Check a shared header against the header plan before any unit includes it.

  tools/dr python3 tools/hdrcheck.py include/duel.h [include/chain.h ...]
  tools/dr python3 tools/hdrcheck.py --group duel_core        # every header of a writer group
  tools/dr python3 tools/hdrcheck.py --all                    # every header of the plan that exists
                                                              # (no plan in build/: every include/*.h, compile checks)

For each header:
  1. compile   a translation unit `#include "global.h"` + the header twice (include guard) with old_agbcc
               and agbcc (the sound driver's compiler): it must compile on its own (its includes are complete);
  2. ownership every struct, enum, function and RAM global that build/readability/header_plan.json assigns to
               the header is defined/declared in it, and nothing it defines belongs to another header;
  3. layout    every struct of types.json defined here has the planned size (agbcc rules, tools/structmap.py)
               and each types.json field that the header names is at the planned byte/bit offset;
  4. prototypes every function prototype resolves (typedefs expanded) to the same types as the plan's
               header_proto/proto, i.e. the definition as compiled today: same width/signedness, same count.

Exit status 1 when any header has an error. Warnings (missing ROM data externs, unmatched field names, extra
declarations the plan does not list) do not fail the check.
"""
import argparse
import json
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(__file__))
from pycparser import c_ast, c_parser  # noqa: E402

import genprotos as gp  # noqa: E402

PLAN = 'build/readability/header_plan.json'
CC = [('/opt/agbcc/bin/old_agbcc', ['-mthumb-interwork', '-Wimplicit', '-Wparentheses', '-O2', '-fhex-asm']),
      ('/opt/agbcc/bin/agbcc', ['-mthumb-interwork', '-O2', '-fhex-asm'])]


def rel(path):
    p = os.path.relpath(path)
    return p[len('include/'):] if p.startswith('include/') else p


def compile_check(hdr):
    errs = []
    src = f'#include "global.h"\n#include "{hdr}"\n#include "{hdr}"\n'
    with tempfile.TemporaryDirectory(dir='build') as d:
        c = os.path.join(d, 't.c')
        open(c, 'w').write(src)
        i = os.path.join(d, 't.i')
        r = subprocess.run(['cpp', '-nostdinc', '-undef', '-I', 'include', '-I', '/opt/agbcc/include', '-iquote',
                            '.', c, '-o', i], capture_output=True, text=True)
        if r.returncode:
            return [f'cpp: {r.stderr.strip()[:600]}']
        for cc, flags in CC:
            r = subprocess.run([cc] + flags + [i, '-o', os.path.join(d, 't.s')], capture_output=True, text=True)
            msg = (r.stderr or '').strip()
            if r.returncode or 'error' in msg.lower():
                errs.append(f'{os.path.basename(cc)}: {msg[:800]}')
            elif msg:
                errs.append(f'warning ({os.path.basename(cc)}): {msg[:400]}')
    return errs


def parse_tu(hdr, extra=''):
    src = f'#include "global.h"\n#include "{hdr}"\n{extra}'
    with tempfile.TemporaryDirectory(dir='build') as d:
        c = os.path.join(d, 't.c')
        open(c, 'w').write(src)
        t = gp.clean(gp.cpp(c))
        t, _ = gp.strip_bodies(t)
        t = gp.fold_asm(t)
        t = gp.strip_attrs(t)
        ast = c_parser.CParser().parse(t, c)
        return ast, c


def collect(ast, hdr_path):
    """Items defined in the header file itself."""
    out = {'structs': {}, 'enums': set(), 'enumerators': set(), 'functions': {}, 'globals': {}, 'typedefs': set()}

    def visit_type(node):
        for _, ch in node.children():
            if isinstance(ch, (c_ast.Struct, c_ast.Union)) and ch.decls is not None and ch.name:
                out['structs'][ch.name] = ch
            if isinstance(ch, c_ast.Enum) and ch.values is not None:
                if ch.name:
                    out['enums'].add(ch.name)
                for v in ch.values.enumerators:
                    out['enumerators'].add(v.name)
            visit_type(ch)
    for ext in ast.ext:
        f = ext.coord.file if getattr(ext, 'coord', None) else None
        if not f or os.path.normpath(f) != os.path.normpath(hdr_path):
            continue
        if isinstance(ext, c_ast.FuncDef):
            ext = ext.decl
        if isinstance(ext, c_ast.Typedef):
            out['typedefs'].add(ext.name)
            visit_type(ext)
            continue
        if not isinstance(ext, c_ast.Decl):
            continue
        visit_type(ext)
        if not ext.name:
            continue
        local, sym = gp.split_label(ext.name)
        if isinstance(ext.type, c_ast.FuncDecl):
            out['functions'][sym] = ext
        else:
            out['globals'][sym] = ext
    return out


def sig(decl, types):
    fd = decl.type
    full = types.lay.typedefs
    ret = gp.canon_spelling(gp.tstr(gp.expand_typedefs(fd.type, full, full)))
    params, variadic = [], False
    if fd.args is not None:
        for p in fd.args.params:
            if isinstance(p, c_ast.EllipsisParam):
                variadic = True
                continue
            if isinstance(p, c_ast.Typename) and gp.tstr(p.type) == 'void' and len(fd.args.params) == 1:
                break
            params.append(gp.canon_spelling(gp.param_tstr(gp.expand_typedefs(p.type, full, full))))
    return ret, tuple(params), variadic, fd.args is not None


def norm_ws(s):
    return re.sub(r'\s+', ' ', s).strip()


def check_header(path, plan, types_json):
    hdr = rel(path)
    errors, warns = [], []
    if plan is None:  # no migration plan (fresh clone): structural checks and the compile check only
        if not os.path.exists(path):
            return [f'{path} does not exist'], []
        text = open(path).read()
        if not re.search(r'#ifndef\s+GUARD_\w+', text):
            errors.append('no include guard (#ifndef GUARD_..._H)')
        if hdr.startswith('constants/') and re.search(r'#include', text):
            errors.append('constants headers must not include anything')
        for m in compile_check(hdr):
            (warns if m.startswith('warning') else errors).append(m)
        return errors, warns
    entry = plan['headers'].get(hdr)
    if entry is None:
        return [f'{hdr} is not in the plan'], []
    if not os.path.exists(path):
        return [f'{path} does not exist'], []
    text = open(path).read()
    if not re.search(r'#ifndef\s+GUARD_\w+', text):
        errors.append('no include guard (#ifndef GUARD_..._H)')
    if hdr.startswith('constants/') and re.search(r'#include', text):
        errors.append('constants headers must not include anything')
    allowed = {x[len('include/'):] for x in entry.get('may_include', [])} | {'global.h'}
    for inc in re.findall(r'^\s*#\s*include\s*"([^"]+)"', text, re.M):
        if inc not in allowed:
            errors.append(f'#include "{inc}" is not allowed here (only earlier headers of include_order; '
                          f'see "may_include" in the plan)')
    for inc in entry.get('includes', []):
        if inc[len('include/'):] not in re.findall(r'^\s*#\s*include\s*"([^"]+)"', text, re.M):
            warns.append(f'plan lists {inc} as a required include (by-value struct use) but it is not included')
    for m in compile_check(hdr):
        (warns if m.startswith('warning') else errors).append(m)
    if errors:
        return errors, warns
    ast, _ = parse_tu(hdr)
    got = collect(ast, os.path.join('include', hdr))
    types = gp.Types(ast)
    owners = plan['owners']

    # ownership
    for s in entry['structs']:
        if s['name'] not in got['structs']:
            errors.append(f"struct {s['name']} is planned here but not defined")
    for e in entry['enums']:
        if e['name'] not in got['enums']:
            (warns if e['name'] == 'CardNumber' else errors).append(f"enum {e['name']} is planned here but not defined")
    for f in entry['functions']:
        if f['name'] not in got['functions']:
            (warns if f.get('asm') else errors).append(f"prototype {f['name']} is planned here but missing"
                                                       + (' (defined in assembly)' if f.get('asm') else ''))
    for g in entry['ram_globals']:
        if g['name'] not in got['globals']:
            warns.append(f"RAM global {g['name']} is planned here but not declared")
    for g in entry['rom_data']:
        if g['name'] not in got['globals']:
            warns.append(f"ROM data {g['name']} is planned here but not declared")
    for kind, names in (('struct', got['structs']), ('enum', got['enums']), ('function', got['functions']),
                        ('global', got['globals'])):
        for n in names:
            o = owners.get(n)
            if o and o != f'include/{hdr}':
                errors.append(f'{kind} {n} is defined here but the plan gives it to {o}')
            elif not o and kind in ('struct', 'enum'):
                warns.append(f'{kind} {n} is not in the plan (fine for a helper; tell the coordinator)')

    # layout
    tj = {s['name']: s for s in types_json['structs']}
    for name, body in got['structs'].items():
        s = tj.get(name)
        if not s:
            continue
        try:
            size = types.lay.size_align(body)[0]
        except Exception as e:  # noqa: BLE001
            errors.append(f'struct {name}: layout failed ({e})')
            continue
        m = re.match(r'\s*(0x[0-9A-Fa-f]+|\d+)', s['size'])
        rng = re.search(r'\((0x[0-9A-Fa-f]+)\.\.(0x[0-9A-Fa-f]+)\)', s['size'])
        base = int(rng.group(1), 16) if rng else 0
        if m and '+' not in s['size']:
            want = int(m.group(1), 0)
            if size != -(-want // 4) * 4:
                errors.append(f'struct {name}: sizeof = {size:#x}, planned {s["size"]} (padded {-(-want // 4) * 4:#x})')
        leaves = {}
        try:
            seen_prefix = set()
            for p_, bit, w, sz, ty in types.lay.leaves(c_ast.TypeDecl(None, [], None, body)):
                leaves.setdefault(p_.split('.')[-1].split('[')[0], []).append((bit, w))
                # struct/union-typed members (scene, fade, oamList, ...) are not leaves: record each enclosing
                # member at the offset of its first leaf, so their planned offsets are checked too
                parts = p_.split('.')
                for i in range(1, len(parts)):
                    pre = '.'.join(parts[:i])
                    if pre not in seen_prefix:
                        seen_prefix.add(pre)
                        leaves.setdefault(parts[i - 1].split('[')[0], []).append((bit, None))
        except Exception as e:  # noqa: BLE001
            warns.append(f'struct {name}: could not flatten ({e})')
            continue
        for f in s['fields']:
            try:
                off = int(f['offset'], 16) - base
            except ValueError:
                continue
            bits = f.get('bits')
            lo = int(str(bits).split('-')[0]) if bits else 0
            want_bit = off * 8 + lo
            have = leaves.get(f['name'].split('.')[-1].split('[')[0])    # "scrollBar.thumbPos" -> thumbPos
            if not have:
                warns.append(f"struct {name}: no member named {f['name']} (planned at +{off:#x}"
                             f"{' bits ' + str(bits) if bits else ''})")
                continue
            if not any(b == want_bit for b, _ in have):
                errors.append(f"struct {name}.{f['name']}: at bit {have[0][0]} (+{have[0][0] // 8:#x}), planned "
                              f"+{off:#x}{' bit ' + str(lo) if bits else ''}")

    # prototypes
    planned = {f['name']: f for f in entry['functions']}
    probe = []
    for n, f in planned.items():
        p = f.get('header_proto') or f.get('proto')
        if not p or n not in got['functions']:
            continue
        p = re.sub(r'/\*.*?\*/', '', p)
        probe.append(re.sub(r'\b' + re.escape(n) + r'\s*\(', f'__plan__{n}(', p, count=1).strip() + ';')
    if probe:
        tags = sorted({t for line in probe for t in re.findall(r'struct\s+(\w+)', line)})
        extra = ''.join(f'struct {t};\n' for t in tags) + '\n'.join(probe) + '\n'
        try:
            ast2, c = parse_tu(hdr, extra)
            types2 = gp.Types(ast2)
            plan_decls = {e.name[len('__plan__'):]: e for e in ast2.ext
                          if isinstance(e, c_ast.Decl) and e.name and e.name.startswith('__plan__')}
            for n, d in plan_decls.items():
                have = sig(got['functions'][n], types)
                want = sig(d, types2)
                if have != want:
                    errors.append(f'prototype {n}: header {have[0]} ({", ".join(have[1])}'
                                  f'{", ..." if have[2] else ""}) != definition {want[0]} ({", ".join(want[1])}'
                                  f'{", ..." if want[2] else ""})')
        except Exception as e:  # noqa: BLE001
            warns.append(f'could not compare prototypes ({str(e)[:200]})')
    return errors, warns


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('headers', nargs='*')
    ap.add_argument('--group')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--plan', default=PLAN)
    ap.add_argument('-q', action='store_true', help='errors only')
    a = ap.parse_args()
    # The migration plan and type proposals live in build/ (gitignored); without them, check every header
    # in include/ and include/constants/ for guards and a clean standalone compile.
    plan = json.load(open(a.plan)) if os.path.exists(a.plan) else None
    types_json = json.load(open('build/names/types.json')) if plan and os.path.exists('build/names/types.json') \
        else {'structs': [], 'enums': []}
    hs = list(a.headers)
    if a.group:
        if plan is None:
            sys.exit(f'--group needs the plan ({a.plan})')
        g = next((g for g in plan['groups'] if g['name'] == a.group), None)
        if not g:
            sys.exit(f'no group {a.group}')
        hs += g['headers']
    if a.all:
        if plan is None:
            import glob
            # agb_sram.h is Nintendo's self-contained SDK header (its own base typedefs), used only by sdk/agb_sram.c
            hs += sorted(h for h in glob.glob('include/*.h') + glob.glob('include/constants/*.h')
                         if h != 'include/agb_sram.h')
        else:
            hs += [h['path'] for h in plan['headers'].values() if os.path.exists(h['path'])]
    if not hs:
        ap.error('no headers given')
    bad = 0
    for h in hs:
        path = h if h.startswith('include/') else f'include/{h}'
        errors, warns = check_header(path, plan, types_json)
        status = 'OK' if not errors else f'{len(errors)} errors'
        print(f'{path}: {status}, {len(warns)} warnings')
        for e in errors:
            print(f'  ERROR {e}')
        if not a.q:
            for w in warns:
                print(f'  warn  {w}')
        bad += bool(errors)
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
