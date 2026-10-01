#!/usr/bin/env python3
"""EDS custom LZSS codec helper (stdlib only).

Format (hypothesis confirmed against many ROM blobs, see wiki/data/graphics-formats.md):
  u32  packed_size            (bytes of LZSS stream that follow; blob is padded to 4)
  u8[] LZSS stream, Okumura-style:
       flag byte, LSB first; bit=1 -> literal byte, bit=0 -> 2-byte ref
       ref b0,b1: pos = b0 | (b1 & 0xF0) << 4 ; len = (b1 & 0x0F) + 3
       4096-byte ring buffer, initial write pos 0xFEE, pre-filled with 0x00
Usage:
  python3 tools/lzss.py <addr>            -> prints packed/unpacked sizes
  python3 tools/lzss.py <addr> out.bin    -> also writes decompressed bytes
"""
import sys, struct

ROM_PATH = __file__.rsplit('/tools/', 1)[0] + '/baserom.gba'

def a2o(x):
    x = int(x, 16) if isinstance(x, str) else x
    return x - 0x08000000 if x >= 0x08000000 else x

def decompress(buf, off, fill=0, strict=True):
    """Return (data, packed_size). Raises ValueError on malformed stream if strict."""
    n = struct.unpack_from('<I', buf, off)[0]
    if strict and (n == 0 or n > 0x40000 or off + 4 + n > len(buf)):
        raise ValueError('bad size %#x' % n)
    src = off + 4; end = src + n
    ring = bytearray([fill]) * 4096
    r = 0xFEE
    out = bytearray()
    while src < end:
        flags = buf[src]; src += 1
        for bit in range(8):
            if src >= end:
                break
            if flags & 1:
                c = buf[src]; src += 1
                out.append(c); ring[r] = c; r = (r + 1) & 0xFFF
            else:
                if src + 1 >= end + (0 if strict else 1):
                    if strict: raise ValueError('truncated ref')
                    break
                b0 = buf[src]; b1 = buf[src + 1]; src += 2
                p = b0 | ((b1 & 0xF0) << 4); l = (b1 & 0x0F) + 3
                for k in range(l):
                    c = ring[(p + k) & 0xFFF]
                    out.append(c); ring[r] = c; r = (r + 1) & 0xFFF
            flags >>= 1
    return bytes(out), n

def main():
    rom = open(ROM_PATH, 'rb').read()
    off = a2o(sys.argv[1])
    data, n = decompress(rom, off)
    print(f"{0x08000000+off:08x}: packed {n:#x} -> unpacked {len(data):#x}; next blob at "
          f"{0x08000000+((off+4+n+3)&~3):08x}")
    if len(sys.argv) > 2:
        open(sys.argv[2], 'wb').write(data)

if __name__ == '__main__':
    main()
