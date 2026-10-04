/*
 * Duel screen text box, info bar and graphics loaders (wiki/functions/duel-info-bar-c.md).
 *
 * Text box (gTextBox, struct TextBox in text_box.h): the framed message that slides down from the top of the
 * duel screen on BG3. TextBoxOpen renders a string into the 216 text tiles (TextBoxDrawText), after which
 * DuelMainStep calls TextBoxUpdate every frame: slide in (TextBoxSlideIn), wait for the menu input, slide
 * out (TextBoxSlideOut). The menu kind (enum TextBoxMenuKind, TextBoxSetMenu) selects the input handler and
 * the sprite callback: a plain message closes on A, Yes/No answers with pulsing YES/NO labels and a pointing
 * hand, the two-line choice shows an arrow, and callers may install their own callbacks. The answer is left
 * in gTextBox.result.
 *
 * Info bar: DuelScreen_DrawCursorInfo redraws the two text rows at the bottom of the screen for the card or
 * pile under the field cursor (the drawing helpers are in battle_scene.c).
 *
 * Screen setup, called by DuelScreen_Init: the BG control registers, the scrolling field background on BG0
 * (DuelScreen_VBlank drifts it), the OBJ and BG graphics loads, and the life-point numbers and the
 * D S M B M E phase letters on BG2.
 *
 * Duel graphics layout (VRAM, 4bpp tiles of 0x20 bytes): OBJ tiles start at 0x06010000 (1D mapping); BG
 * tiles live in character block 1 at 0x06004000, so BG tile n is at 0x06004000 + n * 0x20. A sprite's attr2
 * is tile | palette << 12; a BG map entry is the same.
 */
#include "global.h"
#include "legacy/gba.h"                    /* REG_*, BG_PLTT, OBJ_PLTT, VRAM, OBJ_VRAM0, key masks */
#include "legacy/main.h"                   /* gMain: keys, frame counter, BG map buffers */
#include "util.h"                   /* CopyDoubleWords, StrLen */
#include "bg.h"                     /* LoadBgImage4bpp, LoadSystemGfx */
#include "sprite.h"                 /* AddSprite, AddAffineSprite, SPRITE_SHAPE_*, gHandCursorPal/Gfx */
#include "text.h"                   /* TextCanvasInit, TextDraw*Glyph, TextWordLength, TextCanvasToTiles */
#include "save.h"                   /* gSaveData */
#include "card_data.h"              /* gCardIconPal, gCardIcon*Gfx */
#include "duel_flow.h"              /* gPulseScaleCurve */
#include "constants/duel.h"         /* enum DuelArea */
#include "constants/sound.h"        /* SE_CURSOR, SE_CONFIRM, SE_CANCEL, SE_ERROR */

/* ---- BEGIN duel.h stand-in (pre-H0) ---- */
/*
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md),
 * and duel_screen.h includes it. This block declares the part of the canonical duel.h that this unit and
 * duel_screen.h use, with the header's names, types and bitfield containers (unused bytes are padding), and
 * defines duel.h's include guard so the legacy header stays out. PlaySE is declared here too: the legacy
 * sound.h lacks it. After H0, replace the block (BEGIN to END) with #include "legacy/duel.h" and #include "sound.h"
 * (see build/readability/issues/duel_info_bar.md).
 */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID */
    u32 unk12:20;
};

/* A card location on the duel screen (DuelScreen.from / .to). */
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13 */
    u16 isDefense:1;                /* bit 14 */
    u16 isFaceUp:1;                 /* bit 15 */
    u16 unk2;
};

struct DuelZone {                   /* one field zone, 0x94 bytes */
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0 */
    u8 isFaceUp:1;                  /* +0x06 bit 1 */
    u8 unk6_2:6;
    u8 unk7[0x94 - 7];
};

struct DuelPlayer {                 /* one player's side, 0xD64 bytes */
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002 */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 fusionCount;                 /* +0x005: entries in fusionDeck[] */
    u8 banishedCount;               /* +0x006: entries in banished[] */
    u8 unk7[0xD64 - 7];
};

struct DuelZonesPlayer {            /* gDuelZones: the zones of each player with the player stride */
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuelPlayers[0].zones */
extern struct DuelZone gDuelSpellTrapZones;     /* 0x020195F0 = gDuelPlayers[0].zones[ZONE_SPELL_0] */
extern struct DuelZone gDuelFieldZone;          /* 0x020198D4 = gDuelPlayers[0].zones[ZONE_FIELD] */

/* 1 if the player's hand is shown (handRevealed, The Eye of Truth, Ceremonial Bell, ...). */
int IsHandRevealed(int player);
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "duel_screen.h"            /* gDuelScreen, DuelInfo_*, TextCellsClear, DuelCursor_GetCardId */
#include "text_box.h"               /* gTextBox, the TextBox* functions defined here */

/*
 * Transitional, until H0 installs the new include/gba.h: the legacy gba.h lacks these names. The values are
 * the new header's; the block is skipped once it is in place.
 */
#ifndef BGCNT_PRIORITY
#define DISPCNT_OBJ_1D_MAP      0x0040  /* OBJ tiles in 1D mapping */
#define BGCNT_PRIORITY(n)       (n)             /* 0 = front, 3 = back */
#define BGCNT_CHARBASE(n)       ((n) << 2)      /* tile data at VRAM + n * 0x4000 */
#define BGCNT_SCREENBASE(n)     ((n) << 8)      /* map at VRAM + n * 0x800 */
#define BGCNT_TXT256x512        0x8000          /* map size: 256 x 512 */
#endif
#ifndef OAM_ATTR2_PALETTE
#define OAM_ATTR2_PALETTE(n)    ((n) << 12)     /* 16-colour palette 0-15 */
#endif

/*
 * Local views kept on purpose (matching choices, see build/readability/HEADERS.md):
 * - the matched code tests the language byte of the save image as a whole byte (gSaveData +4);
 * - the first byte of gDuelScreen (fast, uiGfxLoaded) is tested as a byte in TextBoxHandleChoiceInput;
 * - TextBoxUpdate calls TextBoxSlideIn / TextBoxSlideOut through u16-returning prototypes (the header says
 *   int), so the caller narrows the result with `lsl 16`;
 * - TextBoxDrawText takes the word length as an int (the header returns u8, which would add a mask).
 */
extern u8 gSaveDataBytes[] asm("gSaveData");    /* byte view of struct SaveData gSaveData */
#define SAVE_FLAGS4_SJIS_TEXT   0x80            /* gSaveData +4 bit 7: text uses 2-byte Shift-JIS characters */

extern u8 gDuelScreenFlags asm("gDuelScreen");  /* first byte of gDuelScreen: bit 0 fast, bit 1 uiGfxLoaded */

extern u16 TextBoxSlideInU16(void) asm("TextBoxSlideIn");
extern u16 TextBoxSlideOutU16(void) asm("TextBoxSlideOut");
extern int TextWordLengthInt(const u8 *s) asm("TextWordLength");

/* ROM data used only here. Palettes are 16 colours (0x20 bytes). */
extern const u16 gAButtonIconFrames[];      /* 0x081A423C: [32] A-button icon frame (0-2) per 4-frame tick */
extern u16 *const gFieldBackgroundImages[]; /* 0x08086550: [15] 32x32-px textures for BG0; 0 = normal field,
                                               1-14 = field-magic backgrounds (GetFieldMagicIndex) */
extern const u32 gTextBoxColors[];          /* 0x081A4214: [10] palette indices for TextBoxDrawText: [0] = 5,
                                               the background; [1-8] the text colours '@1'-'@8'; [9] = 3, the
                                               shadow */
/* OBJ graphics (LoadDuelUiGfx) */
extern const u8 gUnk_0867817C[];            /* card back, 4 poses of 32x32 (also BG tile 0x70) */
extern const u8 gUnk_0867FC3C[];            /* OBJ palette 2: the card command menu icons */
extern const u8 gUnk_0867FC5C[];            /* card command menu icons, 16x16 each, 4 tiles per command */
extern const u8 gUnk_0868045C[];            /* OBJ palette 3: the card command menu labels */
extern const u8 gUnk_0868047C[];            /* card command menu labels ("Card View", "Def Pos", ...), 32x16
                                               each, 8 tiles per command */
extern const u8 gUnk_0868167C[];            /* OBJ palette 4: button icons, YES/NO labels, arrow */
extern const u8 gUnk_0868467C[];            /* OBJ palette 5: attack and link markers */
extern const u8 gUnk_0868487C[];            /* "can attack" marker frames (MARKER_TILE_CAN_ATTACK) */
extern const u8 gUnk_086849FC[];            /* equip link marker frames (MARKER_TILE_LINK_EQUIP) */
extern const u8 gUnk_08684B7C[];            /* other link marker frames (MARKER_TILE_LINK_OTHER) */
extern const u8 gUnk_0868557C[];            /* OBJ palette 6: the link "Wait..." sprites */
extern const u8 gUnk_0868559C[];            /* link "Wait..." sprites (DrawLinkWaitIndicator) and related
                                               graphics, 64 tiles */
extern const u8 gButtonIconsGfx[];          /* A and B button press frames, 16x16 each, 40 tiles */
extern const u8 gYesNoLabelsGfx[];          /* 'YES' and 'NO' labels, 32x16 each, 16 tiles */
extern const u8 gDuelUiIconsGfx[];          /* small marker icons and the right arrow, 8 tiles */
/* BG graphics (LoadDuelBgGfx) */
extern const u8 gUnk_0867B97C[];            /* thick card pile (a deck of more than 5 cards), 4x4 tiles */
extern const u8 gUnk_08684EFC[];            /* mark for a monster zone held for a banished card: the first
                                               2x2 tiles of this sheet survive the digit load */
extern const u8 gDuelDigitsPal[];           /* BG palette 3, used by the digit tiles */
extern const u8 gDuelDigitsGfx[];           /* digits 0-9 in 6 colour sets, 60 tiles */
extern const u8 gPhaseIndicatorPal[];       /* BG palette 4, used by the 'LP' label and the phase letters */
extern const u8 gLpLabelGfx[];              /* the 'LP' label, 2 tiles */
extern const u8 gPhaseIndicatorGfx[];       /* phase letters D S M B M E: highlighted, then normal, 12 tiles */
extern const u8 gTextBoxPal[];              /* BG palette 8, used by the text box frame and text */
extern const u8 gTextBoxFrameGfx[];         /* the 3x3 text box frame, 9 tiles */
/* Info bar labels (DuelScreen_DrawCursorInfo) */
extern const u8 gStrMyFusionDeck[];         /* "My Fusion Deck:" */
extern const u8 gStrOpponentFusionDeck[];   /* "Opponent Fusion Feck" (typo in the ROM) */
extern const u8 gStrMyDeck[];               /* "My Deck:" */
extern const u8 gStrOpponentDeck[];         /* "Opponent Deck" */
extern const u8 gStrMyGraveyard[];          /* "My Graveyard:" */
extern const u8 gStrOpponentGraveyard[];    /* "Opponent Graveyard:" */
extern const u8 gStrMyRemovedCards[];       /* "My Removed Cards:" */
extern const u8 gStrOpponentRemovedCards[]; /* "Opponent Removed Cards:" */

/* VRAM and palette addresses of the duel graphics. */
#define TILE_SIZE               0x20                            /* bytes per 4bpp 8x8 tile */
#define TILES(n)                ((n) * TILE_SIZE)
#define PALETTE_SIZE            0x20                            /* bytes per 16-colour palette */
#define BG_TILE_ADDR(n)         (VRAM + 0x4000 + TILES(n))      /* BG character block 1 */
#define OBJ_TILE_ADDR(n)        (OBJ_VRAM0 + TILES(n))
#define BG_PLTT_ADDR(n)         (BG_PLTT + (n) * PALETTE_SIZE)
#define OBJ_PLTT_ADDR(n)        (OBJ_PLTT + (n) * PALETTE_SIZE)

/*
 * BG tile numbers loaded by LoadDuelBgGfx. The BG2 field cells use them with a palette in bits 12-15
 * (FIELD_TILE_CARD_BACK 0x1070, FIELD_TILE_THICK_PILE 0x1230, FIELD_TILE_HELD_ZONE_MARK 0x1240 in
 * duel_screen.h).
 */
#define BG_TILE_CARD_BACK       0x070
#define BG_TILE_CARD_ICON_NORMAL 0x0B0  /* 64 tiles per frame colour: Normal, Effect, Fusion, Ritual, Magic, Trap */
#define BG_TILE_THICK_PILE      0x230
#define BG_TILE_HELD_ZONE_MARK  0x240   /* loaded as 16 tiles; the digit load overwrites all but the first 4 */
#define BG_TILE_DIGITS          0x244   /* digits 0-9; a colour set is 10 tiles, 6 sets */
#define BG_TILE_LP_LABEL        0x280
#define BG_TILE_PHASE_LETTERS   0x282   /* highlighted D S M B M E, then the normal ones from 0x288 */
#define BG_TILE_TEXT_FRAME      0x2CE   /* 3x3 text box frame; the centre (0x2D2) is the blank tile */
#define BG_TILE_BOX_TEXT        0x2D7   /* first of the 216 tiles of gTextBox.tiles (TextBoxDrawText) */

/*
 * OBJ tile numbers loaded by LoadDuelUiGfx, with their OBJ palette. The three marker sheets are the enum
 * ZoneMarkerTile of duel_screen.h (palette 5).
 */
#define OBJ_TILE_HAND_CURSOR    0x000   /* palette 0 */
#define OBJ_TILE_CARD_BACK      0x040   /* palette 0 */
#define OBJ_TILE_CARD_ICON_NORMAL 0x080 /* palette 1; 64 tiles per frame colour: Normal, Effect, Fusion, Ritual,
                                           Magic, Trap */
#define OBJ_TILE_COMMAND_ICONS  0x224   /* palette 2; card command menu, 4 tiles per command */
#define OBJ_TILE_COMMAND_LABELS 0x264   /* palette 3; card command menu, 8 tiles per command */
#define OBJ_TILE_BUTTON_ICONS   0x2E4   /* palette 4: A and B press frames, 4 tiles each */
#define OBJ_TILE_YES_LABEL      0x30C   /* palette 4 */
#define OBJ_TILE_NO_LABEL       0x314   /* palette 4 */
#define OBJ_TILE_UI_ICONS       0x31C   /* palette 4: the right arrow is the fourth tile */
#define OBJ_TILE_CHOICE_ARROW   (OBJ_TILE_UI_ICONS + 3)
#define OBJ_TILE_WAIT_SPRITES   0x324   /* palette 6 (DrawLinkWaitIndicator) */

/* ---- Text box constants ---- */

#define TEXTBOX_MIN_WIDTH       5       /* text size limits in cells (TextBoxDrawText clamps) */
#define TEXTBOX_MAX_WIDTH       24
#define TEXTBOX_MIN_HEIGHT      2
#define TEXTBOX_MAX_HEIGHT      11
#define TEXTBOX_TILE_COUNT      216     /* gTextBox.tiles holds this many 8x8 tiles */
#define TEXTBOX_LINE_HEIGHT     12      /* pixels per text line */
#define TEXTBOX_SJIS_ADVANCE    10      /* pixels per 2-byte character (font size 10) */
#define TEXTBOX_LATIN_ADVANCE   5       /* pixels per Latin character in the word-wrap estimate */
#define TEXTBOX_FONT_SIZE_10    0xA00   /* sizeColor argument of the TextDraw*Glyph functions: size << 8 */
#define TEXTBOX_COLOR_SHADOW    9       /* gTextBoxColors index of the drop shadow */
#define TEXTBOX_COLOR_DEFAULT   1       /* gTextBoxColors index used until a '@d' code (and for '@0') */
#define TEXTBOX_MAP_PALETTE     0x8000  /* BG palette 8 in a map entry */
#define TEXTBOX_FRAME_MAP_TILE  (TEXTBOX_MAP_PALETTE | BG_TILE_TEXT_FRAME)
#define TEXTBOX_TEXT_MAP_TILE   (TEXTBOX_MAP_PALETTE | BG_TILE_BOX_TEXT)
#define BG3_MAP_BLOCK           5       /* screen block of the BG3 map: the text box */
#define TEXTBOX_MAP             (gMain.bgMapBuffer[BG3_MAP_BLOCK])
#define MAP_ROW_STRIDE          0x20    /* map entries per BG map row */
#define SCREEN_LAST_ROW         0x13    /* the screen has 20 tile rows, 0-19 */

/* The nine tiles of the text box frame, as offsets from BG_TILE_TEXT_FRAME. */
enum TextBoxFrameTile {
    FRAME_TOP_LEFT = 0,
    FRAME_TOP = 1,
    FRAME_TOP_RIGHT = 2,
    FRAME_LEFT = 3,
    FRAME_CENTER = 4,               /* blank: TextBoxClearTiles fills the text tiles with it */
    FRAME_RIGHT = 5,
    FRAME_BOTTOM_LEFT = 6,
    FRAME_BOTTOM = 7,
    FRAME_BOTTOM_RIGHT = 8
};

/* Two-line choice menu: after A the chosen line blinks while gTextBox.menuTimer counts up to 60. */
#define CHOICE_BLINK_LAST       0x3B    /* the timer stops the blink after this value (60 frames) */
#define CHOICE_BLINK_SKIP       7       /* holding B (or fast mode) adds this much per frame ... */
#define CHOICE_BLINK_SKIP_LIMIT 0x33    /* ... while the timer is at most this */

/* attr2 of the sprites the text box draws: OBJ palette 4 (button icons, YES/NO labels, arrow). */
#define TEXTBOX_OBJ_ATTR2(tile) (OAM_ATTR2_PALETTE(4) | (tile))

/* Info bar */
#define INFO_LABEL_GLYPH_WIDTH  10      /* glyph width argument of DuelInfo_DrawLabelNumber */

/* Life points and phase letters (BG2 map, screen block 3 of gMain.bgMapBuffer; cell = row * 32 + column). */
#define BG2_MAP_BLOCK           3
#define LP_CELL_PLAYER0         0x267   /* row 19, column 7 */
#define LP_CELL_PLAYER1         0x294   /* row 20, column 20 */
#define LP_DIGITS               5       /* LP number width in cells */
#define LP_DIGIT_MAP_TILE       (OAM_ATTR2_PALETTE(3) | BG_TILE_DIGITS)     /* BG palette 3 */
#define DIGIT_COLOR_SET_TILES   10      /* tiles per colour set of the digit sheet */
#define PHASE_CELL_PLAYER0      0x286   /* row 20, column 6 */
#define PHASE_CELL_PLAYER1      0x273   /* row 19, column 19 */
#define PHASE_LETTER_COUNT      6       /* D S M B M E */
#define PHASE_MAP_PALETTE       OAM_ATTR2_PALETTE(4)    /* BG palette 4 */
#define PHASE_NORMAL_TILE       (BG_TILE_PHASE_LETTERS + PHASE_LETTER_COUNT)
#define PHASE_HIGHLIGHT_SHIFT   PHASE_LETTER_COUNT      /* the highlighted tile is 6 below the normal one */

/* Field background: a 4x4-tile texture repeated over the 32x32 BG0 map (BG palette 5, tiles 0x60-0x6F). */
#define BG0_MAP_BLOCK           0       /* screen block of the BG0 map: the field background */
#define FIELD_BG_PAL_START      0x50    /* first palette colour: palette 5 */
#define FIELD_BG_TILE_BASE      0x60
#define FIELD_BG_MAP_TILE       (OAM_ATTR2_PALETTE(5) | FIELD_BG_TILE_BASE)
#define FIELD_BG_BLOCK          4       /* the texture is 4 x 4 tiles */
#define BG_MAP_SIZE             0x20    /* a BG map is 32 x 32 entries */

/*
 * Matching: the zones are addressed with explicit byte arithmetic, in the operand order below (player offset
 * first for DUEL_ZONE, zone offset first for SPELL_TRAP_ZONE). gDuelZones[player].zones[zone] and a base
 * of zone 5 or 10 through gDuelZones compile to other code; the ROM loads the alias symbols
 * gDuelSpellTrapZones (zone 5 of player 0) and gDuelFieldZone (zone 10) from their own literals.
 */
#define DUEL_ZONE(player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))
#define SPELL_TRAP_ZONE(player, index) \
    ((struct DuelZone *)(((index) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer)) + (u32)&gDuelSpellTrapZones))
#define FIELD_ZONE(player) \
    ((struct DuelZone *)((player) * sizeof(struct DuelPlayer) + (u32)&gDuelFieldZone))

/*
 * Redraws the info bar (the two text rows at the bottom of the screen) for what the field cursor is on:
 * (selPlayer, selArea, selIndex) in gDuelScreen, with selArea an enum DuelArea. A card is described only if it
 * is the human player's own, or the opponent's and face up (in the hand: IsHandRevealed returns 1); otherwise
 * the bar stays blank. The four piles print their label and card count ("My Deck: 35"); the opponent's deck
 * and fusion deck show the label alone, so their sizes stay hidden.
 */
void DuelScreen_DrawCursorInfo(void)
{
    u16 cardId;
    u32 player;                     /* selPlayer: 0 = human, 1 = opponent */
    u32 area;                       /* selArea: enum DuelArea */
    u32 index;                      /* selIndex: zone within the row, or the hand index */
    u32 playerCopy;
    u32 areaPlusIndex;
    int isVisible;                  /* the card may be shown: own card, or the opponent's face-up card */

    cardId = DuelCursor_GetCardId();
    player = gDuelScreen.selPlayer;
    area = gDuelScreen.selArea;
    index = gDuelScreen.selIndex;
    /* Matching: the ROM keeps copies of selPlayer and selArea + selIndex in callee-saved registers, computed
     * before TextCellsClear; only the monster-row case uses them (the other cases recompute area + index). */
    playerCopy = player;
    areaPlusIndex = area + index;
    TextCellsClear();
    switch (area) {
    case DUEL_AREA_MONSTER:
        if (player != 0)
            isVisible = DUEL_ZONE(player & 1, index)->isFaceUp;
        else
            isVisible = 1;
        if (cardId && isVisible)
            DuelInfo_DrawMonsterZone(playerCopy, areaPlusIndex);
        break;
    case DUEL_AREA_SPELL_TRAP:
        /* selIndex counts within the spell/trap row (zone 5 + selIndex) */
        if (player != 0)
            isVisible = SPELL_TRAP_ZONE(player & 1, index)->isFaceUp;
        else
            isVisible = 1;
        if (cardId && isVisible)
            DuelInfo_DrawSpellZone(cardId, 1, player, area + index);
        break;
    case DUEL_AREA_FIELD:
        if (player != 0)
            isVisible = FIELD_ZONE(player & 1)->isFaceUp;
        else
            isVisible = 1;
        if (cardId && isVisible)
            DuelInfo_DrawSpellZone(cardId, 1, player, area + index);
        break;
    case DUEL_AREA_HAND:
        if (player != 0)
            isVisible = (u16)IsHandRevealed(player);
        else
            isVisible = 1;
        if (cardId && isVisible)
            DuelInfo_DrawCard(cardId, 1);
        break;
    case DUEL_AREA_FUSION_DECK:
        switch (player) {
        case 0: {
            const u8 *label = gStrMyFusionDeck;
            u8 count = gDuelPlayers[player].fusionCount;
            DuelInfo_DrawLabelNumber(INFO_LABEL_GLYPH_WIDTH, label, count, StrLen(label));
            break;
        }
        case 1:
            DuelInfo_DrawLabelNumber(INFO_LABEL_GLYPH_WIDTH, gStrOpponentFusionDeck, 0, 0);
            break;
        }
        break;
    case DUEL_AREA_DECK:
        switch (player) {
        case 0: {
            const u8 *label = gStrMyDeck;
            u8 count = gDuelPlayers[player].deckCount;
            DuelInfo_DrawLabelNumber(INFO_LABEL_GLYPH_WIDTH, label, count, StrLen(label));
            break;
        }
        case 1:
            DuelInfo_DrawLabelNumber(INFO_LABEL_GLYPH_WIDTH, gStrOpponentDeck, 0, 0);
            break;
        }
        break;
    case DUEL_AREA_GRAVEYARD:
        switch (player) {
        case 0: {
            const u8 *label = gStrMyGraveyard;
            u8 count = gDuelPlayers[player].graveCount;
            DuelInfo_DrawLabelNumber(INFO_LABEL_GLYPH_WIDTH, label, count, StrLen(label));
            break;
        }
        case 1: {
            const u8 *label = gStrOpponentGraveyard;
            u8 count = gDuelPlayers[player].graveCount;
            DuelInfo_DrawLabelNumber(INFO_LABEL_GLYPH_WIDTH, label, count, StrLen(label));
            break;
        }
        }
        break;
    case DUEL_AREA_BANISHED:
        switch (player) {
        case 0: {
            const u8 *label = gStrMyRemovedCards;
            u8 count = gDuelPlayers[player].banishedCount;
            DuelInfo_DrawLabelNumber(INFO_LABEL_GLYPH_WIDTH, label, count, StrLen(label));
            break;
        }
        case 1: {
            const u8 *label = gStrOpponentRemovedCards;
            u8 count = gDuelPlayers[player].banishedCount;
            DuelInfo_DrawLabelNumber(INFO_LABEL_GLYPH_WIDTH, label, count, StrLen(label));
            break;
        }
        }
        break;
    }
}

/*
 * Draw callback of the two-line choice menu (TEXTBOX_MENU_TWO_CHOICE): an 8x8 arrow sprite left of the line
 * gTextBox.result (12 pixels per line), following the box while it slides. Once a line is confirmed the arrow
 * blinks (menuTimer bit 1).
 */
void TextBoxDrawChoiceCursor(void)
{
    struct TextBox *box = &gTextBox;
    u32 x = (box->x + 1) * 8;
    /* the line's y with the box fully shown: text row y is on screen row y + 2 */
    int y = (box->y + 2) * 8 + box->result * TEXTBOX_LINE_HEIGHT - 2;
    /* minus the rows the box still has to slide in (0 once revealRow = y + height + 2) */
    y -= (box->height + box->y - box->revealRow + 2) * 8;
    if (box->menuState == TEXTBOX_MENU_STATE_CONFIRMED) {
        if (box->menuTimer & 2)
            AddSprite((y << 16) | x, SPRITE_SHAPE_8x8, TEXTBOX_OBJ_ATTR2(OBJ_TILE_CHOICE_ARROW));
    } else {
        AddSprite((y << 16) | x, SPRITE_SHAPE_8x8, TEXTBOX_OBJ_ATTR2(OBJ_TILE_CHOICE_ARROW));
    }
}


/*
 * Input callback of the two-line choice menu; returns 1 once the choice is made and has blinked. In state
 * SELECT, Up and Down toggle gTextBox.result between line 0 and line 1 (SE_CURSOR), A confirms (SE_CONFIRM;
 * state CONFIRMED, timer 0) and B only plays SE_ERROR: the menu cannot be cancelled. In state CONFIRMED the
 * chosen line blinks for 60 frames (see CHOICE_BLINK_*), then the state becomes DONE and the next call
 * returns 1.
 */
u16 TextBoxHandleChoiceInput(void)
{
    u32 menuState = gTextBox.menuState;
    /* FAKEMATCH: a dead byte copy of menuState steers register allocation of the switch value */
    u8 menuStateCopy;
    switch ((u8)menuState) {
    case TEXTBOX_MENU_STATE_CONFIRMED:
        if (gTextBox.menuTimer <= CHOICE_BLINK_LAST) {
            gTextBox.menuTimer++;
            menuStateCopy = menuState;
            /* Holding B, or fast mode, skips ahead. menuStateCopy is CONFIRMED (1) in this case, which is also
             * the mask of gDuelScreen.fast, bit 0 of its first byte. */
            if ((gMain.heldKeys & B_BUTTON) || (gDuelScreenFlags & menuStateCopy)) {
                if (gTextBox.menuTimer <= CHOICE_BLINK_SKIP_LIMIT)
                    gTextBox.menuTimer += CHOICE_BLINK_SKIP;
            }
        } else
            gTextBox.menuState = menuState + 1;     /* DONE */
        break;
    case TEXTBOX_MENU_STATE_DONE:
        return 1;
    default:
        if (gMain.newKeys & DPAD_DOWN) {
            PlaySE(SE_CURSOR);
            gTextBox.result = 1 - gTextBox.result;
        }
        if (gMain.newKeys & DPAD_UP) {
            PlaySE(SE_CURSOR);
            gTextBox.result = 1 - gTextBox.result;
        }
        if (gMain.newKeys & A_BUTTON) {
            PlaySE(SE_CONFIRM);
            gTextBox.menuState = TEXTBOX_MENU_STATE_CONFIRMED;
            gTextBox.menuTimer = 0;
        }
        if (gMain.newKeys & B_BUTTON)
            PlaySE(SE_ERROR);
        break;
    }
    return 0;
}

/*
 * Fills the 216 text tiles of gTextBox with the blank centre tile of the frame (copied from VRAM) and marks
 * them for upload (DuelScreen_Update copies them to VRAM at BG tile 0x2D7).
 */
void TextBoxClearTiles(void)
{
    u8 *tile = gTextBox.tiles;
    int i;
    for (i = 0; i < TEXTBOX_TILE_COUNT; i++) {
        CopyDoubleWords(tile, (void *)BG_TILE_ADDR(BG_TILE_TEXT_FRAME + FRAME_CENTER), TILE_SIZE);
        tile += TILE_SIZE;
    }
    gTextBox.tilesDirty = 1;
}

/*
 * Sets the box rectangle and renders `text` into gTextBox.tiles. pos = x | y << 8 and size = width | height <<
 * 8, in cells; the size is clamped to 5-24 x 2-11. Text codes: '\n' starts a new line, '@d' (d = 0-9)
 * selects colour gTextBoxColors[d] ('@0' is the default colour 1). Every glyph is drawn twice, first as a
 * shadow one pixel right and down. In Shift-JIS mode (gSaveData +4 bit 7) each character is 2 bytes and
 * 10 pixels wide; otherwise Latin characters are drawn 5 pixels apart and a word that does not fit is wrapped.
 */
void TextBoxDrawText(u16 pos, u16 size, const u8 *text)
{
    int color = TEXTBOX_COLOR_DEFAULT;
    int penX = 0;
    int penY = 0;
    gTextBox.x = (u8)pos;
    gTextBox.y = pos >> 8;
    gTextBox.width = (u8)size;
    gTextBox.height = size >> 8;
    if (gTextBox.width > TEXTBOX_MAX_WIDTH)
        gTextBox.width = TEXTBOX_MAX_WIDTH;
    if (gTextBox.height > TEXTBOX_MAX_HEIGHT)
        gTextBox.height = TEXTBOX_MAX_HEIGHT;
    if (gTextBox.width < TEXTBOX_MIN_WIDTH)
        gTextBox.width = TEXTBOX_MIN_WIDTH;
    if (gTextBox.height < TEXTBOX_MIN_HEIGHT)
        gTextBox.height = TEXTBOX_MIN_HEIGHT;
    TextBoxClearTiles();
    TextCanvasInit(gTextBox.width, gTextBox.height);
    while (*text != '\0') {
        switch (*text) {
        case '@':
            /* colour code: '@' followed by a digit */
            if ((u8)(text[1] - '0') <= 9) {
                color = text[1] - '0';
                if (color == 0)
                    color = TEXTBOX_COLOR_DEFAULT;
                text++;
            }
            break;
        case '\n':
            penX = 0;
            penY += TEXTBOX_LINE_HEIGHT;
            break;
        default:
            if (gSaveDataBytes[4] & SAVE_FLAGS4_SJIS_TEXT) {
                /* 2-byte character: wrap when there is no room for another one */
                if (penX + TEXTBOX_SJIS_ADVANCE >= gTextBox.width * 8) {
                    penX = 0;
                    penY += TEXTBOX_LINE_HEIGHT;
                }
                TextDrawSjisGlyph((*text << 8) | text[1], penX + 1, penY + 1,
                                  (u8)gTextBoxColors[TEXTBOX_COLOR_SHADOW] | TEXTBOX_FONT_SIZE_10);
                TextDrawSjisGlyph((*text << 8) | text[1], penX, penY, (u8)gTextBoxColors[color] | TEXTBOX_FONT_SIZE_10);
                penX += TEXTBOX_SJIS_ADVANCE;
                text++;
            } else {
                /* Latin: wrap before a word that would run past the right edge */
                if (TextWordLengthInt(text) * TEXTBOX_LATIN_ADVANCE + penX > gTextBox.width * 8) {
                    penX = 0;
                    penY += TEXTBOX_LINE_HEIGHT;
                }
                TextDrawLatinGlyph(*text, penX + 1, penY + 1, (u8)gTextBoxColors[TEXTBOX_COLOR_SHADOW] | TEXTBOX_FONT_SIZE_10);
                TextDrawLatinGlyph(*text, penX, penY, (u8)gTextBoxColors[color] | TEXTBOX_FONT_SIZE_10);
                penX += TEXTBOX_LATIN_ADVANCE;
            }
            break;
        }
        text++;
    }
    /* gTextBoxColors[0] (read as a halfword) is the background colour of the tiles */
    TextCanvasToTiles((u16 *)gTextBox.tiles, *(u16 *)gTextBoxColors);
}

/*
 * Writes the framed box into the BG3 map (TEXTBOX_MAP) so that it ends just above map row `row`: a blank row,
 * the top border, gTextBox.height text rows (a left border, width text tiles, a right border), the bottom
 * border and a blank row. The frame starts one column left of gTextBox.x. When the box is fully shown
 * (row = y + height + 2) the top border is on map row y - 1; while it slides, the blank rows erase the
 * previous position. Rows are clipped to the screen: screenRow is the map row + 2 (the same offset that
 * TextBoxDrawChoiceCursor applies to y) and must be 0-19. The text tiles are numbered row by row from
 * BG_TILE_BOX_TEXT.
 */
void TextBoxDrawTilemap(u32 row)
{
    u32 top = row - gTextBox.height;
    u32 screenRow = top - 2;
    u16 cell;
    u32 start;
    int i;                                      /* column in the border rows, row in the text rows */
    int col;                                    /* column in a text row */
    int frame;
    u16 textTile;
    start = gTextBox.x + 0xFFFF;                /* column x - 1 */
    start += (top - 4) * MAP_ROW_STRIDE;        /* first map row: the blank row above the box */
    cell = start;
    frame = TEXTBOX_FRAME_MAP_TILE;
    textTile = TEXTBOX_TEXT_MAP_TILE;
    /* blank row above the box */
    if (screenRow <= SCREEN_LAST_ROW) {
        for (i = 0; i < gTextBox.width + 2; i++)
            TEXTBOX_MAP[cell + i] = 0;
    }
    screenRow++;
    cell += MAP_ROW_STRIDE;
    /* top border */
    if (screenRow <= SCREEN_LAST_ROW) {
        TEXTBOX_MAP[cell] = frame + FRAME_TOP_LEFT;
        for (i = 1; i <= gTextBox.width; i++)
            TEXTBOX_MAP[cell + i] = frame + FRAME_TOP;
        TEXTBOX_MAP[cell + gTextBox.width + 1] = frame + FRAME_TOP_RIGHT;
    }
    screenRow++;
    cell += MAP_ROW_STRIDE;
    /* text rows */
    for (i = 0; i < gTextBox.height; i++) {
        if (screenRow <= SCREEN_LAST_ROW) {
            TEXTBOX_MAP[cell] = frame + FRAME_LEFT;
            for (col = 1; col <= gTextBox.width; col++)
                TEXTBOX_MAP[cell + col] = textTile++;
            TEXTBOX_MAP[cell + gTextBox.width + 1] = frame + FRAME_RIGHT;
        }
        screenRow++;
        cell += MAP_ROW_STRIDE;
    }
    /* bottom border */
    if (screenRow <= SCREEN_LAST_ROW) {
        TEXTBOX_MAP[cell] = frame + FRAME_BOTTOM_LEFT;
        for (i = 1; i <= gTextBox.width; i++)
            TEXTBOX_MAP[cell + i] = frame + FRAME_BOTTOM;
        TEXTBOX_MAP[cell + gTextBox.width + 1] = frame + FRAME_BOTTOM_RIGHT;
    }
    screenRow++;
    cell += MAP_ROW_STRIDE;
    /* blank row below the box */
    if (screenRow <= SCREEN_LAST_ROW) {
        for (i = 0; i < gTextBox.width + 2; i++)
            TEXTBOX_MAP[cell + i] = 0;
    }
}

/*
 * One frame of the box sliding in: draws the box down to gTextBox.revealRow, then advances the row. Returns 1
 * once the row has reached y + height + 2 (the box is fully shown), else 0.
 */
int TextBoxSlideIn(void)
{
    struct TextBox *box = &gTextBox;
    u8 *revealRow = &box->revealRow;
    TextBoxDrawTilemap(*revealRow);
    if (*revealRow < box->height + box->y + 2) {
        (*revealRow)++;
        return 0;
    }
    return 1;
}

/* One frame of the box sliding out: moves revealRow up one row and redraws. Returns 1 when it reaches 0 (gone). */
int TextBoxSlideOut(void)
{
    struct TextBox *box = &gTextBox;
    u8 *revealRow = &box->revealRow;
    TextBoxDrawTilemap(--*revealRow);
    if (*revealRow == 0)
        return 1;
    return 0;
}

/*
 * Default input of the box (no inputCallback). Message: A closes it (SE_CANCEL). Yes/No: A answers with the
 * highlighted label (SE_CONFIRM; result 1 = YES), B answers No (SE_CANCEL, result 0), Left/Right move the
 * highlight. Returns 1 once the box is answered.
 */
u16 TextBoxHandleInput(void)
{
    struct TextBox *box = &gTextBox;
    switch (box->menuKind) {
    case TEXTBOX_MENU_MESSAGE:
        if (gMain.newKeys & A_BUTTON) {
            PlaySE(SE_CANCEL);
            return 1;
        }
        break;
    case TEXTBOX_MENU_YES_NO:
        if (gMain.newKeys & A_BUTTON) {
            PlaySE(SE_CONFIRM);
            switch (box->yesNoCursor) {
            case 0:                 /* YES highlighted */
                box->result = 1;
                break;
            case 1:                 /* NO highlighted */
                box->result = 0;
                break;
            }
            return 1;
        }
        if (gMain.newKeys & B_BUTTON) {
            PlaySE(SE_CANCEL);
            box->result = 0;
            return 1;
        }
        if (gMain.newKeys & (DPAD_RIGHT | DPAD_LEFT))
            box->yesNoCursor = 1 - box->yesNoCursor;
        break;
    }
    return 0;
}

/*
 * Default sprite callback of the box (no drawCallback). Message: the animated A-button icon at the lower right
 * of the box. Yes/No: the YES and NO labels as affine sprites left and right of the box centre, the
 * highlighted one pulsing (gPulseScaleCurve), and the pointing hand under it.
 */
void TextBoxDrawSprites(void)
{
    struct TextBox *box = &gTextBox;
    u32 pos;
    u32 y;
    u32 scale;
    switch (box->menuKind) {
    case TEXTBOX_MENU_MESSAGE:
        pos = (box->x + box->width - 2) * 8;
        pos |= (box->revealRow - 4) << 19;
        AddSprite(pos, SPRITE_SHAPE_16x16,
                  (u16)(gAButtonIconFrames[gMain.frameCounter >> 2 & 0x1F] * 4 + TEXTBOX_OBJ_ATTR2(OBJ_TILE_BUTTON_ICONS)));
        break;
    case TEXTBOX_MENU_YES_NO:
        /* YES label, 0x30 pixels left of the centre */
        pos = (box->x + (box->width >> 1)) * 8;
        pos -= 0x30;
        y = (box->revealRow - 4) * 8;
        if (box->yesNoCursor == 0)
            scale = gPulseScaleCurve[gMain.frameCounter >> 2 & 0xF];
        else
            scale = 0x100;
        pos |= y << 16;
        AddAffineSprite(pos, SPRITE_SHAPE_32x16, TEXTBOX_OBJ_ATTR2(OBJ_TILE_YES_LABEL), scale << 16);
        /* NO label, 0x10 pixels right of the centre */
        box = &gTextBox;
        pos = (box->x + (box->width >> 1)) * 8;
        pos += 0x10;
        if (box->yesNoCursor == 1)
            scale = gPulseScaleCurve[gMain.frameCounter >> 2 & 0xF];
        else
            scale = 0x100;
        pos |= y << 16;
        AddAffineSprite(pos, SPRITE_SHAPE_32x16, TEXTBOX_OBJ_ATTR2(OBJ_TILE_NO_LABEL), scale << 16);
        /* pointing hand, 8 pixels below the selected label (the labels are 0x40 apart) */
        box = &gTextBox;
        pos = (box->x + (box->width >> 1)) * 8;
        pos -= 0x30;
        pos += box->yesNoCursor * 0x40;
        y += 8;
        pos |= y << 16;
        AddSprite(pos, SPRITE_SHAPE_32x32, OBJ_TILE_HAND_CURSOR);
        break;
    }
}

/*
 * Opens the text box: sets the rectangle and renders `text` (TextBoxDrawText), stores the TextBoxFlags and
 * resets the menu to a plain message with the default callbacks, step 0 and result 0. yesNoCursor is kept,
 * so the Yes/No highlight remembers the last answer. Hides the field cursor. TextBoxUpdate then runs the box.
 * pos = x | y << 8 and size = width | height << 8, in cells.
 */
void TextBoxOpen(u16 pos, u16 size, u16 flags, const u8 *text)
{
    struct TextBox *box = &gTextBox;
    box->unk2 = 0;
    TextBoxDrawText(pos, size, text);
    box->flags = flags;
    box->menuKind = TEXTBOX_MENU_MESSAGE;
    box->drawCallback = NULL;
    box->inputCallback = NULL;
    box->step = TEXTBOX_STEP_SLIDE_IN;
    box->revealRow = 0;
    box->menuState = TEXTBOX_MENU_STATE_SELECT;
    box->menuTimer = 0;
    box->result = 0;
    box->active = 1;
    gDuelScreen.showCursor = 0;
}

/*
 * Selects the menu of an open box (enum TextBoxMenuKind). MESSAGE and YES_NO use the default callbacks
 * (both pointers cleared), TWO_CHOICE the choice-menu ones; any other kind (CUSTOM) keeps the callbacks
 * passed in.
 */
void TextBoxSetMenu(u16 menuKind, void (*drawCallback)(void), u16 (*inputCallback)(void))
{
    struct TextBox *box = &gTextBox;
    box->menuKind = menuKind;
    box->drawCallback = drawCallback;
    box->inputCallback = inputCallback;
    switch (box->menuKind) {
    case TEXTBOX_MENU_MESSAGE:
    case TEXTBOX_MENU_YES_NO:
        box->drawCallback = NULL;
        box->inputCallback = NULL;
        break;
    case TEXTBOX_MENU_TWO_CHOICE:
        box->drawCallback = TextBoxDrawChoiceCursor;
        box->inputCallback = TextBoxHandleChoiceInput;
        break;
    }
}

/*
 * Per-frame driver of the box, called by DuelMainStep while it is active. gTextBox.step runs through
 * TextBoxStep: SLIDE_IN (TextBoxSlideIn, if flags has TEXTBOX_FLAG_SLIDE_IN), INPUT (the input callback or
 * TextBoxHandleInput, if flags has WAIT_INPUT), SLIDE_OUT (TextBoxSlideOut, if SLIDE_OUT). A step moves on
 * when its routine returns nonzero, or at once when its flag is clear; after the last step the box is
 * deactivated. Note that gTextBox.flags is tested, not the key state. Returns 1 while the box is active
 * (including the frame it closes), else 0.
 *
 * FAKEMATCH: initialized step r2, callback r1, and next-step r0 retain the target's switch copy and callback
 * ABI registers. The next-step input and two empty clobbers preserve distinct case store tails; no code is
 * emitted.
 */
int TextBoxUpdate(void)
{
    struct TextBox *box = &gTextBox;
    u8 *stepPtr;
    register u32 step __asm__("r2");
    int stepSwitch;
    register u16 (*inputCb)(void) __asm__("r1");
    register u32 nextStep __asm__("r0");
    if (box->active) {
        stepPtr = &box->step;
        step = *stepPtr;
        stepSwitch = step;
        switch (stepSwitch) {
        case TEXTBOX_STEP_SLIDE_IN:
            if (box->flags & TEXTBOX_FLAG_SLIDE_IN) {
                if (!TextBoxSlideInU16())
                    break;
                nextStep = *stepPtr + 1;
            } else
                nextStep = step + 1;
            *stepPtr = nextStep;
            break;
        case TEXTBOX_STEP_INPUT:
            if (box->flags & TEXTBOX_FLAG_WAIT_INPUT) {
                inputCb = box->inputCallback;
                if (inputCb) {
                    if (!inputCb())
                        break;
                    nextStep = *stepPtr + 1;
                    goto menu_store;
                } else {
                    if (!TextBoxHandleInput())
                        break;
                }
                nextStep = *stepPtr + 1;
            } else
                nextStep = step + 1;
            __asm__ volatile("" : : "r"(nextStep));
        menu_store:
            *stepPtr = nextStep;
            __asm__ volatile("" : : : "r3");
            break;
        case TEXTBOX_STEP_SLIDE_OUT:
            /* stepSwitch is TEXTBOX_FLAG_SLIDE_OUT (2) here too; the ROM tests the switch value itself */
            if (box->flags & stepSwitch) {
                if (!TextBoxSlideOutU16())
                    break;
                nextStep = *stepPtr + 1;
            } else
                nextStep = step + 1;
            *stepPtr = nextStep;
            __asm__ volatile("" : : : "r1");
            break;
        default:
            box->active = 0;
            break;
        }
        return 1;
    }
    return 0;
}


/* VBlank callback of the duel screen: scrolls the BG0 field background one pixel diagonally per frame. */
void DuelScreen_VBlank(void)
{
    int scroll = gDuelScreen.fieldBgScroll - 1;
    gDuelScreen.fieldBgScroll = scroll;
    REG_BG0VOFS = scroll;
    REG_BG0HOFS = scroll;
}

/*
 * Sets the four BG control registers: BG0 field background (priority 3, character block 1, screen block 0),
 * BG1 board (priority 2, screen block 1, 256x512), BG2 card cells, life points and phase letters (priority 1,
 * screen block 3, 256x512), BG3 text box (priority 0, screen block 5).
 */
void DuelScreen_InitBgCnt(void)
{
    REG_BG0CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(0);
    REG_BG1CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(1) | BGCNT_TXT256x512;
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(3) | BGCNT_TXT256x512;
    REG_BG3CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(5);
}

/*
 * Loads field background `background` (0 = the plain field, 1-14 = the field-magic textures) as a 32x32-pixel
 * image (4x4 tiles) and tiles it over the whole 32x32 BG0 map, then records the choice in
 * gDuelScreen.fieldBackground.
 */
void DuelScreen_LoadFieldBackground(u16 background)
{
    u16 blockRow;
    u16 blockCol;
    LoadBgImage4bpp(0, FIELD_BG_PAL_START, FIELD_BG_TILE_BASE, gFieldBackgroundImages[background]);
    for (blockRow = 0; blockRow <= BG_MAP_SIZE - 1; blockRow += FIELD_BG_BLOCK) {
        for (blockCol = 0; blockCol <= BG_MAP_SIZE - 1; blockCol += FIELD_BG_BLOCK) {
            /* the 16 entries of one copy of the texture: row r, column c = FIELD_BG_MAP_TILE + r * 4 + c */
            int b = blockRow * BG_MAP_SIZE + blockCol;
            gMain.bgMapBuffer[0][b + 0] = FIELD_BG_MAP_TILE + 0;
            gMain.bgMapBuffer[0][b + 1] = FIELD_BG_MAP_TILE + 1;
            gMain.bgMapBuffer[0][b + 2] = FIELD_BG_MAP_TILE + 2;
            gMain.bgMapBuffer[0][b + 3] = FIELD_BG_MAP_TILE + 3;
            gMain.bgMapBuffer[0][b + 0x20] = FIELD_BG_MAP_TILE + 4;
            gMain.bgMapBuffer[0][b + 0x21] = FIELD_BG_MAP_TILE + 5;
            gMain.bgMapBuffer[0][b + 0x22] = FIELD_BG_MAP_TILE + 6;
            gMain.bgMapBuffer[0][b + 0x23] = FIELD_BG_MAP_TILE + 7;
            gMain.bgMapBuffer[0][b + 0x40] = FIELD_BG_MAP_TILE + 8;
            gMain.bgMapBuffer[0][b + 0x41] = FIELD_BG_MAP_TILE + 9;
            gMain.bgMapBuffer[0][b + 0x42] = FIELD_BG_MAP_TILE + 10;
            gMain.bgMapBuffer[0][b + 0x43] = FIELD_BG_MAP_TILE + 11;
            gMain.bgMapBuffer[0][b + 0x60] = FIELD_BG_MAP_TILE + 12;
            gMain.bgMapBuffer[0][b + 0x61] = FIELD_BG_MAP_TILE + 13;
            gMain.bgMapBuffer[0][b + 0x62] = FIELD_BG_MAP_TILE + 14;
            gMain.bgMapBuffer[0][b + 0x63] = FIELD_BG_MAP_TILE + 15;
        }
    }
    gDuelScreen.fieldBackground = background;
}

/*
 * Loads the duel's OBJ graphics: switches OBJ tiles to 1D mapping, copies seven 16-colour palettes to OBJ
 * palettes 0-6 and the tile sheets to OBJ VRAM from tile 0 up to tile 0x364 (VRAM 0x06016C80), then sets
 * DuelScreen.uiGfxLoaded. The palette numbers are the ones the sprites' attr2 values select.
 */
void LoadDuelUiGfx(void)
{
    REG_DISPCNT |= DISPCNT_OBJ_1D_MAP;
    CopyDoubleWords((void *)OBJ_PLTT_ADDR(0), gHandCursorPal, PALETTE_SIZE);
    CopyDoubleWords((void *)OBJ_PLTT_ADDR(1), gCardIconPal, PALETTE_SIZE);
    CopyDoubleWords((void *)OBJ_PLTT_ADDR(5), gUnk_0868467C, PALETTE_SIZE);
    CopyDoubleWords((void *)OBJ_PLTT_ADDR(2), gUnk_0867FC3C, PALETTE_SIZE);
    CopyDoubleWords((void *)OBJ_PLTT_ADDR(3), gUnk_0868045C, PALETTE_SIZE);
    CopyDoubleWords((void *)OBJ_PLTT_ADDR(4), gUnk_0868167C, PALETTE_SIZE);
    CopyDoubleWords((void *)OBJ_PLTT_ADDR(6), gUnk_0868557C, PALETTE_SIZE);
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_HAND_CURSOR), gHandCursorGfx, TILES(64));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_CARD_BACK), gUnk_0867817C, TILES(64));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_CARD_ICON_NORMAL + 0x00), gCardIconNormalGfx, TILES(64));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_CARD_ICON_NORMAL + 0x40), gCardIconEffectGfx, TILES(64));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_CARD_ICON_NORMAL + 0x80), gCardIconFusionGfx, TILES(64));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_CARD_ICON_NORMAL + 0xC0), gCardIconRitualGfx, TILES(64));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_CARD_ICON_NORMAL + 0x100), gCardIconMagicGfx, TILES(64));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_CARD_ICON_NORMAL + 0x140), gCardIconTrapGfx, TILES(64));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(MARKER_TILE_CAN_ATTACK), gUnk_0868487C, TILES(12));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(MARKER_TILE_LINK_EQUIP), gUnk_086849FC, TILES(12));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(MARKER_TILE_LINK_OTHER), gUnk_08684B7C, TILES(12));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_COMMAND_ICONS), gUnk_0867FC5C, TILES(64));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_COMMAND_LABELS), gUnk_0868047C, TILES(128));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_BUTTON_ICONS), gButtonIconsGfx, TILES(40));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_YES_LABEL), gYesNoLabelsGfx, TILES(16));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_UI_ICONS), gDuelUiIconsGfx, TILES(8));
    CopyDoubleWords((void *)OBJ_TILE_ADDR(OBJ_TILE_WAIT_SPRITES), gUnk_0868559C, TILES(64));
    gDuelScreen.uiGfxLoaded = 1;
}

/*
 * Loads the duel's BG graphics: the system font, five BG palettes (1-4 and 8) and the tile sheets of
 * character block 1 from BG tile 0x70 up: card back, the mini-card icons, the thick pile and held-zone
 * marks, the LP digits, the 'LP' label, the phase letters and the text box frame.
 */
void LoadDuelBgGfx(void)
{
    LoadSystemGfx();
    CopyDoubleWords((void *)BG_PLTT_ADDR(1), gHandCursorPal, PALETTE_SIZE);
    CopyDoubleWords((void *)BG_PLTT_ADDR(2), gCardIconPal, PALETTE_SIZE);
    CopyDoubleWords((void *)BG_PLTT_ADDR(3), gDuelDigitsPal, PALETTE_SIZE);
    CopyDoubleWords((void *)BG_PLTT_ADDR(4), gPhaseIndicatorPal, PALETTE_SIZE);
    CopyDoubleWords((void *)BG_PLTT_ADDR(8), gTextBoxPal, PALETTE_SIZE);
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_CARD_BACK), gUnk_0867817C, TILES(64));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_CARD_ICON_NORMAL + 0x00), gCardIconNormalGfx, TILES(64));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_CARD_ICON_NORMAL + 0x40), gCardIconEffectGfx, TILES(64));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_CARD_ICON_NORMAL + 0x80), gCardIconFusionGfx, TILES(64));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_CARD_ICON_NORMAL + 0xC0), gCardIconRitualGfx, TILES(64));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_CARD_ICON_NORMAL + 0x100), gCardIconMagicGfx, TILES(64));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_CARD_ICON_NORMAL + 0x140), gCardIconTrapGfx, TILES(64));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_THICK_PILE), gUnk_0867B97C, TILES(16));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_HELD_ZONE_MARK), gUnk_08684EFC, TILES(16));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_DIGITS), gDuelDigitsGfx, TILES(60));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_LP_LABEL), gLpLabelGfx, TILES(2));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_PHASE_LETTERS), gPhaseIndicatorGfx, TILES(12));
    CopyDoubleWords((void *)BG_TILE_ADDR(BG_TILE_TEXT_FRAME), gTextBoxFrameGfx, TILES(9));
}

/*
 * Prints `value` as a 5-digit number into BG map block `screenBlock` of gMain.bgMapBuffer: the digits go
 * right to left from cell + 4, leading zeros are blank (a zero prints one '0'). The tile of digit d is
 * LP_DIGIT_MAP_TILE + colorSet * 10 + d (BG palette 3; colorSet picks one of the 6 colour sets of the sheet).
 */
void DrawBgNumber(u32 screenBlock, u32 cell, u32 colorSet, int value)
{
    int rest = value;
    u16 cellIndex = ((cell << 16) + 0x40000) >> 16;     /* the last digit's cell: (cell + 4) & 0xFFFF */
    int digit = 0;
    u16 *entry;

    /* FAKEMATCH: keep the value live separately from the tilemap cell address. */
    __asm__ __volatile__("" : : "r"(rest));
    for (; digit <= LP_DIGITS - 1; digit++) {
        entry = &gMain.bgMapBuffer[screenBlock][cellIndex];
        if (rest == 0 && digit > 0)
            *entry = rest;
        else {
            int tile = rest % 10 + LP_DIGIT_MAP_TILE;
            *entry = tile + colorSet * DIGIT_COLOR_SET_TILES;
        }
        rest /= 10;
        cellIndex--;
    }
}

/* Draws a player's life points in the LP box on BG2: player 0 at row 19, column 7; player 1 at row 20, column 20. */
void DrawLifePoints(int player, int lifePoints)
{
    switch (player) {
    case 0:
        DrawBgNumber(BG2_MAP_BLOCK, LP_CELL_PLAYER0, 0, lifePoints);
        break;
    case 1:
        DrawBgNumber(BG2_MAP_BLOCK, LP_CELL_PLAYER1, 0, lifePoints);
        break;
    }
}

/*
 * Draws both players' phase letters D S M B M E on BG2 (player 0 at row 20, columns 6-11; player 1 at row
 * 19, columns 19-24). The letter of `phase` for `turnPlayer` uses the highlighted tile, the others the normal
 * ones.
 */
void DrawPhaseIndicator(int turnPlayer, int phase)
{
    int player;
    int letter;
    for (player = 0; player <= 1; player++) {
        u16 cell;
        u16 firstCell = PHASE_CELL_PLAYER0;
        if (player != 0)
            firstCell -= PHASE_CELL_PLAYER0 - PHASE_CELL_PLAYER1;
        cell = firstCell;
        for (letter = 0; letter <= PHASE_LETTER_COUNT - 1; letter++) {
            int tile = PHASE_NORMAL_TILE;
            if (player == turnPlayer && letter == phase)
                tile -= PHASE_HIGHLIGHT_SHIFT;
            gMain.bgMapBuffer[BG2_MAP_BLOCK][cell] = tile + letter + PHASE_MAP_PALETTE;
            cell++;
        }
    }
}
