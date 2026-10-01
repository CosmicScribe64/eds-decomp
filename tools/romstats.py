#!/usr/bin/env python3
"""Per-block statistics for the baserom (stdlib only).

Usage: python3 tools/romstats.py [start] [end] [blocksize]
  addresses may be 0x08xxxxxx or file offsets; default 0x08080A24..0x08800000, 0x1000.
Columns: z=zero fraction, ff=0xFF fraction, ent=Shannon entropy (bits/byte),
ptr=fraction of aligned u32 words that are 0x08000000..0x087FFFFF,
asc=printable-ASCII fraction, pal=fraction of u16 with bit15 clear (BGR555 candidates),
nib=fraction of bytes whose both nibbles are < 0x10 (always 1) -> replaced by
lo=fraction of bytes < 0x10 (4bpp tile data using palette slots 0-15 in low nibble only).
"""
import sys, struct, math, collections

ROM = open(__file__.rsplit('/tools/', 1)[0] + '/baserom.gba', 'rb').read()

def a2o(x):
    x = int(x, 16) if isinstance(x, str) else x
    return x - 0x08000000 if x >= 0x08000000 else x

def entropy(b):
    c = collections.Counter(b); n = len(b)
    return -sum(v / n * math.log2(v / n) for v in c.values()) if n else 0.0

def stats(b):
    n = len(b)
    words = struct.unpack_from('<%dI' % (n // 4), b)
    halves = struct.unpack_from('<%dH' % (n // 2), b)
    return dict(
        z=b.count(0) / n,
        ff=b.count(0xff) / n,
        ent=entropy(b),
        ptr=sum(1 for w in words if 0x08000000 <= w < 0x08800000) / len(words),
        asc=sum(1 for x in b if 32 <= x < 127) / n,
        pal=sum(1 for h in halves if h < 0x8000) / len(halves),
    )

def main():
    s = a2o(sys.argv[1]) if len(sys.argv) > 1 else 0x80A24
    e = a2o(sys.argv[2]) if len(sys.argv) > 2 else 0x800000
    bs = int(sys.argv[3], 0) if len(sys.argv) > 3 else 0x1000
    s &= ~3
    for off in range(s, e, bs):
        st = stats(ROM[off:min(off + bs, e)])
        print(f"{0x08000000+off:08x} z={st['z']:.2f} ff={st['ff']:.2f} ent={st['ent']:.2f} "
              f"ptr={st['ptr']:.3f} asc={st['asc']:.2f} pal={st['pal']:.2f}")

if __name__ == '__main__':
    main()
