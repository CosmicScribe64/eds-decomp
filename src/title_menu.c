#include "global.h"
#include "main.h"

#include "gba.h"

/* gMain (0x03000040) comes from the shared include/main.h. */
#define gMain gMain

/* gTitleState at 0x0201527C */
struct TitleState {
    u16 scroll;                         /* 0x0: BG3 scroll, decremented each frame */
    u8 savePresent : 1;                 /* 0x2 bit 0 */
    u8 continueSelected : 1;            /* 0x2 bit 1 */
};

/* The same flags seen as u32 bitfields: Title_ConfirmDeleteSave compares them with one word load. */
struct TitleStateW {
    u16 scroll;
    u32 savePresent : 1;
    u32 continueSelected : 1;
};

extern struct TitleState gTitleState;
#define gTitleState gTitleState

/* 0x02013D90: follows the save image */
struct Unk02013D90 {
    u8 flags;               /* +0x00: bit0 selects the right-hand layout */
    u8 filler1;
    u16 card;               /* +0x02: card shown on the detail screen */
    u8 filler4[0x28];
    s32 unk2C;              /* +0x2C: number shown by CardDetail_DrawAtk */
    s32 unk30;              /* +0x30: number shown by CardDetail_DrawDef */
    s32 unk34;              /* +0x34: scroll position */
    s32 unk38;              /* +0x38: scroll target */
    s32 unk3C;              /* +0x3C: written by CardDetail_RenderText from the text height */
};

extern struct Unk02013D90 gCardDetail;

/* gTextWork at 0x02000000: text renderer work area (see wiki ram-map) */
struct TextWork {
    u8 glyphs[0x10000];                 /* 0x00000: rendered text pixels */
    u8 width;                           /* 0x10000 */
    u8 height;                          /* 0x10001 */
    u8 unk10002;                        /* 0x10002 */
    u8 penY;                            /* 0x10003: pixel row reached by the last DrawText (hypothesis) */
    u8 unk10004;                        /* 0x10004: bit 7 = wrap flag, bits 0-6 = spacing */
};

extern struct TextWork gTextCanvas;
#define gTextWork gTextCanvas

extern u16 (*const gTitleSteps[])(void);

void AddSprite(u32 yx, u16 shapeSize, u16 attr2); /* AddSprite */
void CardDetail_DrawNumber(int x, int y, int value);
void StartDialogue(u16 eventId);   /* StartDialogue */
u32 CB_Bustup(void);           /* Bustup text-box runner */
void PlayBGM(u16 song);      /* PlayBGM */
u32 StarterDeckSelect_Run(void);           /* new-game setup */
s32 StrLen(const u8 *str);  /* StrLen */
void TextCanvasInitEx(u8 width, u8 height, u16 wrap, u8 spacing); /* text window setup */
void TextDrawString(int x, int y, u16 attr, const u8 *str);      /* DrawText */
void CardDetail_RenderText(u16 size, u32 pos, const u8 *str, u16 colors, int lineHeight, u16 wrap);
void TextCanvasToTiles(void *dest, u16 value); /* fill 8bpp tiles (hypothesis) */
void CopyDoubleWords(void *dst, const void *src, u32 size); /* CopyDoubleWords */
void MemCopy16(void *dst, const void *src, u32 size); /* MemCopy16 */
u16 FadeFromBlack(u16 step);
u16 FadeToBlack(u16 step);
void PlaySE(u16 id);
void SetBrightnessBlack(void);
void ResetVideo(void);
void ResetBgScroll(void);
void Title_InitBgCnt(void);
extern const u8 gDeletePromptObjPal[];
extern const u8 gDeletePromptObjGfx[];
extern const u8 gDeletePromptBgPal[];
extern const u8 gDeletePromptBgBitmap[];

/* Draws a full-screen OBJ picture as a 5x4 grid of 64x32 sprites covering the
 * 240x160 screen. Tiles use 2D mapping (32 tiles per row), starting at tile
 * row 16. */
void Title_DrawDeletePrompt(void)
{
    int row, col;

    for (row = 0; row <= 4; row++)
        for (col = 0; col <= 3; col++)
            AddSprite((col << 6) | (row << 21), 0x40C0, (row * 4 + 16) * 32 + col * 8);
}

/*
 * Title step 5: when "Continue" was picked without a save (or "New Game" with one) this
 * shows a full-screen notice picture; B goes back to the title menu, START continues.
 */
u16 Title_ConfirmDeleteSave(void)
{
    switch (gMain.seqState0) {
    case 0:
        if ((u8)((struct TitleStateW *)&gTitleState)->savePresent
            == (u8)((struct TitleStateW *)&gTitleState)->continueSelected)
            return 1;
        REG_DISPCNT = 4;
        gMain.vblankFlags = 1;
        gMain.seqState0++;
        break;
    case 1:
        CopyDoubleWords((void *)0x05000200, gDeletePromptObjPal, 0x20);
        CopyDoubleWords((void *)0x06014000, gDeletePromptObjGfx, 0x4000);
        MemCopy16((void *)0x05000000, gDeletePromptBgPal, 0x200);
        MemCopy16((void *)0x06000000, gDeletePromptBgBitmap, 0x9600);
        MemCopy16((void *)0x0600A000, gDeletePromptBgBitmap, 0x9600);
        gMain.seqState0++;
        break;
    case 2:
        REG_DISPCNT = 0x1F04;
        Title_DrawDeletePrompt();
        if (FadeFromBlack(2))
            gMain.seqState0++;
        break;
    case 3:
        Title_DrawDeletePrompt();
        if (gMain.newKeys & B_BUTTON) {
            PlaySE(2);
            gMain.seqState0 = 10;
        } else if (gMain.newKeys & START_BUTTON) {
            PlaySE(1);
            gMain.seqState0++;
        }
        break;
    case 4:
        Title_DrawDeletePrompt();
        if (FadeToBlack(2))
            return 1;
        break;
    case 10:
        Title_DrawDeletePrompt();
        if (FadeToBlack(2))
            gMain.seqState0++;
        return 0;
    case 11:
        SetBrightnessBlack();
        ResetVideo();
        ResetBgScroll();
        Title_InitBgCnt();
        gMain.vblankFlags = 3;
        gMain.seqState0++;
        return 0;
    case 12:
        gMain.seqIndexTop = 1;
        gMain.seqState0 = 0;
        break;
    }
    return 0;
}
/*
 * Title step 6 (Title_StartGame): Continue returns at once. New Game plays
 * Tea's intro dialogue (event 100), runs new-game setup, then event 101.
 */
u16 Title_StartGame(void)
{
    if (gTitleState.continueSelected)
        return 1;

    switch (gMain.seqState0) {
    case 0:
        StartDialogue(100);
        PlayBGM(1);
        gMain.seqState0++;
    case 1:
        if (CB_Bustup()) {
            gMain.seqState0++;
            gMain.seqIndex1 = 0;
        }
        return 0;
    case 2:
        if (StarterDeckSelect_Run()) {
            StartDialogue(101);
            gMain.seqState0++;
        }
        return 0;
    default:
        return CB_Bustup();
    }
}

/* CB_Title: the title-screen step runner (table 0x081988B0, index gMain.seqIndexTop). */
u16 CB_Title(void)
{
    u16 (*step)(void) = gTitleSteps[gMain.seqIndexTop];

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

/* HBlank handler: per-scanline BG1/BG2 horizontal scroll from a 16-entry table. */
void CardDetail_HBlank(void)
{
    gMain.lastVcount = REG_VCOUNT;
    if (gMain.lastVcount < 160) {
        REG_BG1HOFS = gMain.hblankScroll[gMain.lastVcount & 15];
        REG_BG2HOFS = gMain.hblankScroll[gMain.lastVcount & 15];
    }
}

/*
 * Renders a string into the text work buffer, with a drop shadow. Text longer than
 * 399 chars, or text that ends below row 0xBF, is re-rendered with tighter spacing;
 * the last fallbacks drop the shadow and use a fixed line height of 8.
 */
/*
 * FAKEMATCH: the original calls TextCanvasInitEx as if it took full-width int arguments
 * (its real body never truncates width/height). Calling it through the shared u8
 * prototype makes agbcc copy width/height into fresh pseudos at the second call,
 * which moves them to other registers.
 */
typedef void (*TextWinFunc_08005860)(u32 width, u32 height, u16 wrap, u32 spacing);
#define SetupWin_08005860 ((TextWinFunc_08005860)TextCanvasInitEx)

void CardDetail_RenderText(u16 size, u32 pos, const u8 *str, u16 colors, int lineHeight, u16 wrap)
{
    u32 color = (u8)colors;
    u32 shadowColor = (u8)(colors >> 8);
    u32 width = (u8)size;
    u32 height = (u8)(size >> 8);
    u32 x = (u16)pos;
    u32 y = (u16)(pos >> 16);
    u32 attr;

    /* FAKEMATCH: an extra (code-free) use of color raises its global-alloc priority
     * above width/height, so color gets r9, height sl and width is spilled to the stack. */
    asm("" : : "r"(color));
    if (StrLen(str) < 400) {
        SetupWin_08005860(width, height, wrap, 2);
        TextDrawString(x + 1, y + 1, shadowColor | ((u8)lineHeight << 8), str);
        TextDrawString(x, y, color | ((u8)lineHeight << 8), str);
        gCardDetail.unk3C = (gTextWork.penY - lineHeight) & (gTextWork.penY - lineHeight + 1);
        if (gTextWork.penY < 0xC0)
            return;
    }

    SetupWin_08005860(width, height, wrap, 1);
    TextDrawString(x + 1, y + 1, shadowColor | ((u8)lineHeight << 8), str);
    TextDrawString(x, y, color | ((u8)lineHeight << 8), str);
    gCardDetail.unk3C = (gTextWork.penY - lineHeight) & (gTextWork.penY - lineHeight + 1);
    if (gTextWork.penY < 0xC0)
        return;

    /* No shadow, fixed line height 8. attr is a u32 so the 0x800 stays a full-width
     * register and the orr ties to it, as in the original. */
    SetupWin_08005860(width, height, wrap, 1);
    attr = color | (8 << 8);
    TextDrawString(x, y, attr, str);
    gCardDetail.unk3C = (gTextWork.penY - 8) & (gTextWork.penY - 8 + 1);
    if (gTextWork.penY < 0xC0)
        return;

    SetupWin_08005860(width, height, wrap, 0);
    TextDrawString(x, y, attr, str);
    gCardDetail.unk3C = (gTextWork.penY - 8) & (gTextWork.penY - 8 + 1);
}
/*
 * Renders `str` into a text window of size.w x size.h tiles (via CardDetail_RenderText), clears the
 * tiles at `tile`, and maps them row by row into gMain.bgMapBuffer[bg] at (pos.x, pos.y).
 */
void CardDetail_DrawTextBox(u32 bg, u16 pos, u16 size, u16 tile, u16 colors, u16 lineHeight,
                  u32 textPos, const u8 *str, u16 wrap, u16 fill)
{
    u32 x = (u8)pos;
    u32 y = pos >> 8;
    s32 w = (u8)size;
    s32 h = size >> 8;
    s32 row, col;

    CardDetail_RenderText(size, textPos, str, colors, lineHeight, wrap);
    TextCanvasToTiles((void *)(0x06004000 + tile * 32), fill);
    for (row = 0; row < h; row++)
        for (col = 0; col < w; col++)
            gMain.bgMapBuffer[bg][(row + y) * 32 + x + col] = tile++;
}
void ClearBgMapBuffer0(void);
void StrCopy(u8 *dst, const u8 *src);   /* StrCpy */
void StrCat(u8 *dst, const u8 *src);   /* StrCat */
void TextCanvasInit(u8 a, u8 b);
void TextDrawNumber(int x, int y, u16 attr, int value); /* DrawNumber */
extern const u8 gTrapIconPal[];
extern const u8 gTrapIconGfx[];
extern const u8 gMagicIconPal[];
extern const u8 gMagicIconGfx[];
extern const u8 gDivineIconPal[];
extern const u8 gDivineIconGfx[];
extern const u8 gUnk_08637394[];
extern const u8 gSpellTrapSubtypeIconPal[];
extern const u8 *const gCardIconPals[];
extern const u8 *const gCardIconGfx[];
extern const u8 *const gCardTypeNames[];
extern const u8 *const gSpellTrapSubtypeSuffixes[];
extern const u8 gStrOpenBracket[];
extern const u8 gStrCloseBracket[];
extern const u8 gStrAtk[];
extern const u8 gStrDef[];
extern const u8 gStrNotACard[];
extern const u8 gStrEffectSuffix[];
extern const u8 gStrFusionEffectSuffix[];
extern const u8 gStrFusionSuffix[];
extern const u8 gStrRitualEffectSuffix[];
extern const u8 gStrRitualSuffix[];
extern const u8 gStrNotPlayable[];

/* Card stats word (0x08621DE0) and card number (0x08622AB4) by card ID. The constant-address
 * forms make GCC reload the table base at each use, as the ROM does. */
#define STATS_5A70(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define TYPE_5A70(id) ((STATS_5A70(id) & 0x1F00000) >> 20)
#define NUMBER_5A70(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

/* The switches on the card type go through this inline: inlined RTL keeps the stats address as
 * (plus reg const), the same form the Icon/Level inlines use, so CSE shares the address register. */
static inline int Type5A70(u16 id)
{
    return TYPE_5A70(id);
}

static inline int Icon5A70(u16 id)
{
    switch ((int)TYPE_5A70(id)) {
    case 21:
    case 22:
        return (STATS_5A70(id) & 0xE0000) >> 17;
    default:
        return 0;
    }
}

static inline int Level5A70(u16 id)
{
    switch ((int)TYPE_5A70(id)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    default:
        return (STATS_5A70(id) & 0x1E000000) >> 25;
    }
}

/* Shown ATK/DEF (0 for spell/trap/ritual types, 4000 for type 24). The u16 return type gives the
 * inline its own result register (r0) and the copy into the argument register that the ROM has. */
static inline u16 Atk5A70(u16 id)
{
    switch ((int)TYPE_5A70(id)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    default:
        return ((STATS_5A70(id) << 14) >> 23) * 10;
    }
}

static inline u16 Def5A70(u16 id)
{
    switch ((int)TYPE_5A70(id)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    default:
        return (STATS_5A70(id) & 0x1FF) * 10;
    }
}

/* Same as GetCardSubtype in card_detail. */
static inline int Subtype5A70(u16 id)
{
    switch (NUMBER_5A70(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((int)TYPE_5A70(id)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    default:
        return (STATS_5A70(id) & 0xC0000) >> 18;
    }
}

/* BG map entry (x, y) of gMain.bgMapBuffer[bg]. Written directly (not through an inline taking
 * bg), so the constant bg folds into the gMain offset (0x0300245C for bg 4). */
#define TILE_5A70(bg, x, y) (gMain.bgMapBuffer[bg][(u16)(x) + (u16)(y) * 32])

static inline u32 Attr5A70(u16 id)
{
    return STATS_5A70(id) >> 29;
}

/*
 * Card detail screen text and icons: the card name (centred when gCardDetail.flags bit 0 is
 * set), the type/attribute palette and OBJ tiles, the spell/trap icon; then, unless flag bit 0
 * is set, either the token layout (attribute icon, level stars, type line, ATK/DEF numbers)
 * or the type/subtype line and the card description. Resets the description scroll.
 */
void CardDetail_DrawInfo(u16 id)
{
    const u8 *name;
    int len, n;
    int x, y;
    u16 lh;
    int i, j;
    u16 tile;
    u8 buf[0x80];

    name = (const u8 *)0x0822C720 + id * 64;
    len = StrLen(name);
    x = 4;
    y = 2;
    lh = 12;
    if (len > 36) {
        y = 4;
        lh = 10;
    }
    if (gCardDetail.flags & 1)
        x = 120 - ((len * lh) >> 1);

    switch (Type5A70(id)) {
    case 21:
        CopyDoubleWords((void *)0x05000220, gTrapIconPal, 0x20);
        CopyDoubleWords((void *)0x06010400, gTrapIconGfx, 0x80);
        if (Icon5A70(id)) {
            /* The base is loaded before the inline runs, so it is assigned first. */
            const u8 *p = gUnk_08637394;
            p += (Icon5A70(id) - 1) * 32;
            CopyDoubleWords((void *)0x05000240, gSpellTrapSubtypeIconPal, 0x20);
            CopyDoubleWords((void *)0x06010480, p, 0x20);
        }
        break;
    case 22:
        CopyDoubleWords((void *)0x05000220, gMagicIconPal, 0x20);
        CopyDoubleWords((void *)0x06010400, gMagicIconGfx, 0x80);
        if (Icon5A70(id)) {
            const u8 *p = gUnk_08637394;
            p += (Icon5A70(id) - 1) * 32;
            CopyDoubleWords((void *)0x05000240, gSpellTrapSubtypeIconPal, 0x20);
            CopyDoubleWords((void *)0x06010480, p, 0x20);
        }
        break;
    case 23:
        break;
    case 24:
        CopyDoubleWords((void *)0x05000220, gDivineIconPal, 0x20);
        CopyDoubleWords((void *)0x06010400, gDivineIconGfx, 0x80);
        break;
    default:
    {
        u32 attr = STATS_5A70(id) >> 29;

        /* Nested ifs: a single && chain folds into one (attr - 1) <= 5 range test. */
        if (attr != 0) {
            if (attr <= 6 && TYPE_5A70(id) <= 20) {
                CopyDoubleWords((void *)0x05000220, gCardIconPals[attr], 0x20);
                CopyDoubleWords((void *)0x06010400, gCardIconGfx[attr], 0x80);
            }
        }
        break;
    }
    }

    ClearBgMapBuffer0();
    CardDetail_DrawTextBox(0, 0, 0x220, 0x1E4, 0x807, lh, (u16)x | (y << 16), name, 1, 1);
    if (gCardDetail.flags & 1)
        return;

    if ((u16)(NUMBER_5A70(id) - 0x780) <= 0x4F) {
        CopyDoubleWords((void *)0x05000020, gCardIconPals[Attr5A70(id)], 0x20);
        CopyDoubleWords((void *)0x06004400, gCardIconGfx[Attr5A70(id)], 0x80);
        TILE_5A70(0, 13, 2) = 0x1020;
        TILE_5A70(0, 14, 2) = 0x1021;
        TILE_5A70(0, 13, 3) = 0x1022;
        TILE_5A70(0, 14, 3) = 0x1023;
        for (i = 0; i < Level5A70(id); i++)
            TILE_5A70(0, i + 15, 3) = 3;
        StrCopy(buf, gStrOpenBracket);
        StrCat(buf, gCardTypeNames[Type5A70(id)]);
        StrCat(buf, gStrCloseBracket);
        TextDrawString(4, 0x14, 0xA08, buf);
        TextDrawString(3, 0x13, 0xA07, buf);
        TextCanvasInit(0x11, 0x10);
        n = StrLen(gStrAtk);
        TextDrawString(4, 0x20, 0xA0D, gStrAtk);
        TextDrawString(3, 0x1F, 0xA05, gStrAtk);
        TextDrawNumber((n + 4) * 5 + 4, 0x20, 0xA08, Atk5A70(id));
        TextDrawNumber((n + 4) * 5 + 3, 0x1F, 0xA07, Atk5A70(id));
        n = StrLen(gStrDef);
        TextDrawString(4, 0x2C, 0xA0B, gStrDef);
        TextDrawString(3, 0x2B, 0xA03, gStrDef);
        TextDrawNumber((n + 4) * 5 + 4, 0x2C, 0xA08, Def5A70(id));
        TextDrawNumber((n + 4) * 5 + 3, 0x2B, 0xA07, Def5A70(id));
        TextCanvasToTiles((void *)0x06008900, 9);
        tile = 0x248;
        for (i = 0; i <= 15; i++)
            for (j = 0; j <= 16; j++)
                TILE_5A70(4, j + 13, i + 2) = tile++;
        CardDetail_DrawTextBox(4, 0x120D, 0x212, 0x224, 0x806, 10, 0x30003, gStrNotACard, 0, 11);
        gMain.bgVofs[3] = 0;
        gCardDetail.unk38 = 0;
        gCardDetail.unk34 = 0;
    } else {
        StrCopy(buf, gStrOpenBracket);
        switch (Type5A70(id)) {
        case 21:
        case 22:
            StrCat(buf, gCardTypeNames[Type5A70(id)]);
            if (Icon5A70(id))
                StrCat(buf, gSpellTrapSubtypeSuffixes[Icon5A70(id)]);
            break;
        case 23:
        case 24:
            StrCat(buf, gCardTypeNames[Type5A70(id)]);
            break;
        default:
            StrCat(buf, gCardTypeNames[Type5A70(id)]);
            switch (Subtype5A70(id)) {
            case 0:
                break;
            case 1:
                StrCat(buf, gStrEffectSuffix);
                break;
            case 2:
                switch (NUMBER_5A70(id)) {
                case 0x32C:
                case 0x4D9:
                case 0x536:
                case 0x5F6:
                    StrCat(buf, gStrFusionEffectSuffix);
                    break;
                default:
                    StrCat(buf, gStrFusionSuffix);
                    break;
                }
                break;
            case 3:
                if (NUMBER_5A70(id) == 0x2DA)
                    StrCat(buf, gStrRitualEffectSuffix);
                else
                    StrCat(buf, gStrRitualSuffix);
                break;
            }
            break;
        }
        StrCat(buf, gStrCloseBracket);
        len = StrLen(buf);
        lh = 10;
        if (len > 22)
            lh = 8;
        /* The (u16) keeps CSE from sharing the 9 with the last argument. */
        CardDetail_DrawTextBox(0, 0x20D, 0x212, 0x224, 0x807, lh, ((u16)(9 - (lh >> 1)) << 16) | 4, buf, 1, 9);
        CardDetail_DrawTextBox(4, 0x40D, 0x1812, 0x248, 0x907, 10, 0x20004, (const u8 *)0x082461A0 + id * 480, 1, 0);
        if (TYPE_5A70(id) > 22)
            CardDetail_DrawTextBox(4, 0x120D, 0x211, 0x369, 0x806, 10, 0x30003, gStrNotPlayable, 0, 11);
        /* Duplicated in both branches: each copy reuses the 0 of the last call's arguments and
         * cross-jumping merges the stores. */
        gMain.bgVofs[3] = 0;
        gCardDetail.unk38 = 0;
        gCardDetail.unk34 = 0;
    }
}

/* Draws a decimal number with digit sprites (tile 0x3030 + digit), right to left from x + 0x14. */
void CardDetail_DrawNumber(int x, int y, int value)
{
    x += 0x14;
    if (value == 0) {
        AddSprite((y << 16) | x, 0, 0x3030);
    } else {
        do {
            AddSprite(x | (y << 16), 0, value % 10 + 0x3030);
            value /= 10;
            x -= 4;
        } while (value != 0);
    }
}

void CardDetail_DrawAtk(void)
{
    int x;

    if (gCardDetail.flags & 1)
        x = 0x48;
    else
        x = 0;
    AddSprite((x + 0x3E) | (0x86 << 16), 0x4000, 0x303A);
    CardDetail_DrawNumber(x + 0x44, 0x86, gCardDetail.unk2C);
}

void CardDetail_DrawDef(void)
{
    int x;

    if (gCardDetail.flags & 1)
        x = 0x48;
    else
        x = 0;
    AddSprite((x + 0x3E) | (0x8E << 16), 0x4000, 0x303C);
    CardDetail_DrawNumber(x + 0x44, 0x8E, gCardDetail.unk30);
}

static inline int Level64AC(u16 id)
{
    switch ((int)((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    default:
        return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
    }
}
/* Spell/trap subtype icon index (bits 17-19 of the stats word for types 21 and 22). */
static inline int Icon64AC(u16 id)
{
    switch ((int)((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20)) {
    case 21:
    case 22:
        return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0xE0000) >> 17;
    default:
        return 0;
    }
}
/* Card level as a u8: the narrow return type gives the inline its own result register (r0) and the
 * copy into `level` that the ROM keeps. */
static inline u8 CardLevel64AC(u16 id)
{
    switch ((int)((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    default:
        return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
    }
}
/* Card detail screen: level stars (squeezed together above 9), the spell/trap icon, and the fixed
 * ATK/DEF digits of the three Divine cards (hypothesis from the sprite tiles). Case 24 is written
 * first: that body order reproduces the ROM's block layout. The type switch reads the card through
 * an int local, so its zero-extending load is not shared with the u16 load of the card-number check. */
void CardDetail_DrawSprites(void)
{
    int x;
    int level;
    int i;
    u32 attr;
    int id;

    if (gCardDetail.flags & 1)
        x = 0x48;
    else
        x = 0;
    level = CardLevel64AC(gCardDetail.card);
    if ((u16)(((const u16 *)0x08622AB4)[gCardDetail.card & 0x7FF] - 0x780) <= 0x4F)
        return;
    id = gCardDetail.card;
    switch ((int)((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20)) {
    case 24:
        for (i = 0; i <= 9; i++)
            AddSprite((x + 0x54 - i * 8) | 0x260000, 0, 2);
        AddSprite((x + 0x4C) | 0x160000, 0x40, 0x1020);
        switch (((const u16 *)0x08622AB4)[gCardDetail.card & 0x7FF]) {
        case 0x776:
            AddSprite(0x86003F, 0x4000, 0x303A);
            AddSprite(0x86004B, 0, 0x3034);
            AddSprite(0x86004F, 0, 0x3030);
            AddSprite(0x860053, 0, 0x3030);
            AddSprite(0x860057, 0, 0x3030);
            AddSprite(0x8E003F, 0x4000, 0x303C);
            AddSprite(0x8E004B, 0, 0x3034);
            AddSprite(0x8E004F, 0, 0x3030);
            AddSprite(0x8E0053, 0, 0x3030);
            AddSprite(0x8E0057, 0, 0x3030);
            break;
        case 0x777:
            AddSprite(0x86003F, 0x4000, 0x303A);
            AddSprite(0x86004B, 0, 0x303F);
            AddSprite(0x86004F, 0, 0x3030);
            AddSprite(0x860053, 0, 0x3030);
            AddSprite(0x860057, 0, 0x3030);
            AddSprite(0x8E003F, 0x4000, 0x303C);
            AddSprite(0x8E004B, 0, 0x303F);
            AddSprite(0x8E004F, 0, 0x3030);
            AddSprite(0x8E0053, 0, 0x3030);
            AddSprite(0x8E0057, 0, 0x3030);
            break;
        case 0x778:
            AddSprite(0x86003F, 0x4000, 0x303A);
            AddSprite(0x86004B, 0, 0x303E);
            AddSprite(0x86004F, 0, 0x303E);
            AddSprite(0x860053, 0, 0x303E);
            AddSprite(0x860057, 0, 0x303E);
            AddSprite(0x8E003F, 0x4000, 0x303C);
            AddSprite(0x8E004B, 0, 0x303E);
            AddSprite(0x8E004F, 0, 0x303E);
            AddSprite(0x8E0053, 0, 0x303E);
            AddSprite(0x8E0057, 0, 0x303E);
            break;
        }
        break;
    case 21:
    case 22:
        AddSprite((x + 0x4C) | 0x160000, 0x40, 0x1020);
        if (Icon64AC(gCardDetail.card) != 0)
            AddSprite((x + 0x50) | 0x240000, 0, 0x2024);
        break;
    case 23:
        break;
    default:
        for (i = 0; i < level; i++) {
            if (level <= 9)
                AddSprite((x + 0x54 - i * 8) | 0x260000, 0, 2);
            else {
                int t = 0x54 - 0x4E * i / level;
                AddSprite((t + x) | 0x260000, 0, 2);
            }
        }
        attr = ((const u32 *)0x08621DE0)[gCardDetail.card & 0x7FF] >> 29;
        if (attr != 0)
            if (attr <= 6)
                AddSprite((x + 0x4C) | 0x160000, 0x40, 0x1020);
        CardDetail_DrawAtk();
        CardDetail_DrawDef();
        break;
    }
}
