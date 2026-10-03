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
extern struct Main gMain;
extern struct OamEntry gMain_oamBuffer[];
extern const s16 gSineTable128[];

/* Animated OBJ graphics stream state (hypothesis): header = 0x20 byte palette, u16 count, count*4 bytes of table, then count * {u16 tiles; tiles*32 bytes}. */
struct SprAnim {
    u8 *base;               /* +0 */
    u8 *cur;                /* +4 */
    u16 unk8;               /* +8 */
    u16 unkA;               /* +A */
    u16 count;              /* +C */
    u16 pieces;             /* +E */
};
extern void MemCopy16(void *dst, const void *src, u32 size);
extern void SetOamAffineRotScale(u16 idx, u16 scale, u16 angle);
extern void SprAnimRewind(struct SprAnim *a);

/* Save mirror at 0x02011C20 (see collection). */
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
extern struct SaveHead gSaveData;
extern u8 gSaveDataSignature[];
extern const u8 gSaveSignature[];
extern char *strcpy(char *, const char *);
extern const u16 gCardIdToNumber[];
extern void AddCardToTrunk(u16 id);
extern void MemClear16(void *, u32);
extern void SetSeEnabled(u32);
extern void SetBgmEnabled(u32);
extern void SetTextMode(u16 v);
extern void SetTextModeLatin(void);
extern void WriteSaveSignature(void);
extern u32 MemDiffers(const u8 *a, const u8 *b, u8 n);
void SetOamAffineScale(u16 idx, u16 scale) {
    u16 *p = (u16 *)gMain_oamBuffer;
    p += (u32)idx << 4;
    p[3] = scale;
    p[7] = 0;
    p[11] = 0;
    p[15] = scale;
}

/* Affine matrix `idx` = rotation by `angle` (128 steps per turn) and scale (8.8). */
void SetOamAffineRotScale(u16 idx, u16 scale, u16 angle) {
    struct OamEntry *o = gMain_oamBuffer;
    s32 s = gSineTable128[angle & 0x7F];
    s32 c = gSineTable128[(angle + 0x20) & 0x7F];
    s32 ns = gSineTable128[(angle + 0x40) & 0x7F];
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
void SetOamAffineShear(u16 idx, u16 scale, u16 shear) {
    struct OamEntry *o = gMain_oamBuffer;
    s16 t;
    o += (u32)idx << 2;
    o[0].affine = scale;
    o[1].affine = shear;
    t = shear;
    o[2].affine = -t;
    o[3].affine = scale;
}

/* Append one OAM entry (AddSprite): yx = y << 16 | x, shape = attr0/attr1 high bits, attr2 = tile/palette. */
void AddSprite(u32 yx, u16 shape, u16 attr2) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gMain;
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
void AddSpriteAlpha(u32 yx, u16 shape, u16 attr2) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gMain;
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
void AddSprite8bpp(u32 yx, u16 shape, u16 attr2) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gMain;
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
void AddSprite8bppFlip(u32 yx, u16 shape, u16 attr2, u16 extra) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gMain;
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
void AddSprite8bppAlpha(u32 yx, u16 shape, u16 attr2) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gMain;
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
void AddAffineSprite8bppAlpha(u32 yx, u16 shape, u16 attr2, u32 sa) {
    u16 x = yx;
    u16 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    /* u32 (not u16) scale: changes the global-alloc priorities so scale takes r8. */
    u32 scale = sa >> 16;
    u16 angle = sa;
    /* gMain is referenced directly (no local pointer): GCSE then gives the
     * ROM's copy of the base address before the first compare. */
    if (gMain.oamCount == 0x80)
        return;
    if (gMain.affineCount == 0x20)
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
        struct OamEntry *e = &gMain.oam[gMain.oamCount];
        e->attr0 = a0 | (y & 0xFF) | 0x2700;
        e->attr1 = (x & 0x1FF) | a1 | (gMain.affineCount << 9);
        e->attr2 = attr2 << 1;
        SetOamAffineRotScale(gMain.affineCount, scale, angle);
        gMain.oamCount++;
        gMain.affineCount++;
    }
}


/* AddSprite variant with extra attr1 bits (flip / size). */
void AddSpriteFlip(u32 yx, u16 shape, u16 attr2, u16 extra) {
    struct Main *m;
    u16 x = yx;
    u32 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gMain;
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

void AddSpriteXY(u16 x, s16 y, u16 shape, u16 attr2) {
    struct Main *m;
    s16 nv;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    u8 *cnt;
    m = &gMain;
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
void AddAffineSprite(u32 yx, u16 shape, u16 attr2, u32 sa) {
    u16 x = yx;
    u16 y = yx >> 16;
    u32 a0 = shape & 0xFF00;
    u16 a1 = (shape << 8) & ~0x1FF;
    /* angle is declared before scale but assigned after it: the two tie in
     * global-alloc priority, so the lower pseudo (angle) takes r8 as in the ROM. */
    u16 angle, scale;
    scale = sa >> 16;
    angle = sa;
    /* gMain is referenced directly (no local pointer): GCSE then gives the
     * ROM's copy of the base address before the first compare. */
    if (gMain.oamCount == 0x80)
        return;
    if (gMain.affineCount == 0x20)
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
        struct OamEntry *e = &gMain.oam[gMain.oamCount];
        e->attr0 = a0 | (y & 0xFF) | 0x300;
        e->attr1 = (x & 0x1FF) | a1 | (gMain.affineCount << 9);
        e->attr2 = attr2;
        SetOamAffineRotScale(gMain.affineCount, scale, angle);
        gMain.oamCount++;
        gMain.affineCount++;
    }
}



void ClearObjPalettesAndFirstTiles(void) {
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
void SprAnimLoad(u8 *src, struct SprAnim *a) {
    u8 *p;
    u16 n;
    u16 i;
    u8 *dst;
    a->base = src;
    a->cur = src;
    REG_DISPCNT |= 0x40;
    MemCopy16((void *)0x050003E0, src, 0x20);
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
            MemCopy16(dst, a->cur, len);
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
    SprAnimRewind(a);
}

void SprAnimRewind(struct SprAnim *a) {
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

struct OamBitsA20 {
    u32 y:8;
    u32 affineMode:2;
    u32 objMode:2;
    u32 mosaic:1;
    u32 bpp:1;
    u32 shape:2;
    u32 x:9;
    u32 matrixNum:5;
    u32 size:2;
    u16 tileNum:10;
    u16 priority:2;
    u16 paletteNum:4;
    u16 affineParam;
};
struct MainA20 {
    u32 rngState;
    u8 pad0[0x4430 - 4];
    struct OamBitsA20 oam[128];
};
#define gMainA20 (*(struct MainA20 *)&gMain)
static inline u16 A20_Read16(u8 **pp) {
    u16 v = *(u16 *)*pp;
    *pp += 2;
    return v;
}
/* Set OAM entry i's size bits from the frame table entry base[0x22 + fmt*4] (0/0x4000/0x8000/0xC000).
 * Being an inline lets integrate.c fold the 0x4433 offsets into the adds, so loop.c does not hoist them. */
static inline void A20_SetSize(int i, struct SprAnim *a, u16 fmt) {
    u8 *p = a->base;
    u16 sz, t;
    p += 0x22;
    p += fmt * 4;
    t = *(u16 *)p;
    /* FAKEMATCH: keeps the ROM's `ldrh r0; adds r1, r0, #0` copy, with every compare on the copy */
    asm("" : "+r"(t));
    sz = t;
    asm("" : "+r"(sz));
    switch (sz) {
    case 0:
        gMainA20.oam[i].size = 0;
        break;
    case 0x4000:
        gMainA20.oam[i].size = 1;
        break;
    case 0x8000:
        gMainA20.oam[i].size = 2;
        break;
    case 0xC000:
        gMainA20.oam[i].size = 3;
        break;
    }
}
/* Emit the OAM entries of the current animation frame at (x, y) plus per-piece offsets (hypothesis). */
void SprAnimDrawFrame(u16 x, u16 y, struct SprAnim *a, u16 flag) {
    u8 *cur = a->cur;
    int i, next;
    if (a->unkA >= a->unk8) {
        SprAnimRewind(a);
        return;
    }
    REG_DISPCNT |= 0x40;
    {
        u16 n = A20_Read16(&cur);
        a->pieces = n;
    }
    for (i = 0; i < a->pieces; i = next) {
        u16 fmt = A20_Read16(&cur);
        u16 dx = A20_Read16(&cur);
        u16 dy = A20_Read16(&cur);
        u8 *p;
        u16 len;
        u16 cnt;
        int k;
        gMainA20.oam[i].bpp = 0;
        gMainA20.oam[i].x = (s16)dx + (s16)x;
        gMainA20.oam[i].y = dy + y;
        k = fmt;
        p = a->base;
        p += 0x20;
        cnt = A20_Read16(&p);
        p += cnt * 4;
        next = i + 1;
        do {
            len = A20_Read16(&p);
            p += len << 5;
        } while (--k != -1);
        gMainA20.oam[i].tileNum = fmt * len + 1;
        gMainA20.oam[i].paletteNum = 15;
        gMainA20.oam[i].priority = 0;
        A20_SetSize(i, a, fmt);
    }
    if (flag != 0) {
        a->cur = cur;
        a->unkA++;
    }
}

struct OamBitsBEC {
    u32 y:8;
    u32 affineMode:2;
    u32 objMode:2;
    u32 mosaic:1;
    u32 bpp:1;
    u32 shape:2;
    u32 x:9;
    u32 matrixNum:5;
    u32 size:2;
    u16 tileNum:10;
    u16 priority:2;
    u16 paletteNum:4;
    u16 affineParam;
};
struct MainBEC {
    u32 rngState;
    u8 pad0[0x4430 - 4];
    struct OamBitsBEC oam[128];
};
#define gMainBEC (*(struct MainBEC *)&gMain)
static inline u16 BEC_Read16(u8 **pp) {
    u16 v = *(u16 *)*pp;
    *pp += 2;
    return v;
}
/* Emit the OAM entries of the current animation frame at a fixed position (x, y) (hypothesis). */
static inline void BEC_SetSize(int i, struct SprAnim *a, u16 fmt) {
    u8 *p = a->base;
    u16 sz, t;
    p += 0x22;
    p += fmt * 4;
    t = *(u16 *)p;
    /* FAKEMATCH: keeps the ROM's `ldrh r0; adds r1, r0, #0` copy, with every compare on the copy */
    asm("" : "+r"(t));
    sz = t;
    asm("" : "+r"(sz));
    switch (sz) {
    case 0:
        gMainBEC.oam[i].size = 0;
        break;
    case 0x4000:
        gMainBEC.oam[i].size = 1;
        break;
    case 0x8000:
        gMainBEC.oam[i].size = 2;
        break;
    case 0xC000:
        gMainBEC.oam[i].size = 3;
        break;
    }
}
/* Emit the OAM entries of the current animation frame at a fixed position (x, y) (hypothesis). */
void SprAnimDrawFrameAt(u16 x, u16 y, struct SprAnim *a, u16 flag) {
    u8 *cur = a->cur;
    int i, next;
    if (a->unkA >= a->unk8) {
        SprAnimRewind(a);
        return;
    }
    REG_DISPCNT |= 0x40;
    {
        u16 n = BEC_Read16(&cur);
        a->pieces = n;
    }
    for (i = 0; i < a->pieces; i = next) {
        u16 fmt = BEC_Read16(&cur);
        u8 *p;
        u16 len;
        u16 cnt;
        int k;
        cur += 4;
        gMainBEC.oam[i].bpp = 0;
        gMainBEC.oam[i].x = (s16)x;
        gMainBEC.oam[i].y = y;
        k = fmt;
        p = a->base;
        p += 0x20;
        cnt = BEC_Read16(&p);
        p += cnt * 4;
        next = i + 1;
        do {
            len = BEC_Read16(&p);
            p += len << 5;
        } while (--k != -1);
        gMainBEC.oam[i].tileNum = fmt * len + 1;
        gMainBEC.oam[i].paletteNum = 15;
        gMainBEC.oam[i].priority = 0;
        gMainBEC.oam[i].shape = 0;
        BEC_SetSize(i, a, fmt);
    }
    if (flag != 0) {
        a->cur = cur;
        a->unkA++;
    }
}

struct OamBitsDAC {
    u32 y:8;
    u32 affineMode:2;
    u32 objMode:2;
    u32 mosaic:1;
    u32 bpp:1;
    u32 shape:2;
    u32 x:9;
    u32 matrixNum:3;
    u32 hflip:1;
    u32 vflip:1;
    u32 size:2;
    u16 tileNum:10;
    u16 priority:2;
    u16 paletteNum:4;
    u16 affineParam;
};
struct MainDAC {
    u32 rngState;
    u8 pad0[0x4430 - 4];
    struct OamBitsDAC oam[128];
};
#define gMainDAC (*(struct MainDAC *)&gMain)
static inline u16 DAC_Read16(u8 **pp) {
    u16 v = *(u16 *)*pp;
    *pp += 2;
    return v;
}
static inline void DAC_SetSize(int i, struct SprAnim *a, u16 fmt) {
    u8 *p = a->base;
    u16 sz, t;
    p += 0x22;
    p += fmt * 4;
    t = *(u16 *)p;
    /* FAKEMATCH: keeps the ROM's `ldrh r0; adds r1, r0, #0` copy, with every compare on the copy */
    asm("" : "+r"(t));
    sz = t;
    asm("" : "+r"(sz));
    switch (sz) {
    case 0:
        gMainDAC.oam[i].size = 0;
        break;
    case 0x4000:
        gMainDAC.oam[i].size = 1;
        break;
    case 0x8000:
        gMainDAC.oam[i].size = 2;
        break;
    case 0xC000:
        gMainDAC.oam[i].size = 3;
        break;
    }
}
/* Emit the OAM entries of the current animation frame at a fixed position (x, y) (hypothesis). */
void SprAnimDrawFrameAtFlip(u32 yx, struct SprAnim *a, u16 flag, u16 hflip) {
    u16 x = yx;
    u16 y = yx >> 16;
    u8 *cur = a->cur;
    int i, next;
    if (a->unkA >= a->unk8) {
        SprAnimRewind(a);
        return;
    }
    REG_DISPCNT |= 0x40;
    {
        u16 n = DAC_Read16(&cur);
        a->pieces = n;
    }
    for (i = 0; i < a->pieces; i = next) {
        u16 fmt = DAC_Read16(&cur);
        u8 *p;
        u16 len;
        u16 cnt;
        int k;
        cur += 4;
        gMainDAC.oam[i].bpp = 0;
        gMainDAC.oam[i].x = x;
        gMainDAC.oam[i].y = y;
        k = fmt;
        p = a->base;
        p += 0x20;
        cnt = DAC_Read16(&p);
        p += cnt * 4;
        next = i + 1;
        do {
            len = DAC_Read16(&p);
            p += len << 5;
        } while (--k != -1);
        gMainDAC.oam[i].tileNum = fmt * len + 1;
        gMainDAC.oam[i].paletteNum = 15;
        gMainDAC.oam[i].priority = 0;
        gMainDAC.oam[i].shape = 0;
        gMainDAC.oam[i].hflip = hflip;
        DAC_SetSize(i, a, fmt);
    }
    if (flag != 0) {
        a->cur = cur;
        a->unkA++;
    }
}
/* Random: LCG (MSVC constants) on gMain.rngState, rotated by 16 (written as shifts, not a rotate); returns 15 bits. */
/* The generator returns a zero-extended 15-bit value to word consumers. */
int Random(void) {
    struct Main *m = &gMain;
    u32 x = m->rngState * 0x343FD + 0x269EC3;
    u32 t = x << 16;
    x >>= 16;
    x |= t;
    m->rngState = x;
    return (u16)((x << 1) >> 17);
}

/* memcmp-like: returns 1 if the first n bytes differ, else 0. */
u32 MemDiffers(const u8 *a, const u8 *b, u8 n) {
    u8 i;
    for (i = 0; i < n; i++) {
        if (*a++ != *b++)
            return 1;
    }
    return 0;
}

u32 IsSaveSignatureValid(void) {
    if (MemDiffers(gSaveSignature, gSaveDataSignature, 8) == 0)
        return 1;
    return 0;
}

/* Write the save signature. */
void WriteSaveSignature(void) {
    strcpy((char *)gSaveDataSignature, (const char *)gSaveSignature);
}

u32 IsSaveChecksumValid(void) {
    u16 sum;
    u16 *p;
    u16 i;
    u8 *base;
    sum = 0;
    p = (u16 *)&gSaveData;
    i = 0;
    base = (u8 *)p;
    for (; i <= 0x10B5; p++, i++)
        sum += *p;
    if (*(u16 *)(base + 0x216E) == (u16)(~sum + 1))
        return 1;
    return 0;
}

void UpdateSaveChecksum(void) {
    u16 sum;
    u16 *p;
    u16 i;
    u8 *base;
    u16 v;
    sum = 0;
    p = (u16 *)&gSaveData;
    i = 0;
    base = (u8 *)p;
    for (; i <= 0x10B5; p++, i++)
        sum += *p;
    v = ~sum + 1;
    *(u16 *)(base + 0x216E) = v;
}

void SetTextMode(u16 v) {
    struct SaveHead *s = &gSaveData;
    u8 t = v & 0x7F;
    s->modeByte = t;
    if (v == 0)
        s->modeByte = t | 0x80;
}

void SetTextModeLatin(void) {
    SetTextMode(1);
}

/* Reset the whole save mirror to defaults. */
void InitSaveData(void) {
    MemClear16(&gSaveData, 0x2170);
    SetSeEnabled(1);
    SetBgmEnabled(1);
    SetTextModeLatin();
    WriteSaveSignature();
}

/* Debug "Get all card": give 3 copies of every card id 1..0x334 (skipping ids whose key is in 0x780..0x7CF). */
void DebugGetAllCards(void) {
    s32 id;
    for (id = 1; id <= 0x334; ) {
        u16 k = *(const u16 *)((const u8 *)gCardIdToNumber + ((id & 0x7FF) << 1)) - 0x780;
        s32 next = id + 1;
        if (k > 0x4F) {
            u8 *s;
            struct CardRec *r;
            u32 idv = id << 16;
            s = (u8 *)&gSaveData;
            r = (struct CardRec *)(s + id * 4);
            do {
                AddCardToTrunk(idv >> 16);
            } while (r->e.c.count + r->e.b.n1 + r->e.b.n2 + r->e.b.n3 <= 2);
        }
        id = next;
    }
}
