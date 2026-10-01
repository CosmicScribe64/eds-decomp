#!/usr/bin/env python3
"""Target bytes for code units, with or without the baserom.

    from target import ROM, rom_available
    ROM[0x7A1A8:0x7A298]      # bytes at ROM offset 0x7A1A8 (address 0x0807A1A8)

With `baserom.gba` present, ROM is the real image. Without it (e.g. a CI or web-agent sandbox that
must never hold the game), ROM is a view that assembles each code unit's original assembly
(`asm/<unit>.s`, which includes `asm/nonmatching/<unit>/*.s`), links it alone at the unit's ROM
address and returns the resulting bytes. The original assembly was verified to reassemble byte for
byte (`make compare` with every unit forced to asm), so for code the two are identical.
Only code units (those in config/functions.tsv with an asm/<unit>.s wrapper) can be served this
way; data, graphics and the header still need the real ROM.

Assembled units are cached in build/target-cache/<unit>.bin (rebuilt when any input is newer).

    python3 tools/target.py --verify [unit ...]   # with a ROM: check the assembled bytes equal it
"""
import os
import re
import subprocess
import sys
import tempfile

BASE = 0x08000000
ROM_PATH = 'baserom.gba'
CACHE = 'build/target-cache'


def rom_available():
    return os.path.exists(ROM_PATH)


def _run(cmd):
    return subprocess.run(cmd, capture_output=True, text=True)


def _units():
    """unit -> (lo, hi) address range, from config/functions.tsv."""
    units = {}
    for line in open('config/functions.tsv'):
        if line.startswith('#'):
            continue
        f = line.rstrip('\n').split('\t')
        a, size, unit = int(f[0], 16), int(f[2], 16), f[4]
        lo, hi = units.get(unit, (a, a + size))
        units[unit] = (min(lo, a), max(hi, a + size))
    return units


def known_symbols():
    """name -> (address, is_thumb) from everything committed: config/symbols.txt (generated from the
    last full link), config/functions.tsv and symbols.ld. Callers handle address-suffixed names."""
    syms = {}
    if os.path.exists('config/symbols.txt'):
        for line in open('config/symbols.txt'):
            p = line.split()
            if len(p) >= 2 and not line.startswith('#'):
                syms[p[0]] = (int(p[1], 16), len(p) > 2 and p[2] == 't')
    for line in open('config/functions.tsv'):
        if line.startswith('#'):
            continue
        f = line.rstrip('\n').split('\t')
        syms[f[3]] = (int(f[0], 16), f[1] == 't')
    for line in open('symbols.ld'):
        m = re.match(r'\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;', line)
        if m:
            syms.setdefault(m.group(1), (int(m.group(2), 16), False))
    return syms


def resolve_symbol(name, syms):
    if name in syms:
        return syms[name]
    m = re.search(r'_([0-9A-Fa-f]{8})$', name)
    if not m:
        return None
    addr = int(m.group(1), 16)
    return addr, name.startswith('sub_') and BASE <= addr < 0x08080A20


def _inputs(unit):
    files = [f'asm/{unit}.s', 'asm/macros.inc', 'config/functions.tsv', 'symbols.ld']
    d = f'asm/nonmatching/{unit}'
    if os.path.isdir(d):
        files += [os.path.join(d, f) for f in os.listdir(d)]
    if os.path.exists('config/symbols.txt'):
        files.append('config/symbols.txt')
    return files


def assemble_unit(unit, src=None):
    """Bytes of asm/<unit>.s (or another assembly source) linked at the unit's ROM address.
    The original assembly is cached."""
    lo, hi = _units()[unit]
    cache = f'{CACHE}/{unit}.bin' if src is None else None
    if cache and os.path.exists(cache) and os.path.getmtime(cache) >= max(os.path.getmtime(f) for f in _inputs(unit)):
        return open(cache, 'rb').read()
    src = src or f'asm/{unit}.s'
    if not os.path.exists(src):
        raise KeyError(f'no original assembly for unit {unit} ({src})')
    tmp = tempfile.mkdtemp(prefix='target_')
    obj = f'{tmp}/unit.o'
    r = _run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-mthumb-interwork', '-I', '.', '-o', obj, src])
    if r.returncode:
        raise RuntimeError(f'assembling {src} failed:\n{r.stderr}')
    nm = _run(['arm-none-eabi-nm', obj]).stdout
    undef = [l.split()[-1] for l in nm.splitlines() if l.split()[0] == 'U']
    syms = known_symbols()
    stub = ['\t.text']
    for s in undef:
        v = resolve_symbol(s, syms)
        if v is None:
            raise RuntimeError(f'{unit}: cannot resolve {s} without the ROM (add it to config/symbols.txt)')
        stub += [f'\t.global {s}', f'\t.thumb_set {s}, 0x{v[0]:08X}' if v[1] else f'\t.set {s}, 0x{v[0]:08X}']
    open(f'{tmp}/syms.s', 'w').write('\n'.join(stub) + '\n')
    r = _run(['arm-none-eabi-as', '-mcpu=arm7tdmi', '-o', f'{tmp}/syms.o', f'{tmp}/syms.s'])
    if r.returncode:
        raise RuntimeError('symbol stubs failed:\n' + r.stderr)
    open(f'{tmp}/link.ld', 'w').write(f'SECTIONS {{ . = 0x{lo:08X}; .text : {{ {obj}(.text) }} }}\n')
    r = _run(['arm-none-eabi-ld', '-T', f'{tmp}/link.ld', '-o', f'{tmp}/unit.elf', obj, f'{tmp}/syms.o'])
    if r.returncode:
        raise RuntimeError(f'linking {unit} failed:\n{r.stderr}')
    _run(['arm-none-eabi-objcopy', '-O', 'binary', '-j', '.text', f'{tmp}/unit.elf', f'{tmp}/text.bin'])
    data = open(f'{tmp}/text.bin', 'rb').read()
    if len(data) < hi - lo:
        raise RuntimeError(f'{unit}: assembled 0x{len(data):X} bytes, expected 0x{hi - lo:X}')
    if cache:
        os.makedirs(CACHE, exist_ok=True)
        open(cache, 'wb').write(data[:hi - lo])
    return data[:hi - lo]


class _AssembledRom:
    """Read-only, slice-only stand-in for the ROM image, covering code units."""

    def __init__(self):
        self._units = None

    def _unit_at(self, addr):
        if self._units is None:
            self._units = sorted((lo, hi, u) for u, (lo, hi) in _units().items())
        for lo, hi, u in self._units:
            if lo <= addr < hi:
                return lo, hi, u
        raise KeyError(f'0x{addr:08X} is not inside a code unit; this needs the real baserom.gba')

    def __getitem__(self, s):
        if not isinstance(s, slice) or s.step not in (None, 1):
            raise TypeError('only contiguous slices are supported without the ROM')
        out = bytearray()
        a, end = BASE + s.start, BASE + s.stop
        while a < end:
            lo, hi, u = self._unit_at(a)
            data = assemble_unit(u)
            take = min(end, hi) - a
            out += data[a - lo:a - lo + take]
            a += take
        return bytes(out)

    def __len__(self):
        return 0x800000


ROM = open(ROM_PATH, 'rb').read() if rom_available() else _AssembledRom()


def main():
    if '--verify' not in sys.argv:
        sys.exit(__doc__)
    if not rom_available():
        sys.exit('--verify needs baserom.gba')
    real = open(ROM_PATH, 'rb').read()
    units = [u for u in sys.argv[1:] if u != '--verify'] or [u for u in _units() if os.path.exists(f'asm/{u}.s')]
    bad = 0
    for u in units:
        lo, hi = _units()[u]
        ok = assemble_unit(u) == real[lo - BASE:hi - BASE]
        bad += not ok
        if not ok:
            print(f'{u}: assembled bytes DIFFER from the ROM')
    print(f'{len(units) - bad}/{len(units)} units: assembled original == ROM')
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
