#!/usr/bin/env python3
"""Run decomp-permuter over every `#if 0` draft in the repo, smallest function first, forever.

  tools/dr python3 tools/permute_batch.py [--workers 3] [-j 2] [--minutes 5] [--max-size 0x400]

It rescans src/ before every job, so drafts that agents park while it runs are picked up too; when
nothing is left it waits and rescans. Each function is tried once (it is listed in batch.log afterwards). Progress goes to build/permuter/batch.log
(one line per function: unit, func, size, best score). It only reads src/ and never edits it;
apply the score-0 results by hand (see wiki/tools/decomp-permuter.md).
"""
import argparse
import os
import re
import subprocess
import threading
import time


def drafts():
    sizes = {}
    for line in open('config/functions.tsv'):
        if not line.startswith('#'):
            f = line.rstrip('\n').split('\t')
            sizes[f[3]] = (int(f[2], 16), f[4])
    out = []
    for fn in sorted(os.listdir('src')):
        if not fn.startswith('code_') or not fn.endswith('.c'):
            continue
        lines = open(f'src/{fn}').read().split('\n')
        for i, l in enumerate(lines):
            m = re.match(r'^\s*INCLUDE_ASM\(\s*"[^"]*"\s*,\s*(\w+)\s*\)', l)
            if m and i and lines[i - 1].strip() == '#endif' and m.group(1) in sizes:
                size, unit = sizes[m.group(1)]
                note = next((lines[k] for k in range(i - 2, -1, -1) if lines[k].startswith('#if 0')), '')
                out.append((size, unit, m.group(1), note))
    return sorted(out)


def best(func):
    d = f'build/permuter/{func}'
    if not os.path.isdir(d):
        return None
    scores = [int(o.split('-')[1]) for o in os.listdir(d) if o.startswith('output-')]
    return min(scores) if scores else None


REGALLOC_NOTE = re.compile(r'regist|regalloc|swap|allocation|\br\d+\b', re.I)


def profile_for(note, a):
    if a.profile != 'auto':
        return a.profile
    return 'regalloc' if REGALLOC_NOTE.search(note) else 'default'


def work(item, a):
    size, unit, func, note = item
    if best(func) == 0:
        return
    t = time.time()
    prof = profile_for(note, a)
    r = subprocess.run(['python3', 'tools/permute.py', unit, func, '--run', '-j', str(a.j),
                        '--minutes', str(a.minutes), '--profile', prof], capture_output=True, text=True)
    status = best(func)
    if r.returncode and status is None:
        status = 'setup-failed: ' + (r.stderr.strip().splitlines() or ['?'])[-1][:120]
    with open(a.log, 'a') as f:
        f.write(f'{unit}\t{func}\t0x{size:X}\t{status}\t{int(time.time() - t)}s\t{prof}\n')


LOCK = threading.Lock()
TAKEN = set()


def tried(path):
    return {l.split('\t')[1] for l in open(path)} if os.path.exists(path) else set()


def first_scores():
    best = {}
    p = 'build/permuter/batch.log'
    if os.path.exists(p):
        for l in open(p):
            f = l.rstrip('\n').split('\t')
            if len(f) > 3 and f[3].isdigit():
                best[f[1]] = min(int(f[3]), best.get(f[1], 1 << 30))
    return best


def next_item(a):
    with LOCK:
        done = tried(a.log) | TAKEN
        items = drafts()
        if a.retry_below:
            # phase 2: closest first, only drafts the first pass got within retry_below
            sc = first_scores()
            items = sorted((it for it in items if 0 < sc.get(it[2], 0) <= a.retry_below), key=lambda it: sc[it[2]])
        for item in items:
            if item[0] <= a.max_size and item[2] not in done:
                TAKEN.add(item[2])
                return item
    return None


def worker(a):
    while True:
        item = next_item(a)
        if item is None:
            time.sleep(120)
            continue
        work(item, a)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--workers', type=int, default=3)
    ap.add_argument('-j', type=int, default=2)
    ap.add_argument('--minutes', type=float, default=5)
    ap.add_argument('--max-size', type=lambda s: int(s, 0), default=0x400)
    ap.add_argument('--profile', choices=['auto', 'default', 'regalloc'], default='auto',
                    help='auto: regalloc profile for drafts whose NONMATCHING note mentions register allocation')
    ap.add_argument('--retry-below', type=int, default=0,
                    help='phase 2: re-run drafts whose first-pass best score is 1..N, closest first')
    ap.add_argument('--log', default=None)
    a = ap.parse_args()
    a.log = a.log or ('build/permuter/batch2.log' if a.retry_below else 'build/permuter/batch.log')
    os.makedirs('build/permuter', exist_ok=True)
    threads = [threading.Thread(target=worker, args=(a,), daemon=True) for _ in range(a.workers)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()


if __name__ == '__main__':
    main()
