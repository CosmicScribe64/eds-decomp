#include "global.h"
#include "gba.h"

/*
 * OAM affine/sprite emitters, video helpers, Random, save signature, misc.
 * See wiki/functions/code-08076144.md
 */

struct OamEntry {
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 affine;             /* every 4th entry's pad halfword holds an affine parameter */
};

struct Main {
    u32 rngState;               /* +0 */
    u8 pad0[0x4430 - 4];
    struct OamEntry oam[128];   /* +0x4430 */
    u8 oamCount;                /* +0x4830 */
    u8 affineCount;             /* +0x4831 */
};
extern struct Main gUnk_03000040;
extern struct OamEntry gUnk_03004470[];
extern const s16 gUnk_081A77A8[];

/* Animated OBJ graphics stream state (hypothesis): header = 0x20 byte palette, u16 count, count*4 bytes of table, then count * {u16 tiles; tiles*32 bytes}. */
struct SprAnim {
    u8 *base;               /* +0 */
    u8 *cur;                /* +4 */
    u16 unk8;               /* +8 */
    u16 unkA;               /* +A */
    u16 count;              /* +C */
    u16 pieces;             /* +E */
};
extern void sub_08075294(void *dst, const void *src, u32 size);
extern void sub_08076160(u16 idx, u16 scale, u16 angle);
extern void sub_080769DC(struct SprAnim *a);

/* Save mirror at 0x02011C20 (see code_0807717C). */
struct CardCount {
    u16 count : 10;
    u16 rest : 6;
    u16 unkA;
};
struct CardBits {
    u8 unk0;
    u8 pad : 2;
    u8 n1 : 2;
    u8 n2 : 2;
    u8 n3 : 2;
    u16 unkA;
};
union CardEntry {
    struct CardCount c;
    struct CardBits b;
};
struct CardRec {
    u8 pad0[8];
    union CardEntry e;
};
struct SaveHead {
    u8 pad0[4];
    u8 modeByte;            /* mode:7 | jpFont:1<<7 */
};
extern struct SaveHead gUnk_02011C20;
extern u8 gUnk_02013D86[];
extern const u8 gUnk_081A78A8[];
extern char *strcpy(char *, const char *);
extern const u16 gUnk_08622AB4[];
extern void sub_08077498(u16 id);
extern void sub_08075278(void *, u32);
extern void sub_08077A74(u32);
extern void sub_08077AB0(u32);
extern void sub_080770BC(u16 v);
extern void sub_080770DC(void);
extern void sub_0807701C(void);
extern u32 sub_08076FC4(const u8 *a, const u8 *b, u8 n);
void sub_08076144(u16 idx, u16 scale) {
    u16 *p = (u16 *)gUnk_03004470;
    p += (u32)idx << 4;
    p[3] = scale;
    p[7] = 0;
    p[11] = 0;
    p[15] = scale;
}

/* Affine matrix `idx` = rotation by `angle` (128 steps per turn) and scale (8.8). */
void sub_08076160(u16 idx, u16 scale, u16 angle) {
    struct OamEntry *o = gUnk_03004470;
    s32 s = gUnk_081A77A8[angle & 0x7F];
    s32 c = gUnk_081A77A8[(angle + 0x20) & 0x7F];
    s32 ns = gUnk_081A77A8[(angle + 0x40) & 0x7F];
    o += (u32)idx << 2;
    s *= scale;
    c *= scale;
    ns *= scale;
    s >>= 8;
    c >>= 8;
    ns >>= 8;
    o[0].affine = c;
    o[1].affine = s;
    o[2].affine = ns;
    o[3].affine = c;
}

/* Affine matrix `idx` = scale with a shear (PB = shear, PC = -shear). */
void sub_080761CC(u16 idx, u16 scale, u16 shear) {
    struct OamEntry *o = gUnk_03004470;
    s16 t;
    o += (u32)idx << 2;
    o[0].affine = scale;
    o[1].affine = shear;
    t = shear;
    o[2].affine = -t;
    o[3].affine = scale;
}

/* Append one OAM entry (AddSprite): yx = y << 16 | x, shape = attr0/attr1 high bits, attr2 = tile/palette. */
void sub_080761F0(u32 yx, u16 shape, u16 attr2) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gUnk_03000040;
    cnt = &m->oamCount;
    if (*cnt != 0x80) {
        u32 off = *cnt << 3;
        struct OamEntry *arr = m->oam;
        struct OamEntry *e = (struct OamEntry *)((u8 *)arr + off);
        e->attr0 = a0 | (y & 0xFF);
        e->attr1 = (x & 0x1FF) | a1;
        e->attr2 = attr2;
        (*cnt)++;
    }
}

/* Same with attr0 |= 0x400 (semi-transparent). */
void sub_0807625C(u32 yx, u16 shape, u16 attr2) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gUnk_03000040;
    cnt = &m->oamCount;
    if (*cnt != 0x80) {
        u32 off = *cnt << 3;
        struct OamEntry *arr = m->oam;
        struct OamEntry *e = (struct OamEntry *)((u8 *)arr + off);
        e->attr0 = a0 | (y & 0xFF) | 0x400;
        e->attr1 = (x & 0x1FF) | a1;
        e->attr2 = attr2;
        (*cnt)++;
    }
}

/* Same with 256-colour mode (attr0 |= 0x2000) and tile index doubled. */
void sub_080762D0(u32 yx, u16 shape, u16 attr2) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gUnk_03000040;
    cnt = &m->oamCount;
    if (*cnt != 0x80) {
        u32 off = *cnt << 3;
        struct OamEntry *arr = m->oam;
        struct OamEntry *e = (struct OamEntry *)((u8 *)arr + off);
        e->attr0 = a0 | (y & 0xFF) | 0x2000;
        e->attr1 = (x & 0x1FF) | a1;
        e->attr2 = attr2 << 1;
        (*cnt)++;
    }
}

/* 256-colour sprite with extra attr1 bits (flip/size). */
void sub_08076348(u32 yx, u16 shape, u16 attr2, u16 extra) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gUnk_03000040;
    cnt = &m->oamCount;
    if (*cnt != 0x80) {
        u32 off = *cnt << 3;
        struct OamEntry *arr = m->oam;
        struct OamEntry *e = (struct OamEntry *)((u8 *)arr + off);
        e->attr0 = a0 | (y & 0xFF) | 0x2000;
        e->attr1 = (x & 0x1FF) | a1 | extra;
        e->attr2 = attr2 << 1;
        (*cnt)++;
    }
}

/* 256-colour semi-transparent sprite (attr0 |= 0x2400). */
void sub_080763D0(u32 yx, u16 shape, u16 attr2) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gUnk_03000040;
    cnt = &m->oamCount;
    if (*cnt != 0x80) {
        u32 off = *cnt << 3;
        struct OamEntry *arr = m->oam;
        struct OamEntry *e = (struct OamEntry *)((u8 *)arr + off);
        e->attr0 = a0 | (y & 0xFF) | 0x2400;
        e->attr1 = (x & 0x1FF) | a1;
        e->attr2 = attr2 << 1;
        (*cnt)++;
    }
}

/* Affine (rotate/scale) sprite, 8bpp+alpha, double-size box: (x, y) is the sprite centre; the switch on the shape/size code subtracts half the sprite size. sa = scale << 16 | angle. */
#if 0 /* NONMATCHING: 22 diff lines. The OAM count is read through the global
       * (m assigned after), where the ROM copies m before the first compare.
       * The scale/angle registers r8/r9 are swapped, and the oam index
       * association differs. */
void sub_08076448(u32 yx, u16 shape, u16 attr2, u32 sa) {
    struct Main *m;
    u16 x = yx;
    u16 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    int scale = sa >> 16;
    u16 angle = sa;
    if (gUnk_03000040.oamCount == 0x80)
        return;
    m = &gUnk_03000040;
    if (m->affineCount == 0x20)
        return;
    switch (shape) {
    case 0x0000:
        x = x - 4;
        y = y - 4;
        break;
    case 0x8000:
        x = x - 4;
        y = y - 8;
        break;
    case 0x8040:
        x = x - 4;
        y = y - 0x10;
        break;
    case 0x4000:
        x = x - 8;
        y = y - 4;
        break;
    case 0x0040:
        x = x - 8;
        y = y - 8;
        break;
    case 0x8080:
        x = x - 8;
        y = y - 0x10;
        break;
    case 0x4040:
        x = x - 0x10;
        y = y - 4;
        break;
    case 0x4080:
        x = x - 0x10;
        y = y - 8;
        break;
    case 0x0080:
        x = x - 0x10;
        y = y - 0x10;
        break;
    case 0x80C0:
        x = x - 0x10;
        y = y - 0x20;
        break;
    case 0x40C0:
        x = x - 0x20;
        y = y - 0x10;
        break;
    case 0x00C0:
        x = x - 0x20;
        y = y - 0x20;
        break;
    }
    {
        u8 *cnt = &m->oamCount;
        struct OamEntry *e = &m->oam[*cnt];
        e->attr0 = a0 | (y & 0xFF) | 0x2700;
        e->attr1 = (x & 0x1FF) | a1 | (m->affineCount << 9);
        e->attr2 = attr2 << 1;
        sub_08076160(m->affineCount, scale, angle);
        (*cnt)++;
        m->affineCount++;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08076144", sub_08076448); /* 0x08076448 size 0x1DC */


/* AddSprite variant with extra attr1 bits (flip / size). */
void sub_08076624(u32 yx, u16 shape, u16 attr2, u16 extra) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gUnk_03000040;
    cnt = &m->oamCount;
    if (*cnt != 0x80) {
        u32 off = *cnt << 3;
        struct OamEntry *arr = m->oam;
        struct OamEntry *e = (struct OamEntry *)((u8 *)arr + off);
        e->attr0 = a0 | (y & 0xFF);
        e->attr1 = (x & 0x1FF) | a1 | extra;
        e->attr2 = attr2;
        (*cnt)++;
    }
}

void sub_080766A4(u16 x, s16 y, u16 shape, u16 attr2) {
    struct Main *m;
    s16 nv;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gUnk_03000040;
    cnt = &m->oamCount;
    if (*cnt != 0x80) {
        u32 off = *cnt << 3;
        struct OamEntry *arr = m->oam;
        struct OamEntry *e = (struct OamEntry *)((u8 *)arr + off);
        e->attr0 = a0 | (y & 0xFF);
        nv = x;
        e->attr1 = (nv & 0x1FF) | a1;
        e->attr2 = attr2;
        (*cnt)++;
    }
}

/* Affine (rotate/scale) sprite, 4bpp, double-size box: (x, y) is the sprite centre; the switch on the shape/size code subtracts half the sprite size. sa = scale << 16 | angle. */
#if 0 /* NONMATCHING: 14 diff lines. The ROM copies m before the first compare
       * and computes the OAM entry as index + (m + oam) after the count load. */
void sub_08076714(u32 yx, u16 shape, u16 attr2, u32 sa) {
    struct Main *m;
    u16 x = yx;
    u16 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u16 scale = sa >> 16;
    u16 angle = sa;
    if (gUnk_03000040.oamCount == 0x80)
        return;
    m = &gUnk_03000040;
    if (m->affineCount == 0x20)
        return;
    switch (shape) {
    case 0x0000:
        x = x - 4;
        y = y - 4;
        break;
    case 0x8000:
        x = x - 4;
        y = y - 8;
        break;
    case 0x8040:
        x = x - 4;
        y = y - 0x10;
        break;
    case 0x4000:
        x = x - 8;
        y = y - 4;
        break;
    case 0x0040:
        x = x - 8;
        y = y - 8;
        break;
    case 0x8080:
        x = x - 8;
        y = y - 0x10;
        break;
    case 0x4040:
        x = x - 0x10;
        y = y - 4;
        break;
    case 0x4080:
        x = x - 0x10;
        y = y - 8;
        break;
    case 0x0080:
        x = x - 0x10;
        y = y - 0x10;
        break;
    case 0x80C0:
        x = x - 0x10;
        y = y - 0x20;
        break;
    case 0x40C0:
        x = x - 0x20;
        y = y - 0x10;
        break;
    case 0x00C0:
        x = x - 0x20;
        y = y - 0x20;
        break;
    }
    {
        u8 *cnt = &m->oamCount;
        struct OamEntry *e = (struct OamEntry *)(*cnt * 8 + (u8 *)m->oam);
        e->attr0 = a0 | (y & 0xFF) | 0x300;
        e->attr1 = (x & 0x1FF) | a1 | (m->affineCount << 9);
        e->attr2 = attr2;
        sub_08076160(m->affineCount, scale, angle);
        (*cnt)++;
        m->affineCount++;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08076144", sub_08076714); /* 0x08076714 size 0x1DC */



void sub_080768F0(void) {
    vu16 zero = 0;
    vu32 *dma = (vu32 *)0x040000D4;
    dma[0] = (u32)&zero;
    dma[1] = 0x05000200;
    dma[2] = 0x81000100;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
    zero = 0;
    dma = (vu32 *)0x040000D4;
    dma[0] = (u32)&zero;
    dma[1] = 0x06010000;
    dma[2] = 0x81000040;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
}

/* Load a sprite animation stream: palette 15, then the tile blocks into OBJ VRAM from tile 1; leaves `cur` at the first frame header. */
void sub_0807695C(u8 *src, struct SprAnim *a) {
    u8 *p;
    u16 n;
    u16 i;
    u8 *dst;
    a->base = src;
    a->cur = src;
    REG_DISPCNT |= 0x40;
    sub_08075294((void *)0x050003E0, src, 0x20);
    p = a->cur;
    n = *(u16 *)(p + 0x20);
    p += 0x22;
    a->count = n;
    dst = (u8 *)0x06010020;
    a->cur = p + n * 4;
    i = 0;
    if (i < n) {
        do {
            u32 len;
            u8 *q = a->cur;
            len = *(u16 *)q;
            a->cur = q + 2;
            len <<= 5;
            sub_08075294(dst, a->cur, len);
            dst += len;
            a->cur += len;
            i++;
        } while (i < a->count);
    }
    {
        u16 v = *(u16 *)a->cur;
        a->cur += 2;
        a->unk8 = v;
        a->unkA = 0;
    }
    sub_080769DC(a);
}

void sub_080769DC(struct SprAnim *a) {
    /* FAKEMATCH: preserve pointer/count scheduling and ROM iterator registers. */
    u32 n;
    register u8 *p __asm__("r0");
    register u32 i __asm__("r4");
    p = a->base;
    n = *(u16 *)(p + 0x20);
    p += 0x22;
    __asm__ __volatile__("" : : "r"(n));
    a->count = n;
    p += n * 4;
    a->cur = p;
    i = 0;
    /* Keep the initialized zero in the iterator register for the unsigned compare. */
    __asm__ __volatile__("" : "+r"(i) : "r"(n));
    if (i < n) {
        u8 *q = p;
        do {
            register u32 next __asm__("r0");
            u32 len = *(u16 *)q;
            q += 2;
            q += len << 5;
            next = i + 1;
            i = (u16)next;
        } while (i < n);
        a->cur = q;
    }
    {
        u16 v = *(u16 *)a->cur;
        a->cur += 2;
        a->unk8 = v;
        a->unkA = 0;
    }
}

#if 0 /* NONMATCHING: same structure as sub_08076BEC; register allocation and constant hoisting differ */
/* Emit the OAM entries of the current animation frame at (x, y) plus per-piece offsets (hypothesis). */
void sub_08076A20(u16 x, u16 y, struct SprAnim *a, u16 flag) {
    struct Main *m;
    u8 *cur = a->cur;
    s16 i;
    u16 n;
    if (a->unkA >= a->unk8) {
        sub_080769DC(a);
        return;
    }
    REG_DISPCNT |= 0x40;
    n = *(u16 *)cur;
    cur += 2;
    i = 0;
    a->pieces = n;
    if (i < n) {
        m = &gUnk_03000040;
        do {
            u8 fmt = *(u16 *)cur;
            s16 dx;
            u16 dy;
            u8 *o;
            u8 *p;
            u16 len;
            s32 k;
            s8 off;
            cur += 2;
            dx = *(u16 *)cur;
            cur += 2;
            dy = *(u16 *)cur;
            cur += 2;
            off = i << 3;
            o[0x4431] &= ~0x20;
            o = (u8 *)m + off;
            *(u16 *)(o + 0x4432) = (*(u16 *)(o + 0x4432) & ~0x1FF) | ((dx + (s16)x) & 0x1FF);
            o[0x4430] = dy + y;
            k = fmt;
            p = a->base + 0x20;
            p += 2 + *(u16 *)p * 4;
            i++;
            do {
                len = *(u16 *)p;
                p += 2;
                p += len << 5;
            } while (--k != -1);
            o = (u8 *)m + off;
            *(u16 *)(o + 0x4434) = (*(u16 *)(o + 0x4434) & ~0x3FF) | ((fmt * len + 1) & 0x3FF);
            o[0x4435] = (o[0x4435] | 0xF0) & ~0xC;
            switch (*(u16 *)(a->base + 0x22 + fmt * 4)) {
            case 0:
                o[0x4433] &= 0x3F;
                break;
            case 0x4000:
                o[0x4433] = (o[0x4433] & 0x3F) | 0x40;
                break;
            case 0x8000:
                o[0x4433] = (o[0x4433] & 0x3F) | 0x80;
                break;
            case 0xC000:
                o[0x4433] |= 0xC0;
                break;
            }
        } while (i < a->pieces);
    }
    if (flag != 0) {
        a->cur = cur;
        a->unkA++;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08076144", sub_08076A20); /* 0x08076A20 size 0x1CC */

#if 0 /* NONMATCHING: same structure; register allocation and constant hoisting differ (agbcc hoists the 0x4433 literal into r9 where the ROM hoists -1) */
/* Emit the OAM entries of the current animation frame at a fixed position (x, y) (hypothesis). */
void sub_08076BEC(u16 x, u16 y, struct SprAnim *a, u16 flag) {
    struct Main *m;
    u8 *cur = a->cur;
    u32 xm;
    s16 i;
    u16 n;
    if (a->unkA >= a->unk8) {
        sub_080769DC(a);
        return;
    }
    REG_DISPCNT |= 0x40;
    cur += 2;
    n = *(u16 *)cur;
    a->pieces = n;
    i = 0;
    if (i < n) {
        m = &gUnk_03000040;
        xm = ((u32)x << 23) >> 23;
        do {
            u16 fmt = *(u16 *)cur;
            u8 *o;
            u8 *p;
            s16 len;
            s32 k;
            u8 off;
            cur += 6;
            off = i << 3;
            o = (u8 *)m + off;
            *(u16 *)(o + 0x4432) = (*(u16 *)(o + 0x4432) & ~0x1FF) | xm;
            o[0x4431] &= ~0x20;
            o[0x4430] = y;
            k = fmt;
            p = a->base + 0x20;
            p += 2 + *(u16 *)p * 4;
            i++;
            do {
                len = *(u16 *)p;
                p += 2;
                p += len << 5;
            } while (--k != -1);
            *(u16 *)(o + 0x4434) = (*(u16 *)(o + 0x4434) & ~0x3FF) | ((fmt * len + 1) & 0x3FF);
            o = (u8 *)m + off;
            o[0x4435] = (o[0x4435] | 0xF0) & ~0xC;
            o[0x4431] &= 0x3F;
            switch (*(u16 *)(a->base + 0x22 + fmt * 4)) {
            case 0:
                o[0x4433] &= 0x3F;
                break;
            case 0x4000:
                o[0x4433] = (o[0x4433] & 0x3F) | 0x40;
                break;
            case 0x8000:
                o[0x4433] = (o[0x4433] & 0x3F) | 0x80;
                break;
            case 0xC000:
                o[0x4433] |= 0xC0;
                break;
            }
        } while (i < a->pieces);
    }
    if (flag != 0) {
        a->cur = cur;
        a->unkA++;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08076144", sub_08076BEC); /* 0x08076BEC size 0x1C0 */

#if 0 /* NONMATCHING: 289 lines; structure follows sub_08076BEC. Target keeps i+1 in a stack slot (frame 0x10 vs 0x0C) and x in r3 */
/* Emit the OAM entries of the current animation frame at the packed position yx, optionally flipped
 * horizontally (hypothesis; a variant of sub_08076BEC). */
void sub_08076DAC(u32 yx, struct SprAnim *a, u16 flag, u16 hflip)
{
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u8 *cur = a->cur;
    int i;
    u16 n;
    u32 xm;
    if (a->unkA >= a->unk8) {
        sub_080769DC(a);
        return;
    }
    REG_DISPCNT |= 0x40;
    n = *(u16 *)cur;
    cur += 2;
    a->pieces = n;
    i = 0;
    if (i < n) {
        m = &gUnk_03000040;
        xm = x & 0x1FF;
        do {
            u16 fmt = *(u16 *)cur;
            u8 *o;
            u8 *p;
            u16 len;
            int k;
            int off;
            cur += 6;
            off = i << 3;
            o = (u8 *)m + off;
            o[0x4431] &= ~0x20;
            *(u16 *)(o + 0x4432) = (*(u16 *)(o + 0x4432) & ~0x1FF) | xm;
            o[0x4430] = y;
            k = fmt;
            p = a->base + 0x20;
            p += 2 + *(u16 *)p * 4;
            i++;
            do {
                len = *(u16 *)p;
                p += 2;
                p += len << 5;
            } while (--k != -1);
            o = (u8 *)m + off;
            *(u16 *)(o + 0x4434) = (*(u16 *)(o + 0x4434) & ~0x3FF) | ((fmt * len + 1) & 0x3FF);
            o[0x4435] = (o[0x4435] | 0xF0) & ~0xC;
            o[0x4431] &= 0x3F;
            o[0x4433] = (o[0x4433] & ~0x10) | ((hflip & 1) << 4);
            switch (*(u16 *)(a->base + 0x22 + fmt * 4)) {
            case 0:
                o[0x4433] &= 0x3F;
                break;
            case 0x4000:
                o[0x4433] = (o[0x4433] & 0x3F) | 0x40;
                break;
            case 0x8000:
                o[0x4433] = (o[0x4433] & 0x3F) | 0x80;
                break;
            case 0xC000:
                o[0x4433] |= 0xC0;
                break;
            }
        } while (i < a->pieces);
    }
    if (flag != 0) {
        a->cur = cur;
        a->unkA++;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08076144", sub_08076DAC); /* 0x08076DAC size 0x1F0 */
/* Random: LCG (MSVC constants) on gMain.rngState, rotated by 16 (written as shifts, not a rotate); returns 15 bits. */
/* The generator returns a zero-extended 15-bit value to word consumers. */
int sub_08076F9C(void) {
    struct Main *m = &gUnk_03000040;
    u32 x = m->rngState * 0x343FD + 0x269EC3;
    u32 t = x << 16;
    x >>= 16;
    x |= t;
    m->rngState = x;
    return (u16)((x << 1) >> 17);
}

/* memcmp-like: returns 1 if the first n bytes differ, else 0. */
u32 sub_08076FC4(const u8 *a, const u8 *b, u8 n) {
    u8 i;
    for (i = 0; i < n; i++) {
        if (*a++ != *b++)
            return 1;
    }
    return 0;
}

u32 sub_08076FF8(void) {
    if (sub_08076FC4(gUnk_081A78A8, gUnk_02013D86, 8) == 0)
        return 1;
    return 0;
}

/* Write the save signature. */
void sub_0807701C(void) {
    strcpy((char *)gUnk_02013D86, (const char *)gUnk_081A78A8);
}

u32 sub_08077034(void) {
    u16 sum;
    u16 *p;
    u16 i;
    u8 *base;
    sum = 0;
    p = (u16 *)&gUnk_02011C20;
    i = 0;
    base = (u8 *)p;
    for (; i <= 0x10B5; p++, i++)
        sum += *p;
    if (*(u16 *)(base + 0x216E) == (u16)(~sum + 1))
        return 1;
    return 0;
}

void sub_08077080(void) {
    u16 sum;
    u16 *p;
    u16 i;
    u8 *base;
    u16 v;
    sum = 0;
    p = (u16 *)&gUnk_02011C20;
    i = 0;
    base = (u8 *)p;
    for (; i <= 0x10B5; p++, i++)
        sum += *p;
    v = ~sum + 1;
    *(u16 *)(base + 0x216E) = v;
}

void sub_080770BC(u16 v) {
    struct SaveHead *s = &gUnk_02011C20;
    u8 t = v & 0x7F;
    s->modeByte = t;
    if (v == 0)
        s->modeByte = t | 0x80;
}

void sub_080770DC(void) {
    sub_080770BC(1);
}

/* Reset the whole save mirror to defaults. */
void sub_080770E8(void) {
    sub_08075278(&gUnk_02011C20, 0x2170);
    sub_08077A74(1);
    sub_08077AB0(1);
    sub_080770DC();
    sub_0807701C();
}

/* Debug "Get all card": give 3 copies of every card id 1..0x334 (skipping ids whose key is in 0x780..0x7CF). */
void sub_08077114(void) {
    s32 id;
    for (id = 1; id <= 0x334; ) {
        u16 k = *(const u16 *)((const u8 *)gUnk_08622AB4 + ((id & 0x7FF) << 1)) - 0x780;
        s32 next = id + 1;
        if (k > 0x4F) {
            u8 *s;
            struct CardRec *r;
            u32 idv = id << 16;
            s = (u8 *)&gUnk_02011C20;
            r = (struct CardRec *)(s + id * 4);
            do {
                sub_08077498(idv >> 16);
            } while (r->e.c.count + r->e.b.n1 + r->e.b.n2 + r->e.b.n3 <= 2);
        }
        id = next;
    }
}
