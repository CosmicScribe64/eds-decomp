#!/usr/bin/env python3
"""Check every C unit against its target (no ROM needed): the CI gate.

  tools/dr python3 tools/check_all.py [-j N] [unit ...]

Runs tools/check.py on each unit with a src/<unit>.c and fails (exit 1) if any unit does not report
`unit bytes MATCH`. With baserom.gba the target is the ROM; without it, the unit's original assembly
(tools/target.py). Units are independent, so they run in parallel.
"""
import argparse
import concurrent.futures
import os
import subprocess
import sys


def units():
    out = []
    for line in open('units.txt'):
        u = line.split('#')[0].strip()
        if u and not u.startswith('@') and os.path.exists(f'src/{u}.c') and os.path.exists(f'asm/{u}.s'):
            out.append(u)
    return out


def asm_units():
    """Units whose source is authored assembly (src/<u>.s) with an original asm/<u>.s to compare to."""
    out = []
    for line in open('units.txt'):
        u = line.split('#')[0].strip()
        if u and not u.startswith('@') and os.path.exists(f'src/{u}.s') and os.path.exists(f'asm/{u}.s'):
            out.append(u)
    return out


def check_asm(u):
    sys.path.insert(0, 'tools')
    import target
    try:
        ok = target.assemble_unit(u, f'src/{u}.s') == target.assemble_unit(u)
        return u, ok, 'authored assembly MATCH' if ok else 'authored assembly DIFFERS from the original'
    except Exception as e:  # noqa: BLE001
        return u, False, str(e)[-300:]


def check(u):
    r = subprocess.run([sys.executable, 'tools/check.py', u], capture_output=True, text=True)
    last = (r.stdout.strip().splitlines() or [''])[-1]
    return u, r.returncode == 0 and 'unit bytes MATCH' in last, last or r.stderr.strip()[-300:]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('-j', type=int, default=os.cpu_count() or 2)
    ap.add_argument('units', nargs='*')
    a = ap.parse_args()
    us = a.units or units()
    bad = []
    with concurrent.futures.ThreadPoolExecutor(a.j) as ex:
        for u, ok, msg in ex.map(check, us):
            if not ok:
                bad.append(u)
                print(f'FAIL {u}: {msg}', flush=True)
    if not a.units:
        for u in asm_units():
            u, ok, msg = check_asm(u)
            us.append(u)
            if not ok:
                bad.append(u)
                print(f'FAIL {u}: {msg}', flush=True)
    print(f'{len(us) - len(bad)}/{len(us)} units match')
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
