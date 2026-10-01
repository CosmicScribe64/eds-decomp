#!/usr/bin/env python3
"""Build the interactive decomp-progress treemap page.

Usage: python3 tools/mkmap.py OUT.html
Reads config/functions.tsv, src/**/*.c|.s and wiki/functions/code-*.md; inlines the data
into tools/map_template.html.
Function status: 0 = asm (not attempted), 1 = asm with a draft C attempt under #if 0,
                 2 = matching C, 3 = hand-written / SDK source.
"""
import datetime
import json
import os
import re
import sys

out = sys.argv[1]
units = {}
order = []
for line in open('config/functions.tsv'):
    if line.startswith('#'):
        continue
    f = line.rstrip('\n').split('\t')
    u = f[4]
    if u not in units:
        units[u] = []
        order.append(u)
    units[u].append((int(f[0], 16), int(f[2], 16), f[3]))


def draft_names(text):
    names = set()
    for block in re.findall(r'#if 0(.*?)#endif', text, re.S):
        names |= set(re.findall(r'\b(sub_[0-9A-F]{8})\s*\(', block))
    return names


def unit_desc(u):
    path = f'wiki/functions/{u.replace("_", "-").lower()}.md'
    if not os.path.exists(path):
        return ''
    lines = open(path).read().split('\n')
    body = False
    for ln in lines:
        s = ln.strip()
        if s.startswith('# '):
            body = True
            continue
        if not body or not s or s.startswith(('|', '#', '>', '---', '```')):
            continue
        s = re.sub(r'\[\[([^\]|]+)(\|[^\]]+)?\]\]', r'\1', s)
        s = re.sub(r'[*`]', '', s)
        return s[:260] + ('…' if len(s) > 260 else '')
    return ''


data = []
for u in order:
    c, s = f'src/{u}.c', f'src/{u}.s'
    if os.path.exists(c):
        text = open(c).read()
        inc = set(re.findall(r'INCLUDE_ASM\([^,]+,\s*(\w+)\)', text))
        drafts = draft_names(text)
        stat = lambda n: (1 if n in drafts else 0) if n in inc else 2
    elif os.path.exists(s):
        stat = lambda n: 3
    else:
        stat = lambda n: 0
    if u.startswith('sdk/'):
        stat = lambda n: 3
    data.append({'u': u, 'd': unit_desc(u),
                 'f': [[a, sz, n, stat(n)] for a, sz, n in units[u]]})

# ---- labels: what we know / think each function is
def clean(t):
    t = re.sub(r'\[\[([^\]|]+)(\|[^\]]+)?\]\]', r'\1', t)
    t = re.sub(r'[*`]', '', t).strip()
    return t


labels = {}   # addr -> [name, purpose, kind]  kind: 0 known, 1 proposed, 2 guess


def put(addr, name='', purpose='', kind=1):
    cur = labels.setdefault(addr, ['', '', kind])
    if name and not cur[0]:
        cur[0] = name
        cur[2] = kind
    if purpose and not cur[1]:
        cur[1] = purpose


for u in order:
    for a, sz, n in units[u]:
        if not n.startswith('sub_'):
            put(a, n, '', 0)
for line in open('config/names.txt') if os.path.exists('config/names.txt') else []:
    m = re.match(r'^(0x0[89][0-9A-Fa-f]{6})\s+(\w+)\s*(?:#\s*(.*))?$', line.strip())
    if m:
        why = m.group(3) or ''
        put(int(m.group(1), 16), m.group(2), clean(why), 2 if 'hyp' in why.lower() else 0)
for path in sorted(os.listdir('wiki/functions')):
    if not path.startswith('code-'):
        continue
    cols = None
    for ln in open(f'wiki/functions/{path}'):
        if not ln.startswith('|'):
            cols = None
            continue
        cells = [c.strip() for c in ln.strip().strip('|').split('|')]
        if cols is None:
            cols = [c.lower() for c in cells]
            continue
        if set(''.join(cells)) <= set('-: '):
            continue
        m = re.search(r'(?:0x|sub_)(0[89][0-9A-Fa-f]{6})', cells[0])
        if not m:
            continue
        addr = int(m.group(1), 16)
        name = purpose = ''
        guess = False
        for c, v in zip(cols, cells):
            if 'name' in c:
                v2 = clean(v)
                guess = guess or bool(re.search(r'hyp|\?|guess|weak', v2, re.I))
                mm = re.match(r'([A-Za-z_][A-Za-z0-9_]*)', v2)
                if mm and not mm.group(1).startswith('sub_') and mm.group(1).lower() not in ('none', 'n', 'unknown', 'tbd'):
                    name = mm.group(1)
            elif any(k in c for k in ('purpose', 'what', 'role', 'description', 'does')):
                purpose = clean(v)
                guess = guess or bool(re.search(r'\bhyp', purpose, re.I))
        if len(purpose) > 180:
            purpose = purpose[:177].rsplit(' ', 1)[0] + '…'
        put(addr, name, purpose, 2 if guess else 1)
for d in data:
    for f in d['f']:
        if f[0] in labels:
            f.append(labels[f[0]])

# ---- whole-ROM segments from wiki/rom/rom-map.md
def romrows():
    rows = []
    for ln in open('wiki/rom/rom-map.md'):
        if not ln.startswith('|'):
            continue
        cells = [c.strip() for c in ln.strip().strip('|').split('|')]
        addrs = re.findall(r'0x(08[0-9A-Fa-f]{6})', ' '.join(cells[:2]))
        if len(addrs) < 2:
            continue
        a, b = int(addrs[0], 16), int(addrs[1], 16)
        if b <= a:
            continue
        text = clean(cells[2] if len(cells) > 2 and not re.fullmatch(r'0x[0-9A-Fa-f]+', cells[1].strip('`')) else cells[-1])
        if len(cells) >= 4 and re.search(r'0x08', cells[1]):
            text = clean(cells[2])
        vh = cells[3] if len(cells) >= 4 and re.search(r'^(V|H)', cells[3]) else ''
        rows.append([a, b, text, vh])
    return rows


GROUPS = [  # coarse ranges -> group label
    (0x08000000, 0x08080A20, 'Code'), (0x08080A20, 0x08087FD0, 'Strings & tables'),
    (0x08087FD0, 0x08139F5C, 'Sound'), (0x08139F5C, 0x081ABE4C, 'Game tables & dialogue'),
    (0x081ABE4C, 0x081C0000, 'Padding'), (0x081C0000, 0x0822C720, 'Fonts'),
    (0x0822C720, 0x08625460, 'Card data'), (0x08625460, 0x087F8568, 'Graphics'),
    (0x087F8568, 0x08800000, 'Padding'),
]
rows = romrows()
fine = [r for r in rows if not any(r is not o and o[0] <= r[0] and r[1] <= o[1] and (o[1] - o[0]) < (r[1] - r[0]) for o in rows)]
# keep the smallest rows that tile the ROM; fill gaps with the coarse description
fine = sorted([r for r in rows if not any(o is not r and r[0] <= o[0] and o[1] <= r[1] and (o[1] - o[0]) < (r[1] - r[0]) for o in rows)])
segs, cur = [], 0x08000000
for a, b, text, vh in fine + [[0x08800000, 0x08800000, '', '']]:
    if a > cur:
        segs.append([cur, a, 'Unlabelled bytes', 'U'])
    if b > a and a >= cur:
        segs.append([a, b, text, vh])
        cur = b
    elif b > cur:
        segs.append([cur, b, text, vh])
        cur = b
out_segs = []
for a, b, text, vh in segs:
    g = next((n for lo, hi, n in GROUPS if lo <= a < hi), 'Other')
    t = text.lower()
    if re.match(r'(zero padding|all zero|terminator)', t):
        kind = 'pad'
    elif a < 0x08080A20:
        kind = 'code'
    elif vh.startswith('V') and 'H' not in vh:
        kind = 'v'
    elif vh.startswith('H'):
        kind = 'h'
    else:
        kind = 'u'
    if g == 'Game tables & dialogue' and 'sound' in t[:40]:
        g = 'Sound'
    if len(text) > 300:
        text = text[:297].rsplit(' ', 1)[0] + '…'
    out_segs.append([a, b - a, g, text, kind])
# merge header + code into one code block
code_blocks = [x for x in out_segs if x[4] == 'code']
out_segs = [x for x in out_segs if x[4] != 'code']
out_segs.insert(0, [0x08000000, 0x08080A20 - 0x08000000, 'Code', 'Header, startup code, game code, sound driver, SDK, libgcc/libc.', 'code'])

payload = {'rom': out_segs, 'units': data, 'date': datetime.date.today().isoformat()}
tpl = open('tools/map_template.html').read()
open(out, 'w').write(tpl.replace('/*DATA*/null', json.dumps(payload, separators=(',', ':'))))
tot = sum(len(d['f']) for d in data)
done = sum(1 for d in data for f in d['f'] if f[3] >= 2)
nl = sum(1 for d in data for f in d['f'] if len(f) > 4 and f[4][0])
np_ = sum(1 for d in data for f in d['f'] if len(f) > 4)
print(f'wrote {out}: {len(data)} units, {tot} functions, {done} in C/source, {nl} named, {np_} with a label')
