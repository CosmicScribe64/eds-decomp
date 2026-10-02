#!/usr/bin/env python3
"""Map every USA function to its counterpart in the Japanese ROM (AY5J, roms/base_jp.gba).

    tools/dr python3 tools/jpmap.py                       # analyse, match, write build/jp/*
    tools/dr python3 tools/jpmap.py --compile-test        # + recompile matched USA C at JP addresses
    tools/dr python3 tools/jpmap.py --assets              # + where each config/assets.tsv range is in JP
    tools/dr python3 tools/jpmap.py --diff sub_08044224   # + aligned USA/JP listing (and case map)

Read-only research tool: it reads both ROMs and the repo and writes only under build/jp/ (about 30 s).

Steps
 1. Function discovery. USA: extents from config/functions.tsv, instruction / literal / jump-table
    classification from tools/disasm.py's recursive-descent Analyzer (it reproduces functions.tsv
    exactly). JP: the same Analyzer with its module globals retargeted to the JP image (code range,
    the JP-only ARM routine, the library symbols found by searching the USA library bytes, the AgbMain
    seed from crt0). Seeding with Thumb pointers from JP data tables was tried and adds nothing.
 2. Tokens per function at three strictness levels (`thumb_tokens()`):
      E  exact modulo relocation: raw instructions; BL targets, ROM/RAM addresses in literal pools
         and jump-table targets (relative to the function) masked;
      O  E with load/store offsets, add/sub immediates and non-address pool constants masked
         (struct layout / buffer size changes);
      S  shape: instruction format + registers only, every immediate masked;
    plus F (format only, registers dropped) for similarity: 2*LCS/(len_a+len_b) (rapidfuzz InDel ratio;
    difflib's ratio as a fallback), as in tools/similar.py.
 3. Matching (`Matcher.run()`): unique E/O/S hashes as anchors; call-graph propagation (aligned calls
    and Thumb function pointers of matched pairs vote for their counterparts); neighbour extension along
    both link orders; then a global search at decreasing thresholds where rapidfuzz preselects
    candidates and `score()` combines format similarity, shared constants (card numbers, masks, sizes)
    and shared references through the function/address map learned so far.
 4. Reports in build/jp/: map.tsv (every USA function + JP-only rows), units.tsv (per USA unit),
    blocks.tsv / link_order.tsv (pieces of link order both builds share), ram_map.tsv /
    romdata_map.tsv (USA->JP address pairs from aligned literal pools), summary.txt; optionally
    compile_test.txt, assets.tsv, <func>_listing.txt.
"""
import argparse
import bisect
import collections
import difflib
import hashlib
import os
import re
import struct
import subprocess
import sys

try:
    from rapidfuzz import fuzz, process     # in the Docker image; LCS (InDel) ratio, fast
except ImportError:                         # pragma: no cover
    fuzz = process = None

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import disasm  # noqa: E402  (reads baserom.gba at import; its globals are retargeted below)

BASE = 0x08000000
US_PATH = 'baserom.gba'
JP_PATH = 'roms/base_jp.gba'
JP_SHA1 = 'dc25f733cee913afdf187ec03df30909fe28b03c'
OUT = 'build/jp'

# --- JP layout (found by comparing the images; asserted in jp_layout()) ---------------------------
JP_CODE_START = 0x0800022C   # crt0 is 4 bytes longer: IntrMain loads its IE mask (0x2008) from a pool
JP_ARM_FUNCS = {0x0805CA00: 0x68}  # APCS-frame ARM HBlank routine (BG1HOFS/BLDALPHA per line); JP only
# libc functions JP links but USA does not (identified by reading them: newlib word-at-a-time versions)
JP_LIB_EXTRA = {0x08089A60: 'strcat', 0x08089AA8: 'strcmp'}
# JP-only Mobile Adapter GB library ("MAGB", gameboy.datacenter.ne.jp HTTP/SMTP client), linked after AgbSram
JP_MOBILE_LIB = (0x0807D2E4, 0x08087F80)


def u16(rom, a):
    return struct.unpack_from('<H', rom, a - BASE)[0]


def u32(rom, a):
    return struct.unpack_from('<I', rom, a - BASE)[0]


# ------------------------------------------------------------------------------------------------
# Layout and function discovery
# ------------------------------------------------------------------------------------------------
class Image:
    """One ROM image with its functions classified (insns / literals / jump tables / padding)."""

    def __init__(self, name, rom):
        self.name, self.rom = name, rom
        self.funcs = {}        # addr -> dict(addr, size, mode, name, unit)
        self.order = []        # sorted function addresses
        self.insns = {}        # addr -> disasm.Insn
        self.lits = {}         # addr -> value
        self.jtabs = {}        # addr -> target
        self.lib = {}          # library symbol addr -> name (outside the analysed code range)
        self.code_start = self.code_end = self.text_end = None

    def func_at(self, a):
        i = bisect.bisect_right(self.order, a) - 1
        if i >= 0:
            f = self.funcs[self.order[i]]
            if a < f['addr'] + f['size']:
                return f
        return None


def run_analyzer(rom, code_start, code_end, seeds, arm_regions, arm_funcs, lib_syms, veneer, extra_seeds=()):
    """tools/disasm.py's Analyzer with its module globals pointed at `rom`."""
    saved = {k: getattr(disasm, k) for k in
             ('rom', 'CODE_START', 'CODE_END', 'SEEDS_THUMB', 'ARM_REGIONS', 'ARM_FUNCS', 'LIB_SYMS', 'VENEER')}
    try:
        disasm.rom = rom
        disasm.CODE_START, disasm.CODE_END = code_start, code_end
        disasm.SEEDS_THUMB = list(seeds) + sorted(set(extra_seeds) - set(seeds))
        disasm.ARM_REGIONS, disasm.ARM_FUNCS = arm_regions, arm_funcs
        disasm.LIB_SYMS, disasm.VENEER = lib_syms, veneer
        an = disasm.Analyzer()
        an.run()
        return an
    finally:
        for k, v in saved.items():
            setattr(disasm, k, v)


def load_us(rom):
    img = Image('USA', rom)
    img.code_start, img.code_end, img.text_end = disasm.CODE_START, disasm.CODE_END, disasm.TEXT_END
    img.lib = dict(disasm.LIB_SYMS)
    an = run_analyzer(rom, disasm.CODE_START, disasm.CODE_END, disasm.SEEDS_THUMB, disasm.ARM_REGIONS,
                      disasm.ARM_FUNCS, disasm.LIB_SYMS, disasm.VENEER)
    img.insns, img.lits, img.jtabs = an.insns, an.lits, an.jtabs
    for line in open('config/functions.tsv'):
        if line.startswith('#'):
            continue
        f = line.rstrip('\n').split('\t')
        a = int(f[0], 16)
        img.funcs[a] = dict(addr=a, size=int(f[2], 16), mode=f[1], name=f[3], unit=f[4])
    img.order = sorted(img.funcs)
    found = set(an.funcs)
    listed = set(img.funcs)
    img.analyzer_check = (len(found & listed), len(found - listed), len(listed - found))
    return img


def jp_layout(us, jp):
    """Locate the JP library tail and SDK pieces by searching for the USA bytes."""
    lay = {}
    # libgcc/libc: each USA library function's leading bytes, searched in JP
    lib = {}
    names = sorted(disasm.LIB_SYMS.items())
    for i, (a, n) in enumerate(names):
        nxt = names[i + 1][0] if i + 1 < len(names) else disasm.VENEER
        seg = us[a - BASE:min(nxt, a + 0x40) - BASE]
        if len(seg) < 4:
            continue
        hits = [m.start() for m in re.finditer(re.escape(seg), jp[:0x100000])]
        if len(hits) == 1:
            lib[BASE + hits[0]] = n
    lay['lib'] = lib
    callvia = [a for a, n in lib.items() if n == '_call_via_r0']
    assert callvia, 'JP: _call_via_r0 not found'
    lay['code_end'] = callvia[0]
    # end of .text: after the last library function found (strcpy), as in USA
    strcpy = [a for a, n in lib.items() if n == 'strcpy']
    lay['text_end'] = (strcpy[0] + 0x4C) if strcpy else None
    # AgbMain from crt0's literal (same position as USA: 0x08000220)
    assert u32(us, 0x08000220) == 0x08075F65
    lay['agbmain'] = u32(jp, 0x08000220) - 1
    assert u16(jp, JP_CODE_START) & 0xFF00 == 0xB500 and u32(jp, JP_CODE_START - 4) == 0x03000000
    for a, size in JP_ARM_FUNCS.items():
        assert u32(jp, a + 4) == 0xE1A0C00D, 'JP ARM function moved?'
    return lay


def data_thumb_pointers(rom, code_start, code_end, data_lo, data_hi, push_only=True):
    """Odd words in ROM data that point at a Thumb function start (pointer tables)."""
    out = set()
    for o in range(data_lo - BASE, data_hi - BASE - 3, 4):
        v = struct.unpack_from('<I', rom, o)[0]
        if not (v & 1) or not (code_start <= v - 1 < code_end):
            continue
        t = v - 1
        h = struct.unpack_from('<H', rom, t - BASE)[0]
        if (h & 0xFF00) in (0xB500, 0xB400) and t % 4 == 0:
            out.add(t)
        elif not push_only:
            out.add(t)
    return out


def load_jp(us, jp):
    lay = jp_layout(us, jp)
    img = Image('JP', jp)
    img.code_start, img.code_end, img.text_end = JP_CODE_START, lay['code_end'], lay['text_end']
    img.lib = dict(lay['lib'])
    img.lib.update(JP_LIB_EXTRA)
    arm_regions = [(a, a + s) for a, s in JP_ARM_FUNCS.items()]
    seeds = [lay['agbmain']]
    tbl = data_thumb_pointers(jp, img.code_start, img.code_end, img.text_end, 0x08300000)
    # Pointer-table seeds were tried and change nothing (the gap scan already finds those functions),
    # so the JP list is built exactly like the USA one (tools/disasm.py's seeds and gap scan).
    an = run_analyzer(jp, img.code_start, img.code_end, seeds, arm_regions, list(JP_ARM_FUNCS),
                      img.lib, 0)
    img.insns, img.lits, img.jtabs = an.insns, an.lits, an.jtabs
    starts = sorted(an.funcs)
    ends = starts[1:] + [img.code_end]
    for a, e in zip(starts, ends):
        f = an.funcs[a]
        img.funcs[a] = dict(addr=a, size=e - a, mode=f.mode, name=f'sub_{a:08X}', unit='', why=an.why.get(a, ''))
    img.order = starts
    img.table_seeds = tbl
    img.errors = an.errors
    img.lib_calls = collections.Counter(t for t in an.calls if t >= img.code_end)
    return img


# ------------------------------------------------------------------------------------------------
# Tokens
# ------------------------------------------------------------------------------------------------
def classify_word(img, v):
    """Literal-pool word -> 'RAM' / 'CODE' / 'ROMD' (relocated addresses) or None (constant)."""
    if 0x02000000 <= v < 0x04000000:
        return 'RAM'
    if BASE <= v < 0x0A000000:
        if img.code_start <= (v & ~1) < (img.text_end or img.code_end) or v < img.code_start:
            return 'CODE'
        return 'ROMD'
    return None


def thumb_tokens(h, ins):
    """(E, O, S, F) tokens of one Thumb instruction."""
    k = ins.kind
    if k == 'bl':
        return ('BL',), ('BL',), ('BL',), 'BL'
    if k == 'ldrpc':
        return ('L', h), ('L', h), ('LDR', (h >> 8) & 7), 'LDR'
    if k == 'b':
        return h, h, ('B',), 'B'
    if k == 'bc':
        c = (h >> 8) & 15
        return h, h, ('BC', c), ('BC', c)
    if k in ('ret', 'movpc'):
        return h, h, h, ('RET', k)
    top5 = h >> 11
    if top5 <= 2:
        return h, h, h & ~0x07C0, ('SH', top5)
    if top5 == 3:
        if (h >> 10) & 1:
            return h, h & ~0x01C0, h & ~0x01C0, ('AS3', (h >> 9) & 1)
        return h, h, h, ('ASR', (h >> 9) & 1)
    if 4 <= top5 <= 7:
        o = h & ~0xFF if top5 >= 6 else h
        return h, o, h & ~0xFF, ('I8', top5)
    if (h >> 10) == 0x10:
        return h, h, h, ('ALU', (h >> 6) & 15)
    if (h >> 10) == 0x11:
        return h, h, h, ('HI', (h >> 8) & 3)
    if (h >> 12) == 5:
        return h, h, h, ('LSR', (h >> 9) & 7)
    if (h >> 13) == 3 or (h >> 12) == 8:
        return h, h & ~0x07C0, h & ~0x07C0, ('LSI', top5)
    if (h >> 12) in (9, 10):
        return h, h, h & ~0xFF, ('SP', top5)
    if (h >> 8) == 0xB0:
        return h, h, ('SPADJ', h & 0x80), 'SPADJ'
    if (h >> 12) == 11:
        return h, h, h, ('PP', (h >> 11) & 1)
    if (h >> 12) == 12:
        return h, h, h, ('LDM', (h >> 11) & 1)
    return h, h, h, ('X', h >> 8)


class FuncTok:
    """Token streams of one function. Each position has E/O/S/F tokens and a payload:
    ('bl', target) | ('lit', value, class) | ('jt', target) | None; `addrs` gives the address."""
    __slots__ = ('E', 'O', 'S', 'F', 'pay', 'addrs', 'hE', 'hO', 'hS', 'nbl', 'K', 'calls', 'refs')

    def __init__(self):
        self.E, self.O, self.S, self.F, self.pay, self.addrs = [], [], [], [], [], []


def tokens(img, f):
    t = FuncTok()
    a, end = f['addr'], f['addr'] + f['size']
    rom = img.rom
    if f['mode'] == 'a':
        while a < end:
            w = u32(rom, a)
            t.E.append(('A', w)); t.O.append(('A', w)); t.S.append(('A', w)); t.F.append(('A', w >> 20))
            t.pay.append(None); t.addrs.append(a)
            a += 4
    while a < end:
        if a in img.insns:
            ins = img.insns[a]
            h = u16(rom, a)
            e, o, s, fz = thumb_tokens(h, ins)
            t.E.append(e); t.O.append(o); t.S.append(s); t.F.append(fz); t.addrs.append(a)
            t.pay.append(('bl', ins.target) if ins.kind == 'bl' else None)
            a += ins.size
        elif a in img.lits and a % 4 == 0:
            v = img.lits[a]
            c = classify_word(img, v)
            if c:
                t.E.append(('W', c)); t.O.append(('W', c)); t.S.append(('W', c)); t.F.append(('W', c))
            else:
                t.E.append(('K', v)); t.O.append(('K',)); t.S.append(('K',)); t.F.append('K')
            t.pay.append(('lit', v, c)); t.addrs.append(a)
            a += 4
        elif a in img.jtabs:
            v = img.jtabs[a]
            t.E.append(('JT', v - f['addr'])); t.O.append(('JT', v - f['addr'])); t.S.append(('JT',)); t.F.append('JT')
            t.pay.append(('jt', v)); t.addrs.append(a)
            a += 4
        else:
            a += 2      # alignment padding / unreached halfwords (not compared)
    t.hE, t.hO, t.hS = hash(tuple(t.E)), hash(tuple(t.O)), hash(tuple(t.S))
    t.nbl = sum(1 for p in t.pay if p and p[0] == 'bl')
    # constants signature: non-address pool words and cmp immediates (card numbers, sizes, masks)
    t.K = collections.Counter()
    for e, p in zip(t.E, t.pay):
        if p and p[0] == 'lit' and p[2] is None:
            t.K[('K', p[1])] += 1
        elif isinstance(e, int) and (e >> 11) == 5 and (e & 0xFF) > 1:
            t.K[('C', e & 0xFF)] += 1
    t.calls = {p[1] for p in t.pay if p and p[0] == 'bl'}
    # everything the function refers to: callees, function pointers, RAM and ROM-data addresses
    t.refs = {('f', p[1]) for p in t.pay if p and p[0] == 'bl'}
    t.refs |= {('f', p[1] & ~1) for p in t.pay if p and p[0] == 'lit' and p[2] == 'CODE'}
    t.refs |= {('a', p[1]) for p in t.pay if p and p[0] == 'lit' and p[2] in ('RAM', 'ROMD')}
    return t


# ------------------------------------------------------------------------------------------------
# Matching
# ------------------------------------------------------------------------------------------------
class Matcher:
    def __init__(self, us, jp):
        self.us, self.jp = us, jp
        self.tu = {a: tokens(us, f) for a, f in us.funcs.items()}
        self.tj = {a: tokens(jp, f) for a, f in jp.funcs.items()}
        fmap = {}
        for t in list(self.tu.values()) + list(self.tj.values()):
            t.F = [fmap.setdefault(x, len(fmap)) for x in t.F]
        # F as a string (one character per token) for rapidfuzz
        self.su = {a: ''.join(chr(0x100 + x) for x in t.F) for a, t in self.tu.items()}
        self.sj = {a: ''.join(chr(0x100 + x) for x in t.F) for a, t in self.tj.items()}
        self.u2j, self.j2u, self.how = {}, {}, {}
        self.jp_lib_by_name = {n: a for a, n in jp.lib.items()}
        self.amap_image = set()
        self.uidx = {a: i for i, a in enumerate(us.order)}
        self.jidx = {a: i for i, a in enumerate(jp.order)}
        self._ratio = {}
        self.amap = {}          # USA RAM/ROM-data address -> JP address (learned from matched pairs)
        self.jref_index = collections.defaultdict(set)   # JP ref -> JP functions using it
        for j, t in self.tj.items():
            for r in t.refs:
                self.jref_index[r].add(j)

    # -- helpers
    def pair(self, u, j, how):
        if u in self.u2j or j in self.j2u:
            return False
        self.u2j[u], self.j2u[j], self.how[u] = j, u, how
        return True

    def ratio(self, u, j):
        """Similarity of the format-token streams: 2*LCS/(len_a+len_b) (rapidfuzz InDel ratio),
        or difflib's ratio when rapidfuzz is unavailable."""
        k = (u, j)
        if k not in self._ratio and fuzz is not None:
            self._ratio[k] = fuzz.ratio(self.su[u], self.sj[j]) / 100.0
        if k not in self._ratio:
            a, b = self.tu[u].F, self.tj[j].F
            if not a or not b:
                self._ratio[k] = 1.0 if a == b else 0.0
            else:
                sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
                if sm.real_quick_ratio() < 0.3 or sm.quick_ratio() < 0.3:
                    self._ratio[k] = sm.quick_ratio() * 0.5
                else:
                    self._ratio[k] = sm.ratio()
        return self._ratio[k]

    def mapped_refs(self, u):
        out = set()
        for kind, v in self.tu[u].refs:
            if kind == 'f':
                m = self.u2j.get(v)
                if m is None and v in self.us.lib:
                    m = self.jp_lib_by_name.get(self.us.lib[v])
                if m is not None:
                    out.add(('f', m))
            elif v in self.amap:
                out.add(('a', self.amap[v]))
        return out

    def known_jrefs(self, j):
        return {r for r in self.tj[j].refs if (r[0] == 'f' and (r[1] in self.j2u or r[1] in self.jp.lib))
                or (r[0] == 'a' and r[1] in self.amap_image)}

    def score(self, u, j):
        """Composite evidence for a changed pair: format similarity (0.4), shared constants (0.2) and
        shared references through the current function/address map (0.4); a component without data
        is left out and the weights renormalised."""
        parts = [(0.4, self.ratio(u, j))]
        a, b = self.tu[u], self.tj[j]
        if a.K or b.K:
            inter, uni = sum((a.K & b.K).values()), sum((a.K | b.K).values())
            parts.append((0.2, inter / uni))
        mu, kj = self.mapped_refs(u), self.known_jrefs(j)
        if mu or kj:
            parts.append((0.4, 2 * len(mu & kj) / (len(mu) + len(kj))))
        return sum(w * v for w, v in parts) / sum(w for w, _ in parts)

    def learn_addr_map(self):
        """USA->JP RAM / ROM-data address pairs from aligned literal pools of matched pairs. Pairs from
        identical / same-shape functions (exact alignment) are 'strong'; pairs from changed functions
        (LCS alignment, where JP often uses a base+offset where USA uses an absolute alias) are 'weak'."""
        pairs = collections.defaultdict(collections.Counter)
        strong = collections.defaultdict(collections.Counter)
        for u, j in self.u2j.items():
            a, b = self.tu[u], self.tj[j]
            exact = a.hS == b.hS and a.S == b.S
            for iu, ij in self.align(u, j):
                pu, pj = a.pay[iu], b.pay[ij]
                if pu and pj and pu[0] == 'lit' and pj[0] == 'lit' and pu[2] in ('RAM', 'ROMD') and pu[2] == pj[2]:
                    pairs[pu[1]][pj[1]] += 1
                    if exact:
                        strong[pu[1]][pj[1]] += 1
        self.addr_pairs, self.addr_strong = pairs, strong
        self.amap = {}
        for k, c in pairs.items():
            src = strong.get(k) or c
            (v, n), = src.most_common(1)
            if n * 2 > sum(src.values()):
                self.amap[k] = v
        self.amap_image = set(self.amap.values())

    def align(self, u, j):
        """Pairs of aligned token positions (iu, ij) with equal S tokens."""
        a, b = self.tu[u], self.tj[j]
        if a.hS == b.hS and len(a.S) == len(b.S):
            return [(i, i) for i in range(len(a.S))]
        sm = difflib.SequenceMatcher(None, a.S, b.S, autojunk=False)
        out = []
        for blk in sm.get_matching_blocks():
            out += [(blk.a + k, blk.b + k) for k in range(blk.size)]
        return out

    # -- phases
    def anchors(self):
        n = 0
        for lvl in ('hE', 'hO', 'hS'):
            bu, bj = collections.defaultdict(list), collections.defaultdict(list)
            for u, t in self.tu.items():
                if u not in self.u2j and len(t.S) >= 2:
                    bu[getattr(t, lvl)].append(u)
            for j, t in self.tj.items():
                if j not in self.j2u and len(t.S) >= 2:
                    bj[getattr(t, lvl)].append(j)
            for h, us_ in bu.items():
                js = bj.get(h, [])
                if len(us_) == 1 and len(js) == 1:
                    n += self.pair(us_[0], js[0], f'unique-{lvl[1]}')
        return n

    def refs(self, u, j):
        """Aligned (usa_target, jp_target) pairs of calls and Thumb function pointers."""
        out = []
        a, b = self.tu[u], self.tj[j]
        for iu, ij in self.align(u, j):
            pu, pj = a.pay[iu], b.pay[ij]
            if not pu or not pj or pu[0] != pj[0]:
                continue
            if pu[0] == 'bl':
                out.append((pu[1], pj[1]))
            elif pu[0] == 'lit' and pu[2] == 'CODE' and pj[2] == 'CODE' and (pu[1] & 1) == (pj[1] & 1):
                out.append((pu[1] & ~1, pj[1] & ~1))
        return out

    def propagate(self):
        """Matched callers vote for the counterparts of their callees."""
        votes = collections.defaultdict(collections.Counter)
        for u, j in list(self.u2j.items()):
            for tu, tj in self.refs(u, j):
                if tu in self.us.funcs and tj in self.jp.funcs:
                    votes[tu][tj] += 1
        n = 0
        cand = []
        for tu, c in votes.items():
            if tu in self.u2j:
                continue
            (tj, k), = c.most_common(1)
            if tj in self.j2u or k * 2 <= sum(c.values()):
                continue
            cand.append((-k, tu, tj))
        for _, tu, tj in sorted(cand):
            r = self.ratio(tu, tj)
            if r >= 0.35 or self.tu[tu].hS == self.tj[tj].hS:
                n += self.pair(tu, tj, 'callgraph')
        return n

    def neighbours(self, thr=0.55):
        n = 0
        uo, jo = self.us.order, self.jp.order
        changed = True
        while changed:
            changed = False
            for u, j in sorted(self.u2j.items()):
                iu, ij = self.uidx[u], self.jidx[j]
                for d in (1, -1):
                    k = 1
                    while 0 <= iu + d * k < len(uo) and 0 <= ij + d * k < len(jo):
                        nu, nj = uo[iu + d * k], jo[ij + d * k]
                        if nu in self.u2j or nj in self.j2u:
                            break
                        if self.ratio(nu, nj) < thr and self.score(nu, nj) < thr:
                            break
                        n += self.pair(nu, nj, 'neighbour')
                        changed = True
                        k += 1
        return n

    def global_search(self, thr=0.6, min_len=6, margin=0.03, pool=8):
        """Best unmatched JP candidate for every unmatched USA function: rapidfuzz preselects by
        format similarity over all unmatched JP functions, `score()` decides."""
        lo, hi = JP_MOBILE_LIB if self.jp.name == 'JP' else (0, 0)
        ju = [j for j in self.jp.order if j not in self.j2u and len(self.tj[j].F) >= min_len
              and not lo <= j < hi]   # the JP-only Mobile Adapter GB library has no USA counterpart
        choices = [self.sj[j] for j in ju]
        cands = []
        for u in self.us.order:
            if u in self.u2j or len(self.tu[u].F) < min_len:
                continue
            if process is not None:
                res = process.extract(self.su[u], choices, scorer=fuzz.ratio, limit=pool, score_cutoff=30)
                for _, sc, i in res:
                    self._ratio[(u, ju[i])] = sc / 100.0
                js = [ju[i] for _, _, i in res]
            else:
                js = [j for _, j in sorted(((self.ratio(u, j), j) for j in ju), reverse=True)[:pool]]
            # plus JP functions that use the counterparts of this function's references
            votes = collections.Counter()
            for r in sorted(self.mapped_refs(u)):
                g = self.jref_index.get(r, ())
                if len(g) <= 30:
                    votes.update(sorted(x for x in g if x not in self.j2u))
            js = list(dict.fromkeys(js + [x for x, _ in votes.most_common(pool)]))
            lu = len(self.tu[u].F)
            # plausible sizes only; short functions need near-identical format streams
            js = [j for j in js if 0.5 <= len(self.tj[j].F) / lu <= 2.0 and
                  (min(lu, len(self.tj[j].F)) >= 12 or self.ratio(u, j) >= 0.85)]
            sc = sorted(((self.score(u, j), j) for j in js), reverse=True)
            if not sc:
                continue
            r0, j0 = sc[0]
            second = sc[1][0] if len(sc) > 1 else 0.0
            if r0 >= thr and r0 - second >= margin:
                cands.append((r0, u, j0))
        # a JP function claimed by two USA functions goes to the better one only if it wins clearly
        claims = collections.defaultdict(list)
        for r, u, j in cands:
            claims[j].append((r, u))
        n = 0
        for j, lst in claims.items():
            lst.sort(reverse=True)
            if len(lst) > 1 and lst[0][0] - lst[1][0] < margin:
                continue
            n += self.pair(lst[0][1], j, 'similarity')
        return n

    def run(self, log=print):
        log(f'anchors: {self.anchors()}')
        for it in range(10):
            a = self.propagate()
            b = self.neighbours()
            c = self.anchors()
            log(f'round {it}: callgraph +{a}, neighbours +{b}, anchors +{c}')
            if not (a or b or c):
                break
        for thr in (0.9, 0.8, 0.7, 0.6, 0.55, 0.5, 0.45):
            self.learn_addr_map()
            g = self.global_search(thr)
            log(f'global search >= {thr}: +{g}')
            for it in range(10):
                a = self.propagate()
                b = self.neighbours()
                c = self.anchors()
                if not (a or b or c):
                    break
                log(f'  round {it}: callgraph +{a}, neighbours +{b}, anchors +{c}')
        # weaker neighbour extension: a gap of one between two matched pairs
        b = self.fill_single_gaps()
        log(f'single gaps: +{b}')

    def fill_single_gaps(self, thr=0.35):
        n = 0
        uo, jo = self.us.order, self.jp.order
        for i in range(1, len(uo) - 1):
            u = uo[i]
            if u in self.u2j:
                continue
            p, q = uo[i - 1], uo[i + 1]
            if p in self.u2j and q in self.u2j:
                jp_, jq = self.jidx[self.u2j[p]], self.jidx[self.u2j[q]]
                if jq - jp_ == 2:
                    j = jo[jp_ + 1]
                    if j not in self.j2u and self.ratio(u, j) >= thr:
                        n += self.pair(u, j, 'gap')
        return n

    # -- classification
    def status(self, u):
        j = self.u2j.get(u)
        if j is None:
            return 'no-match', '', 0.0
        a, b = self.tu[u], self.tj[j]
        if a.E == b.E:
            return 'identical-mod-relocation', '', 1.0
        if a.O == b.O:
            return 'same-shape-different-constants', 'offsets-only', 1.0
        if a.S == b.S:
            return 'same-shape-different-constants', 'immediates', 1.0
        return 'changed', '', self.ratio(u, j)

    def call_consistent(self, u):
        """Every aligned call / function pointer goes to the mapped counterpart."""
        j = self.u2j[u]
        bad = 0
        for tu, tj in self.refs(u, j):
            m = self.u2j.get(tu)
            if tu in self.us.funcs and m is not None and m != tj:
                bad += 1
            elif tu in self.us.lib and self.jp.lib.get(tj) != self.us.lib[tu]:
                bad += 1
        return bad == 0


# ------------------------------------------------------------------------------------------------
# Reports
# ------------------------------------------------------------------------------------------------
STATUS_ORDER = ['identical-mod-relocation', 'same-shape-different-constants', 'changed', 'no-match']


def confidence(m, u):
    s, d, r = m.status(u)
    if s == 'no-match':
        return ''
    if s != 'changed':
        return 'high'
    sc = m.score(u, m.u2j[u])
    if sc >= 0.7 or (m.how[u] in ('callgraph', 'neighbour') and sc >= 0.5):
        return 'high'
    return 'medium' if sc >= 0.5 else 'low'


def link_blocks(m):
    """Maximal runs of USA functions (in USA order) whose counterparts appear in the same order in
    JP with no other matched function in between: the pieces of the link order both builds share."""
    jidx = m.jidx
    blocks = []
    cur = []
    for u in m.us.order:
        j = m.u2j.get(u)
        if j is None:
            continue
        if cur:
            pj = jidx[m.u2j[cur[-1]]]
            between = [x for x in m.jp.order[pj + 1:jidx[j]] if x in m.j2u]
            if jidx[j] > pj and not between:
                cur.append(u)
                continue
            blocks.append(cur)
        cur = [u]
    if cur:
        blocks.append(cur)
    return blocks


def addr_ranges(pairs):
    """[(usa, jp)] sorted -> runs with a constant delta: (usa_lo, usa_hi, delta, n)."""
    out = []
    for us_, jp_ in sorted(pairs):
        d = jp_ - us_
        if out and out[-1][2] == d:
            lo, hi, _, n = out[-1]
            out[-1] = (lo, us_, d, n + 1)
        else:
            out.append((us_, us_, d, 1))
    return out


def write_reports(m, out=OUT):
    os.makedirs(out, exist_ok=True)
    us, jp = m.us, m.jp
    m.learn_addr_map()
    rows = []
    stat = {}
    for u in us.order:
        f = us.funcs[u]
        s, d, r = m.status(u)
        stat[u] = (s, d, r)
        j = m.u2j.get(u)
        row = [f'0x{u:08X}', f['name'], f['unit'], f'0x{f["size"]:X}']
        if j is None:
            row += ['', '', '', '', s, '', '', '', '']
        else:
            row += [f'0x{j:08X}', f'0x{jp.funcs[j]["size"]:X}', f'{m.ratio(u, j):.3f}', f'{m.score(u, j):.3f}',
                    s, d, m.how[u], 'yes' if m.call_consistent(u) else 'no', confidence(m, u)]
        rows.append(row)
    for j in jp.order:
        if j not in m.j2u:
            rows.append(['', '', '', '', f'0x{j:08X}', f'0x{jp.funcs[j]["size"]:X}', '', '', 'jp-only', '', '', '', ''])
    with open(f'{out}/map.tsv', 'w') as fh:
        fh.write('# usa_addr\tusa_name\tusa_unit\tusa_size\tjp_addr\tjp_size\tsimilarity\tscore\tstatus\tdetail'
                 '\tmatched_by\tcalls_consistent\tconfidence\n')
        fh.write('# similarity = 2*LCS/(len_a+len_b) of format tokens; score adds shared constants and references\n')
        for r in rows:
            fh.write('\t'.join(r) + '\n')

    # per-unit table
    units = collections.OrderedDict()
    for u in us.order:
        units.setdefault(us.funcs[u]['unit'], []).append(u)
    with open(f'{out}/units.tsv', 'w') as fh:
        fh.write('# unit\tfuncs\tbytes\tidentical\tsame_shape\tchanged\tno_match\tpct_bytes_identical\t'
                 'pct_bytes_same_or_better\tpct_bytes_matched\tjp_range\n')
        for unit, fs in units.items():
            c = collections.Counter(stat[u][0] for u in fs)
            b = collections.Counter()
            for u in fs:
                b[stat[u][0]] += us.funcs[u]['size']
            tot = sum(us.funcs[u]['size'] for u in fs)
            js = sorted(m.u2j[u] for u in fs if u in m.u2j)
            jr = f'0x{js[0]:08X}-0x{js[-1]:08X}' if js else ''
            fh.write(f'{unit}\t{len(fs)}\t0x{tot:X}\t{c[STATUS_ORDER[0]]}\t{c[STATUS_ORDER[1]]}\t{c[STATUS_ORDER[2]]}\t'
                     f'{c[STATUS_ORDER[3]]}\t{100 * b[STATUS_ORDER[0]] / tot:.1f}\t'
                     f'{100 * (b[STATUS_ORDER[0]] + b[STATUS_ORDER[1]]) / tot:.1f}\t'
                     f'{100 * (tot - b[STATUS_ORDER[3]]) / tot:.1f}\t{jr}\n')

    # link-order blocks
    blocks = link_blocks(m)
    with open(f'{out}/blocks.tsv', 'w') as fh:
        fh.write('# Runs of USA functions whose JP counterparts keep the same relative order (unmatched '
                 'functions on either side may sit inside a run).\n')
        fh.write('# usa_first\tusa_last\tjp_first\tjp_last\tmatched_funcs\tusa_bytes\tusa_units\n')
        for bl in blocks:
            a, z = bl[0], bl[-1]
            ub = us.funcs[z]['addr'] + us.funcs[z]['size'] - a
            un = sorted({us.funcs[u]['unit'] for u in bl}, key=lambda x: x)
            fh.write(f'0x{a:08X}\t0x{z:08X}\t0x{m.u2j[a]:08X}\t0x{m.u2j[z]:08X}\t{len(bl)}\t0x{ub:X}\t'
                     f'{",".join(un) if len(un) <= 4 else un[0] + ".." + un[-1]}\n')

    with open(f'{out}/link_order.tsv', 'w') as fh:
        fh.write('# The JP link order in terms of USA code: blocks with >= 2 matched functions sorted by JP address.\n')
        fh.write('# jp_first\tjp_last\tusa_first\tusa_last\tmatched_funcs\tusa_units\n')
        for bl in sorted((b for b in blocks if len(b) >= 2), key=lambda b: m.u2j[b[0]]):
            un = sorted({us.funcs[u]['unit'] for u in bl})
            fh.write(f'0x{m.u2j[bl[0]]:08X}\t0x{m.u2j[bl[-1]]:08X}\t0x{bl[0]:08X}\t0x{bl[-1]:08X}\t{len(bl)}\t'
                     f'{",".join(un) if len(un) <= 3 else un[0] + ".." + un[-1]}\n')

    # address maps
    for kind, lo, hi, name in (('RAM', 0x02000000, 0x04000000, 'ram_map'), ('ROMD', BASE, 0x0A000000, 'romdata_map')):
        with open(f'{out}/{name}.tsv', 'w') as fh:
            fh.write('# USA address -> JP address seen in aligned literal pools of matched function pairs.\n'
                     '# strong = from identical/same-shape pairs (exact alignment); weak = from changed pairs, where\n'
                     '# JP often loads a struct base that USA reaches through an absolute alias (many USA -> one JP).\n')
            fh.write('# usa_addr\tjp_addr\tdelta\tevidence\tvotes\tother_jp_values\n')
            pairs = []
            for k in sorted(m.addr_pairs):
                if not (lo <= k < hi):
                    continue
                st = m.addr_strong.get(k)
                c = st or m.addr_pairs[k]
                (v, n), = c.most_common(1)
                others = ','.join(f'0x{x:08X}:{y}' for x, y in m.addr_pairs[k].most_common() if x != v)
                fh.write(f'0x{k:08X}\t0x{v:08X}\t{v - k:+#x}\t{"strong" if st else "weak"}\t{n}\t{others}\n')
                if st and n * 2 > sum(st.values()):
                    pairs.append((k, v))
            fh.write('\n# strong pairs only: runs with a constant delta (usa_lo\tusa_hi\tdelta\tpairs)\n')
            for a, b, d, n in addr_ranges(pairs):
                fh.write(f'# 0x{a:08X}\t0x{b:08X}\t{d:+#x}\t{n}\n')
    return stat, blocks


def summary(m, stat, blocks, out=OUT):
    us, jp = m.us, m.jp
    L = []
    tot_b = sum(f['size'] for f in us.funcs.values())
    L.append(f'USA functions: {len(us.funcs)} (0x{tot_b:X} bytes); JP functions: {len(jp.funcs)} '
             f'(0x{sum(f["size"] for f in jp.funcs.values()):X} bytes, code 0x{jp.code_start:08X}-0x{jp.code_end:08X})')
    missing = sorted(jp.table_seeds - set(jp.funcs))
    L.append(f'JP data words pointing at Thumb push starts (pointer tables): {len(jp.table_seeds)}, '
             f'not already found as functions: {len(missing)}; analyzer messages: {len(jp.errors)}')
    L.append('')
    L.append('status                              funcs   USA bytes   share')
    for s in STATUS_ORDER:
        fs = [u for u in us.order if stat[u][0] == s]
        b = sum(us.funcs[u]['size'] for u in fs)
        L.append(f'{s:34s} {len(fs):6d}   0x{b:06X}   {100 * b / tot_b:5.1f}%')
        if s == 'same-shape-different-constants':
            for d in ('offsets-only', 'immediates'):
                fd = [u for u in fs if stat[u][1] == d]
                bd = sum(us.funcs[u]['size'] for u in fd)
                L.append(f'  {d:32s} {len(fd):6d}   0x{bd:06X}   {100 * bd / tot_b:5.1f}%')
    conf = collections.Counter(confidence(m, u) for u in us.order if stat[u][0] == 'changed')
    L.append(f'changed pairs by confidence: {dict(conf)}')
    sims = [stat[u][2] for u in us.order if stat[u][0] == 'changed']
    for lo in (0.9, 0.75, 0.5):
        L.append(f'  changed with similarity >= {lo}: {sum(1 for x in sims if x >= lo)}')
    jo = [j for j in jp.order if j not in m.j2u]
    L.append(f'JP-only functions: {len(jo)} (0x{sum(jp.funcs[j]["size"] for j in jo):X} bytes)')
    lo, hi = JP_MOBILE_LIB
    jm = [j for j in jo if lo <= j < hi]
    L.append(f'  of which in the Mobile Adapter GB library 0x{lo:08X}-0x{hi:08X}: {len(jm)} '
             f'(0x{sum(jp.funcs[j]["size"] for j in jm):X} bytes); '
             f'matched functions in that range: {sum(1 for j in m.j2u if lo <= j < hi)}')
    cc = sum(1 for u in m.u2j if m.call_consistent(u))
    L.append(f'pairs whose aligned calls/function pointers all agree with the map: {cc}/{len(m.u2j)}')
    L.append(f'matched by: {dict(collections.Counter(m.how.values()))}')
    L.append('')
    big = [b for b in blocks if len(b) >= 3]
    L.append(f'link-order blocks: {len(blocks)} ({len(big)} with >= 3 matched functions, covering '
             f'{sum(len(b) for b in big)} pairs)')
    mono = sum(1 for a, b in zip(blocks, blocks[1:]) if m.u2j[b[0]] > m.u2j[a[-1]])
    L.append(f'  consecutive blocks that are also ascending in JP: {mono}/{max(1, len(blocks) - 1)}')
    L.append('')
    for kind, lo, hi in (('EWRAM', 0x02000000, 0x03000000), ('IWRAM', 0x03000000, 0x04000000),
                         ('ROM data', BASE, 0x0A000000)):
        ks = [k for k in m.amap if lo <= k < hi]
        ss = [k for k in ks if k in m.addr_strong]
        same = sum(1 for k in ss if m.amap[k] == k)
        L.append(f'{kind}: {len(ks)} USA addresses paired ({len(ss)} strong: {same} unchanged, '
                 f'{len(addr_ranges([(k, m.amap[k]) for k in ss]))} constant-delta runs)')
    L.append('')
    L.append('library symbols in JP: ' + ', '.join(f'{n}@0x{a:08X}' for a, n in sorted(jp.lib.items())
                                                   if not n.startswith('_call_via')))
    un, jn = set(us.lib.values()), set(jp.lib.values())
    L.append(f'library functions only in JP: {sorted(jn - un)}; only in USA: {sorted(un - jn)}; '
             f'JP calls into the library area not identified: '
             f'{[hex(a) for a in sorted(jp.lib_calls) if a not in jp.lib]}')
    text = '\n'.join(L) + '\n'
    open(f'{out}/summary.txt', 'w').write(text)
    return text


def match(log=print):
    us_rom = open(US_PATH, 'rb').read()
    jp_rom = open(JP_PATH, 'rb').read()
    if hashlib.sha1(jp_rom).hexdigest() != JP_SHA1:
        sys.exit(f'{JP_PATH}: unexpected SHA-1 (want {JP_SHA1})')
    us = load_us(us_rom)
    log(f'USA: analyzer reproduces config/functions.tsv: {us.analyzer_check}')
    jp = load_jp(us_rom, jp_rom)
    log(f'JP: {len(jp.funcs)} functions')
    m = Matcher(us, jp)
    m.run(log=log)
    return m


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--compile-test', action='store_true', help='recompile matched USA C at JP addresses')
    ap.add_argument('--diff', metavar='FUNC', help='write build/jp/<FUNC>_listing.txt (aligned USA/JP listing)')
    ap.add_argument('-n', type=int, default=10, help='functions for --compile-test')
    ap.add_argument('--assets', action='store_true', help='compare the asset ranges (build/jp/assets.tsv)')
    args = ap.parse_args()
    m = match(log=lambda s: print(s, file=sys.stderr))
    stat, blocks = write_reports(m)
    print(summary(m, stat, blocks))
    if args.diff:
        write_listing(m, args.diff)
    if args.compile_test:
        compile_test(m, args.n)
    if args.assets:
        rows = compare_assets(m.us.rom, m.jp.rom)
        c = collections.Counter(r[4] for r in rows)
        b = collections.Counter()
        for r in rows:
            b[r[4]] += r[1] - r[0]
        print('assets: ' + ', '.join(f'{k} {c[k]} (0x{b[k]:X} bytes)' for k in c))


# ------------------------------------------------------------------------------------------------
# Data / assets
# ------------------------------------------------------------------------------------------------
def compare_assets(us_rom, jp_rom, out=OUT, chunk=64, samples=48):
    """Where each USA asset range (config/assets.tsv) is in JP: whole range found byte-identical, or the
    share of sampled non-zero 64-byte chunks found anywhere in JP (and their most common delta)."""
    rows = []
    for line in open('config/assets.tsv'):
        p = line.split()
        if not p or line.startswith('#'):
            continue
        lo, hi, typ, path = int(p[0], 16), int(p[1], 16), p[2], p[3]
        data = us_rom[lo - BASE:hi - BASE]
        if typ == 'zero' or not data.strip(b'\0'):
            rows.append((lo, hi, typ, path, 'zero-fill', '', '', ''))
            continue
        k = jp_rom.find(data)
        if k >= 0:
            rows.append((lo, hi, typ, path, 'identical', f'0x{BASE + k:08X}', '1.00', f'{BASE + k - lo:+#x}'))
            continue
        offs = [o for o in range(0, len(data) - chunk + 1, chunk) if data[o:o + chunk].strip(b'\0')]
        if len(offs) > samples:
            step = len(offs) / samples
            offs = [offs[int(i * step)] for i in range(samples)]
        deltas = collections.Counter()
        found = 0
        for o in offs:
            q = jp_rom.find(data[o:o + chunk])
            if q >= 0:
                found += 1
                deltas[BASE + q - (lo + o)] += 1
        frac = found / max(1, len(offs))
        d, nd = deltas.most_common(1)[0] if deltas else (0, 0)
        st = 'mostly-shared' if frac >= 0.8 else 'partly-shared' if frac >= 0.2 else 'not-found'
        rows.append((lo, hi, typ, path, st, f'0x{lo + d:08X}' if nd else '', f'{frac:.2f}',
                     f'{d:+#x} ({nd}/{found})' if nd else ''))
    with open(f'{out}/assets.tsv', 'w') as fh:
        fh.write('# USA asset range -> JP. identical = whole range found byte-for-byte; otherwise share of sampled\n'
                 '# non-zero 64-byte chunks found anywhere in the JP image and their most common address delta.\n')
        fh.write('# usa_start\tusa_end\ttype\tpath\tstatus\tjp_start\tshared\tdelta\n')
        for r in rows:
            fh.write(f'0x{r[0]:08X}\t0x{r[1]:08X}\t' + '\t'.join(r[2:]) + '\n')
    return rows


# ------------------------------------------------------------------------------------------------
# Compiler test: USA C recompiled and linked at the JP address
# ------------------------------------------------------------------------------------------------
AGBCC = '/opt/agbcc'
DEFAULT_CC = ('old_agbcc', '-mthumb-interwork -Wimplicit -Wparentheses -O2 -fhex-asm')


def _run(cmd, **kw):
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


def unit_compiler(unit):
    for line in open('config/cflags.txt'):
        p = line.split()
        if p and not line.startswith('#') and p[0] == unit:
            return p[1], ' '.join(p[2:])
    return DEFAULT_CC


def c_functions():
    """USA functions that are matched C (defined in src/<unit>.c and not INCLUDE_ASM)."""
    out = set()
    for fn in os.listdir('src'):
        if not fn.endswith('.c'):
            continue
        text = open(f'src/{fn}', errors='replace').read()
        asm = set(re.findall(r'^\s*INCLUDE_ASM\([^,]*,\s*(\w+)\)', text, re.M))
        for name in re.findall(r'^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{', text, re.M):
            if name not in asm:
                out.add(name)
    return out


def compile_unit(unit, tmp):
    cc, flags = unit_compiler(unit)
    i, s = f'{tmp}/{unit.replace("/", "_")}.i', f'{tmp}/{unit.replace("/", "_")}.s'
    r = _run(['cpp', '-nostdinc', '-undef', '-I', 'include', '-I', f'{AGBCC}/include', '-iquote', '.',
              f'src/{unit}.c', '-o', i])
    if r.returncode:
        raise RuntimeError(r.stderr)
    r = _run([f'{AGBCC}/bin/{cc}'] + flags.split() + [i, '-o', s])
    if r.returncode:
        raise RuntimeError(r.stderr)
    return open(s).read()


def extract_function(asm_text, name):
    """One function of agbcc output, with the file header lines it needs."""
    lines = asm_text.split('\n')
    start = end = None
    for k, l in enumerate(lines):
        if start is None and re.match(rf'\s*\.globl\s+{name}\s*$', l):
            start = k
            while start > 0 and lines[start - 1].strip().startswith('.align'):
                start -= 1
        elif start is not None and re.match(rf'\s*\.size\s+{name}\s*,', l):
            end = k + 1
            break
    if start is None or end is None:
        raise KeyError(f'{name} not found in compiler output')
    head = ['\t.include "asm/macros.inc"', '\t.code\t16', '.text']
    # the Makefile appends `.align 2, 0` to every compiled unit: zero padding, not Thumb nops
    return '\n'.join(head + lines[start:end] + ['\t.align 2, 0']) + '\n'


def decode_bl(rom, a):
    h1, h2 = u16(rom, a), u16(rom, a + 2)
    off = ((h1 & 0x7FF) << 12) | ((h2 & 0x7FF) << 1)
    if off & 0x400000:
        off -= 0x800000
    return a + 4 + off


def compile_one(m, u, asm_text, tmp, syms):
    import target
    us, jp = m.us, m.jp
    f = us.funcs[u]
    j = m.u2j[u]
    name = f['name']
    src = f'{tmp}/{name}.s'
    open(src, 'w').write(extract_function(asm_text, name))
    obj = f'{tmp}/{name}.o'
    r = _run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-mthumb-interwork', '-I', '.', '-o', obj, src])
    if r.returncode:
        raise RuntimeError(r.stderr)
    raw = _run(['arm-none-eabi-objcopy', '-O', 'binary', '-j', '.text', obj, f'{tmp}/{name}.raw'])
    body = open(f'{tmp}/{name}.raw', 'rb').read()
    relocs = []
    for line in _run(['arm-none-eabi-objdump', '-r', '-j', '.text', obj]).stdout.splitlines():
        p = line.split()
        if len(p) == 3 and re.match(r'^[0-9a-f]{8}$', p[0]):
            relocs.append((int(p[0], 16), p[1], p[2]))
    defs, notes, agree, checked = {}, [], 0, 0
    for off, typ, sym in relocs:
        if sym.startswith('.'):
            continue                       # section-relative (jump tables): placed by the link
        usv = target.resolve_symbol(sym, syms)
        if typ in ('R_ARM_THM_CALL', 'R_ARM_THM_PC22'):
            jt = decode_bl(jp.rom, j + off)
            defs[sym] = (jt, True)
            want = m.u2j.get(usv[0]) if usv else None
            if usv and usv[0] in us.lib:
                want = m.jp_lib_by_name.get(us.lib[usv[0]])
            checked += 1
            agree += want == jt
            if want != jt:
                notes.append(f'call {sym}: JP bytes go to 0x{jt:08X}, map says {want and hex(want)}')
        elif typ == 'R_ARM_ABS32':
            addend = struct.unpack_from('<I', body, off)[0]
            jv = u32(jp.rom, j + off)
            uv = u32(us.rom, u + off)
            if usv and (usv[0] + addend) & 0xFFFFFFFF != uv:
                notes.append(f'{sym}: USA word 0x{uv:08X} != symbol+addend')
            defs[sym] = ((jv - addend) & 0xFFFFFFFF, bool(usv and usv[1]))
            checked += 1
            mapped = m.amap.get(uv)
            if mapped is None and (uv & 1) and (uv - 1) in m.u2j:
                mapped = m.u2j[uv - 1] + 1
            agree += mapped == jv
        else:
            notes.append(f'unhandled relocation {typ} {sym}')
    stub = ['\t.text']
    for s_, (v, thumb) in sorted(defs.items()):
        stub += [f'\t.global {s_}', f'\t.thumb_set {s_}, 0x{v & ~1:08X}' if thumb and (v & 1 or v % 2 == 0) and
                 s_.startswith('sub_') else f'\t.set {s_}, 0x{v:08X}']
    open(f'{tmp}/{name}_syms.s', 'w').write('\n'.join(stub) + '\n')
    r = _run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-o', f'{tmp}/{name}_syms.o', f'{tmp}/{name}_syms.s'])
    if r.returncode:
        raise RuntimeError(r.stderr)
    open(f'{tmp}/{name}.ld', 'w').write(f'SECTIONS {{ . = 0x{j:08X}; .text : {{ {obj}(.text) }} }}\n')
    r = _run(['arm-none-eabi-ld', '-T', f'{tmp}/{name}.ld', '-o', f'{tmp}/{name}.elf', obj, f'{tmp}/{name}_syms.o'])
    if r.returncode:
        raise RuntimeError(r.stderr)
    _run(['arm-none-eabi-objcopy', '-O', 'binary', '-j', '.text', f'{tmp}/{name}.elf', f'{tmp}/{name}.bin'])
    got = open(f'{tmp}/{name}.bin', 'rb').read()
    want = jp.rom[j - BASE:j - BASE + len(got)]
    usa_ok = us.rom[u - BASE:u - BASE + len(body)]
    return dict(name=name, usa=u, jp=j, size=len(got), equal=got == want,
                first_diff=next((k for k in range(len(got)) if got[k] != want[k]), None),
                relocs=len(relocs), checked=checked, agree=agree, notes=notes,
                moved=sum(1 for o, t, s_ in relocs if t == 'R_ARM_ABS32' and
                          u32(jp.rom, j + o) != u32(us.rom, u + o)))


def compile_test(m, n=10, out=OUT):
    import target
    syms = target.known_symbols()
    cfun = c_functions()
    cands = []
    for u, j in m.u2j.items():
        f = m.us.funcs[u]
        if f['name'] not in cfun or m.status(u)[0] != 'identical-mod-relocation' or f['size'] < 0x30:
            continue
        t = m.tu[u]
        ndata = sum(1 for p in t.pay if p and p[0] == 'lit' and p[2] in ('RAM', 'ROMD', 'CODE'))
        if ndata + t.nbl == 0:
            continue
        cands.append((f['size'], u))
    cands.sort(reverse=True)
    picked, seen_units = [], set()
    for size, u in cands:                 # largest first, one per unit
        unit = m.us.funcs[u]['unit']
        if unit in seen_units:
            continue
        picked.append(u)
        seen_units.add(unit)
        if len(picked) == n:
            break
    tmp = f'{out}/cc'
    os.makedirs(tmp, exist_ok=True)
    cache = {}
    res = []
    for u in picked:
        unit = m.us.funcs[u]['unit']
        if unit not in cache:
            cache[unit] = compile_unit(unit, tmp)
        try:
            res.append(compile_one(m, u, cache[unit], tmp, syms))
        except Exception as e:  # noqa: BLE001
            res.append(dict(name=m.us.funcs[u]['name'], usa=u, jp=m.u2j[u], size=0, equal=False,
                            first_diff=None, relocs=0, checked=0, agree=0, notes=[str(e)[:200]], moved=0))
    with open(f'{out}/compile_test.txt', 'w') as fh:
        fh.write(f'USA C recompiled with the USA compiler/flags (config/cflags.txt) and linked at the JP address;\n'
                 f'relocated symbols take their JP values. {sum(r["equal"] for r in res)}/{len(res)} byte-identical.\n\n')
        fh.write('function\tusa\tjp\tbytes\tequal\trelocs\tmoved_words\trelocs_agreeing_with_map\tnotes\n')
        for r in res:
            fh.write(f'{r["name"]}\t0x{r["usa"]:08X}\t0x{r["jp"]:08X}\t0x{r["size"]:X}\t{r["equal"]}\t{r["relocs"]}\t'
                     f'{r["moved"]}\t{r["agree"]}/{r["checked"]}\t{"; ".join(r["notes"])}\n')
    print(open(f'{out}/compile_test.txt').read())
    return res


# ------------------------------------------------------------------------------------------------
# Aligned listings and switch dispatch maps
# ------------------------------------------------------------------------------------------------
def render(img, lo, hi):
    """[(addr, text)] for [lo, hi) using the analyzer's classification."""
    disasm.rom = img.rom
    out = []
    a = lo
    while a < hi:
        if a in img.lits and a % 4 == 0:
            out.append((a, f'.4byte 0x{img.lits[a]:08X}'))
            a += 4
        elif a in img.jtabs:
            out.append((a, f'.4byte 0x{img.jtabs[a]:08X} @ case'))
            a += 4
        elif a in img.insns:
            i = img.insns[a]
            if i.kind == 'ldrpc':
                out.append((a, f'ldr {disasm.REGS[i.reg]}, =0x{u32(img.rom, i.lit):08X}'))
            elif i.kind in ('b', 'bc'):
                out.append((a, f'{i.text} +0x{i.target - lo:X}' if lo <= i.target < hi else f'{i.text} 0x{i.target:08X}'))
            elif i.kind == 'bl':
                inside = lo <= i.target < hi
                out.append((a, f'bl +0x{i.target - lo:X} @ far' if inside else f'bl 0x{i.target:08X}'))
            else:
                out.append((a, i.text))
            a += i.size
        else:
            a += 2
    return out


_NORM = [(re.compile(r'\b(r\d+|sl|fp|ip|sb)\b'), 'R'), (re.compile(r'#-?(0x[0-9A-Fa-f]+|\d+)'), '#N'),
         (re.compile(r'0x[0-9A-Fa-f]+'), 'N')]


def _norm(t):
    for rx, rep in _NORM:
        t = rx.sub(rep, t)
    return t


def side_by_side(us, ulo, uhi, jp, jlo, jhi, width=46):
    """Two-column listing aligned on normalised instructions. Marks: ' ' same text, '~' same shape
    (registers/constants differ), '|' replaced, '<' USA only, '>' JP only. Addresses are offsets."""
    lu, lj = render(us, ulo, uhi), render(jp, jlo, jhi)
    sm = difflib.SequenceMatcher(None, [_norm(t) for _, t in lu], [_norm(t) for _, t in lj], autojunk=False)
    rows = []
    for op, a1, a2, b1, b2 in sm.get_opcodes():
        for k in range(max(a2 - a1, b2 - b1)):
            x = lu[a1 + k] if a1 + k < a2 else None
            y = lj[b1 + k] if b1 + k < b2 else None
            mark = {'equal': ' ', 'replace': '|', 'delete': '<', 'insert': '>'}[op]
            if op == 'equal' and x[1] != y[1]:
                mark = '~'
            left = f'+{x[0] - ulo:04X} {x[1]}' if x else ''
            right = f'+{y[0] - jlo:04X} {y[1]}' if y else ''
            rows.append(f'{left:{width}s} {mark} {right}')
    return rows, sm.ratio()


def dispatch_map(rom, start, end, nargs=3, numreg=1, values=range(0x10000)):
    """Emulate a switch's compare tree (agbcc binary search, far-jump `bl`s included) for every value of
    the switch argument (passed in r<numreg>). Returns {(body_addr, how): [values]}; the largest group is
    the default. `how` is 'eq' (entered after an equality test) or 'fall' (first non-tree instruction)."""
    M = 0xFFFFFFFF
    res = collections.defaultdict(list)
    disasm.rom = rom
    cache = {}

    def dec(a):
        if a not in cache:
            cache[a] = disasm.decode_thumb(a)
        return cache[a]

    for v in values:
        R = [0] * 16
        R[numreg] = v
        flags = None
        pc = start
        body = None
        for _ in range(500):
            i = dec(pc)
            h = u16(rom, pc)
            top5 = h >> 11
            if i.kind == 'bc':
                N, Z, C, V = flags
                c = (h >> 8) & 15
                taken = [Z, not Z, C, not C, N, not N, V, not V, C and not Z, (not C) or Z, N == V, N != V,
                         (not Z) and N == V, Z or N != V][c]
                if c == 1 and not taken:
                    nx = dec(pc + 2)
                    body = (nx.target if nx.kind in ('b', 'bl') else pc + 2, 'eq')
                    break
                if c == 0 and taken:
                    body = (i.target, 'eq')
                    break
                pc = i.target if taken else pc + 2
                continue
            if i.kind == 'b' or (i.kind == 'bl' and start <= i.target < end):
                pc = i.target
                continue
            if i.kind == 'ldrpc':
                R[i.reg] = u32(rom, i.lit)
            elif top5 <= 2:
                rd, rs, imm = h & 7, (h >> 3) & 7, (h >> 6) & 31
                x = R[rs]
                R[rd] = (x << imm) & M if top5 == 0 else x >> (imm or 32) if top5 == 1 else \
                    ((x - (1 << 32) if x >> 31 else x) >> (imm or 32)) & M
            elif top5 == 3:
                rd, rs, x = h & 7, (h >> 3) & 7, (h >> 6) & 7
                o = x if (h >> 10) & 1 else R[x]
                R[rd] = (R[rs] - o) & M if (h >> 9) & 1 else (R[rs] + o) & M
            elif 4 <= top5 <= 7:
                rd, imm = (h >> 8) & 7, h & 0xFF
                if top5 == 4:
                    R[rd] = imm
                elif top5 == 5:
                    a, b = R[rd], imm
                    r = (a - b) & M
                    flags = (bool(r >> 31), r == 0, a >= b, ((a ^ b) & (a ^ r)) >> 31 == 1)
                elif top5 == 6:
                    R[rd] = (R[rd] + imm) & M
                else:
                    R[rd] = (R[rd] - imm) & M
            elif (h >> 10) == 0x10 and ((h >> 6) & 15) == 10:
                a, b = R[h & 7], R[(h >> 3) & 7]
                r = (a - b) & M
                flags = (bool(r >> 31), r == 0, a >= b, ((a ^ b) & (a ^ r)) >> 31 == 1)
            elif (h >> 10) == 0x11 and ((h >> 8) & 3) in (1, 2):
                rd, rs = (h & 7) | ((h >> 4) & 8), (h >> 3) & 15
                if (h >> 8) & 3 == 2:
                    R[rd] = R[rs]
                else:
                    a, b = R[rd], R[rs]
                    r = (a - b) & M
                    flags = (bool(r >> 31), r == 0, a >= b, ((a ^ b) & (a ^ r)) >> 31 == 1)
            elif flags is None and i.kind in ('op', 'raw'):
                if (h >> 13) == 3 or (h >> 12) in (5, 8, 9) and (h >> 11) & 1:
                    R[h & 7] = 0       # prologue load (locals start at 0)
            else:
                body = (pc, 'fall')
                break
            pc += i.size
        res[body or (pc, 'limit')].append(v)
    return res


def case_table(rom, start, size, **kw):
    """[(body_addr, how, [values])] in body (= source) order, plus the default body address."""
    res = dispatch_map(rom, start, start + size, **kw)
    default = max(res.items(), key=lambda kv: len(kv[1]))[0]
    rows = [(b, how, vs) for (b, how), vs in sorted(res.items()) if (b, how) != default]
    return rows, default[0]


def write_listing(m, name, out=OUT):
    us, jp = m.us, m.jp
    u = next(a for a, f in us.funcs.items() if f['name'] == name)
    j = m.u2j.get(u)
    if j is None:
        sys.exit(f'{name}: no JP counterpart in the map')
    fu, fj = us.funcs[u], jp.funcs[j]
    rows, r = side_by_side(us, u, u + fu['size'], jp, j, j + fj['size'])
    path = f'{out}/{name}_listing.txt'
    with open(path, 'w') as fh:
        fh.write(f'{name}: USA 0x{u:08X} (0x{fu["size"]:X} bytes) vs JP 0x{j:08X} (0x{fj["size"]:X} bytes); '
                 f'status {m.status(u)[0]}, format similarity {m.ratio(u, j):.3f}, aligned-text ratio {r:.3f}\n')
        fh.write("marks: ' ' same, '~' same shape, '|' replaced, '<' USA only, '>' JP only; +offsets from the start\n\n")
        fh.write('\n'.join(rows) + '\n')
        cu, du = case_table(us.rom, u, fu['size'])
        cj, dj = case_table(jp.rom, j, fj['size'])
        if len(cu) >= 3 and len(cj) >= 3:
            fh.write('\n\nSwitch dispatch (every u16 value of r1 run through the compare tree), bodies in ROM order:\n')
            for tag, rows_, a, d in (('USA', cu, u, du), ('JP', cj, j, dj)):
                fh.write(f'{tag}: {len(rows_)} non-default bodies, {sum(len(v) for _, _, v in rows_)} values; '
                         f'default +0x{d - a:X}\n')
                for b, how, vs in rows_:
                    fh.write(f'  +0x{b - a:04X} {how:4s} {" ".join(f"0x{v:03X}" for v in vs)}\n')
    print(f'wrote {path}')
    return path


if __name__ == '__main__':
    main()
