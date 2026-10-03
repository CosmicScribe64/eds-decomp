/*
 * text_canvas (0x080740BC-0x080750E0): the multi-player SIO link step, the unused debug menu
 * and the text canvas glyph renderers (wiki/functions/text-canvas-c.md).
 *
 * LinkSioMain / LinkSioStartTransfer / LinkSioSetSendData / LinkSioCheckRecvData are the
 * per-frame step of the multi-player SIO driver (struct LinkSio in link.h, modelled on the
 * SDK's MultiSio): the GBA with SI low becomes the parent and clocks the transfers from
 * Timer3. Every frame moves one 10-halfword frame per player; word 1 is the negated
 * checksum, so a valid frame sums to 0xFFFF.
 *
 * CB_DebugMenu runs the unreferenced developer menu (gDebugMenuSteps): DebugMenu_Init sets
 * up the screen, DebugMenu_DrawAndFadeIn draws the build number, the item list, the save's
 * language, today's date and its calendar events, DebugMenu_HandleInput moves the cursor
 * (Left/Right/L/R also change gSaveData.days), and DebugMenu_Launch installs the picked
 * item's callback. The CB_Debug* functions are the menu items themselves (cheats and the
 * Exodia / Destiny Board scene players).
 *
 * The Text* functions plot 1bpp font glyphs into the 8bpp tile-ordered canvas gTextCanvas
 * (0x02000000): TextPlotRow8 / TextPlotRow16 write one glyph row, the glyph renderers pick
 * the 8/10/12(/16) px font from the size byte of the sizeColor argument, and the string
 * renderers word-wrap (Japanese kinsoku rules on the Shift-JIS path) and track the drawn
 * extents in the canvas header. gSaveData.sjisText selects the Shift-JIS path; the USA game
 * always draws Latin.
 */
#include "global.h"
#include "gba.h"                    /* REG_SIOCNT, REG_TM3CNT_L/H, REG_IE, REG_IF, REG_IME, REG_DISPCNT,
                                   REG_BG0CNT, CpuSet, A_BUTTON, B_BUTTON, DPAD_UP/DOWN/LEFT/RIGHT,
                                   R_BUTTON, L_BUTTON */
#include "calendar.h"               /* struct Date, GetCurrentDate, GetHolidayFlags, GetCalendarEvents, GetDayOfWeek */
#include "constants/game.h"         /* enum Language */
#include "debug.h"                  /* struct DebugMenuItem, struct CalendarEventName, CB_Debug* / DebugMenu_* (defined here) */
#include "link.h"                   /* struct LinkSio gLinkSio, enum LinkSioPacketType / LinkSioType / LinkSioStatus,
                                   LinkSioMain / LinkSioStartTransfer / LinkSioSetSendData / LinkSioCheckRecvData (defined here) */
#include "main.h"                   /* struct Main gMain (newKeys, vblankFlags, seqIndexCampaign, seqIndex1, seqState0..2) */
#include "save.h"                   /* struct SaveData gSaveData (language, sjisText, days), InitSaveData, RecordDuelWin */

/* The debug menu's tables (debug.h: used by one unit each, so they stay local externs here). */
extern struct DebugMenuItem gDebugMenuItems[];          /* 0x081A73A0: label + scene callback per item */
extern u16 (*gDebugMenuSteps[])(void);                  /* 0x081A768C: the menu step functions */
extern struct CalendarEventName gCalendarEventNames[];  /* 0x08087720: event bit -> debug label */

/* Debug menu text (ROM strings). */
extern u8 gStrDebugDateTemplate[];  /* 0x08087B34 */
extern u8 gStrRareHunterComing[];   /* 0x08087B40 */
extern u8 gStrLangEnglish[];        /* 0x08087B58 */
extern u8 gStrLangJapanese[];       /* 0x08087B60 */
extern u8 gStrLangGerman[];         /* 0x08087B68 */
extern u8 gStrLangFrench[];         /* 0x08087B70 */
extern u8 gStrLangItalian[];        /* 0x08087B78 */

/* The two win scenes the debug items play, and the campaign unlock checks of 'Next Level'. */
extern u16 ExodiaScene_Run(void);
/* Matching: duel_scenes.h declares a u32 return; this unit tests the narrowed u16 result. */
extern u16 DestinyBoardScene_Run(void);
extern u32 IsCampaignLevel2Unlocked(void);
extern u32 IsCampaignLevel3Unlocked(void);
extern u32 IsCampaignLevel4Unlocked(void);
extern u32 IsCampaignLevel5Unlocked(void);

/* Video setup of DebugMenu_Init, the BG text writers and the main-callback setter. */
extern void ResetVideo(void);
extern void ResetBgScroll(void);
extern void SetBrightnessBlack(void);
extern void LoadSystemGfx(void);
extern void ClearBgMapBuffer0(void);
extern void SetMainCallback(void *);
/* Matching: text.h declares these with u16 cell/colors/tile and a u32 SetTextArea pair of
 * cells; the debug menu passes packed full-width arguments (e.g. 0x08070033), so the wide
 * views stay here (build/readability/issues/text_canvas.md). */
extern void DrawBgString(u32, u32, u32, const void *);
extern void DrawBgDecimal(u32, u32, u32, u32);
extern void SetTextArea(u32, u32);
/* Matching: palette.h declares a u32 return; DebugMenu_DrawAndFadeIn returns the narrowed
 * u16 step result (same form as link_sio.c). */
extern u16 FadeFromBlack(u32);
/* Matching: sprite.h declares AddSprite(u32, u16, u16); the cursor call passes full-width
 * packed coordinates, so the wide view stays here. */
extern void AddSprite(u32, u32, u32);
extern void MemClear16(void *, u32);
extern u32 __umodsi3(u32, u32);

extern u8 gDuelScene[];             /* 0x02017A30 (struct DuelScene in duel_scenes.h; only byte +0xB,
                                       the scene step, is written here) */
/* &gLinkSio.rxWork, declared by the original unit and never used (link.h); kept as declared. */
extern u8 *gUnk_03006598;           /* 0x03006598 */

/* ---- Local views kept for matching (build/readability/issues/text_canvas.md) ----
 * text.h cannot be included here: its `extern struct TextCanvas gTextCanvas` conflicts with
 * the byte view below, and DrawBgString / DrawBgDecimal / SetTextArea and the kinsoku tests
 * are called through the wider views above. The Text* prototypes are therefore repeated
 * here as defined (identical to text.h); the renderers are defined below. */

/* The text canvas as bytes: gTextCanvas[0x10000 + n] reaches the header (width, height,
 * right, bottom, layout) with base + separate offset literal like the ROM; a struct view
 * would fold the two into one literal (text.h). */
extern u8 gTextCanvas[];            /* 0x02000000 */
extern const u16 gAsciiToSjisTable[];   /* 0x081A76A0: ASCII 0x20..0x7E -> full-width Shift-JIS */

/* 1bpp fonts (bit 7 / bit 15 = leftmost pixel); addressed as u16 rows by the glyph renderers. */
extern u16 gFontKanji8x8[];         /* 0x081C0000 */
extern u16 gFontKanji10x10[];       /* 0x081D0200 */
extern u16 gFontKanji12x12[];       /* 0x081F8700 */
extern u16 gFontLatin8x8[];         /* 0x08228D00 */
extern u16 gFontLatin8x10[];        /* 0x08229500 */
extern u16 gFontLatin8x12[];        /* 0x08229F00 */
extern u16 gFontLatin8x16[];        /* 0x0822AB00 */

/* Shift-JIS helpers (defined in text_render.c / text_bg.c). Matching: text.h declares
 * IsLineStartForbidden / IsLineEndForbidden as int(u16 sjis); the word-wrap code below tests
 * the full u32 character, so the u32 views stay here. */
extern u32 SjisToGlyphIndex(u16);
extern u32 IsLineStartForbidden(u32);
extern u32 IsLineEndForbidden(u32);

/* The canvas and glyph renderers (defined below; prototypes as in text.h). */
u8 TextWordLength(const u8 *);
void TextCanvasInit(u32 w, u32 h);
void TextCanvasInitEx(u32 w, u32 h, u16 flag, u32 val);
void TextPlotRow8(u8 bits, s32 x, s32 y, u32 color);
void TextPlotRow16(u16 bits, s32 x, s32 y, u32 color);
void TextDrawSjisGlyph(u16 sjis, s32 x, s32 y, u16 sc);
void TextDrawLatinGlyph(u8 ch, s32 x, s32 y, u16 sc);
void TextDrawGlyph(u16 ch, s32 x, s32 y, u16 sc);
void TextDrawSjisString(s32 x, s32 y, u16 sc, const u8 *str);
void TextDrawLatinString(s32 x, s32 y, u16 sc, const u8 *str);
void TextDrawString(s32 x, s32 y, u16 sc, const u8 *str);
void TextDrawSjisNumber(s32 x, s32 y, u16 sc, s32 value);
void TextDrawLatinNumber(s32 x, s32 y, u16 sc, s32 value);

/* SIOCNT in multi-player mode (+ SIOMLT_SEND), as the SDK's SioMultiCnt bitfield struct.
 * The bitfield form is load-bearing: it reproduces the ROM's folded bit tests in
 * LinkSioMain (wiki/functions/text-canvas-c.md). LINK_SIOCNT_BAK is gLinkSio.sioCnt
 * (link.h) seen in that layout. */
struct SioMultiCnt {
    u16 baudRate : 2;
    u16 si : 1;
    u16 sd : 1;
    u16 id : 2;
    u16 error : 1;
    u16 enable : 1;
    u16 unused : 4;
    u16 mode : 2;
    u16 ifEnable : 1;
    u16 unused2 : 1;
    u16 data;
};
#define LINK_SIOCNT_BAK (*(struct SioMultiCnt *)&gLinkSio.sioCnt)

/* Link main step (MultiSioMain-like): stage 0 snapshots SIOCNT; when SD is high and no transfer runs it
 * becomes the parent if SI is low and the IRQ state reached 0xC (Timer3 IRQ instead of serial IRQ),
 * then stage 1 runs LinkSioCheckRecvData each frame. Returns the receive flags | 0x80 when parent. */
u16 LinkSioMain(u8 *rx) {
    switch (gLinkSio.stage) {
    case 0:
        *(u32 *)&LINK_SIOCNT_BAK = *(vu32 *)&REG_SIOCNT;
        if (LINK_SIOCNT_BAK.sd == 1 && LINK_SIOCNT_BAK.enable == 0) {
            if (LINK_SIOCNT_BAK.si == 0 && gLinkSio.state == 0xC) {
                REG_IME = 0;
                REG_IE &= 0xFF7F;
                REG_IE |= 0x40;
                REG_IME = 1;
                ((volatile struct SioMultiCnt *)&REG_SIOCNT)->ifEnable = 0;
                REG_IF = 0xC0;
                *(vu32 *)&REG_TM3CNT_L = 0xB1FC;
                gLinkSio.master = LINK_SIO_PARENT;
                gLinkSio.transferEnabled = 1;
            }
            if (gLinkSio.txBuf[2] == 0)
                gLinkSio.txBuf[2] = LINKSIO_PKT_IDLE;
            gLinkSio.stage = 1;
        } else {
            break;
        }
        /* fallthrough */
    case 1:
        gLinkSio.stepResult = LinkSioCheckRecvData(rx);
        if ((gLinkSio.stepResult & 3) == 0 && gLinkSio.master == LINK_SIO_PARENT)
            LinkSioStartTransfer();
        break;
    }
    gLinkSio.stepResult |= (gLinkSio.master == LINK_SIO_PARENT) << 7;
    return gLinkSio.stepResult;
}

void LinkSioStartTransfer(void) {
    if (gLinkSio.stage != 0 && gLinkSio.transferEnabled != 0) {
        vu16 *sio = &REG_SIOCNT;
        u16 v = 0xFEFE;
        sio[1] = v;
        *sio = *sio | 0x80;
        REG_TM3CNT_H = 0xC0;
    }
}

/* Build the link send packet: txBuf[1] = ~(sum of txBuf[0..9]) after copying 12 halfwords from src to txBuf[2]. */
void LinkSioSetSendData(void *src) {
    u16 sum = 0;
    u32 i;
    u16 *p;
    gLinkSio.txBuf[1] = 0;
    CpuSet(src, &gLinkSio.txBuf[2], 0xC);
    for (i = 0, p = &gLinkSio.txBuf[0]; i <= 9; p++, i++)
        sum += *p;
    gLinkSio.txBuf[1] = ~sum;
}

/* Link receive step: rotates the RX double buffer (rxDone/rxWork, via tmpFrame), validates each of the 2 received packets in the last buffer by its 0xFFFF checksum, copies good ones out to rx + slot*16, clears them, and returns the per-slot status bits (gLinkSio.rxStatus). */
u16 LinkSioCheckRecvData(u8 *rx) {
    u32 zero;
    REG_IME = 0;
    gLinkSio.tmpFrame = (u16 *)gLinkSio.rxWork;
    gLinkSio.rxWork = gLinkSio.rxDone;
    gLinkSio.rxDone = (u16 (*)[12])gLinkSio.tmpFrame;
    gLinkSio.tmp0 = gLinkSio.dataReady;
    gLinkSio.dataReady = 0;
    REG_IME = 1;
    gLinkSio.rxStatus = 0;
    if (gLinkSio.tmp0 != 0) {
        for (gLinkSio.i = 0; gLinkSio.i < 2; gLinkSio.i++) {
            gLinkSio.tmpFrame = (u16 *)((u8 *)gLinkSio.rxWork + gLinkSio.i * 24);
            gLinkSio.tmp1 = 0;
            for (gLinkSio.j = 0; (u32)gLinkSio.j < 10; gLinkSio.j++)
                gLinkSio.tmp1 += gLinkSio.tmpFrame[gLinkSio.j];
            if (gLinkSio.tmp1 == 0xFFFF) {
                CpuSet(gLinkSio.tmpFrame + 2, rx + gLinkSio.i * 16, 8);
                gLinkSio.rxStatus |= 1 << gLinkSio.i;
            } else {
                gLinkSio.rxStatus |= 1 << (gLinkSio.i + 4);
            }
            zero = 0;
            CpuSet(&zero, gLinkSio.tmpFrame + 2, 0x05000004);
        }
    }
    gLinkSio.rxStatusAccum |= gLinkSio.rxStatus;
    return gLinkSio.rxStatus;
}

u32 CB_DebugExodiaScene(void) {
    struct Main *m = &gMain;
    u8 *step = &m->seqIndex1;
    switch (*step) {
    case 0:
        m->seqState1 = 0;
        gDuelScene[0xB] = 0;
        (*step)++;
        return 0;
    case 1:
        m->seqState1++;
        if (m->seqState1 <= 7) {
            m->vblankFlags &= 0xFFFE;
        } else {
            m->vblankFlags |= 1;
            m->seqState1 = 0;
            if (ExodiaScene_Run() != 0)
                (*step)++;
        }
        return 0;
    default:
        return 1;
    }
}

u32 CB_DebugDestinyBoardScene(void) {
    struct Main *m = &gMain;
    u8 *step = &m->seqIndex1;
    switch (*step) {
    case 0:
        gDuelScene[0xB] = 0;
        (*step)++;
        return 0;
    case 1:
        if (DestinyBoardScene_Run() != 0)
            (*step)++;
        return 0;
    default:
        return 1;
    }
}

u32 CB_DebugInitSaveData(void) {
    InitSaveData();
    return 1;
}

/* Debug menu "Get all card" */
u32 CB_DebugGetAllCards(void) {
    DebugGetAllCards();
    return 1;
}

/* Debug menu "Next Level": bump the counters of a block of cards depending on the current level. */
u32 CB_DebugNextLevel(void) {
    s32 i, j;
    if (IsCampaignLevel2Unlocked() == 0) {
        for (i = 0; i <= 4; ) {
            u32 id = i + 1;
            RecordDuelWin(id);
            RecordDuelWin(id);
            i = id;
        }
    } else if (IsCampaignLevel3Unlocked() == 0) {
        for (i = 0; i <= 4; i++) {
            u32 id = i + 6;
            RecordDuelWin(id);
            RecordDuelWin(id);
            RecordDuelWin(id);
        }
    } else if (IsCampaignLevel4Unlocked() == 0) {
        for (i = 0; i <= 4; i++) {
            u32 id = i + 0xB;
            RecordDuelWin(id);
            RecordDuelWin(id);
            RecordDuelWin(id);
            RecordDuelWin(id);
        }
    } else if (IsCampaignLevel5Unlocked() == 0) {
        for (i = 0; i <= 4; i++) {
            u32 id = i + 0x10;
            RecordDuelWin(id);
            RecordDuelWin(id);
            RecordDuelWin(id);
            RecordDuelWin(id);
            RecordDuelWin(id);
        }
    } else {
        i = 0;
        do {
            i++;
            for (j = 0x13; j >= 0; j--)
                RecordDuelWin(i);
        } while (i <= 0x13);
    }
    return 1;
}

u32 DebugMenu_Init(void) {
    gMain.vblankFlags = 3;
    REG_DISPCNT = 0x140;
    ResetVideo();
    REG_BG0CNT = 5;
    ResetBgScroll();
    SetBrightnessBlack();
    LoadSystemGfx();
    return 1;
}

void DebugMenu_DrawDate(void) {
    struct Date d;
    GetCurrentDate(&d);
    GetHolidayFlags(d.year, d.month, d.day);
    GetCalendarEvents(d.year, d.month, d.day);
    GetDayOfWeek(d.year, d.month, d.day);
    DrawBgString(0x33, 0x807, 0x3C0, gStrDebugDateTemplate);
    DrawBgDecimal(0x08070033, 0x000403C0, d.year, 1);
    DrawBgDecimal(0x08070038, 0x000203C5, d.month, 1);
    DrawBgDecimal(0x0807003B, 0x000203C8, d.day, 1);
}

/* Draw the date/time-dependent flag lines of the debug menu. */
void DebugMenu_DrawCalendarEvents(void) {
    s32 line = 2;
    u16 y = 0x3A0;
    struct Date d;
    u32 mask;
    u32 i;
    GetCurrentDate(&d);
    mask = GetCalendarEvents(d.year, d.month, d.day);
    if (gSaveData.days != 0 && (u16)__umodsi3(gSaveData.days, 0x3C) == 0) {
        DrawBgString(0x42, 0x802, y, gStrRareHunterComing);
        line = 3;
        y += 0x20;
    }
    for (i = 0; i <= 0x1C; i++) {
        if (gCalendarEventNames[i].mask & mask) {
            DrawBgString((((u32)line << 16) >> 11) + 2, 0x804, y, gCalendarEventNames[i].name);
            line++;
            y = y + 0x20;
        }
    }
}

void DebugMenu_DrawLanguage(void) {
    switch (gSaveData.language) {
    case LANGUAGE_ENGLISH:
        DrawBgString(0x21, 0x805, 0x3F0, gStrLangEnglish);
        break;
    case LANGUAGE_JAPANESE:
        DrawBgString(0x21, 0x805, 0x3F0, gStrLangJapanese);
        break;
    case LANGUAGE_GERMAN:
        DrawBgString(0x21, 0x805, 0x3F0, gStrLangGerman);
        break;
    case LANGUAGE_FRENCH:
        DrawBgString(0x21, 0x805, 0x3F0, gStrLangFrench);
        break;
    case LANGUAGE_ITALIAN:
        DrawBgString(0x21, 0x805, 0x3F0, gStrLangItalian);
        break;
    }
}

/* Debug menu step 0: draw the item list; step 1: enable BG0/BG3; then fade in. */
u16 DebugMenu_DrawAndFadeIn(void) {
    struct Main *m = &gMain;
    switch (m->seqIndex1) {
    case 0: {
        s32 i;
        struct DebugMenuItem *it, *first;
        u32 y;
        ClearBgMapBuffer0();
        DebugMenu_DrawLanguage();
        DrawBgDecimal(0x08070025, 0x000403F8, 0x83C, 1);
        SetTextArea(0, 0x27D);
        i = 0;
        first = gDebugMenuItems;
        if (first->callback != 0) {
            it = first;
            y = 0x20;
            do {
                u32 col = 2;
                s32 row = i * 2 + 4;
                if (row > 0x13) {
                    col = 0x10;
                    row -= 0x10;
                }
                col |= (u32)(row << 16) >> 11;
                DrawBgString(col, 0x807, y, it->name);
                it++;
                y += 0x10;
                i++;
            } while (it->callback != 0);
        }
        DebugMenu_DrawDate();
        DebugMenu_DrawCalendarEvents();
        gMain.seqIndex1++;
        return 0;
    }
    case 1:
        REG_DISPCNT |= 0x1100;
        m->seqIndex1++;
        return 0;
    default:
        return FadeFromBlack(4);
    }
}

/* Debug menu input: up/down move the cursor, A selects, B jumps to the Title item, left/right/L/R change gSaveData.day (and step the menu back to redraw the date). */
u32 DebugMenu_HandleInput(void) {
    struct Main *m;
    u8 *step;
    u32 col;
    s32 row;
    if (gMain.newKeys & DPAD_DOWN) {
        gMain.seqIndex1++;
        if (gDebugMenuItems[gMain.seqIndex1].callback == 0)
            gMain.seqIndex1 = 0;
    }
    if (gMain.newKeys & DPAD_UP) {
        if (gMain.seqIndex1 == 0) {
            if (gDebugMenuItems[gMain.seqIndex1].callback != 0) {
                do {
                    gMain.seqIndex1++;
                } while (gDebugMenuItems[gMain.seqIndex1].callback != 0);
            }
        }
        gMain.seqIndex1--;
    }
    col = 1;
    m = &gMain;
    step = &m->seqIndex1;
    row = *step * 2 + 4;
    if (row > 0x13) {
        col = 0xF;
        row -= 0x10;
    }
    AddSprite((col << 3) | (row << 19), 0, 2);
    if (m->newKeys & A_BUTTON)
        return 1;
    if (m->newKeys & B_BUTTON) {
        *step = DEBUG_ITEM_TITLE; /* enum DebugMenuItemId (debug.h) */
        return 1;
    }
    if (m->newKeys & DPAD_RIGHT) {
        gSaveData.days++;
        m->seqIndexCampaign--;
        ClearBgMapBuffer0();
        DebugMenu_DrawDate();
    }
    if (gMain.newKeys & DPAD_LEFT) {
        if (gSaveData.days != 0) {
            gSaveData.days--;
            gMain.seqIndexCampaign--;
            ClearBgMapBuffer0();
            DebugMenu_DrawDate();
        }
    }
    if (gMain.newKeys & R_BUTTON) {
        gSaveData.days += 0x1E;
        ClearBgMapBuffer0();
        DebugMenu_DrawDate();
    }
    if (gMain.newKeys & L_BUTTON) {
        if (gSaveData.days > 0x1E)
            gSaveData.days -= 0x1E;
        else
            gSaveData.days = 0;
        ClearBgMapBuffer0();
        DebugMenu_DrawDate();
    }
    return 0;
}

/* Debug menu final step: launch the selected item's callback. */
u32 DebugMenu_Launch(void) {
    struct Main *m;
    u8 *items;
    u32 off;
    REG_DISPCNT = 0;
    items = (u8 *)gDebugMenuItems;
    m = &gMain;
    off = m->seqIndex1 * 0x44;
    items += 0x40;
    SetMainCallback(*(void **)(off + (u32)items));
    m->seqIndexCampaign = 0;
    gDuelScene[0xB] = 0;
    return 0;
}

/* CB_DebugMenu (unused): runs the current step function; when it returns non-zero, advance to the next step. */
u32 CB_DebugMenu(void) {
    u16 (**tbl)(void) = gDebugMenuSteps;
    struct Main *m = &gMain;
    u8 *idx = &m->seqIndexCampaign;
    u16 (*cb)(void) = tbl[*idx];
    if (cb != 0) {
        u16 r = cb();
        if (r != 0) {
            u32 s = *idx;
            if (s <= 1) {
                m->seqState0 = 0;
                m->seqIndex1 = 0;
                m->seqState1 = 0;
                m->seqState2 = 0;
            }
            *idx = s + 1;
        }
        return 0;
    }
}

/* Maps ASCII 0x20..0x7E to its full-width Shift-JIS code, and anything else to 0. */
u16 AsciiToFullwidthSjis(u16 ch) {
    u32 x = ch - 0x20;
    if ((u16)x <= 0x5E)
        return gAsciiToSjisTable[x];
    return 0;
}

/* Length (in visible characters) of the next word: stops at NUL, space, newline or "\n"; "@0", "@2", "@3" colour codes are zero-width. */
u8 TextWordLength(const u8 *p) {
    s32 n = 0;
    while (*p != 0) {
        switch (*p) {
        case 0x5C:
            if (p[1] != 0x6E)
                break;
            goto done;
        case 0x40:
            switch (p[1]) {
            case 0x30:
            case 0x32:
            case 0x33:
                p++;
                n--;
            }
            break;
        case 0:
        case 0x20:
        case 0xA:
            goto done;
        }
        p++;
        n++;
    }
done:
    return n;
}

/* Text canvas init: width/height in tiles, clear the 64KiB EWRAM canvas. */
void TextCanvasInit(u32 w, u32 h) {
    gTextCanvas[0x10000] = w;
    gTextCanvas[0x10001] = h;
    gTextCanvas[0x10004] = 0;
    MemClear16(gTextCanvas, 0x10000);
}

void TextCanvasInitEx(u32 w, u32 h, u16 flag, u32 val) {
    u8 *p;
    gTextCanvas[0x10000] = w;
    gTextCanvas[0x10001] = h;
    p = &gTextCanvas[0x10004];
    *p = (val & 0x7F) | (flag << 7);
    MemClear16(gTextCanvas, 0x10000);
}

void TextPlotRow8(u8 bits, s32 x, s32 y, u32 color) {
    s32 xl = ((short)x) & 7; u8 yl = y & 7;
    s32 xt = x >> 3;
    s32 yt = y >> 3;
    s32 *new_var;
    s32 i;
    for (i = 0; i <= 7; i++) {
        if ((0x80 >> i) & bits)
            gTextCanvas[xl + yl * 8 + ((*(new_var = &xt) + gTextCanvas[0x10000] * yt) << 6)] = color;
        xl++;
        if (xl > 7) {
            xl = 0;
            xt++;
        }
    }
}

void TextPlotRow16(u16 bits, s32 x, s32 y, u32 color) {
    s32 xl = ((short)x) & 7; u8 yl = y & 7;
    s32 xt = x >> 3;
    s32 yt = y >> 3;
    s32 *new_var;
    s32 i;
    for (i = 0; i <= 15; i++) {
        if ((0x8000 >> i) & bits)
            gTextCanvas[xl + yl * 8 + ((*(new_var = &xt) + gTextCanvas[0x10000] * yt) << 6)] = color;
        xl++;
        if (xl > 7) {
            xl = 0;
            xt++;
        }
    }
}

/* Draw a Shift-JIS glyph (size 8/10/12 = high byte of sc, colour = low byte) at (x, y). */
void TextDrawSjisGlyph(u16 sjis, s32 x, s32 y, u16 sc) {
    u8 size = sc >> 8;
    u8 color = sc;
    const u16 *p;
    s32 i;
    u32 n;
    switch (size) {
    case 8:
        p = (const u16 *)(SjisToGlyphIndex(sjis) * 8 + (u32)gFontKanji8x8);
        for (i = 3; i >= 0; i--) {
            u32 v = *p++;
            v <<= 17;
            TextPlotRow8((v << 8) >> 24, x, y++, color);
            TextPlotRow8(v >> 24, x, y++, color);
        }
        break;
    case 10:
        p = (const u16 *)(SjisToGlyphIndex(sjis) * 20 + (u32)gFontKanji10x10);
        goto rows;
    case 12:
        p = (const u16 *)(SjisToGlyphIndex(sjis) * 24 + (u32)gFontKanji12x12);
    rows:
        n = size;
        if (n != 0) {
            i = n;
            do {
                u16 w = *p++;
                u16 sw = (w >> 8) | ((u8)w << 8);
                TextPlotRow16((u16)(sw << 1), x, y++, color);
            } while (--i != 0);
        }
        break;
    }
}

void TextDrawLatinGlyph(u8 ch, s32 x, s32 y, u16 sc) {
    u8 size = sc >> 8;
    u8 color = sc;
    const u16 *p;
    u32 off;
    u32 base;
    s32 i;
    u32 n;
    switch (size) {
    case 8:
        off = ch * 8;
        base = (u32)gFontLatin8x8;
        p = (const u16 *)(off + base);
        for (i = 3; i >= 0; i--) {
            u32 v = *p++;
            v <<= 17;
            TextPlotRow8((v << 8) >> 24, x, y++, color);
            TextPlotRow8(v >> 24, x, y++, color);
        }
        break;
    case 10:
        off = ch * 10;
        base = (u32)gFontLatin8x10;
        goto rows;
    case 12:
        off = ch * 12;
        base = (u32)gFontLatin8x12;
        goto rows;
    case 16:
        off = ch * 16;
        base = (u32)gFontLatin8x16;
    rows:
        p = (const u16 *)(off + base);
        n = size >> 1;
        if (n != 0) {
            u32 cnt = n;
            do {
                u32 v = *p++;
                v <<= 17;
                TextPlotRow8((v << 8) >> 24, x, y++, color);
                TextPlotRow8(v >> 24, x, y++, color);
            } while (--cnt != 0);
        }
        break;
    }
}

/* Draw a glyph: Shift-JIS renderer when gSaveData.sjisText is set, otherwise the Latin one. */
void TextDrawGlyph(u16 ch, s32 x, s32 y, u16 sc) {
    if (gSaveData.sjisText != 0)
        TextDrawSjisGlyph(ch, x, y, sc);
    else
        TextDrawLatinGlyph(ch, x, y, sc);
}

/* Draw a Shift-JIS string at (x, y) with word wrap; tracks the max x/y extents in the canvas header (+2/+3). */
void TextDrawSjisString(s32 x0, s32 y0, u16 sc, const u8 *str) {
    s32 size = sc >> 8;
    s32 x = x0;
    s32 y = y0;
    s32 startX = x;
    gTextCanvas[0x10002] = 0;
    gTextCanvas[0x10003] = 0;
    if (*str != 0) {
        do {
            u32 c = (str[0] << 8) | str[1];
            if (gTextCanvas[0x10004] & 0x80) {
                if (x + size * 3 > gTextCanvas[0x10000] * 8) {
                    if (IsLineStartForbidden(c) == 0)
                        goto wrap;
                }
                if (x + size * 4 > gTextCanvas[0x10000] * 8) {
                    if (IsLineEndForbidden(c) != 0) {
                    wrap:
                        x = startX;
                        y += size + (((u32)gTextCanvas[0x10004] << 25) >> 25);
                    }
                }
            }
            TextDrawSjisGlyph(c, x, y, sc);
            if (gTextCanvas[0x10002] < x + size)
                gTextCanvas[0x10002] = x + size;
            if (gTextCanvas[0x10003] < y + size)
                gTextCanvas[0x10003] = y + size;
            x += size;
            str += 2;
        } while (*str != 0);
    }
}

/* Draw a Latin string at (x, y) with word wrap; tracks the max x/y extents in the canvas header (+2/+3). */
void TextDrawLatinString(s32 x0, s32 y0, u16 sc, const u8 *str) {
    s32 size = sc >> 8;
    s32 x = x0;
    s32 y = y0;
    s32 startX = x;
    gTextCanvas[0x10002] = 0;
    gTextCanvas[0x10003] = 0;
    if (*str != 0) {
        do {
            if (gTextCanvas[0x10004] & 0x80) {
                s32 w = TextWordLength(str);
                if (x + ((w * size) >> 1) > (gTextCanvas[0x10000] - 2) * 8) {
                    x = startX;
                    y += size + (((u32)gTextCanvas[0x10004] << 25) >> 25);
                }
            }
            TextDrawLatinGlyph(*str, x, y, sc);
            if (gTextCanvas[0x10002] < x + (s32)((u32)size >> 1))
                gTextCanvas[0x10002] = x + (s32)((u32)size >> 1);
            if (gTextCanvas[0x10003] < y + size)
                gTextCanvas[0x10003] = y + size;
            x += (u32)size >> 1;
            if (size == 0x10)
                x++;
            str++;
        } while (*str != 0);
    }
}

/* Draw a string: Shift-JIS path when gSaveData.sjisText is set, otherwise Latin. */
void TextDrawString(s32 x, s32 y, u16 sc, const u8 *str) {
    if (gSaveData.sjisText != 0)
        TextDrawSjisString(x, y, sc, str);
    else
        TextDrawLatinString(x, y, sc, str);
}

/* Draw a decimal number right-aligned at x (least significant digit first, moving left) using full-width Shift-JIS digits. */
void TextDrawSjisNumber(s32 x, s32 y, u16 sc, s32 value) {
    s32 size = sc >> 8;
    do {
        TextDrawSjisGlyph(value % 10 + 0x824F, x, y, sc);
        x -= size;
        value /= 10;
    } while (value != 0);
}

/* Same with Latin digits (advance is half the font size). */
void TextDrawLatinNumber(s32 x, s32 y, u16 sc, s32 value) {
    s32 half = sc >> 9;
    do {
        TextDrawLatinGlyph(value % 10 + 0x30, x, y, sc);
        x -= half;
        value /= 10;
    } while (value != 0);
}

