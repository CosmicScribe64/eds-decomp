#!/usr/bin/env python3
"""Static cross-reference database for the EDS decomp: who calls what, which globals, data and text each function
touches. Built from the linked ELF, so it covers every function whether it is C or asm.

  tools/dr python3 tools/xref.py build                 # build/eds.elf -> build/xref/xref.json (needs the Docker image)
  python3 tools/xref.py func <name|addr>               # summary card of one function
  python3 tools/xref.py global <name|addr> [--offset 0x40C]   # every function touching a global (or one field)
  python3 tools/xref.py strings <regex>                # functions that use matching text
  python3 tools/xref.py graph <name|addr> [--depth 3] [--up]  # call tree (callees, or callers with --up)
  python3 tools/xref.py unit <unit>                    # one-screen overview of a unit
  python3 tools/xref.py subsystems [--min 4]           # call-graph + shared-global clusters with suggested labels
  python3 tools/xref.py what <addr>                    # what lives at an address (function, global field, asset)
  python3 tools/xref.py selftest                       # check the database against facts documented in the wiki
Queries take --json for machine-readable output and --all to stop truncating. They use only the standard
library, so they also run on the host; `build` needs capstone and pycparser (tools/dr). Rebuild after code or
C declaration changes; renames in config/functions.tsv show up without a rebuild. See build/xref/README.md.

How it works (build):
  * code: every function symbol in the ELF, ranges from config/functions.tsv, $t/$a/$d mapping symbols to skip
    literal pools and jump tables; disassembled with capstone.
  * per function, a forward constant/pointer propagation over the CFG tracks register and stack-slot values:
    literal-pool constants, mov/add/sub/shift arithmetic, pointers plus unknown index ("indexed"), the
    incoming arguments r0-r3, pointers loaded from globals and from tables. Every load/store whose address is
    known becomes an access record (global or field, size, read/write, indexed or not); accesses through an
    argument become parameter-field records.
  * calls: `bl` targets, tail branches, `_call_via_rN` and `mov lr, pc; bx` indirect calls. Indirect targets
    are resolved when the register holds a constant, a function loaded from a ROM table (all entries of the
    table at that element offset), or a RAM slot (all functions ever stored into that slot, including stores
    of a function pointer passed as an argument, propagated through call sites).
  * names: config/functions.tsv, symbols.ld, config/symbols.txt and the ELF; proposed names from
    config/names.txt; struct field names merged from every C unit's declarations (the tools/structmap.py
    layout rules); IO register names from include/gba.h; asset paths from config/assets.tsv; text decoded
    with the tools/assets.py text helpers; wiki pages that mention each address.
xref.json holds decoded game text, so it is derived from the ROM: it stays in build/ and is never committed.
"""
import argparse
import bisect
import collections
import glob
import json
import os
import re
import struct
import sys
import time

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(REPO, 'tools'))
OUT = os.path.join(REPO, 'build', 'xref', 'xref.json')
ELF = os.path.join(REPO, 'build', 'eds.elf')
BASE = 0x08000000
CODE_END = 0x08080A20
ROM_END = 0x08800000
VERSION = 1

REGIONS = [  # (lo, hi, name)
    (0x00000000, 0x00004000, 'BIOS'),
    (0x02000000, 0x02040000, 'EWRAM'),
    (0x03000000, 0x03008000, 'IWRAM'),
    (0x04000000, 0x04000400, 'IO'),
    (0x05000000, 0x05000400, 'PLTT'),
    (0x06000000, 0x06018000, 'VRAM'),
    (0x07000000, 0x07000400, 'OAM'),
    (0x08000000, CODE_END, 'CODE'),
    (CODE_END, ROM_END, 'ROM'),
    (0x0E000000, 0x0E010000, 'SRAM'),
]
_REG_LO = [r[0] for r in REGIONS]
SWI_NAMES = {0x00: 'SoftReset', 0x01: 'RegisterRamReset', 0x02: 'Halt', 0x04: 'IntrWait', 0x05: 'VBlankIntrWait',
             0x06: 'Div', 0x08: 'Sqrt', 0x0A: 'ArcTan2', 0x0B: 'CpuSet', 0x0C: 'CpuFastSet', 0x0E: 'BgAffineSet',
             0x0F: 'ObjAffineSet', 0x11: 'LZ77UnCompWram', 0x12: 'LZ77UnCompVram', 0x19: 'SoundBias'}
DATA_TABLE_PREFIXES = ('rodata/', 'tables/', 'sound/se_table', 'sound/song_table', 'sound/lookup_tables',
                       'text/')    # (+ gfx/scene_sets) assets scanned for function pointers
SWI_NARGS = {0x00: 0, 0x01: 1, 0x02: 0, 0x04: 2, 0x05: 0, 0x06: 2, 0x08: 1, 0x0A: 2, 0x0B: 3, 0x0C: 3, 0x0E: 3,
             0x0F: 4, 0x11: 2, 0x12: 2, 0x19: 1}
# IO registers not spelled out in include/gba.h (GBATEK names).
IO_EXTRA = {0x000: 'DISPCNT', 0x004: 'DISPSTAT', 0x006: 'VCOUNT', 0x020: 'BG2PA', 0x022: 'BG2PB', 0x024: 'BG2PC',
            0x026: 'BG2PD', 0x028: 'BG2X', 0x02C: 'BG2Y', 0x030: 'BG3PA', 0x038: 'BG3X', 0x03C: 'BG3Y',
            0x04C: 'MOSAIC', 0x050: 'BLDCNT', 0x052: 'BLDALPHA', 0x054: 'BLDY', 0x060: 'SOUND1CNT_L',
            0x062: 'SOUND1CNT_H', 0x064: 'SOUND1CNT_X', 0x068: 'SOUND2CNT_L', 0x06C: 'SOUND2CNT_H',
            0x070: 'SOUND3CNT_L', 0x072: 'SOUND3CNT_H', 0x074: 'SOUND3CNT_X', 0x078: 'SOUND4CNT_L',
            0x07C: 'SOUND4CNT_H', 0x080: 'SOUNDCNT_L', 0x082: 'SOUNDCNT_H', 0x084: 'SOUNDCNT_X', 0x088: 'SOUNDBIAS',
            0x090: 'WAVE_RAM', 0x0A0: 'FIFO_A', 0x0A4: 'FIFO_B', 0x0B0: 'DMA0SAD', 0x0B4: 'DMA0DAD',
            0x0B8: 'DMA0CNT_L', 0x0BA: 'DMA0CNT_H', 0x0BC: 'DMA1SAD', 0x0C0: 'DMA1DAD', 0x0C4: 'DMA1CNT_L',
            0x0C6: 'DMA1CNT_H', 0x0C8: 'DMA2SAD', 0x0CC: 'DMA2DAD', 0x0D0: 'DMA2CNT_L', 0x0D2: 'DMA2CNT_H',
            0x0D4: 'DMA3SAD', 0x0D8: 'DMA3DAD', 0x0DC: 'DMA3CNT_L', 0x0DE: 'DMA3CNT_H', 0x100: 'TM0CNT_L',
            0x102: 'TM0CNT_H', 0x104: 'TM1CNT_L', 0x106: 'TM1CNT_H', 0x108: 'TM2CNT_L', 0x10A: 'TM2CNT_H',
            0x10C: 'TM3CNT_L', 0x10E: 'TM3CNT_H', 0x120: 'SIOMULTI0', 0x122: 'SIOMULTI1', 0x124: 'SIOMULTI2',
            0x126: 'SIOMULTI3', 0x128: 'SIOCNT', 0x12A: 'SIOMLT_SEND', 0x130: 'KEYINPUT', 0x132: 'KEYCNT',
            0x134: 'RCNT', 0x200: 'IE', 0x202: 'IF', 0x204: 'WAITCNT', 0x208: 'IME'}


BIOS_RAM = {0x03007FFC: 'INTR_VECTOR', 0x03007FF8: 'INTR_CHECK', 0x03FFFFFC: 'INTR_VECTOR(mirror)',
            0x03FFFFF8: 'INTR_CHECK(mirror)'}


def region(a):
    i = bisect.bisect_right(_REG_LO, a) - 1
    if i >= 0 and REGIONS[i][0] <= a < REGIONS[i][1]:
        return REGIONS[i][2]
    if 0x03FF8000 <= a < 0x04000000:
        return 'IWRAM'   # mirror (BIOS IRQ vector / IntrCheck at 0x03FFFFF8..)
    return None


def hx(a):
    return f'0x{a:08X}'


def parse_addr(s):
    s = s.strip()
    m = re.fullmatch(r'(?:0x)?([0-9A-Fa-f]{7,8})', s)
    if m:
        return int(m.group(1), 16)
    return None


# =============================================================================================== inputs
def read_elf(path):
    data = open(path, 'rb').read()
    if data[:4] != b'\x7fELF':
        sys.exit(f'{path}: not an ELF file')
    shoff, = struct.unpack_from('<I', data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', data, 0x2E)
    secs = []
    for i in range(shnum):
        secs.append(struct.unpack_from('<IIIIIIIIII', data, shoff + i * shentsize))
    names_off = secs[shstrndx][4]

    def nm(o):
        return data[o:data.index(b'\0', o)].decode()
    sections = {}
    for i, s in enumerate(secs):
        sections[nm(names_off + s[0])] = dict(idx=i, type=s[1], addr=s[3], off=s[4], size=s[5], link=s[6])
    rom = bytearray(ROM_END - BASE)
    for name in ('.text', '.rodata'):
        s = sections.get(name)
        if s and s['type'] == 1:
            rom[s['addr'] - BASE:s['addr'] - BASE + s['size']] = data[s['off']:s['off'] + s['size']]
    sym = sections['.symtab']
    stroff = secs[sym['link']][4]
    syms = []
    for o in range(sym['off'], sym['off'] + sym['size'], 16):
        st_name, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', data, o)
        if not st_name:
            continue
        syms.append((nm(stroff + st_name), value, size, info >> 4, info & 0xF, shndx))
    text_idx = sections['.text']['idx']
    return bytes(rom), syms, text_idx


def read_functions_tsv():
    rows = {}
    for line in open(os.path.join(REPO, 'config', 'functions.tsv')):
        if line.startswith('#') or not line.strip():
            continue
        f = line.rstrip('\n').split('\t')
        rows[int(f[0], 16)] = dict(mode=f[1], size=int(f[2], 16), name=f[3], unit=f[4])
    return rows


def read_symbol_files():
    """Extra name -> address from symbols.ld and config/symbols.txt."""
    out = {}
    p = os.path.join(REPO, 'symbols.ld')
    if os.path.exists(p):
        for line in open(p):
            m = re.match(r'\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;', line)
            if m:
                out[m.group(1)] = int(m.group(2), 16)
    p = os.path.join(REPO, 'config', 'symbols.txt')
    if os.path.exists(p):
        for line in open(p):
            f = line.split()
            if len(f) >= 2 and not line.startswith('#'):
                out[f[0]] = int(f[1], 16)
    return out


def read_names_txt():
    out = {}
    p = os.path.join(REPO, 'config', 'names.txt')
    if not os.path.exists(p):
        return out
    for line in open(p):
        m = re.match(r'\s*(0x[0-9A-Fa-f]{8})\s+(\w+)\s*(?:#\s*(.*))?$', line)
        if m:
            out[int(m.group(1), 16)] = [m.group(2), (m.group(3) or '').strip()]
    return out


def read_map_units():
    """(lo, hi, unit) ranges of .text from build/eds.map (covers libgcc/libc/sdk too)."""
    out = []
    p = os.path.join(REPO, 'build', 'eds.map')
    if not os.path.exists(p):
        return out
    for line in open(p):
        m = re.match(r'\s*\.text\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)', line)
        if m:
            lo, sz, obj = int(m.group(1), 16), int(m.group(2), 16), m.group(3)
            if not sz:
                continue
            mm = re.search(r'build/(?:src|asm|data)/(.+)\.o$', obj)
            if mm:
                unit = mm.group(1)
            else:
                mm = re.search(r'lib(\w+)\.a\((.+)\.o\)', obj)
                unit = f'lib{mm.group(1)}:{mm.group(2)}' if mm else os.path.basename(obj)
            out.append((lo, lo + sz, unit))
    return sorted(out)


def read_io_names():
    out = dict((k, 'REG_' + v) for k, v in IO_EXTRA.items())
    p = os.path.join(REPO, 'include', 'gba.h')
    if os.path.exists(p):
        for line in open(p):
            m = re.match(r'#define\s+(REG_\w+)\s+REG(?:16|32)\((0x[0-9A-Fa-f]+)\)', line)
            if m:
                out[int(m.group(2), 16)] = m.group(1)
    return out


def read_data_labels():
    """ROM data labels from data/*.s: address -> name."""
    out = {}
    for p in glob.glob(os.path.join(REPO, 'data', '*.s')):
        for line in open(p):
            m = re.match(r'(\w+):\s*@\s*0x([0-9A-Fa-f]{8})', line)
            if m:
                out[int(m.group(2), 16)] = m.group(1)
    return out


def read_unmatched():
    """Function names still INCLUDE_ASM in a C unit."""
    inc = set()
    for p in glob.glob(os.path.join(REPO, 'src', '**', '*.c'), recursive=True):
        for m in re.finditer(r'^\s*INCLUDE_ASM\(\s*"[^"]*"\s*,\s*(\w+)\s*\)', open(p, errors='replace').read(),
                             re.M):
            inc.add(m.group(1))
    return inc


# ======================================================================================== struct fields
def collect_struct_decls():
    """Every C unit's declarations of address-suffixed (or symbol-named) globals, flattened to leaf fields.
    Returns {name: [(unit, info)]} where info = dict(type, size, elem, count, leaves=[...]).
    Needs cpp + pycparser (the Docker image); returns {} with a warning otherwise."""
    try:
        import structmap
        from pycparser import c_ast, c_parser
    except Exception as e:  # noqa: BLE001
        print(f'xref: struct fields skipped ({e}); run the build through tools/dr', file=sys.stderr)
        return {}, []
    structmap.subprocess = _CppShim     # strip __attribute__((...(...))) before structmap's own regexes
    cwd = os.getcwd()
    os.chdir(REPO)
    decls, failures = collections.defaultdict(list), []
    try:
        files = sorted(glob.glob('src/**/*.c', recursive=True))
        for f in files:
            try:
                text = structmap.preprocess(f)
                ast = c_parser.CParser().parse(text, f)
                lay = structmap.Layout(ast)
            except Exception as e:  # noqa: BLE001
                failures.append(f'{f}: {str(e)[:100]}')
                continue
            unit = f[4:-2]
            for ext in ast.ext:
                if not (isinstance(ext, c_ast.Decl) and ext.name) or isinstance(ext.type, c_ast.FuncDecl):
                    continue
                name = re.sub(r'__alias\d+$', '', ext.name)
                try:
                    info = _flatten_decl(lay, ext.type, c_ast, structmap)
                except Exception:  # noqa: BLE001
                    continue
                if info:
                    decls[name].append((unit, info))
    finally:
        os.chdir(cwd)
    return decls, failures


def strip_attributes(t):
    """Remove __attribute__((...)) with nested parentheses (structmap's regex stops at the first '))')."""
    out, i = [], 0
    while True:
        m = re.compile(r'\b__attribute__\s*\(').search(t, i)
        if not m:
            out.append(t[i:])
            return ''.join(out)
        out.append(t[i:m.start()])
        j, d = m.end(), 1
        while d and j < len(t):
            d += {'(': 1, ')': -1}.get(t[j], 0)
            j += 1
        i = j


class _CppShim:
    """Stands in for structmap's subprocess module: runs cpp, then strips attributes from its output."""
    @staticmethod
    def run(cmd, **kw):
        import subprocess
        r = subprocess.run(cmd, **kw)
        if isinstance(r.stdout, str):
            r.stdout = strip_attributes(r.stdout)
        return r


def _flatten_decl(lay, t, c_ast, structmap):
    count = None
    elem_t = t
    if isinstance(t, c_ast.ArrayDecl):
        elem_t = t.type
        try:
            count = lay.const(t.dim) if t.dim is not None else 0
        except Exception:  # noqa: BLE001
            count = 0
    esize, _ = lay.size_align(elem_t)
    leaves = []

    def walk(ty, base, path, depth):
        r = lay.resolve(ty)
        inner = r.type if isinstance(r, c_ast.TypeDecl) else r
        if isinstance(inner, c_ast.IdentifierType) and len(inner.names) == 1 and inner.names[0] in lay.typedefs:
            return walk(lay.typedefs[inner.names[0]], base, path, depth)
        if isinstance(inner, (c_ast.Struct, c_ast.Union)):
            body = inner if inner.decls is not None else lay.structs.get((type(inner).__name__, inner.name))
            if body is None:
                return
            for name, off, w, s, fty in lay.members(body)[0]:
                sub = f'{path}.{name}' if path else name
                if w is not None:
                    unit_bits = (off // (s * 8)) * (s * 8) if s else off
                    leaves.append((base + unit_bits // 8, s, off - unit_bits, w, None, sub,
                                   structmap.typestr(fty)))
                    continue
                walk(fty, base + off // 8, sub, depth + 1)
            return
        if isinstance(r, c_ast.ArrayDecl):
            s, _ = lay.size_align(r)
            es, _ = lay.size_align(r.type)
            if path:
                leaves.append((base, s, None, None, es, path, structmap.typestr(r)))
            if depth < 6 and structmap._is_struct(lay, r.type):
                walk(r.type, base, path + '[0]', depth + 1)
            return
        s, _ = lay.size_align(r)
        if path:
            leaves.append((base, s, None, None, None, path, structmap.typestr(r)))

    walk(elem_t, 0, '', 0)
    return dict(type=structmap.typestr(t), elem=esize, count=count, leaves=leaves)


def merge_decls(decls, name_addr):
    """Merge every unit's view of each global.
    -> {addr: {names, type, elem, count, size, units, leaves: [[off,size,bit,width,elem,name,type,votes,alts]]}}
    type/elem/count come from the most common declared type (count None = not an array, 0 = unsized array);
    size is the largest extent any view declares (0 = unknown)."""
    out = {}
    for name, views in decls.items():
        a = name_addr.get(name)
        if a is None:
            m = re.search(r'_?([0-9A-Fa-f]{8})$', name)
            if not m:
                continue
            a = int(m.group(1), 16)
        if region(a) is None:
            continue
        g = out.setdefault(a, dict(names=collections.Counter(), views=[], units=set(),
                                   leaves=collections.defaultdict(lambda: collections.defaultdict(set))))
        for unit, info in views:
            g['names'][name] += 1
            g['views'].append(info)
            g['units'].add(unit)
            for off, size, bit, width, elem, path, ty in info['leaves']:
                g['leaves'][(off, size, bit, width, elem)][(path, ty)].add(unit)
    res = {}
    for a, g in out.items():
        types = collections.Counter(v['type'] for v in g['views'])
        primary = types.most_common(1)[0][0]
        pv = [v for v in g['views'] if v['type'] == primary][0]

        def extent(v):
            if v['count'] is None:
                return v['elem']
            return v['elem'] * v['count'] if v['count'] else 0
        size = max(extent(v) for v in g['views'])
        leaves = []
        for key, names in g['leaves'].items():
            ranked = sorted(names.items(), key=lambda kv: (-len(kv[1]), _unk_name(kv[0][0]), kv[0][0]))
            (path, ty), units = ranked[0]
            alts = [p for (p, _), _u in ranked[1:4] if p != path]
            leaves.append([key[0], key[1], key[2], key[3], key[4], path, ty, len(units), alts])
        leaves.sort(key=lambda x: (x[0], -x[1], x[2] or 0))
        res[a] = dict(names=[n for n, _ in g['names'].most_common()], type=primary,
                      types=[t for t, _ in types.most_common(4)], elem=pv['elem'], count=pv['count'],
                      size=size, units=len(g['units']), leaves=leaves)
    return res


def _unk_name(path):
    last = path.split('.')[-1]
    return 1 if re.match(r'^(unk|pad|filler|field|f_?[0-9a-fA-F]+$|_)', last, re.I) else 0


# =============================================================================================== wiki
def wiki_index(known_names):
    """address -> {page: mentions} for every wiki page mentioning the address or a current symbol name."""
    out = collections.defaultdict(dict)
    titles = {}
    pat = re.compile(r'(?:sub_|gUnk_|0x|_)([0-9A-Fa-f]{8})\b')
    namepat = None
    if known_names:
        namepat = re.compile(r'\b(' + '|'.join(sorted((re.escape(n) for n in known_names), key=len,
                                                      reverse=True)) + r')\b')
    for p in sorted(glob.glob(os.path.join(REPO, 'wiki', '**', '*.md'), recursive=True)):
        page = os.path.basename(p)[:-3]
        if page in ('log', 'index'):
            continue
        rel = os.path.relpath(p, os.path.join(REPO, 'wiki'))
        text = open(p, errors='replace').read()
        m = re.search(r'^title:\s*(.+)$', text, re.M)
        titles[page] = [rel, m.group(1).strip() if m else page]
        seen = collections.Counter()
        for m in pat.finditer(text):
            seen[int(m.group(1), 16)] += 1
        if namepat:
            for m in namepat.finditer(text):
                seen[known_names[m.group(1)]] += 1
        for a, n in seen.items():
            out[a][page] = n
    return out, titles


# ========================================================================================= analysis
class Ctx:
    """Everything the per-function analysis needs."""

    def __init__(self, rom, funcs, mapping, call_via, fn_by_thumb, sym_ranges, strides):
        self.rom = rom
        self.funcs = funcs            # addr -> dict(name, size, mode)
        self.mapping = mapping        # sorted [(addr, kind)] kind in t/a/d
        self.map_lo = [m[0] for m in mapping]
        self.call_via = call_via      # addr -> register number
        self.fn_by_thumb = fn_by_thumb  # value (addr|thumb) -> function addr
        self.sym_ranges = sym_ranges  # sorted [(lo, hi)] of known objects for pointer joins
        self.sym_lo = [s[0] for s in sym_ranges]
        self.strides = strides        # table addr -> element size

    def word(self, a, size=4):
        o = a - BASE
        if 0 <= o and o + size <= len(self.rom):
            return int.from_bytes(self.rom[o:o + size], 'little')
        return None

    def obj_of(self, a):
        """Start of the known object containing a, for widening pointer joins."""
        i = bisect.bisect_right(self.sym_lo, a) - 1
        while i >= 0:
            lo, hi = self.sym_ranges[i]
            if lo <= a < hi:
                return lo
            if a - lo > 0x20000:
                break
            i -= 1
        r = region(a)
        if r in ('VRAM', 'PLTT', 'OAM', 'IO', 'SRAM'):
            return {'VRAM': 0x06000000, 'PLTT': 0x05000000, 'OAM': 0x07000000, 'IO': 0x04000000,
                    'SRAM': 0x0E000000}[r]
        return None


REGNUM = {f'r{i}': i for i in range(13)}
REGNUM.update(sb=9, sl=10, fp=11, ip=12, sp=13, lr=14, pc=15)
COND_AL = 15
LOADS = {'ldr': 4, 'ldrh': 2, 'ldrb': 1, 'ldrsh': 2, 'ldrsb': 1, 'ldrt': 4, 'ldrbt': 1}
STORES = {'str': 4, 'strh': 2, 'strb': 1, 'strt': 4, 'strbt': 1}
PTR_REGIONS = {'EWRAM', 'IWRAM', 'IO', 'PLTT', 'VRAM', 'OAM', 'ROM', 'SRAM', 'CODE'}


def is_ptr(v):
    return isinstance(v, int) and region(v) in PTR_REGIONS


def decode_function(ctx, md_t, md_a, start, size, mode):
    """-> list of insns (addr, size, mnem, ops, cc, writeback) for the code parts of [start, start+size)."""
    import capstone
    end = start + size
    chunks = []
    i = bisect.bisect_right(ctx.map_lo, start) - 1
    kind = ctx.mapping[i][1] if i >= 0 else mode
    if kind == 'd' and i >= 0 and ctx.mapping[i][0] < start:
        kind = mode  # a function symbol starts code even if the last mapping symbol was data
    pos = start
    j = bisect.bisect_right(ctx.map_lo, start)
    while pos < end:
        nxt = ctx.map_lo[j] if j < len(ctx.map_lo) else end
        nxt = min(max(nxt, pos), end)
        if kind in ('t', 'a') and nxt > pos:
            chunks.append((pos, nxt, kind))
        if nxt >= end:
            break
        pos = nxt
        if j < len(ctx.mapping):
            kind = ctx.mapping[j][1]
            j += 1
    insns = []
    for lo, hi, k in chunks:
        md = md_t if k == 't' else md_a
        a = lo
        while a < hi:
            got = False
            for ins in md.disasm(ctx.rom[a - BASE:hi - BASE], a):
                got = True
                ops = []
                for o in ins.operands:
                    if o.type == capstone.arm.ARM_OP_REG:
                        ops.append(('r', REGNUM.get(ins.reg_name(o.reg), -1), o.shift.type, o.shift.value))
                    elif o.type == capstone.arm.ARM_OP_IMM:
                        ops.append(('i', o.imm & 0xFFFFFFFF if o.imm >= 0 else o.imm))
                    elif o.type == capstone.arm.ARM_OP_MEM:
                        ops.append(('m', REGNUM.get(ins.reg_name(o.mem.base), -1) if o.mem.base else -1,
                                    REGNUM.get(ins.reg_name(o.mem.index), -1) if o.mem.index else -1,
                                    o.mem.disp, bool(o.subtracted), o.shift.type != 0))
                    else:
                        ops.append(('?',))
                rd = tuple(REGNUM.get(ins.reg_name(r), -1) for r in ins.regs_access()[0])
                insns.append((ins.address, ins.size, ins.mnemonic.split('.')[0], ops, ins.cc, ins.writeback, k, rd))
                a = ins.address + ins.size
            if a < hi and (not got or a < hi):
                if not got:
                    a += 2 if k == 't' else 4
                # disasm stopped at an undecodable word; resume after it
    return insns


BRANCH_RE = re.compile(r'^b(eq|ne|cs|hs|cc|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al)?$')


class FuncAnalysis:
    """Forward propagation of register / stack-slot values over one function's CFG."""
    MAXVISIT = 40

    def __init__(self, ctx, faddr, fsize, insns):
        self.ctx, self.start, self.end, self.insns = ctx, faddr, faddr + fsize, insns
        self.by_addr = {ins[0]: n for n, ins in enumerate(insns)}
        self.rec = None   # recorder during the final pass

    # ---------------------------------------------------------------- value helpers
    # values: int (known 32-bit), None (unknown), or a tuple (kind, key, off, idx):
    #   ('g', base, off|None, True)   pointer into the object at base, unknown index added
    #   ('arg', n, off|None, idx)     incoming argument register n (+off)
    #   ('ld', addr, off|None, idx)   pointer loaded from the global word at addr (+off)
    #   ('ldt', base, eoff, off)      word loaded from table base (+element offset eoff) (+off)
    #   ('sp', off)                   stack address (frame-relative)
    def add(self, a, b):
        if isinstance(a, int) and isinstance(b, int):
            return (a + b) & 0xFFFFFFFF
        if a is None and b is None:
            return None
        if a is None or (isinstance(a, int) and isinstance(b, tuple)):
            a, b = b, a
        # a is known (int or symbolic); b is int, symbolic or unknown
        if isinstance(a, int):            # b unknown: pointer constant plus an unknown index
            return ('g', a, 0, True) if is_ptr(a) else None
        if a[0] == 'sp':
            return self._addk(a, b) if isinstance(b, int) else None
        if isinstance(b, int):
            if is_ptr(b):                 # a was an index, b is the base pointer
                return ('g', b, 0, True)
            return self._addk(a, b)
        if isinstance(b, tuple) and b[0] == 'g' and a[0] != 'g':
            a = b
        if a[0] == 'ldt':
            return ('ldt', a[1], a[2], None)
        return (a[0], a[1], a[2], True)          # + unknown index

    def _addk(self, a, k):
        k = k - (1 << 32) if k & 0x80000000 else k
        if a[0] == 'sp':
            return ('sp', a[1] + k)
        if a[0] == 'ldt':
            return ('ldt', a[1], a[2], None if a[3] is None else a[3] + k)
        return (a[0], a[1], None if a[2] is None else a[2] + k, a[3])

    def join(self, a, b):
        if a == b:
            return a
        if a is None or b is None:
            return None
        if isinstance(a, int) and isinstance(b, int):
            if is_ptr(a) and is_ptr(b):
                oa, ob = self.ctx.obj_of(a), self.ctx.obj_of(b)
                if oa is not None and oa == ob:
                    return ('g', oa, None, True)
            return None
        if isinstance(a, int):
            a, b = b, a
        if isinstance(b, int):
            if a[0] == 'g' and is_ptr(b) and self.ctx.obj_of(b) == a[1]:
                return ('g', a[1], None, True)
            return None
        if a[0] != b[0] or a[1] != b[1]:
            return None
        if a[0] == 'sp':
            return None
        if a[0] == 'ldt':
            return ('ldt', a[1], a[2] if a[2] == b[2] else None, a[3] if a[3] == b[3] else None)
        return (a[0], a[1], a[2] if a[2] == b[2] else None, a[3] or b[3] or a[2] != b[2])

    def join_state(self, s1, s2):
        r1, k1 = s1
        r2, k2 = s2
        regs = [self.join(x, y) for x, y in zip(r1, r2)]
        slots = {}
        for o, v in k1.items():
            if o in k2:
                j = self.join(v, k2[o])
                if j is not None:
                    slots[o] = j
        return regs, slots

    # ---------------------------------------------------------------- CFG
    def blocks(self):
        leaders = {self.insns[0][0]} if self.insns else set()
        for n, ins in enumerate(self.insns):
            a, sz, mn, ops, cc, wb, k, _ = ins
            if (BRANCH_RE.match(mn) or (mn == 'bl' and self.start <= ops[0][1] < self.end)) and ops \
                    and ops[0][0] == 'i':
                leaders.add(ops[0][1])
                if n + 1 < len(self.insns):
                    leaders.add(self.insns[n + 1][0])
            elif mn in ('bx', 'pop') or (ops and ops[0][0] == 'r' and ops[0][1] == 15 and mn in ('mov', 'add', 'ldr')):
                if n + 1 < len(self.insns):
                    leaders.add(self.insns[n + 1][0])
        # jump-table entries: data words inside the function that point at its code
        for a in self.jt_candidates():
            leaders.add(a)
        self.leaders = sorted(x for x in leaders if x in self.by_addr)
        self.block_of = {}
        bl = []
        for i, a in enumerate(self.leaders):
            n0 = self.by_addr[a]
            hi = self.leaders[i + 1] if i + 1 < len(self.leaders) else None
            n1 = n0
            while n1 + 1 < len(self.insns) and self.insns[n1 + 1][0] != hi \
                    and self.insns[n1 + 1][0] == self.insns[n1][0] + self.insns[n1][1]:
                n1 += 1
            bl.append((n0, n1))
            self.block_of[a] = len(bl) - 1
        self.bl = bl

    def data_ranges(self):
        ctx, out = self.ctx, []
        j = max(bisect.bisect_right(ctx.map_lo, self.start) - 1, 0)
        while j < len(ctx.mapping) and ctx.mapping[j][0] < self.end:
            if ctx.mapping[j][1] == 'd':
                lo = max(ctx.mapping[j][0], self.start)
                hi = min(ctx.mapping[j + 1][0] if j + 1 < len(ctx.mapping) else self.end, self.end)
                if hi > lo:
                    out.append((lo, hi))
            j += 1
        return out

    def jt_candidates(self):
        """Data words inside the function that point at its own code (jump-table entries)."""
        if not hasattr(self, '_jt'):
            out = []
            for lo, hi in self.data_ranges():
                a = (lo + 3) & ~3
                while a + 4 <= hi:
                    w = self.ctx.word(a)
                    if w is not None and self.start <= (w & ~1) < self.end and (w & ~1) in self.by_addr:
                        out.append(w & ~1)
                    a += 4
            self._jt = out
        return self._jt

    def jt_entries(self, table):
        out = []
        a = table
        while True:
            w = self.ctx.word(a)
            if w is None or not (self.start <= (w & ~1) < self.end) or (w & ~1) not in self.by_addr:
                break
            out.append(w & ~1)
            a += 4
            if len(out) > 512:
                break
        return out

    # ---------------------------------------------------------------- run
    def run(self, recorder):
        if not self.insns:
            return
        self.blocks()
        init = [None] * 16
        for i in range(4):
            init[i] = ('arg', i, 0, False)
        init[13] = ('sp', 0)
        entry = self.block_of.get(self.start, 0)
        ins_state = {entry: (init, {})}
        visits = collections.Counter()
        work = [entry]
        inwork = {entry}
        self.rec = None
        while work:
            work.sort()
            b = work.pop(0)
            inwork.discard(b)
            visits[b] += 1
            st = ins_state[b]
            if visits[b] > self.MAXVISIT:
                st = ([None] * 13 + [st[0][13], None, None], {})
                ins_state[b] = st
            for succ, out in self.exec_block(b, st):
                if succ not in ins_state:
                    ins_state[succ] = out
                    changed = True
                else:
                    new = self.join_state(ins_state[succ], out)
                    changed = new != ins_state[succ]
                    if changed:
                        ins_state[succ] = new
                if changed and succ not in inwork and visits[succ] <= self.MAXVISIT:
                    work.append(succ)
                    inwork.add(succ)
        # final recording pass with the converged states; unreachable blocks start unknown
        self.rec = recorder
        for b in range(len(self.bl)):
            st = ins_state.get(b)
            if st is None:
                st = ([None] * 13 + [('sp', 0), None, None], {})
            for _ in self.exec_block(b, st):
                pass
        self.rec = None

    def exec_block(self, b, st):
        regs, slots = list(st[0]), dict(st[1])
        n0, n1 = self.bl[b]
        succs = []
        for n in range(n0, n1 + 1):
            ins = self.insns[n]
            res = self.step(ins, regs, slots, n)
            if res is not None:
                kind, targets = res
                for t in targets:
                    if t in self.block_of:
                        succs.append(self.block_of[t])
                if kind == 'end':
                    return [(s, (regs, slots)) for s in succs]
        # fall through
        nxt = self.insns[n1][0] + self.insns[n1][1]
        if nxt in self.block_of:
            succs.append(self.block_of[nxt])
        return [(s, (list(regs), dict(slots))) for s in succs]

    def operand_value(self, op, regs, ins_addr, mode):
        if op[0] == 'i':
            return op[1] & 0xFFFFFFFF
        if op[0] == 'r':
            r = op[1]
            if r == 15:
                return (ins_addr + (4 if mode == 't' else 8)) & 0xFFFFFFFF
            v = regs[r] if 0 <= r < 16 else None
            if op[2] and op[3]:      # shifted register operand (ARM)
                if isinstance(v, int) and op[2] == 2:   # LSL
                    return (v << op[3]) & 0xFFFFFFFF
                return None if not isinstance(v, int) else None
            return v
        return None

    def mem_addr(self, op, regs, ins_addr, mode):
        _, base, index, disp, sub, shifted = op
        if base == 15:
            pc = (ins_addr + 4) & ~3 if mode == 't' else ins_addr + 8
            return (pc + disp) & 0xFFFFFFFF, True
        bv = regs[base] if 0 <= base < 16 else None
        if index >= 0:
            iv = None if shifted else regs[index]
            if sub:
                iv = (-iv) & 0xFFFFFFFF if isinstance(iv, int) else None
            v = self.add(bv, iv)
        else:
            v = self.add(bv, disp & 0xFFFFFFFF) if bv is not None else None
        return v, False

    def load_value(self, addr, size, signed):
        if isinstance(addr, int):
            r = region(addr)
            if r in ('ROM', 'CODE'):
                w = self.ctx.word(addr, size)
                if w is not None and signed and w & (1 << (size * 8 - 1)):
                    w = (w - (1 << (size * 8))) & 0xFFFFFFFF
                return w
            if size == 4 and r in ('EWRAM', 'IWRAM'):
                return ('ld', addr, 0, False)
            return None
        if isinstance(addr, tuple):
            if addr[0] == 'sp':
                return None
            if size == 4 and addr[0] == 'g':
                return ('ldt', addr[1], addr[2], 0)
            if size == 4 and addr[0] in ('arg', 'ld', 'ldt'):
                return None
        return None

    def step(self, ins, regs, slots, n):
        a, sz, mn, ops, cc, wb, mode, rd_regs = ins
        rec = self.rec
        if rec:      # parameter use: an instruction reads a register still holding an incoming argument
            for r in rd_regs:
                v = regs[r] if 0 <= r < 16 else None
                if isinstance(v, tuple) and v[0] == 'arg':
                    rec.used_args.add(v[1])
                elif isinstance(v, tuple) and v[0] == 'ret':
                    rec.ret_used.add(v[1])
        conditional = cc != COND_AL and cc != 0 and not BRANCH_RE.match(mn)
        saved = list(regs) if conditional else None

        def setr(r, v):
            if 0 <= r < 16:
                regs[r] = v

        def finish():
            if conditional:
                for i in range(16):
                    regs[i] = self.join(saved[i], regs[i])

        base_mn = mn
        if mode == 'a' and len(mn) > 2 and mn[-2:] in ('eq', 'ne', 'cs', 'hs', 'cc', 'lo', 'mi', 'pl', 'vs', 'vc',
                                                       'hi', 'ls', 'ge', 'lt', 'gt', 'le') and cc != COND_AL:
            base_mn = mn[:-2]
        m = base_mn.rstrip('s') if base_mn not in ('ldrsh', 'ldrsb', 'bics', 'movs', 'mvns') else base_mn
        if base_mn in ('movs', 'mvns', 'bics'):
            m = base_mn[:-1]
        # ---- branches and calls
        if base_mn == 'bl' and self.start <= ops[0][1] < self.end and ops[0][1] != self.start:
            return ('end', [ops[0][1]])        # agbcc's far jump inside a large function, not a call
        if base_mn == 'bl':
            t = ops[0][1]
            args = regs[:4]
            if rec:
                rec.call(a, t, args, regs, self)
            for r in (0, 1, 2, 3, 12, 14):
                regs[r] = None
            if t not in self.ctx.call_via:
                regs[0] = ('ret', t, 0, False)     # the callee's return value, if anyone reads it
            if any(isinstance(x, tuple) and x[0] == 'sp' for x in args):
                slots.clear()
            return None
        if BRANCH_RE.match(mn) and ops and ops[0][0] == 'i':
            t = ops[0][1]
            uncond = mn == 'b' or (mode == 'a' and cc == COND_AL)
            if not (self.start <= t < self.end):
                if rec:
                    rec.tail(a, t, regs)
                if uncond:
                    return ('end', [])
                return ('cont', [])
            if uncond:
                return ('end', [t])
            return ('cont', [t])
        if base_mn == 'bx':
            r = ops[0][1]
            v = regs[r] if 0 <= r < 16 else None
            prev = self.insns[n - 1] if n > 0 else None
            is_call = prev is not None and prev[2] == 'mov' and prev[3][0][1] == 14 and prev[3][1][1] == 15
            if r == 14 and not is_call:
                return ('end', []) if cc == COND_AL or mode == 't' else ('cont', [])
            if is_call:
                if rec:
                    rec.icall(a, v, regs, self)
                for rr in (0, 1, 2, 3, 12, 14):
                    regs[rr] = None
                return None
            if isinstance(v, tuple) and v[0] == 'ldt' and self.start <= v[1] < self.end:
                return ('end', self.jt_entries(v[1] + (v[2] or 0)))
            if isinstance(v, int) and region(v) == 'CODE' and not (self.start <= (v & ~1) < self.end):
                if rec:
                    rec.tail(a, v & ~1, regs)
            elif isinstance(v, tuple) and v[0] in ('ld', 'ldt') and rec:
                rec.icall(a, v, regs, self)          # jump through a function-pointer slot or table
            return ('end', []) if cc == COND_AL or mode == 't' else ('cont', [])
        if base_mn in ('svc', 'swi'):
            num = ops[0][1] >> 16 if mode == 'a' and ops[0][1] > 0xFF else ops[0][1]
            if rec:
                rec.swi(a, num, regs)
                for r in range(SWI_NARGS.get(num, 4)):     # the BIOS reads its arguments implicitly
                    if isinstance(regs[r], tuple) and regs[r][0] == 'arg':
                        rec.used_args.add(regs[r][1])
            for r in (0, 1, 3):
                regs[r] = None
            return None

        # ---- writes to pc (mov pc, rX / add pc / ldr pc)
        if ops and ops[0][0] == 'r' and ops[0][1] == 15 and base_mn in ('mov', 'add', 'ldr'):
            prev = self.insns[n - 1] if n > 0 else None
            is_call = prev is not None and prev[2] == 'mov' and prev[3][0][1] == 14 and prev[3][1][1] == 15
            if base_mn == 'ldr':
                addr, _ = self.mem_addr(ops[1], regs, a, mode)
                v = self.load_value(addr, 4, False)
                if rec:
                    rec.mem(a, addr, 4, 'r', self)
            elif base_mn == 'mov':
                v = self.operand_value(ops[1], regs, a, mode)
            else:
                v = self.add(regs[15] if False else (a + 4), self.operand_value(ops[1], regs, a, mode))
            if is_call:
                if rec:
                    rec.icall(a, v, regs, self)
                for rr in (0, 1, 2, 3, 12, 14):
                    regs[rr] = None
                return None
            if isinstance(v, tuple) and v[0] == 'ldt' and self.start <= v[1] < self.end:
                return ('end', self.jt_entries(v[1] + (v[2] or 0)))
            if base_mn == 'mov' and ops[1][0] == 'r' and ops[1][1] == 14:
                return ('end', []) if cc == COND_AL else ('cont', [])
            if isinstance(v, tuple) and v[0] == 'ldt':
                return ('end', self.jt_entries(v[1] + (v[2] or 0)))
            return ('end', self.jt_candidates()) if cc == COND_AL else ('cont', self.jt_candidates())

        # ---- push / pop / ldm / stm
        if base_mn in ('push', 'stmdb') and (base_mn == 'push' or (ops[0][1] == 13 and wb)):
            rl = [o[1] for o in (ops if base_mn == 'push' else ops[1:])]
            sp = regs[13]
            if isinstance(sp, tuple) and sp[0] == 'sp':
                nsp = sp[1] - 4 * len(rl)
                for i, r in enumerate(sorted(rl)):
                    slots[nsp + 4 * i] = regs[r]
                regs[13] = ('sp', nsp)
            else:
                regs[13] = None
            finish()
            return None
        if base_mn == 'pop' or (base_mn in ('ldm', 'ldmia', 'ldmfd') and ops[0][1] == 13 and wb):
            rl = [o[1] for o in (ops if base_mn == 'pop' else ops[1:])]
            sp = regs[13]
            for i, r in enumerate(sorted(rl)):
                v = slots.get(sp[1] + 4 * i) if isinstance(sp, tuple) and sp[0] == 'sp' else None
                if r == 15:
                    finish()
                    return ('end', [])
                setr(r, v)
            regs[13] = ('sp', sp[1] + 4 * len(rl)) if isinstance(sp, tuple) and sp[0] == 'sp' else None
            finish()
            return None
        if base_mn in ('ldm', 'ldmia', 'ldmfd', 'stm', 'stmia', 'stmea', 'ldmib', 'stmib', 'ldmda', 'stmda',
                       'ldmdb', 'stmdb', 'stmfd'):
            bv = regs[ops[0][1]]
            rl = [o[1] for o in ops[1:]]
            load = base_mn.startswith('ldm')
            for i, r in enumerate(sorted(rl)):
                ad = self.add(bv, 4 * i) if bv is not None else None
                if rec and not (isinstance(ad, tuple) and ad[0] == 'sp'):
                    rec.mem(a, ad, 4, 'r' if load else 'w', self, value=None if load else regs[r])
                if load:
                    if r == 15:
                        finish()
                        return ('end', [])
                    setr(r, self.load_value(ad, 4, False) if isinstance(ad, int) else None)
            if wb and not (load and ops[0][1] in rl):
                regs[ops[0][1]] = self.add(bv, 4 * len(rl)) if bv is not None else None
            finish()
            return None

        # ---- loads / stores
        if base_mn in LOADS or base_mn in STORES:
            size = LOADS.get(base_mn) or STORES.get(base_mn)
            rd = ops[0][1]
            mop = ops[1] if len(ops) > 1 and ops[1][0] == 'm' else None
            if mop is None:
                finish()
                return None
            addr, lit = self.mem_addr(mop, regs, a, mode)
            post = mode == 'a' and len(ops) > 2       # post-indexed: the address is the base register
            if post:
                addr = regs[mop[1]] if 0 <= mop[1] < 16 else None
            if base_mn in LOADS:
                if lit:
                    v = self.ctx.word(addr, size) if addr is not None else None
                    if rec:
                        rec.literal(a, v, self)
                    setr(rd, v)
                elif isinstance(addr, tuple) and addr[0] == 'sp' and addr[1] is not None:
                    if addr[1] >= 0 and rec:            # above the entry sp: a stack argument
                        rec.used_args.add(4 + addr[1] // 4)
                    setr(rd, slots.get(addr[1]) if size == 4 else None)
                else:
                    if rec:
                        rec.mem(a, addr, size, 'r', self)
                    setr(rd, self.load_value(addr, size, base_mn in ('ldrsh', 'ldrsb')))
            else:
                v = regs[rd] if 0 <= rd < 16 else None
                if rec and size == 4 and is_ptr(v) and region(v) != 'CODE':
                    rec.escapes[v] += 1            # a data address stored somewhere (stack argument, register)
                if isinstance(addr, tuple) and addr[0] == 'sp' and addr[1] is not None:
                    if size == 4:
                        slots[addr[1]] = v
                    else:
                        slots.pop(addr[1] & ~3, None)
                else:
                    if rec:
                        rec.mem(a, addr, size, 'w', self, value=v)
            if post and mop[1] >= 0:
                off = self.operand_value(ops[2], regs, a, mode)
                regs[mop[1]] = self.add(regs[mop[1]], off) if regs[mop[1]] is not None else None
            elif wb and mop[1] >= 0 and mop[1] != 15:
                regs[mop[1]] = self.add(regs[mop[1]], mop[3] & 0xFFFFFFFF) if regs[mop[1]] is not None else None
            finish()
            return None

        # ---- data processing
        if not ops:
            finish()
            return None
        rd = ops[0][1] if ops[0][0] == 'r' else -1
        if m in ('cmp', 'cmn', 'tst', 'teq'):
            return None
        if base_mn == 'adr':
            pc = (a + 4) & ~3 if mode == 't' else a + 8
            v = (pc + ops[1][1]) & 0xFFFFFFFF
            if rec:
                rec.literal(a, v, self)
            setr(rd, v)
            finish()
            return None
        if m in ('mov', 'mvn'):
            v = self.operand_value(ops[1], regs, a, mode) if len(ops) > 1 else None
            if m == 'mvn':
                v = (~v) & 0xFFFFFFFF if isinstance(v, int) else None
            if rec and isinstance(v, int) and ops[1][0] == 'i':
                rec.literal(a, v, self, imm=True)
            setr(rd, v)
            finish()
            return None
        if len(ops) == 2:
            x, y = self.operand_value(ops[0], regs, a, mode), self.operand_value(ops[1], regs, a, mode)
        elif len(ops) >= 3:
            x, y = self.operand_value(ops[1], regs, a, mode), self.operand_value(ops[2], regs, a, mode)
        else:
            x = y = None
        v = None
        if m == 'add':
            v = self.add(x, y)
        elif m == 'sub':
            if isinstance(y, int):
                v = self.add(x, (-y) & 0xFFFFFFFF) if x is not None else None
            else:
                v = self.add(x, None) if isinstance(x, (int, tuple)) and x is not None else None
                if isinstance(x, tuple) and x[0] == 'sp':
                    v = None
        elif m == 'rsb':
            v = (y - x) & 0xFFFFFFFF if isinstance(x, int) and isinstance(y, int) else None
        elif m in ('lsl', 'lsr', 'asr', 'ror'):
            if isinstance(x, int) and isinstance(y, int):
                y &= 0xFF
                if m == 'lsl':
                    v = (x << y) & 0xFFFFFFFF if y < 32 else 0
                elif m == 'lsr':
                    v = x >> y if y < 32 else 0
                elif m == 'asr':
                    sx = x - (1 << 32) if x & 0x80000000 else x
                    v = (sx >> min(y, 31)) & 0xFFFFFFFF
                else:
                    y &= 31
                    v = ((x >> y) | (x << (32 - y))) & 0xFFFFFFFF
        elif m in ('and', 'orr', 'eor', 'bic', 'mul'):
            if isinstance(x, int) and isinstance(y, int):
                v = {'and': x & y, 'orr': x | y, 'eor': x ^ y, 'bic': x & ~y, 'mul': x * y}[m] & 0xFFFFFFFF
        elif m in ('adc', 'sbc', 'neg', 'mla', 'umull', 'smull'):
            v = None
        if rd == 13:
            if m in ('add', 'sub') and isinstance(regs[13], tuple):
                pass
            else:
                v = v if isinstance(v, tuple) else None
        setr(rd, v)
        if m == 'umull' or m == 'smull':
            setr(ops[1][1], None)
        finish()
        return None


class Recorder:
    """Collects the facts of one function during the final pass."""

    def __init__(self, ctx, faddr):
        self.ctx, self.faddr = ctx, faddr
        self.calls = []        # [target, kind, site, args, extra]
        self.mems = collections.Counter()     # (addr, size, rw, idx) -> n   (addr = exact or object base)
        self.memoff = {}       # (addr, size, rw, idx) -> off known within object (or None)
        self.params = collections.Counter()   # (argn, off, size, rw, idx)
        self.deref = collections.Counter()    # (kind, base, eoff, off, size, rw)  kind ld|ldt
        self.lits = collections.Counter()     # value -> n (pointer-like literals/immediates)
        self.fnrefs = collections.Counter()   # fn addr -> n
        self.swis = collections.Counter()
        self.slot_stores = set()  # (slot addr or ('g', base, off), fn addr)
        self.param_slots = set()  # (argn, slot)
        self.used_args = set()    # argument indices this function reads (4+ = stack arguments)
        self.escapes = collections.Counter()   # data addresses stored to memory or the stack
        self.passes = set()       # (callee, callee arg index, own arg index): argument passed through unchanged
        self.ret_used = set()     # callees whose return value (r0 after the call) this function reads
        self.ret_pass = set()     # (callee, next callee, arg index): a return value passed straight on

    def _fn(self, v):
        if isinstance(v, int):
            return self.ctx.fn_by_thumb.get(v)
        return None

    def call(self, site, t, args, regs, fa):
        r = self.ctx.call_via.get(t)
        if r is not None:
            self.icall(site, regs[r], regs, fa, via=t)
            return
        self.calls.append([t, 'bl', site, self._args(args)])
        for i, v in enumerate(args):
            self._escape(v, site)
            if isinstance(v, tuple) and v[0] == 'arg':
                if v[2] == 0 and not v[3]:
                    self.passes.add((t, i, v[1]))
                else:
                    self.used_args.add(v[1])
            elif isinstance(v, tuple) and v[0] == 'ret':
                self.ret_pass.add((v[1], t, i))

    def _escape(self, v, site):
        f = self._fn(v)
        if f is not None:
            self.fnrefs[f] += 1

    def _args(self, args):
        out = {}
        for i, v in enumerate(args):
            if isinstance(v, int):
                out[f'r{i}'] = v
            elif isinstance(v, tuple) and v[0] in ('arg', 'ld', 'g') and v[2] is not None and not v[3]:
                out[f'r{i}'] = [v[0], v[1], v[2]]
            elif isinstance(v, tuple) and v[0] == 'g':
                out[f'r{i}'] = ['g', v[1], v[2]]
            elif isinstance(v, tuple) and v[0] == 'ldt' and v[3] == 0:
                out[f'r{i}'] = ['ldt', v[1], v[2]]
        return out

    def icall(self, site, v, regs, fa, via=None):
        r = self.ctx.call_via.get(via) if via is not None else None
        args = self._args([None if i == r else x for i, x in enumerate(regs[:4])])
        f = self._fn(v)
        if f is not None:
            self.calls.append([f, 'ptr', site, args])
            return
        if isinstance(v, tuple):
            if v[0] == 'ld' and v[2] == 0 and not v[3]:
                self.calls.append([None, 'slot', site, args, v[1]])
                return
            if v[0] == 'ld':
                self.calls.append([None, 'slotidx', site, args, v[1], v[2]])
                return
            if v[0] == 'ldt':
                self.calls.append([None, 'table', site, args, v[1], v[2]])
                return
            if v[0] == 'arg':
                self.calls.append([None, 'argfn', site, args, v[1], v[2]])
                return
        self.calls.append([None, 'unknown', site, args])

    def tail(self, site, t, regs):
        f = self.ctx.funcs.get(t)
        if f is not None or region(t) == 'CODE':
            self.calls.append([t, 'tail', site, self._args(regs[:4])])

    def swi(self, site, n, regs):
        self.swis[n] += 1

    def literal(self, site, v, fa, imm=False):
        if v is None:
            return
        f = self._fn(v)
        if f is not None and not imm:
            self.fnrefs[f] += 1
            return
        if not imm and is_ptr(v):
            self.lits[v] += 1

    def mem(self, site, addr, size, rw, fa, value=None):
        if rw == 'w' and value is not None:
            f = self._fn(value)
            if f is not None:
                self.fnrefs[f] += 1
                if isinstance(addr, int):
                    self.slot_stores.add((addr, f))
                elif isinstance(addr, tuple) and addr[0] == 'g':
                    self.slot_stores.add((addr[1], f))
            elif isinstance(value, tuple) and value[0] == 'arg' and value[2] == 0 and not value[3]:
                if isinstance(addr, int):
                    self.param_slots.add((value[1], addr))
            elif isinstance(value, int) and region(value) == 'ROM':
                self.lits[value] += 1
        if addr is None:
            return
        if isinstance(addr, int):
            if region(addr) in ('CODE',):
                return
            if region(addr) is None:
                return
            self.mems[(addr, size, rw, False)] += 1
            return
        k = addr[0]
        if k == 'g':
            if region(addr[1]) in ('CODE', None):
                return
            self.mems[(addr[1], size, rw, True)] += 1
            key = (addr[1], size, rw, True)
            offs = self.memoff.setdefault(key, set())
            offs.add(addr[2])
        elif k == 'arg':
            self.params[(addr[1], addr[2], size, rw, addr[3])] += 1
        elif k == 'ld':
            self.deref[('ld', addr[1], None, addr[2], size, rw, addr[3])] += 1
        elif k == 'ldt':
            self.deref[('ldt', addr[1], addr[2], addr[3], size, rw, True)] += 1


# ===================================================================================== text decoding
TEXT_ASSET_PREFIXES = ('rodata/', 'tables/', 'text/')   # where C string literals and text tables live
# Fixed record layouts of the text and card tables (facts about the ROM, independent of the asset file format).
RECORD_TABLES = {'cards/names': 0x40, 'cards/descriptions': 0x1E0, 'text/dialogue': 0x304, 'text/duelists': 0x84,
                 'cards/number_to_id': 2, 'cards/id_to_number': 2, 'cards/sort_keys': 2, 'cards/stats': 4,
                 'cards/passwords': 4, 'tables/card_effect_handlers': 0x18}


def asset_stem(path):
    """'cards/passwords.csv' -> 'cards/passwords' (asset files change format; the ROM layout does not)."""
    return re.sub(r'\.(json|csv|png|pal|bin|txt)$', '', path)


_TEXT = None


def text_helpers():
    """(dec, text_field) from tools/assets.py; a local copy if it can't be imported (e.g. a broken plugin)."""
    global _TEXT
    if _TEXT is None:
        try:
            import assets
            _TEXT = (assets.dec, assets.text_field)
        except BaseException:  # noqa: BLE001  (assets.py may sys.exit on a manifest it can't load)
            table = []
            for b in range(256):
                try:
                    table.append(bytes([b]).decode('cp1252'))
                except UnicodeDecodeError:
                    table.append(chr(b))

            def dec(raw):
                return ''.join(table[x] for x in raw)

            def text_field(raw):
                n = raw.find(b'\0')
                n = len(raw) if n < 0 else n
                tail = raw[n:]
                return dec(raw[:n]) if tail.strip(b'\0') == b'' else {'text': dec(raw[:n]), 'raw_tail': tail.hex()}
            _TEXT = (dec, text_field)
    return _TEXT


def read_manifest():
    """config/assets.tsv rows as dicts (start, end, type, path, options); unknown types are fine here."""
    path = os.environ.get('EDS_ASSET_MANIFEST', os.path.join(REPO, 'config', 'assets.tsv'))
    out = []
    for line in open(path):
        line = line.split('#')[0].strip()
        if not line:
            continue
        f = line.split()
        if len(f) < 4:
            continue
        p = dict(kv.split('=', 1) for kv in f[4:] if '=' in kv)
        p.update(start=int(f[0], 16), end=int(f[1], 16), type=f[2], path=f[3], stem=asset_stem(f[3]))
        out.append(p)
    return sorted(out, key=lambda p: p['start'])


def decode_cstring(rom, a, maxlen=0x400):
    """NUL-terminated game text at address a, or None when the bytes don't look like text."""
    dec = text_helpers()[0]
    o = a - BASE
    if not (0 <= o < len(rom)):
        return None
    end = rom.find(b'\0', o, o + maxlen)
    if end < 0:
        return None
    raw = rom[o:end]
    if len(raw) < 2:
        return None
    if any(b < 0x20 and b not in (9, 10, 13) for b in raw):
        return None
    if all(0x20 <= b < 0x7F or b in (9, 10, 13) for b in raw):
        letters = sum(1 for b in raw if chr(b).isalnum())
        if letters < max(2, len(raw) // 3) or len(set(raw)) < 2:
            return None
        return dec(raw)
    try:
        s = raw.decode('shift_jis')
    except UnicodeDecodeError:
        s = None
    if s is not None:       # Japanese text: full-width characters only (half-width katakana is mostly data)
        wide = sum(1 for ch in s if ord(ch) >= 0x3000 and not 0xFF61 <= ord(ch) <= 0xFF9F)
        if wide >= 3 and all(ord(ch) < 0x7F or (ord(ch) >= 0x3000 and not 0xFF61 <= ord(ch) <= 0xFF9F)
                             for ch in s) and len(set(s)) > 2:
            return s
        return None
    # Windows-1252 English text with a few accented letters
    ascii_n = sum(1 for b in raw if 0x20 <= b < 0x7F)
    if ascii_n >= 0.85 * len(raw) and sum(1 for b in raw if chr(b).isalpha()) >= len(raw) // 2:
        return dec(raw)
    return None


def string_records(rom, lo, hi, limit=96):
    """Inline text at a constant stride inside [lo, hi) (records like {...; char name[N]; ...}), or []."""
    found, a = [], (lo + 3) & ~3
    hi = min(hi, lo + 0x8000)
    while a < hi and len(found) <= limit:
        t = decode_cstring(rom, a)
        if t is not None and len(t) >= 3:
            found.append((a, t))
            end = rom.find(b'\0', a - BASE) + BASE
            a = (end + 4) & ~3
        else:
            a += 4
    if len(found) < 3:
        return []
    steps = {found[i + 1][0] - found[i][0] for i in range(len(found) - 1)}
    return found if len(steps) == 1 else []


class AssetMap:
    def __init__(self, rom, labels):
        self.rom = rom
        self.rows = read_manifest()
        self.lo = [p['start'] for p in self.rows]
        self.labels = sorted(labels.items())
        self.label_lo = [x[0] for x in self.labels]

    def asset(self, a):
        i = bisect.bisect_right(self.lo, a) - 1
        if i >= 0 and self.rows[i]['start'] <= a < self.rows[i]['end']:
            return self.rows[i]
        return None

    def label(self, a):
        i = bisect.bisect_right(self.label_lo, a) - 1
        if i >= 0:
            lo, name = self.labels[i]
            p = self.asset(a)
            if p is None or p['start'] <= lo:
                return lo, name
        return None, None

    def label_end(self, lo):
        i = bisect.bisect_right(self.label_lo, lo)
        hi = self.labels[i][0] if i < len(self.labels) else ROM_END
        p = self.asset(lo)
        if p:
            hi = min(hi, p['end'])
        return hi

    def by_path(self, stem):
        for p in self.rows:
            if p['stem'] == asset_stem(stem):
                return p
        return None

    def card_name(self, cid):
        """English name of card ID cid (1..820), or None."""
        p = self.by_path('cards/names')
        if p is None or not cid:
            return None
        size = RECORD_TABLES['cards/names']
        if not (0 < cid < (p['end'] - p['start']) // size):
            return None
        o = p['start'] - BASE + cid * size
        t = text_helpers()[1](self.rom[o:o + size])
        return t['text'] if isinstance(t, dict) else t

    def number_to_id(self, no):
        """Card number -> card ID (the CardNumberToId rule on wiki/data/card-id-map.md)."""
        p = self.by_path('cards/number_to_id')
        if p is None or no == 0xFFFF or no < 0:
            return 0
        idx, extra = ((no & 0x7FF), 0) if no <= 1999 else (((no - 2000) & 0x7FF), 1)
        o = p['start'] - BASE + idx * 2
        cid = int.from_bytes(self.rom[o:o + 2], 'little')
        return cid + extra if cid else 0

    def card_label(self, cid):
        n = self.card_name(cid)
        return f'card {cid} "{n}"' if n else f'card {cid}'

    def describe(self, a):
        """What a ROM data address is: dict(asset, atype, index, field, what, text). Keyed on the asset's
        path (minus extension) and the ROM's fixed record layouts, so it survives asset format changes."""
        text_field = text_helpers()[1]
        p = self.asset(a)
        if p is None:
            return {}
        d = dict(asset=p['path'], atype=p['type'], aoff=a - p['start'])
        stem = p['stem']
        rel = a - p['start']
        size = RECORD_TABLES.get(stem)
        if size is None:
            return d
        d['record'] = True
        d['index'], d['field'] = i, fld = divmod(rel, size)
        s = p['start'] - BASE + i * size          # record start (file offset)
        if rel == 0 and stem != 'cards/number_to_id':
            d['what'] = f'table base ({(p["end"] - p["start"]) // size} x 0x{size:X})'
            return d
        if stem in ('cards/names', 'cards/descriptions'):
            if fld == 0:
                d['text'] = text_field(self.rom[s:s + size])
            d['what'] = ('name of ' if stem == 'cards/names' else 'description of ') + self.card_label(i)
        elif stem == 'text/dialogue':
            ev = int.from_bytes(self.rom[s:s + 2], 'little')
            d['what'] = f'dialogue record {i} (event {ev})'
            if fld in (0, 4):
                d['text'] = text_field(self.rom[s + 4:s + 0x304])
        elif stem == 'text/duelists':
            d['text'] = text_field(self.rom[s + 4:s + 0x44])
            d['what'] = f'duelist {i}'
        elif stem == 'cards/number_to_id':
            cid = self.number_to_id(i)
            d['what'] = f'card number {i} -> ' + (self.card_label(cid) if cid else 'none')
        elif stem in ('cards/id_to_number', 'cards/stats', 'cards/passwords', 'cards/sort_keys'):
            d['what'] = {'cards/id_to_number': 'number of ', 'cards/stats': 'stats of ',
                         'cards/passwords': 'password of ', 'cards/sort_keys': 'sort key of '}[stem] + \
                self.card_label(i)
        elif stem == 'tables/card_effect_handlers':
            key = int.from_bytes(self.rom[s:s + 4], 'little')
            cid = self.number_to_id(key) if key < 0x1000 else 0
            d['what'] = (f'effect record {i}, key {key}' + (f' = {self.card_label(cid)}' if cid else '') +
                         (f', fn[{(fld - 4) // 4}]' if fld >= 4 else ''))
        if isinstance(d.get('text'), dict):
            d['text'] = d['text']['text']
        return d


# ========================================================================================== build
def cmd_build(args):
    t0 = time.time()
    try:
        import capstone
    except ImportError:
        sys.exit('xref build needs capstone: run it as `tools/dr python3 tools/xref.py build`')
    elf = args.elf
    if not os.path.exists(elf):
        sys.exit(f'{elf} is missing: build it first with `tools/dr make -j8 compare`')
    rom, syms, text_idx = read_elf(elf)
    ftsv = read_functions_tsv()
    extra = read_symbol_files()
    proposed = read_names_txt()
    io_names = read_io_names()
    data_labels = read_data_labels()
    map_units = read_map_units()
    unmatched = read_unmatched()

    # ---- functions and symbols
    funcs = {}
    mapping = []
    data_syms = {}
    cand_names = collections.defaultdict(set)
    call_via = {}
    for name, value, size, bind, typ, shndx in syms:
        if name.startswith('$'):
            if shndx == text_idx and name[1:2] in ('t', 'a', 'd'):
                mapping.append((value, name[1]))
            continue
        if typ == 2 and shndx == text_idx:     # FUNC
            a = value & ~1
            thumb = value & 1
            m = re.fullmatch(r'_call_via_(\w+)', name)
            if m:
                call_via[a] = REGNUM.get(m.group(1), -1)
            if a not in funcs or bind == 1:
                funcs[a] = dict(name=name, size=size, mode='t' if thumb else 'a', bind=bind)
        elif typ in (0, 1) and bind == 1:          # global data label / absolute symbol
            if region(value) and region(value) != 'CODE':
                cand_names[value].add(name)
    for a, r in ftsv.items():
        f = funcs.setdefault(a, dict(name=r['name'], size=r['size'], mode=r['mode']))
        f['name'], f['size'], f['mode'], f['unit'] = r['name'], r['size'], r['mode'], r['unit']
    hand_named = set()
    for name, a in extra.items():
        if region(a) not in (None, 'CODE'):
            cand_names[a].add(name)
            hand_named.add(name)
    for a, name in data_labels.items():
        cand_names[a].add(name)
    for a, name in BIOS_RAM.items():
        cand_names[a].add(name)

    def name_rank(n, a):
        if n in hand_named:
            return (0, n)
        if re.fullmatch(r'gUnk_%08X' % a, n):
            return (1, n)
        if 'alias' in n.lower() or not re.search('%08X' % a, n, re.I) and n not in BIOS_RAM.values():
            return (3, n)
        return (2, n)
    for a, ns in cand_names.items():
        data_syms[a] = min(ns, key=lambda n: name_rank(n, a))
    mapping.sort()
    # thumb-bit values -> function
    fn_by_thumb = {}
    for a, f in funcs.items():
        fn_by_thumb[a | (1 if f['mode'] == 't' else 0)] = a
    # unit for library code
    mlo = [m[0] for m in map_units]
    for a, f in funcs.items():
        if 'unit' not in f:
            i = bisect.bisect_right(mlo, a) - 1
            f['unit'] = map_units[i][2] if i >= 0 and a < map_units[i][1] else '?'
        f['src'] = 'asm' if f['name'] in unmatched else ('lib' if f['unit'].startswith('lib') else 'c')
    for a, f in funcs.items():
        if f['unit'] in ('crt0', 'sound_mixer_arm', 'sdk/libagbsyscall', 'veneer') or f['unit'].startswith('asm'):
            f['src'] = 'asm'

    # ---- struct declarations (field names)
    name_addr = {f['name']: a for a, f in funcs.items()}
    name_addr.update({n: a for a, ns in cand_names.items() for n in ns})
    name_addr.update(extra)
    decls_raw, failures = ({}, []) if args.no_structs else collect_struct_decls()
    decls = merge_decls(decls_raw, name_addr)
    for a, d in decls.items():
        if a not in data_syms:
            data_syms[a] = d['names'][0]

    # ---- object ranges (for pointer joins and display)
    sorted_syms = sorted(data_syms)
    obj = {}
    for i, a in enumerate(sorted_syms):
        nxt = sorted_syms[i + 1] if i + 1 < len(sorted_syms) else a + 0x1000
        hi = min(nxt, a + 0x10000)
        if region(a) != region(hi - 1):
            hi = a + 4
        obj[a] = hi
    for a, d in decls.items():
        if d['size']:
            obj[a] = max(obj.get(a, a), a + d['size'])
    assetmap = AssetMap(rom, data_labels)
    for a in data_labels:
        obj[a] = assetmap.label_end(a)
    sym_ranges = sorted(obj.items())
    strides = {a: d['elem'] for a, d in decls.items() if d['elem'] and d['count'] is not None}

    ctx = Ctx(rom, funcs, mapping, call_via, fn_by_thumb, sym_ranges, strides)
    md_t = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
    md_t.detail = True
    md_a = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    md_a.detail = True

    # ---- analyse every function
    out_funcs = {}
    for a in sorted(funcs):
        f = funcs[a]
        if not f['size'] or region(a) != 'CODE':
            continue
        insns = decode_function(ctx, md_t, md_a, a, f['size'], f['mode'])
        rec = Recorder(ctx, a)
        try:
            FuncAnalysis(ctx, a, f['size'], insns).run(rec)
        except Exception as e:  # noqa: BLE001
            print(f'xref: {f["name"]}: analysis failed: {e!r}', file=sys.stderr)
        out_funcs[a] = (f, rec, len(insns))

    def table_members(base, eoff):
        """Functions stored in a ROM table (all words of the label at element offset eoff) or RAM slots."""
        out = []
        if region(base) == 'ROM':
            lo, _ = assetmap.label(base)
            lo = base if lo is None else lo
            hi = obj.get(lo, base + 0x400)
            stride = strides.get(base) or strides.get(lo)
            p = base
            while p + 4 <= hi and p - base < 0x4000:
                w = ctx.word(p)
                if w in fn_by_thumb:
                    if eoff is None or not stride or (p - base - eoff) % stride == 0:
                        out.append(fn_by_thumb[w])
                p += 4
            out = list(dict.fromkeys(out))
        else:
            hi = obj.get(base, base + 4)
            for slot, fns in sorted(slot_fns.items()):
                if base <= slot < hi:
                    out += sorted(fns)
        return list(dict.fromkeys(out))

    # ---- resolve RAM function-pointer slots (stores + argument propagation through call sites)
    slot_fns = collections.defaultdict(set)
    param_slot = collections.defaultdict(set)
    for a, (f, rec, _) in out_funcs.items():
        for slot, fn in rec.slot_stores:
            slot_fns[slot].add(fn)
        for argn, slot in rec.param_slots:
            param_slot[(a, argn)].add(slot)
    for _ in range(4):  # wrappers that forward their own argument
        changed = False
        for a, (f, rec, _) in out_funcs.items():
            for c in rec.calls:
                if c[0] is None:
                    continue
                for i in range(4):
                    v = c[3].get(f'r{i}')
                    if isinstance(v, list) and v[0] == 'arg' and v[2] == 0:
                        new = param_slot.get((c[0], i), set()) - param_slot[(a, v[1])]
                        if new:
                            param_slot[(a, v[1])] |= new
                            changed = True
        if not changed:
            break
    for a, (f, rec, _) in out_funcs.items():
        for c in rec.calls:
            if c[0] is None:
                continue
            for i in range(4):
                v = c[3].get(f'r{i}')
                if isinstance(v, int) and v in fn_by_thumb:
                    fns = {fn_by_thumb[v]}
                elif isinstance(v, list) and v[0] == 'ldt' and region(v[1]) == 'ROM':
                    fns = set(table_members(v[1], v[2]))
                else:
                    continue
                for slot in param_slot.get((c[0], i), ()):
                    slot_fns[slot] |= fns

    # ---- parameter counts: arguments read directly, or passed unchanged to a callee that reads them
    used = {a: set(rec.used_args) for a, (f, rec, _) in out_funcs.items()}
    for _ in range(10):
        changed = False
        for a, (f, rec, _) in out_funcs.items():
            for callee, j, i in rec.passes:
                if j in used.get(callee, ()) and i not in used[a]:
                    used[a].add(i)
                    changed = True
        if not changed:
            break

    returns = set()
    for a, (f, rec, _) in out_funcs.items():
        returns |= rec.ret_used
        for t, nxt, j in rec.ret_pass:
            if j in used.get(nxt, ()):
                returns.add(t)

    # ---- serialise
    J = {}
    for a, (f, rec, ninsn) in out_funcs.items():
        calls = []
        for c in rec.calls:
            t, kind, site, cargs = c[0], c[1], c[2], c[3]
            if kind == 'bl' or kind == 'tail' or kind == 'ptr':
                tgt = t if t in funcs else None
                if tgt is None and t is not None:
                    # bl into the middle of a function: point at the containing one
                    tgt = t
                calls.append([tgt, kind, site, cargs])
            elif kind in ('slot', 'slotidx'):
                fns = sorted(slot_fns.get(c[4], set())) if kind == 'slot' else table_members(c[4], None)
                calls.append([None, kind, site, cargs, c[4], fns])
            elif kind == 'table':
                fns = table_members(c[4], c[5])
                calls.append([None, kind, site, cargs, c[4], c[5], fns])
            elif kind == 'argfn':
                calls.append([None, kind, site, cargs, c[4]])
            else:
                calls.append([None, kind, site, cargs])
        mem = []
        for (addr, size, rw, idx), cnt in sorted(rec.mems.items()):
            offs = rec.memoff.get((addr, size, rw, idx))
            offl = sorted(o for o in offs if o is not None) if offs else []
            unk = bool(offs and None in offs)
            mem.append([addr, size, rw, idx, cnt, offl, unk])
        params = [[n, off, size, rw, idx, cnt] for (n, off, size, rw, idx), cnt in
                  sorted(rec.params.items(), key=lambda kv: (kv[0][0], kv[0][1] if kv[0][1] is not None else -1,
                                                              kv[0][2], kv[0][3]))]
        deref = [[k, base, eoff, off, size, rw, cnt] for (k, base, eoff, off, size, rw, idx), cnt in
                 sorted(rec.deref.items(), key=lambda kv: (kv[0][1], str(kv[0][2]), str(kv[0][3]), kv[0][4]))]
        # ROM data references and their text
        rom_refs = collections.Counter()
        for v, cnt in list(rec.lits.items()) + list(rec.escapes.items()):
            if region(v) == 'ROM':
                rom_refs[v] += cnt
        for addr, size, rw, idx, cnt, offl, unk in mem:
            if region(addr) == 'ROM':
                rom_refs[addr] += cnt
        for c in rec.calls:
            for v in c[3].values():
                if isinstance(v, int) and region(v) == 'ROM':
                    rom_refs[v] += 0
        strings = []
        roms = []
        seen_str = set()
        for v in sorted(rom_refs):
            lo, lname = assetmap.label(v)
            desc = assetmap.describe(v)
            entry = dict(addr=v)
            if lo is not None:
                entry['label'] = lname
                entry['loff'] = v - lo
            entry.update({k: desc[k] for k in ('asset', 'index', 'what') if k in desc})
            roms.append(entry)
            text = desc.get('text')
            textual = desc.get('asset', '').startswith(TEXT_ASSET_PREFIXES) and not desc.get('record')
            if text is None and textual:
                text = decode_cstring(rom, v)
                if text is None and lo is not None and v == lo:
                    recs = string_records(rom, lo, obj.get(lo, lo + 4))
                    for w, s2 in recs:
                        if (w, s2) not in seen_str:
                            seen_str.add((w, s2))
                            strings.append([w, s2, f'records in {lname}'])
                if text is None and lo is not None:
                    # a table of pointers to strings
                    hi = obj.get(lo, lo + 4)
                    p, got = v, []
                    while p + 4 <= min(hi, v + 0x200) and len(got) < 48:
                        w = ctx.word(p)
                        if region(w or 0) == 'ROM' and assetmap.describe(w).get('asset', '').startswith(
                                TEXT_ASSET_PREFIXES):
                            s = decode_cstring(rom, w)
                            if s is not None and (w, s) not in seen_str:
                                got.append([w, s])
                        p += 4
                    if len(got) >= 2:
                        for w, s in got:
                            seen_str.add((w, s))
                            strings.append([w, s, f'table {hx(v)}'])
                        text = None
            if text:
                if (v, text) not in seen_str:
                    seen_str.add((v, text))
                    via = desc['asset'] + (f'[{desc["index"]}]' if 'index' in desc else '') \
                        if desc.get('record') else 'ptr'
                    strings.append([v, text, via])
        taken = set(v for v in rec.escapes)
        for c in rec.calls:
            for k, v in c[3].items():
                if isinstance(v, int) and is_ptr(v) and region(v) != 'CODE':
                    taken.add((v, c[0], int(k[1:])))
        covered = set()
        for m in mem:
            covered.add(m[0])
            covered.update(m[0] + o for o in m[5])
        covered.update(x[1] for x in deref)
        used_addrs = sorted(covered | set(rec.escapes))

        def accessed_inside(v):   # some access falls inside the object that starts at v (a base pointer)
            hi = v + max(obj.get(v, v + 4) - v, decls[v]['size'] if v in decls else 0, 4)
            i = bisect.bisect_left(used_addrs, v)
            return i < len(used_addrs) and used_addrs[i] < hi
        other = sorted(v for v in rec.lits if region(v) in ('EWRAM', 'IWRAM', 'IO', 'SRAM') and v not in covered
                       and v not in rec.escapes and not accessed_inside(v))
        J[hx(a)] = dict(
            name=f['name'], addr=a, size=f['size'], mode=f['mode'], unit=f['unit'], src=f.get('src', 'c'),
            ninsn=ninsn, nparams=(max(used[a]) + 1) if used[a] else 0, ret=a in returns, calls=calls, mem=mem, params=params, deref=deref, rom=roms, strings=strings,
            fnrefs=sorted(rec.fnrefs), swi=sorted(rec.swis), slots=sorted([list(x) for x in rec.slot_stores]),
            taken=taken, lits=other,
            param_slots=sorted([list(x) for x in rec.param_slots]))

    # ---- data addresses each function passes on or stores ("address taken"), call args filtered by arity
    for key, fj in J.items():
        keep = set()
        for x in fj['taken']:
            if isinstance(x, tuple):
                v, callee, i = x
                n = J[hx(callee)]['nparams'] if callee is not None and hx(callee) in J else 4
                if i < n:
                    keep.add(v)
            else:
                keep.add(x)
        fj['taken'] = sorted(keep)
        fj['lits'] = [v for v in fj['lits'] if v not in keep]

    # ---- function pointers stored in ROM data tables, and the functions that use those tables
    label_users = collections.defaultdict(set)
    for key, fj in J.items():
        for r in fj['rom']:
            if 'label' in r:
                label_users[r['addr'] - r['loff']].add(fj['addr'])
    data_fnrefs = collections.defaultdict(list)
    for p in assetmap.rows:
        if not (p['path'].startswith(DATA_TABLE_PREFIXES) or p['path'] == 'gfx/scene_sets'):
            continue
        a = (p['start'] + 3) & ~3
        while a + 4 <= p['end']:
            w = ctx.word(a)
            if w in fn_by_thumb:
                data_fnrefs[fn_by_thumb[w]].append(a)
            a += 4
    for fn, addrs in data_fnrefs.items():
        key = hx(fn)
        if key not in J:
            continue
        rows = []
        for d in addrs:
            lo, lname = assetmap.label(d)
            desc = assetmap.describe(d)
            users = sorted(label_users.get(lo, ())) if lo is not None else []
            rows.append([d, lname, d - lo if lo is not None else None, desc.get('what') or desc.get('asset'), users])
        J[key]['datarefs'] = rows

    # ---- call arguments that are dialogue event IDs (StartDialogue-like parameters)
    dlg = {}
    pd = assetmap.by_path('text/dialogue')
    if pd:
        for i, o in enumerate(range(pd['start'], pd['end'] - 0x303, 0x304)):
            ev, sp = struct.unpack_from('<HH', rom, o - BASE)
            t = text_helpers()[1](rom[o - BASE + 4:o - BASE + 0x304])
            dlg.setdefault(ev, [i, sp, (t['text'] if isinstance(t, dict) else t)[:240], o + 4])
    sites = collections.defaultdict(list)     # (callee, arg) -> [(caller, value)]
    for key, fj in J.items():
        for c in fj['calls']:
            if c[0] is not None and c[0] in out_funcs:
                for k, v in c[3].items():
                    if isinstance(v, int):
                        sites[(c[0], int(k[1:]))].append((fj['addr'], v))
    decoders = {}
    for (callee, i), vals in sites.items():
        fj = J.get(hx(callee))
        if fj is None or i >= fj['nparams']:
            continue
        distinct = {v for _, v in vals}
        if len(distinct) >= 2 and sum(1 for v in distinct if v in dlg) >= 0.8 * len(distinct) and \
                min(distinct) >= 100:
            decoders[(callee, i)] = 'dialogue'
    for _ in range(3):   # wrappers that pass their own argument on
        for a, (f, rec, _) in out_funcs.items():
            for callee, j, i in rec.passes:
                if (callee, j) in decoders and (a, i) not in decoders:
                    decoders[(a, i)] = decoders[(callee, j)]
    for (callee, i), kind in decoders.items():
        for caller, v in sites.get((callee, i), ()):
            if v in dlg:
                idx, sp, text, addr = dlg[v]
                J[hx(caller)]['strings'].append([addr, text, f'dialogue event {v} (record {idx})'])
    for key, fj in J.items():
        seen, uniq = set(), []
        for x in fj['strings']:
            if (x[0], x[2]) not in seen:
                seen.add((x[0], x[2]))
                uniq.append(x)
        fj['strings'] = uniq

    # ---- callers (inverse edges)
    callers = collections.defaultdict(list)
    for key, fj in J.items():
        a = fj['addr']
        for c in fj['calls']:
            targets = [c[0]] if c[0] is not None else (c[-1] if c[1] in ('slot', 'slotidx', 'table') else [])
            for t in targets:
                if t is None:
                    continue
                callers[t].append([a, c[1], c[2]])
        for t in fj['fnrefs']:
            callers[t].append([a, 'ref', None])
    for key, fj in J.items():
        for d, lname, loff, what, users in fj.get('datarefs', []):
            for u in users:
                callers[fj['addr']].append([u, 'data', d])
    for key, fj in J.items():
        cl = callers.get(fj['addr'], [])
        real = {c for c, k, _ in cl if k not in ('ref', 'data')}
        cl = [x for x in cl if x[1] not in ('ref', 'data') or x[0] not in real]
        seen, uniq = set(), []
        for x in cl:
            k = (x[0], x[1]) if x[1] == 'data' else tuple(x)
            if k not in seen:
                seen.add(k)
                uniq.append(x)
        cl = uniq
        fj['callers'] = sorted(cl, key=lambda x: (x[0], x[2] or 0))

    # ---- wiki and names
    known = {}
    for a, f in funcs.items():
        if not re.match(r'^(sub|gUnk)_', f['name']) and len(f['name']) > 3 and not f['name'].startswith('_'):
            known[f['name']] = a
    for a, n in data_syms.items():
        if not re.match(r'^(sub|gUnk)_', n) and len(n) > 3 and not n.startswith('_'):
            known.setdefault(n, a)
    wiki, titles = wiki_index(known)

    db = dict(
        version=VERSION, built=time.strftime('%Y-%m-%d %H:%M:%S'), elf=os.path.relpath(elf, REPO),
        inputs=_input_stamps(),
        functions=J,
        data_syms={hx(a): n for a, n in sorted(data_syms.items())},
        aliases={hx(a): sorted(ns - {data_syms.get(a)}) for a, ns in sorted(cand_names.items())
                 if len(ns) > 1},
        objects={hx(a): hi - a for a, hi in sorted(obj.items())},
        decls={hx(a): d for a, d in sorted(decls.items())},
        io={f'{k:03X}': v for k, v in sorted(io_names.items())},
        proposed={hx(a): v for a, v in sorted(proposed.items())},
        wiki={hx(a): p for a, p in sorted(wiki.items())},
        wiki_titles=titles,
        slot_fns={hx(a): sorted(v) for a, v in sorted(slot_fns.items())},
        arg_decoders={f'{hx(c)}:{i}': k for (c, i), k in sorted(decoders.items())},
        assets=[[p['start'], p['end'], p['type'], p['path']] for p in assetmap.rows],
        dialogue={str(ev): v for ev, v in sorted(dlg.items())},
        struct_failures=failures,
    )
    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    tmp = args.out + f'.tmp{os.getpid()}'
    with open(tmp, 'w') as fh:
        json.dump(db, fh, separators=(',', ':'), ensure_ascii=False)
    os.replace(tmp, args.out)
    n_calls = sum(len(f['calls']) for f in J.values())
    n_mem = sum(len(f['mem']) for f in J.values())
    print(f'xref: {len(J)} functions, {n_calls} call sites, {n_mem} global accesses, {len(decls)} declared '
          f'globals ({len(failures)} units unparsed) -> {os.path.relpath(args.out, REPO)} '
          f'in {time.time() - t0:.1f}s')


def _input_stamps():
    paths = [ELF, 'config/functions.tsv', 'config/names.txt', 'symbols.ld', 'config/symbols.txt',
             'config/assets.tsv']
    out = {}
    for p in paths:
        q = p if os.path.isabs(p) else os.path.join(REPO, p)
        if os.path.exists(q):
            out[os.path.relpath(q, REPO)] = os.path.getmtime(q)
    srcs = glob.glob(os.path.join(REPO, 'src', '**', '*.c'), recursive=True) + \
        glob.glob(os.path.join(REPO, 'include', '*.h'))
    out['src+include (newest)'] = max((os.path.getmtime(p) for p in srcs), default=0)
    wk = glob.glob(os.path.join(REPO, 'wiki', '**', '*.md'), recursive=True)
    out['wiki (newest)'] = max((os.path.getmtime(p) for p in wk), default=0)
    return out


# ========================================================================================== queries
class DB:
    def __init__(self, path):
        if not os.path.exists(path):
            sys.exit(f'{os.path.relpath(path, REPO)} not found: run `tools/dr python3 tools/xref.py build` first')
        self.j = json.load(open(path))
        self.path = path
        self.f = self.j['functions']
        self.by_addr = {v['addr']: v for v in self.f.values()}
        self.addrs = sorted(self.by_addr)
        # current names (functions.tsv may have been renamed since the build)
        try:
            for a, r in read_functions_tsv().items():
                if a in self.by_addr:
                    self.by_addr[a]['name'] = r['name']
        except OSError:
            pass
        self.name_to_addr = {v['name']: v['addr'] for v in self.f.values()}
        self.data_syms = {int(k, 16): v for k, v in self.j['data_syms'].items()}
        try:
            for n, a in read_symbol_files().items():
                if region(a) not in (None, 'CODE'):
                    self.data_syms[a] = n
        except OSError:
            pass
        self.data_name_to_addr = {v: k for k, v in self.data_syms.items()}
        for k, ns in self.j.get('aliases', {}).items():
            for n in ns:
                self.data_name_to_addr.setdefault(n, int(k, 16))
        self.objects = {int(k, 16): v for k, v in self.j['objects'].items()}
        self.obj_lo = sorted(self.objects)
        self._obj_cache = {}
        self.decls = {int(k, 16): v for k, v in self.j['decls'].items()}
        self.io = {int(k, 16): v for k, v in self.j['io'].items()}
        self.proposed = {int(k, 16): v for k, v in self.j['proposed'].items()}
        self.wiki = {int(k, 16): (v if isinstance(v, dict) else {p: 1 for p in v})
                     for k, v in self.j['wiki'].items()}
        self.titles = self.j['wiki_titles']
        self.decoders = {}
        for k, v in self.j.get('arg_decoders', {}).items():
            fa, i = k.split(':')
            self.decoders[(int(fa, 16), int(i))] = v
        self.dialogue = self.j.get('dialogue', {})

    def stale_warning(self):
        old = []
        for p, t in self.j.get('inputs', {}).items():
            if p.endswith('(newest)'):
                continue
            q = os.path.join(REPO, p)
            if os.path.exists(q) and os.path.getmtime(q) > t + 1:
                old.append(p)
        if old:
            print(f'(note: {", ".join(old)} changed since the xref build; names come from functions.tsv, but '
                  f'rebuild with `tools/dr python3 tools/xref.py build` for fresh code facts)', file=sys.stderr)

    # ---- naming
    def fname(self, a, proposed=True):
        if a is None:
            return '?'
        f = self.by_addr.get(a)
        if f:
            n = f['name']
            if proposed and a in self.proposed and self.proposed[a][0] != n:
                n += f' ({self.proposed[a][0]}?)'
            return n
        cf = self.containing_func(a)
        if cf:
            return f'{cf["name"]}+0x{a - cf["addr"]:X}'
        return hx(a)

    def containing_func(self, a):
        i = bisect.bisect_right(self.addrs, a) - 1
        if i >= 0:
            f = self.by_addr[self.addrs[i]]
            if f['addr'] <= a < f['addr'] + max(f['size'], 1):
                return f
        return None

    def resolve_func(self, s):
        if s in self.name_to_addr:
            return self.by_addr[self.name_to_addr[s]]
        for a, (n, _) in self.proposed.items():
            if n == s and a in self.by_addr:
                return self.by_addr[a]
        a = parse_addr(s)
        if a is not None:
            return self.containing_func(a & ~1)
        low = s.lower()
        cands = [f for f in self.by_addr.values() if low in f['name'].lower() or
                 (f['addr'] in self.proposed and low in self.proposed[f['addr']][0].lower())]
        if len(cands) == 1:
            return cands[0]
        if cands:
            sys.exit(f'ambiguous, {len(cands)} matches: ' + ', '.join(
                self.fname(c['addr']) for c in sorted(cands, key=lambda c: c['addr'])[:30]))
        return None

    def object_of(self, a):
        """(base, size) of the object containing a. Among declared C globals that cover a, the one giving a
        real field name wins (named > placeholder like unk/filler > none), then the innermost (closest start);
        without a declaration, the nearest symbol below whose extent covers a."""
        if a in self._obj_cache:
            return self._obj_cache[a]
        cands = []
        i = bisect.bisect_right(self.obj_lo, a) - 1
        while i >= 0 and a - self.obj_lo[i] <= 0x10000:
            lo = self.obj_lo[i]
            if lo <= a < lo + self.objects[lo]:
                cands.append(lo)
            i -= 1
        decl = [c for c in cands if c in self.decls and self.decls[c]['size'] and a < c + self.decls[c]['size']]
        if decl:
            def quality(c):
                fld = self.field(c, a - c)
                if not fld:
                    q = 0
                else:
                    last = re.split(r'[.\[{]', fld.strip('.'))[-1] if fld.strip('.[]0123456789') else ''
                    q = 1 if (not last or _unk_name(fld.split('.')[-1].strip('{}')) or 'filler' in fld
                              or 'unk' in fld.split('.')[-1].lower() or 'pad' in fld.split('.')[-1].lower()) else 2
                return (q, c)
            b = max(decl, key=quality)
            res = (b, self.decls[b]['size'])
        elif cands:
            b = max(cands)
            res = (b, self.objects[b])
        else:
            res = (None, None)
        self._obj_cache[a] = res
        return res

    def sym_name(self, a):
        n = self.data_syms.get(a)
        if n is None:
            return None
        if a in self.proposed and self.proposed[a][0] != n:
            n += f' ({self.proposed[a][0]}?)'
        return n

    def field(self, base, off, size=None):
        """Field path for byte offset off inside the declared global at base."""
        d = self.decls.get(base)
        if d is None or off is None:
            return None
        elem = d['elem'] or 0
        prefix = ''
        if elem and d.get('count') is not None:
            k, off = divmod(off, elem)
            prefix = f'[{k}]'
        name = self._leaf(d['leaves'], off, size)
        if not name:
            return prefix if prefix and prefix != '[0]' else None
        return f'{prefix}.{name}' if prefix else name

    def _leaf(self, leaves, off, size, depth=0):
        """Best field name for an access of `size` bytes at byte offset `off`. Ranking: a bitfield storage
        unit or a field that starts there with the same size, then one that starts there, then the innermost
        containing field; ties go to the name more units use, then to the smaller field."""
        best, bkey = None, None
        for leaf in leaves:
            lo, sz, bit, width, elem, path, ty, votes, alts = leaf
            if not (lo <= off < lo + max(sz, 1)):
                continue
            fits = size is None or off + size <= lo + sz
            if width is not None:
                if off != lo or not fits:
                    continue
                cls = 3
            elif off == lo and (size is None or size == sz):
                cls = 2
            elif off == lo and fits:
                cls = 1
            elif fits:
                cls = 0
            else:
                cls = -1
            key = (cls, votes, -sz)
            if bkey is None or key > bkey:
                best, bkey = leaf, key
        if best is None:
            return None
        lo, sz, bit, width, elem, path, ty, votes, alts = best
        if width is not None:
            names = []
            for l2, s2, b2, w2, e2, p2, t2, v2, al2 in leaves:
                if l2 == lo and w2 is not None and s2 == sz and p2.split('.')[-1] not in names:
                    names.append(p2.split('.')[-1])
            prefix = path.rsplit('.', 1)[0] + '.' if '.' in path else ''
            if len(names) == 1:
                return path
            return prefix + '{' + ','.join(names[:4]) + (',…' if len(names) > 4 else '') + '}'
        if off == lo and (bkey[0] >= 1 or not elem):
            return path
        rel = off - lo
        if elem and depth < 4:
            k, r = divmod(rel, elem)
            sub = f'{path}[0]'
            inner = [x for x in leaves if x[5].startswith(sub + '.') or x[5].startswith(sub + '[')]
            if inner:
                n = self._leaf(inner, lo + r, size, depth + 1)
                if n:
                    return n.replace(sub, f'{path}[{k}]', 1)
            return f'{path}[{k}]' + (f'+0x{r:X}' if r else '')
        return f'{path}+0x{rel:X}'

    def describe_addr(self, a, size=None, short=False, field=True):
        """Human name for a data address: symbol(+off) field, IO register, region."""
        r = region(a)
        if r == 'IO':
            n = self.io.get(a - 0x04000000)
            if n:
                return n
            lo = max((k for k in self.io if k <= a - 0x04000000), default=None)
            return f'{self.io[lo]}+0x{a - 0x04000000 - lo:X}' if lo is not None else f'IO+0x{a - 0x04000000:X}'
        if r in ('VRAM', 'PLTT', 'OAM', 'SRAM', 'BIOS'):
            base = {'VRAM': 0x06000000, 'PLTT': 0x05000000, 'OAM': 0x07000000, 'SRAM': 0x0E000000, 'BIOS': 0}[r]
            return f'{r}+0x{a - base:X}'
        base, sz = self.object_of(a)
        if base is None:
            return hx(a)
        name = self.sym_name(base) or hx(base)
        off = a - base
        s = name if off == 0 else f'{name}+0x{off:X}'
        fld = self.field(base, off, size) if field else None
        if fld:
            s += f' {fld}'
        if a != base and a in self.data_syms and not short:
            s += f' (= {self.data_syms[a]})'
        return s

    # ---- global index (built on demand)
    def accesses(self):
        if hasattr(self, '_acc'):
            return self._acc
        acc = []   # (addr, size, rw, idx, func addr, count, offs, unk)
        for f in self.by_addr.values():
            for addr, size, rw, idx, cnt, offl, unk in f['mem']:
                if idx and offl:
                    for o in offl:
                        acc.append((addr + o, size, rw, True, f['addr'], cnt, addr))
                    if unk:
                        acc.append((addr, size, rw, True, f['addr'], cnt, None))
                else:
                    acc.append((addr, size, rw, idx, f['addr'], cnt, addr if idx else None))
            for k, base, eoff, off, size, rw, cnt in f['deref']:
                acc.append((base, 4, 'r', k == 'ldt', f['addr'], cnt, 'deref'))
        self._acc = acc
        return acc


def fmt_args(db, cargs, limit=None, strs=None, callee=None):
    """Call-site register values. limit = the callee's parameter count (None = unknown: show r0-r3)."""
    out = []
    for k in sorted(cargs):
        if limit is not None and int(k[1:]) >= limit:
            continue
        v = cargs[k]
        dec = db.decoders.get((callee, int(k[1:]))) if callee is not None else None
        if isinstance(v, int) and dec == 'dialogue' and str(v) in db.dialogue:
            out.append(f'{k}={v:#x} {_q(db.dialogue[str(v)][2], 40)}')
        elif isinstance(v, int):
            if region(v) == 'CODE' and (v & ~1) in db.by_addr:
                out.append(f'{k}={db.fname(v & ~1, False)}')
            elif strs and v in strs:
                out.append(f'{k}={_q(strs[v], 30)}')
            elif region(v) not in (None, 'CODE') and v >= 0x02000000:
                out.append(f'{k}=&{db.describe_addr(v, short=True)}')
            else:
                out.append(f'{k}={v:#x}' if v > 9 else f'{k}={v}')
        elif isinstance(v, list):
            if v[0] == 'arg':
                out.append(f'{k}=arg{v[1]}' + (f'+0x{v[2]:X}' if v[2] else ''))
            elif v[0] == 'ld':
                out.append(f'{k}=*{db.describe_addr(v[1], 4, short=True)}' + (f'+0x{v[2]:X}' if v[2] else ''))
            elif v[0] == 'g':
                out.append(f'{k}=&{db.describe_addr(v[1], short=True, field=False)}[i]' +
                           (f'+0x{v[2]:X}' if v[2] else ''))
            elif v[0] == 'ldt':
                out.append(f'{k}={db.describe_addr(v[1], short=True, field=False)}[i]' +
                           (f'.+0x{v[2]:X}' if v[2] else ''))
    return ' '.join(out)


def rwstr(rws):
    return ''.join(c for c in 'rw' if c in rws)


def wiki_links(db, a, unit=None):
    """-> (pages about a, best first; its unit page or None; other unit pages). Pages whose title names the
    function (or its proposed name) come first, then by directory (functions, game, data, ...), then by how
    often the page mentions the address."""
    pages = db.wiki.get(a, {})
    unitpage = unit.lower().replace('_', '-').split('/')[-1] if unit else None
    main = [p for p in pages if p != unitpage and not p.startswith('code-')]
    rank = {'functions': 0, 'game': 1, 'data': 2, 'rom': 3, 'concepts': 4, 'tools': 5, 'questions': 6,
            'sources': 7}
    names = {hx(a)[2:].lower()}
    f = db.by_addr.get(a)
    if f:
        names.add(f['name'].lower())
    if a in db.proposed:
        names.add(db.proposed[a][0].lower())
    if a in db.data_syms:
        names.add(db.data_syms[a].lower())

    def key(p):
        rel, title = db.titles.get(p, [p, p])
        in_title = any(n in title.lower() for n in names)
        return (0 if in_title else 1, rank.get(rel.split('/')[0], 9), -pages[p], p)
    main.sort(key=key)
    others = [p for p in pages if p.startswith('code-') and p != unitpage]
    return main, (unitpage if unitpage in db.titles else None), others


def func_card(db, f, args):
    a = f['addr']
    out = []
    flag = {'c': 'C', 'asm': 'asm', 'lib': 'library'}.get(f['src'], f['src'])
    if f['src'] == 'asm' and f['unit'].startswith('code_'):
        flag = 'INCLUDE_ASM (not matched)'
    out.append(f"{f['name']}  {hx(a)}-{hx(a + f['size'])} (0x{f['size']:X} bytes, "
               f"{'Thumb' if f['mode'] == 't' else 'ARM'}, {f['ninsn']} insns)  unit {f['unit']}  [{flag}]")
    np = f.get('nparams', 0)
    regs_s = 'r0' if np == 1 else f'r0-r{min(np, 4) - 1}'
    out.append(f'  parameters: {np}' + (f' ({regs_s}' + (f' + {np - 4} on the stack' if np > 4 else '') + ')'
                                       if np else ' (or unused)') + f'; returns {_ret(f)}')
    if a in db.proposed:
        n, why = db.proposed[a]
        if n != f['name']:
            out.append(f'  proposed name: {n}  # {why}' if why else f'  proposed name: {n}')
    main, unitpage, _ = wiki_links(db, a, f['unit'])
    if main or unitpage:
        w = ', '.join(f'[[{p}]]' for p in main[:6])
        if unitpage:
            w += (', ' if w else '') + f'unit page [[{unitpage}]]'
        out.append(f'  wiki: {w}')
    # callers
    cl = collections.OrderedDict()
    for c, kind, site in f['callers']:
        cl.setdefault((c, kind), []).append(site)
    direct = [(c, k, s) for (c, k), s in cl.items() if k not in ('ref', 'data')]
    refs = [(c, k, s) for (c, k), s in cl.items() if k == 'ref']
    nsites = sum(len(s) for _, _, s in direct)
    out.append(f'callers: {len({c for c, _, _ in direct})} functions, {nsites} sites' + (':' if direct else ''))
    lim = None if args.all else 25
    for c, k, s in direct[:lim]:
        tag = '' if k == 'bl' else f' [{k}]'
        sites = ', '.join(hx(x)[4:] if x else '?' for x in s[:4]) + (' …' if len(s) > 4 else '')
        out.append(f'  {db.fname(c)}{tag}  x{len(s)} at {sites}')
    if lim and len(direct) > lim:
        out.append(f'  … {len(direct) - lim} more (--all)')
    if refs:
        out.append(f'address taken by: ' + ', '.join(db.fname(c) for c, _, _ in refs[:lim]) +
                   (f' … {len(refs) - lim} more' if lim and len(refs) > lim else ''))
    if f.get('datarefs'):
        out.append('stored in ROM data tables:')
        for d, lname, loff, what, users in f['datarefs'][:None if args.all else 12]:
            line = f'  {hx(d)} {lname or ""}' + (f'+0x{loff:X}' if loff else '') + (f'  {what}' if what else '')
            if users:
                line += '  (table used by ' + ', '.join(db.fname(u) for u in users[:6]) + \
                    (' …' if len(users) > 6 else '') + ')'
            out.append(line)
        if not args.all and len(f['datarefs']) > 12:
            out.append(f"  … {len(f['datarefs']) - 12} more (--all)")
    # callees
    calls = f['calls']
    strs = {x[0]: x[1] for x in f['strings']}
    out.append(f'callees: {len(calls)} sites' + (':' if calls else ''))
    agg = collections.OrderedDict()
    for c in calls:
        t, kind = c[0], c[1]
        if kind in ('bl', 'tail', 'ptr'):
            key = (t, kind)
            agg.setdefault(key, []).append(c)
        else:
            agg.setdefault((c[2], kind), []).append(c)
    shown = 0
    for (t, kind), cs in agg.items():
        if lim and shown >= 40 and not args.all:
            out.append(f'  … {len(agg) - shown} more (--all)')
            break
        shown += 1
        c = cs[0]
        if kind in ('bl', 'tail', 'ptr'):
            argset = []
            lim_n = db.by_addr[t]['nparams'] if t in db.by_addr else None
            for x in cs:
                s = fmt_args(db, x[3], lim_n, strs, t)
                if s and s not in argset:
                    argset.append(s)
            tag = '' if kind == 'bl' else f' [{kind}]'
            line = f'  {db.fname(t)}{tag}' + (f' x{len(cs)}' if len(cs) > 1 else '')
            if argset:
                line += '  (' + '; '.join(argset[:4]) + (' …' if len(argset) > 4 else '') + ')'
            out.append(line)
        elif kind == 'slot':
            fns = c[5]
            out.append(f'  indirect via *{db.describe_addr(c[4], 4)} at {hx(c[2])}: ' +
                       (f'{len(fns)} candidates: ' + ', '.join(db.fname(x, False) for x in fns[:8]) +
                        (' …' if len(fns) > 8 else '') if fns else 'no stores found'))
        elif kind == 'slotidx':
            fns = c[5]
            out.append(f'  indirect via pointer *{db.describe_addr(c[4], 4)} at {hx(c[2])}' +
                       (f': {len(fns)} candidates' if fns else ''))
        elif kind == 'table':
            fns = c[6]
            eo = f'+0x{c[5]:X}' if c[5] else ''
            out.append(f'  indirect via table {db.describe_addr(c[4], field=False)}{eo}[i] at {hx(c[2])}: ' +
                       (f'{len(fns)} candidates: ' + ', '.join(db.fname(x, False) for x in fns[:8]) +
                        (' …' if len(fns) > 8 else '') if fns else 'no functions in table'))
        elif kind == 'argfn':
            out.append(f'  indirect via callback argument arg{c[4]} at {hx(c[2])}')
        else:
            out.append(f'  indirect (unresolved) at {hx(c[2])}')
    if f['swi']:
        out.append('  SWI: ' + ', '.join(f'{n:#x} {SWI_NAMES.get(n, "?")}' for n in f['swi']))
    # globals
    g = collections.OrderedDict()
    for addr, size, rw, idx, cnt, offl, unk in f['mem']:
        if region(addr) == 'ROM':
            continue
        keys = []
        if idx:
            keys += [(addr + o, True) for o in (offl or [])]
            if unk or not offl:
                keys.append((addr, 'any'))
        else:
            keys.append((addr, False))
        for k in keys:
            e = g.setdefault(k, [set(), set()])
            e[0].add(rw)
            e[1].add(size)
    if g:
        out.append(f'globals ({len(g)}):')
        rows = []
        for (addr, idx), (rws, sizes) in sorted(g.items(), key=lambda kv: (kv[0][0], str(kv[0][1]))):
            what = db.describe_addr(addr, max(sizes) if idx != 'any' else None, field=idx != 'any')
            if idx == 'any':
                what += ' [i] (offset varies)'
            elif idx:
                what += ' [i]'
            szs = '/'.join(_sz(z).strip() for z in sorted(sizes))
            rows.append(f'  {rwstr(rws):2s} {szs:7s} {hx(addr)} {what}')
        out += rows if (args.all or len(rows) <= 40) else rows[:40] + [f'  … {len(rows) - 40} more (--all)']
    if f.get('lits'):
        out.append('also loads these addresses (use not tracked, e.g. after a merge of two pointers): ' +
                   ', '.join(f'&{db.describe_addr(v, short=True)}' for v in f['lits'][:12]) +
                   (f' … +{len(f["lits"]) - 12}' if len(f['lits']) > 12 else ''))
    taken = [v for v in f.get('taken', []) if region(v) not in ('ROM', 'CODE')]
    if taken:
        out.append('takes the address of (passes it on or stores it): ' +
                   ', '.join(f'&{db.describe_addr(v, short=True)}' for v in taken[:12]) +
                   (f' … +{len(taken) - 12}' if len(taken) > 12 else ''))
    if f['deref']:
        out.append('through pointers loaded from globals/tables:')
        rows = collections.OrderedDict()
        for k, base, eoff, off, size, rw, cnt in f['deref']:
            if k == 'ld':
                src = f'(*{db.describe_addr(base, 4, short=True)})'
            else:
                src = f'{db.describe_addr(base, short=True, field=False)}[i]' + \
                    (f'.+0x{eoff:X}' if eoff else ('' if eoff == 0 else '.+?'))
            rows.setdefault((src, off, size), set()).add(rw)
        lines = [f'  {rwstr(rw):2s} {_sz(size)} {src}->' + (f'+0x{off:X}' if off else ('+0' if off == 0 else '+?'))
                 for (src, off, size), rw in rows.items()]
        out += lines if (args.all or len(lines) <= 25) else lines[:25] + [f'  … {len(lines) - 25} more (--all)']
    if f['params']:
        out.append('parameter fields (arg->+off):')
        rows = collections.OrderedDict()
        for n, off, size, rw, idx, cnt in f['params']:
            rows.setdefault((n, off, size, idx), set()).add(rw)
        lines = [f'  {rwstr(rw):2s} {_sz(size)} arg{n}' + (f'[i]' if idx else '') +
                 (f'+0x{off:X}' if off else ('' if off == 0 else '+?'))
                 for (n, off, size, idx), rw in rows.items()]
        out += lines if (args.all or len(lines) <= 25) else lines[:25] + [f'  … {len(lines) - 25} more (--all)']
    if f['rom']:
        out.append('ROM data:')
        for r in f['rom'][:None if args.all else 25]:
            s = f"  {hx(r['addr'])} {r.get('label', '')}" + (f"+0x{r['loff']:X}" if r.get('loff') else '')
            if r['addr'] in db.proposed:
                s += f' ({db.proposed[r["addr"]][0]}?)'
            if 'asset' in r:
                s += f"  {r['asset']}" + (f" [{r['index']}]" if 'index' in r and not r.get('what') else '')
            if r.get('what'):
                s += f"  {r['what']}"
            out.append(s)
        if not args.all and len(f['rom']) > 25:
            out.append(f"  … {len(f['rom']) - 25} more (--all)")
    if f['strings']:
        out.append('strings:')
        for s in f['strings'][:None if args.all else 20]:
            out.append(f'  {hx(s[0])} {_q(s[1])}' + (f'  ({s[2]})' if s[2] != 'ptr' else ''))
        if not args.all and len(f['strings']) > 20:
            out.append(f"  … {len(f['strings']) - 20} more (--all)")
    if f['slots']:
        out.append('stores function pointers: ' + ', '.join(
            f'{db.describe_addr(s, 4, short=True)} <- {db.fname(fn, False)}' for s, fn in f['slots'][:10]))
    if f['param_slots']:
        out.append('stores its argument into: ' + ', '.join(
            f'arg{n} -> {db.describe_addr(s, 4, short=True)}' for n, s in f['param_slots']))
    return '\n'.join(out)


def _ret(f):
    return 'a value in r0 that callers use' if f.get('ret') else 'no caller reads r0 (void?)'


def _sz(n):
    return {1: 'u8 ', 2: 'u16', 4: 'u32'}.get(n, f'{n}B ')


def _q(s, n=90):
    return json.dumps(s[:n] + ('…' if len(s) > n else ''), ensure_ascii=False)


def cmd_func(db, args):
    f = db.resolve_func(args.name)
    if f is None:
        sys.exit(f'no function {args.name!r}')
    if args.json:
        d = dict(f)
        d['proposed'] = db.proposed.get(f['addr'])
        d['wiki'] = db.wiki.get(f['addr'], [])
        print(json.dumps(d, indent=1, ensure_ascii=False))
        return
    print(func_card(db, f, args))


def resolve_global(db, s):
    a = db.data_name_to_addr.get(s)
    if a is None:
        for k, (n, _) in db.proposed.items():
            if n == s:
                a = k
        if a is None:
            a = parse_addr(s)
    if a is None:
        m = re.search(r'([0-9A-Fa-f]{8})$', s)
        a = int(m.group(1), 16) if m else None
    return a


def cmd_global(db, args):
    a = resolve_global(db, args.name)
    if a is None:
        sys.exit(f'no global {args.name!r}')
    base, size = db.object_of(a)
    exact = base is None or a != base or args.exact
    if args.offset is not None:
        lo = (base if base is not None else a) + int(args.offset, 0)
        hi = lo + 1
    elif exact and a != base:
        lo, hi = a, a + 1
    else:
        lo, hi = a, a + (size or 1)
        if a in db.decls and db.decls[a]['size']:
            hi = a + db.decls[a]['size']
    rows = collections.defaultdict(lambda: collections.defaultdict(set))   # off -> func -> {rw}
    sizes = collections.defaultdict(set)
    deref = collections.defaultdict(set)
    anyoff = collections.defaultdict(set)
    for addr, sz, rw, idx, fa, cnt, ob in db.accesses():
        if ob == 'deref':
            if lo <= addr < hi:
                deref[fa].add('r')
            continue
        if ob is None and idx and lo <= addr < hi and args.offset is None:
            anyoff[fa].add(rw)                   # indexed access whose offset is unknown
            continue
        if lo <= addr < hi:
            rows[addr][fa].add(rw + ('[i]' if idx else ''))
            sizes[addr].add(sz)
    callers_ptr = collections.defaultdict(set)
    weak = collections.defaultdict(set)
    for f in db.by_addr.values():
        for v in f.get('taken', []):
            if lo <= v < hi:
                callers_ptr[f['addr']].add(hx(v))
        for v in f.get('lits', []):
            if lo <= v < hi:
                weak[f['addr']].add(hx(v))
    if args.json:
        print(json.dumps(dict(addr=a, base=base, range=[lo, hi], name=db.describe_addr(a),
                              decl=db.decls.get(base), proposed=db.proposed.get(a),
                              accesses={hx(k): {db.fname(fa, False): sorted(v) for fa, v in fs.items()}
                                        for k, fs in sorted(rows.items())},
                              deref={db.fname(fa, False): sorted(v) for fa, v in deref.items()},
                              indexed_unknown_offset={db.fname(fa, False): sorted(v) for fa, v in anyoff.items()},
                              address_taken_by={db.fname(fa, False): sorted(v) for fa, v in callers_ptr.items()},
                              untracked_loads={db.fname(fa, False): sorted(v) for fa, v in weak.items()}),
                         indent=1, ensure_ascii=False))
        return
    head = db.describe_addr(lo, hi - lo if hi - lo in (1, 2, 4) else None, field=lo != base)
    print(f'{head}  {hx(lo)}-{hx(hi)}' + (f'  region {region(a)}' if region(a) else ''))
    if a in db.proposed:
        print(f'  proposed name: {db.proposed[a][0]}  # {db.proposed[a][1]}')
    if base in db.decls:
        d = db.decls[base]
        print(f"  declared in {d['units']} units as {', '.join(d['names'][:3])}: {d['type']}"
              + (f" (element 0x{d['elem']:X} x {d['count'] or '?'})" if d.get('count') not in (None, 1) else
                 f" (0x{d['size']:X} bytes)" if d['size'] else ''))
    main, _, _ = wiki_links(db, a)
    if main:
        print('  wiki: ' + ', '.join(f'[[{p}]]' for p in main[:8]))
    funcs = set(anyoff)
    for off in rows:
        funcs |= set(rows[off])
    print(f'{len(funcs)} functions access it directly, at {len(rows)} distinct addresses:')
    lim = None if args.all else 60
    lines = []
    for addr in sorted(rows):
        fs = rows[addr]
        what = db.describe_addr(addr, max(sizes[addr]) if sizes[addr] else None, short=True)
        szs = '/'.join(_sz(s).strip() for s in sorted(sizes[addr]))
        w = sum(1 for v in fs.values() if any(x.startswith('w') for x in v))
        lines.append(f'  {hx(addr)} {szs:7s} {what}   ({len(fs)} funcs, {w} write)')
        items = sorted(fs.items(), key=lambda kv: kv[0])
        txt = ', '.join(f"{db.fname(fa, False)}:{''.join(sorted(set(x[0] for x in v)))}"
                        + ('[i]' if any('[i]' in x for x in v) else '') for fa, v in items[:12])
        if len(items) > 12:
            txt += f' … +{len(items) - 12}'
        lines.append(f'      {txt}')
    if lim and len(lines) > lim * 2:
        lines = lines[:lim * 2] + [f'  … {len(rows) - lim} more addresses (--all)']
    print('\n'.join(lines))
    if anyoff:
        print(f'{len(anyoff)} functions index into it at offsets the analysis could not pin down: ' +
              ', '.join(f'{db.fname(x, False)}:{"".join(sorted(v))}' for x, v in sorted(anyoff.items())[:30]) +
              (' …' if len(anyoff) > 30 else ''))
    if deref:
        print(f'{len(deref)} functions load a pointer from it and dereference it: ' +
              ', '.join(db.fname(x, False) for x in sorted(deref)[:30]) + (' …' if len(deref) > 30 else ''))
    if weak:
        print(f'{len(weak)} functions load its address but the access was not tracked: ' +
              ', '.join(db.fname(x, False) for x in sorted(weak)[:30]) + (' …' if len(weak) > 30 else ''))
    if callers_ptr:
        print(f'{len(callers_ptr)} functions take its address (pass it to a call or store it): ' +
              ', '.join(db.fname(x, False) for x in sorted(callers_ptr)[:30]) + (' …' if len(callers_ptr) > 30 else ''))


def cmd_strings(db, args):
    rx = re.compile(args.regex, 0 if args.case else re.I)
    hits = []
    for f in db.by_addr.values():
        for s in f['strings']:
            if rx.search(s[1]):
                hits.append((s[0], s[1], f['addr'], s[2]))
    hits.sort()
    if args.json:
        print(json.dumps([dict(addr=a, text=t, func=db.fname(fa, False), via=v) for a, t, fa, v in hits],
                         indent=1, ensure_ascii=False))
        return
    by = collections.OrderedDict()
    for a, t, fa, v in hits:
        by.setdefault((a, t, v), []).append(fa)
    for (a, t, v), fs in by.items():
        print(f'{hx(a)} {_q(t, 70)}' + (f' ({v})' if v != 'ptr' else '') + ' <- ' +
              ', '.join(db.fname(x) for x in fs[:8]) + (' …' if len(fs) > 8 else ''))
    print(f'{len(by)} strings, {len({h[2] for h in hits})} functions', file=sys.stderr)


def call_edges(db, f, include_refs=False):
    out = []
    for c in f['calls']:
        if c[0] is not None:
            out.append((c[0], c[1]))
        elif c[1] in ('slot', 'slotidx'):
            out += [(x, c[1]) for x in c[5]]
        elif c[1] == 'table':
            out += [(x, 'table') for x in c[6]]
    if include_refs:
        out += [(x, 'ref') for x in f['fnrefs']]
    seen, res = set(), []
    for t, k in out:
        if t not in seen:
            seen.add(t)
            res.append((t, k))
    return res


def cmd_graph(db, args):
    f = db.resolve_func(args.name)
    if f is None:
        sys.exit(f'no function {args.name!r}')
    seen = set()
    lines = []
    tree = {}

    def rec(a, depth, prefix, kind, node):
        g = db.by_addr.get(a)
        label = db.fname(a) + ('' if kind in ('bl', None) else f' [{kind}]')
        if g is None:
            lines.append(f'{prefix}{label}')
            return
        if a in seen and depth > 0:
            lines.append(f'{prefix}{label} …')
            node['seen'] = True
            return
        seen.add(a)
        lines.append(f'{prefix}{label}')
        if depth >= args.depth:
            return
        if args.up:
            nxt = []
            s = set()
            for c, k, site in g['callers']:
                if (k != 'ref' or args.refs) and c not in s:
                    s.add(c)
                    nxt.append((c, k))
        else:
            nxt = call_edges(db, g, args.refs)
        if args.hide_hubs:
            nxt = [(t, k) for t, k in nxt if len({c for c, kk, _ in db.by_addr.get(t, {}).get('callers', [])
                                                   if kk != 'ref'}) <= args.hide_hubs]
        node['children'] = []
        for t, k in nxt:
            child = dict(name=db.fname(t, False), addr=t, kind=k)
            node['children'].append(child)
            rec(t, depth + 1, prefix + '  ', k, child)

    tree = dict(name=f['name'], addr=f['addr'])
    rec(f['addr'], 0, '', None, tree)
    if args.json:
        print(json.dumps(tree, indent=1))
    else:
        print('\n'.join(lines))


def cmd_unit(db, args):
    fs = [f for f in db.by_addr.values() if f['unit'] == args.unit or f['unit'].lower() == args.unit.lower()]
    if not fs:
        m = parse_addr(args.unit.split('_')[-1]) if '_' in args.unit or parse_addr(args.unit) else None
        cands = sorted({f['unit'] for f in db.by_addr.values() if args.unit.lower() in f['unit'].lower()})
        if m is not None:
            g = db.containing_func(m)
            if g:
                cands = [g['unit']]
        if len(cands) == 1:
            fs = [f for f in db.by_addr.values() if f['unit'] == cands[0]]
        else:
            sys.exit(f'no unit {args.unit!r}' + (f'; candidates: {", ".join(cands[:20])}' if cands else ''))
    fs.sort(key=lambda f: f['addr'])
    unit = fs[0]['unit']
    members = {f['addr'] for f in fs}
    ext_callers = collections.Counter()
    ext_callees = collections.Counter()
    globs = collections.Counter()
    strings = []
    assets = collections.Counter()
    rows = []
    for f in fs:
        ins = {c for c, k, _ in f['callers'] if k != 'ref'}
        outs = {t for t, k in call_edges(db, f)}
        for c in ins - members:
            ext_callers[c] += 1
        for t in outs - members:
            ext_callees[t] += 1
        gl = set()
        for addr, size, rw, idx, cnt, offl, unk in f['mem']:
            if region(addr) in ('EWRAM', 'IWRAM'):
                b, _ = db.object_of(addr)
                gl.add(b if b is not None else addr)
            if region(addr) == 'IO':
                gl.add(0x04000000)
        for g in gl:
            globs[g] += 1
        for r in f['rom']:
            if 'asset' in r:
                assets[r['asset']] += 1
        for s in f['strings'][:3]:
            strings.append((f['addr'], s[1]))
        note = []
        if f['addr'] in db.proposed:
            note.append(db.proposed[f['addr']][0] + '?')
        main, _, _ = wiki_links(db, f['addr'], unit)
        if main:
            note.append('[[' + main[0] + ']]')
        if f['src'] == 'asm' and unit.startswith('code_'):
            note.append('ASM')
        s1 = (f['strings'][0][1] if f['strings'] else '')
        rows.append((f, len(ins), len(outs), note, s1))
    if args.json:
        print(json.dumps(dict(unit=unit, functions=[dict(name=f['name'], addr=f['addr'], size=f['size'],
                                                         callers=ni, callees=no, notes=nt)
                                                    for f, ni, no, nt, _ in rows],
                              external_callers={db.fname(k, False): v for k, v in ext_callers.most_common()},
                              external_callees={db.fname(k, False): v for k, v in ext_callees.most_common()},
                              globals={db.describe_addr(k, short=True): v for k, v in globs.most_common()},
                              assets=dict(assets)), indent=1, ensure_ascii=False))
        return
    lo, hi = fs[0]['addr'], fs[-1]['addr'] + fs[-1]['size']
    main, unitpage, _ = wiki_links(db, lo, unit)
    print(f'unit {unit}: {len(fs)} functions, {hx(lo)}-{hx(hi)} (0x{hi - lo:X} bytes)' +
          (f'  wiki [[{unitpage}]]' if unitpage else ''))
    print(f'  {"function":24s} {"size":>6s} {"in":>3s} {"out":>3s}  notes / first string')
    for f, ni, no, nt, s1 in rows:
        extra = ' '.join(nt)
        if s1:
            extra += ('  ' if extra else '') + _q(s1, 40)
        print(f"  {f['name']:24s} {f['size']:#6x} {ni:3d} {no:3d}  {extra}")
    print('called from outside: ' + ', '.join(f'{db.fname(k, False)}' for k, _ in ext_callers.most_common(15)) +
          (f' … +{len(ext_callers) - 15}' if len(ext_callers) > 15 else ''))
    print('calls outside: ' + ', '.join(f'{db.fname(k, False)} x{v}' for k, v in ext_callees.most_common(15)) +
          (f' … +{len(ext_callees) - 15}' if len(ext_callees) > 15 else ''))
    print('globals (functions using them): ' + ', '.join(
        f'{db.describe_addr(k, short=True, field=False) if k != 0x04000000 else "IO regs"} x{v}'
        for k, v in globs.most_common(14)))
    if assets:
        print('assets: ' + ', '.join(f'{k} x{v}' for k, v in assets.most_common(8)))


# ========================================================================================= clustering
def louvain(nodes, edges, resolution=1.0):
    """Louvain modularity clustering (deterministic: nodes are visited in the given order).
    edges: {(u, v): w} undirected. Returns a list of partitions {node: community}, finest level first."""
    adj = collections.defaultdict(dict)
    for (u, v), w in edges.items():
        adj[u][v] = adj[u].get(v, 0) + w
        adj[v][u] = adj[v].get(u, 0) + w
    for n in nodes:
        adj.setdefault(n, {})
    cur_nodes = list(nodes)
    cur_adj = adj
    members = {n: [n] for n in nodes}
    levels = []
    for _level in range(12):
        m2 = sum(sum(nb.values()) for nb in cur_adj.values())
        if m2 == 0:
            break
        deg = {n: sum(cur_adj[n].values()) for n in cur_nodes}
        comm = {n: n for n in cur_nodes}
        tot = dict(deg)
        for _round in range(30):
            moved = False
            for n in cur_nodes:
                c0 = comm[n]
                links = collections.defaultdict(float)
                for nb, w in cur_adj[n].items():
                    if nb != n:
                        links[comm[nb]] += w
                tot[c0] -= deg[n]
                best, gain = c0, links.get(c0, 0) - resolution * tot[c0] * deg[n] / m2
                for c, w in links.items():
                    g = w - resolution * tot[c] * deg[n] / m2
                    if g > gain + 1e-12:
                        best, gain = c, g
                tot[best] += deg[n]
                if best != c0:
                    comm[n] = best
                    moved = True
            if not moved:
                break
        groups = collections.defaultdict(list)
        for n in cur_nodes:
            groups[comm[n]].append(n)
        if len(groups) == len(cur_nodes):
            break
        members = {c: [x for n in ns for x in members[n]] for c, ns in groups.items()}
        levels.append({x: c for c, xs in members.items() for x in xs})
        new_adj = collections.defaultdict(dict)
        for n in cur_nodes:
            for nb, w in cur_adj[n].items():
                a, b = comm[n], comm[nb]
                new_adj[a][b] = new_adj[a].get(b, 0) + w
        cur_nodes = sorted(groups)
        cur_adj = new_adj
    if not levels:
        levels.append({n: n for n in nodes})
    return levels


STOP = set('the a an of to and or in on for is it you your this that with be are was by at as from not no yes '
           'can will have has please select which what do does card cards one all any its into than then out '
           'get got use used'.split())
LABEL_DIRS = ('game', 'functions', 'data', 'rom')


def subsystem_graph(db, args):
    fs = [f for f in db.by_addr.values() if f['unit'].startswith('code_') or f['unit'].startswith('sound')]
    nodes = [f['addr'] for f in sorted(fs, key=lambda f: f['addr'])]
    nodeset = set(nodes)
    indeg = collections.Counter()
    for f in fs:
        indeg[f['addr']] = len({c for c, k, _ in f['callers'] if k != 'ref'})
    hubs = {a for a in nodes if indeg[a] > args.hub}
    edges = collections.Counter()

    def addw(u, v, w):
        if u == v or u in hubs or v in hubs or u not in nodeset or v not in nodeset:
            return
        edges[(min(u, v), max(u, v))] += w
    for f in fs:
        for t, k in call_edges(db, f, include_refs=True):
            addw(f['addr'], t, 0.6 if k == 'ref' else (0.5 if k in ('table', 'slot', 'slotidx') else 1.0))
    users = collections.defaultdict(set)
    for f in fs:
        for addr, size, rw, idx, cnt, offl, unk in f['mem']:
            if region(addr) in ('EWRAM', 'IWRAM', 'ROM'):
                b, _ = db.object_of(addr)
                b = addr if b is None else b
                big = b in db.decls and db.decls[b]['size'] > 0x400
                users[(b, addr - b if big else 0)].add(f['addr'])
        for r in f['rom']:
            users[('rom', r.get('label') or r['addr'])].add(f['addr'])
    for key, us in users.items():
        us = sorted(u for u in us if u in nodeset and u not in hubs)
        if 2 <= len(us) <= args.gmax:
            w = args.gweight / (len(us) - 1)
            for i in range(len(us)):
                for j in range(i + 1, len(us)):
                    addw(us[i], us[j], w)
    for i in range(len(nodes) - 1):
        a, b = nodes[i], nodes[i + 1]
        if db.by_addr[a]['unit'] == db.by_addr[b]['unit']:
            addw(a, b, args.adjacency)
    return [n for n in nodes if n not in hubs], edges, hubs, indeg


def cmd_subsystems(db, args):
    nodes, edges, hubs, indeg = subsystem_graph(db, args)
    levels = louvain(nodes, edges, args.resolution)
    fine = levels[0]
    coarse = levels[min(args.level, len(levels) - 1)] if args.level >= 0 else levels[-1]
    stats = _label_stats(db, nodes)
    top = collections.defaultdict(lambda: collections.defaultdict(list))
    for n in nodes:
        top[coarse[n]][fine[n]].append(n)
    result = []
    for c, subs in sorted(top.items(), key=lambda kv: min(min(v) for v in kv[1].values())):
        ms = sorted(x for v in subs.values() for x in v)
        if len(ms) < args.min:
            continue
        entry = _label_cluster(db, ms, stats)
        entry['parts'] = [_label_cluster(db, sorted(v), stats) for _, v in
                          sorted(subs.items(), key=lambda kv: min(kv[1])) if len(v) >= args.min] \
            if len(subs) > 1 else []
        result.append(entry)
    if args.json:
        print(json.dumps(dict(clusters=result, hubs=[db.fname(h, False) for h in sorted(hubs)]), indent=1,
                         ensure_ascii=False))
        return
    for i, c in enumerate(result):
        print(f"[{i + 1}] {c['label']}  ({len(c['functions'])} functions, {c['span']}; {c['units']})")
        for e in c['evidence']:
            print(f'      {e}')
        if not c['parts']:
            print('      members: ' + ', '.join(c['functions'][:args.show]) +
                  (f' … +{len(c["functions"]) - args.show}' if len(c['functions']) > args.show else ''))
        for j, p in enumerate(c['parts']):
            print(f"   [{i + 1}.{j + 1}] {p['label']}  ({len(p['functions'])} functions, {p['span']})")
            if args.verbose:
                for e in p['evidence']:
                    print(f'          {e}')
            print('          ' + ', '.join(p['functions'][:args.show]) +
                  (f' … +{len(p["functions"]) - args.show}' if len(p['functions']) > args.show else ''))
    print(f'\nshared utilities (more than {args.hub} callers, left out of the clustering): ' +
          ', '.join(db.fname(h) for h in sorted(hubs, key=lambda h: -indeg[h])[:40]) +
          (f' … +{len(hubs) - 40}' if len(hubs) > 40 else ''))


def _func_features(db, a):
    f = db.by_addr[a]
    pages = [p for p in wiki_links(db, a, f['unit'])[0]
             if db.titles.get(p, [''])[0].split('/')[0] in LABEL_DIRS]
    words = set()
    for s in f['strings']:
        for w in re.findall(r'[A-Za-z]{3,}', s[1]):
            w = w.lower()
            if w not in STOP and len(set(w)) > 1:
                words.add(w)
    globs = set()
    for addr, size, rw, idx, cnt, offl, unk in f['mem']:
        if region(addr) in ('EWRAM', 'IWRAM'):
            b, _ = db.object_of(addr)
            globs.add(b if b is not None else addr)
    assets = {r['asset'] for r in f['rom'] if 'asset' in r and not r['asset'].startswith('rodata/')}
    return pages[:2], words, globs, assets


def _label_stats(db, nodes):
    """Document frequencies over all clustered functions, for picking distinctive evidence."""
    df = dict(pages=collections.Counter(), words=collections.Counter(), globs=collections.Counter(),
              assets=collections.Counter())
    feats = {}
    for a in nodes:
        p, w, g, s = _func_features(db, a)
        feats[a] = (p, w, g, s)
        df['pages'].update(set(p))
        df['words'].update(w)
        df['globs'].update(g)
        df['assets'].update(s)
    return dict(df=df, feats=feats, n=len(nodes))


def _distinct(counter, df, n, size, k):
    """Items common in the cluster and rare elsewhere (share inside the cluster x coverage)."""
    scored = []
    for item, c in counter.items():
        if c < 2 and size > 3:
            continue
        inside = c / df[item] if df[item] else 0
        cover = c / size
        scored.append((inside * cover, item, c))
    scored.sort(key=lambda x: (-x[0], str(x[1])))
    return [(item, c) for sc, item, c in scored[:k] if sc > 0.02]


def _label_cluster(db, ms, stats):
    pages, words, globs, assets = (collections.Counter() for _ in range(4))
    proposed = []
    for a in ms:
        p, w, g, s = stats['feats'].get(a) or _func_features(db, a)
        pages.update(p)
        words.update(w)
        globs.update(g)
        assets.update(s)
        if a in db.proposed:
            proposed.append(db.proposed[a][0])
    df, size = stats['df'], len(ms)
    tp = _distinct(pages, df['pages'], stats['n'], size, 3)
    tw = _distinct(words, df['words'], stats['n'], size, 8)
    tg = _distinct(globs, df['globs'], stats['n'], size, 4)
    ta = _distinct(assets, df['assets'], stats['n'], size, 3)
    ev, label = [], []
    if tp:
        ev.append('wiki: ' + ', '.join(f'[[{p}]] x{c}' for p, c in tp))
        label.append(db.titles.get(tp[0][0], [tp[0][0], tp[0][0]])[1])
    if proposed:
        ev.append('proposed names: ' + ', '.join(proposed[:8]) + (' …' if len(proposed) > 8 else ''))
        pre = collections.Counter(_name_stem(n) for n in proposed)
        best = [x for x, c in pre.most_common(2) if c >= 2 or size <= 12]
        if not label and best:
            label.append('/'.join(best))
    if ta:
        ev.append('assets: ' + ', '.join(f'{k} x{v}' for k, v in ta))
        if len(label) < 2:
            label.append(ta[0][0])
    if tw:
        ev.append('text: ' + ', '.join(w for w, _ in tw))
        if len(label) < 2:
            label.append('"' + ' '.join(w for w, _ in tw[:3]) + '"')
    if tg:
        ev.append('globals: ' + ', '.join(f'{db.describe_addr(g, short=True, field=False)} x{c}' for g, c in tg))
        if not label:
            label.append('uses ' + db.describe_addr(tg[0][0], short=True, field=False))
    units = sorted({db.by_addr[a]['unit'] for a in ms})
    lo = ms[0]
    hi = max(a + db.by_addr[a]['size'] for a in ms)
    return dict(label=' / '.join(label) or '(no distinctive evidence)', functions=[db.fname(a, False) for a in ms],
                span=f'{hx(lo)}-{hx(hi)}', units=_unit_ranges(units), evidence=ev)


def _name_stem(n):
    """CB_Title -> CB_Title, MainMenu_Init -> MainMenu, SoundMixFifo -> SoundMix."""
    if n.startswith('CB_'):
        return n
    if '_' in n:
        return n.split('_')[0]
    words = re.findall(r'[A-Z][a-z0-9]*', n)
    return ''.join(words[:2]) if words else n


def _unit_ranges(units):
    if len(units) <= 3:
        return ', '.join(units)
    return f'{len(units)} units {units[0]} … {units[-1]}'


def cmd_what(db, args):
    a = parse_addr(args.addr)
    if a is None:
        a = resolve_global(db, args.addr)
    if a is None:
        sys.exit(f'not an address: {args.addr}')
    f = db.containing_func(a & ~1)
    if f:
        print(f"code: {f['name']}+0x{(a & ~1) - f['addr']:X} (unit {f['unit']})" +
              (f"  proposed {db.proposed[f['addr']][0]}" if f['addr'] in db.proposed else ''))
        return
    print(f'{hx(a)}: {db.describe_addr(a)}  [{region(a)}]')
    if a in db.proposed:
        print(f'  proposed name: {db.proposed[a][0]}  # {db.proposed[a][1]}')
    for p in db.j.get('assets', []):
        if p[0] <= a < p[1]:
            print(f'  asset {p[3]} ({p[2]}), +0x{a - p[0]:X} into {hx(p[0])}-{hx(p[1])}')
    base, size = db.object_of(a)
    if base is not None and base in db.decls:
        d = db.decls[base]
        print(f"  declared as {d['type']} in {d['units']} units" +
              (f" (element 0x{d['elem']:X})" if d.get('count') is not None else ''))
    refs, whats, stored = set(), set(), []
    for g in db.by_addr.values():
        for r in g['rom']:
            if r['addr'] == a:
                refs.add(g['addr'])
                if r.get('what'):
                    whats.add(r['what'])
        for m in g['mem']:
            if m[0] == a or (m[3] and m[0] <= a < m[0] + 0x10000 and a - m[0] in (m[5] or [])):
                refs.add(g['addr'])
        for d, lname, loff, what, users in g.get('datarefs', []):
            if d == a:
                stored.append((g['addr'], what))
    for w in sorted(whats):
        print(f'  {w}')
    for fa, w in stored:
        print(f'  holds a pointer to {db.fname(fa)}' + (f'  ({w})' if w else ''))
    if refs:
        print(f'  used by {len(refs)} functions: ' + ', '.join(db.fname(x, False) for x in sorted(refs)[:20]) +
              (' …' if len(refs) > 20 else ''))
    main, _, _ = wiki_links(db, a)
    if main:
        print('  wiki: ' + ', '.join(f'[[{p}]]' for p in main[:6]))


def cmd_selftest(db, args):
    """Facts documented in the wiki (and checked against the C sources) that the database must reproduce."""
    F = db.by_addr
    fails = []

    def check(what, ok):
        print(('PASS ' if ok else 'FAIL ') + what)
        if not ok:
            fails.append(what)

    def callers(a):
        return {c for c, k, _ in F[a]['callers'] if k not in ('ref', 'data')}

    def callees(a):
        return {t for t, k in call_edges(db, F[a])}

    def mem(a):
        return {(m[0], m[2]) for m in F[a]['mem']}
    lz = 0x0807A1A8   # wiki/functions/lzss-decompress.md
    check('LZSS decoder: called by the three dialogue-scene loaders, 10 bl sites, calls nothing',
          callers(lz) == {0x08000420, 0x08000570, 0x08000708} and
          sum(1 for c, k, _ in F[lz]['callers'] if k == 'bl') == 10 and not F[lz]['calls'])
    check('LZSS decoder: reads and writes the ring buffer 0x02030000 with an index, 3 parameters',
          any(m[0] == 0x02030000 and m[3] for m in F[lz]['mem']) and F[lz]['nparams'] == 3)
    ml = 0x08075D70   # wiki/game/program-flow.md section 2
    check('MainLoop: FrameSyncUpdate, SetMainCallback(CB_MainMenu), DebugHook; reads gMain.callback',
          {0x08075CB4, 0x080754F8, 0x08075D6C} <= callees(ml) and (0x03000450, 'r') in mem(ml) and
          any(c[0] == 0x080754F8 and c[3].get('r0') == 0x08003AA5 for c in F[ml]['calls']))
    check('MainLoop: gMain.callback slot resolves to the scene callbacks (License, MainMenu, Title)',
          {0x08004EAC, 0x08003AA4, 0x080057BC} <= callees(ml))
    check('MainLoop: intrCheck/frameCounter/frameCounter8/lagCounter at 0x0300044C/489E/48A0/48A6',
          {(0x0300044C, 'w'), (0x0300489E, 'w'), (0x030048A0, 'w'), (0x030048A6, 'w')} <= mem(ml))
    gi = 0x08075DF4   # program-flow.md, GameInit steps 2-7
    slots = {tuple(x) for x in F[gi]['slots']}
    check('GameInit: IntrTable VBlank/Timer2/DMA1/slot13 and gMain.callback = License',
          {(0x03000008, 0x0807569C), (0x03000018, 0x0807570C), (0x03000024, 0x0807E324),
           (0x03000034, 0x08075740), (0x03000450, 0x08004EAC)} <= slots)
    check('GameInit: ReadSram(SRAM, 0x02011C20, 0x2170); writes IE, WAITCNT, IME, INTR_VECTOR',
          any(c[0] == 0x0807ED28 and c[3].get('r0') == 0x0E000000 and c[3].get('r1') == 0x02011C20 and
              c[3].get('r2') == 0x2170 for c in F[gi]['calls']) and
          {(0x04000200, 'w'), (0x04000204, 'w'), (0x04000208, 'w'), (0x03007FFC, 'w')} <= mem(gi))
    smc = 0x080754F8  # set-main-callback.md
    check('SetMainCallback: stores its argument into gMain.callback and calls SaveGame',
          [0, 0x03000450] in F[smc]['param_slots'] and 0x080754BC in callees(smc))
    check('sub_08000420 (scene loader): 5 parameters, like its C signature',
          F[0x08000420]['nparams'] == 5)
    dm = 0x08021A48   # names.txt DuelMainStep: phase table 0x08198F80 indexed by *(u8*)0x02015EE8
    check('DuelMainStep: dispatches sDuelPhaseTable 0x08198F80 and reads the phase byte 0x02015EE8',
          any(c[1] == 'table' and c[4] == 0x08198F80 and 0x08021834 in c[6] for c in F[dm]['calls']) and
          (0x02015EE8, 'r') in mem(dm))
    im = 0x080000FC   # interrupt-handlers.md
    check('IntrMain (ARM): IntrTable dispatch reaches VBlankIntr and Timer2Intr',
          {0x0807569C, 0x0807570C} <= callees(im))
    pw = 0x0807C304   # card-data-functions.md: password -> card ID over the password table
    check('Password lookup reads cards/passwords and returns a value',
          any(r.get('asset', '').startswith('cards/passwords') for r in F[pw]['rom']) and F[pw]['ret'])
    cn = 0x08000C54   # card-data-functions.md: card number -> ID -> name
    check('sub_08000C54 uses the number->ID map and the card name table',
          {'cards/number_to_id', 'cards/names'} <= {asset_stem(r.get('asset', '')) for r in F[cn]['rom']})
    eff = 0x0802C77C  # cards.md: effect table 0x0819A9D4 {u32 key; fn[5]}
    check('Effect handler sub_0802C77C is fn[3] of the Monster Eye record (key 401) in 0x0819A9D4',
          any(d[0] == 0x0819B08C and 'Monster Eye' in (d[3] or '') for d in F[eff].get('datarefs', [])))
    sd = 0x08001C10   # names.txt StartDialogue(eventId)
    check('StartDialogue: argument 0 decoded as a dialogue event ID',
          db.decoders.get((sd, 0)) == 'dialogue')
    pk = 0x08062F6C
    check('Booster pack names found as inline records (Vol.1 ... Limited Collection)',
          any(x[1] == 'Vol.1' for x in F[pk]['strings']) if pk in F else False)
    print(f'{len(fails)} failed' if fails else 'all checks passed')
    if fails:
        sys.exit(1)


# ============================================================================================ main
def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n\n')[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--db', default=OUT, help='xref database (default build/xref/xref.json)')
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('build', help='(re)build build/xref/xref.json from the ELF')
    p.add_argument('--elf', default=ELF)
    p.add_argument('--out', default=OUT)
    p.add_argument('--no-structs', action='store_true', help='skip the C struct field pass')
    p = sub.add_parser('func', help='summary card of a function')
    p.add_argument('name')
    p.add_argument('--all', action='store_true', help='no truncation')
    p.add_argument('--json', action='store_true')
    p = sub.add_parser('global', help='functions touching a global')
    p.add_argument('name')
    p.add_argument('--offset', help='only this byte offset inside the object')
    p.add_argument('--exact', action='store_true', help='only accesses at exactly this address')
    p.add_argument('--all', action='store_true')
    p.add_argument('--json', action='store_true')
    p = sub.add_parser('strings', help='functions using text that matches a regex')
    p.add_argument('regex')
    p.add_argument('--case', action='store_true', help='case-sensitive')
    p.add_argument('--json', action='store_true')
    p = sub.add_parser('graph', help='call tree')
    p.add_argument('name')
    p.add_argument('--depth', type=int, default=2)
    p.add_argument('--up', action='store_true', help='callers instead of callees')
    p.add_argument('--refs', action='store_true', help='also follow address-taken function references')
    p.add_argument('--hide-hubs', type=int, default=0, metavar='N', help='skip callees with more than N callers')
    p.add_argument('--json', action='store_true')
    p = sub.add_parser('unit', help='one-screen overview of a unit')
    p.add_argument('unit')
    p.add_argument('--json', action='store_true')
    p = sub.add_parser('subsystems', help='cluster functions into candidate subsystems')
    p.add_argument('--min', type=int, default=3, help='smallest cluster to list')
    p.add_argument('--hub', type=int, default=15, help='functions with more callers are utilities (excluded)')
    p.add_argument('--gmax', type=int, default=12, help='globals used by more functions are ignored')
    p.add_argument('--gweight', type=float, default=0.3, help='weight of sharing one global')
    p.add_argument('--adjacency', type=float, default=0.5, help='weight between neighbouring functions of a unit')
    p.add_argument('--resolution', type=float, default=1.0, help='higher = smaller clusters')
    p.add_argument('--level', type=int, default=-1, help='Louvain level for the top clusters (-1 = coarsest)')
    p.add_argument('--show', type=int, default=10, help='members to print per cluster')
    p.add_argument('--verbose', '-v', action='store_true', help='evidence for every sub-cluster too')
    p.add_argument('--json', action='store_true')
    p = sub.add_parser('what', help='what lives at an address')
    p.add_argument('addr')
    sub.add_parser('selftest', help='check the database against facts documented in the wiki')
    args = ap.parse_args()
    if args.cmd == 'build':
        return cmd_build(args)
    db = DB(args.db)
    db.stale_warning()
    {'func': cmd_func, 'global': cmd_global, 'strings': cmd_strings, 'graph': cmd_graph, 'unit': cmd_unit,
     'subsystems': cmd_subsystems, 'what': cmd_what, 'selftest': cmd_selftest}[args.cmd](db, args)


if __name__ == '__main__':
    main()
