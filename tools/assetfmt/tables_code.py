"""Code tables and the cartridge header: data that the game code reads directly, as JSON.

  tables_code_header    cartridge header after the entry branch (0x08000004-0x080000C0)
                        -> <path>.json (title, codes, version, checksum "auto") + <path>.logo.bin
  tables_code_layout    a mixed .rodata range: C string literals and the small const tables around them
                        -> <path>/strings.json, <path>/tables.json and the files tables.json includes
  tables_code_effects   card-effect dispatch table, 426 x {u16 number; u16 flags; fn[5]}
                        -> <path>.json, one row per entry, handlers by function name

Layout files are JSON lists of items, each placed at its ROM address ("addr"); bytes no item covers are
zero. A string may grow into the zero padding up to the next item. A table has a "type" in the spec
language below and "values". Pointers are written "0x08XXXXXX <what it points to>" and only the first word
is read back; function pointers are function names (the Thumb bit comes from config/functions.tsv).
"label" (the symbol the C code uses), "name" and keys starting with "_" are comments the build ignores.
Formats are described in build/assetwf/tables_code/NOTES.md.
"""
import bisect
import json
import os
import re
import struct

A = None  # tools/assets.py, set by register()
BASE = 0x08000000
ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))


def register(assets_module):
    global A
    A = assets_module
    return {
        'tables_code_header': (x_header, b_header),
        'tables_code_layout': (x_layout, b_layout),
        'tables_code_effects': (x_effects, b_effects),
    }


# ============================================================================================ symbols
_FUNCS = None


def funcs():
    """(addr -> (mode, name), name -> (addr, mode)) from config/functions.tsv (empty if it is missing)."""
    global _FUNCS
    if _FUNCS is None:
        by_addr, by_name = {}, {}
        try:
            for line in open(os.path.join(ROOT, 'config', 'functions.tsv')):
                if line.startswith('#') or not line.strip():
                    continue
                f = line.rstrip('\n').split('\t')
                by_addr[int(f[0], 16)] = (f[1], f[3])
                by_name[f[3]] = (int(f[0], 16), f[1])
        except FileNotFoundError:
            pass
        _FUNCS = (by_addr, by_name)
    return _FUNCS


def fn_name(v):
    """Function pointer -> the function's name when it points exactly at a known function (with the Thumb
    bit for a Thumb function), else "0x..."; NULL -> None."""
    if v == 0:
        return None
    ent = funcs()[0].get(v & ~1)
    if ent and (ent[0] == 't') == bool(v & 1):
        return ent[1]
    return '0x%08X' % v


def resolve(tok):
    """A symbol in a value field -> its address. Functions get the Thumb bit (sub_XXXXXXXX works without
    config/functions.tsv, and keeps working after the function is renamed)."""
    ent = funcs()[1].get(tok)
    if ent:
        return ent[0] | (1 if ent[1] == 't' else 0)
    m = re.fullmatch(r'(sub|gUnk)_([0-9A-Fa-f]{8})', tok)
    if m:
        a = int(m.group(2), 16)
        if m.group(1) == 'sub':
            return a | (1 if funcs()[0].get(a, ('t',))[0] == 't' else 0)
        return a
    raise ValueError(f'unknown symbol {tok!r} (write 0x..., sub_XXXXXXXX or a name from config/functions.tsv)')


def num(v):
    """JSON value -> int. A string is read up to its first space ("0x...", a decimal number or a symbol);
    the rest is a comment. null is 0."""
    if v is None:
        return 0
    if isinstance(v, bool):
        return int(v)
    if isinstance(v, int):
        return v
    if isinstance(v, str) and v.split():
        tok = v.split()[0]
        if re.fullmatch(r'-?(0x[0-9A-Fa-f]+|\d+)', tok):
            return int(tok, 0)
        return resolve(tok)
    raise ValueError(f'expected a number, got {v!r}')


# ========================================================================================= spec language
# spec   := atom ('[' N ']')*              C order: u8[6][2] is 6 rows of 2
# atom   := u8 s8 u16 s16 u32 s32          integers (decimal in JSON)
#         | x8 x16 x32                     unsigned, written "0x.."
#         | ptr                            u32 pointer: null or "0x08XXXXXX <comment>"
#         | fn                             u32 function pointer: null, a function name, or "0x.."
#         | card | cardnum | duelist       u16 card ID / card number / character ID, written "N <name>"
#         | sjis16                         u16 Shift-JIS code (high byte = lead byte), written as the character
#         | char[N]                        fixed text field (NUL-padded, Windows-1252)
#         | bytes[N]                       raw bytes as hex
#         | '{' field (',' field)* '}'     struct; field := name ':' spec; name '_' merges the fields of a
#                                          bits group or struct into this object
#         | bitsN '(' name ':' width (',' name ':' width)* ')'   N = 8/16/32, fields from bit 0 up
#         | a name from TYPEDEFS           (OamTemplate, AnimStep)
# A field name ending in '?' is left out of the JSON when it is 0 (and defaults to 0 when missing).
_INTS = {'u8': (1, False), 's8': (1, True), 'u16': (2, False), 's16': (2, True), 'u32': (4, False),
         's32': (4, True), 'x8': (1, False), 'x16': (2, False), 'x32': (4, False)}
_REFS = ('card', 'cardnum', 'duelist')
_SPEC_CACHE = {}


def parse_spec(s):
    if s in _SPEC_CACHE:
        return _SPEC_CACHE[s]
    toks = re.findall(r'0x[0-9A-Fa-f]+|\d+|[A-Za-z_]\w*|[{}\[\](),:?]', s)
    if ''.join(toks) != re.sub(r'\s+', '', s):
        raise ValueError(f'bad type spec {s!r}')
    pos = [0]

    def peek():
        return toks[pos[0]] if pos[0] < len(toks) else None

    def take(want=None):
        t = peek()
        if t is None or (want is not None and t != want):
            raise ValueError(f'bad type spec {s!r}: expected {want or "more"} at {t!r}')
        pos[0] += 1
        return t

    def name():
        n = take()
        opt = peek() == '?'
        if opt:
            take()
        return n, opt

    def bits():
        size = int(take()[4:]) // 8
        take('(')
        fields = []
        while True:
            n, opt = name()
            take(':')
            fields.append((n, int(take(), 0), opt))
            if peek() != ',':
                break
            take()
        take(')')
        if sum(w for _, w, _ in fields) > size * 8:
            raise ValueError(f'bad type spec {s!r}: the bit fields do not fit')
        return ('bits', size, fields)

    def atom():
        t = take()
        if t == '{':
            fields = []
            while True:
                n, opt = name()
                take(':')
                f = spec()
                if n == '_' and f[0] not in ('bits', 'struct'):
                    raise ValueError(f'bad type spec {s!r}: only bits and structs merge with _')
                fields.append((None if n == '_' else n, f, opt))
                if peek() != ',':
                    break
                take()
            take('}')
            return ('struct', fields)
        if re.fullmatch(r'bits(8|16|32)', t):
            pos[0] -= 1
            return bits()
        if t in ('char', 'bytes'):
            take('[')
            n = int(take(), 0)
            take(']')
            return (t, n)
        if t in _INTS:
            return ('int', t)
        if t in ('ptr', 'fn', 'sjis16') or t in _REFS:
            return (t,)
        if t in TYPEDEFS:
            return parse_spec(TYPEDEFS[t])
        raise ValueError(f'bad type spec {s!r}: unknown type {t!r}')

    def spec():
        a = atom()
        dims = []
        while peek() == '[':
            take()
            dims.append(int(take(), 0))
            take(']')
        for n in reversed(dims):
            a = ('array', a, n)
        return a

    r = spec()
    if pos[0] != len(toks):
        raise ValueError(f'bad type spec {s!r}: unexpected {toks[pos[0]]!r}')
    _SPEC_CACHE[s] = r
    return r


def spec_size(sp):
    k = sp[0]
    if k == 'int':
        return _INTS[sp[1]][0]
    if k in ('ptr', 'fn'):
        return 4
    if k in _REFS or k == 'sjis16':
        return 2
    if k in ('char', 'bytes', 'bits'):
        return sp[1]
    if k == 'struct':
        return sum(spec_size(f) for _, f, _ in sp[1])
    return spec_size(sp[1]) * sp[2]  # array


def ptr_offsets(sp, off=0):
    """Offsets of the ptr fields in one element (for collecting pointer targets)."""
    k = sp[0]
    if k == 'ptr':
        return [off]
    if k == 'struct':
        out = []
        for _, f, _ in sp[1]:
            out += ptr_offsets(f, off)
            off += spec_size(f)
        return out
    if k == 'array':
        n = spec_size(sp[1])
        return [o for i in range(sp[2]) for o in ptr_offsets(sp[1], off + i * n)]
    return []


def sjis(b, need_wide=False):
    """Shift-JIS text that round-trips exactly (and has a 2-byte character if need_wide), or None."""
    try:
        t = b.decode('shift_jis')
        if t.encode('shift_jis') != b:
            return None
    except (UnicodeDecodeError, UnicodeEncodeError):
        return None
    if any(ord(c) < 0x20 and c not in '\t\n' for c in t):
        return None
    if need_wide and not any(len(c.encode('shift_jis')) == 2 for c in t):
        return None
    return t


def decode(sp, b, o, ctx):
    k = sp[0]
    if k == 'int':
        size, signed = _INTS[sp[1]]
        v = int.from_bytes(b[o:o + size], 'little', signed=signed)
        return ('0x%0*X' % (size * 2, v)) if sp[1][0] == 'x' else v
    if k == 'ptr':
        v = struct.unpack_from('<I', b, o)[0]
        if v == 0:
            return None
        d = ctx.describe(v)
        return '0x%08X' % v + (' ' + d if d else '')
    if k == 'fn':
        return fn_name(struct.unpack_from('<I', b, o)[0])
    if k in _REFS:
        v = struct.unpack_from('<H', b, o)[0]
        name = {'card': ctx.card_name, 'cardnum': ctx.card_by_number, 'duelist': ctx.duelist}[k](v)
        return f'{v} {name}' if name else v
    if k == 'sjis16':
        v = struct.unpack_from('<H', b, o)[0]
        t = sjis(bytes([v >> 8, v & 0xFF]) if v > 0xFF else bytes([v]))
        return t if t and len(t) == 1 and not t.isspace() else '0x%04X' % v
    if k == 'char':
        return A.text_field(b[o:o + sp[1]])
    if k == 'bytes':
        return b[o:o + sp[1]].hex()
    if k == 'bits':
        v = int.from_bytes(b[o:o + sp[1]], 'little')
        out, sh = {}, 0
        for n, w, opt in sp[2]:
            f = (v >> sh) & ((1 << w) - 1)
            if f or not opt:
                out[n] = f
            sh += w
        if v >> sh:
            raise ValueError('bits set outside the declared fields')
        return out
    if k == 'struct':
        out = {}
        for n, f, opt in sp[1]:
            v = decode(f, b, o, ctx)
            if n is None:
                out.update(v)
            elif v or not opt:
                out[n] = v
            o += spec_size(f)
        return out
    n = spec_size(sp[1])  # array
    return [decode(sp[1], b, o + i * n, ctx) for i in range(sp[2])]


def encode(sp, v):
    k = sp[0]
    if k == 'int':
        size, signed = _INTS[sp[1]]
        x = num(v)
        lo, hi = (-(1 << (8 * size - 1)), 1 << (8 * size - 1)) if signed else (0, 1 << (8 * size))
        if not lo <= x < hi:
            raise ValueError(f'{v!r} does not fit in {sp[1]}')
        return x.to_bytes(size, 'little', signed=signed)
    if k in ('ptr', 'fn'):
        x = num(v)
        if not 0 <= x < 1 << 32:
            raise ValueError(f'{v!r} is not a pointer')
        return struct.pack('<I', x)
    if k in _REFS:
        x = num(v)
        if not 0 <= x < 0x10000:
            raise ValueError(f'{v!r} does not fit in u16')
        return struct.pack('<H', x)
    if k == 'sjis16':
        if isinstance(v, str) and len(v) == 1:
            e = v.encode('shift_jis')
            x = e[0] << 8 | e[1] if len(e) == 2 else e[0]
        else:
            x = num(v)
        return struct.pack('<H', x)
    if k == 'char':
        return A.field_bytes(v, sp[1])
    if k == 'bytes':
        r = bytes.fromhex(v)
        if len(r) != sp[1]:
            raise ValueError(f'{len(r)} bytes given, {sp[1]} expected')
        return r
    if not isinstance(v, (dict, list)):
        raise ValueError(f'expected an object or list, got {v!r}')
    if k == 'bits':
        x, sh = 0, 0
        for n, w, opt in sp[2]:
            if n not in v and not opt:
                raise ValueError(f'missing field {n!r}')
            f = num(v.get(n, 0))
            if not 0 <= f < 1 << w:
                raise ValueError(f'{n}={f} does not fit in {w} bits')
            x |= f << sh
            sh += w
        return x.to_bytes(sp[1], 'little')
    if k == 'struct':
        out = b''
        for n, f, opt in sp[1]:
            if n is None:
                out += encode(f, v)
            elif n in v:
                out += encode(f, v[n])
            elif opt:
                out += bytes(spec_size(f))
            else:
                raise ValueError(f'missing field {n!r}')
        return out
    if len(v) != sp[2]:  # array
        raise ValueError(f'expected a list of {sp[2]}, got {len(v)}')
    return b''.join(encode(sp[1], x) for x in v)


# ------------------------------------------------------------------------------------------ JSON out
def _j(v):
    return json.dumps(v, ensure_ascii=False)


def dump_items(items):
    """A list of items, one per line; a table's values one element per line (numbers 16 per line)."""
    out = ['[']
    for n, it in enumerate(items):
        sep = ',' if n < len(items) - 1 else ''
        if 'values' not in it:
            out.append(_j(it) + sep)
            continue
        head = {k: v for k, v in it.items() if k != 'values'}
        vals = it['values']
        out.append(_j(head)[:-1] + ', "values": [')
        if all(isinstance(x, int) or x is None for x in vals):
            rows = [', '.join(_j(x) for x in vals[i:i + 16]) for i in range(0, len(vals), 16)]
        else:
            rows = [_j(x) for x in vals]
        out += [' ' + r + (',' if i < len(rows) - 1 else '') for i, r in enumerate(rows)]
        out.append(']}' + sep)
    out.append(']')
    return ('\n'.join(out) + '\n').encode()


# ======================================================================================== ROM context
class Ctx:
    """Lookups used while extracting: names for comments, labels, pointer targets."""

    def __init__(self, rom):
        self.rom = rom
        self._labels = self._assets = self._duelists = self._targets = self._where = None

    def u16(self, a):
        return struct.unpack_from('<H', self.rom, a - BASE)[0]

    def u32(self, a):
        return struct.unpack_from('<I', self.rom, a - BASE)[0]

    def card_name(self, cid):
        if self.rom is None or not 0 < cid < 821:
            return None
        o = 0x0822C720 - BASE + cid * 0x40  # cards/names.json
        return A.dec(self.rom[o:o + 0x40].split(b'\0')[0]) or None

    def card_by_number(self, n):
        if self.rom is None or not 0 <= n < 2048:
            return None
        return self.card_name(self.u16(0x08623DF4 + 2 * n))  # cards/number_to_id.json

    def duelist(self, i):
        if self.rom is None:
            return None
        if self._duelists is None:
            self._duelists = {}
            for r in range(28):  # text/duelists.json
                o = 0x08139F64 - BASE + r * 0x84
                did = struct.unpack_from('<I', self.rom, o)[0]
                self._duelists.setdefault(did, A.dec(self.rom[o + 0x44:o + 0x84].split(b'\0')[0]))
        return self._duelists.get(i) or None

    def labels(self):
        """Addresses the C code refers to by symbol (the labels of data/rodata_*.s)."""
        if self._labels is None:
            self._labels = set()
            for unit in A.DATA_UNITS:
                try:
                    self._labels |= {a for _, a in A.labels_of(os.path.join(ROOT, 'data', unit + '.s'))}
                except OSError:
                    pass
        return self._labels

    def targets(self):
        """Addresses that pointer tables point to: every ptr field of the schema, plus the words of the
        other code-table ranges (tables/pointer_tables and .rodata 2)."""
        if self._targets is None:
            t = set()
            if self.rom is not None:
                for a, e in schema(self).items():
                    sp = parse_spec(e['type']) if e.get('type') not in (None, 'strings') else None
                    if sp is None or e.get('count') is None:
                        continue
                    size, offs = spec_size(sp), ptr_offsets(sp)
                    for i in range(e['count']):
                        t |= {self.u32(a + i * size + o) for o in offs}
                for lo, hi in OTHER_POINTER_RANGES:
                    t |= set(struct.unpack_from(f'<{(hi - lo) // 4}I', self.rom, lo - BASE))
            self._targets = {x for x in t if BASE <= x < BASE + 0x800000}
        return self._targets

    def assets(self):
        if self._assets is None:
            self._assets = []
            try:
                for line in open(A.MANIFEST):
                    f = line.split('#')[0].split()
                    if len(f) >= 4:
                        self._assets.append((int(f[0], 16), int(f[1], 16), f[3]))
            except OSError:
                pass
            self._assets.sort()
        return self._assets

    def where(self, a):
        """The schema table that holds address a -> (start, element size, name), or None."""
        if self._where is None:
            rows = []
            for s, e in schema(self).items():
                if e.get('count') and e.get('type') not in (None, 'strings'):
                    rows.append((s, s + e['count'] * spec_size(parse_spec(e['type'])),
                                 spec_size(parse_spec(e['type'])), e.get('tag') or e.get('name', '')))
            rows.sort()
            self._where = ([r[0] for r in rows], rows)
        starts, rows = self._where
        i = bisect.bisect_right(starts, a) - 1
        if i >= 0 and rows[i][0] <= a < rows[i][1]:
            return rows[i][0], rows[i][2], rows[i][3]
        return None

    def string_at(self, a):
        if self.rom is None:
            return None
        o = a - BASE
        e = self.rom.find(b'\0', o, o + 0x400)
        if e <= o:
            return None
        b = self.rom[o:e]
        if all(32 <= c < 127 or c == 10 for c in b):
            return A.dec(b)
        return sjis(b, need_wide=True)

    def describe(self, a):
        """What a ROM pointer points to, for the comment after the address."""
        if not BASE <= a < BASE + 0x800000:
            return None
        f = funcs()[0].get(a & ~1)
        if f and (f[0] == 't') == bool(a & 1):
            return f[1]
        w = self.where(a)
        if w:
            s, size, name = w
            i = (a - s) // size
            return name + ('' if a == s else f' [{i}]' if a == s + i * size else f' +0x{a - s:X}')
        s = self.string_at(a)
        if s is None and any(lo <= a < hi for lo, hi in LAYOUT_RANGES) and a % 4 == 0 and self.u32(a) == 0:
            s = ''  # an empty string in the string areas
        if s is not None:
            s = s.replace('\n', '\\n')
            return json.dumps(s[:40] + ('...' if len(s) > 40 else ''), ensure_ascii=False)
        for lo, hi, path in self.assets():
            if lo <= a < hi:
                return path + ('' if a == lo else '+0x%X' % (a - lo))
        return None


# ============================================================================================== header
# GBA cartridge header, ROM+0x04..0xBF (the entry branch at +0x00 is code in asm/crt0.s).
HDR_FIELDS = [  # (name, ROM offset, size, kind)
    ('title', 0xA0, 12, 'text'), ('game_code', 0xAC, 4, 'text'), ('maker_code', 0xB0, 2, 'text'),
    ('fixed_96h', 0xB2, 1, 'hex'), ('main_unit', 0xB3, 1, 'int'), ('device_type', 0xB4, 1, 'int'),
    ('reserved_b5', 0xB5, 7, 'bytes'), ('version', 0xBC, 1, 'int'), ('checksum', 0xBD, 1, 'checksum'),
    ('reserved_be', 0xBE, 2, 'bytes'),
]


def header_checksum(h):
    """Complement check over ROM+0xA0..0xBC (h is indexed by ROM offset)."""
    return -(sum(h[0xA0:0xBD]) + 0x19) & 0xFF


def x_header(data, p, rom):
    h = bytes(4) + data  # index by ROM offset
    base = os.path.basename(p['path'])
    d = {'_note': f'Cartridge header, ROM+0x04..0xBF. The Nintendo logo (ROM+0x04..0x9F, checked by the BIOS) '
                  f'is {base}.logo.bin. "checksum": "auto" is recomputed at build time; a number forces a value.'}
    for name, off, size, kind in HDR_FIELDS:
        raw = h[off:off + size]
        if kind == 'text':
            d[name] = A.text_field(raw)
        elif kind == 'hex':
            d[name] = '0x%02X' % raw[0]
        elif kind == 'int':
            d[name] = raw[0]
        elif kind == 'bytes':
            d[name] = raw.hex()
        else:
            ok = raw[0] == header_checksum(h)
            d[name] = 'auto' if ok else '0x%02X' % raw[0]
            d['_checksum'] = '0x%02X in the ROM%s' % (raw[0], '' if ok else ' (does not match the computed value)')
    return {p['path'] + '.json': (json.dumps(d, indent=1, ensure_ascii=False) + '\n').encode(),
            p['path'] + '.logo.bin': h[0x04:0xA0]}


def b_header(read, p):
    d = json.loads(read(p['path'] + '.json'))
    logo = read(p['path'] + '.logo.bin')
    if len(logo) != 0x9C:
        raise ValueError('the logo must be 156 bytes')
    h = bytearray(0xC0)
    h[0x04:0xA0] = logo
    auto = False
    for name, off, size, kind in HDR_FIELDS:
        v = d[name]
        if kind == 'text':
            raw = A.field_bytes(v, size)
        elif kind == 'bytes':
            raw = bytes.fromhex(v)
        elif kind == 'checksum' and v == 'auto':
            auto, raw = True, b'\0'
        else:
            raw = bytes([num(v)])
        if len(raw) != size:
            raise ValueError(f'header {name}: {len(raw)} bytes, expected {size}')
        h[off:off + size] = raw
    if auto:
        h[0xBD] = header_checksum(h)
    return bytes(h[4:])


# ============================================================================================= effects
# 0x0819A9D4: sorted by card number; sub_08047058 binary-searches it (fixed 0..0x1A9) with a card's number.
# The slot names follow the code that calls them: +4 resolve (code_08020AF4, code_08035198 "executor"),
# +8 check (code_0802CAE8 checkZone, code_08046738 runs it per card), +0xC prepare (code_0802CAE8),
# +0x10 / +0x14 chain_a / chain_b (code_0801F454 fnA/fnB, code_08050A70 fn10/fn14).
EFF_COUNT, EFF_SIZE = 426, 0x18
EFF_SLOTS = ['resolve', 'check', 'prepare', 'chain_a', 'chain_b']


def x_effects(data, p, rom):
    if len(data) != EFF_COUNT * EFF_SIZE:
        raise ValueError('unexpected size')
    ctx = Ctx(rom)
    rows = []
    for i in range(EFF_COUNT):
        number, flags, *fns = struct.unpack_from('<HH5I', data, i * EFF_SIZE)
        row = {'number': number}
        card = ctx.card_by_number(number)
        if card:
            row['card'] = card
        if flags:
            row['flags'] = '0x%04X' % flags
        for slot, f in zip(EFF_SLOTS, fns):
            row[slot] = fn_name(f)
        rows.append(row)
    out = ['['] + [' ' + _j(r) + (',' if i < len(rows) - 1 else '') for i, r in enumerate(rows)] + [']']
    return {p['path'] + '.json': ('\n'.join(out) + '\n').encode()}


def b_effects(read, p):
    rows = json.loads(read(p['path'] + '.json'))
    if len(rows) != EFF_COUNT:
        raise ValueError(f'the table must have {EFF_COUNT} entries (sub_08047058 searches 0..0x1A9), not {len(rows)}')
    nums = [num(r['number']) for r in rows]
    if nums != sorted(set(nums)):
        raise ValueError('entries must be sorted by number, without duplicates (the lookup is a binary search)')
    out = bytearray()
    for r, n in zip(rows, nums):
        out += struct.pack('<HH', n, num(r.get('flags')))
        for slot in EFF_SLOTS:
            out += struct.pack('<I', num(r.get(slot)))
    return bytes(out)


# ============================================================================================== layout
def x_layout(data, p, rom):
    ctx = Ctx(rom)
    groups = {}
    for fname, it in layout_items(p['start'], p['end'], data, ctx):
        groups.setdefault(fname, []).append(it)
    strings = groups.pop('strings.json', [])
    tables = groups.pop('tables.json', [])
    files = {}
    for fname in sorted(groups):
        tables.append({'include': fname})
        files[f"{p['path']}/{fname}"] = dump_items(groups[fname])
    files[f"{p['path']}/strings.json"] = dump_items(strings)
    files[f"{p['path']}/tables.json"] = dump_items(tables)
    return files


def b_layout(read, p):
    lo, hi = p['start'], p['end']
    items = json.loads(read(f"{p['path']}/strings.json"))
    for it in json.loads(read(f"{p['path']}/tables.json")):
        if 'include' in it:
            items += json.loads(read(f"{p['path']}/{it['include']}"))
        else:
            items.append(it)
    placed = sorted(((num(it['addr']), it) for it in items), key=lambda x: x[0])
    out = bytearray(hi - lo)
    for i, (a, it) in enumerate(placed):
        nxt = placed[i + 1][0] if i + 1 < len(placed) else hi
        if not lo <= a < hi:
            raise ValueError(f'item at 0x{a:08X} is outside 0x{lo:08X}-0x{hi:08X}')
        if nxt == a:
            raise ValueError(f'two items at 0x{a:08X}')
        try:
            b = item_bytes(it, nxt - a)
        except (ValueError, KeyError, TypeError, UnicodeEncodeError) as e:
            raise ValueError(f'item at 0x{a:08X}: {e}')
        if len(b) > nxt - a:
            raise ValueError(f'item at 0x{a:08X} needs {len(b)} bytes but the next item starts at '
                             f'0x{nxt:08X} ({nxt - a} bytes)')
        out[a - lo:a - lo + len(b)] = b
    return bytes(out)


def item_bytes(it, room):
    if 'text' in it:
        if isinstance(it['text'], dict):  # bytes after the NUL that aren't zero
            return A.field_bytes(it['text'], room)
        return A.enc(it['text']) + b'\0'
    if 'sjis' in it:
        return it['sjis'].encode('shift_jis') + b'\0'
    if 'hex' in it:
        return bytes.fromhex(it['hex'])
    sp = parse_spec(it['type'])
    if not isinstance(it['values'], list):
        raise ValueError('"values" must be a list')
    return b''.join(encode(sp, v) for v in it['values'])


# --------------------------------------------------------------------------------------- extraction
def string_item(raw, any_text):
    """One slot (text + NUL + padding) -> {'text'} / {'sjis'}, or None if it doesn't look like text.
    Without any_text only clean ASCII or Shift-JIS with zero padding is accepted."""
    n = raw.find(b'\0')
    if n < 0:
        return None
    body, tail = raw[:n], raw[n:]
    if any_text:
        if any(c >= 0x80 for c in body) and not tail.strip(b'\0'):
            t = sjis(body, need_wide=True)
            if t is not None:
                return {'sjis': t}
        return {'text': A.text_field(raw)}
    if tail.strip(b'\0') or any(c < 0x20 and c not in (9, 10) for c in body) or b'\x7f' in body:
        return None
    if any(c >= 0x80 for c in body):
        t = sjis(body, need_wide=True)
        return {'sjis': t} if t is not None else None
    return {'text': A.dec(body)}


def segments(lo, b, ctx, any_text=False, min_len=2):
    """Split a block into strings (4-aligned, NUL-terminated) and raw chunks -> [(addr, kind, fields)].
    A string's slot runs to the next 4-aligned non-zero byte, label or pointer target, so the text can grow
    into its padding. A pointer target in the padding is an empty string of its own."""
    cuts = ctx.labels() | ctx.targets()
    out, pend, o = [], None, 0

    def flush(end):
        if pend is not None:
            chunk = b[pend:end].rstrip(b'\0')
            if chunk:
                out.append((lo + pend, 'hex', {'hex': chunk.hex()}))

    while o < len(b):
        a = lo + o
        st = None
        if a % 4 == 0 and b[o]:
            e = b.find(b'\0', o)
            if e >= 0 and (e - o >= min_len or a in ctx.labels() or a in ctx.targets()):
                s = e + 1
                while s < len(b) and not ((lo + s) % 4 == 0 and (b[s] or lo + s in cuts)):
                    s += 1
                it = string_item(b[o:s], any_text)
                st = (it, s) if it else None
        elif a % 4 == 0 and pend is None and a in ctx.targets():  # an empty string a pointer table uses
            s = o + 1
            while s < len(b) and not ((lo + s) % 4 == 0 and (b[s] or lo + s in cuts)):
                s += 1
            st = ({'text': ''}, s) if not b[o:s].strip(b'\0') else None
        if st:
            flush(o)
            pend = None
            out.append((a, 'str', st[0]))
            o = st[1]
            continue
        if pend is None and b[o]:
            pend = max(0, o - a % 4)
        o += 1
    flush(len(b))
    return out


def guess_words(b, ctx):
    """'fn' or 'ptr' for a block of u32 words that are all NULL or ROM pointers (at least two), else None."""
    b = b.rstrip(b'\0')
    b += bytes(-len(b) % 4)
    ws = struct.unpack(f'<{len(b) // 4}I', b)
    nz = [w for w in ws if w]
    if len(nz) < 2 or any(not BASE <= w < BASE + 0x800000 for w in nz):
        return None
    return 'fn' if all(not fn_name(w).startswith('0x') for w in nz) else 'ptr'


def mk(addr, ctx, **fields):
    it = {'addr': '0x%08X' % addr}
    if addr in ctx.labels():
        it['label'] = 'gUnk_%08X' % addr
    it.update(fields)
    return it


def table_item(addr, spec, b, ctx, count=None, name=None):
    sp = parse_spec(spec)
    size = spec_size(sp)
    n = len(b) // size if count is None else count
    if n * size > len(b):
        raise ValueError(f'table at 0x{addr:08X} ({spec} x {n}) does not fit in 0x{len(b):X} bytes')
    it = mk(addr, ctx, **({'name': name} if name else {}))
    it['type'] = spec
    it['values'] = [decode(sp, b, i * size, ctx) for i in range(n)]
    return it, n * size


def auto_items(addr, b, ctx, fname, any_text=False):
    """Strings, pointer arrays and raw chunks in a block that has no declared type."""
    out = []
    if addr % 4 == 0 and len(b) >= 8:
        g = guess_words(b, ctx)
        if g:
            n = (len(b.rstrip(b'\0')) + 3) // 4
            return [(fname, table_item(addr, g, b, ctx, count=n)[0])]
    for sa, kind, f in segments(addr, b, ctx, any_text, min_len=0 if any_text else 2):
        if kind == 'str':
            out.append(('strings.json', mk(sa, ctx, **f)))
        else:
            out.append((fname, mk(sa, ctx, name='unknown', **f)))
    return out


def block_items(addr, b, ctx, ent):
    """Items for the block [addr, addr+len(b)); ent is its schema entry or None."""
    ent = ent or {}
    fname = ent.get('file', 'tables.json')
    t = ent.get('type')
    if t is None:
        return auto_items(addr, b, ctx, fname)
    if t == 'strings':
        out = auto_items(addr, b, ctx, fname, any_text=True)
        if any(f != 'strings.json' for f, _ in out):
            raise ValueError(f'0x{addr:08X} is declared as strings but holds other data')
        return out
    it, used = table_item(addr, t, b, ctx, ent.get('count'), ent.get('name'))
    return [(fname, it)] + auto_items(addr + used, b[used:], ctx, fname)


def layout_items(lo, hi, data, ctx):
    ents = {a: e for a, e in schema(ctx).items() if lo <= a < hi}
    cuts = set(ents) | {a for a in ctx.labels() if lo <= a < hi} | {lo}
    for a, e in ents.items():  # a label inside a declared table doesn't split it
        if e.get('count') is not None and e['type'] != 'strings':
            end = a + e['count'] * spec_size(parse_spec(e['type']))
            cuts -= {c for c in cuts if a < c < end and c not in ents}
            if end < hi:
                cuts.add(end)
    cuts = sorted(cuts)
    items = []
    for s, e in zip(cuts, cuts[1:] + [hi]):
        items += block_items(s, data[s - lo:e - lo], ctx, ents.get(s))
    return items


# ============================================================================================== schema
_SCHEMA = None


def schema(ctx):
    """addr -> {type, count, name[, file]}: every table the code reads, in both layout ranges. Gaps hold
    strings, which are found by scanning."""
    global _SCHEMA
    if _SCHEMA is None:
        s = derived_schema(ctx)
        for a, t, n, name in TABLES:
            s[a] = {'type': t, 'count': n, 'name': name}
        _SCHEMA = s
    return _SCHEMA


LAYOUT_RANGES = [(0x08080A20, 0x08087FB4), (0x0819790C, 0x0819A9D4)]  # the tables_code_layout rows
# Other groups' ranges that hold pointer tables into ours (their words are pointer-target candidates).
OTHER_POINTER_RANGES = [(0x0819D1C4, 0x0819D34C), (0x0819DD64, 0x081A7A0C)]

# Sprite animations (sub_08078670 / sub_080786D0 / sub_08077EF4). A track list is a NULL-terminated array of
# pointers to step arrays. A step is {u8 frames; u8 count; u16 unk2; const OamTemplate *sprites}; a step with
# frames == 0 ends the array. The sprites are `count` 8-byte OAM templates {attr0, attr1, attr2, u16 unk6}.
ANIM_STEP, OAM = 'AnimStep', 'OamTemplate'
TYPEDEFS = {
    'AnimStep': '{frames:u8,count:u8,unk2?:u16,sprites:ptr}',
    'OamTemplate': ('{_:bits16(y:8,mode?:6,shape:2),_:bits16(x:9,unused?:3,hflip?:1,vflip?:1,size:2),'
                    '_:bits16(tile:10,prio?:2,pal:4),unk6?:u16}'),
}
SCENE_SETS, SCENE_SET_COUNT = 0x081976A0, 31  # gfx/scene_sets: {bitmapLz, bgPal, objPal, objTilesLz, anim}
ANIM_LISTS = {  # track lists passed to sub_08078670 besides the scene sets' own
    0x0819A698: 'code_08027580', 0x081999F8: 'code_08025108', 0x08199A04: 'code_08025108',
    0x08199D9C: 'code_08026124', 0x08199CC8: 'code_08026124', 0x0819A780: 'code_08028684',
    0x081999C4: 'unreferenced, lists the die-toss step arrays', 0x081A6118: 'code_08069284',
    0x081A70FC: 'code_0806A92C and others',
}
ANIM_STEP_PTRS = {0x081999EC: (3, 'die-toss frames by kind (struct DieFrame *[3], code_08025108)')}
ANIM_STEPS = {0x08081FD4: 'die-toss frames (struct DieFrame[], code_08025108)'}
DECK_POOLS, DECK_POOL_COUNT = 0x08198744, 11  # struct DeckPool {const u16 *cards; u32 count:10, ...}


def derived_schema(ctx):
    """Tables found by following pointers from the ROM: animations and the starter-deck pool lists."""
    rom = ctx.rom
    if rom is None:
        return {}
    u32 = ctx.u32
    ents = {}
    for i in range(DECK_POOL_COUNT):
        cards, w = u32(DECK_POOLS + 8 * i), u32(DECK_POOLS + 8 * i + 4)
        ents[cards] = {'type': 'cardnum', 'count': w & 0x3FF, 'name': f'starter deck pool {i}',
                       'tag': f'deck pool {i}'}
    lists = {}
    for i in range(SCENE_SET_COUNT):
        a = u32(SCENE_SETS + i * 0x14 + 0x10)
        if a:
            lists.setdefault(a, []).append(f'scene set {i}')
    for a, who in ANIM_LISTS.items():
        lists.setdefault(a, []).append(who)
    steps, sprites, anim = {}, {}, {}
    for l, who in lists.items():
        ptrs = []
        while not ptrs or ptrs[-1]:
            ptrs.append(u32(l + 4 * len(ptrs)))
        anim[l] = {'type': 'ptr', 'count': len(ptrs), 'name': 'animation tracks: ' + ', '.join(who),
                   'tag': 'tracks of ' + who[0]}
        for t, p in enumerate(ptrs[:-1]):
            steps.setdefault(p, []).append(f'{who[0]} track {t}')
    for a, (n, who) in ANIM_STEP_PTRS.items():
        anim[a] = {'type': 'ptr', 'count': n, 'name': who}
        for t in range(n):
            steps.setdefault(u32(a + 4 * t), []).append(f'die-toss kind {t}')
    for a, who in ANIM_STEPS.items():
        steps.setdefault(a, []).append(who)
    for st, who in steps.items():
        n = 0
        while True:
            fr, cnt = rom[st - BASE + 8 * n], rom[st - BASE + 8 * n + 1]
            n += 1
            if fr == 0:
                break
            spr = u32(st + 8 * n - 4)
            sprites[spr] = max(sprites.get(spr, 0), cnt)
        anim[st] = {'type': ANIM_STEP, 'count': n, 'name': 'animation steps: ' + ', '.join(who),
                    'tag': 'steps of ' + who[0]}
    groups = []  # sprite groups that overlap become one table
    for a in sorted(sprites):
        e = a + 8 * sprites[a]
        if groups and a < groups[-1][1]:
            groups[-1][1] = max(groups[-1][1], e)
        else:
            groups.append([a, e])
    for a, e in groups:
        anim.setdefault(a, {'type': OAM, 'count': (e - a) // 8, 'name': 'sprite templates', 'tag': 'sprites'})
    for e in anim.values():
        e['file'] = 'anims.json'
    ents.update(anim)
    return ents


# Every table the code reads in .rodata 1 (0x08080A20-0x08087FB4) and the scene-scripts range
# (0x0819790C-0x0819A9D4): (address, type, count or None for "up to the next label", name). Types come from
# the extern declarations in src/; "unreferenced" means no code or table points at it by this address.
STEP = 'fn'  # NULL-terminated arrays of u16 (*)(void) step functions
TABLES = [
    # ---- .rodata 1
    (0x08080A48, '{x:u8,y:u8,pad?:u16}', 5, 'text box: sparkle start positions (struct SparkleStart, code_08000228)'),
    (0x08080A5C, 'u8', 8, 'text box: sparkle palettes (code_08000228)'),
    (0x08080AA8, 'u16', 7, 'code_08000228'),
    (0x08080AB6, 'u16', 5, 'opponent select: sparkle start x per slot (code_08001364)'),
    (0x08080AC0, 'u16', 10, 'digit sprite tiles 0-9 (sub_08000324)'),
    (0x08080AD4, 'u16', 6, 'struct Unk08080AD4 (code_08001364)'),
    (0x08080AE0, 'u8', 25, 'opponent record index [page * 5 + slot] (code_08001364)'),
    (0x08080AFA, '{x:u8,y:u8}', 6, 'unreferenced positions'),
    (0x080813C4, 'u8', 32, 'unreferenced'),
    (0x080815A8, 's16[24]', 6, 'code_0800AB08'),
    (0x080816C8, 's16[8]', 6, 'code_0800AB08'),
    (0x08081728, 's32', 16, 'slide-in x offsets (code_080162C4)'),
    (0x08081768, 's32', 24, 'zoom curve, 0x100 = 1.0 (code_08013CDC, code_080150DC)'),
    (0x080817FC, '{win:u16,lose:u16,draw:u16,unk6:u16,unk8:u16,winAlt4:u16,winAlt9:u16,unkE:u16}', 25,
     'dialogue event per opponent and duel result (struct OpponentText, code_0801BCFC)'),
    (0x0808198C, 'u16', 25, 'dialogue event per opponent (code_0801BCFC)'),
    (0x080819BE, 'u16', 28, 'booster pack IDs checked with sub_08063EDC (code_0801BCFC)'),
    (0x080819F6, 'u16', 25, 'BGM per opponent (code_0801A7B4)'),
    (0x08081A28, 'duelist', 20, 'random opponents, 4 pools of 5 (code_0801A7B4)'),
    (0x08081A50, 'duelist', 4, 'random opponents (code_0801A7B4)'),
    (0x08081A58, 'duelist', 5, 'random opponents (code_0801A7B4)'),
    (0x08081A62, 'duelist', 5, 'random opponents (code_0801A7B4)'),
    (0x08081A6C, 'cardnum', 60, 'notable cards (code_0801A7B4; see wiki special-card-lists)'),
    (0x08081AE4, 'u16', None, 'dialogue event per opponent (code_0801A7B4, code_0801BCFC)'),
    (0x08081B16, 'u16', None, 'dialogue event per opponent (code_0801A7B4)'),
    (0x08081B48, 'u16', None, 'dialogue event per opponent (code_0801A7B4)'),
    (0x08081B7A, 'u16', None, 'dialogue event per opponent (code_0801A7B4)'),
    (0x08081BAE, 'u16', None, 'dialogue event per opponent (code_0801A7B4)'),
    (0x08081BE2, 'u16', None, 'dialogue event per opponent (code_0801A7B4)'),
    (0x08081C16, 'u16', None, 'dialogue event per opponent (code_0801A7B4)'),
    (0x08081C42, 'u16', None, 'dialogue event per opponent (code_0801A7B4)'),
    (0x08081C76, 'u16', None, 'dialogue event per opponent (code_0801A7B4)'),
    (0x08081F7C, 'u32', 1, 'unreferenced'),
    (0x08081F80, 'u16', 8, 'coin-toss token tile per frame (code_0802408C)'),
    (0x08081F90, 'u16', 10, 'coin-toss result tile, +5 = second set (code_0802408C)'),
    (0x08081FA4, 'u16', 24, 'sparkle tile per frame, 0-terminated (code_08025108)'),
    (0x080822FC, 'u8[4]', 3, 'code_08025108'),
    (0x08082308, 'u8[2][2]', 6, 'code_08025108'),
    (0x080823B0, '{y:u16,x:u16}', 5, 'mover start positions (code_08026124)'),
    (0x080823C4, 's8', 64, 'code_08026124'),
    (0x08082404, 's8', 8, 'counter index per tick, -1 = none (code_08027580)'),
    (0x080826DC, 'u8', 3, 'three stop positions (code_08027580)'),
    (0x080826E0, 'u16', 3, 'tiles (code_08027580, code_08029750)'),
    (0x080826E6, 'u16', 2, 'code_08027580, code_08028684, code_08029750'),
    (0x080826EA, 'u16', 10, 'code_08027580'),
    (0x080826FE, 'u8', 5, 'code_08027580'),
    (0x08082703, 'u8', 3, 'code_08027580, code_08029750'),
    (0x08082706, 'u16', 3, 'code_08028684, code_08029750'),
    (0x0808270C, 'u16', 2, 'code_08028684, code_08029750'),
    (0x08082710, 'u8', 2, 'code_08028684, code_08029750'),
    (0x08082712, 'u16', 17, 'squares 0-256 (code_08028684, code_08029750)'),
    (0x08086394, 'cardnum', 54, 'cards the AI tries (sub_08059408, code_080590E4)'),
    (0x08086400, 'cardnum', 36, 'cards the AI tries (sub_08059408, code_080590E4)'),
    (0x08086448, 'cardnum', 20, 'cards checked by code_0805A30C'),
    (0x08086470, 'strings', None, 'glyph strings for sub_0805EE30 (0x81 and 0xC4 are custom glyphs)'),
    (0x08086478, 'strings', None, 'glyph strings for sub_0805EE30'),
    (0x0808649C, 'strings', None, 'glyph strings for sub_0805EE30'),
    (0x080864A4, 'strings', None, 'glyph strings for sub_0805EE30'),
    (0x08086550, 'ptr', 15, 'images (code_0805F96C)'),
    (0x0808658C, 'u16', 8, 'scroll offsets (code_080629F0)'),
    (0x0808659C, 'x16', 24, 'sprite attribute word per kind (code_080619E8)'),
    (0x080865CC, 'u16', 8, 'slide easing (code_08064AF0)'),
    (0x080865DC, '{id:u16,pad?:u16,image:ptr,name:char[0x40]}', 23,
     'booster packs: id, cover art, name (struct PackInfo, code_08063A28; see wiki booster-packs)'),
    (0x0808733C, 'u16', 11, 'code_08064AF0'),
    (0x08087352, 'u16', 21, 'code_08064AF0'),
    (0x0808737C, 'u16', 7, 'code_08064AF0'),
    (0x0808738A, 'u16', 5, 'code_08064AF0'),
    (0x08087394, 'ptr', 11, 'code_08064AF0'),
    (0x080873C0, 'ptr', 25, 'code_08064AF0'),
    (0x08087424, 'ptr', 7, 'code_08064AF0'),
    (0x08087440, 'ptr', 4, 'code_08064AF0'),
    (0x08087450, 'u8', 20, 'BG tile per index (code_08065E6C)'),
    (0x08087464, 's16', 7, 'code_08065E6C'),
    (0x08087472, 'u16', 7, 'code_08065E6C'),
    (0x08087480, 'u8', 8, 'category index (code_08065E6C)'),
    (0x08087488, '{x:u16,y:u16}', 3, 'code_08065E6C'),
    (0x08087494, 's16', 4, 'code_0806704C'),
    (0x0808749C, 's16', 2, 'code_0806704C'),
    (0x080874A0, OAM, 1, 'sprite template (code_08068180)'),
    (0x080874A8, 'sjis16', 86, 'hiragana with voicing marks folded (hypothesis: a sort map; unreferenced by address)'),
    (0x08087568, 'u8', 4, 'code_08064AF0'),
    (0x0808756C, 'u8[4]', 7, 'filter cursor moves (code_08069284)'),
    (0x08087588, 'u8[4]', 7, 'filter cursor moves (code_08069284)'),
    (0x080875A4, 'u8[4]', 6, 'filter cursor moves (code_08069284)'),
    (0x080875BC, 'u8', 6, 'object per sort selection (code_08069284)'),
    (0x080875C4, 'x16', 7, 'unreferenced bit masks'),
    (0x080875D2, 's16', 13, 'code_08065E6C and others'),
    (0x08087600, 's8', 4, 'per SIOCNT baud setting (code_08071F40)'),
    (0x08087634, 'u32', 10, 'decimal digit characters (code_08071F40)'),
    (0x0808765C, 'u32', 16, 'hex digit characters (code_08071F40)'),
    (0x0808769C, 'u32', 6, 'unreferenced'),
    (0x0808771C, 'u32', 1, 'unreferenced'),
    (0x08087720, '{mask:x32,name:char[0x20]}', 29, 'flag names by mask (struct FlagRow, code_080740BC)'),
    (0x08087B90, 'u8', 2, 'text box start {col, row} (code_08000228)'),
    (0x08087BA4, 's16', 320, 'sine table, 256 steps per turn, 0x100 = 1.0; the last 64 repeat the first '
                             'quarter for cosine (gUnk_08087D8C)'),
    (0x08087E24, '{x:u8,y:u8,unk2:u16,flags:x16,_:bits16(up:4,down:4,left:4,right:4)}', 11,
     'password keypad: key position and d-pad neighbours (struct KeyNav / MenuEntry, code_0807B6B8)'),
    (0x08087E7C, 'u16', 6, 'code_0807B6B8'),
    (0x08087E88, 's32', 32, 'code_0807C7C8'),
    (0x08087F08, '{lo:u16,hi:u16}', 32, 'struct Pair16 (code_0807C7C8)'),
    # ---- scene scripts and lists
    (0x081979DC, OAM, 4, 'sprite templates (unreferenced)'),
    (0x081980D4, '{flags:x32,name:char[0x40]}', 9, 'calendar events: event flag, name (struct CalendarEvent, code_08001364)'),
    (0x08198338, STEP, 5, 'calendar steps (code_08002388)'),
    (0x0819834C, 'duelist', 26, 'opponent per [page * 5 + slot] (code_08002388)'),
    (0x08198380, STEP, 11, 'opponent-select steps (code_08002388)'),
    (0x081983AC, '{x:s16,y:s16}', 5, 'slot screen positions (struct Pos16, code_08002388)'),
    (0x081983C0, '{x:s16,y:s16}', 32, 'bobbing offsets (struct Bob, code_080034B8)'),
    (0x08198440, '{pal:ptr,bitmap:ptr}', 5, 'page backgrounds, mode-4 bitmaps (struct Bitmap, code_08002388)'),
    (0x08198468, 'ptr', 25, 'opponent names [page * 5 + slot] (code_08002388)'),
    (0x081984CC, 'ptr', 3, '"Win", "Lose", "Draw" (code_08002388)'),
    (0x081984D8, STEP, 7, 'main menu launch table (code_080034B8)'),
    (0x081984F4, STEP, 5, 'main menu steps (code_080034B8)'),
    (0x08198508, 'u16[2][16]', 2, 'record scroll offsets [parity][direction - 1][step] (code_080034B8)'),
    (0x08198588, STEP, 6, 'record steps (code_080034B8)'),
    (0x081985A0, 'ptr', 25, 'image packs (code_080034B8)'),
    (0x08198604, 'ptr', 5, 'image packs (code_080034B8)'),
    (0x08198618, 'u16', 8, 'sprite tile per frame (code_080034B8)'),
    (0x08198628, 'u8', 12, 'days per month (code_080034B8, code_080044E4)'),
    (0x08198744, '{cards:ptr,_:bits32(count:10,take0:5,take1:5,take2:5,unk25:7)}', 11,
     'starter deck pools: card list, size, copies drawn for starting choice 0/1/2 (struct DeckPool, code_080044E4)'),
    (0x0819879C, STEP, 5, 'code_080044E4'),
    (0x081987B0, 's16[16]', 4, 'unreferenced HBlank scroll waves'),
    (0x08198830, 's16[16]', 4, 'HBlank scroll waves; row 0 is copied by code_080044E4'),
    (0x081988B0, STEP, 8, 'code_08005500'),
    (0x081988D0, 'ptr', 25, 'monster type names by type (code_08005500)'),
    (0x08198934, 'ptr', 7, 'magic/trap subtype suffixes (code_08005500)'),
    (0x08198950, 'ptr', 11, 'code_08005500, code_080619E8'),
    (0x0819897C, 'ptr', 11, 'code_08005500, code_080619E8'),
    (0x081989A8, 'ptr', 10, 'code_08029750, code_080619E8'),
    (0x081989D0, 'ptr', 7, 'code_08029750, code_080619E8'),
    (0x081989EC, 'ptr', 25, 'code_08029750, code_080619E8'),
    (0x08198A50, 's16[16]', 24, 'HBlank scroll waves (code_08006878)'),
    (0x08198D50, STEP, 3, 'Card Detail steps (code_08006878)'),
    (0x08198D5C, STEP, 3, 'Auto Detail steps (code_08006878)'),
    (0x08198D68, '{pal:ptr,gfx:ptr,bgm:u16,pad?:u16}', 3, 'banners: OBJ palette, tiles, BGM (struct BannerGfx, code_08012C4C)'),
    (0x08198D8C, 'u16', 32, 'code_08013CDC'),
    (0x08198DCC, 'cardnum', 12, 'cards checked by code_08013CDC'),
    (0x08198DE4, '{unk0:u32,unk4:u32,unk8:u32,text:char[0x40]}', 2, 'chain banners (code_08019554)'),
    (0x08198E7C, STEP, 12, 'code_0801A7B4'),
    (0x08198EAC, STEP, 12, 'campaign steps (code_0801CE68)'),
    (0x08198EDC, STEP, 7, 'code_0801E260'),
    (0x08198EF8, STEP, 7, 'message handlers by id (code_0801E260)'),
    (0x08198F14, STEP, 3, 'code_0801E260'),
    (0x08198F20, '{opponent:duelist,bgm:u16}', 24, 'BGM per opponent (struct OpponentBgm, code_0801E260)'),
    (0x08198F80, STEP, 11, 'duel phase table (code_08020AF4)'),
    (0x08198FAC, STEP, 4, 'coin-toss steps (code_0802408C)'),
    (0x08199A10, STEP, 6, 'code_08025108'),
    (0x08199A28, STEP, 6, 'code_08025108'),
    (0x08199A40, STEP, 6, 'code_08025108'),
    (0x08199A58, STEP, 6, 'code_08026124'),
    (0x08199A70, STEP, 6, 'code_08026124'),
    (0x08199D74, OAM, 5, 'sprite templates (struct Tmpl8, code_08026124)'),
    (0x08199DA4, STEP, 10, 'code_08026124'),
    (0x08199DCC, '{src:ptr,dst:x32,x:s16,y:s16}', 4, 'struct ObjInit (code_08027580)'),
    (0x08199DFC, STEP, 5, 'code_08027580'),
    (0x0819A6B0, STEP, 8, 'code_08029750'),
    (0x0819A6D0, STEP, 18, 'code_08029750'),
    (0x0819A718, STEP, 5, 'code_08029750'),
    (0x0819A72C, STEP, 4, 'code_08029750'),
    (0x0819A73C, STEP, 5, 'code_08029750'),
    (0x0819A788, 's32[4]', 3, 'scroll offsets [dir][timer] (code_0802AAC0 reads the low halves)'),
    (0x0819A7B8, STEP, 4, 'code_0802AAC0'),
    (0x0819A7C8, '{result:cardnum,a:cardnum,b:cardnum,pad?:u16}', 53,
     'fusions: result and two materials, e.g. Flame Swordsman = Flame Manipulator + Masaki (code_0803C838; '
     '999 ends the list)'),
    (0x0819A970, '{result:cardnum,a:cardnum,b:cardnum,c:cardnum}', 4,
     'fusions with three materials, e.g. Blue-Eyes Ultimate Dragon (code_0803C838; 999 ends the list)'),
    (0x0819A990, 'bits32(a:13,num:13,cnt:6)', 17, 'struct RecipeEnt (code_080431E4)'),
]
