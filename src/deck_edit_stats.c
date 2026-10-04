/*
 * deck_edit_stats (0x0806C4E4-0x0806D51C): the Statistics sub-screen of Deck Edit and the other
 * card-list screens (wiki/functions/deck-edit-stats-c.md).
 *
 * DeckEdit_Init (step 0 of every card-list screen) lives here with the Statistics steps:
 * DeckStats_ClearState resets the sub-screen bytes of gDeckEdit, DeckStats_Init loads the
 * graphics and fades in, and DeckStats_Update scrolls the BG3 pattern, watches A/B and runs
 * the panel blend. DeckStats_Compute fills the seven statistics rows in gScratchBuffer
 * (struct DeckStatsRow[7]; row 6 is the SUM) from the counts of DeckStats_CountCategory over
 * the list shown (gDeckEdit.curList): Normal/Effect/Fusion monsters, Magic, Trap and Ritual
 * monsters, with rounded percentages. DeckStats_DrawNumbers draws the rows with the digit
 * sprites. GetCardCopiesInList reads a card's copies from gSaveData (trunk, saved deck or
 * side deck, the deck count including the fusion-deck copies).
 */
#include "global.h"
#include "card_data.h"            /* CARD_ID_MASK, CARD_STATS_TYPE, CARD_STATS_KIND, gCardStats, gCardIdToNumber */
#include "constants/card_stats.h" /* enum CardType */
#include "constants/cards.h"      /* CARD_OBELISK_THE_TORMENTOR, CARD_SLIFER_THE_SKY_DRAGON, CARD_THE_WINGED_DRAGON_OF_RA */
#include "constants/sound.h"      /* SE_CANCEL */
#include "legacy/gba.h"                  /* REG_DISPCNT, REG_BG0CNT..REG_BG3CNT, REG_BG*HOFS, REG_BG*VOFS, REG_BLDCNT,
                                       VRAM, OBJ_VRAM0, BG_PLTT, OBJ_PLTT, A_BUTTON, B_BUTTON, CpuFastSet, CpuSet */
#include "legacy/main.h"                 /* struct Main gMain (newKeys, seqState1, vblankFlags) */
#include "bg.h"                   /* CopyMapRect, CopyMapRectAddOffset, CopyTileSheetTo2D, CropMapBlock */
#include "deck_edit.h"            /* struct DeckEdit gDeckEdit, enum DeckEditList, enum DeckStatsCategory,
                                       struct DeckStatsRow, DeckEdit_BuildCardLists, DeckEdit_SetListCard,
                                       DeckEdit_CountSideDeckMonsters, DeckEdit_CalcScrollBar,
                                       gDeckEditDigitSprites, gDeckEditObjTiles, gDeckEditObjPal, gListFilterSortPageMap */
#include "palette.h"              /* struct Fade, enum FadeState, FadeStart, FadeTick, SetBldAlpha */
#include "save.h"                 /* struct SaveData gSaveData */
#include "sprite.h"               /* DrawNumberSprites, enum NumberSpriteMode, OamListFlush, OamListClear, ObjAffineInit */
#include "text.h"                 /* ClearKatakanaFlag */
#include "util.h"                 /* MemClear16, DivFix8, Ease_Init, gScratchBuffer */

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u16 seId);

/* Step table of the Statistics screen, run by DeckEdit_RunStatistics on gMain.seqState1. */
extern u16 (*const gDeckStatsSteps[])(void);   /* 0x081A724C */

/* gSaveData.trunk entry reached from the save base (the trunk starts at +8): save.h's
 * struct TrunkEntry without the tail flag byte. */
struct TrunkEntryView {
    u8 pad[8];
    u16 count : 10;                 /* copies in the trunk */
    u16 deckCopies : 2;             /* copies in the saved Deck */
    u16 sideCopies : 2;             /* copies in the saved Side Deck */
    u16 fusionCopies : 2;           /* copies in the saved Fusion Deck */
};

u16 DeckStats_ClearState(void)
{
    gDeckEdit.bgScrollY = 0;
    gDeckEdit.bgScrollX = 0;
    gDeckEdit.filterSel = 0;
    gDeckEdit.sortSel = 0;
    gDeckEdit.panelShown = 0;
    gDeckEdit.inputLock = 0;
    gDeckEdit.subPhase = 0;
    return 1;
}

/* Word arguments are explicitly narrowed, including the default selector
 * return preserved by the ROM. FAKEMATCH: the initialized selector binding
 * retains its original scratch register; no instructions are supplied. */
u32 GetCardCopiesInList(int listWord, int cardWord)
{
    int list = (u16)listWord;
    register int selector asm("r3") = list;
    u32 id;

    id = (u16)cardWord;

    if (list == 1) goto main_count;
    if (list > 1) goto above_one;
    if (list == 0) goto owned_count;
    return list;
above_one:
    if (selector == 2) goto side_count;
    return list;
owned_count:
    {
        u32 base = (u32)&gSaveData;
        struct TrunkEntryView *entry;

        entry = (struct TrunkEntryView *)(id * 4 + base);
        return entry->count;
    }
main_count:
    {
        u32 base = (u32)&gSaveData;
        u8 *entry;
        u32 packed;
        u32 main;

        entry = (u8 *)(id * 4 + base);
        packed = entry[9];
        main = (packed << 28) >> 30;

        return main + (packed >> 6);
    }
side_count:
    {
        u32 base = (u32)&gSaveData;
        struct TrunkEntryView *entry;

        entry = (struct TrunkEntryView *)(id * 4 + base);
        return entry->sideCopies;
    }
}

/* Local view of gDeckEdit for the list rows and counts (same offsets as struct DeckEdit's
 * lists/listCount/listRow; the union reaches each list's row array from the lists base). */
struct CountState {
    u8 pad[0x644];
    union {
        u16 trunkRows[1][0x394];
        struct { u8 pad[0x66A]; u16 deckRows[1][0x394]; } deckView;
        struct { u8 pad[0x70A]; u16 sideRows[1][0x394]; } sideView;
    } lists;
    u8 pad1478[0x1C];
    u16 listCount[2][3];
    u8 listRow[3];
};
#define CS_STATE (*(struct CountState *)&gDeckEdit)
u32 GetCardCopiesInList(int, int);
/* Literal table addresses (gCardStats / gCardIdToNumber), reloaded per use like the ROM;
 * see card_data.h on why the symbol form generates different code. */
#define STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER_OF(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_KIND_OF(id) ((int)CARD_STATS_TYPE(STATS_WORD(id)))
/* Card frame kind: CARD_FRAME_RITUAL for Obelisk, CARD_FRAME_EFFECT for Slifer/Ra, 7/8/9 for
 * Magic/Trap/Ticket cards, else the stats kind bits. Direct returns (not `v = ...; break;`)
 * keep jump2 from threading `kind == 0` past the test. */
static inline u8 StatsFrameKind(u16 id)
{
    switch (CARD_NUMBER_OF(id)) {
    case CARD_OBELISK_THE_TORMENTOR: return CARD_FRAME_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA: return CARD_FRAME_EFFECT;
    default:
        switch (CARD_KIND_OF(id)) {
        case CARD_TYPE_MAGIC: return 7;
        case CARD_TYPE_TRAP: return 8;
        case CARD_TYPE_TICKET: return 9;
        default: return CARD_STATS_KIND(STATS_WORD(id));
        }
    }
}
#define COUNT_KIND(kindValue) \
    for (i = 0; i < CS_STATE.listCount[row][list]; i++) { \
        u16 card = cards[i]; \
        switch (CARD_KIND_OF(card)) { \
        case CARD_TYPE_TRAP: \
        case CARD_TYPE_MAGIC: \
            break; \
        default: \
            if (StatsFrameKind(cards[i]) == (kindValue)) \
                total += GetCardCopiesInList(list, cards[i]); \
            break; \
        } \
    }
#define COUNT_TYPE(typeValue) \
    for (i = 0; i < CS_STATE.listCount[row][list]; i++) { \
        u16 card = cards[i]; \
        if (CARD_KIND_OF(card) == (typeValue)) total += GetCardCopiesInList(list, card); \
    }
/* Sums GetCardCopiesInList copy counts over list `list`'s current row for category 1-6
 * (frame kind 0/1/2, Magic, Trap, frame kind 3). */
u32 DeckStats_CountCategory(u8 list, u8 category)
{
    u16 total = 0;
    u8 row = CS_STATE.listRow[list];
    u16 *cards;
    u16 i;
    switch (list) {
    case DECKEDIT_LIST_TRUNK: cards = CS_STATE.lists.trunkRows[row]; break;
    case DECKEDIT_LIST_MAIN_DECK: cards = CS_STATE.lists.deckView.deckRows[row]; break;
    case DECKEDIT_LIST_SIDE_DECK: cards = CS_STATE.lists.sideView.sideRows[row]; break;
    }
    switch (category) {
    case DECK_STATS_NORMAL: COUNT_KIND(CARD_FRAME_NORMAL); break;
    case DECK_STATS_EFFECT: COUNT_KIND(CARD_FRAME_EFFECT); break;
    case DECK_STATS_FUSION: COUNT_KIND(CARD_FRAME_FUSION); break;
    case DECK_STATS_MAGIC: COUNT_TYPE(CARD_TYPE_MAGIC); break;
    case DECK_STATS_TRAP: COUNT_TYPE(CARD_TYPE_TRAP); break;
    case DECK_STATS_RITUAL: COUNT_KIND(CARD_FRAME_RITUAL); break;
    }
    return total;
}
/* The four list sizes at the end of gSaveData (struct SaveData's trunkSize/deckSize/
 * sideDeckSize/fusionDeckSize), reached through the register-bound base below. */
struct SaveCountsView { u8 pad[0x20C6]; u16 trunkSize, deckSize, sideDeckSize, fusionDeckSize; };
extern struct SaveCountsView gSaveDataCounts asm("gSaveData");
/* The statistics rows in gScratchBuffer (util.h), viewed as struct DeckStatsRow[7]. */
#define DECK_STATS_ROWS ((struct DeckStatsRow *)gScratchBuffer)
void DeckEdit_BuildCardLists(void);
u32 DeckStats_CountCategory(u8, u8);
int DivFix8(int, int);

/* FAKEMATCH: initialized bindings and empty allocation hints preserve the
 * original count-load and sum order. Case 0/2 share a halfword load; case 1
 * derives the extra-copy address from the main-copy offset. No instructions
 * are supplied by the hints. Caller-saved bindings are dead before calls. */
void DeckStats_Compute(void)
{
    struct DeckStatsRow *rows = DECK_STATS_ROWS;
    int value, percent;
    u16 quotient;
    u32 last;
    u8 *cursor;
    DeckEdit_BuildCardLists();
    {
        u16 *count;
        switch (gDeckEdit.curList) {
        case 0: {
            register u32 base asm("r0") = (u32)&gSaveDataCounts;
            register u32 off asm("r2") = 0x20C6;
            asm("" : : "r"(base), "r"(off));
            base += off;
            count = (u16 *)base;
            goto read_count;
        }
        case 1: {
            register u32 base asm("r0") = (u32)&gSaveDataCounts;
            register u32 off asm("r2") = 0x20C8;
            u16 *m = (u16 *)(base + off);
            off += 4;
            base += off;
            rows[6].count = *m + *(u16 *)base;
            break;
        }
        case 2: {
            u32 base = (u32)&gSaveDataCounts;
            u32 off = 0x20CA;
            base += off;
            count = (u16 *)base;
        }
        read_count:
            rows[6].count = *count;
            break;
        }
    }
    rows[6].percent = 100;
    {
        register u32 base asm("r4") = (u32)&gDeckEdit;
        register u32 off asm("r2") = 0x1C1C;
        asm("" : : "r"(off));
        base += off;
        cursor = (u8 *)base;
    }
    rows[0].count = DeckStats_CountCategory(*cursor, 1);
    rows[1].count = DeckStats_CountCategory(*cursor, 2);
    rows[2].count = DeckStats_CountCategory(*cursor, 3);
    rows[3].count = DeckStats_CountCategory(*cursor, 4);
    rows[4].count = DeckStats_CountCategory(*cursor, 5);
    last = DeckStats_CountCategory(*cursor, 6);
    rows[5].count = last;
    {
        register int a asm("r1") = rows[1].count;
        register int b asm("r2") = rows[0].count;
        int c;
        asm("" : : "r"(a), "r"(b));
        a += b;
        b = rows[2].count;
        asm("" : : "r"(b));
        b += a;
        c = rows[3].count;
        asm("" : : "r"(c));
        c += b;
        a = rows[4].count;
        asm("" : : "r"(a));
        a += c;
        last += a;
        rows[6].count = last;
    }
    {
        int input = rows[0].count;
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[0].percent = percent;
    }
    {
        int input = rows[1].count;
        asm("" : : "r"(input));
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[1].percent = percent;
    }
    {
        int input = rows[2].count;
        asm("" : : "r"(input));
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[2].percent = percent;
    }
    {
        int input = rows[3].count;
        asm("" : : "r"(input));
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[3].percent = percent;
    }
    {
        int input = rows[4].count;
        asm("" : : "r"(input));
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[4].percent = percent;
    }
    {
        int input = rows[5].count;
        asm("" : : "r"(input));
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[5].percent = percent;
    }
}

/* Matching: sprite.h's DrawNumberSprites narrows its arguments to u16/u8; the ROM passes
 * them as words (the y argument `i * 16 + 0x24` is not narrowed), so this unit calls it
 * through the word form. */
void DrawNumberSpritesWord(u16 value, int numDigits, int mode, int x, int y, const u8 *digitTemplates, int unused, int spacing, int sheetX, int sheetY, int priority, void *list) asm("DrawNumberSprites");
/* FAKEMATCH: the do-while(0) wrapper changes agbcc's loop-invariant hoisting
   so the loop constants land in the same registers as the ROM. */
void DeckStats_DrawNumbers(void)
{
    u8 i;
    do {
        for (i = 0; i <= 5; i++) {
            struct DeckStatsRow *row = &DECK_STATS_ROWS[i];
            DrawNumberSpritesWord(row->count, 4, NUMSPRITE_NO_LEADING_ZEROS, 0xA8, i * 16 + 0x24, gDeckEditDigitSprites, 1, 8, 0, 0, 0, &gDeckEdit);
            DrawNumberSpritesWord(row->percent, 3, NUMSPRITE_NO_LEADING_ZEROS, 0xC8, i * 16 + 0x24, gDeckEditDigitSprites, 1, 8, 0, 0, 0, &gDeckEdit);
        }
    } while (0);
    DrawNumberSpritesWord(DECK_STATS_ROWS[6].count, 4, NUMSPRITE_NO_LEADING_ZEROS, 0x98, 0x8C, gDeckEditDigitSprites, 1, 8, 0, 0, 0, &gDeckEdit);
}
/* bg.h's CropMapBlock takes u16 arguments; the ASM crop helper decodes its word-valued
 * scalar arguments on entry, so this unit calls it through the word form. */
u32 CropMapBlockWord(u16 *src, int sx, int sy, int srcW, void *dstMap, int dx, int dy, int w, int h, int mapSize) asm("CropMapBlock");
extern const u8 gDeckStatsPatternMap[], gDeckStatsPanelMap[], gDeckStatsBg1Map[], gDeckStatsListIconMap[];
extern const u8 gDeckStatsBgTiles[], gDeckStatsLabelTiles[], gDeckStatsBgPal[];
/* = gCardIdToNumber[0x439] (the card number of card ID 0x439, Obelisk); read by
 * DeckStats_Init's discarded card-type computation. */
extern const u16 gDeckStatsObeliskNumber asm("gCardIdToNumber_1081");
/* Alias of gDeckEdit.panelAlpha (header note: address-suffixed aliases are matching choices). */
extern u8 gDeckEditPanelAlpha asm("gDeckEditPanelAlpha");   /* 0x0201F770 */
/* FAKEMATCH: two initialized bindings retain the shared tile source and
 * fade-speed store allocation. Three empty constraints preserve the original
 * discarded card-type computation and its range tests without instructions. */
int DeckStats_Init(void)
{
    u32 zero0 = 0, zero1;
    u16 x, y;
    register const u8 *tiles asm("r10");
    CpuFastSet(&zero0, (void *)VRAM, 0x01004000);
    zero1 = 0;
    CpuFastSet(&zero1, (void *)OBJ_VRAM0, 0x01002000);
    for (y = 0; y <= 3; y++) {
        for (x = 0; x <= 3; x++)
            CopyMapRectAddOffset((u16 *)gDeckStatsPatternMap, (void *)(VRAM + 0xF000 + (x * 8 + y * 256) * 2), 8, 8, 8, 0, 0);
    }
    CopyMapRect((void *)gDeckStatsPanelMap, (void *)(VRAM + 0xE000), 30, 20);
    CopyMapRect((void *)gDeckStatsBg1Map, (void *)(VRAM + 0xD000), 30, 20);
    CopyMapRect((void *)gListFilterSortPageMap, (void *)(VRAM + 0xC000), 30, 20);
    tiles = gDeckStatsListIconMap;
    CropMapBlockWord((u16 *)tiles, 0, gDeckEdit.curList * 5, 7, (void *)(VRAM + 0xD000), 20, 0, 7, 5, 0);
    CropMapBlockWord((u16 *)tiles, 0, gDeckEdit.curList * 5, 7, (void *)(VRAM + 0xC000), 20, 0, 7, 5, 0);
    CpuFastSet(gDeckStatsBgTiles, (void *)VRAM, 0x800);
    CpuFastSet(gDeckStatsLabelTiles, (void *)(VRAM + 0x2000), 0x800);
    CopyTileSheetTo2D((u8 *)gDeckEditObjTiles, (void *)OBJ_VRAM0, 16);
    CpuFastSet(gDeckStatsBgPal, (void *)BG_PLTT, 0x80);
    CpuSet(gDeckEditObjPal, (void *)OBJ_PLTT, 0x100);
    REG_BG0CNT = 0x1800;
    REG_BG1CNT = 0x1A01;
    REG_BG2CNT = 0x1C02;
    REG_BG3CNT = 0x1E02;
    {
        int step = -0x180;
        u32 base = (u32)&gDeckEdit;
        u32 off = 0x618;

        FadeStart(FADE_BLACK, step, 0, (struct Fade *)(base + off));
    }
    REG_BG0HOFS = 0; REG_BG0VOFS = 0;
    REG_BG1HOFS = 0; REG_BG1VOFS = 0;
    REG_BG2HOFS = 0; REG_BG2VOFS = 0;
    REG_BG3HOFS = 0; REG_BG3VOFS = 0;
    {
        register int speed asm("r0") = 16;
        gDeckEditPanelAlpha = speed;
    }
    REG_DISPCNT = 0x1A00;
    DeckStats_Compute();
    {
        u32 card = 0x439;
        int number;
        int threshold = CARD_OBELISK_THE_TORMENTOR;
        asm("" : "+r"(card));
        number = gDeckStatsObeliskNumber;
        asm("" : "+r"(threshold));
        if (number == threshold) goto done_type;
        if (number < threshold) goto compute_type;
        if (number <= CARD_THE_WINGED_DRAGON_OF_RA) goto done_type;
    compute_type:
        {
            u32 off = card * 4;
            u32 value = CARD_STATS_TYPE(*(const u32 *)((u32)gCardStats + off));
            asm("" : : "r"(value));
        }
    done_type: ;
    }
    return 1;
}

void DeckStats_DrawNumbers(void);
/* Local view of gDeckEdit reached from the fade (gDeckEditFade = &gDeckEdit.fade): the
 * Statistics screen reads its background scroll and panel bytes from this base. */
struct DeckStatsFadeView {
    struct Fade fade;               /* +0x0000 (= gDeckEdit.fade) */
    u8 pad8[0x1634 - 0x8];
    u16 bgScrollX;                  /* +0x1634 (= gDeckEdit.bgScrollX) */
    u16 bgScrollY;                  /* +0x1636 (= gDeckEdit.bgScrollY) */
    u8 pad1638;
    s8 panelAlphaStep;              /* +0x1639 (= gDeckEdit.panelAlphaStep): set to 1 to close the panel */
    u8 pad163A;
    u8 inputLock;                   /* +0x163B (= gDeckEdit.inputLock) */
};
extern struct DeckStatsFadeView gDeckEditFade;   /* 0x0201E138 */

u16 DeckStats_Update(void)
{
    u32 keys = gMain.newKeys & 0x3FF;
    FadeTick((struct Fade *)&gDeckEditFade);
    DeckStats_DrawNumbers();
    gDeckEditFade.bgScrollX += 0x80;
    gDeckEditFade.bgScrollY += 0x80;
    REG_BG3HOFS = gDeckEditFade.bgScrollX >> 8;
    REG_BG3VOFS = gDeckEditFade.bgScrollY >> 8;
    if (gDeckEditFade.fade.state == FADE_STATE_IDLE && gDeckEditFade.inputLock == 0) {
        switch (keys) {
        case A_BUTTON:
        case B_BUTTON:
            gDeckEditFade.panelAlphaStep = 1;
            PlaySE(SE_CANCEL);
            break;
        }
    }
    if (gDeckEdit.fade.state == FADE_STATE_FADED_OUT) {
        gDeckEdit.exitMode = DECKEDIT_EXIT_LIST_VIEW;
        gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
        return 1;
    }
    if (gDeckEdit.fade.state == FADE_STATE_FADED_IN) {
        REG_BLDCNT = 0x3F44;
        SetBldAlpha(16);
        REG_DISPCNT |= 0x400;
        gDeckEdit.fade.state = FADE_STATE_IDLE;
        gDeckEdit.panelAlphaStep = -1;
        gDeckEdit.panelShown = 1;
    }
    if (gDeckEdit.panelAlphaStep != 0) {
        gDeckEdit.panelAlpha += gDeckEdit.panelAlphaStep;
        if (gDeckEdit.panelAlpha == 8)
            gDeckEdit.panelAlphaStep = 0;
        if (gDeckEdit.panelAlpha == 16) {
            REG_DISPCNT &= 0xFBFF;
            FadeStart(FADE_BLACK, 0x180, 0, &gDeckEdit.fade);
            gDeckEdit.panelAlphaStep = 0;
            gDeckEdit.panelShown = 0;
        }
        SetBldAlpha(gDeckEdit.panelAlpha);
    }
    OamListFlush((struct OamList *)&gDeckEdit);
    OamListClear((u8 *)&gDeckEdit);
    return 0;
}
u16 DeckEdit_RunStatistics(void)
{
    if (gDeckStatsSteps[gMain.seqState1]) {
        if (gDeckStatsSteps[gMain.seqState1]())
            gMain.seqState1++;
        return 0;
    }
    return 1;
}
/* Local view of gDeckEdit for DeckEdit_Init (same offsets as struct DeckEdit). */
struct InitState {
    u8 pad[0x620]; u16 listPos[3];
    u8 pad626[0x630 - 0x626];
    u16 bg3Hofs, bg3Vofs; u8 cardArtPage, scrollDir;
    u16 unk636, bg1Hofs, bg1Vofs, bg0Hofs, bg0Vofs;
    u8 pad640[0x1494 - 0x640];
    u16 listCount[2][3]; u8 listRow[3];
    u8 pad14A3[0x1710 - 0x14A3];
    u8 redrawOnPageSlide:1; u8 rest:7;
    u8 pad1711[0x18AC - 0x1711]; u16 brightness;
    u8 pad18AE[0x1BB0 - 0x18AE]; u16 scrollBar[2];
    u8 upArrowDirty:1; u8 rest4:7; u8 upArrowFrame;
    u8 downArrowDirty:1; u8 rest6:7; u8 downArrowFrame;
    u8 pad1BB8[0x1C1C - 0x1BB8]; u8 curList, prevList;
};
/* Alias of &gDeckEdit.frameSlots (header note: address-suffixed aliases are matching choices). */
extern u8 gDeckEditFrameSlots[] asm("gDeckEditFrameSlots");   /* 0x0201F6D8 */
/* The cursor-row page/ring byte at gDeckEditFrameSlots + 0x7C (gDeckEdit + 0x1C34). */
struct CursorRowPageView { u8 cursorRowPage:1; u8 listRowRing:4; u8 rest:3; u8 pad[7]; };
#define INIT (*(struct InitState *)&gDeckEdit)
/* Trunk entry from the save base; the u8 bitfields continue the entry's first halfword
 * (save.h's struct TrunkEntry fields deckCopies/sideCopies/fusionCopies). */
struct TrunkEntryInit { u16 count : 10; u8 deckCopies : 2; u8 sideCopies : 2; u8 fusionCopies : 2; };
struct TrunkInit { u8 pad0[8]; struct TrunkEntryInit e[1]; };
#define TRUNK_INIT ((struct TrunkInit *)&gSaveData)
int DeckEdit_Init(void)
{
    u16 i, j;
    struct CursorRowPageView *panel;
    MemClear16(&INIT, 0x1C5C);
    gMain.vblankFlags = 1;
    REG_BG0VOFS = 0; REG_BG0HOFS = 0;
    REG_BG1VOFS = 0; REG_BG1HOFS = 0;
    REG_BG2VOFS = 0; REG_BG2HOFS = 0;
    REG_BG3VOFS = 0; REG_BG3HOFS = 0;
    REG16(0x28) = 0; REG16(0x2A) = 0;
    REG16(0x3C) = 0; REG16(0x3E) = 0;
    REG_DISPCNT &= 0xE0FF;
    OamListClear((u8 *)&INIT);
    INIT.bg3Vofs = 0; INIT.bg3Hofs = 0;
    INIT.bg1Vofs = 0; INIT.bg1Hofs = 0;
    INIT.bg0Vofs = 0; INIT.bg0Hofs = 0;
    for (i = 0; i <= 2; i++) {
        INIT.listPos[i] = 0; INIT.listRow[i] = 0;
        for (j = 0; j <= 1; j++) INIT.listCount[j][i] = 0;
    }
    INIT.prevList = 0; INIT.curList = 0;
    INIT.cardArtPage = 0; INIT.scrollDir = 0;
    INIT.redrawOnPageSlide = 0; INIT.brightness = 0xFC00;
    Ease_Init(0, 0, 0, (struct Ease *)((u8 *)&INIT + 0x628));
    ClearKatakanaFlag((u8 *)&INIT + 0x640);
    ObjAffineInit((struct ObjAffine *)((u8 *)&INIT + 0x18B0));
    for (i = 1; i <= 0x334 && CARD_NUMBER_OF(i) != 0xFFFF; i++) {
        if ((u16)(CARD_NUMBER_OF(i) - CARD_NUMBER_TOKEN_FIRST) > 0x4F) {
            if (TRUNK_INIT->e[i].count)
                DeckEdit_SetListCard(i, DECKEDIT_LIST_TRUNK, INIT.listRow[0], INIT.listCount[INIT.listRow[0]][0]++);
            if (TRUNK_INIT->e[i].deckCopies || TRUNK_INIT->e[i].fusionCopies)
                DeckEdit_SetListCard(i, DECKEDIT_LIST_MAIN_DECK, INIT.listRow[1], INIT.listCount[INIT.listRow[1]][1]++);
            if (TRUNK_INIT->e[i].sideCopies)
                DeckEdit_SetListCard(i, DECKEDIT_LIST_SIDE_DECK, INIT.listRow[2], INIT.listCount[INIT.listRow[2]][2]++);
        }
    }
    DeckEdit_CountSideDeckMonsters();
    DeckEdit_CalcScrollBar(INIT.listCount[INIT.listRow[INIT.curList]][INIT.curList], INIT.listPos[INIT.curList], INIT.scrollBar);
    INIT.upArrowDirty = 1; INIT.downArrowDirty = 1;
    if (INIT.listCount[INIT.listRow[INIT.curList]][INIT.curList] > 5)
        INIT.downArrowFrame = INIT.upArrowFrame = 1;
    else INIT.downArrowFrame = INIT.upArrowFrame = 0;
    DeckEdit_ResetFrameSlots(gDeckEditFrameSlots);
    DeckEdit_ResetCardMove(gDeckEditFrameSlots + 0x68);
    panel = (struct CursorRowPageView *)(gDeckEditFrameSlots + 0x7C);
    panel->cursorRowPage = 0; panel->listRowRing = 0;
    return 1;
}
