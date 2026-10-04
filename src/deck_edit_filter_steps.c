/*
 * deck_edit_filter_steps (0x0806A92C-0x0806C4E4): the List Filter step runners and the Campaign
 * side-deck swap screen (wiki/functions/deck-edit-filter-steps-c.md).
 *
 * DeckEdit_RunListFilter and ProhibitCardSelect_RunListFilter run the shared gListFilterSteps table
 * (step byte gMain.seqState1 for Deck Edit, gChain.targetWork2 for the Prohibition picker) and return
 * 1 once the table's NULL entry is reached. The rest of the unit is the side-deck swap: the player
 * picks a card in the Main Deck list and one in the Side Deck list and the two trade places.
 * SideDeckSwap_Init builds the two lists from gSaveData.trunk (card numbers 1920-1999, the fusion
 * monsters, stay out), SideDeckSwap_Update is the per-frame list/menu handler (a near copy of
 * TradeCardSelect_Update in deck_edit_prohibit.c), and SideDeckSwap_UpdateExchange runs the
 * swapSelector state machine that commits the exchange in SideDeckSwap_ExchangeCards.
 */
#include "global.h"
#include "legacy/gba.h"          /* A_BUTTON .. L_BUTTON, REG_DISPCNT, REG_BG0HOFS .. REG_BG3VOFS, REG_BLDCNT */
#include "legacy/main.h"         /* struct Main gMain (newKeys, seqState1, vblankFlags) */
#include "chain.h"        /* struct ChainState gChain (targetWork2) */
#include "save.h"         /* struct SaveData gSaveData, AddCardToSavedDeck / SideDeck / FusionDeck, Remove... */
#include "bg.h"           /* FillMapRectWrap, LoadCardArt8bpp */
#include "deck_edit.h"    /* struct DeckEdit gDeckEdit, enum DeckEditList / DeckEditSlide / SideDeckSwapState,
                               the DeckEdit_* / SideDeckSwap_* prototypes */

/* List Filter sub-screen step table (served by the two runners above). */
extern u16 (*const gListFilterSteps[])(void);   /* 0x081A723C */

/* sound.h does not declare the staged PlaySE (see duel_response.c). */
void PlaySE(u32 seId);

void ResetBgScroll(void);

/* ---- Local views and aliases kept for matching ---- */

/* Byte view of one gSaveData.trunk entry. The trunk sits at gSaveData + 8, so the copy-count byte of
 * entry i is at gSaveData + 4*i + 9. The namesake in save.h (struct TrunkEntry) reads the same bits
 * through a u16 container; the ROM reads them from the byte, and the paired shifts of the two getters
 * below are the matched form. */
struct TrunkEntryByteView {
    u8 unk0[9];
    u8 countHigh:2;     /* +9 bits 0-1: high bits of the u16 copy count (not read here) */
    u8 deckCopies:2;    /* +9 bits 2-3: copies in the saved Deck */
    u8 sideCopies:2;    /* +9 bits 4-5: copies in the saved Side Deck */
    u8 fusionCopies:2;  /* +9 bits 6-7: copies in the saved Fusion Deck */
    u8 unkA[2];
};

/* Return the extracted count before testing it, preserving both bit reads. */
static inline int TrunkDeckCopies(struct TrunkEntryByteView *entry)
{
    return entry->deckCopies;
}

static inline int TrunkSideCopies(struct TrunkEntryByteView *entry)
{
    return entry->sideCopies;
}

/* Matching: this unit declares DeckEdit_GetListCard with a full-width result; the search in
 * SideDeckSwap_FindCardInList compares it directly (deck_edit.h declares the u16 form). */
u32 DeckEdit_GetListCardW(int list, int row, int col) asm("DeckEdit_GetListCard");

/* Matching: u16 result in this unit (deck_edit.h declares u32). */
u16 DeckEdit_GetSelectedCardCopiesW(void) asm("DeckEdit_GetSelectedCardCopies");

/* Matching: full-width argument and result in this unit (deck_edit.h declares u16). */
u32 DeckEdit_IsFusionMonsterW(u32 cardId) asm("DeckEdit_IsFusionMonster");

/* Matching: the definition ignores a fourth argument (the text flags) that deck_edit.h's prototype omits. */
void DeckEdit_DrawAtkDefW(void *map, int col, int row, void *textFlags) asm("DeckEdit_DrawAtkDef");

/* Address-suffixed views of gDeckEdit (these forms are matching choices, see deck_edit.h). */
extern u8 gDeckEditFrameSlots[];   /* 0x0201F6D8 = &gDeckEdit.frameSlots; also the base for the cardMove
                                (+0x68), listRowRing byte (+0x7C), commandMenu (+0x84) and swap byte
                                (+0xA2) views in SideDeckSwap_Init */

/* The swap arm byte at gDeckEdit + 0x1866 (inside anims, unnamed in struct DeckEdit):
 * 1 while armed, 0 when the exchange fires, -1 afterwards. Kept as a struct field access (not pointer
 * arithmetic) because that is the matched form. */
struct SwapExchangeByteView {
    u8 pad0[0x1866];
    s8 exchange;
};
#define EXCHANGE (((struct SwapExchangeByteView *)&gDeckEdit)->exchange)

/* The list filter/sort marks at gDeckEdit + 0x1C40 (commandMenu.filter) and +0x1C43 (commandMenu.sort).
 * SideDeckSwap_ExchangeCards clears two of each through this flat view; the nested commandMenu access
 * materializes the offsets differently and does not match. */
struct CommandMenuMarksView {
    u8 pad0[0x1C40];
    u8 filter[2];   /* +0x1C40: commandMenu.filter[0..1] */
    u8 pad1C42;
    u8 sort[2];     /* +0x1C43: commandMenu.sort[0..1] */
};
#define MENU_MARKS (*(struct CommandMenuMarksView *)&gDeckEdit)

u16 DeckEdit_RunListFilter(void)
{
    if (gListFilterSteps[gMain.seqState1]) {
        if (gListFilterSteps[gMain.seqState1]())
            gMain.seqState1++;
        return 0;
    }
    return 1;
}

u16 ProhibitCardSelect_RunListFilter(void)
{
    if (gListFilterSteps[gChain.targetWork2]) {
        if (gListFilterSteps[gChain.targetWork2]())
            gChain.targetWork2++;
        return 0;
    }
    return 1;
}

/* Bitfield views reached from &gDeckEdit.frameSlots in SideDeckSwap_Init. The same bits have canonical
 * names in struct DeckEdit, but the ROM addresses them from the frameSlots base, so the pointer form
 * stays. */
struct ListRowRingByteView {        /* gDeckEdit + 0x1C34 */
    u8 cursorRowPage:1;             /* bit 0 */
    u8 listRowRing:4;               /* bits 1-4 */
    u8 unk5:3;
    u8 pad[7];
};
struct CommandMenuWordView {        /* gDeckEdit + 0x1C3C (commandMenu as a word) */
    u32 low:15;                     /* bits 0-14: timer, anim, prevRows, rows */
    u32 choice:3;                   /* bits 15-17: enum DeckEditCommand */
    u32 high:14;
    u8 pad[4];
};
struct CommandMenuAnimByteView {    /* gDeckEdit + 0x1C3D */
    u8 anim:3;                      /* bits 0-2: enum CommandMenuAnim */
    u8 unk3:5;
    u8 pad[7];
};
struct SwapStateByteView {          /* gDeckEdit + 0x1C5A */
    u8 menuVariant:2;               /* bits 0-1: enum DeckEditMenuVariant */
    u8 swapSelector:3;              /* bits 2-4: enum SideDeckSwapState */
    u8 unk5:3;
    u8 pad[7];
};

int SideDeckSwap_Init(void)
{
    u16 i, j;
    struct ListRowRingByteView *rowByte;
    struct SwapStateByteView *swapByte;
    struct CommandMenuWordView *menuWord;
    MemClear16(&gDeckEdit, 0x1C5C);
    gMain.vblankFlags = 1;
    ResetBgScroll();
    REG_BG0VOFS = 0; REG_BG0HOFS = 0;
    REG_BG1VOFS = 0; REG_BG1HOFS = 0;
    REG_BG2VOFS = 0; REG_BG2HOFS = 0;
    REG_BG3VOFS = 0; REG_BG3HOFS = 0;
    REG16(0x28) = 0; REG16(0x2A) = 0;   /* BG2 affine origin (gba.h has no name) */
    REG16(0x3C) = 0; REG16(0x3E) = 0;   /* BG3 affine origin */
    REG_DISPCNT &= 0xE0FF;
    OamListClear((u8 *)&gDeckEdit);
    gDeckEdit.bg3Vofs = 0;
    gDeckEdit.bg3Hofs = 0;
    gDeckEdit.bg1Vofs = 0;
    gDeckEdit.bg1Hofs = 0;
    gDeckEdit.bg0Vofs = 0;
    gDeckEdit.bg0Hofs = 0;
    for (i = 0; i <= 2; i++) {
        gDeckEdit.listPos[i] = 0;
        gDeckEdit.listRow[i] = 0;
        for (j = 0; j <= 1; j++) gDeckEdit.listCount[j][i] = 0;
    }
    gDeckEdit.curList = 2;
    gDeckEdit.prevList = 2;
    gDeckEdit.cardArtPage = 0;
    gDeckEdit.scrollDir = DECKEDIT_SCROLL_NONE;
    gDeckEdit.redrawOnPageSlide = 0;
    gDeckEdit.brightness = 0xFC00;
    Ease_Init(0, 0, 0, &gDeckEdit.scrollEase);
    ClearKatakanaFlag((u8 *)&gDeckEdit.textFlags);
    ObjAffineInit(gDeckEdit.objAffine);
    /* Card-number ROM view (gCardIdToNumber at 0x08622AB4); the mask is formed before the table base.
     * Numbers 1920-1999 (0x780..0x7CF) are the fusion monsters and stay out of both lists. */
    for (i = 1; ((const u16 *)0x08622AB4)[i & 0x7FF] != 0xFFFF; ) {
        if ((u16)(((const u16 *)0x08622AB4)[i & 0x7FF] - 0x780) > 0x4F) {
            u8 *trunk = (u8 *)&gSaveData;
            struct TrunkEntryByteView *entry = (struct TrunkEntryByteView *)(trunk + i * 4);
            if (TrunkDeckCopies(entry) != 0)
                DeckEdit_SetListCard(i, DECKEDIT_LIST_MAIN_DECK, gDeckEdit.listRow[1], gDeckEdit.listCount[gDeckEdit.listRow[1]][1]++);
            if (TrunkSideCopies(entry) != 0)
                DeckEdit_SetListCard(i, DECKEDIT_LIST_SIDE_DECK, gDeckEdit.listRow[2], gDeckEdit.listCount[gDeckEdit.listRow[2]][2]++);
        }
        i++;
        if (i > 0x334) break;
    }
    DeckEdit_CountSideDeckMonsters();
    DeckEdit_CalcScrollBar(gDeckEdit.listCount[gDeckEdit.listRow[gDeckEdit.curList]][gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList], (u16 *)&gDeckEdit.scrollBar);
    gDeckEdit.scrollBar.upArrowDirty = 1;
    gDeckEdit.scrollBar.downArrowDirty = 1;
    if (gDeckEdit.listCount[gDeckEdit.listRow[gDeckEdit.curList]][gDeckEdit.curList] > 5)
        gDeckEdit.scrollBar.downArrowFrame = gDeckEdit.scrollBar.upArrowFrame = 1;
    else gDeckEdit.scrollBar.downArrowFrame = gDeckEdit.scrollBar.upArrowFrame = 0;
    DeckEdit_ResetFrameSlots(gDeckEditFrameSlots);
    DeckEdit_ResetCardMove(gDeckEditFrameSlots + 0x68);
    rowByte = (struct ListRowRingByteView *)(gDeckEditFrameSlots + 0x7C);
    rowByte->cursorRowPage = 0; rowByte->listRowRing = 0;
    swapByte = (struct SwapStateByteView *)(gDeckEditFrameSlots + 0xA2);
    swapByte->menuVariant = DECKEDIT_MENU_SIDE_SWAP;
    menuWord = (struct CommandMenuWordView *)((u8 *)rowByte + 8);
    menuWord->choice = DECKEDIT_CMD_TO_MAIN_DECK;
    ((struct CommandMenuAnimByteView *)((u8 *)menuWord + 1))->anim = CMDMENU_REFRESH;
    ((struct CommandMenuAnimByteView *)((u8 *)menuWord + 1))->anim = CMDMENU_OPEN;
    swapByte->swapSelector = SWAP_STATE_IDLE;
    AnimBlockInit((struct AnimSeq **)gDeckEditAnimScripts, gDeckEditFrameSlots - 0x4A0);
    return 1;
}

u16 SideDeckSwap_EnterListView(void)
{
    DeckEdit_InitListView();
    if (++gDeckEdit.curList == 3)
        gDeckEdit.curList = 1;
    gDeckEdit.cursorRowPage = 0;
    gDeckEdit.listRowRing = 0;
    gDeckEdit.commandMenu.choice = 0;
    gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
    DeckEdit_StartListSlide(2);
    DeckEdit_DrawStatementLabels(gDeckEdit.curList);
    gDeckEdit.commandMenu.choice = gDeckEdit.curList + 1;
    gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
    return 1;
}
void SideDeckSwap_HandleListSwitch(u8 *cursor)
{
    if (gDeckEdit.swapSelector == SWAP_STATE_IDLE) {
        if (gMain.newKeys & R_BUTTON) {
            if (++*cursor == 3)
                *cursor = 1;
            gDeckEdit.cursorRowPage = 0;
            gDeckEdit.listRowRing = 0;
            gDeckEdit.commandMenu.choice = 0;
            gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
            DeckEdit_StartListSlide(2);
            DeckEdit_DrawStatementLabels(*cursor);
            gDeckEdit.commandMenu.choice = *cursor + 1;
            gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
            PlaySE(0);
        } else if (gMain.newKeys & L_BUTTON) {
            if (*cursor != 1)
                (*cursor)--;
            else
                *cursor = 2;
            gDeckEdit.cursorRowPage = 0;
            gDeckEdit.listRowRing = 0;
            gDeckEdit.commandMenu.choice = 0;
            gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
            DeckEdit_StartListSlide(3);
            DeckEdit_DrawStatementLabels(*cursor);
            gDeckEdit.commandMenu.choice = *cursor + 1;
            gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
            PlaySE(0);
        }
    }
}
u16 SideDeckSwap_FindCardInList(u16 card)
{
    u16 col;
    for (col = 0; col < gDeckEdit.listCount[gDeckEdit.listRow[gDeckEdit.curList]][gDeckEdit.curList]; col++) {
        if (DeckEdit_GetListCardW(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], col) == card)
            return col;
    }
    return 0;
}
void SideDeckSwap_ExchangeCards(void)
{
    u16 card2, card1;
    u32 count;
    u32 copy;

    if (DeckEdit_IsFusionMonsterW(DeckEdit_GetListCardW(DECKEDIT_LIST_MAIN_DECK, gDeckEdit.listRow[1], gDeckEdit.listPos[1])))
        RemoveCardFromSavedFusionDeck(DeckEdit_GetListCardW(DECKEDIT_LIST_MAIN_DECK, gDeckEdit.listRow[1], gDeckEdit.listPos[1]));
    else
        RemoveCardFromSavedDeck(DeckEdit_GetListCardW(DECKEDIT_LIST_MAIN_DECK, gDeckEdit.listRow[1], gDeckEdit.listPos[1]));
    RemoveCardFromSavedSideDeck(DeckEdit_GetListCardW(DECKEDIT_LIST_SIDE_DECK, gDeckEdit.listRow[2], gDeckEdit.listPos[2]));
    if (DeckEdit_IsFusionMonsterW(DeckEdit_GetListCardW(DECKEDIT_LIST_SIDE_DECK, gDeckEdit.listRow[2], gDeckEdit.listPos[2])))
        AddCardToSavedFusionDeck(DeckEdit_GetListCardW(DECKEDIT_LIST_SIDE_DECK, gDeckEdit.listRow[2], gDeckEdit.listPos[2]));
    else
        AddCardToSavedDeck(DeckEdit_GetListCardW(DECKEDIT_LIST_SIDE_DECK, gDeckEdit.listRow[2], gDeckEdit.listPos[2]));
    AddCardToSavedSideDeck(DeckEdit_GetListCardW(DECKEDIT_LIST_MAIN_DECK, gDeckEdit.listRow[1], gDeckEdit.listPos[1]));
    card2 = DeckEdit_GetListCardW(DECKEDIT_LIST_SIDE_DECK, gDeckEdit.listRow[2], gDeckEdit.listPos[2]);
    card1 = DeckEdit_GetListCardW(DECKEDIT_LIST_MAIN_DECK, gDeckEdit.listRow[1], gDeckEdit.listPos[1]);
    gDeckEdit.listRow[1] = 0;
    gDeckEdit.listRow[2] = 0;
    MENU_MARKS.filter[0] = 0;
    MENU_MARKS.sort[0] = 0;
    MENU_MARKS.filter[1] = 0;
    MENU_MARKS.sort[1] = 0;
    gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
    DeckEdit_BuildCardLists();
    switch (gDeckEdit.curList) {
    case DECKEDIT_LIST_MAIN_DECK:
        gDeckEdit.listPos[1] = SideDeckSwap_FindCardInList(card2);
        count = gDeckEdit.listCount[gDeckEdit.listRow[2]][2];
        copy = count;
        /* FAKEMATCH: keep the ROM's separate comparison and decrement values. */
        __asm__ __volatile__("" : "+r"(copy));
        if (copy == gDeckEdit.listPos[2]) {
            if (copy == 0)
                gDeckEdit.listPos[2] = copy;
            else
                gDeckEdit.listPos[2] = count - 1;
        }
        break;
    case DECKEDIT_LIST_SIDE_DECK:
        gDeckEdit.listPos[2] = SideDeckSwap_FindCardInList(card1);
        count = gDeckEdit.listCount[gDeckEdit.listRow[1]][1];
        copy = count;
        __asm__ __volatile__("" : "+r"(copy));
        if (copy == gDeckEdit.listPos[1]) {
            if (copy == 0)
                gDeckEdit.listPos[1] = copy;
            else
                gDeckEdit.listPos[1] = count - 1;
        }
        break;
    }
    DeckEdit_StartListSlide(2);
    DeckEdit_CountSideDeckMonsters();
}
void SideDeckSwap_UpdateExchange(void)
{
    if (DeckEdit_GetSelectedCardCopiesW() != 0) {
        switch (gDeckEdit.swapSelector) {
        case SWAP_STATE_IDLE: break;
        case SWAP_STATE_FIRST_PICKED:
            switch (gDeckEdit.commandMenu.choice) {
            case DECKEDIT_CMD_TO_MAIN_DECK:
                gDeckEdit.commandMenu.choice = DECKEDIT_CMD_TO_SIDE_DECK;
                gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
                gDeckEdit.curList = 2;
                DeckEdit_StartListSlide(2);
                DeckEdit_DrawStatementLabels(gDeckEdit.curList);
                break;
            case DECKEDIT_CMD_TO_SIDE_DECK:
                gDeckEdit.commandMenu.choice = DECKEDIT_CMD_TO_MAIN_DECK;
                gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
                gDeckEdit.curList = 1;
                DeckEdit_StartListSlide(2);
                DeckEdit_DrawStatementLabels(gDeckEdit.curList);
                break;
            }
            gDeckEdit.swapSelector++;
            break;
        case SWAP_STATE_PICK_SECOND: break;
        case SWAP_STATE_SECOND_PICKED:
            EXCHANGE = 1;
            gDeckEdit.swapSelector = SWAP_STATE_ANIMATING;
            break;
        case SWAP_STATE_ANIMATING:
            if (EXCHANGE == 0) {
                EXCHANGE = -1;
                SideDeckSwap_ExchangeCards();
                gDeckEdit.swapSelector = SWAP_STATE_IDLE;
            }
            break;
        }
    }
}
void SideDeckSwap_CancelExchange(void)
{
    gDeckEdit.swapSelector = SWAP_STATE_IDLE;
    switch (gDeckEdit.commandMenu.choice) {
    case DECKEDIT_CMD_TO_MAIN_DECK:
        gDeckEdit.commandMenu.choice = DECKEDIT_CMD_TO_SIDE_DECK;
        gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
        gDeckEdit.curList = 2;
        DeckEdit_StartListSlide(2);
        DeckEdit_DrawStatementLabels(gDeckEdit.curList);
        break;
    case DECKEDIT_CMD_TO_SIDE_DECK:
        gDeckEdit.commandMenu.choice = DECKEDIT_CMD_TO_MAIN_DECK;
        gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
        gDeckEdit.curList = 1;
        DeckEdit_StartListSlide(2);
        DeckEdit_DrawStatementLabels(gDeckEdit.curList);
        break;
    }
}
/*
 * SideDeckSwap_Update is a near copy of the matched TradeCardSelect_Update (deck_edit_prohibit); these
 * views mirror that unit's declarations so the shared code compiles identically. The menu arm
 * (menuOpen == 1) differs: selector-driven exchange choices 2/3 and a 3-bit choice cycle. The views keep
 * the ROM's field addresses and bit layouts (see build/readability/issues/deck_edit_filter_steps.md);
 * the tail pointer arithmetic in the function preserves the ROM's register allocation proven by --diff,
 * so only the top-level architecture can be clarified here.
 */

extern u8 gDeckEditScrollEase[];      /* 0x0201E148 = &gDeckEdit.scrollEase */
extern u8 gDeckEditCardMove[];      /* 0x0201F740 = &gDeckEdit.cardMove */
extern u16 gDeckEditListPos[];     /* 0x0201E140 = gDeckEdit.listPos */
extern u8 gDeckEditCurList;        /* 0x0201F73C = gDeckEdit.curList */
extern const u8 gDeckEditArtFrameSprites[];    /* sprite templates drawn by SideDeckSwap_Update */
extern const u8 gDeckEditPanelCornerSprite[];

/* gDeckEdit as one view: fade/scrollEase scalars, the scroll bar arrows and the command menu word. */
struct SideDeckSwapFrameView {
    u8 pad0[0x618];
    u8 fadeBlock[6];            /* +0x618: struct Fade (color, pad, level, step, state) */
    u8 fadeState;               /* +0x61E: enum FadeState */
    u8 pad61F[0x628 - 0x61F];
    u8 scrollEaseState;         /* +0x628: struct Ease.state of gDeckEdit.scrollEase */
    u8 pad629;
    s16 scrollEaseStep;         /* +0x62A: gDeckEdit.scrollEase.cur */
    u8 pad62C[0x1710 - 0x62C];
    u8 redrawOnPageSlide : 1;   /* +0x1710 bit 0 */
    u8 redrawOther : 7;
    u8 pad1711[0x17DA - 0x1711];
    u8 frameDirty;              /* +0x17DA */
    u8 pad17DB[0x1866 - 0x17DB];
    s8 exchange;                /* +0x1866: 1 while the swap is armed, 0 when it fires, -1 after */
    u8 pad1867[0x1BB4 - 0x1867];
    u8 upArrowDirty;            /* +0x1BB4 */
    u8 upArrowFrame;            /* +0x1BB5 */
    u8 downArrowDirty;          /* +0x1BB6 */
    u8 downArrowFrame;          /* +0x1BB7 */
    u8 frameSlotCount;          /* +0x1BB8: struct FrameSlotRing.head */
    u8 pad1BB9[0x1C14 - 0x1BB9];
    u8 frameSlot5Active;        /* +0x1C14: frameSlots.slot[5].active */
    u8 pad1C15[0x1C20 - 0x1C15];
    u8 cardMoveStep;            /* +0x1C20: struct CardMove.step */
    u8 pad1C21[0x1C3C - 0x1C21];
    u32 menuOtherLow : 15;      /* +0x1C3C */
    u16 menuChoice : 3;         /* bits 15-17: enum DeckEditCommand */
    u32 menuOtherHigh : 14;
    u8 pad1C40[0x1C58 - 0x1C40];
    u16 nameTabTimer;           /* +0x1C58 */
};

/* The command menu word (+0x1C3C) read whole. */
struct CommandMenuRawView {
    u8 pad0[0x1C3C];
    u32 word;
};

/* Animation bits at +0x1C3D (struct DeckEditCommandMenu.anim). */
struct CommandMenuPhaseView {
    u8 pad0[0x1C3D];
    u8 anim : 3;        /* enum CommandMenuAnim */
    u8 rest3D : 5;
};

struct FrameSlotRowView {
    u8 pad0[12];
    u8 active;          /* byte at (gDeckEdit + 0x1BC4 + 16 * slot): the slot is shown */
    u8 padD[3];
};

struct FrameSlotRowsView {
    u8 pad0[0x1BB8];
    struct FrameSlotRowView rows[6];
};

/* gDeckEdit.scrollEase through its gDeckEditScrollEase base. */
struct ScrollEaseView {
    u8 state;
    u8 pad1;
    s16 cur;
    u8 pad4[9];
    u8 scrollDir;       /* +0xD = gDeckEdit.scrollDir */
};

#define FRAME (*(struct SideDeckSwapFrameView *)&gDeckEdit)
#define MENU_WORD (((struct CommandMenuRawView *)&gDeckEdit)->word)
#define MENU_PHASE (((struct CommandMenuPhaseView *)&gDeckEdit)->anim)
#define SLOT_ROWS ((struct FrameSlotRowsView *)&gDeckEdit)->rows
#define TWEEN (*(struct ScrollEaseView *)gDeckEditScrollEase)
#define STATE_BYTES ((u8 *)&gDeckEdit)
#define CUR_LIST_ALIAS gDeckEditCurList
#define LIST_COUNT(list) (gDeckEdit.listCount[gDeckEdit.listRow[list]][list])
#define TWEEN_STATE FRAME.scrollEaseState
#define TWEEN_STEP FRAME.scrollEaseStep
#define CURVE gDeckEditEaseCurve[TWEEN_STEP]
#define GET_LIST_CARD ((u32 (*)(u8, u8, int))DeckEdit_GetListCard)
#define DRAW_LIST_ROW ((void (*)(int, u8 *, int, int, u32, int))DeckEdit_DrawListRowName)
#define INIT_FRAME_SLOT ((void (*)(int, int, int, u8 *, u8 *))DeckEdit_InitFrameSlot)
#define DRAW_CURSOR_ROW ((void (*)(int, u8 *, int, int, void *))DeckEdit_DrawCursorRowName)
#define DRAW_NO_CARDS ((void (*)(int, u8 *, int, int, void *))DeckEdit_DrawNoCardsText)
#define LOAD_CARD_ART ((void (*)(int, u32, int))LoadCardArt8bpp)
#define PLACE_CARD_ART ((void (*)(int, int, int, int))DeckEdit_PlaceCardArt)
#define EASE_MUL ((s32 (*)(s32, u16))MulFix8)
#define SET_BLEND ((void (*)(u32))SetBldAlpha)
#define CALC_SCROLL_BAR ((void (*)(u16, u16, u16 *))DeckEdit_CalcScrollBar)
#define DRAW_SCROLL_BAR ((void (*)(u16, u16, u16 *))DeckEdit_DrawScrollBar)
#define TWEEN_FRAME_SLOTS ((void (*)(s16, u8, u8, u8 *, u8 *))DeckEdit_TweenFrameSlots)
#define PLAY_SE_CALL ((void (*)(int))PlaySE)
#define FADE_START_C FadeStart
/* FAKEMATCH: same callee under other return types, so the menu transition calls are not cross-jumped. */
#define FADE_START_INT ((int (*)(u32, u32, u32, void *))FadeStart)
#define FADE_START_U16 ((u16 (*)(u32, u32, u32, void *))FadeStart)
#define UPDATE_CARD_MOVE0 ((void (*)(void))DeckEdit_UpdateCardMove)

/* Matching: bg.h declares the 6-argument form with u16 params; this call site passes a seventh
 * argument (the text flags at STATE_BYTES + 0x640) that the definition ignores, and the matched form
 * passes the shifted row through an int parameter. */
void FillMapRectWrapW(int tile, void *map, int x, int y, int w, int h, void *textFlags) asm("FillMapRectWrap");

/* Matching: int parameters (deck_edit.h declares u16); the shifted row argument is the matched form. */
void DeckEdit_DrawLevelStarsW(void *map, int col, int row, int stars) asm("DeckEdit_DrawLevelStars");

/* Matching: int parameters and void result (sprite.h declares the u16 form); the -1 coordinates are
 * the matched form. */
void OamListAddSpriteGroupW(const void *sprites, int layer, int count, int x, int y, int mode, int priority, int sheetX, int sheetY, int format, int attr0Flags, void *oamList) asm("OamListAddSpriteGroup");
int SideDeckSwap_Update(void)
{
    u16 row;
    /* FAKEMATCH: keep the original fill slot and its stack-store scheduling. */
    volatile u32 clear;
    u32 keys;
    u8 slot;
    u8 horizontalOffset;
    u8 blend;
    u8 *tail;
    u8 *list;
    u8 *objects;
    struct DeckEdit *state;

    keys = gMain.newKeys & 0x3FF;   /* all ten key bits (gba.h has no mask constant) */
    Ease_Tick((struct Ease *)gDeckEditScrollEase);
    /* Crossing the midpoint of a horizontal page slide changes the visible rows. */
    if ((*((u8 *)&gDeckEditScrollEase + 0x10E8) & 1) &&
        ((TWEEN.cur == 4 && TWEEN.scrollDir == DECKEDIT_SCROLL_PAGE_RIGHT) ||
         (TWEEN.cur == 3 && TWEEN.scrollDir == DECKEDIT_SCROLL_PAGE_LEFT))) {
        FRAME.redrawOnPageSlide = 0;
        clear = 0;
        CpuFastSet((const void *)&clear, (void *)0x0600D000, 0x01000200);
        if ((s16)gDeckEditListPos[CUR_LIST_ALIAS] - 2 >= 0) {
            DRAW_LIST_ROW(GET_LIST_CARD(CUR_LIST_ALIAS, gDeckEdit.listRow[CUR_LIST_ALIAS], gDeckEditListPos[CUR_LIST_ALIAS] - 2),
                         (u8 *)0x0600D000, 0, (u8)gDeckEdit.bg1Vofs >> 3, (u32)(STATE_BYTES + 0x640), 1);
        }
        if ((s16)gDeckEditListPos[CUR_LIST_ALIAS] - 1 >= 0) {
            DRAW_LIST_ROW(GET_LIST_CARD(CUR_LIST_ALIAS, gDeckEdit.listRow[CUR_LIST_ALIAS], gDeckEditListPos[CUR_LIST_ALIAS] - 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x10) & 0xFF) >> 3, (u32)(STATE_BYTES + 0x640), 2);
        }
        if ((s16)gDeckEditListPos[CUR_LIST_ALIAS] + 1 < LIST_COUNT(CUR_LIST_ALIAS)) {
            DRAW_LIST_ROW(GET_LIST_CARD(CUR_LIST_ALIAS, gDeckEdit.listRow[CUR_LIST_ALIAS], gDeckEditListPos[CUR_LIST_ALIAS] + 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x48) & 0xFF) >> 3, (u32)(STATE_BYTES + 0x640), 4);
        }
        if ((s16)gDeckEditListPos[CUR_LIST_ALIAS] + 2 < LIST_COUNT(CUR_LIST_ALIAS)) {
            DRAW_LIST_ROW(GET_LIST_CARD(CUR_LIST_ALIAS, gDeckEdit.listRow[CUR_LIST_ALIAS], gDeckEditListPos[CUR_LIST_ALIAS] + 2),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.bg1Vofs + 0x58) & 0xFF) >> 3, (u32)(STATE_BYTES + 0x640), 5);
        }
        FillMapRectWrapW(0, (void *)0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, 30, 6, STATE_BYTES + 0x640);
        if (LIST_COUNT(CUR_LIST_ALIAS) != 0) {
            DRAW_CURSOR_ROW(GET_LIST_CARD(CUR_LIST_ALIAS, gDeckEdit.listRow[CUR_LIST_ALIAS], gDeckEditListPos[CUR_LIST_ALIAS]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, STATE_BYTES + 0x640);
            DeckEdit_DrawCardIcons(0);
            DeckEdit_DrawAtkDefW((u8 *)0x0600C000, 11, ((gDeckEdit.bg0Vofs + 0x38) & 0xFF) >> 3, STATE_BYTES + 0x640);
            DeckEdit_DrawLevelStarsW((u8 *)0x0600C000, 17, ((gDeckEdit.bg0Vofs + 0x38) & 0xFF) >> 3, 6);
        } else {
            DRAW_NO_CARDS(GET_LIST_CARD(CUR_LIST_ALIAS, gDeckEdit.listRow[CUR_LIST_ALIAS], gDeckEditListPos[CUR_LIST_ALIAS]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.bg0Vofs + 0x20) & 0xFF) >> 3, STATE_BYTES + 0x640);
        }
    }
    TWEEN_FRAME_SLOTS(TWEEN_STEP, TWEEN_STATE, gDeckEdit.scrollDir, STATE_BYTES + 0x18B0, STATE_BYTES + 0x1BB8);

    /* UP/DOWN move vertically; PAGE_RIGHT/PAGE_LEFT move a five-card page horizontally. */
    switch (gDeckEdit.scrollDir) {
    case DECKEDIT_SCROLL_UP:
    case DECKEDIT_SCROLL_DOWN:
        switch (TWEEN_STATE) {
        case TICK_RUNNING:
            REG_BG3HOFS = gDeckEdit.bg3Hofs;
            REG_BG3VOFS = gDeckEdit.bg3Vofs + (EASE_MUL(0x5000, CURVE) >> 8);
            REG_BG1HOFS = gDeckEdit.bg1Hofs;
            REG_BG1VOFS = gDeckEdit.bg1Vofs + (EASE_MUL(0x1000, CURVE) >> 8);
            REG_BG0HOFS = gDeckEdit.bg0Hofs;
            REG_BG0VOFS = gDeckEdit.bg0Vofs + (EASE_MUL(0x2800, CURVE) >> 8);
            break;
        case TICK_DONE:
            TWEEN_STATE = TICK_IDLE;
            gDeckEdit.bg3Vofs += EASE_MUL(0x5000, CURVE) >> 8;
            gDeckEdit.bg1Vofs += EASE_MUL(0x1000, CURVE) >> 8;
            gDeckEdit.bg0Vofs += EASE_MUL(0x2800, CURVE) >> 8;
            if (FRAME.upArrowFrame) {
                FRAME.upArrowFrame = 1;
                FRAME.upArrowDirty |= 1;
            }
            if (FRAME.downArrowFrame) {
                FRAME.downArrowFrame = 1;
                FRAME.downArrowDirty |= 1;
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
        switch (TWEEN_STATE) {
        case TICK_RUNNING:
            REG_BG3HOFS = gDeckEdit.bg3Hofs + (EASE_MUL(0x5000, CURVE) >> 8);
            REG_BG3VOFS = gDeckEdit.bg3Vofs;
            horizontalOffset = EASE_MUL(0x4000, CURVE) >> 8;
            blend = CURVE >> 3;
            REG_BLDCNT = 0x3F43;
            if (TWEEN_STEP <= 3) {
                REG_BG1HOFS = horizontalOffset;
                REG_BG1VOFS = gDeckEdit.bg1Vofs;
                REG_BG0HOFS = horizontalOffset;
                REG_BG0VOFS = gDeckEdit.bg0Vofs;
                SET_BLEND(blend >> 1);
            } else {
                REG_BG1HOFS = horizontalOffset + 0xFFC0;
                REG_BG1VOFS = gDeckEdit.bg1Vofs;
                REG_BG0HOFS = horizontalOffset + 0xFFC0;
                REG_BG0VOFS = gDeckEdit.bg0Vofs;
                SET_BLEND((0x20 - blend) >> 1);
            }
            break;
        case TICK_DONE:
            TWEEN_STATE = TICK_IDLE;
            gDeckEdit.bg3Hofs += EASE_MUL(0x5000, CURVE) >> 8;
            if (FRAME.upArrowFrame) {
                FRAME.upArrowFrame = 1;
                FRAME.upArrowDirty |= 1;
            }
            if (FRAME.downArrowFrame) {
                FRAME.downArrowFrame = 1;
                FRAME.downArrowDirty |= 1;
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

    if (FRAME.fadeState == FADE_STATE_IDLE) {
        switch (gDeckEdit.menuOpen) {
        case 0:
            if (TWEEN_STATE != TICK_RUNNING) {
                switch (keys) {
                case B_BUTTON:
                    FADE_START_C(0, 0x180, 0, (struct Fade *)(STATE_BYTES + 0x618));
                    gDeckEdit.exitMode = DECKEDIT_EXIT_LEAVE;
                    PLAY_SE_CALL(2);
                    break;
                case DPAD_UP:
                    DeckEdit_ScrollListUp(&row);
                    goto moved;
                case DPAD_DOWN:
                    DeckEdit_ScrollListDown(&row);
                    goto moved;
                default:
                    if (LIST_COUNT(gDeckEdit.curList) > 5) {
                        switch (keys) {
                        case DPAD_RIGHT:
                            if (gDeckEdit.listPos[gDeckEdit.curList] + 5 > LIST_COUNT(gDeckEdit.curList) - 1)
                                gDeckEdit.listPos[gDeckEdit.curList] = 0;
                            else
                                gDeckEdit.listPos[gDeckEdit.curList] += 5;
                            gDeckEdit.brightness = 0xFC00;
                            Ease_Start(0, 6, 1, (struct Ease *)(STATE_BYTES + 0x628));
                            gDeckEdit.cardArtPage ^= 1;
                            LOAD_CARD_ART(GET_LIST_CARD(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]),
                                         0x06008000 + gDeckEdit.cardArtPage * 0x1680, gDeckEdit.cardArtPage);
                            PLACE_CARD_ART(((gDeckEdit.bg3Hofs & 0xFF) >> 3) + 29, ((gDeckEdit.bg3Vofs & 0xFF) >> 3) + 2, gDeckEdit.cardArtPage, 1);
                            gDeckEdit.scrollDir = DECKEDIT_SCROLL_PAGE_RIGHT;
                            FRAME.redrawOnPageSlide = 1;
                            CALC_SCROLL_BAR(LIST_COUNT(gDeckEdit.curList), gDeckEdit.listPos[gDeckEdit.curList], (u16 *)&gDeckEdit.scrollBar);
                            if (FRAME.downArrowFrame) {
                                FRAME.downArrowFrame = 2;
                                FRAME.downArrowDirty |= 1;
                            }
                            row = gDeckEdit.listPos[gDeckEdit.curList] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < LIST_COUNT(gDeckEdit.curList)) {
                                    INIT_FRAME_SLOT(slot, GET_LIST_CARD(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], row),
                                                 slot + 1, STATE_BYTES + 0x1BB8, STATE_BYTES + 0x18B0);
                                } else {
                                    SLOT_ROWS[slot].active = 0;
                                }
                            }
                            FRAME.frameSlot5Active = 0;
                            FRAME.frameSlotCount = 5;
                            FRAME.nameTabTimer = 30;
                        moved:
                            PLAY_SE_CALL(0);
                            break;
                        case DPAD_LEFT:
                            if (gDeckEdit.listPos[gDeckEdit.curList] <= 4)
                                gDeckEdit.listPos[gDeckEdit.curList] = LIST_COUNT(gDeckEdit.curList) - 1;
                            else
                                gDeckEdit.listPos[gDeckEdit.curList] -= 5;
                            gDeckEdit.brightness = 0xFC00;
                            Ease_Start(6, 0, -1, (struct Ease *)(STATE_BYTES + 0x628));
                            gDeckEdit.cardArtPage ^= 1;
                            LOAD_CARD_ART(GET_LIST_CARD(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]),
                                         0x06008000 + gDeckEdit.cardArtPage * 0x1680, gDeckEdit.cardArtPage);
                            PLACE_CARD_ART(((gDeckEdit.bg3Hofs & 0xFF) >> 3) + 9, ((gDeckEdit.bg3Vofs & 0xFF) >> 3) + 2, gDeckEdit.cardArtPage, 1);
                            gDeckEdit.bg3Hofs -= 0x50;
                            gDeckEdit.scrollDir = DECKEDIT_SCROLL_PAGE_LEFT;
                            FRAME.redrawOnPageSlide = 1;
                            CALC_SCROLL_BAR(LIST_COUNT(gDeckEdit.curList), gDeckEdit.listPos[gDeckEdit.curList], (u16 *)&gDeckEdit.scrollBar);
                            if (FRAME.upArrowFrame) {
                                FRAME.upArrowFrame = 2;
                                FRAME.upArrowDirty |= 1;
                            }
                            row = gDeckEdit.listPos[gDeckEdit.curList] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < LIST_COUNT(gDeckEdit.curList)) {
                                    INIT_FRAME_SLOT(slot, GET_LIST_CARD(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], row),
                                                 slot + 1, STATE_BYTES + 0x1BB8, STATE_BYTES + 0x18B0);
                                } else {
                                    SLOT_ROWS[slot].active = 0;
                                }
                            }
                            FRAME.frameSlot5Active = 0;
                            FRAME.frameSlotCount = 5;
                            FRAME.nameTabTimer = 30;
                            PLAY_SE_CALL(0);
                            break;
                        }
                    }
                    break;
                }
            }
            if (keys == A_BUTTON) {
                MENU_PHASE = CMDMENU_OPEN;
                PLAY_SE_CALL(1);
                break;
            }
            SideDeckSwap_HandleListSwitch(&gDeckEditCurList);
            break;
        case 1:
            if (FRAME.cardMoveStep == 0 && FRAME.exchange == -1) {
                switch (keys) {
                case DPAD_RIGHT:
                    if (gDeckEdit.swapSelector != SWAP_STATE_IDLE)
                        break;
                    if (++FRAME.menuChoice == 1)
                        FRAME.menuChoice++;
                    switch (gDeckEdit.curList + 1) {
                    case 2:
                        if ((MENU_WORD & 0x38000) == 0x18000)
                            FRAME.menuChoice++;
                        break;
                    case 3:
                        if ((MENU_WORD & 0x38000) == 0x10000)
                            FRAME.menuChoice++;
                        break;
                    }
                    if ((MENU_WORD & 0x38000) == 0x38000)
                        FRAME.menuChoice = 0;
                    /* FAKEMATCH: one more use of the GCSE copy of &gDeckEdit lifts its allocation
                     * priority above the menu-arm constants, so it keeps r6 as in the ROM. */
                    asm("" : : "r"(&gDeckEdit));
                    MENU_PHASE = CMDMENU_REFRESH;
                    goto menu_moved;
                case DPAD_LEFT:
                    if (gDeckEdit.swapSelector != SWAP_STATE_IDLE)
                        break;
                    if ((MENU_WORD & 0x38000) == 0) {
                        FRAME.menuChoice = 6;
                    } else {
                        if (--FRAME.menuChoice == 1)
                            FRAME.menuChoice = (u16)(FRAME.menuChoice - 1);
                        switch (gDeckEdit.curList + 1) {
                        case 2:
                            if ((MENU_WORD & 0x38000) == 0x18000)
                                FRAME.menuChoice = (u16)(FRAME.menuChoice - 1);
                            break;
                        case 3:
                            if ((MENU_WORD & 0x38000) == 0x10000)
                                FRAME.menuChoice = (u16)(FRAME.menuChoice - 2);
                            break;
                        }
                    }
                    {
                        /* FAKEMATCH: a fourth local quantity in this block, so local-alloc sorts the phase-store temporaries by priority */
                        u32 extra;
                        asm volatile("" : "=r"(extra));
                    }
                    {
                        /* FAKEMATCH: r5 stays busy over the store so its reloads take r0/r1 */
                        register u32 busy asm("r5");
                        asm("" : "=r"(busy));
                        MENU_PHASE = CMDMENU_REFRESH;
                        asm("" : : "r"(busy));
                    }
                menu_moved:
                    PLAY_SE_CALL(0);
                    break;
                case A_BUTTON:
                    switch (FRAME.menuChoice) {
                    case DECKEDIT_CMD_CARD_VIEW:
                        gDeckEdit.exitMode = DECKEDIT_EXIT_CARD_VIEW;
                        FADE_START_INT(0, 0x180, 0, STATE_BYTES + 0x618);
                        PLAY_SE_CALL(1);
                        break;
                    case DECKEDIT_CMD_TO_MAIN_DECK:
                        if (DeckEdit_GetSelectedCardCopiesW() != 0) {
                            gDeckEdit.swapSelector++;
                            PLAY_SE_CALL(1);
                        }
                        break;
                    case DECKEDIT_CMD_TO_SIDE_DECK:
                        if (DeckEdit_GetSelectedCardCopiesW() != 0) {
                            gDeckEdit.swapSelector++;
                            PLAY_SE_CALL(1);
                        }
                        break;
                    case DECKEDIT_CMD_LIST_FILTER:
                        gDeckEdit.exitMode = DECKEDIT_EXIT_LIST_FILTER;
                        FADE_START_U16(0, 0x180, 0, STATE_BYTES + 0x618);
                        PLAY_SE_CALL(1);
                        break;
                    case DECKEDIT_CMD_STATISTICS:
                        gDeckEdit.exitMode = DECKEDIT_EXIT_STATISTICS;
                        FADE_START_C(0, 0x180, 0, (struct Fade *)(STATE_BYTES + 0x618));
                        PLAY_SE_CALL(1);
                        break;
                    case DECKEDIT_CMD_EXIT:
                        FADE_START_C(0, 0x180, 0, (struct Fade *)(STATE_BYTES + 0x618));
                        gDeckEdit.exitMode = DECKEDIT_EXIT_LEAVE;
                        PLAY_SE_CALL(1);
                        break;
                    }
                    break;
                case B_BUTTON:
                    if (gDeckEdit.swapSelector == SWAP_STATE_PICK_SECOND) {
                        SideDeckSwap_CancelExchange();
                        PLAY_SE_CALL(2);
                    }
                    break;
                default:
                    SideDeckSwap_HandleListSwitch(&gDeckEditCurList);
                    if (*((u8 *)&gDeckEditCurList - 0x15F4) != 1) {   /* gDeckEditScrollEase (scrollEase.state), reached from &curList in the ROM's offset form */
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

    SideDeckSwap_UpdateExchange();
    OamListAddSpriteGroupW(gDeckEditArtFrameSprites, 5, 12, -1, -1, 3, 2, 0, 0, 0, 0, &gDeckEdit);
    FRAME.frameDirty = 1;
    DRAW_SCROLL_BAR(gDeckEdit.listPos[gDeckEdit.curList], LIST_COUNT(gDeckEdit.curList), (u16 *)&gDeckEdit.scrollBar);
    DeckEdit_DrawFrameSlots(STATE_BYTES + 0x1BBC, (int)&gDeckEdit);
    for (slot = 1; slot <= 6; slot++)
        ObjAffineApply((struct ObjAffine *)(STATE_BYTES + 0x18B0 + slot * 24));
    tail = gDeckEditCardMove;
    UPDATE_CARD_MOVE0();
    list = tail - 4;
    DeckEdit_UpdatePanelHighlight(list, tail - 3, (struct AnimState *)(objects = tail - 0x508));
    DeckEdit_UpdateCommandMenuAnim((struct DeckEditCommandMenu *)(tail + 0x1C));
    DeckEdit_DrawCommandMenu((struct DeckEditCommandMenu *)(tail + 0x1C));
    OamListAddSpriteGroupW(gDeckEditPanelCornerSprite, 0, 1, -1, -1, 0, 0, 1, 1, 0, 0, state = (struct DeckEdit *)(tail - 0x1C20));
    DeckEdit_DrawCardCounts(*list);
    AnimBlockTick(objects);
    AnimBlockDraw(objects, 0, 0, 0, 0, 0, 3, 0, 0, (u32)state);
    DeckEdit_UpdateNameIndexLetters();
    DeckEdit_DrawNameIndexTab();
    OamListFlush((struct OamList *)state);
    OamListClear((u8 *)state);
    FadeTick((struct Fade *)(tail - 0x1608));
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
