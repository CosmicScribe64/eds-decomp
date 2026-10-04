#!/usr/bin/env python3
"""Rename symbols across the repo (asm, src, data, config, symbols.ld, wiki).

  python3 tools/rename.py OLD NEW [OLD NEW ...]
  python3 tools/rename.py --file names.txt      # lines: "<old-or-address> <new> [# comment]"

A function's asm file asm/nonmatching/<unit>/<old>.s is renamed to <new>.s, and the
.include / INCLUDE_ASM references follow. A RAM symbol (e.g. gMain) that no asm/data
file defines gets an entry in symbols.ld. Only run this while nobody else is editing src/.
In names files, an address (0x08XXXXXX / 0x02XXXXXX / 0x03XXXXXX) stands for the symbol that
currently names it (sub_/gUnk_ form or the config/functions.tsv name).
"""
import os
import re
import sys

TEXT_DIRS = ['asm', 'src', 'data', 'include', 'wiki']
TEXT_FILES = ['config/functions.tsv', 'symbols.ld', 'units.txt']


def all_files():
    for d in TEXT_DIRS:
        for root, _, files in os.walk(d):
            for f in files:
                if f.endswith(('.s', '.c', '.h', '.inc', '.md')) and f != 'log.md':
                    yield os.path.join(root, f)
    for f in TEXT_FILES:
        if os.path.exists(f):
            yield f


def func_names():
    names = {}
    for line in open('config/functions.tsv'):
        if not line.startswith('#'):
            p = line.split('\t')
            names[int(p[0], 16)] = p[3]
    return names


def resolve(old, fnames):
    if re.fullmatch(r'0x[0-9A-Fa-f]{8}', old):
        a = int(old, 16)
        if a in fnames:
            return fnames[a]
        return f'gUnk_{a:08X}'
    return old


def main():
    args = sys.argv[1:]
    pairs = []
    if args and args[0] == '--file':
        for line in open(args[1]):
            line = line.split('#')[0].split()
            if len(line) >= 2:
                pairs.append((line[0], line[1]))
    else:
        pairs = list(zip(args[0::2], args[1::2]))
    fnames = func_names()
    pairs = [(resolve(o, fnames), n) for o, n in pairs]
    pairs = [(o, n) for o, n in pairs if o != n]
    if not pairs:
        return
    mapping = dict(pairs)
    pat = re.compile(r'\b(' + '|'.join(re.escape(o) for o in mapping) + r')\b')

    # rename per-function asm files
    for root, _, files in os.walk('asm/nonmatching'):
        for f in files:
            stem = f[:-2]
            if f.endswith('.s') and stem in mapping:
                os.rename(os.path.join(root, f), os.path.join(root, mapping[stem] + '.s'))

    defined = set()
    changed = 0
    for path in list(all_files()):
        text = open(path).read()
        new = pat.sub(lambda m: mapping[m.group(1)], text)
        if path.startswith(('asm', 'data', 'src')) and path.endswith('.s'):
            defined |= set(re.findall(r'^(\w+):', new, re.M))  # labels after the rename
        if new != text:
            open(path, 'w').write(new)
            changed += 1

    # RAM / absolute symbols need a symbols.ld entry
    fn_addrs = set(fnames)
    syms = open('symbols.ld').read()
    add = []
    for old, new in pairs:
        m = re.search(r'_([0-9A-F]{8})$', old)
        # A symbol needs an absolute entry only when nothing defines it: RAM/IO symbols, and ROM aliases that are
        # neither a function (defined by its C/asm; an absolute entry would also lose a Thumb function's bit 0)
        # nor a label in asm/data .s files (e.g. an alias into the middle of a ROM table).
        # Reserved names (__udivsi3 and other libgcc/libc symbols) come from the libraries: an absolute entry would
        # keep the library object out of the link.
        if m and new not in defined and int(m.group(1), 16) not in fn_addrs and not new.startswith('__') \
                and not re.search(rf'^\s*{re.escape(new)}\s*=', syms, re.M):
            add.append(f'{new} = 0x{m.group(1)};')
    if add:
        with open('symbols.ld', 'a') as f:
            f.write('\n'.join(add) + '\n')
    print(f'renamed {len(pairs)} symbols in {changed} files; {len(add)} symbols.ld entries added')


if __name__ == '__main__':
    main()
