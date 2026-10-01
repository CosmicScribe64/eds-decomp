#!/usr/bin/env python3
"""Write shields.io endpoint files for the README progress badges.

  python3 tools/badges.py OUTDIR

Writes OUTDIR/code.json and OUTDIR/functions.json from tools/progress.py. CI publishes them to the
`badges` branch on every push to main, and the README reads them through
https://img.shields.io/endpoint?url=<raw URL of the file>.
"""
import json
import os
import re
import subprocess
import sys


def colour(pct):
    return ('brightgreen' if pct >= 90 else 'green' if pct >= 75 else 'yellowgreen' if pct >= 60
            else 'yellow' if pct >= 40 else 'orange' if pct >= 20 else 'red')


def main():
    out = sys.argv[1] if len(sys.argv) > 1 else 'build/badges'
    text = subprocess.run([sys.executable, 'tools/progress.py'], capture_output=True, text=True, check=True).stdout
    m = re.search(r'Decompiled: (\d+)/(\d+) functions \(([\d.]+)%\), \S+ bytes \(([\d.]+)%\)', text)
    done, total, fpct, cpct = int(m.group(1)), int(m.group(2)), float(m.group(3)), float(m.group(4))
    os.makedirs(out, exist_ok=True)
    badges = {
        'code.json': {'label': 'Code', 'message': f'{cpct:.1f}%', 'color': colour(cpct)},
        'functions.json': {'label': 'Functions', 'message': f'{done:,} / {total:,}', 'color': colour(fpct)},
    }
    for name, b in badges.items():
        with open(os.path.join(out, name), 'w') as f:
            json.dump({'schemaVersion': 1, **b}, f)
            f.write('\n')
    print(f'code {cpct:.1f}%, functions {done}/{total}')


if __name__ == '__main__':
    main()
