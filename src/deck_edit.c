/*
 * deck_edit (0x0806ED44-0x0807093B): the Deck Edit scene and its three sibling pickers, all driven by the
 * same gDeckEdit list-view machinery (wiki/functions/deck-edit-c.md).
 *
 * Four step tables with one layout (enum DeckEditStep in deck_edit.h) run these screens:
 *   - gDeckEditSteps: the main-menu Deck Edit (CB_DeckEdit on gMain.seqIndex1).
 *   - gSideDeckSwapSteps: the Campaign side-deck swap (SideDeckSwap_Run on gMain.seqIndex1).
 *   - gTradeCardSelectSteps: the Card Trading picker (TradeCardSelect_Run on gMain.seqState2).
 *   - gProhibitCardSelectSteps: the Prohibition picker (ProhibitCardSelect_Run on gChain.targetWork).
 * Each table is stepped until its NULL entry; the step-3 *_SwitchScreen steps route on gDeckEdit.exitMode.
 * This unit holds the runners, two SwitchScreen variants, the shared scene init
 * (ProhibitCardSelect_Init doubles as the picker init), the list-view init with the prohibit banner
 * (ProhibitCardSelect_InitListView) and the picker per-frame handler (ProhibitCardSelect_Update, a twin
 * of DeckEdit_Update in deck_edit_view.c and TradeCardSelect_Update in deck_edit_prohibit.c).
 *
 * The shared structs and prototypes live in include/deck_edit.h; why this unit still declares local
 * views instead of including it is recorded in build/readability/issues/deck_edit.md.
 */
#include "global.h"
#include "legacy/gba.h"                    /* A_BUTTON, B_BUTTON, DPAD_*, REG_* */
#include "constants/sound.h"        /* SE_CURSOR, SE_CONFIRM, SE_CANCEL, SE_ERROR */

/* ---- BEGIN deck_edit.h stand-in (pre-H0) ----
 * include/deck_edit.h cannot be included here: it pulls in sprite.h, util.h and palette.h, whose
 * prototypes conflict with the wide caller views this unit's matched code uses. The enums below are
 * copied unchanged from include/deck_edit.h (and TickState from include/util.h) so the state machines
 * read semantically; swap to the real header at the H0 milestone.
 * (build/readability/issues/deck_edit.md) */
enum DeckEditStep { DECKEDIT_STEP_INIT = 0, DECKEDIT_STEP_ENTER_LIST_VIEW = 1, DECKEDIT_STEP_LIST_VIEW = 2, DECKEDIT_STEP_PROHIBIT_SELECT_OVER = 3, DECKEDIT_STEP_AWAIT_CHOICE = 4, DECKEDIT_STEP_LIST_FILTER = 5, DECKEDIT_STEP_RESET_LIST_VIEW = 6, DECKEDIT_STEP_REFILL_SUPPRESSED = 7, DECKEDIT_STEP_REFILL_CLEAR = 8, DECKEDIT_STEP_STATISTICS = 9, DECKEDIT_STEP_CARD_LIST_RUN = 10, DECKEDIT_STEP_SIDE_DECK_SWAP_OVER = 11, DECKEDIT_STEP_CARD_VIEW = 13, DECKEDIT_STEP_FADE_TICK = 14, DECKEDIT_STEP_TRADE_PICKER_OVER = 15 };
enum DeckEditExitMode { DECKEDIT_EXIT_LEAVE = 0, DECKEDIT_EXIT_LIST_FILTER = 1, DECKEDIT_EXIT_CARD_VIEW = 2, DECKEDIT_EXIT_STATISTICS = 3, DECKEDIT_EXIT_LIST_VIEW = 4 };
enum DeckEditCommand { DECKEDIT_CMD_CARD_VIEW = 0, DECKEDIT_CMD_TO_TRUNK = 1, DECKEDIT_CMD_TO_MAIN_DECK = 2, DECKEDIT_CMD_TO_SIDE_DECK = 3, DECKEDIT_CMD_LIST_FILTER = 4, DECKEDIT_CMD_STATISTICS = 5, DECKEDIT_CMD_EXIT = 6 };
enum CommandMenuAnim { CMDMENU_IDLE = 0, CMDMENU_OPEN = 1, CMDMENU_CLOSE = 2, CMDMENU_REFRESH = 3 };
enum DeckEditMenuVariant { DECKEDIT_MENU_BROWSE = 0, DECKEDIT_MENU_DECK_ENTRY = 1, DECKEDIT_MENU_PICK_CARD = 2 };
enum CardListMode { CARD_LIST_MODE_DECK_EDIT = 0, CARD_LIST_MODE_SIDE_DECK = 1, CARD_LIST_MODE_PROHIBIT = 2 };
enum DeckEditScrollDir { DECKEDIT_SCROLL_NONE = 0, DECKEDIT_SCROLL_UP = 1, DECKEDIT_SCROLL_DOWN = 2, DECKEDIT_SCROLL_PAGE_RIGHT = 3, DECKEDIT_SCROLL_PAGE_LEFT = 4 };
enum TickState { TICK_IDLE = 0, TICK_RUNNING = 1, TICK_DONE = 2 };
/* ---- END deck_edit.h stand-in ---- */

/* ---- Local views kept for matching (build/readability/issues/deck_edit.md) ---- */

/* gMain (0x03000040) as this unit reads it, a subset of include/main.h with main.h's names. The one
 * divergence is pickedCardId: main.h folds +0x4872 into unk4871[3], and this unit stores it as a
 * halfword, so the merged header cannot express it. */
struct Main {
    u8 unk0[6];
    u16 newKeys;                    /* +0x0006 */
    u8 unk8[0x40E - 0x8];
    u16 vblankFlags;                /* +0x040E */
    u8 unk410[0x4859 - 0x410];
    u8 seqIndex1;                   /* +0x4859: step into gDeckEditSteps / gSideDeckSwapSteps */
    u8 seqState1;                   /* +0x485A */
    u8 seqState2;                   /* +0x485B: step into gTradeCardSelectSteps */
    u8 unk485C[0x4872 - 0x485C];
    u16 pickedCardId;               /* +0x4872: card ID chosen in the list popup ('Decide') */
    u8 mode4874 : 2;                /* +0x4874 bits 0-1: enum CardListMode */
    u8 unk4874_2 : 6;
    u8 unk4875[0x4878 - 0x4875];
};
extern struct Main gMain;           /* 0x03000040 */

/* gDeckEdit (0x0201DB20) as this unit reads it, a subset of struct DeckEdit in include/deck_edit.h with
 * that header's field names. It stays a local view: deck_edit.h drags in sprite.h, whose prototypes
 * (OamListAddSpriteGroup, AnimBlockDraw, ...) differ from the wide views the matched calls here use. */
struct DeckEditView {
    u8 unk0[0x620];
    u16 listPos[3];                 /* +0x0620: selected card index per enum DeckEditList */
    u8 unk626[2];
    u8 tweenState;                  /* +0x0628: scrollEase.state */
    u8 unk629;
    s16 tweenStep;                  /* +0x062A: scrollEase.cur */
    u8 unk62C[4];
    u16 bg3Hofs;                    /* +0x0630 */
    u16 bg3Vofs;                    /* +0x0632 */
    u8 cardArtPage;                 /* +0x0634: card art double buffer 0/1 */
    u8 scrollDir;                   /* +0x0635: enum DeckEditScrollDir */
    u16 unk636;                     /* +0x0636 */
    u16 bg1Hofs;                    /* +0x0638 */
    u16 bg1Vofs;                    /* +0x063A */
    u16 bg0Hofs;                    /* +0x063C */
    u16 bg0Vofs;                    /* +0x063E */
    u8 unk640[0x1494 - 0x640];
    u16 listCount[2][3];            /* +0x1494: [row][list] number of cards */
    u8 listRow[3];                  /* +0x14A0: row shown per list (0 full, 1 filtered/sorted) */
    u8 unk14A3[0x1710 - 0x14A3];
    u8 redrawOnPageSlide : 1;       /* +0x1710 bit 0 */
    u8 unk1710_1 : 7;
    u8 unk1711[0x18AC - 0x1711];
    u16 brightness;                 /* +0x18AC: blend ramp, 0xFC00 after a redraw; read as s16 */
    u8 unk18AE[0x1BB0 - 0x18AE];
    u16 scrollBarBase;              /* +0x1BB0: &scrollBar, written as its thumbLen/thumbPos words */
    u16 unk1BB2;
    u8 upArrowDirty : 1;            /* +0x1BB4 bit 0 (scrollBar.upArrowDirty) */
    u8 unk1BB4_1 : 7;
    u8 upArrowFrame;                /* +0x1BB5 (scrollBar.upArrowFrame) */
    u8 downArrowDirty : 1;          /* +0x1BB6 bit 0 (scrollBar.downArrowDirty) */
    u8 unk1BB6_1 : 7;
    u8 downArrowFrame;              /* +0x1BB7 (scrollBar.downArrowFrame) */
    u8 unk1BB8[0x1C1C - 0x1BB8];
    u8 curList;                     /* +0x1C1C: enum DeckEditList shown */
    u8 prevList;                    /* +0x1C1D */
    u8 unk1C1E[0x1C48 - 0x1C1E];
    u8 menuOpen : 1;                /* +0x1C48 bit 0: command bar open, input goes to the bar */
    u8 exitMode : 4;                /* +0x1C48 bits 1-4: enum DeckEditExitMode */
    u8 unk1C48_5 : 3;
    u8 unk1C49[0x1C5A - 0x1C49];
    u8 menuVariant : 2;             /* +0x1C5A bits 0-1: enum DeckEditMenuVariant */
    u8 swapSelector : 3;            /* +0x1C5A bits 2-4: enum SideDeckSwapState */
    u8 sideSwapMode : 1;            /* +0x1C5A bit 5: side-deck swap running */
    u8 unk1C5A_6 : 2;
    u8 unk1C5B[3];
};
extern struct DeckEditView gDeckEdit;   /* 0x0201DB20 */
extern u16 gCardDetail;                 /* 0x02013D90 (card_detail.h): bit 0 cleared on the way to Card Detail */
extern u8 gDeckEditFade[];              /* = gDeckEdit.fade (0x0201E138) */
extern void FadeTick(void *p);
extern u16 DeckEdit_GetListCard(u8 list, u8 row, u16 col);
extern void CardDetail_Init(u16 id, int a, int b);
extern u16 (*const gDeckEditSteps[])(void);     /* 0x081A725C */

int DeckEdit_TickFadeIn(void)
{
    FadeTick(gDeckEditFade);
    if (gDeckEditFade[6] == 3)      /* Fade.state (+6) == FADE_DONE */
        return 1;
    return 0;
}
int DeckEdit_SwitchScreen(void)
{
    gMain.seqState1 = 0;
    /* Matching: the explicit case DECKEDIT_EXIT_LEAVE is load-bearing; the compiler drops the jump
     * table without it. */
    switch (gDeckEdit.exitMode) {
    case DECKEDIT_EXIT_LEAVE:
        return 1;
    case DECKEDIT_EXIT_LIST_FILTER:
        gMain.seqIndex1 = DECKEDIT_STEP_LIST_FILTER;
        return 0;
    case DECKEDIT_EXIT_CARD_VIEW:
        gMain.seqIndex1 = DECKEDIT_STEP_CARD_VIEW;
        gCardDetail &= 1;
        CardDetail_Init(DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]), 0, 0);
        return 0;
    case DECKEDIT_EXIT_STATISTICS:
        gMain.seqIndex1 = DECKEDIT_STEP_STATISTICS;
        return 0;
    case DECKEDIT_EXIT_LIST_VIEW:
        gMain.seqIndex1 = DECKEDIT_STEP_ENTER_LIST_VIEW;
        return 0;
    default:
        return 1;
    }
}
int TradeCardSelect_SwitchScreen(void)
{
    gMain.seqState1 = 0;
    /* Same routing as DeckEdit_SwitchScreen, writing the trade step byte (see the matching note there). */
    switch (gDeckEdit.exitMode) {
    case DECKEDIT_EXIT_LEAVE:
        return 1;
    case DECKEDIT_EXIT_LIST_FILTER:
        gMain.seqState2 = DECKEDIT_STEP_LIST_FILTER;
        return 0;
    case DECKEDIT_EXIT_CARD_VIEW:
        gMain.seqState2 = DECKEDIT_STEP_CARD_VIEW;
        gCardDetail &= 1;
        CardDetail_Init(DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]), 0, 0);
        return 0;
    case DECKEDIT_EXIT_STATISTICS:
        gMain.seqState2 = DECKEDIT_STEP_STATISTICS;
        return 0;
    case DECKEDIT_EXIT_LIST_VIEW:
        gMain.seqState2 = DECKEDIT_STEP_ENTER_LIST_VIEW;
        return 0;
    default:
        return 1;
    }
}
/* Deck Edit scene callback: runs step gDeckEditSteps[gMain.seqIndex1]; advances when it returns non-zero, returns 1 at the end of the table. */
u16 CB_DeckEdit(void)
{
    if (gMain.seqIndex1 == 0)
        gMain.mode4874 = CARD_LIST_MODE_DECK_EDIT;
    gDeckEdit.sideSwapMode = 0;
    if (gDeckEditSteps[gMain.seqIndex1] != 0) {
        if (gDeckEditSteps[gMain.seqIndex1]())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
extern u16 (*const gSideDeckSwapSteps[])(void);         /* 0x081A72A0 */
extern u16 (*const gTradeCardSelectSteps[])(void);      /* 0x081A72E4 */
extern u16 (*const gProhibitCardSelectSteps[])(void);   /* 0x081A7330 */
extern const u8 gStrDebugSwapSelectorChangedFmt[];
extern void DebugPrintf(const void *fmt, int a1, int a2);   /* debug.h; fixed-arity caller view */
extern void DebugPrintFlush(void);

u16 SideDeckSwap_Run(void)
{
    /* Matching: the step byte is held through a pointer alias of &gMain; the target reloads it after
     * the fn() call and bumps it through the pointer, which a direct gMain.seqIndex1 access loses. */
    struct Main *m = &gMain;
    u8 *stepPtr;
    struct Main *runner;
    u8 state;
    u16 (*fn)(void);
    u8 oldSelector;
    state = m->seqIndex1;
    runner = m;
    switch (state) {
    case 0:
        m->mode4874 = CARD_LIST_MODE_DECK_EDIT;
        break;
    case 1:
        gDeckEdit.sideSwapMode = 1;
        break;
    }
    fn = gSideDeckSwapSteps[*(stepPtr = &runner->seqIndex1)];
    if (fn != 0) {
        oldSelector = gDeckEdit.swapSelector;
        if (fn())
            (*stepPtr)++;
        if (oldSelector != gDeckEdit.swapSelector) {
            DebugPrintf(gStrDebugSwapSelectorChangedFmt, oldSelector, gDeckEdit.swapSelector);
            DebugPrintFlush();
        }
        return 0;
    }
    return 1;
} /* 0x0806EF74 size 0xA8 */
/* Runs step gTradeCardSelectSteps[gMain.seqState2]; advances on non-zero, returns 1 at the NULL end. */
u16 TradeCardSelect_Run(void)
{
    if (gTradeCardSelectSteps[gMain.seqState2] != 0) {
        if (gTradeCardSelectSteps[gMain.seqState2]())
            gMain.seqState2++;
        return 0;
    }
    return 1;
}
/* gChain (0x02017A40, include/chain.h) read as bytes; its targetWork byte (+0x3E6) is this runner's
 * step. The byte alias of the symbol is the matched form. */
extern u8 gChainBytes[] asm("gChain");
#define gChainTargetWork (gChainBytes[0x3E6])

/* Runs step gProhibitCardSelectSteps[gChain.targetWork]; advances on non-zero, returns 1 at the NULL end. */
u16 ProhibitCardSelect_Run(void)
{
    if (gChainTargetWork == 0)
        gMain.mode4874 = CARD_LIST_MODE_PROHIBIT;
    if (gProhibitCardSelectSteps[gChainTargetWork] != 0) {
        if (gProhibitCardSelectSteps[gChainTargetWork]())
            gChainTargetWork++;
        return 0;
    }
    return 1;
}
u16 ProhibitCardSelect_StartAndRun(void)
{
    /* Matching: the step byte is read once into v through a pointer base, then stored back through it;
     * direct gMain.seqState2 accesses lose the reload. */
    struct Main *m = &gMain;
    u8 *tradeStep = &m->seqState2;
    u8 v = *tradeStep;
    if (v == 0) {
        gChainTargetWork = v;
        (*tradeStep)++;
        return 0;
    }
    return ProhibitCardSelect_Run();
}
/* gDeckEditFrameSlots is &gDeckEdit.frameSlots (struct FrameSlotRing in deck_edit.h). The deck-edit code also
 * uses it as a base: cardMove at +0x68, the cursor-row byte at +0x7C (gDeckEdit +0x1C34), the command
 * menu at +0x84 (+0x1C3C), its animation byte at +0x85 (+0x1C3D), the +0x1C5A byte at +0xA2, and anims
 * at -0x4A0. The four small views below name those bytes. */
struct FrameSlotsView {
    u8 unk0[0x100];
};
/* gDeckEdit +0x1C34: cursor-row text page and row-name ring rotation. */
struct CursorRowByte {
    u8 cursorRowPage : 1;           /* bit 0 */
    u8 listRowRing : 4;             /* bits 1-4 */
    u8 unk1C34_5 : 3;
    u8 unk[7];
};
/* gDeckEdit +0x1C5A: command-bar variant and the side-deck swap selector. */
struct MenuVariantByte {
    u8 menuVariant : 2;             /* bits 0-1: enum DeckEditMenuVariant */
    u8 swapSelector : 3;            /* bits 2-4: enum SideDeckSwapState */
    u8 unk1C5A_5 : 3;
    u8 unk[7];
};
/* gDeckEdit +0x1C3C as one word (struct DeckEditCommandMenu). */
struct CommandMenuWord {
    u32 unk1C3C_0 : 15;
    u32 choice : 3;                 /* bits 15-17: enum DeckEditCommand */
    u32 unk1C3C_18 : 14;
    u8 unk[4];
};
/* gDeckEdit +0x1C3D: command-bar animation. */
struct CommandMenuAnimByte {
    u8 anim : 3;                    /* bits 0-2: enum CommandMenuAnim */
    u8 unk1C3D_3 : 5;
    u8 unk[7];
};
extern struct FrameSlotsView gDeckEditFrameSlots;     /* 0x0201F6D8 = &gDeckEdit.frameSlots */
/* gCardIdToNumber (0x08622AB4) read by literal address: indexing the array symbol directly changes
 * the target card-list loop's register allocation. */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
/* Callee views in the argument widths this unit's matched code uses; include/deck_edit.h (and
 * sprite.h, util.h, palette.h) have each definition's own types. */
void MemClear16(void *dst, u32 size);
void OamListClear(void *p);
void Ease_Init(int a, int b, int c, void *p);
void ClearKatakanaFlag(void *p);
void ObjAffineInit(void *p);
void DeckEdit_ResetFrameSlots(void *p);
void DeckEdit_ResetCardMove(void *p);
void AnimBlockInit(const void *a, void *b);
void DeckEdit_SetListCard(u16 val, u8 list, u8 row, u16 col);
void DeckEdit_CalcScrollBar(u16 a, u16 b, u16 *out);
extern const u8 gDeckEditAnimScripts[];

/* Deck Edit scene init: clears the state block, resets the BG scroll registers, builds the card lists. */
int ProhibitCardSelect_Init(void)
{
    u16 i;
    u16 j;
    struct CursorRowByte *cursorRow;
    struct MenuVariantByte *variantByte;
    struct CommandMenuWord *menuWord;
    MemClear16(&gDeckEdit, 0x1C5C);
    gMain.vblankFlags = 1;
    REG16(0x12) = 0;
    REG16(0x10) = 0;
    REG16(0x16) = 0;
    REG16(0x14) = 0;
    REG16(0x1A) = 0;
    REG16(0x18) = 0;
    REG16(0x1E) = 0;
    REG16(0x1C) = 0;
    REG16(0x28) = 0;
    REG16(0x2A) = 0;
    REG16(0x3C) = 0;
    REG16(0x3E) = 0;
    REG_DISPCNT &= 0xE0FF;
    OamListClear(&gDeckEdit);
    gDeckEdit.bg3Vofs = 0;
    gDeckEdit.bg3Hofs = 0;
    gDeckEdit.bg1Vofs = 0;
    gDeckEdit.bg1Hofs = 0;
    gDeckEdit.bg0Vofs = 0;
    gDeckEdit.bg0Hofs = 0;
    for (i = 0; i <= 2; i++) {
        gDeckEdit.listPos[i] = 0;
        gDeckEdit.listRow[i] = 0;
        for (j = 0; j <= 1; j++)
            gDeckEdit.listCount[j][i] = 0;
    }
    gDeckEdit.prevList = 0;
    gDeckEdit.curList = 0;
    gDeckEdit.cardArtPage = 0;
    gDeckEdit.scrollDir = 0;
    gDeckEdit.redrawOnPageSlide = 0;
    gDeckEdit.brightness = 0xFC00;
    Ease_Init(0, 0, 0, (u8 *)&gDeckEdit + 0x628);
    ClearKatakanaFlag((u8 *)&gDeckEdit + 0x640);
    ObjAffineInit((u8 *)&gDeckEdit + 0x18B0);
    for (i = 1; CARD_NUMBER(i) != 0xFFFF; ) {
        if ((u16)(CARD_NUMBER(i) - 0x76C) > 0x63) {
            DeckEdit_SetListCard(i, 0, gDeckEdit.listRow[0], gDeckEdit.listCount[gDeckEdit.listRow[0]][0]++);
        }
        i++;
        if (i > 0x334)
            break;
    }
    DeckEdit_CalcScrollBar(gDeckEdit.listCount[gDeckEdit.listRow[gDeckEdit.curList]][gDeckEdit.curList],
                 gDeckEdit.listPos[gDeckEdit.curList], &gDeckEdit.scrollBarBase);
    gDeckEdit.upArrowDirty = 1;
    gDeckEdit.downArrowDirty = 1;
    if (gDeckEdit.listCount[gDeckEdit.listRow[gDeckEdit.curList]][gDeckEdit.curList] > 5)
        gDeckEdit.downArrowFrame = gDeckEdit.upArrowFrame = 1;
    else
        gDeckEdit.downArrowFrame = gDeckEdit.upArrowFrame = 0;
    DeckEdit_ResetFrameSlots(&gDeckEditFrameSlots);
    DeckEdit_ResetCardMove((u8 *)&gDeckEditFrameSlots + 0x68);
    cursorRow = (struct CursorRowByte *)((u8 *)&gDeckEditFrameSlots + 0x7C);
    cursorRow->cursorRowPage = 0;
    cursorRow->listRowRing = 0;
    variantByte = (struct MenuVariantByte *)((u8 *)&gDeckEditFrameSlots + 0xA2);
    variantByte->menuVariant = DECKEDIT_MENU_PICK_CARD;
    menuWord = (struct CommandMenuWord *)((u8 *)cursorRow + 8);
    menuWord->choice = 0;
    ((struct CommandMenuAnimByte *)((u8 *)menuWord + 1))->anim = CMDMENU_REFRESH;
    variantByte->swapSelector = 0;
    AnimBlockInit(gDeckEditAnimScripts, (u8 *)&gDeckEditFrameSlots - 0x4A0);
    gMain.pickedCardId = 0;
    return 1;
}

void ProhibitCardSelect_UnusedNop(void)
{
}
extern const u8 gProhibitSelectFrameMap[], gProhibitSelectBgTiles[], gDeckEditLabelTiles[], gDeckEditObjTiles[], gDeckEditCardStackObjTiles[], gDeckEditCardIconObjTiles[], gDeckEditCardFrameObjTiles[];
extern const u8 gProhibitSelectBgPals4to7[], gProhibitSelectBgPals1to3[], gProhibitSelectBgPal3[], gDeckEditObjPal[], gDeckEditStatIconPal[];
extern u8 gDeckEditCurList;        /* = &gDeckEdit.curList, the picked list byte */
extern u16 gDeckEditListPos[];     /* = gDeckEdit.listPos, the cursor positions */
extern u8 gDeckEditObjAffine[];      /* = gDeckEdit.objAffine, used as the row-object base in InitListView */
void CopyMapRect(const void *src, void *dst, u32 w, u32 h);
void CopyTileSheetTo2D(const void *src, void *dst, u32 n);
void DeckEdit_LoadCardBoxTiles(u8 *dst);
void DeckEdit_LoadCardIconTiles(u8 *dst);
void DeckEdit_DrawListRowName(u16 idx, u8 *map, u16 col, u16 row, u32 unused, u8 slot);
void DeckEdit_InitFrameSlot(u8 slot, int x, u8 kind, u8 *base, u8 *arr);
void DeckEdit_DrawCursorRowName(u16 id, u8 *map, u16 col, u16 row, void *p);
void DeckEdit_DrawNoCardsText(u32 unused, u8 *map, u16 col, u16 row, void *p);
extern u16 DeckEdit_GetListCard(u8 list, u8 row, u16 col);
void LoadCardArt8bpp(u16 a, u32 b, u16 c);
void DeckEdit_PlaceCardArt(u8 a, u8 b, u8 c, u8 d);
void DeckEdit_DrawCardIcons(u16 pos);
void DeckEdit_DrawAtkDef(u8 *map, u16 col, u16 row, void *p);
void DeckEdit_DrawLevelStars(u8 *map, u16 col, u16 row, u8 perRow);
void FadeStart(u32 a, u32 b, u32 c, void *p);
void LoadDigitTiles(u8 *dst, u32 unused, u8 pal, int a, int b);
extern void CpuFastSet(const void *src, void *dst, u32 cnt);
extern void CpuSet(const void *src, void *dst, u32 cnt);
#define PSTATE ((u8 *)&gDeckEdit)
#define OBJS ((u8 *)&gDeckEdit + 0x1718)
struct FB { u8 f : 8; };
#define OBJF(off) (((struct FB *)(OBJS + (off)))->f)

/* Deck Edit card-list view init: clears VRAM, loads graphics and palettes, draws the visible rows of the list. */
int ProhibitCardSelect_InitListView(void)
{
    /* FAKEMATCH: promoted caller views preserve the ROM register arguments;
     * the existing callees decode their narrow values on entry. */
    extern u32 GetListCard_wide(u8, u8, int) asm("DeckEdit_GetListCard");
    extern void DrawListRowName_wide(int, u8 *, int, int, u32, int) asm("DeckEdit_DrawListRowName");
    extern void InitFrameSlot_wide(int, int, int, u8 *, u8 *) asm("DeckEdit_InitFrameSlot");
    extern void DrawCursorRowName_wide(int, u8 *, int, int, void *) asm("DeckEdit_DrawCursorRowName");
    extern void DrawNoCardsText_wide(int, u8 *, int, int, void *) asm("DeckEdit_DrawNoCardsText");
    /* FAKEMATCH: the staged array address preserves the second loop's register allocation. */
    u8 (*objectRows)[];
    extern void LoadCardArt8bpp_wide(int, u32, int) asm("LoadCardArt8bpp");
    extern void PlaceCardArt_wide(int, int, int, int) asm("DeckEdit_PlaceCardArt");
    vu32 z1;
    vu32 z2;
    u8 j;
    s16 k;
    /* FAKEMATCH: stage the next index after row/count loads to retain their allocation. */
    int next;
    z1 = 0;
    CpuFastSet((void *)&z1, (void *)0x06000000, 0x01004000);
    z2 = 0;
    CpuFastSet((void *)&z2, (void *)0x06010000, 0x01002000);
    CopyMapRect(gProhibitSelectFrameMap, (void *)0x0600E000, 0x1E, 0x14);
    CpuFastSet(gProhibitSelectBgTiles, (void *)0x06000000, 0x800);
    CpuFastSet(gDeckEditLabelTiles, (void *)0x06004000, 0x800);
    CopyTileSheetTo2D(gDeckEditObjTiles, (void *)0x06010000, 0x10);
    CopyTileSheetTo2D(gDeckEditCardStackObjTiles, (void *)0x06010200, 0x10);
    CopyTileSheetTo2D(gDeckEditCardIconObjTiles, (void *)0x06014000, 0x10);
    CopyTileSheetTo2D(gDeckEditCardFrameObjTiles, (void *)0x06014200, 0x10);
    DeckEdit_LoadCardBoxTiles((u8 *)0x06006000);
    AnimBlockInit(gDeckEditAnimScripts, OBJS);
    OBJF(0xE) |= 0xFF;
    OBJF(0x22) |= 0xFF;
    OBJF(0x36) |= 0xFF;
    OBJF(0x4A) |= 0xFF;
    OBJF(0x5E) |= 0xFF;
    OBJF(0x72) |= 0xFF;
    OBJF(0x86) |= 0xFF;
    OBJF(0x9A) |= 0xFF;
    OBJF(0xAE) |= 0xFF;
    OBJF(0xD6) |= 0xFF;
    OBJF(0xEA) |= 0xFF;
    OBJF(0xFE) |= 0xFF;
    OBJF(0x112) |= 0xFF;
    OBJF(0x126) |= 0xFF;
    objectRows = &gDeckEditObjAffine;
    OBJF(0x13A) |= 0xFF;
    OBJF(0x14E) |= 0xFF;
    CpuFastSet(gProhibitSelectBgPals4to7, (void *)0x05000080, 0x20);
    CpuFastSet(gProhibitSelectBgPals1to3, (void *)0x05000020, 0x18);
    CpuFastSet(gProhibitSelectBgPal3, (void *)0x05000060, 8);
    CpuSet(gDeckEditObjPal, (void *)0x05000200, 0x100);
    *(u16 *)0x05000044 = 0x7758;
    DeckEdit_LoadCardIconTiles((u8 *)0x06002000);
    CpuSet(gDeckEditStatIconPal, (void *)0x05000000, 0x10);
#define CURLIST gDeckEditCurList   /* picked list byte, alias of gDeckEdit.curList */
/* Number of cards in the row of list currently shown. */
#define CNT(list) (gDeckEdit.listCount[gDeckEdit.listRow[list]][list])
    k = 1;
    for (j = 0; j < 2; j++) {
        if ((s16)gDeckEdit.listPos[CURLIST] + (s16)k - 3 >= 0) {
            DrawListRowName_wide(GetListCard_wide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST] + (s16)k - 3),
                         (u8 *)0x0600D000, 0, ((s16)k - 1) * 2, (u32)(PSTATE + 0x640), -(s16)k + 2);
            InitFrameSlot_wide((s16)k - 1, GetListCard_wide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST] + (s16)k - 3),
                         (s16)k, PSTATE + 0x1BB8, *objectRows);
            k = (s16)k + 1;
        }
    }
    for (j = 0; j < 2; j = next) {
        int row = gDeckEditListPos[CURLIST] + j;
        int count = CNT(CURLIST) - 1;
        next = j + 1;
        if (row < count) {
            DrawListRowName_wide(GetListCard_wide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] + next),
                         (u8 *)0x0600D000, 0, j * 2 + 9, (u32)(PSTATE + 0x640), j + 4);
            InitFrameSlot_wide(j + 3, GetListCard_wide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] + j + 1),
                         j + 4, PSTATE + 0x1BB8, *objectRows);
        }
    }
    if (CNT(CURLIST) != 0) {
        DrawCursorRowName_wide(GetListCard_wide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST]),
                     (u8 *)0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
        InitFrameSlot_wide(2, GetListCard_wide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST]),
                     3, PSTATE + 0x1BB8, PSTATE + 0x18B0);
        LoadCardArt8bpp_wide(GetListCard_wide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST]),
                     0x06008000 + gDeckEdit.cardArtPage * 0x1680, gDeckEdit.cardArtPage);
        PlaceCardArt_wide(0x13, ((gDeckEdit.bg3Vofs & 0xFF) >> 3) + 2, gDeckEdit.cardArtPage, 1);
        DeckEdit_DrawCardIcons(0);
        DeckEdit_DrawAtkDef((u8 *)0x0600C000, 0xB, 7, PSTATE + 0x640);
        DeckEdit_DrawLevelStars((u8 *)0x0600C000, 0x11, 7, 6);
    } else {
        DrawNoCardsText_wide(GetListCard_wide(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEdit.listPos[CURLIST]),
                     (u8 *)0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
    }
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
    FadeStart(0, -0x180, 0, gDeckEditFade);
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = 0;
    REG_DISPCNT = 0x7F00;
    LoadDigitTiles((u8 *)0x06006000, 0x300, 1, 0, 0);
    return 1;
}
/* ProhibitCardSelect_Update is a near copy of the matched TradeCardSelect_Update (deck_edit_prohibit); the local
 * declarations below mirror that unit so the shared code compiles identically. */
/* Field views of gDeckEdit for the per-frame handler, with include/deck_edit.h's field names. */
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
    u8 frameDirty;              /* +0x17DA */
    u8 unk17DB[0x1866 - 0x17DB];
    s8 menuOwner;               /* +0x1866: which sprite group owns the command bar (-1 = none) */
    u8 unk1867[0x1BB4 - 0x1867];
    u8 upArrowDirty;            /* +0x1BB4 (scrollBar.upArrowDirty) */
    u8 upArrowFrame;            /* +0x1BB5 (scrollBar.upArrowFrame) */
    u8 downArrowDirty;          /* +0x1BB6 (scrollBar.downArrowDirty) */
    u8 downArrowFrame;          /* +0x1BB7 (scrollBar.downArrowFrame) */
    u8 frameSlotHead;           /* +0x1BB8: frameSlots.head */
    u8 unk1BB9[0x1C14 - 0x1BB9];
    u8 rowAnimState;            /* +0x1C14 */
    u8 unk1C15[0x1C20 - 0x1C15];
    u8 cardMoveStep;            /* +0x1C20: cardMove.step (enum CardMoveStep); 0 = idle */
    u8 unk1C21[0x1C3C - 0x1C21];
    u32 unk1C3C_0 : 15;         /* +0x1C3C: commandMenu timer/anim as bits */
    u16 menuChoice : 3;         /* bits 15-17: commandMenu.choice (enum DeckEditCommand) */
    u32 unk1C3C_18 : 14;
    u8 unk1C40[0x1C58 - 0x1C40];
    u16 rowAnimTimer;           /* +0x1C58 */
};
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
/* gDeckEdit.scrollEase (struct Ease) with the +0x635 scrollDir reached from the same base. */
struct ScrollEaseView {
    u8 state;                   /* +0x00 (DeckEditView +0x628): enum TickState */
    u8 unk1;
    s16 cur;                    /* +0x02: Ease.cur compared as s16 (step 4/3 for a page slide) */
    u8 unk4[9];
    u8 scrollDir;               /* +0x0D (DeckEditView +0x635): enum DeckEditScrollDir */
};
extern struct ScrollEaseView gDeckEditScrollEase;     /* 0x0201E148 = &gDeckEdit.scrollEase */
extern u8 gDeckEditCardMove[];                      /* 0x0201F740 = &gDeckEdit.cardMove */
extern const u16 gDeckEditEaseCurve[];          /* 0x080875D2 */
extern const u8 gDeckEditArtFrameSprites[], gDeckEditPanelCornerSprite[];   /* ROM sprite scripts (names unknown) */
void DeckEdit_DrawScrollBar(u16 position, u16 count, u16 *state);
void DeckEdit_TweenFrameSlots(s16 step, u8 tweenState, u8 direction, u8 *objects, u8 *rows);
void DeckEdit_DrawFrameSlots(u8 *rows, struct DeckEditView *state);
void DeckEdit_UpdateCardMove(void *tail);
void DeckEdit_UpdatePanelHighlight(u8 *list, u8 *row, void *objects);
void DeckEdit_DrawCommandMenu(void *state);
void DeckEdit_UpdateCommandMenuAnim(void *state);
void DeckEdit_ScrollListUp(u16 *position);
void DeckEdit_ScrollListDown(u16 *position);
void DeckEdit_DrawCardCounts(u8 list);
void SideDeckSwap_UpdateExchange(void);
void DeckEdit_UpdateNameIndexLetters(void);
void DeckEdit_DrawNameIndexTab(void);
void PlaySE(int sound);
void OamListAddSpriteGroup(const void *script, int a, int b, int c, int d, int e,
                 int f, int g, int h, int i, int j, void *state);
void AnimBlockDraw(void *objects, int a, int b, int c, int d, int e,
                 int f, int g, int h, void *state);
void AnimBlockTick(void *objects);

void FillMapRectWrap(int tile, u32 map, int col, int row, int width, int height, void *work);
void OamListFlush(void *state);

void Ease_Start(int from, int to, int increment, void *state);
void Ease_Tick(void *state);
void SetBldAlpha(u32 level);
s32 MulFix8(s32 scale, u16 value);
void ObjAffineApply(void *object);
#define DeckEdit_DrawListRowName ((void (*)(int, u8 *, int, int, u32, int))DeckEdit_DrawListRowName)
#define DeckEdit_InitFrameSlot ((void (*)(int, int, int, u8 *, u8 *))DeckEdit_InitFrameSlot)
#define DeckEdit_DrawCursorRowName ((void (*)(int, u8 *, int, int, void *))DeckEdit_DrawCursorRowName)
#define DeckEdit_DrawNoCardsText ((void (*)(int, u8 *, int, int, void *))DeckEdit_DrawNoCardsText)
#define LoadCardArt8bpp ((void (*)(int, u32, int))LoadCardArt8bpp)
#define DeckEdit_PlaceCardArt ((void (*)(int, int, int, int))DeckEdit_PlaceCardArt)
#define DeckEdit_DrawCardIcons ((void (*)(int))DeckEdit_DrawCardIcons)
#define DeckEdit_DrawAtkDef ((void (*)(u8 *, int, int, void *))DeckEdit_DrawAtkDef)
#define DeckEdit_DrawLevelStars ((void (*)(u8 *, int, int, int))DeckEdit_DrawLevelStars)

/* The unit's shared prototype narrows the column; the ROM passes it as a word. */
#define sub_08068D1C_word ((u32 (*)(u8, u8, int))DeckEdit_GetListCard)
/* CardSelect_HandleListSwitch is an empty function; the ROM still passes it the list pointer. */
void CardSelect_HandleListSwitch(void *list);
#define DECK_TWEEN_STATE DECK_FRAME.tweenState
#define DECK_TWEEN_STEP DECK_FRAME.tweenStep
#define DECK_SCROLL_CURVE gDeckEditEaseCurve[DECK_TWEEN_STEP]
#define DeckEdit_GetListCard sub_08068D1C_word

/* The command-bar animation byte (gDeckEdit +0x1C3D, enum CommandMenuAnim), reached as a struct
 * member: the offset-struct access shape is the matched form for the DECK_MENU_ANIM write. */
struct DeckAnimFields {
    u8 unk0[0x1C3D];
    u8 anim : 3;
    u8 unk1C3D_3 : 5;
};
#define DECK_MENU_ANIM (((struct DeckAnimFields *)&gDeckEdit)->anim)
/* The same byte through a plain pointer, for the FAKEMATCH-marked writes below. */
struct MenuAnimBits { u8 anim : 3; u8 unk1C3D_3 : 5; };
/* FAKEMATCH: same callee under other return types, so the three menu transition calls are not cross-jumped. */
#define FadeStart_asInt ((int (*)(u32, u32, u32, void *))FadeStart)
#define FadeStart_asU16 ((u16 (*)(u32, u32, u32, void *))FadeStart)
int ProhibitCardSelect_Update(void)
{
    u16 row;
    /* FAKEMATCH: keep the original fill slot and its stack-store scheduling. */
    volatile u32 clear;
    u32 keys;
    u8 slot;
    u8 horizontalOffset;
    u8 blend;
    /* FAKEMATCH: one phase-byte pointer for both menu arms (a global pseudo, like the ROM's r2). */
    u8 *pp;
    u8 *tail;
    u8 *objects;
    struct DeckEditView *state;

    keys = gMain.newKeys & 0x3FF;
    Ease_Tick(&gDeckEditScrollEase);
    /* Crossing the midpoint of a horizontal page slide changes the visible rows. */
    if ((*((u8 *)&gDeckEditScrollEase + 0x10E8) & 1) &&
        ((gDeckEditScrollEase.cur == 4 && gDeckEditScrollEase.scrollDir == 3) ||
         (gDeckEditScrollEase.cur == 3 && gDeckEditScrollEase.scrollDir == 4))) {
        DECK_FRAME.redrawOnPageSlide = 0;
        clear = 0;
        CpuFastSet((const void *)&clear, (void *)0x0600D000, 0x01000200);
        if ((s16)gDeckEditListPos[CURLIST] - 2 >= 0) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] - 2),
                         (u8 *)0x0600D000, 0, (u8)gDeckEdit.bg1Vofs >> 3, (u32)(PSTATE + 0x640), 1);
        }
        if ((s16)gDeckEditListPos[CURLIST] - 1 >= 0) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] - 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x10) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 2);
        }
        if ((s16)gDeckEditListPos[CURLIST] + 1 < CNT(CURLIST)) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] + 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x48) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 4);
        }
        if ((s16)gDeckEditListPos[CURLIST] + 2 < CNT(CURLIST)) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST] + 2),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x58) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 5);
        }
        FillMapRectWrap(0, 0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, 30, 6, PSTATE + 0x640);
        if (CNT(CURLIST) != 0) {
            DeckEdit_DrawCursorRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
            DeckEdit_DrawCardIcons(0);
            DeckEdit_DrawAtkDef((u8 *)0x0600C000, 11, ((gDeckEdit.bg0Vofs + 0x38) & 0xFF) >> 3, PSTATE + 0x640);
            DeckEdit_DrawLevelStars((u8 *)0x0600C000, 17, ((gDeckEdit.bg0Vofs + 0x38) & 0xFF) >> 3, 6);
        } else {
            DeckEdit_DrawNoCardsText(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gDeckEditListPos[CURLIST]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
        }
    }
    DeckEdit_TweenFrameSlots(DECK_TWEEN_STEP, DECK_TWEEN_STATE, gDeckEdit.scrollDir, PSTATE + 0x18B0, PSTATE + 0x1BB8);

    /* Directions UP/DOWN move vertically; PAGE_RIGHT/PAGE_LEFT move a five-card page horizontally. */
    switch (gDeckEdit.scrollDir) {
    case DECKEDIT_SCROLL_UP:
    case DECKEDIT_SCROLL_DOWN:
        switch (DECK_TWEEN_STATE) {
        case TICK_RUNNING:
            REG_BG3HOFS = gDeckEdit.bg3Hofs;
            REG_BG3VOFS = gDeckEdit.bg3Vofs + (MulFix8(0x5000, DECK_SCROLL_CURVE) >> 8);
            REG_BG1HOFS = gDeckEdit.bg1Hofs;
            REG_BG1VOFS = gDeckEdit.bg1Vofs + (MulFix8(0x1000, DECK_SCROLL_CURVE) >> 8);
            REG_BG0HOFS = gDeckEdit.bg0Hofs;
            REG_BG0VOFS = gDeckEdit.bg0Vofs + (MulFix8(0x2800, DECK_SCROLL_CURVE) >> 8);
            break;
        case TICK_DONE:
            DECK_TWEEN_STATE = TICK_IDLE;
            gDeckEdit.bg3Vofs += MulFix8(0x5000, DECK_SCROLL_CURVE) >> 8;
            gDeckEdit.bg1Vofs += MulFix8(0x1000, DECK_SCROLL_CURVE) >> 8;
            gDeckEdit.bg0Vofs += MulFix8(0x2800, DECK_SCROLL_CURVE) >> 8;
            if (DECK_FRAME.upArrowFrame) {
                DECK_FRAME.upArrowFrame = 1;
                DECK_FRAME.upArrowDirty |= 1;
            }
            if (DECK_FRAME.downArrowFrame) {
                DECK_FRAME.downArrowFrame = 1;
                DECK_FRAME.downArrowDirty |= 1;
            }
            gDeckEdit.scrollDir = 0;
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
            REG_BG3HOFS = gDeckEdit.bg3Hofs + (MulFix8(0x5000, DECK_SCROLL_CURVE) >> 8);
            REG_BG3VOFS = gDeckEdit.bg3Vofs;
            horizontalOffset = MulFix8(0x4000, DECK_SCROLL_CURVE) >> 8;
            blend = DECK_SCROLL_CURVE >> 3;
            REG_BLDCNT = 0x3F43;
            if (DECK_TWEEN_STEP <= 3) {
                REG_BG1HOFS = horizontalOffset;
                REG_BG1VOFS = gDeckEdit.bg1Vofs;
                REG_BG0HOFS = horizontalOffset;
                REG_BG0VOFS = gDeckEdit.bg0Vofs;
                SetBldAlpha(blend >> 1);
            } else {
                REG_BG1HOFS = horizontalOffset + 0xFFC0;
                REG_BG1VOFS = gDeckEdit.bg1Vofs;
                REG_BG0HOFS = horizontalOffset + 0xFFC0;
                REG_BG0VOFS = gDeckEdit.bg0Vofs;
                SetBldAlpha((0x20 - blend) >> 1);
            }
            break;
        case TICK_DONE:
            DECK_TWEEN_STATE = TICK_IDLE;
            gDeckEdit.bg3Hofs += MulFix8(0x5000, DECK_SCROLL_CURVE) >> 8;
            if (DECK_FRAME.upArrowFrame) {
                DECK_FRAME.upArrowFrame = 1;
                DECK_FRAME.upArrowDirty |= 1;
            }
            if (DECK_FRAME.downArrowFrame) {
                DECK_FRAME.downArrowFrame = 1;
                DECK_FRAME.downArrowDirty |= 1;
            }
            gDeckEdit.scrollDir = 0;
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
            if (DECK_TWEEN_STATE != 1) {
                switch (keys) {
                case B_BUTTON:
                    PlaySE(SE_ERROR);
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
                            Ease_Start(0, 6, 1, PSTATE + 0x628);
                            gDeckEdit.cardArtPage ^= 1;
                            LoadCardArt8bpp(DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]),
                                         0x06008000 + gDeckEdit.cardArtPage * 0x1680, gDeckEdit.cardArtPage);
                            DeckEdit_PlaceCardArt(((gDeckEdit.bg3Hofs & 0xFF) >> 3) + 29, ((gDeckEdit.bg3Vofs & 0xFF) >> 3) + 2, gDeckEdit.cardArtPage, 1);
                            gDeckEdit.scrollDir = 3;
                            DECK_FRAME.redrawOnPageSlide = 1;
                            DeckEdit_CalcScrollBar(CNT(gDeckEdit.curList), gDeckEdit.listPos[gDeckEdit.curList], &gDeckEdit.scrollBarBase);
                            if (DECK_FRAME.downArrowFrame) {
                                DECK_FRAME.downArrowFrame = 2;
                                DECK_FRAME.downArrowDirty |= 1;
                            }
                            row = gDeckEdit.listPos[gDeckEdit.curList] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < CNT(gDeckEdit.curList)) {
                                    DeckEdit_InitFrameSlot(slot, DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], row),
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
                            Ease_Start(6, 0, -1, PSTATE + 0x628);
                            gDeckEdit.cardArtPage ^= 1;
                            LoadCardArt8bpp(DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]),
                                         0x06008000 + gDeckEdit.cardArtPage * 0x1680, gDeckEdit.cardArtPage);
                            DeckEdit_PlaceCardArt(((gDeckEdit.bg3Hofs & 0xFF) >> 3) + 9, ((gDeckEdit.bg3Vofs & 0xFF) >> 3) + 2, gDeckEdit.cardArtPage, 1);
                            gDeckEdit.bg3Hofs -= 0x50;
                            gDeckEdit.scrollDir = 4;
                            DECK_FRAME.redrawOnPageSlide = 1;
                            DeckEdit_CalcScrollBar(CNT(gDeckEdit.curList), gDeckEdit.listPos[gDeckEdit.curList], &gDeckEdit.scrollBarBase);
                            if (DECK_FRAME.upArrowFrame) {
                                DECK_FRAME.upArrowFrame = 2;
                                DECK_FRAME.upArrowDirty |= 1;
                            }
                            row = gDeckEdit.listPos[gDeckEdit.curList] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < CNT(gDeckEdit.curList)) {
                                    DeckEdit_InitFrameSlot(slot, DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], row),
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
                DECK_MENU_ANIM = CMDMENU_OPEN;
                goto confirm_sound;
            }
            CardSelect_HandleListSwitch(&gDeckEditCurList);
            break;
        case 1:
            if (DECK_FRAME.cardMoveStep == 0 && DECK_FRAME.menuOwner == -1) {
                switch (keys) {
                case DPAD_RIGHT:
                    switch (++DECK_FRAME.menuChoice) {
                    case DECKEDIT_CMD_TO_TRUNK:
                        DECK_FRAME.menuChoice = DECKEDIT_CMD_LIST_FILTER;
                        break;
                    case 7:
                        DECK_FRAME.menuChoice = DECKEDIT_CMD_CARD_VIEW;
                        break;
                    case DECKEDIT_CMD_STATISTICS:
                        DECK_FRAME.menuChoice = DECKEDIT_CMD_EXIT;
                        break;
                    }
                    {
                        /* FAKEMATCH: r3 kept busy over the add, so the offset and byte reloads take r4/r5 */
                        register u32 busy asm("r3");
                        pp = (u8 *)&gDeckEdit;
                        asm("" : "=r"(busy));
                        pp += 0x1C3D;
                        asm("" : : "r"(busy));
                    }
                    ((struct MenuAnimBits *)pp)->anim = CMDMENU_REFRESH;
                    PlaySE(SE_CURSOR);
                    break;
                case DPAD_LEFT:
                    switch (DECK_FRAME.menuChoice) {
                    case DECKEDIT_CMD_LIST_FILTER:
                        DECK_FRAME.menuChoice = DECKEDIT_CMD_CARD_VIEW;
                        break;
                    case DECKEDIT_CMD_CARD_VIEW:
                        DECK_FRAME.menuChoice = DECKEDIT_CMD_EXIT;
                        break;
                    case DECKEDIT_CMD_EXIT:
                        DECK_FRAME.menuChoice = DECKEDIT_CMD_LIST_FILTER;
                        break;
                    default:
                        DECK_FRAME.menuChoice = (u16)(DECK_FRAME.menuChoice - 1);
                        break;
                    }
                    {
                        /* FAKEMATCH: r1 kept busy over the add, so the offset and byte reloads take r3/r4 */
                        register u32 busy asm("r1");
                        pp = (u8 *)&gDeckEdit;
                        asm("" : "=r"(busy));
                        pp += 0x1C3D;
                        asm("" : : "r"(busy));
                    }
                    ((struct MenuAnimBits *)pp)->anim = CMDMENU_REFRESH;
                    PlaySE(SE_CURSOR);
                    break;
                case A_BUTTON:
                    switch (DECK_FRAME.menuChoice) {
                    case DECKEDIT_CMD_CARD_VIEW:
                        gDeckEdit.exitMode = DECKEDIT_EXIT_CARD_VIEW;
                        FadeStart_asInt(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case DECKEDIT_CMD_LIST_FILTER:
                        gDeckEdit.exitMode = DECKEDIT_EXIT_LIST_FILTER;
                        FadeStart_asU16(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case DECKEDIT_CMD_STATISTICS:
                        gDeckEdit.exitMode = DECKEDIT_EXIT_STATISTICS;
                        FadeStart(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case DECKEDIT_CMD_EXIT:
                        if (DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]) != 0
                            && CNT(gDeckEdit.curList) != 0) {
                            gMain.pickedCardId = DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]);
                            FadeStart(0, 0x180, 0, PSTATE + 0x618);
                            gDeckEdit.exitMode = DECKEDIT_EXIT_LEAVE;
confirm_sound:
                            PlaySE(SE_CONFIRM);
                            goto draw_frame;
                        }
                        break;
                    }
                    break;
                case B_BUTTON:
                    ((struct MenuAnimBits *)(PSTATE + 0x1C3D))->anim = CMDMENU_CLOSE;
                    PlaySE(SE_CANCEL);
                    break;
                default:
                    CardSelect_HandleListSwitch(&gDeckEditCurList);
                    if (*((u8 *)&gDeckEditCurList - 0x15F4) != 1) {
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
    OamListAddSpriteGroup(gDeckEditArtFrameSprites, 5, 11, -1, -1, 3, 2, 0, 0, 0, 0, &gDeckEdit);
    DECK_FRAME.frameDirty = 1;
    DeckEdit_DrawScrollBar(gDeckEdit.listPos[gDeckEdit.curList], CNT(gDeckEdit.curList), &gDeckEdit.scrollBarBase);
    DeckEdit_DrawFrameSlots(PSTATE + 0x1BBC, &gDeckEdit);
    for (slot = 1; slot <= 6; slot++)
        ObjAffineApply(PSTATE + 0x18B0 + slot * 24);
    tail = gDeckEditCardMove;
    DeckEdit_UpdateCardMove(tail);
    DeckEdit_UpdateCommandMenuAnim(tail + 0x1C);
    DeckEdit_DrawCommandMenu(tail + 0x1C);
    OamListAddSpriteGroup(gDeckEditPanelCornerSprite, 0, 1, -1, -1, 0, 0, 1, 1, 0, 0, state = (struct DeckEditView *)(tail - 0x1C20));
    objects = tail - 0x508;
    AnimBlockTick(objects);
    AnimBlockDraw(objects, 0, 0, 0, 0, 0, 3, 0, 0, state);
    DeckEdit_UpdateNameIndexLetters();
    DeckEdit_DrawNameIndexTab();
    OamListFlush(state);
    OamListClear(state);
    FadeTick(tail - 0x1608);
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

