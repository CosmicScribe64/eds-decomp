/*
 * bg_image (0x08072FAC-0x080740BC): image-pack loaders, BG map helpers and the multi-player link
 * layer (wiki/functions/bg-image-c.md).
 *
 * The LoadBgImage* functions load an image pack (struct ImagePackCount / ImagePackCell in bg.h): the
 * palette to BG palette RAM, the tiles to charblock 1 (0x06004000) and one map entry per cell into a
 * RAM map buffer (gBgMaps, the rows of gMain.bgMapBuffer). The 8bpp loader LoadBgImage adds the
 * palette base to every non-zero pixel and writes plain tile numbers; the 4bpp loaders fold the
 * palette bank into the map entries instead. The variants differ in the target buffer (0, 1, 4 or the
 * buffer given as an argument) and in whether cell positions are absolute or relative to the first
 * cell. ClearBgMapBuffers, FillMapRect, CopyBgTileBufferToVram and ResetVideo look after the map
 * buffers, the BG tile staging buffer and the text BG registers.
 *
 * The LinkSio* half is the low level of the link cable: LinkSioInit clears gLinkSio (struct LinkSio
 * in link.h) and installs the serial IRQ handler, LinkSioStop removes it again, LinkSioSend queues
 * one message, and the LinkSioRecv* functions pump LinkSioMain (text_canvas.c) and pick a finished
 * message out of the received frames. Only LinkSioRecv is used by the game; LinkSioRecvMultiBlock,
 * LinkSioRecvSingle and LinkSioRecvDebug are unreferenced driver variants.
 */
#include "global.h"
#include "gba.h"                    /* REG_IME, REG_IE, REG_IF, REG_SIOCNT, REG_RCNT, REG_BG0CNT, REG_BG2X..REG_BG3PD, BG_PLTT, VRAM, CpuSet */
#include "bg.h"                     /* struct ImagePackCount, struct ImagePackCell, gBgTileBuffer, the loader and map-buffer prototypes */
#include "debug.h"                  /* DebugPrintf, DebugPrintFlush */
#include "link.h"                   /* struct LinkSio gLinkSio, enum LinkSioPacketType, LinkSioMain, LinkSioSetSendData, LinkSerialIntr */
#include "text.h"                   /* SetTextArea */
#include "util.h"                   /* MemCopy16, MemClear16, CopyDoubleWords */

/* ---- Map buffers ---- */

/* The eight 0x800-byte BG map buffers (gMain.bgMapBuffer at 0x0300045C), under the symbol they have
 * always used. The [8][0x800] row form is a matching choice: gBgMaps[row] derives a buffer's address
 * the way the ROM does (build/readability/issues/bg_image.md). */
extern u8 gBgMaps[8][0x800] asm("gBgMaps");

/* The halfword FillMapRect fills with (0 in practice, so it clears). */
extern u16 gMapFillTile[];                  /* 0x081A7760 */

/* ---- Image-pack loaders ---- */

/* 8bpp pack into map buffer 0: palStart is added to every non-zero pixel byte on the way to VRAM,
 * and the map entries are tile + tileBase / 2 with no palette bits. Returns the tile count. */
u16 LoadBgImage(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack)
{
    const u16 *base = pack;
    u16 *tileCount = (u16 *)((u8 *)base + 8 + base[0] * 2);
    u16 *tiles = (u16 *)((u8 *)base + 0x10 + base[0] * 2);
    u16 *dst = (u16 *)(VRAM + 0x4000 + tileBase * 32);
    u16 *cellCount = (u16 *)((u8 *)tiles + tileCount[0] * 64);
    u16 *cells = cellCount + 4;
    u16 i;
    u16 v;
    for (i = 0; i < tileCount[0] * 32; i++) {
        u16 w = *tiles;
        v = w;
        /* Matching: the empty asm keeps w in the loaded register; without it old_agbcc merges the
           *tiles load into v and the test/add register roles swap. */
        __asm__ __volatile__("" : : "r"(w));
        if (w & 0xFF00)
            v += palStart << 8;
        if (v & 0xFF)
            v += palStart;
        *dst++ = v;
        tiles++;
    }
    MemCopy16((void *)(BG_PLTT + palStart * 2), pack + 4, base[0] * 2);
    for (i = 0; i < cellCount[0]; i++) {
        u16 pos = *cells++;
        u16 tile = *cells++;
        u16 idx = (pos & 0x3F) | ((pos & 0xFF00) >> 3);
        idx += mapOffset;
        ((u16 *)gBgMaps[0])[idx] = tile + tileBase / 2;
    }
    return tileCount[0];
}

/* Unused. 4bpp pack into map buffer 4 with cell positions relative to the first cell (a later cell
 * left of or above the first one wraps). Returns the tile count. */
u16 LoadBgImage4bppMap4Rel(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack)
{
    u16 *tileCount = (u16 *)((u8 *)pack + 8 + pack[0] * 2);
    u16 *tiles = (u16 *)((u8 *)pack + 0x10 + pack[0] * 2);
    u8 *dst = (u8 *)(VRAM + 0x4000 + tileBase * 32);
    u16 *cellCount = (u16 *)((u8 *)tiles + tileCount[0] * 32);
    u16 *cells = cellCount + 4;
    u16 i;
    int col0 = 0;
    int row0 = 0;
    MemCopy16(dst, tiles, tileCount[0] * 32);
    MemCopy16((void *)(BG_PLTT + palStart * 2), pack + 4, pack[0] * 2);
    for (i = 0; i < cellCount[0]; i++) {
        u16 pos = *cells++;
        u16 tile = *cells++;
        int col = pos & 0x3F;
        int row = (pos & 0xFF00) >> 8;
        u16 idx;
        if (i == 0) {
            col0 = col;
            row0 = row;
        }
        {
            u16 dc = col - col0;
            u16 dr = row - row0;
            idx = dc | (dr << 5);
        }
        idx += mapOffset;
        /* = gBgMaps[4]; the integer-literal base is a matching choice (issues note). */
        ((u16 *)0x0300245C)[idx] = (tile + tileBase) | (palStart >> 4) << 12;
    }
    return tileCount[0];
}

/* 4bpp pack, palette and tiles only (no map entries). Returns the tile count. */
u16 LoadBgImage4bppGfx(u16 palStart, u16 tileBase, const u16 *pack)
{
    u16 *tileCount = (u16 *)((u8 *)pack + 8 + pack[0] * 2);
    u16 *tiles = (u16 *)((u8 *)pack + 0x10 + pack[0] * 2);
    MemCopy16((void *)(VRAM + 0x4000 + tileBase * 32), tiles, tileCount[0] * 32);
    MemCopy16((void *)(BG_PLTT + palStart * 2), pack + 4, pack[0] * 2);
    return tileCount[0];
}

/* 4bpp pack into map buffer 1 (tiles and palette via LoadBgImage4bppGfx). */
u16 LoadBgImage4bppMap1(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack)
{
    u8 *map;
    u16 *tileCount = (u16 *)((u8 *)pack + 8 + pack[0] * 2);
    u16 *tiles = (u16 *)((u8 *)pack + 0x10 + pack[0] * 2);
    u16 *cellCount = (u16 *)((u8 *)tiles + tileCount[0] * 32);
    u16 *cells = cellCount + 4;
    u16 i;
    LoadBgImage4bppGfx(palStart, tileBase, pack);
    for (i = 0; i < cellCount[0]; i++) {
        u16 pos = *cells++;
        u16 tile = *cells++;
        /* FAKEMATCH: the split shift (>>1 then >>2) and re-assigning the map
           base each iteration are codegen nudges (permuter): they make old_agbcc
           hoist the map literal ahead of the palette-bank value and keep it in ip. */
        u16 idx = ((pos & 0x3F) | (((pos & 0xFF00) >> 1) >> 2)) + mapOffset;
        map = gBgMaps[1];
        ((u16 *)map)[idx] = (tile + tileBase) | (palStart >> 4) << 12;
    }
    return tileCount[0];
}

/* 4bpp pack into map buffer 0, tiles copied inline. */
u16 LoadBgImage4bpp(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack)
{
    u16 *tileCount = (u16 *)((u8 *)pack + 8 + pack[0] * 2);
    u16 *tiles = (u16 *)((u8 *)pack + 0x10 + pack[0] * 2);
    u8 *dst = (u8 *)(VRAM + 0x4000 + tileBase * 32);
    u16 *cellCount = (u16 *)((u8 *)tiles + tileCount[0] * 32);
    u16 *cells = cellCount + 4;
    u8 *map;
    u16 i;
    MemCopy16(dst, tiles, tileCount[0] * 32);
    MemCopy16((void *)(BG_PLTT + palStart * 2), pack + 4, pack[0] * 2);
    for (i = 0; i < cellCount[0]; i++) {
        u16 pos = *cells++;
        u16 tile = *cells++;
        u16 idx = (pos & 0x3F) | ((pos & 0xFF00) >> 3);
        idx += mapOffset;
        /* FAKEMATCH: routing the map base through a pointer variable makes
           old_agbcc keep the 0xFF00 mask in ip and reload the literal like the target. */
        map = (u8 *)0x0300045C;     /* = gBgMaps[0] */
        ((u16 *)map)[idx] = (tile + tileBase) | (palStart >> 4) << 12;
    }
    return tileCount[0];
}

/* 4bpp pack into map buffer `map` (gBgMaps[map]).
 * FAKEMATCH: the row binding is declared inside the guarded block (permuter); the register pins and the
 * empty asm keep row in sl and the mask in ip like the ROM */
u16 LoadBgImage4bppToMap(u32 map, u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack)
{
    u16 *tileCount = (u16 *)((u8 *)pack + 8 + pack[0] * 2);
    u16 *tiles = (u16 *)((u8 *)pack + 0x10 + pack[0] * 2);
    u8 *dst = (u8 *)(VRAM + 0x4000 + tileBase * 32);
    u16 *cellCount = (u16 *)((u8 *)tiles + tileCount[0] * 32);
    u16 *cells = cellCount + 4;
    u16 i;
    register u32 mask __asm__("r12");
    u16 n;

    MemCopy16(dst, tiles, tileCount[0] * 32);
    MemCopy16((void *)(BG_PLTT + palStart * 2), pack + 4, pack[0] * 2);

    i = 0;
    n = cellCount[0];
    if (i < n) {
        register u32 row __asm__("r10") = map;
        mask = 0xFF00;
        row = (u32)gBgMaps + row * 0x800;
        __asm__ volatile("" : "+r"(row));
        do {
            u16 pos = *cells++;
            u16 tile = *cells++;
            u16 idx = (pos & 0x3F) | ((pos & mask) >> 3);
            idx += mapOffset;
            ((u16 *)row)[idx] = (tile + tileBase) | (palStart >> 4) << 12;
            i++;
        } while (i < cellCount[0]);
    }
    return tileCount[0];
}

/* Unused. 4bpp pack into map buffer 1 with each cell pos taken minus the first cell's pos, so the
 * first cell lands at mapOffset (a cell left of the first one borrows from its row). */
u16 LoadBgImage4bppMap1Rel(u16 mapOffset, u16 palStart, u16 tileBase, const u16 *pack)
{
    u8 *map;
    u16 *tileCount = (u16 *)((u8 *)pack + 8 + pack[0] * 2);
    u16 *tiles = (u16 *)((u8 *)pack + 0x10 + pack[0] * 2);
    u16 *cellCount = (u16 *)((u8 *)tiles + tileCount[0] * 32);
    u16 *cells = cellCount + 4;
    u16 first = cellCount[4];
    u16 i;
    LoadBgImage4bppGfx(palStart, tileBase, pack);
    for (i = 0; i < cellCount[0]; i++) {
        u16 pos = *cells++ - first;
        u16 tile = *cells++;
        /* FAKEMATCH: split shift + per-iteration map assignment (see LoadBgImage4bppMap1). */
        u16 idx = ((pos & 0x3F) | (((pos & 0xFF00) >> 1) >> 2)) + mapOffset;
        map = gBgMaps[1];
        ((u16 *)map)[idx] = (tile + tileBase) | (palStart >> 4) << 12;
    }
    return tileCount[0];
}

/* ---- Map buffer maintenance ---- */

/* Clears all 8 map buffers, gBgTileBuffer and the u16 at 0x02010010 (which nothing else uses). */
void ClearBgMapBuffers(void)
{
    int i;
    for (i = 0; i < 8; i++)
        MemClear16(gBgMaps[i], 0x800);
    {
        u16 *q = (u16 *)gBgTileBuffer;
        MemClear16(q, 0x1C00);
        q -= 2;
        *q = 0;
    }
}

/* Clears map buffer 0, gBgTileBuffer and the u16 at 0x02010010. */
void ClearBgMapBuffer0(void)
{
    u16 *q;
    MemClear16(gBgMaps[0], 0x800);
    q = (u16 *)gBgTileBuffer;
    MemClear16(q, 0x1C00);
    q -= 2;
    *q = 0;
}

/* Fills a width x height block of map buffer `map` from `cell` with gMapFillTile[0] (0, so it clears it). */
void FillMapRect(u16 map, u16 cell, u16 width, u16 height)
{
    u16 *dst = (u16 *)gBgMaps[map];
    u16 x;
    dst += cell;
    for (; height != 0; dst += 0x20, height--) {
        for (x = 0; x < width; x++)
            dst[x] = *gMapFillTile;
    }
}

/* Unused. Uploads gBgTileBuffer to charblock 1, tiles 0x20-0xFF (the buffer is only ever zero). */
void CopyBgTileBufferToVram(void)
{
    CopyDoubleWords((void *)(VRAM + 0x4400), gBgTileBuffer, 0x1C00);
}

/* Video reset: clears the map buffers, loads the system font, sets the text area to cells 0..0x27E,
 * BG2/BG3 reference points 0 and identity matrices, BG0CNT = charblock 1, screenblock 0. */
void ResetVideo(void)
{
    ClearBgMapBuffers();
    LoadSystemGfx();
    SetTextArea(0, 0x27E);
    REG_BG2X = 0;
    REG_BG2Y = 0;
    REG_BG2PA = 0x100;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 0x100;
    REG_BG3X = 0;
    REG_BG3Y = 0;
    REG_BG3PA = 0x100;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 0x100;
    REG_BG0CNT = 4;
}

/* ---- Multi-player SIO driver (gLinkSio) ---- */

/* Link install: clear gLinkSio, set up the frame buffers and install LinkSerialIntr in both
 * interrupt slots. Unless the SIOCNT snapshot (sioCnt) says this GBA is a child, the Timer3 IRQ is
 * enabled as well; the parent clocks the transfers from it. */
void LinkSioInit(u32 *serialIntrSlot, u32 *timer3IntrSlot)
{
    u8 i;
    u32 zero;
    REG_IME = 0;
    REG_IE &= 0xFF3F;               /* serial + Timer3 IRQs off */
    REG_IME = 1;
    zero = 0;
    CpuSet(&zero, &gLinkSio, 0x050002CE);   /* fill the whole 0xB38-byte block with 0 */
    gLinkSio.rxBuf = gLinkSio.rxFrames[0];
    gLinkSio.rxDone = gLinkSio.rxFrames[1];
    gLinkSio.rxWork = gLinkSio.rxFrames[2];
    for (i = 0; i < 2; i++) {
        gLinkSio.unkA28[i] |= 0xFF;
        gLinkSio.rxReadBuf[i] = 0;
        gLinkSio.rxWriteBuf[i] = 0;
    }
    gLinkSio.unkA26 = 0;
    REG_RCNT = 0xC000;
    REG_SIOCNT = 0x1000;
    REG_SIOCNT = 0;
    REG_SIOCNT = 3;
    REG_SIOCNT |= 0x2000;
    REG_RCNT = 0;
    gLinkSio.state = 0xC;                   /* idle */
    gLinkSio.txBuf[2] = LINKSIO_PKT_IDLE;   /* +0xA40: the idle packet header */
    gLinkSio.serialIntrSlot = serialIntrSlot;
    gLinkSio.timer3IntrSlot = timer3IntrSlot;
    REG_IME = 0;
    REG_IE |= 0x80;                 /* serial IRQ on */
    *serialIntrSlot = (u32)LinkSerialIntr;
    *timer3IntrSlot = (u32)LinkSerialIntr;
    REG_SIOCNT |= 0x4000;           /* SIO IRQ enable */
    REG_IME = 1;
    /* Matching: sioCnt is the u32 SIOCNT snapshot in link.h, but the ROM tests its SI bit with a
       byte load, so read the low byte (issues note). */
    if (!(*(u8 *)&gLinkSio.sioCnt & 4)) {
        REG_IME = 0;
        REG_IE |= 0x40;             /* Timer3 IRQ on */
        REG_IME = 1;
    }
}

/* Disable the serial and Timer3 IRQs, clear both interrupt slots and reset SIOCNT. */
void LinkSioStop(void)
{
    REG_IME = 0;
    REG_IE &= 0xFF3F;
    *gLinkSio.timer3IntrSlot = 0;
    *gLinkSio.serialIntrSlot = 0;
    REG_IME = 1;
    REG_SIOCNT = 0x2000;
    REG_IF = 0xC0;
}

/* Queue `size` bytes (1..0x100) from src for sending on the multi-player link; returns 0 while the
 * previous message of our slot is still in flight. */
u32 LinkSioSend(void *src, int size)
{
    u32 id = (REG_SIOCNT & 0x30) >> 4;      /* this GBA's multi-player id */
    /* Matching: the neg/orr/lsr form makes slot 0/1 without a branch; id != 0 compiles to one. */
    int slot = (u32)(-id | id) >> 31;
    struct LinkSio *link = &gLinkSio;
    if (link->msgPending[slot] != 0)
        return 0;
    if ((u32)(size - 1) <= 0xFF) {
        link->blockCount[slot] = (size + 0x10) / 16;
        link->msgPending[slot] = size + 1;
        link->blockIndex[slot] = 0;
        if (size <= 0xE)
            link->txMsg[0] = size | LINKSIO_PKT_LAST;
        else
            link->txMsg[0] = size | LINKSIO_PKT_FIRST;
        CpuSet(src, gLinkSioTxPayload, (size / 2) & 0x1FFFFF);
        LinkSioSetSendData(gLinkSioTxPayload - 1);
    }
    return 1;
}

/* One 16-byte SIO frame packet as LinkSioMain hands it out: header + 7 payload halfwords. */
struct LinkSioFramePacket {
    u16 hdr;                    /* enum LinkSioPacketType | length (bits 0-8) */
    u16 data[7];
};

/* Unused receive variant: pump LinkSioMain, then for each player slot with data reassemble
 * FIRST / MIDDLE / LAST blocks into that slot's rxMsg area, acknowledging our own slot's blocks
 * with the next tx packet (RESEND asks for a block again). When slot `id`'s message is complete,
 * copy it to dst and return its length. */
u32 LinkSioRecvMultiBlock(u32 id, void *dst)
{
    struct LinkSioFramePacket rx[2];
    struct LinkSioFramePacket tx;
    u16 flags;
    u8 i;
    u8 mask;
    u32 len;
    u8 slot;
    mask = 1;
    slot = 0;
    len = 0;
    flags = LinkSioMain((u8 *)rx);
    if (flags & (LINK_SIO_CHECKSUM_ERROR_0 | LINK_SIO_CHECKSUM_ERROR_1)) {
        tx.hdr = LINKSIO_PKT_RESEND;
        LinkSioSetSendData(&tx);
    } else if (flags & 0xF) {
        for (i = 0; i < 2; i++) {
            if (flags & 0xF & mask) {
                switch (rx[slot].hdr & 0xF000) {
                case LINKSIO_PKT_IDLE:
                    break;
                case LINKSIO_PKT_FIRST:
                    gLinkSio.msgPending[slot] = rx[slot].hdr & 0x1FF;
                    /* Matching: signed division; its sign-fixup branch (removed later by
                       combine) keeps the gLinkSio base from being hoisted first. */
                    gLinkSio.blockCount[slot] = (gLinkSio.msgPending[slot] + 0x10) / 16;
                    gLinkSio.msgPending[slot] += gLinkSio.blockCount[slot];
                    gLinkSio.blockIndex[slot] = 0;
                    /* fall through */
                case LINKSIO_PKT_MIDDLE:
                    CpuSet(rx[slot].data, gLinkSio.rxMsg[slot] + gLinkSio.blockIndex[slot] * 7, 7);
                    gLinkSio.blockIndex[slot]++;
                    if (slot == (REG_SIOCNT & 0x30) >> 4) {
                        if (gLinkSio.blockIndex[slot] == 0)
                            tx.hdr = LINKSIO_PKT_FIRST | gLinkSio.msgPending[slot];
                        else if (gLinkSio.blockIndex[slot] == gLinkSio.blockCount[slot] - 1)
                            tx.hdr = LINKSIO_PKT_LAST;
                        else
                            tx.hdr = LINKSIO_PKT_MIDDLE;
                        CpuSet((gLinkSio.txMsg + gLinkSio.blockIndex[slot] * 8), tx.data, 7);
                        LinkSioSetSendData(&tx);
                    }
                    gLinkSio.blockIndex[slot]++;
                    break;
                case LINKSIO_PKT_LAST:
                    if (gLinkSio.blockCount[slot] == 0)
                        gLinkSio.msgPending[slot] = rx[slot].hdr & 0x1FF;
                    if (slot == (REG_SIOCNT & 0x30) >> 4)
                        gLinkSio.txBuf[2] = LINKSIO_PKT_IDLE;
                    CpuSet(rx[slot].data, gLinkSio.rxMsg[slot] + gLinkSio.blockIndex[slot] * 7, 7);
                    gLinkSio.blockIndex[slot]++;
                    break;
                case LINKSIO_PKT_RESEND:
                    gLinkSio.blockIndex[slot]--;
                    if (slot == (REG_SIOCNT & 0x30) >> 4) {
                        if (gLinkSio.blockIndex[slot] == 0)
                            tx.hdr = LINKSIO_PKT_FIRST | gLinkSio.msgPending[slot];
                        else if (gLinkSio.blockIndex[slot] == gLinkSio.blockCount[slot] - 1)
                            tx.hdr = LINKSIO_PKT_LAST;
                        else
                            tx.hdr = LINKSIO_PKT_MIDDLE;
                        CpuSet((gLinkSio.txMsg + gLinkSio.blockIndex[slot] * 8), tx.data, 7);
                        LinkSioSetSendData(&tx);
                    }
                    break;
                }
                if (gLinkSio.blockIndex[slot] * 15 >= gLinkSio.msgPending[slot]) {
                    if (slot == id) {
                        len = gLinkSio.msgPending[slot];
                        CpuSet(gLinkSio.rxMsg[slot], dst, len >> 1);
                    }
                    gLinkSio.msgPending[slot] = 0;
                    gLinkSio.blockIndex[slot] = 0;
                }
            }
            mask <<= 1;
            slot++;
        }
    }
    if (len != 0)
        return len;
    return 0;
}

/* Unused receive variant for single-block messages: run LinkSioMain, and if slot `id` holds a
 * complete LINKSIO_PKT_LAST packet copy it to dst; returns its length. */
u16 LinkSioRecvSingle(u32 id, void *dst)
{
    u8 pktBuf[0x20];
    u16 ret;
    u32 v;
    int f;
    (void)REG_SIOCNT;
    ret = 0;
    v = LinkSioMain(pktBuf) << 16;
    /* FAKEMATCH: mask via a local + volatile deref so old_agbcc loads the
       0x1FF literal before the halfword and keeps the loaded value in r0. */
    f = 0xF0000;
    f = f & v;
    f = (u32)f >> 16;
    if ((f >> id) & 1) {
        u16 *pkt = (u16 *)(pktBuf + id * 16);
        u16 t = *pkt & 0xF000;
        if (t == LINKSIO_PKT_IDLE) {
        } else if (t == LINKSIO_PKT_LAST) {
            u16 *src = pkt + 1;
            u16 *q = gLinkSio.rxMsg[0][id];     /* the slot's staging row */
            CpuSet(src, q, 7);
            {
                u16 m = 0x1FF;
                ret = *(vu16 *)q & m;
            }
            CpuSet(q, dst, ret / 2);
        }
        gLinkSio.msgPending[id] = 0;
        gLinkSio.blockIndex[id] = 0;
        return ret;
    }
    return 0;
}

/* Debug-format strings of the stripped debug print (kept local to this unit, see debug.h). */
extern u8 gStrLinkDbgBufferStoredBoth[];    /* 0x080876B4 */
extern u8 gStrLinkDbgBufferOutput[];        /* 0x080876D4 */
extern u8 gStrLinkDbgBufferStored[];        /* 0x080876F8 */

/* gLinkSio.rxMsgLen (link.h: u8[2][2], [buffer][slot]) as one flat u8[4] at +0xA14, indexed [buffer * 2 + slot].
 * Matching: LinkSioRecvDebug only matches when the table is reached through a cast of &gLinkSio to this
 * view; gLinkSio.rxMsgLen, flat-cast to u8 *, folds the index differently and the function comes out 4 bytes
 * shorter. Every other field of the function is the canonical member. */
struct LinkSioRxLenView {
    u8 pad0[0xA14];
    u8 rxMsgLen[4];                 /* +0xA14: stored header per [buffer * 2 + slot] (low byte used) */
};
#define gLinkRxMsgLen (((struct LinkSioRxLenView *)&gLinkSio)->rxMsgLen)

/* Unused receive variant of the debug build: pump LinkSioMain, then for each slot with a complete
 * LINKSIO_PKT_LAST packet print it (DebugPrintf), store it in the slot's double buffer, and copy
 * the local player's finished packet to dst; returns its length. */
u32 LinkSioRecvDebug(int slot, void *dst)
{
    u16 rx[3][8];
    u8 k; /* declared before ret: k gets the lower spill slot (sp+0x38) */
    u8 one = 1; /* FAKEMATCH: the case 1/2 `&= 1` uses r8 holding this 1 */
    u8 i = 0;
    u32 ret = 0;
    u16 t;
    u32 v;
    int f;
    u16 *pkt;
    u8 *row;
    u16 *payload;

    v = LinkSioMain((u8 *)rx) << 16;
    /* same three-step split as LinkSioRecvSingle: in-place AND, lsrs, signed switch */
    f = 0xF0000;
    f = f & v;
    f = (u32)f >> 16;
    switch (f) {
    case LINKSIO_RX_OK_0 | LINKSIO_RX_OK_1:
        for (k = 0; k < 2; k++) {
            u16 *p = rx[i];
            t = *p & 0xF000;
            /* a switch (not if/else) so i*2 is computed before the 0x1000 load */
            switch (t) {
            case LINKSIO_PKT_IDLE:
                break;
            case LINKSIO_PKT_LAST:
                if (i == (REG_SIOCNT & 0x30) >> 4)
                    gLinkSio.txBuf[2] = LINKSIO_PKT_IDLE;
                DebugPrintf(gStrLinkDbgBufferStoredBoth, i, gLinkSio.rxWriteBuf[i]);
                CpuSet(p + 1,
                       (u8 *)gLinkSio.rxMsg[gLinkSio.rxWriteBuf[i]++][i] + gLinkSio.blockIndex[i] * 14,
                       7);
                gLinkSio.blockIndex[i]++;
                gLinkRxMsgLen[i + gLinkSio.rxWriteBuf[i] * 2] = *p;
                if (i == slot) {
                    if (--gLinkSio.rxPending != 0xFFFF) {
                        ret = gLinkRxMsgLen[i + gLinkSio.rxReadBuf[i] * 2];
                        DebugPrintf(gStrLinkDbgBufferOutput, i, gLinkSio.rxReadBuf[i], ret);
                        CpuSet(gLinkSio.rxMsg[gLinkSio.rxReadBuf[i]++][i], dst, ret >> 1);
                        gLinkSio.rxReadBuf[i] &= 1;
                    }
                }
                gLinkSio.rxWriteBuf[i] &= 1;
                break;
            }
            gLinkSio.msgPending[i] = 0;
            gLinkSio.blockIndex[i] = 0;
            i++;
        }
        DebugPrintFlush();
        return ret;
    case LINKSIO_RX_OK_0:
        t = rx[0][0] & 0xF000;
        if (t == LINKSIO_PKT_IDLE) {
        } else if (t == LINKSIO_PKT_LAST) {
            DebugPrintf(gStrLinkDbgBufferStored, 0, gLinkSio.rxWriteBuf[0], rx[0][0] & 0x1FF);
            DebugPrintFlush();
            gLinkRxMsgLen[gLinkSio.rxWriteBuf[0] * 2] = rx[0][0];
            /* FAKEMATCH: temporaries keep the target's order (src arg first,
               then rxMsg base + blockIndex*14) so the tail cross-jumps with case 2 */
            payload = &rx[0][1];
            row = (u8 *)gLinkSio.rxMsg[gLinkSio.rxWriteBuf[0]++];
            CpuSet(payload, row + gLinkSio.blockIndex[0] * 14, 7);
            gLinkSio.rxWriteBuf[0] &= one;
            gLinkSio.blockIndex[0]++;
            gLinkSio.rxPending++;
        }
        break;
    case LINKSIO_RX_OK_1:
        pkt = rx[1]; /* pointer kept in r7 (case 1 reads rx[0][0] directly) */
        t = *pkt & 0xF000;
        if (t == LINKSIO_PKT_IDLE) {
        } else if (t == LINKSIO_PKT_LAST) {
            DebugPrintf(gStrLinkDbgBufferStored, 1, gLinkSio.rxWriteBuf[1], *pkt & 0x1FF);
            DebugPrintFlush();
            gLinkRxMsgLen[gLinkSio.rxWriteBuf[1] * 2 + 1] = *pkt;
            CpuSet(pkt + 1,
                   (u8 *)gLinkSio.rxMsg[gLinkSio.rxWriteBuf[1]++][1] + gLinkSio.blockIndex[1] * 14, 7);
            gLinkSio.rxWriteBuf[1] &= one;
            gLinkSio.blockIndex[1]++;
            gLinkSio.rxPending++;
        }
        break;
    }
    return 0;
}
#undef gLinkRxMsgLen

/* Public link receive: pump LinkSioMain, then scan the two player slots for a complete
 * LINKSIO_PKT_LAST packet and copy it to dst; returns the byte count. (msgPending holds the
 * received length as a scratch while scanning.) */
u16 LinkSioRecv(int slot, void *dst)
{
    LinkSioMain((u8 *)gLinkSio.rxPackets);
    gLinkSio.tmp0 = 0;
    gLinkSio.tmp1 = 0;
    if ((gLinkSio.stepResult & 0xF) == (LINKSIO_RX_OK_0 | LINKSIO_RX_OK_1)) {
        for (gLinkSio.i = 0; gLinkSio.i < 2; gLinkSio.i++) {
            /* Matching: the header is read twice from memory (CSE merges the loads);
               a u16 local for it swaps the AND operands. */
            u16 t = gLinkSio.rxPackets[gLinkSio.tmp0][0] & 0xF000;
            if (t == LINKSIO_PKT_IDLE) {
            } else if (t == LINKSIO_PKT_LAST) {
                gLinkSio.msgPending[gLinkSio.tmp0] = gLinkSio.rxPackets[gLinkSio.tmp0][0] & 0x1FF;
                CpuSet(&gLinkSio.rxPackets[gLinkSio.tmp0][1],
                       (u8 *)gLinkSio.rxMsg[gLinkSio.rxWriteBuf[gLinkSio.tmp0]++][gLinkSio.tmp0]
                           + gLinkSio.blockIndex[gLinkSio.tmp0] * 12,
                       7);
                gLinkSio.blockIndex[gLinkSio.tmp0]++;
                if (gLinkSio.tmp0 == slot) {
                    gLinkSio.tmp1 = gLinkSio.msgPending[gLinkSio.tmp0];
                    CpuSet(gLinkSio.rxMsg[gLinkSio.rxReadBuf[gLinkSio.tmp0]++][gLinkSio.tmp0], dst,
                           gLinkSio.tmp1 >> 1);
                }
                gLinkSio.rxWriteBuf[gLinkSio.tmp0] &= 1;
                gLinkSio.rxReadBuf[gLinkSio.tmp0] &= 1;
            }
            gLinkSio.msgPending[gLinkSio.tmp0] = 0;
            gLinkSio.blockIndex[gLinkSio.tmp0] = 0;
            gLinkSio.tmp0++;
        }
        gLinkSio.unkA26 = 0;
        return gLinkSio.tmp1;
    }
    return 0;
}
