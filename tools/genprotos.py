#!/usr/bin/env python3
"""Exact prototypes of every function as DEFINED in src/, and every local declaration that differs from them.

  tools/dr python3 tools/genprotos.py                      # all C units -> build/readability/protos.json + report
  tools/dr python3 tools/genprotos.py card_stats duel_zones # some units only (merged into the same outputs)
  tools/dr python3 tools/genprotos.py --show GetZoneCardStats   # one function: definition + every unit's view

Each C unit (units.txt) is preprocessed like tools/structmap.py does (cpp -DOBJDIFF_BASE, the repo include
path) but with line markers kept, function bodies are emptied, asm labels (`T name(...) asm("Sym")`, the
alias views) are folded into the declaration, and the result is parsed with pycparser. Recorded per unit:

  definitions  every function body: symbol, storage (static/inline), return type, parameter types and names
  local_decls  every prototype the unit itself declares (not one that comes from include/*.h), for functions
               defined elsewhere or later in the unit, compared with the definition
  implicit     functions the unit calls with no file-scope declaration (implicit `int f()`, kind 'undeclared'),
               or only through a block-scope prototype inside a body (kind 'block_decl', text recorded)
  globals      every file-scope extern variable declaration (name, asm-label symbol, type)

A local declaration is compared with the definition (or, for functions defined in assembly, with nothing) and
classified by what the difference does to the CALLER's code:

  same          identical types (parameter names may differ)
  type          same calling convention, different spelling: typedef vs base type, pointee type, struct tag,
                32-bit int vs pointer, u32 vs s32 parameters. Safe to replace with the shared prototype.
  abi           a parameter or the return value differs in width or signedness below 32 bits (u8/u16/s8/s16/
                char vs anything else), the return type differs in signedness or void-ness, or a struct passed
                by value differs in size. The shared prototype CHANGES THE CALLER's code (narrowing).
  unprototyped  the unit declares `T f()` (no parameter list): calls do default promotions and no narrowing.
  arity         the number of parameters (or variadic-ness) differs.
  asm           the function is defined in assembly (no C definition to compare with); listed for completeness.

Outputs:
  build/readability/protos.json              machine-readable (functions, units, globals, statics)
  build/readability/proto_mismatches.txt     human report: every abi/unprototyped/arity/type local declaration

Plain `char` is unsigned in agbcc (verified: `int f(char c)` narrows with lsl/lsr), enums are 4 bytes.
"""
import argparse
import collections
import concurrent.futures
import datetime
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(__file__))
from pycparser import c_ast, c_generator, c_parser  # noqa: E402

import target  # noqa: E402

OUT_JSON = 'build/readability/protos.json'
OUT_REPORT = 'build/readability/proto_mismatches.txt'
GEN = c_generator.CGenerator()


# ---------------------------------------------------------------- preprocessing

def c_units():
    out = []
    for line in open('units.txt'):
        u = line.split('#')[0].strip()
        if u and not u.startswith('@') and os.path.exists(f'src/{u}.c'):
            out.append(u)
    return out


def cpp(path):
    r = subprocess.run(['cpp', '-nostdinc', '-undef', '-DOBJDIFF_BASE', '-I', 'include', '-I',
                        '/opt/agbcc/include', '-iquote', '.', path], capture_output=True, text=True)
    if r.returncode:
        raise RuntimeError(f'cpp failed: {r.stderr.strip()[:300]}')
    return r.stdout


def _blank(s):
    """Replace a span by the same number of newlines (keeps pycparser line numbers right)."""
    return '\n' * s.count('\n')


def _match_paren(t, i):
    """t[i] == '(' -> index after the matching ')' (string/char literal aware)."""
    depth, j = 0, i
    while j < len(t):
        c = t[j]
        if c in '"\'':
            k = j + 1
            while k < len(t) and t[k] != c:
                k += 2 if t[k] == '\\' else 1
            j = k + 1
            continue
        if c == '(':
            depth += 1
        elif c == ')':
            depth -= 1
            if depth == 0:
                return j + 1
        j += 1
    return len(t)


def _match_paren_back(t, i):
    """t[i] == ')' -> index of the matching '('."""
    depth, j = 0, i
    while j >= 0:
        c = t[j]
        if c == ')':
            depth += 1
        elif c == '(':
            depth -= 1
            if depth == 0:
                return j
        j -= 1
    return 0


def strip_bodies(t):
    """Empty every function body ({} with the same newlines). Returns (text, [(name, body_text)])."""
    out, i, depth, start = [], 0, 0, 0
    bodies = []
    n = len(t)
    while i < n:
        c = t[i]
        if c == '#' and (i == 0 or t[i - 1] == '\n'):          # line marker: skip the line
            j = t.find('\n', i)
            i = n if j < 0 else j
            continue
        if c in '"\'':
            k = i + 1
            while k < n and t[k] != c:
                k += 2 if t[k] == '\\' else 1
            i = k + 1
            continue
        if c == '{':
            if depth == 0:
                head = t[start:i]
                headcode = '\n'.join(l for l in head.split('\n') if not l.startswith('#'))
                if re.search(r'\)\s*$', headcode) and '=' not in headcode.split('(')[0]:
                    j, d = i + 1, 1
                    while d and j < n:
                        ch = t[j]
                        if ch in '"\'':
                            k = j + 1
                            while k < n and t[k] != ch:
                                k += 2 if t[k] == '\\' else 1
                            j = k + 1
                            continue
                        d += {'{': 1, '}': -1}.get(ch, 0)
                        j += 1
                    body = t[i:j]
                    hc = headcode.rstrip()
                    m = re.search(r'(\w+)\s*$', hc[:_match_paren_back(hc, len(hc) - 1)])
                    bodies.append((m.group(1) if m else None, body, i))
                    out.append(head + '{' + _blank(body) + '}')
                    i = start = j
                    continue
            depth += 1
        elif c == '}':
            depth -= 1
        elif c == ';' and depth == 0:
            out.append(t[start:i + 1])
            start = i + 1
        i += 1
    out.append(t[start:])
    return ''.join(out), bodies


ASM_RE = re.compile(r'\b(?:asm|__asm__|__asm)\b\s*(?:volatile\s*|__volatile__\s*)?\(')
ATTR_RE = re.compile(r'\b__attribute__\s*\(')


def fold_asm(t):
    """File-scope asm statements are dropped; asm labels rename the declarator to NAME__asmlabel__SYM."""
    out, pos = [], 0
    for m in ASM_RE.finditer(t):
        if m.start() < pos:
            continue
        end = _match_paren(t, m.end() - 1)
        k = m.start() - 1
        while k >= 0 and t[k] in ' \t\n':
            k -= 1
        prev = t[k] if k >= 0 else ';'
        if prev in ';}' or (k >= 0 and t[t.rfind('\n', 0, k) + 1] == '#'):
            # statement: asm("...");
            e2 = end
            while e2 < len(t) and t[e2] in ' \t':
                e2 += 1
            if e2 < len(t) and t[e2] == ';':
                e2 += 1
            out.append(t[pos:m.start()])
            out.append(_blank(t[m.start():e2]))
            pos = e2
            continue
        sym = re.match(r'\s*"([^"]*)"\s*\)', t[m.end():end])
        # find the declarator name before the label
        j = k
        if t[j] == ')':
            j = _match_paren_back(t, j) - 1
            while j >= 0 and t[j] in ' \t\n':
                j -= 1
        while j >= 0 and t[j] == ']':
            j = t.rfind('[', 0, j) - 1
            while j >= 0 and t[j] in ' \t\n':
                j -= 1
        e = j + 1
        while j >= 0 and (t[j].isalnum() or t[j] == '_'):
            j -= 1
        name = t[j + 1:e]
        if not sym or not name:
            out.append(t[pos:m.start()])
            out.append(_blank(t[m.start():end]))
            pos = end
            continue
        out.append(t[pos:j + 1])
        out.append(f'{name}__asmlabel__{sym.group(1)}')
        out.append(t[e:m.start()])
        out.append(_blank(t[m.start():end]))
        pos = end
    out.append(t[pos:])
    return ''.join(out)


def strip_attrs(t):
    out, pos = [], 0
    for m in ATTR_RE.finditer(t):
        if m.start() < pos:
            continue
        end = _match_paren(t, m.end() - 1)
        out.append(t[pos:m.start()])
        out.append(_blank(t[m.start():end]))
        pos = end
    out.append(t[pos:])
    return ''.join(out)


def clean(t):
    t = re.sub(r'\b__inline__\b|\b__inline\b', 'inline', t)
    t = re.sub(r'\b__volatile__\b', 'volatile', t)
    t = re.sub(r'\b__const\b', 'const', t)
    t = re.sub(r'\b__signed__\b', 'signed', t)
    t = re.sub(r'\b__extension__\b', '', t)
    t = re.sub(r'\bextern int __objdiff_skipped_asm\s*;', '', t)
    return t


# ---------------------------------------------------------------- types

class Types:
    """Typedef/struct resolution for one translation unit (sizes via tools/structmap.Layout)."""

    def __init__(self, ast, path=None):
        import structmap
        self.lay = structmap.Layout(ast)
        # typedefs the unit itself defines (a header prototype must not use them)
        self.local_typedefs = {e.name for e in ast.ext if isinstance(e, c_ast.Typedef)
                               and (path is None or (e.coord and e.coord.file == path))}

    def base(self, t):
        """Strip TypeDecl/typedefs -> the underlying node (IdentifierType/Struct/PtrDecl/...)."""
        seen = 0
        while True:
            seen += 1
            if seen > 50:
                return t
            if isinstance(t, (c_ast.TypeDecl, c_ast.Typename)):
                t = t.type
                continue
            if isinstance(t, c_ast.IdentifierType) and len(t.names) == 1 and t.names[0] in self.lay.typedefs:
                t = self.lay.typedefs[t.names[0]]
                continue
            return t

    def abi(self, t):
        """Calling-convention class of a type: 'void', 'u8', 's16', 'i32', 'u32', 's32', 'ptr', 'struct:N', ..."""
        b = self.base(t)
        if isinstance(b, (c_ast.PtrDecl, c_ast.ArrayDecl, c_ast.FuncDecl)):
            return 'ptr'
        if isinstance(b, c_ast.Enum):
            return 's32'
        if isinstance(b, (c_ast.Struct, c_ast.Union)):
            try:
                return f'struct:{self.lay.size_align(b)[0]}'
            except Exception:  # noqa: BLE001
                return 'struct:?'
        if isinstance(b, c_ast.IdentifierType):
            names = [x for x in b.names if x not in ('const', 'volatile')]
            if names == ['void']:
                return 'void'
            if 'float' in names:
                return 'f32'
            if 'double' in names:
                return 'f64'
            uns = 'unsigned' in names
            if 'char' in names:
                return 's8' if 'signed' in names else 'u8'
            if 'short' in names:
                return 'u16' if uns else 's16'
            if names.count('long') == 2:
                return 'u64' if uns else 's64'
            return 'u32' if uns else 's32'
        return '?'

    @staticmethod
    def narrow(a):
        return a in ('u8', 's8', 'u16', 's16')


def tstr(t):
    """Type spelling without a declarator name; array parameters decay to pointers."""
    if isinstance(t, c_ast.EllipsisParam):
        return '...'
    s = GEN._generate_type(t, emit_declname=False)
    s = re.sub(r'\{.*\}', '{...}', s, flags=re.S)        # anonymous struct/union bodies
    return re.sub(r'\s+', ' ', s).strip()


def canon_spelling(s):
    """Spell equal C types equally: 'signed int' -> 'int', 'short int' -> 'short', 'unsigned' -> 'unsigned int'."""
    s = re.sub(r'\bsigned (int|short|long)\b', r'\1', s)
    s = re.sub(r'\b(short|long) int\b', r'\1', s)
    s = re.sub(r'\bunsigned(?! (int|short|char|long)\b)', 'unsigned int', s)
    return s


def param_tstr(t):
    s = tstr(t)
    s = re.sub(r'\s*\[[^\]]*\]$', ' *', s)
    return re.sub(r'\s+\*', ' *', s).replace('* *', '**')


def set_declname(t, name):
    while not isinstance(t, c_ast.TypeDecl):
        t = t.type
    t.declname = name


def split_label(name):
    if name and '__asmlabel__' in name:
        a, b = name.split('__asmlabel__', 1)
        return a, b
    return name, name


def expand_typedefs(node, typedefs, names):
    """Deep copy of a type node with the typedef names in `names` replaced by their definitions."""
    import copy
    if isinstance(node, c_ast.TypeDecl) and isinstance(node.type, c_ast.IdentifierType) \
            and len(node.type.names) == 1 and node.type.names[0] in names and node.type.names[0] in typedefs:
        t = copy.deepcopy(typedefs[node.type.names[0]])
        inner = t
        while not isinstance(inner, c_ast.TypeDecl):
            inner = inner.type
        inner.declname = node.declname
        inner.quals = list(node.quals or []) + [q for q in (inner.quals or []) if q not in (node.quals or [])]
        return expand_typedefs(t, typedefs, names)
    if isinstance(node, (c_ast.TypeDecl, c_ast.PtrDecl, c_ast.ArrayDecl, c_ast.Typename, c_ast.Decl)):
        n = copy.copy(node)
        n.type = expand_typedefs(node.type, typedefs, names)
        return n
    if isinstance(node, c_ast.FuncDecl):
        n = copy.copy(node)
        n.type = expand_typedefs(node.type, typedefs, names)
        if node.args is not None:
            n.args = c_ast.ParamList([expand_typedefs(x, typedefs, names) if not isinstance(
                x, (c_ast.EllipsisParam, c_ast.ID)) else x for x in node.args.params])
        return n
    return node


def funcdecl_info(decl, types):
    """decl: c_ast.Decl whose type is FuncDecl -> dict."""
    fd = decl.type
    local, sym = split_label(decl.name)
    ret = fd.type
    params, variadic, proto = [], False, True
    if fd.args is None:
        proto = False
    else:
        for p in fd.args.params:
            if isinstance(p, c_ast.EllipsisParam):
                variadic = True
                continue
            if isinstance(p, c_ast.ID):        # K&R identifier list (types come from the param_decls)
                params.append({'type': 'int', 'name': p.name, 'abi': 's32'})
                continue
            pt = p.type
            if isinstance(p, c_ast.Typename) and tstr(pt) == 'void' and len(fd.args.params) == 1:
                break
            params.append({'type': param_tstr(pt), 'name': getattr(p, 'name', None), 'abi': types.abi(pt),
                           'ctype': canon_spelling(param_tstr(expand_typedefs(pt, types.lay.typedefs,
                                                                              types.lay.typedefs)))})
    # pretty prototype with the symbol name
    import copy
    d2 = copy.deepcopy(decl)
    d2.storage = [s for s in (d2.storage or []) if s not in ('extern',)]
    set_declname(d2.type, sym)
    d2.name = sym
    text = GEN.visit(d2)
    text = re.sub(r'__asmlabel__\w+', '', text)
    d3 = expand_typedefs(d2, types.lay.typedefs, types.local_typedefs)
    htext = re.sub(r'__asmlabel__\w+', '', GEN.visit(d3))
    return {
        'symbol': sym, 'local_name': local, 'returns': tstr(ret), 'ret_abi': types.abi(ret), 'params': params,
        'ret_ctype': canon_spelling(tstr(expand_typedefs(ret, types.lay.typedefs, types.lay.typedefs))),
        'variadic': variadic, 'prototyped': proto, 'storage': list(decl.storage or []),
        'funcspec': list(decl.funcspec or []), 'proto': text, 'hproto': htext,
    }


def compare(local, defn):
    """-> (status, [diff strings]); status per the module docstring."""
    if defn is None:
        return 'asm', []
    if not local['prototyped'] and defn['prototyped'] and (defn['params'] or defn['variadic']):
        return 'unprototyped', ['declared without a parameter list: no narrowing at call sites']
    diffs, status = [], 'same'
    order = {'same': 0, 'type': 1, 'abi': 2, 'arity': 3}

    def bump(s):
        nonlocal status
        if order[s] > order[status]:
            status = s
    if local['prototyped'] and (len(local['params']) != len(defn['params'])
                                or local['variadic'] != defn['variadic']):
        bump('arity')
        diffs.append(f"{len(local['params'])}{'+...' if local['variadic'] else ''} params here, "
                     f"{len(defn['params'])}{'+...' if defn['variadic'] else ''} in the definition")
    la, da = local['ret_abi'], defn['ret_abi']
    if local.get('ret_ctype') == defn.get('ret_ctype') and local['returns'] != defn['returns']:
        diffs.append(f"return {local['returns']} vs {defn['returns']} (same type)")
    elif local['returns'] != defn['returns']:
        if la != da and ((la == 'void') != (da == 'void') or Types.narrow(la) or Types.narrow(da)
                         or {la, da} == {'u32', 's32'} or la.startswith('struct') or da.startswith('struct')):
            bump('abi')
            diffs.append(f"return {local['returns']} [{la}] vs {defn['returns']} [{da}]")
        else:
            bump('type')
            diffs.append(f"return {local['returns']} vs {defn['returns']}")
    if local['prototyped']:
        for i, (lp, dp) in enumerate(zip(local['params'], defn['params'])):
            if lp['type'] == dp['type']:
                continue
            if lp.get('ctype') and lp.get('ctype') == dp.get('ctype'):
                diffs.append(f"param {i + 1}: {lp['type']} vs {dp['type']} (same type)")
                continue
            a, b = lp['abi'], dp['abi']
            if a != b and (Types.narrow(a) or Types.narrow(b) or a.startswith('struct') or b.startswith('struct')
                           or a.startswith('f') or b.startswith('f')):
                bump('abi')
                diffs.append(f"param {i + 1}: {lp['type']} [{a}] vs {dp['type']} [{b}]")
            else:
                bump('type')
                diffs.append(f"param {i + 1}: {lp['type']} vs {dp['type']}")
    return status, diffs


def kr_proto(fdef, info, types):
    """Fill info for an old-style (K&R) definition: real parameter types, and a header prototype that uses
    the promoted types (GCC rejects a prototype whose narrow types differ from the promoted ones)."""
    import copy
    kd = {d.name: d for d in fdef.param_decls}
    plist = []
    for prm in info['params']:
        d = kd.get(prm['name'])
        if d is None:
            d = c_ast.Decl(prm['name'], [], [], [], [], c_ast.TypeDecl(prm['name'], [], None,
                                                                         c_ast.IdentifierType(['int'])), None, None)
        prm['type'], prm['abi'] = param_tstr(d.type), types.abi(d.type)
        prm['ctype'] = canon_spelling(param_tstr(expand_typedefs(d.type, types.lay.typedefs, types.lay.typedefs)))
        plist.append(copy.deepcopy(d))

    def render(params, local_only=False):
        fd = c_ast.FuncDecl(c_ast.ParamList(params), copy.deepcopy(fdef.decl.type.type))
        set_declname(fd, info['symbol'])
        decl = c_ast.Decl(info['symbol'], [], [], [], [], fd, None, None)
        if local_only:
            decl = expand_typedefs(decl, types.lay.typedefs, types.local_typedefs)
        return GEN.visit(decl)
    info['proto'] = render(plist)
    promoted = []
    for prm, d in zip(info['params'], plist):
        if Types.narrow(prm['abi']):
            d = copy.deepcopy(d)
            d.type = c_ast.TypeDecl(d.name, [], None, c_ast.IdentifierType(['int']))
        promoted.append(d)
    info['header_proto'] = render(promoted, True) + '  /* K&R definition: promoted parameter types */'
    info['hproto'] = info['header_proto']


# ---------------------------------------------------------------- per unit

KNOWN = None


def analyse(unit):
    path = f'src/{unit}.c'
    raw = cpp(path)
    t = clean(raw)
    t, bodies = strip_bodies(t)
    t = fold_asm(t)
    t = strip_attrs(t)
    ast = c_parser.CParser().parse(t, path)
    types = Types(ast, path)
    res = {'unit': unit, 'file': path, 'definitions': [], 'decls': [], 'globals': [], 'implicit': []}
    first_decl_line = {}
    for ext in ast.ext:
        if isinstance(ext, c_ast.FuncDef):
            info = funcdecl_info(ext.decl, types)
            info['line'] = ext.decl.coord.line if ext.decl.coord else None
            info['kr'] = ext.param_decls is not None
            if info['kr']:
                # old-style definition: callers see no prototype; the parameter types come from the
                # declaration list. A shared prototype must use the promoted types (u8/u16/s8/s16 -> int).
                kr_proto(ext, info, types)
            res['definitions'].append(info)
            first_decl_line.setdefault(info['symbol'], info['line'])
            continue
        if not isinstance(ext, c_ast.Decl) or not ext.name:
            continue
        f = ext.coord.file if ext.coord else path
        src = 'unit' if f == path else os.path.relpath(f) if not f.startswith('/') else f
        if isinstance(ext.type, c_ast.FuncDecl):
            info = funcdecl_info(ext, types)
            info['line'] = ext.coord.line if ext.coord else None
            info['from'] = src
            res['decls'].append(info)
            if src == 'unit':
                first_decl_line.setdefault(info['symbol'], info['line'])
            continue
        if 'typedef' in (ext.storage or []):
            continue
        local, sym = split_label(ext.name)
        gtype = tstr(ext.type).replace('__asmlabel__', '')
        try:
            size = types.lay.size_align(ext.type)[0]
        except Exception:  # noqa: BLE001
            size = None
        res['globals'].append({'symbol': sym, 'local_name': local, 'type': gtype, 'storage': list(ext.storage or []),
                               'size': size, 'line': ext.coord.line if ext.coord else None, 'from': src})
    # implicit declarations: calls to known functions with no file-scope declaration in the unit or its
    # headers. A block-scope prototype inside a body counts as a declaration (kind 'block_decl').
    declared = {d['symbol'] for d in res['decls']} | {d['symbol'] for d in res['definitions']}
    declared |= {d['local_name'] for d in res['decls']} | {d['local_name'] for d in res['definitions']}
    for fname, body, off in bodies:
        for m in re.finditer(r'(?<![\w.>])([A-Za-z_]\w*)\s*\(', body):
            nm = m.group(1)
            if nm in KNOWN and nm not in declared:
                blk = re.search(r'(?:^|[;{}])\s*(?:extern\s+)?(?:const\s+|volatile\s+|unsigned\s+|signed\s+|struct\s+)*'
                                r'\w+[\s*]+' + re.escape(nm) + r'\s*\([^;{}]*\)\s*;', body)
                res['implicit'].append({'symbol': nm, 'in': fname, 'kind': 'block_decl' if blk else 'undeclared',
                                        'decl': blk.group(0).strip(' \t\n;{}') + ';' if blk else None})
                declared.add(nm)
    return res


# ---------------------------------------------------------------- main

def load_known():
    syms = {}
    for line in open('config/functions.tsv'):
        if line.startswith('#'):
            continue
        f = line.rstrip('\n').split('\t')
        syms[f[3]] = {'addr': f[0], 'mode': f[1], 'size': f[2], 'unit': f[4]}
    return syms


def main():
    global KNOWN
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('units', nargs='*')
    ap.add_argument('-j', type=int, default=os.cpu_count() or 2)
    ap.add_argument('--out', default=OUT_JSON)
    ap.add_argument('--report', default=OUT_REPORT)
    ap.add_argument('--show', metavar='FUNC', help='print one function (reads the existing --out)')
    a = ap.parse_args()
    if a.show:
        db = json.load(open(a.out))
        f = db['functions'].get(a.show)
        if not f:
            sys.exit(f'{a.show}: not in {a.out}')
        print(json.dumps(f, indent=1))
        return
    KNOWN = load_known()
    units = a.units or c_units()
    results, failures = {}, {}
    with concurrent.futures.ProcessPoolExecutor(a.j, initializer=_init) as ex:
        futs = {ex.submit(analyse, u): u for u in units}
        for fu in concurrent.futures.as_completed(futs):
            u = futs[fu]
            try:
                results[u] = fu.result()
            except Exception as e:  # noqa: BLE001
                failures[u] = str(e)[:400]
    old = {}
    if a.units and os.path.exists(a.out):
        old = json.load(open(a.out)).get('_units_raw', {})
    old.update(results)
    db = build(old, failures)
    os.makedirs(os.path.dirname(a.out), exist_ok=True)
    json.dump(db, open(a.out, 'w'), indent=1)
    report(db, a.report)
    s = db['summary']
    print(f"{s['units_parsed']}/{s['units']} units parsed, {s['functions_defined']} C definitions "
          f"({s['static_definitions']} static), {s['asm_functions']} asm-only functions; local declarations: "
          + ', '.join(f'{k} {v}' for k, v in sorted(s['local_decl_status'].items()))
          + f"; implicit calls {s['implicit_calls']}")
    for u, e in sorted(failures.items()):
        print(f'  PARSE FAILURE {u}: {e}', file=sys.stderr)
    print(f'wrote {a.out} and {a.report}')


def _init():
    global KNOWN
    os.chdir(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    KNOWN = load_known()


def build(raw, failures):
    known = load_known()
    syms = target.known_symbols()
    funcs = {}
    statics = collections.defaultdict(list)
    for u, r in sorted(raw.items()):
        for d in r['definitions']:
            if 'static' in d['storage']:
                statics[d['symbol']].append({'unit': u, 'line': d['line'], 'proto': d['proto'],
                                             'inline': 'inline' in d['funcspec']})
                continue
            k = known.get(d['symbol'], {})
            funcs[d['symbol']] = {
                'addr': k.get('addr'), 'unit': u, 'line': d['line'], 'proto': d['proto'], 'returns': d['returns'],
                'params': [{'type': p['type'], 'name': p['name'], 'abi': p['abi'], 'ctype': p.get('ctype')}
                           for p in d['params']],
                'ret_ctype': d.get('ret_ctype'), 'hproto': d.get('hproto', d['proto']),
                'variadic': d['variadic'], 'prototyped': d['prototyped'], 'kr': d.get('kr', False),
                'header_proto': d.get('header_proto', d['proto']),
                'ret_abi': d['ret_abi'], 'views': [],
            }
    # functions defined in assembly (crt0, mixer, BIOS stubs, libgcc...): from functions.tsv
    asm_funcs = {n: k for n, k in known.items() if n not in funcs}
    units = {}
    status_count = collections.Counter()
    globals_ = collections.defaultdict(list)
    implicit_total = 0
    for u, r in sorted(raw.items()):
        decls = []
        for d in r['decls']:
            if d['from'] != 'unit':
                continue
            if 'static' in d['storage'] and d['symbol'] in {x['symbol'] for x in r['definitions']}:
                continue                               # forward declaration of a unit-local static
            defn = funcs.get(d['symbol'])
            if defn is None and d['symbol'] in statics and any(s['unit'] == u for s in statics[d['symbol']]):
                continue
            st, diffs = compare(d, defn)
            if defn and defn['unit'] == u and st == 'same':
                st = 'same'                            # own forward declaration
            entry = {'symbol': d['symbol'], 'local_name': d['local_name'], 'line': d['line'], 'proto': d['proto'],
                     'status': st, 'diffs': diffs, 'defined_in': defn['unit'] if defn else
                     ('asm:' + asm_funcs[d['symbol']]['unit'] if d['symbol'] in asm_funcs else None)}
            if d['local_name'] != d['symbol']:
                entry['alias'] = True
            decls.append(entry)
            status_count[st] += 1
            if defn is not None and defn['unit'] != u:
                defn['views'].append({'unit': u, 'line': d['line'], 'status': st, 'proto': d['proto'],
                                      'local_name': d['local_name']})
        hdr = collections.Counter(d['from'] for d in r['decls'] if d['from'] != 'unit')
        for g in r['globals']:
            if g['from'] != 'unit':
                continue
            addr = target.resolve_symbol(g['symbol'], syms)
            globals_[g['symbol']].append({'unit': u, 'local_name': g['local_name'], 'type': g['type'],
                                          'size': g['size'], 'line': g['line'],
                                          'defines': 'extern' not in g['storage'],
                                          'addr': f'0x{addr[0]:08X}' if addr else None})
        implicit_total += len(r['implicit'])
        units[u] = {'file': r['file'], 'definitions': [d['symbol'] for d in r['definitions']
                                                       if 'static' not in d['storage']],
                    'statics': [d['symbol'] for d in r['definitions'] if 'static' in d['storage']],
                    'local_decls': decls, 'implicit': r['implicit'],
                    'header_decls': dict(hdr), 'error': None}
    for u, e in failures.items():
        units[u] = {'error': e}
    asm_out = {}
    for n, k in sorted(asm_funcs.items()):
        views = []
        for u, info in units.items():
            for d in info.get('local_decls', []):
                if d['symbol'] == n:
                    views.append({'unit': u, 'line': d['line'], 'proto': d['proto'], 'local_name': d['local_name']})
        asm_out[n] = {'addr': k['addr'], 'unit': k['unit'], 'views': views}
    summary = {
        'units': len(raw) + len(failures), 'units_parsed': len(raw), 'functions_defined': len(funcs),
        'static_definitions': sum(len(v) for v in statics.values()), 'asm_functions': len(asm_out),
        'local_decl_status': dict(status_count), 'implicit_calls': implicit_total,
        'functions_with_abi_views': sum(1 for f in funcs.values()
                                        if any(v['status'] in ('abi', 'unprototyped', 'arity') for v in f['views'])),
    }
    gl = {}
    for s, decls in sorted(globals_.items()):
        types_ = collections.Counter(d['type'] for d in decls)
        gl[s] = {'addr': next((d['addr'] for d in decls if d['addr']), None),
                 'types': dict(types_.most_common()), 'units': len({d['unit'] for d in decls}), 'decls': decls}
    return {'generated': datetime.date.today().isoformat(), 'tool': 'tools/genprotos.py', 'summary': summary,
            'functions': funcs, 'asm_functions': asm_out, 'statics': dict(statics), 'units': units,
            'globals': gl, '_units_raw': raw}


def report(db, path):
    L = []
    s = db['summary']
    L.append('# Local prototypes that differ from the definition (tools/genprotos.py, '
             f"{db['generated']})")
    L.append('# status: abi = the shared prototype changes the caller (narrowing/sign/void); unprototyped = `T f()`;')
    L.append('# arity = parameter count differs; type = spelling only (safe). Keep abi/unprototyped/arity views')
    L.append('# as local prototypes (or casts) with a comment when the unit adopts the shared header.')
    L.append(f"# {s['local_decl_status']}; implicit calls: {s['implicit_calls']}")
    L.append('')
    order = ['abi', 'unprototyped', 'arity', 'type']
    for st in order:
        rows = []
        for u, info in sorted(db['units'].items()):
            for d in info.get('local_decls', []):
                if d['status'] == st:
                    rows.append((u, d))
        L.append(f'## {st} ({len(rows)})')
        for u, d in rows:
            defn = db['functions'].get(d['symbol'])
            L.append(f"{u}:{d['line']}: {d['proto']}")
            if defn:
                L.append(f"    definition {defn['unit']}:{defn['line']}: {defn['proto']}")
            for x in d['diffs']:
                L.append(f'    - {x}')
        L.append('')
    imp = [(u, x) for u, info in sorted(db['units'].items()) for x in info.get('implicit', [])]
    L.append(f'## implicit (called without any declaration in the unit: implicit `int f()`) ({len(imp)})')
    for u, x in imp:
        defn = db['functions'].get(x['symbol'])
        L.append(f"{u}: {x['symbol']} (in {x['in']})" + (f"  definition: {defn['proto']}" if defn else '  (asm)'))
    L.append('')
    multi = {n: v for n, v in db['statics'].items() if len({x['unit'] for x in v}) > 1}
    L.append(f'## static helpers defined in more than one unit ({len(multi)}; candidates for shared inlines/macros,'
             ' but each copy may be a matching choice)')
    for n, v in sorted(multi.items(), key=lambda kv: -len(kv[1])):
        L.append(f"{n} x{len(v)}: " + ', '.join(sorted({x['unit'] for x in v})))
    L.append('')
    gconf = {n: g for n, g in db['globals'].items() if len(g['types']) > 1}
    L.append(f'## globals declared with more than one type ({len(gconf)})')
    for n, g in sorted(gconf.items(), key=lambda kv: -len(kv[1]['types'])):
        L.append(f"{n} ({g['addr']}): " + '; '.join(f'{t} x{c}' for t, c in g['types'].items()))
    open(path, 'w').write('\n'.join(L) + '\n')


if __name__ == '__main__':
    main()
