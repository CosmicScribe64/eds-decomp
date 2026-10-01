#include "global.h"

#include "main.h"

#include "gba.h"

/*
 * Dialogue ("Bustup") scene module: character-name lookup, the Mode-4 scene
 * loaders, text-box state init, the `$`-code text processor and the cursor
 * sparkle. See wiki/functions/code-08000228.md.
 */

/* Character name record (0x08139F64, 28 x 0x84). See [[duelist-table]]. */
struct Duelist {
    u32 id;
    char name[0x40];
    char shortName[0x40];
};

struct SparklePos {
    u8 x;
    u8 y;
    u8 filler2[2];
};

/* Text-box / scene state (0x0201478C = gUnk_02013DE0 + 0x9AC). */
struct TextBox {
    void *bitmap;           /* +0x00 decoded scene bitmap (EWRAM) */
    const u16 *bgPal;       /* +0x04 */
    const u16 *objPal;      /* +0x08 */
    void *boxBitmap;        /* +0x0C decoded dialogue-box bitmap (EWRAM) */
    const void *anim;       /* +0x10 */
    const u8 *text;         /* +0x14 current script text pointer */
    u8 delay;               /* +0x18 frames until the next glyph */
    s8 state;               /* +0x19 0/1 printing (1 = instant), 2 = inserting name,
                               3 = page flip, <0 = stopped (-1 $c, -2 end, -3 box full) */
    u8 filler1A[2];
    u8 col;                 /* +0x1C cursor column (half-width glyph units) */
    u8 row;                 /* +0x1D cursor row */
    u8 filler1E[2];
    u8 page;                /* +0x20 displayed Mode-4 page (DISPCNT bit 4) */
    u8 filler21;
    u16 boxDirty;           /* +0x22 1 = redraw the dialogue box */
    u16 textColor;          /* +0x24 `$rX` colour, default 7 */
    u8 filler26[2];
    u8 sparkleX;            /* +0x28 (0x9D4) sparkle orbit centre */
    u8 filler29;
    u8 sparkleY;            /* +0x2A (0x9D6) */
    u8 filler2B[0x3C - 0x2B];
    u8 sparkleHead;         /* +0x3C (0x9E8) history ring write index */
    u8 filler3D[3];
    struct SparklePos sparkleHist[30]; /* +0x40 (0x9EC) */
    u8 nameIdx;             /* +0xB8 read index into name[] */
    u8 name[0x43];          /* +0xB9 name inserted by $Q/$q/$i */
};

/* Sprite/animation object (stride 0x14) */
struct AnimObj {
    u8 filler0[0xE];
    u8 unkE;                /* +0x0E */
    u8 filler0F[3];
    u8 unk12;               /* +0x12 */
    u8 filler13;
};

/* Script / dialogue state at 0x02013DE0 (fields this unit touches). */
struct ScriptState {
    u8 filler0[0x810];
    struct AnimObj objs[20];        /* +0x810 */
    u8 filler9A0[4];
    u8 objCount:3;                  /* +0x9A4 */
    u8 filler9A5;
    u8 flag9A6;                     /* +0x9A6 `$h`/`$k` flag */
    u8 blinkIndex:5;                /* +0x9A7 index into gUnk_08080AA8 */
    u16 blinkTimer;                 /* +0x9A8 */
    u8 filler9AA[2];
    struct TextBox textBox;         /* +0x9AC */
    u8 unkAA8[0x12E8 - 0xAA8];      /* +0xAA8 initialised by sub_0807A2EC */
    u8 unk12E8;                     /* +0x12E8 */
    u8 unk12E9;                     /* +0x12E9 index into gUnk_08080A48 (sparkle start pos) */
    u8 unk12EA;                     /* +0x12EA */
    u8 speaker;                     /* +0x12EB copy of gMain speaker */
    u8 filler12EC[0x12F9 - 0x12EC];
    u8 unk12F9;                     /* +0x12F9 */
    u8 unk12FA;                     /* +0x12FA */
    u8 filler12FB[0x136C - 0x12FB];
    u16 dialogueIndex;              /* +0x136C copy of gMain dialogueIndex */
    u8 flag136E;                    /* +0x136E copy of gMain +0x4884 bit 3 */
    u8 unk136F;                     /* +0x136F */
    u8 unk1370;                     /* +0x1370 */
    u8 filler1371[3];
    u8 timer1374[4];                /* +0x1374 sub_0807B0C0 timer */
    u8 timer1378[4];                /* +0x1378 sub_0807B0C0 timer */
    u8 flags137C;                   /* +0x137C bit 1: auto-advance */
};

extern struct ScriptState gUnk_02013DE0;
extern struct Duelist gUnk_08139F64[];
extern const u8 gUnk_08087B90[];
extern const u16 gUnk_08080AA8[];

extern u8 gUnk_02014888[];
extern u8 gUnk_02031014[];
extern u8 gUnk_0203A614[];
extern const u16 gUnk_08080AC0[];
extern const u16 *const gUnk_08139F5C[];
extern const u16 gUnk_0874D5B0;
extern const u16 gUnk_0874D5B2;
extern const u8 gUnk_0874D5B4[];
extern const u16 gUnk_0874E104[];
extern const u16 gUnk_0874E304[];

/* LZSS blob: {u16 sizeLo, sizeHi; u8 stream[]} */
#define LZ_DECOMPRESS(blob, dest) \
    sub_0807A1A8((u8 *)(blob) + 4, dest, ((blob)[1] << 16) | (blob)[0])

/* Scene-set descriptor (0x081976A0, 31 x 0x14). See [[graphics-formats]]. */
struct SceneSet {
    const u16 *bitmapLz;
    const u16 *bgPal;
    const u16 *objPal;
    const u16 *objTilesLz;
    const void *anim;
};

u16 *sub_0807A320(u32 a, void *b);
void *sub_0807B6B8();
void sub_0807A1A8(const u8 *src, void *dest, s32 size);
void sub_08077CEC(const void *src, void *dest, u32 n);
u32 sub_08078670(const void *anim, void *objs);
void sub_08075294(void *dest, const void *src, u32 size);
void sub_080008A4(u8 page, void **gfx, u16 size);

/* Returns the full (full != 0) or short name of character `id`. */
char *sub_08000228(u32 id, u16 full)
{
    u32 i;

    for (i = 1; i <= 27; i++) {
        if (gUnk_08139F64[i].id == id) {
            if (full)
                return gUnk_08139F64[i].name;
            return gUnk_08139F64[i].shortName;
        }
    }
    return gUnk_08139F64[0].name;
}

/* Copies `rows` 240-byte rows from `src` into `dest` (row pitch `pitch` bytes). */
void sub_08000270(u8 *src, u32 *dest, u16 width, u16 rows, u16 pitch)
{
    u8 i;

    for (i = 0; i < rows; i++)
        CpuSet(src + i * 240, dest + ((i * pitch) >> 2), width / 2);
}

void sub_080002C0(u16 x, u16 y, u16 attr, u16 n)
{
    u8 i;

    for (i = 0; i < n * 2 + 2; i++) {
        u16 *obj = sub_0807A320(0, (void *)0x02014888);
        *(u32 *)obj = 0x80004000 | attr;
        obj[1] |= y + i * 32;
        obj[2] = (x + i * 4) | 0x2200;
    }
}

/* Draws `value` as `digits` right-aligned 8x8 digit sprites; leading zeros are blank. */
void sub_08000324(u16 value, u16 x, u16 y, u16 digits, u8 pal)
{
    u8 i;
    u16 digit;

    for (i = 0; i < digits; i++) {
        digit = value % 10;
        value = value / 10;
        if (i == 0) {
            sub_0807B6B8(0, gUnk_08080AC0[digit], x + (digits - i - 1) * 8, y, 8, 8, 4, pal, 0, 0, 0, 0, gUnk_02014888);
        } else if (value != 0 || digit != 0) {
            sub_0807B6B8(0, gUnk_08080AC0[digit], x + (digits - (u16)(i + 1)) * 8, y, 8, 8, 4, pal, 0, 0, 0, 0, gUnk_02014888);
        }
    }
}

/* Loads scene set `set` (240x96 bitmap) and dialogue box `box` into both Mode-4 pages. */
void sub_08000420(const struct SceneSet *set, const u8 *text, struct TextBox *tb, void *objs, u8 box)
{
    const u16 *blob = gUnk_08139F5C[box];

    LZ_DECOMPRESS(blob, gUnk_0203A614);
    tb->boxBitmap = gUnk_0203A614;
    CpuSet(gUnk_0203A614, (void *)0x06005A00, 0x1E00);
    CpuSet(gUnk_0203A614, (void *)0x0600FA00, 0x1E00);
    LZ_DECOMPRESS(set->bitmapLz, gUnk_02031014);
    tb->bitmap = gUnk_02031014;
    sub_080008A4(0, &tb->bitmap, 0x5A00);
    sub_080008A4(1, &tb->bitmap, 0x5A00);
    tb->bgPal = set->bgPal;
    tb->objPal = set->objPal;
    if (set->objTilesLz) {
        LZ_DECOMPRESS(set->objTilesLz, gUnk_02031014);
        sub_08077CEC(gUnk_02031014, (void *)0x06014000, 16);
    } else {
        u16 zero = 0;
        CpuSet(&zero, (void *)0x06014000, 0x01002000);
    }
    tb->anim = set->anim;
    tb->text = text;
    if ((tb->bgPal = set->bgPal))
        CpuSet(tb->bgPal + 16, (void *)0x05000020, 0xF8);
    if ((tb->objPal = set->objPal))
        CpuSet(tb->objPal, (void *)0x05000200, 0x100);
    if ((tb->anim = set->anim))
        gUnk_02013DE0.objCount = sub_08078670(tb->anim, objs);
    sub_08075294((void *)0x05000000, gUnk_0874E304, 0x20);
}
/* Like sub_08000420, for a 240x80 scene: bitmap at rows 16-95 under a header strip. */
void sub_08000570(const struct SceneSet *set, const u8 *text, struct TextBox *tb, void *objs, u8 box)
{
    const u16 *blob = gUnk_08139F5C[box];

    LZ_DECOMPRESS(blob, gUnk_0203A614);
    tb->boxBitmap = gUnk_0203A614;
    CpuSet(gUnk_0203A614, (void *)0x06005A00, 0x1E00);
    CpuSet(gUnk_0203A614, (void *)0x0600FA00, 0x1E00);
    /* header strip: LZSS blob 0x0874D5B0, referenced as three separate labels */
    sub_0807A1A8(gUnk_0874D5B4, gUnk_02031014, gUnk_0874D5B0 | (gUnk_0874D5B2 << 16));
    CpuSet(gUnk_02031014, (void *)0x06000000, 0x780);
    CpuSet(gUnk_02031014, (void *)0x0600A000, 0x780);
    LZ_DECOMPRESS(set->bitmapLz, gUnk_02031014);
    tb->bitmap = gUnk_02031014;
    CpuSet(gUnk_02031014, (void *)0x06000F00, 0x2580);
    CpuSet(tb->bitmap, (void *)0x0600AF00, 0x2580);
    if (set->objTilesLz) {
        LZ_DECOMPRESS(set->objTilesLz, gUnk_02031014);
        sub_08077CEC(gUnk_02031014, (void *)0x06014000, 16);
    } else {
        u16 zero = 0;
        CpuSet(&zero, (void *)0x06014000, 0x01002000);
    }
    tb->text = text;
    sub_08075294((void *)0x05000000, gUnk_0874E104, 0x40);
    if ((tb->bgPal = set->bgPal))
        CpuSet(tb->bgPal + 16, (void *)0x05000020, 0xF8);
    if ((tb->objPal = set->objPal))
        CpuSet(tb->objPal, (void *)0x05000200, 0x100);
    if ((tb->anim = set->anim))
        gUnk_02013DE0.objCount = sub_08078670(tb->anim, objs);
    sub_08075294((void *)0x05000000, gUnk_0874E304, 0x20);
}

/* Like sub_08000420, but loads the scene bitmap before the dialogue box. */
void sub_08000708(const struct SceneSet *set, const u8 *text, struct TextBox *tb, void *objs, u8 box)
{
    const u16 *blob;

    LZ_DECOMPRESS(set->bitmapLz, gUnk_02031014);
    tb->bitmap = gUnk_02031014;
    sub_080008A4(0, &tb->bitmap, 0x5A00);
    sub_080008A4(1, &tb->bitmap, 0x5A00);
    blob = gUnk_08139F5C[box];
    LZ_DECOMPRESS(blob, gUnk_0203A614);
    tb->boxBitmap = gUnk_0203A614;
    tb->bgPal = set->bgPal;
    tb->objPal = set->objPal;
    if (set->objTilesLz) {
        LZ_DECOMPRESS(set->objTilesLz, gUnk_02031014);
        sub_08077CEC(gUnk_02031014, (void *)0x06014000, 16);
    } else {
        u16 zero = 0;
        CpuSet(&zero, (void *)0x06014000, 0x01002000);
    }
    tb->anim = set->anim;
    if ((tb->bgPal = set->bgPal))
        CpuSet(tb->bgPal + 16, (void *)0x05000020, 0xF8);
    if ((tb->objPal = set->objPal))
        CpuSet(tb->objPal, (void *)0x05000200, 0x100);
    if ((tb->anim = set->anim))
        gUnk_02013DE0.objCount = sub_08078670(tb->anim, objs);
    sub_08075294((void *)0x05000000, gUnk_0874E304, 0x20);
}

/* Selects the displayed Mode-4 page (DISPCNT bit 4). */
void sub_08000838(struct TextBox *tb)
{
    s32 dispcnt = *(vu16 *)0x04000000 & ~0x10;
    if (tb->page)
        dispcnt |= 0x10;
    *(vu16 *)0x04000000 = dispcnt;
}

/* Redraws the dialogue-box bitmap into the hidden page when flagged. */
void sub_08000854(struct TextBox *tb)
{
    if (tb->boxDirty == 1)
        CpuSet(tb->boxBitmap, (void *)(tb->page == 1 ? 0x06005A00 : 0x0600FA00), 0x1E00);
}

void sub_08000880(u8 bg, void **gfx)
{
    void *src = *gfx;
    void *dest = (void *)0x0600A000;
    if (bg == 0)
        dest = (void *)0x06000000;
    CpuSet(src, dest, 0x4B00);
}

#define PAGE_VRAM(page) ((page) == 0 ? 0x06000000 : 0x0600A000)

/* Copies a decoded scene bitmap of `size` bytes to Mode-4 page `page`, in quarters. */
void sub_080008A4(u8 page, void **gfx, u16 size)
{
    CpuSet(*gfx, (void *)PAGE_VRAM(page), size >> 3);
    CpuSet((u8 *)*gfx + (size >> 2), (void *)((size >> 2) + PAGE_VRAM(page)), size >> 3);
    CpuSet((u8 *)*gfx + (size >> 2) * 2, (void *)((size >> 2) * 2 + PAGE_VRAM(page)), size >> 3);
    CpuSet((u8 *)*gfx + (size >> 2) * 3, (void *)((size >> 2) * 3 + PAGE_VRAM(page)), size >> 3);
}

void sub_0800093C(struct TextBox *tb)
{
    tb->boxDirty = 1;
}

/* Text-box state init. */
void sub_08000944(struct TextBox *tb)
{
    tb->delay = 2;
    tb->state = 0;
    if (gUnk_02013DE0.flags137C & 2)
        tb->state = 1;
    gUnk_02013DE0.flag9A6 = 0;
    tb->col = gUnk_08087B90[0];
    tb->row = gUnk_08087B90[1];
    tb->textColor = 7;
}

void sub_0800098C(struct AnimObj *obj)
{
    obj->unkE = 1;
}

/* Steps the blink timer; on expiry loads the next duration from the
 * zero-terminated table gUnk_08080AA8 (wrapping) and flags `obj`. */
void sub_08000994(struct AnimObj *obj)
{
    u16 t = --gUnk_02013DE0.blinkTimer;

    if (t == 0xFFFF) {
        if ((gUnk_02013DE0.blinkTimer = gUnk_08080AA8[gUnk_02013DE0.blinkIndex++]) == 0) {
            gUnk_02013DE0.blinkIndex = 0;
            gUnk_02013DE0.blinkTimer = gUnk_08080AA8[gUnk_02013DE0.blinkIndex++];
        }
        obj->unkE = 1;
    }
}

void sub_08000A28(void)
{
    u8 i;

    gUnk_02013DE0.blinkTimer = 0;
    gUnk_02013DE0.blinkIndex = 0;
    for (i = 0; i < 20; i++)
        gUnk_02013DE0.objs[i].unk12 |= 0xFF;
}

void sub_08000A78(void)
{
    u8 i;

    gUnk_02013DE0.blinkTimer = 0;
    gUnk_02013DE0.blinkIndex = 0;
    for (i = 0; i < 20; i++)
        gUnk_02013DE0.objs[i].unk12 |= 0xFF;
}

s32 sub_0807B4D0(s16 a, s16 b);
extern const s16 gUnk_08087BA4[];
extern const u8 gUnk_08080A5C[];

#define SPARKLE_BASE_X gUnk_02013DE0.textBox.sparkleX
#define SPARKLE_BASE_Y gUnk_02013DE0.textBox.sparkleY
#define SPARKLE_HEAD gUnk_02013DE0.textBox.sparkleHead
#define SPARKLE_HIST gUnk_02013DE0.textBox.sparkleHist

/* Cursor sparkle: 6 sprites orbiting (sparkleX, sparkleY); older ones replay
 * positions from a 30-entry history ring, 3 frames apart. */
#if 0 /* NONMATCHING: r7 and r8 are swapped (base vs &sparkleHead), and the no-op `add r4,#0`
       * at the fallback join is missing. The attr/gap locals reproduce the target's
       * `mov r3,#0; orr` and the `mul` by 3. */
void sub_08000AC8(void)
{
    u8 i;
    s8 x, y;
    u8 attr = 0; u8 gap = 3;

    for (i = 5; i != 0xFF; i--) {
        x = SPARKLE_BASE_X + (sub_0807B4D0(gUnk_08087BA4[(gUnk_02013DE0.blinkTimer + i * 8) * 2 % 256 + 64], 0x400) >> 8);
        y = SPARKLE_BASE_Y + (sub_0807B4D0(gUnk_08087BA4[(gUnk_02013DE0.blinkTimer + i * 8) * 4 % 256], 0x300) >> 8);
        if (i != 5) {
            u16 d = (i + 1) * gap - 30;
            s32 idx = (SPARKLE_HEAD - d) % 30;
            if (SPARKLE_HIST[idx].x != 0xFF) {
                u32 *obj = sub_0807B6B8(1, 4, SPARKLE_HIST[idx].x, SPARKLE_HIST[idx].y | attr, 0x20, 0x10, 4, gUnk_08080A5C[i], 0, 0, 0, gUnk_02014888);
                *obj |= 0x400;
                continue;
            }
        }
        sub_0807B6B8(0, 4, x, y, 0x20, 0x10, 4, gUnk_08080A5C[5 - i]);
        SPARKLE_HIST[SPARKLE_HEAD].x = x;
        SPARKLE_HIST[SPARKLE_HEAD].y = y;
        SPARKLE_HEAD = ++SPARKLE_HEAD % 30;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08000228", sub_08000AC8); /* 0x08000AC8 size 0x18C */

/* gMain (0x03000040): canonical layout in include/main.h. */
#define gMain gUnk_03000040

struct SaveData {
    u32 unk0;
    u8 flags4;              /* +0x04 bit 7: Shift-JIS text mode */
};
extern struct SaveData gUnk_02011C20;
#define gSaveData gUnk_02011C20

extern const u8 gUnk_08080A20[];
extern const u8 gUnk_08080A30[];
extern const u16 gUnk_08623DF4[];
extern const u8 gUnk_0822C720[][0x40]; /* names, 0x40 bytes each */
extern struct AnimObj gUnk_02014EB4;

u8 sub_08079ED4(const u8 *s);
u32 sub_08079F10(const u8 *s, u32 digits);
u32 sub_08079F40(const u8 *s, u8 col, u32 maxCol);
void sub_08079E50(const u8 *glyph, u32 x, u32 y, u32 vram, u32 color, u32 a5, u32 a6, u32 a7, u32 a8);
void sub_080752D0(u8 *dst, const u8 *src);
void sub_0801A7DC(const u8 *fmt, u32 arg);
void sub_0801A7E8(void);

#define IS_SJIS() (gSaveData.flags4 & 0x80)

/* Per-frame text processor: prints one glyph of tb->text (or of an inserted
 * name), handling tabs/newlines and `$` control codes. */
void sub_08000C54(struct TextBox *tb)
{
    const u8 *p = tb->text;
    s32 state;
    /* FAKEMATCH: retain the initialized control-code byte in r1. */
    register u8 c asm("r1");
    u8 nameGlyph[4];
    u8 glyph[4];
    /* Assigned in both glyph paths before any use. */
    register u8 *glyphPtr asm("r1");

    if (tb->state < 0)
        return;
    state = tb->state;

    switch (state) {
    default:
        if (*p == 0)
            goto finished;
        break;
    case 2: {
        u8 *idx = &tb->nameIdx;
        u8 i = *idx;
        u8 *name = tb->name;
        u8 sjis;

        if (name[i] != 0) {
            if (sub_08079F40(&tb->name[tb->nameIdx], tb->col, 30) == 0) {
                tb->col = gUnk_08087B90[0];
                tb->row++;
            }
            tb->delay = state;
            sjis = IS_SJIS();
            if (sjis) {
                nameGlyph[0] = name[(*idx)++];
                nameGlyph[1] = name[(*idx)++];
                nameGlyph[2] = 0;
            } else {
                nameGlyph[0] = name[(*idx)++];
                nameGlyph[1] = sjis;
            }
            switch (tb->page) {
            case 0:
            case 1:
                sub_08079E50(nameGlyph, tb->col * 6 + 30, tb->row * 13 + 102, PAGE_VRAM(tb->page), tb->textColor, 14, 12, 240, 8);
                break;
            }
            if (IS_SJIS())
                tb->col += 2;
            else
                tb->col += 1;
            sub_0800098C(&gUnk_02014EB4);
        } else {
            tb->state = 0;
            if (gUnk_02013DE0.flags137C & state)
                tb->state = 1;
            tb->textColor = 7;
        }
        return;
    }
    case 3:
        tb->state = 1;
        tb->page ^= 1;
        tb->col = gUnk_08087B90[0];
        tb->row = gUnk_08087B90[1];
        sub_0800093C(tb);
        return;
    }

    while (*p == '\t' || *p == '\n')
        tb->text = ++p;

    if (*p == '$') {
        p++;
        c = *p;
        if (c != '$') {
        switch (c) {
        case 'h': {
            /* FAKEMATCH: preserve the initialized base/offset operands. */
            u8 *base = (u8 *)&gUnk_02013DE0;
            register u32 offset asm("r2") = 0x9A6;
            asm("" : "+r"(base), "+r"(offset));
            base[offset] = 0;
            p++;
            break;
        }
        case 'k':
            gUnk_02013DE0.flag9A6 = 1;
            p++;
            break;
        case 'n':
            tb->col = gUnk_08087B90[0];
            tb->row++;
            p++;
            break;
        case 'p':
            tb->page ^= 1;
            tb->delay = 2;
            tb->col = gUnk_08087B90[0];
            tb->row = gUnk_08087B90[1];
            sub_0800093C(tb);
            p++;
            break;
        case 'c':
            if (!(gUnk_02013DE0.flags137C & 2))
                tb->state = -1;
            p++;
            break;
        case 'r':
            p++;
            switch (*p) {
            case 'a': tb->textColor = 10; break;
            case 'b': tb->textColor = 11; break;
            case 'c': tb->textColor = 12; break;
            case 'd': tb->textColor = 13; break;
            case 'e': tb->textColor = 14; break;
            case 'f': tb->textColor = 15; break;
            default: tb->textColor = *p - '0'; break;
            }
            p++;
            break;
        case 'b':
            p++;
            gUnk_02013DE0.speaker = sub_08079ED4(p);
            sub_0801A7DC(gUnk_08080A20, gUnk_02013DE0.speaker);
            sub_0801A7E8();
            if (gUnk_02013DE0.speaker > 39)
                gUnk_02013DE0.speaker = 0;
            p += 2;
            gMain.seqIndex1 += 4;
            break;
        case 'i': {
            u16 id;
            /* Narrow only when forming the 64-byte name-table offset. */
            u32 n;
            u8 *dst;
            p++;
            dst = tb->name;
            id = sub_08079F10(p, 4);
            if (id == 0xFFFF)
                n = 0;
            else if (id < 2000)
                n = ((const u16 *)0x08623DF4)[id & 0x7FF];
            else
                n = ((const u16 *)0x08623DF4)[(id - 2000) & 0x7FF] + 1;
            sub_080752D0(dst, (const u8 *)(0x0822C720 + ((u32)(u16)n << 6)));
            p += 4;
            tb->textColor = 3;
            tb->state = 2;
            tb->nameIdx = 0;
            break;
        }
        case 'q': {
            u8 *dst;
            p++;
            dst = tb->name;
            sub_080752D0(dst, sub_08000228(sub_08079F10(p, 2), 1));
            p += 2;
            tb->textColor = 5;
            tb->state = 2;
            tb->nameIdx = 0;
            break;
        }
        case 'Q': {
            u8 *dst;
            p++;
            dst = tb->name;
            sub_080752D0(dst, sub_08000228(sub_08079F10(p, 2), 0));
            p += 2;
            tb->textColor = 4;
            tb->state = 2;
            tb->nameIdx = 0;
            break;
        }
        }
        }
        tb->text = p;
        return;
    }

    if (tb->state == 1)
        tb->delay = 0;
    if (--tb->delay != 0xFF)
        return;
    if (sub_08079F40(p, tb->col, 30) == 0) {
        tb->col = gUnk_08087B90[0];
        tb->row++;
        if (tb->row > 3) {
            if (gUnk_02013DE0.flags137C & 2) {
                sub_0801A7DC(gUnk_08080A30, tb->row);
                gUnk_02013DE0.flags137C |= 1;
            } else {
                tb->state = -3;
            }
            return;
        }
    }
    if (tb->col == gUnk_08087B90[0] && (*p == ' ' || *p == '.')) {
        p++;
        tb->text++;
    }
    {
    u8 sjis = IS_SJIS();
    if (sjis) {
        glyphPtr = glyph;
        {
            /* Read the first byte before resetting the terminator value. */
            u8 first = p[0];
            sjis = 0;
            glyphPtr[0] = first;
        }
        glyphPtr[1] = p[1];
        glyphPtr[2] = sjis;
        tb->text += 2;
    } else {
        glyphPtr = glyph;
        glyphPtr[0] = p[0];
        glyphPtr[1] = sjis;
        tb->text += 1;
    }
    }
    p = glyphPtr;
    tb->delay = 2;
    switch (tb->page) {
    case 0:
    case 1:
        sub_08079E50(p, tb->col * 6 + 30, tb->row * 13 + 102, PAGE_VRAM(tb->page), tb->textColor, 14, 12, 240, 8);
        break;
    }
    if (IS_SJIS())
        tb->col += 2;
    else
        tb->col += 1;
    sub_0800098C(&gUnk_02014EB4);
    return;

finished:
    tb->state = -2;
    if (gUnk_02013DE0.flags137C & 2)
        gUnk_02013DE0.flags137C |= 1;
}

struct SparkleStart {
    u8 x;
    u8 y;
    u8 filler2[2];
};
extern const struct SparkleStart gUnk_08080A48[];

void sub_08075278(void *dst, u32 size); /* MemClear16 */
void sub_0807A2EC(void *p);
void sub_0807A398(s16 x0, s16 y0, s16 x1, s16 y1, void *line);
void sub_0807B0C0(void *timer);

/* Dialogue scene init: clears the script state, resets BG scroll/affine registers
 * and the sparkle trail, and copies the speaker/dialogue index from gMain. */
void sub_080011F0(void)
{
    u8 i;

    sub_08075278(&gUnk_02013DE0, sizeof(struct ScriptState));
    gMain.vblankFlags = 1;
    *(vu16 *)0x04000000 &= 0xE0FF;
    *(vu16 *)0x04000016 = 0;
    *(vu16 *)0x04000014 = 0;
    *(vu16 *)0x0400001A = 0;
    *(vu16 *)0x04000018 = 0;
    *(vu16 *)0x0400001E = 0;
    *(vu16 *)0x0400001C = 0;
    *(vu16 *)0x04000028 = 0;
    *(vu16 *)0x0400002A = 0;
    *(vu16 *)0x0400002C = 0;
    *(vu16 *)0x0400002E = 0;
    gUnk_02013DE0.textBox.page = 0;
    gUnk_02013DE0.unk12E9 = 0;
    gUnk_02013DE0.unk12EA = 0;
    gUnk_02013DE0.unk12E8 = 0;
    gUnk_02013DE0.dialogueIndex = 0;
    sub_0807A2EC(gUnk_02013DE0.unkAA8);
    gUnk_02013DE0.textBox.sparkleHead = 0xFF;
    for (i = 0; i < 30; i++)
        gUnk_02013DE0.textBox.sparkleHist[i].x |= 0xFF;
    sub_0807A398(gUnk_08080A48[gUnk_02013DE0.unk12E9].x, gUnk_08080A48[gUnk_02013DE0.unk12E9].y,
                 gUnk_08080A48[gUnk_02013DE0.unk12E9].x, gUnk_08080A48[gUnk_02013DE0.unk12E9].y, &gUnk_02013DE0.textBox.sparkleX);
    gUnk_02013DE0.unk12FA = 5;
    gUnk_02013DE0.unk12F9 = 0;
    {
        u8 *flag = &gUnk_02013DE0.flag136E; /* take the address first; this order is needed to match */
        *flag = gMain.flag4884_3;
    }
    gUnk_02013DE0.speaker = gMain.speaker;
    gUnk_02013DE0.dialogueIndex = gMain.dialogueIndex;
    gUnk_02013DE0.unk136F = 0;
    gUnk_02013DE0.unk1370 = 0;
    sub_0807B0C0(gUnk_02013DE0.timer1374);
    sub_0807B0C0(gUnk_02013DE0.timer1378);
}
