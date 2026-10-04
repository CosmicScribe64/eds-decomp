/*
 * deck_edit_view (0x0806D51C-0x0806ED44): the card-list view of the Deck Edit screen
 * (wiki/functions/deck-edit-view-c.md).
 *
 * DeckEdit_ResetListView clears the scroll, animation and view-state fields after a sub-screen
 * hands control back to the list. DeckEdit_InitListView loads the frame graphics and palettes,
 * draws the visible rows around the cursor and configures the windows and backgrounds;
 * DeckEdit_EnterListView is its step-table entry. DeckEdit_Update is the per-frame handler: it
 * ticks the scroll tween, redraws the rows when a horizontal page slide crosses its midpoint,
 * handles browse and command-bar input, and draws the frame, scroll bar and command bar.
 * The state lives in gDeckEdit (struct DeckEdit, 0x0201DB20), shared with the twin handlers
 * ProhibitCardSelect_Update (deck_edit.c) and TradeCardSelect_Update (deck_edit_prohibit.c).
 */
#include "global.h"
#include "card_data.h"            /* CARD_ID_MASK, gCardIdToNumber, gCardStats */
#include "constants/cards.h"      /* CARD_OBELISK_THE_TORMENTOR, CARD_SLIFER_THE_SKY_DRAGON, CARD_THE_WINGED_DRAGON_OF_RA */
#include "constants/card_stats.h" /* CARD_STATS_TYPE_MASK/SHIFT, enum CardType, enum CardKind */
#include "constants/sound.h"      /* SE_CURSOR, SE_CONFIRM, SE_CANCEL, SE_ERROR */
#include "gba.h"                        /* A_BUTTON, B_BUTTON, DPAD_*, REG_*, CpuSet, CpuFastSet */
#include "main.h"                       /* struct Main gMain, newKeys, vblankFlags */
#include "sound.h"                      /* PlaySE */
#include "deck_edit.h"                  /* enums, struct DeckEdit gDeckEdit */

/* ---- Local views kept for matching (build/readability/issues/deck_edit_view.md) ---- */

void MemClear16Wide(void *dst, u32 size) asm("MemClear16");
void DeckEdit_ResetFrameSlotsWide(void *p) asm("DeckEdit_ResetFrameSlots");
void DeckEdit_ResetCardMoveWide(void *p) asm("DeckEdit_ResetCardMove");
void DeckEdit_CalcScrollBar(u16 a, u16 b, u16 *out);

/* Deck Edit step: reset the card list state and re-sync the list slide. */
int DeckEdit_ResetListView(void)
{
    gMain.vblankFlags = 1;
    gDeckEdit.bg3Vofs = 0;
    gDeckEdit.bg3Hofs = 0;
    gDeckEdit.bg1Vofs = 0;
    gDeckEdit.bg1Hofs = 0;
    gDeckEdit.bg0Vofs = 0;
    gDeckEdit.bg0Hofs = 0;
    DeckEdit_ResetFrameSlotsWide((u8 *)&gDeckEdit + 0x1BB8);
    DeckEdit_ResetCardMoveWide((u8 *)&gDeckEdit + 0x1C20);
    gDeckEdit.brightness = 0xFC00;
    gDeckEdit.nameIndexCache[0] = 0;
    gDeckEdit.exitMode = DECKEDIT_EXIT_LIST_VIEW;
    DeckEdit_CalcScrollBar(gDeckEdit.listCount[gDeckEdit.listRow[gDeckEdit.curList]][gDeckEdit.curList],
                 gDeckEdit.listPos[gDeckEdit.curList], (u16 *)&gDeckEdit.scrollBar);
    gDeckEdit.cursorRowPage = 0;
    gDeckEdit.listRowRing = 0;
    gDeckEdit.exitMode = DECKEDIT_EXIT_LIST_VIEW;
    gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
    return 1;
}

/* Zero the 0x20-byte sprite entry at dst + idx * 0x20. */
void ClearTile4bpp(void *dst, int idx)
{
    MemClear16Wide((u8 *)dst + (idx << 5), 0x20);
}

/* ROM graphics of the card-list screens (deck_edit.h; single-unit tables stay local externs). */
extern const u8 gDeckEditBgTiles[];             /* 0x086E3030 */
extern const u8 gDeckEditBgPals4to7[];          /* 0x086ED030 */
extern const u8 gDeckEditBgPals1to3[];          /* 0x086ED0B0 */
extern const u8 gDeckEditBgPal3[];              /* 0x086ED190 */
extern const u8 gDeckEditStatIconPal[];                /* 0x08704EE8: first BG palette */
extern u8 gDeckEditCurList;                        /* 0x0201F73C = &gDeckEdit.curList */
extern u16 gDeckEditListPos[];                     /* 0x0201E140 = &gDeckEdit.listPos */
extern u8 gDeckEditObjAffine[];                      /* 0x0201F3D0 = &gDeckEdit.objAffine */
void CopyMapRect(const void *src, void *dst, u32 w, u32 h);
void CopyTileSheetTo2D(const void *src, void *dst, u32 n);
void AnimBlockInitWide(const void *a, void *b) asm("AnimBlockInit");
void DeckEdit_DrawListRowNameWide(int idx, u8 *map, int col, int row, u32 unused, int slot) asm("DeckEdit_DrawListRowName");
void DeckEdit_InitFrameSlotWide(int slot, int x, int kind, u8 *base, u8 *arr) asm("DeckEdit_InitFrameSlot");
void DeckEdit_DrawCursorRowNameWide(int id, u8 *map, int col, int row, void *p) asm("DeckEdit_DrawCursorRowName");
void DeckEdit_DrawNoCardsTextWide(int unused, u8 *map, int col, int row, void *p) asm("DeckEdit_DrawNoCardsText");
extern u32 DeckEdit_GetListCardWide(u8 list, u8 row, int col) asm("DeckEdit_GetListCard");
void LoadCardArt8bpp(int a, u32 b, int c);
void DeckEdit_PlaceCardArtWide(int a, int b, int c, int d) asm("DeckEdit_PlaceCardArt");
void DeckEdit_DrawCardIconsWide(int pos) asm("DeckEdit_DrawCardIcons");
void DeckEdit_DrawAtkDefWide(u8 *map, int col, int row, void *p) asm("DeckEdit_DrawAtkDef");
void DeckEdit_DrawLevelStarsWide(u8 *map, int col, int row, int perRow) asm("DeckEdit_DrawLevelStars");
void FadeStartWide(u32 a, u32 b, u32 c, void *p) asm("FadeStart");
void LoadDigitTilesWide(u8 *dst, u32 unused, u8 pal, int a, int b) asm("LoadDigitTiles");
extern void CpuFastSet(const void *src, void *dst, u32 cnt);
extern void CpuSet(const void *src, void *dst, u32 cnt);
#define PSTATE ((u8 *)&gDeckEdit)
/* The AnimBlock at gDeckEdit +0x1718; OBJF(n) is a byte inside it. */
#define OBJS ((u8 *)&gDeckEdit + 0x1718)
struct AnimActiveByte { u8 active : 8; };
/* Matching: AnimState.active (+0x0E, stride 0x14) of each list animation, set in bulk here. */
#define OBJF(off) (((struct AnimActiveByte *)(OBJS + (off)))->active)
#define CURLIST gDeckEditCurList
#define CNT(list) (gDeckEdit.listCount[gDeckEdit.listRow[list]][list])

/* Deck Edit card-list view init: clears VRAM, loads graphics and palettes, draws the visible rows of the list. */
int DeckEdit_InitListView(void)
{
    u32 z1;
    u32 z2;
    u8 j;
    u8 *rowObjects;
    s16 k;
    int v;
    int next;
    z1 = 0;
    CpuFastSet((void *)&z1, (void *)0x06000000, 0x01004000);
    z2 = 0;
    CpuFastSet((void *)&z2, (void *)0x06010000, 0x01002000);
    CopyMapRect(gDeckEditFrameMap, (void *)0x0600E000, 0x1E, 0x14);
    CpuFastSet(gDeckEditBgTiles, (void *)0x06000000, 0x800);
    CpuFastSet(gDeckEditLabelTiles, (void *)0x06004000, 0x800);
    CopyTileSheetTo2D(gDeckEditObjTiles, (void *)0x06010000, 0x10);
    CopyTileSheetTo2D(gDeckEditCardStackObjTiles, (void *)0x06010200, 0x10);
    CopyTileSheetTo2D(gDeckEditCardIconObjTiles, (void *)0x06014000, 0x10);
    CopyTileSheetTo2D(gDeckEditCardFrameObjTiles, (void *)0x06014200, 0x10);
    ClearTile4bpp((void *)0x06010000, 0x8A);
    ClearTile4bpp((void *)0x06010000, 0xAA);
    ClearTile4bpp((void *)0x06010000, 0xCA);
    ClearTile4bpp((void *)0x06010000, 0xEA);
    DeckEdit_LoadCardBoxTiles((u8 *)0x06006000);
    AnimBlockInitWide(gDeckEditAnimScripts, OBJS);
    OBJF(0xE) |= 0xFF;
    /* FAKEMATCH: staging this constant pointer preserves later address allocation. */
    rowObjects = gDeckEditObjAffine;
    OBJF(0x22) |= 0xFF;
    OBJF(0x36) |= 0xFF;
    OBJF(0x4A) |= 0xFF;
    OBJF(0x5E) |= 0xFF;
    OBJF(0x72) |= 0xFF;
    OBJF(0x86) |= 0xFF;
    OBJF(0x9A) |= 0xFF;
    OBJF(0xAE) |= 0xFF;
    OBJF(0xD6) = 0;
    OBJF(0xEA) = 0;
    OBJF(0xFE) = 0;
    OBJF(0x112) |= 0xFF;
    OBJF(0x126) |= 0xFF;
    OBJF(0x13A) |= 0xFF;
    OBJF(0x14E) |= 0xFF;
    CpuFastSet(gDeckEditBgPals4to7, (void *)0x05000080, 0x20);
    CpuFastSet(gDeckEditBgPals1to3, (void *)0x05000020, 0x18);
    CpuFastSet(gDeckEditBgPal3, (void *)0x05000060, 8);
    CpuSet(gDeckEditObjPal, (void *)0x05000200, 0x100);
    *(u16 *)0x05000044 = 0x7758;
    DeckEdit_LoadCardIconTiles((u8 *)0x06002000);
    CpuSet(gDeckEditStatIconPal, (void *)0x05000000, 0x10);
    k = 1;
    for (j = 0; j < 2; j++) {
        if ((s16)gDeckEdit.listPos[CURLIST] + (s16)k - 3 >= 0) {
            DeckEdit_DrawListRowNameWide(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST] + (s16)k - 3),
                         (u8 *)0x0600D000, 0, ((s16)k - 1) * 2, (u32)(PSTATE + 0x640), -(s16)k + 2);
            DeckEdit_InitFrameSlotWide((s16)k - 1, DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST] + (s16)k - 3),
                         (s16)k, PSTATE + 0x1BB8, rowObjects);
        }
        k = (s16)k + 1;
    }
    for (j = 0; j < 2; j = next) {
        int pos = gDeckEditListPos[CURLIST] + j;
        int limit = CNT(CURLIST) - 1;
        next = j + 1;
        if (pos < limit) {
            DeckEdit_DrawListRowNameWide(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] + next),
                         (u8 *)0x0600D000, 0, j * 2 + 9, (u32)(PSTATE + 0x640), j + 4);
            DeckEdit_InitFrameSlotWide(j + 3, DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] + j + 1),
                         j + 4, PSTATE + 0x1BB8, rowObjects);
        }
    }
    if (CNT(CURLIST) != 0) {
        DeckEdit_DrawCursorRowNameWide(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST]),
                     (u8 *)0x0600C000, 0, (v = (gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
        DeckEdit_InitFrameSlotWide(2, DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST]),
                     3, PSTATE + 0x1BB8, PSTATE + 0x18B0);
        LoadCardArt8bpp(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST]),
                     0x06008000 + gDeckEdit.cardArtPage * 0x1680, gDeckEdit.cardArtPage);
        DeckEdit_PlaceCardArtWide(0x13, ((gDeckEdit.bg3Vofs & 0xFF) >> 3) + 2, gDeckEdit.cardArtPage, 1);
        DeckEdit_DrawCardIconsWide(0);
        DeckEdit_DrawAtkDefWide((u8 *)0x0600C000, 0xB, 7, PSTATE + 0x640);
        DeckEdit_DrawLevelStarsWide((u8 *)0x0600C000, 0x11, 7, 6);
    } else {
        DeckEdit_DrawNoCardsTextWide(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST]),
                     (u8 *)0x0600C000, 0, (v = (gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
    }
    DeckEdit_DrawStatementLabels(gDeckEdit.curList);
    REG_WIN0H = 0xF0;
    REG_WIN0V = 0x244C;
    REG_WIN1H = 0xF0;
    REG_WIN1V = 0x70;
    REG_WININ = 0x3E3D;
    REG_WINOUT = 0x3C;
    REG_BG0CNT = 0x1800;
    REG_BG1CNT = 0x1A01;
    REG_BG2CNT = 0x1C02;
    REG_BG3CNT = 0x1E8B;
    FadeStartWide(0, -0x180, 0, PSTATE + 0x618);
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = 0;
    REG_DISPCNT = 0x7F00;
    LoadDigitTilesWide((u8 *)0x06006000, 0x300, 1, 0, 0);
    return 1;
}

/* Deck Edit step: draw the card list view, then run the browse loop. */
int DeckEdit_EnterListView(void)
{
    DeckEdit_InitListView();
    return 1;
}

/* ---- Local views of gDeckEdit for the frame handler (kept for matching) ---- */

/* Flat per-frame view of gDeckEdit (deck_edit.h struct DeckEdit, read through this unit's own
 * access shapes): the fade head, the scroll tween, the scroll-bar arrows, the frame-slot ring
 * head and the command-menu word. Same overlays as deck_edit.c's ProhibitCardSelect_Update. */
struct DeckFrameFields {
    u8 unk0[0x618];
    u8 fadeHead[6];             /* +0x618: struct Fade head (color, level, step) */
    u8 fadeState;               /* +0x61E: Fade.state */
    u8 unk61F[0x628 - 0x61F];
    u8 tweenState;              /* +0x628: scrollEase.state (enum TickState) */
    u8 unk629;
    s16 tweenStep;              /* +0x62A: scrollEase.cur as s16 */
    u8 unk62C[0x1710 - 0x62C];
    u8 redrawOnPageSlide : 1;   /* +0x1710 bit 0 */
    u8 unk1710_1 : 7;
    u8 unk1711[0x17DA - 0x1711];
    u8 frameDirty;              /* +0x17DA: inside anims */
    u8 unk17DB[0x1BB4 - 0x17DB];
    u8 upArrowDirty;            /* +0x1BB4 (scrollBar.upArrowDirty) */
    u8 upArrowFrame;            /* +0x1BB5 (scrollBar.upArrowFrame) */
    u8 downArrowDirty;          /* +0x1BB6 (scrollBar.downArrowDirty) */
    u8 downArrowFrame;          /* +0x1BB7 (scrollBar.downArrowFrame) */
    u8 frameSlotHead;           /* +0x1BB8: frameSlots.head */
    u8 unk1BB9[0x1C14 - 0x1BB9];
    u8 rowAnimState;            /* +0x1C14: frameSlots.slot[5].active */
    u8 unk1C15[0x1C20 - 0x1C15];
    u8 cardMoveStep;            /* +0x1C20: cardMove.step; 0 = idle */
    u8 unk1C21[0x1C3B - 0x1C21];
    u8 cursorCardFrame;         /* +0x1C3B: enum CardFrame of the cursor card */
    u32 unk1C3C_0 : 15;         /* +0x1C3C: commandMenu timer/anim/rows as bits */
    u16 menuChoice : 3;         /* bits 15-17: commandMenu.choice (enum DeckEditCommand) */
    u32 unk1C3C_18 : 14;
    u8 unk1C40[0x1C58 - 0x1C40];
    u16 rowAnimTimer;           /* +0x1C58: nameTabTimer */
};
struct DeckPhaseByte { u8 anim : 3; u8 rest : 5; };
/* One frame slot (deck_edit.h struct FrameSlot) reached from the +0x1BB8 ring base. */
struct FrameSlotView {
    u8 unk0[12];
    u8 active;
    u8 unk13[3];
};
struct DeckRowFields {
    u8 unk0[0x1BB8];
    struct FrameSlotView rows[6];
};
#define DECK_ROWS ((struct DeckRowFields *)&gDeckEdit)->rows
#define DECK_FRAME (*(struct DeckFrameFields *)&gDeckEdit)
/* gDeckEdit.scrollEase (struct Ease) with the +0x635 scrollDir byte reached from the same base. */
struct ScrollEaseView {
    u8 state;                   /* +0x00 (gDeckEdit +0x628): enum TickState */
    u8 unk1;
    s16 cur;                    /* +0x02: Ease.cur compared as s16 */
    u8 unk4[9];
    u8 scrollDir;               /* +0x0D (gDeckEdit +0x635): enum DeckEditScrollDir */
};
extern struct ScrollEaseView gDeckEditScrollEase;         /* 0x0201E148 = &gDeckEdit.scrollEase */
extern u8 gDeckEditCardMove[];                          /* 0x0201F740 = &gDeckEdit.cardMove */
extern const u16 gDeckEditEaseCurve[];              /* 0x080875D2 */
extern const u16 gCardIdToNumber[];                 /* 0x08622AB4 */
extern const u32 gCardStats[];                      /* 0x08621DE0 */
extern const u8 gDeckEditArtFrameSprites[], gDeckEditPanelCornerSprite[];   /* ROM sprite scripts (names unknown) */
void DeckEdit_DrawScrollBarWide(u16 position, u16 count, u16 *state) asm("DeckEdit_DrawScrollBar");
void DeckEdit_TweenFrameSlotsWide(s16 step, u8 tweenState, u8 direction, u8 *objects, u8 *rows) asm("DeckEdit_TweenFrameSlots");
void DeckEdit_DrawFrameSlotsWide(u8 *rows, void *state) asm("DeckEdit_DrawFrameSlots");
void DeckEdit_StartCardMoveWide(u8 selected, int list, u8 *state) asm("DeckEdit_StartCardMove");
void DeckEdit_UpdateCardMoveWide(void) asm("DeckEdit_UpdateCardMove");
void DeckEdit_UpdatePanelHighlightWide(u8 *list, u8 *row, void *objects) asm("DeckEdit_UpdatePanelHighlight");
void DeckEdit_HandleShoulderKeys(u8 *list);
void DeckEdit_DrawCommandMenuWide(void *state) asm("DeckEdit_DrawCommandMenu");
void DeckEdit_UpdateCommandMenuAnimWide(void *state) asm("DeckEdit_UpdateCommandMenuAnim");
void DeckEdit_ScrollListUp(u16 *position);
void DeckEdit_ScrollListDown(u16 *position);
void DeckEdit_DrawCardCounts(u8 list);
u32 DeckEdit_GetSelectedCardCopies(void);
void DeckEdit_UpdateNameIndexLetters(void);
void DeckEdit_DrawNameIndexTab(void);
void OamListAddSpriteGroupWide(const void *script, int a, int b, int c, int d, int e,
                 int f, int g, int h, int i, int j, void *state) asm("OamListAddSpriteGroup");
void AnimBlockDrawWide(void *objects, int a, int b, int c, int d, int e,
                 int f, int g, int h, void *state) asm("AnimBlockDraw");
void AnimBlockTickWide(void *objects) asm("AnimBlockTick");
void FadeTickWide(void *state) asm("FadeTick");
void FillMapRectWrap(int tile, u32 map, int col, int row, int width, int height, void *work);
void OamListFlushWide(void *state) asm("OamListFlush");
void OamListClearWide(void *state) asm("OamListClear");
void Ease_StartWide(int from, int to, int increment, void *state) asm("Ease_Start");
void Ease_TickWide(void *state) asm("Ease_Tick");
void SetBldAlphaWide(u32 level) asm("SetBldAlpha");
s32 MulFix8U16(s32 scale, u16 value) asm("MulFix8");
void ObjAffineApplyWide(void *object) asm("ObjAffineApply");

#define DECK_TWEEN_STATE DECK_FRAME.tweenState
#define DECK_TWEEN_STEP DECK_FRAME.tweenStep
#define DECK_SCROLL_CURVE gDeckEditEaseCurve[DECK_TWEEN_STEP]

/* Deck Edit card-list frame: animate, handle list/menu input, draw, and fade. */
int DeckEdit_Update(void)
{
    u16 row;
    /* FAKEMATCH: keep the original fill slot and its stack-store scheduling. */
    volatile u32 clear;
    u32 keys;
    u8 slot;
    u8 horizontalOffset;
    u8 blend;
    u16 card;
    int number;
    u32 kind;
    u8 *tail;
    u8 *list;
    u8 *objects;
    struct DeckEditView *state;

    keys = gMain.newKeys & 0x3FF;
    Ease_TickWide(&gDeckEditScrollEase);
    /* Crossing the midpoint of a horizontal page slide changes the visible rows.
     * (The flag is gDeckEdit +0x1710, redrawOnPageSlide, read through the scrollEase alias.) */
    if ((*((u8 *)&gDeckEditScrollEase + 0x10E8) & 1) &&
        ((gDeckEditScrollEase.cur == 4 && gDeckEditScrollEase.scrollDir == DECKEDIT_SCROLL_PAGE_RIGHT) ||
         (gDeckEditScrollEase.cur == 3 && gDeckEditScrollEase.scrollDir == DECKEDIT_SCROLL_PAGE_LEFT))) {
        DECK_FRAME.redrawOnPageSlide = 0;
        clear = 0;
        CpuFastSet((const void *)&clear, (void *)0x0600D000, 0x01000200);
        if ((s16)gDeckEditListPos[CURLIST] - 2 >= 0) {
            DeckEdit_DrawListRowNameWide(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] - 2),
                         (u8 *)0x0600D000, 0, (u8)gDeckEdit.bg1Vofs >> 3, (u32)(PSTATE + 0x640), 1);
        }
        if ((s16)gDeckEditListPos[CURLIST] - 1 >= 0) {
            DeckEdit_DrawListRowNameWide(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] - 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x10) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 2);
        }
        if ((s16)gDeckEditListPos[CURLIST] + 1 < CNT(CURLIST)) {
            DeckEdit_DrawListRowNameWide(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] + 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x48) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 4);
        }
        if ((s16)gDeckEditListPos[CURLIST] + 2 < CNT(CURLIST)) {
            DeckEdit_DrawListRowNameWide(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] + 2),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x58) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 5);
        }
        FillMapRectWrap(0, 0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, 30, 6, PSTATE + 0x640);
        if (CNT(CURLIST) != 0) {
            DeckEdit_DrawCursorRowNameWide(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
            DeckEdit_DrawCardIconsWide(0);
            DeckEdit_DrawAtkDefWide((u8 *)0x0600C000, 11, ((gDeckEdit.bg0Vofs + 0x38) & 0xFF) >> 3, PSTATE + 0x640);
            DeckEdit_DrawLevelStarsWide((u8 *)0x0600C000, 17, ((gDeckEdit.bg0Vofs + 0x38) & 0xFF) >> 3, 6);
        } else {
            DeckEdit_DrawNoCardsTextWide(DeckEdit_GetListCardWide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
        }
    }
    DeckEdit_TweenFrameSlotsWide(DECK_TWEEN_STEP, DECK_TWEEN_STATE, gDeckEdit.scrollDir, PSTATE + 0x18B0, PSTATE + 0x1BB8);

    /* Vertical directions scroll by card; the page directions slide five cards sideways. */
    switch (gDeckEdit.scrollDir) {
    case DECKEDIT_SCROLL_UP:
    case DECKEDIT_SCROLL_DOWN:
        switch (DECK_TWEEN_STATE) {
        case TICK_RUNNING:
            REG_BG3HOFS = gDeckEdit.bg3Hofs;
            REG_BG3VOFS = gDeckEdit.bg3Vofs + (MulFix8U16(0x5000, DECK_SCROLL_CURVE) >> 8);
            REG_BG1HOFS = gDeckEdit.bg1Hofs;
            REG_BG1VOFS = gDeckEdit.bg1Vofs + (MulFix8U16(0x1000, DECK_SCROLL_CURVE) >> 8);
            REG_BG0HOFS = gDeckEdit.bg0Hofs;
            REG_BG0VOFS = gDeckEdit.bg0Vofs + (MulFix8U16(0x2800, DECK_SCROLL_CURVE) >> 8);
            break;
        case TICK_DONE:
            DECK_TWEEN_STATE = TICK_IDLE;
            gDeckEdit.bg3Vofs += MulFix8U16(0x5000, DECK_SCROLL_CURVE) >> 8;
            gDeckEdit.bg1Vofs += MulFix8U16(0x1000, DECK_SCROLL_CURVE) >> 8;
            gDeckEdit.bg0Vofs += MulFix8U16(0x2800, DECK_SCROLL_CURVE) >> 8;
            if (DECK_FRAME.upArrowFrame) {
                DECK_FRAME.upArrowFrame = 1;
                DECK_FRAME.upArrowDirty |= 1;
            }
            if (DECK_FRAME.downArrowFrame) {
                DECK_FRAME.downArrowFrame = 1;
                DECK_FRAME.downArrowDirty |= 1;
            }
            gDeckEdit.scrollDir = DECKEDIT_SCROLL_NONE;
            /* fall through */
        default:
            REG_BG3HOFS = gDeckEdit.bg3Hofs;
            REG_BG3VOFS = gDeckEdit.bg3Vofs;
            REG_BG1HOFS = gDeckEdit.bg1Hofs;
            REG_BG1VOFS = gDeckEdit.bg1Vofs;
            REG_BG0HOFS = gDeckEdit.bg0Hofs;
            REG_BG0VOFS = gDeckEdit.bg0Vofs;
            break;
        }
        break;
    case DECKEDIT_SCROLL_PAGE_RIGHT:
    case DECKEDIT_SCROLL_PAGE_LEFT:
        switch (DECK_TWEEN_STATE) {
        case TICK_RUNNING:
            REG_BG3HOFS = gDeckEdit.bg3Hofs + (MulFix8U16(0x5000, DECK_SCROLL_CURVE) >> 8);
            REG_BG3VOFS = gDeckEdit.bg3Vofs;
            horizontalOffset = MulFix8U16(0x4000, DECK_SCROLL_CURVE) >> 8;
            blend = DECK_SCROLL_CURVE >> 3;
            REG_BLDCNT = 0x3F43;
            if (DECK_TWEEN_STEP <= 3) {
                REG_BG1HOFS = horizontalOffset;
                REG_BG1VOFS = gDeckEdit.bg1Vofs;
                REG_BG0HOFS = horizontalOffset;
                REG_BG0VOFS = gDeckEdit.bg0Vofs;
                SetBldAlphaWide(blend >> 1);
            } else {
                REG_BG1HOFS = horizontalOffset + 0xFFC0;
                REG_BG1VOFS = gDeckEdit.bg1Vofs;
                REG_BG0HOFS = horizontalOffset + 0xFFC0;
                REG_BG0VOFS = gDeckEdit.bg0Vofs;
                SetBldAlphaWide((0x20 - blend) >> 1);
            }
            break;
        case TICK_DONE:
            DECK_TWEEN_STATE = TICK_IDLE;
            gDeckEdit.bg3Hofs += MulFix8U16(0x5000, DECK_SCROLL_CURVE) >> 8;
            if (DECK_FRAME.upArrowFrame) {
                DECK_FRAME.upArrowFrame = 1;
                DECK_FRAME.upArrowDirty |= 1;
            }
            if (DECK_FRAME.downArrowFrame) {
                DECK_FRAME.downArrowFrame = 1;
                DECK_FRAME.downArrowDirty |= 1;
            }
            gDeckEdit.scrollDir = DECKEDIT_SCROLL_NONE;
            /* fall through */
        default:
            REG_BG3HOFS = gDeckEdit.bg3Hofs;
            REG_BG3VOFS = gDeckEdit.bg3Vofs;
            REG_BG1HOFS = gDeckEdit.bg1Hofs;
            REG_BG1VOFS = gDeckEdit.bg1Vofs;
            REG_BG0HOFS = gDeckEdit.bg0Hofs;
            REG_BG0VOFS = gDeckEdit.bg0Vofs;
            REG_BLDCNT = 0x3FC8;
            REG_BLDALPHA = 0x1000;
            break;
        }
        break;
    }

    if (DECK_FRAME.fadeState == 0) {
        switch (gDeckEdit.menuOpen) {
        case 0:
            if (DECK_TWEEN_STATE != TICK_RUNNING) {
                switch (keys) {
                case B_BUTTON:
                    FadeStartWide(0, 0x180, 0, PSTATE + 0x618);
                    gDeckEdit.exitMode = DECKEDIT_EXIT_LEAVE;
                    PlaySE(SE_CANCEL);
                    break;
                case DPAD_UP:
                    DeckEdit_ScrollListUp(&row);
                    break;
                case DPAD_DOWN:
                    DeckEdit_ScrollListDown(&row);
                    break;
                default:
                    if (CNT(gDeckEdit.curList) > 5) {
                        switch (keys) {
                        case DPAD_RIGHT:
                            if (gDeckEdit.listPos[gDeckEdit.curList] + 5 > CNT(gDeckEdit.curList) - 1)
                                gDeckEdit.listPos[gDeckEdit.curList] = 0;
                            else
                                gDeckEdit.listPos[gDeckEdit.curList] += 5;
                            gDeckEdit.brightness = 0xFC00;
                            Ease_StartWide(0, 6, 1, PSTATE + 0x628);
                            gDeckEdit.cardArtPage ^= 1;
                            LoadCardArt8bpp(DeckEdit_GetListCardWide(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]),
                                         0x06008000 + gDeckEdit.cardArtPage * 0x1680, gDeckEdit.cardArtPage);
                            DeckEdit_PlaceCardArtWide(((gDeckEdit.bg3Hofs & 0xFF) >> 3) + 29, ((gDeckEdit.bg3Vofs & 0xFF) >> 3) + 2, gDeckEdit.cardArtPage, 1);
                            gDeckEdit.scrollDir = DECKEDIT_SCROLL_PAGE_RIGHT;
                            DECK_FRAME.redrawOnPageSlide = 1;
                            DeckEdit_CalcScrollBar(CNT(gDeckEdit.curList), gDeckEdit.listPos[gDeckEdit.curList], (u16 *)&gDeckEdit.scrollBar);
                            if (DECK_FRAME.downArrowFrame) {
                                DECK_FRAME.downArrowFrame = 2;
                                DECK_FRAME.downArrowDirty |= 1;
                            }
                            row = gDeckEdit.listPos[gDeckEdit.curList] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < CNT(gDeckEdit.curList)) {
                                    DeckEdit_InitFrameSlotWide(slot, DeckEdit_GetListCardWide(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], row),
                                                 slot + 1, PSTATE + 0x1BB8, PSTATE + 0x18B0);
                                } else {
                                    DECK_ROWS[slot].active = 0;
                                }
                            }
                            DECK_FRAME.rowAnimState = 0;
                            DECK_FRAME.frameSlotHead = 5;
                            DECK_FRAME.rowAnimTimer = 30;
                            PlaySE(SE_CURSOR);
                            break;
                        case DPAD_LEFT:
                            if (gDeckEdit.listPos[gDeckEdit.curList] <= 4)
                                gDeckEdit.listPos[gDeckEdit.curList] = CNT(gDeckEdit.curList) - 1;
                            else
                                gDeckEdit.listPos[gDeckEdit.curList] -= 5;
                            gDeckEdit.brightness = 0xFC00;
                            Ease_StartWide(6, 0, -1, PSTATE + 0x628);
                            gDeckEdit.cardArtPage ^= 1;
                            LoadCardArt8bpp(DeckEdit_GetListCardWide(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]),
                                         0x06008000 + gDeckEdit.cardArtPage * 0x1680, gDeckEdit.cardArtPage);
                            DeckEdit_PlaceCardArtWide(((gDeckEdit.bg3Hofs & 0xFF) >> 3) + 9, ((gDeckEdit.bg3Vofs & 0xFF) >> 3) + 2, gDeckEdit.cardArtPage, 1);
                            gDeckEdit.bg3Hofs -= 0x50;
                            gDeckEdit.scrollDir = DECKEDIT_SCROLL_PAGE_LEFT;
                            DECK_FRAME.redrawOnPageSlide = 1;
                            DeckEdit_CalcScrollBar(CNT(gDeckEdit.curList), gDeckEdit.listPos[gDeckEdit.curList], (u16 *)&gDeckEdit.scrollBar);
                            if (DECK_FRAME.upArrowFrame) {
                                DECK_FRAME.upArrowFrame = 2;
                                DECK_FRAME.upArrowDirty |= 1;
                            }
                            row = gDeckEdit.listPos[gDeckEdit.curList] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < CNT(gDeckEdit.curList)) {
                                    DeckEdit_InitFrameSlotWide(slot, DeckEdit_GetListCardWide(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], row),
                                                 slot + 1, PSTATE + 0x1BB8, PSTATE + 0x18B0);
                                } else {
                                    DECK_ROWS[slot].active = 0;
                                }
                            }
                            DECK_FRAME.rowAnimState = 0;
                            DECK_FRAME.frameSlotHead = 5;
                            DECK_FRAME.rowAnimTimer = 30;
                            PlaySE(SE_CURSOR);
                            break;
                        }
                    }
                    break;
                }
            }
            if (keys == A_BUTTON) {
                gDeckEdit.commandMenu.anim = CMDMENU_OPEN;
                goto confirm_sound;
            }
            DeckEdit_HandleShoulderKeys(&gDeckEditCurList);
            break;
        case 1:
            if (DECK_FRAME.cardMoveStep == 0) {
                switch (keys) {
                case DPAD_RIGHT:
                    if (++DECK_FRAME.menuChoice == gDeckEdit.curList + 1)
                        DECK_FRAME.menuChoice++;
                    if (DECK_FRAME.menuChoice == 7)
                        DECK_FRAME.menuChoice = 0;
                    DECK_FRAME.menuChoice = DECK_FRAME.menuChoice;
                    gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
                    PlaySE(SE_CURSOR);
                    break;
                case DPAD_LEFT:
                    if (DECK_FRAME.menuChoice == 0) {
                        DECK_FRAME.menuChoice = 6;
                    } else {
                        if (--DECK_FRAME.menuChoice == gDeckEdit.curList + 1)
                            DECK_FRAME.menuChoice--;
                    }
                    {
                        /* FAKEMATCH: same registers as the DPAD_RIGHT tail so the two tails merge */
                        register u8 *b asm("r2") = (u8 *)&gDeckEdit;
                        register u32 busy asm("r3");
                        struct DeckPhaseByte *p;
                        asm("" : "=r"(busy));
                        p = (struct DeckPhaseByte *)(b + 0x1C3D);
                        asm("" : : "r"(busy));
                        p->anim = CMDMENU_REFRESH;
                    }
                    PlaySE(SE_CURSOR);
                    break;
                case A_BUTTON:
                    switch (DECK_FRAME.menuChoice) {
                    case DECKEDIT_CMD_CARD_VIEW:
                        if (CNT(gDeckEdit.curList) != 0) {
                            gDeckEdit.exitMode = DECKEDIT_EXIT_CARD_VIEW;
                            FadeStartWide(0, 0x180, 0, PSTATE + 0x618);
                            /* FAKEMATCH: keep this transition call separate from the other menu cases. */
                            asm("");
                            goto confirm_sound;
                        }
                        break;
                    case DECKEDIT_CMD_TO_TRUNK:
                        if ((u16)DeckEdit_GetSelectedCardCopies() == 0)
                            goto error_sound;
                        DeckEdit_StartCardMoveWide(DECK_FRAME.cursorCardFrame, DECKEDIT_LIST_TRUNK, PSTATE + 0x1C20);
                        break;
                    case DECKEDIT_CMD_TO_MAIN_DECK:
                        if ((u16)DeckEdit_GetSelectedCardCopies() == 0)
                            goto error_sound;
                        DeckEdit_StartCardMoveWide(DECK_FRAME.cursorCardFrame, DECKEDIT_LIST_MAIN_DECK, PSTATE + 0x1C20);
                        break;
                    case DECKEDIT_CMD_TO_SIDE_DECK:
                        if ((u16)DeckEdit_GetSelectedCardCopies() == 0)
                            goto error_sound;
                        card = DeckEdit_GetListCardWide(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]);
                        /* Matching: the card tables are read through their literal ROM addresses
                         * (gCardIdToNumber / gCardStats); the symbol forms allocate differently. */
                        number = ((const u16 *)0x08622AB4)[card & CARD_ID_MASK];
                        switch (number) {
                        case CARD_OBELISK_THE_TORMENTOR:
                            kind = CARD_KIND_RITUAL;
                            break;
                        case CARD_SLIFER_THE_SKY_DRAGON:
                        case CARD_THE_WINGED_DRAGON_OF_RA:
                            kind = CARD_KIND_EFFECT;
                            break;
                        default:
                            switch ((int)((((const u32 *)0x08621DE0)[card & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT)) {
                            case CARD_TYPE_MAGIC: kind = CARD_KIND_MAGIC; break;
                            case CARD_TYPE_TRAP: kind = CARD_KIND_TRAP; break;
                            case CARD_TYPE_TICKET: kind = CARD_KIND_TICKET; break;
                            default: kind = (((const u32 *)0x08621DE0)[card & CARD_ID_MASK] & CARD_STATS_KIND_MASK) >> CARD_STATS_KIND_SHIFT; break;
                            }
                        }
                        if (kind != CARD_KIND_FUSION) {
                            DeckEdit_StartCardMoveWide(DECK_FRAME.cursorCardFrame, DECKEDIT_LIST_SIDE_DECK, PSTATE + 0x1C20);
                        } else {
error_sound:
                            PlaySE(SE_ERROR);
                        }
                        break;
                    case DECKEDIT_CMD_LIST_FILTER:
                        gDeckEdit.exitMode = DECKEDIT_EXIT_LIST_FILTER;
                        FadeStartWide(0, 0x180, 0, PSTATE + 0x618);
                        /* FAKEMATCH: keep this transition call separate from the other menu cases. */
                        asm("");
                        goto confirm_sound;
                    case DECKEDIT_CMD_STATISTICS:
                        gDeckEdit.exitMode = DECKEDIT_EXIT_STATISTICS;
                        FadeStartWide(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case DECKEDIT_CMD_EXIT:
                        gDeckEdit.commandMenu.anim = CMDMENU_CLOSE;
                        FadeStartWide(0, 0x180, 0, PSTATE + 0x618);
                        gDeckEdit.exitMode = DECKEDIT_EXIT_LEAVE;
confirm_sound:
                        PlaySE(SE_CONFIRM);
                        goto draw_frame;
                    }
                    break;
                case B_BUTTON:
                    gDeckEdit.commandMenu.anim = CMDMENU_CLOSE;
                    PlaySE(SE_CANCEL);
                    break;
                default:
                    DeckEdit_HandleShoulderKeys(&gDeckEditCurList);
                    /* scrollEase.state (read through the curList alias) while a scroll runs */
                    if (*((u8 *)&gDeckEditCurList - 0x15F4) != TICK_RUNNING) {
                        switch (keys) {
                        case DPAD_UP: DeckEdit_ScrollListUp(&row); break;
                        case DPAD_DOWN: DeckEdit_ScrollListDown(&row); break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }

draw_frame:
    OamListAddSpriteGroupWide(gDeckEditArtFrameSprites, 5, 12, -1, -1, 3, 2, 0, 0, 0, 0, &gDeckEdit);
    DECK_FRAME.frameDirty = 1;
    DeckEdit_DrawScrollBarWide(gDeckEdit.listPos[gDeckEdit.curList], CNT(gDeckEdit.curList), (u16 *)&gDeckEdit.scrollBar);
    DeckEdit_DrawFrameSlotsWide(PSTATE + 0x1BBC, &gDeckEdit);
    for (slot = 1; slot <= 6; slot++)
        ObjAffineApplyWide(PSTATE + 0x18B0 + slot * 24);
    tail = gDeckEditCardMove;
    /* Matching: the rest of the frame reads gDeckEdit through the cardMove alias (tail):
     * tail - 4 = curList, tail - 3 = prevList, tail - 0x508 = anims, tail + 0x1C = commandMenu,
     * tail - 0x1C20 = &gDeckEdit, tail - 0x1608 = fade, tail[-0x1602] = fade.state,
     * tail[-0x15F8] = scrollEase.state, tail - 0x374 = brightness. */
    DeckEdit_UpdateCardMoveWide();
    list = tail - 4;
    DeckEdit_UpdatePanelHighlightWide(list, tail - 3, objects = tail - 0x508);
    DeckEdit_UpdateCommandMenuAnimWide(tail + 0x1C);
    DeckEdit_DrawCommandMenuWide(tail + 0x1C);
    OamListAddSpriteGroupWide(gDeckEditPanelCornerSprite, 0, 1, -1, -1, 0, 0, 1, 1, 0, 0, state = (struct DeckEditView *)(tail - 0x1C20));
    DeckEdit_DrawCardCounts(*list);
    AnimBlockTickWide(objects);
    AnimBlockDrawWide(objects, 0, 0, 0, 0, 0, 3, 0, 0, state);
    DeckEdit_UpdateNameIndexLetters();
    DeckEdit_DrawNameIndexTab();
    OamListFlushWide(state);
    OamListClearWide(state);
    FadeTickWide(tail - 0x1608);
    if (tail[-0x1602] == 2)
        goto complete;
    if (tail[-0x1602] == 3) {
        tail[-0x1602] = 0;
        REG_BLDCNT = 0x3FC8;
        REG_BLDALPHA = 0x1000;
        goto done;
    }
    goto fade;
complete:
    return 1;
fade:
    if (tail[-0x15F8] == 0 && tail[-0x1602] == 0) {
        if (*(s16 *)(tail - 0x374) >= 0) {
            REG_BLDCNT = 0x3FC8;
            REG_BLDY = *(s16 *)(tail - 0x374) >> 8;
        } else {
            REG_BLDCNT = 0x3F88;
            REG_BLDY = -*(s16 *)(tail - 0x374) >> 8;
        }
        if ((s16)gDeckEdit.brightness > 0x3FF)
            gDeckEdit.brightness = 0x400;
        else
            gDeckEdit.brightness += 0x30;
    }
done:
    return 0;
}

