/*
 * sprite (0x08076144-0x0807717C): OAM sprite/affine emitters, sprite animation streams, Random, and
 * the save signature/checksum helpers (wiki/functions/sprite-c.md).
 *
 * The AddSprite and AddAffineSprite emitters append entries to the OAM shadow buffer (gMain.oamBuffer,
 * viewed here as struct OamEntry[128]); the SetOamAffine* functions write the affine matrices that
 * live in the fourth halfword of every fourth buffer entry. SprAnimLoad / SprAnimRewind /
 * SprAnimDrawFrame* play the ROM sprite animation streams (struct SprAnim in sprite.h) straight into
 * the same buffer, and Random is the game's MSVC-style LCG on gMain.rngState. The tail of the unit
 * is save-mirror code shared with collection.c: the signature and checksum over gSaveData, the
 * text-mode byte, InitSaveData and the debug "get all cards" helper.
 */
#include "global.h"   /* u8/u16/u32/s16/s32, vu16/vu32 */
#include "gba.h"      /* REG_DISPCNT, OBJ_PLTT, OBJ_VRAM0 */
#include "main.h"     /* struct Main gMain (rngState, oamBuffer, oamCount, affineCount) */
#include "util.h"     /* MemCopy16, MemClear16, gSineTable128 */
#include "sprite.h"   /* struct SprAnim, enum SpriteShape, the AddSprite and SprAnim prototypes */
#include "card_data.h" /* CARD_ID_MASK, gCardIdToNumber */
#include "save.h"      /* struct SaveData gSaveData, gSaveDataSignature, AddCardToTrunk, the signature/checksum prototypes */

/* ---- Local views kept for matching ---- */

/* One trunk record (gSaveData + 8 + id*4, struct TrunkEntry in save.h) in the two forms this
 * unit's code was compiled from: the copy count through the low halfword, the three per-deck
 * counters through byte +1. save.h packs the counters as u16 bitfields of the low halfword;
 * the ROM loads the byte, so the byte view stays (the same view as collection.c). */
struct TrunkCount {
    u16 count:10;                   /* bits 0-9: copies in the trunk (max 0x3FF) */
    u16 rest:6;
    u16 unk2;
};
struct TrunkCopies {                /* byte view of +1 of the record */
    u8 unk0;
    u8 pad:2;
    u8 deckCopies:2;                /* bits 2-3: copies in the saved Deck */
    u8 sideCopies:2;                /* bits 4-5: copies in the saved Side Deck */
    u8 fusionCopies:2;              /* bits 6-7: copies in the saved Fusion Deck */
    u16 unk2;
};
union TrunkEntryView {
    struct TrunkCount c;
    struct TrunkCopies b;
};

/* Card record viewed from the save base: the record for `id` sits at base + id*4 + 8. */
struct CardRec {
    u8 pad0[8];
    union TrunkEntryView e;
};

/* Save byte +4 as one whole byte: SetTextMode stores it with a single strb, where save.h's
 * bitfields (language:7, sjisText:1) would compile to read-modify-write sequences. */
struct SaveHead {
    u8 pad0[4];
    u8 flags4;              /* +4: language:7 | sjisText:1<<7 */
};
extern const u8 gSaveSignature[];   /* 0x081A78A8: "DMEX1INT" */
extern char *strcpy(char *, const char *);
extern void SetSeEnabled(u32);
extern void SetBgmEnabled(u32);
extern void SetTextMode(u16 v);
extern void SetTextModeLatin(void);
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
    o[0].affineParam = c;
    o[1].affineParam = s;
    o[2].affineParam = ns;
    o[3].affineParam = c;
}

/* Affine matrix `idx` = scale with a shear (PB = shear, PC = -shear). */
void SetOamAffineShear(u16 idx, u16 scale, u16 shear) {
    struct OamEntry *o = gMain_oamBuffer;
    s16 t;
    o += (u32)idx << 2;
    o[0].affineParam = scale;
    o[1].affineParam = shear;
    t = shear;
    o[2].affineParam = -t;
    o[3].affineParam = scale;
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
        struct OamEntry *arr = (struct OamEntry *)m->oamBuffer;
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
        struct OamEntry *arr = (struct OamEntry *)m->oamBuffer;
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
        struct OamEntry *arr = (struct OamEntry *)m->oamBuffer;
        struct OamEntry *e = (struct OamEntry *)((u8 *)arr + off);
        e->attr0 = a0 | (y & 0xFF) | 0x2000;
        e->attr1 = (x & 0x1FF) | a1;
        e->attr2 = attr2 << 1;
        (*cnt)++;
    }
}

/* 256-colour sprite with extra attr1 bits (flip/size). */
void AddSprite8bppFlip(u32 yx, u16 shape, u16 attr2, u16 flip) {
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
        struct OamEntry *arr = (struct OamEntry *)m->oamBuffer;
        struct OamEntry *e = (struct OamEntry *)((u8 *)arr + off);
        e->attr0 = a0 | (y & 0xFF) | 0x2000;
        e->attr1 = (x & 0x1FF) | a1 | flip;
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
        struct OamEntry *arr = (struct OamEntry *)m->oamBuffer;
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
    case SPRITE_SHAPE_8x8:
        x = x - 4;
        y = y - 4;
        break;
    case SPRITE_SHAPE_8x16:
        x = x - 4;
        y = y - 8;
        break;
    case SPRITE_SHAPE_8x32:
        x = x - 4;
        y = y - 0x10;
        break;
    case SPRITE_SHAPE_16x8:
        x = x - 8;
        y = y - 4;
        break;
    case SPRITE_SHAPE_16x16:
        x = x - 8;
        y = y - 8;
        break;
    case SPRITE_SHAPE_16x32:
        x = x - 8;
        y = y - 0x10;
        break;
    case SPRITE_SHAPE_32x8:
        x = x - 0x10;
        y = y - 4;
        break;
    case SPRITE_SHAPE_32x16:
        x = x - 0x10;
        y = y - 8;
        break;
    case SPRITE_SHAPE_32x32:
        x = x - 0x10;
        y = y - 0x10;
        break;
    case SPRITE_SHAPE_32x64:
        x = x - 0x10;
        y = y - 0x20;
        break;
    case SPRITE_SHAPE_64x32:
        x = x - 0x20;
        y = y - 0x10;
        break;
    case SPRITE_SHAPE_64x64:
        x = x - 0x20;
        y = y - 0x20;
        break;
    }
    {
        struct OamEntry *e = &((struct OamEntry *)gMain.oamBuffer)[gMain.oamCount];
        e->attr0 = a0 | (y & 0xFF) | 0x2700;
        e->attr1 = (x & 0x1FF) | a1 | (gMain.affineCount << 9);
        e->attr2 = attr2 << 1;
        SetOamAffineRotScale(gMain.affineCount, scale, angle);
        gMain.oamCount++;
        gMain.affineCount++;
    }
}


/* AddSprite variant with extra attr1 bits (flip / size). */
void AddSpriteFlip(u32 yx, u16 shape, u16 attr2, u16 flip) {
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
        struct OamEntry *arr = (struct OamEntry *)m->oamBuffer;
        struct OamEntry *e = (struct OamEntry *)((u8 *)arr + off);
        e->attr0 = a0 | (y & 0xFF);
        e->attr1 = (x & 0x1FF) | a1 | flip;
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
        struct OamEntry *arr = (struct OamEntry *)m->oamBuffer;
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
    case SPRITE_SHAPE_8x8:
        x = x - 4;
        y = y - 4;
        break;
    case SPRITE_SHAPE_8x16:
        x = x - 4;
        y = y - 8;
        break;
    case SPRITE_SHAPE_8x32:
        x = x - 4;
        y = y - 0x10;
        break;
    case SPRITE_SHAPE_16x8:
        x = x - 8;
        y = y - 4;
        break;
    case SPRITE_SHAPE_16x16:
        x = x - 8;
        y = y - 8;
        break;
    case SPRITE_SHAPE_16x32:
        x = x - 8;
        y = y - 0x10;
        break;
    case SPRITE_SHAPE_32x8:
        x = x - 0x10;
        y = y - 4;
        break;
    case SPRITE_SHAPE_32x16:
        x = x - 0x10;
        y = y - 8;
        break;
    case SPRITE_SHAPE_32x32:
        x = x - 0x10;
        y = y - 0x10;
        break;
    case SPRITE_SHAPE_32x64:
        x = x - 0x10;
        y = y - 0x20;
        break;
    case SPRITE_SHAPE_64x32:
        x = x - 0x20;
        y = y - 0x10;
        break;
    case SPRITE_SHAPE_64x64:
        x = x - 0x20;
        y = y - 0x20;
        break;
    }
    {
        struct OamEntry *e = &((struct OamEntry *)gMain.oamBuffer)[gMain.oamCount];
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
    dma[1] = OBJ_PLTT;
    dma[2] = 0x81000100;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
    zero = 0;
    dma = (vu32 *)0x040000D4;
    dma[0] = (u32)&zero;
    dma[1] = OBJ_VRAM0;
    dma[2] = 0x81000040;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
}

/* Load a sprite animation stream: palette 15, then the tile blocks into OBJ VRAM from tile 1; leaves `cur` at the first frame header. */
void SprAnimLoad(u8 *src, struct SprAnim *a) {
    const u8 *p;
    u16 n;
    u16 i;
    u8 *dst;
    a->base = src;
    a->cur = src;
    REG_DISPCNT |= 0x40;  /* 1D OBJ mapping */
    MemCopy16((void *)(OBJ_PLTT + 0x1E0), src, 0x20); /* OBJ palette 15 */
    p = a->cur;
    n = *(u16 *)(p + 0x20);
    p += 0x22;
    a->blockCount = n;
    dst = (u8 *)(OBJ_VRAM0 + 0x20); /* OBJ tile 1 */
    a->cur = p + n * 4;
    i = 0;
    if (i < n) {
        do {
            u32 len;
            const u8 *q = a->cur;
            len = *(u16 *)q;
            a->cur = q + 2;
            len <<= 5;
            MemCopy16(dst, a->cur, len);
            dst += len;
            a->cur += len;
            i++;
        } while (i < a->blockCount);
    }
    {
        u16 v = *(u16 *)a->cur;
        a->cur += 2;
        a->frameCount = v;
        a->frameIndex = 0;
    }
    SprAnimRewind(a);
}

void SprAnimRewind(struct SprAnim *a) {
    /* FAKEMATCH: preserve pointer/count scheduling and ROM iterator registers. */
    u32 n;
    register const u8 *p __asm__("r0");
    register u32 i __asm__("r4");
    p = a->base;
    n = *(u16 *)(p + 0x20);
    p += 0x22;
    __asm__ __volatile__("" : : "r"(n));
    a->blockCount = n;
    p += n * 4;
    a->cur = p;
    i = 0;
    /* Keep the initialized zero in the iterator register for the unsigned compare. */
    __asm__ __volatile__("" : "+r"(i) : "r"(n));
    if (i < n) {
        const u8 *q = p;
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
        a->frameCount = v;
        a->frameIndex = 0;
    }
}

/* ---- Local views kept for matching: the SprAnimDrawFrame family's bitfield views of the OAM
 * buffer. Each draw function was compiled from its own bitfield layout of one OAM entry (the DAC
 * view splits out hflip/vflip), so the three near-identical views stay separate; the gMainX macros
 * view gMain through them. ---- */
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
static inline u16 A20_Read16(const u8 **pp) {
    u16 v = *(u16 *)*pp;
    *pp += 2;
    return v;
}
/* Set OAM entry i's size bits from the frame table entry base[0x22 + fmt*4] (0/0x4000/0x8000/0xC000).
 * Being an inline lets integrate.c fold the 0x4433 offsets into the adds, so loop.c does not hoist them. */
static inline void A20_SetSize(int i, struct SprAnim *a, u16 fmt) {
    const u8 *p = a->base;
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
void SprAnimDrawFrame(u16 x, u16 y, struct SprAnim *a, u16 advance) {
    const u8 *cur = a->cur;
    int i, next;
    if (a->frameIndex >= a->frameCount) {
        SprAnimRewind(a);
        return;
    }
    REG_DISPCNT |= 0x40;  /* 1D OBJ mapping */
    {
        u16 n = A20_Read16(&cur);
        a->pieceCount = n;
    }
    for (i = 0; i < a->pieceCount; i = next) {
        u16 fmt = A20_Read16(&cur);
        u16 dx = A20_Read16(&cur);
        u16 dy = A20_Read16(&cur);
        const u8 *p;
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
    if (advance != 0) {
        a->cur = cur;
        a->frameIndex++;
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
static inline u16 BEC_Read16(const u8 **pp) {
    u16 v = *(u16 *)*pp;
    *pp += 2;
    return v;
}
/* Set OAM entry i's size bits from the frame table entry base[0x22 + fmt*4] (0/0x4000/0x8000/0xC000). */
static inline void BEC_SetSize(int i, struct SprAnim *a, u16 fmt) {
    const u8 *p = a->base;
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
void SprAnimDrawFrameAt(u16 x, u16 y, struct SprAnim *a, u16 advance) {
    const u8 *cur = a->cur;
    int i, next;
    if (a->frameIndex >= a->frameCount) {
        SprAnimRewind(a);
        return;
    }
    REG_DISPCNT |= 0x40;  /* 1D OBJ mapping */
    {
        u16 n = BEC_Read16(&cur);
        a->pieceCount = n;
    }
    for (i = 0; i < a->pieceCount; i = next) {
        u16 fmt = BEC_Read16(&cur);
        const u8 *p;
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
    if (advance != 0) {
        a->cur = cur;
        a->frameIndex++;
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
static inline u16 DAC_Read16(const u8 **pp) {
    u16 v = *(u16 *)*pp;
    *pp += 2;
    return v;
}
/* Set OAM entry i's size bits from the frame table entry base[0x22 + fmt*4] (0/0x4000/0x8000/0xC000). */
static inline void DAC_SetSize(int i, struct SprAnim *a, u16 fmt) {
    const u8 *p = a->base;
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
void SprAnimDrawFrameAtFlip(u32 yx, struct SprAnim *a, u16 advance, u16 hflip) {
    u16 x = yx;
    u16 y = yx >> 16;
    const u8 *cur = a->cur;
    int i, next;
    if (a->frameIndex >= a->frameCount) {
        SprAnimRewind(a);
        return;
    }
    REG_DISPCNT |= 0x40;  /* 1D OBJ mapping */
    {
        u16 n = DAC_Read16(&cur);
        a->pieceCount = n;
    }
    for (i = 0; i < a->pieceCount; i = next) {
        u16 fmt = DAC_Read16(&cur);
        const u8 *p;
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
    if (advance != 0) {
        a->cur = cur;
        a->frameIndex++;
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

/* 1 when gSaveData.signature is "DMEX1INT" (no callers in the USA ROM). */
u32 IsSaveSignatureValid(void) {
    if (MemDiffers(gSaveSignature, gSaveDataSignature, 8) == 0)
        return 1;
    return 0;
}

/* Write the save signature. */
void WriteSaveSignature(void) {
    strcpy((char *)gSaveDataSignature, (const char *)gSaveSignature);
}

/* 1 when the halfword at +0x216E (struct SaveData.checksum) equals the negated sum of the
 * first 0x10B6 halfwords of the save mirror. */
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

/* Recompute the save checksum and store it at +0x216E. */
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

/* Set the save's text language (byte +4); v == 0 (Japanese) also sets the Shift-JIS bit. */
void SetTextMode(u16 v) {
    struct SaveHead *s = (struct SaveHead *)&gSaveData;
    u8 t = v & 0x7F;
    s->flags4 = t;
    if (v == 0)
        s->flags4 = t | 0x80;
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
        u16 k = *(const u16 *)((const u8 *)gCardIdToNumber + ((id & CARD_ID_MASK) << 1)) - 0x780;
        s32 next = id + 1;
        if (k > 0x4F) {
            u8 *s;
            struct CardRec *r;
            u32 idv = id << 16;
            s = (u8 *)&gSaveData;
            r = (struct CardRec *)(s + id * 4);
            do {
                AddCardToTrunk(idv >> 16);
            } while (r->e.c.count + r->e.b.deckCopies + r->e.b.sideCopies + r->e.b.fusionCopies <= 2);
        }
        id = next;
    }
}
