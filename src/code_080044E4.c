#include "global.h"

#include "gba.h"

/* gMain: the big system struct at 0x03000040 (see wiki ram-map). */
struct Main {
    u32 rngState;                       /* 0x0000 */
    u16 heldKeys;                       /* 0x0004 */
    u16 newKeys;                        /* 0x0006 */
    u16 prevKeys;                       /* 0x0008 */
    u16 keyRepeatTimer;                 /* 0x000A */
    u8 intrMainBuf[0x400];              /* 0x000C */
    u16 intrCheck;                      /* 0x040C */
    u16 vblankFlags;                    /* 0x040E */
    u16 (*callback)(void);              /* 0x0410 */
    void (*vblankCallback)(void);       /* 0x0414 */
    void (*vblankCallbackEarly)(void);  /* 0x0418 */
    u16 bgMapBuffer[8][0x400];          /* 0x041C */
    u16 unk441C;                        /* 0x441C */
    u16 unk441E;                        /* 0x441E */
    u16 bgVofs[4];                      /* 0x4420 */
    u16 bgHofs[4];                      /* 0x4428 */
    u8 oamBuffer[0x400];                /* 0x4430 */
    u8 oamCount;                        /* 0x4830 */
    u8 unk4831;                         /* 0x4831 */
    u8 brightness;                      /* 0x4832 */
    u8 unk4833;                         /* 0x4833 */
    u16 unk4834;                        /* 0x4834 */
    u16 hblankScroll[16];               /* 0x4836 */
    u8 unk4856;                         /* 0x4856 */
    u8 seqIndexCampaign;                /* 0x4857 */
    u8 seqState0;                       /* 0x4858 */
    u8 seqIndex1;                       /* 0x4859 */
    u8 seqState1;                       /* 0x485A */
    u8 seqState2;                       /* 0x485B */
    u16 currentBgm;                     /* 0x485C */
    u16 frameCounter;                   /* 0x485E */
    u8 frameCounter8;                   /* 0x4860 */
    u8 vblankCounter8;                  /* 0x4861 */
    u16 unk4862;                        /* 0x4862 */
    u16 vblankCounter;                  /* 0x4864 */
    u16 lagCounter;                     /* 0x4866 */
    u16 lastSeFrame;                    /* 0x4868 */
    u8 filler486A[0x4878 - 0x486A];     /* 0x486A */
    u8 seqIndexTop;                     /* 0x4878 */
    u8 unk4879;                         /* 0x4879 */
    u8 unk487A;                         /* 0x487A */
};

/* gTitleState at 0x0201527C */
struct TitleState {
    u16 scroll;                         /* 0x0: BG3 scroll, decremented each frame */
    u16 savePresent : 1;                 /* 0x2 bit 0 */
    u16 continueSelected : 1;            /* 0x2 bit 1 */
};

extern struct Main gMain;
#define gMain gMain
extern struct TitleState gTitleState;
#define gTitleState gTitleState
/* gSaveData at 0x02011C20 (0x2170-byte save image) */
struct SaveData {
    u8 filler0[0x2150];
    u16 days;                           /* 0x2150: in-game calendar day count (hypothesis) */
    u8 filler2152[0x215E - 0x2152];
    u16 unk215E;                        /* 0x215E: unlock counter (hypothesis) */
    u16 unk2160;                        /* 0x2160 */
};
extern struct SaveData gSaveData;
#define gSaveData gSaveData

extern u16 (*const gLicenseSteps[])(void);
extern const u8 gKonamiLogoImage[];
extern const u8 gTitleCopyrightImage[];
/* IWRAM 0x03000000: interrupt vectors (hypothesis: +4 = HBlank callback) */
struct IntrVectors {
    u32 unk0;
    void (*hblankCallback)(void);
};
extern struct IntrVectors IntrTable;
u16 FadeFromBlack(u16 step);
u16 CB_Title(void);
s32 StrLen(const u8 *str); /* StrLen */
void TextDrawString(s32 x, s32 y, u16 color, const u8 *str); /* DrawText (hypothesis) */
void TextCanvasToTiles(void *dest, u16 b);
void TextCanvasInit(u8 a, u8 b);
extern const u8 gKcejLogoImage[];

/* Starting-deck card pool (0x08198744, 11 entries). See [[deck-lists]]. */
struct DeckPool {
    const u16 *cards;
    u32 count:10;       /* cards in the pool */
    u32 take0:5;        /* copies drawn for starting choice 0 */
    u32 take1:5;        /* ... choice 1 */
    u32 take2:5;        /* ... choice 2 */
};
extern const struct DeckPool gStarterDeckPools[];
extern const u16 gCardNumberToId[]; /* card ID to card index */
extern const u8 gStrStarterDeckErrorFmt[];
s32 Random(void); /* Random */
void AddCardToSavedDeck(u16 card);
void DebugPrintf(const u8 *fmt, u32 arg);
void DebugPrintFlush(void);

/* Converts a card ID (0..1999 and 2000+) to a card index; 0xFFFF maps to 0. Inlined wherever it appears. */
static inline u16 CardIdToIndex(u16 id)
{
    if (id == 0xFFFF)
        return 0;
    if (id < 2000) {
        /* FAKEMATCH: keep the initialized lookup mask and its working copy
         * separate, then add the table to the already scaled byte offset. */
        register u32 mask asm("r3") = 0x7FF;
        register u32 copy asm("r1") = mask;
        u32 off;
        register const u16 *table asm("r4");

        asm("" : : "r"(mask));
        off = (id & copy) * 2;
        table = gCardNumberToId;
        off += (u32)table;
        return *(const u16 *)off;
    }
    {
        u32 off = ((id - 2000) & 0x7FF) * 2;
        /* FAKEMATCH: the alternate-ID path uses a separate table scratch. */
        register const u16 *table asm("r3");

        table = gCardNumberToId;
        off += (u32)table;
        return *(const u16 *)off + 1;
    }
}
extern const u8 gStrLicensedByNintendo[];
void LoadBgImage4bpp(u16 a, u16 b, u16 c, const void *img);

u32 GetDayOfWeek(u32 year, u32 month, u32 day); /* day of week */
u32 IsLeapYear(u32 year);
u32 GetHolidayFlags(u32 year, u32 month, u32 day);
u32 IsDayOff(u32 a, u32 b, u32 c);
/* Unpacked date */
struct Date {
    u32 year:12;
    u32 month:4;
    u32 day:5;
    u32 weekday:3;
};
void DayCountToDate(struct Date *date, u16 days);
extern const u8 gDaysPerMonth[]; /* days per month */
void Title_DrawMenu(void);
void Title_InitBgCnt(void);
void Title_LoadGraphics(void);
void LoadBgImage(u16 mapBase, u16 palIdx, u16 tileBase, const void *img);
void ClearBgMapBuffers(void);
void ResetVideo(void);
void MemClear16(void *dst, u32 size);
void ResetBgScroll(void);
void SetBrightnessBlack(void);
void SetBrightnessWhite(void);
u16 FadeToBlack(u16 step);
u16 FadeToWhite(u16 step);
u16 FadeFromWhite(u16 step);
void AddSprite(u32 yx, u16 shapeSize, u16 attr2);
u32 IsSaveChecksumValid(void);
void PlaySE(u16 id);
void PlayBGMNoTrack(u16 id);
void FadeOutBGM(void);

/*
 * Returns a bitmask of restrictions/flags for the calendar entry (arg0, arg1, arg2),
 * which is a year, a month and a day-like value. Starts from GetHolidayFlags()'s flags and
 * ORs in bits per case, then checks surrounding days via IsDayOff (day-info) and
 * the save-data unlock counters.
 */
static inline u32 WeekOfMonth(u32 year, u32 month, u32 day)
{
    GetDayOfWeek(year, month, 1);
    GetDayOfWeek(year, month, day);
    return (day - 1) / 7 + 1;
}

u32 GetCalendarEvents(u32 year, u32 month, u32 day)
{
    u32 bit20, bit21;
    u32 flags = GetHolidayFlags(year, month, day);

    switch (month - 2) {
    case 0:
        if (day == 0xE)
            flags |= 0x40000;
        break;
    case 1:
        if (day == 0xE)
            flags |= 0x80000;
        break;
    case 4:
        if (day == 0x1C)
            flags |= 0x8000;
        if (WeekOfMonth(year, month, day) == 1 && GetDayOfWeek(year, month, day) == 6)
            flags |= 0x10000000;
        if (WeekOfMonth(year, month, day - 1) == 1 && GetDayOfWeek(year, month, day - 1) == 6
            && gSaveData.unk2160 != 0)
            flags |= 0x20000000;
        break;
    case 8:
        if (day == 0x1F)
            flags |= 0x10000;
        break;
    case 9:
        if (GetDayOfWeek(year, month, day) == 0) {
            switch (WeekOfMonth(year, month, day)) {
            case 1:
                flags |= 0x1000000;
                break;
            case 2:
                if (gSaveData.unk215E != 0)
                    flags |= 0x2000000;
                break;
            case 3:
                if (gSaveData.unk215E > 1)
                    flags |= 0x4000000;
                break;
            case 4:
                if (gSaveData.unk215E > 2)
                    flags |= 0x8000000;
                break;
            }
        }
        break;
    case 10:
        if (day == 0x18)
            flags |= 0x20000;
        break;
    }
    if (GetDayOfWeek(year, month, day) == 6) {
        u32 v = WeekOfMonth(year, month, day);
        if (v == 2 || v == 4)
            flags |= 0x400000;
    }
    /* The shared exit keeps flags at one return use, which lets month win r6 over flags. */
    if ((IsDayOff(year, month, day) << 16) != 0)
        goto end;
    bit20 = 0;
    bit21 = 0;
    if (GetDayOfWeek(year, month, day) == 2
        && (year > 0x7D1 || month > 1 || day > 2))
        bit20 = 1;
    if (GetDayOfWeek(year, month, day) == 1
        && (IsDayOff(year, month, day + 1) << 16) != 0)
        bit20 = 1;
    if (GetDayOfWeek(year, month, day) == 6
        && (IsDayOff(year, month, day + 2) << 16) != 0
        && (IsDayOff(year, month, day + 3) << 16) != 0)
        bit20 = 1;
    if (bit20 != 0)
        flags |= 0x100000;
    if (day == 0x15)
        bit21 = 1;
    if (day == 0x14 && (IsDayOff(year, month, 0x15) << 16) != 0)
        bit21 = 1;
    if (day == 0x13 && (IsDayOff(year, month, 0x14) << 16) != 0
        && (IsDayOff(year, month, 0x15) << 16) != 0)
        bit21 = 1;
    if (day == 0x12 && (IsDayOff(year, month, 0x13) << 16) != 0
        && (IsDayOff(year, month, 0x14) << 16) != 0
        && (IsDayOff(year, month, 0x15) << 16) != 0)
        bit21 = 1;
    if (bit21 != 0)
        flags |= 0x200000;
end:
    return flags;
}

/* Converts a day count (day 0 = 2001-01-01) into a packed date plus weekday.
 * Day 36524 (2100-02-29, not a leap day) is skipped. */
void DayCountToDate(struct Date *date, u16 days)
{
    u16 year, month;
    s32 monthLen = 31;

    if (days > 36523)
        days++;
    year = days / 1461 * 4;
    days = days % 1461;
    year += days / 365 + 2001;
    if (days == 1460) {
        days = 365;
        year--;
    } else {
        days = days % 365;
    }
    month = 0;
    while (days >= monthLen) {
        days -= monthLen;
        month++;
        monthLen = gDaysPerMonth[month];
        if (month == 1)
            monthLen += IsLeapYear(year);
    }
    date->year = year;
    date->month = month + 1;
    date->day = days + 1;
    date->weekday = GetDayOfWeek(year, month + 1, days + 1);
    if (date->day == 0)
        date->day++;
}
void GetCurrentDate(struct Date *date)
{
    DayCountToDate(date, gSaveData.days);
}

u32 GetWeekOfMonth(u32 a, u32 b, u32 c)
{
    GetDayOfWeek(a, b, 1);
    GetDayOfWeek(a, b, c);
    return (c - 1) / 7 + 1;
}

/* Builds the starting deck for `choice` (choice % 3) by shuffling each of the 11 pools
 * and adding the first take<n> cards of each. The scene supplies choice 0..2. */
void BuildStarterDeck(s32 choice)
{
    u16 buf[64];
    const struct DeckPool *pool = gStarterDeckPools;
    u32 p;
    s32 i, n;

    for (p = 0; p <= 10; pool++, p++) {
        const u16 *src = pool->cards;

        for (i = 0; i < pool->count; i++)
            buf[i] = src[i];
        for (i = 0; i < pool->count * 4; i++) {
            s32 a = Random() % pool->count;
            s32 b = Random() % pool->count;
            u16 t = buf[a];
            buf[a] = buf[b];
            buf[b] = t;
        }
        switch (choice % 3) {
        case 0:
            n = pool->take0;
            break;
        case 1:
            n = pool->take1;
            break;
        case 2:
            n = pool->take2;
            break;
        }
        for (i = 0; i < n; i++) {
            u16 id = buf[i % pool->count];
            u16 idx = CardIdToIndex(id);

            if (idx)
                AddCardToSavedDeck(idx);
            else
                DebugPrintf(gStrStarterDeckErrorFmt, id);
        }
    }
    DebugPrintFlush();
}

/* HBlank handler: wavy BG1 horizontal scroll */
void Title_HBlank(void)
{
    REG_BG1HOFS = gMain.hblankScroll[(REG_VCOUNT + gMain.frameCounter) & 0xF];
}

/* License step 0: License_InitVideo */
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
        gMain.vblankFlags = 3;
        ResetVideo();
        ResetBgScroll();
        REG_BG0CNT = 0x84;
        REG_BG1CNT = 0x105;
        REG_BG2CNT = 0x206;
        REG_BG3CNT = 0x307;
        *(vu16 *)0x05000000 = 0xFFFF;
        gMain.seqState0++;
        return 0;
    }
}
/* License step 1: draws the centred notice text 0x080813F0 (drawn twice, offset, as an outline) and holds it. */
u16 License_ShowNintendoNotice(void)
{
    s32 x, i, j, k;

    switch (gMain.seqState0) {
    case 0:
        ClearBgMapBuffers();
        TextCanvasInit(0x20, 3);
        x = (240 - StrLen(gStrLicensedByNintendo) * 9) / 2;
        for (i = 1; i >= 0; i--)
            for (j = 0; j <= 1; j++)
                for (k = 0; k <= 0; k++)
                    TextDrawString(x + j + i, k + i, i == 1 ? 0x100F : 0x1008, gStrLicensedByNintendo);
        TextCanvasToTiles((void *)0x06004400, 0);
        for (j = 0; j < 96; j++)
            gMain.bgMapBuffer[1][0x120 + j] = j + 0x20;
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT |= 0x200;
        if (!FadeFromWhite(1))
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
        if (!FadeToWhite(1))
            break;
        REG_DISPCNT &= ~0x200;
        return 1;
    }
    return 0;
}
/* License step 2: shows the logo image 0x087D01F4, holds it 120 frames, fades out. */
u16 License_ShowKonamiLogo(void)
{
    switch (gMain.seqState0) {
    case 0:
        ClearBgMapBuffers();
        LoadBgImage(0, 0, 0x20, gKonamiLogoImage);
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT |= 0x100;
        if (!FadeFromWhite(1))
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
        if (!FadeToWhite(1))
            break;
        REG_DISPCNT &= ~0x100;
        return 1;
    }
    return 0;
}
/* License step 3: second logo (0x087D292C), then hands over to the title screen. */
u16 License_ShowKcejLogo(void)
{
    switch (gMain.seqState0) {
    case 0:
        ClearBgMapBuffers();
        LoadBgImage(0, 0, 0x20, gKcejLogoImage);
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT |= 0x100;
        if (FadeFromWhite(1))
            gMain.seqState0++;
        return 0;
    case 2:
        if (gMain.seqIndex1++ >= 120) {
            gMain.seqIndex1 = 0;
            gMain.seqState0++;
        }
        return 0;
    case 3:
        if (FadeToBlack(1)) {
            REG_DISPCNT &= ~0x100;
            gMain.seqState0++;
        }
        return 0;
    default:
        gMain.vblankCallbackEarly = NULL;
        gMain.vblankCallback = NULL;
        REG_IME = 0;
        REG_IE &= ~2;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE &= ~2;
        IntrTable.hblankCallback = NULL;
        REG_IME = 1;
        gMain.seqIndexTop = 0;
        gMain.unk4879 = 0;
        gMain.unk487A = 0;
        gMain.seqIndexCampaign = 0;
        gMain.seqState0 = 0;
        gMain.seqIndex1 = 0;
        gMain.seqState1 = 0;
        gMain.seqState2 = 0;
        gMain.callback = CB_Title;
        return 0;
    }
}

/* CB_License: runs the step table at 0x0819879C */
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

/* Title VBlank callback: scroll BG3 diagonally */
void Title_VBlank(void)
{
    gTitleState.scroll--;
    REG_BG3VOFS = gTitleState.scroll >> 2;
    REG_BG3HOFS = gTitleState.scroll;
}

/*
 * Draws the "New Game" / "Continue" labels (a 64x32 + 32x32 sprite each).
 * The unselected option uses the tiles 12 further on (the dimmed version).
 */
void Title_DrawMenu(void)
{
    s32 i;

    for (i = 0; i <= 1; i++) {
        s32 x = 0x18 + i * 0x70;
        s32 t = i * 0x80 + 0x200;
        u16 tile = t;
        if (gTitleState.continueSelected != i)
            tile += 12;
        AddSprite(x | (0x68 << 16), 0x40C0, tile);
        AddSprite((0x58 + i * 0x70) | (0x68 << 16), 0x80, tile + 8);
    }
}

void Title_InitBgCnt(void)
{
    REG_DISPCNT = 0;
    REG_BG0CNT = 0x84;
    REG_BG1CNT = 0x105;
    REG_BG2CNT = 0x206;
    REG_BG3CNT = 0x307;
}

/* Title step: sets up the title screen. Copies the palettes/text/logo images, fills a
 * 4x4-tile pattern into the IWRAM tile map, halves the brightness of palette entries
 * 0xC0..0xCF, copies the HBlank scroll table and installs the VBlank/HBlank callbacks. */
extern u8 gUnk_03004876[];
extern const u8 gSystemFontPal[];
extern const u8 gStrNewGame[];
extern const u8 gStrContinue[];
extern const u8 gTitleLogoWave[];
extern const u8 gTitleLogoImage[];
extern const u8 gTitleFlameImage[];
extern const u8 gTitleCoinImage[];
extern const u8 gTitleGridImage[];
void CopyDoubleWords(void *dst, const void *src, u32 size);
void MemCopy16(void *dst, const void *src, u32 size);
void Title_VBlank(void);
void Title_HBlank(void);
/* Title screen setup: palettes and graphics, a repeating 4x4 tile block over BG map 3,
 * half-brightness palette entries 0xC0..0xCF, HBlank scroll table and the VBlank/HBlank callbacks. */
void Title_LoadGraphics(void)
{
    s32 x, i;

    CopyDoubleWords((void *)0x05000200, gSystemFontPal, 0x20);
    TextCanvasInit(0x20, 0x10);
    TextDrawString(9, 9, 0x100F, gStrNewGame);
    TextDrawString(8, 8, 0x1007, gStrNewGame);
    TextDrawString(0x73, 0xB, 0xC01, gStrNewGame);
    TextDrawString(0x72, 0xA, 0xC0D, gStrNewGame);
    TextDrawString(1, 0x29, 0x100F, gStrContinue);
    TextDrawString(0, 0x28, 0x1007, gStrContinue);
    TextDrawString(0x69, 0x2B, 0xC01, gStrContinue);
    TextDrawString(0x68, 0x2A, 0xC0D, gStrContinue);
    TextCanvasToTiles((void *)0x06014000, 0);
    MemCopy16((void *)0x05000000, gSystemFontPal, 0x20);
    *(s16 *)0x05000000 = 0;
    LoadBgImage(0x20, 0x10, 0x10, gTitleLogoImage);
    LoadBgImage4bpp(0x409, 0xA0, 0x2B8, gTitleFlameImage);
    LoadBgImage4bpp(0x809, 0xB0, 0x310, gTitleCoinImage);
    LoadBgImage4bpp(0xC00, 0xC0, 0x388, gTitleGridImage);
    /* Plain constants: postreload move2add turns the reloads into the ROM's `adds r1, #1` chain.
     * The outer counter shares `i` with the palette loop, which puts it in r5. */
    for (i = 0; i <= 0x1F; i += 4) {
        for (x = 0; x <= 0x1F; x += 4) {
            s32 o = (i << 5) + x;

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
    for (i = 0; i <= 0xF; i++) {
        u16 c = ((u16 *)0x05000180)[i];
        u16 r = c & 0x1F;
        u16 g = c & 0x3E0;
        u16 b = c & 0x7C00;

        r = (r >> 1) & 0x1F;
        g = (g >> 1) & 0x3E0;
        b = (b >> 1) & 0x7C00;
        ((u16 *)0x05000180)[i] = r | g | b;
    }
    MemCopy16(gMain.hblankScroll, gTitleLogoWave, 0x20);
    gMain.vblankCallback = Title_VBlank;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    IntrTable.hblankCallback = Title_HBlank;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE |= 2;
    REG_IME = 1;
}
/* Title step 0: Title_Init */
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
        gMain.vblankFlags = 3;
        gMain.seqState0++;
        return 0;
    }
    return 1;
}
/* Title step 1: Title_Setup */
u16 Title_Setup(void)
{
    switch (gMain.seqState0) {
    default:
        Title_LoadGraphics();
        PlayBGMNoTrack(0);
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
        gMain.vblankFlags = 3;
        gMain.seqState0++;
        return 0;
    }
}
/* Title step 2: fade in, then drop the HBlank handler and draw the menu labels. */
u16 Title_FadeIn(void)
{
    switch (gMain.seqState0) {
    case 0:
        REG_DISPCNT = 0x600;
        gMain.seqState0++;
    case 1:
        if (!(gMain.frameCounter & 3) && FadeFromBlack(1))
            gMain.seqState0++;
        return 0;
    case 2:
        if (FadeToWhite(4)) {
            REG_IME = 0;
            REG_IE &= ~2;
            IntrTable.hblankCallback = NULL;
            REG_IME = 1;
            REG_IME = 0;
            REG_IE &= ~2;
            REG_IME = 1;
            REG_DISPCNT |= 0x1900;
            LoadBgImage4bpp(0xA20, 0x90, 0x284, gTitleCopyrightImage);
            Title_DrawMenu();
            gMain.seqState0++;
        }
        return 0;
    default:
        Title_DrawMenu();
        return FadeFromWhite(1);
    }
}

/* Title step 4: Title_FadeOut */
u16 Title_FadeOut(void)
{
    Title_DrawMenu();
    if (FadeToBlack(4)) {
        gMain.vblankCallback = NULL;
        return 1;
    }
    return 0;
}

/* Title step 3: Title_HandleInput */
u16 Title_HandleInput(void)
{
    Title_DrawMenu();
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT)) {
        if (gTitleState.savePresent) {
            gTitleState.continueSelected = 1 - gTitleState.continueSelected;
            PlaySE(0);
        } else {
            PlaySE(3);
        }
    }
    if (gMain.newKeys & A_BUTTON) {
        PlaySE(1);
        FadeOutBGM();
        return 1;
    }
    return 0;
}
