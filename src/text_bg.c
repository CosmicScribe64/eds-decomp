/*
 * text_bg (0x08071F40-0x0807304C): the BASICSIO link packet layer, BG text drawing and the
 * 8bpp image-pack loader (wiki/functions/text-bg-c.md).
 *
 * The first half is the packet layer over the multi-player SIO driver (struct LinkBuf in
 * link.h): LinkSendPacket stamps the sequence number into a 12-byte packet and hands it to
 * LinkSioSend; LinkVBlankHook, installed as the per-frame hook by LinkInit, receives one
 * packet per frame and answers it by type nibble (single/part/last data, ack, dup-ack,
 * resend); LinkQueueMessage splits an outgoing message into 5-halfword packets in the send
 * queue and LinkRecvMessage reassembles received messages out of the 64-entry ring.
 *
 * The second half draws text onto the BG maps in gMain.bgMapBuffer: ExpandGlyphNibble
 * expands the 1bpp fonts to 4bpp tiles (RenderBoldGlyphTile for 1-byte text,
 * RenderSjisGlyphTile for Shift-JIS), DrawBgString, DrawBgSjisString and
 * DrawBgFullwidthString write the map entries (the two wrapping variants honour the text
 * area set by SetTextArea and the Japanese line-break rules), DrawBgDecimal and DrawBgHex
 * print numbers through DrawBgString, DrawCardPortrait unpacks a card portrait into
 * charblock 1, and LoadBgImageMap1 loads an 8bpp image pack into map buffer 1.
 */
#include "global.h"
#include "bg.h"        /* LoadBgImageMap1, SetBgMapEntry, DrawCardPortrait */
#include "debug.h"     /* DebugPrintf, DebugPrintFlush */
#include "legacy/gba.h"       /* REG_SIOCNT */
#include "link.h"      /* struct LinkBuf gLinkBuf, enum LinkPacketType, the Link* packet layer */
#include "legacy/main.h"      /* struct Main gMain (vblankCallbackEarly) */
#include "text.h"      /* SetTextArea, the DrawBg* printers, the glyph tile renderers */
#include "util.h"      /* MemCopy16, MemClear16 */

/* Link packet templates in ROM, answered by LinkVBlankHook. */
extern u8 gLinkPacketAck[];                 /* 0x081A7374 */
extern u8 gLinkPacketDupAck[];              /* 0x081A7382 */
extern u8 gLinkPacketResend[];              /* 0x081A7390 */
/* The link partner's SIO slot and the receive hook's debug strings. */
extern s8 gLinkPartnerSlot[];               /* 0x08087600 */
extern u8 gStrDebugLinkReceiveRetry[];      /* 0x08087604 */
extern u8 gStrDebugLinkRecvTimeout[];       /* 0x08087614 */
extern u8 gUnk_03005204[];                  /* 0x03005204 = &gLinkBuf.sendBuf; LinkSendPacket reaches sendSeq and lastSentPacket at negative offsets from it (matching choice, see link.h) */
extern u32 sub_0807F0AC(u32 a, u32 b);      /* 0x0807F0AC: the game's unsigned divide (the linked __udivsi3) */

/* ---- Local views kept for matching ---- */

/* Matching: LinkSioRecv is called with a third argument (the byte count) that the link.h
 * prototype does not take; the count lands in a dead register. */
extern u16 LinkSioRecv3(s32 id, void *buf, u32 n) asm("LinkSioRecv");

extern u8 IntrTable[];                      /* 0x03000000 */

/* gMain views of the text area (SetTextArea) and of the BG map buffers with the text area
 * end: the draw functions recompute the map pointer from these symbols on a wrap. */
struct MainTextAreaView {
    u8 pad[0x441C];
    u16 textAreaStart;                      /* +0x441C */
    u16 textAreaEnd;                        /* +0x441E */
};
extern struct MainTextAreaView gMainTextArea asm("gMain");
/* The eight 0x800-byte BG map buffers (gMain.bgMapBuffer at 0x0300045C), under the symbol
 * they have always used (same form as bg_image.c). */
extern u8 gBgMaps[8][0x800] asm("gUnk_0300045C");
struct MainBgMapView {
    u8 pad[0x41C];
    u16 bgMapBuffer[0x2000];                /* flat view of gMain.bgMapBuffer at 0x0300045C */
    u16 textAreaStart;                      /* +0x441C */
    u16 textAreaEnd;                        /* +0x441E */
};
extern struct MainBgMapView gMainBgMap asm("gMain");
/* Matching: the draw functions test the narrowed (u16) results of the line-break predicates. */
extern u16 IsLineStartForbiddenU16(u16 c) asm("IsLineStartForbidden");
extern u16 IsLineEndForbiddenU16(u16 c) asm("IsLineEndForbidden");
extern u32 gHexDigitChars[];               /* 0x0808765C */
extern u32 gDecimalDigitChars[];           /* 0x08087634 */

/* Send a packet: stamp the sequence nibble, transmit, and on success ack it. */
int LinkSendPacket(void *pkt)
{
    int ok;
    MemCopy16(gUnk_03005204, pkt, 12);
    /* gUnk_03005204 - 0x31A is gLinkBuf.sendSeq, - 0x532 is gLinkBuf.lastSentPacket. */
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

/* Send the head of the send queue and remove it (shifting the rest down), or send an
   ACK packet when the queue is empty. The byte-pointer form keeps the field offsets as
   the ROM computes them (sendCount +0x300, queueIndex +0x830). */
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

/* Store a received data packet in the receive ring (recvWrite +0x528, recvRing +0x52C)
   and answer with an ACK packet. */
void LinkStoreRecvPacket(void *pkt)
{
    u8 *b = (u8 *)&gLinkBuf;
    u16 *wr = (u16 *)(b + 0x528);
    int off = *wr * 12;
    u32 addr = (u32)b + 0x52C;
    MemCopy16((u8 *)(addr + off), pkt, 12);
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
        b->recvSize = LinkSioRecv3(gLinkPartnerSlot[(REG_SIOCNT & 0x30) >> 4], b->recvPacket, 12);
        if (b->recvSize != 0) {
            u16 *last = &b->lastRecvSeq;
            u16 t = *(u16 *)b->recvPacket >> 8;
            if (*last == (t & 0xF)) {
                LinkSendPacket(gLinkPacketDupAck);
                b->recvSize = zero;
                b->busy = 0;
                return 0;
            }
            *last = t & 0xF;
            switch (t & 0xF0) {
            case LINK_PKT_RESEND << 4:
                DebugPrintf(gStrDebugLinkReceiveRetry);
                DebugPrintFlush();
                LinkSendPacket(b->lastSentPacket);
                b->timeoutFrames = zero;
                b->busy = 0;
                return 0;
            case LINK_PKT_DUP_ACK << 4:
            case LINK_PKT_ACK << 4: {
                    struct LinkBuf *next;
                    LinkSendNextQueued();
                    next = &gLinkBuf;
                    next->timeoutFrames = 0;
                    next->busy = 0;
                    return 0;
                }
            case LINK_PKT_PART << 4:
                LinkStoreRecvPacket(b->recvPacket);
                b->timeoutFrames = zero;
                b->busy = 0;
                return 0;
            case LINK_PKT_SINGLE << 4:
                LinkStoreRecvPacket(b->recvPacket);
                b->msgReady = 1;
                b->timeoutFrames = zero;
                b->busy = 0;
                return 1;
            case LINK_PKT_LAST << 4:
                LinkStoreRecvPacket(b->recvPacket);
                b->msgReady = 1;
                b->timeoutFrames = zero;
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
        u16 *timer = &b->timeoutFrames;
        u32 updated = *timer + 1;
        *timer = updated;
        if ((u16)updated > 0x78) {
            DebugPrintf(gStrDebugLinkRecvTimeout, *timer);
            DebugPrintFlush();
            *timer = 0;
            b->timedOut = 1;
        }
        b->busy = 0;
        return 0;
    }
}

/* Search the ring from a valid index 0..63; ignore the sequence nibble. */
int LinkIsRecvMessageComplete(int ringIndex)
{
    u32 root = (u32)&gLinkBuf;
    u32 b;
    u32 firstOff = (u32)ringIndex * 12;
    u32 first = root + 0x52C;
    u32 len = *(u8 *)(firstOff + first) + 5;
    u16 key = (u8)len | (LINK_PKT_LAST << 12);
    u32 i = 0;
    /* FAKEMATCH: the wide value retains the per-iteration literal reload. */
    unsigned long long mask = 0xF0FF;
    b = root;
    for (; i < sub_0807F0AC(len, 5) + 3; i++) {
        u32 slot = (ringIndex + i) & 0x3F;
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
    struct LinkPacket *q;
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
    wr = &b->recvWrite;
    rdOffset = 0x52A;
    rd = (u16 *)((u32)b + rdOffset);
    if (*wr == *rd)
        goto empty;
    firstOff = (u32)*rd * 12;
    rdOffset += 2;
    q = (struct LinkPacket *)((u32)b + rdOffset);
    p = (u8 *)(firstOff + (u32)q);
    t = (*(u16 *)p >> 8) & 0xF0;
    switch (t) {
    case LINK_PKT_SINGLE << 4:
        MemCopy16(dest, p + 2, 10);
        *rd = (*rd + 1) & 0x3F;
        return p[0];
    case LINK_PKT_PART << 4:
        if (!(u16)LinkIsRecvMessageComplete(*rd))
            goto empty;
        {
            u16 *loopRd = rd;
            struct LinkPacket *loopQ = q;
            while (1) {
                u16 h = *(u16 *)p;
                t = (h >> 8) & 0xF0;
                if (t == (LINK_PKT_PART << 4)) {
                    MemCopy16(dest + (u8)h * 2, p + 2, 10);
                    *loopRd = (*loopRd + 1) & 0x3F;
                } else if (t == (LINK_PKT_LAST << 4)) {
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
typedef struct { u16 h[6]; } LinkPacketWords;
struct LinkQueueView {
    LinkPacketWords sendQueue[64];
    u16 sendCount;         /* +0x300 */
};
/* Halfword view of the send queue (struct LinkBuf at 0x030049D0), kept for matching. */
#define LINKQ (*(struct LinkQueueView *)&gLinkBuf)
/* Queue `size` bytes (rounded up to halfwords, n) as 12-byte packets: up to 5 halfwords in
   one LINK_PKT_SINGLE packet, otherwise LINK_PKT_PART chunks (sent from the tail, header =
   remaining count) and a final LINK_PKT_LAST packet (header = total count). Returns 1 on
   success, 0 if the queue is full. */
int LinkQueueMessage(u16 *src, u32 size)
{
    u32 n = (size + 1) >> 1;
    u32 j;
    u32 total;
    u32 off;
    if (LINKQ.sendCount < 0x40) {
        off = LINKQ.sendCount * 12;
        if (n <= 5) {
            *(u16 *)((u8 *)LINKQ.sendQueue + off) = (u8)n | (LINK_PKT_SINGLE << 12);
            for (j = 0; j < 5; j++) {
                /* FAKEMATCH: signed j < n, unsigned j < 5 */
                if ((int)j < (int)n)
                    LINKQ.sendQueue[LINKQ.sendCount].h[j + 1] = *src++;
                else
                    LINKQ.sendQueue[LINKQ.sendCount].h[j + 1] = 0;
            }
            LINKQ.sendCount++;
            return 1;
        }
        /* FAKEMATCH: (u8)n kept as n << 24 before the loop; the loop's `>> 24` is hoisted after
           the base/count copies (a u8 local puts both shifts before them). */
        total = n << 24;
        while (1) {
            if (n <= 5) {
                LINKQ.sendQueue[LINKQ.sendCount].h[0] = (total >> 24) | (LINK_PKT_LAST << 12);
                for (j = 0; j < 5; j++)
                    LINKQ.sendQueue[LINKQ.sendCount].h[j + 1] = src[j];
                LINKQ.sendCount++;
                return 1;
            }
            n -= 5;
            LINKQ.sendQueue[LINKQ.sendCount].h[0] = (u8)n | (LINK_PKT_PART << 12);
            for (j = 0; j < 5; j++)
                LINKQ.sendQueue[LINKQ.sendCount].h[j + 1] = src[n + j];
            LINKQ.sendCount++;
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
    gLinkBuf.lastRecvSeq = 0xF;
    gMain.vblankCallbackEarly = LinkVBlankHook;
}

/* Shut the link driver down and clear the buffers. */
void LinkShutdown(void)
{
    LinkSioStop();
    gMain.vblankCallbackEarly = NULL;
    MemClear16(&gLinkBuf, 0x840);
}

/* Shift-JIS style code (hi, lo) to a linear index (hypothesis). */
u32 SjisToGlyphIndex(u16 sjis)
{
    u8 hi = sjis >> 8;
    u8 lo = sjis + 0xC0;
    u8 t;
    if (hi <= 0x9F)
        t = hi + 0x80;
    else
        t = hi + 0x40;
    return lo + t * 0xC0;
}

#define B (fg & 0xF)
#define C (bg & 0xF)
/* Expand a 4-bit glyph row `bits` to four 4bpp pixels: a set bit gives colour fg, a clear
   bit gives colour bg (bit 3 is the leftmost pixel). */
u16 ExpandGlyphNibble(u16 bits, u16 fg, u16 bg)
{
    switch (bits) {
    case 0: {
        u32 x = bg;
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
        u32 x = fg;
        return (x & 0xF) | (x & 0xF) << 4 | ((x & 0xF) | (x & 0xF) << 4) << 8;
    }
    }
}
#undef B
#undef C





void RenderBoldGlyphTile(u8 ch, u16 *dst, u16 fg, u16 bg)
{
    u16 *src = (u16 *)(gFontLatin8x8Bold + ch * 8);
    int i;
    for (i = 0; i < 4; i++) {
        *dst++ = ExpandGlyphNibble((*src >> 4) & 0xF, fg, bg);
        *dst++ = ExpandGlyphNibble(*src & 0xF, fg, bg);
        *dst++ = ExpandGlyphNibble((*src >> 12) & 0xF, fg, bg);
        *dst++ = ExpandGlyphNibble((*src >> 8) & 0xF, fg, bg);
        src++;
    }
}

/* Same as RenderBoldGlyphTile for the 2-byte (kanji) glyph table at 0x081C0000. */
void RenderSjisGlyphTile(u16 sjis, u16 *dst, u16 fg, u16 bg)
{
    u16 *src = (u16 *)(gFontKanji8x8 + SjisToGlyphIndex(sjis) * 8);
    int i;
    for (i = 0; i < 4; i++) {
        *dst++ = ExpandGlyphNibble((*src >> 4) & 0xF, fg, bg);
        *dst++ = ExpandGlyphNibble(*src & 0xF, fg, bg);
        *dst++ = ExpandGlyphNibble((*src >> 12) & 0xF, fg, bg);
        *dst++ = ExpandGlyphNibble((*src >> 8) & 0xF, fg, bg);
        src++;
    }
}

/* Store the text area start/end cells in the main struct at +0x441C / +0x441E. */
void SetTextArea(u16 startCell, u16 endCell)
{
    gMainTextArea.textAreaStart = startCell;
    gMainTextArea.textAreaEnd = endCell;
}

/* Is this Shift-JIS code a character that may not start a line (punctuation, small kana)? */
int IsLineStartForbidden(u16 sjis)
{
    switch (sjis) {
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

int IsLineEndForbidden(u16 sjis)
{
    if (sjis == 0x8169 || sjis == 0x8175)
        return 1;
    return 0;
}

/* Same for 2-byte (Shift-JIS) text with line-break rules (no line start on IsLineStartForbidden chars,
   no line end on IsLineEndForbidden chars). Each char is read as a halfword and byte-swapped; the
   (u8) casts and the repeated *(u16 *)str reads set cell's live length so that ch gets r5. */
void DrawBgSjisString(u16 cell, u16 colors, u16 tile, u8 *str)
{
    u16 *map = (u16 *)gBgMaps;
    u8 fg = colors;
    u8 bg = colors >> 8;
    u16 base = cell;
    map += cell;
    while (1) {
        u16 ch;
        if ((u8)*(u16 *)str == 0)
            return;
        ch = (u8)(*(u16 *)str >> 8) | ((u8)*(u16 *)str << 8);
        if (((cell & 0x1F) >= (gMainBgMap.textAreaEnd & 0x1F) - 2 && !IsLineStartForbiddenU16(ch))
            || ((cell & 0x1F) >= (gMainBgMap.textAreaEnd & 0x1F) - 3 && IsLineEndForbiddenU16(ch))) {
            base += 0x20;
            cell = base;
            map = (u16 *)gBgMaps + base;
        }
        RenderSjisGlyphTile(ch, (u16 *)(0x06004000 + tile * 32), fg, bg);
        *map++ = tile;
        cell++;
        str += 2;
        tile++;
    }
}

/* Draw a string of 1-byte chars (converted with AsciiToFullwidthSjis) into the BG map at
   cell `cell`, glyph tiles from `tile`; wraps to the next 32-cell line near the right edge
   (text area end @ +0x441E). */
void DrawBgFullwidthString(u16 cell, u16 colors, u16 tile, u8 *str)
{
    u16 *map = (u16 *)gBgMaps;
    u8 fg = colors;
    u8 bg = colors >> 8;
    u16 base = cell;
    map += cell;
    while (1) {
        u16 ch;
        if (*str == 0)
            return;
        ch = AsciiToFullwidthSjis(*str);
        if (ch != 0) {
            if ((cell & 0x1F) >= (gMainBgMap.textAreaEnd & 0x1F) - 2) {
                base += 0x20;
                cell = base;
                map = &gMainBgMap.bgMapBuffer[base];
            }
            RenderSjisGlyphTile(ch, (u16 *)(0x06004000 + tile * 32), fg, bg);
            *map++ = tile;
            cell++;
            tile++;
        }
        str++;
    }
}

/* Draw a NUL-terminated ASCII string: glyph tiles to 0x06004000 + tile*32, map entries at (cell). */
void DrawBgString(u16 cell, u16 colors, u16 tile, char *str)
{
    u16 *map = (u16 *)gBgMaps;
    u8 fg = colors;
    u8 bg = colors >> 8;
    map += cell;
    while (*str != 0) {
        u8 c = *str;
        u16 t = tile;
        RenderBoldGlyphTile(c, (u16 *)(0x06004000 + t * 32), fg, bg);
        tile = t + 1;
        *map++ = t;
        str++;
    }
}

/* Print `value` as decimal (n digits, padded with zeros if `zeroPad` else blanks). */
void DrawBgDecimal(u32 cellColors, u32 tileDigits, int value, u16 zeroPad)
{
    char buf[12];
    u16 tile = tileDigits;
    u16 n = tileDigits >> 16;
    int first = 1;
    if (n <= 8) {
        buf[n] = 0;
        if (value < 0)
            value = -value;
        do {
            n--;
            if (value != 0 || first)
                buf[n] = gDecimalDigitChars[value % 10];
            else
                buf[n] = zeroPad ? '0' : ' ';
            value /= 10;
            first = 0;
        } while (n != 0);
        DrawBgString(cellColors, cellColors >> 16, tile, buf);
    }
}

/* Print `value` as hex (n digits, zero padded). cellColors = cell | colors << 16,
   tileDigits = tile | n << 16. */
void DrawBgHex(u32 cellColors, u32 tileDigits, int value)
{
    char buf[12];
    u16 tile = tileDigits;
    u16 n = tileDigits >> 16;
    if ((u16)(n - 1) <= 7) {
        buf[n] = 0;
        if (value < 0)
            value = -value;
        do {
            n--;
            if (value != 0)
                buf[n] = gHexDigitChars[value & 0xF];
            else
                buf[n] = '0';
            value >>= 4;
        } while (n != 0);
        DrawBgString(cellColors, cellColors >> 16, tile, buf);
    }
}

/* Draw card portrait `cardId`: fills a 9 x 10 cell block of the BG map buffer at
 * (screenBlock & 7) * 0x800 + cell * 2 with ascending tiles from tileBase >> 1, copies the
 * card's 64-colour palette to bank pal >> 4, unpacks its 6bpp image (720 x 3 halfwords) to
 * 0x06004000 + tileBase * 32, then adds the palette base (u8)pal to every pixel.
 * Matching notes: GCSE/PRE hoists palBase>>4, cardId<<7, tileBase<<5, cardId<<4 and
 * palBase<<24 into the entry block, and the order of its expression hash table (size = insn
 * count / 2) decides which spilled value gets which stack slot. `pal` as a u32 copy (no
 * shortened u16 shift for pal >> 4) and the `lim` bound local keep the insn count at the
 * ROM's, so palBase<<24 lands at sp+0 and tileBase*32 at sp+4. `off` puts tileBase*32 ahead
 * of cardId*0x10E0 in that order while the pointer add stays after src; the ROM tables are
 * integer addresses so reload rematerializes them. */
void DrawCardPortrait(u16 screenBlock, u16 cell, u16 cardId, u16 tileBase, u16 palBase)
{
    u32 pal = palBase;
    u16 *map = (u16 *)(0x0300045C + (screenBlock & 7) * 0x800);
    u16 i;
    u16 tile;
    u16 j;
    const u16 *src;
    u16 *dst;
    u32 off;
    u16 m6, m12;
    u32 lim;

    map += cell;
    i = 0;
    tile = tileBase >> 1;
    for (; i < 10; i++) {
        for (j = 0; j < 9; j++)
            map[j] = tile++;
        map += 0x20;
    }
    MemCopy16((void *)(0x05000000 + (pal >> 4) * 32), (const void *)(0x08608360 + cardId * 0x80), 0x80);
    off = tileBase * 32;
    src = (const u16 *)(0x082A6500 + cardId * 0x10E0);
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
    dst = (u16 *)(0x06004000 + tileBase * 32);
    for (i = 0; i < 0xB40; i++) {
        *dst = (*dst & 0x3F3F) + ((u8)pal << 8 | (u8)pal);
        dst++;
    }
}


/* Write tile entry `entry` at (cell, screenBlock) of the BG map buffer at 0x0300045C. */
void SetBgMapEntry(u16 screenBlock, u16 cell, u16 entry)
{
    u8 *row = gBgMaps[screenBlock];
    u8 *cellPtr = row + cell * 2;
    *(u16 *)cellPtr = entry;
}

/* Load an image pack (see wiki data/graphics-formats): palette to 0x05000000 + palStart*2,
   8bpp tiles to 0x06004000 + tileBase*32 (palStart added to non-zero pixel bytes), cells to
   map buffer 1 (gBgMaps[1] at 0x03000C5C). Returns the tile count. */
u16 LoadBgImageMap1(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack)
{
    const u16 *h = pack;
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
            v += palStart << 8;
        if (v & 0xFF)
            v += palStart;
        *dst++ = v;
        tiles++;
    }
    MemCopy16((void *)(0x05000000 + palStart * 2), pack + 4, h[0] * 2);
    for (i = 0; i < hdrC[0]; i++) {
        u16 pos = *cells++;
        u16 tile = *cells++;
        u16 idx = (pos & 0x3F) | ((pos & 0xFF00) >> 3);
        idx += mapOffset;
        ((u16 *)gBgMaps[1])[idx] = tile + tileBase / 2;
    }
    return hdrT[0];
}
