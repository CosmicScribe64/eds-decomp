#include "global.h"

#include "main.h"

#include "gba.h"

/* gMain (struct Main) comes from the shared main.h header. */
#define gMain gMain

/* Record scene state (0x0201F814). */
struct Record {
    u16 unk0_0:1;
    u16 unk0_1:2;
    u16 unk0_3:10;
    u16 unk1_5:3; /* bits 13-15 */
    u16 unk2;
};
extern struct Record gRecordScreen;
#define gRecord gRecordScreen

/* Per-index counters in the save mirror (gSaveData + 0x20D0 + idx*4), viewed two ways
 * so agbcc emits the ROM's `ldrh` for the low field and `ldr` for the middle one. */
struct Card2A { u16 a : 11; u16 rest : 5; u16 hi; };
struct Card2B { u32 lo : 11; u32 b : 11; u32 c : 10; };
struct Card2Rec  { u8 pad0[0x20D0]; struct Card2A a; };
struct Card2RecB { u8 pad0[0x20D0]; struct Card2B b; };
struct Card2C { u16 lo; u16 pad : 6; u16 c : 10; };
struct Card2RecC { u8 pad0[0x20D0]; struct Card2C c; };
extern u8 gSaveData[];
extern u16 gRecordMarkerAnimTiles[];

/* 0x0201F7E0: Campaign opponent-select state (shared with campaign_select.c). */
struct OpponentSelect {
    u8 page : 3;        /* bits 0-2 */
    u16 cursor : 3;     /* bits 3-5: selected slot */
    u16 unk6 : 3;       /* bits 6-8 */
    u16 unk9 : 4;       /* bits 9-12 */
    u32 unk13 : 4;      /* bits 13-16 */
    u32 unk17 : 15;
    u16 cursorX[8];     /* +0x04 */
    u16 cursorY[8];     /* +0x14 */
    s32 unk24;          /* +0x24 */
    s32 unk28;          /* +0x28 */
    s32 targetX;        /* +0x2C */
    s32 targetY;        /* +0x30 */
};
extern struct OpponentSelect gOpponentSelect;
#define gSel gOpponentSelect
struct Pos16 { s16 x; s16 y; };
extern struct Pos16 gOpponentSelectSlotPos[5];
extern const u8 *gWinLoseDrawLabels[3]; /* "Win", "Lose", "Draw" */
struct Bob { u16 x; u16 y; };
extern struct Bob gOpponentCursorWobble[];
/* Record scroll offsets [parity][direction - 1][step]; indexed as a real 3-D array so
 * Record_HandleInput keeps the (direction - 1) unfolded. */
extern u16 gRecordScrollHofs[2][2][16];
extern const u8 *gRecordPortraitImages[];
extern const u8 *gRecordPageNameImages[];
extern const u8 gRecordRows5Image[], gRecordRows4Image[], gRecordUnknownPortraitImage[];
s32 StrLen(const u8 *str); /* StrLen */
void AddSprite8bpp(u32 yx, u16 shapeSize, u16 attr2);
void AddSprite8bppAlpha(u32 yx, u16 shapeSize, u16 attr2);
void OpponentSelect_DrawSelectionRing(s32 x, s32 y);
void OpponentSelect_DrawPageArrows(void);

extern u16 gMainMenuCursor; /* gMainMenuCursor */
#define gMainMenuCursor gMainMenuCursor

typedef u16 (*StepFunc)(void);
extern StepFunc gMainMenuTable[]; /* main menu launch table */
extern StepFunc gMainMenuSteps[]; /* main menu steps */
extern StepFunc gRecordSteps[]; /* record steps */
extern const u8 gDaysPerMonth[]; /* days per month */
extern const u8 gMainMenuObjPal[], gMainMenuObjGfx[], gMainMenuSkyImage[];
extern const u8 gRecordMarkerObjPal[], gRecordArrowObjPal[], gRecordMarkerObjGfx[], gRecordArrowObjGfxTop[];
extern const u8 gRecordArrowObjGfxBottom[], gRecordDigitPal[], gRecordDigitGfx[], gRecordBgPatternImage[], gRecordFrameImage[];

u16 FadeToBlack(u16 step); /* FadeToBlack */
u16 FadeFromBlack(u16 step); /* FadeFromBlack */
void AddSprite(u32 yx, u16 shapeSize, u16 attr2); /* AddSprite */
void PlaySE(u16 id); /* PlaySE */
void FadeOutBGM(void);   /* FadeOutBGM */
void SetMainCallback(StepFunc cb); /* SetMainCallback */
void MemClear16(void *dst, u32 size); /* MemClear16 */
void MemCopy16(void *dst, const void *src, u32 size); /* MemCopy16 */
void CopyDoubleWords(void *dst, const void *src, u32 size); /* CopyDoubleWords */
void ResetBgScroll(void); /* ResetBgScroll */
void SetBrightnessBlack(void); /* SetBrightnessBlack */
void ResetVideo(void); /* ResetVideo */
u16 LoadBgImage(u16 mapBase, u16 palIdx, u16 tileBase, const void *img); /* LoadBgImage */
void LoadBgImage4bppToMap(u32 a, u32 b, u32 c, u32 d, const void *e);
void PlayBGMNoTrack(u16 id); /* PlayBGMNoTrack */
u16 IsCampaignLevel2Unlocked(void);
u16 IsCampaignLevel3Unlocked(void);
u16 IsCampaignLevel4Unlocked(void);
u16 IsCampaignLevel5Unlocked(void);

void MainMenu_DrawItems(void);
u16 Record_HasPrevPage(void);
u16 Record_HasNextPage(void);
void Record_DrawPageArrows(void);
void Record_DrawResultMarkers(void);
void Record_DrawSprites(void);
void Record_DrawPage(s32 a, s32 b);
void FillMapRect(u16 row, u16 col, u16 w, u16 h);
s32 IsOpponentUnlocked(u16 id);
u32 IsLeapYear(u32 year);
s32 GetDayOfWeek(u32 year, u32 month, u32 day);
u32 GetHolidayFlags(u32 year, u32 month, u32 day);

/* Opponent-select cursor: shift the 8-position trail back, aim it at the
 * selected slot (easing over 15 frames, else bobbing on idle), draw the trail
 * sprites and then the portrait frame. */
void OpponentSelect_DrawCursor(s32 slot, u16 flag)
{
    s32 i;

    for (i = 7; i != 0; i--) {
        gSel.cursorX[i] = gSel.cursorX[i - 1];
        gSel.cursorY[i] = gSel.cursorY[i - 1];
    }
    gSel.targetX = gOpponentSelectSlotPos[slot].x + 0x10;
    gSel.targetY = gOpponentSelectSlotPos[slot].y;
    if (slot != gSel.unk6) {
        gSel.unk6 = slot;
        gSel.unk9 = 0xF;
        gSel.unk13 = 0;
        gSel.unk24 = gSel.cursorX[0];
        gSel.unk28 = gSel.cursorY[0];
    }
    if (gSel.unk9) {
        s32 dx = gSel.unk24 - gSel.targetX;
        s32 dy = gSel.unk28 - gSel.targetY;
        dx *= gSel.unk9;
        dy *= gSel.unk9;
        dx /= 16;
        dy /= 16;
        gSel.cursorX[0] = gSel.targetX + dx;
        gSel.cursorY[0] = gSel.targetY + dy;
        gSel.unk9--;
    } else {
        gSel.cursorX[0] = gSel.targetX + gOpponentCursorWobble[(gMain.frameCounter >> 1) & 0x1F].x;
        gSel.cursorY[0] = gSel.targetY + gOpponentCursorWobble[(gMain.frameCounter >> 1) & 0x1F].y;
    }
    for (i = 0; i <= 7; i++) {
        if (i == 0) {
            if (flag == 0) {
                REG_BLDCNT = 0;
                REG_BLDALPHA = 0;
            }
            AddSprite8bpp((gSel.cursorY[0] << 16) | gSel.cursorX[0], 0x4080, 0x104);
        } else if (flag == 0) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = 0x808;
            AddSprite8bppAlpha(gSel.cursorX[i] | (gSel.cursorY[i] << 16), 0x4080, 0x104);
        }
    }
    OpponentSelect_DrawSelectionRing(gOpponentSelectSlotPos[slot].x, gOpponentSelectSlotPos[slot].y);
    OpponentSelect_DrawPageArrows();
}
/* Draw a clamped 0..99 number as (up to) two decimal digit sprites at (x, y). */
void OpponentSelect_DrawNumber(s32 x, s32 y, s32 value)
{
    s32 v = value;

    if (v > 99)
        v = 99;
    if (v < 0)
        v = 0;
    AddSprite((x + 8) | (y << 16), 0, ((v % 10) + 0x280) | 0x3000);
    v /= 10;
    if (v > 0)
        AddSprite(x | (y << 16), 0, ((v % 10) + 0x280) | 0x3000);
    else
        AddSprite(x | (y << 16), 0, 0x32A0);
}
/* Record screen: draw a duelist's win/loss/draw labels and counters, centred at the top. */
void OpponentSelect_DrawDuelistInfo(u16 duelist)
{
    s32 x;
    s32 total;
    const u8 **p;
    s32 i;
    u32 y;

    total = 2;
    p = gWinLoseDrawLabels;
    i = 2;
    do {
        total += 0x10 + StrLen(*p) * 5;
        p++;
        i--;
    } while (i >= 0);
    x = 0x78 - total / 2;
    if ((u16)(duelist - 1) > 0x17)
        return;
    y = 0x270000;
    AddSprite(x | y, 0x4080, 0x328A);
    x += StrLen(gWinLoseDrawLabels[0]) * 5;
    {
        u8 *saveBase = (u8 *)gSaveData;
        struct Card2Rec *w;

        w = (struct Card2Rec *)(saveBase + duelist * 4);
        OpponentSelect_DrawNumber(x, 0x28, w->a.a);
    }
    x += 0x11;
    AddSprite(x | y, 0x4080, 0x328E);
    x += StrLen(gWinLoseDrawLabels[1]) * 5;
    {
        struct Card2RecB *w = (struct Card2RecB *)((u8 *)gSaveData + duelist * 4);
        OpponentSelect_DrawNumber(x, 0x28, w->b.b);
    }
    x += 0x11;
    AddSprite(x | y, 0x4080, 0x3292);
    x += StrLen(gWinLoseDrawLabels[2]) * 5;
    {
        struct Card2RecC *w = (struct Card2RecC *)((u8 *)gSaveData + duelist * 4);
        OpponentSelect_DrawNumber(x, 0x28, w->c.c);
    }
    AddSprite(0x000A0048, 0x4080, (gSel.cursor * 0x40 + 0x2C0) | 0x3000);
    AddSprite(0x000A0068, 0x4080, (gSel.cursor * 0x40 + 0x2C4) | 0x3000);
    AddSprite(0x000A0088, 0x4080, (gSel.cursor * 0x40 + 0x2C8) | 0x3000);
}
/* MainMenu_DrawItems: "MENU" header plus 7 rows of 4 sprites; the row under the cursor uses tiles +0x10. */
void MainMenu_DrawItems(void)
{
    s32 i;
    u32 x;

    AddSprite(0x000A0038, 0x4080, 0);
    AddSprite(0x000A0058, 0x4080, 4);
    AddSprite(0x000A0078, 0x4080, 8);
    AddSprite(0x000A0098, 0x4080, 12);
    for (i = 0, x = 0x38; i <= 6; i++) {
        u16 tile = i * 0x40 + 0x40;
        if (gMainMenuCursor == i)
            tile += 0x10;
        AddSprite(((i * 16 + 0x1D) << 16) | x, 0x4080, tile);
        AddSprite(((i * 16 + 0x1D) << 16) | 0x58, 0x4080, tile + 4);
        AddSprite(((i * 16 + 0x1D) << 16) | 0x78, 0x4080, tile + 8);
        AddSprite(((i * 16 + 0x1D) << 16) | 0x98, 0x4080, tile + 12);
    }
}

/* MainMenu_Init: sub-state machine that clears DISPCNT, resets video and BGM,
 * and loads graphics. */
u16 MainMenu_Init(void)
{
    struct Main *main = &gMain;
    u8 *state = &main->seqState1;

    switch (*state) {
    case 0:
        REG_DISPCNT = 0;
        gMainMenuCursor %= 7;
        break;
    case 1:
        SetBrightnessBlack();
        ResetVideo();
        ResetBgScroll();
        REG_BG1CNT = 0x84;
        main->vblankFlags = 3;
        PlayBGMNoTrack(3);
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

/* Main menu step 1: turn on BG1 and OBJ, draw the items, fade in. */
u16 MainMenu_FadeIn(void)
{
    REG_DISPCNT = 0x1200;
    MainMenu_DrawItems();
    return FadeFromBlack(1);
}

/* MainMenu_HandleInput: Up/Down move the cursor (wrapping over 7 items), A confirms. */
u16 MainMenu_HandleInput(void)
{
    MainMenu_DrawItems();
    if (gMain.newKeys & DPAD_UP) {
        gMainMenuCursor += 6;
        gMainMenuCursor %= 7;
        PlaySE(0);
    }
    if (gMain.newKeys & DPAD_DOWN) {
        gMainMenuCursor += 8;
        gMainMenuCursor %= 7;
        PlaySE(0);
    }
    if (gMain.newKeys & A_BUTTON) {
        PlaySE(1);
        FadeOutBGM();
        return 1;
    }
    return 0;
}

/* MainMenu_Launch: fade out, then switch to the scene picked by the cursor. */
u16 MainMenu_Launch(void)
{
    MainMenu_DrawItems();
    if (FadeToBlack(2)) {
        gMain.step488A = 0;
        SetMainCallback(gMainMenuTable[gMainMenuCursor]);
    }
    return 0;
}

/* CB_MainMenu: step runner over gMainMenuSteps, index gMain.seqIndex1. */
u16 CB_MainMenu(void)
{
    StepFunc step = gMainMenuSteps[(u8)gMain.seqIndex1];
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

/* Record step 0: clear the record state, reset video, set up BG0-3. */
u16 Record_Init(void)
{
    MemClear16(&gRecord, 4);
    SetBrightnessBlack();
    ResetBgScroll();
    ResetVideo();
    REG_MOSAIC = 0;
    REG_DISPCNT = 0;
    REG_BG0CNT = 4;
    REG_BG1CNT = 0x4105;
    REG_BG2CNT = 0x4306;
    REG_BG3CNT = 0x0507;
    gMain.vblankFlags = 0x63;
    return 1;
}

/* Record step 1: load palettes/tiles and BG images, draw both panels, reset the view state. */
u16 Record_LoadGfx(void)
{
    MemCopy16((void *)0x05000220, gRecordMarkerObjPal, 0x20);
    MemCopy16((void *)0x06010000, gRecordMarkerObjGfx, 0x200);
    MemCopy16((void *)0x05000200, gRecordArrowObjPal, 0x20);
    MemCopy16((void *)0x06010400, gRecordArrowObjGfxTop, 0x200);
    MemCopy16((void *)0x06010800, gRecordArrowObjGfxBottom, 0x200);
    MemCopy16((void *)0x05000000, gRecordDigitPal, 0x20);
    MemCopy16((void *)0x06004080, gRecordDigitGfx, 0x140);
    LoadBgImage4bppToMap(0, 0, 0x10, 0x10, gRecordFrameImage);
    LoadBgImage4bppToMap(5, 0, 0x20, 0x80, gRecordBgPatternImage);
    Record_DrawPage(0, 0);
    Record_DrawPage(1, 1);
    gRecord.unk0_0 = 0;
    gRecord.unk0_1 = 0;
    gRecord.unk0_3 = 0;
    gRecord.unk1_5 = 0;
    return 1;
}

u16 Record_FadeIn(void)
{
    Record_DrawSprites();
    REG_DISPCNT = 0x1F00;
    return FadeFromBlack(2);
}

/* Record-screen input: when a card is selected, animate/scroll it; otherwise
 * LEFT/RIGHT (with R/L) move the selection and confirm. Returns 1 to leave. */
/* Record_HasNextPage/Record_HasPrevPage are defined later in the unit; the original called them
 * undeclared (implicit int), so their u16 results are tested without narrowing. */
typedef int (*IntFn_08003C78)(void);
u16 Record_HandleInput(void)
{
    Record_DrawSprites();
    if (gRecord.unk0_1) {
        if (gRecord.unk0_3) {
            gRecord.unk0_3--;
            gMain.bgHofs[1] = gRecordScrollHofs[gRecord.unk0_0][gRecord.unk0_1 - 1][gRecord.unk0_3];
            gMain.bgHofs[2] = gRecordScrollHofs[gRecord.unk0_0][gRecord.unk0_1 - 1][gRecord.unk0_3];
        } else {
            switch (gRecord.unk0_1) {
            case 1:
                gRecord.unk1_5++;
                break;
            case 2:
                gRecord.unk1_5--;
                break;
            }
            gRecord.unk0_1 = 0;
            gRecord.unk0_0 = 1 - gRecord.unk0_0;
            if (gRecord.unk0_0) {
                gMain.bgHofs[1] = 0x100;
                gMain.bgHofs[2] = 0x100;
            } else {
                gMain.bgHofs[1] = 0;
                gMain.bgHofs[2] = 0;
            }
        }
    } else {
        if (gMain.newKeys & 0x110) {
            if (((IntFn_08003C78)Record_HasNextPage)()) {
                Record_DrawPage(gRecord.unk1_5 + 1, 1 - gRecord.unk0_0);
                gRecord.unk0_1 = 1;
                gRecord.unk0_3 = 0x10;
                PlaySE(0);
                return 0;
            }
            PlaySE(3);
        }
        if (gMain.newKeys & 0x220) {
            if (((IntFn_08003C78)Record_HasPrevPage)()) {
                Record_DrawPage(gRecord.unk1_5 - 1, 1 - gRecord.unk0_0);
                gRecord.unk0_1 = 2;
                gRecord.unk0_3 = 0x10;
                PlaySE(0);
                return 0;
            }
            PlaySE(3);
        }
    }
    if (gMain.newKeys & 3) {
        PlaySE(2);
        return 1;
    }
    return 0;
}

u16 Record_FadeOut(void)
{
    Record_DrawSprites();
    return FadeToBlack(2);
}

/* CB_Record: step runner over gRecordSteps, index gMain.seqState2. */
u16 CB_Record(void)
{
    StepFunc step = gRecordSteps[(u8)gMain.seqState2];
    if (step != NULL) {
        if (step())
            gMain.seqState2++;
        return 0;
    }
    return 1;
}

u16 Record_HasPrevPage(void)
{
    if (gRecord.unk1_5)
        return 1;
    return 0;
}

u16 Record_HasNextPage(void)
{
    switch (gRecord.unk1_5) {
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

void Record_DrawPageArrows(void)
{
    u32 frame = (gMain.frameCounter >> 3) & 3;
    if (Record_HasPrevPage())
        AddSprite(0x00200010, 0x40, frame * 4 + 0x20);
    if (Record_HasNextPage())
        AddSprite(0x002000E0, 0x40, frame * 4 + 0x22);
}

/* Record scene: draw the up/down arrows left of each of the 4-5 shown cards. */
struct WF3F88Rec { u32 wins : 11; u32 losses : 11; u32 draws : 10; };
struct WF3F88Save { u8 pad0[0x20D0]; struct WF3F88Rec rec[32]; };
#define WF3F88Save (*(struct WF3F88Save *)gSaveData)

void Record_DrawResultMarkers(void)
{
    s32 i, n;
    u32 frame, y;
    s32 diff, d;

    if (gRecord.unk0_1)
        return;
    n = (gRecord.unk1_5 <= 3) ? 5 : 4;
    frame = (gMain.frameCounter >> 3) & 7;
    for (i = 0; i < n; i++) {
        diff = WF3F88Save.rec[gRecord.unk1_5 * 5 + i + 1].wins - WF3F88Save.rec[gRecord.unk1_5 * 5 + i + 1].losses;
        d = (u32)diff >> 31;
        if (diff > 0)
            d = -1;
        AddSprite(((i * 24 + 0x2B) << 16) | (0x84 + d * 0x30), 0x4000, gRecordMarkerAnimTiles[frame] + 0x1000);
    }
}

void Record_DrawSprites(void)
{
    Record_DrawResultMarkers();
    Record_DrawPageArrows();
}

/* Writes a 3-digit decimal number into BG map buffer `bg` at tile (x, y); digit tiles start at 4. */
void Record_DrawNumber(s32 bg, s32 x, s32 y, s32 value)
{
    s32 i;

    x += 2;
    for (i = 0; i < 3; i++) {
        gMain.bgMapBuffer[bg][(u16)x + (u16)y * 32] = value % 10 + 4;
        value /= 10;
        x--;
    }
}
/* Draw the opponent-record panels: a 4- or 5-row block (a = page), each row's
 * portrait plus the win/loss/draw counters, or an empty placeholder. */
/* The ROM passes `k + 1` to IsOpponentUnlocked untruncated, so this caller saw an int parameter
 * (the callee's real prototype takes u16). */
typedef s32 (*WF40E4Fn)(s32);
void Record_DrawPage(s32 a, s32 b)
{
    u16 tileBase = b * 0xC0;
    s32 i, n;

    FillMapRect(b + 1, 0, 0x20, 0x20);
    FillMapRect(b + 3, 0, 0x20, 0x20);
    /* The ternary must already have the parameter's type: a conversion around it makes agbcc
     * evaluate it into a pseudo instead of storing each arm straight into the stack slot. */
    LoadBgImage4bppToMap(b + 3, 0x63, 0x30, tileBase + 0xA0, (a <= 3) ? (const void *)gRecordRows5Image : (const void *)gRecordRows4Image);
    LoadBgImage4bppToMap(b + 1, 0x63, 0x40, tileBase + 0xC0, gRecordPageNameImages[a]);
    n = 4;
    if (a <= 3)
        n = 5;
    for (i = 0; i < n; i++) {
        u16 k = a * 5 + i;

        if (((WF40E4Fn)IsOpponentUnlocked)(k + 1)) {
            LoadBgImage4bppToMap(b + 1, (u16)(i * 3 + 4) * 32 + 4, (i + 5 + b * 5) * 16, tileBase + 0xE0 + i * 12, gRecordPortraitImages[k]);
            Record_DrawNumber(b + 1, 0xD, i * 3 + 5, WF3F88Save.rec[k + 1].wins);
            Record_DrawNumber(b + 1, 0x13, i * 3 + 5, WF3F88Save.rec[k + 1].draws);
            Record_DrawNumber(b + 1, 0x19, i * 3 + 5, WF3F88Save.rec[k + 1].losses);
        } else {
            LoadBgImage4bppToMap(b + 1, (u16)(i * 3 + 4) * 32 + 4, (i + 5 + b * 5) * 16, tileBase + 0xE0 + i * 12, gRecordUnknownPortraitImage);
            FillMapRect(b + 1, i * 0x60 + 0x63, 8, 1);
        }
    }
}

/* Leap year test (Gregorian). */
u32 IsLeapYear(u32 year)
{
    if ((year & 3) != 0 || (year % 100 == 0 && year % 400 != 0))
        return 0;
    return 1;
}

/* Days in month (1-based), with Feb +1 in leap years. */
u32 GetDaysInMonth(u32 year, u32 month)
{
    u32 days = gDaysPerMonth[month - 1];
    if (month == 2)
        days += IsLeapYear(year);
    return days;
}

/* Day of the week (0 = Sunday) for a date, counting days since 2000-01-01 (a Saturday). */
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

/* Holiday bit mask for a date (Japanese public holidays of ~2000-2002 plus two extra days). */
u32 GetHolidayFlags(u32 year, u32 month, u32 day)
{
    u32 flags = 0;

    switch (month) {
    case 1:
        if (day == 1)
            flags |= 1;
        GetDayOfWeek(year, month, 1);
        GetDayOfWeek(year, month, day);
        if ((day - 1) / 7 == 1 && GetDayOfWeek(year, month, day) == 1)
            flags |= 0x800; /* Coming of Age Day: 2nd Monday */
        break;
    case 2:
        if (day == 11)
            flags |= 2;
        if (day == 24)
            flags |= 0x2000;
        break;
    case 4:
        if (day == 29)
            flags |= 4;
        break;
    case 5:
        switch (day) {
        case 3:
            flags |= 8;
            break;
        case 4:
            flags |= 0x10;
            break;
        case 5:
            flags |= 0x20;
            break;
        }
        break;
    case 7:
        if (day == 20)
            flags |= 0x40;
        if (day == 7)
            flags |= 0x4000;
        break;
    case 9:
        if (day == 15)
            flags |= 0x80;
        break;
    case 10:
        GetDayOfWeek(year, month, 1);
        GetDayOfWeek(year, month, day);
        if ((day - 1) / 7 == 1 && GetDayOfWeek(year, month, day) == 1)
            flags |= 0x1000; /* Health and Sports Day: 2nd Monday */
        break;
    case 11:
        switch (day) {
        case 3:
            flags |= 0x100;
            break;
        case 23:
            flags |= 0x200;
            break;
        }
        break;
    case 12:
        if (day == 23)
            flags |= 0x400;
        break;
    }
    return flags;
}

/* TRUE if the date is a "red" day: Sunday, a holiday, or the Monday after a holiday. */
u32 IsDayOff(u32 year, u32 month, u32 day)
{
    if (GetDayOfWeek(year, month, day) == 0
        || (GetHolidayFlags(year, month, day) & 0x7FFF)
        || (GetDayOfWeek(year, month, day) == 1 && (GetHolidayFlags(year, month, day - 1) & 0x7FFF)))
        return 1;
    return 0;
}
