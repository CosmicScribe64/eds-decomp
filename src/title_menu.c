/*
 * title_menu (0x08005500-0x08006877): the last title-screen steps and the Card Detail viewer's drawing.
 *
 * Title: CB_Title runs gTitleSteps (enum TitleStep; steps 0-4 are in title_screen.c). Step 5 shows the
 * "delete all save data?" prompt when New Game is chosen over a save, step 6 starts a new game (Tea's intro,
 * the starter deck choice) or lets Continue through to the main menu.
 *
 * Card Detail (card_detail.h; the viewer's steps are in card_detail.c): the shadowed text renderer and the
 * text-box mapper, the name/type/description page (CardDetail_DrawInfo) and the per-frame sprites (level
 * stars, icons, ATK/DEF digits).
 */
#include "global.h"
#include "legacy/gba.h"
#include "legacy/main.h"
#include "legacy/sound.h"
#include "constants/sound.h"
#include "constants/cards.h"
#include "util.h"
#include "palette.h"
#include "bg.h"
#include "sprite.h"
#include "text.h"
#include "card_data.h"
#include "card_detail.h"
#include "bustup.h"
#include "booster.h"
#include "title_screen.h"

/*
 * Until step H0 of the header plan installs the new gba.h, main.h and sound.h (build/readability/HEADERS.md),
 * include/ holds the legacy versions, which lack these names. The fallbacks repeat the staged headers'
 * values and prototypes; delete this block after H0.
 */
#ifndef DISPCNT_BG0_ON
#define DISPCNT_MODE_4          0x0004
#define DISPCNT_BG_ALL_ON       0x0F00
#define DISPCNT_OBJ_ON          0x1000
#define VBLANK_COPY_OAM         0x1
#define VBLANK_COPY_BG_MAPS     0x2
void ResetBgScroll(void);
void PlaySE(u32 seId);
void PlayBGM(u32 songId);
#endif

/*
 * Local views of functions, kept on purpose (matching choices, see build/readability/HEADERS.md):
 * - the fades are called as returning u16, so each caller truncates the result (lsls #16), while CB_Bustup
 *   and StarterDeckSelect_Run (u16 in their definitions) are called as returning u32 (no truncation);
 * - TextDrawNumber is called with the value as a 4th argument (the definition names only three).
 */
u16 FadeToBlackU16(s32 step) asm("FadeToBlack");
u16 FadeFromBlackU16(s32 step) asm("FadeFromBlack");
u32 CB_BustupU32(void) asm("CB_Bustup");
u32 StarterDeckSelect_RunU32(void) asm("StarterDeckSelect_Run");
void TextDrawNumber4(s32 x, s32 y, u16 sizeColor, s32 value) asm("TextDrawNumber");

/* Font size and colour index packed for the sizeColor argument of the TextDraw* functions (text.h). */
#define TEXT_SIZE_COLOR(size, color) (((size) << 8) | (color))

/* Sprite position argument of AddSprite: y << 16 | x. */
#define SPRITE_YX(x, y) (((y) << 16) | (x))

/* Packed arguments of CardDetail_DrawTextBox: map position or size in tiles (x | y << 8), text colour and
 * shadow colour (color | shadow << 8), and the text position on the canvas in pixels (x | y << 16). */
#define BOX_XY(x, y) (((y) << 8) | (x))
#define TEXT_COLORS(color, shadow) (((shadow) << 8) | (color))
#define TEXT_XY(x, y) (((y) << 16) | (x))

/* OBJ attr2 values of the Card Detail sprites: OBJ palette 3 with the digit tiles 0x30-0x39, the ATK/DEF
 * labels and the '?' and 'X' glyphs (loaded by CardDetail_InitVideo); the level star (tile 2, palette 0),
 * the attribute/spell-trap icon (tile 0x20, palette 1) and the spell/trap subtype icon (tile 0x24, palette 2)
 * loaded by CardDetail_DrawInfo. */
#define OBJ_DIGIT(d)            ((d) + 0x3030)
#define OBJ_LABEL_ATK           0x303A
#define OBJ_LABEL_DEF           0x303C
#define OBJ_GLYPH_QUESTION      0x303E
#define OBJ_GLYPH_X             0x303F
#define OBJ_STAR                0x0002
#define OBJ_TYPE_ICON           0x1020
#define OBJ_SUBTYPE_ICON        0x2024

/* ROM data used only by this unit. */
extern u16 (*const gTitleSteps[])(void);        /* 0x081988B0: Title_Init .. Title_StartGame, NULL */
extern const u8 gDeletePromptObjPal[];          /* 0x087D01D4 */
extern const u8 gDeletePromptObjGfx[];          /* 0x087CC1D4: 256x128 4bpp, "ATTENTION ... delete all save data?" */
extern const u8 gDeletePromptBgPal[];           /* 0x087CBFD4: 256 colours */
extern const u8 gDeletePromptBgBitmap[];        /* 0x087C29D4: 240x160 Mode-4 bitmap (stone frame) */
extern const u8 gTrapIconPal[];                 /* 0x08636348 */
extern const u8 gTrapIconGfx[];                 /* 0x086366A8 */
extern const u8 gMagicIconPal[];                /* 0x08636368 */
extern const u8 gMagicIconGfx[];                /* 0x08636728 */
extern const u8 gDivineIconPal[];               /* 0x08636388 */
extern const u8 gDivineIconGfx[];               /* 0x086367A8 */
extern const u8 gSpellTrapSubtypeIconGfx_1[];                /* 8x8 4bpp icons of SpellSubtype 1-6, one tile each */
extern const u8 gSpellTrapSubtypeIconPal[];     /* 0x08637454 */
extern const char *const gCardTypeNames[];      /* 0x081988D0: by enum CardType */
extern const char *const gSpellTrapSubtypeSuffixes[]; /* 0x08198934: by enum SpellSubtype */
extern const char gStrOpenBracket[];            /* "[" */
extern const char gStrCloseBracket[];           /* "]" */
extern const char gStrAtk[];                    /* "ATK" */
extern const char gStrDef[];                    /* "DEF" */
extern const char gStrNotACard[];               /* "This is not a card." (tokens) */
extern const char gStrEffectSuffix[];           /* "/Effect" */
extern const char gStrFusionEffectSuffix[];     /* "/Fusion/Effect" */
extern const char gStrFusionSuffix[];           /* "/Fusion" */
extern const char gStrRitualEffectSuffix[];     /* "/Ritual/Effect" */
extern const char gStrRitualSuffix[];           /* "/Ritual" */
extern const char gStrNotPlayable[];            /* "This is not able to play." (Ticket, Divine) */

/* ---- Title screen steps 5 and 6 ---- */

/* Adds the full-screen delete-save prompt picture: 4 columns x 5 rows of 64x32 sprites over the 240x160
 * screen, from OBJ tile 0x200 (tile row 16 with 2D mapping, 32 tiles per row). */
void Title_DrawDeletePrompt(void)
{
    int row, col;

    for (row = 0; row <= 4; row++)
        for (col = 0; col <= 3; col++)
            AddSprite((col << 6) | (row << 21), SPRITE_SHAPE_64x32, (row * 4 + 16) * 32 + col * 8);
}

/* gMain.seqState0 in Title_ConfirmDeleteSave. */
enum DeletePromptState {
    DELETE_PROMPT_CHECK = 0,            /* skip unless New Game was picked over a save; Mode 4, display off */
    DELETE_PROMPT_LOAD = 1,             /* prompt sprites and the background bitmap (both Mode-4 pages) */
    DELETE_PROMPT_FADE_IN = 2,
    DELETE_PROMPT_INPUT = 3,            /* START: delete (go on), B: keep the save */
    DELETE_PROMPT_CONFIRMED = 4,        /* fade out, then on to Title_StartGame */
    DELETE_PROMPT_CANCEL = 10,          /* B: fade out ... */
    DELETE_PROMPT_CANCEL_RESET = 11,    /* ... reset the video ... */
    DELETE_PROMPT_CANCEL_RESTART = 12,  /* ... and restart the title at TITLE_STEP_SETUP */
};

/*
 * TITLE_STEP_CONFIRM_DELETE_SAVE: returns 1 at once unless New Game was picked with a save present
 * (continueSelected can only be 1 when there is a save). Otherwise shows the "delete all save data?" prompt:
 * START returns 1 (Title_StartGame then starts the new game, which overwrites the save), B fades out and goes
 * back to TITLE_STEP_SETUP by writing gMain.seqIndexTop itself.
 */
u16 Title_ConfirmDeleteSave(void)
{
    switch (gMain.seqState0) {
    case DELETE_PROMPT_CHECK:
        if (gTitleState.savePresent == gTitleState.continueSelected)
            return 1;
        REG_DISPCNT = DISPCNT_MODE_4;
        gMain.vblankFlags = VBLANK_COPY_OAM;
        gMain.seqState0++;
        break;
    case DELETE_PROMPT_LOAD:
        CopyDoubleWords((void *)OBJ_PLTT, gDeletePromptObjPal, 0x20);
        CopyDoubleWords((void *)(OBJ_VRAM0 + 0x4000), gDeletePromptObjGfx, 0x4000); /* OBJ tile 0x200 */
        MemCopy16((void *)BG_PLTT, gDeletePromptBgPal, 0x200);
        MemCopy16((void *)VRAM, gDeletePromptBgBitmap, 240 * 160);            /* Mode-4 page 0 */
        MemCopy16((void *)(VRAM + 0xA000), gDeletePromptBgBitmap, 240 * 160); /* Mode-4 page 1 */
        gMain.seqState0++;
        break;
    case DELETE_PROMPT_FADE_IN:
        REG_DISPCNT = DISPCNT_MODE_4 | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
        Title_DrawDeletePrompt();
        if (FadeFromBlackU16(2))
            gMain.seqState0++;
        break;
    case DELETE_PROMPT_INPUT:
        Title_DrawDeletePrompt();
        if (gMain.newKeys & B_BUTTON) {
            PlaySE(SE_CANCEL);
            gMain.seqState0 = DELETE_PROMPT_CANCEL;
        } else if (gMain.newKeys & START_BUTTON) {
            PlaySE(SE_CONFIRM);
            gMain.seqState0++;
        }
        break;
    case DELETE_PROMPT_CONFIRMED:
        Title_DrawDeletePrompt();
        if (FadeToBlackU16(2))
            return 1;
        break;
    case DELETE_PROMPT_CANCEL:
        Title_DrawDeletePrompt();
        if (FadeToBlackU16(2))
            gMain.seqState0++;
        return 0;
    case DELETE_PROMPT_CANCEL_RESET:
        SetBrightnessBlack();
        ResetVideo();
        ResetBgScroll();
        Title_InitBgCnt();
        gMain.vblankFlags = VBLANK_COPY_OAM | VBLANK_COPY_BG_MAPS;
        gMain.seqState0++;
        return 0;
    case DELETE_PROMPT_CANCEL_RESTART:
        gMain.seqIndexTop = TITLE_STEP_SETUP;
        gMain.seqState0 = 0;
        break;
    }
    return 0;
}

/*
 * TITLE_STEP_START_GAME: Continue returns 1 at once. New Game plays Tea's intro (dialogue event 100, BGM 1),
 * runs the starter deck choice (StarterDeckSelect_Run: deck choice, new save, BuildStarterDeck), then
 * dialogue event 101, and returns 1 when that dialogue ends.
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
        if (CB_BustupU32()) {
            gMain.seqState0++;
            gMain.seqIndex1 = 0;
        }
        return 0;
    case 2:
        if (StarterDeckSelect_RunU32()) {
            StartDialogue(101);
            gMain.seqState0++;
        }
        return 0;
    default:
        return CB_BustupU32();
    }
}

/* Title scene callback: runs gTitleSteps[gMain.seqIndexTop]; a step that returns 1 moves on to the next one
 * with the sub-states reset. Returns 1 at the table's NULL end, and MainLoop then switches to the main menu. */
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

/* ---- Card Detail drawing ---- */

/* Card Detail HBlank handler (the wave intro): on visible lines, BG1HOFS and BG2HOFS from
 * gMain.hblankScroll[line & 15]. */
void CardDetail_HBlank(void)
{
    gMain.lastVcount = REG_VCOUNT;
    if (gMain.lastVcount < 160) {
        REG_BG1HOFS = gMain.hblankScroll[gMain.lastVcount & 15];
        REG_BG2HOFS = gMain.hblankScroll[gMain.lastVcount & 15];
    }
}

/*
 * Renders str into the text canvas (size = width | height << 8 in tiles, pos = x | y << 16 in px, colors =
 * colour | shadow colour << 8) with a drop shadow at +1,+1 and line gap 2. Text of 400+ characters, or text
 * that reaches canvas row 0xC0, is drawn again with line gap 1, then without the shadow at font size 8, then
 * with line gap 0. Each pass stores the text height in gCardDetail.textHeight.
 */
void CardDetail_RenderText(u16 size, u32 pos, const u8 *str, u16 colors, int fontSize, u16 wrap)
{
    u32 color = (u8)colors;
    u32 shadowColor = (u8)(colors >> 8);
    u32 width = (u8)size;
    u32 height = (u8)(size >> 8);
    u32 x = (u16)pos;
    u32 y = (u16)(pos >> 16);
    u32 sizeColor;

    /* FAKEMATCH: an extra (code-free) use of color raises its global-alloc priority
     * above width/height, so color gets r9, height sl and width is spilled to the stack. */
    asm("" : : "r"(color));
    if (StrLen(str) < 400) {
        TextCanvasInitEx(width, height, wrap, 2);
        TextDrawString(x + 1, y + 1, shadowColor | ((u8)fontSize << 8), str);
        TextDrawString(x, y, color | ((u8)fontSize << 8), str);
        gCardDetail.textHeight = (gTextCanvas.bottom - fontSize) & (gTextCanvas.bottom - fontSize + 1);
        if (gTextCanvas.bottom < 0xC0)
            return;
    }

    TextCanvasInitEx(width, height, wrap, 1);
    TextDrawString(x + 1, y + 1, shadowColor | ((u8)fontSize << 8), str);
    TextDrawString(x, y, color | ((u8)fontSize << 8), str);
    gCardDetail.textHeight = (gTextCanvas.bottom - fontSize) & (gTextCanvas.bottom - fontSize + 1);
    if (gTextCanvas.bottom < 0xC0)
        return;

    /* No shadow, font size 8. sizeColor is a u32 so the 0x800 stays a full-width
     * register and the orr ties to it, as in the original. */
    TextCanvasInitEx(width, height, wrap, 1);
    sizeColor = color | (8 << 8);
    TextDrawString(x, y, sizeColor, str);
    gCardDetail.textHeight = (gTextCanvas.bottom - 8) & (gTextCanvas.bottom - 8 + 1);
    if (gTextCanvas.bottom < 0xC0)
        return;

    TextCanvasInitEx(width, height, wrap, 0);
    TextDrawString(x, y, sizeColor, str);
    gCardDetail.textHeight = (gTextCanvas.bottom - 8) & (gTextCanvas.bottom - 8 + 1);
}

/*
 * Renders str into a width x height tile canvas (CardDetail_RenderText), converts it to BG tiles from `tile`
 * (charblock 1; background pixels = bgColor) and maps those tiles row by row into gMain.bgMapBuffer[bg] at
 * mapPos (x | y << 8, in tiles).
 */
void CardDetail_DrawTextBox(u32 bg, u16 mapPos, u16 size, u16 tile, u16 colors, u16 fontSize,
                            u32 textPos, const u8 *str, u16 wrap, u16 bgColor)
{
    u32 x = (u8)mapPos;
    u32 y = mapPos >> 8;
    s32 width = (u8)size;
    s32 height = size >> 8;
    s32 row, col;

    CardDetail_RenderText(size, textPos, str, colors, fontSize, wrap);
    TextCanvasToTiles((u16 *)(VRAM + 0x4000 + tile * 32), bgColor);
    for (row = 0; row < height; row++)
        for (col = 0; col < width; col++)
            gMain.bgMapBuffer[bg][(row + y) * 32 + x + col] = tile++;
}

/*
 * gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4) read by address: the constant-address form makes
 * GCC reload the table base at each use, as the ROM does.
 */
#define CARD_STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])    /* gCardStats[id] */
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])    /* gCardIdToNumber[id] */
#define CARD_TYPE(id)       CARD_STATS_TYPE(CARD_STATS_WORD(id))                 /* enum CardType */

/* Card numbers 1920-1999 are monster tokens: they have their own page and no stat sprites. */
#define IS_TOKEN(id) \
    ((u16)(CARD_NUMBER(id) - CARD_NUMBER_TOKEN_FIRST) < CARD_NUMBER_TOKEN_END - CARD_NUMBER_TOKEN_FIRST)

/*
 * Stat helpers (card_detail.c has its own copies). The switches on the card type go through GetCardType:
 * inlined RTL keeps the stats address as (plus reg const), the same form the other inlines use, so CSE shares
 * the address register.
 */
static inline int GetCardType(u16 id)
{
    return CARD_TYPE(id);
}

/* enum SpellSubtype of a Magic or Trap card, else 0. */
static inline int GetSpellSubtype(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return CARD_STATS_SUBTYPE(CARD_STATS_WORD(id));
    default:
        return 0;
    }
}

/* Level stars: 0 for Trap/Magic/Ticket, 10 for the Divine cards. */
static inline int GetCardLevel(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS_WORD(id));
    }
}

/* GetCardLevel as a u8: the narrow return type gives the inline its own result register (r0) and the copy
 * into `level` that CardDetail_DrawSprites keeps. */
static inline u8 GetCardLevelU8(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS_WORD(id));
    }
}

/* Shown ATK/DEF: 0 for Trap/Magic/Ticket, 4000 for the Divine cards. The u16 return type gives the inline its
 * own result register (r0) and the copy into the argument register that the ROM has. */
static inline u16 GetCardAtk(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_ATK(CARD_STATS_WORD(id)) * CARD_STATS_POINTS_SCALE;
    }
}

static inline u16 GetCardDef(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_DEF(CARD_STATS_WORD(id)) * CARD_STATS_POINTS_SCALE;
    }
}

/* enum CardKind: Obelisk counts as a ritual monster, Slifer and Ra as effect monsters. */
static inline int GetCardKind(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    default:
        return CARD_STATS_KIND(CARD_STATS_WORD(id));
    }
}

static inline u32 GetCardAttribute(u16 id)
{
    return CARD_STATS_ATTR(CARD_STATS_WORD(id));
}

/* Cell (x, y) of gMain.bgMapBuffer[bg]. Written directly (not through an inline taking bg), so the constant
 * bg folds into the gMain offset (0x0300245C for bg 4). */
#define BG_MAP_CELL(bg, x, y) (gMain.bgMapBuffer[bg][(u16)(x) + (u16)(y) * 32])

/*
 * Draws the Card Detail text page of a card:
 * - the name at the top (centred in the wave-intro layout, smaller when longer than 36 characters);
 * - the icon graphics: Trap/Magic icon plus the spell/trap subtype icon, the Divine icon, or the attribute
 *   icon of a monster;
 * then, unless gCardDetail.waveIntro is set:
 * - tokens: the attribute icon and level stars on BG0, ATK and DEF, and "This is not a card.";
 * - other cards: "[Type/Subtype]", the description, and "This is not able to play." for Ticket and Divine
 *   cards.
 * Resets the description scroll.
 */
void CardDetail_DrawInfo(u16 cardId)
{
    const u8 *name;
    int len, labelLen;
    int nameX, nameY;
    u16 fontSize;
    int i, j;
    u16 tile;
    u8 buf[0x80];

    name = (const u8 *)0x0822C720 + cardId * CARD_NAME_SIZE; /* gCardNames */
    len = StrLen(name);
    nameX = 4;
    nameY = 2;
    fontSize = 12;
    if (len > 36) {
        nameY = 4;
        fontSize = 10;
    }
    if (gCardDetail.waveIntro)
        nameX = 120 - ((len * fontSize) >> 1);

    /* Icons: OBJ palette 1 / tile 0x20 (type or attribute), OBJ palette 2 / tile 0x24 (spell/trap subtype). */
    switch (GetCardType(cardId)) {
    case CARD_TYPE_TRAP:
        CopyDoubleWords((void *)(OBJ_PLTT + 0x20), gTrapIconPal, 0x20);
        CopyDoubleWords((void *)(OBJ_VRAM0 + 0x400), gTrapIconGfx, 0x80);
        if (GetSpellSubtype(cardId)) {
            /* The base is loaded before the inline runs, so it is assigned first. */
            const u8 *icon = gSpellTrapSubtypeIconGfx_1;
            icon += (GetSpellSubtype(cardId) - 1) * 32;
            CopyDoubleWords((void *)(OBJ_PLTT + 0x40), gSpellTrapSubtypeIconPal, 0x20);
            CopyDoubleWords((void *)(OBJ_VRAM0 + 0x480), icon, 0x20);
        }
        break;
    case CARD_TYPE_MAGIC:
        CopyDoubleWords((void *)(OBJ_PLTT + 0x20), gMagicIconPal, 0x20);
        CopyDoubleWords((void *)(OBJ_VRAM0 + 0x400), gMagicIconGfx, 0x80);
        if (GetSpellSubtype(cardId)) {
            const u8 *icon = gSpellTrapSubtypeIconGfx_1;
            icon += (GetSpellSubtype(cardId) - 1) * 32;
            CopyDoubleWords((void *)(OBJ_PLTT + 0x40), gSpellTrapSubtypeIconPal, 0x20);
            CopyDoubleWords((void *)(OBJ_VRAM0 + 0x480), icon, 0x20);
        }
        break;
    case CARD_TYPE_TICKET:
        break;
    case CARD_TYPE_DIVINE:
        CopyDoubleWords((void *)(OBJ_PLTT + 0x20), gDivineIconPal, 0x20);
        CopyDoubleWords((void *)(OBJ_VRAM0 + 0x400), gDivineIconGfx, 0x80);
        break;
    default:
    {
        u32 attr = CARD_STATS_ATTR(CARD_STATS_WORD(cardId));

        /* Nested ifs: a single && chain folds into one (attr - 1) <= 5 range test. */
        if (attr != 0) {
            if (attr <= ATTRIBUTE_WIND && CARD_TYPE(cardId) <= CARD_TYPE_REPTILE) {
                CopyDoubleWords((void *)(OBJ_PLTT + 0x20), (const void *)gCardIconPals[attr], 0x20);
                CopyDoubleWords((void *)(OBJ_VRAM0 + 0x400), (const void *)gCardIconGfx[attr], 0x80);
            }
        }
        break;
    }
    }

    ClearBgMapBuffer0();
    CardDetail_DrawTextBox(0, BOX_XY(0, 0), BOX_XY(32, 2), 0x1E4, TEXT_COLORS(7, 8), fontSize,
                           (u16)nameX | (nameY << 16), name, 1, 1);
    if (gCardDetail.waveIntro)
        return;

    if (IS_TOKEN(cardId)) {
        /* Attribute icon (BG palette 1, tiles 0x20-0x23) and the level stars (tile 3) on BG0, right of the
         * name. */
        CopyDoubleWords((void *)(BG_PLTT + 0x20), (const void *)gCardIconPals[GetCardAttribute(cardId)], 0x20);
        CopyDoubleWords((void *)(VRAM + 0x4400), (const void *)gCardIconGfx[GetCardAttribute(cardId)], 0x80);
        BG_MAP_CELL(0, 13, 2) = 0x1020;
        BG_MAP_CELL(0, 14, 2) = 0x1021;
        BG_MAP_CELL(0, 13, 3) = 0x1022;
        BG_MAP_CELL(0, 14, 3) = 0x1023;
        for (i = 0; i < GetCardLevel(cardId); i++)
            BG_MAP_CELL(0, i + 15, 3) = 3;
        StrCopy(buf, gStrOpenBracket);
        StrCat(buf, gCardTypeNames[GetCardType(cardId)]);
        StrCat(buf, gStrCloseBracket);
        /* "[Type]" with its shadow; TextCanvasInit clears the canvas right after, so these two are never
         * converted to tiles. Then ATK and DEF with shadows on a 17x16-tile canvas, mapped to
         * gMain.bgMapBuffer[4] at (13, 2). */
        TextDrawString(4, 0x14, TEXT_SIZE_COLOR(10, 8), buf);
        TextDrawString(3, 0x13, TEXT_SIZE_COLOR(10, 7), buf);
        TextCanvasInit(17, 16);
        labelLen = StrLen(gStrAtk);
        TextDrawString(4, 0x20, TEXT_SIZE_COLOR(10, 13), gStrAtk);
        TextDrawString(3, 0x1F, TEXT_SIZE_COLOR(10, 5), gStrAtk);
        TextDrawNumber4((labelLen + 4) * 5 + 4, 0x20, TEXT_SIZE_COLOR(10, 8), GetCardAtk(cardId));
        TextDrawNumber4((labelLen + 4) * 5 + 3, 0x1F, TEXT_SIZE_COLOR(10, 7), GetCardAtk(cardId));
        labelLen = StrLen(gStrDef);
        TextDrawString(4, 0x2C, TEXT_SIZE_COLOR(10, 11), gStrDef);
        TextDrawString(3, 0x2B, TEXT_SIZE_COLOR(10, 3), gStrDef);
        TextDrawNumber4((labelLen + 4) * 5 + 4, 0x2C, TEXT_SIZE_COLOR(10, 8), GetCardDef(cardId));
        TextDrawNumber4((labelLen + 4) * 5 + 3, 0x2B, TEXT_SIZE_COLOR(10, 7), GetCardDef(cardId));
        TextCanvasToTiles((u16 *)(VRAM + 0x8900), 9); /* tile 0x248 */
        tile = 0x248;
        for (i = 0; i <= 15; i++)
            for (j = 0; j <= 16; j++)
                BG_MAP_CELL(4, j + 13, i + 2) = tile++;
        CardDetail_DrawTextBox(4, BOX_XY(13, 18), BOX_XY(18, 2), 0x224, TEXT_COLORS(6, 8), 10, TEXT_XY(3, 3),
                               gStrNotACard, 0, 11);
        gMain.bgVofs[3] = 0;
        gCardDetail.scrollTarget = 0;
        gCardDetail.scrollPos = 0;
    } else {
        StrCopy(buf, gStrOpenBracket);
        switch (GetCardType(cardId)) {
        case CARD_TYPE_TRAP:
        case CARD_TYPE_MAGIC:
            StrCat(buf, gCardTypeNames[GetCardType(cardId)]);
            if (GetSpellSubtype(cardId))
                StrCat(buf, gSpellTrapSubtypeSuffixes[GetSpellSubtype(cardId)]);
            break;
        case CARD_TYPE_TICKET:
        case CARD_TYPE_DIVINE:
            StrCat(buf, gCardTypeNames[GetCardType(cardId)]);
            break;
        default:
            StrCat(buf, gCardTypeNames[GetCardType(cardId)]);
            switch (GetCardKind(cardId)) {
            case CARD_KIND_NORMAL:
                break;
            case CARD_KIND_EFFECT:
                StrCat(buf, gStrEffectSuffix);
                break;
            case CARD_KIND_FUSION:
                /* Fusions with an effect; 1241, 1334 and 1526 are not EDS cards. */
                switch (CARD_NUMBER(cardId)) {
                case CARD_ALLIGATORS_SWORD_DRAGON:
                case 1241:
                case 1334:
                case 1526:
                    StrCat(buf, gStrFusionEffectSuffix);
                    break;
                default:
                    StrCat(buf, gStrFusionSuffix);
                    break;
                }
                break;
            case CARD_KIND_RITUAL:
                if (CARD_NUMBER(cardId) == CARD_RELINQUISHED)
                    StrCat(buf, gStrRitualEffectSuffix);
                else
                    StrCat(buf, gStrRitualSuffix);
                break;
            }
            break;
        }
        StrCat(buf, gStrCloseBracket);
        len = StrLen(buf);
        fontSize = 10;
        if (len > 22)
            fontSize = 8;
        /* The (u16) keeps CSE from sharing the 9 with the last argument. */
        CardDetail_DrawTextBox(0, BOX_XY(13, 2), BOX_XY(18, 2), 0x224, TEXT_COLORS(7, 8), fontSize,
                               ((u16)(9 - (fontSize >> 1)) << 16) | 4, buf, 1, 9);
        /* The description (gCardDescriptions, by address) in an 18x24-tile box on bgMapBuffer[4], which
         * scrolls (BG3) */
        CardDetail_DrawTextBox(4, BOX_XY(13, 4), BOX_XY(18, 24), 0x248, TEXT_COLORS(7, 9), 10, TEXT_XY(4, 2),
                               (const u8 *)0x082461A0 + cardId * CARD_DESCRIPTION_SIZE, 1, 0);
        if (CARD_TYPE(cardId) > CARD_TYPE_MAGIC)
            CardDetail_DrawTextBox(4, BOX_XY(13, 18), BOX_XY(17, 2), 0x369, TEXT_COLORS(6, 8), 10, TEXT_XY(3, 3),
                                   gStrNotPlayable, 0, 11);
        /* Duplicated in both branches: each copy reuses the 0 of the last call's arguments and
         * cross-jumping merges the stores. */
        gMain.bgVofs[3] = 0;
        gCardDetail.scrollTarget = 0;
        gCardDetail.scrollPos = 0;
    }
}

/* Adds digit sprites for value (OBJ_DIGIT tiles), right to left from x + 0x14, 4 px apart; one 0 for 0. */
void CardDetail_DrawNumber(int x, int y, int value)
{
    x += 0x14;
    if (value == 0) {
        AddSprite((y << 16) | x, SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
    } else {
        do {
            AddSprite(x | (y << 16), SPRITE_SHAPE_8x8, OBJ_DIGIT(value % 10));
            value /= 10;
            x -= 4;
        } while (value != 0);
    }
}

/* Adds the "ATK" label and gCardDetail.atk at y 0x86 (0x48 further right in the wave-intro layout). */
void CardDetail_DrawAtk(void)
{
    int x;

    if (gCardDetail.waveIntro)
        x = 0x48;
    else
        x = 0;
    AddSprite((x + 0x3E) | (0x86 << 16), SPRITE_SHAPE_16x8, OBJ_LABEL_ATK);
    CardDetail_DrawNumber(x + 0x44, 0x86, gCardDetail.atk);
}

/* Adds the "DEF" label and gCardDetail.def at y 0x8E. */
void CardDetail_DrawDef(void)
{
    int x;

    if (gCardDetail.waveIntro)
        x = 0x48;
    else
        x = 0;
    AddSprite((x + 0x3E) | (0x8E << 16), SPRITE_SHAPE_16x8, OBJ_LABEL_DEF);
    CardDetail_DrawNumber(x + 0x44, 0x8E, gCardDetail.def);
}

/*
 * Per-frame Card Detail sprites; nothing for tokens (CardDetail_DrawInfo puts their stars on BG0):
 * - Divine: 10 stars, the icon and fixed ATK/DEF glyphs (Obelisk 4000, Slifer X000, Ra ????);
 * - Trap/Magic: the icon and, if it has one, the subtype icon;
 * - monsters: the level stars (squeezed together above 9), the attribute icon and ATK/DEF.
 * Case CARD_TYPE_DIVINE is written first: that body order reproduces the ROM's block layout. The type
 * switch reads the card through an int local, so its zero-extending load is not shared with the u16 load of
 * the card-number check.
 */
void CardDetail_DrawSprites(void)
{
    int x;
    int level;
    int i;
    u32 attr;
    int id;

    if (gCardDetail.waveIntro)
        x = 0x48;
    else
        x = 0;
    level = GetCardLevelU8(gCardDetail.cardId);
    if (IS_TOKEN(gCardDetail.cardId))
        return;
    id = gCardDetail.cardId;
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_DIVINE:
        for (i = 0; i <= 9; i++)
            AddSprite((x + 0x54 - i * 8) | (0x26 << 16), SPRITE_SHAPE_8x8, OBJ_STAR);
        AddSprite((x + 0x4C) | (0x16 << 16), SPRITE_SHAPE_16x16, OBJ_TYPE_ICON);
        switch (CARD_NUMBER(gCardDetail.cardId)) {
        case CARD_OBELISK_THE_TORMENTOR: /* 4000 */
            AddSprite(SPRITE_YX(0x3F, 0x86), SPRITE_SHAPE_16x8, OBJ_LABEL_ATK);
            AddSprite(SPRITE_YX(0x4B, 0x86), SPRITE_SHAPE_8x8, OBJ_DIGIT(4));
            AddSprite(SPRITE_YX(0x4F, 0x86), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            AddSprite(SPRITE_YX(0x53, 0x86), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            AddSprite(SPRITE_YX(0x57, 0x86), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            AddSprite(SPRITE_YX(0x3F, 0x8E), SPRITE_SHAPE_16x8, OBJ_LABEL_DEF);
            AddSprite(SPRITE_YX(0x4B, 0x8E), SPRITE_SHAPE_8x8, OBJ_DIGIT(4));
            AddSprite(SPRITE_YX(0x4F, 0x8E), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            AddSprite(SPRITE_YX(0x53, 0x8E), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            AddSprite(SPRITE_YX(0x57, 0x8E), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            break;
        case CARD_SLIFER_THE_SKY_DRAGON: /* X000 */
            AddSprite(SPRITE_YX(0x3F, 0x86), SPRITE_SHAPE_16x8, OBJ_LABEL_ATK);
            AddSprite(SPRITE_YX(0x4B, 0x86), SPRITE_SHAPE_8x8, OBJ_GLYPH_X);
            AddSprite(SPRITE_YX(0x4F, 0x86), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            AddSprite(SPRITE_YX(0x53, 0x86), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            AddSprite(SPRITE_YX(0x57, 0x86), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            AddSprite(SPRITE_YX(0x3F, 0x8E), SPRITE_SHAPE_16x8, OBJ_LABEL_DEF);
            AddSprite(SPRITE_YX(0x4B, 0x8E), SPRITE_SHAPE_8x8, OBJ_GLYPH_X);
            AddSprite(SPRITE_YX(0x4F, 0x8E), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            AddSprite(SPRITE_YX(0x53, 0x8E), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            AddSprite(SPRITE_YX(0x57, 0x8E), SPRITE_SHAPE_8x8, OBJ_DIGIT(0));
            break;
        case CARD_THE_WINGED_DRAGON_OF_RA: /* ???? */
            AddSprite(SPRITE_YX(0x3F, 0x86), SPRITE_SHAPE_16x8, OBJ_LABEL_ATK);
            AddSprite(SPRITE_YX(0x4B, 0x86), SPRITE_SHAPE_8x8, OBJ_GLYPH_QUESTION);
            AddSprite(SPRITE_YX(0x4F, 0x86), SPRITE_SHAPE_8x8, OBJ_GLYPH_QUESTION);
            AddSprite(SPRITE_YX(0x53, 0x86), SPRITE_SHAPE_8x8, OBJ_GLYPH_QUESTION);
            AddSprite(SPRITE_YX(0x57, 0x86), SPRITE_SHAPE_8x8, OBJ_GLYPH_QUESTION);
            AddSprite(SPRITE_YX(0x3F, 0x8E), SPRITE_SHAPE_16x8, OBJ_LABEL_DEF);
            AddSprite(SPRITE_YX(0x4B, 0x8E), SPRITE_SHAPE_8x8, OBJ_GLYPH_QUESTION);
            AddSprite(SPRITE_YX(0x4F, 0x8E), SPRITE_SHAPE_8x8, OBJ_GLYPH_QUESTION);
            AddSprite(SPRITE_YX(0x53, 0x8E), SPRITE_SHAPE_8x8, OBJ_GLYPH_QUESTION);
            AddSprite(SPRITE_YX(0x57, 0x8E), SPRITE_SHAPE_8x8, OBJ_GLYPH_QUESTION);
            break;
        }
        break;
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        AddSprite((x + 0x4C) | (0x16 << 16), SPRITE_SHAPE_16x16, OBJ_TYPE_ICON);
        if (GetSpellSubtype(gCardDetail.cardId) != 0)
            AddSprite((x + 0x50) | (0x24 << 16), SPRITE_SHAPE_8x8, OBJ_SUBTYPE_ICON);
        break;
    case CARD_TYPE_TICKET:
        break;
    default:
        for (i = 0; i < level; i++) {
            if (level <= 9)
                AddSprite((x + 0x54 - i * 8) | (0x26 << 16), SPRITE_SHAPE_8x8, OBJ_STAR);
            else {
                /* 10+ stars share the 0x4E px of 9 star spacings */
                int t = 0x54 - 0x4E * i / level;
                AddSprite((t + x) | (0x26 << 16), SPRITE_SHAPE_8x8, OBJ_STAR);
            }
        }
        attr = CARD_STATS_ATTR(CARD_STATS_WORD(gCardDetail.cardId));
        if (attr != 0)
            if (attr <= ATTRIBUTE_WIND)
                AddSprite((x + 0x4C) | (0x16 << 16), SPRITE_SHAPE_16x16, OBJ_TYPE_ICON);
        CardDetail_DrawAtk();
        CardDetail_DrawDef();
        break;
    }
}
