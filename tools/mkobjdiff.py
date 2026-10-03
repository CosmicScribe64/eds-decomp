#!/usr/bin/env python3
"""Generate objdiff.json (objdiff GUI + `objdiff-cli report generate`, the format decomp.dev reads).

  python3 tools/mkobjdiff.py > objdiff.json
  python3 tools/mkobjdiff.py --summary build/report.json      # print the report's headline numbers

Units come from units.txt. For each code unit:
  target_path  build/objdiff/target/<u>.o  the original assembly (no baserom needed)
  base_path    build/objdiff/base/<u>.o    the C, with INCLUDE_ASM functions left out (only if src/<u>.c exists)
Both have absolute data references resolved by tools/objdiff_resolve.py.
A unit is "complete" (decomp.dev's "fully linked") when its C has no INCLUDE_ASM left.
The SDK units are original SDK source/verified C, so they are their own target.
"""
import json
import os
import re
import sys

CATEGORIES = [
    {'id': 'game', 'name': 'Game'},
    {'id': 'sound', 'name': 'Sound driver'},
    {'id': 'sdk', 'name': 'SDK'},
]


def units():
    for line in open('units.txt'):
        line = line.split('#')[0].strip()
        if line and not line.startswith('@'):
            yield line


def cflags(unit):
    for line in open('config/cflags.txt'):
        p = line.split('#')[0].split()
        if p and p[0] == unit:
            return p[1], ' '.join(p[2:])
    mk = open('Makefile').read()
    return (re.search(r'^DEFAULT_CC1\s*:=\s*(\S+)', mk, re.M).group(1),
            re.search(r'^DEFAULT_CFLAGS\s*:=\s*(.*)$', mk, re.M).group(1).strip())


def make_config():
    out = []
    for u in units():
        if u.startswith('sound_'):
            cat = 'sound'
        elif u.startswith('sdk/'):
            cat = 'sdk'
        elif u in ('crt0', 'veneer') or u.startswith('rodata_') or u.startswith('@'):
            continue  # crt0, veneer, data
        else:
            cat = 'game'
        entry = {'name': f'{cat}/{u}', 'metadata': {'progress_categories': [cat]}}
        c = f'src/{u}.c'
        if cat == 'sdk' or os.path.exists(f'src/{u}.s'):  # authored assembly: complete as written
            obj = f'build/src/{u}.o' if os.path.exists(c) else f'build/srcasm/{u}.o'
            entry.update(target_path=obj, base_path=obj)
            entry['metadata']['complete'] = True
            entry['metadata']['source_path'] = c if os.path.exists(c) else f'src/{u}.s'
        else:
            entry['target_path'] = f'build/objdiff/target/{u}.o'
            if os.path.exists(c):
                entry['base_path'] = f'build/objdiff/base/{u}.o'
                entry['metadata']['source_path'] = c
                entry['metadata']['complete'] = not re.search(r'^\s*INCLUDE_ASM\(', open(c).read(), re.M)
                cc, flags = cflags(u)
                entry['scratch'] = {'platform': 'gba', 'compiler': cc, 'c_flags': flags}
            else:
                entry['metadata']['complete'] = False
        out.append(entry)
    return {
        '$schema': 'https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json',
        'custom_make': 'make',
        'build_target': False,
        'build_base': True,
        'watch_patterns': ['*.c', '*.h', '*.s', '*.inc'],
        'ignore_patterns': ['build/**/*'],
        'progress_categories': CATEGORIES,
        'units': out,
    }


def summary(path):
    r = json.load(open(path))

    def line(name, m):
        return (f'{name:14s} code {m.get("matched_code_percent", 0):6.2f}%  '
                f'functions {m.get("matched_functions", 0)}/{m.get("total_functions", 0)} '
                f'({m.get("matched_functions_percent", 0):.2f}%)  '
                f'fully linked {m.get("complete_code_percent", 0):6.2f}%  fuzzy {m.get("fuzzy_match_percent", 0):6.2f}%')
    print(line('all', r['measures']))
    for c in r.get('categories', []):
        print(line(c['name'], c['measures']))


if __name__ == '__main__':
    if len(sys.argv) == 3 and sys.argv[1] == '--summary':
        summary(sys.argv[2])
    else:
        json.dump(make_config(), sys.stdout, indent=2)
        sys.stdout.write('\n')
