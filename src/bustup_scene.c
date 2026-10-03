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

/* Text-box / scene state (0x0201478C = gBustup + 0x9AC). */
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
    u8 blinkIndex:5;                /* +0x9A7 index into gBlinkIntervals */
    u16 blinkTimer;                 /* +0x9A8 */
    u8 filler9AA[2];
    struct TextBox textBox;         /* +0x9AC */
    u8 unkAA8[0x12E8 - 0xAA8];      /* +0xAA8 initialised by OamListClear */
    u8 unk12E8;                     /* +0x12E8 */
    u8 unk12E9;                     /* +0x12E9 index into gBustupSlotPositions (sparkle start pos) */
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
    u8 timer1374[4];                /* +0x1374 Timer_Reset timer */
    u8 timer1378[4];                /* +0x1378 Timer_Reset timer */
    u8 flags137C;                   /* +0x137C bit 1: auto-advance */
};

extern struct ScriptState gBustup;
extern struct Duelist gDuelists[];
extern const u8 gBustupTextHome[];
extern const u16 gBlinkIntervals[];

extern u8 gBustupSprites[];
extern u8 gBustupBitmapBuffer[];
extern u8 gBustupBoxBitmap[];
extern const u16 gBustupDigitTiles[];
extern const u16 *const gDialogueBoxGfx[];
extern const u16 gDialogueHeaderLz;
extern const u16 gDialogueHeaderLzSizeHi;
extern const u8 gDialogueHeaderLzData[];
extern const u16 gDialogueBoxPalette[];
extern const u16 gDialogueTextPalette[];

/* LZSS blob: {u16 sizeLo, sizeHi; u8 stream[]} */
#define LZ_DECOMPRESS(blob, dest) \
    LZSSDecompress((u8 *)(blob) + 4, dest, ((blob)[1] << 16) | (blob)[0])

/* Scene-set descriptor (0x081976A0, 31 x 0x14). See [[graphics-formats]]. */
struct SceneSet {
    const u16 *bitmapLz;
    const u16 *bgPal;
    const u16 *objPal;
    const u16 *objTilesLz;
    const void *anim;
};

u16 *OamListAlloc(u32 a, void *b);
void *OamListAddSprite();
void LZSSDecompress(const u8 *src, void *dest, s32 size);
void CopyTileSheetTo2D(const void *src, void *dest, u32 n);
u32 AnimBlockInit(const void *anim, void *objs);
void MemCopy16(void *dest, const void *src, u32 size);
void CopyBitmapToPage(u8 page, void **gfx, u16 size);

/* Returns the full (full != 0) or short name of character `id`. */
char *GetDuelistName(u32 id, u16 full)
{
    u32 i;

    for (i = 1; i <= 27; i++) {
        if (gDuelists[i].id == id) {
            if (full)
                return gDuelists[i].name;
            return gDuelists[i].shortName;
        }
    }
    return gDuelists[0].name;
}

/* Copies `rows` 240-byte rows from `src` into `dest` (row pitch `pitch` bytes). */
void CopyBitmapRows(u8 *src, u32 *dest, u16 width, u16 rows, u16 pitch)
{
    u8 i;

    for (i = 0; i < rows; i++)
        CpuSet(src + i * 240, dest + ((i * pitch) >> 2), width / 2);
}

void Bustup_DrawLabel(u16 x, u16 y, u16 attr, u16 n)
{
    u8 i;

    for (i = 0; i < n * 2 + 2; i++) {
        u16 *obj = OamListAlloc(0, (void *)0x02014888);
        *(u32 *)obj = 0x80004000 | attr;
        obj[1] |= y + i * 32;
        obj[2] = (x + i * 4) | 0x2200;
    }
}

/* Draws `value` as `digits` right-aligned 8x8 digit sprites; leading zeros are blank. */
void Bustup_DrawNumber(u16 value, u16 x, u16 y, u16 digits, u8 pal)
{
    u8 i;
    u16 digit;

    for (i = 0; i < digits; i++) {
        digit = value % 10;
        value = value / 10;
        if (i == 0) {
            OamListAddSprite(0, gBustupDigitTiles[digit], x + (digits - i - 1) * 8, y, 8, 8, 4, pal, 0, 0, 0, 0, gBustupSprites);
        } else if (value != 0 || digit != 0) {
            OamListAddSprite(0, gBustupDigitTiles[digit], x + (digits - (u16)(i + 1)) * 8, y, 8, 8, 4, pal, 0, 0, 0, 0, gBustupSprites);
        }
    }
}

/* Loads scene set `set` (240x96 bitmap) and dialogue box `box` into both Mode-4 pages. */
void Bustup_LoadSceneSet(const struct SceneSet *set, const u8 *text, struct TextBox *tb, void *objs, u8 box)
{
    const u16 *blob = gDialogueBoxGfx[box];

    LZ_DECOMPRESS(blob, gBustupBoxBitmap);
    tb->boxBitmap = gBustupBoxBitmap;
    CpuSet(gBustupBoxBitmap, (void *)0x06005A00, 0x1E00);
    CpuSet(gBustupBoxBitmap, (void *)0x0600FA00, 0x1E00);
    LZ_DECOMPRESS(set->bitmapLz, gBustupBitmapBuffer);
    tb->bitmap = gBustupBitmapBuffer;
    CopyBitmapToPage(0, &tb->bitmap, 0x5A00);
    CopyBitmapToPage(1, &tb->bitmap, 0x5A00);
    tb->bgPal = set->bgPal;
    tb->objPal = set->objPal;
    if (set->objTilesLz) {
        LZ_DECOMPRESS(set->objTilesLz, gBustupBitmapBuffer);
        CopyTileSheetTo2D(gBustupBitmapBuffer, (void *)0x06014000, 16);
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
        gBustup.objCount = AnimBlockInit(tb->anim, objs);
    MemCopy16((void *)0x05000000, gDialogueTextPalette, 0x20);
}
/* Like Bustup_LoadSceneSet, for a 240x80 scene: bitmap at rows 16-95 under a header strip. */
void Bustup_LoadSceneSetWithHeader(const struct SceneSet *set, const u8 *text, struct TextBox *tb, void *objs, u8 box)
{
    const u16 *blob = gDialogueBoxGfx[box];

    LZ_DECOMPRESS(blob, gBustupBoxBitmap);
    tb->boxBitmap = gBustupBoxBitmap;
    CpuSet(gBustupBoxBitmap, (void *)0x06005A00, 0x1E00);
    CpuSet(gBustupBoxBitmap, (void *)0x0600FA00, 0x1E00);
    /* header strip: LZSS blob 0x0874D5B0, referenced as three separate labels */
    LZSSDecompress(gDialogueHeaderLzData, gBustupBitmapBuffer, gDialogueHeaderLz | (gDialogueHeaderLzSizeHi << 16));
    CpuSet(gBustupBitmapBuffer, (void *)0x06000000, 0x780);
    CpuSet(gBustupBitmapBuffer, (void *)0x0600A000, 0x780);
    LZ_DECOMPRESS(set->bitmapLz, gBustupBitmapBuffer);
    tb->bitmap = gBustupBitmapBuffer;
    CpuSet(gBustupBitmapBuffer, (void *)0x06000F00, 0x2580);
    CpuSet(tb->bitmap, (void *)0x0600AF00, 0x2580);
    if (set->objTilesLz) {
        LZ_DECOMPRESS(set->objTilesLz, gBustupBitmapBuffer);
        CopyTileSheetTo2D(gBustupBitmapBuffer, (void *)0x06014000, 16);
    } else {
        u16 zero = 0;
        CpuSet(&zero, (void *)0x06014000, 0x01002000);
    }
    tb->text = text;
    MemCopy16((void *)0x05000000, gDialogueBoxPalette, 0x40);
    if ((tb->bgPal = set->bgPal))
        CpuSet(tb->bgPal + 16, (void *)0x05000020, 0xF8);
    if ((tb->objPal = set->objPal))
        CpuSet(tb->objPal, (void *)0x05000200, 0x100);
    if ((tb->anim = set->anim))
        gBustup.objCount = AnimBlockInit(tb->anim, objs);
    MemCopy16((void *)0x05000000, gDialogueTextPalette, 0x20);
}

/* Like Bustup_LoadSceneSet, but loads the scene bitmap before the dialogue box. */
void Bustup_ChangeSceneSet(const struct SceneSet *set, const u8 *text, struct TextBox *tb, void *objs, u8 box)
{
    const u16 *blob;

    LZ_DECOMPRESS(set->bitmapLz, gBustupBitmapBuffer);
    tb->bitmap = gBustupBitmapBuffer;
    CopyBitmapToPage(0, &tb->bitmap, 0x5A00);
    CopyBitmapToPage(1, &tb->bitmap, 0x5A00);
    blob = gDialogueBoxGfx[box];
    LZ_DECOMPRESS(blob, gBustupBoxBitmap);
    tb->boxBitmap = gBustupBoxBitmap;
    tb->bgPal = set->bgPal;
    tb->objPal = set->objPal;
    if (set->objTilesLz) {
        LZ_DECOMPRESS(set->objTilesLz, gBustupBitmapBuffer);
        CopyTileSheetTo2D(gBustupBitmapBuffer, (void *)0x06014000, 16);
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
        gBustup.objCount = AnimBlockInit(tb->anim, objs);
    MemCopy16((void *)0x05000000, gDialogueTextPalette, 0x20);
}

/* Selects the displayed Mode-4 page (DISPCNT bit 4). */
void Bustup_ShowPage(struct TextBox *tb)
{
    s32 dispcnt = *(vu16 *)0x04000000 & ~0x10;
    if (tb->page)
        dispcnt |= 0x10;
    *(vu16 *)0x04000000 = dispcnt;
}

/* Redraws the dialogue-box bitmap into the hidden page when flagged. */
void Bustup_ClearHiddenBox(struct TextBox *tb)
{
    if (tb->boxDirty == 1)
        CpuSet(tb->boxBitmap, (void *)(tb->page == 1 ? 0x06005A00 : 0x0600FA00), 0x1E00);
}

void CopyFullBitmapToPage(u8 bg, void **gfx)
{
    void *src = *gfx;
    void *dest = (void *)0x0600A000;
    if (bg == 0)
        dest = (void *)0x06000000;
    CpuSet(src, dest, 0x4B00);
}

#define PAGE_VRAM(page) ((page) == 0 ? 0x06000000 : 0x0600A000)

/* Copies a decoded scene bitmap of `size` bytes to Mode-4 page `page`, in quarters. */
void CopyBitmapToPage(u8 page, void **gfx, u16 size)
{
    CpuSet(*gfx, (void *)PAGE_VRAM(page), size >> 3);
    CpuSet((u8 *)*gfx + (size >> 2), (void *)((size >> 2) + PAGE_VRAM(page)), size >> 3);
    CpuSet((u8 *)*gfx + (size >> 2) * 2, (void *)((size >> 2) * 2 + PAGE_VRAM(page)), size >> 3);
    CpuSet((u8 *)*gfx + (size >> 2) * 3, (void *)((size >> 2) * 3 + PAGE_VRAM(page)), size >> 3);
}

void Bustup_MarkBoxDirty(struct TextBox *tb)
{
    tb->boxDirty = 1;
}

/* Text-box state init. */
void Bustup_InitTextBox(struct TextBox *tb)
{
    tb->delay = 2;
    tb->state = 0;
    if (gBustup.flags137C & 2)
        tb->state = 1;
    gBustup.flag9A6 = 0;
    tb->col = gBustupTextHome[0];
    tb->row = gBustupTextHome[1];
    tb->textColor = 7;
}

void AnimStateStart(struct AnimObj *obj)
{
    obj->unkE = 1;
}

/* Steps the blink timer; on expiry loads the next duration from the
 * zero-terminated table gBlinkIntervals (wrapping) and flags `obj`. */
void Bustup_TickBlink(struct AnimObj *obj)
{
    u16 t = --gBustup.blinkTimer;

    if (t == 0xFFFF) {
        if ((gBustup.blinkTimer = gBlinkIntervals[gBustup.blinkIndex++]) == 0) {
            gBustup.blinkIndex = 0;
            gBustup.blinkTimer = gBlinkIntervals[gBustup.blinkIndex++];
        }
        obj->unkE = 1;
    }
}

void Bustup_ResetBlink(void)
{
    u8 i;

    gBustup.blinkTimer = 0;
    gBustup.blinkIndex = 0;
    for (i = 0; i < 20; i++)
        gBustup.objs[i].unk12 |= 0xFF;
}

void Bustup_ResetBlinkUnused(void)
{
    u8 i;

    gBustup.blinkTimer = 0;
    gBustup.blinkIndex = 0;
    for (i = 0; i < 20; i++)
        gBustup.objs[i].unk12 |= 0xFF;
}

s32 MulFix8(s16 a, s16 b);
extern const s16 gSineTable[];
extern const u8 gCursorTrailPalettes[];

#define SPARKLE_BASE_X gBustup.textBox.sparkleX
#define SPARKLE_BASE_Y gBustup.textBox.sparkleY
#define SPARKLE_HEAD gBustup.textBox.sparkleHead
#define SPARKLE_HIST gBustup.textBox.sparkleHist

/* Cursor sparkle: 6 sprites orbiting (sparkleX, sparkleY); older ones replay
 * positions from a 30-entry history ring, 3 frames apart. */
void Bustup_DrawCursorTrail(void)
{
    u8 i;
    int x, y;
    /* Zero y offset and the 3-frame trail gap stay in variables: the ROM keeps the
     * `add r4,#0` / `mov r3,#0; orr` and the `mul` by 3 that constants would fold away. */
    int off = 0; u8 gap = 3;

    for (i = 5; i != 0xFF; i--) {
        x = (u8)(SPARKLE_BASE_X + (MulFix8(gSineTable[(gBustup.blinkTimer + i * 8) * 2 % 256 + 64], 0x400) >> 8));
        y = (u8)(SPARKLE_BASE_Y + (MulFix8(gSineTable[(gBustup.blinkTimer + i * 8) * 4 % 256], 0x300) >> 8));
        if (i != 5) {
            s32 d = (i + 1) * gap - 30;
            s32 idx = (SPARKLE_HEAD - d) % 30;
            if (SPARKLE_HIST[idx].x != 0xFF) {
                u32 *obj = OamListAddSprite(1, 4, SPARKLE_HIST[idx].x, SPARKLE_HIST[idx].y + off, 0x20, 0x10, 4, gCursorTrailPalettes[i], 0, 0, 0, gBustup.unkAA8); /* 0x02014888; as a member it gives the base its extra ref (r7, not r8) */
                *obj |= 0x400;
                continue;
            }
        }
        y += off;
        OamListAddSprite(0, 4, x, y, 0x20, 0x10, 4, gCursorTrailPalettes[5 - i]);
        SPARKLE_HIST[SPARKLE_HEAD].x = x;
        SPARKLE_HIST[SPARKLE_HEAD].y = y;
        SPARKLE_HEAD = ++SPARKLE_HEAD % 30;
    }
}

/* gMain (0x03000040): canonical layout in include/main.h. */
#define gMain gMain

struct SaveData {
    u32 unk0;
    u8 flags4;              /* +0x04 bit 7: Shift-JIS text mode */
};
extern struct SaveData gSaveData;
#define gSaveData gSaveData

extern const u8 gStrDebugChangeBg[];
extern const u8 gStrDebugLineOverflow[];
extern const u16 gCardNumberToId[];
extern const u8 gCardNames[][0x40]; /* names, 0x40 bytes each */
extern struct AnimObj gBustupMouthAnim;

u8 ParseTwoDigits(const u8 *s);
u32 ParseDigits(const u8 *s, u32 digits);
u32 NextWordFits(const u8 *s, u8 col, u32 maxCol);
void BitmapDrawStringShadow(const u8 *glyph, u32 x, u32 y, u32 vram, u32 color, u32 a5, u32 a6, u32 a7, u32 a8);
void StrCopy(u8 *dst, const u8 *src);
void DebugPrintf(const u8 *fmt, u32 arg);
void DebugPrintFlush(void);

#define IS_SJIS() (gSaveData.flags4 & 0x80)

/* Per-frame text processor: prints one glyph of tb->text (or of an inserted
 * name), handling tabs/newlines and `$` control codes. */
void Bustup_UpdateTextBox(struct TextBox *tb)
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
            if (NextWordFits(&tb->name[tb->nameIdx], tb->col, 30) == 0) {
                tb->col = gBustupTextHome[0];
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
                BitmapDrawStringShadow(nameGlyph, tb->col * 6 + 30, tb->row * 13 + 102, PAGE_VRAM(tb->page), tb->textColor, 14, 12, 240, 8);
                break;
            }
            if (IS_SJIS())
                tb->col += 2;
            else
                tb->col += 1;
            AnimStateStart(&gBustupMouthAnim);
        } else {
            tb->state = 0;
            if (gBustup.flags137C & state)
                tb->state = 1;
            tb->textColor = 7;
        }
        return;
    }
    case 3:
        tb->state = 1;
        tb->page ^= 1;
        tb->col = gBustupTextHome[0];
        tb->row = gBustupTextHome[1];
        Bustup_MarkBoxDirty(tb);
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
            u8 *base = (u8 *)&gBustup;
            register u32 offset asm("r2") = 0x9A6;
            asm("" : "+r"(base), "+r"(offset));
            base[offset] = 0;
            p++;
            break;
        }
        case 'k':
            gBustup.flag9A6 = 1;
            p++;
            break;
        case 'n':
            tb->col = gBustupTextHome[0];
            tb->row++;
            p++;
            break;
        case 'p':
            tb->page ^= 1;
            tb->delay = 2;
            tb->col = gBustupTextHome[0];
            tb->row = gBustupTextHome[1];
            Bustup_MarkBoxDirty(tb);
            p++;
            break;
        case 'c':
            if (!(gBustup.flags137C & 2))
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
            gBustup.speaker = ParseTwoDigits(p);
            DebugPrintf(gStrDebugChangeBg, gBustup.speaker);
            DebugPrintFlush();
            if (gBustup.speaker > 39)
                gBustup.speaker = 0;
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
            id = ParseDigits(p, 4);
            if (id == 0xFFFF)
                n = 0;
            else if (id < 2000)
                n = ((const u16 *)0x08623DF4)[id & 0x7FF];
            else
                n = ((const u16 *)0x08623DF4)[(id - 2000) & 0x7FF] + 1;
            StrCopy(dst, (const u8 *)(0x0822C720 + ((u32)(u16)n << 6)));
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
            StrCopy(dst, GetDuelistName(ParseDigits(p, 2), 1));
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
            StrCopy(dst, GetDuelistName(ParseDigits(p, 2), 0));
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
    if (NextWordFits(p, tb->col, 30) == 0) {
        tb->col = gBustupTextHome[0];
        tb->row++;
        if (tb->row > 3) {
            if (gBustup.flags137C & 2) {
                DebugPrintf(gStrDebugLineOverflow, tb->row);
                gBustup.flags137C |= 1;
            } else {
                tb->state = -3;
            }
            return;
        }
    }
    if (tb->col == gBustupTextHome[0] && (*p == ' ' || *p == '.')) {
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
        BitmapDrawStringShadow(p, tb->col * 6 + 30, tb->row * 13 + 102, PAGE_VRAM(tb->page), tb->textColor, 14, 12, 240, 8);
        break;
    }
    if (IS_SJIS())
        tb->col += 2;
    else
        tb->col += 1;
    AnimStateStart(&gBustupMouthAnim);
    return;

finished:
    tb->state = -2;
    if (gBustup.flags137C & 2)
        gBustup.flags137C |= 1;
}

struct SparkleStart {
    u8 x;
    u8 y;
    u8 filler2[2];
};
extern const struct SparkleStart gBustupSlotPositions[];

void MemClear16(void *dst, u32 size); /* MemClear16 */
void OamListClear(void *p);
void LineInit(s16 x0, s16 y0, s16 x1, s16 y1, void *line);
void Timer_Reset(void *timer);

/* Dialogue scene init: clears the script state, resets BG scroll/affine registers
 * and the sparkle trail, and copies the speaker/dialogue index from gMain. */
void Bustup_InitState(void)
{
    u8 i;

    MemClear16(&gBustup, sizeof(struct ScriptState));
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
    gBustup.textBox.page = 0;
    gBustup.unk12E9 = 0;
    gBustup.unk12EA = 0;
    gBustup.unk12E8 = 0;
    gBustup.dialogueIndex = 0;
    OamListClear(gBustup.unkAA8);
    gBustup.textBox.sparkleHead = 0xFF;
    for (i = 0; i < 30; i++)
        gBustup.textBox.sparkleHist[i].x |= 0xFF;
    LineInit(gBustupSlotPositions[gBustup.unk12E9].x, gBustupSlotPositions[gBustup.unk12E9].y,
                 gBustupSlotPositions[gBustup.unk12E9].x, gBustupSlotPositions[gBustup.unk12E9].y, &gBustup.textBox.sparkleX);
    gBustup.unk12FA = 5;
    gBustup.unk12F9 = 0;
    {
        u8 *flag = &gBustup.flag136E; /* take the address first; this order is needed to match */
        *flag = gMain.flag4884_3;
    }
    gBustup.speaker = gMain.speaker;
    gBustup.dialogueIndex = gMain.dialogueIndex;
    gBustup.unk136F = 0;
    gBustup.unk1370 = 0;
    Timer_Reset(gBustup.timer1374);
    Timer_Reset(gBustup.timer1378);
}
