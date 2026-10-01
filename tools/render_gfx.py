#!/usr/bin/env python3
"""Render EDS graphics to PNG for inspection (needs Pillow; stdlib decoders).

Pillow is NOT in the toolchain image. Install it into a throwaway dir and point
PYLIB at it, and ALWAYS write output outside the repo (never commit game assets):

  docker run --rm -v "$PWD":/work -v <scratch>:/scratch -w /work eds-decomp:latest \
      sh -c 'pip install -q --target /scratch/pylib pillow; \
             PYLIB=/scratch/pylib python3 tools/render_gfx.py card 1 /scratch/card1.png'

Modes (addresses are 0x08xxxxxx):
  card <id> <out.png>                   card art id (1..820), 72x80, 6bpp + 64-colour palette
  pack <addr> <out.png>                 "image pack" (palette + tiles + cell list), drawn on a 32x32 map
  bitmap <bmp> <pal> <w> <h> <out.png>  8bpp linear bitmap (Mode 4); bmp may be LZSS if prefixed "lz:"
  font1bpp <addr> <glyph_h> <out.png>   256-glyph 8-px-wide 1bpp font sheet (16x16 grid)
See wiki/data/graphics-formats.md, wiki/data/card-art.md, wiki/data/font.md.
"""
import os, sys, struct
sys.dont_write_bytecode = True
sys.path.insert(0, os.environ.get('PYLIB', ''))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from PIL import Image
from lzss import decompress

ROM = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'baserom.gba'), 'rb').read()
O = lambda a: a - 0x08000000 if a >= 0x08000000 else a

def rgb(c):
    return ((c & 31) << 3, ((c >> 5) & 31) << 3, ((c >> 10) & 31) << 3)

def palette(addr, n):
    return [rgb(struct.unpack_from('<H', ROM, O(addr) + 2 * i)[0]) for i in range(n)]

def blit(im, px, pal, x0, y0):
    for y in range(8):
        for x in range(8):
            v = px[y * 8 + x]
            im.putpixel((x0 + x, y0 + y), pal[v] if v < len(pal) else (255, 0, 255))

def card(cid, out):
    ART, PAL, ST = 0x082A6500, 0x08608360, 0x10E0
    raw = ROM[O(ART) + cid * ST:O(ART) + (cid + 1) * ST]
    v = int.from_bytes(raw, 'little')
    px = [(v >> (6 * i)) & 0x3F for i in range(ST * 8 // 6)]   # 5760 px = 90 tiles
    pal = palette(PAL + cid * 0x80, 64)
    im = Image.new('RGB', (72, 80))
    for t in range(90):                                        # 9 tiles wide, row-major
        blit(im, px[t * 64:(t + 1) * 64], pal, (t % 9) * 8, (t // 9) * 8)
    im.save(out)

def pack(addr, out):
    o = O(addr)
    nc = struct.unpack_from('<H', ROM, o)[0]
    pal = palette(addr + 8, nc)
    p = o + 8 + nc * 2
    nt = struct.unpack_from('<H', ROM, p)[0]; t0 = p + 8
    # guess bpp: whichever layout leaves a 4x-repeated count after the tiles
    for ts in (64, 32):
        q = t0 + nt * ts
        c = struct.unpack_from('<4H', ROM, q)
        if c[0] == c[1] == c[2] == c[3]:
            break
    ncell = c[0]
    tiles = []
    for t in range(nt):
        b = ROM[t0 + t * ts:t0 + (t + 1) * ts]
        tiles.append(list(b) if ts == 64 else [v for x in b for v in (x & 15, x >> 4)])
    im = Image.new('RGB', (256, 256))
    for i in range(ncell):
        pos, ti = struct.unpack_from('<HH', ROM, q + 8 + 4 * i)
        x, y = pos & 0x3F, pos >> 8
        if ti < nt and x < 32 and y < 32:
            blit(im, tiles[ti], pal, x * 8, y * 8)
    im.save(out)
    print(f'{nc} colours, {nt} tiles ({ts * 8 // 64}bpp), {ncell} cells')

def bitmap(bmp, pal, w, h, out):
    if bmp.startswith('lz:'):
        data, _ = decompress(ROM, O(int(bmp[3:], 16)))
    else:
        data = ROM[O(int(bmp, 16)):O(int(bmp, 16)) + w * h]
    p = palette(int(pal, 16), 256)
    im = Image.new('RGB', (w, h)); im.putdata([p[x] for x in data[:w * h]]); im.save(out)

def font1bpp(addr, h, out):
    im = Image.new('1', (16 * 9, 16 * (h + 1)), 1)
    for g in range(256):
        for r in range(h):
            row = ROM[O(addr) + g * h + r]
            for x in range(8):
                if row & (0x80 >> x):
                    im.putpixel(((g % 16) * 9 + x, (g // 16) * (h + 1) + r), 0)
    im.save(out)

if __name__ == '__main__':
    m, a = sys.argv[1], sys.argv[2:]
    if m == 'card': card(int(a[0], 0), a[1])
    elif m == 'pack': pack(int(a[0], 16), a[1])
    elif m == 'bitmap': bitmap(a[0], a[1], int(a[2]), int(a[3]), a[4])
    elif m == 'font1bpp': font1bpp(int(a[0], 16), int(a[1]), a[2])
    else: sys.exit(__doc__)
