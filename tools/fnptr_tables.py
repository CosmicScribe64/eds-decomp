#!/usr/bin/env python3
"""Find tables of function pointers in the ROM's data (state machines, callback lists, effect handlers).

  python3 tools/fnptr_tables.py [--min 3] [--out config/fnptr_tables.tsv]

A word counts as a function pointer when it equals a known function's address (+1 for Thumb) from
config/functions.tsv. Runs of at least --min consecutive such words (4-byte aligned, outside code) are reported
as tables, together with who loads the table's address from a literal pool (the code that indexes it).
Output TSV: table address, entries, referencing functions, member functions.
Idea from the khcom decomp's function_pointer_evidence.py.
"""
import argparse
import bisect
import struct

BASE = 0x08000000
CODE_END = 0x08080A20


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--min', type=int, default=3)
    ap.add_argument('--out', default='config/fnptr_tables.tsv')
    a = ap.parse_args()
    rom = open('baserom.gba', 'rb').read()
    funcs = {}
    starts = []
    for line in open('config/functions.tsv'):
        if line.startswith('#'):
            continue
        f = line.rstrip('\n').split('\t')
        addr, size = int(f[0], 16), int(f[2], 16)
        funcs[addr | (1 if f[1] == 't' else 0)] = f[3]
        starts.append((addr, addr + size, f[3]))
    starts.sort()
    lo_list = [s[0] for s in starts]

    def containing(addr):
        i = bisect.bisect_right(lo_list, addr) - 1
        if i >= 0 and starts[i][0] <= addr < starts[i][1]:
            return starts[i][2]
        return None

    words = struct.unpack_from(f'<{len(rom) // 4}I', rom)
    # literal-pool references to data addresses, from inside functions
    refs = {}
    for i, w in enumerate(words):
        addr = BASE + i * 4
        if addr >= CODE_END:
            break
        if CODE_END <= w < BASE + len(rom):
            f = containing(addr)
            if f:
                refs.setdefault(w, set()).add(f)
    tables = []
    i = (CODE_END - BASE) // 4
    while i < len(words):
        j = i
        while j < len(words) and (words[j] in funcs or (words[j] == 0 and j > i and j + 1 < len(words) and words[j + 1] in funcs)):
            j += 1
        n_ptr = sum(1 for k in range(i, j) if words[k] in funcs)
        if n_ptr >= a.min:
            addr = BASE + i * 4
            members = [funcs.get(words[k], 'NULL') for k in range(i, j)]
            tables.append((addr, j - i, sorted(refs.get(addr, [])), members))
            i = j
        else:
            i += 1
    with open(a.out, 'w') as f:
        f.write('# table\tentries\treferenced_by\tmembers (in order; NULL = 0)\n')
        for addr, n, by, members in tables:
            f.write(f'0x{addr:08X}\t{n}\t{",".join(by) or "-"}\t{",".join(members)}\n')
    total = sum(n for _, n, _, _ in tables)
    print(f'{len(tables)} tables, {total} entries -> {a.out}')
    for addr, n, by, members in sorted(tables, key=lambda t: -t[1])[:12]:
        print(f'  0x{addr:08X} {n:4d} entries  used by {", ".join(by[:3]) or "-"}  first: {", ".join(members[:3])}')


if __name__ == '__main__':
    main()
