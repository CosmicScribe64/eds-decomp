#!/usr/bin/env python3
"""EDS disassembler.

Discovers functions in the baserom by recursive descent and emits pret-style
assembly (agbcc "divided" Thumb syntax) that reassembles to identical bytes.

Outputs (paths relative to repo root):
  asm/crt0.s                      header + ARM startup code
  asm/nonmatching/<unit>/<fn>.s   one file per function
  asm/<unit>.s                    unit file that .includes its functions in order
  asm/veneer.s                    linker-style thumb->ARM veneer at end of .text
  data/<unit>.s                   .rodata: .incbin chunks split at referenced labels
  units.txt                       link order of units
  config/functions.tsv            discovered function list (addr, mode, size, name)

Run inside the toolchain container:  tools/dr python3 tools/disasm.py
"""
import os
import struct
import sys
from collections import defaultdict

import capstone

ROM_PATH = 'baserom.gba'
BASE = 0x08000000
CRT0_END = 0x08000228       # header + crt0 + intr dispatcher + its pool
CODE_START = 0x08000228
CODE_END = 0x0807EE98       # libgcc (linked from agbcc's libgcc.a) starts here
LIB_END = 0x08080A18        # end of libc; veneer follows
VENEER = 0x08080A18         # linker thumb->ARM veneer (8 bytes)
TEXT_END = 0x08080A20       # .rodata starts here
ROM_END = 0x08800000

# Known entry points (thumb unless noted).
SEEDS_THUMB = [0x08075F64]  # AgbMain
ARM_REGIONS = [(0x0807EAD0, 0x0807ECF8)]  # sound mixer (+ IWRAM-copied routine)
ARM_FUNCS = [0x0807EAD0, 0x0807EAF0, 0x0807EC1C]

# Library functions (resolved by the linker from libgcc.a / libc.a).
LIB_SYMS = {
    0x0807EE98: '_call_via_r0', 0x0807EE9C: '_call_via_r1', 0x0807EEA0: '_call_via_r2',
    0x0807EEA4: '_call_via_r3', 0x0807EEA8: '_call_via_r4', 0x0807EEAC: '_call_via_r5',
    0x0807EEB0: '_call_via_r6', 0x0807EEB4: '_call_via_r7', 0x0807EEB8: '_call_via_r8',
    0x0807EEBC: '_call_via_r9', 0x0807EEC0: '_call_via_sl', 0x0807EEC4: '_call_via_fp',
    0x0807EEC8: '_call_via_ip', 0x0807EECC: '_call_via_sp', 0x0807EED0: '_call_via_lr',
    0x0807EED4: '__divsi3', 0x0807EF6C: '__modsi3', 0x0807F03C: '__muldi3',
    0x0807F0AC: '__udivsi3', 0x0807F124: '__umodsi3',
    0x08080918: 'memcpy', 0x08080978: 'memset', 0x080809CC: 'strcpy',
    0x080808CC: '__lshrdi3', 0x08080900: '__negdi2',
}
# soft-float (dp-bit.o at 0x0807F1E4, fp-bit.o at 0x0807FF80): name -> offset
_DPBIT = dict(__pack_d=0x0, __unpack_d=0x148, __adddf3=0x48C, __subdf3=0x4BC, __muldf3=0x4F4,
              __divdf3=0x79C, __fpcmp_parts_d=0x924, __cmpdf2=0xA24, __eqdf2=0xA50, __nedf2=0xA9C,
              __gtdf2=0xAE8, __gedf2=0xB34, __ltdf2=0xB80, __ledf2=0xBCC, __floatsidf=0xC18,
              __fixdfsi=0xC94, __negdf2=0xD08, __make_dp=0xD30, __truncdfsf2=0xD58)
_FPBIT = dict(__pack_f=0x0, __unpack_f=0xB8, __addsf3=0x2B0, __subsf3=0x2DC, __mulsf3=0x310,
              __divsf3=0x474, __fpcmp_parts_f=0x560, __cmpsf2=0x644, __eqsf2=0x66C, __nesf2=0x6B4,
              __gtsf2=0x6FC, __gesf2=0x744, __ltsf2=0x78C, __lesf2=0x7D4, __floatsisf=0x81C,
              __fixsfsi=0x87C, __negsf2=0x8E4, __make_fp=0x908, __extendsfdf2=0x920)
LIB_SYMS.update({0x0807F1E4 + o: n for n, o in _DPBIT.items()})
LIB_SYMS.update({0x0807FF80 + o: n for n, o in _FPBIT.items()})

# Hand-assigned names (address -> name).
NAMES = {
    0x08075F64: 'AgbMain',
    # libagbsyscall (BIOS SWI stubs)
    0x0807ECF8: 'CpuFastSet', 0x0807ECFC: 'CpuSet', 0x0807ED00: 'Div',
    # AgbSram v1.12
    0x0807ED04: 'ReadSram_Core', 0x0807ED28: 'ReadSram', 0x0807ED8C: 'WriteSram',
    0x0807EDCC: 'VerifySram_Core', 0x0807EDFC: 'VerifySram', 0x0807EE60: 'WriteSramEx',
}

UNIT_BYTES = 0x1000         # target size of a code unit (split only between functions)

rom = open(ROM_PATH, 'rb').read()


def u16(a):
    return struct.unpack_from('<H', rom, a - BASE)[0]


def u32(a):
    return struct.unpack_from('<I', rom, a - BASE)[0]


def in_code(a):
    return CODE_START <= a < CODE_END


def is_data_addr(a):
    return TEXT_END <= a < ROM_END


REGS = ['r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'r6', 'r7', 'r8', 'r9', 'sl', 'fp', 'ip', 'sp', 'lr', 'pc']
CONDS = ['eq', 'ne', 'cs', 'cc', 'mi', 'pl', 'vs', 'vc', 'hi', 'ls', 'ge', 'lt', 'gt', 'le']


def rlist(bits, extra=None):
    regs = [REGS[i] for i in range(8) if bits & (1 << i)]
    if extra:
        regs.append(extra)
    return '{' + ', '.join(regs) + '}'


def hx(v):
    return f'#0x{v:X}' if v > 9 else f'#{v}'


class Insn:
    __slots__ = ('addr', 'size', 'kind', 'text', 'target', 'lit', 'reg')

    def __init__(self, addr, size, kind, text, target=None, lit=None, reg=None):
        self.addr, self.size, self.kind, self.text = addr, size, kind, text
        self.target, self.lit, self.reg = target, lit, reg


def decode_thumb(a):
    """Decode one Thumb instruction in agbcc/GAS divided syntax.
    kind: 'op' normal, 'b' unconditional branch, 'bc' conditional branch, 'bl' call,
          'ret' (bx/pop pc), 'movpc' (mov pc, rX), 'ldrpc' literal load,
          'raw' (emit as .2byte), 'bad' undefined."""
    h = u16(a)
    top5 = h >> 11
    if top5 <= 2:  # shift imm
        op = ['lsl', 'lsr', 'asr'][top5]
        imm = (h >> 6) & 31
        rs, rd = (h >> 3) & 7, h & 7
        if imm == 0 and op != 'lsl':
            return Insn(a, 2, 'raw', f'.2byte 0x{h:04X}')
        return Insn(a, 2, 'op', f'{op} {REGS[rd]}, {REGS[rs]}, {hx(imm)}')
    if top5 == 3:  # add/sub
        i = (h >> 10) & 1
        op = 'sub' if (h >> 9) & 1 else 'add'
        rn, rs, rd = (h >> 6) & 7, (h >> 3) & 7, h & 7
        if i:
            return Insn(a, 2, 'op', f'{op} {REGS[rd]}, {REGS[rs]}, {hx(rn)}')
        return Insn(a, 2, 'op', f'{op} {REGS[rd]}, {REGS[rs]}, {REGS[rn]}')
    if 4 <= top5 <= 7:  # mov/cmp/add/sub imm8
        op = ['mov', 'cmp', 'add', 'sub'][top5 - 4]
        rd, imm = (h >> 8) & 7, h & 0xFF
        return Insn(a, 2, 'op', f'{op} {REGS[rd]}, {hx(imm)}')
    if (h >> 10) == 0x10:  # ALU
        op = ['and', 'eor', 'lsl', 'lsr', 'asr', 'adc', 'sbc', 'ror',
              'tst', 'neg', 'cmp', 'cmn', 'orr', 'mul', 'bic', 'mvn'][(h >> 6) & 15]
        rs, rd = (h >> 3) & 7, h & 7
        return Insn(a, 2, 'op', f'{op} {REGS[rd]}, {REGS[rs]}')
    if (h >> 10) == 0x11:  # hi reg ops / bx
        op = (h >> 8) & 3
        h1, h2 = (h >> 7) & 1, (h >> 6) & 1
        rs = ((h >> 3) & 7) + 8 * h2
        rd = (h & 7) + 8 * h1
        if op == 3:
            if h1 or (h & 7):
                return Insn(a, 2, 'raw', f'.2byte 0x{h:04X}')
            return Insn(a, 2, 'ret', f'bx {REGS[rs]}', reg=rs)
        if not h1 and not h2:
            return Insn(a, 2, 'raw', f'.2byte 0x{h:04X}')
        name = ['add', 'cmp', 'mov'][op]
        if op == 2 and rd == 15:
            return Insn(a, 2, 'movpc', f'mov pc, {REGS[rs]}', reg=rs)
        if op == 0 and rd == 15:
            return Insn(a, 2, 'raw', f'.2byte 0x{h:04X}')
        return Insn(a, 2, 'op', f'{name} {REGS[rd]}, {REGS[rs]}')
    if (h >> 11) == 9:  # ldr pc-relative
        rd, imm = (h >> 8) & 7, (h & 0xFF) * 4
        lit = ((a + 4) & ~3) + imm
        return Insn(a, 2, 'ldrpc', None, lit=lit, reg=rd)
    if (h >> 12) == 5:
        ro, rb, rd = (h >> 6) & 7, (h >> 3) & 7, h & 7
        if (h >> 9) & 1 == 0:
            op = ['str', 'strb', 'ldr', 'ldrb'][(h >> 10) & 3]
        else:
            op = ['strh', 'ldsb', 'ldrh', 'ldsh'][(h >> 10) & 3]
        return Insn(a, 2, 'op', f'{op} {REGS[rd]}, [{REGS[rb]}, {REGS[ro]}]')
    if (h >> 13) == 3:  # ldr/str imm
        b, l = (h >> 12) & 1, (h >> 11) & 1
        imm = (h >> 6) & 31
        rb, rd = (h >> 3) & 7, h & 7
        op = ('ldr' if l else 'str') + ('b' if b else '')
        off = imm if b else imm * 4
        m = f'[{REGS[rb]}]' if off == 0 else f'[{REGS[rb]}, {hx(off)}]'
        return Insn(a, 2, 'op', f'{op} {REGS[rd]}, {m}')
    if (h >> 12) == 8:  # ldrh/strh imm
        l = (h >> 11) & 1
        off = ((h >> 6) & 31) * 2
        rb, rd = (h >> 3) & 7, h & 7
        m = f'[{REGS[rb]}]' if off == 0 else f'[{REGS[rb]}, {hx(off)}]'
        return Insn(a, 2, 'op', f'{"ldrh" if l else "strh"} {REGS[rd]}, {m}')
    if (h >> 12) == 9:  # sp-relative
        l = (h >> 11) & 1
        rd, off = (h >> 8) & 7, (h & 0xFF) * 4
        return Insn(a, 2, 'op', f'{"ldr" if l else "str"} {REGS[rd]}, [sp, {hx(off)}]')
    if (h >> 12) == 10:  # add rd, pc/sp
        sp = (h >> 11) & 1
        rd, off = (h >> 8) & 7, (h & 0xFF) * 4
        if sp:
            return Insn(a, 2, 'op', f'add {REGS[rd]}, sp, {hx(off)}')
        return Insn(a, 2, 'raw', f'.2byte 0x{h:04X} @ add {REGS[rd]}, pc, {hx(off)}')
    if (h >> 8) == 0xB0:  # adjust sp
        off = (h & 0x7F) * 4
        return Insn(a, 2, 'op', f'{"sub" if h & 0x80 else "add"} sp, {hx(off)}')
    if (h >> 12) == 11 and ((h >> 9) & 3) == 2:  # push/pop
        l, r = (h >> 11) & 1, (h >> 8) & 1
        if l:
            if r:
                return Insn(a, 2, 'ret', f'pop {rlist(h & 0xFF, "pc")}')
            return Insn(a, 2, 'op', f'pop {rlist(h & 0xFF)}')
        return Insn(a, 2, 'op', f'push {rlist(h & 0xFF, "lr" if r else None)}')
    if (h >> 12) == 12:  # stmia/ldmia
        l = (h >> 11) & 1
        rb = (h >> 8) & 7
        if (h & 0xFF) == 0:
            return Insn(a, 2, 'raw', f'.2byte 0x{h:04X}')
        return Insn(a, 2, 'op', f'{"ldmia" if l else "stmia"} {REGS[rb]}!, {rlist(h & 0xFF)}')
    if (h >> 12) == 13:  # cond branch / swi
        cond = (h >> 8) & 15
        if cond == 15:
            return Insn(a, 2, 'op', f'swi {hx(h & 0xFF)}')
        if cond == 14:
            return Insn(a, 2, 'bad', f'.2byte 0x{h:04X}')
        off = h & 0xFF
        if off & 0x80:
            off -= 0x100
        return Insn(a, 2, 'bc', f'b{CONDS[cond]}', target=a + 4 + off * 2)
    if (h >> 11) == 0x1C:  # b
        off = h & 0x7FF
        if off & 0x400:
            off -= 0x800
        return Insn(a, 2, 'b', 'b', target=a + 4 + off * 2)
    if (h >> 11) == 0x1E:  # bl prefix
        h2 = u16(a + 2)
        if (h2 >> 11) != 0x1F:
            return Insn(a, 2, 'bad', f'.2byte 0x{h:04X}')
        off = ((h & 0x7FF) << 12) | ((h2 & 0x7FF) << 1)
        if off & 0x400000:
            off -= 0x800000
        return Insn(a, 4, 'bl', 'bl', target=a + 4 + off)
    return Insn(a, 2, 'bad', f'.2byte 0x{h:04X}')


class Func:
    def __init__(self, addr, mode):
        self.addr, self.mode = addr, mode
        self.end = None


class Analyzer:
    def __init__(self):
        self.funcs = {}             # addr -> Func
        self.insns = {}             # addr -> Insn (thumb)
        self.lits = {}              # addr -> value  (4-byte literal pool words)
        self.jtabs = {}             # addr -> target  (jump table entries)
        self.labels = set()         # local branch targets
        self.pending = []
        self.errors = []
        self.calls = defaultdict(list)  # bl target -> [call sites]
        self.ptr_funcs = set()          # targets of odd (thumb) pointers in literal pools
        self.farjumps = set()           # bl targets that are agbcc "far jump" labels
        self.why = {}
        self.entered = set()            # function starts reached by another function's flow
        self.bad = set()                # rejected gap-seeded starts (turned out to be data)
        self.cov = bytearray(CODE_END - CODE_START)
        for lo, hi in ARM_REGIONS:
            self.cov[lo - CODE_START:hi - CODE_START] = b'\x01' * (hi - lo)

    def mark(self, a, n):
        lo = max(a, CODE_START) - CODE_START
        hi = min(a + n, CODE_END) - CODE_START
        if hi > lo:
            self.cov[lo:hi] = b'\x01' * (hi - lo)

    def reset(self):
        far, bad = self.farjumps, self.bad
        self.__init__()
        self.farjumps, self.bad = far, bad

    def add_func(self, addr, mode='t', why='seed'):
        if addr in self.funcs or addr in LIB_SYMS:
            return
        if not in_code(addr) or (addr in self.bad and not why.startswith('bl@')):
            return
        self.why[addr] = why
        self.funcs[addr] = Func(addr, mode)
        if mode == 't':
            self.pending.append(addr)

    # ---------------------------------------------------------------- thumb
    def explore(self, start):
        todo = [start]
        seen = set()
        while todo:
            a = todo.pop()
            while True:
                if a != start and a in self.funcs:
                    self.entered.add(a)
                if a in seen or a in self.insns:
                    break
                if not in_code(a) or any(lo <= a < hi for lo, hi in ARM_REGIONS):
                    self.errors.append(f'flow leaves code at {a:08X} (func {start:08X})')
                    break
                if a in self.lits or a in self.jtabs:
                    self.errors.append(f'flow runs into data at {a:08X} (func {start:08X})')
                    break
                ins = decode_thumb(a)
                if ins.kind == 'bad':
                    self.errors.append(f'bad insn {u16(a):04X} at {a:08X} (func {start:08X})')
                    break
                seen.add(a)
                self.insns[a] = ins
                self.mark(a, ins.size)
                k = ins.kind
                if k == 'ldrpc':
                    self.lits[ins.lit] = u32(ins.lit)
                    self.mark(ins.lit, 4)
                    v = self.lits[ins.lit]
                    if v & 1 and in_code(v - 1) and (v - 1) % 4 == 0 and (v - 1) not in self.bad:
                        self.ptr_funcs.add(v - 1)
                        self.add_func(v - 1, why=f'ptr@{a:08X}')
                elif k == 'bl':
                    if ins.target in self.farjumps:
                        self.labels.add(ins.target)
                        todo.append(ins.target)
                        break
                    if in_code(ins.target) or ins.target in LIB_SYMS or ins.target == VENEER:
                        self.calls[ins.target].append(a)
                        self.add_func(ins.target, why=f'bl@{a:08X}')
                    else:
                        self.errors.append(f'bl to {ins.target:08X} at {a:08X}')
                elif k == 'b':
                    self.labels.add(ins.target)
                    todo.append(ins.target)
                    break
                elif k == 'bc':
                    self.labels.add(ins.target)
                    todo.append(ins.target)
                elif k == 'ret':
                    break
                elif k == 'movpc':
                    self.jump_table(a, start, todo)
                    break
                a += ins.size

    def jump_table(self, a, fstart, todo):
        """agbcc switch: lsl; ldr rT,=table; add; ldr; mov pc, rX."""
        table = None
        count = None
        p = a
        for _ in range(8):
            p -= 2
            ins = self.insns.get(p)
            if ins is None:
                break
            if ins.kind == 'ldrpc' and table is None:
                v = u32(ins.lit)
                if in_code(v) and v > a:
                    table = v
        # bound from an earlier "cmp rN, #imm ; bhi" if present
        p = a
        for _ in range(12):
            p -= 2
            ins = self.insns.get(p)
            if ins is None:
                break
            if ins.kind == 'bc' and ins.text in ('bhi', 'bls'):
                prev = self.insns.get(p - 2)
                if prev and prev.text.startswith('cmp') and '#' in prev.text:
                    count = int(prev.text.split('#')[1], 0) + 1
                break
        if table is None:
            self.errors.append(f'unresolved mov pc at {a:08X} (func {fstart:08X})')
            return
        t = table
        lowest = 1 << 32
        n = 0
        while t < lowest and (count is None or n < count):
            v = u32(t)
            if not in_code(v) or v & 1:
                break
            self.jtabs[t] = v
            self.mark(t, 4)
            self.labels.add(v)
            self.labels.add(t) if n == 0 else None
            if v > table:
                lowest = min(lowest, v)
            todo.append(v)
            t += 4
            n += 1
        if count is not None and n != count:
            self.errors.append(f'jump table at {table:08X}: {n} entries, cmp says {count}')

    def run(self):
        for it in range(8):
            self.reset()
            for s in SEEDS_THUMB:
                self.add_func(s)
            for s in ARM_FUNCS:
                self.funcs[s] = Func(s, 'a')
            while True:
                while self.pending:
                    self.explore(self.pending.pop())
                if not self.fill_gaps():
                    break
            far = self.classify_far() | self.farjumps
            lit_halves = {l + 2 for l in self.lits}
            bad = self.bad | {f for f in self.funcs if f in self.lits or f in self.jtabs or f in lit_halves}
            bad |= {f for f in self.entered if not (f % 4 == 0 and self.is_push_lr(f))
                    and f not in SEEDS_THUMB and f not in ARM_FUNCS}
            print(f'pass {it}: {len(self.funcs)} funcs, {len(far)} far-jump targets, '
                  f'{len(bad)} rejected', file=sys.stderr)
            if far == self.farjumps and bad == self.bad:
                break
            self.farjumps, self.bad = far, bad

    def is_push_lr(self, a):
        return (u16(a) & 0xFF00) == 0xB500

    def reaches_bx_lr(self, a):
        seen = set()
        for _ in range(400):
            if a in seen or not in_code(a):
                return False
            seen.add(a)
            ins = decode_thumb(a)
            if ins.kind == 'ret':
                return ins.text == 'bx lr'
            if ins.kind in ('bad', 'movpc'):
                return False
            if ins.kind == 'b':
                a = ins.target
                continue
            a += ins.size
        return False

    def classify_far(self):
        strong = set(SEEDS_THUMB) | set(ARM_FUNCS) | self.ptr_funcs
        strong |= {f for f in self.funcs if f % 4 == 0 and self.is_push_lr(f)}
        ss = sorted(strong) + [CODE_END]
        import bisect
        far = set()
        for t, sites in self.calls.items():
            if t in strong or not in_code(t):
                continue
            i = bisect.bisect_right(ss, t) - 1
            lo, hi = (ss[i] if i >= 0 else CODE_START), ss[i + 1] if i + 1 < len(ss) else CODE_END
            if all(lo <= s < hi for s in sites) and (t % 4 == 2 or not self.reaches_bx_lr(t)):
                far.add(t)
            elif t % 4 == 2:
                self.errors.append(f'2-aligned bl target {t:08X} called from outside its interval')
        return far

    def gaps(self):
        import re
        return [(CODE_START + m.start(), CODE_START + m.end())
                for m in re.finditer(b'\x00+', bytes(self.cov))]

    def fill_gaps(self):
        """Seed functions at the start of uncovered regions that look like code.
        Prologue (push) starts are tried first; leaf guesses only when none remain."""
        if self._fill_gaps(push_only=True):
            return True
        return self._fill_gaps(push_only=False)

    def _fill_gaps(self, push_only):
        added = False
        for lo, hi in self.gaps():
            a = lo
            # skip alignment padding
            while a + 2 <= hi and u16(a) in (0, 0x46C0):
                a += 2
            if a >= hi:
                continue
            if a in self.funcs or a in self.bad:
                continue
            if push_only and (u16(a) & 0xFF00) not in (0xB500, 0xB400):
                continue
            if self.plausible_func(a, hi):
                self.add_func(a, why=f'gap {lo:08X}-{hi:08X}')
                added = True
        return added

    def plausible_func(self, a, hi):
        h = u16(a)
        if (h & 0xFF00) == 0xB500 or (h & 0xFF00) == 0xB400:  # push
            return True
        # leaf function: walk straight-line code until a return
        p = a
        for _ in range(64):
            if p >= hi + 0x200:
                return False
            ins = decode_thumb(p)
            if ins.kind in ('bad', 'raw', 'bl'):
                return False
            if ins.kind in ('ret', 'movpc', 'b'):
                return True
            if ins.kind == 'ldrpc' and not (a < ins.lit < a + 0x400):
                return False
            p += ins.size
        return False


# ---------------------------------------------------------------- ARM (capstone)
csa = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)


def arm_lines(lo, hi, labels, names, lits):
    """Disassemble ARM region [lo,hi). Returns list of (addr, size, text)."""
    out = []
    a = lo
    while a < hi:
        if a in lits:
            out.append((a, 4, f'.4byte 0x{u32(a):08X}'))
            a += 4
            continue
        w = u32(a)
        ins = next(csa.disasm(rom[a - BASE:a - BASE + 4], a), None)
        if ins is None:
            out.append((a, 4, f'.4byte 0x{w:08X}'))
            a += 4
            continue
        text = f'{ins.mnemonic} {ins.op_str}'
        if ins.mnemonic.startswith('b') and ins.op_str.startswith('#') and ins.mnemonic not in ('bic', 'bics'):
            t = int(ins.op_str[1:], 16)
            text = f'{ins.mnemonic} {names.get(t) or "_%08X" % t}'
            labels.add(t)
        elif '[pc, #' in ins.op_str and ins.mnemonic.startswith('ldr'):
            off = int(ins.op_str.split('#')[1].rstrip(']!'), 16)
            if '#-' in ins.op_str:
                off = -off
            lit = a + 8 + off
            lits.add(lit)
        out.append((a, 4, text))
        a += 4
    return out


# ---------------------------------------------------------------- output
HEADER = '''\t.include "asm/macros.inc"

\t.syntax divided
\t.text

'''


def main():
    an = Analyzer()
    an.run()
    funcs = sorted(an.funcs)

    # --- ARM literal pools / labels
    arm_lits = set()
    arm_labels = set()
    names = {a: f'sub_{a:08X}' for a in funcs}
    names.update(LIB_SYMS)
    names.update({a: n for a, n in NAMES.items() if a in names})
    names[VENEER] = '__sub_0807EAD0_from_thumb'
    for lo, hi in ARM_REGIONS:
        arm_lines(lo, hi, arm_labels, names, arm_lits)  # prepass for lits
    # IWRAM-relative ARM routine copied to RAM uses `sub r1, pc, #x` (position independent) - fine.

    # --- data references
    data_refs = set()
    for v in list(an.lits.values()):
        if is_data_addr(v):
            data_refs.add(v)

    def lit_expr(v):
        if v & 1 and (v - 1) in names and (in_code(v - 1) or (v - 1) in LIB_SYMS or v - 1 == VENEER):
            return names[v - 1]
        if (v in names) and an.funcs.get(v) and an.funcs[v].mode == 'a':
            return names[v]
        if is_data_addr(v):
            return f'gUnk_{v:08X}'
        return f'0x{v:08X}'

    # --- function extents: partition [CODE_START, CODE_END)
    starts = funcs + [CODE_END]
    os.makedirs('asm/nonmatching', exist_ok=True)
    os.makedirs('config', exist_ok=True)
    code_splits, data_splits = read_splits()
    units = []          # (unit_name, [func addrs], is_src)
    fset = set(funcs)
    for (lo, uname, src), (hi, _, _) in zip(code_splits, code_splits[1:]):
        if lo not in fset:
            print(f'warning: split {uname} at {lo:08X} is not a function start', file=sys.stderr)
        ufuncs = [f for f in funcs if lo <= f < hi]
        if uname != 'auto':
            units.append((uname, ufuncs, src))
            continue
        cur = []
        cur_start = None
        for fa in ufuncs:
            if cur and (fa - cur_start) >= UNIT_BYTES:
                units.append((f'code_{cur_start:08X}', cur, False))
                cur = []
            if not cur:
                cur_start = fa
            cur.append(fa)
        if cur:
            units.append((f'code_{cur_start:08X}', cur, False))

    func_tsv = []
    all_labels = an.labels | set(an.lits) | arm_labels | {t for t in an.jtabs if t - 4 not in an.jtabs}
    for ui, (uname, ufuncs, src) in enumerate(units):
        udir = f'asm/nonmatching/{uname}'
        includes = []
        for fa in ufuncs:
            fend = starts[starts.index(fa) + 1]
            f = an.funcs[fa]
            if src:
                func_tsv.append(f'0x{fa:08X}\t{f.mode}\t0x{fend - fa:X}\t{names[fa]}\t{uname}\tsrc')
                continue
            os.makedirs(udir, exist_ok=True)
            lines = emit_function(an, f, fend, names, all_labels, lit_expr, arm_lits)
            path = f'{udir}/{names[fa]}.s'
            with open(path, 'w') as fh:
                fh.write('\n'.join(lines) + '\n')
            includes.append(path)
            func_tsv.append(f'0x{fa:08X}\t{f.mode}\t0x{fend - fa:X}\t{names[fa]}\t{uname}\t{an.why.get(fa, "")}')
        if src:
            continue
        with open(f'asm/{uname}.s', 'w') as fh:
            fh.write(HEADER)
            for p in includes:
                fh.write(f'\t.include "{p}"\n')

    with open('config/functions.tsv', 'w') as fh:
        fh.write('# addr\tmode\tsize\tname\tunit\n')
        fh.write('\n'.join(func_tsv) + '\n')

    # --- data
    emit_data(data_refs, data_splits)

    # --- units.txt
    with open('units.txt', 'w') as fh:
        fh.write('# Link order. A unit links src/<unit>.c, src/<unit>.s, asm/<unit>.s or data/<unit>.s\n')
        fh.write('# (first that exists). "@libs" = libgcc/libc; "@rodata <unit>" places that unit\'s\n')
        fh.write('# .rodata at that point of the .rodata section instead of next to its .text.\n')
        fh.write('crt0\n')
        for uname, _, _ in units:
            fh.write(f'{uname}\n')
        fh.write('@libs\n')
        fh.write('veneer\n')
        for lo, uname, src in data_splits[:-1]:
            fh.write(f'@rodata {uname}\n' if src else f'{uname}\n')

    print(f'{len(funcs)} functions, {len(units)} units, {len(data_refs)} data labels', file=sys.stderr)
    gaps = an.gaps()
    print(f'{len(an.errors)} analysis messages; {len(gaps)} uncovered gaps '
          f'({sum(h - l for l, h in gaps)} bytes)', file=sys.stderr)
    with open('config/disasm_report.txt', 'w') as fh:
        fh.write('\n'.join(an.errors) + '\n')
        fh.write('\n# uncovered gaps\n')
        for l, h in gaps:
            if any(u16(x) != 0 for x in range(l, h - 1, 2)) or h - l > 2:
                fh.write(f'{l:08X}-{h:08X} ({h - l} bytes)\n')


def emit_function(an, f, fend, names, all_labels, lit_expr, arm_lits):
    fa = f.addr
    name = names[fa]
    out = []
    if f.mode == 'a':
        out.append(f'\tarm_func_start {name}')
        out.append(f'{name}: @ 0x{fa:08X}')
        out.append('\t.syntax unified')
        dummy = set()
        for a, size, text in arm_lines(fa, fend, dummy, names, arm_lits):
            if a != fa and (a in all_labels):
                out.append(f'_{a:08X}:')
            if a in arm_lits:
                out.append(f'\t.4byte 0x{u32(a):08X}')
                continue
            out.append(f'\t{text}')
        out.append('\t.syntax divided')
        out.append(f'\tarm_func_end {name}')
        return out

    if fa & 3:
        out.append(f'\tnon_word_aligned_thumb_func_start {name}')
    else:
        out.append(f'\tthumb_func_start {name}')
    out.append(f'{name}: @ 0x{fa:08X}')
    a = fa
    while a < fend:
        if a != fa and a in all_labels and a not in an.lits:
            out.append(f'_{a:08X}:')
        if a in an.insns:
            ins = an.insns[a]
            if a + ins.size > fend:
                out.append(f'\t.2byte 0x{u16(a):04X}')
                a += 2
                continue
            out.append('\t' + render(ins, names, an))
            a += ins.size
        elif a in an.lits:
            if a & 3:
                out.append(f'\t.2byte 0x{u16(a):04X}')
                a += 2
                continue
            out.append(f'_{a:08X}: .4byte {lit_expr(an.lits[a])}')
            a += 4
        elif a in an.jtabs:
            out.append(f'\t.4byte _{an.jtabs[a]:08X}')
            a += 4
        else:
            # padding or unknown bytes
            h = u16(a)
            nxt = a + 2
            if h == 0 and (nxt & 3) == 0 and (nxt in an.lits or nxt == fend) and not (a & 1):
                out.append('\t.align 2, 0')
            else:
                out.append(f'\t.2byte 0x{h:04X}')
            a += 2
    # .size before any trailing alignment padding, as agbcc emits it (objdiff compares symbol sizes)
    if out[-1] == '\t.align 2, 0':
        out.insert(len(out) - 1, f'\tthumb_func_end {name}')
    else:
        out.append(f'\tthumb_func_end {name}')
    out.append('')
    return out


def render(ins, names, an):
    k = ins.kind
    if k == 'ldrpc':
        return f'ldr {REGS[ins.reg]}, _{ins.lit:08X} @ =0x{u32(ins.lit):08X}'
    if k in ('b', 'bc'):
        t = ins.target
        return f'{ins.text} {names.get(t) if t in an.funcs else "_%08X" % t}'
    if k == 'bl':
        t = ins.target
        if t in an.farjumps:
            return f'bl _{t:08X} @ far jump'
        return f'bl {names.get(t, "sub_%08X" % t)}'
    return ins.text


def read_splits():
    code, data = [], []
    for line in open('config/splits.txt'):
        line = line.split('#')[0].split()
        if not line:
            continue
        lst = code
        if line[0] == 'data':
            lst = data
            line = line[1:]
        lst.append((int(line[0], 16), line[1], len(line) > 2 and line[2] == 'src'))
    return code, data


def emit_data(refs, splits):
    for (lo, uname, src), (hi, _, _) in zip(splits, splits[1:]):
        pts = sorted(r for r in refs if lo <= r < hi)
        if src:
            if pts:
                print(f'warning: data refs inside src unit {uname}: {[hex(p) for p in pts]}', file=sys.stderr)
            continue
        lines = ['\t.section .rodata', '']
        cur = lo
        for p in pts + [hi]:
            if p > cur:
                lines.append(f'\t.incbin "baserom.gba", 0x{cur - BASE:X}, 0x{p - cur:X}')
            if p < hi:
                lines.append(f'\t.global gUnk_{p:08X}')
                lines.append(f'gUnk_{p:08X}: @ 0x{p:08X}')
            cur = p
        path = f'data/{uname}.s'
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, 'w') as fh:
            fh.write('\n'.join(lines) + '\n')


if __name__ == '__main__':
    main()
