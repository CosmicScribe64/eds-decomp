/*
 * deck_edit_prohibit (0x0807093C-0x08071F3F): the exit step of the Prohibition picker and the Card
 * Trading picker's scene init and frame handler, all built on the shared gDeckEdit list view
 * (wiki/functions/deck-edit-prohibit-c.md).
 *
 *   - ProhibitCardSelect_SwitchScreen: step 3 of gProhibitCardSelectSteps. Like
 *     DeckEdit_SwitchScreen, but the next step goes into gChain.targetWork (the picker runs inside
 *     the duel chain's scratch bytes).
 *   - TradeCardSelect_Init: step 0 of gTradeCardSelectSteps. Clears the state block, resets the BG
 *     scroll registers, and builds the three card lists from the save trunk, filtered by
 *     gMain.cardListMode (enum CardListMode: the trading filter hides card numbers 1210-1899 and
 *     1920-1999, the deck-edit filter hides the token numbers 1920-1999).
 *   - TradeCardSelect_Update: step 2, the per-frame list and command-bar handler, a twin of
 *     DeckEdit_Update (deck_edit_view.c) and ProhibitCardSelect_Update (deck_edit.c).
 * CardSelect_HandleListSwitch is the pickers' empty R/L handler.
 *
 * The shared structs and prototypes live in include/deck_edit.h; why this unit still declares local
 * views instead of including it is recorded in build/readability/issues/deck_edit_prohibit.md.
 */
#include "global.h"
#include "gba.h"                    /* A_BUTTON, B_BUTTON, DPAD_*, REG_* */
#include "constants/sound.h"        /* SE_CURSOR, SE_CONFIRM, SE_CANCEL, SE_ERROR */

/* ---- BEGIN deck_edit.h stand-in (pre-H0) ----
 * include/deck_edit.h cannot be included here: it pulls in sprite.h, util.h and palette.h, whose
 * prototypes conflict with the wide caller views this unit's matched code uses. The enums below are
 * copied unchanged from include/deck_edit.h (and TickState from include/util.h) so the state machines
 * read semantically; swap to the real header at the H0 milestone.
 * (build/readability/issues/deck_edit_prohibit.md) */
enum DeckEditStep { DECKEDIT_STEP_INIT = 0, DECKEDIT_STEP_ENTER_LIST_VIEW = 1, DECKEDIT_STEP_UPDATE = 2, DECKEDIT_STEP_SWITCH_SCREEN = 3, DECKEDIT_STEP_END = 4, DECKEDIT_STEP_LIST_FILTER = 5, DECKEDIT_STEP_STATISTICS = 9, DECKEDIT_STEP_CARD_VIEW = 13 };
enum DeckEditExitMode { DECKEDIT_EXIT_LEAVE = 0, DECKEDIT_EXIT_LIST_FILTER = 1, DECKEDIT_EXIT_CARD_VIEW = 2, DECKEDIT_EXIT_STATISTICS = 3, DECKEDIT_EXIT_LIST_VIEW = 4 };
enum DeckEditCommand { DECKEDIT_CMD_CARD_VIEW = 0, DECKEDIT_CMD_TO_TRUNK = 1, DECKEDIT_CMD_TO_MAIN_DECK = 2, DECKEDIT_CMD_TO_SIDE_DECK = 3, DECKEDIT_CMD_LIST_FILTER = 4, DECKEDIT_CMD_STATISTICS = 5, DECKEDIT_CMD_EXIT = 6 };
enum CommandMenuAnim { CMDMENU_IDLE = 0, CMDMENU_OPEN = 1, CMDMENU_CLOSE = 2, CMDMENU_REFRESH = 3 };
enum DeckEditMenuVariant { DECKEDIT_MENU_EDIT = 0, DECKEDIT_MENU_SIDE_SWAP = 1, DECKEDIT_MENU_PICK_CARD = 2 };
enum CardListMode { CARD_LIST_MODE_DECK_EDIT = 0, CARD_LIST_MODE_TRADE = 1, CARD_LIST_MODE_PROHIBIT = 2 };
enum DeckEditScrollDir { DECKEDIT_SCROLL_NONE = 0, DECKEDIT_SCROLL_UP = 1, DECKEDIT_SCROLL_DOWN = 2, DECKEDIT_SCROLL_PAGE_RIGHT = 3, DECKEDIT_SCROLL_PAGE_LEFT = 4 };
enum TickState { TICK_IDLE = 0, TICK_RUNNING = 1, TICK_DONE = 2 };
/* ---- END deck_edit.h stand-in ---- */

/* ---- Local views kept for matching (build/readability/issues/deck_edit_prohibit.md) ---- */

/* gMain (0x03000040) as this unit reads it, a subset of include/main.h with main.h's names. The one
 * divergence is pickedCardId: main.h folds +0x4872 into unk4871[3], and this unit stores it as a
 * halfword, so the merged header cannot express it. */
struct Main {
    u8 unk0[6];
    u16 newKeys;                    /* +0x0006 */
    u8 unk8[0x40E - 0x8];
    u16 vblankFlags;                /* +0x040E */
    u8 unk410[0x485A - 0x410];
    u8 seqState1;                   /* +0x485A */
    u8 unk485B[0x4872 - 0x485B];
    u16 pickedCardId;               /* +0x4872: card ID chosen in the list popup ('Decide') */
    u8 mode4874 : 2;                /* +0x4874 bits 0-1: enum CardListMode */
    u8 unk4874_2 : 6;
    u8 unk4875[0x4878 - 0x4875];
};
extern struct Main gMain;           /* 0x03000040 */

/* gChain (0x02017A40, include/chain.h) read as bytes; its targetWork byte (+0x3E6) is the Prohibition
 * picker's step, targetWork2 (+0x3E7) the second scratch byte the switch screen clears. The byte
 * alias of the symbol is the matched form. */
extern u8 gChainBytes[] asm("gChain");
#define gChainTargetWork (gChainBytes[0x3E6])
#define gChainTargetWork2 (gChainBytes[0x3E7])

/* gDeckEdit (0x0201DB20) as this unit reads it, a subset of struct DeckEdit in include/deck_edit.h
 * with that header's field names. It stays a local view: deck_edit.h drags in sprite.h, whose
 * prototypes (OamListAddSpriteGroup, AnimBlockDraw, ...) differ from the wide views the matched
 * calls here use. */
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

/* gUnk_0201F6D8 is &gDeckEdit.frameSlots (struct FrameSlotRing in deck_edit.h). The deck-edit code
 * also uses it as a base: cardMove at +0x68, the cursor-row byte at +0x7C (gDeckEdit +0x1C34), the
 * command menu at +0x84 (+0x1C3C), its animation byte at +0x85 (+0x1C3D), the +0x1C5A byte at +0xA2,
 * and anims at -0x4A0. The four small views below name those bytes. */
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
extern struct FrameSlotsView gUnk_0201F6D8;     /* 0x0201F6D8 = &gDeckEdit.frameSlots */

/* One card of the collection (struct TrunkEntry in include/save.h, gSaveData + 8, one entry per
 * card ID) as this unit tests it: the copy counts are read with byte loads, so the 2-bit fields
 * keep u8 containers where save.h uses u16 ones. */
struct TrunkEntry {
    u16 count : 10;                 /* bits 0-9: copies in the trunk */
    u8 deckCopies : 2;              /* +0x1 bits 2-3: copies in the saved Deck */
    u8 sideCopies : 2;              /* +0x1 bits 4-5: copies in the saved Side Deck */
    u8 fusionCopies : 2;            /* +0x1 bits 6-7: copies in the saved Fusion Deck */
};
struct TrunkView {
    u8 unk0[8];
    struct TrunkEntry trunk[1];
};
extern struct TrunkView gSaveData;  /* 0x02011C20 */

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
void DeckEdit_CountSideDeckMonsters(void);
extern const u8 gDeckEditAnimScripts[];         /* 0x081A70FC */
extern u16 gCardDetail;                 /* 0x02013D90 (card_detail.h): bit 0 cleared on the way to Card Detail */
extern u16 DeckEdit_GetListCard(u8 list, u8 row, u16 col);
extern void CardDetail_Init(u16 id, int a, int b);

/* Deck Edit exit: like DeckEdit_SwitchScreen but the step goes into gChain.targetWork. */
int ProhibitCardSelect_SwitchScreen(void)
{
    gMain.seqState1 = 0;
    gChainTargetWork2 = 0;
    /* Matching: the explicit case DECKEDIT_EXIT_LEAVE is load-bearing; the compiler drops the jump
     * table without it. */
    switch (gDeckEdit.exitMode) {
    case DECKEDIT_EXIT_LEAVE:
        return 1;
    case DECKEDIT_EXIT_LIST_FILTER:
        gChainTargetWork = DECKEDIT_STEP_LIST_FILTER;
        return 0;
    case DECKEDIT_EXIT_CARD_VIEW:
        gChainTargetWork = DECKEDIT_STEP_CARD_VIEW;
        gCardDetail &= 1;
        CardDetail_Init(DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]), 0, 0);
        return 0;
    case DECKEDIT_EXIT_STATISTICS:
        gChainTargetWork = DECKEDIT_STEP_STATISTICS;
        return 0;
    case DECKEDIT_EXIT_LIST_VIEW:
        gChainTargetWork = DECKEDIT_STEP_ENTER_LIST_VIEW;
        return 0;
    default:
        return 1;
    }
}

/* Card Trading scene init (the DeckEdit_Init variant whose card lists are filtered by
 * gMain.cardListMode): clears the state block, resets BG scroll, builds the three card lists. */
int TradeCardSelect_Init(void)
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
    switch (gMain.mode4874) {
    /* Matching: the two-case switch lists CARD_LIST_MODE_TRADE first so the ROM's block order
     * (mode 1 code first) comes out. */
    case CARD_LIST_MODE_TRADE:
        for (i = 1; i <= 0x334 && CARD_NUMBER(i) != 0xFFFF; i++) {
            if ((u16)(CARD_NUMBER(i) - 0x4BA) <= 0x315 && (u16)(CARD_NUMBER(i) - 0x76C) > 0x13)
                continue;
            if (gSaveData.trunk[i].count)
                DeckEdit_SetListCard(i, 0, gDeckEdit.listRow[0], gDeckEdit.listCount[gDeckEdit.listRow[0]][0]++);
            if (gSaveData.trunk[i].deckCopies || gSaveData.trunk[i].fusionCopies)
                DeckEdit_SetListCard(i, 1, gDeckEdit.listRow[1], gDeckEdit.listCount[gDeckEdit.listRow[1]][1]++);
            if (gSaveData.trunk[i].sideCopies)
                DeckEdit_SetListCard(i, 2, gDeckEdit.listRow[2], gDeckEdit.listCount[gDeckEdit.listRow[2]][2]++);
        }
        break;
    case CARD_LIST_MODE_DECK_EDIT:
        for (i = 1; i <= 0x334 && CARD_NUMBER(i) != 0xFFFF; i++) {
            if ((u16)(CARD_NUMBER(i) - 0x780) > 0x4F) {
                if (gSaveData.trunk[i].count)
                    DeckEdit_SetListCard(i, 0, gDeckEdit.listRow[0], gDeckEdit.listCount[gDeckEdit.listRow[0]][0]++);
                if (gSaveData.trunk[i].deckCopies || gSaveData.trunk[i].fusionCopies)
                    DeckEdit_SetListCard(i, 1, gDeckEdit.listRow[1], gDeckEdit.listCount[gDeckEdit.listRow[1]][1]++);
                if (gSaveData.trunk[i].sideCopies)
                    DeckEdit_SetListCard(i, 2, gDeckEdit.listRow[2], gDeckEdit.listCount[gDeckEdit.listRow[2]][2]++);
            }
        }
        break;
    }
    DeckEdit_CountSideDeckMonsters();
    DeckEdit_CalcScrollBar(gDeckEdit.listCount[gDeckEdit.listRow[gDeckEdit.curList]][gDeckEdit.curList],
                 gDeckEdit.listPos[gDeckEdit.curList], &gDeckEdit.scrollBarBase);
    gDeckEdit.upArrowDirty = 1;
    gDeckEdit.downArrowDirty = 1;
    if (gDeckEdit.listCount[gDeckEdit.listRow[gDeckEdit.curList]][gDeckEdit.curList] > 5)
        gDeckEdit.downArrowFrame = gDeckEdit.upArrowFrame = 1;
    else
        gDeckEdit.downArrowFrame = gDeckEdit.upArrowFrame = 0;
    DeckEdit_ResetFrameSlots(&gUnk_0201F6D8);
    DeckEdit_ResetCardMove((u8 *)&gUnk_0201F6D8 + 0x68);
    cursorRow = (struct CursorRowByte *)((u8 *)&gUnk_0201F6D8 + 0x7C);
    cursorRow->cursorRowPage = 0;
    cursorRow->listRowRing = 0;
    variantByte = (struct MenuVariantByte *)((u8 *)&gUnk_0201F6D8 + 0xA2);
    variantByte->menuVariant = DECKEDIT_MENU_PICK_CARD;
    menuWord = (struct CommandMenuWord *)((u8 *)cursorRow + 8);
    menuWord->choice = 0;
    ((struct CommandMenuAnimByte *)((u8 *)menuWord + 1))->anim = CMDMENU_REFRESH;
    variantByte->swapSelector = 0;
    AnimBlockInit(gDeckEditAnimScripts, (u8 *)&gUnk_0201F6D8 - 0x4A0);
    gMain.pickedCardId = 0;
    return 1;
}

void CardSelect_HandleListSwitch(void)
{
}
/* TradeCardSelect_Update is a near copy of the matched DeckEdit_Update (deck_edit_view); the local
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
/* gDeckEdit +0x1C3C read as one word (the choice bits are tested as a mask). */
struct CommandMenuRawView {
    u8 unk0[0x1C3C];
    u32 word;
};
#define DECK_MENU_RAW (((struct CommandMenuRawView *)&gDeckEdit)->word)
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
extern struct ScrollEaseView gUnk_0201E148;     /* 0x0201E148 = &gDeckEdit.scrollEase */
extern u8 gUnk_0201F740[];                      /* 0x0201F740 = &gDeckEdit.cardMove */
extern u8 gUnk_0201F73C;                        /* 0x0201F73C = &gDeckEdit.curList */
extern u16 gUnk_0201E140[];                     /* 0x0201E140 = gDeckEdit.listPos */
extern const u16 gDeckEditEaseCurve[];          /* 0x080875D2 */
extern const u8 gUnk_081A6524[], gUnk_081A6EA4[];   /* ROM sprite scripts (names unknown) */
void DeckEdit_DrawScrollBar(u16 position, u16 count, u16 *state);
void DeckEdit_TweenFrameSlots(s16 step, u8 tweenState, u8 direction, u8 *objects, u8 *rows);
void DeckEdit_DrawFrameSlots(u8 *rows, struct DeckEditView *state);
void DeckEdit_UpdateCardMove(void);
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
void FadeTick(void *state);
void FillMapRectWrap(int tile, u32 map, int col, int row, int width, int height, void *work);
void OamListFlush(void *state);
void OamListClear(void *state);
void Ease_Start(int from, int to, int increment, void *state);
void Ease_Tick(void *state);
void SetBldAlpha(u32 level);
s32 MulFix8(s32 scale, u16 value);
void ObjAffineApply(void *object);
void DeckEdit_DrawListRowName(int idx, u8 *map, int col, int row, u32 unused, int slot);
void DeckEdit_InitFrameSlot(int slot, int x, int kind, u8 *base, u8 *arr);
void DeckEdit_DrawCursorRowName(int id, u8 *map, int col, int row, void *p);
void DeckEdit_DrawNoCardsText(int unused, u8 *map, int col, int row, void *p);
void LoadCardArt8bpp(int a, u32 b, int c);
void DeckEdit_PlaceCardArt(int a, int b, int c, int d);
void DeckEdit_DrawCardIcons(int pos);
void DeckEdit_DrawAtkDef(u8 *map, int col, int row, void *p);
void DeckEdit_DrawLevelStars(u8 *map, int col, int row, int perRow);
void FadeStart(u32 a, u32 b, u32 c, void *p);
/* The unit's shared prototype narrows the column; the ROM passes it as a word. */
extern u32 sub_08068D1C_word(u8 list, u8 row, int col) __asm__("DeckEdit_GetListCard");
/* CardSelect_HandleListSwitch is the empty function above; the ROM still passes it the list pointer. */
void sub_08070F14_arg(void *) __asm__("CardSelect_HandleListSwitch");
#define PSTATE ((u8 *)&gDeckEdit)
#define CURLIST gUnk_0201F73C
#define CNT(list) (gDeckEdit.listCount[gDeckEdit.listRow[list]][list])
#define DECK_TWEEN_STATE DECK_FRAME.tweenState
#define DECK_TWEEN_STEP DECK_FRAME.tweenStep
#define DECK_SCROLL_CURVE gDeckEditEaseCurve[DECK_TWEEN_STEP]
#define DeckEdit_GetListCard sub_08068D1C_word
#define CardSelect_HandleListSwitch sub_08070F14_arg

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
/* Card Trading card-list frame: animate, handle list/menu input, draw, and fade. */
int TradeCardSelect_Update(void)
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
    u8 *list;
    u8 *objects;
    struct DeckEditView *state;

    keys = gMain.newKeys & 0x3FF;
    Ease_Tick(&gUnk_0201E148);
    /* Crossing the midpoint of a horizontal page slide changes the visible rows. */
    if ((*((u8 *)&gUnk_0201E148 + 0x10E8) & 1) &&
        ((gUnk_0201E148.cur == 4 && gUnk_0201E148.scrollDir == 3) ||
         (gUnk_0201E148.cur == 3 && gUnk_0201E148.scrollDir == 4))) {
        DECK_FRAME.redrawOnPageSlide = 0;
        clear = 0;
        CpuFastSet((const void *)&clear, (void *)0x0600D000, 0x01000200);
        if ((s16)gUnk_0201E140[CURLIST] - 2 >= 0) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gUnk_0201E140[CURLIST] - 2),
                         (u8 *)0x0600D000, 0, (u8)gDeckEdit.bg1Vofs >> 3, (u32)(PSTATE + 0x640), 1);
        }
        if ((s16)gUnk_0201E140[CURLIST] - 1 >= 0) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gUnk_0201E140[CURLIST] - 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x10) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 2);
        }
        if ((s16)gUnk_0201E140[CURLIST] + 1 < CNT(CURLIST)) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gUnk_0201E140[CURLIST] + 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x48) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 4);
        }
        if ((s16)gUnk_0201E140[CURLIST] + 2 < CNT(CURLIST)) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gUnk_0201E140[CURLIST] + 2),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x58) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 5);
        }
        FillMapRectWrap(0, 0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, 30, 6, PSTATE + 0x640);
        if (CNT(CURLIST) != 0) {
            DeckEdit_DrawCursorRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gUnk_0201E140[CURLIST]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
            DeckEdit_DrawCardIcons(0);
            DeckEdit_DrawAtkDef((u8 *)0x0600C000, 11, ((gDeckEdit.bg0Vofs + 0x38) & 0xFF) >> 3, PSTATE + 0x640);
            DeckEdit_DrawLevelStars((u8 *)0x0600C000, 17, ((gDeckEdit.bg0Vofs + 0x38) & 0xFF) >> 3, 6);
        } else {
            DeckEdit_DrawNoCardsText(DeckEdit_GetListCard(CURLIST, gDeckEdit.listRow[CURLIST], gUnk_0201E140[CURLIST]),
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
                    FadeStart(0, 0x180, 0, PSTATE + 0x618);
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
                            Ease_Start(0, 6, 1, PSTATE + 0x628);
                            gDeckEdit.cardArtPage ^= 1;
                            LoadCardArt8bpp(DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]),
                                         0x06008000 + gDeckEdit.cardArtPage * 0x1680, gDeckEdit.cardArtPage);
                            DeckEdit_PlaceCardArt(((gDeckEdit.bg3Hofs & 0xFF) >> 3) + 29, ((gDeckEdit.bg3Vofs & 0xFF) >> 3) + 2, gDeckEdit.cardArtPage, 1);
                            gDeckEdit.scrollDir = DECKEDIT_SCROLL_PAGE_RIGHT;
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
                            gDeckEdit.scrollDir = DECKEDIT_SCROLL_PAGE_LEFT;
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
            CardSelect_HandleListSwitch(&gUnk_0201F73C);
            break;
        case 1:
            if (DECK_FRAME.cardMoveStep == 0 && DECK_FRAME.menuOwner == -1) {
                switch (keys) {
                case DPAD_RIGHT:
                    if (++DECK_FRAME.menuChoice == DECKEDIT_CMD_TO_TRUNK)
                        DECK_FRAME.menuChoice = DECKEDIT_CMD_LIST_FILTER;
                    else if ((DECK_MENU_RAW & 0x38000) == 0x38000)
                        DECK_FRAME.menuChoice = DECKEDIT_CMD_CARD_VIEW;
                    {
                        /* FAKEMATCH: r5 stays busy over the add so the offset reload takes r0 */
                        register u32 busy asm("r5");
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
                    default:
                        DECK_FRAME.menuChoice = (u16)(DECK_FRAME.menuChoice - 1);
                        break;
                    }
                    {
                        /* FAKEMATCH: r4 stays busy over the add so the offset reload takes r5 */
                        register u32 busy asm("r4");
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
                        if (CNT(gDeckEdit.curList) == 0)
                            goto error_sound;
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
error_sound:
                        PlaySE(SE_ERROR);
                        break;
                    }
                    break;
                case B_BUTTON:
                    ((struct MenuAnimBits *)(PSTATE + 0x1C3D))->anim = CMDMENU_CLOSE;
                    PlaySE(SE_CANCEL);
                    break;
                default:
                    CardSelect_HandleListSwitch(&gUnk_0201F73C);
                    if (*((u8 *)&gUnk_0201F73C - 0x15F4) != 1) {
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
    SideDeckSwap_UpdateExchange();
    OamListAddSpriteGroup(gUnk_081A6524, 5, 12, -1, -1, 3, 2, 0, 0, 0, 0, &gDeckEdit);
    DECK_FRAME.frameDirty = 1;
    DeckEdit_DrawScrollBar(gDeckEdit.listPos[gDeckEdit.curList], CNT(gDeckEdit.curList), &gDeckEdit.scrollBarBase);
    DeckEdit_DrawFrameSlots(PSTATE + 0x1BBC, &gDeckEdit);
    for (slot = 1; slot <= 6; slot++)
        ObjAffineApply(PSTATE + 0x18B0 + slot * 24);
    tail = gUnk_0201F740;
    DeckEdit_UpdateCardMove();
    list = tail - 4;
    DeckEdit_UpdatePanelHighlight(list, tail - 3, objects = tail - 0x508);
    DeckEdit_UpdateCommandMenuAnim(tail + 0x1C);
    DeckEdit_DrawCommandMenu(tail + 0x1C);
    OamListAddSpriteGroup(gUnk_081A6EA4, 0, 1, -1, -1, 0, 0, 1, 1, 0, 0, state = (struct DeckEditView *)(tail - 0x1C20));
    DeckEdit_DrawCardCounts(*list);
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

