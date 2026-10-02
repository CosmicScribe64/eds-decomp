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

extern void sub_080735D4(void *a, void *b);
extern void sub_0807373C(void);
extern void sub_08075278(void *p, u32 size);
extern void sub_080761F0(u32 a, u32 b, u32 c);
extern u8 gUnk_0201F7B0[];
extern u8 gUnk_03000000[];
extern struct OamEntry *sub_0807A320(u8 layer, struct OamList *l);
extern int sub_0807B4D0_i(int a, int b) asm("sub_0807B4D0");
extern s16 gUnk_08087BA4[];
extern void sub_08077EF4(u8 *a, u32 b, u32 c, int x, u16 y, u32 f, u8 g, u8 h, u8 i, u32 j, u32 k, u32 l);

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
extern void sub_08072EB0(u32 a, u32 b, u32 c, void *src);
extern void sub_0807C058(u16 a, u16 b, u16 id, u16 c);
extern u8 gUnk_08631558[];
extern u8 gUnk_0862EEC0[];
extern u8 gUnk_08633BF0[];
extern u8 gUnk_08627AF8[];
extern u8 gUnk_0862A190[];
extern u8 gUnk_0862C828[];
extern u8 gUnk_08625460[];
extern void sub_08073574(void);
extern void sub_08073498(void);
extern void sub_080731D0(u32 a, u32 b, u32 c, void *src);
extern void sub_08075294(u32 dst, void *src, u32 n);
extern void sub_080759F4(void);
extern u8 gUnk_0863862C[];
extern u8 gUnk_0863975C[];
extern u8 gUnk_08639DFC[];
extern u8 gUnk_08639E1C[];
struct MainBig {
    u8 pad[0x485E];
    u16 counter;
};
extern struct MainBig gUnk_03000040;
struct MainB {
    u8 pad[0x40E];
    u16 field;
};
extern struct MainB gUnk_03000040_b asm("gUnk_03000040");
extern u16 gUnk_08087E7C[];

struct MenuEntry {
    u8 x, y;
    u16 pad;
    u16 flags : 12;
    u16 pad2;
};
extern struct MenuEntry gUnk_08087E24[];
extern u8 gUnk_08623120[][4];
extern int sub_08075AE4(int);

/* Link-menu state at 0x0201F7B0 (size 0x18). */
struct LinkState {
    u8 pad[0x10];
    u8 cur;
    u8 a : 6;
    u32 b : 6;
    u16 c : 12;
    u16 d;
};
extern struct LinkState gUnk_0201F7B0_s asm("gUnk_0201F7B0");

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

extern u16 sub_0807BE34(void *p_, void *unused);
extern u32 sub_0807BE60(void *a, void *b);
extern u16 sub_0807BED8_x(void) asm("sub_0807BED8");
extern int sub_08073784(void *p, int n);
extern int sub_08073F04(int id, void *p, int n);


#if 0 /* NONMATCHING: structure and case constants match. The target folds the
       * u16 narrowing of x/y at their use (x in r6, y in r4, `lsl r3,#16`
       * right before the mask), while the build emits y's `lsl` in the
       * prologue and swaps r4/r6. The case block order also differs slightly. */
/* Allocates an OAM entry on `layer` and fills attr0-2: y/x, shape/size from the w x h pixel size,
 * tile, palette bank, priority; mode 8 = 256 colours. Returns the entry. */
struct OamEntry *sub_0807B6B8(u8 layer, u16 tile, u16 x, u16 y, u8 w, u8 h, u8 mode, u8 pal, u32 unused,
                               u16 flags, u8 aff, u8 prio, struct OamList *list)
{
    u32 xx = x & 0x1FF;
    u32 yy = y & 0xFF;
    struct OamEntry *e = sub_0807A320(layer, list);

    u32 v = aff << 25;

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
            e->w |= xx << 16 | yy;
            break;
        case 16:
            e->w |= 0x8000 | (xx << 16 | yy);
            break;
        case 32:
            e->w |= 0x40008000 | (xx << 16 | yy);
            break;
        case 64:
            while (1)
                ;
        }
        break;
    case 16:
        switch (h) {
        case 8:
            e->w |= 0x4000 | (xx << 16 | yy);
            break;
        case 16:
            e->w |= 0x40000000 | (xx << 16 | yy);
            break;
        case 32:
            e->w |= 0x80008000 | (xx << 16 | yy);
            break;
        case 64:
            while (1)
                ;
        }
        break;
    case 32:
        switch (h) {
        case 8:
            e->w |= 0x40004000 | (xx << 16 | yy);
            break;
        case 16:
            e->w |= 0x80004000 | (xx << 16 | yy);
            break;
        case 32:
            e->w |= 0x80000000 | (xx << 16 | yy);
            break;
        case 64:
            e->w |= 0xC0008000 | (xx << 16 | yy);
            break;
        }
        break;
    case 64:
        switch (h) {
        case 8:
        case 16:
            while (1)
                ;
        case 32:
            e->w |= 0xC0004000 | (xx << 16 | yy);
            break;
        case 64:
            e->w |= 0xC0000000 | (xx << 16 | yy);
            break;
        }
        break;
    }
    e->h = pal << 12 | tile | prio << 10;
    return e;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0807B6B8", sub_0807B6B8); /* 0x0807B6B8 size 0x1AC */
#if 0 /* NONMATCHING: register allocation only. The target keeps count in sl
       * and arg11 in r8, spilling the other u8 args; the build keeps arg11 in
       * sl and spills count. Structure and call arguments match. */
/* Draws a decimal number with sprites (digit sprite table `base`, 8 bytes per digit), right to left. */
void sub_0807B864(u16 num, u8 count, u8 mode, u16 x, u16 y, u8 *base, u32 unused, u8 step, u8 h, u8 i, u8 g, u32 l)
{
    int n;
    u8 k = 0;
    s16 d;

    switch (mode) {
    case 0:
        for (n = 0; n < count; n++) {
            d = num % 10;
            num = num / 10;
            sub_08077EF4(base + d * 8, 0, 1, x - k++ * step, y, 2, g, h, i, 0, 0, l);
        }
        break;
    case 1:
        if (num == 0) {
            sub_08077EF4(base, 0, 1, x, y, 2, g, h, i, num, num, l);
        } else {
            for (n = 0; n < count; n++) {
                d = num % 10;
                num = num / 10;
                if (d == 0 && num == 0)
                    break;
                sub_08077EF4(base + d * 8, 0, 1, x - k++ * step, y, 2, g, h, i, 0, 0, l);
            }
        }
        break;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0807B6B8", sub_0807B864); /* 0x0807B864 size 0x170 */
void sub_0807B9D4(u16 x0, u16 y0, u16 x1, u16 y1, u16 dur, u16 b, struct Tween *t, u8 mode)
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
void sub_0807BAB4(struct Tween *t)
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
            t->x = t->x0 + (sub_0807B4D0_i(t->dx << 4, (-gUnk_08087BA4[(u8)t->step2 + 0x40] + 0x100) >> 1) >> 4);
            t->y = t->y0 + (sub_0807B4D0_i(t->dy << 4, (-gUnk_08087BA4[(u8)t->step2 + 0x40] + 0x100) >> 1) >> 4);
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
            t->x = t->x0 + (sub_0807B4D0_i(t->dx << 4, gUnk_08087BA4[(u8)((u16)t->step2 >> 8)]) >> 4);
            t->y = t->y0 + (sub_0807B4D0_i(t->dy << 4, gUnk_08087BA4[(u16)t->step2 >> 8]) >> 4);
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
            t->x = t->x0 + (sub_0807B4D0_i(t->dx << 4, 0x100 - gUnk_08087BA4[((u16)t->step2 >> 8) + 0x40]) >> 4);
            t->y = t->y0 + (sub_0807B4D0_i(t->dy << 4, 0x100 - gUnk_08087BA4[((u16)t->step2 >> 8) + 0x40]) >> 4);
        }
        break;
    }
}
/* BGnHOFS/VOFS pair: bg selects the register pair (stride 4). */
void sub_0807BCD4(u32 hofs, u32 vofs, u8 bg)
{
    *(u32 *)(0x04000010 + bg * 4) = (hofs & 0x1FF) | (vofs & 0x1FF) << 16;
}
void sub_0807BCF4(u8 *p)
{
    p[8] = 0;
}
u32 sub_0807BCFC(u16 id, u16 data, struct LinkSync *p)
{
    struct LinkEntry *tx = &p->tx;
    struct LinkEntry *rx = &p->rx;
    u8 *st = &p->state;
    u8 other = 1 ^ sub_0807BED8_x();

    switch (p->state) {
    case 0:
        if (sub_0807BE34(tx, rx))
            (*st)++;
        p->timeout = 300;
        tx->phase = 0;
        break;
    case 1:
        tx->id = id;
        tx->data = data;
        tx->phase = 1;
        if (sub_08073784(tx, 4))
            *st = 2;
        break;
    case 2:
        if (sub_08073F04(other, rx, 4)) {
            switch (rx->phase) {
            case 1:
                if (rx->id != id) {
                    *st = 0;
                    return 0;
                }
                tx->phase = 4;
                sub_08073784(tx, 4);
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
        sub_0807BE60(tx, rx);
        return 1;
    }
    if (--p->timeout == 0xFFFF)
        *st = 0;
    return 0;
}
u16 sub_0807BE34(void *p_, void *unused)
{
    u8 i;
    u16 *p = p_;
    u8 *base = gUnk_03000000;

    sub_080735D4(base, base + 0x1C);
    for (i = 0; i < 2; i++) {
        p[1] = 0;
        p += 2;
    }
    return 1;
}
u32 sub_0807BE60(void *a, void *b)
{
    sub_0807373C();
    sub_08075278((void *)0x03005B60, 0xB38);
    return 1;
}
/* Multi-player SIO setup: baud rate a & 3, IRQ enable b & 1; clears SIOMLT_SEND and SIOMULTI0-3. */
int sub_0807BE7C(u8 baud, u8 irq)
{
    u16 i;

    *(vu16 *)0x04000134 = 0;
    REG_SIOCNT = (baud & 3) | (((irq & 1) << 15) | 0x2000);
    *(vu16 *)0x0400012A = 0;
    for (i = 0; i < 4; i++)
        ((vu16 *)0x04000120)[i] = 0;
}
u32 sub_0807BED8(void)
{
    return (REG_SIOCNT & 0x30) >> 4;
}
void sub_0807BEE8(u16 data)
{
    *(vu16 *)0x0400012A = data;
}
u16 sub_0807BEF8(u8 id)
{
    return ((vu16 *)0x04000120)[id];
}
u16 sub_0807BF08(void)
{
    return REG_SIOCNT & 0x40;
}
u16 sub_0807BF1C(void)
{
    return REG_SIOCNT & 8;
}
u16 sub_0807BF30(void)
{
    return REG_SIOCNT & 4;
}
void sub_0807BF44(void)
{
    REG_SIOCNT |= 0x80;
}
u16 sub_0807BF54(void)
{
    return REG_SIOCNT & 0x80;
}
void sub_0807BF68(void)
{
    int i;
    u32 v;

    i = 0;
    v = 0x18;
    for (; i < 8; i++) {
        sub_080761F0(0x80000 | v, 0x8000, gUnk_0201F7B0[i] + 0x1080);
        v += 8;
    }
}
void sub_0807BF9C(int idx)
{
    u32 pos = ((idx << 3) + 0x18) | 0x100000;
    u16 *tbl = gUnk_08087E7C;
    u16 v = (gUnk_03000040.counter >> 3) % 6;

    sub_080761F0(pos, 0, tbl[v]);
}
void sub_0807BFE0(void)
{
    int i;

    for (i = 0; i <= 10; i++) {
        u32 attr = 0x1000 | gUnk_08087E24[i].flags;

        if (i == gUnk_0201F7B0[0x10] >> 4) {
            if (i <= 9) {
                sub_080761F0(gUnk_08087E24[i].y << 16 | gUnk_08087E24[i].x, 0x40, attr);
            } else {
                sub_080761F0(0x880030, 0x4040, 0x1044);
                sub_080761F0(0x880050, 0x4000, 0x1048);
            }
        }
    }
}
extern const u8 gUnk_08608360[];
extern const u16 gUnk_082A6500[];

/* Fill or clear the ten-row portrait map, then unpack the card's 6bpp art. */
#if 0 /* NONMATCHING: clear/fill and pixel-repacking logic follows the ROM; i uses ip rather than r8 and ROM-table/0xFC0 hoisting differs. */
void sub_0807C058(u16 a, u16 b, u16 id, u16 c)
{
    u8 *row = (u8 *)(0x0300045C + (a & 7) * 0x800);
    u16 *map = (u16 *)(row + b * 2);
    u16 i;
    u16 j;
    u16 tile;
    u32 off;
    const u16 *src;
    u16 *dst;

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
        sub_08075294(0x05000100, (void *)(gUnk_08608360 + id * 0x80), 0x80);
        src = gUnk_082A6500 + id * 0x870;
        dst = (u16 *)(0x06004000 + off);
        for (i = 0; i <= 0x2CF; i++) {
            u32 x = src[0];
            int y = src[1];
            u32 z = src[2];
            u32 t;
            u16 p0, p1, p2, p3;

            p0 = (x & 0x3F) | (x & 0xFC0) << 2;
            p1 = x >> 12 | (y & 3) << 4 | (y & 0xFC) << 6;
            t = y >> 8;
            p2 = (t & 0x3F) | ((t >> 6) | (z & 0xF) << 2) << 8;
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
#endif
INCLUDE_ASM("asm/nonmatching/code_0807B6B8", sub_0807C058); /* 0x0807C058 size 0x178 */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_KIND(id) ((int)((CARD_STATS(id) & 0x1F00000) >> 20))

/* Loads the card frame graphics for card `id` (kind 0x15-0x17 = Magic/Trap/Ritual frames, else by card number / type). */
void sub_0807C1D0(u16 id)
{
    void *tbl;
    int v;
    int n;

    switch (CARD_KIND(id)) {
    case 0x15:
        tbl = gUnk_08631558;
        goto load;
    case 0x16:
        tbl = gUnk_0862EEC0;
        goto load;
    case 0x17:
        tbl = gUnk_08633BF0;
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
        tbl = gUnk_08627AF8;
        break;
    case 2:
        tbl = gUnk_0862A190;
        break;
    case 3:
        tbl = gUnk_0862C828;
        break;
    default:
        tbl = gUnk_08625460;
        break;
    }
load:
    {
        register u32 a asm("r0") = 0x420;
        register u32 b asm("r1") = 0x20;
        sub_08072EB0(a, b, 0x100, tbl);
    }
    sub_0807C058(2, 0xA2, id, 0x300);
}
u16 sub_0807C304(void)
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
    base = gUnk_0201F7B0;
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
u32 sub_0807C374(void)
{
    vu32 *dma;

    sub_08073574();
    sub_08073498();
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
    sub_080731D0(0, 0x10, 0x20, gUnk_0863862C);
    sub_080731D0(0x800, 0x10, 0x88, gUnk_0863975C);
    sub_08075294(0x05000220, gUnk_08639DFC, 0x20);
    dma = (vu32 *)0x040000D4;
    dma[0] = (u32)gUnk_08639E1C;
    dma[1] = 0x06010000;
    dma[2] = 0x80001600;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
    sub_080759F4();
    return 1;
}
u32 sub_0807C46C(void)
{
    struct LinkState *s = &gUnk_0201F7B0_s;
    u16 mask;
    int zero;

    sub_08075278(s, 0x18);
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
u16 sub_0807C4A8(void)
{
    *(vu16 *)0x04000000 = 0x1E00;
    *(vu16 *)0x0400004C = 0;
    return sub_08075AE4(2);
}
/* Password entry state at 0x0201F7B0 (same block as struct LinkState). */
struct PwView {
    u8 digits[8];   /* +0x00 entered digits */
    u8 shown[8];    /* +0x08 copy used for the lookup */
    u8 pos : 4;     /* +0x10 cursor position 0-7 */
    u32 key : 4;    /*       selected key: 0-9 digits, 10 = OK */
    u8 blink : 6;   /* +0x11 */
    u32 timer : 6;  /* +0x10 bits 14-19 */
    u16 c : 12;     /* +0x12 bits 4-15 */
    u16 card;       /* +0x14 result of sub_0807C304 */
};
/* Keypad layout: screen position and the neighbours for up/down (byte 6) and left/right (byte 7). */
struct KeyNav { u8 x, y; u8 pad[4]; u8 up : 4; u8 down : 4; u8 left : 4; u8 right : 4; };
extern struct KeyNav gUnk_08087E24_n[] asm("gUnk_08087E24");
extern const u8 gUnk_08087F88[], gUnk_08087F94[], gUnk_08087F98[];
void sub_08077AEC(int se);
void sub_0801A7DC(const u8 *fmt, ...);
void sub_0801A7E8(void);
#define PW ((struct PwView *)gUnk_0201F7B0)
struct KeysView { u8 pad[6]; u16 pressed; };
extern struct KeysView gKeys_C4CC asm("gUnk_03000040");
#define PW_KEYS (gKeys_C4CC.pressed)
#if 0 /* NONMATCHING: 63 lines; A-button block: target extracts key as (b<<24)>>28 with an unsigned compare */
/* Password entry, one frame: L/R move the cursor, the d-pad moves between keys, A enters a digit (or looks up
 * the password on OK), B deletes or cancels. Returns 1 when the screen is done. */
u32 sub_0807C4CC(void)
{
    int i;

    sub_0807BF68();
    sub_0807BF9C(PW->pos);
    sub_0807BFE0();
    PW->blink++;
    PW->timer = 0x20;
    if (PW_KEYS & 0x200) {
        PW->pos = (PW->pos + 7) & 7;
        PW->blink = 0x20;
        sub_08077AEC(0x25);
    }
    if (PW_KEYS & 0x100) {
        PW->pos = (PW->pos + 9) & 7;
        PW->blink = 0x20;
        sub_08077AEC(0x25);
    }
    if (PW_KEYS & 0x40) {
        PW->key = gUnk_08087E24_n[PW->key].up;
        PW->timer = 0x20;
        sub_08077AEC(0x25);
    }
    if (PW_KEYS & 0x80) {
        PW->key = gUnk_08087E24_n[PW->key].down;
        PW->timer = 0x20;
        sub_08077AEC(0x25);
    }
    if (PW_KEYS & 0x20) {
        PW->key = gUnk_08087E24_n[PW->key].left;
        PW->timer = 0x20;
        sub_08077AEC(0x25);
    }
    if (PW_KEYS & 0x10) {
        PW->key = gUnk_08087E24_n[PW->key].right;
        PW->timer = 0x20;
        sub_08077AEC(0x25);
    }
    if (PW_KEYS & 1) {
        sub_08077AEC(0x26);
        if ((u32)PW->key <= 9) {
            PW->digits[PW->pos] = PW->key;
            sub_080761F0((gUnk_08087E24_n[PW->key].x - 4) | ((gUnk_08087E24_n[PW->key].y - 12) << 16), 0x80, 0x10E0);
            if (PW->pos <= 6) {
                PW->pos++;
                PW->blink = 0x20;
            } else {
                PW->key = 10;
                PW->timer = 0x20;
            }
        } else {
            sub_080761F0(0x780024, 0x80, 0x10E3);
            sub_080761F0(0x780044, 0x80, 0x10E7);
            sub_08075294((u32)PW->shown, PW->digits, 8);
            PW->card = sub_0807C304();
            PW->c = 0;
            sub_0801A7DC(gUnk_08087F88);
            for (i = 0; i <= 7; i++)
                sub_0801A7DC(gUnk_08087F94, PW->shown[i] + '0');
            sub_0801A7DC(gUnk_08087F98, PW->card);
            sub_0801A7E8();
            return 1;
        }
    }
    if (PW_KEYS & 2) {
        if (PW->pos == 0)
            goto cancel;
        PW->pos--;
        PW->blink = 0x20;
        sub_08077AEC(0x25);
    }
    if (!(PW_KEYS & 0xC))
        return 0;
cancel:
    sub_08077AEC(2);
    ((u8 *)&gUnk_03000040)[0x4859] = 10;
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0807B6B8", sub_0807C4CC); /* 0x0807C4CC size 0x2FC */
