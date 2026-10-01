#!/usr/bin/env python3
"""Re-verify the data-area facts recorded in wiki/rom/rom-map.md (stdlib only).

Run:  python3 tools/verify_rom_map.py      (prints one line per check, exits 1 on failure)
Each check reads the baserom directly; nothing is written.
"""
import struct, sys, re
sys.dont_write_bytecode = True
sys.path.insert(0, __file__.rsplit('/', 1)[0])
from lzss import decompress

ROM = open(__file__.rsplit('/tools/', 1)[0] + '/baserom.gba', 'rb').read()
O = lambda a: a - 0x08000000
u16 = lambda a: struct.unpack_from('<H', ROM, O(a))[0]
u32 = lambda a: struct.unpack_from('<I', ROM, O(a))[0]
fails = 0

def check(name, cond, detail=''):
    global fails
    print(('OK   ' if cond else 'FAIL ') + name + (f'  [{detail}]' if detail else ''))
    if not cond:
        fails += 1

def cstr(a, n):
    return ROM[O(a):O(a) + n].split(b'\0')[0]

# --- rodata start -----------------------------------------------------------
check('rodata starts with "Change BG:%d\\n" at 0x08080A20', cstr(0x08080A20, 16) == b'Change BG:%d\n')

# --- sound data ---------------------------------------------------------------
def ptr_table(a, lo, hi):
    out = []
    while lo <= u32(a) < hi:
        out.append(u32(a)); a += 4
    return out, a

s1, end1 = ptr_table(0x08088A20, 0x08088000, 0x080E1000)
check('PCM bank 1: 26 samples, table 0x08088A20-0x08088A88', len(s1) == 26 and end1 == 0x08088A88, f'{len(s1)}')
ok = all(u32(p) == 0x800 and (p + 0xC + u32(p + 4) + 0xF) & ~0xF == (s1[i + 1] if i + 1 < len(s1) else 0x080E09D0)
         for i, p in enumerate(s1))
check('PCM bank 1 headers {0x800,len,loop} chain to 0x080E09D0 (16-aligned)', ok)
songs = 0; a = 0x080E09D0
while 0x080E0000 <= u32(a) < 0x08140000:
    songs += 1; a += 0x18
check('song table 0x080E09D0: 58 x 0x18, ends at first sequence 0x080E0F40', songs == 58 and a == 0x080E0F40 == u32(0x080E09D0))
s2, end2 = ptr_table(0x0811B420, 0x0811B000, 0x08140000)
check('PCM bank 2: 36 samples, table 0x0811B420-0x0811B4B0', len(s2) == 36 and end2 == 0x0811B4B0)
last = s2[-1]
check('PCM bank 2 last sample ends at 0x08139542', last + 0xC + u32(last + 4) == 0x08139542)

# --- character table + dialogue ------------------------------------------------
check('character table: record 1 = "Yugi Muto" at 0x08139F64 + 0x84', u32(0x08139FE8) == 1 and cstr(0x08139FEC, 0x40) == b'Yugi Muto')
a = 0x0813ADF4; n = 0
while u32(a) != 0xFFFFFFFF:
    t = ROM[O(a) + 4:O(a) + 0x304]; s = t.split(b'\0')[0]
    if any(t[len(s):]) or not s:
        break
    n += 1; a += 0x304
check('dialogue: 490 x 0x304 records 0x0813ADF4-0x0819739C, then 0xFFFFFFFF', n == 490 and a == 0x0819739C)

# --- fonts --------------------------------------------------------------------
check('SJIS 8x8 font: glyph 0x8141 (index 193) first row at 0x081C060C',
      not any(ROM[O(0x081C0000):O(0x081C0600)]) and ROM[O(0x081C060C)] != 0)
for base, h in [(0x08228D00, 8), (0x08229500, 10), (0x08229F00, 12), (0x0822AB00, 16), (0x0822BB00, 8)]:
    first = next(i for i in range(256 * h) if ROM[O(base) + i])
    check(f'CP1252 font {base:#010x} (8x{h}): first ink in glyph 0x21 "!"', first // h == 0x21, f'glyph {first // h:#x}')

# --- card banks (1-based, slot 0 blank) ---------------------------------------
check('card names: slot 0 at 0x0822C720 blank, slot 1 = "7 Colored Fish"',
      not any(ROM[O(0x0822C720):O(0x0822C760)]) and cstr(0x0822C760, 0x40) == b'7 Colored Fish')
check('0x08239460-0x082461A0 all zero (821 x 0x40 unused)', not any(ROM[O(0x08239460):O(0x082461A0)]))
check('card descriptions: 0x082461A0 + id*0x1E0, id 1 = "A rare rainbow fish..."',
      cstr(0x082461A0 + 0x1E0, 0x1E0).startswith(b'A rare rainbow fish'))
check('card art: 821 x 0x10E0 from 0x082A6500 abuts palettes at 0x08608360', 0x082A6500 + 821 * 0x10E0 == 0x08608360)
check('card art slot 0 blank, slot 1 not blank', not any(ROM[O(0x082A6500):O(0x082A75E0)]) and any(ROM[O(0x082A75E0):O(0x082A75E0) + 0x10E0]))
check('card palettes: 821 x 0x80 end at 0x08621DE0', 0x08608360 + 821 * 0x80 == 0x08621DE0)
names = {cstr(0x0822C720 + i * 0x40, 0x40) for i in (u16(0x08623DF4 + 2 * n) for n in (20, 34, 81, 751))}
check('card-number map 0x08623DF4: #20/#34/#81/#751 = Exodia/Dark Magician/Red-Eyes/Jinzo',
      names == {b'Exodia the Forbidden One', b'Dark Magician', b'Red-Eyes B. Dragon', b'Jinzo'})

# --- LZSS scene sets -----------------------------------------------------------
a = 0x081976A0; nsets = 0; bad = 0
while True:
    e = struct.unpack_from('<5I', ROM, O(a))
    if not any(e) or not all(p == 0 or 0x08000000 <= p < 0x08800000 for p in e):
        break
    for i in (0, 3):
        if e[i]:
            try:
                d, _ = decompress(ROM, O(e[i]))
                if len(d) not in (0x4B00, 0x5A00, 0x5A01, 0x2000, 0x2001):
                    bad += 1
            except ValueError:
                bad += 1
    nsets += 1; a += 0x14
check('scene-set table 0x081976A0: 31 descriptors, every LZSS blob decodes to 0x4B00/0x5A00/0x2000 (+1)', nsets == 31 and bad == 0, f'{nsets} sets, {bad} bad')
d, n = decompress(ROM, O(0x0874C650))
check('dialogue-box bitmap 0x0874C650 decodes to 240x64 (0x3C00)', len(d) == 0x3C00)

# --- Mode 4 bitmaps --------------------------------------------------------------
check('Mode-4 bitmap table 0x08198440: 5 x {pal, bitmap} with bitmap+0x9600 == pal',
      all(u32(0x08198440 + 8 * i + 4) + 0x9600 == u32(0x08198440 + 8 * i) for i in range(5)))

# --- misc ------------------------------------------------------------------------
check('ASCII->SJIS table 0x081A76A0: " "->0x8140, "A"->0x8260, "a"->0x8281',
      u16(0x081A76A0) == 0x8140 and u16(0x081A76A0 + 2 * (0x41 - 0x20)) == 0x8260 and u16(0x081A76A0 + 2 * (0x61 - 0x20)) == 0x8281)
check('sine table 0x081ABC4C: [0]=0, [64]=0x1000', u16(0x081ABC4C) == 0 and u16(0x081ABC4C + 128) == 0x1000)
check('zero pad 0x081ABE4C-0x081C0000', not any(ROM[O(0x081ABE4C):O(0x081C0000)]))
check('zero pad 0x087F8568-0x08800000 (last non-zero byte 0x087F8567)', ROM[O(0x087F8567)] != 0 and not any(ROM[O(0x087F8568):]))
check('no Thumb/ARM SWI other than 0x0C/0x0B/0x06 in .text',
      sorted(ROM[o] for o in range(0xC0, 0x80A20, 2) if ROM[o + 1] == 0xDF and struct.unpack_from('<H', ROM, o + 2)[0] == 0x4770) == [6, 0xB, 0xC])

print(f'\n{fails} failure(s)')
sys.exit(1 if fails else 0)
