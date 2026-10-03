#include "global.h"
#include "main.h"

#include "gba.h"

/* Calendar screen (0x08002388-0x08002930) and Campaign opponent-select
 * screen (0x08002940-0x08003298). */

typedef u16 (*StepFunc)(void);

/* struct Main (gMain, 0x03000040) comes from include/main.h. */
#define gMain gMain

/* 0x0201F7E0: Campaign opponent-select state (0x34 bytes). */
struct OpponentSelect {
    u8 page:3;      /* bits 0-2: page (row of gOpponentSelectDuelists) */
    u16 cursor:3;    /* bits 3-5: selected slot on the page (0-4) */
    u16 unk6:3;     /* bits 6-8 */
    u16 unk9:4;     /* bits 9-12 */
    u32 unk13:4;    /* bits 13-16 */
    u32 unk17:15;
    u16 cursorX[8]; /* +0x04 */
    u16 cursorY[8]; /* +0x14 */
    u8 filler24[0x10];
};

extern struct OpponentSelect gOpponentSelect;

struct Pos16 {
    s16 x;
    s16 y;
};

extern StepFunc gCalendarSteps[];                /* Calendar steps */
extern u16 gOpponentSelectDuelists[];               /* opponent (duelist) id, [page * 5 + slot] */
extern StepFunc gOpponentSelectSteps[];                /* opponent-select steps */
extern struct Pos16 gOpponentSelectSlotPos[5];     /* slot screen positions */

extern const u8 gOpponentSelectObjPal[];                /* opponent-select OBJ palette */
extern const u8 gOpponentSelectObjTiles[];                /* opponent-select OBJ tiles */

#define gSel gOpponentSelect

/* Calendar scene state at 0x0201F7D0 (same layout as bustup_runner.c). */
struct Calendar {
    u16 unk0;               /* +0x00 anchor date */
    u16 date;               /* +0x02 current date */
    u32 shownEvents;        /* +0x04 event mask currently drawn */
    u16 blink:2;            /* +0x08 bits 0-1: setup/animation state (3 = steady) */
    u16 secondHalf:1;       /* +0x08 bit 2: showing weeks 3+ of the month */
    u16 page:1;             /* +0x08 bit 3: displayed Mode-4 frame (DISPCNT bit 4) */
    u16 weekday:3;          /* +0x08 bits 4-6: cursor column (day of week) */
    u16 week:3;             /* +0x08 bits 7-9: cursor row (week within the page) */
};
extern struct Calendar gCalendar;
#define gCalendar gCalendar

/* Unpacked date (DayCountToDate output). */
struct Date {
    u32 year:12;
    u32 month:4;
    u32 day:5;
    u32 weekday:3;
};

/* Save image at 0x02011C20 (only the in-game day counter). */
struct SaveData {
    u8 filler0[0x2150];
    u16 days;               /* +0x2150: in-game calendar day count */
};
extern struct SaveData gSaveData;
#define gSaveData gSaveData

extern const u8 gCalendarBgPal[];
extern const u8 gSystemFontPal[];
extern const u8 gCalendarBgBitmap[];
extern const u8 gCalendarNumberPal[];
extern const u8 gCalendarNumberTiles[];
extern const u8 gCalendarWeekdayPal[];
extern const u8 gCalendarWeekdayTiles[];
extern const u8 gCalendarMonthNamePal[];
extern const u8 gCalendarIconPal[];
extern const u8 gCalendarIconTiles[];

void Calendar_UpdateEventNames(void);
void Calendar_SetCursorDate(u16 date);
void Calendar_DrawCursorAndHeader(void);
void DayCountToDate(struct Date *date, u16 days);
s32 GetDaysInMonth(u32 year, u32 month);
s32 GetDayOfWeek(u32 year, u32 month, s32 day);
u32 GetCalendarEvents(u32 year, u32 month, s32 day);

struct Bitmap {
    const u16 *pal;     /* 256-colour palette */
    const u8 *bitmap;   /* 240x160 8bpp mode-4 bitmap */
};

extern struct Bitmap gOpponentSelectPageBgs[5];          /* page background per page */
extern const u8 *gOpponentSelectNames[];               /* opponent names, [page * 5 + slot] */
extern const u8 *gWinLoseDrawLabels[3];              /* "Win", "Lose", "Draw" */
extern const u8 gUnknownOpponentName[];                /* "[ Unknown ]" */
extern const u8 gOpponentSelectTextPal[];
extern const u8 gOpponentSelectDigitTiles[];
extern const u8 gOpponentSelectDimDigitTiles[];

s32 StrLen(const u8 *str);    /* StrLen */
void TextDrawString(s32 x, s32 y, u16 color, const u8 *str); /* DrawText (hypothesis) */
void TextCanvasInitEx(s32 a, s32 b, u16 c, s32 d);
void TextCanvasToTiles(void *dest, u16 b);

void OpponentSelect_DrawCursor(s32 slot, u16 flag);
void OpponentSelect_DrawDuelistInfo(u16 duelist);
s32 IsOpponentUnlocked(u16 duelist);
u16 IsCampaignLevel2Unlocked(void);
u16 IsCampaignLevel3Unlocked(void);
u16 IsCampaignLevel4Unlocked(void);
u16 IsCampaignLevel5Unlocked(void);
void AddSprite8bppFlip(u32 yx, u16 shape, u16 tile, u16 flip);
void AddSprite8bpp(u32 yx, u16 shape, u16 tile);
void AddSprite(u32 yx, u16 shape, u16 tile);
void MemCopy16(void *dest, const void *src, u32 size); /* MemCopy16 */
void SetBrightnessBlack(void);    /* SetBrightnessBlack */
void ResetBgScroll(void);    /* ResetBgScroll */
void ResetVideo(void);    /* ResetVideo */
void PlaySE(u16 id);  /* PlaySE */
void OpponentSelect_DrawLockedCovers(s32 scroll);
void OpponentSelect_SnapCursor(u32 slot);
void OpponentSelect_LoadPage(s32 page);
u16 FadeToBlack(u8 step);  /* FadeToBlack */
u16 FadeFromBlack(u8 step);  /* FadeFromBlack */
void SetMainCallback(StepFunc cb); /* SetMainCallback */
u16 CB_MainMenu(void);     /* main menu callback */
void Calendar_DrawMonth(void);

/* Calendar step 0: draw the month grid (year digits, day numbers, event
 * icons) and the cursor. */
void Calendar_DrawMonth(void)
{
    struct Date d0, d1;
    s32 year, x, digits, digit;
    s32 day, cell, col, daysInMonth;
    s32 pal, iconX, iconX2;
    register s32 rowY asm("r9"); /* FAKEMATCH: unpinned, rowY loses r9 to day (its constant init doubles its live length) */
    u32 iconY, dayY;
    u32 step; /* one pseudo for 0x180000 that gets no register, so reload rematerialises it per use */
    u32 events;

    Calendar_DrawCursorAndHeader();
    DayCountToDate(&d0, gCalendar.date);
    DayCountToDate(&d1, gCalendar.unk0);

    /* Year, right to left in 4 decimal digits. */
    year = d0.year;
    x = 0;
    for (digits = 3; digits >= 0; digits--) {
        digit = year % 10;
        AddSprite((0xE0 - x) | 0x100000, 0, digit + 0x6240);
        year = year / 10;
        x += 6;
    }

    day = 1;
    cell = GetDayOfWeek(d0.year, d0.month, day);
    col = cell;
    daysInMonth = GetDaysInMonth(d0.year, d0.month);
    if (gCalendar.secondHalf) {
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
    step = 0x180000;
    iconY = 0x300000;
    dayY = 0x280000;
    rowY = 0;
    iconX = col << 5;
    while (day <= daysInMonth && cell <= 20) {
        events = GetCalendarEvents(d0.year, d0.month, day);
        pal = 0;
        iconX2 = iconX + 0x18;
        switch (col) {
        case 0:
            pal = 1;
            break;
        case 6:
            pal = 2;
            break;
        }
        AddSprite((iconX + 0x10) | dayY, 0, (pal << 5) + day + 0x6200);
        if (events & 0x100000) {
            AddSprite8bpp(iconX2 | iconY, 0x40, 0x172);
            iconX2 += 8;
        }
        if (events & 0x200000) {
            AddSprite8bpp(((rowY + 0x30) << 16) | iconX2, 0x40, 0x174);
            iconX2 += 8;
        }
        if (events & 0x3F400000) {
            iconX2 |= (rowY + 0x30) << 16;
            AddSprite8bpp(iconX2, 0x40, 0x170);
        }
        if (d0.year == d1.year && d0.month == d1.month && day == d1.day) {
            AddSprite8bpp((iconX + 0xD) | ((rowY + 0x25) << 16), 0x80, 0x17C);
        }
        iconX += 0x20;
        col++;
        if (col > 6) {
            iconX = 0;
            col = 0;
            iconY += step;
            dayY += step;
            rowY += 0x18;
        }
        day++;
        cell++;
    }
}

/* Calendar step 1: clear the calendar state, set up video and load graphics. */
/* DMA fill, then wait, as two separate blocks with their own register
 * pointers, like the SDK's DmaSet and DmaWait macros. */
u16 Calendar_Init(void)
{
    {
        vu16 zero = 0;
        vu32 *dma = (vu32 *)0x040000D4;

        dma[0] = (u32)&zero;
        dma[1] = (u32)&gCalendar;
        dma[2] = 0x81000006;
        dma[2];
    }
    {
        vu32 *dmaRegs = (vu32 *)0x040000D4;

        while (dmaRegs[2] & 0x80000000)
            ;
    }
    gCalendar.unk0 = gSaveData.days;
    gCalendar.date = gSaveData.days;
    SetBrightnessBlack();
    ResetBgScroll();
    ResetVideo();
    REG_MOSAIC = 0;
    REG_BLDCNT = 0;
    REG_BLDY = 0;
    REG_BG0CNT = 3;
    REG_BG1CNT = 3;
    REG_BG2CNT = 3;
    REG_BG3CNT = 3;
    REG_DISPCNT = 4;
    gMain.vblankFlags = 1;
    MemCopy16((void *)0x05000000, gCalendarBgPal, 0x1E0);
    MemCopy16((void *)0x050001E0, gSystemFontPal, 0x20);
    MemCopy16((void *)0x06000000, gCalendarBgBitmap, 0x9600);
    MemCopy16((void *)0x0600A000, gCalendarBgBitmap, 0x9600);
    MemCopy16((void *)0x050002C0, gCalendarNumberPal, 0x20);
    MemCopy16((void *)0x06014000, gCalendarNumberTiles, 0xC00);
    MemCopy16((void *)0x05000300, gCalendarWeekdayPal, 0x20);
    MemCopy16((void *)0x06015400, gCalendarWeekdayTiles, 0x800);
    MemCopy16((void *)0x050002E0, gCalendarMonthNamePal, 0x20);
    gCalendar.blink = 0;
    MemCopy16((void *)0x05000200, gCalendarIconPal, 0xC0);
    MemCopy16((void *)0x06015C00, gCalendarIconTiles, 0x1000);
    Calendar_SetCursorDate(gCalendar.date);
    Calendar_UpdateEventNames();
    return 1;
}


u16 Calendar_FadeIn(void)
{
    Calendar_DrawMonth();
    REG_DISPCNT = 0x1F04;
    return FadeFromBlack(4);
}

/* Calendar step 2: handle date (R/L), cursor (d-pad) and confirm input. */
u16 Calendar_HandleInput(void)
{
    struct Date d;

    Calendar_DrawMonth();
    if (gCalendar.blink == 3) {
        if (gMain.newKeys & R_BUTTON) {
            DayCountToDate(&d, gCalendar.date);
            gCalendar.date -= d.day;
            gCalendar.date = gCalendar.date + GetDaysInMonth(d.year, d.month) + 1;
            gCalendar.blink &= ~3;
            Calendar_UpdateEventNames();
            PlaySE(0);
        }
        if (gMain.newKeys & L_BUTTON) {
            DayCountToDate(&d, gCalendar.date);
            if (gCalendar.date > 30) {
                gCalendar.date -= d.day;
                gCalendar.blink &= ~3;
                Calendar_UpdateEventNames();
                PlaySE(0);
                DayCountToDate(&d, gCalendar.date);
            }
        }
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        gCalendar.weekday = (gCalendar.weekday + 1) % 7;
        Calendar_UpdateEventNames();
        PlaySE(0);
    }
    if (gMain.newKeys & DPAD_LEFT) {
        gCalendar.weekday = (gCalendar.weekday + 6) % 7;
        Calendar_UpdateEventNames();
        PlaySE(0);
    }
    if (gMain.newKeys & DPAD_UP) {
        if (gCalendar.week != 0) {
            gCalendar.week = (gCalendar.week - 1) & 7;
            Calendar_UpdateEventNames();
            PlaySE(0);
        } else if (gCalendar.secondHalf) {
            gCalendar.secondHalf = 0;
            gCalendar.week = 2;
            Calendar_UpdateEventNames();
            PlaySE(0);
        } else {
            PlaySE(3);
        }
    }
    if (gMain.newKeys & DPAD_DOWN) {
        if (gCalendar.week <= 1) {
            gCalendar.week = (gCalendar.week + 1) & 7;
            Calendar_UpdateEventNames();
            PlaySE(0);
        } else if (!gCalendar.secondHalf) {
            gCalendar.secondHalf = 1;
            gCalendar.week = 0;
            Calendar_UpdateEventNames();
            PlaySE(0);
        } else {
            PlaySE(3);
        }
    }
    if (gMain.newKeys & (A_BUTTON | B_BUTTON)) {
        PlaySE(2);
        return 1;
    }
    return 0;
}

u16 Calendar_FadeOut(void)
{
    return FadeToBlack(4);
}

/* Calendar scene main callback: step runner over gCalendarSteps. */
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

/* Opponent-select step 0: clear the state, set up video and load graphics. */
u16 OpponentSelect_Init(void)
{
    {
        vu16 zero = 0;
        vu32 *dma = (vu32 *)0x040000D4;

        dma[0] = (u32)&zero;
        dma[1] = (u32)&gSel;
        dma[2] = 0x8100001A;
        dma[2];
        while (dma[2] & 0x80000000)
            ;
    }
    SetBrightnessBlack();
    ResetBgScroll();
    ResetVideo();
    REG_MOSAIC = 0;
    REG_BLDCNT = 0;
    REG_BLDY = 0;
    REG_BG0CNT = 3;
    REG_BG1CNT = 3;
    REG_BG2CNT = 3;
    REG_BG3CNT = 3;
    REG_DISPCNT = 4;
    gMain.vblankFlags = 1;
    MemCopy16((void *)0x05000200, gOpponentSelectObjPal, 0x60);
    MemCopy16((void *)0x06014000, gOpponentSelectObjTiles, 0x1000);
    OpponentSelect_SnapCursor(gSel.cursor);
    OpponentSelect_LoadPage(gSel.page);
    return 1;
}
/* Opponent-select step 1: draw the screen and fade in. */
u16 OpponentSelect_FadeIn(void)
{
    OpponentSelect_DrawCursor(gSel.cursor, 1);
    OpponentSelect_DrawLockedCovers(0);
    OpponentSelect_DrawDuelistInfo(gOpponentSelectDuelists[gSel.page * 5 + gSel.cursor]);
    REG_DISPCNT = 0x1F04;
    return FadeFromBlack(4);
}
u16 OpponentSelect_HandleInput(void)
{
    u16 next;

    OpponentSelect_DrawCursor(gSel.cursor, 0);
    OpponentSelect_DrawLockedCovers(0);
    OpponentSelect_DrawDuelistInfo(gOpponentSelectDuelists[gSel.page * 5 + gSel.cursor]);

    if (gMain.newKeys & DPAD_LEFT) {
        if (gSel.cursor < 4)
            gSel.cursor++;
        else
            gSel.cursor = 0;
        if (gSel.page == 4 && gSel.cursor == 0)
            gSel.cursor = 1;
        PlaySE(0);
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        if (gSel.cursor != 0)
            gSel.cursor--;
        else
            gSel.cursor = 4;
        if (gSel.page == 4 && gSel.cursor == 0)
            gSel.cursor = 4;
        PlaySE(0);
    }
    if (gMain.newKeys & L_BUTTON) {
        if (gSel.page != 0) {
            PlaySE(0);
            gMain.seqIndex1 = 5;
            return 0;
        }
        PlaySE(3);
    }
    if (gMain.newKeys & R_BUTTON) {
        next = 0;
        switch (gSel.page) {
        case 0:
            next = IsCampaignLevel2Unlocked();
            break;
        case 1:
            next = IsCampaignLevel3Unlocked();
            break;
        case 2:
            next = IsCampaignLevel4Unlocked();
            break;
        case 3:
            next = IsCampaignLevel5Unlocked();
            break;
        }
        if (next) {
            PlaySE(0);
            gMain.seqIndex1 = 6;
            return 0;
        }
        PlaySE(3);
    }
    if (gMain.newKeys & A_BUTTON) {
        if (IsOpponentUnlocked(gOpponentSelectDuelists[gSel.page * 5 + gSel.cursor])) {
            gMain.opponent = gOpponentSelectDuelists[gSel.page * 5 + gSel.cursor];
            PlaySE(1);
            return 1;
        }
        PlaySE(3);
    }
    if (gMain.newKeys & B_BUTTON) {
        PlaySE(1);
        gMain.seqIndex1 = 8;
    }
    return 0;
}
/* Opponent-select step 3: draw the screen and fade out. */
u16 OpponentSelect_FadeOut(void)
{
    OpponentSelect_DrawCursor(gSel.cursor, 0);
    OpponentSelect_DrawLockedCovers(0);
    OpponentSelect_DrawDuelistInfo(gOpponentSelectDuelists[gSel.page * 5 + gSel.cursor]);
    return FadeToBlack(4);
}

/* Set BG2X (affine reference point) to x * 2 pixels (20.8 fixed). */
void OpponentSelect_SetBgScroll(s32 x)
{
    vu16 *reg = (vu16 *)0x04000028;
    u32 v = x << 9;

    *reg++ = v;
    *reg = (v >> 16) & 0xFFF;
}

u16 OpponentSelect_PrevPage(void)
{
    switch (gMain.seqState1) {
    default:
        OpponentSelect_DrawCursor(gSel.cursor, 1);
        OpponentSelect_SetBgScroll(gMain.brightness);
        OpponentSelect_DrawDuelistInfo(gOpponentSelectDuelists[gSel.page * 5 + gSel.cursor]);
        if (FadeFromBlack(3)) {
            OpponentSelect_SetBgScroll(0);
            gMain.seqIndex1 = 2;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    case 0:
        OpponentSelect_DrawCursor(gSel.cursor, 1);
        OpponentSelect_DrawDuelistInfo(gOpponentSelectDuelists[gSel.page * 5 + gSel.cursor]);
        OpponentSelect_SetBgScroll(-gMain.brightness);
        if (FadeToBlack(3)) {
            OpponentSelect_DrawLockedCovers(-gMain.brightness);
            gMain.seqState1++;
            return 0;
        }
        OpponentSelect_DrawLockedCovers(-gMain.brightness);
        return 0;
    case 1:
        OpponentSelect_LoadPage(gSel.page - 1);
        gMain.seqState1++;
        return 0;
    }
    OpponentSelect_DrawLockedCovers(gMain.brightness);
    return 0;
}
u16 OpponentSelect_NextPage(void)
{
    switch (gMain.seqState1) {
    default:
        OpponentSelect_DrawCursor(gSel.cursor, 1);
        OpponentSelect_DrawDuelistInfo(gOpponentSelectDuelists[gSel.page * 5 + gSel.cursor]);
        OpponentSelect_SetBgScroll(-gMain.brightness);
        if (FadeFromBlack(3)) {
            OpponentSelect_SetBgScroll(0);
            gMain.seqIndex1 = 2;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    case 0:
        OpponentSelect_DrawCursor(gSel.cursor, 1);
        OpponentSelect_DrawDuelistInfo(gOpponentSelectDuelists[gSel.page * 5 + gSel.cursor]);
        OpponentSelect_SetBgScroll(gMain.brightness);
        if (FadeToBlack(3)) {
            OpponentSelect_DrawLockedCovers(gMain.brightness);
            gMain.seqState1++;
            return 0;
        }
        OpponentSelect_DrawLockedCovers(gMain.brightness);
        return 0;
    case 1:
        OpponentSelect_LoadPage(gSel.page + 1);
        gMain.seqState1++;
        return 0;
    }
    OpponentSelect_DrawLockedCovers(-gMain.brightness);
    return 0;
}

u16 OpponentSelect_ExitToMainMenu(void)
{
    SetMainCallback(CB_MainMenu);
    return 0;
}

/* Campaign opponent-select runner over gOpponentSelectSteps. */
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

/* Draw a 64x64 frame from four mirrored corner pieces at (x, y). */
void OpponentSelect_DrawSelectionRing(s32 x, s32 y)
{
    AddSprite8bppFlip(x | (y << 16), 0x4080, 0x100, 0);
    AddSprite8bppFlip(x | ((y + 0x10) << 16), 0x40, 0x120, 0);
    AddSprite8bppFlip((x + 0x20) | (y << 16), 0x4080, 0x100, 0x1000);
    AddSprite8bppFlip((x + 0x30) | ((y + 0x10) << 16), 0x40, 0x120, 0x1000);
    AddSprite8bppFlip(x | ((y + 0x30) << 16), 0x4080, 0x100, 0x2000);
    AddSprite8bppFlip(x | ((y + 0x20) << 16), 0x40, 0x120, 0x2000);
    AddSprite8bppFlip((x + 0x20) | ((y + 0x30) << 16), 0x4080, 0x100, 0x3000);
    AddSprite8bppFlip((x + 0x30) | ((y + 0x20) << 16), 0x40, 0x120, 0x3000);
}
/* Draw the page-scroll arrows: left if not on the first page,
 * right if the next page is unlocked. */
void OpponentSelect_DrawPageArrows(void)
{
    u16 next;

    if (gSel.page != 0)
        AddSprite8bpp(0x00600000, 0x40, 0x12C);
    next = 0;
    switch (gSel.page) {
    case 0:
        next = IsCampaignLevel2Unlocked();
        break;
    case 1:
        next = IsCampaignLevel3Unlocked();
        break;
    case 2:
        next = IsCampaignLevel4Unlocked();
        break;
    case 3:
        next = IsCampaignLevel5Unlocked();
        break;
    }
    if (next)
        AddSprite8bpp(0x006000E0, 0x40, 0x12E);
}
/* Draw the portrait frames of the unlocked opponents on the current page,
 * shifted left by 2*scroll pixels. */
void OpponentSelect_DrawLockedCovers(s32 scroll)
{
    s32 start = 0;
    s32 i;

    if (gSel.page > 3)
        start = 1;
    for (i = start; i <= 4; i++) {
        if (IsOpponentUnlocked(gOpponentSelectDuelists[gSel.page * 5 + i]) == 0) {
            s32 x = gOpponentSelectSlotPos[i].x - scroll * 2;
            s32 y = gOpponentSelectSlotPos[i].y;

            AddSprite8bppFlip(x | (y << 16), 0x80, 0x108, 0);
            AddSprite8bppFlip((x + 0x20) | (y << 16), 0x80, 0x108, 0x1000);
            AddSprite8bppFlip(x | ((y + 0x20) << 16), 0x80, 0x108, 0x2000);
            AddSprite8bppFlip((x + 0x20) | ((y + 0x20) << 16), 0x80, 0x108, 0x3000);
        }
    }
}
/* Move the cursor to `slot` and reset its animation. */
void OpponentSelect_SnapCursor(u32 slot)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        gSel.cursorX[i] = gOpponentSelectSlotPos[slot].x + 0x10;
        gSel.cursorY[i] = gOpponentSelectSlotPos[slot].y;
    }
    gSel.unk6 = slot;
    gSel.unk9 = 0;
    gSel.unk13 = 0;
}
void OpponentSelect_LoadPage(s32 page)
{
    s32 count;
    s32 i;

    count = 4;
    if (page <= 3)
        count = 5;
    OpponentSelect_SetBgScroll(0);
    MemCopy16((void *)0x05000000, gOpponentSelectPageBgs[page].pal, 0x200);
    MemCopy16((void *)0x06000000, gOpponentSelectPageBgs[page].bitmap, 0x9600);
    MemCopy16((void *)0x0600A000, gOpponentSelectPageBgs[page].bitmap, 0x9600);
    gSel.page = page;
    if (page == 4 && gSel.cursor == 0) {
        gSel.cursor = 4;
        OpponentSelect_SnapCursor((u16)gSel.cursor);
    }
    TextCanvasInitEx(0x20, 10, 0, 0);
    for (i = 0; i < count; i++) {
        s32 slot = i;

        if (count != 5)
            slot = i + 1;
        if (IsOpponentUnlocked(gOpponentSelectDuelists[page * 5 + slot]) == 0) {
            TextDrawString(0x31 - StrLen(gUnknownOpponentName) * 5 / 2, slot * 16 + 1, 0xA05, gUnknownOpponentName);
            TextDrawString(0x30 - StrLen(gUnknownOpponentName) * 5 / 2, slot * 16, 0xA01, gUnknownOpponentName);
        } else {
            const u8 *name = gOpponentSelectNames[page * 5 + slot];
            s32 w = StrLen(name) * 5 / 2;

            TextDrawString(0x31 - w, slot * 16 + 1, 0xA05, name);
            TextDrawString(0x30 - w, slot * 16, 0xA01, name);
        }
    }
    TextCanvasToTiles((void *)0x06015800, 0);
    TextCanvasInitEx(0x20, 2, 0, 0);
    for (i = 0; i <= 2; i++) {
        const u8 *label = gWinLoseDrawLabels[i];

        TextDrawString(0x51 + i * 0x20, 1, (u8)(i + 6) | 0xA00, label);
        TextDrawString(0x50 + i * 0x20, 0, (u8)(i + 2) | 0xA00, label);
    }
    TextCanvasToTiles((void *)0x06015000, 0);
    MemCopy16((void *)0x05000260, gOpponentSelectTextPal, 0x20);
    MemCopy16((void *)0x06015000, gOpponentSelectDigitTiles, 0x140);
    MemCopy16((void *)0x06015400, gOpponentSelectDimDigitTiles, 0x140);
}
