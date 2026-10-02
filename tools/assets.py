#!/usr/bin/env python3
"""Extract the game's data from your ROM into editable files, and build them back.

  python3 tools/assets.py extract [--rom baserom.gba]   # ROM -> assets/   (run once, via `make setup`)
  python3 tools/assets.py build                          # assets/ -> build/assets/*.bin (the Makefile runs it)
  python3 tools/assets.py gen-data                       # regenerate data/*.s from config/assets.tsv
  python3 tools/assets.py check                          # manifest covers the data exactly once
  python3 tools/assets.py verify [--rom baserom.gba]     # build every asset and compare with the ROM
  (extract/verify take --only PATH_PREFIX; EDS_ASSET_MANIFEST / EDS_ASSETS_DIR / EDS_ASSETS_OUT override the
   manifest, assets/ and build/assets/, for developing a converter privately. More formats: tools/assetfmt/)

The repository contains no game data. `config/assets.tsv` lists every byte range of the ROM that isn't
code, with a type that decides the file format in `assets/`:

  bin         raw bytes                                         <path>.bin
  zero        all-zero padding (no file)
  strings     fixed-size text records (size=N)                  <path>  (JSON list)
  dialogue    {u16 event; u16 speaker; char text[0x300]}        <path>  (JSON list)
  duelists    {u32 id; char name[0x40]; char short[0x40]}       <path>  (JSON list)
  u16         array of u16                                      <path>  (JSON list)
  palette     BGR555 colours                                    <path>  (JASC-PAL text)
  card_art    821 6bpp 72x80 images, then their 64-colour palettes   <path>/NNN.png
  card_stats  821 u32 stat words                                <path>  (CSV)
  font1bpp    1bpp font (h=, bits=8|16, cols=)                  <path>  (PNG sheet)
  tiles4bpp   4bpp 8x8 tiles (cols=, pal=ADDRESS for viewing)   <path>  (PNG sheet)

Every conversion is checked during extraction: the converted files are built back immediately and compared
with the ROM bytes. If they differ, that asset is written as raw .bin instead and listed in
assets/fallback.txt, so a build from extracted assets always matches.
Text uses Windows-1252 with Latin-1 for the five undefined bytes, a lossless one-to-one mapping.
"""
import csv
import io
import json
import os
import re
import struct
import sys
import zlib

BASE = 0x08000000
MANIFEST = os.environ.get('EDS_ASSET_MANIFEST', 'config/assets.tsv')  # overrides let a converter be developed
ASSETS = os.environ.get('EDS_ASSETS_DIR', 'assets')                   # against a private manifest and output
OUT = os.environ.get('EDS_ASSETS_OUT', 'build/assets')
DATA_UNITS = {  # data unit -> (start, end) address range; see units.txt
    'rodata_08080A20': (0x08080A20, 0x08087FB4),
    'rodata_08087FD0': (0x08087FD0, 0x08800000),
}

# ---------------------------------------------------------------------------------------------- text
_DEC = []
for _b in range(256):
    try:
        _DEC.append(bytes([_b]).decode('cp1252'))
    except UnicodeDecodeError:
        _DEC.append(chr(_b))
_ENC = {c: b for b, c in enumerate(_DEC)}
assert len(_ENC) == 256


def dec(b):
    return ''.join(_DEC[x] for x in b)


def enc(s):
    try:
        return bytes(_ENC[c] for c in s)
    except KeyError as e:
        raise ValueError(f'character {e} is not in the game text encoding (Windows-1252)')


def text_field(raw):
    """Fixed-size NUL-terminated field -> JSON value (a string, or an object when the tail isn't zero)."""
    n = raw.find(b'\0')
    n = len(raw) if n < 0 else n
    text, tail = raw[:n], raw[n:]
    if tail.strip(b'\0') == b'':
        return dec(text)
    return {'text': dec(text), 'raw_tail': tail.hex()}


def field_bytes(v, size):
    if isinstance(v, dict):
        b = enc(v['text']) + bytes.fromhex(v['raw_tail'])
    else:
        b = enc(v)
    if len(b) > size:
        raise ValueError(f'text is {len(b)} bytes, the field holds {size}: {str(v)[:40]!r}')
    return b + bytes(size - len(b))


# ----------------------------------------------------------------------------------------------- png
def png_write(width, height, pixels, palette):
    """8-bit indexed PNG. pixels: bytes of width*height indices; palette: list of (r, g, b)."""
    def chunk(t, d):
        c = struct.pack('>I', len(d)) + t + d
        return c + struct.pack('>I', zlib.crc32(t + d) & 0xFFFFFFFF)
    raw = b''.join(b'\0' + pixels[y * width:(y + 1) * width] for y in range(height))
    return (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 3, 0, 0, 0))
            + chunk(b'PLTE', b''.join(bytes(c) for c in palette)) + chunk(b'IDAT', zlib.compress(raw, 9))
            + chunk(b'IEND', b''))


def png_read(data):
    """Indexed PNG (bit depth 1/2/4/8) -> (width, height, pixel indices, palette)."""
    assert data[:8] == b'\x89PNG\r\n\x1a\n', 'not a PNG'
    pos, idat, plte = 8, b'', []
    while pos < len(data):
        n, t = struct.unpack('>I4s', data[pos:pos + 8])
        d = data[pos + 8:pos + 8 + n]
        if t == b'IHDR':
            w, h, depth, ctype, _, _, interlace = struct.unpack('>IIBBBBB', d)
        elif t == b'PLTE':
            plte = [tuple(d[i:i + 3]) for i in range(0, len(d), 3)]
        elif t == b'IDAT':
            idat += d
        pos += 12 + n
    if ctype != 3 or interlace:
        raise ValueError('the PNG must be indexed (palette) colour and not interlaced')
    raw = zlib.decompress(idat)
    stride = (w * depth + 7) // 8
    bpp = 1
    out, prev, i = bytearray(), bytearray(stride), 0
    for _ in range(h):
        f, line = raw[i], bytearray(raw[i + 1:i + 1 + stride])
        i += 1 + stride
        for x in range(stride):
            a = line[x - bpp] if x >= bpp else 0
            b, c = prev[x], prev[x - bpp] if x >= bpp else 0
            if f == 1:
                line[x] = (line[x] + a) & 255
            elif f == 2:
                line[x] = (line[x] + b) & 255
            elif f == 3:
                line[x] = (line[x] + (a + b) // 2) & 255
            elif f == 4:
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                line[x] = (line[x] + (a if pa <= pb and pa <= pc else b if pb <= pc else c)) & 255
        prev = line
        if depth == 8:
            out += line[:w]
        else:
            per = 8 // depth
            for x in range(w):
                out.append((line[x // per] >> (8 - depth * (x % per + 1))) & ((1 << depth) - 1))
    return w, h, bytes(out), plte


def bgr555_to_rgb(c):
    r, g, b = c & 31, (c >> 5) & 31, (c >> 10) & 31
    return (r << 3 | r >> 2, g << 3 | g >> 2, b << 3 | b >> 2)


def rgb_to_bgr555(rgb):
    r, g, b = rgb[:3]
    return (r >> 3) | (g >> 3) << 5 | (b >> 3) << 10


# ------------------------------------------------------------------------------------------ converters
# Each converter has extract(data, params, rom) -> {relative path: bytes} and build(read, params) -> bytes,
# where read(relative path) returns that file's bytes.

def x_bin(data, p, rom):
    return {p['path'] + '.bin': data}


def b_bin(read, p):
    return read(p['path'] + '.bin')


def x_zero(data, p, rom):
    if data.strip(b'\0'):
        raise ValueError('not all zero')
    return {}


def b_zero(read, p):
    return bytes(p['end'] - p['start'])


def x_strings(data, p, rom):
    size = int(p['size'], 0)
    assert len(data) % size == 0
    items = [text_field(data[i:i + size]) for i in range(0, len(data), size)]
    return {p['path']: json.dumps(items, ensure_ascii=False, indent=1).encode() + b'\n'}


def b_strings(read, p):
    size = int(p['size'], 0)
    return b''.join(field_bytes(v, size) for v in json.loads(read(p['path'])))


def x_dialogue(data, p, rom):
    items = []
    for i in range(0, len(data), 0x304):
        ev, sp = struct.unpack_from('<HH', data, i)
        items.append({'event': ev, 'speaker': sp, 'text': text_field(data[i + 4:i + 0x304])})
    return {p['path']: json.dumps(items, ensure_ascii=False, indent=1).encode() + b'\n'}


def b_dialogue(read, p):
    return b''.join(struct.pack('<HH', d['event'], d['speaker']) + field_bytes(d['text'], 0x300)
                    for d in json.loads(read(p['path'])))


def x_duelists(data, p, rom):
    items = []
    for i in range(0, len(data), 0x84):
        items.append({'id': struct.unpack_from('<I', data, i)[0], 'name': text_field(data[i + 4:i + 0x44]),
                      'short_name': text_field(data[i + 0x44:i + 0x84])})
    return {p['path']: json.dumps(items, ensure_ascii=False, indent=1).encode() + b'\n'}


def b_duelists(read, p):
    return b''.join(struct.pack('<I', d['id']) + field_bytes(d['name'], 0x40) + field_bytes(d['short_name'], 0x40)
                    for d in json.loads(read(p['path'])))


def x_u16(data, p, rom):
    vals = list(struct.unpack(f'<{len(data) // 2}H', data))
    return {p['path']: (json.dumps(vals) + '\n').encode()}


def b_u16(read, p):
    vals = json.loads(read(p['path']))
    return struct.pack(f'<{len(vals)}H', *vals)


def _optional(read, rel, default):
    try:
        return json.loads(read(rel))
    except (KeyError, FileNotFoundError):
        return default


def x_palette(data, p, rom):
    """JASC-PAL text. Bit 15, which the hardware ignores but the ROM sometimes sets, goes into
    <path>.bit15.json (the indices that have it) so the palette file stays standard."""
    cols = struct.unpack(f'<{len(data) // 2}H', data)
    lines = ['JASC-PAL', '0100', str(len(cols))] + ['%d %d %d' % bgr555_to_rgb(c) for c in cols]
    files = {p['path']: ('\r\n'.join(lines) + '\r\n').encode()}
    hi = [i for i, c in enumerate(cols) if c & 0x8000]
    if hi:
        files[p['path'] + '.bit15.json'] = (json.dumps(hi) + '\n').encode()
    return files


def b_palette(read, p):
    lines = read(p['path']).decode().split()
    n = int(lines[2])
    vals = [rgb_to_bgr555(tuple(int(v) for v in lines[3 + 3 * i:6 + 3 * i])) for i in range(n)]
    for i in _optional(read, p['path'] + '.bit15.json', []):
        vals[i] |= 0x8000
    return struct.pack(f'<{n}H', *vals)


CARD_COUNT, ART_SIZE, ART_W, ART_H = 821, 0x10E0, 72, 80


def _art_pixels(packed):
    """6bpp little-endian bit stream in 8bpp tile order (9x10 tiles of 8x8) -> row-major 72x80 indices."""
    stream = bytearray()
    for g in range(0, len(packed), 6):
        v = int.from_bytes(packed[g:g + 6], 'little')
        stream += bytes((v >> (6 * i)) & 63 for i in range(8))
    img = bytearray(ART_W * ART_H)
    for t in range(90):
        tx, ty = t % 9, t // 9
        for py in range(8):
            row = (ty * 8 + py) * ART_W + tx * 8
            img[row:row + 8] = stream[t * 64 + py * 8:t * 64 + py * 8 + 8]
    return bytes(img)


def _art_pack(img):
    stream = bytearray()
    for t in range(90):
        tx, ty = t % 9, t // 9
        for py in range(8):
            row = (ty * 8 + py) * ART_W + tx * 8
            stream += img[row:row + 8]
    out = bytearray()
    for g in range(0, len(stream), 8):
        v = 0
        for i in range(8):
            if stream[g + i] > 63:
                raise ValueError('card art pixels must use palette entries 0-63')
            v |= stream[g + i] << (6 * i)
        out += v.to_bytes(6, 'little')
    return bytes(out)


def x_card_art(data, p, rom):
    """One indexed 72x80 PNG per card, its 64-colour palette as the PNG palette. Palette bit 15 (ignored by
    the hardware) goes into <path>/palette_bit15.json: card number -> colour indices that have it."""
    files, hi = {}, {}
    pal_base = CARD_COUNT * ART_SIZE
    for i in range(CARD_COUNT):
        cols = struct.unpack_from('<64H', data, pal_base + i * 0x80)
        if any(c & 0x8000 for c in cols):
            hi[f'{i:03d}'] = [j for j, c in enumerate(cols) if c & 0x8000]
        files[f"{p['path']}/{i:03d}.png"] = png_write(ART_W, ART_H, _art_pixels(data[i * ART_SIZE:(i + 1) * ART_SIZE]),
                                                     [bgr555_to_rgb(c) for c in cols])
    files[f"{p['path']}/palette_bit15.json"] = (json.dumps(hi, indent=0) + '\n').encode()
    return files


def b_card_art(read, p):
    arts, pals = [], []
    hi = _optional(read, f"{p['path']}/palette_bit15.json", {})
    for i in range(CARD_COUNT):
        w, h, px, plte = png_read(read(f"{p['path']}/{i:03d}.png"))
        if (w, h) != (ART_W, ART_H):
            raise ValueError(f'card art {i:03d}.png must be {ART_W}x{ART_H}')
        plte = (plte + [(0, 0, 0)] * 64)[:64]
        arts.append(_art_pack(px))
        cols = [rgb_to_bgr555(c) for c in plte]
        for j in hi.get(f'{i:03d}', []):
            cols[j] |= 0x8000
        pals.append(struct.pack('<64H', *cols))
    return b''.join(arts) + b''.join(pals)


STAT_FIELDS = [('def', 0, 9), ('atk', 9, 9), ('kind', 18, 2), ('type', 20, 5), ('level', 25, 4), ('attr', 29, 3)]


def x_card_stats(data, p, rom):
    names = None
    if rom is not None:
        n0 = 0x0822C720 - BASE
        names = [dec(rom[n0 + i * 0x40:n0 + (i + 1) * 0x40].split(b'\0')[0]) for i in range(len(data) // 4)]
    buf = io.StringIO()
    w = csv.writer(buf, lineterminator='\n')
    w.writerow(['id', 'name'] + [f for f, _, _ in STAT_FIELDS])
    for i, (word,) in enumerate(struct.iter_unpack('<I', data)):
        w.writerow([i, names[i] if names else ''] + [(word >> s) & ((1 << n) - 1) for _, s, n in STAT_FIELDS])
    return {p['path']: buf.getvalue().encode()}


def b_card_stats(read, p):
    out = bytearray()
    for row in csv.DictReader(io.StringIO(read(p['path']).decode())):
        word = 0
        for f, s, n in STAT_FIELDS:
            v = int(row[f])
            if not 0 <= v < 1 << n:
                raise ValueError(f'card {row["id"]}: {f}={v} does not fit in {n} bits')
            word |= v << s
        out += struct.pack('<I', word)
    return bytes(out)


def x_font1bpp(data, p, rom):
    """1bpp font -> PNG sheet (index 0 = background, 1 = ink). Options: h=glyph height, bits=8 (one byte per
    row) or 16 (big-endian u16 per row, MSB = left), cols=glyphs per sheet row."""
    h, bits, cols = int(p['h']), int(p.get('bits', 8)), int(p['cols'])
    gsize = h * bits // 8
    n = len(data) // gsize
    rows = (n + cols - 1) // cols
    W, H = cols * bits, rows * h
    img = bytearray(W * H)
    for g in range(n):
        gx, gy = (g % cols) * bits, (g // cols) * h
        for y in range(h):
            v = int.from_bytes(data[g * gsize + y * bits // 8:g * gsize + (y + 1) * bits // 8], 'big')
            for x in range(bits):
                img[(gy + y) * W + gx + x] = (v >> (bits - 1 - x)) & 1
    return {p['path']: png_write(W, H, bytes(img), [(255, 255, 255), (0, 0, 0)])}


def b_font1bpp(read, p):
    h, bits, cols = int(p['h']), int(p.get('bits', 8)), int(p['cols'])
    gsize = h * bits // 8
    n = (p['end'] - p['start']) // gsize
    W, _, img, _ = png_read(read(p['path']))
    out = bytearray()
    for g in range(n):
        gx, gy = (g % cols) * bits, (g // cols) * h
        for y in range(h):
            v = 0
            for x in range(bits):
                v = v << 1 | (1 if img[(gy + y) * W + gx + x] else 0)
            out += v.to_bytes(bits // 8, 'big')
    return bytes(out)


def x_tiles4bpp(data, p, rom):
    """4bpp 8x8 tiles -> PNG sheet, cols tiles wide, shown with the 16 colours at pal=ADDRESS.
    The PNG's palette is for viewing only; colours live in their own palette asset."""
    cols = int(p['cols'])
    n = len(data) // 32
    rows = (n + cols - 1) // cols
    W, H = cols * 8, rows * 8
    img = bytearray(W * H)
    for t in range(n):
        tx, ty = (t % cols) * 8, (t // cols) * 8
        for i in range(64):
            b = data[t * 32 + i // 2]
            img[(ty + i // 8) * W + tx + i % 8] = (b >> 4) if i & 1 else (b & 15)
    pal = [(0, 0, 0)] * 16
    if rom is not None and 'pal' in p:
        a = int(p['pal'], 16) - BASE
        pal = [bgr555_to_rgb(c) for c in struct.unpack_from('<16H', rom, a)]
    return {p['path']: png_write(W, H, bytes(img), pal)}


def b_tiles4bpp(read, p):
    cols = int(p['cols'])
    n = (p['end'] - p['start']) // 32
    W, _, img, _ = png_read(read(p['path']))
    out = bytearray()
    for t in range(n):
        tx, ty = (t % cols) * 8, (t // cols) * 8
        for i in range(0, 64, 2):
            lo = img[(ty + i // 8) * W + tx + i % 8]
            hi = img[(ty + (i + 1) // 8) * W + tx + (i + 1) % 8]
            if lo > 15 or hi > 15:
                raise ValueError('4bpp tiles must use palette entries 0-15')
            out.append(lo | hi << 4)
    return bytes(out)


TYPES = {
    'bin': (x_bin, b_bin), 'zero': (x_zero, b_zero), 'strings': (x_strings, b_strings),
    'dialogue': (x_dialogue, b_dialogue), 'duelists': (x_duelists, b_duelists), 'u16': (x_u16, b_u16),
    'palette': (x_palette, b_palette), 'card_art': (x_card_art, b_card_art),
    'card_stats': (x_card_stats, b_card_stats), 'font1bpp': (x_font1bpp, b_font1bpp),
    'tiles4bpp': (x_tiles4bpp, b_tiles4bpp),
}


def _load_plugins():
    """More formats live in tools/assetfmt/<name>.py, one module per format family. Each defines
    register(A) -> {type: (extract, build)}, where A is this module (helpers such as png_write, enc, dec)."""
    import importlib.util
    d = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'assetfmt')
    if not os.path.isdir(d):
        return
    for fn in sorted(os.listdir(d)):
        if fn.endswith('.py') and not fn.startswith('_'):
            spec = importlib.util.spec_from_file_location(f'assetfmt_{fn[:-3]}', os.path.join(d, fn))
            mod = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(mod)
            for name, fns in mod.register(sys.modules[__name__]).items():
                if name in TYPES:
                    sys.exit(f'tools/assetfmt/{fn}: type {name} is already defined')
                TYPES[name] = fns


_load_plugins()


# ------------------------------------------------------------------------------------------- manifest
ONLY = None  # --only PREFIX: extract/verify only assets whose path starts with PREFIX


def manifest():
    out = []
    for line in open(MANIFEST):
        line = line.split('#')[0].strip()
        if not line:
            continue
        f = line.split()
        p = dict(kv.split('=', 1) for kv in f[4:])
        p.update(start=int(f[0], 16), end=int(f[1], 16), type=f[2], path=f[3])
        if p['type'] not in TYPES:
            sys.exit(f'{MANIFEST}: unknown type {p["type"]}')
        if ONLY is None or p['path'].startswith(ONLY):
            out.append(p)
    return out


def fallbacks():
    try:
        return set(open(f'{ASSETS}/fallback.txt').read().split())
    except FileNotFoundError:
        return set()


def asset_bytes(p, fb):
    """Build one asset from assets/ (raw .bin when extraction fell back)."""
    def read(rel):
        return open(f'{ASSETS}/{rel}', 'rb').read()
    kind = 'bin' if p['path'] in fb else p['type']
    data = TYPES[kind][1](read, p)
    if len(data) != p['end'] - p['start']:
        raise ValueError(f"{p['path']}: built 0x{len(data):X} bytes, expected 0x{p['end'] - p['start']:X}")
    return data


def load_rom(path):
    if not os.path.exists(path):
        sys.exit(f'{path} not found. Put your copy of the game in roms/ and run `make setup` (see README.md).')
    return open(path, 'rb').read()


# ------------------------------------------------------------------------------------------- commands
def cmd_extract(rom_path):
    rom = load_rom(rom_path)
    fb = []
    os.makedirs(ASSETS, exist_ok=True)
    for p in manifest():
        data = rom[p['start'] - BASE:p['end'] - BASE]
        try:
            files = TYPES[p['type']][0](data, p, rom)
            back = TYPES[p['type']][1](lambda rel: files[rel], p)
            if back != data:
                raise ValueError('round trip differs')
        except (ValueError, KeyError, AssertionError) as e:
            print(f"note: {p['path']} kept as raw bytes ({e})")
            fb.append(p['path'])
            files = {p['path'] + '.bin': data}
        for rel, content in files.items():
            dst = f'{ASSETS}/{rel}'
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            open(dst, 'wb').write(content)
    open(f'{ASSETS}/fallback.txt', 'w').write(''.join(f'{x}\n' for x in fb))
    n = sum(len(fs) for _, _, fs in os.walk(ASSETS))
    print(f'extracted {len(manifest())} assets into {ASSETS}/ ({n} files)')


def out_path(p):
    return f"{OUT}/{p['path'].replace('/', '__')}.bin"


def cmd_build():
    if not os.path.isdir(ASSETS):
        sys.exit('assets/ is missing. Put your copy of the game in roms/ and run `make setup` (see README.md).')
    fb = fallbacks()
    os.makedirs(OUT, exist_ok=True)
    errors = 0
    for p in manifest():
        if p['type'] == 'zero' and p['path'] not in fb:
            continue
        try:
            data = asset_bytes(p, fb)
        except (ValueError, KeyError, FileNotFoundError) as e:
            print(f"error: {p['path']}: {e}", file=sys.stderr)
            errors += 1
            continue
        dst = out_path(p)
        if not os.path.exists(dst) or open(dst, 'rb').read() != data:
            open(dst, 'wb').write(data)
    if errors:
        sys.exit(1)
    open(f'{OUT}/.built', 'w').write('')


def cmd_verify(rom_path):
    rom = load_rom(rom_path)
    fb, bad = fallbacks(), 0
    for p in manifest():
        if asset_bytes(p, fb) != rom[p['start'] - BASE:p['end'] - BASE]:
            print(f"DIFF {p['path']}")
            bad += 1
    print(f'{len(manifest()) - bad}/{len(manifest())} assets build back to the ROM bytes')
    sys.exit(1 if bad else 0)


def cmd_check():
    m = sorted(manifest(), key=lambda p: p['start'])
    for a, b in zip(m, m[1:]):
        if a['end'] > b['start']:
            sys.exit(f"overlap: {a['path']} and {b['path']}")
    for unit, (lo, hi) in DATA_UNITS.items():
        cov = [p for p in m if lo <= p['start'] < hi]
        pos = lo
        for p in cov:
            if p['start'] != pos:
                sys.exit(f'{unit}: gap 0x{pos:08X}-0x{p["start"]:08X}')
            pos = p['end']
        if pos != hi:
            sys.exit(f'{unit}: ends at 0x{pos:08X}, expected 0x{hi:08X}')
    print(f'manifest OK: {len(m)} assets')


def labels_of(path):
    s = open(path).read()
    return [(name, int(addr, 16)) for name, addr in re.findall(r'^(\w+): @ 0x([0-9A-Fa-f]{8})', s, re.M)]


def cmd_gen_data():
    m = sorted(manifest(), key=lambda p: p['start'])
    for unit, (lo, hi) in DATA_UNITS.items():
        path = f'data/{unit}.s'
        labels = sorted(labels_of(path), key=lambda x: x[1])
        lines = ['@ Generated by tools/assets.py gen-data from config/assets.tsv. Do not edit.',
                 '@ Each asset is built from assets/ into build/assets/ (see tools/assets.py).', '',
                 '\t.section .rodata', '']
        for p in (q for q in m if lo <= q['start'] < hi):
            cuts = sorted({p['start'], p['end']} | {a for _, a in labels if p['start'] < a < p['end']})
            at = {a: [n for n, x in labels if x == a] for a in cuts}
            lines.append(f"@ {p['path']} ({p['type']})")
            for s, e in zip(cuts, cuts[1:]):
                for n in at.get(s, []):
                    lines += [f'\t.global {n}', f'{n}: @ 0x{s:08X}']
                if p['type'] == 'zero':
                    lines.append(f'\t.space 0x{e - s:X}')
                else:
                    lines.append(f'\t.incbin "{out_path(p)}", 0x{s - p["start"]:X}, 0x{e - s:X}')
        open(path, 'w').write('\n'.join(lines) + '\n')
        print(f'wrote {path} ({len(labels)} labels)')


def main():
    global ONLY
    a = sys.argv[1:]
    rom = a[a.index('--rom') + 1] if '--rom' in a else 'baserom.gba'
    if '--only' in a:
        if a[0] not in ('extract', 'verify'):
            sys.exit('--only works with extract and verify')
        ONLY = a[a.index('--only') + 1]
    cmd = a[0] if a else ''
    if cmd == 'extract':
        cmd_extract(rom)
    elif cmd == 'build':
        cmd_build()
    elif cmd == 'verify':
        cmd_verify(rom)
    elif cmd == 'check':
        cmd_check()
    elif cmd == 'gen-data':
        cmd_gen_data()
    else:
        sys.exit(__doc__)


if __name__ == '__main__':
    main()
