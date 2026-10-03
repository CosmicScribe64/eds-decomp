#include "global.h"


/* Sprite list (see code_0807960C): 20 layer heads + 128 linked OAM entries. */
struct OamEntry {
    u32 w;
    u16 h;
    u16 pad;
    s8 next;
    u8 pad2[3];
};

struct OamList {
    s8 head[0x14];
    struct OamEntry e[128];
    u8 count : 7;
    u8 flag : 1;
};

#define REG_SIOCNT (*(vu16 *)0x04000128)

extern void LinkSioInit(void *a, void *b);
extern void LinkSioStop(void);
extern void MemClear16(void *p, u32 size);
extern void AddSprite(u32 a, u32 b, u32 c);
extern u8 gPassword[];
extern u8 IntrTable[];
extern struct OamEntry *OamListAlloc(u8 layer, struct OamList *l);
extern int sub_0807B4D0_i(int a, int b) asm("MulFix8");
extern s16 gSineTable[];
extern void OamListAddSpriteGroup(u8 *a, u32 b, u32 c, int x, u16 y, u32 f, u8 g, u8 h, u8 i, u32 j, u32 k, u32 l);

/* Linear motion / tween record (0x16 bytes). */
struct Tween {
    u16 x, y;
    u16 x0, y0;
    u16 x1, y1;
    s16 step;
    s16 step2;
    s16 dx, dy;
    u8 kind;
    u8 mode;
};
extern void LoadBgImageMap1(u32 a, u32 b, u32 c, void *src);
extern void DrawCardPortraitOrClear(u16 a, u16 b, u16 id, u16 c);
extern u8 gCardFrameTrapGfx[];
extern u8 gCardFrameMagicGfx[];
extern u8 gCardFrameTicketGfx[];
extern u8 gCardFrameEffectGfx[];
extern u8 gCardFrameFusionGfx[];
extern u8 gCardFrameRitualGfx[];
extern u8 gCardFrameNormalGfx[];
extern void ResetVideo(void);
extern void ClearBgMapBuffers(void);
extern void LoadBgImage4bppMap1(u32 a, u32 b, u32 c, void *src);
extern void MemCopy16(u32 dst, void *src, u32 n);
extern void SetBrightnessBlack(void);
extern u8 gPasswordKeypadBgGfx[];
extern u8 gPasswordPanelBgGfx[];
extern u8 gPasswordObjPal[];
extern u8 gPasswordObjGfx[];
struct MainBig {
    u8 pad[0x485E];
    u16 counter;
};
extern struct MainBig gMain;
struct MainB {
    u8 pad[0x40E];
    u16 field;
};
extern struct MainB gUnk_03000040_b asm("gMain");
extern u16 gPasswordSlotCursorFrames[];

struct MenuEntry {
    u8 x, y;
    u16 pad;
    u16 flags : 12;
    u16 pad2;
};
extern struct MenuEntry gPasswordKeypad[];
extern u8 gCardPasswords[][4];
extern int FadeFromBlack(int);

/* Link-menu state at 0x0201F7B0 (size 0x18). */
struct LinkState {
    u8 pad[0x10];
    u8 cur;
    u8 a : 6;
    u32 b : 6;
    u16 c : 12;
    u16 d;
};
extern struct LinkState gUnk_0201F7B0_s asm("gPassword");

/* Two-player link handshake state (a struct of two 4-byte entries + state + timeout). */
struct LinkEntry {
    u8 id;
    u8 phase;
    u16 data;
};

struct LinkSync {
    struct LinkEntry tx;
    struct LinkEntry rx;
    u8 state;
    u8 pad;
    u16 timeout;
};

extern u16 LinkSyncOpen(void *p_, void *unused);
extern u32 LinkSyncClose(void *a, void *b);
extern u16 sub_0807BED8_x(void) asm("SioGetMultiPlayerId");
extern int LinkSioSend(void *p, int n);
extern int LinkSioRecv(int id, void *p, int n);


/* Allocates an OAM entry on `layer` and fills attr0-2: y/x, shape/size from the w x h pixel size,
 * tile, palette bank, priority; mode 8 = 256 colours. Returns the entry. */
/* Volatile view of the entry: the ROM reloads attr0/1 in every case instead of reusing the stored value. */
struct OamEntryV {
    vu32 w;
    u16 h;
};
struct OamEntry *OamListAddSprite(u8 layer, u16 tile, u16 x, int y_, u8 w, u8 h, u8 mode, u8 pal, u32 unused,
                               u16 flags, u8 aff, u8 prio, struct OamList *list)
{
    u16 y;
    struct OamEntryV *e;
    u32 v;

    x &= 0x1FF; /* in place: the longer live range gives x r6 and y r4 */
    y = y_;     /* int param narrowed here, not in the prologue */
    y &= 0xFF;
    e = (struct OamEntryV *)OamListAlloc(layer, list);
    v = aff << 25;

    if (mode == 8) {
        v |= 0x2000;
        v |= flags;
    } else {
        v |= flags;
    }
    e->w = v;
    switch (w) {
    case 8:
        switch (h) {
        case 8:
            e->w |= x << 16 | y;
            break;
        case 16:
            e->w |= 0x8000 | (x << 16 | y);
            break;
        case 32:
            e->w |= 0x40008000 | (x << 16 | y);
            break;
        case 64:
            while (1)
                ;
        }
        break;
    case 16:
        switch (h) {
        case 8:
            e->w |= 0x4000 | (x << 16 | y);
            break;
        case 16:
            e->w |= 0x40000000 | (x << 16 | y);
            break;
        case 32:
            e->w |= 0x80008000 | (x << 16 | y);
            break;
        case 64:
            while (1)
                ;
        }
        break;
    case 32:
        switch (h) {
        case 8:
            e->w |= 0x40004000 | (x << 16 | y);
            break;
        case 16:
            e->w |= 0x80004000 | (x << 16 | y);
            break;
        case 32:
            e->w |= 0x80000000 | (x << 16 | y);
            break;
        case 64:
            e->w |= 0xC0008000 | (x << 16 | y);
            break;
        }
        break;
    case 64:
        switch (h) {
        case 8:
            while (1)
                ;
        case 16:
            while (1)
                ;
        case 32:
            e->w |= 0xC0004000 | (x << 16 | y);
            break;
        case 64:
            e->w |= 0xC0000000 | (x << 16 | y);
            break;
        }
        break;
    }
    e->h = pal << 12 | tile | prio << 10;
    return (struct OamEntry *)e;
}
/* Draws a decimal number with sprites (digit sprite table `base`, 8 bytes per digit), right to left. */
void DrawNumberSprites(u16 num, u8 count, u8 mode, u16 x, u16 y, u8 *base, u32 unused, u8 step, u8 h, u8 i, u8 g, u32 l)
{
    u8 n;
    u8 k = 0;
    u16 d;

    switch (mode) {
    case 0:
        for (n = 0; n < count; n++) {
            d = num % 10;
            num = num / 10;
            OamListAddSpriteGroup(base + d * 8, 0, 1, x - k++ * step, y, 2, g, h, i, 0, 0, l);
        }
        break;
    case 1:
        if (num == 0) {
            OamListAddSpriteGroup(base, 0, 1, x, y, 2, g, h, i, num, num, l);
        } else {
            for (n = 0; n < count; n++) {
                d = num % 10;
                num = num / 10;
                if (d == 0 && num == 0)
                    return;
                OamListAddSpriteGroup(base + d * 8, 0, 1, x - k++ * step, y, 2, g, h, i, 0, 0, l);
            }
        }
        break;
    }
    num++; /* FAKEMATCH: dead late use of num makes CSE keep num (not k) as the zero register */
}
void TweenInit(u16 x0, u16 y0, u16 x1, u16 y1, u16 dur, u16 b, struct Tween *t, u8 mode)
{
    switch (mode) {
    case 2:
    case 3:
        t->step = 0x4000 / (s16)dur;
        t->step2 = 0;
        t->kind = 1;
        {
            int a = (s16)x1, b2 = (s16)x0;
            t->dx = a - b2;
        }
        {
            int a = (s16)y1, b2 = (s16)y0;
            t->dy = a - b2;
        }
        t->x = x0;
        t->y = y0;
        t->x0 = x0;
        t->y0 = y0;
        t->x1 = x1;
        t->y1 = y1;
        break;
    case 0:
        t->x = x0;
        t->y = y0;
        t->x1 = x1;
        t->y1 = y1;
        t->step = dur;
        t->step2 = b;
        t->kind = 1;
        break;
    case 1:
        t->step = ((s16)dur + 0x7F) / (s16)dur;
        t->step2 = 0;
        t->kind = mode;
        {
            int a = (s16)x1, b2 = (s16)x0;
            t->dx = a - b2;
        }
        {
            int a = (s16)y1, b2 = (s16)y0;
            t->dy = a - b2;
        }
        t->x = x0;
        t->y = y0;
        t->x0 = x0;
        t->y0 = y0;
        t->x1 = x1;
        t->y1 = y1;
        break;
    }
    t->mode = mode;
}
/* Tween update (see struct Tween): mode 0 approaches (x1,y1) with a +-1 velocity, 1..3 ease along the sine table. */
void TweenUpdate(struct Tween *t)
{
    int d;

    if (t->kind != 1)
        return;
    switch (t->mode) {
    case 0:
        d = (s16)t->x - (s16)t->x1;
        if (d < 0)
            d = -d;
        if (d <= 7)
            t->step = (t->step > 0) ? 1 : -1;
        t->x += t->step;
        if (t->step >= 0) {
            if ((s16)t->x >= (s16)t->x1)
                t->x = t->x1;
        } else {
            if ((s16)t->x <= (s16)t->x1)
                t->x = t->x1;
        }
        d = (s16)t->y - (s16)t->y1;
        if (d < 0)
            d = -d;
        if (d <= 7)
            t->step2 = (t->step2 > 0) ? 1 : -1;
        t->y += t->step2;
        if (t->step2 >= 0) {
            if ((s16)t->y >= (s16)t->y1)
                t->y = t->y1;
        } else {
            if ((s16)t->y <= (s16)t->y1)
                t->y = t->y1;
        }
        if (*(u32 *)&t->x == *(u32 *)&t->x1)
            t->kind = 2;
        break;
    case 1:
        t->step2 += t->step;
        if (t->step2 > 0x7F) {
            t->step2 = 0x7F;
            t->kind = 2;
            t->x = t->x1;
            t->y = t->y1;
        } else {
            t->x = t->x0 + (sub_0807B4D0_i(t->dx << 4, (-gSineTable[(u8)t->step2 + 0x40] + 0x100) >> 1) >> 4);
            t->y = t->y0 + (sub_0807B4D0_i(t->dy << 4, (-gSineTable[(u8)t->step2 + 0x40] + 0x100) >> 1) >> 4);
        }
        break;
    case 2:
        t->step2 += t->step;
        if (t->step2 > 0x3F00) {
            t->step2 = 0x3F00;
            t->kind = 2;
            t->x = t->x1;
            t->y = t->y1;
        } else {
            t->x = t->x0 + (sub_0807B4D0_i(t->dx << 4, gSineTable[(u8)((u16)t->step2 >> 8)]) >> 4);
            t->y = t->y0 + (sub_0807B4D0_i(t->dy << 4, gSineTable[(u16)t->step2 >> 8]) >> 4);
        }
        /* falls through (as in the original) */
    case 3:
        t->step2 += t->step;
        if (t->step2 > 0x3F00) {
            t->step2 = 0x3F00;
            t->kind = 2;
            t->x = t->x1;
            t->y = t->y1;
        } else {
            t->x = t->x0 + (sub_0807B4D0_i(t->dx << 4, 0x100 - gSineTable[((u16)t->step2 >> 8) + 0x40]) >> 4);
            t->y = t->y0 + (sub_0807B4D0_i(t->dy << 4, 0x100 - gSineTable[((u16)t->step2 >> 8) + 0x40]) >> 4);
        }
        break;
    }
}
/* BGnHOFS/VOFS pair: bg selects the register pair (stride 4). */
void SetBgScrollRegs(u32 hofs, u32 vofs, u8 bg)
{
    *(u32 *)(0x04000010 + bg * 4) = (hofs & 0x1FF) | (vofs & 0x1FF) << 16;
}
void LinkSyncStart(u8 *p)
{
    p[8] = 0;
}
u32 LinkSyncStep(u16 id, u16 data, struct LinkSync *p)
{
    struct LinkEntry *tx = &p->tx;
    struct LinkEntry *rx = &p->rx;
    u8 *st = &p->state;
    u8 other = 1 ^ sub_0807BED8_x();

    switch (p->state) {
    case 0:
        if (LinkSyncOpen(tx, rx))
            (*st)++;
        p->timeout = 300;
        tx->phase = 0;
        break;
    case 1:
        tx->id = id;
        tx->data = data;
        tx->phase = 1;
        if (LinkSioSend(tx, 4))
            *st = 2;
        break;
    case 2:
        if (LinkSioRecv(other, rx, 4)) {
            switch (rx->phase) {
            case 1:
                if (rx->id != id) {
                    *st = 0;
                    return 0;
                }
                tx->phase = 4;
                LinkSioSend(tx, 4);
                p->timeout = 300;
                *st = 5;
                break;
            case 4:
                p->timeout = 300;
                *st = 5;
                return 1;
            case 3:
                p->timeout = 300;
                *st = 0;
                break;
            default:
                *st = 0;
                p->timeout = 300;
                break;
            }
        }
        break;
    case 3:
        *st = 1;
        break;
    case 5:
        LinkSyncClose(tx, rx);
        return 1;
    }
    if (--p->timeout == 0xFFFF)
        *st = 0;
    return 0;
}
u16 LinkSyncOpen(void *p_, void *unused)
{
    u8 i;
    u16 *p = p_;
    u8 *base = IntrTable;

    LinkSioInit(base, base + 0x1C);
    for (i = 0; i < 2; i++) {
        p[1] = 0;
        p += 2;
    }
    return 1;
}
u32 LinkSyncClose(void *a, void *b)
{
    LinkSioStop();
    MemClear16((void *)0x03005B60, 0xB38);
    return 1;
}
/* Multi-player SIO setup: baud rate a & 3, IRQ enable b & 1; clears SIOMLT_SEND and SIOMULTI0-3. */
int SioInitMultiPlayer(u8 baud, u8 irq)
{
    u16 i;

    *(vu16 *)0x04000134 = 0;
    REG_SIOCNT = (baud & 3) | (((irq & 1) << 15) | 0x2000);
    *(vu16 *)0x0400012A = 0;
    for (i = 0; i < 4; i++)
        ((vu16 *)0x04000120)[i] = 0;
}
u32 SioGetMultiPlayerId(void)
{
    return (REG_SIOCNT & 0x30) >> 4;
}
void SioSetMultiSend(u16 data)
{
    *(vu16 *)0x0400012A = data;
}
u16 SioGetMultiRecv(u8 id)
{
    return ((vu16 *)0x04000120)[id];
}
u16 SioGetError(void)
{
    return REG_SIOCNT & 0x40;
}
u16 SioAllReady(void)
{
    return REG_SIOCNT & 8;
}
u16 SioIsChild(void)
{
    return REG_SIOCNT & 4;
}
void SioStart(void)
{
    REG_SIOCNT |= 0x80;
}
u16 SioIsBusy(void)
{
    return REG_SIOCNT & 0x80;
}
void Password_DrawDigits(void)
{
    int i;
    u32 v;

    i = 0;
    v = 0x18;
    for (; i < 8; i++) {
        AddSprite(0x80000 | v, 0x8000, gPassword[i] + 0x1080);
        v += 8;
    }
}
void Password_DrawSlotCursor(int idx)
{
    u32 pos = ((idx << 3) + 0x18) | 0x100000;
    u16 *tbl = gPasswordSlotCursorFrames;
    u16 v = (gMain.counter >> 3) % 6;

    AddSprite(pos, 0, tbl[v]);
}
void Password_DrawKeyCursor(void)
{
    int i;

    for (i = 0; i <= 10; i++) {
        u32 attr = 0x1000 | gPasswordKeypad[i].flags;

        if (i == gPassword[0x10] >> 4) {
            if (i <= 9) {
                AddSprite(gPasswordKeypad[i].y << 16 | gPasswordKeypad[i].x, 0x40, attr);
            } else {
                AddSprite(0x880030, 0x4040, 0x1044);
                AddSprite(0x880050, 0x4000, 0x1048);
            }
        }
    }
}
extern const u8 gCardArtPalettes[];
extern const u16 gCardArtGfx[];

/* Fill or clear the ten-row portrait map, then unpack the card's 6bpp art. */
/* Card portrait (hypothesis: RenderCardPortrait): fills a 9 x 10 tile block of the BG map
 * at 0x0300045C + (a & 7) * 0x800 + b * 2 with ascending tile numbers from c / 2 (or clears
 * a 10 x 10 block when id == 0xFFFF), copies card id's 64-colour palette to 0x05000100 and
 * unpacks its 6bpp image (720 x 3 halfwords) to 0x06004000 + c * 32 with bit 7 set.
 * The ROM tables are integer addresses so GCSE does not hoist their pool loads; t is an int
 * (its 0x3F mask then shares the SImode constant with the other two masks) and is shifted
 * as u16 to keep the logical shift separate from y >> 8. */
void DrawCardPortraitOrClear(u16 a, u16 b, u16 id, u16 c)
{
    u16 *map = (u16 *)(0x0300045C + (a & 7) * 0x800);
    u16 i;
    u16 j;
    u16 tile;
    u32 off;
    const u16 *src;
    u16 *dst;

    map += b;
    if (id == 0xFFFF) {
        i = 0;
        do {
            map[0] = 0;
            map[1] = 0;
            map[2] = 0;
            map[3] = 0;
            map[4] = 0;
            map[5] = 0;
            map[6] = 0;
            map[7] = 0;
            map[8] = 0;
            map[9] = 0;
            map += 0x20;
            i++;
        } while (i <= 9);
    } else {
        i = 0;
        tile = c >> 1;
        off = c << 5;
        do {
            for (j = 0; j <= 8; j++)
                map[j] = tile++;
            map += 0x20;
            i++;
        } while (i <= 9);
        id &= 0x7FF;
        MemCopy16(0x05000100, (void *)(0x08608360 + id * 0x80), 0x80);
        src = (const u16 *)(0x082A6500 + id * 0x10E0);
        dst = (u16 *)(0x06004000 + off);

        for (i = 0; i <= 0x2CF; i++) {
            u32 x = src[0];
            u32 y = src[1];
            u32 z = src[2];
            int t;
            u16 p0, p1, p2, p3;

            p0 = (x & 0x3F) | (x & 0xFC0) << 2;
            p1 = x >> 12 | (y & 3) << 4 | (y & 0xFC) * 64;
            t = y >> 8;
            p2 = (t & 0x3F) | (((u16)t >> 6) | (z & 0xF) << 2) << 8;
            z >>= 4;
            p3 = (z & 0x3F) | (z & 0xFC0) << 2;
            dst[0] = p0 | 0x8080;
            dst[1] = p1 | 0x8080;
            dst[2] = p2 | 0x8080;
            dst[3] = p3 | 0x8080;
            src += 3;
            dst += 4;
        }
    }
}
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_KIND(id) ((int)((CARD_STATS(id) & 0x1F00000) >> 20))

/* Loads the card frame graphics for card `id` (kind 0x15-0x17 = Magic/Trap/Ritual frames, else by card number / type). */
void Password_DrawCard(u16 id)
{
    void *tbl;
    int v;
    int n;

    switch (CARD_KIND(id)) {
    case 0x15:
        tbl = gCardFrameTrapGfx;
        goto load;
    case 0x16:
        tbl = gCardFrameMagicGfx;
        goto load;
    case 0x17:
        tbl = gCardFrameTicketGfx;
        goto load;
    }
    n = ((const u16 *)0x08622AB4)[id & 0x7FF];
    switch (n) {
    case 0x776:
        v = 3;
        break;
    case 0x777:
    case 0x778:
        v = 1;
        break;
    default:
        switch (CARD_KIND(id)) {
        case 0x16:
            v = 7;
            break;
        case 0x15:
            v = 8;
            break;
        case 0x17:
            v = 9;
            break;
        default:
            v = (CARD_STATS(id) & 0xC0000) >> 18;
            break;
        }
        break;
    }
    switch (v) {
    case 1:
        tbl = gCardFrameEffectGfx;
        break;
    case 2:
        tbl = gCardFrameFusionGfx;
        break;
    case 3:
        tbl = gCardFrameRitualGfx;
        break;
    default:
        tbl = gCardFrameNormalGfx;
        break;
    }
load:
    {
        register u32 a asm("r0") = 0x420;
        register u32 b asm("r1") = 0x20;
        LoadBgImageMap1(a, b, 0x100, tbl);
    }
    DrawCardPortraitOrClear(2, 0xA2, id, 0x300);
}
u16 FindCardByPassword(void)
{
    u32 first;
    u8 buf[4];
    u32 i;
    u32 n;
    u8 *d;
    u8 *sp;
    u8 *e;
    /* FAKEMATCH: keep the stack-buffer pointer in r4 while the scan counts in r3. */
    register u8 *bufp __asm__("r4");
    u8 *base;

    i = 0;
    base = gPassword;
    d = buf;
    sp = base + 8;
    for (; i < 4; i++) {
        *d = *sp << 4;
        *d |= sp[1];
        d++;
        sp += 2;
    }
    n = 0;
    bufp = buf;
    first = bufp[0];
    for (; n <= 0x334; n++) {
        e = (u8 *)0x08623120 + n * 4;
        if (e[0] == first && e[1] == bufp[1] && e[2] == bufp[2] && e[3] == bufp[3])
            return n;
    }
    return 0;
}
u32 Password_InitVideo(void)
{
    vu32 *dma;

    ResetVideo();
    ClearBgMapBuffers();
    *(vu16 *)0x04000000 = 0;
    *(vu16 *)0x0400000A = 0x105;
    *(vu16 *)0x0400000C = 0x286;
    *(vu16 *)0x0400000E = 0x307;
    gUnk_03000040_b.field = 0x43;
    *(vu16 *)0x0400004C = 0;
    *(vu16 *)0x04000050 = 0;
    *(vu16 *)0x04000054 = 0;
    *(vu16 *)0x04000012 = 0;
    *(vu16 *)0x04000010 = 0;
    *(vu16 *)0x04000016 = 0;
    *(vu16 *)0x04000014 = 0;
    *(vu16 *)0x0400001A = 0;
    *(vu16 *)0x04000018 = 0;
    *(vu16 *)0x0400001E = 0;
    *(vu16 *)0x0400001C = 0;
    *(vu16 *)0x04000028 = 0;
    *(vu16 *)0x0400002A = 0;
    *(vu16 *)0x0400003C = 0;
    *(vu16 *)0x0400003E = 0;
    LoadBgImage4bppMap1(0, 0x10, 0x20, gPasswordKeypadBgGfx);
    LoadBgImage4bppMap1(0x800, 0x10, 0x88, gPasswordPanelBgGfx);
    MemCopy16(0x05000220, gPasswordObjPal, 0x20);
    dma = (vu32 *)0x040000D4;
    dma[0] = (u32)gPasswordObjGfx;
    dma[1] = 0x06010000;
    dma[2] = 0x80001600;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
    SetBrightnessBlack();
    return 1;
}
u32 Password_InitState(void)
{
    struct LinkState *s = &gUnk_0201F7B0_s;
    u16 mask;
    int zero;

    MemClear16(s, 0x18);
    mask = 15;
    zero = 0;
    /* Schedule the shared clearing constants before the field stores. */
    __asm__ __volatile__("" : : "r"(mask), "r"(zero));
    s->cur = 0;
    s->a = 0;
    s->b = 0;
    s->c = 0;
    s->d = 0;
    return 1;
}
u16 Password_FadeIn(void)
{
    *(vu16 *)0x04000000 = 0x1E00;
    *(vu16 *)0x0400004C = 0;
    return FadeFromBlack(2);
}
/* Password entry state at 0x0201F7B0 (same block as struct LinkState). */
struct PwView {
    u8 digits[8];   /* +0x00 entered digits */
    u8 shown[8];    /* +0x08 copy used for the lookup */
    u8 pos : 4;     /* +0x10 cursor position 0-7 */
    u16 key : 4;    /*       selected key: 0-9 digits, 10 = OK (u16: keeps the A-press extraction separate) */
    u8 blink : 6;   /* +0x11 */
    u32 timer : 6;  /* +0x10 bits 14-19 */
    u16 c : 12;     /* +0x12 bits 4-15 */
    u16 card;       /* +0x14 result of FindCardByPassword */
};
/* Keypad layout: screen position and the neighbours for up/down (byte 6) and left/right (byte 7). */
struct KeyNav { u8 x, y; u8 pad[4]; u8 up : 4; u8 down : 4; u8 left : 4; u8 right : 4; };
extern struct KeyNav gUnk_08087E24_n[] asm("gPasswordKeypad");
extern const u8 gStrDebugPassword[], gStrDebugPasswordDigit[], gStrDebugPasswordCard[];
void PlaySE(int se);
void DebugPrintf(const u8 *fmt, ...);
void DebugPrintFlush(void);
#define PW ((struct PwView *)gPassword)
struct KeysView { u8 pad[6]; u16 pressed; u8 pad2[0x4859 - 8]; u8 step; /* +0x4859 */ };
extern struct KeysView gKeys_C4CC asm("gMain");
#define PW_KEYS (gKeys_C4CC.pressed)
/* Password entry, one frame: L/R move the cursor, the d-pad moves between keys, A enters a digit (or looks up
 * the password on OK), B deletes or cancels. Returns 1 when the screen is done. */
u32 Password_HandleInput(void)
{
    int i;

    Password_DrawDigits();
    Password_DrawSlotCursor(PW->pos);
    Password_DrawKeyCursor();
    PW->blink++;
    PW->timer = 0x20;
    if (PW_KEYS & 0x200) {
        PW->pos = (PW->pos + 7) & 7;
        PW->blink = 0x20;
        PlaySE(0x25);
    }
    if (PW_KEYS & 0x100) {
        PW->pos = (PW->pos + 9) & 7;
        PW->blink = 0x20;
        PlaySE(0x25);
    }
    if (PW_KEYS & 0x40) {
        PW->key = gUnk_08087E24_n[PW->key].up;
        PW->timer = 0x20;
        PlaySE(0x25);
    }
    if (PW_KEYS & 0x80) {
        PW->key = gUnk_08087E24_n[PW->key].down;
        PW->timer = 0x20;
        PlaySE(0x25);
    }
    if (PW_KEYS & 0x20) {
        PW->key = gUnk_08087E24_n[PW->key].left;
        PW->timer = 0x20;
        PlaySE(0x25);
    }
    if (PW_KEYS & 0x10) {
        PW->key = gUnk_08087E24_n[PW->key].right;
        PW->timer = 0x20;
        PlaySE(0x25);
    }
    if (PW_KEYS & 1) {
        PlaySE(0x26);
        if ((u32)PW->key <= 9) {
            PW->digits[PW->pos] = PW->key;
            AddSprite((gUnk_08087E24_n[PW->key].x - 4) | ((gUnk_08087E24_n[PW->key].y - 12) << 16), 0x80, 0x10E0);
            if (PW->pos <= 6) {
                PW->pos++;
                PW->blink = 0x20;
            } else {
                PW->key = 10;
                PW->timer = 0x20;
            }
        } else {
            AddSprite(0x780024, 0x80, 0x10E3);
            AddSprite(0x780044, 0x80, 0x10E7);
            MemCopy16((u32)PW->shown, PW->digits, 8);
            PW->card = FindCardByPassword();
            PW->c = 0;
            DebugPrintf(gStrDebugPassword);
            for (i = 0; i <= 7; i++)
                DebugPrintf(gStrDebugPasswordDigit, PW->shown[i] + '0');
            DebugPrintf(gStrDebugPasswordCard, PW->card);
            DebugPrintFlush();
            return 1;
        }
    }
    if (PW_KEYS & 2) {
        if (PW->pos == 0) {
            /* Same tail as the L/R+B cancel below; cross-jumping merges the two copies, and keeping
             * them separate stops GCSE from reusing this block's 0x03000040 register below. */
            PlaySE(2);
            gKeys_C4CC.step = 10;
            return 1;
        }
        PW->pos--;
        PW->blink = 0x20;
        PlaySE(0x25);
    }
    if (PW_KEYS & 0xC) {
        PlaySE(2);
        gKeys_C4CC.step = 10;
        return 1;
    }
    return 0;
}
