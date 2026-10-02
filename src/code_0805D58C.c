#include "global.h"

#define IO32(off) (*(vu32 *)(0x04000000 + (off)))
#define IO16(off) (*(vu16 *)(0x04000000 + (off)))
void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2);
struct PickBits { u32 f0 : 1; u32 f1 : 4; u32 f2 : 9; u32 rest : 18; };
/* Block at 0x0201D7E4 (step machine that scrolls a picked zone into view). */
struct Pick {
    u32 w0;             /* +0 */
    u8 step;            /* +4 */
    u8 pad5;
    s16 x;              /* +6 */
    s16 y;              /* +8 */
    u16 padA;
    u32 bits;           /* +0xC packed position (struct PickBits) */
    u32 bits2;          /* +0x10 */
    u8 pad14[0x1E - 0x14];
    u16 h1E;
};
extern struct Pick gUnk_0201D7E4;
#define PICK_BITS (*(struct PickBits *)&gUnk_0201D7E4.bits)
#define PICK_BITS2 (*(struct PickBits *)&gUnk_0201D7E4.bits2)
extern const u16 gUnk_081A4454[];
extern const u16 gUnk_081A43E4[];
struct Sel {
    u8 pl;              /* +0 */
    u8 zone;            /* +1 */
    u16 h;              /* +2: low byte = flag, high byte = row */
};
struct ScrFlags {
    u8 pad[0x830];
    u8 f0 : 1;
    u8 rest : 7;
    u8 pad831[3];
    struct Sel sel;     /* +0x834 */
    u8 step;            /* +0x838 */
    u8 cnt;             /* +0x839 */
};
extern struct ScrFlags gUnk_0201CFB0;
extern u8 gUnk_0201930C[];
extern const u16 gUnk_081A4474[];
void sub_08077AEC(u16 se);
void sub_08060FD0(int a, int b);
int sub_08062140(int id);
void sub_08076714(u32 yx, u16 a, u16 attr, u32 c);
#define CARD_ID(w) (((w) << 20) >> 20)
struct MainBig { u8 pad0[4]; u16 held; u8 pad[0x4862 - 6]; u16 vcount; };
extern struct MainBig gUnk_03000040;
struct Blk18450 { u8 pad[0x15D]; u8 k; };
extern struct Blk18450 gUnk_02018450;
extern const u32 gUnk_0819DD94[];
extern const u32 gUnk_081A0594[];
extern const u16 gUnk_081A2D94[];
#define K gUnk_02018450.k
#define H gUnk_03000040.vcount
int sub_080623AC(int a, int b, int c);
int sub_080623EC(int a, int b, int c);
void sub_08024134(u32 player, u32 a, u32 b);
void sub_080241F0(u32 a);
void sub_08024248(int a, int b, int c);
void sub_08060578(void);
void sub_0805DE34(int a, int x, int y, u32 val);
void sub_0805DE6C(int a, int x, int y, u32 val);
void sub_0805DD64(int x, int y, int val);
void sub_0805DDC4(int x, int y, int val, int k);
int sub_08076F9C(void);
void sub_08075294(u32 dst, const void *src, u32 n);
extern const u8 gUnk_08608360[];
extern const u16 gUnk_082A6500[];

extern const u16 gUnk_081A44D4[];
/* Card slot record: 0x94 bytes per zone, 0xD64 bytes per player side, at 0x0201930C. */
struct Zone58C { u32 w; u8 b4; u8 b5; u8 f0 : 1; u8 fb : 1; u8 rest : 6; u8 pad[0x94 - 7]; };
struct Side58C { struct Zone58C z[23]; u8 pad[0xD64 - 23 * 0x94]; };
/* gUnk_081A4474 as [set][frame] and gUnk_081A44D4 as [flag][frame]: real 2D arrays give the
   ROM's (frame*2 + set*48) + base address order. */
extern const u16 gAnimFrames_081A4474[][24];
extern const u16 gAnimScale_081A44D4[][10];
void sub_0805D58C(void)
{
    struct ScrFlags *sc = &gUnk_0201CFB0;
    struct Sel *q = &sc->sel;
    u8 pl, zone;
    int z;
    u8 flag;
    int side;
    int fb;
    int a, b;
    int row;
    int id;
    u8 *step;
    pl = q->pl;
    /* Loading zone through an int temporary shortens zone's live range by one insn before
       combine, so zone is allocated before pl (zone r6, pl r7 as in the ROM). */
    z = q->zone;
    zone = z;
    flag = *(u8 *)&q->h;
    side = pl & 1;
    /* Pointer arithmetic (symbol last) puts the base literal after the offset, as in the ROM. */
    fb = ((struct Zone58C *)(side * 0xD64 + zone * 0x94 + (u32)gUnk_0201930C))->fb;
    row = q->h >> 8;
    id = CARD_ID(((struct Side58C *)gUnk_0201930C)[side].z[zone].w);
    step = &sc->step;
    switch (*step) {
    case 0:
        sub_08077AEC(6);
        sub_08060FD0(pl, zone);
        sc->cnt = 0;
        (*step)++;
        /* fall through */
    case 1:
        if (gUnk_0201CFB0.cnt <= 9) {
            u16 attr;
            a = sub_080623AC(pl, 0, zone);
            b = sub_080623EC(pl, 0, zone);
            attr = gAnimFrames_081A4474[fb][0];
            if (row != 0)
                attr = gAnimFrames_081A4474[fb][(gUnk_0201CFB0.cnt * 24) / 10];
            if (attr & 0x1000) {
                attr &= 0xEFFF;
                {
                    /* An int temporary keeps the (s16) sign extension and the call + 0x1000 order. */
                    int k = sub_08062140(id) + 0x1000;
                    int s = (s16)attr + k;
                    attr = s;
                }
            }
            sub_08076714((b << 16) | a, 0x80, attr | 0x400,
                         0x1000000 | gAnimScale_081A44D4[flag][gUnk_0201CFB0.cnt]);
            gUnk_0201CFB0.cnt++;
            if (gUnk_0201CFB0.cnt <= 9)
                return;
        }
        /* fall through */
    default:
        gUnk_0201CFB0.f0 = 0;
        break;
    }
}
/* Card slot record: 0x94 bytes per zone, 0xD64 bytes per player side, at 0x0201930C. */
struct Zone708 { u32 w; u8 pad[0x94 - 4]; };
struct Side708 { struct Zone708 z[23]; u8 pad[0xD64 - 23 * 0x94]; };
/* Same table as gUnk_081A4474, as [row][frame]: a real 2D array gives the ROM's (frame*2 + row*48) + base. */
extern const u16 gAnimFrames_081A4474[][24];
void sub_0805D708(void)
{
    struct ScrFlags *sc = &gUnk_0201CFB0;
    struct Sel *q = &sc->sel;
    u8 pl, zone;
    u8 flag;
    int row;
    int id;
    u8 *step;
    pl = q->pl;
    zone = q->zone;
    flag = *(u8 *)&q->h;
    row = q->h >> 8;
    id = CARD_ID(((struct Side708 *)gUnk_0201930C)[pl & 1].z[zone].w);
    step = &sc->step;
    switch (*step) {
    case 0:
        sub_08077AEC(6);
        sub_08060FD0(pl, zone);
        sc->cnt = 0;
        (*step)++;
        /* fall through */
    case 1:
        if (gUnk_0201CFB0.cnt <= 0x17) {
            int a = sub_080623AC(pl, 0, zone);
            int b = sub_080623EC(pl, 0, zone);
            /* FAKEMATCH: `pl = row` (row <= 0xFF) reuses pl as the row index; it keeps pl live
               past the second call, which puts pl in r7 and zone in r6 as in the ROM. */
            u16 attr = gAnimFrames_081A4474[pl = row][gUnk_0201CFB0.cnt];
            if (attr & 0x1000) {
                attr &= 0xEFFF;
                {
                    /* An int temporary keeps the (s16) sign extension and the call + 0x1000 order. */
                    int k = sub_08062140(id) + 0x1000;
                    int s = (s16)attr + k;
                    attr = s;
                }
            }
            sub_08076714((b << 16) | a, 0x80, attr | 0x400, flag ? 0x1000020 : 0x1000000);
            gUnk_0201CFB0.cnt++;
            if (gUnk_0201CFB0.cnt <= 0x17)
                return;
        }
        /* fall through */
    default:
        gUnk_0201CFB0.f0 = 0;
        break;
    }
}
void sub_0805D848(void)
{
    struct PickBits *src = &PICK_BITS;
    struct PickBits *dst = &PICK_BITS2;
    if (*((u8 *)dst - 12) == 0) {
        if ((*(u8 *)src & 0x1E) == 0x1A && (*(u8 *)dst & 0x1E) == 0x16)
            sub_08077AEC(14);
        else
            sub_08077AEC(7);
    }
    if (gUnk_0201CFB0.step <= 15) {
        int x = sub_080623AC(src->f0, src->f1, src->f2);
        int y = sub_080623EC(src->f0, src->f1, src->f2);
        int x2 = sub_080623AC(dst->f0, dst->f1, dst->f2);
        int y2 = sub_080623EC(dst->f0, dst->f1, dst->f2);
        int dx, dy;
        u16 tile = 0x40;
        int flip = 0;
        u16 t;
        int old;
        dx = x2 - x;
        dy = y2 - y;
        t = gUnk_081A4454[gUnk_0201CFB0.step];
        dx *= t;
        dy *= t;
        dx /= 256;
        dy /= 256;
        if (((u8 *)dst)[1] & 0x80)
            tile = sub_08062140(*(u32 *)&gUnk_0201CFB0.sel) + 0x1000;
        if ((u8)(((u8 *)src)[1] & 0x40) == (u8)(((u8 *)dst)[1] & 0x40)) {
            if ((u8)(((u8 *)dst)[1] & 0x40)) flip = 0x20;
        } else if ((u8)(((u8 *)dst)[1] & 0x40)) {
            flip = gUnk_0201CFB0.step * 2;
        } else {
            flip = 0x20 - gUnk_0201CFB0.step * 2;
        }
        sub_08076714((x + dx) | ((y + dy) << 16), 0x80, tile + 0x400,
                     ((u32)gUnk_081A43E4[gUnk_0201CFB0.step] << 16) | flip);
        old = gUnk_0201CFB0.step;
        gUnk_0201CFB0.step = old + 1;
        if ((gUnk_03000040.held & 2) || (1 & *(u8 *)&gUnk_0201CFB0)) {
            if (gUnk_0201CFB0.step <= 11)
                gUnk_0201CFB0.step = old + 4;
        }
    }
    if (gUnk_0201CFB0.step == 16)
        gUnk_0201CFB0.f0 = 0;
}
void sub_0805DA1C(void)
{
    struct PickBits *pb = &PICK_BITS;
    struct ScrFlags *screen;
    u8 *step = (u8 *)pb - 8;
    if (*step == 0)
        sub_08077AEC(7);
    if (*step <= 0xF) {
        int a1 = sub_080623AC(pb[0].f0, pb[0].f1, pb[0].f2);
        int b1 = sub_080623EC(pb[0].f0, pb[0].f1, pb[0].f2);
        int a2 = sub_080623AC(pb[1].f0, pb[1].f1, pb[1].f2);
        int b2 = sub_080623EC(pb[1].f0, pb[1].f1, pb[1].f2);
        int dx1 = a2 - a1;
        int dy1 = b2 - b1;
        int dx2 = a1 - a2;
        int dy2 = b1 - b2;
        u16 t = gUnk_081A4454[*step];
        u8 old;
        dx1 *= t;
        dy1 *= t;
        dx1 /= 256;
        dy1 /= 256;
        dx2 *= t;
        dy2 *= t;
        dx2 /= 256;
        dy2 /= 256;
        /* Stage the screen address before drawing; read its flag afterward. */
        screen = &gUnk_0201CFB0;
        sub_08076714((a1 + dx1) | ((b1 + dy1) << 16), 0x80, 0x440, (u32)gUnk_081A43E4[*step] << 16);
        sub_08076714((a2 + dx2) | ((b2 + dy2) << 16), 0x80, 0x440, (u32)gUnk_081A43E4[*step] << 16);
        old = *step;
        *step = old + 1;
        if ((gUnk_03000040.held & 2) || (1 & *(u8 *)screen)) {
            if (*step <= 0xB)
                *step = old + 4;
        }
    }
    if (gUnk_0201CFB0.step == 0x10)
        gUnk_0201CFB0.f0 = 0;
}

void sub_0805DB90(void)
{
    int a = sub_080623AC(PICK_BITS.f0, PICK_BITS.f1, PICK_BITS.f2);
    int b = sub_080623EC(PICK_BITS.f0, PICK_BITS.f1, PICK_BITS.f2);
    u8 *step = &gUnk_0201D7E4.step;
    switch (*step) {
    case 0:
        sub_08024134(0, 0, 0);
        (*step)++;
        break;
    case 1: {
        u32 *w = &gUnk_0201D7E4.w0;
        sub_080241F0(*w);
        (*step)++;
        break;
    }
    case 2: {
        s16 *px = &gUnk_0201D7E4.x;
        s16 *py;
        int t = a + *px;
        py = &gUnk_0201D7E4.y;
        sub_08024248(t, b + *py, 1);
        if (gUnk_0201D7E4.h1E == 0)
            (*step)++;
        break;
    }
    default:
        sub_08060578();
        gUnk_0201CFB0.f0 = 0;
        break;
    }
}
struct WarpTbl32 { u32 a[16][160]; };
struct WarpTbl16 { u16 a[16][160]; };
#define WA (((const struct WarpTbl32 *)gUnk_0819DD94)->a)
#define WB (((const struct WarpTbl32 *)gUnk_081A0594)->a)
#define WC (((const struct WarpTbl16 *)gUnk_081A2D94)->a)
void sub_0805DC38(void)
{
    H = *(vu16 *)0x04000006;
    IO32(0x28) = WA[K][H];
    IO32(0x2C) = WB[K][H];
    IO16(0x20) = WC[K][H];
    IO32(0x38) = WA[K][H];
    IO32(0x3C) = WB[K][H];
    IO16(0x30) = WC[K][H];
}
void sub_0805DD3C(void)
{
    IO32(0x28) = 0;
    IO32(0x2C) = 0;
    IO16(0x20) = 0x100;
    IO32(0x38) = 0;
    IO32(0x3C) = 0;
    IO16(0x30) = 0x100;
}
void sub_0805DD64(int x, int y, int val)
{
    int n = val;
    int base = 0x2020;
    x += 0x14;
    if (n == 0) {
        sub_080761F0((y << 16) | x, 0, base);
    } else {
        do {
            sub_080761F0(x | (y << 16), 0, (u16)(n % 10 + base));
            n /= 10;
            x -= 4;
        } while (n != 0);
    }
}
void sub_0805DDC4(int x, int y, int val, int k)
{
    int n = val;
    u16 base = 0x302E + k * 0x30;
    x += 0x50;
    if (n == 0) {
        sub_080761F0((y << 16) | x, 0x40, base);
    } else {
        do {
            sub_080761F0(x | (y << 16), 0x40, (u16)(base + (n % 10) * 4));
            n /= 10;
            x -= 0x10;
        } while (n != 0);
    }
}
void sub_0805DE34(int a, int x, int y, u32 val)
{
    x += a * 0x78;
    x += 0x47;
    y += 0x7E;
    sub_080761F0((y << 16) | x, 0x4000, 0x202A);
    sub_0805DD64(x + 4, y, val);
}
void sub_0805DE6C(int a, int x, int y, u32 val)
{
    x += a * 0x78;
    x += 0x47;
    y += 0x86;
    sub_080761F0((y << 16) | x, 0x4000, 0x202C);
    sub_0805DD64(x + 4, y, val);
}
void sub_0805DEA4(u16 mask, u32 *vals, u16 flag)
{
    int i;
    u32 *p;
    int sh;
    for (i = 0, p = vals, sh = 0; i <= 1; p++, sh += 8, i++) {
        if (flag == 0 || (mask & (2 << sh)) == 0) {
            if (((int)mask >> sh) & 1)
                sub_0805DE6C(i, 0, 0, *p);
            else
                sub_0805DE34(i, 0, 0, *p);
        }
    }
}
void sub_0805DF04(int a, int b, u16 c)
{
    int x = a * 0x68 + 8;
    int k = a;
    if (c != 0)
        k = sub_08076F9C() & 3;
    sub_0805DDC4(x, 0x40, b, k);
}
/* Portrait loader: copies portrait b's 64-colour palette to palette slot d >> 4, unpacks its
 * 720 x 3 halfwords of packed 6-bit pixels (8 pixels per 3 halfwords) to one pixel per byte at
 * VRAM 0x06004000 + a * 0x4000 + c * 32, then adds the palette base (d & 0xFF) to all 0xB40
 * halfwords. The ROM tables are integer addresses (as in sub_08061D24) so that both bases are
 * rematerialized by reload, which sets the ROM's reload-register rotation. `(s1 & 0xFC) * 64`
 * (not `<< 6`) keeps that term out of the u16 narrowing, so its mask ties to the output. */
void sub_0805DF34(int a, u16 b, u16 c, u16 d)
{
    const u16 *src;
    u16 *dst;
    u16 *p;
    int n;
    u32 i;
    u16 m6, m12;

    sub_08075294(0x05000000 + (d >> 4) * 0x20, (const void *)(0x08608360 + b * 0x80), 0x80);
    src = (const u16 *)(0x082A6500 + b * 0x10E0);
    {
        u32 addr = a << 14;
        addr += 0x06004000;
        addr += c << 5;
        dst = (u16 *)addr;
    }
    m6 = 0x3F;
    m12 = 0xFC0;
    for (n = 720; n != 0; n--) {
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
    {
        u32 addr = a << 14;
        addr += 0x06004000;
        addr += c << 5;
        p = (u16 *)addr;
    }
    for (i = 0; i <= 0xB3F; i++) {
        *p = (*p & 0x3F3F) + ((u8)d << 8 | (u8)d);
        p++;
    }
}
void sub_0805E054(int a, u16 *p, u16 y, u16 b)
{
    int off = *p * 2;
    u16 *cnt = (u16 *)((u8 *)p + (off + 8));
    u16 *src = (u16 *)((u8 *)p + (off + 16));
    u32 addr = a << 14;
    u16 *dst;
    u16 i;
    u16 v;
    addr += 0x06004000;
    addr += y << 5;
    dst = (u16 *)addr;
    for (i = 0; i < (*cnt << 5); i++) {
        u16 w = *src;
        v = w;
        /* Keep the loaded word separate from the pixel accumulator. */
        __asm__ __volatile__("" : : "r"(w));
        if (w & 0xFF00)
            v += (u8)b << 8;
        if (v & 0xFF)
            v += (u8)b;
        *dst = v;
        dst++;
        src++;
    }
    sub_08075294(0x05000000 + (b >> 4) * 0x20, p + 4, 0x40);
}
void sub_0805E100(int a, int x, int y, int v)
{
    u16 *dst = (u16 *)((a << 11) + 0x06000000);
    int row;
    v /= 2;
    dst += x / 2;
    dst += y * 16;
    for (row = 0; row < 10; row++) {
        int j;
        u16 *p;
        if (x & 1) {
            *dst = (u8)v << 8;
            v++;
            p = dst + 1;
            for (j = 0; j < 4; j++) {
                p[j] = (u8)v | (u8)(v + 1) << 8;
                v += 2;
            }
        } else {
            for (j = 0; j < 4; j++) {
                dst[j] = (u8)v | (u8)(v + 1) << 8;
                v += 2;
            }
            dst[4] = (u8)v;
            v++;
        }
        dst += 16;
    }
}
#if 0 /* NONMATCHING: 174 lines; shape right, register allocation differs (t in r7 vs r3, row in r6 vs r7) */
void sub_0805E1D0(int bg, int x, int y, int tile)
{
    u16 *row;
    int t;
    int i, j;

    row = (u16 *)(0x06000000 + bg * 0x800);
    t = tile / 2;
    row += x / 2;
    row += y * 16;
    for (i = 0; i < 4; i++) {
        if (x & 1) {
            row[0] = (u8)t << 8;
            t++;
            for (j = 0; j < 6; j++) {
                row[1 + j] = (u8)t | ((u8)(t + 1) << 8);
                t += 2;
            }
        } else {
            for (j = 0; j < 6; j++) {
                row[j] = (u8)t | ((u8)(t + 1) << 8);
                t += 2;
            }
            row[6] = (u8)t;
            t++;
        }
        row += 16;
    }
    for (i = 0; i < 10; i++) {
        if (x & 1) {
            row[0] |= t << 8;
            row[1] |= (u8)(t + 1);
            row[6] |= (u8)(t + 2) | ((u8)(t + 3) << 8);
        } else {
            row[0] |= (u8)t | ((u8)(t + 1) << 8);
            row[5] |= (u8)(t + 2) << 8;
            row[6] |= (u8)(t + 3);
        }
        t += 4;
        row += 16;
    }
    for (i = 0; i < 4; i++) {
        if (x & 1) {
            row[0] = (u8)t << 8;
            t++;
            for (j = 0; j < 6; j++) {
                row[1 + j] = (u8)t | ((u8)(t + 1) << 8);
                t += 2;
            }
        } else {
            for (j = 0; j < 6; j++) {
                row[j] = (u8)t | ((u8)(t + 1) << 8);
                t += 2;
            }
            row[6] = (u8)t;
            t++;
        }
        row += 16;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0805D58C", sub_0805E1D0); /* 0x0805E1D0 size 0x1E8 */
#if 0 /* NONMATCHING: scene init (loads both players' portrait sprite + palette, sets BG/OBJ tiles, installs the HBlank handler); cleaned-up m2c draft compiles but the prologue of about 50 instructions already diverges (gcc folds the two clear-bit masks into one AND, reorders the arg-halfword masks, and sizes the function smaller) */
extern u8 gUnk_020185A4[];
extern const u32 gUnk_08621DE0[];
extern const u16 gUnk_08622AB4[];
extern const u16 gUnk_08631558[];
extern const u16 gUnk_0862EEC0[];
extern const u16 gUnk_08633BF0[];
extern const u16 gUnk_08627AF8[];
extern const u16 gUnk_0862A190[];
extern const u16 gUnk_0862C828[];
extern const u16 gUnk_08625460[];
extern const u16 gUnk_0863840C[];
extern const u16 gUnk_0863842C[];
extern const u16 gUnk_0868247C[];
extern const u16 gUnk_0868267C[];
extern void (*gUnk_03000000[16])(void);
void sub_08075278(void *dst, u32 size);
void sub_080752B0(void *dst, const void *src, u32 size);
void sub_08073498(void);
void sub_080757AC(void);
void sub_0805DF34(int a, u16 b, u16 c, u32 d);
void sub_0805E054(int a, u16 *p, u16 y, u16 b);
void sub_0805E100(int a, int x, int y, int v);
void sub_0805E1D0(int a, int x, int y, int v);
void sub_0805DC38(void);

void sub_0805E3B8(u16 arg0, u16 arg1)
{
    s8 p0 = arg0;
    s8 p1 = arg1;

    sub_08075278(&gUnk_020185A4, 0xC);
    gUnk_0201CFB0.pad[0] = (-3 & gUnk_0201CFB0.pad[0]) & -5;
    *(u16 *)((u8 *)&gUnk_03000040 + 0x40E) = 1;
    *(vu16 *)0x04000050 = 0;
    *(vu16 *)0x0400000C = 0x4084;
    sub_08073498();
    *(vu16 *)0x0400000E = 0x4188;
    sub_080757AC();
    sub_080752B0((void *)0x05000240, gUnk_0863840C, 0x20);
    sub_080752B0((void *)0x06010400, gUnk_0863842C, 0x1C0);
    sub_080752B0((void *)0x05000260, gUnk_0868247C, 0x20);
    sub_080752B0((void *)0x060105C0, gUnk_0868267C, 0x1800);
    sub_08075278((void *)0x06004000, 0x4000);
    sub_08075278((void *)0x06000000, 0x800);
    if (p0 != 0) {
        const u16 *spr;
        u16 st;
        sub_0805DF34(0, p0, 0x14C, 0x40);
        sub_0805E100(0, 3, 5, 0x14C);
        st = (gUnk_08621DE0[p0 & 0x7FF] & 0x1F00000) >> 20;
        switch (st) {
        case 21:
            spr = gUnk_08631558;
            break;
        case 22:
            spr = gUnk_0862EEC0;
            break;
        case 23:
            spr = gUnk_08633BF0;
            break;
        default: {
            s16 t = gUnk_08622AB4[p0 & 0x7FF];
            int v;
            if (t == 0x776) {
                v = 3;
            } else if (t >= 0x776 && t <= 0x778) {
                v = 1;
            } else {
                int st2 = (gUnk_08621DE0[p0 & 0x7FF] & 0x1F00000) >> 20;
                switch (st2) {
                case 22: v = 7; break;
                case 21: v = 8; break;
                case 23: v = 9; break;
                default: v = (gUnk_08621DE0[p0 & 0x7FF] & 0xC0000) >> 18; break;
                }
            }
            switch (v) {
            case 1: spr = gUnk_08627AF8; break;
            case 2: spr = gUnk_0862A190; break;
            case 3: spr = gUnk_0862C828; break;
            default: spr = gUnk_08625460; break;
            }
            break;
        }
        }
        sub_0805E054(0, (u16 *)spr, 0x2C, 0x80);
        sub_0805E1D0(0, 1, 1, 0x2C);
    }
    sub_08075278((void *)0x06008000, 0x4000);
    sub_08075278((void *)0x06000800, 0x800);
    if (p1 != 0) {
        const u16 *spr;
        u16 st;
        sub_0805DF34(1, p1, 0x14C, 0xA0);
        sub_0805E100(1, 0x12, 5, 0x14C);
        st = (gUnk_08621DE0[p1 & 0x7FF] & 0x1F00000) >> 20;
        switch (st) {
        case 21:
            spr = gUnk_08631558;
            break;
        case 22:
            spr = gUnk_0862EEC0;
            break;
        case 23:
            spr = gUnk_08633BF0;
            break;
        default: {
            s16 t = gUnk_08622AB4[p1 & 0x7FF];
            int v;
            if (t == 0x776) {
                v = 3;
            } else if (t >= 0x776 && t <= 0x778) {
                v = 1;
            } else {
                int st2 = (gUnk_08621DE0[p1 & 0x7FF] & 0x1F00000) >> 20;
                switch (st2) {
                case 22: v = 7; break;
                case 21: v = 8; break;
                case 23: v = 9; break;
                default: v = (gUnk_08621DE0[p1 & 0x7FF] & 0xC0000) >> 18; break;
                }
            }
            switch (v) {
            case 1: spr = gUnk_08627AF8; break;
            case 2: spr = gUnk_0862A190; break;
            case 3: spr = gUnk_0862C828; break;
            default: spr = gUnk_08625460; break;
            }
            break;
        }
        }
        sub_0805E054(1, (u16 *)spr, 0x2C, 0xE0);
        sub_0805E1D0(1, 0x10, 1, 0x2C);
    }
    gUnk_02018450.k = 0;
    *(vu16 *)0x04000208 = 0;
    *(vu16 *)0x04000200 &= 0xFFFD;
    gUnk_03000000[1] = sub_0805DC38;
    *(vu16 *)0x04000208 = 1;
    *(vu16 *)0x04000208 = 0;
    *(vu16 *)0x04000200 |= 2;
    *(vu16 *)0x04000208 = 1;
}
#else
INCLUDE_ASM("asm/nonmatching/code_0805D58C", sub_0805E3B8); /* 0x0805E3B8 size 0x3D0 */
#endif
