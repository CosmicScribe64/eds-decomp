/*
 * Two menu screens built the same way: a NULL-terminated step table run by a small runner, a Mode-4
 * bitmap background and OBJ sprites redrawn every frame.
 *
 *   Calendar (0x08002388-0x08002980, main-menu slot 4, view only): draws the month of
 *   gCalendar.viewDate (year, day numbers coloured by weekday, event icons, today marker); L/R change the
 *   month, the D-pad moves the day cursor, A or B leave. Steps: gCalendarSteps, run by CB_Calendar.
 *
 *   Campaign opponent select (0x08002980-0x080034B8): five pages of up to five opponents arranged in a
 *   ring of portraits. Locked opponents get a stone cover and the name "[ Unknown ]"; L/R slide and fade to
 *   the previous/next page, A picks an unlocked opponent into gMain.opponent, B leaves the Campaign for the
 *   main menu. Steps: gOpponentSelectSteps (enum OpponentSelectStep), run by OpponentSelect_Run, which the
 *   Campaign step Campaign_SelectOpponent calls every frame. The cursor and record drawing
 *   (OpponentSelect_DrawCursor, OpponentSelect_DrawDuelistInfo) live in main_menu.c.
 */
#include "global.h"
#include "gba.h"                /* IO registers, DMA, DISPCNT, OAM flip bits */
#include "main.h"               /* gMain, SetMainCallback, ResetBgScroll */
#include "sound.h"              /* PlaySE */
#include "constants/sound.h"    /* SE_* */
#include "util.h"               /* MemCopy16, StrLen, struct Coords16 */
#include "palette.h"            /* SetBrightnessBlack, FadeToBlack, FadeFromBlack */
#include "bg.h"                 /* ResetVideo */
#include "sprite.h"             /* AddSprite*, SPRITE_SHAPE_* */
#include "text.h"               /* TextCanvasInitEx, TextDrawString, TextCanvasToTiles, gSystemFontPal */
#include "save.h"               /* gSaveData.days */
#include "calendar.h"           /* gCalendar, struct Date, date math, CAL_* event bits, Calendar_* */
#include "campaign.h"           /* gOpponentSelect, OPPSEL_STEP_*, OpponentSelect_*, unlock checks */
#include "main_menu.h"          /* CB_MainMenu */

/*
 * Before H0 (build/readability/HEADERS.md) include/gba.h, main.h and sound.h are still the legacy headers,
 * which lack these names. The values are those of the new headers; delete this block once they are
 * installed (see build/readability/issues/campaign_select.md).
 */
#ifndef DISPCNT_MODE_4
#define REG_BG2X            REG32(0x028)
#define DISPCNT_MODE_4      0x0004
#define DISPCNT_BG_ALL_ON   0x0F00
#define DISPCNT_OBJ_ON      0x1000
#define BGCNT_PRIORITY(n)   (n)
#define DMA_SRC_FIXED       0x0100
#define DMA_16BIT           0x0000
#define DMA_ENABLE          0x8000
#define OAM_ATTR1_HFLIP     0x1000
#define OAM_ATTR1_VFLIP     0x2000
#define VBLANK_COPY_OAM     0x1
void SetMainCallback(u16 (*callback)(void));
void ResetBgScroll(void);
void PlaySE(u32 seId);
#endif

/* A scene step: returns nonzero when done, and the runner moves on to the next entry. */
typedef u16 (*StepFunc)(void);

/* ROM data used only by this unit. */
extern StepFunc gCalendarSteps[];               /* Init, FadeIn, HandleInput, FadeOut, NULL */
extern StepFunc gOpponentSelectSteps[];         /* indexed by enum OpponentSelectStep */
extern u16 gOpponentSelectDuelists[];           /* duelist id per [page * 5 + slot]; [20] = 0 (empty slot) */
extern const u8 *gOpponentSelectNames[];        /* display name per [page * 5 + slot] */
extern struct Mode4Bitmap gOpponentSelectPageBgs[5]; /* background picture per page */
extern const u8 gUnknownOpponentName[];         /* "[ Unknown ]", shown for a locked opponent */
extern const u8 gOpponentSelectObjPal[];        /* 8bpp OBJ colours 0-47 */
extern const u8 gOpponentSelectObjTiles[];      /* 8bpp: ring 0x100/0x120, cover 0x108, arrows 0x12C/0x12E */
extern const u8 gOpponentSelectTextPal[];       /* OBJ palette 3: names, labels, digits */
extern const u8 gOpponentSelectDigitTiles[];    /* bright digits 0-9 */
extern const u8 gOpponentSelectDimDigitTiles[]; /* dim digits 0-9 */
extern const u8 gCalendarBgPal[];               /* BG colours 0-239 */
extern const u8 gCalendarBgBitmap[];            /* 240x160 8bpp calendar page */
extern const u8 gCalendarNumberPal[];           /* OBJ palette 6 */
extern const u8 gCalendarNumberTiles[];         /* 4bpp day numbers 1-31: black, red (Sunday), blue rows */
extern const u8 gCalendarWeekdayPal[];          /* OBJ palette 8 */
extern const u8 gCalendarWeekdayTiles[];        /* 4bpp "Sun".."Sat" headers */
extern const u8 gCalendarMonthNamePal[];        /* OBJ palette 7 */
extern const u8 gCalendarIconPal[];             /* 8bpp OBJ colours 0-95 */
extern const u8 gCalendarIconTiles[];           /* 8bpp: event icons 0x170-0x174, today marker 0x17C */

/* Local views kept on purpose (matching choices, see build/readability/HEADERS.md):
 * - the unit calls FadeToBlack / FadeFromBlack as returning u16 (palette.h: u32) and IsOpponentUnlocked as
 *   returning s32 (campaign.h: u16); the return width decides where the results get narrowed, and the
 *   header prototypes do not match;
 * - OpponentSelect_SnapCursor reads the slot positions through a non-const view: with campaign.h's
 *   `const struct Coords16` the compiler treats the loads as unchanging and allocates the loop differently. */
extern u16 FadeToBlackU16(s32 step) asm("FadeToBlack");
extern u16 FadeFromBlackU16(s32 step) asm("FadeFromBlack");
extern s32 IsOpponentUnlockedS32(u16 duelistId) asm("IsOpponentUnlocked");
extern struct Coords16 gOpponentSelectSlotPosRW[5] asm("gOpponentSelectSlotPos");

/* Mode 4: two 240x160 8bpp frames in VRAM; DISPCNT bit 4 selects the one shown. */
#define MODE4_FRAME0        (VRAM)
#define MODE4_FRAME1        (VRAM + 0xA000)
#define MODE4_FRAME_SIZE    (240 * 160)

/* Palette slots and OBJ tile addresses (in the bitmap modes OBJ tiles start at 4bpp tile 0x200). */
#define BG_PALETTE(n)       (BG_PLTT + (n) * 0x20)
#define OBJ_PALETTE(n)      (OBJ_PLTT + (n) * 0x20)
#define OBJ_TILE(n)         (OBJ_VRAM0 + (n) * 0x20)    /* 4bpp tile n, as in attr2 */
#define OBJ_TILE_8BPP(n)    (OBJ_VRAM0 + (n) * 0x40)    /* 8bpp tile n, as passed to AddSprite8bpp */

/* TextDraw* sizeColor: font size in pixels in the high byte, colour index in the low byte. */
#define SIZE_COLOR(size, color) (((size) << 8) | (color))

/* gCalendar.monthNameState once the month-name tiles are loaded; L/R wait for it. */
#define MONTH_NAME_LOADED   3

#define OPPONENTS_PER_PAGE  5
#define LAST_SLOT           (OPPONENTS_PER_PAGE - 1)
#define LAST_PAGE           4   /* only slots 1-4: slot 0 of the last page is empty */
#define PAGE_DUELIST(page, slot) gOpponentSelectDuelists[(page) * OPPONENTS_PER_PAGE + (slot)]

/*
 * Draws the Calendar's sprites for one frame: cursor, arrows and headers (Calendar_DrawCursorAndHeader), the
 * year, then one sprite per day of the shown half of viewDate's month (rows of 7 cells, 3 rows per half) with
 * its event icons and the today marker.
 */
void Calendar_DrawMonth(void)
{
    struct Date shown, today;
    s32 year, x, digits, digit;
    s32 day, cell, col, daysInMonth;
    s32 colorRow, cellX, iconX;
    /* FAKEMATCH: without the pin, day gets r9 and the loop optimiser's row value (derived from iconY and
     * rowY) gets r10, the other way round from the ROM (tools/regoracle.py: that value would need 16 refs
     * instead of 9 to be allocated before day). */
    register s32 rowY asm("r9");
    u32 iconY, dayY;
    u32 rowStep; /* Matching: 24 << 16 in a variable that gets no register, so it is rematerialised per use */
    u32 events;

    Calendar_DrawCursorAndHeader();
    DayCountToDate(&shown, gCalendar.viewDate);
    DayCountToDate(&today, gCalendar.today);

    /* The year at the top right, right to left in 4 digits (tiles 0x240-0x249, OBJ palette 6). */
    year = shown.year;
    x = 0;
    for (digits = 3; digits >= 0; digits--) {
        digit = year % 10;
        AddSprite((0xE0 - x) | (0x10 << 16), SPRITE_SHAPE_8x8, digit + 0x6240);
        year = year / 10;
        x += 6;
    }

    /* cell = grid position (row * 7 + col) of the day; the 1st sits in its weekday's column. */
    day = 1;
    cell = GetDayOfWeek(shown.year, shown.month, day);
    col = cell;
    daysInMonth = GetDaysInMonth(shown.year, shown.month);
    if (gCalendar.secondHalf) {
        /* Skip the first three rows (cells 0-20); the second half starts at cell 0 again. */
        while (cell <= 20) {
            cell++;
            day++;
            col++;
            col %= 7;
        }
        cell = 0;
    }

    if (day > daysInMonth)
        return;
    if (cell > 20)
        return;
    rowStep = 24 << 16;
    iconY = 0x30 << 16;
    dayY = 0x28 << 16;
    rowY = 0;
    cellX = col << 5;
    while (day <= daysInMonth && cell <= 20) {
        events = GetCalendarEvents(shown.year, shown.month, day);

        /* Day number: tile 0x200 + row * 32 + day, OBJ palette 6; the black, red and blue rows are for
         * weekdays, Sundays and Saturdays. */
        colorRow = 0;
        iconX = cellX + 0x18;
        switch (col) {
        case WEEKDAY_SUNDAY:
            colorRow = 1;
            break;
        case WEEKDAY_SATURDAY:
            colorRow = 2;
            break;
        }
        AddSprite((cellX + 0x10) | dayY, SPRITE_SHAPE_8x8, (colorRow << 5) + day + 0x6200);

        /* Event icons, left to right after the number: magazine days, then any duel event. Their y is iconY,
         * written as (rowY + 0x30) << 16 for the second and third. */
        if (events & CAL_WEEKLY_JUMP) {
            AddSprite8bpp(iconX | iconY, SPRITE_SHAPE_16x16, 0x172);
            iconX += 8;
        }
        if (events & CAL_V_JUMP) {
            AddSprite8bpp(((rowY + 0x30) << 16) | iconX, SPRITE_SHAPE_16x16, 0x174);
            iconX += 8;
        }
        if (events & (CAL_DUEL_CEREMONY | CAL_TOURNAMENT_ROUND1 | CAL_TOURNAMENT_ROUND2 | CAL_TOURNAMENT_SEMIFINAL
                      | CAL_TOURNAMENT_FINAL | CAL_SUGOROKU_PRELIM | CAL_SUGOROKU_MATCH)) {
            iconX |= (rowY + 0x30) << 16;
            AddSprite8bpp(iconX, SPRITE_SHAPE_16x16, 0x170);
        }
        if (shown.year == today.year && shown.month == today.month && day == today.day) {
            AddSprite8bpp((cellX + 0xD) | ((rowY + 0x25) << 16), SPRITE_SHAPE_32x32, 0x17C);
        }

        /* Next cell; after Saturday wrap to the next row (24 px lower). */
        cellX += 0x20;
        col++;
        if (col > WEEKDAY_SATURDAY) {
            cellX = 0;
            col = 0;
            iconY += rowStep;
            dayY += rowStep;
            rowY += 24;
        }
        day++;
        cell++;
    }
}

/*
 * Calendar step 0: clears gCalendar, sets today = viewDate = gSaveData.days, blacks out and resets the video
 * (Mode 4), loads the calendar page into both frames, the palettes and the OBJ tiles, and puts the cursor on
 * today. Returns 1.
 */
u16 Calendar_Init(void)
{
    /* DMA3 fill with a zero halfword, then wait, as two separate blocks with their own register pointers,
     * like the SDK's DmaSet and DmaWait macros. dma[0..2] = SAD, DAD, CNT. */
    {
        vu16 zero = 0;
        vu32 *dma = &REG_DMA3SAD;

        dma[0] = (u32)&zero;
        dma[1] = (u32)&gCalendar;
        dma[2] = ((DMA_ENABLE | DMA_SRC_FIXED | DMA_16BIT) << 16) | (sizeof(gCalendar) / 2);
        dma[2];
    }
    {
        vu32 *dmaRegs = &REG_DMA3SAD;

        while (dmaRegs[2] & (DMA_ENABLE << 16))
            ;
    }
    gCalendar.today = gSaveData.days;
    gCalendar.viewDate = gSaveData.days;
    SetBrightnessBlack();
    ResetBgScroll();
    ResetVideo();
    REG_MOSAIC = 0;
    REG_BLDCNT = 0;
    REG_BLDY = 0;
    REG_BG0CNT = BGCNT_PRIORITY(3);
    REG_BG1CNT = BGCNT_PRIORITY(3);
    REG_BG2CNT = BGCNT_PRIORITY(3);
    REG_BG3CNT = BGCNT_PRIORITY(3);
    REG_DISPCNT = DISPCNT_MODE_4;
    gMain.vblankFlags = VBLANK_COPY_OAM;
    MemCopy16((void *)BG_PALETTE(0), gCalendarBgPal, 0x1E0);
    MemCopy16((void *)BG_PALETTE(15), gSystemFontPal, 0x20);
    MemCopy16((void *)MODE4_FRAME0, gCalendarBgBitmap, MODE4_FRAME_SIZE);
    MemCopy16((void *)MODE4_FRAME1, gCalendarBgBitmap, MODE4_FRAME_SIZE);
    MemCopy16((void *)OBJ_PALETTE(6), gCalendarNumberPal, 0x20);
    MemCopy16((void *)OBJ_TILE(0x200), gCalendarNumberTiles, 0xC00);
    MemCopy16((void *)OBJ_PALETTE(8), gCalendarWeekdayPal, 0x20);
    MemCopy16((void *)OBJ_TILE(0x2A0), gCalendarWeekdayTiles, 0x800);
    MemCopy16((void *)OBJ_PALETTE(7), gCalendarMonthNamePal, 0x20);
    gCalendar.monthNameState = 0;
    MemCopy16((void *)OBJ_PALETTE(0), gCalendarIconPal, 0xC0);
    MemCopy16((void *)OBJ_TILE_8BPP(0x170), gCalendarIconTiles, 0x1000);
    Calendar_SetCursorDate(gCalendar.viewDate);
    Calendar_UpdateEventNames();
    return 1;
}

/* Calendar step 1: draws the month, turns on BG0-3 and OBJ; returns 1 when the fade-in is done. */
u16 Calendar_FadeIn(void)
{
    Calendar_DrawMonth();
    REG_DISPCNT = DISPCNT_MODE_4 | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
    return FadeFromBlackU16(4);
}

/*
 * Calendar step 2: draws the month and handles input. Once the month name is loaded, R moves to the 1st
 * of the next month and L to the last day of the previous one (not before January 2001). The D-pad moves
 * the cursor; UP/DOWN past the third row switch between the two halves of the month. A or B leave:
 * returns 1, else 0.
 */
u16 Calendar_HandleInput(void)
{
    struct Date date;

    Calendar_DrawMonth();
    if (gCalendar.monthNameState == MONTH_NAME_LOADED) {
        if (gMain.newKeys & R_BUTTON) {
            /* Back to the last day of the previous month, then forward by this month's length + 1. */
            DayCountToDate(&date, gCalendar.viewDate);
            gCalendar.viewDate -= date.day;
            gCalendar.viewDate = gCalendar.viewDate + GetDaysInMonth(date.year, date.month) + 1;
            gCalendar.monthNameState &= ~3; /* load the new month's name */
            Calendar_UpdateEventNames();
            PlaySE(SE_CURSOR);
        }
        if (gMain.newKeys & L_BUTTON) {
            DayCountToDate(&date, gCalendar.viewDate);
            /* Days 0-30 are January 2001, the first month. */
            if (gCalendar.viewDate > 30) {
                gCalendar.viewDate -= date.day;
                gCalendar.monthNameState &= ~3; /* load the new month's name */
                Calendar_UpdateEventNames();
                PlaySE(SE_CURSOR);
                DayCountToDate(&date, gCalendar.viewDate); /* result unused */
            }
        }
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        gCalendar.cursorCol = (gCalendar.cursorCol + 1) % 7;
        Calendar_UpdateEventNames();
        PlaySE(SE_CURSOR);
    }
    if (gMain.newKeys & DPAD_LEFT) {
        gCalendar.cursorCol = (gCalendar.cursorCol + 6) % 7;
        Calendar_UpdateEventNames();
        PlaySE(SE_CURSOR);
    }
    if (gMain.newKeys & DPAD_UP) {
        if (gCalendar.cursorRow != 0) {
            gCalendar.cursorRow = (gCalendar.cursorRow - 1) & 7;
            Calendar_UpdateEventNames();
            PlaySE(SE_CURSOR);
        } else if (gCalendar.secondHalf) {
            gCalendar.secondHalf = 0;
            gCalendar.cursorRow = 2;
            Calendar_UpdateEventNames();
            PlaySE(SE_CURSOR);
        } else {
            PlaySE(SE_ERROR);
        }
    }
    if (gMain.newKeys & DPAD_DOWN) {
        if (gCalendar.cursorRow <= 1) {
            gCalendar.cursorRow = (gCalendar.cursorRow + 1) & 7;
            Calendar_UpdateEventNames();
            PlaySE(SE_CURSOR);
        } else if (!gCalendar.secondHalf) {
            gCalendar.secondHalf = 1;
            gCalendar.cursorRow = 0;
            Calendar_UpdateEventNames();
            PlaySE(SE_CURSOR);
        } else {
            PlaySE(SE_ERROR);
        }
    }
    if (gMain.newKeys & (A_BUTTON | B_BUTTON)) {
        PlaySE(SE_CANCEL);
        return 1;
    }
    return 0;
}

/* Calendar step 3: returns 1 when the screen is black (the step table then ends). */
u16 Calendar_FadeOut(void)
{
    return FadeToBlackU16(4);
}

/* Calendar scene callback (main-menu slot 4): runs gCalendarSteps[gMain.seqState2], moving to the next step
 * when one returns nonzero; returns 1 at the table's NULL end. */
u16 CB_Calendar(void)
{
    StepFunc step = gCalendarSteps[gMain.seqState2];

    if (step != NULL) {
        if (step())
            gMain.seqState2++;
        return 0;
    }
    return 1;
}

/*
 * Opponent-select step 0: clears gOpponentSelect (page 0, slot 0), blacks out and resets the video (Mode 4),
 * loads the 8bpp OBJ palette and tiles, puts the cursor on its slot and loads the page. Returns 1.
 */
u16 OpponentSelect_Init(void)
{
    /* DMA3 fill with a zero halfword and wait (dma[0..2] = SAD, DAD, CNT). */
    {
        vu16 zero = 0;
        vu32 *dma = &REG_DMA3SAD;

        dma[0] = (u32)&zero;
        dma[1] = (u32)&gOpponentSelect;
        dma[2] = ((DMA_ENABLE | DMA_SRC_FIXED | DMA_16BIT) << 16) | (sizeof(gOpponentSelect) / 2);
        dma[2];
        while (dma[2] & (DMA_ENABLE << 16))
            ;
    }
    SetBrightnessBlack();
    ResetBgScroll();
    ResetVideo();
    REG_MOSAIC = 0;
    REG_BLDCNT = 0;
    REG_BLDY = 0;
    REG_BG0CNT = BGCNT_PRIORITY(3);
    REG_BG1CNT = BGCNT_PRIORITY(3);
    REG_BG2CNT = BGCNT_PRIORITY(3);
    REG_BG3CNT = BGCNT_PRIORITY(3);
    REG_DISPCNT = DISPCNT_MODE_4;
    gMain.vblankFlags = VBLANK_COPY_OAM;
    MemCopy16((void *)OBJ_PALETTE(0), gOpponentSelectObjPal, 0x60);
    MemCopy16((void *)OBJ_TILE_8BPP(0x100), gOpponentSelectObjTiles, 0x1000);
    OpponentSelect_SnapCursor(gOpponentSelect.cursor);
    OpponentSelect_LoadPage(gOpponentSelect.page);
    return 1;
}

/* Opponent-select step 1: draws the cursor (no trail), the covers and the selected duelist's record, turns
 * on BG0-3 and OBJ; returns 1 when the fade-in is done. */
u16 OpponentSelect_FadeIn(void)
{
    OpponentSelect_DrawCursor(gOpponentSelect.cursor, 1);
    OpponentSelect_DrawLockedCovers(0);
    OpponentSelect_DrawDuelistInfo(PAGE_DUELIST(gOpponentSelect.page, gOpponentSelect.cursor));
    REG_DISPCNT = DISPCNT_MODE_4 | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
    return FadeFromBlackU16(4);
}

/*
 * Opponent-select step 2: draws the screen and handles input. The slots form a ring (0 bottom centre, 1 left,
 * 2 top left, 3 top right, 4 right): LEFT moves to slot + 1, RIGHT to slot - 1, skipping the empty slot 0 of
 * the last page. L and R jump to the page-turn steps (R only if the next page is unlocked). A on an unlocked
 * opponent stores it in gMain.opponent and returns 1. B goes to OPPSEL_STEP_CANCEL, which fades out and
 * returns to the main menu. Returns 0 otherwise.
 */
u16 OpponentSelect_HandleInput(void)
{
    u16 nextPageOpen;

    OpponentSelect_DrawCursor(gOpponentSelect.cursor, 0);
    OpponentSelect_DrawLockedCovers(0);
    OpponentSelect_DrawDuelistInfo(PAGE_DUELIST(gOpponentSelect.page, gOpponentSelect.cursor));

    if (gMain.newKeys & DPAD_LEFT) {
        if (gOpponentSelect.cursor < LAST_SLOT)
            gOpponentSelect.cursor++;
        else
            gOpponentSelect.cursor = 0;
        if (gOpponentSelect.page == LAST_PAGE && gOpponentSelect.cursor == 0)
            gOpponentSelect.cursor = 1;
        PlaySE(SE_CURSOR);
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        if (gOpponentSelect.cursor != 0)
            gOpponentSelect.cursor--;
        else
            gOpponentSelect.cursor = LAST_SLOT;
        if (gOpponentSelect.page == LAST_PAGE && gOpponentSelect.cursor == 0)
            gOpponentSelect.cursor = LAST_SLOT;
        PlaySE(SE_CURSOR);
    }
    if (gMain.newKeys & L_BUTTON) {
        if (gOpponentSelect.page != 0) {
            PlaySE(SE_CURSOR);
            gMain.seqIndex1 = OPPSEL_STEP_PREV_PAGE;
            return 0;
        }
        PlaySE(SE_ERROR);
    }
    if (gMain.newKeys & R_BUTTON) {
        /* From page N (counted from 0), page N + 1 opens at campaign level N + 2. */
        nextPageOpen = 0;
        switch (gOpponentSelect.page) {
        case 0:
            nextPageOpen = IsCampaignLevel2Unlocked();
            break;
        case 1:
            nextPageOpen = IsCampaignLevel3Unlocked();
            break;
        case 2:
            nextPageOpen = IsCampaignLevel4Unlocked();
            break;
        case 3:
            nextPageOpen = IsCampaignLevel5Unlocked();
            break;
        }
        if (nextPageOpen) {
            PlaySE(SE_CURSOR);
            gMain.seqIndex1 = OPPSEL_STEP_NEXT_PAGE;
            return 0;
        }
        PlaySE(SE_ERROR);
    }
    if (gMain.newKeys & A_BUTTON) {
        if (IsOpponentUnlockedS32(PAGE_DUELIST(gOpponentSelect.page, gOpponentSelect.cursor))) {
            gMain.opponent = PAGE_DUELIST(gOpponentSelect.page, gOpponentSelect.cursor);
            PlaySE(SE_CONFIRM);
            return 1;
        }
        PlaySE(SE_ERROR);
    }
    if (gMain.newKeys & B_BUTTON) {
        PlaySE(SE_CONFIRM);
        gMain.seqIndex1 = OPPSEL_STEP_CANCEL;
    }
    return 0;
}

/* Opponent-select steps 3 (opponent chosen) and 8 (B): draws the screen; returns 1 when it is black. */
u16 OpponentSelect_FadeOut(void)
{
    OpponentSelect_DrawCursor(gOpponentSelect.cursor, 0);
    OpponentSelect_DrawLockedCovers(0);
    OpponentSelect_DrawDuelistInfo(PAGE_DUELIST(gOpponentSelect.page, gOpponentSelect.cursor));
    return FadeToBlackU16(4);
}

/* Shifts the Mode-4 background left by 2 * scroll pixels: BG2X (20.8 fixed point) = scroll << 9, written
 * as two halfwords. */
void OpponentSelect_SetBgScroll(s32 scroll)
{
    vu16 *reg = (vu16 *)&REG_BG2X;
    u32 value = scroll << 9;

    *reg++ = value;
    *reg = (value >> 16) & 0xFFF;
}

/*
 * Opponent-select step 5 (L): turns to page - 1. gMain.seqState1: 0 = slide the page right while fading to
 * black; 1 = load page - 1; 2 = slide the new page in from the left while fading in, then go back to
 * OPPSEL_STEP_INPUT. The fade level gMain.brightness doubles as the slide offset (2 px per level), and the
 * background and the covers move together. Returns 0.
 */
u16 OpponentSelect_PrevPage(void)
{
    switch (gMain.seqState1) {
    default:
        OpponentSelect_DrawCursor(gOpponentSelect.cursor, 1);
        OpponentSelect_SetBgScroll(gMain.brightness);
        OpponentSelect_DrawDuelistInfo(PAGE_DUELIST(gOpponentSelect.page, gOpponentSelect.cursor));
        if (FadeFromBlackU16(3)) {
            OpponentSelect_SetBgScroll(0);
            gMain.seqIndex1 = OPPSEL_STEP_INPUT;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    case 0:
        OpponentSelect_DrawCursor(gOpponentSelect.cursor, 1);
        OpponentSelect_DrawDuelistInfo(PAGE_DUELIST(gOpponentSelect.page, gOpponentSelect.cursor));
        OpponentSelect_SetBgScroll(-gMain.brightness);
        if (FadeToBlackU16(3)) {
            OpponentSelect_DrawLockedCovers(-gMain.brightness);
            gMain.seqState1++;
            return 0;
        }
        OpponentSelect_DrawLockedCovers(-gMain.brightness);
        return 0;
    case 1:
        OpponentSelect_LoadPage(gOpponentSelect.page - 1);
        gMain.seqState1++;
        return 0;
    }
    OpponentSelect_DrawLockedCovers(gMain.brightness);
    return 0;
}

/* Opponent-select step 6 (R): OpponentSelect_PrevPage with the slide reversed and page + 1. Returns 0. */
u16 OpponentSelect_NextPage(void)
{
    switch (gMain.seqState1) {
    default:
        OpponentSelect_DrawCursor(gOpponentSelect.cursor, 1);
        OpponentSelect_DrawDuelistInfo(PAGE_DUELIST(gOpponentSelect.page, gOpponentSelect.cursor));
        OpponentSelect_SetBgScroll(-gMain.brightness);
        if (FadeFromBlackU16(3)) {
            OpponentSelect_SetBgScroll(0);
            gMain.seqIndex1 = OPPSEL_STEP_INPUT;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    case 0:
        OpponentSelect_DrawCursor(gOpponentSelect.cursor, 1);
        OpponentSelect_DrawDuelistInfo(PAGE_DUELIST(gOpponentSelect.page, gOpponentSelect.cursor));
        OpponentSelect_SetBgScroll(gMain.brightness);
        if (FadeToBlackU16(3)) {
            OpponentSelect_DrawLockedCovers(gMain.brightness);
            gMain.seqState1++;
            return 0;
        }
        OpponentSelect_DrawLockedCovers(gMain.brightness);
        return 0;
    case 1:
        OpponentSelect_LoadPage(gOpponentSelect.page + 1);
        gMain.seqState1++;
        return 0;
    }
    OpponentSelect_DrawLockedCovers(-gMain.brightness);
    return 0;
}

/* Opponent-select step 9 (after B): installs CB_MainMenu in place of CB_Campaign, ending the Campaign. */
u16 OpponentSelect_ExitToMainMenu(void)
{
    SetMainCallback(CB_MainMenu);
    return 0;
}

/*
 * Runs one frame of gOpponentSelectSteps[gMain.seqIndex1]; a step that returns nonzero moves on to the next
 * one and clears seqState1/seqState2. Returns 1 at a NULL entry (OPPSEL_STEP_DONE: gMain.opponent holds the
 * chosen duelist), else 0. Not a scene callback: Campaign_SelectOpponent calls it.
 */
u16 OpponentSelect_Run(void)
{
    StepFunc step = gOpponentSelectSteps[gMain.seqIndex1];

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

/* Draws the 64x64 selection ring at (x, y): a 32x16 arc (tile 0x100) and a 16x16 side piece (tile 0x120),
 * each mirrored into the four quadrants. */
void OpponentSelect_DrawSelectionRing(s32 x, s32 y)
{
    AddSprite8bppFlip(x | (y << 16), SPRITE_SHAPE_32x16, 0x100, 0);
    AddSprite8bppFlip(x | ((y + 0x10) << 16), SPRITE_SHAPE_16x16, 0x120, 0);
    AddSprite8bppFlip((x + 0x20) | (y << 16), SPRITE_SHAPE_32x16, 0x100, OAM_ATTR1_HFLIP);
    AddSprite8bppFlip((x + 0x30) | ((y + 0x10) << 16), SPRITE_SHAPE_16x16, 0x120, OAM_ATTR1_HFLIP);
    AddSprite8bppFlip(x | ((y + 0x30) << 16), SPRITE_SHAPE_32x16, 0x100, OAM_ATTR1_VFLIP);
    AddSprite8bppFlip(x | ((y + 0x20) << 16), SPRITE_SHAPE_16x16, 0x120, OAM_ATTR1_VFLIP);
    AddSprite8bppFlip((x + 0x20) | ((y + 0x30) << 16), SPRITE_SHAPE_32x16, 0x100,
                      OAM_ATTR1_HFLIP | OAM_ATTR1_VFLIP);
    AddSprite8bppFlip((x + 0x30) | ((y + 0x20) << 16), SPRITE_SHAPE_16x16, 0x120,
                      OAM_ATTR1_HFLIP | OAM_ATTR1_VFLIP);
}

/* Draws the left page arrow (tile 0x12C at (0, 96)) unless on the first page, and the right one (tile 0x12E
 * at (224, 96)) if the next page is unlocked (the same test as R in OpponentSelect_HandleInput). */
void OpponentSelect_DrawPageArrows(void)
{
    u16 nextPageOpen;

    if (gOpponentSelect.page != 0)
        AddSprite8bpp(0x60 << 16, SPRITE_SHAPE_16x16, 0x12C);
    nextPageOpen = 0;
    switch (gOpponentSelect.page) {
    case 0:
        nextPageOpen = IsCampaignLevel2Unlocked();
        break;
    case 1:
        nextPageOpen = IsCampaignLevel3Unlocked();
        break;
    case 2:
        nextPageOpen = IsCampaignLevel4Unlocked();
        break;
    case 3:
        nextPageOpen = IsCampaignLevel5Unlocked();
        break;
    }
    if (nextPageOpen)
        AddSprite8bpp((0x60 << 16) | 0xE0, SPRITE_SHAPE_16x16, 0x12E);
}

/* Draws a 64x64 stone cover (32x32 tile 0x108 mirrored into four quadrants) over the portrait of each locked
 * opponent on the page, shifted left by 2 * scroll pixels to follow the background (OpponentSelect_SetBgScroll). */
void OpponentSelect_DrawLockedCovers(s32 scroll)
{
    s32 firstSlot = 0;
    s32 slot;

    if (gOpponentSelect.page >= LAST_PAGE)
        firstSlot = 1;
    for (slot = firstSlot; slot <= LAST_SLOT; slot++) {
        if (IsOpponentUnlockedS32(PAGE_DUELIST(gOpponentSelect.page, slot)) == 0) {
            s32 x = gOpponentSelectSlotPos[slot].x - scroll * 2;
            s32 y = gOpponentSelectSlotPos[slot].y;

            AddSprite8bppFlip(x | (y << 16), SPRITE_SHAPE_32x32, 0x108, 0);
            AddSprite8bppFlip((x + 0x20) | (y << 16), SPRITE_SHAPE_32x32, 0x108, OAM_ATTR1_HFLIP);
            AddSprite8bppFlip(x | ((y + 0x20) << 16), SPRITE_SHAPE_32x32, 0x108, OAM_ATTR1_VFLIP);
            AddSprite8bppFlip((x + 0x20) | ((y + 0x20) << 16), SPRITE_SHAPE_32x32, 0x108,
                              OAM_ATTR1_HFLIP | OAM_ATTR1_VFLIP);
        }
    }
}

/* Puts the cursor straight onto slot with no ease: the whole trail at the slot's position + (16, 0). */
void OpponentSelect_SnapCursor(u32 slot)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        gOpponentSelect.trailX[i] = gOpponentSelectSlotPosRW[slot].x + 0x10;
        gOpponentSelect.trailY[i] = gOpponentSelectSlotPosRW[slot].y;
    }
    gOpponentSelect.targetSlot = slot;
    gOpponentSelect.moveTimer = 0;
    gOpponentSelect.unk13 = 0;
}

/*
 * Loads page: resets the scroll, copies the page's palette and picture into both Mode-4 frames and makes it
 * the current page (moving the cursor off the empty slot 0 of the last page). Then renders, through the text
 * canvas, each slot's name (or "[ Unknown ]" if locked) into its own 16-px row of OBJ tiles from 0x2C0, and
 * "Win", "Lose", "Draw" into the tiles from 0x280. Finally the text palette and the bright and dim digits are
 * loaded over the first 10 tiles of the label rows (0x280 and 0x2A0).
 */
void OpponentSelect_LoadPage(s32 page)
{
    s32 count;
    s32 i;

    count = OPPONENTS_PER_PAGE - 1;
    if (page < LAST_PAGE)
        count = OPPONENTS_PER_PAGE;
    OpponentSelect_SetBgScroll(0);
    MemCopy16((void *)BG_PALETTE(0), gOpponentSelectPageBgs[page].pal, 0x200);
    MemCopy16((void *)MODE4_FRAME0, gOpponentSelectPageBgs[page].bitmap, MODE4_FRAME_SIZE);
    MemCopy16((void *)MODE4_FRAME1, gOpponentSelectPageBgs[page].bitmap, MODE4_FRAME_SIZE);
    gOpponentSelect.page = page;
    if (page == LAST_PAGE && gOpponentSelect.cursor == 0) {
        gOpponentSelect.cursor = LAST_SLOT;
        OpponentSelect_SnapCursor(gOpponentSelect.cursor);
    }

    /* Names: 10-px font centred on x = 48, colour 1 over a colour-5 shadow at (+1, +1). */
    TextCanvasInitEx(32, 10, 0, 0); /* 32 x 10 tiles */
    for (i = 0; i < count; i++) {
        s32 slot = i;

        if (count != OPPONENTS_PER_PAGE)
            slot = i + 1;
        if (IsOpponentUnlockedS32(PAGE_DUELIST(page, slot)) == 0) {
            TextDrawString(0x31 - StrLen((const char *)gUnknownOpponentName) * 5 / 2, slot * 16 + 1,
                           SIZE_COLOR(10, 5), gUnknownOpponentName);
            TextDrawString(0x30 - StrLen((const char *)gUnknownOpponentName) * 5 / 2, slot * 16,
                           SIZE_COLOR(10, 1), gUnknownOpponentName);
        } else {
            const u8 *name = gOpponentSelectNames[page * OPPONENTS_PER_PAGE + slot];
            s32 halfWidth = StrLen((const char *)name) * 5 / 2;

            TextDrawString(0x31 - halfWidth, slot * 16 + 1, SIZE_COLOR(10, 5), name);
            TextDrawString(0x30 - halfWidth, slot * 16, SIZE_COLOR(10, 1), name);
        }
    }
    TextCanvasToTiles((u16 *)OBJ_TILE(0x2C0), 0);

    /* Win/Lose/Draw at x = 80, 112, 144 (after the digits), colours 2-4 over shadows 6-8. */
    TextCanvasInitEx(32, 2, 0, 0); /* 32 x 2 tiles */
    for (i = 0; i <= 2; i++) {
        const u8 *label = gWinLoseDrawLabels[i];

        TextDrawString(0x51 + i * 0x20, 1, SIZE_COLOR(10, (u8)(i + 6)), label);
        TextDrawString(0x50 + i * 0x20, 0, SIZE_COLOR(10, (u8)(i + 2)), label);
    }
    TextCanvasToTiles((u16 *)OBJ_TILE(0x280), 0);
    MemCopy16((void *)OBJ_PALETTE(3), gOpponentSelectTextPal, 0x20);
    MemCopy16((void *)OBJ_TILE(0x280), gOpponentSelectDigitTiles, 0x140);
    MemCopy16((void *)OBJ_TILE(0x2A0), gOpponentSelectDimDigitTiles, 0x140);
}
