#!/usr/bin/env python3
"""regoracle.py: register-allocation oracle. Says which pseudos agbcc allocated differently from the ROM,
why (global-alloc order, preferences, conflicts, local-alloc, reload rotation), and what in the source has
to change (priority window, refs/live levers), checked by recompiling.

  tools/dr python3 tools/regoracle.py <unit> <func> [--src FILE] [--range LO HI] [--json] [--show LO HI]
  tools/dr python3 tools/regoracle.py --verify-compiler [unit ...]    # patched == stock compiler, traced or not
  tools/dr python3 tools/regoracle.py --selftest <unit> <func> [<unit> <func> ...] [--perturb N [--seed S]]

  --src FILE      C file to compile (default: build/wf/<func>/unit.c when it exists, else src/<unit>.c)
  --range LO HI   only use ROM instructions in [LO, HI) as evidence (addresses, e.g. 0x08044F4C 0x08045020)
  --show LO HI    print the aligned ROM/built listing of [LO, HI): insn uid, source line and the pseudo behind
                  every built register operand (' ' identical, r: other registers, ~: high/low register
                  form, *: different code)
  --json          machine-readable output (same content)
  --top N         rows per table (default 25)
  --no-solve      skip the inverse solve;  --no-verify: do not recompile to check the solution
  --budget N      simulator runs per solve round (default 6000; ~3 ms each for a 1600-allocno function)
  --rounds N      solve again on the solved build while the compiler check keeps improving (default 3)
  --prio R=P,...  what-if: compile with global-alloc priority P for pseudo R (allocno_compare units)
  --force R=H,... what-if: compile with pseudo R forced into hard reg H
  --selftest      on matched functions: sim must reproduce every find_reg decision, no pseudo may differ
  --perturb N     with --selftest: N trials that mis-order 1-3 random pseudos (REGORACLE_PRIO) and check
                  that the solver's priorities recompile to the ROM bytes again

Output: build/regoracle/<func>/ (unit.s with -dp insn uids, trace.txt, trace-solved.txt). Pseudo numbers are
stable for a given source, so --prio/--force and REGORACLE_PRIO strings can be reused.
Changing the compiler patch: edit build/regoracle/old_agbcc-src/gcc, `tools/dr make -C <that>/gcc old -j1`,
then `git -C <that> add -N gcc/regoracle.c gcc/regoracle.h && git -C <that> diff > tools/regoracle_agbcc.patch`
(the next run rebuilds every tree whose stamp no longer matches the patch) and rerun --verify-compiler.

How it works
  1. Compiles with a patched old_agbcc: build/regoracle/old_agbcc-src (pret/agbcc at AGBCC_COMMIT +
     tools/regoracle_agbcc.patch), cloned and built inside tools/dr on first use, rebuilt when the patch
     changes (agbcc-src likewise for units that use agbcc). The patch only adds a trace, written when
     REGORACLE_OUT/REGORACLE_FUNC are set (record formats: gcc/regoracle.c in the patch); code is
     byte-identical to the stock compiler (--verify-compiler: all 112 C units, traced and untraced). The
     trace has decl names and source lines of pseudos (also inline-function locals), the expression each
     pseudo was created for and the pass that made it, REG_N_REFS contributions per insn, live sets,
     REG_EQUIV live doubling, the global-alloc tables (refs, live, conflicts, preferences, classes), every
     find_reg decision, local-alloc quantities, reload register choices (last_spill_reg rotation) and, per
     output insn, which pseudo each hard-register operand is.
  2. Aligns the built function with the ROM instruction by instruction (registers masked; low/high kept
     apart first, then one form per operation inside blocks that still differ; branch targets and pool
     offsets normalised) and reads the ROM's register for every operand where the code is structurally
     equal. Each built operand maps to its pseudo, so every pseudo gets votes for the ROM's register
     (commutative operands are oriented by the other votes; a pseudo spilled here gets votes from its
     reload registers unless our reloads of it align with ROM reloads).
  3. Replays global-alloc in Python (exact port of global.c prune_preferences + find_reg + the allocation
     loop; checked against the compiler's own decisions on every run) and searches priority changes that
     reproduce the ROM's registers (beam search over targeted moves: pass a blocker, let the pseudo that
     holds the register in the ROM go first, regs_someone_prefers, exhaustive 1-moves when stuck). For
     each moved pseudo: the priority window that works, the refs-only or live-only change that lands in
     it, where its refs come from (lines, loop depth) and where it is live, and which pseudos it has to
     pass. The solution is recompiled with REGORACLE_PRIO: kept only if the build gets closer to the ROM.
  4. Explains what reordering cannot fix: the ROM register is a hard conflict (local-alloc pseudo or live
     hard reg), the ROM puts a conflicting pseudo in the same register (they do not overlap there: a
     liveness/statement-order difference, with the insns where they overlap here), or a lower register is
     busy in the ROM (nearest pseudos that the ROM has in it); local-alloc mismatches with the quantity
     that took the register; the first reload-register rotation mismatch (spill slot vs last_spill_reg)
     and the reload allocations in between, flagging the ones in code that differs from the ROM.

Priority = int(floor_log2(refs) * refs / live * 10000 * size); higher is allocated first, ties go to the
lower pseudo. refs is REG_N_REFS (each set or use adds the loop depth: 1 outside loops, +1 per enclosing
loop); live is REG_LIVE_LENGTH in insns (doubled for a pseudo with a REG_EQUIV note).
Validation: --selftest on 7 large matched functions (sim 100%, 0 differing pseudos) and --perturb on 12
functions: 53 of 60 random mis-orderings were undone exactly (recompiled to 0 differing lines); the rest
involve pseudos with no aligned evidence (a high register on one side turns their code into other insns).
"""
import argparse
import difflib
import hashlib
import json
import math
import os
import re
import subprocess
import sys
import tempfile
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)
sys.path.insert(0, os.path.join(ROOT, 'tools'))

AGBCC_REPO = 'https://github.com/pret/agbcc'
AGBCC_COMMIT = 'da598c1d918402c42c0c0d7128ba14567f3175e9'
PATCH = 'tools/regoracle_agbcc.patch'
BUILD = 'build/regoracle'
STOCK_DIR = os.environ.get('AGBCC_DIR', '/opt/agbcc')
BASE = 0x08000000


def run(cmd, env=None, cwd=None):
    return subprocess.run(cmd, capture_output=True, text=True, env=env, cwd=cwd)


def die(msg):
    sys.exit('regoracle: ' + msg)


# ----------------------------------------------------------------------------------------------
# patched compiler

def compiler_path(cc):
    return f'{BUILD}/{cc}-src/gcc/{cc}'


def ensure_compiler(cc='old_agbcc'):
    """Build the patched compiler (once; again when the patch changes)."""
    if cc not in ('old_agbcc', 'agbcc'):
        die(f'unit compiler {cc} is not supported (old_agbcc, agbcc)')
    src = f'{BUILD}/{cc}-src'
    binp = compiler_path(cc)
    stamp = f'{src}/.regoracle-stamp'
    want = hashlib.sha1(open(PATCH, 'rb').read()).hexdigest()
    if os.path.exists(binp) and os.path.exists(stamp) and open(stamp).read().strip() == want:
        return binp
    os.makedirs(BUILD, exist_ok=True)
    log = f'{BUILD}/{cc}-build.log'
    if not os.path.isdir(f'{src}/.git'):
        r = run(['git', 'clone', '-q', AGBCC_REPO, src])
        if r.returncode:
            die(f'cannot clone {AGBCC_REPO} into {src} (network?):\n{r.stderr}')
    steps = [['git', '-C', src, 'checkout', '-q', '-f', AGBCC_COMMIT],
             ['git', '-C', src, 'clean', '-q', '-fdx', 'gcc'],
             ['git', '-C', src, 'apply', os.path.abspath(PATCH)],
             ['make', '-C', f'{src}/gcc', 'old' if cc == 'old_agbcc' else 'normal', '-j1']]
    with open(log, 'w') as lf:
        for cmd in steps:
            r = run(cmd)
            lf.write(' '.join(cmd) + '\n' + r.stdout + r.stderr)
            if r.returncode:
                die(f'building the patched compiler failed at `{" ".join(cmd)}` (see {log})')
    open(stamp, 'w').write(want + '\n')
    return binp


def unit_flags(unit):
    import check
    return check.unit_cflags(unit)


def preprocess(src, out):
    r = run(['cpp', '-nostdinc', '-undef', '-I', 'include', '-I', f'{STOCK_DIR}/include', '-iquote', '.',
             src, '-o', out])
    if r.returncode:
        die(f'cpp failed:\n{r.stderr}')


def compile_traced(unit, func, src, wdir, prio=None, force=None, tag=''):
    """Compile with the patched compiler. Returns (asm path, trace path)."""
    cc, flags = unit_flags(unit)
    comp = ensure_compiler(cc)
    os.makedirs(wdir, exist_ok=True)
    i, s, t = f'{wdir}/unit.i', f'{wdir}/unit{tag}.s', f'{wdir}/trace{tag}.txt'
    if not tag or not os.path.exists(i):
        preprocess(src, i)
    if os.path.exists(t):
        os.remove(t)
    env = dict(os.environ, REGORACLE_OUT=t, REGORACLE_FUNC=func)
    env.pop('REGORACLE_PRIO', None)
    env.pop('REGORACLE_FORCE', None)
    if prio:
        env['REGORACLE_PRIO'] = ','.join(f'{r}={v}' for r, v in sorted(prio.items()))
    if force:
        env['REGORACLE_FORCE'] = ','.join(f'{r}={v}' for r, v in sorted(force.items()))
    r = run([comp] + flags + ['-dp', i, '-o', s], env=env)
    if r.returncode:
        die(f'compile failed:\n{r.stdout}{r.stderr}')
    if not os.path.exists(t) or 'F ' + func not in open(t).read(200000):
        die(f'{func} was not compiled from {src} (is it still INCLUDE_ASM there?)')
    return s, t


# ----------------------------------------------------------------------------------------------
# trace

def parse_rtx(text):
    """Compact RTL (as printed by the patch) -> nested lists. Atoms stay strings; a constant-pool
    reference (printed `=CONST`) becomes ['pool', CONST]."""
    toks = re.findall(r'"[^"]*"|`[^\']*\'|\(|\)|\[|\]|=|[^\s()\[\]=]+', text)
    pos = 0

    def item():
        nonlocal pos
        t = toks[pos]
        pos += 1
        if t == '=':
            return ['pool', item()]
        if t in '([':
            close = ')' if t == '(' else ']'
            out = [] if t == '(' else ['vec']
            while pos < len(toks) and toks[pos] != close:
                out.append(item())
            pos += 1
            return out
        return t
    try:
        return item()
    except IndexError:
        return text


class Trace:
    def __init__(self, path):
        self.decls = []          # (kind, name, origin, line, rtx text)
        self.origin = {}         # regno -> (uid, line, rtx text)
        self.newreg = {}         # regno -> (pass, mode)
        self.uline = {}          # uid -> line (expansion)
        self.chain = []          # ('I', uid, dict) | ('L', uid) | ('NB', uid) | ('NE', uid) ...
        self.insn = {}           # uid -> dict(kind, depth, s, u, dead, unused, pat, equiv)
        self.blocks = []
        self.refs = {}           # regno -> [(uid, depth, delta)]
        self.live = {}           # regno -> [uid...]
        self.eq = {}             # regno -> uid (REG_EQUIV live doubling)
        self.lq = []             # local-alloc quantities
        self.lqs = {}
        self.A = {}
        self.C = {}
        self.P = {}
        self.T = []
        self.Q = {}
        self.Z = {}
        self.GC = {}
        self.GR = {}
        self.GSET = {}
        self.order = []
        self.share = {}
        self.spilled = {}        # regno -> old hard reg (reload spill)
        self.spills = []
        self.RA = []             # reload allocations in order
        self.RL = {}             # uid -> [reload dict]
        self.RLI = {}            # main uid -> [reload insn uids]
        self.reload_of = {}      # reload insn uid -> main uid
        self.FI = {}             # uid -> [(pseudo or None, hard reg, 's'|'u')] as output by final
        for line in open(path):
            line = line.rstrip('\n')
            k = line.split(' ', 1)[0]
            if k == 'I':
                head, _, pat = line.partition('\t')
                p = head.split()
                d = dict(kind=p[2], pat=pat, equiv=None)
                for kv in p[3:]:
                    key, _, v = kv.partition('=')
                    if key == 'd':
                        d['depth'] = int(v)
                    elif key in ('s', 'u', 'dead', 'unused'):
                        d[key] = [int(x) for x in v.split(',') if x]
                    elif key in ('equiv', 'equal'):
                        d['equiv'] = v
                uid = int(p[1])
                self.insn[uid] = d
                self.chain.append(('I', uid))
            elif k in ('L', 'NB', 'NE', 'NV', 'NC'):
                self.chain.append((k, int(line.split()[1])))
            elif k == 'B':
                self.blocks.append(tuple(int(x) for x in line.split()[1:]))
            elif k == 'U':
                _, u, ln = line.split()
                self.uline[int(u)] = int(ln)
            elif k == 'O':
                p = line.split(' ', 4)
                self.origin[int(p[1])] = (int(p[2]), int(p[3]), p[4])
            elif k == 'N':
                p = line.split()
                self.newreg[int(p[1])] = (p[2], p[4])
            elif k == 'D':
                p = line.split(' ', 5)
                self.decls.append((p[1], p[2], p[3], int(p[4]), p[5]))
            elif k == 'RF':
                p = line.split()
                uid, depth = int(p[1]), int(p[2])
                for x in p[3:]:
                    r, dlt = x.split(':')
                    self.refs.setdefault(int(r), []).append((uid, depth, int(dlt)))
            elif k == 'LV':
                p = line.split()
                uid = int(p[1])
                for x in p[2:]:
                    self.live.setdefault(int(x), []).append(uid)
            elif k == 'EQ':
                p = line.split()
                self.eq[int(p[1])] = int(p[2])
            elif k == 'LQ':
                p = line.split()
                d = dict(block=int(p[1]), q=int(p[2]))
                for kv in p[3:]:
                    key, _, v = kv.partition('=')
                    d[key] = [int(x) for x in v.split(',') if x] if key == 'regs' else int(v)
                self.lq.append(d)
            elif k == 'LQS':
                p = line.split()
                self.lqs[(int(p[1]), int(p[2]))] = int(p[3])
            elif k == 'A':
                p = line.split()
                a = int(p[1])
                d = dict(regno=int(p[2]))
                for kv in p[3:]:
                    key, _, v = kv.partition('=')
                    if key in ('hconf', 'pref', 'cpref', 'fpref', 'modeok', 'multi'):
                        d[key] = int(v, 16)
                    elif key == 'mode':
                        d[key] = v
                    else:
                        d[key] = int(v)
                self.A[a] = d
            elif k == 'C':
                p = [int(x) for x in line.split()[1:]]
                self.C[p[0]] = p[1:]
            elif k == 'P':
                p = line.split()
                self.P[int(p[1])] = {kv.split('=')[0]: int(kv.split('=')[1], 16) for kv in p[2:]}
            elif k == 'GORDER':
                self.order = [int(x) for x in line.split()[1:]]
            elif k == 'GSET':
                for kv in line.split()[1:]:
                    key, _, v = kv.partition('=')
                    self.GSET[key] = int(v) if key in ('caller_saves', 'nregs') else int(v, 16)
            elif k == 'GC':
                p = line.split()
                self.GC[int(p[1])] = (p[2], int(p[3], 16))
            elif k == 'GR':
                p = [int(x) for x in line.split()[1:]]
                self.GR[p[0]] = dict(cls=p[1], lrefs=p[2], llive=p[3], ever=p[4])
            elif k == 'GSHARE':
                p = line.split()
                self.share[int(p[1])] = int(p[2])
            elif k == 'T':
                p = line.split()
                d = dict(a=int(p[1]), regno=int(p[2]))
                for kv in p[3:]:
                    key, _, v = kv.partition('=')
                    d[key] = v if key == 'via' else (int(v, 16) if key in ('losers', 'used1', 'nopref') else int(v))
                self.T.append(d)
            elif k == 'Q':
                p = line.split()
                self.Q[int(p[1])] = int(p[2])
            elif k == 'Z':
                p = line.split()
                self.Z[int(p[1])] = int(p[2])
            elif k == 'SP':
                p = line.split()
                self.spilled[int(p[1])] = int(p[2])
            elif k == 'SPILLS':
                self.spills = [int(x) for x in line.split()[2:]]
            elif k == 'RA':
                p = line.split()
                d = dict(uid=int(p[1]), r=int(p[2]))
                for kv in p[3:]:
                    key, _, v = kv.partition('=')
                    d[key] = int(v)
                self.RA.append(d)
            elif k == 'RL':
                head = line.split(' in=', 1)
                p = head[0].split()
                d = dict(uid=int(p[1]), r=int(p[2]))
                for kv in p[3:]:
                    key, _, v = kv.partition('=')
                    d[key] = int(v)
                io = head[1] if len(head) > 1 else ''
                d['in'], _, d['out'] = io.partition(' out=')
                self.RL.setdefault(d['uid'], []).append(d)
            elif k == 'FI':
                p = line.split()
                ents = []
                for x in p[2:]:
                    ps, h, role = x.split(':')
                    ents.append((None if ps == '-' else int(ps), int(h), role))
                self.FI[int(p[1])] = ents
            elif k == 'RLI':
                p = [int(x) for x in line.split()[1:]]
                self.RLI[p[0]] = p[1:]
                for u in p[1:]:
                    self.reload_of[u] = p[0]
        self.allocno_of = {d['regno']: a for a, d in self.A.items()}
        for r, a in self.share.items():
            self.allocno_of[r] = a
        # source line of every chain insn: expansion line, else the previous insn's line
        self.line = {}
        last = 0
        for kind, uid in self.chain:
            if kind != 'I':
                continue
            ln = self.uline.get(uid, 0)
            if ln:
                last = ln
            self.line[uid] = ln or last
        first = next((self.line[u] for k, u in self.chain if k == 'I' and self.line.get(u)), 0)
        for kind, uid in self.chain:
            if kind == 'I' and not self.line[uid]:
                self.line[uid] = first
        for main, rel in self.RLI.items():
            for u in rel:
                self.line.setdefault(u, self.line.get(main, 0))
        self.pos = {uid: k for k, (kind, uid) in enumerate(self.chain)}

    def final_reg(self, r):
        return self.Z.get(r, -1)


# ----------------------------------------------------------------------------------------------
# names

OPS = {'plus': '+', 'minus': '-', 'mult': '*', 'and': '&', 'ior': '|', 'xor': '^', 'ashift': '<<',
       'lshiftrt': '>>', 'ashiftrt': '>>', 'div': '/', 'udiv': '/', 'mod': '%', 'umod': '%',
       'eq': '==', 'ne': '!=', 'lt': '<', 'gt': '>', 'le': '<=', 'ge': '>=', 'ltu': '<', 'gtu': '>',
       'leu': '<=', 'geu': '>='}
MODEC = {'QI': 'u8', 'HI': 'u16', 'SI': 'u32', 'DI': 'u64'}


class Namer:
    def __init__(self, tr):
        self.tr = tr
        self.decl = {}
        for kind, name, origin, line, rtx in tr.decls:
            m = re.match(r'\((?:reg|addressof)(?::\w+)? \(?(?:reg:\w+ )?(\d+)', rtx)
            if m:
                r = int(m.group(1))
                nm = name if origin == '-' else f'{origin}.{name}'
                self.decl.setdefault(r, [])
                if nm not in self.decl[r]:
                    self.decl[r].append(nm)

    def short(self, r):
        if r in self.decl:
            return '/'.join(self.decl[r])
        return None

    def expr(self, x, depth=0):
        if isinstance(x, str):
            if x.startswith('`'):
                return x[1:-1]
            if re.fullmatch(r'-?\d+', x):
                v = int(x)
                return str(v) if -256 < v < 256 else hex(v) if v >= 0 else str(v)
            return x
        if not x:
            return '?'
        head = x[0] if isinstance(x[0], str) else '?'
        op, _, mode = head.partition(':')
        args = x[1:]
        if op == 'reg':
            r = int(args[0])
            if r < 17:
                return ['r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'r6', 'r7', 'r8', 'r9', 'sl', 'fp', 'ip',
                        'sp', 'lr', 'pc', 'ap'][r]
            nm = self.short(r)
            if nm:
                return nm
            if depth < 3:
                d = self.def_expr(r, depth + 1)
                if d:
                    return '(' + d + ')'
            return f'p{r}'
        if depth > 4:
            return '...'
        if op == 'pool':
            return '=' + self.expr(args[0], depth + 1)
        if op == 'mem':
            inner = self.expr(args[0], depth + 1)
            if isinstance(args[0], list) and args[0] and args[0][0] == 'pool':
                return inner
            return f'*({MODEC.get(mode, mode)} *)({inner})' if mode != 'SI' else f'*({inner})'
        if op in OPS and len(args) == 2:
            return f'{self.expr(args[0], depth + 1)} {OPS[op]} {self.expr(args[1], depth + 1)}'
        if op in ('subreg', 'const', 'strict_low_part'):
            return self.expr(args[0], depth + 1)
        if op in ('zero_extend', 'sign_extend', 'truncate'):
            return f'({MODEC.get(mode, mode)}){self.expr(args[0], depth + 1)}'
        if op == 'neg':
            return '-' + self.expr(args[0], depth + 1)
        if op == 'not':
            return '~' + self.expr(args[0], depth + 1)
        if op == 'call':
            return 'call ' + self.expr(args[0][1] if isinstance(args[0], list) else args[0], depth + 1)
        if op == 'label_ref':
            return 'L' + str(args[0])
        return op + '(' + ', '.join(self.expr(a, depth + 1) for a in args if not (isinstance(a, list) and a and a[0] == 'vec')) + ')'

    PASSNOTE = {'jump': 'loop-test copy', 'cse': 'cse', 'cse2': 'cse2', 'gcse': 'gcse', 'loop': 'loop',
                'loop2': 'loop2', 'addressof': 'addressof'}

    def def_expr(self, r, depth):
        """Source expression of an unnamed pseudo: its expansion-time value, else its first set now."""
        tr = self.tr
        if r in tr.origin:
            return self.expr(parse_rtx(tr.origin[r][2]), depth)
        uid = self.first_def(r)
        if uid is None:
            return None
        pat = parse_rtx(tr.insn[uid]['pat'])
        if isinstance(pat, list) and pat and pat[0] == 'set':
            return self.expr(pat[2], depth)
        return self.expr(pat, depth)

    def describe(self, r):
        """Name of a pseudo: its variable, or the expression it was created for."""
        nm = self.short(r)
        if nm:
            return nm
        e = self.def_expr(r, 1)
        if e is None:
            return f'p{r}'
        ps = self.tr.newreg.get(r, ('expand',))[0]
        return f'[{e}]' if ps == 'expand' else f'[{self.PASSNOTE.get(ps, ps)}: {e}]'

    def first_def(self, r):
        if not hasattr(self, '_fd'):
            self._fd = {}
            for kind, uid in self.tr.chain:
                if kind == 'I':
                    for x in self.tr.insn[uid].get('s', []):
                        self._fd.setdefault(x, uid)
        return self._fd.get(r)


# ----------------------------------------------------------------------------------------------
# instructions: built (from the -dp .s) and target (original asm), disassembled by capstone

REGNAMES = {'sb': 9, 'sl': 10, 'fp': 11, 'ip': 12, 'sp': 13, 'lr': 14, 'pc': 15}
ALLOC_RX = re.compile(r'\b(r1[0-2]|r[0-9]|sb|sl|fp|ip)\b')
DATA_SIZES = {'.word': 4, '.4byte': 4, '.long': 4, '.int': 4, '.short': 2, '.hword': 2, '.2byte': 2,
              '.byte': 1}


def regnum(name):
    return REGNAMES[name] if name in REGNAMES else int(name[1:])


def layout_asm(lines, start_addr):
    """Offsets of the instruction lines of one function's asm text. Returns [(addr, size, text, uid)]."""
    out = []
    addr = start_addr
    uid = None
    for raw in lines:
        if re.match(r'^[\w.$]+:', raw.strip()):
            uid = None
        line = raw.split('\t@ ')[0] if re.search(r'\t@ \d+\t', raw) else raw
        m_uid = re.search(r'\t@ (\d+)\t', raw)
        code = re.sub(r'@.*$', '', line).strip()
        if not code:
            continue
        if re.match(r'^[\w.$]+:', code):
            code = re.sub(r'^[\w.$]+:\s*', '', code)
            if not code:
                continue
        if code.startswith('.'):
            d = code.split()[0]
            if d == '.align':
                n = int(code.split()[1].rstrip(','))
                al = 1 << n
                addr = (addr + al - 1) // al * al
            elif d in DATA_SIZES:
                items = code[len(d):].split(',')
                addr += DATA_SIZES[d] * len(items)
            elif d in ('.space', '.skip'):
                addr += int(code.split()[1], 0)
            continue
        mn = code.split()[0]
        if mn.endswith('func_start') or mn.endswith('func_end') or mn in ('thumb_func_start',):
            continue
        if m_uid:
            uid = int(m_uid.group(1))
        size = 4 if mn in ('bl', 'blx') else 2
        out.append((addr, size, code, uid))
        addr += size
    return out


def built_function(asm_path, func):
    text = open(asm_path).read().split('\n')
    try:
        i = text.index(f'{func}:')
    except ValueError:
        die(f'{func} not found in {asm_path}')
    j = i + 1
    while j < len(text) and not text[j].startswith('.Lfe'):
        j += 1
    body = text[i + 1:j]
    # instructions before the first -dp comment (prologue) carry no uid; the extra lines of a
    # multi-instruction template (far branches, epilogue after `add sp`) carry the template's uid
    return [dict(off=addr, size=size, src=code, uid=u) for addr, size, code, u in layout_asm(body, 0)]


def assemble(asm_path, func, wdir):
    o = f'{wdir}/unit.o'
    r = run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-mthumb-interwork', '-I', '.', '-o', o, asm_path])
    if r.returncode:
        die('assemble failed:\n' + r.stderr)
    nm = run(['arm-none-eabi-nm', '-S', o]).stdout
    start = size = None
    for line in nm.splitlines():
        p = line.split()
        if len(p) == 4 and p[3] == func:
            start, size = int(p[0], 16) & ~1, int(p[1], 16)
    run(['arm-none-eabi-objcopy', '-O', 'binary', '-j', '.text', o, f'{wdir}/text.bin'])
    data = open(f'{wdir}/text.bin', 'rb').read()
    return data[start:start + size], size


def target_function(unit, func):
    from target import ROM
    fa = fsize = None
    for line in open('config/functions.tsv'):
        if line.startswith('#'):
            continue
        f = line.rstrip('\n').split('\t')
        if f[3] == func:
            fa, fsize = int(f[0], 16), int(f[2], 16)
    if fa is None:
        die(f'{func} not in config/functions.tsv')
    path = f'asm/nonmatching/{unit}/{func}.s'
    if not os.path.exists(path):
        die(f'{path} missing')
    lines = open(path).read().split('\n')
    items = [dict(off=a, size=s, src=c) for a, s, c, _ in layout_asm(lines, fa)]
    data = bytes(ROM[fa - BASE:fa - BASE + fsize])
    return items, data, fa, fsize


_CS = None


def disasm(items, data, base):
    global _CS
    import capstone
    if _CS is None:
        _CS = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
    for it in items:
        o = it['off'] - base
        chunk = data[o:o + it['size']]
        ins = next(_CS.disasm(chunk, it['off']), None)
        if ins is None:
            it['text'] = '.hword ' + chunk.hex()
            it['mn'] = '.hword'
        else:
            it['text'] = f'{ins.mnemonic} {ins.op_str}'.strip()
            it['mn'] = ins.mnemonic
        it['norm'] = norm(it['text'], it['mn'])
        # level 1: registers masked but low/high kept apart; level 2: one form per operation, any register
        it['mask1'] = ALLOC_RX.sub(lambda m: 'H' if regnum(m.group(1)) >= 8 else 'L', it['norm'])
        it['regs'] = [regnum(x) for x in ALLOC_RX.findall(it['norm'])]
        canon = canonical(it['norm'])
        it['mask2'] = ALLOC_RX.sub('R', canon)
        it['cregs'] = [regnum(x) for x in ALLOC_RX.findall(canon)]


def norm(text, mn):
    if re.match(r'^b(l|lx|eq|ne|cs|hs|cc|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al)?(\.\w+)?$', mn) and mn != 'bx':
        return mn
    t = re.sub(r'\[pc, #-?0x[0-9a-f]+\]', '[pc]', text)
    t = re.sub(r'\[pc, #-?\d+\]', '[pc]', t)
    t = re.sub(r'^(adr|add)\s+(\w+),\s*pc,\s*#\S+', r'\1 \2, pc', t)
    return t


REG = r'(?:r1[0-5]|r[0-9]|sb|sl|fp|ip|sp|lr|pc)'


def canonical(t):
    """One form per operation whatever registers it uses, so that a value in a high register here and a
    low one in the ROM (or the reverse) still aligns: register copies (`adds rd, rs, #0`, `movs rd, rs`,
    `mov rd, rs`) -> `mov rd, rs`; two-operand high-register add `add rd, rm` -> `adds rd, rd, rm`."""
    m = re.fullmatch(rf'(?:adds|add) ({REG}), ({REG}), #0(?:x0)?', t) or re.fullmatch(rf'movs? ({REG}), ({REG})', t)
    if m:
        return f'mov {m.group(1)}, {m.group(2)}'
    m = re.fullmatch(rf'add ({REG}), ({REG})', t)
    if m and m.group(1) not in ('sp', 'pc') and m.group(2) not in ('sp', 'pc'):
        return f'adds {m.group(1)}, {m.group(1)}, {m.group(2)}'
    m = re.fullmatch(rf'adds ({REG}), ({REG}), ({REG})', t)
    if m:
        return t
    return t


SRC_FIRST = re.compile(r'^(str|strb|strh|cmp|cmn|tst|push|stm|stmia|bx|blx)\b')


# ----------------------------------------------------------------------------------------------
# alignment and votes

def align(tg, bt):
    """Pairs (target index, built index, level) of structurally equal instructions: first with
    registers masked as low/high (level 1), then, inside the blocks that still differ, with one form per
    operation and any register (level 2: a value in a high register on one side and a low one on the
    other). Also the full row list for --show, and the identical/differing line counts."""
    sm = difflib.SequenceMatcher(None, [x['mask1'] for x in tg], [x['mask1'] for x in bt], autojunk=False)
    pairs, rows = [], []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            for k in range(i2 - i1):
                pairs.append((i1 + k, j1 + k, 1))
                rows.append((i1 + k, j1 + k, 1))
            continue
        sub = difflib.SequenceMatcher(None, [x['mask2'] for x in tg[i1:i2]], [x['mask2'] for x in bt[j1:j2]],
                                      autojunk=False)
        for t2, a1, a2, b1, b2 in sub.get_opcodes():
            if t2 == 'equal':
                for k in range(a2 - a1):
                    pairs.append((i1 + a1 + k, j1 + b1 + k, 2))
                    rows.append((i1 + a1 + k, j1 + b1 + k, 2))
            else:
                for k in range(max(a2 - a1, b2 - b1)):
                    rows.append((i1 + a1 + k if a1 + k < a2 else None, j1 + b1 + k if b1 + k < b2 else None, 0))
    diff = difflib.SequenceMatcher(None, [x['norm'] for x in tg], [x['norm'] for x in bt], autojunk=False)
    same = sum(i2 - i1 for tag, i1, i2, j1, j2 in diff.get_opcodes() if tag == 'equal')
    difflines = sum(max(i2 - i1, 0) + max(j2 - j1, 0) for tag, i1, i2, j1, j2 in diff.get_opcodes() if tag != 'equal')
    return pairs, same, difflines, rows


def diff_lines(tg, bt):
    d = difflib.SequenceMatcher(None, [x['norm'] for x in tg], [x['norm'] for x in bt], autojunk=False)
    return sum((i2 - i1) + (j2 - j1) for tag, i1, i2, j1, j2 in d.get_opcodes() if tag != 'equal')


COMMUTATIVE = {'adds', 'add', 'ands', 'orrs', 'eors', 'muls', 'mul', 'adcs', 'and', 'orr', 'eor'}


def operand_pseudos(tr, b, regs=None):
    """For each register operand of a built instruction: ('p', pseudo, role) | ('reload', reload dict) |
    None. Uses the final-time operand map (FI records: which pseudo's REG rtx each hard register
    operand is), else the pre-reload insn and the final dispositions."""
    uid = b.get('uid')
    regs = b['regs'] if regs is None else regs
    b = dict(b, regs=regs)
    out = [None] * len(b['regs'])
    if uid is None:
        return out
    main = tr.reload_of.get(uid)
    fi = tr.FI.get(uid)
    if fi is not None:
        srcfirst = bool(SRC_FIRST.match(b['mn']))
        rl = {x['reg']: x for x in tr.RL.get(main if main is not None else uid, []) if x['reg'] >= 0}
        for k, br in enumerate(b['regs']):
            ents = [e for e in fi if e[1] == br]
            ps = list(dict.fromkeys(e[0] for e in ents if e[0] is not None))
            if len(ps) > 1:
                want = 's' if (k == 0 and not srcfirst) else 'u'
                ps = list(dict.fromkeys(e[0] for e in ents if e[0] is not None and e[2] == want))
            if len(ps) == 1:
                roles = ''.join(sorted(set(e[2] for e in ents if e[0] == ps[0])))
                out[k] = ('p', ps[0], roles)
            elif not ps and ents and br in rl:
                out[k] = ('reload', rl[br])
        return out
    d = tr.insn.get(uid)
    if d is not None:
        sets, uses = d.get('s', []), d.get('u', [])
        cands = list(dict.fromkeys(sets + uses))
        srcfirst = bool(SRC_FIRST.match(b['mn']))
        rl = {x['reg']: x for x in tr.RL.get(uid, []) if x['reg'] >= 0}
        for k, br in enumerate(b['regs']):
            cs = [p for p in cands if tr.final_reg(p) == br or
                  (tr.final_reg(p) >= 0 and tr.A.get(tr.allocno_of.get(p, -1), {}).get('size', 1) > 1
                   and tr.final_reg(p) + 1 == br)]
            if len(cs) > 1:
                if k == 0 and not srcfirst:
                    cs = [p for p in cs if p in sets]
                else:
                    cs = [p for p in cs if p in uses]
            if len(cs) == 1:
                out[k] = ('p', cs[0], ('s' if cs[0] in sets else '') + ('u' if cs[0] in uses else ''))
            elif not cs and br in rl:
                out[k] = ('reload', rl[br])
    elif main is not None:
        rl = {x['reg']: x for x in tr.RL.get(main, []) if x['reg'] >= 0}
        for k, br in enumerate(b['regs']):
            if br in rl:
                out[k] = ('reload', rl[br])
    return out


def show(tr, namer, tg, bt, lo, hi, src_lines, rows):
    """Aligned listing of target [lo, hi) with the built code, insn uids, source lines and the pseudo
    behind every built register operand. Marks: ' ' identical, 'r' same but other registers, '~' same
    operation with a high register on one side, '*' different."""
    out = []
    last_t = tg[0]['off']
    for ti, bj, lvl in rows:
        if ti is not None:
            last_t = tg[ti]['off']
        if not (lo <= last_t < hi):
            continue
        if True:
            t = tg[ti] if ti is not None else None
            b = bt[bj] if bj is not None else None
            mark = '*' if not lvl else (' ' if t['norm'] == b['norm'] else ('r' if lvl == 1 else '~'))
            ann = ''
            if b is not None:
                ops = operand_pseudos(tr, b)
                names = []
                for br, op in zip(b['regs'], ops):
                    if op and op[0] == 'p':
                        names.append(f'{rname(br)}={namer.describe(op[1])[:28]}')
                    elif op:
                        x = op[1]['in'] if op[1]['in'] != 'nil' else op[1]['out']
                        names.append(f'{rname(br)}=reload({namer.expr(parse_rtx(x), 1)[:28]})')
                ln = tr.line.get(b.get('uid'), 0) if b.get('uid') is not None else 0
                ann = (f'#{b["uid"]} L{ln} ' if b.get('uid') is not None else '') + ' '.join(dict.fromkeys(names))
            out.append(f"{mark} {('%08X ' % t['off']) if t else ' ' * 9}{(t['norm'] if t else ''):28s} | "
                       f"{(b['norm'] if b else ''):28s} {ann}")
    return '\n'.join(out)


def collect_votes(tr, tg, bt, pairs, rng=None):
    """Votes pseudo -> {ROM hard reg: [ROM addrs]} from structurally equal instruction pairs, plus
    reload-register pairs. The two sources of a commutative instruction may be swapped in the ROM;
    their orientation is decided by the votes of all other operands."""
    votes = {}
    rpairs = []
    groups = []

    def vote(p, reg, addr):
        votes.setdefault(p, {}).setdefault(reg, []).append(addr)

    def rom_home(ti, trg, is_def):
        """A ROM value used through a low-register copy of a high register (`mov rL, rH` just before,
        or defined and then copied up `mov rH, rL`) lives in rH there."""
        if trg >= 8:
            return trg
        if not is_def:
            for k in (1, 2):
                if ti - k < 0:
                    break
                x = tg[ti - k]
                m = re.fullmatch(r'mov (\w+), (\w+)', canonical(x['norm']))
                if m and regnum(m.group(1)) == trg and 8 <= regnum(m.group(2)) <= 12:
                    return regnum(m.group(2))
                if x['regs'][:1] == [trg] and not SRC_FIRST.match(x['mn']):
                    break
        elif ti + 1 < len(tg):
            m = re.fullmatch(r'mov (\w+), (\w+)', canonical(tg[ti + 1]['norm']))
            if m and regnum(m.group(2)) == trg and 8 <= regnum(m.group(1)) <= 12:
                return regnum(m.group(1))
        return trg
    for ti, bi, lvl in pairs:
        t, b = tg[ti], bt[bi]
        if rng and not (rng[0] <= t['off'] < rng[1]):
            continue
        tregs = t['regs'] if lvl == 1 else t['cregs']
        bregs = b['regs'] if lvl == 1 else b['cregs']
        if b.get('uid') is None or len(tregs) != len(bregs):
            continue
        ops = operand_pseudos(tr, b, bregs)
        comm = b['mn'] in COMMUTATIVE and '#' not in b['norm'] and len(bregs) >= 2
        uses = {}
        for k, (op, trg) in enumerate(zip(ops, tregs)):
            if op is None:
                continue
            if op[0] == 'reload':
                rpairs.append(dict(uid=op[1]['uid'], reg=bregs[k], treg=trg, addr=t['off'], reload=op[1]))
                continue
            p, roles = op[1], op[2]
            if b['regs'] and bregs[k] < 8:
                trg = rom_home(ti, trg, 's' in roles and k == 0 and not SRC_FIRST.match(b['mn']))
            if comm and roles == 'u':
                uses.setdefault(p, []).append(trg)
            elif comm and 's' in roles and k == 0:
                vote(p, trg, t['off'])     # the destination is never swapped
                if 'u' in roles:
                    uses.setdefault(p, []).append(trg)
            else:
                vote(p, trg, t['off'])
        if comm and len(uses) == 2:
            (u1, t1), (u2, t2) = uses.items()
            groups.append((u1, t1, u2, t2, t['off']))
        else:
            for p, ts in uses.items():
                for trg in ts:
                    vote(p, trg, t['off'])

    # a pseudo that is in memory here but whose reloads all land in one register in the ROM: the ROM
    # probably kept it in that register (its reloads align with plain register uses there)
    sites = {}
    for rp in rpairs:
        rl = rp['reload']
        for side in ('in', 'out'):
            m = re.fullmatch(r'\(reg:\w+ (\d+)\)', rl[side] or '')
            if m and int(m.group(1)) >= tr.GSET.get('nregs', 17) and tr.final_reg(int(m.group(1))) < 0:
                sites.setdefault(int(m.group(1)), []).append((rp['reg'], rp['treg'], rp['addr'], rp['uid']))
    # our reload insns for p (stack loads, constant rematerialisation) that align with ROM instructions
    # mean the ROM reloads p there too, i.e. it has no register home in the ROM either
    aligned_b = {bi for ti, bi, lvl in pairs}
    by_uid = {}
    for bi, b in enumerate(bt):
        if b.get('uid') is not None:
            by_uid.setdefault(b['uid'], []).append(bi)
    for p, ss in sites.items():
        troms = set(t for b, t, a, u in ss)
        if len(troms) != 1 or not any(b != t for b, t, a, u in ss):
            continue
        on_stack = False
        for b, t, a, u in ss:
            for ru in tr.RLI.get(u, []):
                for bi in by_uid.get(ru, []):
                    if bi in aligned_b and bt[bi]['regs'][:1] == [b]:
                        on_stack = True
        if on_stack:
            continue
        for b, t, a, u in ss:
            vote(p, t, a)

    def support(p, regs):
        v = votes.get(p, {})
        return sum(len(v.get(r, [])) for r in set(regs))
    for u1, t1, u2, t2, addr in groups:
        same = support(u1, t1) + support(u2, t2)
        swap = support(u1, t2) + support(u2, t1)
        if swap > same:
            t1, t2 = t2, t1
        for trg in t1:
            vote(u1, trg, addr)
        for trg in t2:
            vote(u2, trg, addr)
    return votes, rpairs


def confident(nv, ntot, built):
    """Enough votes to call the ROM register: a strict majority; for a pseudo that is in memory here
    (votes come from its reload registers) at least 2 votes and 3/4 of them."""
    if built < 0:
        return nv >= 2 and nv * 4 >= ntot * 3
    return nv * 2 > ntot


def target_regs(tr, votes):
    """pseudo -> (target reg, votes for it, total votes)."""
    out = {}
    for p, v in votes.items():
        best = max(v.items(), key=lambda kv: (len(kv[1]), -kv[0]))
        out[p] = (best[0], len(best[1]), sum(len(x) for x in v.values()), min(min(x) for x in v.values()))
    return out


# ----------------------------------------------------------------------------------------------
# global-alloc simulator (exact port of global.c: prune_preferences + find_reg + the allocation loop)

class Sim:
    def __init__(self, tr, prio_override=None, force=None):
        self.tr = tr
        A = tr.A
        g = tr.GSET
        self.n = len(A)
        self.NR = g['nregs']
        self.ALL = (1 << self.NR) - 1
        self.cls = {c: m for c, (nm, m) in tr.GC.items()}
        self.clsname = {c: nm for c, (nm, m) in tr.GC.items()}
        self.rcls = {r: d['cls'] for r, d in tr.GR.items()}
        self.conflist = [tr.C.get(i, []) for i in range(self.n)]
        self.confset = [set(c) for c in self.conflist]
        self.refs = [A[i]['refs'] for i in range(self.n)]
        self.live = [A[i]['live'] if A[i]['live'] != 0 else -1 for i in range(self.n)]
        self.size = [A[i]['size'] or 1 for i in range(self.n)]
        self.calls = [A[i]['calls'] for i in range(self.n)]
        self.regno = [A[i]['regno'] for i in range(self.n)]
        self.local = [A[i]['local'] for i in range(self.n)]
        self.pref, self.cpref, self.fpref = [], [], []
        for i in range(self.n):
            a = A[i]
            temp = a['hconf'] | (g['fixed'] if a['calls'] == 0 else g['call_used'])
            temp |= ~self.cls[a['pclass']] & 0xFFFFFFFF
            self.pref.append(a['pref'] & ~temp)
            self.cpref.append(a['cpref'] & ~temp)
            self.fpref.append(a['fpref'] & ~temp)
        self.prefconf = [[b for b in self.conflist[i] if self.fpref[b]] for i in range(self.n)]
        self.base_prio = [self.prio_of(self.refs[i], self.live[i], self.size[i]) for i in range(self.n)]
        for r, v in (prio_override or {}).items():   # what-if priorities the compile was made with
            if r in tr.allocno_of:
                self.base_prio[tr.allocno_of[r]] = v
        self.force = {tr.allocno_of[r]: h for r, h in (force or {}).items() if r in tr.allocno_of}
        self.globals = [i for i in range(self.n) if self.local[i] < 0 and A[i]['reglive'] >= 0]

    @staticmethod
    def prio_of(refs, live, size):
        if live == 0:
            live = -1
        fl = refs.bit_length() - 1 if refs > 0 else -1
        return int(((fl * refs) / live) * 10000 * size)

    def order(self, prio):
        return sorted(range(self.n), key=lambda i: (-prio[i], i))

    def compat(self, i, j):
        ci, cj = self.rcls[i], self.rcls[j]
        mi, mj = self.cls[ci], self.cls[cj]
        return ci == cj or (mi & ~mj) == 0 or (mj & ~mi) == 0

    def run(self, order, trace=None, blockers=False):
        tr = self.tr
        A = tr.A
        g = tr.GSET
        n = self.n
        pos = [0] * n
        for k, a in enumerate(order):
            pos[a] = k
        someone = [0] * n
        for a in range(n):
            s = 0
            fa = self.fpref[a]
            pa = pos[a]
            for b in self.prefconf[a]:
                if pos[b] > pa:
                    t = self.fpref[b]
                    if self.size[b] <= self.size[a]:
                        t &= ~fa
                    s |= t
            someone[a] = s
        renum = {}
        for i in range(n):
            if self.local[i] >= 0:
                renum[self.regno[i]] = self.local[i]
        st = dict(used_so_far=g['used_so_far'])
        lrefs = [tr.GR[r]['lrefs'] for r in range(self.NR)]
        llive = [tr.GR[r]['llive'] for r in range(self.NR)]
        hconf = [A[i]['hconf'] for i in range(n)]
        cpref = list(self.cpref)
        pref = list(self.pref)
        via = {}
        taker = {} if blockers else None   # allocno -> {hard reg: earlier allocno that took it}
        info = {}
        processed = set()

        def find_reg(a, losers, alt, acc, retrying):
            cl = A[a]['aclass'] if alt else A[a]['pclass']
            if acc:
                used1 = g['call_fixed']
            elif self.calls[a] == 0:
                used1 = g['fixed']
            else:
                used1 = g['call_used']
            used1 |= g['no_global']
            if losers:
                used1 |= losers
            used1 |= ~self.cls[cl] & self.ALL
            used2 = used1
            used1 |= hconf[a]
            nopref = used1 | (~st['used_so_far'] & self.ALL) | someone[a]
            if blockers and not alt and not acc:
                info[a] = dict(used1=used1, nopref=nopref, used_so_far=st['used_so_far'], someone=someone[a],
                               cls=cl)
            modeok = A[a]['modeok']
            multi = A[a]['multi']
            best = -1
            used = used1
            how = 'none'
            for pas in (0, 1):
                used = nopref if pas == 0 else used1
                i = 0
                while i < self.NR:
                    if not (used >> i) & 1 and (modeok >> i) & 1:
                        lim = i + (2 if (multi >> i) & 1 else 1)
                        j = i + 1
                        while j < lim and not (used >> j) & 1:
                            j += 1
                        if j == lim:
                            best = i
                            break
                        i = j
                    i += 1
                if best >= 0:
                    how = 'pass%d' % pas
                    break
            cpref[a] &= ~used
            done = False
            if cpref[a] and best >= 0:
                for i in range(self.NR):
                    if (cpref[a] >> i) & 1 and (modeok >> i) & 1 and self.compat(i, best):
                        lim = i + (2 if (multi >> i) & 1 else 1)
                        j = i + 1
                        while j < lim and not (used >> j) & 1 and self.compat(j, best + (j - i)):
                            j += 1
                        if j == lim:
                            best, how, done = i, 'cpref', True
                            break
            if not done:
                pref[a] &= ~used
                if pref[a] and best >= 0:
                    for i in range(self.NR):
                        if (pref[a] >> i) & 1 and (modeok >> i) & 1 and self.compat(i, best):
                            lim = i + (2 if (multi >> i) & 1 else 1)
                            j = i + 1
                            while j < lim and not (used >> j) & 1 and self.compat(j, best + (j - i)):
                                j += 1
                            if j == lim:
                                best, how = i, 'pref'
                                break
            if g['caller_saves'] and best < 0:
                if not acc and self.calls[a] and 4 * self.calls[a] < self.refs[a]:
                    find_reg(a, (losers or 0) | g['losing_cs'], alt, True, retrying)
                    if renum.get(self.regno[a], -1) >= 0:
                        via[a] = 'callersave'
                        if trace is not None:
                            trace.append((a, int(alt), 1 if acc else 0, renum[self.regno[a]], 'callersave'))
                        return
            if best < 0 and not retrying and self.size[a] == 1:
                for r in range(self.NR - 1, -1, -1):
                    if lrefs[r] and not (used2 >> r) & 1 and (modeok >> r) & 1:
                        if lrefs[r] / llive[r] < self.refs[a] / self.live[a]:
                            for k in list(renum):
                                if renum[k] == r:
                                    renum[k] = -1
                            best, how = r, 'kick'
                            break
            if trace is not None:
                trace.append((a, int(alt), 1 if acc else 0, best, how))
            if best >= 0:
                renum[self.regno[a]] = best
                nr = 2 if (multi >> best) & 1 else 1
                bits = 0
                for j in range(best, best + nr):
                    bits |= 1 << j
                    st['used_so_far'] |= 1 << j
                    lrefs[j] = 0
                for b in self.conflist[a]:
                    if blockers and b not in processed and not (hconf[b] & bits):
                        taker.setdefault(b, {})
                        for j in range(best, best + nr):
                            taker[b].setdefault(j, a)
                    hconf[b] |= bits
                via[a] = how

        for a in order:
            r = self.regno[a]
            processed.add(a)
            if renum.get(r, -1) >= 0 or A[a]['reglive'] < 0:
                continue
            if a in self.force:   # REGORACLE_FORCE: only that hard reg is allowed
                losers = 0xFFFFFFFF & ~(1 << self.force[a])
                find_reg(a, losers, False, False, False)
                if renum.get(r, -1) < 0:
                    find_reg(a, losers, True, False, False)
                continue
            find_reg(a, 0, False, False, False)
            if renum.get(r, -1) < 0 and A[a]['aclass'] != 0:
                find_reg(a, 0, True, False, False)
        if blockers:
            return renum, via, taker, info
        return renum, via

    def check_against_compiler(self):
        """Replay the compiler's own order; every find_reg decision must agree."""
        trace = []
        renum, via = self.run(self.tr.order, trace)
        comp = [(d['a'], d['alt'], d['acc'], d['best'], d['via']) for d in self.tr.T if d['retry'] == 0]
        bad = sum(1 for x, y in zip(comp, trace) if x != y) + abs(len(comp) - len(trace))
        q = self.tr.Q
        dq = [r for r in set(q) | {k for k, v in renum.items() if v >= 0} if q.get(r, -1) != renum.get(r, -1)]
        return len(comp), bad, dq


# ----------------------------------------------------------------------------------------------
# inverse solve

def solve(sim, want, budget=6000, log=None, weight=None):
    """want: allocno -> target hard reg. Search priority overrides so that the simulated allocation
    gives the wanted registers. Returns dict(prio=new priorities, moved={allocno: (old, new)},
    bad=[allocnos still wrong], sims=n, hard={allocno: reason})."""
    tr = sim.tr
    base = list(sim.base_prio)
    hard = {}
    renum_now, _ = sim.run(sim.order(base))
    for a, w in want.items():
        if (tr.A[a]['hconf'] >> w) & 1 and renum_now.get(sim.regno[a], -1) != w:
            hard[a] = 'hconf'
    W = {a: w for a, w in want.items() if a not in hard}
    wt = {a: (weight or {}).get(a, 1) for a in W}
    count = [0]

    def evaluate(prio):
        count[0] += 1
        order = sim.order(prio)
        renum, via, taker, info = sim.run(order, blockers=True)
        bad = [a for a in W if renum.get(sim.regno[a], -1) != W[a]]
        return dict(prio=prio, order=order, renum=renum, via=via, taker=taker, info=info, bad=bad)

    def cost(prio):
        c = 0.0
        moved = 0
        for a in range(sim.n):
            if prio[a] != base[a]:
                moved += 1
                c += abs(math.log((abs(prio[a]) + 1) / (abs(base[a]) + 1)))
        return moved, c

    def key(ev):
        moved, c = cost(ev['prio'])
        return (sum(wt[a] for a in ev['bad']), moved, c)

    def moves(ev, cap):
        order, renum, taker, info = ev['order'], ev['renum'], ev['taker'], ev['info']
        pos = {a: k for k, a in enumerate(order)}
        prio = ev['prio']
        out = []
        badsorted = sorted(ev['bad'], key=lambda a: pos[a])
        for a in badsorted[:cap]:
            w = W[a]
            s = renum.get(sim.regno[a], -1)
            # 1) someone earlier took w (it was marked as a conflict by that allocno)
            b = (taker.get(a) or {}).get(w)
            if b is not None:
                out.append({a: prio[b] + 1})
                out.append({b: prio[a] - 1})
                # also try moving a just above every earlier conflicting holder of w
            # 2) a got another free reg s first: someone that should hold s must come first (one the
            #    ROM puts in s, or the nearest later conflicting allocnos whose ROM register is unknown)
            if s >= 0 and s != w:
                unknown = 0
                for c in order[pos[a] + 1:]:
                    if c not in sim.confset[a]:
                        continue
                    wc = W.get(c)
                    if wc == s or (wc is None and c in globset and unknown < 6):
                        if wc is None:
                            unknown += 1
                        out.append({c: prio[a] + 1})
                        out.append({a: prio[c] - 1})
            # 2b) a must come after every conflicting allocno the ROM puts in a register a would try
            #     before w (all lower registers of its class, and s)
            if s != w:
                lower = [c for c in sim.conflist[a] if c in W and (W[c] < w or W[c] == s) and W[c] >= 0]
                if lower:
                    out.append({a: min(prio[c] for c in lower) - 1})
                    for c in sorted(lower, key=lambda c: prio[c])[:4]:
                        out.append({a: prio[c] - 1})
            # 3) w not used so far (pass 0 avoids never-used regs): someone holding w must come first
            inf = info.get(a)
            if inf and not (inf['used_so_far'] >> w) & 1:
                for c in order[pos[a] + 1:]:
                    if renum.get(sim.regno[c], -1) == w:
                        out.append({c: prio[a] + 1})
                        break
            # 4) a conflicting allocno that prefers s: once a is above it, s is in a's regs_someone_prefers
            #    and pass 0 skips it
            if s >= 0 and s != w:
                for c in sim.conflist[a]:
                    if (sim.fpref[c] >> s) & 1 and not (sim.fpref[a] >> s) & 1:
                        out.append({a: prio[c] + 1})
            # 5) generic nudges
            for k in (1, 3, 10, 30):
                if pos[a] - k >= 0:
                    out.append({a: prio[order[pos[a] - k]] + 1})
                if pos[a] + k < len(order):
                    out.append({a: prio[order[pos[a] + k]] - 1})
        return out

    globset = set(sim.globals)
    start = evaluate(list(base))
    best = start
    beam = [start]
    seen = {()}
    stall = 0
    while count[0] < budget and best['bad'] and stall < 4:
        cand = []
        for bi, ev in enumerate(beam):
            for mv in moves(ev, 80 if bi == 0 else 10):
                prio = list(ev['prio'])
                for a, v in mv.items():
                    prio[a] = v
                sig = tuple(sorted((a, p) for a, p in enumerate(prio) if p != base[a]))
                if sig in seen:
                    continue
                seen.add(sig)
                cand.append(evaluate(prio))
                if count[0] >= budget:
                    break
            if count[0] >= budget:
                break
        if not cand:
            break
        cand.sort(key=key)
        if key(cand[0]) >= key(best) and count[0] < budget:
            # no step helped: try every distinct priority for the first wrong allocnos (exhaustive 1-moves)
            ev = beam[0]
            pos = {x: k for k, x in enumerate(ev['order'])}
            vals = sorted(set(ev['prio']))
            for a in sorted(ev['bad'], key=lambda x: pos[x])[:4]:
                for q in vals:
                    for v in (q - 1, q + 1):
                        prio = list(ev['prio'])
                        prio[a] = v
                        sig = tuple(sorted((x, p) for x, p in enumerate(prio) if p != base[x]))
                        if sig in seen:
                            continue
                        seen.add(sig)
                        cand.append(evaluate(prio))
                        if count[0] >= budget:
                            break
                    if count[0] >= budget:
                        break
            cand.sort(key=key)
        beam = cand[:4]
        if key(beam[0]) < key(best):
            best = beam[0]
            stall = 0
        else:
            stall += 1
        if log:
            log(f'  solve: {count[0]} sims, best {len(best["bad"])} wrong / {len(W)}')
    # drop moves that do not matter
    prio = list(best['prio'])
    for a in [i for i in range(sim.n) if prio[i] != base[i]]:
        trial = list(prio)
        trial[a] = base[a]
        ev = evaluate(trial)
        if key(ev)[0] <= key(best)[0]:
            prio = trial
            best = ev
    moved = {a: (base[a], prio[a]) for a in range(sim.n) if prio[a] != base[a]}
    return dict(prio=prio, moved=moved, bad=best['bad'], start_bad=start['bad'], sims=count[0], hard=hard,
                want=W, final=best, start=start)


def window(sim, prio, a, ok, span=300):
    """Integer priority range for allocno a (others fixed) over which ok(renum) stays true. The order
    only changes when a crosses another allocno's priority q (ties: lower allocno first), so testing
    q-1, q, q+1 for every other q gives the exact bounds. None = not bounded within the scan."""
    vals = sorted(set(p for i, p in enumerate(prio) if i != a))
    cur = prio[a]
    cands = sorted(set([cur] + [v for q in vals for v in (q - 1, q, q + 1)]))
    k0 = cands.index(cur)
    res = {}

    def test(v):
        if v not in res:
            pr = list(prio)
            pr[a] = v
            renum, via = sim.run(sim.order(pr))
            res[v] = ok(renum)
        return res[v]
    if not test(cur):
        return None
    lo = hi = k0
    while lo > 0 and k0 - lo < span and test(cands[lo - 1]):
        lo -= 1
    while hi + 1 < len(cands) and hi - k0 < span and test(cands[hi + 1]):
        hi += 1
    lo_v = cands[lo] if lo > 0 and not test(cands[lo - 1]) else None
    hi_v = cands[hi] if hi + 1 < len(cands) and not test(cands[hi + 1]) else None
    return lo_v, hi_v


def overlap_lines(tr, p, q):
    a, b = set(tr.live.get(p, [])), set(tr.live.get(q, []))
    both = a & b
    lines = sorted(set(tr.line.get(u, 0) for u in both))
    return len(both), lines


def explain(tr, sim, namer, al, w, ev, W):
    """Why the simulated allocation cannot give allocno al the ROM's register w by reordering."""
    p = sim.regno[al]
    renum, taker, info = ev['renum'], ev['taker'], ev['info']
    got = renum.get(p, -1)
    if (tr.A[al]['hconf'] >> w) & 1:
        locs = [b for b in sim.conflist[al] if sim.local[b] == w]
        return dict(kind='hard', text=f'{rname(w)} is a hard conflict here (' +
                    (', '.join(f'local {sim.regno[b]} {namer.describe(sim.regno[b])[:30]}' for b in locs[:3])
                     if locs else 'a hard register live across it') + ')')
    b = (taker.get(al) or {}).get(w)
    if b is not None:
        q = sim.regno[b]
        if W.get(b) == w:
            n, lines = overlap_lines(tr, p, q)
            return dict(kind='rom-no-conflict', other=q,
                        text=f'{q} {namer.describe(q)[:40]} is also in {rname(w)} in the ROM, so the two do not '
                             f'overlap there; here both are live at {n} insns, lines {lines[:8]}')
        return dict(kind='blocked', other=q,
                    text=f'{rname(w)} taken first by {q} {namer.describe(q)[:40]} (ROM: '
                         f'{rname(W[b]) if b in W else "?"}), and moving either breaks more than it fixes')
    if got >= 0 and got != w:
        busy = [c for c in range(sim.n) if W.get(c) == got and c != al]
        conf = [c for c in busy if c in sim.confset[al]]
        # pseudos the ROM has in `got` whose life ends or starts within a few insns of this one's
        mine = sorted(tr.pos[u] for u in tr.live.get(p, []) if u in tr.pos)
        near = []
        if mine:
            import bisect
            for c in busy:
                if c in sim.confset[al]:
                    continue
                theirs = [tr.pos[u] for u in tr.live.get(sim.regno[c], []) if u in tr.pos]
                dmin = 10 ** 9
                for x in theirs:
                    k = bisect.bisect_left(mine, x)
                    for y in mine[max(0, k - 1):k + 1]:
                        dmin = min(dmin, abs(x - y))
                if dmin <= 8:
                    near.append((dmin, c))
        near.sort()
        nonconf = [c for d, c in near[:3]]
        via = ev['via'].get(al)
        t = f'{rname(w)} is free but {rname(got)} comes first ({via})'
        if conf:
            t += '; the ROM puts conflicting ' + ', '.join(f'{sim.regno[c]} {namer.describe(sim.regno[c])[:30]}' for c in conf[:3]) + f' in {rname(got)}'
        else:
            if nonconf:
                t += f'; in the ROM {rname(got)} is busy during its life: nearest candidates (ROM {rname(got)}, live next to it but not overlapping here): ' + ', '.join(
                    f'{sim.regno[c]} {namer.describe(sim.regno[c])[:40]} ({d} insns apart)' for d, c in near[:3]) + ' (an earlier start or later end of one of them, i.e. a different statement/insn order, would block it)'
            else:
                t += f'; in the ROM {rname(got)} must be busy during its life (a value our code does not have live there)'
        return dict(kind='lower-free', text=t)
    return dict(kind='other', text=f'got {rname(got)}')


def levers(refs, live, size, lo, hi):
    """Smallest refs-only and live-only changes that put the priority into [lo, hi]."""
    p = Sim.prio_of
    out = {}
    lo = -10 ** 9 if lo is None else lo
    hi = 10 ** 9 if hi is None else hi
    for k in sorted(range(-refs + 1, 400), key=abs):
        if lo <= p(refs + k, live, size) <= hi:
            out['refs'] = k
            break
    for k in sorted(range(-live + 1, 20000), key=abs):
        if live + k != 0 and lo <= p(refs, live + k, size) <= hi:
            out['live'] = k
            break
    return out


# ----------------------------------------------------------------------------------------------
# explanations

def refs_by_line(tr, r):
    agg = {}
    for uid, depth, d in tr.refs.get(r, []):
        ln = tr.line.get(uid, 0)
        x = agg.setdefault((ln, depth), 0)
        agg[(ln, depth)] = x + d
    return sorted(agg.items(), key=lambda kv: (-kv[1], kv[0]))


def live_span(tr, r):
    uids = [u for u in tr.live.get(r, []) if u in tr.pos and tr.line.get(u, tr.uline.get(u, 0))]
    if not uids:
        return None
    lines = [tr.line.get(u, tr.uline.get(u, 0)) for u in uids]
    byline = {}
    for ln in lines:
        byline[ln] = byline.get(ln, 0) + 1
    ps = sorted(tr.pos.get(u, 0) for u in uids)
    first = min(uids, key=lambda u: tr.pos.get(u, 0))
    last = max(uids, key=lambda u: tr.pos.get(u, 0))
    return dict(n=len(tr.live.get(r, [])), first=tr.line.get(first, 0), last=tr.line.get(last, 0),
                lines=(min(lines), max(lines)), top=sorted(byline.items(), key=lambda kv: -kv[1])[:4])


def case_of_line(src_lines, ln):
    """Nearest enclosing `case X:` label above a source line (for orientation)."""
    for k in range(min(ln, len(src_lines)) - 1, -1, -1):
        m = re.match(r'\s*case\s+([^:]+):', src_lines[k])
        if m:
            return 'case ' + m.group(1).strip()
        if re.match(r'^\S.*\)\s*$', src_lines[k]) or src_lines[k].startswith('{'):
            return None
    return None


def rname(h):
    return {-1: 'mem', 9: 'r9', 10: 'sl', 11: 'fp', 12: 'ip', 13: 'sp', 14: 'lr', 15: 'pc'}.get(h, f'r{h}')


# ----------------------------------------------------------------------------------------------
# main analysis

def analyse(a):
    unit, func = a.unit, a.func
    src = a.src or (f'build/wf/{func}/unit.c' if os.path.exists(f'build/wf/{func}/unit.c') else f'src/{unit}.c')
    wdir = f'{BUILD}/{func}'
    log = (lambda m: print(m, file=sys.stderr)) if not a.quiet else (lambda m: None)
    prio_ovr = dict((int(x.split('=')[0]), int(x.split('=')[1])) for x in a.prio.split(',')) if a.prio else None
    force_ovr = dict((int(x.split('=')[0]), int(x.split('=')[1])) for x in a.force.split(',')) if a.force else None
    asm, trp = compile_traced(unit, func, src, wdir, prio_ovr, force_ovr)
    tr = Trace(trp)
    namer = Namer(tr)
    src_lines = open(src).read().split('\n')
    tg, tdata, fa, fsize = target_function(unit, func)
    disasm(tg, tdata, fa)
    bt = built_function(asm, func)
    bdata, bsize = assemble(asm, func, wdir)
    disasm(bt, bdata, 0)
    rng = (int(a.range[0], 0), int(a.range[1], 0)) if a.range else None
    pairs, same, dlines, rows_al = align(tg, bt)
    votes, rpairs = collect_votes(tr, tg, bt, pairs, rng)
    tregs = target_regs(tr, votes)
    # target addresses next to code that differs structurally (or paired only at level 2)
    near = set()
    for k, (ti, bj, lvl) in enumerate(rows_al):
        if lvl != 1:
            for kk in range(max(0, k - 2), min(len(rows_al), k + 3)):
                if rows_al[kk][0] is not None:
                    near.add(tg[rows_al[kk][0]]['off'])
    sim = Sim(tr, prio_ovr, force_ovr)
    ncalls, simbad, simdq = sim.check_against_compiler()
    if sim.order(sim.base_prio) != tr.order:
        simbad += 1

    R = dict(unit=unit, func=func, src=src, built_size=bsize, target_size=fsize,
             instr_target=len(tg), instr_built=len(bt), aligned=len(pairs), same=same, diff_lines=dlines,
             allocnos=sim.n, globals=len(sim.globals), find_reg_calls=ncalls, sim_mismatch=simbad,
             sim_disp_mismatch=len(simdq), range=a.range)

    pos = {al: k for k, al in enumerate(tr.order)}
    rows = []
    for p, (t, nv, ntot, addr) in tregs.items():
        built = tr.final_reg(p)
        if t == built or not confident(nv, ntot, built):
            continue
        al = tr.allocno_of.get(p)
        kind = 'global'
        if al is None:
            kind = '?'
        elif sim.local[al] >= 0:
            kind = 'local'
        elif built < 0:
            kind = 'spilled'
        prereload = tr.Q.get(p, -1)
        if kind == 'global' and prereload != built:
            kind = 'reload-changed'
        uid = namer.first_def(p)
        ln = tr.line.get(uid, 0) if uid else 0
        vaddr = votes[p][t]
        rows.append(dict(pseudo=p, allocno=al, name=namer.describe(p), kind=kind, built=built, target=t,
                         votes=nv, total=ntot, addr=addr, order=pos.get(al), line=ln,
                         near_diff=sum(1 for x in vaddr if x in near) * 2 > len(vaddr),
                         case=case_of_line(src_lines, ln) if ln else None,
                         refs=tr.A[al]['refs'] if al is not None else None,
                         live=tr.A[al]['live'] if al is not None else None,
                         prio=sim.base_prio[al] if al is not None else None,
                         via=None))
    agree = sum(1 for p, (t, nv, ntot, addr) in tregs.items() if t == tr.final_reg(p))
    R['pseudos_unsure'] = sum(1 for p, (t, nv, ntot, addr) in tregs.items()
                              if t != tr.final_reg(p) and not confident(nv, ntot, tr.final_reg(p)))
    R['pseudos_with_target'] = len(tregs)
    R['pseudos_agree'] = agree
    R['mismatches'] = rows
    trace = []
    renum0, via0 = sim.run(tr.order)
    for row in rows:
        if row['allocno'] is not None:
            row['via'] = via0.get(row['allocno'])

    # inverse solve over the global allocnos
    want = {}
    weight = {}
    globset = set(sim.globals)
    for p, (t, nv, ntot, addr) in tregs.items():
        al = tr.allocno_of.get(p)
        if al is None or al not in globset or t < 0 or t > 12:
            continue
        q, z = tr.Q.get(p, -1), tr.final_reg(p)
        if not confident(nv, ntot, z):
            continue
        weight[al] = min(nv, 8)
        if q == z:
            want[al] = t
        elif t == z and q >= 0:
            want[al] = q      # reload moved it the same way in both: keep the pre-reload register
    R['solve'] = None
    R['want'] = {sim.regno[al]: w for al, w in want.items()}
    if not a.no_solve and any(want[al] != renum0.get(sim.regno[al], -1) for al in want):
        t0 = time.time()
        sol = solve(sim, want, a.budget, log, weight)
        ev = sol['final']
        S = dict(constraints=len(sol['want']), wrong_before=len(sol['start_bad']), wrong_after=len(sol['bad']),
                 sims=sol['sims'], seconds=round(time.time() - t0, 1), moves=[], hard=[], still_wrong=[])
        fixed = set(sol['start_bad']) - set(sol['bad'])
        broken = set(sol['bad']) - set(sol['start_bad'])
        okset = set(sol['want']) - set(sol['bad'])

        def ok(renum):
            return all(renum.get(sim.regno[x], -1) == sol['want'][x] for x in okset)
        neworder = sim.order(sol['prio'])
        npos = {x: k for k, x in enumerate(neworder)}
        for al, (old, new) in sorted(sol['moved'].items(), key=lambda kv: npos[kv[0]]):
            p = sim.regno[al]
            win = window(sim, sol['prio'], al, ok)
            lo, hi = win if win else (new, new)
            lv = levers(sim.refs[al], sim.live[al], sim.size[al], lo, hi)
            # whom it passes: allocnos between the old and the new position that conflict with it
            passed = []
            for x in neworder:
                if x == al or x not in sim.confset[al]:
                    continue
                was_before = (sim.base_prio[x], -x) > (old, -al)
                now_before = (sol['prio'][x], -x) > (new, -al)
                if was_before != now_before:
                    passed.append(dict(pseudo=sim.regno[x], name=namer.describe(sim.regno[x]),
                                       prio=sol['prio'][x], now='before' if not now_before else 'after'))
            rb = refs_by_line(tr, p)
            ls = live_span(tr, p)
            S['moves'].append(dict(
                pseudo=p, allocno=al, name=namer.describe(p), old_prio=old, new_prio=new, window=[lo, hi],
                refs=sim.refs[al], live=sim.live[al], size=sim.size[al], calls=sim.calls[al],
                built=renum0.get(p, -1), target=sol['want'].get(al), now=ev['renum'].get(p, -1),
                levers=lv, eqdoubled=p in tr.eq,
                refs_from=[dict(line=ln, depth=d, refs=c) for (ln, d), c in rb[:6]],
                live_span=ls, passes=passed[:8]))
        for al in sol['bad']:
            p = sim.regno[al]
            S['still_wrong'].append(dict(pseudo=p, name=namer.describe(p), want=sol['want'][al],
                                         got=ev['renum'].get(p, -1),
                                         why=explain(tr, sim, namer, al, sol['want'][al], ev, sol['want'])))
        for al, why in sol['hard'].items():
            p = sim.regno[al]
            w = want[al]
            holders = [q for q, h in tr.Z.items() if h == w and q != p and al in sim.globals and
                       tr.allocno_of.get(q) in sim.confset[al] and sim.local[tr.allocno_of.get(q)] >= 0]
            S['hard'].append(dict(pseudo=p, name=namer.describe(p), want=w, built=renum0.get(p, -1),
                                  local_holders=[dict(pseudo=q, name=namer.describe(q)) for q in holders[:4]]))
        S['fixed'] = len(fixed)
        S['broken'] = len(broken)
        # recompile with the solution's priorities to measure the real effect
        if not a.no_verify and sol['moved']:
            def check(moves):
                ovr = dict(prio_ovr or {})
                ovr.update({sim.regno[x]: new for x, (old, new) in moves.items()})
                asm2, tr2p = compile_traced(unit, func, src, wdir, prio=ovr, tag='-solved')
                bt2 = built_function(asm2, func)
                bdata2, bsize2 = assemble(asm2, func, wdir)
                disasm(bt2, bdata2, 0)
                return diff_lines(tg, bt2), bsize2, ovr
            after, bsize2, ovr = check(sol['moved'])
            accepted = dict(sol['moved'])
            all_after = after
            if after >= dlines and len(sol['moved']) > 1:
                # the moves together do not help: is there one that does on its own?
                best = None
                for x, mv in sol['moved'].items():
                    r = check({x: mv})
                    if r[0] < dlines and (best is None or r[0] < best[0][0]):
                        best = (r, {x: mv})
                if best:
                    (after, bsize2, ovr), accepted = best
            if after >= dlines:
                accepted = {}
            S['verify'] = dict(diff_lines_before=dlines, diff_lines_after=after, diff_lines_all_moves=all_after,
                               size_before=bsize, size_after=bsize2, rejected=not accepted,
                               regoracle_prio=','.join(f'{r}={v}' for r, v in sorted(ovr.items())))
            for m in S['moves']:
                m['accepted'] = m['allocno'] in accepted
        R['solve'] = S

    # local-alloc mismatches: who took the register in the block
    lqby = {}
    for q in tr.lq:
        for r in q['regs']:
            lqby[r] = q
    for row in rows:
        if row['kind'] != 'local':
            continue
        q = lqby.get(row['pseudo'])
        if not q:
            continue
        row['qty'] = dict(block=q['block'], birth=q['birth'], death=q['death'], refs=q['refs'], pri=q['pri'],
                          sugg=q['sugg'], csugg=q['csugg'])
        rivals = [x for x in tr.lq if x['block'] == q['block'] and x is not q and x['phys'] == row['target']
                  and x['birth'] < q['death'] and q['birth'] < x['death']]
        row['taken_by'] = [dict(pseudos=x['regs'], names=[namer.describe(r) for r in x['regs']], pri=x['pri'],
                                birth=x['birth'], death=x['death']) for x in rivals[:3]]

    # reload register rotation
    rot = []
    if tr.spills:
        n = len(tr.spills)
        slot = {r: k for k, r in enumerate(tr.spills)}
        seq = {(d['uid'], d['r']): k for k, d in enumerate(tr.RA)}
        seen = set()
        for rp in rpairs:
            rl = rp['reload']
            key = (rp['uid'], rl['r'])
            if key in seen or key not in seq or rp['treg'] not in slot:
                continue
            seen.add(key)
            k = seq[key]
            ra = tr.RA[k]
            rot.append(dict(seq=k, uid=rp['uid'], line=tr.line.get(rp['uid'], 0), built=ra['reg'], target=rp['treg'],
                            bslot=ra['idx'], tslot=slot[rp['treg']], prev=ra['prev'], addr=rp['addr'],
                            reload_in=rl['in'], reload_out=rl['out']))
        rot.sort(key=lambda d: d['seq'])
        first_bad = next((d for d in rot if d['built'] != d['target']), None)
        R['reload'] = dict(spill_regs=tr.spills, allocations=len(tr.RA), observed=len(rot),
                           mismatched=sum(1 for d in rot if d['built'] != d['target']))
        if first_bad:
            last_ok = None
            for d in rot:
                if d['seq'] >= first_bad['seq']:
                    break
                if d['built'] == d['target']:
                    last_ok = d
            delta = (first_bad['tslot'] - first_bad['bslot']) % n
            between = tr.RA[(last_ok['seq'] + 1 if last_ok else 0):first_bad['seq']]
            rom_addr = {}
            for d in rot:
                rom_addr.setdefault(d['seq'], d['addr'])
            later = [dict(seq=k, line=tr.line.get(x['uid'], 0), addr=rom_addr[k],
                          what=namer.expr(parse_rtx(next((y['in'] if y['in'] != 'nil' else y['out'] for y in tr.RL.get(x['uid'], []) if y['r'] == x['r']), 'nil')), 1))
                     for k, x in enumerate(tr.RA) if (last_ok['seq'] if last_ok else -1) < k < first_bad['seq']
                     and k in rom_addr and rom_addr[k] > first_bad['addr']]
            aligned_b = {bi for ti, bi, lvl in pairs}
            by_uid = {}
            for bi, b in enumerate(bt):
                if b.get('uid') is not None:
                    by_uid.setdefault(b['uid'], []).append(bi)
            btw = []
            for k in range((last_ok['seq'] + 1 if last_ok else 0), first_bad['seq']):
                x = tr.RA[k]
                rl = next((y for y in tr.RL.get(x['uid'], []) if y['r'] == x['r']), None)
                what = namer.expr(parse_rtx(rl['in'] if rl and rl['in'] != 'nil' else (rl['out'] if rl else 'nil')), 1)
                ins = [bi for u in [x['uid']] + tr.RLI.get(x['uid'], []) for bi in by_uid.get(u, [])
                       if x['reg'] in bt[bi]['regs']]
                btw.append(dict(seq=k, line=tr.line.get(x['uid'], 0), reg=x['reg'], what=what,
                                aligned=bool(ins) and all(bi in aligned_b for bi in ins),
                                text='; '.join(bt[bi]['norm'] for bi in ins[:3])))
            R['reload']['first_mismatch'] = dict(first_bad, delta=delta, last_agree=last_ok,
                                                 between=len(between), later_in_rom=later, between_detail=btw,
                                                 between_lines=sorted(set(tr.line.get(x['uid'], 0) for x in between)))
        R['reload']['rows'] = rot
    else:
        R['reload'] = None
    R['_namer'] = namer
    if getattr(a, 'show', None):
        R['_show'] = show(tr, namer, tg, bt, int(a.show[0], 0), int(a.show[1], 0), src_lines, rows_al)
    return R, tr


def report(R, top):
    o = []
    w = o.append
    w(f"regoracle {R['func']} ({R['unit']}) from {R['src']}")
    w(f"  built 0x{R['built_size']:X} vs target 0x{R['target_size']:X} bytes; {R['instr_built']} vs {R['instr_target']} "
      f"instructions; {R['aligned']} structurally equal (registers masked), {R['same']} identical; "
      f"{R['diff_lines']} differing lines")
    w(f"  global-alloc: {R['allocnos']} allocnos ({R['globals']} for global, rest local); simulator reproduces "
      f"{R['find_reg_calls'] - R['sim_mismatch']}/{R['find_reg_calls']} find_reg decisions"
      + ('' if not R['sim_mismatch'] and not R['sim_disp_mismatch'] else '  ** SIM DISAGREES **'))
    w(f"  ROM register known for {R['pseudos_with_target']} pseudos: {R['pseudos_agree']} agree, "
      f"{len(R['mismatches'])} differ, {R['pseudos_unsure']} unclear (split votes)"
      + (f" (range {R['range'][0]}-{R['range'][1]})" if R['range'] else ''))
    rows = R['mismatches']
    if rows:
        w('')
        w('== Pseudos in a different register than in the ROM (ordered by global-alloc order; local last)')
        w('   (votes: ROM instructions showing that register / all with evidence; ~ = the evidence sits next to code that')
        w('    differs structurally, so the cause is likely the code there, not the allocation order)')
        w('   pseudo  kind     built->ROM  votes  order  refs  live    prio  via      line  name')
        rows = sorted(rows, key=lambda r: (r['kind'] != 'global', r['order'] if r['order'] is not None else 1e9))
        for r in rows[:top]:
            w(f"   {r['pseudo']:6d}  {r['kind']:8s} {rname(r['built']):>4s}->{rname(r['target']):<4s} "
              f"{r['votes']:3d}/{r['total']:<3d}{'~' if r.get('near_diff') else ' '}{r['order'] if r['order'] is not None else '-':>5}  "
              f"{r['refs'] if r['refs'] is not None else '-':>4}  {r['live'] if r['live'] is not None else '-':>5} "
              f"{r['prio'] if r['prio'] is not None else '-':>7}  {(r['via'] or '-'):7s} {r['line']:5d}  "
              f"{r['name'][:60]}" + (f"  ({r['case']})" if r.get('case') else ''))
        if len(rows) > top:
            w(f'   ... {len(rows) - top} more (--top N)')
    rounds = R.get('solve_rounds') or ([R['solve']] if R.get('solve') else [])
    for rk, S in enumerate(rounds, 1):
        if len(rounds) > 1:
            w('')
            w(f'-- solve round {rk}' + (' (on the build with the previous round\'s priorities)' if rk > 1 else ''))
        w('')
        w(f"== Inverse solve (global-alloc): {S['constraints']} pseudos with a known ROM register; "
          f"{S['wrong_before']} wrong now -> {S['wrong_after']} with {len(S['moves'])} priority change(s) "
          f"({S['sims']} simulations, {S['seconds']} s)")
        if S.get('verify'):
            v = S['verify']
            if v.get('rejected'):
                w(f"   compiler check: REJECTED: with these priorities the build gets worse ({v['diff_lines_before']} -> "
                  f"{v.get('diff_lines_all_moves', v['diff_lines_after'])} differing lines). The ROM registers they chase come from "
                  "code that differs structurally (see --show); no priority change is recommended.")
            else:
                w(f"   compiler check (REGORACLE_PRIO={v['regoracle_prio'][:120]}{'...' if len(v['regoracle_prio']) > 120 else ''}):")
                w(f"     differing lines {v['diff_lines_before']} -> {v['diff_lines_after']}, size 0x{v['size_before']:X} -> 0x{v['size_after']:X}"
                  + (f" (only the moves marked [kept]; all together give {v['diff_lines_all_moves']})" if v.get('diff_lines_all_moves') != v['diff_lines_after'] else ''))
        for k, m in enumerate(S['moves'][:top], 1):
            lo, hi = m['window']
            lo = '..' if lo is None else lo
            hi = '..' if hi is None else hi
            tag = '' if 'accepted' not in m else (' [kept]' if m['accepted'] else ' [rejected]')
            w(f"  {k}.{tag} pseudo {m['pseudo']} {m['name'][:70]}: prio {m['old_prio']} -> window [{lo}, {hi}] "
              f"(refs {m['refs']}, live {m['live']}{', REG_EQUIV live x2' if m['eqdoubled'] else ''}"
              f"{', crosses %d calls' % m['calls'] if m['calls'] else ''}); "
              f"reg {rname(m['built'])} -> {rname(m['now'])} (ROM {rname(m['target']) if m['target'] is not None else '?'})")
            lv = m['levers']
            parts = []
            if 'refs' in lv:
                parts.append(f"refs {m['refs']} -> {m['refs'] + lv['refs']} ({lv['refs']:+d})")
            if 'live' in lv:
                parts.append(f"live {m['live']} -> {m['live'] + lv['live']} ({lv['live']:+d} insns)")
            if lv.get('refs') == 0 or lv.get('live') == 0:
                w('     levers: none needed: the priority the source gives it is already in the window')
            else:
                w('     levers: ' + (' or '.join(parts) if parts else 'none in range (needs refs and live together)'))
            if m['refs_from']:
                w('     refs from: ' + ', '.join(f"line {x['line']} {x['refs']:+d}" + (f" (loop depth {x['depth'] - 1})" if x['depth'] > 1 else '')
                                          for x in m['refs_from']))
            if m['live_span']:
                ls = m['live_span']
                w(f"     live {ls['n']} insns, lines {ls['lines'][0]}-{ls['lines'][1]} (from line {ls['first']} to {ls['last']} in insn order); "
                  'most at ' + ', '.join(f'line {ln} ({c})' for ln, c in ls['top']))
            if m['passes']:
                w('     ordering: must now be ' + '; '.join(f"{x['now']} {x['pseudo']} {x['name'][:40]} (prio {x['prio']})" for x in m['passes'][:5]))
        if S['still_wrong']:
            w('   not reproducible by reordering (the conflict graph differs from the ROM\'s):')
            for x in S['still_wrong'][:top]:
                w(f"     {x['pseudo']} {x['name'][:50]} wants {rname(x['want'])}, gets {rname(x['got'])}: {x['why']['text']}")
        if S['hard']:
            w('   impossible by ordering (ROM register is a hard conflict here: a live hard reg or a local-alloc pseudo):')
            for x in S['hard'][:top]:
                w(f"     {x['pseudo']} {x['name'][:50]} wants {rname(x['want'])} (built {rname(x['built'])})"
                  + ('; local pseudos in it: ' + ', '.join(f"{y['pseudo']} {y['name'][:30]}" for y in x['local_holders']) if x['local_holders'] else ''))
    fin = R.get('final')
    if fin and len(rounds) > 1:
        w('')
        w(f"== All rounds: REGORACLE_PRIO={fin['regoracle_prio']}")
        w(f"   differing lines {fin['diff_lines_before']} -> {fin['diff_lines_after']}, size 0x{fin['size_before']:X} -> 0x{fin['size_after']:X}")
    locs = [r for r in R['mismatches'] if r['kind'] == 'local' and r.get('qty')]
    if locs:
        w('')
        w('== Local-alloc (single basic block) mismatches')
        for r in locs[:top]:
            q = r['qty']
            tb = '; ROM reg held here by ' + ', '.join(f"{x['pseudos']} {'/'.join(n[:30] for n in x['names'])} (pri {x['pri']}, life {x['birth']}-{x['death']})" for x in r['taken_by']) if r.get('taken_by') else ''
            w(f"   {r['pseudo']} {r['name'][:50]} line {r['line']}: {rname(r['built'])} -> ROM {rname(r['target'])}; "
              f"qty pri {q['pri']} life {q['birth']}-{q['death']} refs {q['refs']}{' (has copy suggestion)' if q['csugg'] else ''}{tb}")
    rl = R.get('reload')
    if rl:
        w('')
        w(f"== Reload registers: spill regs {[rname(x) for x in rl['spill_regs']]}, {rl['allocations']} allocations, "
          f"{rl['observed']} matched to the ROM, {rl['mismatched']} differ")
        fm = rl.get('first_mismatch')
        if fm:
            n = len(rl['spill_regs'])
            w(f"   first rotation mismatch: reload #{fm['seq']} at line {fm['line']} (ROM 0x{fm['addr']:08X}, reload of {fm['reload_in'] or fm['reload_out']}): "
              f"built {rname(fm['built'])} (slot {fm['bslot']}, last_spill_reg was {fm['prev']}), ROM {rname(fm['target'])} (slot {fm['tslot']})")
            fm['reload_in'] = fm['reload_in']
            la = fm['last_agree']
            if la and fm['between'] == 0:
                w(f"   -> not a rotation count: the previous reload (#{la['seq']}, line {la['line']}) agrees, so the ROM's choice differs "
                  f"because {rname(fm['built'])}/{rname(fm['target'])} availability differs at this insn (another value occupies one of them)")
            else:
                w(f"   -> the ROM's rotation is {fm['delta']} slot(s) ahead: {fm['delta']} more (or {n - fm['delta']} fewer) reload "
                  f"allocations before this point" + (f", i.e. among the {fm['between']} allocations since reload #{la['seq']} at line {la['line']} (last one that agrees)" if la else '')
                  + (f"; lines {fm['between_lines'][:12]}" if fm['between_lines'] else ''))
            for x in fm.get('between_detail', []):
                w(f"     reload #{x['seq']} line {x['line']}: {rname(x['reg'])} for {x['what'][:40]} ({x['text'][:60]})"
                  + ('' if x['aligned'] else ' -- this code differs from the ROM here (not structurally aligned): '
                     'the extra/missing or reordered reload is probably this one'))
            for x in fm.get('later_in_rom', []):
                w(f"   -> reload #{x['seq']} (line {x['line']}, {x['what'][:40]}) comes AFTER it in the ROM (0x{x['addr']:08X}): "
                  "the insn order differs here, not the number of reloads")
    return '\n'.join(o)


# ----------------------------------------------------------------------------------------------
# verification modes

def verify_compiler(units):
    import check
    comp = {}
    bad = 0
    tmp = tempfile.mkdtemp(prefix='regoracle_')
    for u in units:
        src = f'src/{u}.c'
        cc, flags = check.unit_cflags(u)
        if cc not in comp:
            comp[cc] = ensure_compiler(cc)
        preprocess(src, f'{tmp}/u.i')
        r1 = run([f'{STOCK_DIR}/bin/{cc}'] + flags + [f'{tmp}/u.i', '-o', f'{tmp}/ref.s'])
        r2 = run([comp[cc]] + flags + [f'{tmp}/u.i', '-o', f'{tmp}/new.s'])
        env = dict(os.environ, REGORACLE_OUT=f'{tmp}/t.txt', REGORACLE_FUNC='*none*')
        same = open(f'{tmp}/ref.s').read() == open(f'{tmp}/new.s').read()
        # with tracing on for every function of the unit (one at a time would be slow): trace the first one
        nm = re.findall(r'^(\w+):$', open(f'{tmp}/ref.s').read(), re.M)
        same_t = True
        for fn in nm[:3]:
            env['REGORACLE_FUNC'] = fn
            r3 = run([comp[cc]] + flags + [f'{tmp}/u.i', '-o', f'{tmp}/tr.s'], env=env)
            same_t &= open(f'{tmp}/ref.s').read() == open(f'{tmp}/tr.s').read()
        print(f'{u}: {"identical" if same else "DIFFERENT"} (traced: {"identical" if same_t else "DIFFERENT"}), '
              f'{len(nm)} functions')
        bad += (not same) + (not same_t)
    subprocess.run(['rm', '-rf', tmp])
    return bad


def oracle(a):
    """analyse(), then, while the compiler check of a solution improves but does not match yet, analyse
    the solved build again (its new structurally equal code gives new evidence) and solve on top."""
    user_prio = a.prio
    R, tr = analyse(a)
    first = R
    rounds = [R.get('solve')] if R.get('solve') else []
    while len(rounds) < a.rounds:
        v = (R.get('solve') or {}).get('verify')
        if not v or v['diff_lines_after'] >= v['diff_lines_before'] or v['diff_lines_after'] == 0:
            break
        a.prio = v['regoracle_prio']
        a.quiet = True
        R, tr = analyse(a)
        if not R.get('solve'):
            break
        rounds.append(R['solve'])
    a.prio = user_prio
    first['solve_rounds'] = rounds
    final = None
    for S in rounds:
        v = S.get('verify')
        if v and (final is None or v['diff_lines_after'] < final['diff_lines_after']):
            final = dict(v)
    if final:
        final['diff_lines_before'] = first['diff_lines']
        final['size_before'] = first['built_size']
    first['final'] = final
    return first


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('unit', nargs='?')
    ap.add_argument('func', nargs='?')
    ap.add_argument('more', nargs='*')
    ap.add_argument('--src')
    ap.add_argument('--range', nargs=2, metavar=('LO', 'HI'))
    ap.add_argument('--json', action='store_true')
    ap.add_argument('--top', type=int, default=25)
    ap.add_argument('--no-solve', action='store_true')
    ap.add_argument('--no-verify', action='store_true')
    ap.add_argument('--budget', type=int, default=6000)
    ap.add_argument('--rounds', type=int, default=3,
                    help='solve again on the solved build while that keeps improving (default 3)')
    ap.add_argument('--prio')
    ap.add_argument('--force')
    ap.add_argument('--quiet', action='store_true')
    ap.add_argument('--show', nargs=2, metavar=('LO', 'HI'),
                    help='print the aligned target/built listing of [LO, HI) with the pseudo behind every operand')
    ap.add_argument('--verify-compiler', action='store_true')
    ap.add_argument('--selftest', action='store_true')
    ap.add_argument('--perturb', type=int, default=0,
                    help='with --selftest: N trials that mis-order 1-3 random pseudos of a matched function '
                         '(REGORACLE_PRIO) and check that the inverse solve undoes it (0 diff lines again)')
    ap.add_argument('--seed', type=int, default=1)
    a = ap.parse_args()
    if a.verify_compiler:
        units = [x for x in [a.unit, a.func] + a.more if x]
        if not units:
            units = [l.split()[0] for l in open('units.txt') if l.strip() and not l.startswith('#')
                     and os.path.exists(f'src/{l.split()[0]}.c')][:12]
        sys.exit(1 if verify_compiler(units) else 0)
    if not a.unit or not a.func:
        ap.error('unit and func are required')
    if a.selftest and a.perturb:
        import random
        pairs = [a.unit, a.func] + a.more
        rnd = random.Random(a.seed)
        fails = 0
        for u, f in zip(pairs[::2], pairs[1::2]):
            a.unit, a.func, a.quiet, a.no_solve, a.prio = u, f, True, True, None
            R0, tr0 = analyse(a)
            sim0 = Sim(tr0)
            glob = [x for x in sim0.globals if len(sim0.conflist[x]) >= 3]
            for trial in range(a.perturb):
                for attempt in range(20):
                    k = rnd.choice([1, 2, 3])
                    picks = rnd.sample(glob, min(k, len(glob)))
                    ovr = {}
                    for x in picks:
                        y = rnd.choice(sim0.globals)
                        ovr[sim0.regno[x]] = sim0.base_prio[y] + rnd.choice([-1, 1])
                    a.prio = ','.join(f'{r}={v}' for r, v in ovr.items())
                    a.no_solve = True
                    R, tr = analyse(a)
                    if R['diff_lines']:
                        break
                else:
                    print(f'{f}: no perturbation changed the code')
                    continue
                a.no_solve = False
                R = oracle(a)
                S = R.get('solve')
                after = R['final']['diff_lines_after'] if R.get('final') else R['diff_lines']
                ok = after == 0
                fails += not ok
                moved = ', '.join(f"{m['pseudo']}:{m['old_prio']}->{m['new_prio']}" for m in (S['moves'] if S else []))
                print(f"{f} trial {trial}: perturbed {a.prio} -> {R['diff_lines']} diff lines, "
                      f"{len(R['mismatches'])} pseudos differ; solve "
                      f"{(str(S['wrong_before']) + '->' + str(S['wrong_after'])) if S else '-'} with [{moved}]"
                      f"{' +%d more round(s)' % (len(R['solve_rounds']) - 1) if len(R['solve_rounds']) > 1 else ''}; "
                      f"recompiled: {after} diff lines {'OK' if ok else 'NOT FIXED'}")
        sys.exit(1 if fails else 0)
    if a.selftest:
        pairs = [a.unit, a.func] + a.more
        fails = 0
        for u, f in zip(pairs[::2], pairs[1::2]):
            a.unit, a.func = u, f
            a.no_solve = True
            a.quiet = True
            R, tr = analyse(a)
            ok = (not R['sim_mismatch'] and not R['sim_disp_mismatch'] and not R['mismatches']
                  and (not R['reload'] or not R['reload']['mismatched']))
            fails += not ok
            print(f"{f} ({u}): {'OK' if ok else 'FAIL'}: sim {R['find_reg_calls'] - R['sim_mismatch']}/{R['find_reg_calls']}, "
                  f"ROM register known for {R['pseudos_with_target']} pseudos, {len(R['mismatches'])} differ, "
                  f"diff lines {R['diff_lines']}, reload {R['reload']['observed'] if R['reload'] else 0} observed / "
                  f"{R['reload']['mismatched'] if R['reload'] else 0} differ")
        sys.exit(1 if fails else 0)
    R = oracle(a)
    R.pop('_namer', None)
    shown = R.pop('_show', None)
    if a.json:
        print(json.dumps(R, indent=1, default=str))
    else:
        if shown:
            print(shown)
            print()
        print(report(R, a.top))


if __name__ == '__main__':
    main()
