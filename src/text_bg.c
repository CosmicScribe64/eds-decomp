#include "global.h"

extern void MemCopy16(void *dst, const void *src, u32 n);
extern void MemClear16(void *p, u32 n);
extern int LinkSioSend(void *p, u32 n);
extern void LinkSioStop(void);
extern void LinkSioInit(void *a, void *b);
extern u8 gLinkPacketAck[];
extern u8 gLinkPacketDupAck[];
extern u8 gLinkPacketResend[];
extern s8 gLinkPartnerSlot[];
extern u8 gStrDebugLinkReceiveRetry[];
extern u8 gStrDebugLinkRecvTimeout[];
extern void DebugPrintf(const u8 *fmt, ...);
extern void DebugPrintFlush(void);
extern u16 LinkSioRecv(s32 id, void *buf, u32 n);
extern void LinkStoreRecvPacket(void *p);
extern void LinkSendNextQueued(void);
extern u16 sub_08072238_u16(int idx) asm("LinkIsRecvMessageComplete");
#define REG_SIOCNT (*(vu16 *)0x04000128)
extern u8 gUnk_03005204[];
extern u32 sub_0807F0AC(u32 a, u32 b);

typedef struct { u8 b[12]; } Pkt;

/* Link packet buffers at 0x030049D0 (size 0x840). */
struct LinkBuf {
    Pkt rx[64];          /* 0x000 received packets, compacted by LinkSendNextQueued */
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
extern struct LinkBuf gLinkBuf;
extern u8 IntrTable[];
struct MainFn {
    u8 pad[0x418];
    void *hook;         /* per-frame link hook (LinkVBlankHook) */
};
extern struct MainFn gMain;
extern int LinkVBlankHook(void);
struct MainVar {
    u8 pad[0x441C];
    u16 a, b;
};
extern struct MainVar gUnk_03000040_v asm("gMain");
extern u8 gUnk_0300045C[];
struct MainMap {
    u8 pad[0x41C];
    u16 map[0x2000];    /* BG map buffer at 0x0300045C */
    u16 a, w;           /* 0x441C, 0x441E */
};
extern struct MainMap gUnk_03000040_m asm("gMain");
struct MainW {
    u8 pad[0x441E];
    u16 w;
};
extern struct MainW gUnk_03000040_w asm("gMain");
extern u16 AsciiToFullwidthSjis(u8 c);
extern u8 gUnk_03000C5C[];
extern u8 gCardArtPalettes[];
extern u16 gCardArtGfx[];
extern u16 sub_080728C0_u16(u16 c) asm("IsLineStartForbidden");
extern u16 sub_080729F8_u16(u16 c) asm("IsLineEndForbidden");
extern u8 gFontLatin8x8Bold[];
extern u8 gFontKanji8x8[];
extern u32 gHexDigitChars[];
extern u32 gDecimalDigitChars[];
struct HW {
    u16 lo, hi;
};
extern u16 ExpandGlyphNibble(u16 nib, u16 b, u16 c);
extern u32 SjisToGlyphIndex(u16 x);
extern void RenderBoldGlyphTile(u8 ch, u16 *dst, u16 b, u16 c);
extern void RenderSjisGlyphTile(u16 ch, u16 *dst, u16 b, u16 c);
extern void DrawBgString(u16 col, u16 pk, u16 tile, char *s);

extern u16 sub_08071F40_u16(void *p) asm("LinkSendPacket");

/* Send a packet: stamp the sequence nibble, transmit, and on success ack it. */
int LinkSendPacket(void *p)
{
    int ok;
    MemCopy16(gUnk_03005204, p, 12);
    *(u16 *)gUnk_03005204 = (*(u16 *)gUnk_03005204 & 0xF0FF) | (*(u16 *)(gUnk_03005204 - 0x31A) << 8);
    if (LinkSioSend(gUnk_03005204, 12)) {
        MemCopy16(gUnk_03005204 - 0x532, gUnk_03005204, 12);
        *(u16 *)(gUnk_03005204 - 0x31A) = (*(u16 *)(gUnk_03005204 - 0x31A) + 1) & 0xF;
        ok = 1;
    } else {
        ok = 0;
    }
    return ok;
}

/* Pop the head of the received queue (after re-sending the ack) and shift the rest down. */
void LinkSendNextQueued(void)
{
    u8 *b = (u8 *)&gLinkBuf;
    u16 *n = (u16 *)(b + 0x300);
    int *ip;
    if (*n != 0 && (u16)LinkSendPacket(b)) {
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
                MemCopy16(dst, (void *)(off + source), 12);
                (*counter)++;
            } while (*counter < *n);
        }
    } else {
        LinkSendPacket(gLinkPacketAck);
    }
}

/* Queue a packet in the tx ring (64 entries) and send an ack packet. */
void LinkStoreRecvPacket(void *p)
{
    u8 *b = (u8 *)&gLinkBuf;
    u16 *wr = (u16 *)(b + 0x528);
    int off = *wr * 12;
    u32 addr = (u32)b + 0x52C;
    MemCopy16((u8 *)(addr + off), p, 12);
    *wr = (*wr + 1) & 0x3F;
    LinkSendPacket(gLinkPacketAck);
}
/* Dispatch a received link packet; return 1 for a completed data packet. */
int LinkVBlankHook(void)
{
    {
        struct LinkBuf *b = &gLinkBuf;
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
        b->st526 = LinkSioRecv(gLinkPartnerSlot[(REG_SIOCNT & 0x30) >> 4], b->cur, 12);
        if (b->st526 != 0) {
            u16 *last = &b->lastId;
            u16 t = *(u16 *)b->cur >> 8;
            if (*last == (t & 0xF)) {
                LinkSendPacket(gLinkPacketDupAck);
                b->st526 = zero;
                b->busy = 0;
                return 0;
            }
            *last = t & 0xF;
            switch (t & 0xF0) {
            case 0xE0:
                DebugPrintf(gStrDebugLinkReceiveRetry);
                DebugPrintFlush();
                LinkSendPacket(b->pad302);
                b->st51E = zero;
                b->busy = 0;
                return 0;
            case 0xD0:
            case 0xF0: {
                    struct LinkBuf *next;
                    LinkSendNextQueued();
                    next = &gLinkBuf;
                    next->st51E = 0;
                    next->busy = 0;
                    return 0;
                }
            case 0xA0:
                LinkStoreRecvPacket(b->cur);
                b->st51E = zero;
                b->busy = 0;
                return 0;
            case 0x90:
                LinkStoreRecvPacket(b->cur);
                b->fl2 = 1;
                b->st51E = zero;
                b->busy = 0;
                return 1;
            case 0xB0:
                LinkStoreRecvPacket(b->cur);
                b->fl2 = 1;
                b->st51E = zero;
                b->busy = 0;
                return 1;
            default:
                LinkSendPacket(gLinkPacketResend);
                break;
            }
        } else {
            LinkSendPacket(gLinkPacketAck);
        }
    }
    {
        struct LinkBuf *b = &gLinkBuf;
        u16 *timer = &b->st51E;
        u32 updated = *timer + 1;
        *timer = updated;
        if ((u16)updated > 0x78) {
            DebugPrintf(gStrDebugLinkRecvTimeout, *timer);
            DebugPrintFlush();
            *timer = 0;
            b->st522 = 1;
        }
        b->busy = 0;
        return 0;
    }
}

/* Search the ring from a valid index 0..63; ignore the sequence nibble. */
int LinkIsRecvMessageComplete(int idx)
{
    u32 root = (u32)&gLinkBuf;
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
int LinkRecvMessage(u8 *dest)
{
    /* FAKEMATCH: bounded address bindings preserve the original pointer roles. */
    register struct LinkBuf *b asm("r1") = &gLinkBuf;
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
        MemCopy16(dest, p + 2, 10);
        *rd = (*rd + 1) & 0x3F;
        return p[0];
    case 0xA0:
        if (!(u16)LinkIsRecvMessageComplete(*rd))
            goto empty;
        {
            u16 *loopRd = rd;
            Pkt *loopQ = q;
            while (1) {
                u16 h = *(u16 *)p;
                t = (h >> 8) & 0xF0;
                if (t == 0xA0) {
                    MemCopy16(dest + (u8)h * 2, p + 2, 10);
                    *loopRd = (*loopRd + 1) & 0x3F;
                } else if (t == 0xB0) {
                    u8 len = h;
                    MemCopy16(dest, p + 2, 10);
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


/* 12-byte link packet viewed as halfwords: h[0] = type << 12 | count, h[1..5] = payload. */
typedef struct { u16 h[6]; } LinkQPkt;
struct LinkQ {
    LinkQPkt rx[64];
    u16 rxCount;         /* 0x300 */
};
/* Halfword view of the link buffer's rx queue (struct LinkBuf at 0x030049D0). */
#define LINKQ (*(struct LinkQ *)&gLinkBuf)
/* Queue `len` bytes (rounded up to halfwords, n) as 12-byte packets: up to 5 halfwords in one
   0x90 packet, otherwise 0xA0 chunks (sent from the tail, header = remaining count) and a final
   0xB0 packet (header = total count). Returns 1 on success, 0 if the queue is full. */
int LinkQueueMessage(u16 *src, u32 len)
{
    u32 n = (len + 1) >> 1;
    u32 j;
    u32 total;
    u32 off;
    if (LINKQ.rxCount < 0x40) {
        off = LINKQ.rxCount * 12;
        if (n <= 5) {
            *(u16 *)((u8 *)LINKQ.rx + off) = (u8)n | 0x9000;
            for (j = 0; j < 5; j++) {
                /* FAKEMATCH: signed j < n, unsigned j < 5 */
                if ((int)j < (int)n)
                    LINKQ.rx[LINKQ.rxCount].h[j + 1] = *src++;
                else
                    LINKQ.rx[LINKQ.rxCount].h[j + 1] = 0;
            }
            LINKQ.rxCount++;
            return 1;
        }
        /* FAKEMATCH: (u8)n kept as n << 24 before the loop; the loop's `>> 24` is hoisted after
           the base/count copies (a u8 local puts both shifts before them). */
        total = n << 24;
        while (1) {
            if (n <= 5) {
                LINKQ.rx[LINKQ.rxCount].h[0] = (total >> 24) | 0xB000;
                for (j = 0; j < 5; j++)
                    LINKQ.rx[LINKQ.rxCount].h[j + 1] = src[j];
                LINKQ.rxCount++;
                return 1;
            }
            n -= 5;
            LINKQ.rx[LINKQ.rxCount].h[0] = (u8)n | 0xA000;
            for (j = 0; j < 5; j++)
                LINKQ.rx[LINKQ.rxCount].h[j + 1] = src[n + j];
            LINKQ.rxCount++;
        }
    }
    return 0;
}
#undef LINKQ


/* Reset the link buffers, install the link driver and its per-frame hook. */
void LinkInit(void)
{
    LinkSioStop();
    MemClear16(&gLinkBuf, 0x840);
    LinkSioInit(IntrTable, IntrTable + 0x1C);
    gLinkBuf.lastId = 0xF;
    gMain.hook = LinkVBlankHook;
}

/* Shut the link driver down and clear the buffers. */
void LinkShutdown(void)
{
    LinkSioStop();
    gMain.hook = 0;
    MemClear16(&gLinkBuf, 0x840);
}

/* Shift-JIS style code (hi, lo) to a linear index (hypothesis). */
u32 SjisToGlyphIndex(u16 x)
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
u16 ExpandGlyphNibble(u16 nib, u16 b, u16 c)
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





void RenderBoldGlyphTile(u8 ch, u16 *out, u16 b, u16 c)
{
    u16 *src = (u16 *)(gFontLatin8x8Bold + ch * 8);
    int i;
    for (i = 0; i < 4; i++) {
        *out++ = ExpandGlyphNibble((*src >> 4) & 0xF, b, c);
        *out++ = ExpandGlyphNibble(*src & 0xF, b, c);
        *out++ = ExpandGlyphNibble((*src >> 12) & 0xF, b, c);
        *out++ = ExpandGlyphNibble((*src >> 8) & 0xF, b, c);
        src++;
    }
}

/* Same as RenderBoldGlyphTile for the 2-byte (kanji) glyph table at 0x081C0000. */
void RenderSjisGlyphTile(u16 ch, u16 *out, u16 b, u16 c)
{
    u16 *src = (u16 *)(gFontKanji8x8 + SjisToGlyphIndex(ch) * 8);
    int i;
    for (i = 0; i < 4; i++) {
        *out++ = ExpandGlyphNibble((*src >> 4) & 0xF, b, c);
        *out++ = ExpandGlyphNibble(*src & 0xF, b, c);
        *out++ = ExpandGlyphNibble((*src >> 12) & 0xF, b, c);
        *out++ = ExpandGlyphNibble((*src >> 8) & 0xF, b, c);
        src++;
    }
}

/* Store two u16 values in the main struct at +0x441C / +0x441E. */
void SetTextArea(u16 a, u16 b)
{
    gUnk_03000040_v.a = a;
    gUnk_03000040_v.b = b;
}

/* Is this Shift-JIS code a character that may not start a line (punctuation, small kana)? */
int IsLineStartForbidden(u16 c)
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

int IsLineEndForbidden(u16 c)
{
    if (c == 0x8169 || c == 0x8175)
        return 1;
    return 0;
}

/* Same for 2-byte (Shift-JIS) text with line-break rules (no line start on IsLineStartForbidden chars,
   no line end on IsLineEndForbidden chars). Each char is read as a halfword and byte-swapped; the
   (u8) casts and the repeated *(u16 *)s reads set col's live length so that ch gets r5. */
void DrawBgSjisString(u16 col, u16 pk, u16 tile, u8 *s)
{
    u16 *map = (u16 *)gUnk_0300045C;
    u8 lo = pk;
    u8 hi = pk >> 8;
    u16 base = col;
    map += col;
    while (1) {
        u16 ch;
        if ((u8)*(u16 *)s == 0)
            return;
        ch = (u8)(*(u16 *)s >> 8) | ((u8)*(u16 *)s << 8);
        if (((col & 0x1F) >= (gUnk_03000040_m.w & 0x1F) - 2 && !sub_080728C0_u16(ch))
            || ((col & 0x1F) >= (gUnk_03000040_m.w & 0x1F) - 3 && sub_080729F8_u16(ch))) {
            base += 0x20;
            col = base;
            map = (u16 *)gUnk_0300045C + base;
        }
        RenderSjisGlyphTile(ch, (u16 *)(0x06004000 + tile * 32), lo, hi);
        *map++ = tile;
        col++;
        s += 2;
        tile++;
    }
}

/* Draw a string of 1-byte chars (converted with AsciiToFullwidthSjis) into the BG map at cell `col`,
   glyph tiles from `tile`; wraps to the next 32-cell line near the right edge (width @ +0x441E). */
void DrawBgFullwidthString(u16 col, u16 pk, u16 tile, u8 *s)
{
    u16 *map = (u16 *)gUnk_0300045C;
    u8 lo = pk;
    u8 hi = pk >> 8;
    u16 base = col;
    map += col;
    while (1) {
        u16 ch;
        if (*s == 0)
            return;
        ch = AsciiToFullwidthSjis(*s);
        if (ch != 0) {
            if ((col & 0x1F) >= (gUnk_03000040_m.w & 0x1F) - 2) {
                base += 0x20;
                col = base;
                map = &gUnk_03000040_m.map[base];
            }
            RenderSjisGlyphTile(ch, (u16 *)(0x06004000 + tile * 32), lo, hi);
            *map++ = tile;
            col++;
            tile++;
        }
        s++;
    }
}

/* Draw a NUL-terminated ASCII string: glyph tiles to 0x06004000 + tile*32, map entries at (col). */
void DrawBgString(u16 col, u16 pk, u16 tile, char *s)
{
    u16 *map = (u16 *)gUnk_0300045C;
    u8 lo = pk;
    u8 hi = pk >> 8;
    map += col;
    while (*s != 0) {
        u8 c = *s;
        u16 t = tile;
        RenderBoldGlyphTile(c, (u16 *)(0x06004000 + t * 32), lo, hi);
        tile = t + 1;
        *map++ = t;
        s++;
    }
}

/* Print `val` as decimal (n digits, padded with zeros if `zero` else blanks). */
void DrawBgDecimal(u32 a, u32 b, int val, u16 zero)
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
                buf[n] = gDecimalDigitChars[val % 10];
            else
                buf[n] = zero ? '0' : ' ';
            val /= 10;
            first = 0;
        } while (n != 0);
        DrawBgString(a, a >> 16, tile, buf);
    }
}

/* Print `val` as hex (n digits, zero padded). a = col | pk << 16, b = tile | n << 16. */
void DrawBgHex(u32 a, u32 b, int val)
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
                buf[n] = gHexDigitChars[val & 0xF];
            else
                buf[n] = '0';
            val >>= 4;
        } while (n != 0);
        DrawBgString(a, a >> 16, tile, buf);
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
void DrawCardPortrait(u16 a, u16 b, u16 c, u16 d, u16 e)
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
    MemCopy16((void *)(0x05000000 + (pal >> 4) * 32), (const void *)(0x08608360 + c * 0x80), 0x80);
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
void SetBgMapEntry(u16 a, u16 b, u16 e)
{
    u8 *row = gUnk_0300045C + a * 0x800;
    u8 *cell = row + b * 2;
    *(u16 *)cell = e;
}

/* Load an image pack (see wiki data/graphics-formats): palette to 0x05000000 + palIdx*2, 8bpp
   tiles to 0x06004000 + tileBase*32 (palIdx added to non-zero pixel bytes), cells to the map
   buffer at 0x03000C5C. Returns the tile count. */
u16 LoadBgImageMap1(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
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
    MemCopy16((void *)(0x05000000 + palIdx * 2), img + 4, h[0] * 2);
    for (i = 0; i < hdrC[0]; i++) {
        u16 pos = *cells++;
        u16 tile = *cells++;
        u16 idx = (pos & 0x3F) | ((pos & 0xFF00) >> 3);
        idx += mapBase;
        ((u16 *)gUnk_03000C5C)[idx] = tile + tileBase / 2;
    }
    return hdrT[0];
}
