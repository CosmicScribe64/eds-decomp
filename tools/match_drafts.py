#!/usr/bin/env python3
"""Recheck parked C drafts, retaining only exact whole-unit ROM matches.

Run in the toolchain container with no agents writing the selected units:
    tools/dr python3 tools/match_drafts.py [unit ...]

Each draft is enabled in isolation. Failed attempts restore the source verbatim;
successful attempts remove only that draft's #if 0 wrapper and INCLUDE_ASM.
Original sources and check logs are saved under build/match_drafts/. This also
catches stale NONMATCHING annotations after shared-header changes.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess


BLOCK = re.compile(
    r'^#if 0(?P<header>[^\n]*)\n(?P<body>(?:(?!^#if 0).)*?)'
    r'^#(?:(?P<else>else)|endif)[^\n]*\n'
    r'(?:[ \t]*\n)*'
    r'(?P<include>INCLUDE_ASM\([^,]+,\s*(?P<name>\w+)\);[^\n]*)'
    r'(?(else)\n#endif[^\n]*)',
    re.MULTILINE | re.DOTALL,
)


def draft_body(block):
    """Discard the stale draft note, including any multiline continuation."""
    header = block['header'].lstrip()
    if header.startswith('/*'):
        text = header + '\n' + block['body']
        end = text.find('*/')
        if end < 0:
            raise ValueError('unterminated draft annotation')
        return text[end + 2:].lstrip('\n')
    return block['body']


def check(unit):
    return subprocess.run(
        ['python3', 'tools/check.py', unit], capture_output=True, text=True,
        timeout=120,
    )


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('units', nargs='*')
    args = parser.parse_args()
    output = Path('build/match_drafts')
    output.mkdir(parents=True, exist_ok=True)
    paths = ([Path('src') / (u + '.c') for u in args.units] if args.units
             else sorted(Path('src').glob('*.c')))
    results = []
    for path in paths:
        unit = path.stem
        original = path.read_text()
        names = [m['name'] for m in BLOCK.finditer(original)]
        if not names:
            continue
        baseline = check(unit)
        if baseline.returncode:
            (output / (unit + '.baseline.txt')).write_text(baseline.stdout + baseline.stderr)
            raise RuntimeError(f'{unit}: baseline does not match; source unchanged')
        (output / (unit + '.before.c')).write_text(original)
        matched = []
        for name in names:
            source = path.read_text()
            block = next(m for m in BLOCK.finditer(source) if m['name'] == name)
            body = draft_body(block)
            candidate = source[:block.start()] + body.rstrip() + source[block.end():]
            # Check exactly one function body, not a block containing another draft.
            if '#if 0' in body or not re.search(r'\b' + name + r'\s*\([^;{}]*\)\s*\{', body):
                results.append({'unit': unit, 'function': name, 'matched': False,
                                'skipped': 'unexpected draft structure'})
                print(f'{unit}/{name}: skipped unexpected draft structure', flush=True)
                continue
            path.write_text(candidate)
            try:
                result = check(unit)
            except BaseException:
                path.write_text(source)
                raise
            (output / (name + '.txt')).write_text(result.stdout + result.stderr)
            success = result.returncode == 0 and 'unit bytes MATCH' in result.stdout
            if success:
                matched.append(name)
            else:
                path.write_text(source)
            results.append({'unit': unit, 'function': name, 'matched': success})
            (output / 'results.json').write_text(json.dumps(results, indent=2) + '\n')
        print(f'{unit}: {len(matched)}/{len(names)} new matches' +
              (': ' + ', '.join(matched) if matched else ''), flush=True)
    print(f"Total: {sum(r['matched'] for r in results)}/{len(results)} drafts now match", flush=True)


if __name__ == '__main__':
    main()
