#include "global.h"
#include "gba.h"

extern void sub_08075294(void *dst, const void *src, u32 n);
extern void sub_08075278(void *p, u32 n);
extern void sub_080752B0(void *dst, const void *src, u32 n);
extern void sub_08075630(void);
extern void sub_0807289C(int a, int b);
extern u16 gUnk_081A7760[];
extern u8 gUnk_03000C5C[];
extern u8 gUnk_0300045C[];
extern u16 gUnk_0300045C_h[] asm("gUnk_0300045C");
extern u16 gUnk_03000C5C_h[] asm("gUnk_03000C5C");
extern vu32 *gUnk_03005B60[];
extern u8 gBgMaps[8][0x800] asm("gUnk_0300045C");

/* Link-cable SIO state block (0x03005B60, 0xB38 bytes); see wiki code-08072fac. */
struct LinkSio {
    u32 *irqSlotA;              /* +0x000 IRQ vector slot cleared by sub_0807373C */
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
extern struct LinkSio gUnk_03005B60_s asm("gUnk_03005B60");
extern void sub_08075F74(void);
extern u16 sub_080740BC(u8 *rx);
extern u16 gUnk_03005B6A[];
extern void sub_08074218(void *src);
extern u16 gUnk_03006658[];
extern s32 gUnk_0300665C;
extern u16 gUnk_03006670;
extern u16 gUnk_03006672;
extern u16 gUnk_03006674;
extern u8 gUnk_03006676[];

u16 sub_08072FAC(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
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
        ((u16 *)gUnk_0300045C)[idx] = tile + tileBase / 2;
    }
    return hdrT[0];
}

u16 sub_080730A8(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    u8 *dst = (u8 *)(0x06004000 + tileBase * 32);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 32);
    u16 *cells = hdrC + 4;
    u16 i;
    int col0 = 0;
    int row0 = 0;
    sub_08075294(dst, tiles, hdrT[0] * 32);
    sub_08075294((void *)(0x05000000 + palIdx * 2), img + 4, img[0] * 2);
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
u16 sub_08073184(u16 palIdx, u16 tileBase, u16 *img)
{
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    sub_08075294((void *)(0x06004000 + tileBase * 32), tiles, hdrT[0] * 32);
    sub_08075294((void *)(0x05000000 + palIdx * 2), img + 4, img[0] * 2);
    return hdrT[0];
}
/* Loads an image pack (tiles + palette via sub_08073184) and writes its cell list to the map buffer at 0x03000C5C.
   Each cell {pos, tile}: pos & 0x3F = column, pos >> 8 = row (x32), cell = (tile + tileBase) | bank << 12. */
u16 sub_080731D0(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u8 *map;
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 32);
    u16 *cells = hdrC + 4;
    u16 i;
    sub_08073184(palIdx, tileBase, img);
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
u16 sub_0807326C(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    u8 *dst = (u8 *)(0x06004000 + tileBase * 32);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 32);
    u16 *cells = hdrC + 4;
    u8 *map;
    u16 i;
    sub_08075294(dst, tiles, hdrT[0] * 32);
    sub_08075294((void *)(0x05000000 + palIdx * 2), img + 4, img[0] * 2);
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
u16 sub_0807332C(u32 rowArg, u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    u8 *dst = (u8 *)(0x06004000 + tileBase * 32);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 32);
    u16 *cells = hdrC + 4;
    u16 i;
    register u32 mask __asm__("r12");
    u16 n;

    sub_08075294(dst, tiles, hdrT[0] * 32);
    sub_08075294((void *)(0x05000000 + palIdx * 2), img + 4, img[0] * 2);

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

u16 sub_080733F4(u16 mapBase, u16 palIdx, u16 tileBase, u16 *img)
{
    u8 *map;
    u16 *hdrT = (u16 *)((u8 *)img + 8 + img[0] * 2);
    u16 *tiles = (u16 *)((u8 *)img + 0x10 + img[0] * 2);
    u16 *hdrC = (u16 *)((u8 *)tiles + hdrT[0] * 32);
    u16 *cells = hdrC + 4;
    u16 first = hdrC[4];
    u16 i;
    sub_08073184(palIdx, tileBase, img);
    for (i = 0; i < hdrC[0]; i++) {
        u16 pos = *cells++ - first;
        u16 tile = *cells++;
        /* FAKEMATCH: split shift + per-iteration map assignment (see sub_080731D0). */
        u16 idx = ((pos & 0x3F) | (((pos & 0xFF00) >> 1) >> 2)) + mapBase;
        map = gUnk_03000C5C;
        ((u16 *)map)[idx] = (tile + tileBase) | (palIdx >> 4) << 12;
    }
    return hdrT[0];
}
/* Clear the 8 BG map buffers (0x800 each) and the 0xE00-byte buffer at 0x02010014. */
void sub_08073498(void)
{
    int i;
    for (i = 0; i < 8; i++)
        sub_08075278(gBgMaps[i], 0x800);
    {
        u16 *q = (u16 *)0x02010014;
        sub_08075278(q, 0x1C00);
        q -= 2;
        *q = 0;
    }
}
void sub_080734D4(void)
{
    u16 *q;
    sub_08075278(gUnk_0300045C, 0x800);
    q = (u16 *)0x02010014;
    sub_08075278(q, 0x1C00);
    q -= 2;
    *q = 0;
}
/* Fill a w x h block of the map buffer (row stride 0x40 bytes) with one tile (gUnk_081A7760[0]). */
void sub_08073500(u16 row, u16 col, u16 w, u16 h)
{
    u16 *dst = (u16 *)(gUnk_0300045C + row * 0x800);
    u16 x;
    dst += col;
    for (; h != 0; dst += 0x20, h--) {
        for (x = 0; x < w; x++)
            dst[x] = *gUnk_081A7760;
    }
}
void sub_08073558(void)
{
    sub_080752B0((void *)0x06004400, (void *)0x02010014, 0x1C00);
}
/* Clear the map buffers, reset the text limits and the BG2/BG3 affine registers. */
void sub_08073574(void)
{
    sub_08073498();
    sub_08075630();
    sub_0807289C(0, 0x27E);
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
#if 0 /* NONMATCHING: structure matches; target derives 0xA84/0xAB4 as r2+0x30 from the 0xA54 literal and keeps base in r4/copy r5, the build uses separate literals and a different register assignment (reg+const CSE) */
/* Link install: (re)initialise the SIO state block and hook the serial IRQ (sub_08075F74).
   a / b are the two IRQ handler slots (0x03000000 / 0x0300001C). */
void sub_080735D4(u32 *a, u32 *b)
{
    u8 i;
    u8 zero;
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
    REG_RCNT = 0;
    REG_SIOCNT |= 0x2000;
    gUnk_03005B60_s.state = 0xC;
    gUnk_03005B60_s.unkA40 = 0x1000;
    gUnk_03005B60_s.irqSlotB = b;
    gUnk_03005B60_s.irqSlotA = a;
    REG_IME = 0;
    REG_IE |= 0x80;
    *a = (u32)sub_08075F74 | 1;
    *b = (u32)sub_08075F74 | 1;
    REG_SIOCNT |= 0x4000;
    REG_IME = 1;
    if (!(gUnk_03005B60_s.unkB0C & 4)) {
        REG_IME = 0;
        REG_IE |= 0x40;
        REG_IME = 1;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08072FAC", sub_080735D4); /* 0x080735D4 size 0x168 */
void sub_0807373C(void)
{
    *(vu16 *)0x04000208 = 0;
    *(vu16 *)0x04000200 &= 0xFF3F;
    *gUnk_03005B60[1] = 0;
    *gUnk_03005B60[0] = 0;
    *(vu16 *)0x04000208 = 1;
    *(vu16 *)0x04000128 = 0x2000;
    *(vu16 *)0x04000202 = 0xC0;
}
/* Queue `n` bytes (1..0x100) from src for sending on the multi-player link; 0 if the slot is busy. */
/* Queue `n` bytes (1..0x100) from src for sending on the multi-player link; 0 if the slot is busy. */
u32 sub_08073784(void *src, int n)
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
        CpuSet(src, gUnk_03005B6A, (n / 2) & 0x1FFFFF);
        sub_08074218(gUnk_03005B6A - 1);
    }
    return 1;
}
INCLUDE_ASM("asm/nonmatching/code_08072FAC", sub_0807382C); /* 0x0807382C size 0x338 */
/* Link receive: run the link step, and if slot `id` holds a complete packet (type 0x3000) copy it to dst; returns its length. */
u16 sub_08073B64(u32 id, void *dst)
{
    u8 buf[0x20];
    u16 ret;
    u32 v;
    int f;
    (void)REG_SIOCNT;
    ret = 0;
    v = sub_080740BC(buf) << 16;
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
INCLUDE_ASM("asm/nonmatching/code_08072FAC", sub_08073C10); /* 0x08073C10 size 0x2F4 */
#if 0 /* NONMATCHING: first attempt from m2c; structure/target use base 0x03006676 in r4 and derive link as r4-0xB16 (the build materialises separate literals and spills the state into r8/sl); the loop body and order are not aligned yet */
/* Public link receive: pump sub_080740BC, then scan the two player slots for a
   complete type-0x3000 packet and copy it to dst; returns the byte count. */
u16 sub_08073F04(s32 slot, void *dst)
{
    u8 *link = (u8 *)0x03005B60;
    u16 *idp = &gUnk_03006670;
    u16 *prog = (u16 *)(link + 0xAF4);
    s16 id;
    u16 hdr;
    sub_080740BC(gUnk_03006676);
    *idp = 0;
    gUnk_03006672 = 0;
    if ((gUnk_03006674 & 0xF) != 3)
        return 0;
    gUnk_0300665C = 0;
    do {
        hdr = *(u16 *)((u8 *)gUnk_03006676 + id * 16);
        id = *idp;
        if ((hdr & 0xF000) == 0x1000) {
        } else if ((hdr & 0xF000) == 0x3000) {
            u8 *p;
            s8 n;
            gUnk_03006658[id] = hdr & 0x1FF;
            p = (u8 *)(link + 0xA1A) + id;
            *p = n + 1;
            n = *p;
            CpuSet((u8 *)(link + 0xB18) + id * 16,
                   (u8 *)(link + 0x20C) + id * 0x202 + n * 0x404 + prog[id] * 0xC,
                   7);
            prog[id]++;
            if (id == slot) {
                u8 *q = (u8 *)(link + 0xA18) + id;
                s8 c = *q;
                *q = c + 1;
                gUnk_03006672 = gUnk_03006658[id];
                CpuSet((u8 *)(link + 0x20C) + c * 0x404 + id * 0x202, dst,
                       gUnk_03006672 >> 1);
            }
            ((u8 *)(link + 0xA1A))[id] &= 1;
            ((u8 *)(link + 0xA18))[id] &= 1;
        }
        *(u16 *)(link + 0xAF8 + id * 2) = 0;
        prog[id] = 0;
        (*idp)++;
        gUnk_0300665C++;
    } while (gUnk_0300665C <= 1);
    *(u16 *)(link + 0xA26) = 0;
    return *(u16 *)(link + 0xB12);
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08072FAC", sub_08073F04); /* 0x08073F04 size 0x1B8 */
