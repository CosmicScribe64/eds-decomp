#include "global.h"
#include "gba.h"

extern void MemCopy16(void *dst, const void *src, u32 n);
extern void MemClear16(void *p, u32 n);
extern void CopyDoubleWords(void *dst, const void *src, u32 n);
extern void LoadSystemGfx(void);
extern void SetTextArea(int a, int b);
extern u16 gMapFillTile[];
extern u8 gUnk_03000C5C[];
extern u8 gUnk_0300045C[];
extern u16 gUnk_0300045C_h[] asm("gUnk_0300045C");
extern u16 gUnk_03000C5C_h[] asm("gUnk_03000C5C");
extern vu32 *gLinkSio[];
extern u8 gBgMaps[8][0x800] asm("gUnk_0300045C");

/* Link-cable SIO state block (0x03005B60, 0xB38 bytes); see wiki code-08072fac. */
struct LinkSio {
    u32 *irqSlotA;              /* +0x000 IRQ vector slot cleared by LinkSioStop */
    u32 *irqSlotB;              /* +0x004 */
    u16 txHdr;                  /* +0x008 send header: type bits | length */
    u8 txData[0xA18 - 0xA];     /* +0x00A send payload (up to 0x1FF bytes?) */
    u8 unkA18[2];
    u8 unkA1A[2];
    u8 padA1C[0xA26 - 0xA1C];
    u16 unkA26;
    u8 unkA28[2];
    u8 padA2A[2];
    s32 state;                  /* +0xA2C */
    u16 *rxBuf;                 /* +0xA30 */
    u16 *rxDone;                /* +0xA34 */
    u16 *unkA38;
    u8 padA3C[4];
    u16 unkA40;
    u8 padA42[0xA54 - 0xA42];
    u16 buf[3][24];             /* +0xA54 three 0x30-byte packet buffers */
    u8 padAE4[0xAF0 - 0xAE4];
    u16 txBlocks[2];            /* +0xAF0 */
    u16 unkAF4[2];
    u16 txBusy[2];              /* +0xAF8 nonzero while a send is pending */
    u8 padAFC[0xB0C - 0xAFC];
    u8 unkB0C;
};
extern struct LinkSio gUnk_03005B60_s asm("gLinkSio");
extern void LinkSerialIntr(void);
extern u16 LinkSioMain(u8 *rx);
extern u16 gLinkSioTxPayload[];
extern void LinkSioSetSendData(void *src);
extern u16 gUnk_03006658[];
extern s32 gUnk_0300665C;
extern u16 gUnk_03006670;
extern u16 gUnk_03006672;
extern u16 gUnk_03006674;
extern u8 gUnk_03006676[];

u16 LoadBgImage(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
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
        ((u16 *)gUnk_0300045C)[idx] = tile + tileBase / 2;
    }
    return hdrT[0];
}

u16 LoadBgImage4bppMap4Rel(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    u8 *dst = (u8 *)(0x06004000 + tileBase * 32);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 32);
    u16 *cells = hdrC + 4;
    u16 i;
    int col0 = 0;
    int row0 = 0;
    MemCopy16(dst, tiles, hdrT[0] * 32);
    MemCopy16((void *)(0x05000000 + palIdx * 2), img + 4, img[0] * 2);
    for (i = 0; i < hdrC[0]; i++) {
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
        idx += mapBase;
        ((u16 *)0x0300245C)[idx] = (tile + tileBase) | (palIdx >> 4) << 12;
    }
    return hdrT[0];
}
/* Image pack: {u16 nColors, ...; u16 pal[n] @ +8; u16 nTiles @ +8+2n; tiles @ +0x10+2n ...}.
   Copies tiles to 0x06004000 + tileBase*32 and the palette to 0x05000000 + palIdx*2; returns the tile count. */
u16 LoadBgImage4bppGfx(u16 palIdx, u16 tileBase, u16 *img)
{
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    MemCopy16((void *)(0x06004000 + tileBase * 32), tiles, hdrT[0] * 32);
    MemCopy16((void *)(0x05000000 + palIdx * 2), img + 4, img[0] * 2);
    return hdrT[0];
}
/* Loads an image pack (tiles + palette via LoadBgImage4bppGfx) and writes its cell list to the map buffer at 0x03000C5C.
   Each cell {pos, tile}: pos & 0x3F = column, pos >> 8 = row (x32), cell = (tile + tileBase) | bank << 12. */
u16 LoadBgImage4bppMap1(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u8 *map;
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 32);
    u16 *cells = hdrC + 4;
    u16 i;
    LoadBgImage4bppGfx(palIdx, tileBase, img);
    for (i = 0; i < hdrC[0]; i++) {
        u16 pos = *cells++;
        u16 tile = *cells++;
        /* FAKEMATCH: the split shift (>>1 then >>2) and re-assigning the map
           base each iteration are codegen nudges (permuter): they make old_agbcc
           hoist the map literal ahead of the palette-bank value and keep it in ip. */
        u16 idx = ((pos & 0x3F) | (((pos & 0xFF00) >> 1) >> 2)) + mapBase;
        map = gUnk_03000C5C;
        ((u16 *)map)[idx] = (tile + tileBase) | (palIdx >> 4) << 12;
    }
    return hdrT[0];
}
u16 LoadBgImage4bpp(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    u8 *dst = (u8 *)(0x06004000 + tileBase * 32);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 32);
    u16 *cells = hdrC + 4;
    u8 *map;
    u16 i;
    MemCopy16(dst, tiles, hdrT[0] * 32);
    MemCopy16((void *)(0x05000000 + palIdx * 2), img + 4, img[0] * 2);
    for (i = 0; i < hdrC[0]; i++) {
        u16 pos = *cells++;
        u16 tile = *cells++;
        u16 idx = (pos & 0x3F) | ((pos & 0xFF00) >> 3);
        idx += mapBase;
        /* FAKEMATCH: routing the map base through a pointer variable makes
           old_agbcc keep the 0xFF00 mask in ip and reload the literal like the target. */
        map = (u8 *)0x0300045C;
        ((u16 *)map)[idx] = (tile + tileBase) | (palIdx >> 4) << 12;
    }
    return hdrT[0];
}
/* FAKEMATCH: the row binding is declared inside the guarded block (permuter); the register pins and the
   empty asm keep row in sl and the mask in ip like the ROM */
u16 LoadBgImage4bppToMap(u32 rowArg, u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    u8 *dst = (u8 *)(0x06004000 + tileBase * 32);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 32);
    u16 *cells = hdrC + 4;
    u16 i;
    register u32 mask __asm__("r12");
    u16 n;

    MemCopy16(dst, tiles, hdrT[0] * 32);
    MemCopy16((void *)(0x05000000 + palIdx * 2), img + 4, img[0] * 2);

    i = 0;
    n = hdrC[0];
    if (i < n) {
        register u32 row __asm__("r10") = rowArg;
        mask = 0xFF00;
        row = (u32)gUnk_0300045C + row * 0x800;
        __asm__ volatile("" : "+r"(row));
        do {
            u16 pos = *cells++;
            u16 tile = *cells++;
            u16 idx = (pos & 0x3F) | ((pos & mask) >> 3);
            idx += mapBase;
            ((u16 *)row)[idx] = (tile + tileBase) | (palIdx >> 4) << 12;
            i++;
        } while (i < hdrC[0]);
    }
    return hdrT[0];
}

u16 LoadBgImage4bppMap1Rel(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u8 *map;
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 32);
    u16 *cells = hdrC + 4;
    u16 first = hdrC[4];
    u16 i;
    LoadBgImage4bppGfx(palIdx, tileBase, img);
    for (i = 0; i < hdrC[0]; i++) {
        u16 pos = *cells++ - first;
        u16 tile = *cells++;
        /* FAKEMATCH: split shift + per-iteration map assignment (see LoadBgImage4bppMap1). */
        u16 idx = ((pos & 0x3F) | (((pos & 0xFF00) >> 1) >> 2)) + mapBase;
        map = gUnk_03000C5C;
        ((u16 *)map)[idx] = (tile + tileBase) | (palIdx >> 4) << 12;
    }
    return hdrT[0];
}
/* Clear the 8 BG map buffers (0x800 each) and the 0xE00-byte buffer at 0x02010014. */
void ClearBgMapBuffers(void)
{
    int i;
    for (i = 0; i < 8; i++)
        MemClear16(gBgMaps[i], 0x800);
    {
        u16 *q = (u16 *)0x02010014;
        MemClear16(q, 0x1C00);
        q -= 2;
        *q = 0;
    }
}
void ClearBgMapBuffer0(void)
{
    u16 *q;
    MemClear16(gUnk_0300045C, 0x800);
    q = (u16 *)0x02010014;
    MemClear16(q, 0x1C00);
    q -= 2;
    *q = 0;
}
/* Fill a w x h block of the map buffer (row stride 0x40 bytes) with one tile (gMapFillTile[0]). */
void FillMapRect(u16 row, u16 col, u16 w, u16 h)
{
    u16 *dst = (u16 *)(gUnk_0300045C + row * 0x800);
    u16 x;
    dst += col;
    for (; h != 0; dst += 0x20, h--) {
        for (x = 0; x < w; x++)
            dst[x] = *gMapFillTile;
    }
}
void CopyBgTileBufferToVram(void)
{
    CopyDoubleWords((void *)0x06004400, (void *)0x02010014, 0x1C00);
}
/* Clear the map buffers, reset the text limits and the BG2/BG3 affine registers. */
void ResetVideo(void)
{
    ClearBgMapBuffers();
    LoadSystemGfx();
    SetTextArea(0, 0x27E);
    *(vu32 *)0x04000028 = 0;
    *(vu32 *)0x0400002C = 0;
    *(vu16 *)0x04000020 = 0x100;
    *(vu16 *)0x04000022 = 0;
    *(vu16 *)0x04000024 = 0;
    *(vu16 *)0x04000026 = 0x100;
    *(vu32 *)0x04000038 = 0;
    *(vu32 *)0x0400003C = 0;
    *(vu16 *)0x04000030 = 0x100;
    *(vu16 *)0x04000032 = 0;
    *(vu16 *)0x04000034 = 0;
    *(vu16 *)0x04000036 = 0x100;
    *(vu16 *)0x04000008 = 4;
}
/* Link install: (re)initialise the SIO state block and hook the serial IRQ (LinkSerialIntr).
   a / b are the two IRQ handler slots (0x03000000 / 0x0300001C). */
void LinkSioInit(u32 *a, u32 *b)
{
    u8 i;
    u32 zero;
    REG_IME = 0;
    REG_IE &= 0xFF3F;
    REG_IME = 1;
    zero = 0;
    CpuSet(&zero, &gUnk_03005B60_s, 0x050002CE);
    gUnk_03005B60_s.rxBuf = gUnk_03005B60_s.buf[0];
    gUnk_03005B60_s.rxDone = gUnk_03005B60_s.buf[1];
    gUnk_03005B60_s.unkA38 = gUnk_03005B60_s.buf[2];
    for (i = 0; i < 2; i++) {
        gUnk_03005B60_s.unkA28[i] |= 0xFF;
        gUnk_03005B60_s.unkA18[i] = 0;
        gUnk_03005B60_s.unkA1A[i] = 0;
    }
    gUnk_03005B60_s.unkA26 = 0;
    REG_RCNT = 0xC000;
    REG_SIOCNT = 0x1000;
    REG_SIOCNT = 0;
    REG_SIOCNT = 3;
    REG_SIOCNT |= 0x2000;
    REG_RCNT = 0;
    gUnk_03005B60_s.state = 0xC;
    gUnk_03005B60_s.unkA40 = 0x1000;
    gUnk_03005B60_s.irqSlotA = a;
    gUnk_03005B60_s.irqSlotB = b;
    REG_IME = 0;
    REG_IE |= 0x80;
    *a = (u32)LinkSerialIntr;
    *b = (u32)LinkSerialIntr;
    REG_SIOCNT |= 0x4000;
    REG_IME = 1;
    if (!(gUnk_03005B60_s.unkB0C & 4)) {
        REG_IME = 0;
        REG_IE |= 0x40;
        REG_IME = 1;
    }
}
void LinkSioStop(void)
{
    *(vu16 *)0x04000208 = 0;
    *(vu16 *)0x04000200 &= 0xFF3F;
    *gLinkSio[1] = 0;
    *gLinkSio[0] = 0;
    *(vu16 *)0x04000208 = 1;
    *(vu16 *)0x04000128 = 0x2000;
    *(vu16 *)0x04000202 = 0xC0;
}
/* Queue `n` bytes (1..0x100) from src for sending on the multi-player link; 0 if the slot is busy. */
/* Queue `n` bytes (1..0x100) from src for sending on the multi-player link; 0 if the slot is busy. */
u32 LinkSioSend(void *src, int n)
{
    u32 id = (REG_SIOCNT & 0x30) >> 4;
    int slot = (u32)(-id | id) >> 31;
    struct LinkSio *link = &gUnk_03005B60_s;
    if (link->txBusy[slot] != 0)
        return 0;
    if ((u32)(n - 1) <= 0xFF) {
        link->txBlocks[slot] = (n + 0x10) / 16;
        link->txBusy[slot] = n + 1;
        link->unkAF4[slot] = 0;
        if (n <= 0xE)
            link->txHdr = n | 0x3000;
        else
            link->txHdr = n | 0x2000;
        CpuSet(src, gLinkSioTxPayload, (n / 2) & 0x1FFFFF);
        LinkSioSetSendData(gLinkSioTxPayload - 1);
    }
    return 1;
}
/* LinkSio (0x03005B60) as seen by the multi-block receive in LinkSioRecvMultiBlock. */
struct Link82C {
    u8 pad0[8];
    u16 txPkt[32][8];           /* +0x008 outgoing 16-byte packets (header + 7 halfwords) */
    u8 pad208[0x20C - 0x208];
    u16 rxBuf[2][2][0x101];     /* +0x20C reassembly buffer; the ROM steps rxBuf[slot] + cur * 7 rows */
    u8 padA14[0xA40 - 0xA14];
    u16 unkA40;                 /* +0xA40 */
    u8 padA42[0xAF0 - 0xA42];
    u16 nblk[2];                /* +0xAF0 blocks in the current message */
    u16 cur[2];                 /* +0xAF4 current block index */
    u16 busy[2];                /* +0xAF8 message length + block count while receiving */
};
#define gLink82C (*(struct Link82C *)&gUnk_03005B60_s)
struct Pkt82C {
    u16 hdr;
    u16 data[7];
};

/* Multi-block link receive: pump LinkSioMain, then for each player slot with data
   reassemble 0x2000 (first) / 0x4000 (middle) / 0x3000 (last) blocks into rxBuf,
   acknowledging our own slot's blocks with the next tx packet (0x5000 = resend).
   When slot `id`'s message is complete, copy it to dst and return its length. */
u32 LinkSioRecvMultiBlock(u32 id, void *dst)
{
    struct Pkt82C rx[2];
    struct Pkt82C tx;
    u16 flags;
    u8 i;
    u8 mask;
    u32 len;
    u8 slot;
    mask = 1;
    slot = 0;
    len = 0;
    flags = LinkSioMain((u8 *)rx);
    if (flags & 0x30) {
        tx.hdr = 0x5000;
        LinkSioSetSendData(&tx);
    } else if (flags & 0xF) {
        for (i = 0; i < 2; i++) {
            if (flags & 0xF & mask) {
                switch (rx[slot].hdr & 0xF000) {
                case 0x1000:
                    break;
                case 0x2000:
                    gLink82C.busy[slot] = rx[slot].hdr & 0x1FF;
                    /* signed division: its sign-fixup branch (removed later by
                       combine) keeps the gLink base from being hoisted first */
                    gLink82C.nblk[slot] = (gLink82C.busy[slot] + 0x10) / 16;
                    gLink82C.busy[slot] += gLink82C.nblk[slot];
                    gLink82C.cur[slot] = 0;
                    /* fall through */
                case 0x4000:
                    CpuSet(rx[slot].data, gLink82C.rxBuf[slot] + gLink82C.cur[slot] * 7, 7);
                    gLink82C.cur[slot]++;
                    if (slot == (REG_SIOCNT & 0x30) >> 4) {
                        if (gLink82C.cur[slot] == 0)
                            tx.hdr = 0x2000 | gLink82C.busy[slot];
                        else if (gLink82C.cur[slot] == gLink82C.nblk[slot] - 1)
                            tx.hdr = 0x3000;
                        else
                            tx.hdr = 0x4000;
                        CpuSet(gLink82C.txPkt[gLink82C.cur[slot]], tx.data, 7);
                        LinkSioSetSendData(&tx);
                    }
                    gLink82C.cur[slot]++;
                    break;
                case 0x3000:
                    if (gLink82C.nblk[slot] == 0)
                        gLink82C.busy[slot] = rx[slot].hdr & 0x1FF;
                    if (slot == (REG_SIOCNT & 0x30) >> 4)
                        gLink82C.unkA40 = 0x1000;
                    CpuSet(rx[slot].data, gLink82C.rxBuf[slot] + gLink82C.cur[slot] * 7, 7);
                    gLink82C.cur[slot]++;
                    break;
                case 0x5000:
                    gLink82C.cur[slot]--;
                    if (slot == (REG_SIOCNT & 0x30) >> 4) {
                        if (gLink82C.cur[slot] == 0)
                            tx.hdr = 0x2000 | gLink82C.busy[slot];
                        else if (gLink82C.cur[slot] == gLink82C.nblk[slot] - 1)
                            tx.hdr = 0x3000;
                        else
                            tx.hdr = 0x4000;
                        CpuSet(gLink82C.txPkt[gLink82C.cur[slot]], tx.data, 7);
                        LinkSioSetSendData(&tx);
                    }
                    break;
                }
                if (gLink82C.cur[slot] * 15 >= gLink82C.busy[slot]) {
                    if (slot == id) {
                        len = gLink82C.busy[slot];
                        CpuSet(gLink82C.rxBuf[slot], dst, len >> 1);
                    }
                    gLink82C.busy[slot] = 0;
                    gLink82C.cur[slot] = 0;
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
#undef gLink82C
/* Link receive: run the link step, and if slot `id` holds a complete packet (type 0x3000) copy it to dst; returns its length. */
u16 LinkSioRecvSingle(u32 id, void *dst)
{
    u8 buf[0x20];
    u16 ret;
    u32 v;
    int f;
    (void)REG_SIOCNT;
    ret = 0;
    v = LinkSioMain(buf) << 16;
    /* FAKEMATCH: mask via a local + volatile deref so old_agbcc loads the
       0x1FF literal before the halfword and keeps the loaded value in r0. */
    f = 0xF0000;
    f = f & v;
    f = (u32)f >> 16;
    if ((f >> id) & 1) {
        u16 *p = (u16 *)(buf + id * 16);
        u16 t = *p & 0xF000;
        if (t == 0x1000) {
        } else if (t == 0x3000) {
            u16 *src = p + 1;
            u16 *q = (u16 *)(0x03005D6C + id * 0x202);
            CpuSet(src, q, 7);
            {
                u16 m = 0x1FF;
                ret = *(vu16 *)q & m;
            }
            CpuSet(q, dst, ret / 2);
        }
        gUnk_03005B60_s.txBusy[id] = 0;
        gUnk_03005B60_s.unkAF4[id] = 0;
        return ret;
    }
    return 0;
}
/* LinkSio as seen by LinkSioRecvDebug (0x03005B60). */
struct LinkSioC10 {
    u8 pad0[0x20C];
    u16 rxBuf[2][2][0x101];     /* +0x20C [half][slot] */
    u8 unkA14[4];               /* +0xA14 [half*2 + slot] packet length byte */
    u8 unkA18[2];
    u8 unkA1A[2];
    u16 unkA1C;
    u8 padA1E[0xA40 - 0xA1E];
    u16 unkA40;
    u8 padA42[0xAF4 - 0xA42];
    u16 unkAF4[2];
    u16 unkAF8[2];
};
#define gLinkC10 (*(struct LinkSioC10 *)&gUnk_03005B60_s)
extern u8 gStrLinkDbgBufferStoredBoth[];
extern u8 gStrLinkDbgBufferOutput[];
extern u8 gStrLinkDbgBufferStored[];
extern void DebugPrintf(const u8 *fmt, ...);
extern void DebugPrintFlush(void);

/* Debug link receive: pump LinkSioMain, then for each slot with a complete
   type-0x3000 packet print it (DebugPrintf), store it in the slot's double
   buffer, and copy the local player's finished packet to dst; returns its length. */
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
    u16 *q;
    u8 *d;
    u16 *s;

    v = LinkSioMain((u8 *)rx) << 16;
    /* same three-step split as LinkSioRecvSingle: in-place AND, lsrs, signed switch */
    f = 0xF0000;
    f = f & v;
    f = (u32)f >> 16;
    switch (f) {
    case 3:
        for (k = 0; k < 2; k++) {
            u16 *p = rx[i];
            t = *p & 0xF000;
            /* a switch (not if/else) so i*2 is computed before the 0x1000 load */
            switch (t) {
            case 0x1000:
                break;
            case 0x3000:
                if (i == (REG_SIOCNT & 0x30) >> 4)
                    gLinkC10.unkA40 = 0x1000;
                DebugPrintf(gStrLinkDbgBufferStoredBoth, i, gLinkC10.unkA1A[i]);
                CpuSet(p + 1,
                       (u8 *)gLinkC10.rxBuf[gLinkC10.unkA1A[i]++][i] + gLinkC10.unkAF4[i] * 14,
                       7);
                gLinkC10.unkAF4[i]++;
                gLinkC10.unkA14[i + gLinkC10.unkA1A[i] * 2] = *p;
                if (i == slot) {
                    if (--gLinkC10.unkA1C != 0xFFFF) {
                        ret = gLinkC10.unkA14[i + gLinkC10.unkA18[i] * 2];
                        DebugPrintf(gStrLinkDbgBufferOutput, i, gLinkC10.unkA18[i], ret);
                        CpuSet(gLinkC10.rxBuf[gLinkC10.unkA18[i]++][i], dst, ret >> 1);
                        gLinkC10.unkA18[i] &= 1;
                    }
                }
                gLinkC10.unkA1A[i] &= 1;
                break;
            }
            gLinkC10.unkAF8[i] = 0;
            gLinkC10.unkAF4[i] = 0;
            i++;
        }
        DebugPrintFlush();
        return ret;
    case 1:
        t = rx[0][0] & 0xF000;
        if (t == 0x1000) {
        } else if (t == 0x3000) {
            DebugPrintf(gStrLinkDbgBufferStored, 0, gLinkC10.unkA1A[0], rx[0][0] & 0x1FF);
            DebugPrintFlush();
            gLinkC10.unkA14[gLinkC10.unkA1A[0] * 2] = rx[0][0];
            /* FAKEMATCH: temporaries keep the target's order (src arg first,
               then rxBuf base + AF4*14) so the tail cross-jumps with case 2 */
            s = &rx[0][1];
            d = (u8 *)gLinkC10.rxBuf[gLinkC10.unkA1A[0]++];
            CpuSet(s, d + gLinkC10.unkAF4[0] * 14, 7);
            gLinkC10.unkA1A[0] &= one;
            gLinkC10.unkAF4[0]++;
            gLinkC10.unkA1C++;
        }
        break;
    case 2:
        q = rx[1]; /* pointer kept in r7 (case 1 reads rx[0][0] directly) */
        t = *q & 0xF000;
        if (t == 0x1000) {
        } else if (t == 0x3000) {
            DebugPrintf(gStrLinkDbgBufferStored, 1, gLinkC10.unkA1A[1], *q & 0x1FF);
            DebugPrintFlush();
            gLinkC10.unkA14[gLinkC10.unkA1A[1] * 2 + 1] = *q;
            CpuSet(q + 1,
                   (u8 *)gLinkC10.rxBuf[gLinkC10.unkA1A[1]++][1] + gLinkC10.unkAF4[1] * 14, 7);
            gLinkC10.unkA1A[1] &= one;
            gLinkC10.unkAF4[1]++;
            gLinkC10.unkA1C++;
        }
        break;
    }
    return 0;
}
#undef gLinkC10
/* LinkSio as seen by LinkSioRecv (0x03005B60). */
struct LinkSioF04 {
    u8 pad0[0x20C];
    u16 rxBuf[2][2][0x101];     /* +0x20C [half][slot] */
    u8 padA14[4];
    u8 unkA18[2];
    u8 unkA1A[2];
    u8 padA1C[0xA26 - 0xA1C];
    u16 unkA26;
    u8 padA28[0xAF4 - 0xA28];
    u16 unkAF4[2];
    u16 unkAF8[2];
    s32 unkAFC;
    u8 padB00[0x10];
    u16 unkB10;
    u16 unkB12;
    u16 unkB14;
    u16 rx[2][8];               /* +0xB16 */
};
#define gLinkF04 (*(struct LinkSioF04 *)&gUnk_03005B60_s)

/* Public link receive: pump LinkSioMain, then scan the two player slots for a
   complete type-0x3000 packet and copy it to dst; returns the byte count. */
u16 LinkSioRecv(int slot, void *dst)
{
    LinkSioMain((u8 *)gLinkF04.rx);
    gLinkF04.unkB10 = 0;
    gLinkF04.unkB12 = 0;
    if ((gLinkF04.unkB14 & 0xF) == 3) {
        for (gLinkF04.unkAFC = 0; gLinkF04.unkAFC < 2; gLinkF04.unkAFC++) {
            /* the header is read twice from memory (CSE merges the loads);
               a u16 local for it swaps the AND operands */
            u16 t = gLinkF04.rx[gLinkF04.unkB10][0] & 0xF000;
            if (t == 0x1000) {
            } else if (t == 0x3000) {
                gLinkF04.unkAF8[gLinkF04.unkB10] = gLinkF04.rx[gLinkF04.unkB10][0] & 0x1FF;
                CpuSet(&gLinkF04.rx[gLinkF04.unkB10][1],
                       (u8 *)gLinkF04.rxBuf[gLinkF04.unkA1A[gLinkF04.unkB10]++][gLinkF04.unkB10]
                           + gLinkF04.unkAF4[gLinkF04.unkB10] * 12,
                       7);
                gLinkF04.unkAF4[gLinkF04.unkB10]++;
                if (gLinkF04.unkB10 == slot) {
                    gLinkF04.unkB12 = gLinkF04.unkAF8[gLinkF04.unkB10];
                    CpuSet(gLinkF04.rxBuf[gLinkF04.unkA18[gLinkF04.unkB10]++][gLinkF04.unkB10], dst,
                           gLinkF04.unkB12 >> 1);
                }
                gLinkF04.unkA1A[gLinkF04.unkB10] &= 1;
                gLinkF04.unkA18[gLinkF04.unkB10] &= 1;
            }
            gLinkF04.unkAF8[gLinkF04.unkB10] = 0;
            gLinkF04.unkAF4[gLinkF04.unkB10] = 0;
            gLinkF04.unkB10++;
        }
        gLinkF04.unkA26 = 0;
        return gLinkF04.unkB12;
    }
    return 0;
}
#undef gLinkF04
