#!/usr/bin/env python3
"""Find and check your copy of the game, then extract its data into assets/.

  tools/dr make setup                       # looks in roms/ (and the repository root) for the game
  tools/dr python3 tools/setup.py FILE      # or name the file; it must be inside this repository

Accepts a .gba file or a .zip containing one. The ROM must be the USA release (game code AY5E) with
SHA-1 510fbba212aca9bab95ea12f8fd933e62ee34dea. A matching file is saved as roms/base_eng.gba,
baserom.gba is linked to it, and tools/assets.py extracts the data that the build needs.
"""
import glob
import hashlib
import os
import shutil
import sys
import zipfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import assets  # noqa: E402

SHA1 = '510fbba212aca9bab95ea12f8fd933e62ee34dea'
TARGET = 'roms/base_eng.gba'
KNOWN = {
    'dc25f733cee913afdf187ec03df30909fe28b03c': 'the Japanese release (Duel Monsters 5 Expert 1, AY5J). '
                                                'This project builds the USA release for now.',
}


def candidates(args):
    if args:
        return args
    found = []
    for pattern in ('roms/*', 'baserom.gba', '*.gba', '*.zip'):
        for f in sorted(glob.glob(pattern)):
            if f not in found and os.path.basename(f) != 'eds.gba' and f.lower().endswith(('.gba', '.zip', '.agb', '.bin')):
                found.append(f)
    return found


def images(path):
    """(label, bytes) for the file itself or every .gba inside a zip."""
    if zipfile.is_zipfile(path):
        with zipfile.ZipFile(path) as z:
            for n in z.namelist():
                if n.lower().endswith(('.gba', '.agb', '.bin')):
                    yield f'{path}:{n}', z.read(n)
    else:
        yield path, open(path, 'rb').read()


def diagnose(data):
    h = hashlib.sha1(data).hexdigest()
    if h in KNOWN:
        return KNOWN[h]
    code = data[0xAC:0xB0].decode('ascii', 'replace') if len(data) >= 0xC0 else ''
    if code == 'AY5E':
        if len(data) != 0x800000:
            return f'the right game, but {len(data):#x} bytes instead of 0x800000 (a trimmed or padded dump).'
        return ('the right game (AY5E), but modified: an intro, trainer or save patch, or a bad dump. '
                'Use an unmodified dump.')
    if code == 'AY5J':
        return 'a Japanese release (AY5J). This project builds the USA release for now.'
    title = data[0xA0:0xAC].decode('ascii', 'replace').strip('\0 ') if len(data) >= 0xC0 else '?'
    return f'a different game (title {title!r}, code {code!r}).'


def link_baserom():
    if os.path.islink('baserom.gba') or os.path.exists('baserom.gba'):
        os.remove('baserom.gba')
    try:
        os.symlink(TARGET, 'baserom.gba')
    except OSError:
        shutil.copy(TARGET, 'baserom.gba')


def main():
    os.makedirs('roms', exist_ok=True)
    files = candidates(sys.argv[1:])
    if not files:
        sys.exit('No ROM found. Copy your dump of Yu-Gi-Oh! The Eternal Duelist Soul (USA) into the roms/ folder\n'
                 '(a .gba file, or the .zip it came in), then run `tools/dr make setup` again.')
    good, problems = None, []
    for f in files:
        for label, data in images(f):
            if hashlib.sha1(data).hexdigest() == SHA1:
                good = (label, data)
                break
            problems.append(f'  {label}: {diagnose(data)}')
        if good:
            break
    if not good:
        sys.exit('No usable ROM found. Checked:\n' + '\n'.join(problems)
                 + f'\nThe build needs the USA ROM with SHA-1 {SHA1}.')
    label, data = good
    if not (os.path.exists(TARGET) and open(TARGET, 'rb').read() == data):
        open(TARGET, 'wb').write(data)
    link_baserom()
    print(f'ROM OK: {label} (SHA-1 {SHA1[:12]}...), saved as {TARGET}')
    assets.cmd_extract('baserom.gba')
    print('Setup done. Build with:  tools/dr make -j8 compare')


if __name__ == '__main__':
    main()
