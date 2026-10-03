/*
 * Main menu, Record ("DUEL SCORE") screen, opponent-select drawing helpers and calendar date math.
 *
 *  - Main menu: CB_MainMenu runs gMainMenuSteps (Init, FadeIn, HandleInput, Launch); Launch installs
 *    gMainMenuTable[gMainMenuCursor] (enum MainMenuItem) as the scene callback.
 *  - Record screen (main-menu item 3): CB_Record runs gRecordSteps (Init, LoadGfx, FadeIn, HandleInput,
 *    FadeOut). Each page lists five duelists (the last page four) with their win/draw/loss counts from
 *    gSaveData.duelRecords. BG1 (names, portraits, numbers) and BG2 (row frames) are 512 pixels wide: a page
 *    turn draws the new page into the hidden 256-pixel half and scrolls to it in 16 frames.
 *  - Opponent select (Campaign, steps in campaign_select.c): the cursor with its afterimage trail, two-digit
 *    numbers and the selected duelist's name banner and Win/Lose/Draw record.
 *  - Date helpers used by the Calendar and the Campaign: leap years, days per month, weekdays and the
 *    Japanese public holidays of 2000-2002.
 */
#include "global.h"
#include "gba.h"                /* REG_*, keys, DISPCNT/BGCNT/BLDCNT bits, palette and VRAM addresses */
#include "main.h"               /* gMain */
#include "util.h"               /* StrLen, MemClear16, MemCopy16, CopyDoubleWords */
#include "palette.h"            /* FadeToBlack, FadeFromBlack, SetBrightnessBlack */
#include "bg.h"                 /* LoadBgImage, LoadBgImage4bppToMap, FillMapRect, ResetVideo */
#include "sprite.h"             /* AddSprite, AddSprite8bpp, AddSprite8bppAlpha, enum SpriteShape */
#include "save.h"               /* gSaveData.duelRecords */
#include "sound.h"              /* PlaySE, PlayBGMNoTrack, FadeOutBGM */
#include "calendar.h"           /* the date helpers defined here, gDaysPerMonth, HOLIDAY_*, WEEKDAY_* */
#include "campaign.h"           /* gOpponentSelect, OpponentSelect_*, IsCampaignLevelNUnlocked, IsOpponentUnlocked */
#include "main_menu.h"          /* gMainMenuCursor, gRecordScreen, MainMenu_*, Record_* */
#include "constants/game.h"     /* DUELIST_* */
#include "constants/sound.h"    /* SE_* */

/*
 * Transitional, until H0 (build/readability/HEADERS.md) installs the new include/gba.h, main.h and sound.h:
 * the legacy headers lack these names. Each one has the new header's value or prototype, and the block is
 * skipped once the new gba.h is in place (it defines DISPCNT_OBJ_ON). Delete it after H0.
 */
#ifndef DISPCNT_OBJ_ON
#define DISPCNT_BG1_ON          0x0200
#define DISPCNT_BG_ALL_ON       0x0F00
#define DISPCNT_OBJ_ON          0x1000
#define BGCNT_PRIORITY(n)       (n)
#define BGCNT_CHARBASE(n)       ((n) << 2)
#define BGCNT_256COLOR          0x0080
#define BGCNT_SCREENBASE(n)     ((n) << 8)
#define BGCNT_TXT512x256        0x4000
#define BLDCNT_EFFECT_BLEND     0x0040
#define BLDCNT_TGT2_BG0         0x0100
#define BLDCNT_TGT2_BG1         0x0200
#define BLDCNT_TGT2_BG2         0x0400
#define BLDCNT_TGT2_BG3         0x0800
#define BLDALPHA_BLEND(eva, evb) (((evb) << 8) | (eva))
#define OAM_ATTR2_PALETTE(n)    ((n) << 12)
#define VBLANK_COPY_OAM         0x1     /* enum VBlankFlag in the new main.h */
#define VBLANK_COPY_BG_MAPS     0x2
#define VBLANK_BG1_HOFS         0x20
#define VBLANK_BG2_HOFS         0x40
#define subStep step488A                /* gMain +0x488A bits 4-11; the legacy main.h calls it step488A */
void SetMainCallback(u16 (*callback)(void));
void ResetBgScroll(void);
void PlaySE(u32 seId);
void PlayBGMNoTrack(u32 songId);
void FadeOutBGM(void);
#endif

/* ---- Local data: ROM tables and images only this unit uses ---- */

typedef u16 (*StepFunc)(void);
extern const StepFunc gMainMenuTable[MAIN_MENU_COUNT]; /* 0x081984D8: scene callback per enum MainMenuItem */
extern const StepFunc gMainMenuSteps[];            /* 0x081984F4: CB_MainMenu steps, NULL-terminated */
extern const StepFunc gRecordSteps[];              /* 0x08198588: CB_Record steps, NULL-terminated */

/* 0x081983C0: idle wobble of the opponent-select cursor, (x, y) offsets within +-4 px, 32 frames. */
extern const struct Coords16 gOpponentCursorWobble[32];

/* 0x08198508: BG1/BG2 HOFS during a Record page scroll, [shownHalf][scrollDir - 1][scrollTimer]; the timer
 * counts down, so each row runs from its last value to its first (e.g. half 0, next page: 2 ... 256). It has
 * to be indexed as a real 3-D array: that keeps the ROM's separate `scrollDir - 1`. */
extern const u16 gRecordScrollHofs[2][2][16];
/* 0x08198618: OBJ tile of each frame of the result-marker animation (1, 3, 5, 7, 7, 5, 3, 1). */
extern const u16 gRecordMarkerAnimTiles[8];
/* 0x081985A0: portrait image per duelist (index duelistId - 1); entry 24 is gRecordUnknownPortraitImage. */
extern u16 *const gRecordPortraitImages[25];
/* 0x08198604: column of duelist name plates per page. */
extern u16 *const gRecordPageNameImages[5];

/* Image packs (palette, tiles and map, read by the LoadBgImage* loaders; non-const like the loaders' u16 *
 * parameter) and raw palettes and tiles. */
extern u16 gMainMenuSkyImage[];                    /* BG1 background of the main menu */
extern const u8 gMainMenuObjPal[], gMainMenuObjGfx[];  /* "MENU" header and item labels, plain and highlighted */
extern u16 gRecordFrameImage[];                    /* BG0: screen frame with the "DUEL SCORE" title */
extern u16 gRecordBgPatternImage[];                /* BG3: repeating "DUEL SCORE" pattern */
extern u16 gRecordRows5Image[], gRecordRows4Image[];   /* BG2: WIN/DRAW/LOSE frames for 5 or 4 rows */
extern u16 gRecordUnknownPortraitImage[];          /* "???" shown for a locked duelist */
extern const u8 gRecordMarkerObjPal[], gRecordMarkerObjGfx[];  /* result markers: OBJ palette 1, tiles 0x00 */
extern const u8 gRecordArrowObjPal[];              /* page arrows: OBJ palette 0 */
extern const u8 gRecordArrowObjGfxTop[], gRecordArrowObjGfxBottom[];  /* arrow tiles 0x20 and 0x40 (2D map) */
extern const u8 gRecordDigitPal[], gRecordDigitGfx[];  /* digits 0-9: BG palette 0, BG tiles 4-13 */

/* ---- Local views (matching choices, see build/readability/HEADERS.md) ---- */

/* Record_DrawPage passes its computed arguments without the u16 narrowing the real prototype
 * (u32, u16, u16, u16, u16 *) would add at the call. */
u16 LoadBgImage4bppToMapWide(u32 map, u32 mapOffset, u32 palStart, u32 tileBase, u16 *pack)
    asm("LoadBgImage4bppToMap");
/* Record_DrawPage passes its duelist id (`index + 1`) untruncated: it saw an int parameter (the definition
 * takes u16). */
s32 IsOpponentUnlockedInt(s32 duelistId) asm("IsOpponentUnlocked");
/* FadeToBlack returns u32; MainMenu_Launch tests its result as a halfword (lsl #16), as through this u16 view. */
u16 FadeToBlackU16(u16 step) asm("FadeToBlack");

/* ---- Tiles ---- */

/* OBJ tiles of the opponent-select screen, loaded by campaign_select.c (the digits, labels and name banners
 * by OpponentSelect_LoadPage); all but the cursor use OBJ palette 3. */
enum {
    OPPSEL_TILE_CURSOR = 0x104,         /* 8bpp cursor emblem, an AddSprite8bpp tile number */
    OPPSEL_TILE_DIGITS = 0x280,         /* bright digits 0-9 */
    OPPSEL_TILE_LABEL_WIN = 0x28A,      /* "Win", "Lose", "Draw" (gWinLoseDrawLabels) */
    OPPSEL_TILE_LABEL_LOSE = 0x28E,
    OPPSEL_TILE_LABEL_DRAW = 0x292,
    OPPSEL_TILE_DIGIT_DIM = 0x2A0,      /* dim digits 0-9; the dim 0 stands in for a missing tens digit */
    OPPSEL_TILE_NAME_BANNERS = 0x2C0,   /* + slot * 0x40: the name of each slot's duelist, 96 x 16 px */
};

/* BG tile of digit 0 on the Record screen (gRecordDigitGfx, loaded by Record_LoadGfx). */
#define RECORD_TILE_DIGITS 4

/* ---- Opponent select (Campaign) ---- */

/*
 * Moves the opponent-select cursor towards `slot` and draws it. The cursor keeps its last 8 positions:
 * [0] is the head, [1..7] are drawn as semi-transparent afterimages. A new slot starts a 15-frame ease from
 * the current head to the slot's portrait (+16, 0); when it ends, the head wobbles around the target.
 * hideTrail != 0 draws only the head and leaves BLDCNT alone (the fade-in and page-turn steps own it).
 * Then the selection ring and the page arrows.
 */
void OpponentSelect_DrawCursor(s32 slot, u16 hideTrail)
{
    s32 i;

    for (i = 7; i != 0; i--) {
        gOpponentSelect.trailX[i] = gOpponentSelect.trailX[i - 1];
        gOpponentSelect.trailY[i] = gOpponentSelect.trailY[i - 1];
    }
    gOpponentSelect.targetX = gOpponentSelectSlotPos[slot].x + 0x10;
    gOpponentSelect.targetY = gOpponentSelectSlotPos[slot].y;
    if (slot != gOpponentSelect.targetSlot) {
        gOpponentSelect.targetSlot = slot;
        gOpponentSelect.moveTimer = 15;
        gOpponentSelect.unk13 = 0;
        gOpponentSelect.moveStartX = gOpponentSelect.trailX[0];
        gOpponentSelect.moveStartY = gOpponentSelect.trailY[0];
    }
    if (gOpponentSelect.moveTimer) {
        /* head = target + (start - target) * timer / 16 */
        s32 dx = gOpponentSelect.moveStartX - gOpponentSelect.targetX;
        s32 dy = gOpponentSelect.moveStartY - gOpponentSelect.targetY;
        dx *= gOpponentSelect.moveTimer;
        dy *= gOpponentSelect.moveTimer;
        dx /= 16;
        dy /= 16;
        gOpponentSelect.trailX[0] = gOpponentSelect.targetX + dx;
        gOpponentSelect.trailY[0] = gOpponentSelect.targetY + dy;
        gOpponentSelect.moveTimer--;
    } else {
        gOpponentSelect.trailX[0] = gOpponentSelect.targetX + gOpponentCursorWobble[(gMain.frameCounter >> 1) & 0x1F].x;
        gOpponentSelect.trailY[0] = gOpponentSelect.targetY + gOpponentCursorWobble[(gMain.frameCounter >> 1) & 0x1F].y;
    }
    for (i = 0; i <= 7; i++) {
        if (i == 0) {
            /* the head, opaque */
            if (hideTrail == 0) {
                REG_BLDCNT = 0;
                REG_BLDALPHA = 0;
            }
            AddSprite8bpp((gOpponentSelect.trailY[0] << 16) | gOpponentSelect.trailX[0], SPRITE_SHAPE_32x16,
                          OPPSEL_TILE_CURSOR);
        } else if (hideTrail == 0) {
            /* Afterimages: 50% blend over BG0-3. Matching: x term first here, y term first for the head. */
            REG_BLDCNT = BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3;
            REG_BLDALPHA = BLDALPHA_BLEND(8, 8);
            AddSprite8bppAlpha(gOpponentSelect.trailX[i] | (gOpponentSelect.trailY[i] << 16), SPRITE_SHAPE_32x16,
                               OPPSEL_TILE_CURSOR);
        }
    }
    OpponentSelect_DrawSelectionRing(gOpponentSelectSlotPos[slot].x, gOpponentSelectSlotPos[slot].y);
    OpponentSelect_DrawPageArrows();
}

/* Draws value, clamped to 0-99, as two 8x8 digit sprites at (x, y); a missing tens digit is a dimmed 0. */
void OpponentSelect_DrawNumber(s32 x, s32 y, s32 value)
{
    s32 v = value;

    if (v > 99)
        v = 99;
    if (v < 0)
        v = 0;
    AddSprite((x + 8) | (y << 16), SPRITE_SHAPE_8x8, ((v % 10) + OPPSEL_TILE_DIGITS) | OAM_ATTR2_PALETTE(3));
    v /= 10;
    if (v > 0)
        AddSprite(x | (y << 16), SPRITE_SHAPE_8x8, ((v % 10) + OPPSEL_TILE_DIGITS) | OAM_ATTR2_PALETTE(3));
    else
        AddSprite(x | (y << 16), SPRITE_SHAPE_8x8, OPPSEL_TILE_DIGIT_DIM | OAM_ATTR2_PALETTE(3));
}

/*
 * For duelistId 1-24: draws the "Win", "Lose" and "Draw" labels, each followed by its two-digit count from
 * gSaveData.duelRecords, as one line at y = 0x27 centred on x = 0x78 (5 px per label character plus 16 px
 * per number), then the name banner of the selected slot (three 32x16 sprites at y = 10). Other ids draw
 * nothing.
 */
void OpponentSelect_DrawDuelistInfo(u16 duelistId)
{
    s32 x;
    s32 width;
    s32 i;
    u32 y;

    width = 2;
    for (i = 0; i < 3; i++)
        width += 0x10 + StrLen(gWinLoseDrawLabels[i]) * 5;
    x = 0x78 - width / 2;
    if ((u16)(duelistId - DUELIST_YUGI) > DUELIST_GRANDPA - DUELIST_YUGI)
        return;
    y = 0x27 << 16;
    AddSprite(x | y, SPRITE_SHAPE_32x16, OPPSEL_TILE_LABEL_WIN | OAM_ATTR2_PALETTE(3));
    x += StrLen(gWinLoseDrawLabels[0]) * 5;
    OpponentSelect_DrawNumber(x, 0x28, gSaveData.duelRecords[duelistId].wins);
    x += 0x11;
    AddSprite(x | y, SPRITE_SHAPE_32x16, OPPSEL_TILE_LABEL_LOSE | OAM_ATTR2_PALETTE(3));
    x += StrLen(gWinLoseDrawLabels[1]) * 5;
    OpponentSelect_DrawNumber(x, 0x28, gSaveData.duelRecords[duelistId].losses);
    x += 0x11;
    AddSprite(x | y, SPRITE_SHAPE_32x16, OPPSEL_TILE_LABEL_DRAW | OAM_ATTR2_PALETTE(3));
    x += StrLen(gWinLoseDrawLabels[2]) * 5;
    OpponentSelect_DrawNumber(x, 0x28, gSaveData.duelRecords[duelistId].draws);
    AddSprite((10 << 16) | 0x48, SPRITE_SHAPE_32x16,
              (gOpponentSelect.cursor * 0x40 + OPPSEL_TILE_NAME_BANNERS) | OAM_ATTR2_PALETTE(3));
    AddSprite((10 << 16) | 0x68, SPRITE_SHAPE_32x16,
              (gOpponentSelect.cursor * 0x40 + OPPSEL_TILE_NAME_BANNERS + 4) | OAM_ATTR2_PALETTE(3));
    AddSprite((10 << 16) | 0x88, SPRITE_SHAPE_32x16,
              (gOpponentSelect.cursor * 0x40 + OPPSEL_TILE_NAME_BANNERS + 8) | OAM_ATTR2_PALETTE(3));
}

/* ---- Main menu ---- */

/*
 * Draws the "MENU" header (four 32x16 sprites at y = 10) and the 7 item rows (four sprites each, 16 px
 * apart from y = 0x1D). Row i uses OBJ tiles 0x40 + i * 0x40; the row under the cursor uses the highlighted
 * labels 0x10 tiles further.
 */
void MainMenu_DrawItems(void)
{
    s32 i;
    u32 x;

    AddSprite((10 << 16) | 0x38, SPRITE_SHAPE_32x16, 0);
    AddSprite((10 << 16) | 0x58, SPRITE_SHAPE_32x16, 4);
    AddSprite((10 << 16) | 0x78, SPRITE_SHAPE_32x16, 8);
    AddSprite((10 << 16) | 0x98, SPRITE_SHAPE_32x16, 12);
    for (i = 0, x = 0x38; i < MAIN_MENU_COUNT; i++) {
        u16 tile = i * 0x40 + 0x40;
        if (gMainMenuCursor == i)
            tile += 0x10;
        AddSprite(((i * 16 + 0x1D) << 16) | x, SPRITE_SHAPE_32x16, tile);
        AddSprite(((i * 16 + 0x1D) << 16) | 0x58, SPRITE_SHAPE_32x16, tile + 4);
        AddSprite(((i * 16 + 0x1D) << 16) | 0x78, SPRITE_SHAPE_32x16, tile + 8);
        AddSprite(((i * 16 + 0x1D) << 16) | 0x98, SPRITE_SHAPE_32x16, tile + 12);
    }
}

/*
 * Main menu step 0, one sub-state per frame (gMain.seqState1):
 *   0: screen off, keep the cursor in range;
 *   1: black screen, video and scroll reset, BG1 = 8bpp tiles in charblock 1 with map 0, OAM and map copies
 *      on, menu music;
 *   2: OBJ palette and tiles, the sky image on BG1; returns 1 (done).
 */
u16 MainMenu_Init(void)
{
    /* Matching: the ROM keeps &gMain and &gMain.seqState1 in registers; direct gMain accesses do not. */
    struct Main *main = &gMain;
    u8 *state = &main->seqState1;

    switch (*state) {
    case 0:
        REG_DISPCNT = 0;
        gMainMenuCursor %= MAIN_MENU_COUNT;
        break;
    case 1:
        SetBrightnessBlack();
        ResetVideo();
        ResetBgScroll();
        REG_BG1CNT = BGCNT_256COLOR | BGCNT_CHARBASE(1);
        main->vblankFlags = VBLANK_COPY_OAM | VBLANK_COPY_BG_MAPS;
        PlayBGMNoTrack(3);  /* main menu music */
        break;
    default:
        CopyDoubleWords((void *)OBJ_PLTT, gMainMenuObjPal, 0x20);
        CopyDoubleWords((void *)OBJ_VRAM0, gMainMenuObjGfx, 0x4000);
        LoadBgImage(0, 0, 0x20, gMainMenuSkyImage);
        return 1;
    }
    (*state)++;
    return 0;
}

/* Main menu step 1: BG1 and OBJ on, draw the items, fade in; returns 1 when the fade is done. */
u16 MainMenu_FadeIn(void)
{
    REG_DISPCNT = DISPCNT_BG1_ON | DISPCNT_OBJ_ON;
    MainMenu_DrawItems();
    return FadeFromBlack(1);
}

/* Main menu step 2: Up/Down move the cursor, wrapping over the 7 items; A confirms (returns 1). */
u16 MainMenu_HandleInput(void)
{
    MainMenu_DrawItems();
    if (gMain.newKeys & DPAD_UP) {
        gMainMenuCursor += MAIN_MENU_COUNT - 1;     /* previous item */
        gMainMenuCursor %= MAIN_MENU_COUNT;
        PlaySE(SE_CURSOR);
    }
    if (gMain.newKeys & DPAD_DOWN) {
        gMainMenuCursor += MAIN_MENU_COUNT + 1;     /* next item */
        gMainMenuCursor %= MAIN_MENU_COUNT;
        PlaySE(SE_CURSOR);
    }
    if (gMain.newKeys & A_BUTTON) {
        PlaySE(SE_CONFIRM);
        FadeOutBGM();
        return 1;
    }
    return 0;
}

/* Main menu step 3: fade to black, then start the scene of the selected item. Always returns 0: the new
 * scene callback replaces CB_MainMenu. */
u16 MainMenu_Launch(void)
{
    MainMenu_DrawItems();
    if (FadeToBlackU16(2)) {
        gMain.subStep = 0;
        SetMainCallback(gMainMenuTable[gMainMenuCursor]);
    }
    return 0;
}

/* Main menu scene callback: runs gMainMenuSteps[gMain.seqIndex1] and moves to the next step (clearing the
 * sub-states) when it returns non-zero. Returns 1 at the NULL end of the table. */
u16 CB_MainMenu(void)
{
    StepFunc step = gMainMenuSteps[gMain.seqIndex1];
    if (step != NULL) {
        if (step()) {
            gMain.seqIndex1++;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

/* ---- Record screen ---- */

/*
 * Record step 0: clears gRecordScreen, black screen, scroll and video reset. BG0 = frame (map 0), BG1 =
 * names/portraits/numbers (maps 1-2), BG2 = row frames (maps 3-4), BG3 = pattern (map 5); BG1 and BG2 are
 * 512x256 for the page scroll, all tiles in charblock 1. Returns 1.
 */
u16 Record_Init(void)
{
    MemClear16(&gRecordScreen, sizeof(gRecordScreen));
    SetBrightnessBlack();
    ResetBgScroll();
    ResetVideo();
    REG_MOSAIC = 0;
    REG_DISPCNT = 0;
    REG_BG0CNT = BGCNT_CHARBASE(1);
    REG_BG1CNT = BGCNT_TXT512x256 | BGCNT_SCREENBASE(1) | BGCNT_CHARBASE(1) | BGCNT_PRIORITY(1);
    REG_BG2CNT = BGCNT_TXT512x256 | BGCNT_SCREENBASE(3) | BGCNT_CHARBASE(1) | BGCNT_PRIORITY(2);
    REG_BG3CNT = BGCNT_SCREENBASE(5) | BGCNT_CHARBASE(1) | BGCNT_PRIORITY(3);
    gMain.vblankFlags = VBLANK_COPY_OAM | VBLANK_COPY_BG_MAPS | VBLANK_BG1_HOFS | VBLANK_BG2_HOFS;
    return 1;
}

/* Record step 1: OBJ palettes and tiles of the result markers and page arrows, the digit tiles, the frame
 * (BG0) and pattern (BG3) images; draws page 0 into half 0 and page 1 into half 1 and resets the scroll
 * state. Returns 1. */
u16 Record_LoadGfx(void)
{
    MemCopy16((void *)(OBJ_PLTT + 0x20), gRecordMarkerObjPal, 0x20);   /* OBJ palette 1 */
    MemCopy16((void *)OBJ_VRAM0, gRecordMarkerObjGfx, 0x200);
    MemCopy16((void *)OBJ_PLTT, gRecordArrowObjPal, 0x20);             /* OBJ palette 0 */
    MemCopy16((void *)(OBJ_VRAM0 + 0x400), gRecordArrowObjGfxTop, 0x200);
    MemCopy16((void *)(OBJ_VRAM0 + 0x800), gRecordArrowObjGfxBottom, 0x200);
    MemCopy16((void *)BG_PLTT, gRecordDigitPal, 0x20);
    MemCopy16((void *)(VRAM + 0x4000 + RECORD_TILE_DIGITS * 0x20), gRecordDigitGfx, 0x140); /* charblock 1 */
    LoadBgImage4bppToMap(0, 0, 0x10, 0x10, gRecordFrameImage);
    LoadBgImage4bppToMap(5, 0, 0x20, 0x80, gRecordBgPatternImage);
    Record_DrawPage(0, 0);
    Record_DrawPage(1, 1);
    gRecordScreen.shownHalf = 0;
    gRecordScreen.scrollDir = RECORD_SCROLL_NONE;
    gRecordScreen.scrollTimer = 0;
    gRecordScreen.page = 0;
    return 1;
}

/* Record step 2: draw the sprites, BG0-3 and OBJ on, fade in; returns 1 when the fade is done. */
u16 Record_FadeIn(void)
{
    Record_DrawSprites();
    REG_DISPCNT = DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
    return FadeFromBlack(2);
}

/* Matching: Record_HandleInput tests the u16 results of Record_HasNextPage/HasPrevPage without narrowing them,
 * as an int (as if they were called undeclared: they are defined after it), so it calls them through this. */
typedef int (*IntReturnFunc)(void);

/*
 * Record step 3. During a page scroll: count scrollTimer down and set BG1/BG2 HOFS from gRecordScrollHofs;
 * at 0, move to the new page and flip shownHalf. Idle: Right or R scrolls to the next page if it is unlocked,
 * Left or L to the previous one (the new page is first drawn into the hidden half); a refused move plays
 * the error sound. A or B returns 1 (leave).
 */
u16 Record_HandleInput(void)
{
    Record_DrawSprites();
    if (gRecordScreen.scrollDir) {
        if (gRecordScreen.scrollTimer) {
            gRecordScreen.scrollTimer--;
            gMain.bgHofs[1] =
                gRecordScrollHofs[gRecordScreen.shownHalf][gRecordScreen.scrollDir - 1][gRecordScreen.scrollTimer];
            gMain.bgHofs[2] =
                gRecordScrollHofs[gRecordScreen.shownHalf][gRecordScreen.scrollDir - 1][gRecordScreen.scrollTimer];
        } else {
            switch (gRecordScreen.scrollDir) {
            case RECORD_SCROLL_NEXT:
                gRecordScreen.page++;
                break;
            case RECORD_SCROLL_PREV:
                gRecordScreen.page--;
                break;
            }
            gRecordScreen.scrollDir = RECORD_SCROLL_NONE;
            gRecordScreen.shownHalf = 1 - gRecordScreen.shownHalf;
            if (gRecordScreen.shownHalf) {
                gMain.bgHofs[1] = 0x100;    /* right half */
                gMain.bgHofs[2] = 0x100;
            } else {
                gMain.bgHofs[1] = 0;
                gMain.bgHofs[2] = 0;
            }
        }
    } else {
        if (gMain.newKeys & (R_BUTTON | DPAD_RIGHT)) {
            if (((IntReturnFunc)Record_HasNextPage)()) {
                Record_DrawPage(gRecordScreen.page + 1, 1 - gRecordScreen.shownHalf);
                gRecordScreen.scrollDir = RECORD_SCROLL_NEXT;
                gRecordScreen.scrollTimer = 16;
                PlaySE(SE_CURSOR);
                return 0;
            }
            PlaySE(SE_ERROR);
        }
        if (gMain.newKeys & (L_BUTTON | DPAD_LEFT)) {
            if (((IntReturnFunc)Record_HasPrevPage)()) {
                Record_DrawPage(gRecordScreen.page - 1, 1 - gRecordScreen.shownHalf);
                gRecordScreen.scrollDir = RECORD_SCROLL_PREV;
                gRecordScreen.scrollTimer = 16;
                PlaySE(SE_CURSOR);
                return 0;
            }
            PlaySE(SE_ERROR);
        }
    }
    if (gMain.newKeys & (A_BUTTON | B_BUTTON)) {
        PlaySE(SE_CANCEL);
        return 1;
    }
    return 0;
}

/* Record step 4: draw the sprites and fade to black; returns 1 when black. */
u16 Record_FadeOut(void)
{
    Record_DrawSprites();
    return FadeToBlack(2);
}

/* Record scene callback: runs gRecordSteps[gMain.seqState2] and moves to the next step when it returns
 * non-zero (without resetting the sub-states). Returns 1 at the NULL end of the table. */
u16 CB_Record(void)
{
    StepFunc step = gRecordSteps[gMain.seqState2];
    if (step != NULL) {
        if (step())
            gMain.seqState2++;
        return 0;
    }
    return 1;
}

/* Returns 1 if the shown page is not the first. */
u16 Record_HasPrevPage(void)
{
    if (gRecordScreen.page)
        return 1;
    return 0;
}

/* Returns whether the page after the shown one is unlocked: the same campaign-level tests that open the
 * opponent-select pages. The last page (4) has no next page. */
u16 Record_HasNextPage(void)
{
    switch (gRecordScreen.page) {
    case 0:
        return IsCampaignLevel2Unlocked();
    case 1:
        return IsCampaignLevel3Unlocked();
    case 2:
        return IsCampaignLevel4Unlocked();
    case 3:
        return IsCampaignLevel5Unlocked();
    }
    return 0;
}

/* Draws the left (x = 0x10) and right (x = 0xE0) page arrows at y = 0x20 when there is a page that way;
 * 16x16 sprites animated over 4 frames of 8 ticks. */
void Record_DrawPageArrows(void)
{
    u32 frame = (gMain.frameCounter >> 3) & 3;
    if (Record_HasPrevPage())
        AddSprite((0x20 << 16) | 0x10, SPRITE_SHAPE_16x16, frame * 4 + 0x20);
    if (Record_HasNextPage())
        AddSprite((0x20 << 16) | 0xE0, SPRITE_SHAPE_16x16, frame * 4 + 0x22);
}

/*
 * Unless a page scroll runs, puts an animated 16x8 marker on each row of the shown page: under WIN
 * (x = 0x54) if wins > losses, under LOSE (x = 0xB4) if losses > wins, else under DRAW (x = 0x84). Rows are
 * 24 px apart from y = 0x2B.
 */
void Record_DrawResultMarkers(void)
{
    s32 i, rowCount;
    u32 frame;
    s32 diff, column;

    if (gRecordScreen.scrollDir)
        return;
    rowCount = (gRecordScreen.page <= 3) ? 5 : 4;
    frame = (gMain.frameCounter >> 3) & 7;
    for (i = 0; i < rowCount; i++) {
        diff = gSaveData.duelRecords[gRecordScreen.page * 5 + i + 1].wins
             - gSaveData.duelRecords[gRecordScreen.page * 5 + i + 1].losses;
        /* column = -1 (WIN), 0 (DRAW) or 1 (LOSE) */
        column = diff < 0;
        if (diff > 0)
            column = -1;
        AddSprite(((i * 24 + 0x2B) << 16) | (0x84 + column * 0x30), SPRITE_SHAPE_16x8,
                  gRecordMarkerAnimTiles[frame] + OAM_ATTR2_PALETTE(1));
    }
}

/* Per-frame sprites of the Record screen. */
void Record_DrawSprites(void)
{
    Record_DrawResultMarkers();
    Record_DrawPageArrows();
}

/* Writes value as 3 decimal digits (leading zeros) into gMain.bgMapBuffer[screenblock], the last digit at
 * (tileX + 2, tileY). */
void Record_DrawNumber(s32 screenblock, s32 tileX, s32 tileY, s32 value)
{
    s32 i;

    tileX += 2;
    for (i = 0; i < 3; i++) {
        /* Matching: both coordinates are truncated to 16 bits. */
        gMain.bgMapBuffer[screenblock][(u16)tileX + (u16)tileY * 32] = value % 10 + RECORD_TILE_DIGITS;
        value /= 10;
        tileX--;
    }
}

/*
 * Draws `page` into BG half `half` (screenblock 1 + half of BG1 and 3 + half of BG2): clears both maps, puts
 * the 5- or 4-row WIN/DRAW/LOSE frame on BG2 and the page's column of name plates on BG1, then for each row
 * (3 map rows from row 3) the portrait and the wins/draws/losses (3 digits) of duelist page * 5 + row + 1
 * if it is unlocked, else the "???" portrait and no name. Each half has its own tiles (from half * 0xC0)
 * and each portrait its own palette (bank 5 + row, + 5 in half 1).
 */
void Record_DrawPage(s32 page, s32 half)
{
    u16 tileBase = half * 0xC0;
    s32 row, rowCount;

    FillMapRect(half + 1, 0, 0x20, 0x20);
    FillMapRect(half + 3, 0, 0x20, 0x20);
    /* Matching: the ternary must already have the parameter's type (a conversion around it makes agbcc
     * evaluate it into a pseudo instead of storing each arm straight into the stack slot), and the map row
     * in `(u16)(row * 3 + 4) * 32 + 4` below is truncated to 16 bits. */
    LoadBgImage4bppToMapWide(half + 3, 0x63, 0x30, tileBase + 0xA0,
                             (page <= 3) ? gRecordRows5Image : gRecordRows4Image);
    LoadBgImage4bppToMapWide(half + 1, 0x63, 0x40, tileBase + 0xC0, gRecordPageNameImages[page]);
    rowCount = 4;
    if (page <= 3)
        rowCount = 5;
    for (row = 0; row < rowCount; row++) {
        u16 index = page * 5 + row;     /* duelistId - 1 */

        /* name on map row row * 3 + 3, portrait from cell (4, row * 3 + 4), numbers on map row row * 3 + 5 */
        if (IsOpponentUnlockedInt(index + 1)) {
            LoadBgImage4bppToMapWide(half + 1, (u16)(row * 3 + 4) * 32 + 4, (row + 5 + half * 5) * 16,
                                     tileBase + 0xE0 + row * 12, gRecordPortraitImages[index]);
            Record_DrawNumber(half + 1, 0xD, row * 3 + 5, gSaveData.duelRecords[index + 1].wins);
            Record_DrawNumber(half + 1, 0x13, row * 3 + 5, gSaveData.duelRecords[index + 1].draws);
            Record_DrawNumber(half + 1, 0x19, row * 3 + 5, gSaveData.duelRecords[index + 1].losses);
        } else {
            LoadBgImage4bppToMapWide(half + 1, (u16)(row * 3 + 4) * 32 + 4, (row + 5 + half * 5) * 16,
                                     tileBase + 0xE0 + row * 12, gRecordUnknownPortraitImage);
            FillMapRect(half + 1, row * 0x60 + 0x63, 8, 1);    /* the name: 8 x 1 cells at (3, row * 3 + 3) */
        }
    }
}

/* ---- Date helpers (Calendar, Campaign) ---- */

/* 1 if year is a Gregorian leap year, else 0. */
u32 IsLeapYear(u32 year)
{
    if ((year & 3) != 0 || (year % 100 == 0 && year % 400 != 0))
        return 0;
    return 1;
}

/* Number of days in month (1-12) of year. */
u32 GetDaysInMonth(u32 year, u32 month)
{
    u32 days = gDaysPerMonth[month - 1];
    if (month == 2)
        days += IsLeapYear(year);
    return days;
}

/*
 * Weekday (enum Weekday, 0 = Sunday) of a date from 2000 on. Counts the days since 2000-01-01, a Saturday:
 * 1461 per 4-year cycle, then 365 per year of the cycle plus the leap day of the cycle's first year (the
 * +1, taken back if the date is in that year), then the months.
 */
s32 GetDayOfWeek(u32 year, u32 month, u32 day)
{
    u32 m;
    s32 days = day - 1;
    u32 yearInCycle = year & 3;

    if (year >= 2000)
        year -= 2000;
    days += (year / 4) * 1461;
    days += 1 + yearInCycle * 365;
    if ((year & 3) == 0)
        days--;
    for (m = 1; m < month; m++) {
        days += gDaysPerMonth[m - 1];
        if (m == 2)
            days += IsLeapYear(year + 2000);
    }
    return (days + 6) % 7;
}

/*
 * HolidayFlag bits of a date: the Japanese public holidays of 2000-2002 (fixed dates, Coming of Age Day and
 * Sports Day on the 2nd Monday; no equinox days) plus Feb 24 and Jul 7.
 */
u32 GetHolidayFlags(u32 year, u32 month, u32 day)
{
    u32 flags = 0;

    switch (month) {
    case 1:
        if (day == 1)
            flags |= HOLIDAY_NEW_YEARS_DAY;
        /* Two calls whose results are unused; the ROM makes them. */
        GetDayOfWeek(year, month, 1);
        GetDayOfWeek(year, month, day);
        if ((day - 1) / 7 == 1 && GetDayOfWeek(year, month, day) == WEEKDAY_MONDAY)  /* 2nd Monday */
            flags |= HOLIDAY_COMING_OF_AGE_DAY;
        break;
    case 2:
        if (day == 11)
            flags |= HOLIDAY_FOUNDATION_DAY;
        if (day == 24)
            flags |= HOLIDAY_FEB_24;
        break;
    case 4:
        if (day == 29)
            flags |= HOLIDAY_GREENERY_DAY;
        break;
    case 5:
        switch (day) {
        case 3:
            flags |= HOLIDAY_CONSTITUTION_DAY;
            break;
        case 4:
            flags |= HOLIDAY_NATIONAL_HOLIDAY_MAY4;
            break;
        case 5:
            flags |= HOLIDAY_CHILDRENS_DAY;
            break;
        }
        break;
    case 7:
        if (day == 20)
            flags |= HOLIDAY_MARINE_DAY;
        if (day == 7)
            flags |= HOLIDAY_JUL_7;
        break;
    case 9:
        if (day == 15)
            flags |= HOLIDAY_RESPECT_FOR_AGED_DAY;
        break;
    case 10:
        GetDayOfWeek(year, month, 1);   /* unused, as in January */
        GetDayOfWeek(year, month, day);
        if ((day - 1) / 7 == 1 && GetDayOfWeek(year, month, day) == WEEKDAY_MONDAY)  /* 2nd Monday */
            flags |= HOLIDAY_SPORTS_DAY;
        break;
    case 11:
        switch (day) {
        case 3:
            flags |= HOLIDAY_CULTURE_DAY;
            break;
        case 23:
            flags |= HOLIDAY_LABOR_THANKSGIVING_DAY;
            break;
        }
        break;
    case 12:
        if (day == 23)
            flags |= HOLIDAY_EMPERORS_BIRTHDAY;
        break;
    }
    return flags;
}

/* 1 if the date is a day off: a Sunday, a holiday, or a Monday after a holiday (substitute holiday). The
 * day before a Monday the 1st is looked up as day 0 of the same month, which is never a holiday. */
u32 IsDayOff(u32 year, u32 month, u32 day)
{
    if (GetDayOfWeek(year, month, day) == WEEKDAY_SUNDAY
        || (GetHolidayFlags(year, month, day) & HOLIDAY_MASK)
        || (GetDayOfWeek(year, month, day) == WEEKDAY_MONDAY && (GetHolidayFlags(year, month, day - 1) & HOLIDAY_MASK)))
        return 1;
    return 0;
}
