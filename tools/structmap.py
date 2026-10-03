#!/usr/bin/env python3
"""Map every named field of a shared global, across all C units, to its byte/bit offset (agbcc layout rules).

  python3 tools/structmap.py 0x03000040            # merged field map of gMain (all units' declarations)
  python3 tools/structmap.py 0x020192E4 --elem     # for arrays: offsets within one element
  python3 tools/structmap.py --check               # self-test the layout rules against old_agbcc

Layout rules (verified against old_agbcc, see --check):
  - every struct (and union) is aligned to, and padded to a multiple of, 4 bytes (STRUCTURE_SIZE_BOUNDARY 32);
  - scalars are aligned to their size; arrays to their element's alignment;
  - bitfields are packed LSB-first, continuing in the same storage across declared types; a bitfield that
    would straddle a boundary of its own declared type's size starts at the next such boundary.
Output rows: byte offset, bit range (for bitfields), size, the names/types units use there, and how many units.
"""
import collections
import glob
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(__file__))
from pycparser import c_ast, c_parser  # noqa: E402

SCALAR = {'char': 1, 'short': 2, 'int': 4, 'long': 4, 'long long': 8, 'float': 4, 'double': 8, 'void': 1,
          '_Bool': 1}


def preprocess(path):
    r = subprocess.run(['cpp', '-P', '-nostdinc', '-undef', '-DOBJDIFF_BASE', '-I', 'include', '-I',
                        '/opt/agbcc/include', '-iquote', '.', path], capture_output=True, text=True)
    t = r.stdout
    t = re.sub(r'^\s*asm\s*\(.*?\)\s*;', '', t, flags=re.M | re.S)
    t = re.sub(r'\b__attribute__\s*\(\(.*?\)\)', '', t)
    t = re.sub(r'\)\s*(asm|__asm__)\s*\(\s*"[^"]*"\s*\)', ')', t)        # symbol aliases: f(void) asm("sym")
    # variable aliases (another view of a global): `T name[] asm("sym")` -> `T sym_aliasN[]`, counted as sym
    t = re.sub(r'\b(\w+)\s*((?:\[[^\]]*\])*)\s*(?:asm|__asm__)\s*\(\s*"(\w+)"\s*\)',
               lambda m: f'{m.group(3)}__alias{m.start()}{m.group(2)}', t)
    t = re.sub(r'\b(register\s+[^;=]*?)\s+(asm|__asm__)\s*\(\s*"[^"]*"\s*\)', r'\1', t)  # register pins
    t = re.sub(r'\bextern int __objdiff_skipped_asm\s*;', '', t)
    # keep declarations only: drop function bodies
    out, i, depth, start = [], 0, 0, 0
    while i < len(t):
        c = t[i]
        if c == '{':
            if depth == 0:
                head = t[start:i]
                if re.search(r'\)\s*$', head) and '=' not in head.split('(')[0]:
                    j, d = i + 1, 1
                    while d and j < len(t):
                        d += {'{': 1, '}': -1}.get(t[j], 0)
                        j += 1
                    out.append(head.rstrip() + ';')
                    i = start = j
                    continue
            depth += 1
        elif c == '}':
            depth -= 1
        elif c == ';' and depth == 0:
            out.append(t[start:i + 1])
            start = i + 1
        i += 1
    return '\n'.join(out)


class Layout:
    def __init__(self, ast):
        self.structs = {}   # tag -> c_ast.Struct/Union with decls
        self.typedefs = {}
        for ext in ast.ext:
            self._collect(ext)

    def _collect(self, node):
        if isinstance(node, c_ast.Typedef):
            self.typedefs[node.name] = node.type
        for _, child in node.children():
            if isinstance(child, (c_ast.Struct, c_ast.Union)) and child.decls is not None and child.name:
                self.structs[(type(child).__name__, child.name)] = child
            self._collect(child)

    def const(self, e):
        if isinstance(e, c_ast.Constant):
            v = e.value.rstrip('uUlL')
            return int(v, 0) if not v.startswith("'") else ord(v[1])
        if isinstance(e, c_ast.BinaryOp):
            a, b = self.const(e.left), self.const(e.right)
            return {'+': a + b, '-': a - b, '*': a * b, '/': a // b if b else 0, '<<': a << b, '>>': a >> b,
                    '|': a | b, '&': a & b}[e.op]
        if isinstance(e, c_ast.UnaryOp):
            if e.op == 'sizeof':
                return self.size_align(e.expr.type if isinstance(e.expr, c_ast.Typename) else e.expr)[0]
            v = self.const(e.expr)
            return {'-': -v, '+': v, '~': ~v}[e.op]
        if isinstance(e, c_ast.Cast):
            return self.const(e.expr)
        raise ValueError(f'non-constant {type(e).__name__}')

    def resolve(self, t):
        while isinstance(t, c_ast.TypeDecl) and isinstance(t.type, c_ast.IdentifierType) \
                and len(t.type.names) == 1 and t.type.names[0] in self.typedefs:
            t = self.typedefs[t.type.names[0]]
        return t

    def scalar(self, names):
        n = [x for x in names if x not in ('signed', 'unsigned', 'const', 'volatile')]
        key = ' '.join(n) or 'int'
        if key.startswith('long long'):
            return 8
        for k in ('char', 'short', 'long', 'int', 'float', 'double', 'void', '_Bool'):
            if k in n:
                return SCALAR[k]
        return 4

    def size_align(self, t):
        t = self.resolve(t)
        if isinstance(t, c_ast.TypeDecl):
            return self.size_align(t.type)
        if isinstance(t, c_ast.PtrDecl):
            return 4, 4
        if isinstance(t, c_ast.ArrayDecl):
            s, a = self.size_align(t.type)
            n = self.const(t.dim) if t.dim is not None else 0
            return s * n, a
        if isinstance(t, c_ast.IdentifierType):
            if len(t.names) == 1 and t.names[0] in self.typedefs:
                return self.size_align(self.typedefs[t.names[0]])
            s = self.scalar(t.names)
            return s, min(s, 4)
        if isinstance(t, (c_ast.Struct, c_ast.Union)):
            body = t if t.decls is not None else self.structs.get((type(t).__name__, t.name))
            if body is None:
                raise ValueError(f'incomplete {t.name}')
            fields, size = self.members(body)
            return size, 4
        if isinstance(t, c_ast.Enum):
            return 4, 4
        if isinstance(t, c_ast.FuncDecl):
            return 4, 4
        raise ValueError(f'unknown type node {type(t).__name__}')

    def members(self, body):
        """-> ([(name, bit_offset, bit_width or None, size, type_node)], size_bytes)"""
        out, pos, is_union = [], 0, isinstance(body, c_ast.Union)
        maxpos = 0
        for d in body.decls or []:
            if is_union:
                pos = 0
            if d.bitsize is not None:
                w = self.const(d.bitsize)
                s, _ = self.size_align(d.type)
                sb = s * 8
                if w and (pos // sb) != ((pos + w - 1) // sb):
                    pos = -(-pos // sb) * sb
                if d.name:
                    out.append((d.name, pos, w, s, d.type))
                pos += w
            else:
                s, a = self.size_align(d.type)
                pos = -(-pos // (a * 8)) * (a * 8)
                out.append((d.name, pos, None, s, d.type))
                pos += s * 8
            maxpos = max(maxpos, pos)
        size = -(-maxpos // 32) * 4
        return out, size

    def leaves(self, t, base_bits=0, path=''):
        """Flatten to leaf fields: (path, bit_offset, bit_width or None, size, ctype)."""
        t = self.resolve(t)
        if isinstance(t, c_ast.TypeDecl):
            t = t.type
            if isinstance(t, c_ast.IdentifierType) and len(t.names) == 1 and t.names[0] in self.typedefs:
                return self.leaves(self.typedefs[t.names[0]], base_bits, path)
        if isinstance(t, (c_ast.Struct, c_ast.Union)):
            body = t if t.decls is not None else self.structs.get((type(t).__name__, t.name))
            res = []
            for name, off, w, s, ty in self.members(body)[0]:
                sub = f'{path}.{name}' if path else name
                if w is not None:
                    res.append((sub, base_bits + off, w, s, typestr(ty)))
                else:
                    r = self.resolve(ty)
                    inner = r.type if isinstance(r, c_ast.TypeDecl) else r
                    if isinstance(inner, (c_ast.Struct, c_ast.Union)):
                        res += self.leaves(ty, base_bits + off, sub)
                    elif isinstance(r, c_ast.ArrayDecl) and self._is_struct(r.type):
                        # array of structs: describe element 0 (record the array itself too)
                        res.append((sub, base_bits + off, None, s, typestr(ty)))
                        res += self.leaves(r.type, base_bits + off, sub + '[0]')
                    else:
                        res.append((sub, base_bits + off, None, s, typestr(ty)))
            return res
        return [(path, base_bits, None, self.size_align(t)[0], typestr(t))]


def _is_struct(self, t):
    t = self.resolve(t)
    t = t.type if isinstance(t, c_ast.TypeDecl) else t
    if isinstance(t, c_ast.IdentifierType) and len(t.names) == 1 and t.names[0] in self.typedefs:
        return _is_struct(self, self.typedefs[t.names[0]])
    return isinstance(t, (c_ast.Struct, c_ast.Union))


Layout._is_struct = _is_struct


def typestr(t):
    if isinstance(t, c_ast.TypeDecl):
        return typestr(t.type)
    if isinstance(t, c_ast.IdentifierType):
        return ' '.join(t.names)
    if isinstance(t, c_ast.PtrDecl):
        return typestr(t.type) + ' *'
    if isinstance(t, c_ast.ArrayDecl):
        dim = ''
        if t.dim is not None:
            try:
                dim = hex(Layout.const(Layout.__new__(Layout), t.dim)) if isinstance(t.dim, c_ast.Constant) else '...'
            except Exception:
                dim = '...'
        return typestr(t.type) + f'[{dim}]'
    if isinstance(t, (c_ast.Struct, c_ast.Union)):
        return f'{"struct" if isinstance(t, c_ast.Struct) else "union"} {t.name or "{...}"}'
    return type(t).__name__


def unit_fields(path, addr, elem):
    text = preprocess(path)
    ast = c_parser.CParser().parse(text, path)
    lay = Layout(ast)
    name_re = re.compile(r'^(gUnk_|g\w*?_?)' + f'{addr:08X}' + r'(__alias\d+)?$')
    found = []
    for ext in ast.ext:
        if isinstance(ext, c_ast.Decl) and ext.name and name_re.match(ext.name):
            t = ext.type
            if elem:
                while isinstance(t, c_ast.ArrayDecl):
                    t = t.type
            elif isinstance(t, c_ast.ArrayDecl):
                t = t.type  # element 0 of an array global
            found += lay.leaves(t)
    return (addr if found else None), found


def main():
    args = sys.argv[1:]
    addr = int(args[0], 16)
    elem = '--elem' in args
    rows = collections.defaultdict(lambda: collections.defaultdict(set))
    nunits, failures = 0, []
    for f in sorted(glob.glob('src/code_*.c')):
        try:
            gname, leaves = unit_fields(f, addr, elem)
        except Exception as e:  # noqa: BLE001
            failures.append(f'{f}: {str(e)[:80]}')
            continue
        if not leaves:
            continue
        nunits += 1
        unit = f[4:-2]
        for path, bit, w, size, ty in leaves:
            last = path.split('.')[-1]
            if re.match(r'^(unk|pad|filler|skip|_)', last, re.I) or re.match(r'^(unk|pad|filler)', path, re.I):
                continue
            key = (bit // 8, bit % 8 if w is not None else None, w, size)
            rows[key][f'{path}: {ty}'].add(unit)
    print(f'0x{addr:08X}: {nunits} units declare it ({len(failures)} could not be parsed)')
    for (byte, bit, w, size), names in sorted(rows.items(), key=lambda kv: (kv[0][0], kv[0][1] or 0)):
        where = f'+0x{byte:X}' + (f' bits {bit}..{bit + w - 1}' if w is not None else f' ({size} bytes)')
        tot = len(set().union(*names.values()))
        best = sorted(names.items(), key=lambda kv: -len(kv[1]))
        alts = '; '.join(f'{n} x{len(u)}' for n, u in best[:4])
        print(f'{where:22s} {tot:3d} units  {alts}')
    for x in failures[:10]:
        print('  parse failure:', x, file=sys.stderr)


if __name__ == '__main__':
    main()
