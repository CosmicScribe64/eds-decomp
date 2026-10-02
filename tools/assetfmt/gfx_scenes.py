"""Scene graphics: the dialogue ("bust-up") scene sets, the dialogue box and their pointer tables.

Types (see build/assetwf/gfx_scenes/NOTES.md and wiki/data/graphics-formats.md):

  gfx_scenes_set    one scene set: LZSS bitmap, BG palette [, OBJ palette, LZSS OBJ tiles]
                    -> <path>/bitmap.png [+ sprites.png] + scene.json          (option set=N)
  gfx_scenes_box    dialogue box: LZSS box bitmap, LZSS header strip, 256- and 16-colour palettes
                    -> <path>/box.png, header.png, box.pal, text.pal, dialogue_box.json
  gfx_scenes_table  the 31 scene-set descriptors {bitmap, bgPal, objPal, objTiles, anim}  -> JSON
  gfx_scenes_ptrs   a plain u32 pointer list (kind=func annotates Thumb function pointers) -> JSON

LZSS is the game's Okumura-style format (sub_0807A1A8). lz_encode() reproduces the original compressor
exactly (all 59 streams): Okumura's binary-tree match finder (one tree per first byte, bytes 1-17
compared, first longest match wins, a full 18-byte match replaces the old node), no pre-inserted
"space" strings, 0xFF read past the end of the input, and a window node that leaves the window is
removed by merging its two subtrees by age (oldest on top; a node that replaced another inherits that
node's age). Edited data is compressed the same way; if the result no longer fits the original slot,
lz_encode_optimal() (shortest parse) is tried before giving up, because the ROM layout is fixed.
"""
import json
import re
import struct

NIL = -1
F = 18          # longest match
WIN = 4078      # farthest match distance (N - F)
TH = 2          # matches of THRESHOLD bytes or fewer are sent as literals
BASE = 0x08000000

# Character ID -> scene set, from sub_08001C78 (the portrait lookup). Inverted for annotations.
SET_CHARACTER = {0: 34, 1: 35, 2: 37, 3: 32, 4: 33, 5: 2, 6: 1, 7: 3, 8: 5, 9: 4, 10: 20, 11: 16, 12: 19,
                 13: 24, 14: 8, 15: 10, 16: 6, 17: 7, 18: 9, 19: 11, 20: 12, 21: 17, 22: 13, 23: 18, 24: 15,
                 25: 21, 26: 22, 27: 23, 28: 14, 29: 39, 30: 38}
DUELISTS = (0x08139F64, 0x84, 28)   # duelist table: {u32 id; char name[0x40]; char short[0x40]}
SCENE_TABLE = 0x081976A0


# ------------------------------------------------------------------------------------------------ LZSS
def lz_decode(blob):
    """{u32 packedSize; stream} -> decoded bytes (as sub_0807A1A8: 4 KiB ring, first write at 0xFEE)."""
    n = struct.unpack_from('<I', blob)[0]
    src, end = 4, 4 + n
    if end > len(blob):
        raise ValueError('LZSS stream runs past the end of its slot')
    ring = bytearray(4096)
    r = 0xFEE
    out = bytearray()
    while src < end:
        flags = blob[src]
        src += 1
        for _ in range(8):
            if src >= end:
                break
            if flags & 1:
                c = blob[src]
                src += 1
                out.append(c)
                ring[r] = c
                r = (r + 1) & 0xFFF
            else:
                if src + 2 > end:
                    raise ValueError('LZSS reference cut off by the end of the stream')
                p = blob[src] | (blob[src + 1] & 0xF0) << 4
                ln = (blob[src + 1] & 0x0F) + 3
                src += 2
                for _ in range(ln):
                    c = ring[p]
                    out.append(c)
                    ring[r] = c
                    r = (r + 1) & 0xFFF
                    p = (p + 1) & 0xFFF
            flags >>= 1
    return bytes(out), end


class _Tree:
    """Okumura match tree over absolute input positions, with the original tool's deletion rule."""

    def __init__(self, data):
        n = len(data)
        self.buf = bytes(data) + b'\xff' * F   # the original compressor read 0xFF past the end
        self.l = [NIL] * n
        self.r = [NIL] * n
        self.d = [NIL] * n        # parent; <= -2 means "root of tree -2-d"
        self.age = list(range(n))
        self.root = [NIL] * 256

    def _relink(self, parent, old, new):
        if parent <= -2:
            self.root[-2 - parent] = new
        elif self.r[parent] == old:
            self.r[parent] = new
        else:
            self.l[parent] = new

    def insert(self, x):
        """Add position x; return (longest match length, its position) as Okumura's InsertNode."""
        buf, l, r, d = self.buf, self.l, self.r, self.d
        c0 = buf[x]
        l[x] = r[x] = NIL
        p = self.root[c0]
        if p == NIL:
            self.root[c0] = x
            d[x] = -2 - c0
            return 0, 0
        ml, mp = 0, 0
        while True:
            i = 1
            while i < F and buf[x + i] == buf[p + i]:
                i += 1
            if i > ml:
                ml, mp = i, p
                if i >= F:                      # same 18 bytes: x takes p's place (and age)
                    self.age[x] = self.age[p]
                    lp, rp = l[p], r[p]
                    d[x], l[x], r[x] = d[p], lp, rp
                    if lp != NIL:
                        d[lp] = x
                    if rp != NIL:
                        d[rp] = x
                    self._relink(d[p], p, x)
                    d[p] = NIL
                    return ml, mp
            if i < F and buf[x + i] < buf[p + i]:
                if l[p] == NIL:
                    l[p] = x
                    d[x] = p
                    return ml, mp
                p = l[p]
            else:
                if r[p] == NIL:
                    r[p] = x
                    d[x] = p
                    return ml, mp
                p = r[p]

    def delete(self, p):
        l, r, d, age = self.l, self.r, self.d, self.age
        if p < 0 or d[p] == NIL:
            return
        a, b = l[p], r[p]
        if a == NIL or b == NIL:
            q = b if a == NIL else a
        else:                                   # merge the subtrees, older node on top
            q = NIL
            attach, side = NIL, 0
            while a != NIL and b != NIL:
                if age[a] < age[b]:
                    nxt, nside, nxt_a, nxt_b = a, 1, r[a], b
                else:
                    nxt, nside, nxt_a, nxt_b = b, 0, a, l[b]
                if attach == NIL:
                    q = nxt
                else:
                    if side:
                        r[attach] = nxt
                    else:
                        l[attach] = nxt
                    d[nxt] = attach
                attach, side, a, b = nxt, nside, nxt_a, nxt_b
            rest = a if a != NIL else b
            if side:
                r[attach] = rest
            else:
                l[attach] = rest
            if rest != NIL:
                d[rest] = attach
        if q != NIL:
            d[q] = d[p]
        self._relink(d[p], p, q)
        d[p] = NIL


def _emit(tokens):
    """[(pos, len, src)] (len 1 = literal) -> stream bytes."""
    out = bytearray()
    code = bytearray()
    flags, bit = 0, 0
    for tok in tokens:
        if tok[0] == 'L':
            flags |= 1 << bit
            code.append(tok[1])
        else:
            _, ln, src = tok
            rp = (src + 0xFEE) & 0xFFF
            code += bytes((rp & 0xFF, (rp >> 4) & 0xF0 | (ln - TH - 1)))
        bit += 1
        if bit == 8:
            out.append(flags)
            out += code
            flags, bit, code = 0, 0, bytearray()
    if bit:
        out.append(flags)
        out += code
    return bytes(out)


def lz_encode(data):
    """Compress exactly as the original tool did (greedy, Okumura tree). Returns the stream only."""
    n = len(data)
    if n == 0:
        return b''
    t = _Tree(data)
    tokens = []
    ml, mp = t.insert(0)
    pos = 0
    while pos < n:
        ln = min(ml, n - pos)
        if ln <= TH:
            ln = 1
            tokens.append(('L', data[pos]))
        else:
            tokens.append(('R', ln, mp))
        for k in range(pos + 1, pos + ln + 1):
            t.delete(k - WIN - 1)
            if k < n:
                ml, mp = t.insert(k)
        pos += ln
    return _emit(tokens)


def lz_encode_optimal(data):
    """Shortest parse for this format (used only when an edit no longer fits the original slot)."""
    n = len(data)
    t = _Tree(data)
    best = [t.insert(0)]
    for x in range(1, n):
        t.delete(x - WIN - 1)
        best.append(t.insert(x))
    cost = [0] * (n + 1)
    step = [1] * (n + 1)
    for x in range(n - 1, -1, -1):
        c, s = 9 + cost[x + 1], 1
        for ln in range(TH + 1, min(best[x][0], n - x) + 1):
            if 17 + cost[x + ln] < c:
                c, s = 17 + cost[x + ln], ln
        cost[x], step[x] = c, s
    tokens, x = [], 0
    while x < n:
        if step[x] == 1:
            tokens.append(('L', data[x]))
        else:
            tokens.append(('R', step[x], best[x][1]))
        x += step[x]
    return _emit(tokens)


def lz_blob(data, slot, what):
    """{u32 size; stream} for `data` that fits in `slot` bytes (the original encoder first)."""
    s = lz_encode(data)
    if 4 + len(s) > slot:
        s = lz_encode_optimal(data)
        if 4 + len(s) > slot:
            raise ValueError(f'{what}: compresses to {4 + len(s)} bytes but its slot holds {slot} '
                             '(the ROM layout is fixed; simplify the edit)')
    return struct.pack('<I', len(s)) + s


# --------------------------------------------------------------------------------------- helpers
def register(A):
    def pal_to_rgb(raw):
        cols = struct.unpack(f'<{len(raw) // 2}H', raw)
        return [A.bgr555_to_rgb(c) for c in cols], [i for i, c in enumerate(cols) if c & 0x8000]

    def rgb_to_pal(rgb, count, bit15):
        rgb = (list(rgb) + [(0, 0, 0)] * count)[:count]
        vals = [A.rgb_to_bgr555(c) for c in rgb]
        for i in bit15:
            vals[i] |= 0x8000
        return struct.pack(f'<{count}H', *vals)

    def jasc(rgb):
        return ('\r\n'.join(['JASC-PAL', '0100', str(len(rgb))] + ['%d %d %d' % c for c in rgb]) + '\r\n').encode()

    def unjasc(text):
        v = text.decode().split()
        n = int(v[2])
        return [tuple(int(x) for x in v[3 + 3 * i:6 + 3 * i]) for i in range(n)]

    def dump(obj):
        """Indented JSON with lists of numbers kept on one line."""
        text = json.dumps(obj, indent=1, ensure_ascii=False)
        text = re.sub(r'\[[\s\d,-]*\]',
                      lambda m: '[' + ', '.join(x for x in re.split(r'[\s,]+', m.group(0)[1:-1]) if x) + ']', text)
        return (text + '\n').encode()

    def hexaddr(a):
        return '0x%08X' % a

    row_cache = []

    def rows():
        """Every manifest row (ignoring --only), for pointer annotations."""
        if not row_cache:
            try:
                for line in open(A.MANIFEST):
                    f = line.split('#')[0].split()
                    if len(f) >= 4:
                        row_cache.append((int(f[0], 16), int(f[1], 16), f[2], f[3]))
            except OSError:
                pass
        return row_cache

    def set_layout(data, start):
        """Components of one scene-set row: [(name, offset, slot)]; raises if the row isn't one."""
        comps, off = [], 0
        n = struct.unpack_from('<I', data, 0)[0]
        end = (start + 4 + n + 3) & ~3
        comps.append(('bitmap', 0, end - start))
        off = end - start
        comps.append(('bg_palette', off, 0x200))
        off += 0x200
        if off < len(data):
            comps.append(('obj_palette', off, 0x200))
            off += 0x200
            n = struct.unpack_from('<I', data, off)[0]
            end = (start + off + 4 + n + 3) & ~3
            comps.append(('obj_tiles', off, end - start - off))
            off = end - start
        if off != len(data):
            raise ValueError(f'scene set layout ends at +0x{off:X}, the row is 0x{len(data):X} bytes')
        return comps

    BOX_PARTS = [('box', 'box.png'), ('header', 'header.png'), ('box_palette', 'box.pal'), ('text_palette', 'text.pal')]

    def box_layout(data, start):
        n0 = struct.unpack_from('<I', data, 0)[0]
        o1 = (start + 4 + n0 + 3 & ~3) - start
        n1 = struct.unpack_from('<I', data, o1)[0]
        o2 = (start + o1 + 4 + n1 + 3 & ~3) - start
        if o2 + 0x220 != len(data):
            raise ValueError('dialogue box layout does not fill its row')
        return [('box', 0, o1), ('header', o1, o2 - o1), ('box_palette', o2, 0x200), ('text_palette', o2 + 0x200, 0x20)]

    def describe(addr, rom):
        """Human note for a pointer target: asset file/part, or asset path + offset."""
        for s, e, typ, path in rows():
            if s <= addr < e:
                if rom is not None and typ in ('gfx_scenes_set', 'gfx_scenes_box'):
                    try:
                        if typ == 'gfx_scenes_set':
                            lay = set_layout(rom[s - BASE:e - BASE], s)
                            files = {'bitmap': 'bitmap.png', 'bg_palette': 'bitmap.png (its palette)',
                                     'obj_palette': 'sprites.png (its palette)', 'obj_tiles': 'sprites.png'}
                        else:
                            lay = box_layout(rom[s - BASE:e - BASE], s)
                            files = dict(BOX_PARTS)
                        for name, off, _ in lay:
                            if s + off == addr:
                                return f'{path}/{files[name]}'
                    except (ValueError, struct.error):
                        pass
                return path if addr == s else f'{path}+0x{addr - s:X}'
        return None

    def ptr_note(v, rom, func=False):
        if v == 0:
            return None
        if func:
            note = f'sub_{v & ~1:08X}' + (' (Thumb)' if v & 1 else ' (ARM)')
        else:
            note = describe(v, rom)
        return hexaddr(v) + (f' {note}' if note else '')

    def ptr_value(s):
        if s is None:
            return 0
        v = int(str(s).split()[0], 16)
        if not 0 <= v < 1 << 32:
            raise ValueError(f'pointer {s!r} out of range')
        return v

    def char_name(cid, rom):
        if rom is not None:
            a, size, count = DUELISTS
            for i in range(count):
                o = a - BASE + i * size
                if struct.unpack_from('<I', rom, o)[0] == cid and cid:
                    return f'{A.dec(rom[o + 4:o + 0x44].split(bytes(1))[0])} (character {cid})'
        return f'character {cid}'

    # ------------------------------------------------------------------------------- image helpers
    def bitmap_png(raw, w, pal_rgb):
        h = len(raw) // w
        return A.png_write(w, h, raw[:w * h], pal_rgb), raw[w * h:]

    def read_png(read, rel, w, h):
        pw, ph, px, plte = A.png_read(read(rel))
        if (pw, ph) != (w, h):
            raise ValueError(f'{rel} must be {w}x{h} (it is {pw}x{ph})')
        return px, plte

    OBJ_SIZE = {(0, 0): (1, 1), (0, 1): (2, 2), (0, 2): (4, 4), (0, 3): (8, 8), (1, 0): (2, 1), (1, 1): (4, 1),
                (1, 2): (4, 2), (1, 3): (8, 4), (2, 0): (1, 2), (2, 1): (1, 4), (2, 2): (2, 4), (2, 3): (4, 8)}

    def tile_banks(rom, set_index):
        """Palette bank of each sprite tile, from the set's animation frames (for viewing only).
        anim = NULL-terminated list of step arrays {u8 frames; u8 count; u16 pad; u32 frame}, a step with
        frames 0 ends the array; a frame is `count` OAM templates {u16 attr0, attr1, attr2, pad}, where
        attr2 bits 0-7 are row<<4 | column in the 16-tile-wide sheet and bits 12-15 the palette bank."""
        banks = {}
        if rom is None or set_index is None:
            return banks
        try:
            def u32(a):
                return struct.unpack_from('<I', rom, a - BASE)[0]
            anim = u32(SCENE_TABLE + 0x14 * set_index + 16)
            for i in range(64 if anim else 0):
                seq = u32(anim + 4 * i)
                if seq == 0:
                    break
                for k in range(256):
                    frames, count = rom[seq - BASE + 8 * k], rom[seq - BASE + 8 * k + 1]
                    if frames == 0:
                        break
                    frame = u32(seq + 8 * k + 4)
                    for j in range(count):
                        a0, a1, a2 = struct.unpack_from('<3H', rom, frame - BASE + 8 * j)
                        w, h = OBJ_SIZE.get((a0 >> 14, a1 >> 14), (1, 1))
                        for dy in range(h):
                            for dx in range(w):
                                if (a2 & 15) + dx < 16:
                                    banks[((a2 >> 4 & 15) + dy) * 16 + (a2 & 15) + dx] = a2 >> 12
        except (struct.error, IndexError):
            return {}
        return banks

    def tiles_png(raw, pal_rgb, banks=None, cols=16):
        """4bpp 8x8 tiles -> 8-bit PNG, `cols` tiles wide (16 = one 0x200-byte OBJ VRAM row).
        Pixel value = bank * 16 + colour, so each tile shows with its palette bank (bank 0 if unknown)."""
        nt = len(raw) // 32
        rws = (nt + cols - 1) // cols
        W, H = cols * 8, rws * 8
        img = bytearray(W * H)
        for t in range(nt):
            tx, ty = t % cols * 8, t // cols * 8
            hi = (banks or {}).get(t, 0) << 4
            for i in range(64):
                b = raw[t * 32 + i // 2]
                img[(ty + i // 8) * W + tx + i % 8] = hi | (b >> 4 if i & 1 else b & 15)
        return A.png_write(W, H, bytes(img), pal_rgb), raw[nt * 32:]

    def png_tiles(px, W, nt, rel, cols=16):
        out = bytearray()
        for t in range(nt):
            tx, ty = t % cols * 8, t // cols * 8
            vals = [px[(ty + i // 8) * W + tx + i % 8] for i in range(64)]
            if len({v >> 4 for v in vals}) > 1:
                raise ValueError(f'{rel}: tile {t} mixes colours from different 16-colour banks')
            for i in range(0, 64, 2):
                out.append(vals[i] & 15 | (vals[i + 1] & 15) << 4)
        return bytes(out)

    def place(buf, off, slot, blob, what):
        if len(blob) > slot:
            raise ValueError(f'{what} is {len(blob)} bytes, its slot holds {slot}')
        buf[off:off + len(blob)] = blob

    def check_pad(data, off, used, slot, what):
        if data[off + used:off + slot].strip(b'\0'):
            raise ValueError(f'{what}: padding after the data is not zero')

    # ------------------------------------------------------------------------------- scene set
    def x_set(data, p, rom):
        lay = set_layout(data, p['start'])
        d = p['path']
        meta = {'set': int(p['set']) if 'set' in p else None}
        if meta['set'] is not None and meta['set'] in SET_CHARACTER:
            meta['character'] = char_name(SET_CHARACTER[meta['set']], rom)
        meta['layout'] = {name: hexaddr(p['start'] + off) for name, off, _ in lay}
        files = {}
        comp = {name: (off, slot) for name, off, slot in lay}
        off, slot = comp['bitmap']
        raw, used = lz_decode(data[off:off + slot])
        check_pad(data, off, used, slot, 'bitmap')
        bg_rgb, bg_hi = pal_to_rgb(data[comp['bg_palette'][0]:comp['bg_palette'][0] + 0x200])
        png, tail = bitmap_png(raw, 240, bg_rgb)
        files[f'{d}/bitmap.png'] = png
        meta['bitmap'] = {'width': 240, 'height': len(raw) // 240, 'trailing_bytes': tail.hex()}
        meta['bg_palette_bit15'] = bg_hi
        if 'obj_tiles' in comp:
            ob_rgb, ob_hi = pal_to_rgb(data[comp['obj_palette'][0]:comp['obj_palette'][0] + 0x200])
            off, slot = comp['obj_tiles']
            raw, used = lz_decode(data[off:off + slot])
            check_pad(data, off, used, slot, 'sprites')
            png, tail = tiles_png(raw, ob_rgb, tile_banks(rom, meta['set']))
            files[f'{d}/sprites.png'] = png
            meta['sprites'] = {'tiles': len(raw) // 32, 'trailing_bytes': tail.hex()}
            meta['obj_palette_bit15'] = ob_hi
        files[f'{d}/scene.json'] = dump(meta)
        return files

    def b_set(read, p):
        d = p['path']
        meta = json.loads(read(f'{d}/scene.json'))
        start, size = p['start'], p['end'] - p['start']
        offs = sorted((int(v, 16) - start, k) for k, v in meta['layout'].items())
        slots = {k: (o, (offs[i + 1][0] if i + 1 < len(offs) else size) - o) for i, (o, k) in enumerate(offs)}
        buf = bytearray(size)
        bm = meta['bitmap']
        px, plte = read_png(read, f'{d}/bitmap.png', bm['width'], bm['height'])
        raw = px + bytes.fromhex(bm['trailing_bytes'])
        off, slot = slots['bitmap']
        place(buf, off, slot, lz_blob(raw, slot, f'{d}/bitmap.png'), 'bitmap')
        off, slot = slots['bg_palette']
        place(buf, off, slot, rgb_to_pal(plte, 256, meta['bg_palette_bit15']), 'bg palette')
        if 'sprites' in meta:
            sp = meta['sprites']
            nt = sp['tiles']
            W, H, px, plte = A.png_read(read(f'{d}/sprites.png'))
            if (W, H) != (128, (nt + 15) // 16 * 8):
                raise ValueError(f'{d}/sprites.png must be 128x{(nt + 15) // 16 * 8}')
            raw = png_tiles(px, W, nt, f'{d}/sprites.png') + bytes.fromhex(sp['trailing_bytes'])
            off, slot = slots['obj_palette']
            place(buf, off, slot, rgb_to_pal(plte, 256, meta['obj_palette_bit15']), 'obj palette')
            off, slot = slots['obj_tiles']
            place(buf, off, slot, lz_blob(raw, slot, f'{d}/sprites.png'), 'sprites')
        return bytes(buf)

    # ------------------------------------------------------------------------------- dialogue box
    def x_box(data, p, rom):
        lay = box_layout(data, p['start'])
        d = p['path']
        comp = {name: (off, slot) for name, off, slot in lay}
        pal_rgb, pal_hi = pal_to_rgb(data[comp['box_palette'][0]:comp['box_palette'][0] + 0x200])
        txt_rgb, txt_hi = pal_to_rgb(data[comp['text_palette'][0]:comp['text_palette'][0] + 0x20])
        meta = {'layout': {name: hexaddr(p['start'] + off) for name, off, _ in lay}}
        files = {}
        for name in ('box', 'header'):
            off, slot = comp[name]
            raw, used = lz_decode(data[off:off + slot])
            check_pad(data, off, used, slot, name)
            png, tail = bitmap_png(raw, 240, pal_rgb)
            files[f'{d}/{name}.png'] = png
            meta[name] = {'width': 240, 'height': len(raw) // 240, 'trailing_bytes': tail.hex()}
        files[f'{d}/box.pal'] = jasc(pal_rgb)
        files[f'{d}/text.pal'] = jasc(txt_rgb)
        meta['box_palette_bit15'] = pal_hi
        meta['text_palette_bit15'] = txt_hi
        files[f'{d}/dialogue_box.json'] = dump(meta)
        return files

    def b_box(read, p):
        d = p['path']
        meta = json.loads(read(f'{d}/dialogue_box.json'))
        start, size = p['start'], p['end'] - p['start']
        offs = sorted((int(v, 16) - start, k) for k, v in meta['layout'].items())
        slots = {k: (o, (offs[i + 1][0] if i + 1 < len(offs) else size) - o) for i, (o, k) in enumerate(offs)}
        buf = bytearray(size)
        for name in ('box', 'header'):
            m = meta[name]
            px, _ = read_png(read, f'{d}/{name}.png', m['width'], m['height'])
            off, slot = slots[name]
            place(buf, off, slot, lz_blob(px + bytes.fromhex(m['trailing_bytes']), slot, f'{d}/{name}.png'), name)
        off, slot = slots['box_palette']
        place(buf, off, slot, rgb_to_pal(unjasc(read(f'{d}/box.pal')), 256, meta['box_palette_bit15']), 'box.pal')
        off, slot = slots['text_palette']
        place(buf, off, slot, rgb_to_pal(unjasc(read(f'{d}/text.pal')), 16, meta['text_palette_bit15']), 'text.pal')
        return bytes(buf)

    # ------------------------------------------------------------------------------- tables
    FIELDS = ['bitmap', 'bg_palette', 'obj_palette', 'obj_tiles', 'anim']

    def x_table(data, p, rom):
        items = []
        for i in range(len(data) // 0x14):
            vals = struct.unpack_from('<5I', data, i * 0x14)
            e = {'set': i}
            if i in SET_CHARACTER:
                e['character'] = char_name(SET_CHARACTER[i], rom)
            for k, v in zip(FIELDS, vals):
                e[k] = ptr_note(v, rom)
            items.append(e)
        return {p['path']: dump(items)}

    def b_table(read, p):
        return b''.join(struct.pack('<5I', *(ptr_value(e[k]) for k in FIELDS)) for e in json.loads(read(p['path'])))

    def x_ptrs(data, p, rom):
        func = p.get('kind') == 'func'
        vals = struct.unpack(f'<{len(data) // 4}I', data)
        return {p['path']: dump([ptr_note(v, rom, func) for v in vals])}

    def b_ptrs(read, p):
        vals = [ptr_value(v) for v in json.loads(read(p['path']))]
        return struct.pack(f'<{len(vals)}I', *vals)

    return {
        'gfx_scenes_set': (x_set, b_set),
        'gfx_scenes_box': (x_box, b_box),
        'gfx_scenes_table': (x_table, b_table),
        'gfx_scenes_ptrs': (x_ptrs, b_ptrs),
    }
