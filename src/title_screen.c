/*
 * title_screen (0x080044E4-0x080054FF): calendar events and date conversion, the New Game starter-deck
 * builder, the boot license screens and most of the title screen.
 *
 * Boot: GameInit installs CB_License, which runs gLicenseSteps (Nintendo notice, Konami logo, KCEJ logo); the
 * last step installs CB_Title (title_menu.c), which runs gTitleSteps. Steps 0-4 of that table are here:
 * Title_Init, Title_Setup, Title_FadeIn, Title_HandleInput and Title_FadeOut (enum TitleStep).
 */
#include "global.h"
#include "gba.h"
#include "main.h"
#include "sound.h"
#include "constants/sound.h"
#include "util.h"
#include "palette.h"
#include "bg.h"
#include "sprite.h"
#include "text.h"
#include "save.h"
#include "debug.h"
#include "card_data.h"
#include "booster.h"
#include "calendar.h"
#include "title_screen.h"

/* Matching: views of the palette.h fades (u32 FadeToBlack(s32) etc., asm labels FadeToBlack, FadeFromBlack,
 * FadeToWhite, FadeFromWhite) that return u16: this unit's callers truncate the result (lsls #16) before
 * testing or returning it, and with palette.h's u32 return the truncations disappear and the unit stops
 * matching. The calls and the ROM symbols are the same. */
u16 FadeToBlackU16(s32 step) asm("FadeToBlack");
u16 FadeFromBlackU16(s32 step) asm("FadeFromBlack");
u16 FadeToWhiteU16(s32 step) asm("FadeToWhite");
u16 FadeFromWhiteU16(s32 step) asm("FadeFromWhite");

/* The HBlank IRQ (IntrTable slot and IE bit), changed with IME off. */
#define DISABLE_HBLANK_INTR() (REG_IME = 0, REG_IE &= ~INTR_FLAG_HBLANK, REG_IME = 1)
#define SET_HBLANK_HANDLER(handler) \
    (REG_IME = 0, REG_IE &= ~INTR_FLAG_HBLANK, IntrTable[INTR_SLOT_HBLANK] = (handler), REG_IME = 1)
#define ENABLE_HBLANK_INTR() (REG_IME = 0, REG_IE |= INTR_FLAG_HBLANK, REG_IME = 1)

/* ROM data used only by this unit. */
extern u16 (*const gLicenseSteps[])(void);              /* 0x0819879C: the 4 License_* steps, NULL */
extern const char gStrLicensedByNintendo[];             /* 0x080813F0 */
extern const u16 gKonamiLogoImage[];                    /* 0x087D01F4: 8bpp image pack */
extern const u16 gKcejLogoImage[];                      /* 0x087D292C: 8bpp image pack */
extern const struct StarterDeckPool gStarterDeckPools[];/* 0x08198744: 11 pools */
extern const char gStrStarterDeckErrorFmt[];            /* 0x080813E4: debug message for an unknown card number */
extern const char gStrNewGame[];                        /* 0x08081408 */
extern const char gStrContinue[];                       /* 0x08081414 */
extern const u16 gTitleLogoWave[];                      /* 0x08198830: 16 BG1 HOFS values (Title_HBlank) */
extern const u16 gTitleLogoImage[];                     /* 0x087BDAA8: 8bpp, the logo (BG0) */
extern const u16 gTitleFlameImage[];                    /* 0x087C1DCC: 4bpp, the flames (BG1) */
extern const u16 gTitleCoinImage[];                     /* 0x087C0CD4: 4bpp, the coin (BG2) */
extern const u16 gTitleGridImage[];                     /* 0x0867DFCC: 4bpp, one 4x4-tile block of the BG3 grid */
extern const u16 gTitleCopyrightImage[];                /* 0x087C056C: 4bpp, "(c)1996 KAZUKI TAKAHASHI" line */

/* ---- Calendar ---- */

/*
 * 1-based week of the month of a date. The two GetDayOfWeek calls are discarded, but the ROM makes them.
 * GetWeekOfMonth below is the same body out of line; GetCalendarEvents only matches with this inline copy.
 */
static inline u32 WeekOfMonth(u32 year, u32 month, u32 day)
{
    GetDayOfWeek(year, month, 1);
    GetDayOfWeek(year, month, day);
    return (day - 1) / 7 + 1;
}

/* IsDayOff tested through the low halfword of its result, as the ROM does (lsl #16). */
#define IS_DAY_OFF(year, month, day) ((IsDayOff(year, month, day) << 16) != 0)

/*
 * Returns the CalendarEvent bits of a date: GetHolidayFlags' holiday bits, the seasonal days, the June
 * SUGOROKU and November tournament duels (the later rounds gated by save progress), the Duel Ceremony
 * Saturdays and, unless the date is itself a day off, the Weekly Jump and V Jump release days, which move
 * earlier when their usual day is off.
 */
u32 GetCalendarEvents(u32 year, u32 month, u32 day)
{
    u32 weeklyJump, vJump;
    u32 flags = GetHolidayFlags(year, month, day);

    switch (month) {
    case 2:
        if (day == 14)
            flags |= CAL_VALENTINES_DAY;
        break;
    case 3:
        if (day == 14)
            flags |= CAL_WHITE_DAY;
        break;
    case 6:
        if (day == 28)
            flags |= CAL_MURAN_BIRTHDAY;
        /* SUGOROKU prelim on the first Saturday; the match the next day, if the player won the prelim. */
        if (WeekOfMonth(year, month, day) == 1 && GetDayOfWeek(year, month, day) == WEEKDAY_SATURDAY)
            flags |= CAL_SUGOROKU_PRELIM;
        if (WeekOfMonth(year, month, day - 1) == 1 && GetDayOfWeek(year, month, day - 1) == WEEKDAY_SATURDAY
            && gSaveData.sugorokuQualified != 0)
            flags |= CAL_SUGOROKU_MATCH;
        break;
    case 10:
        if (day == 31)
            flags |= CAL_HALLOWEEN;
        break;
    case 11:
        /* Tournament on the first four Sundays; each later round needs the previous one won. */
        if (GetDayOfWeek(year, month, day) == WEEKDAY_SUNDAY) {
            switch (WeekOfMonth(year, month, day)) {
            case 1:
                flags |= CAL_TOURNAMENT_ROUND1;
                break;
            case 2:
                if (gSaveData.tournamentRound != 0)
                    flags |= CAL_TOURNAMENT_ROUND2;
                break;
            case 3:
                if (gSaveData.tournamentRound > 1)
                    flags |= CAL_TOURNAMENT_SEMIFINAL;
                break;
            case 4:
                if (gSaveData.tournamentRound > 2)
                    flags |= CAL_TOURNAMENT_FINAL;
                break;
            }
        }
        break;
    case 12:
        if (day == 24)
            flags |= CAL_CHRISTMAS_EVE;
        break;
    }
    if (GetDayOfWeek(year, month, day) == WEEKDAY_SATURDAY) {
        u32 week = WeekOfMonth(year, month, day);
        if (week == 2 || week == 4)
            flags |= CAL_DUEL_CEREMONY;
    }
    /* No magazine on a day off. The shared exit keeps one use of flags at the return, which lets month win
     * r6 over flags. */
    if (IS_DAY_OFF(year, month, day))
        goto end;
    weeklyJump = 0;
    vJump = 0;
    /* Weekly Jump: Tuesdays (except 2001-01-02, day 1 of the game); Monday if Tuesday is off; Saturday if
     * Monday and Tuesday are both off. */
    if (GetDayOfWeek(year, month, day) == WEEKDAY_TUESDAY
        && (year > 2001 || month > 1 || day > 2))
        weeklyJump = 1;
    if (GetDayOfWeek(year, month, day) == WEEKDAY_MONDAY
        && IS_DAY_OFF(year, month, day + 1))
        weeklyJump = 1;
    if (GetDayOfWeek(year, month, day) == WEEKDAY_SATURDAY
        && IS_DAY_OFF(year, month, day + 2)
        && IS_DAY_OFF(year, month, day + 3))
        weeklyJump = 1;
    if (weeklyJump != 0)
        flags |= CAL_WEEKLY_JUMP;
    /* V Jump: the 21st, or the last working day before it (back to the 18th). */
    if (day == 21)
        vJump = 1;
    if (day == 20 && IS_DAY_OFF(year, month, 21))
        vJump = 1;
    if (day == 19 && IS_DAY_OFF(year, month, 20)
        && IS_DAY_OFF(year, month, 21))
        vJump = 1;
    if (day == 18 && IS_DAY_OFF(year, month, 19)
        && IS_DAY_OFF(year, month, 20)
        && IS_DAY_OFF(year, month, 21))
        vJump = 1;
    if (vJump != 0)
        flags |= CAL_V_JUMP;
end:
    return flags;
}

/*
 * Unpacks the day count `days` (day 0 = 2001-01-01) into *date: year, month, day and weekday. It works in
 * 1461-day (4-year) cycles whose last year is a leap year. 2100 is not one, so from day 36524 (2101-01-01) on
 * the count moves on by one day, skipping the 366th day the cycle would give 2100.
 */
void DayCountToDate(struct Date *date, u16 days)
{
    u16 year, month;
    s32 monthLength = 31;

    if (days > 36523)
        days++;
    year = days / 1461 * 4;
    days = days % 1461;
    year += days / 365 + 2001;
    if (days == 1460) {
        /* Dec 31 of the cycle's leap year (day 366) */
        days = 365;
        year--;
    } else {
        days = days % 365;
    }
    month = 0;
    while (days >= monthLength) {
        days -= monthLength;
        month++;
        monthLength = gDaysPerMonth[month];
        if (month == 1)
            monthLength += IsLeapYear(year);
    }
    date->year = year;
    date->month = month + 1;
    date->day = days + 1;
    date->weekday = GetDayOfWeek(year, month + 1, days + 1);
    if (date->day == 0) /* the 5-bit field wrapped; never for a valid day count */
        date->day++;
}

/* Stores today's in-game date (gSaveData.days) in *date. */
void GetCurrentDate(struct Date *date)
{
    DayCountToDate(date, gSaveData.days);
}

/* Returns the 1-based week of the month, (day - 1) / 7 + 1. Unreferenced out-of-line copy of WeekOfMonth. */
u32 GetWeekOfMonth(u32 year, u32 month, u32 day)
{
    GetDayOfWeek(year, month, 1);
    GetDayOfWeek(year, month, day);
    return (day - 1) / 7 + 1;
}

/* ---- New Game starter deck ---- */

/*
 * Returns the card ID of a card number through gCardNumberToId: 0xFFFF (no card) gives 0, and an alternate-art
 * number (2000 + n) gives the ID of n plus 1.
 */
static inline u16 CardNumberToId(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number < CARD_NUMBER_ALT_ART) {
        /* FAKEMATCH: keep the initialized lookup mask and its working copy
         * separate, then add the table to the already scaled byte offset. */
        register u32 mask asm("r3") = 0x7FF;
        register u32 copy asm("r1") = mask;
        u32 off;
        register const u16 *table asm("r4");

        asm("" : : "r"(mask));
        off = (number & copy) * 2;
        table = gCardNumberToId;
        off += (u32)table;
        return *(const u16 *)off;
    }
    {
        u32 off = ((number - CARD_NUMBER_ALT_ART) & 0x7FF) * 2;
        /* FAKEMATCH: the alternate-art path uses a separate table scratch. */
        register const u16 *table asm("r3");

        table = gCardNumberToId;
        off += (u32)table;
        return *(const u16 *)off + 1;
    }
}

/*
 * Builds the starting deck for `choice` (enum StarterDeck, taken % 3): shuffles each of the 11
 * gStarterDeckPools (count * 4 random swaps) and adds its first pick0/pick1/pick2 cards to the saved deck.
 * A card number with no card ID goes to DebugPrintf (a stub in the retail game) instead.
 */
void BuildStarterDeck(s32 choice)
{
    u16 cards[64];
    const struct StarterDeckPool *pool = gStarterDeckPools;
    u32 poolIndex;
    s32 i, picks;

    for (poolIndex = 0; poolIndex <= 10; pool++, poolIndex++) {
        const u16 *src = pool->cards;

        for (i = 0; i < pool->count; i++)
            cards[i] = src[i];
        for (i = 0; i < pool->count * 4; i++) {
            s32 a = Random() % pool->count;
            s32 b = Random() % pool->count;
            u16 tmp = cards[a];
            cards[a] = cards[b];
            cards[b] = tmp;
        }
        /* No default: choice is 0-2, so one case always sets picks. */
        switch (choice % 3) {
        case STARTER_DECK_BLACK:
            picks = pool->pick0;
            break;
        case STARTER_DECK_RED:
            picks = pool->pick1;
            break;
        case STARTER_DECK_GREEN:
            picks = pool->pick2;
            break;
        }
        for (i = 0; i < picks; i++) {
            u16 number = cards[i % pool->count];
            u16 cardId = CardNumberToId(number);

            if (cardId)
                AddCardToSavedDeck(cardId);
            else
                DebugPrintf(gStrStarterDeckErrorFmt, number);
        }
    }
    DebugPrintFlush();
}

/* ---- Boot license screens ---- */

/* Title HBlank handler, installed by Title_LoadGraphics and removed by Title_FadeIn at the white flash: makes
 * the flames (BG1) wave, one HOFS value per scanline from a 16-line table that moves on every frame. */
void Title_HBlank(void)
{
    REG_BG1HOFS = gMain.hblankScroll[(REG_VCOUNT + gMain.frameCounter) & 0xF];
}

/* License step 0: white screen, display off; then video reset, the default BG0-3CNT and a white backdrop.
 * Returns 1 when done. */
u16 License_InitVideo(void)
{
    switch (gMain.seqState0) {
    default:
        return 1;
    case 0:
        SetBrightnessWhite();
        REG_DISPCNT = 0;
        gMain.seqState0++;
        return 0;
    case 1:
        gMain.vblankFlags = VBLANK_COPY_OAM | VBLANK_COPY_BG_MAPS;
        ResetVideo();
        ResetBgScroll();
        REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(1) | BGCNT_256COLOR | BGCNT_SCREENBASE(0);
        REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(1);
        REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(2);
        REG_BG3CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(3);
        *(vu16 *)BG_PLTT = 0xFFFF;
        gMain.seqState0++;
        return 0;
    }
}

/*
 * License step 1: "LICENSED BY NINTENDO" centred on BG1 (rows 9-11). Each pass draws the string twice, one
 * pixel apart, for bold text: first the shadow (colour 15) at +1,+1, then the text (colour 8). Fades in from
 * white, holds 120 frames and fades out to white. Returns 1 when done.
 */
u16 License_ShowNintendoNotice(void)
{
    s32 left, shadow, dx, dy;

    switch (gMain.seqState0) {
    case 0:
        ClearBgMapBuffers();
        TextCanvasInit(32, 3);
        left = (240 - StrLen(gStrLicensedByNintendo) * 9) / 2;
        /* The dy loop runs once (dy = 0), as in the ROM. */
        for (shadow = 1; shadow >= 0; shadow--)
            for (dx = 0; dx <= 1; dx++)
                for (dy = 0; dy <= 0; dy++)
                    TextDrawString(left + dx + shadow, dy + shadow,
                                   shadow == 1 ? TEXT_SIZE_COLOR(16, 15) : TEXT_SIZE_COLOR(16, 8),
                                   gStrLicensedByNintendo);
        TextCanvasToTiles((u16 *)(VRAM + 0x4400), 0); /* charblock 1, tile 0x20 */
        for (dx = 0; dx < 96; dx++)
            gMain.bgMapBuffer[1][9 * 32 + dx] = dx + 0x20;
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT |= DISPCNT_BG1_ON;
        if (!FadeFromWhiteU16(1))
            return 0;
        gMain.seqState0++;
        return 0;
    case 2:
        if (gMain.seqIndex1++ < 120)
            return 0;
        gMain.seqIndex1 = 0;
        gMain.seqState0++;
        return 0;
    default:
        if (!FadeToWhiteU16(1))
            break;
        REG_DISPCNT &= ~DISPCNT_BG1_ON;
        return 1;
    }
    return 0;
}

/* License step 2: the Konami logo on BG0; fades in from white, holds 120 frames, fades out to white. Returns 1
 * when done. */
u16 License_ShowKonamiLogo(void)
{
    switch (gMain.seqState0) {
    case 0:
        ClearBgMapBuffers();
        LoadBgImage(0, 0, 0x20, gKonamiLogoImage);
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT |= DISPCNT_BG0_ON;
        if (!FadeFromWhiteU16(1))
            return 0;
        gMain.seqState0++;
        return 0;
    case 2:
        if (gMain.seqIndex1++ < 120)
            return 0;
        gMain.seqIndex1 = 0;
        gMain.seqState0++;
        return 0;
    default:
        if (!FadeToWhiteU16(1))
            break;
        REG_DISPCNT &= ~DISPCNT_BG0_ON;
        return 1;
    }
    return 0;
}

/*
 * License step 3: the "Konami Computer Entertainment Japan" logo; fades in from white, holds 120 frames and
 * fades to black. Then it switches to the title screen itself (SetMainCallback inlined, without the save):
 * clears both VBlank callbacks and the HBlank IRQ, zeroes the step bytes and installs CB_Title. It never
 * returns 1.
 */
u16 License_ShowKcejLogo(void)
{
    switch (gMain.seqState0) {
    case 0:
        ClearBgMapBuffers();
        LoadBgImage(0, 0, 0x20, gKcejLogoImage);
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT |= DISPCNT_BG0_ON;
        if (FadeFromWhiteU16(1))
            gMain.seqState0++;
        return 0;
    case 2:
        if (gMain.seqIndex1++ >= 120) {
            gMain.seqIndex1 = 0;
            gMain.seqState0++;
        }
        return 0;
    case 3:
        if (FadeToBlackU16(1)) {
            REG_DISPCNT &= ~DISPCNT_BG0_ON;
            gMain.seqState0++;
        }
        return 0;
    default:
        gMain.vblankCallbackEarly = NULL;
        gMain.vblankCallback = NULL;
        DISABLE_HBLANK_INTR();
        SET_HBLANK_HANDLER(NULL);
        gMain.seqIndexTop = 0;
        gMain.seq4879 = 0;
        gMain.seq487A = 0;
        gMain.seqIndexCampaign = 0;
        gMain.seqState0 = 0;
        gMain.seqIndex1 = 0;
        gMain.seqState1 = 0;
        gMain.seqState2 = 0;
        gMain.callback = CB_Title;
        return 0;
    }
}

/* Boot scene callback: runs gLicenseSteps[gMain.seqIndexTop]; a step that returns 1 moves on to the next one
 * with the sub-states reset. Returns 1 at the table's NULL end. */
u16 CB_License(void)
{
    u16 (*step)(void) = gLicenseSteps[gMain.seqIndexTop];

    if (step != NULL) {
        if (step()) {
            gMain.seqIndexTop++;
            gMain.seqState0 = 0;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

/* ---- Title screen ---- */

/* Title VBlank callback: scrolls the BG3 grid diagonally (HOFS = bgScroll, VOFS = bgScroll / 4; bgScroll goes
 * down by one per frame). */
void Title_VBlank(void)
{
    gTitleState.bgScroll--;
    REG_BG3VOFS = gTitleState.bgScroll >> 2;
    REG_BG3HOFS = gTitleState.bgScroll;
}

/*
 * Adds the "New Game" (left) and "Continue" (right) labels, each a 64x32 and a 32x32 sprite at y 0x68. The
 * labels' tiles start at OBJ tile 0x200 + 0x80 * option; the unselected option uses the dimmed copy 12 tiles
 * further on.
 */
void Title_DrawMenu(void)
{
    s32 option;

    for (option = 0; option <= 1; option++) {
        s32 x = 0x18 + option * 0x70;
        s32 t = option * 0x80 + 0x200;
        u16 tile = t;

        if (gTitleState.continueSelected != option)
            tile += 12;
        AddSprite(x | (0x68 << 16), SPRITE_SHAPE_64x32, tile);
        AddSprite((0x58 + option * 0x70) | (0x68 << 16), SPRITE_SHAPE_32x32, tile + 8);
    }
}

/* Display off and the default BG0-3CNT (BG0 8bpp, all on charblock 1, screenblocks 0-3, priorities 0-3). */
void Title_InitBgCnt(void)
{
    REG_DISPCNT = 0;
    REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(1) | BGCNT_256COLOR | BGCNT_SCREENBASE(0);
    REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(1);
    REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(2);
    REG_BG3CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(3);
}

/*
 * One-shot title setup:
 * - renders the menu labels into OBJ tiles from 0x200 (2D mapping, 32 tiles per row; "Continue" 4 rows
 *   below "New Game"): each label twice with a shadow, a bright 16-px copy and a dimmed 12-px copy whose
 *   sprite starts 12 tiles further right;
 * - loads the logo (BG0), flames (BG1), coin (BG2) and grid (BG3) images and tiles the 4x4 grid block over
 *   the whole BG3 map;
 * - halves the brightness of BG palette 12 (colours 0xC0-0xCF, the grid);
 * - copies the flame wave table to gMain.hblankScroll and installs Title_VBlank and Title_HBlank.
 */
void Title_LoadGraphics(void)
{
    s32 col, i;

    CopyDoubleWords((void *)OBJ_PLTT, gSystemFontPal, 0x20);
    TextCanvasInit(32, 16);
    TextDrawString(9, 9, TEXT_SIZE_COLOR(16, 15), gStrNewGame);
    TextDrawString(8, 8, TEXT_SIZE_COLOR(16, 7), gStrNewGame);
    TextDrawString(0x73, 0xB, TEXT_SIZE_COLOR(12, 1), gStrNewGame);
    TextDrawString(0x72, 0xA, TEXT_SIZE_COLOR(12, 13), gStrNewGame);
    TextDrawString(1, 0x29, TEXT_SIZE_COLOR(16, 15), gStrContinue);
    TextDrawString(0, 0x28, TEXT_SIZE_COLOR(16, 7), gStrContinue);
    TextDrawString(0x69, 0x2B, TEXT_SIZE_COLOR(12, 1), gStrContinue);
    TextDrawString(0x68, 0x2A, TEXT_SIZE_COLOR(12, 13), gStrContinue);
    TextCanvasToTiles((u16 *)(OBJ_VRAM0 + 0x4000), 0); /* OBJ tile 0x200 */
    MemCopy16((void *)BG_PLTT, gSystemFontPal, 0x20);
    *(s16 *)BG_PLTT = 0; /* black backdrop */
    /* mapOffset 0x400 * n + cell addresses gMain.bgMapBuffer[n]: BG1, BG2 and BG3 */
    LoadBgImage(0x20, 0x10, 0x10, gTitleLogoImage);
    LoadBgImage4bpp(0x409, 0xA0, 0x2B8, gTitleFlameImage);
    LoadBgImage4bpp(0x809, 0xB0, 0x310, gTitleCoinImage);
    LoadBgImage4bpp(0xC00, 0xC0, 0x388, gTitleGridImage);
    /* Map entries 0xC388-0xC397: palette 12, the grid's tiles 0x388-0x397 in a 4x4 block.
     * Plain constants: postreload move2add turns the reloads into the ROM's `adds r1, #1` chain.
     * The outer counter shares `i` with the palette loop, which puts it in r5. */
    for (i = 0; i <= 0x1F; i += 4) {
        for (col = 0; col <= 0x1F; col += 4) {
            s32 o = (i << 5) + col;

            gMain.bgMapBuffer[3][o] = 0xC388;
            gMain.bgMapBuffer[3][o + 1] = 0xC389;
            gMain.bgMapBuffer[3][o + 2] = 0xC38A;
            gMain.bgMapBuffer[3][o + 3] = 0xC38B;
            gMain.bgMapBuffer[3][o + 0x20] = 0xC38C;
            gMain.bgMapBuffer[3][o + 0x21] = 0xC38D;
            gMain.bgMapBuffer[3][o + 0x22] = 0xC38E;
            gMain.bgMapBuffer[3][o + 0x23] = 0xC38F;
            gMain.bgMapBuffer[3][o + 0x40] = 0xC390;
            gMain.bgMapBuffer[3][o + 0x41] = 0xC391;
            gMain.bgMapBuffer[3][o + 0x42] = 0xC392;
            gMain.bgMapBuffer[3][o + 0x43] = 0xC393;
            gMain.bgMapBuffer[3][o + 0x60] = 0xC394;
            gMain.bgMapBuffer[3][o + 0x61] = 0xC395;
            gMain.bgMapBuffer[3][o + 0x62] = 0xC396;
            gMain.bgMapBuffer[3][o + 0x63] = 0xC397;
        }
    }
    /* BGR555: halve each channel. */
    for (i = 0; i <= 0xF; i++) {
        u16 color = ((u16 *)(BG_PLTT + 0x180))[i];
        u16 r = color & 0x1F;
        u16 g = color & 0x3E0;
        u16 b = color & 0x7C00;

        r = (r >> 1) & 0x1F;
        g = (g >> 1) & 0x3E0;
        b = (b >> 1) & 0x7C00;
        ((u16 *)(BG_PLTT + 0x180))[i] = r | g | b;
    }
    MemCopy16(gMain.hblankScroll, gTitleLogoWave, 0x20);
    gMain.vblankCallback = Title_VBlank;
    SET_HBLANK_HANDLER(Title_HBlank);
    ENABLE_HBLANK_INTR();
}

/* TITLE_STEP_INIT: clears gTitleState and puts the cursor on Continue when a valid save exists; display off,
 * black screen, video and BG reset. Returns 1 when done. */
u16 Title_Init(void)
{
    switch (gMain.seqState0) {
    case 0:
        MemClear16(&gTitleState, 4);
        gTitleState.savePresent = IsSaveChecksumValid();
        gTitleState.continueSelected = gTitleState.savePresent;
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT = 0;
        gMain.seqState0++;
        return 0;
    case 2:
        SetBrightnessBlack();
        ResetVideo();
        ResetBgScroll();
        Title_InitBgCnt();
        gMain.vblankFlags = VBLANK_COPY_OAM | VBLANK_COPY_BG_MAPS;
        gMain.seqState0++;
        return 0;
    }
    return 1;
}

/* TITLE_STEP_SETUP (also where the delete-save prompt's B returns to): display off, video and BG reset, then
 * Title_LoadGraphics and the title song. Returns 1 when done. */
u16 Title_Setup(void)
{
    switch (gMain.seqState0) {
    default:
        Title_LoadGraphics();
        PlayBGMNoTrack(SONG_TITLE);
        return 1;
    case 0:
        REG_DISPCNT = 0;
        gMain.seqState0++;
        return 0;
    case 1:
        SetBrightnessBlack();
        ResetVideo();
        ResetBgScroll();
        Title_InitBgCnt();
        gMain.vblankFlags = VBLANK_COPY_OAM | VBLANK_COPY_BG_MAPS;
        gMain.seqState0++;
        return 0;
    }
}

/*
 * TITLE_STEP_FADE_IN: shows the waving flames and the coin (BG1, BG2) and fades in from black one level every
 * 4th frame; then flashes to white, stops the wave, shows the logo, the grid and the sprites (BG0, BG3, OBJ)
 * with the copyright line and the menu, and fades back from white. Returns 1 when the fade is done.
 */
u16 Title_FadeIn(void)
{
    switch (gMain.seqState0) {
    case 0:
        REG_DISPCNT = DISPCNT_BG1_ON | DISPCNT_BG2_ON;
        gMain.seqState0++;
    case 1:
        if (!(gMain.frameCounter & 3) && FadeFromBlackU16(1))
            gMain.seqState0++;
        return 0;
    case 2:
        if (FadeToWhiteU16(4)) {
            SET_HBLANK_HANDLER(NULL);
            DISABLE_HBLANK_INTR();
            REG_DISPCNT |= DISPCNT_BG0_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON;
            LoadBgImage4bpp(0xA20, 0x90, 0x284, gTitleCopyrightImage); /* BG2 map, row 17 */
            Title_DrawMenu();
            gMain.seqState0++;
        }
        return 0;
    default:
        Title_DrawMenu();
        return FadeFromWhiteU16(1);
    }
}

/* TITLE_STEP_FADE_OUT: draws the menu while fading to black, then drops the VBlank scroll callback. Returns 1
 * when the fade is done. */
u16 Title_FadeOut(void)
{
    Title_DrawMenu();
    if (FadeToBlackU16(4)) {
        gMain.vblankCallback = NULL;
        return 1;
    }
    return 0;
}

/* TITLE_STEP_HANDLE_INPUT: draws the menu; Left/Right toggle New Game/Continue when a save exists (else a
 * buzzer); A confirms, fades out the music and returns 1 (otherwise 0). */
u16 Title_HandleInput(void)
{
    Title_DrawMenu();
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT)) {
        if (gTitleState.savePresent) {
            gTitleState.continueSelected = 1 - gTitleState.continueSelected;
            PlaySE(SE_CURSOR);
        } else {
            PlaySE(SE_ERROR);
        }
    }
    if (gMain.newKeys & A_BUTTON) {
        PlaySE(SE_CONFIRM);
        FadeOutBGM();
        return 1;
    }
    return 0;
}
