/*
 * Bust-up dialogue scenes, part 1 (the runner and its steps are in bustup_runner.c): duelist names, the three
 * Mode-4 scene loaders, page display, the text box and its `$`-code printer, the eye blink, the cursor trail
 * of the unused opponent preview, and the module init.
 *
 * The scene is a Mode-4 bitmap (240x160, 8 bpp, two pages): the portrait fills rows 0-95 and the dialogue box
 * rows 96-159. Text is drawn into page tb->page; a page flip (`$p`, or a full box) shows the other page and
 * clears its box. See wiki/functions/bustup-scene-c.md and wiki/game/text-system.md.
 */
#include "global.h"
#include "gba.h"        /* REG_DISPCNT, REG_BG1HOFS..REG_BG3VOFS, REG16, VRAM, BG_PLTT, OBJ_PLTT */
#include "main.h"       /* gMain */
#include "bustup.h"     /* gBustup, struct BustupTextBox / SceneSet / Duelist / BytePos, the functions defined here */
#include "bg.h"         /* LZSSDecompress, CopyTileSheetTo2D, TILE_COLORS_16 */
#include "sprite.h"     /* struct AnimState / OamList / OamListEntry, ANIM_PLAYING, OamListAlloc, OamListClear */
#include "util.h"       /* MemClear16, MemCopy16, StrCopy, gSineTable, LineInit, Timer_Reset */
#include "text.h"       /* ParseTwoDigits, ParseDigits */
#include "debug.h"      /* DebugPrintf, DebugPrintFlush */
#include "card_data.h"  /* CARD_NUMBER_COUNT, CARD_NUMBER_ALT_ART, CARD_NAME_SIZE */

/* ---- Names of the new gba.h and main.h ----
 * include/ holds the legacy gba.h and main.h until step H0 of build/readability/header_plan.json installs the
 * new ones, which define these with the same values. Delete this block after H0. */
#ifndef CPU_SET_SRC_FIXED
#define CPU_SET_SRC_FIXED       0x01000000
#define DISPCNT_BG_ALL_ON       0x0F00
#define DISPCNT_OBJ_ON          0x1000
#define OAM_ATTR0_BLEND         0x0400
#define OAM_ATTR0_H_RECTANGLE   0x4000
#define OAM_ATTR1_SIZE(n)       ((n) << 14)
#define OAM_ATTR2_PALETTE(n)    ((n) << 12)
#define VBLANK_COPY_OAM         0x1     /* enum VBlankFlag in the new main.h */
void CpuSet(const void *src, void *dest, u32 control);
#endif

/* The 16-bit halves of the BG2 reference point (gba.h names only the 32-bit REG_BG2X / REG_BG2Y). */
#define REG_BG2X_L          REG16(0x028)
#define REG_BG2X_H          REG16(0x02A)
#define REG_BG2Y_L          REG16(0x02C)
#define REG_BG2Y_H          REG16(0x02E)

/* ---- Mode-4 screen layout of the bust-up scene ---- */
#define SCREEN_WIDTH        240
#define MODE4_PAGE0         VRAM                    /* 0x06000000 */
#define MODE4_PAGE1         (VRAM + 0xA000)         /* 0x0600A000 */
#define MODE4_PAGE(page)    ((page) == 0 ? MODE4_PAGE0 : MODE4_PAGE1)
#define DISPCNT_PAGE1       0x0010                  /* DISPCNT bit 4: Mode-4/5 frame select */
#define OBJ_VRAM_BITMAP     (VRAM + 0x14000)        /* OBJ tiles in the bitmap modes (0x06014000) */
#define OBJ_VRAM_BITMAP_BYTES 0x4000                /* its size: OBJ tiles 0x200-0x3FF */
#define BG_PLTT_FROM_16     (BG_PLTT + 16 * 2)      /* BG colour 16: colours 0-15 are the text palette */
#define SCENE_BG_COLORS     0xF8                    /* from colour 16: the last 8 land in OBJ colours 0-7 */
#define OBJ_COLORS          0x100

#define SCENE_BYTES         (96 * SCREEN_WIDTH)     /* 0x5A00: portrait, rows 0-95 */
#define BOX_OFFSET          SCENE_BYTES             /* the dialogue box starts at row 96 */
#define BOX_BYTES           (64 * SCREEN_WIDTH)     /* 0x3C00: box, rows 96-159 */
#define HEADER_BYTES        (16 * SCREEN_WIDTH)     /* 0xF00: header strip of the 240x80 scenes, rows 0-15 */
#define SCENE80_BYTES       (80 * SCREEN_WIDTH)     /* 0x4B00: 240x80 scene, rows 16-95 */
#define SCREEN_BYTES        (160 * SCREEN_WIDTH)    /* 0x9600: a whole page */

/* ---- Text box layout (Bustup_UpdateTextBox) ---- */
#define TEXT_COLS           30      /* columns per line, 6 px each */
#define TEXT_ROWS           4       /* lines per page, 13 px each */
#define TEXT_X(col)         ((col) * 6 + 30)
#define TEXT_Y(row)         ((row) * 13 + 102)
#define GLYPH_DELAY         2       /* tb->delay after a glyph: one glyph every 3 frames */
#define SPEAKER_MAX         39      /* `$bNN` above this selects speaker 0 */

#define DUELIST_COUNT       28      /* records of gDuelists */
#define TRAIL_LENGTH        30      /* entries of gBustup.textBox.trail[] */

/* LZSS blob {u16 sizeLo, sizeHi; u8 stream[]}: decompress `blob` to `dest`. */
#define LZ_DECOMPRESS(blob, dest) \
    LZSSDecompress((u8 *)(blob) + 4, dest, ((blob)[1] << 16) | (blob)[0])

/* ---- ROM data used only by this unit ---- */
extern struct Duelist gDuelists[];              /* 0x08139F64: character names, entry 0 is " " */
extern const u16 *const gDialogueBoxGfx[];      /* 0x08139F5C: LZSS dialogue box bitmaps, by the loaders' `box` */
extern const u16 gDialogueHeaderLz;             /* 0x0874D5B0: LZSS header strip, read as three labels: size */
extern const u16 gDialogueHeaderLzSizeHi;       /*   low halfword, size high halfword (+2), stream (+4) */
extern const u8 gDialogueHeaderLzData[];
extern const u16 gDialogueBoxPalette[];         /* 0x0874E104: box palette (colours 0-31 are loaded) */
extern const u16 gDialogueTextPalette[];        /* 0x0874E304: 16-colour text palette (the `$rX` colours) */
extern const u8 gBustupTextHome[];              /* 0x08087B90: text cursor home {col, row} = {0, 0} */
extern const u16 gBlinkIntervals[];             /* 0x08080AA8: frames between blinks, 0-terminated */
extern const u16 gBustupDigitTiles[];           /* 0x08080AC0: OBJ tiles of the digits 0-9 */
extern const struct BytePos gBustupSlotPositions[]; /* 0x08080A48: the 5 slots of the unused preview */
extern const u8 gCursorTrailPalettes[];         /* 0x08080A5C: OBJ palettes of the 6 trail sprites */
extern const char gStrDebugChangeBg[];          /* "Change BG:%d\n" */
extern const char gStrDebugLineOverflow[];      /* "Overflow the line ?:%d\n" */

/* ---- Local views (matching choices, see build/readability/HEADERS.md) ---- */

/* Byte +4 of gSaveData: IS_SJIS tests its bit 7 (SaveData.sjisText) with `& 0x80`; reading the 1-bit field
 * changes the code. */
extern u8 gSaveDataBytes[] asm("gSaveData");
#define IS_SJIS()           (gSaveDataBytes[4] & 0x80)

/* Two parts of gBustup accessed in another shape than struct BustupState declares them: the +0x810 array as
 * AnimState records (Bustup_ResetBlink; the header's u8[20][0x14] forms the address differently), and the
 * autoNext/autoAdvance bits as one byte (one test in Bustup_UpdateTextBox). */
struct BustupStateView {
    u8 unk0[0x810];
    struct AnimState unk810[20];    /* +0x0810 */
    u8 pad9A0[0x137C - 0x9A0];
    u8 autoFlags;                   /* +0x137C: bit 0 autoNext, bit 1 autoAdvance */
};
#define gBustupView (*(struct BustupStateView *)&gBustup)

/* gCardNumberToId and gCardNames, read through their integer addresses (the `$i` code). */
#define CARD_NUMBER_TO_ID   ((const u16 *)0x08623DF4)
#define CARD_NAMES_ADDR     0x0822C720

/* Called without a prototype: Bustup_DrawNumber passes all 13 arguments, promoted rather than narrowed to the
 * parameter types, and Bustup_DrawCursorTrail passes only 12 and 8 of them (as in the ROM). */
extern struct OamListEntry *OamListAddSpriteUnprototyped() asm("OamListAddSprite");
/* u32 parameters: the header's u8/u16 ones would add narrowing at the calls. */
extern void BitmapDrawStringShadowU32(const u8 *str, u32 x, u32 y, u32 bitmap, u32 color, u32 shadowColor,
                                      u32 size, u32 width, u32 bpp) asm("BitmapDrawStringShadow");
/* Wider return types: the header's s16 (MulFix8) and u8 (AnimBlockInit, NextWordFits) returns change the code
 * at the calls (an extra extension, or another register choice). */
extern s32 MulFix8S32(s16 a, s16 b) asm("MulFix8");
extern u32 AnimBlockInitU32(const void *const *scripts, void *block) asm("AnimBlockInit");
extern u32 NextWordFitsU32(const u8 *s, u8 col, u32 maxCol) asm("NextWordFits");

/* Returns the full (`full` != 0, `$q`) or short (`$Q`) name of character `id`, or entry 0 (" ") if no
 * record has that id. */
char *GetDuelistName(u32 id, u16 full)
{
    u32 i;

    for (i = 1; i < DUELIST_COUNT; i++) {
        if (gDuelists[i].id == id) {
            if (full)
                return gDuelists[i].name;
            return gDuelists[i].shortName;
        }
    }
    return gDuelists[0].name;
}

/* CpuSets `rows` rows of `width` bytes from a 240-byte-wide bitmap to dest, `pitch` bytes apart. Uncalled. */
void CopyBitmapRows(u8 *src, u32 *dest, u16 width, u16 rows, u16 pitch)
{
    u8 row;

    for (row = 0; row < rows; row++)
        CpuSet(src + row * SCREEN_WIDTH, dest + ((row * pitch) >> 2), width / 2);
}

/* Adds 2n + 2 32x16 sprites side by side to gBustupSprites (layer 0): sprite i is at (x + 32 i, y) and shows
 * OBJ tile (tile + 4 i) | 0x200 (the bitmap modes only have OBJ tiles 0x200-0x3FF), palette 2. Only the unused
 * opponent preview calls it. */
void Bustup_DrawLabel(u16 tile, u16 x, u16 y, u16 n)
{
    u8 i;

    for (i = 0; i < n * 2 + 2; i++) {
        /* Matching: the list is passed as the integer address of gBustupSprites, and the entry is filled as
         * u16 attr[3] (struct OamListEntry field accesses change the code). */
        u16 *attr = (u16 *)OamListAlloc(0, (struct OamList *)0x02014888);

        /* attr0 = wide shape | y and attr1 = size 2 (32x16), stored as one word */
        *(u32 *)attr = ((u32)OAM_ATTR1_SIZE(2) << 16) | OAM_ATTR0_H_RECTANGLE | y;
        attr[1] |= x + i * 32;
        attr[2] = (tile + i * 4) | OAM_ATTR2_PALETTE(2) | 0x200;
    }
}

/* Draws `value` right-aligned in `digits` 8x8 digit sprites (OBJ palette `pal`) into gBustupSprites. The
 * units digit is always drawn; leading zeros are left blank. */
void Bustup_DrawNumber(u16 value, u16 x, u16 y, u16 digits, u8 pal)
{
    u8 i;
    u16 digit;

    for (i = 0; i < digits; i++) {
        digit = value % 10;
        value = value / 10;
        if (i == 0) {
            OamListAddSpriteUnprototyped(0, gBustupDigitTiles[digit], x + (digits - i - 1) * 8, y, 8, 8, 4, pal,
                                         0, 0, 0, 0, &gBustupSprites);
        } else if (value != 0 || digit != 0) {
            /* FAKEMATCH: the same x as above, but only `(u16)(i + 1)` gives the ROM's code here. */
            OamListAddSpriteUnprototyped(0, gBustupDigitTiles[digit], x + (digits - (u16)(i + 1)) * 8, y, 8, 8,
                                         4, pal, 0, 0, 0, 0, &gBustupSprites);
        }
    }
}

/* Loads a 240x96 scene set into both Mode-4 pages: the dialogue box `box` (rows 96-159), the portrait bitmap
 * (rows 0-95), the BG colours from 16 up, the OBJ palette and tiles, and the animation tracks (`anims`, a
 * struct AnimState[20] block). Points tb->text at `text` and loads the text palette (colours 0-15). */
void Bustup_LoadSceneSet(const struct SceneSet *set, const u8 *text, struct BustupTextBox *tb, void *anims, u8 box)
{
    const u16 *boxLz = gDialogueBoxGfx[box];

    LZ_DECOMPRESS(boxLz, gBustupBoxBitmap);
    tb->boxBitmap = gBustupBoxBitmap;
    CpuSet(gBustupBoxBitmap, (void *)(MODE4_PAGE0 + BOX_OFFSET), BOX_BYTES / 2);
    CpuSet(gBustupBoxBitmap, (void *)(MODE4_PAGE1 + BOX_OFFSET), BOX_BYTES / 2);
    LZ_DECOMPRESS(set->bitmapLz, gBustupBitmapBuffer);
    tb->bitmap = gBustupBitmapBuffer;
    CopyBitmapToPage(0, &tb->bitmap, SCENE_BYTES);
    CopyBitmapToPage(1, &tb->bitmap, SCENE_BYTES);
    tb->bgPal = set->bgPal;
    tb->objPal = set->objPal;
    if (set->objTilesLz) {
        LZ_DECOMPRESS(set->objTilesLz, gBustupBitmapBuffer);
        CopyTileSheetTo2D(gBustupBitmapBuffer, (u8 *)OBJ_VRAM_BITMAP, TILE_COLORS_16);
    } else {
        u16 zero = 0;
        CpuSet(&zero, (void *)OBJ_VRAM_BITMAP, CPU_SET_SRC_FIXED | OBJ_VRAM_BITMAP_BYTES / 2);  /* clear */
    }
    tb->anim = set->anim;
    tb->text = text;
    if ((tb->bgPal = set->bgPal))
        CpuSet(tb->bgPal + 16, (void *)BG_PLTT_FROM_16, SCENE_BG_COLORS);
    if ((tb->objPal = set->objPal))
        CpuSet(tb->objPal, (void *)OBJ_PLTT, OBJ_COLORS);
    if ((tb->anim = set->anim))
        gBustup.animCount = AnimBlockInitU32(tb->anim, anims);
    MemCopy16((void *)BG_PLTT, gDialogueTextPalette, 16 * 2);
}

/* Bustup_LoadSceneSet for the 240x80 scene sets (speakers 0x20-0x23): the header strip fills rows 0-15 above
 * the bitmap (rows 16-95), and the box palette (colours 0-31) is loaded before the text palette. */
void Bustup_LoadSceneSetWithHeader(const struct SceneSet *set, const u8 *text, struct BustupTextBox *tb,
                                   void *anims, u8 box)
{
    const u16 *boxLz = gDialogueBoxGfx[box];

    LZ_DECOMPRESS(boxLz, gBustupBoxBitmap);
    tb->boxBitmap = gBustupBoxBitmap;
    CpuSet(gBustupBoxBitmap, (void *)(MODE4_PAGE0 + BOX_OFFSET), BOX_BYTES / 2);
    CpuSet(gBustupBoxBitmap, (void *)(MODE4_PAGE1 + BOX_OFFSET), BOX_BYTES / 2);
    /* Matching: the header blob is read through three labels instead of LZ_DECOMPRESS. */
    LZSSDecompress((u8 *)gDialogueHeaderLzData, gBustupBitmapBuffer,
                   gDialogueHeaderLz | (gDialogueHeaderLzSizeHi << 16));
    CpuSet(gBustupBitmapBuffer, (void *)MODE4_PAGE0, HEADER_BYTES / 2);
    CpuSet(gBustupBitmapBuffer, (void *)MODE4_PAGE1, HEADER_BYTES / 2);
    LZ_DECOMPRESS(set->bitmapLz, gBustupBitmapBuffer);
    tb->bitmap = gBustupBitmapBuffer;
    CpuSet(gBustupBitmapBuffer, (void *)(MODE4_PAGE0 + HEADER_BYTES), SCENE80_BYTES / 2);
    CpuSet(tb->bitmap, (void *)(MODE4_PAGE1 + HEADER_BYTES), SCENE80_BYTES / 2);
    if (set->objTilesLz) {
        LZ_DECOMPRESS(set->objTilesLz, gBustupBitmapBuffer);
        CopyTileSheetTo2D(gBustupBitmapBuffer, (u8 *)OBJ_VRAM_BITMAP, TILE_COLORS_16);
    } else {
        u16 zero = 0;
        CpuSet(&zero, (void *)OBJ_VRAM_BITMAP, CPU_SET_SRC_FIXED | OBJ_VRAM_BITMAP_BYTES / 2);  /* clear */
    }
    tb->text = text;
    MemCopy16((void *)BG_PLTT, gDialogueBoxPalette, 32 * 2);
    if ((tb->bgPal = set->bgPal))
        CpuSet(tb->bgPal + 16, (void *)BG_PLTT_FROM_16, SCENE_BG_COLORS);
    if ((tb->objPal = set->objPal))
        CpuSet(tb->objPal, (void *)OBJ_PLTT, OBJ_COLORS);
    if ((tb->anim = set->anim))
        gBustup.animCount = AnimBlockInitU32(tb->anim, anims);
    MemCopy16((void *)BG_PLTT, gDialogueTextPalette, 16 * 2);
}

/* After `$bNN`: loads the new speaker's 240x96 scene set into both pages, but keeps the box on screen and
 * tb->text (`text` is unused). The box is decoded again for later page clears, not drawn. */
void Bustup_ChangeSceneSet(const struct SceneSet *set, const u8 *text, struct BustupTextBox *tb, void *anims, u8 box)
{
    const u16 *boxLz;

    LZ_DECOMPRESS(set->bitmapLz, gBustupBitmapBuffer);
    tb->bitmap = gBustupBitmapBuffer;
    CopyBitmapToPage(0, &tb->bitmap, SCENE_BYTES);
    CopyBitmapToPage(1, &tb->bitmap, SCENE_BYTES);
    boxLz = gDialogueBoxGfx[box];
    LZ_DECOMPRESS(boxLz, gBustupBoxBitmap);
    tb->boxBitmap = gBustupBoxBitmap;
    tb->bgPal = set->bgPal;
    tb->objPal = set->objPal;
    if (set->objTilesLz) {
        LZ_DECOMPRESS(set->objTilesLz, gBustupBitmapBuffer);
        CopyTileSheetTo2D(gBustupBitmapBuffer, (u8 *)OBJ_VRAM_BITMAP, TILE_COLORS_16);
    } else {
        u16 zero = 0;
        CpuSet(&zero, (void *)OBJ_VRAM_BITMAP, CPU_SET_SRC_FIXED | OBJ_VRAM_BITMAP_BYTES / 2);  /* clear */
    }
    tb->anim = set->anim;
    if ((tb->bgPal = set->bgPal))
        CpuSet(tb->bgPal + 16, (void *)BG_PLTT_FROM_16, SCENE_BG_COLORS);
    if ((tb->objPal = set->objPal))
        CpuSet(tb->objPal, (void *)OBJ_PLTT, OBJ_COLORS);
    if ((tb->anim = set->anim))
        gBustup.animCount = AnimBlockInitU32(tb->anim, anims);
    MemCopy16((void *)BG_PLTT, gDialogueTextPalette, 16 * 2);
}

/* Shows Mode-4 page tb->page (DISPCNT frame select). */
void Bustup_ShowPage(struct BustupTextBox *tb)
{
    s32 dispcnt = REG_DISPCNT & ~DISPCNT_PAGE1;

    if (tb->page)
        dispcnt |= DISPCNT_PAGE1;
    REG_DISPCNT = dispcnt;
}

/* If tb->boxDirty == 1, copies the clean box bitmap over the box of the page that is not tb->page. */
void Bustup_ClearHiddenBox(struct BustupTextBox *tb)
{
    if (tb->boxDirty == 1)
        CpuSet(tb->boxBitmap, (void *)(tb->page == 1 ? MODE4_PAGE0 + BOX_OFFSET : MODE4_PAGE1 + BOX_OFFSET),
               BOX_BYTES / 2);
}

/* Copies a whole 240x160 bitmap from *bitmap to Mode-4 page `page`. Uncalled. */
void CopyFullBitmapToPage(u8 page, void **bitmap)
{
    void *src = *bitmap;
    void *dest = (void *)MODE4_PAGE1;

    if (page == 0)
        dest = (void *)MODE4_PAGE0;
    CpuSet(src, dest, SCREEN_BYTES / 2);
}

/* Copies `size` bytes from *bitmap to the start of Mode-4 page `page`, in four CpuSets of size / 4 bytes. */
void CopyBitmapToPage(u8 page, void **bitmap, u16 size)
{
    CpuSet(*bitmap, (void *)MODE4_PAGE(page), size >> 3);
    CpuSet((u8 *)*bitmap + (size >> 2), (void *)((size >> 2) + MODE4_PAGE(page)), size >> 3);
    CpuSet((u8 *)*bitmap + (size >> 2) * 2, (void *)((size >> 2) * 2 + MODE4_PAGE(page)), size >> 3);
    CpuSet((u8 *)*bitmap + (size >> 2) * 3, (void *)((size >> 2) * 3 + MODE4_PAGE(page)), size >> 3);
}

/* Requests a box clear and page display for the next frame (Bustup_ClearHiddenBox, Bustup_ShowPage). */
void Bustup_MarkBoxDirty(struct BustupTextBox *tb)
{
    tb->boxDirty = 1;
}

/* Text box init: print one glyph every 3 frames (every frame in auto mode), clear the `$h`/`$k` flag, cursor
 * home, default colour. */
void Bustup_InitTextBox(struct BustupTextBox *tb)
{
    tb->delay = GLYPH_DELAY;
    tb->state = TEXT_STATE_PRINT;
    if (gBustup.autoAdvance)
        tb->state = TEXT_STATE_FAST;
    gBustup.scriptFlag = 0;
    tb->col = gBustupTextHome[0];
    tb->row = gBustupTextHome[1];
    tb->textColor = TEXT_COLOR_DEFAULT;
}

/* Replays a finished one-shot animation track from its first step. */
void AnimStateStart(struct AnimState *st)
{
    st->active = ANIM_PLAYING;
}

/* Counts the blink timer down; when it wraps, loads the next interval of the 0-terminated gBlinkIntervals
 * (starting over after the 0) and restarts the eye track. */
void Bustup_TickBlink(struct AnimState *eyes)
{
    u16 timer = --gBustup.blinkTimer;

    if (timer == 0xFFFF) {
        /* Matching: only the assignment-as-condition form gives the ROM's zero extension. */
        if ((gBustup.blinkTimer = gBlinkIntervals[gBustup.blinkIndex++]) == 0) {
            gBustup.blinkIndex = 0;
            gBustup.blinkTimer = gBlinkIntervals[gBustup.blinkIndex++];
        }
        eyes->active = ANIM_PLAYING;
    }
}

/* Blinks on the next tick (timer and interval index 0). Also sets byte +0x12 (AnimState.unk12) of the 20
 * entries at gBustup.unk810, which nothing reads. */
void Bustup_ResetBlink(void)
{
    u8 i;

    gBustup.blinkTimer = 0;
    gBustup.blinkIndex = 0;
    for (i = 0; i < 20; i++)
        gBustupView.unk810[i].unk12 |= 0xFF;
}

/* Uncalled, byte-identical copy of Bustup_ResetBlink. */
void Bustup_ResetBlinkUnused(void)
{
    u8 i;

    gBustup.blinkTimer = 0;
    gBustup.blinkIndex = 0;
    for (i = 0; i < 20; i++)
        gBustupView.unk810[i].unk12 |= 0xFF;
}

/*
 * Cursor trail of the unused opponent preview: 6 32x16 sprites (OBJ tile 4). Each frame the head (i = 5) is
 * drawn at the cursor line position plus a wobble (x +-4 px on the cosine, y +-3 px on the sine, phase
 * gBustup.blinkTimer + 8 i) and recorded in the 30-entry ring gBustup.textBox.trail. Sprite i < 5 replays the
 * ring entry (i + 1) * 3 places behind the write index, semi-transparent, on layer 1. The ROM calls
 * OamListAddSprite with 12 and 8 of its 13 arguments; the calls are kept as they are.
 */
void Bustup_DrawCursorTrail(void)
{
    u8 i;
    int x, y;
    /* FAKEMATCH: the zero y offset and the 3-frame gap stay in variables: the ROM keeps the `add r4,#0` and
     * the `mov r3,#0; orr` of the offset, and the `mul` by 3, that constants would fold away. */
    int yOffset = 0;
    u8 gap = 3;

    for (i = 5; i != 0xFF; i--) {
        x = (u8)(gBustup.textBox.cursor.x
                 + (MulFix8S32(gSineTable[(gBustup.blinkTimer + i * 8) * 2 % 256 + 64], 0x400) >> 8));
        y = (u8)(gBustup.textBox.cursor.y
                 + (MulFix8S32(gSineTable[(gBustup.blinkTimer + i * 8) * 4 % 256], 0x300) >> 8));
        if (i != 5) {
            /* the entry (i + 1) * gap places behind the write index; - TRAIL_LENGTH keeps it non-negative */
            s32 lag = (i + 1) * gap - TRAIL_LENGTH;
            s32 slot = (gBustup.textBox.trailHead - lag) % TRAIL_LENGTH;

            if (gBustup.textBox.trail[slot].x != 0xFF) {
                /* Matching: the list as the member gBustup.sprites (not the gBustupSprites symbol) gives the
                 * base register its extra use (r7, not r8). It lands in the `priority` slot. */
                u32 *attr01 = (u32 *)OamListAddSpriteUnprototyped(1, 4, gBustup.textBox.trail[slot].x,
                                                                  gBustup.textBox.trail[slot].y + yOffset, 32, 16,
                                                                  4, gCursorTrailPalettes[i], 0, 0, 0,
                                                                  &gBustup.sprites);
                *attr01 |= OAM_ATTR0_BLEND;
                continue;
            }
            /* an empty entry: drawn and recorded like the head */
        }
        y += yOffset;
        OamListAddSpriteUnprototyped(0, 4, x, y, 32, 16, 4, gCursorTrailPalettes[5 - i]);
        gBustup.textBox.trail[gBustup.textBox.trailHead].x = x;
        gBustup.textBox.trail[gBustup.textBox.trailHead].y = y;
        gBustup.textBox.trailHead = ++gBustup.textBox.trailHead % TRAIL_LENGTH;
    }
}

/*
 * Per-frame text printer. One call prints at most one glyph, of the script (tb->text) or of an inserted name
 * (state TEXT_STATE_INSERT_NAME), at (col * 6 + 30, row * 13 + 102) of page tb->page, then restarts the mouth
 * animation. Tabs and newlines in the script are skipped. Lines wrap at 30 columns; a line-initial ' ' or '.'
 * is dropped. In Shift-JIS mode a glyph is 2 bytes and 2 columns. Control codes:
 *   $QNN  short / $qNN full name of duelist NN      $iNNNN card name of card number NNNN (>= 2000: alt art)
 *   $bNN  change the speaker's scene (step 6)       $c     wait for A (ignored in auto mode)
 *   $h/$k clear / set gBustup.scriptFlag            $n     newline
 *   $p    next page                                 $rX    colour X (0-9, a-f)
 * Wrapping onto a fifth line stops with TEXT_STATE_BOX_FULL (auto mode: a debug print and autoNext instead);
 * the end of the text stops with TEXT_STATE_END (auto mode: autoNext too).
 */
void Bustup_UpdateTextBox(struct BustupTextBox *tb)
{
    const u8 *str = tb->text;
    s32 state;
    /* FAKEMATCH: the control-code byte is pinned to r1. */
    register u8 code asm("r1");
    u8 nameGlyph[4];
    u8 glyph[4];
    /* FAKEMATCH: pinned to r1; assigned in both glyph paths before any use. */
    register u8 *glyphPtr asm("r1");

    if (tb->state < 0)
        return;
    state = tb->state;

    switch (state) {
    default:
        if (*str == 0)
            goto finished;
        break;
    case TEXT_STATE_INSERT_NAME: {
        /* Prints the next glyph of tb->name[] */
        u8 *nameIdx = &tb->nameIdx;
        u8 pos = *nameIdx;
        u8 *name = tb->name;
        u8 sjis;

        if (name[pos] != 0) {
            if (NextWordFitsU32(&tb->name[tb->nameIdx], tb->col, TEXT_COLS) == 0) {
                tb->col = gBustupTextHome[0];
                tb->row++;
            }
            tb->delay = state;      /* = TEXT_STATE_INSERT_NAME = GLYPH_DELAY (matching: reuses the register) */
            sjis = IS_SJIS();
            if (sjis) {
                nameGlyph[0] = name[(*nameIdx)++];
                nameGlyph[1] = name[(*nameIdx)++];
                nameGlyph[2] = 0;
            } else {
                nameGlyph[0] = name[(*nameIdx)++];
                nameGlyph[1] = sjis;
            }
            switch (tb->page) {
            case 0:
            case 1:
                BitmapDrawStringShadowU32(nameGlyph, TEXT_X(tb->col), TEXT_Y(tb->row), MODE4_PAGE(tb->page),
                                          tb->textColor, 14, 12, SCREEN_WIDTH, 8);
                break;
            }
            if (IS_SJIS())
                tb->col += 2;
            else
                tb->col += 1;
            AnimStateStart(&gBustupMouthAnim);
        } else {
            /* End of the name: back to the script */
            tb->state = TEXT_STATE_PRINT;
            if (gBustupView.autoFlags & state)   /* state = 2: the autoAdvance bit */
                tb->state = TEXT_STATE_FAST;
            tb->textColor = TEXT_COLOR_DEFAULT;
        }
        return;
    }
    case TEXT_STATE_NEXT_PAGE:
        tb->state = TEXT_STATE_FAST;
        tb->page ^= 1;
        tb->col = gBustupTextHome[0];
        tb->row = gBustupTextHome[1];
        Bustup_MarkBoxDirty(tb);
        return;
    }

    while (*str == '\t' || *str == '\n')
        tb->text = ++str;

    if (*str == '$') {
        str++;
        code = *str;
        if (code != '$') {
        switch (code) {
        case 'h': {
            /* FAKEMATCH: gBustup.scriptFlag = 0 through a base and an offset bound to r2. */
            u8 *base = (u8 *)&gBustup;
            register u32 offset asm("r2") = OFFSET_OF(struct BustupState, scriptFlag);
            asm("" : "+r"(base), "+r"(offset));
            base[offset] = 0;
            str++;
            break;
        }
        case 'k':
            gBustup.scriptFlag = 1;
            str++;
            break;
        case 'n':
            tb->col = gBustupTextHome[0];
            tb->row++;
            str++;
            break;
        case 'p':
            tb->page ^= 1;
            tb->delay = GLYPH_DELAY;
            tb->col = gBustupTextHome[0];
            tb->row = gBustupTextHome[1];
            Bustup_MarkBoxDirty(tb);
            str++;
            break;
        case 'c':
            if (!gBustup.autoAdvance)
                tb->state = TEXT_STATE_WAIT_BUTTON;
            str++;
            break;
        case 'r':
            str++;
            switch (*str) {
            case 'a': tb->textColor = 10; break;
            case 'b': tb->textColor = 11; break;
            case 'c': tb->textColor = 12; break;
            case 'd': tb->textColor = 13; break;
            case 'e': tb->textColor = 14; break;
            case 'f': tb->textColor = 15; break;
            default: tb->textColor = *str - '0'; break;
            }
            str++;
            break;
        case 'b':
            /* The runner goes from BUSTUP_STEP_UPDATE to BUSTUP_STEP_CHANGE_SPEAKER, which loads the scene */
            str++;
            gBustup.speaker = ParseTwoDigits(str);
            DebugPrintf(gStrDebugChangeBg, gBustup.speaker);
            DebugPrintFlush();
            if (gBustup.speaker > SPEAKER_MAX)
                gBustup.speaker = 0;
            str += 2;
            gMain.seqIndex1 += BUSTUP_STEP_CHANGE_SPEAKER - BUSTUP_STEP_UPDATE;
            break;
        case 'i': {
            u16 number;
            u32 cardId;     /* Matching: narrowed to u16 only when forming the name offset */
            u8 *dst;

            str++;
            dst = tb->name;     /* Matching: taken before the ParseDigits call */
            number = ParseDigits(str, 4);
            /* An alternate-art number (2000 + n) has the ID of card n plus 1. */
            if (number == 0xFFFF)
                cardId = 0;
            else if (number < CARD_NUMBER_ALT_ART)
                cardId = CARD_NUMBER_TO_ID[number & (CARD_NUMBER_COUNT - 1)];
            else
                cardId = CARD_NUMBER_TO_ID[(number - CARD_NUMBER_ALT_ART) & (CARD_NUMBER_COUNT - 1)] + 1;
            StrCopy((char *)dst, (const char *)(CARD_NAMES_ADDR + (u32)(u16)cardId * CARD_NAME_SIZE));
            str += 4;
            tb->textColor = TEXT_COLOR_CARD_NAME;
            tb->state = TEXT_STATE_INSERT_NAME;
            tb->nameIdx = 0;
            break;
        }
        case 'q': {
            u8 *dst;

            str++;
            dst = tb->name;
            StrCopy((char *)dst, GetDuelistName(ParseDigits(str, 2), 1));
            str += 2;
            tb->textColor = TEXT_COLOR_FULL_NAME;
            tb->state = TEXT_STATE_INSERT_NAME;
            tb->nameIdx = 0;
            break;
        }
        case 'Q': {
            u8 *dst;

            str++;
            dst = tb->name;
            StrCopy((char *)dst, GetDuelistName(ParseDigits(str, 2), 0));
            str += 2;
            tb->textColor = TEXT_COLOR_SHORT_NAME;
            tb->state = TEXT_STATE_INSERT_NAME;
            tb->nameIdx = 0;
            break;
        }
        }
        }
        /* "$$" is no escape: str is left on the second '$', which starts the next code */
        tb->text = str;
        return;
    }

    /* A glyph of the script */
    if (tb->state == TEXT_STATE_FAST)
        tb->delay = 0;
    if (--tb->delay != 0xFF)    /* print when the delay counter wraps */
        return;
    if (NextWordFitsU32(str, tb->col, TEXT_COLS) == 0) {
        tb->col = gBustupTextHome[0];
        tb->row++;
        if (tb->row >= TEXT_ROWS) {
            if (gBustup.autoAdvance) {
                DebugPrintf(gStrDebugLineOverflow, tb->row);
                gBustup.autoNext = 1;
            } else {
                tb->state = TEXT_STATE_BOX_FULL;
            }
            return;
        }
    }
    if (tb->col == gBustupTextHome[0] && (*str == ' ' || *str == '.')) {
        str++;
        tb->text++;
    }
    {
    u8 sjis = IS_SJIS();
    if (sjis) {
        glyphPtr = glyph;
        {
            /* Matching: read the first byte before the terminator value is set. */
            u8 first = str[0];
            sjis = 0;
            glyphPtr[0] = first;
        }
        glyphPtr[1] = str[1];
        glyphPtr[2] = sjis;
        tb->text += 2;
    } else {
        glyphPtr = glyph;
        glyphPtr[0] = str[0];
        glyphPtr[1] = sjis;
        tb->text += 1;
    }
    }
    str = glyphPtr;
    tb->delay = GLYPH_DELAY;
    switch (tb->page) {
    case 0:
    case 1:
        BitmapDrawStringShadowU32(str, TEXT_X(tb->col), TEXT_Y(tb->row), MODE4_PAGE(tb->page), tb->textColor,
                                  14, 12, SCREEN_WIDTH, 8);
        break;
    }
    if (IS_SJIS())
        tb->col += 2;
    else
        tb->col += 1;
    AnimStateStart(&gBustupMouthAnim);
    return;

finished:
    tb->state = TEXT_STATE_END;
    if (gBustup.autoAdvance)
        gBustup.autoNext = 1;
}

/* Clears gBustup, enables only the OAM flush, hides every layer, zeroes the BG1-3 scroll and the BG2 reference
 * point, empties the sprite list and the trail, parks the cursor line on its slot, and copies the speaker and
 * the dialogue index from gMain. */
void Bustup_InitState(void)
{
    u8 i;

    MemClear16(&gBustup, sizeof(struct BustupState));
    gMain.vblankFlags = VBLANK_COPY_OAM;
    REG_DISPCNT &= ~(DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_BG2X_L = 0;
    REG_BG2X_H = 0;
    REG_BG2Y_L = 0;
    REG_BG2Y_H = 0;
    gBustup.textBox.page = 0;
    gBustup.selectSlot = 0;
    gBustup.selectPage = 0;
    gBustup.unk12E8 = 0;
    gBustup.dialogueIndex = 0;
    OamListClear((u8 *)&gBustup.sprites);
    gBustup.textBox.trailHead = 0xFF;
    for (i = 0; i < TRAIL_LENGTH; i++)
        gBustup.textBox.trail[i].x |= 0xFF;     /* empty */
    LineInit(gBustupSlotPositions[gBustup.selectSlot].x, gBustupSlotPositions[gBustup.selectSlot].y,
             gBustupSlotPositions[gBustup.selectSlot].x, gBustupSlotPositions[gBustup.selectSlot].y,
             &gBustup.textBox.cursor);
    gBustup.unk12FA = 5;
    gBustup.unk12F9 = 0;
    {
        /* Matching: the destination address is taken first. */
        u8 *flag = &gBustup.unk136E;
        *flag = gMain.flag4884_3;
    }
    gBustup.speaker = gMain.speaker;
    gBustup.dialogueIndex = gMain.dialogueIndex;
    gBustup.slideDir = 0;
    gBustup.unk1370 = 0;
    Timer_Reset(&gBustup.timer0);
    Timer_Reset(&gBustup.timer1);
}
