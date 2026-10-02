#include "global.h"

extern void sub_08075294(void *dst, const void *src, u32 n);
extern void sub_08075278(void *p, u32 n);
extern int sub_08073784(void *p, u32 n);
extern void sub_0807373C(void);
extern void sub_080735D4(void *a, void *b);
extern u8 gUnk_081A7374[];
extern u8 gUnk_081A7382[];
extern u8 gUnk_081A7390[];
extern s8 gUnk_08087600[];
extern u8 gUnk_08087604[];
extern u8 gUnk_08087614[];
extern void sub_0801A7DC(const u8 *fmt, ...);
extern void sub_0801A7E8(void);
extern u16 sub_08073F04(s32 id, void *buf, u32 n);
extern void sub_08072010(void *p);
extern void sub_08071FA0(void);
extern u16 sub_08072238_u16(int idx) asm("sub_08072238");
#define REG_SIOCNT (*(vu16 *)0x04000128)
extern u8 gUnk_03005204[];
extern u32 sub_0807F0AC(u32 a, u32 b);

typedef struct { u8 b[12]; } Pkt;

/* Link packet buffers at 0x030049D0 (size 0x840). */
struct LinkBuf {
    Pkt rx[64];          /* 0x000 received packets, compacted by sub_08071FA0 */
    u16 rxCount;         /* 0x300 */
    u8 pad302[0x30E - 0x302];
    u8 cur[12];          /* 0x30E */
    u8 pad31a[0x51C - 0x31A];
    u16 lastId;          /* 0x51C */
    u16 st51E;
    u16 pad520;
    u16 st522;
    u8 busy : 1;         /* 0x524 bit 0: handler running */
    u8 fl1 : 1;
    u8 fl2 : 1;          /* bit 2: data packet received */
    u8 pad525;
    u16 st526;
    u16 wr;              /* 0x528 */
    u16 rd;              /* 0x52A tx read index */
    Pkt tx[64];          /* 0x52C */
    int i;               /* 0x830 */
};
extern struct LinkBuf gUnk_030049D0;
extern u8 gUnk_03000000[];
struct MainFn {
    u8 pad[0x418];
    void *hook;         /* per-frame link hook (sub_08072054) */
};
extern struct MainFn gUnk_03000040;
extern int sub_08072054(void);
struct MainVar {
    u8 pad[0x441C];
    u16 a, b;
};
extern struct MainVar gUnk_03000040_v asm("gUnk_03000040");
extern u8 gUnk_0300045C[];
struct MainMap {
    u8 pad[0x41C];
    u16 map[0x2000];    /* BG map buffer at 0x0300045C */
    u16 a, w;           /* 0x441C, 0x441E */
};
extern struct MainMap gUnk_03000040_m asm("gUnk_03000040");
struct MainW {
    u8 pad[0x441E];
    u16 w;
};
extern struct MainW gUnk_03000040_w asm("gUnk_03000040");
extern u16 sub_08074A90(u8 c);
extern u8 gUnk_03000C5C[];
extern u8 gUnk_08608360[];
extern u16 gUnk_082A6500[];
extern u16 sub_080728C0_u16(u16 c) asm("sub_080728C0");
extern u16 sub_080729F8_u16(u16 c) asm("sub_080729F8");
extern u8 gUnk_0822BB00[];
extern u8 gUnk_081C0000[];
extern u32 gUnk_0808765C[];
extern u32 gUnk_08087634[];
struct HW {
    u16 lo, hi;
};
extern u16 sub_080725B0(u16 nib, u16 b, u16 c);
extern u32 sub_08072584(u16 x);
extern void sub_08072778(u8 ch, u16 *dst, u16 b, u16 c);
extern void sub_08072808(u16 ch, u16 *dst, u16 b, u16 c);
extern void sub_08072BB4(u16 col, u16 pk, u16 tile, char *s);

extern u16 sub_08071F40_u16(void *p) asm("sub_08071F40");

/* Send a packet: stamp the sequence nibble, transmit, and on success ack it. */
int sub_08071F40(void *p)
{
    int ok;
    sub_08075294(gUnk_03005204, p, 12);
    *(u16 *)gUnk_03005204 = (*(u16 *)gUnk_03005204 & 0xF0FF) | (*(u16 *)(gUnk_03005204 - 0x31A) << 8);
    if (sub_08073784(gUnk_03005204, 12)) {
        sub_08075294(gUnk_03005204 - 0x532, gUnk_03005204, 12);
        *(u16 *)(gUnk_03005204 - 0x31A) = (*(u16 *)(gUnk_03005204 - 0x31A) + 1) & 0xF;
        ok = 1;
    } else {
        ok = 0;
    }
    return ok;
}

/* Pop the head of the received queue (after re-sending the ack) and shift the rest down. */
void sub_08071FA0(void)
{
    u8 *b = (u8 *)&gUnk_030049D0;
    u16 *n = (u16 *)(b + 0x300);
    int *ip;
    if (*n != 0 && (u16)sub_08071F40(b)) {
        u16 left = *n - 1;
        int zero = 0;
        *n = left;
        ip = (int *)(b + 0x830);
        *ip = zero;
        if (*ip < *n) {
            u8 *queue = b;
            int *counter = ip;
            do {
                u32 off = *counter * 12;
                void *dst = (void *)(off + (u32)queue);
                /* FAKEMATCH: preserve the separate next-packet address. */
                register u32 source asm("r2") = (u32)queue;
                source += 12;
                sub_08075294(dst, (void *)(off + source), 12);
                (*counter)++;
            } while (*counter < *n);
        }
    } else {
        sub_08071F40(gUnk_081A7374);
    }
}

/* Queue a packet in the tx ring (64 entries) and send an ack packet. */
void sub_08072010(void *p)
{
    u8 *b = (u8 *)&gUnk_030049D0;
    u16 *wr = (u16 *)(b + 0x528);
    int off = *wr * 12;
    u32 addr = (u32)b + 0x52C;
    sub_08075294((u8 *)(addr + off), p, 12);
    *wr = (*wr + 1) & 0x3F;
    sub_08071F40(gUnk_081A7374);
}
/* Dispatch a received link packet; return 1 for a completed data packet. */
int sub_08072054(void)
{
    {
        struct LinkBuf *b = &gUnk_030049D0;
        u8 flags = *(u8 *)((u32)b + 0x524);
        u32 zero = 1;
        zero &= flags;
        if (zero)
            return 0;
        {
            u32 ready = 1;
            ready |= flags;
            *(u8 *)((u32)b + 0x524) = ready;
        }
        b->st526 = sub_08073F04(gUnk_08087600[(REG_SIOCNT & 0x30) >> 4], b->cur, 12);
        if (b->st526 != 0) {
            u16 *last = &b->lastId;
            u16 t = *(u16 *)b->cur >> 8;
            if (*last == (t & 0xF)) {
                sub_08071F40(gUnk_081A7382);
                b->st526 = zero;
                b->busy = 0;
                return 0;
            }
            *last = t & 0xF;
            switch (t & 0xF0) {
            case 0xE0:
                sub_0801A7DC(gUnk_08087604);
                sub_0801A7E8();
                sub_08071F40(b->pad302);
                b->st51E = zero;
                b->busy = 0;
                return 0;
            case 0xD0:
            case 0xF0: {
                    struct LinkBuf *next;
                    sub_08071FA0();
                    next = &gUnk_030049D0;
                    next->st51E = 0;
                    next->busy = 0;
                    return 0;
                }
            case 0xA0:
                sub_08072010(b->cur);
                b->st51E = zero;
                b->busy = 0;
                return 0;
            case 0x90:
                sub_08072010(b->cur);
                b->fl2 = 1;
                b->st51E = zero;
                b->busy = 0;
                return 1;
            case 0xB0:
                sub_08072010(b->cur);
                b->fl2 = 1;
                b->st51E = zero;
                b->busy = 0;
                return 1;
            default:
                sub_08071F40(gUnk_081A7390);
                break;
            }
        } else {
            sub_08071F40(gUnk_081A7374);
        }
    }
    {
        struct LinkBuf *b = &gUnk_030049D0;
        u16 *timer = &b->st51E;
        u32 updated = *timer + 1;
        *timer = updated;
        if ((u16)updated > 0x78) {
            sub_0801A7DC(gUnk_08087614, *timer);
            sub_0801A7E8();
            *timer = 0;
            b->st522 = 1;
        }
        b->busy = 0;
        return 0;
    }
}

/* Search the ring from a valid index 0..63; ignore the sequence nibble. */
int sub_08072238(int idx)
{
    u32 root = (u32)&gUnk_030049D0;
    u32 b;
    u32 firstOff = (u32)idx * 12;
    u32 first = root + 0x52C;
    u32 len = *(u8 *)(firstOff + first) + 5;
    u16 key = (u8)len | 0xB000;
    u32 i = 0;
    /* FAKEMATCH: the wide value retains the per-iteration literal reload. */
    unsigned long long mask = 0xF0FF;
    b = root;
    for (; i < sub_0807F0AC(len, 5) + 3; i++) {
        u32 slot = (idx + i) & 0x3F;
        u32 off = slot * 12;
        /* FAKEMATCH: keep the ring base separate from the packet offset. */
        register u32 ptr asm("r0") = 0x52C;
        ptr += b;
        off += ptr;
        if ((*(u16 *)off & (u32)mask) == key)
            return 1;
    }
    return 0;
}
/* Fetch one queued packet into `dest`: a 0x90 packet is a single 10-byte chunk; 0xA0.. 0xB0 packets
   are multi-part (0xA0 parts are stored at dest + index*2, 0xB0 ends). Returns the length byte or 0. */
int sub_080722B0(u8 *dest)
{
    /* FAKEMATCH: bounded address bindings preserve the original pointer roles. */
    register struct LinkBuf *b asm("r1") = &gUnk_030049D0;
    Pkt *q;
    register u8 *p asm("r5");
    u16 *rd;
    u16 t;
    u16 *wr;
    u32 rdOffset;
    register u32 firstOff asm("r0");
    {
        struct LinkBuf *test = b;
        while (test->busy)
            ;
    }
    {
        u8 *flags = (u8 *)((u32)b + 0x524);
        u32 value = *flags;
        u32 ready = 4;
        ready &= value;
        if (!ready)
            goto empty;
        ready = ~4u;
        ready &= value;
        *flags = ready;
    }
    wr = &b->wr;
    rdOffset = 0x52A;
    rd = (u16 *)((u32)b + rdOffset);
    if (*wr == *rd)
        goto empty;
    firstOff = (u32)*rd * 12;
    rdOffset += 2;
    q = (Pkt *)((u32)b + rdOffset);
    p = (u8 *)(firstOff + (u32)q);
    t = (*(u16 *)p >> 8) & 0xF0;
    switch (t) {
    case 0x90:
        sub_08075294(dest, p + 2, 10);
        *rd = (*rd + 1) & 0x3F;
        return p[0];
    case 0xA0:
        if (!(u16)sub_08072238(*rd))
            goto empty;
        {
            u16 *loopRd = rd;
            Pkt *loopQ = q;
            while (1) {
                u16 h = *(u16 *)p;
                t = (h >> 8) & 0xF0;
                if (t == 0xA0) {
                    sub_08075294(dest + (u8)h * 2, p + 2, 10);
                    *loopRd = (*loopRd + 1) & 0x3F;
                } else if (t == 0xB0) {
                    u8 len = h;
                    sub_08075294(dest, p + 2, 10);
                    *loopRd = (*loopRd + 1) & 0x3F;
                    return len;
                }
                {
                    register u32 off asm("r0") = (u32)*loopRd * 12;
                    u32 addr = (u32)loopQ;
                    p = (u8 *)(off + addr);
                }
            }
        }
    default:
        *rd = (*rd + 1) & 0x3F;
        goto empty;
    }
empty:
    return 0;
}


#if 0 /* NONMATCHING: the target re-reads rxCount and recomputes &rx[rxCount] + (++j)*2 per halfword
       * (stores alias), compares j<n signed but j<=4 unsigned, and pushes r8-sl. This version
       * hoists the entry pointer. */
/* Queue `len` bytes (rounded up to halfwords, n) as 12-byte packets: up to 5 halfwords in one
   0x90 packet, otherwise 0xA0 chunks (sent from the tail, header = remaining count) and a final
   0xB0 packet (header = total count). Returns 1 on success, 0 if the queue is full. */
int sub_080723B4(u16 *src, u32 len)
{
    struct LinkBuf *b = &gUnk_030049D0;
    s16 n = (len + 1) >> 1;
    u16 *cnt = &b->rxCount;
    u16 *e;
    u16 *s2;
    s16 j;
    u16 zero;
    u16 hdr;
    if (*cnt > 0x3F)
        return 0;
    if (n <= 5) {
        e = (u16 *)&b->rx[*cnt];
        e[0] = (u8)n | 0x9000;
        for (j = 0; j <= 4; j++) {
            if (j < n)
                e[j + 1] = *src++;
            else
                e[j + 1] = 0;
        }
        b->rxCount++;
        return 1;
    }
    zero = 0;
    hdr = (u8)n | 0xB000;
    while (1) {
        if (n <= 5) {
            e = (u16 *)&b->rx[*cnt];
            e[0] = hdr;
            s2 = src;
            for (j = 0; j <= 4; j++)
                e[j + 1] = *s2++;
            (*cnt)++;
            return 1;
        }
        n -= 5;
        e = (u16 *)&b->rx[*cnt];
        e[0] = (u8)n | 0xA000;
        s2 = src + n;
        for (j = 0; j <= 4; j++)
            e[j + 1] = *s2++;
        (*cnt)++;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08071F40", sub_080723B4); /* 0x080723B4 size 0x15C */


/* Reset the link buffers, install the link driver and its per-frame hook. */
void sub_08072510(void)
{
    sub_0807373C();
    sub_08075278(&gUnk_030049D0, 0x840);
    sub_080735D4(gUnk_03000000, gUnk_03000000 + 0x1C);
    gUnk_030049D0.lastId = 0xF;
    gUnk_03000040.hook = sub_08072054;
}

/* Shut the link driver down and clear the buffers. */
void sub_0807255C(void)
{
    sub_0807373C();
    gUnk_03000040.hook = 0;
    sub_08075278(&gUnk_030049D0, 0x840);
}

/* Shift-JIS style code (hi, lo) to a linear index (hypothesis). */
u32 sub_08072584(u16 x)
{
    u8 hi = x >> 8;
    u8 lo = x + 0xC0;
    u8 t;
    if (hi <= 0x9F)
        t = hi + 0x80;
    else
        t = hi + 0x40;
    return lo + t * 0xC0;
}

#define B (b & 0xF)
#define C (c & 0xF)
/* Expand a 4-bit glyph row `nib` to four 4bpp pixels: a set bit gives colour b, a clear bit
   gives colour c (bit 3 is the leftmost pixel). */
u16 sub_080725B0(u16 nib, u16 b, u16 c)
{
    switch (nib) {
    case 0: {
        u32 x = c;
        return (x & 0xF) | (x & 0xF) << 4 | ((x & 0xF) | (x & 0xF) << 4) << 8;
    }
    case 1:
        return (C | C << 4) | ((C | B << 4) << 8);
    case 2:
        return (C | C << 4) | ((B | C << 4) << 8);
    case 3:
        return (C | C << 4) | ((B | B << 4) << 8);
    case 4:
        return (C | B << 4) | ((C | C << 4) << 8);
    case 5:
        return (C | B << 4) | ((C | B << 4) << 8);
    case 6:
        return (C | B << 4) | ((B | C << 4) << 8);
    case 7:
        return (C | B << 4) | ((B | B << 4) << 8);
    case 8:
        return (B | C << 4) | ((C | C << 4) << 8);
    case 9:
        return (B | C << 4) | ((C | B << 4) << 8);
    case 10:
        return (B | C << 4) | ((B | C << 4) << 8);
    case 11:
        return (B | C << 4) | ((B | B << 4) << 8);
    case 12:
        return (B | B << 4) | ((C | C << 4) << 8);
    case 13:
        return (B | B << 4) | ((C | B << 4) << 8);
    case 14:
        return (B | B << 4) | ((B | C << 4) << 8);
    case 15: {
        u32 x = b;
        return (x & 0xF) | (x & 0xF) << 4 | ((x & 0xF) | (x & 0xF) << 4) << 8;
    }
    }
}
#undef B
#undef C





void sub_08072778(u8 ch, u16 *out, u16 b, u16 c)
{
    u16 *src = (u16 *)(gUnk_0822BB00 + ch * 8);
    int i;
    for (i = 0; i < 4; i++) {
        *out++ = sub_080725B0((*src >> 4) & 0xF, b, c);
        *out++ = sub_080725B0(*src & 0xF, b, c);
        *out++ = sub_080725B0((*src >> 12) & 0xF, b, c);
        *out++ = sub_080725B0((*src >> 8) & 0xF, b, c);
        src++;
    }
}

/* Same as sub_08072778 for the 2-byte (kanji) glyph table at 0x081C0000. */
void sub_08072808(u16 ch, u16 *out, u16 b, u16 c)
{
    u16 *src = (u16 *)(gUnk_081C0000 + sub_08072584(ch) * 8);
    int i;
    for (i = 0; i < 4; i++) {
        *out++ = sub_080725B0((*src >> 4) & 0xF, b, c);
        *out++ = sub_080725B0(*src & 0xF, b, c);
        *out++ = sub_080725B0((*src >> 12) & 0xF, b, c);
        *out++ = sub_080725B0((*src >> 8) & 0xF, b, c);
        src++;
    }
}

/* Store two u16 values in the main struct at +0x441C / +0x441E. */
void sub_0807289C(u16 a, u16 b)
{
    gUnk_03000040_v.a = a;
    gUnk_03000040_v.b = b;
}

/* Is this Shift-JIS code a character that may not start a line (punctuation, small kana)? */
int sub_080728C0(u16 c)
{
    switch (c) {
    case 0x8141:
    case 0x8142:
    case 0x8145:
    case 0x8148:
    case 0x8149:
    case 0x815B:
    case 0x815D:
    case 0x815E:
    case 0x816A:
    case 0x8176:
    case 0x8178:
    case 0x817A:
    case 0x829F:
    case 0x82A1:
    case 0x82A3:
    case 0x82A5:
    case 0x82A7:
    case 0x82C1:
    case 0x82E1:
    case 0x82E3:
    case 0x82E5:
    case 0x8340:
    case 0x8342:
    case 0x8344:
    case 0x8346:
    case 0x8348:
    case 0x8362:
    case 0x8383:
    case 0x8385:
    case 0x8387:
    case 0x8395:
    case 0x8396:
        return 1;
    default:
        return 0;
    }
}

int sub_080729F8(u16 c)
{
    if (c == 0x8169 || c == 0x8175)
        return 1;
    return 0;
}

#if 0 /* NONMATCHING: register allocation differs (target: s r9, col r6, tile r8, map sl, 0x1F mask
       * hoisted in r7, width pointer spilled to [sp,0xC] across the sub_080728C0 call; built
       * keeps the width pointer in a register and masks inline) */
/* Same for 2-byte (Shift-JIS) text with line-break rules (no line start on sub_080728C0 chars,
   no line end on sub_080729F8 chars). */
void sub_08072A14(u16 col, u16 pk, u16 tile, u8 *s)
{
    u16 lo = pk;
    u8 hi = pk >> 8;
    u16 *map = (u16 *)gUnk_0300045C;
    u16 base = col;
    map += col;
    while (1) {
        struct MainMap *m = &gUnk_03000040_m;
        u8 w = *(u16 *)s;
        u8 b = *s;
        u16 ch;
        if (b == 0)
            break;
        ch = (w >> 8) | (b << 8);
        if (((col & 0x1F) >= (m->w & 0x1F) - 2 && !sub_080728C0_u16(ch))
            || ((col & 0x1F) >= (m->w & 0x1F) - 3 && sub_080729F8_u16(ch))) {
            base += 0x20;
            col = base;
            map = &m->map[base];
        }
        sub_08072808(ch, (u16 *)(0x06004000 + tile * 32), lo, hi);
        *map++ = tile;
        col++;
        tile++;
        s += 2;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08071F40", sub_08072A14); /* 0x08072A14 size 0xE4 */

#if 0 /* NONMATCHING: register allocation differs. The base+offset shape matches (a u64 temp
       * prevents folding), but agbcc keeps `hi` in sl and map/base in r7/r8, while the target
       * spends r6 reloading 0x03000040 and spills lo/hi to the stack, leaving the 0x1F mask in
       * sl and base in r9. */
/* Draw a string of 1-byte chars (converted with sub_08074A90) into the BG map at cell `col`,
   glyph tiles from `tile`; wraps to the next 32-cell line near the right edge (width @ +0x441E). */
void sub_08072AF8(u16 col, u16 pk, u16 tile, u8 *s)
{
    u8 lo = pk;
    u8 hi = pk >> 8;
    u16 *map = (u16 *)gUnk_0300045C;
    s16 base = col;
    unsigned long long mb = 0x03000040; /* FAKEMATCH: 64-bit temp stops agbcc folding the map base */
    map += col;
    while (1) {
        u16 ch;
        if (*s == 0)
            return;
        ch = sub_08074A90(*s);
        if (ch != 0) {
            if ((col & 0x1F) >= (((struct MainMap *)(u32)mb)->w & 0x1F) - 2) {
                base += 0x20;
                col = base;
                map = &((struct MainMap *)(u32)mb)->map[base];
            }
            sub_08072808(ch, (u16 *)(0x06004000 + tile * 32), lo, hi);
            *map++ = tile;
            col++;
            tile++;
        }
        s++;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08071F40", sub_08072AF8); /* 0x08072AF8 size 0xBC */

/* Draw a NUL-terminated ASCII string: glyph tiles to 0x06004000 + tile*32, map entries at (col). */
void sub_08072BB4(u16 col, u16 pk, u16 tile, char *s)
{
    u16 *map = (u16 *)gUnk_0300045C;
    u8 lo = pk;
    u8 hi = pk >> 8;
    map += col;
    while (*s != 0) {
        u8 c = *s;
        u16 t = tile;
        sub_08072778(c, (u16 *)(0x06004000 + t * 32), lo, hi);
        tile = t + 1;
        *map++ = t;
        s++;
    }
}

/* Print `val` as decimal (n digits, padded with zeros if `zero` else blanks). */
void sub_08072C0C(u32 a, u32 b, int val, u16 zero)
{
    char buf[12];
    u16 tile = b;
    u16 n = b >> 16;
    int first = 1;
    if (n <= 8) {
        buf[n] = 0;
        if (val < 0)
            val = -val;
        do {
            n--;
            if (val != 0 || first)
                buf[n] = gUnk_08087634[val % 10];
            else
                buf[n] = zero ? '0' : ' ';
            val /= 10;
            first = 0;
        } while (n != 0);
        sub_08072BB4(a, a >> 16, tile, buf);
    }
}

/* Print `val` as hex (n digits, zero padded). a = col | pk << 16, b = tile | n << 16. */
void sub_08072CAC(u32 a, u32 b, int val)
{
    char buf[12];
    u16 tile = b;
    u16 n = b >> 16;
    if ((u16)(n - 1) <= 7) {
        buf[n] = 0;
        if (val < 0)
            val = -val;
        do {
            n--;
            if (val != 0)
                buf[n] = gUnk_0808765C[val & 0xF];
            else
                buf[n] = '0';
            val >>= 4;
        } while (n != 0);
        sub_08072BB4(a, a >> 16, tile, buf);
    }
}

/* Draw card portrait `c`: fills a 9 x 10 cell block of the BG map buffer at (a & 7) * 0x800 + b * 2 with
 * ascending tiles from d >> 1, copies the card's 64-colour palette to bank pal >> 4, unpacks its 6bpp image
 * (720 x 3 halfwords) to 0x06004000 + d * 32, then adds the palette base (u8)pal to every pixel.
 * Matching notes: GCSE/PRE hoists e>>4, c<<7, d<<5, c<<4 and e<<24 into the entry block, and the order of
 * its expression hash table (size = insn count / 2) decides which spilled value gets which stack slot.
 * `pal` as a u32 copy (no shortened u16 shift for pal >> 4) and the `lim` bound local keep the insn count
 * at the ROM's, so e<<24 lands at sp+0 and d*32 at sp+4. `off` puts d*32 ahead of c*0x10E0 in that order
 * while the pointer add stays after src; the ROM tables are integer addresses so reload rematerializes them. */
void sub_08072D28(u16 a, u16 b, u16 c, u16 d, u16 e)
{
    u32 pal = e;
    u16 *map = (u16 *)(0x0300045C + (a & 7) * 0x800);
    u16 i;
    u16 tile;
    u16 j;
    const u16 *src;
    u16 *dst;
    u32 off;
    u16 m6, m12;
    u32 lim;

    map += b;
    i = 0;
    tile = d >> 1;
    for (; i < 10; i++) {
        for (j = 0; j < 9; j++)
            map[j] = tile++;
        map += 0x20;
    }
    sub_08075294((void *)(0x05000000 + (pal >> 4) * 32), (const void *)(0x08608360 + c * 0x80), 0x80);
    off = d * 32;
    src = (const u16 *)(0x082A6500 + c * 0x10E0);
    dst = (u16 *)(0x06004000 + off);
    i = 0;
    m6 = 0x3F;
    m12 = 0xFC0;
    lim = 0x2CF;
    for (; i <= lim; i++) {
        u16 s0 = src[0];
        u32 s1 = src[1];
        u32 s2 = src[2];
        u16 t, x;
        dst[0] = (s0 & m6) | ((s0 & m12) << 2);
        dst[1] = (s0 >> 12) | ((s1 & 3) << 4) | ((s1 & 0xFC) * 64);
        t = s1 >> 8;
        dst[2] = (t & m6) | (((t >> 6) | ((s2 & 0xF) << 2)) << 8);
        x = s2 >> 4;
        dst[3] = (x & m6) | ((x & m12) << 2);
        src += 3;
        dst += 4;
    }
    dst = (u16 *)(0x06004000 + d * 32);
    for (i = 0; i < 0xB40; i++) {
        *dst = (*dst & 0x3F3F) + ((u8)pal << 8 | (u8)pal);
        dst++;
    }
}


/* Write tile entry `e` at (col b, row-block a) of the BG map buffer at 0x0300045C. */
void sub_08072E98(u16 a, u16 b, u16 e)
{
    u8 *row = gUnk_0300045C + a * 0x800;
    u8 *cell = row + b * 2;
    *(u16 *)cell = e;
}

/* Load an image pack (see wiki data/graphics-formats): palette to 0x05000000 + palIdx*2, 8bpp
   tiles to 0x06004000 + tileBase*32 (palIdx added to non-zero pixel bytes), cells to the map
   buffer at 0x03000C5C. Returns the tile count. */
u16 sub_08072EB0(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u16 *h = img;
    u16 *hdrT = (u16 *)((u8 *)h + 8 + h[0] * 2);
    u16 *tiles = (u16 *)((u8 *)h + 0x10 + h[0] * 2);
    u16 *dst = (u16 *)(0x06004000 + tileBase * 32);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 64);
    u16 *cells = hdrC + 4;
    u16 i;
    u16 v;
    for (i = 0; i < hdrT[0] * 32; i++) {
        u16 w = *tiles;
        v = w;
        /* barrier: keeps w in the loaded register (old_agbcc otherwise merges
           the *tiles load into v and the test/add register roles swap) */
        __asm__ __volatile__("" : : "r"(w));
        if (w & 0xFF00)
            v += palIdx << 8;
        if (v & 0xFF)
            v += palIdx;
        *dst++ = v;
        tiles++;
    }
    sub_08075294((void *)(0x05000000 + palIdx * 2), img + 4, h[0] * 2);
    for (i = 0; i < hdrC[0]; i++) {
        u16 pos = *cells++;
        u16 tile = *cells++;
        u16 idx = (pos & 0x3F) | ((pos & 0xFF00) >> 3);
        idx += mapBase;
        ((u16 *)gUnk_03000C5C)[idx] = tile + tileBase / 2;
    }
    return hdrT[0];
}
