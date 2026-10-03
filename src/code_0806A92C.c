#include "global.h"

struct Main { u8 pad[6]; u16 keys; u8 pad8[0x40E - 8]; u16 vblankFlags; u8 pad410[0x485A - 0x410]; u8 step; };
struct Popup { u8 pad[0x3E7]; u8 step; };
extern struct Main gMain;
extern struct Popup gChain;
extern u16 (*const gListFilterSteps[])(void);
struct TransferRamp { u8 phase, pad; s16 current, end, step; };
struct TransferSlot { u8 pad[8]; u8 visible; u8 tail[7]; };
struct TransferSlots { u8 count; u8 pad[3]; struct TransferSlot entry[6]; };
struct DeckState {
    u8 pad0[0x618]; u8 fade[6]; u8 animState; u8 pad61F;
    u16 col[3];
    u8 pad626[2]; struct TransferRamp ramp;
    u16 h630, h632;
    u8 b634, b635;
    u16 h636, h638, h63A, h63C, h63E;
    u8 cardsWork[0x1494 - 0x640];
    u16 count[2][3];
    u8 row[3];
    u8 pad14A3[0x1710 - 0x14A3];
    u8 flag1710 : 1;
    u8 rest1710 : 7;
    u8 pad1711[0x17DA - 0x1711]; u8 draw17DA; u8 pad17DB[0x1866 - 0x17DB];
    s8 exchange;
    u8 pad1867[0x18AC - 0x1867];
    u16 h18AC;
    u8 pad18AE[2]; u8 sprites[0x300];
    u16 slide[2];
    u8 dirty4 : 1; u8 rest4 : 7;
    u8 b1BB5;
    u8 dirty6 : 1; u8 rest6 : 7;
    u8 b1BB7;
    struct TransferSlots slots;
    u8 cursor;
    u8 b1C1D;
    u8 pad1C1E[2]; u8 inputBusy; u8 pad1C21[0x1C34 - 0x1C21];
    u8 active : 1;
    u8 mode : 4;
    u8 rest34 : 3;
    u8 pad1C35[7];
    union {
        u32 word;
        struct { u32 low : 15; u32 choice : 3; u32 high : 14; } bits;
        struct { u8 first; u8 phase : 3; u8 rest : 5; u8 tail[2]; } bytes;
    } menu;
    u8 marks[6];
    u8 pad1C46[2]; u8 menuOpen:1; u8 exitMode:4; u8 high48:3; u8 pad1C49[0x1C58 - 0x1C49]; u16 selectionTimer;
    u8 low5A : 2;
    u8 selector : 3;
    u8 high5A : 3;
};
typedef char transfer_fade_offset_check[(u32)&((struct DeckState *)0)->fade == 0x618 ? 1 : -1];
typedef char transfer_animState_offset_check[(u32)&((struct DeckState *)0)->animState == 0x61e ? 1 : -1];
typedef char transfer_ramp_offset_check[(u32)&((struct DeckState *)0)->ramp == 0x628 ? 1 : -1];
typedef char transfer_ramp_current_offset_check[(u32)&((struct DeckState *)0)->ramp.current == 0x62a ? 1 : -1];
typedef char transfer_cardsWork_offset_check[(u32)&((struct DeckState *)0)->cardsWork == 0x640 ? 1 : -1];
typedef char transfer_count_offset_check[(u32)&((struct DeckState *)0)->count == 0x1494 ? 1 : -1];
typedef char transfer_row_offset_check[(u32)&((struct DeckState *)0)->row == 0x14a0 ? 1 : -1];
typedef char transfer_draw17DA_offset_check[(u32)&((struct DeckState *)0)->draw17DA == 0x17da ? 1 : -1];
typedef char transfer_sprites_offset_check[(u32)&((struct DeckState *)0)->sprites == 0x18b0 ? 1 : -1];
typedef char transfer_slots_offset_check[(u32)&((struct DeckState *)0)->slots == 0x1bb8 ? 1 : -1];
typedef char transfer_slots_entry_0__visible_offset_check[(u32)&((struct DeckState *)0)->slots.entry[0].visible == 0x1bc4 ? 1 : -1];
typedef char transfer_slots_entry_5__visible_offset_check[(u32)&((struct DeckState *)0)->slots.entry[5].visible == 0x1c14 ? 1 : -1];
typedef char transfer_cursor_offset_check[(u32)&((struct DeckState *)0)->cursor == 0x1c1c ? 1 : -1];
typedef char transfer_inputBusy_offset_check[(u32)&((struct DeckState *)0)->inputBusy == 0x1c20 ? 1 : -1];
typedef char transfer_menu_offset_check[(u32)&((struct DeckState *)0)->menu == 0x1c3c ? 1 : -1];
typedef char transfer_pad1C49_offset_check[(u32)&((struct DeckState *)0)->pad1C49 == 0x1c49 ? 1 : -1];
typedef char transfer_selectionTimer_offset_check[(u32)&((struct DeckState *)0)->selectionTimer == 0x1c58 ? 1 : -1];
extern struct DeckState gDeckEdit;
u32 DeckEdit_GetListCard(int list, int row, int col);
void DeckEdit_StartListSlide(u8);
void DeckEdit_DrawStatementLabels(u8);
void DeckEdit_InitListView(void);
void PlaySE(u16);
u32 DeckEdit_IsFusionMonster(u32);
void RemoveCardFromSavedFusionDeck(u16);
void RemoveCardFromSavedDeck(u16);
void RemoveCardFromSavedSideDeck(u16);
void AddCardToSavedFusionDeck(u16);
void AddCardToSavedDeck(u16);
void AddCardToSavedSideDeck(u16);
void DeckEdit_BuildCardLists(void);
void DeckEdit_CountSideDeckMonsters(void);
u16 DeckEdit_GetSelectedCardCopies(void);
void SideDeckSwap_ExchangeCards(void);

u16 DeckEdit_RunListFilter(void)
{
    if (gListFilterSteps[gMain.step]) {
        if (gListFilterSteps[gMain.step]())
            gMain.step++;
        return 0;
    }
    return 1;
}

u16 ProhibitCardSelect_RunListFilter(void)
{
    if (gListFilterSteps[gChain.step]) {
        if (gListFilterSteps[gChain.step]())
            gChain.step++;
        return 0;
    }
    return 1;
}
#define REG16(off) (*(volatile u16 *)(0x04000000 + (off)))
struct B7C { u8 a:1; u8 b:4; u8 c:3; u8 pad[7]; };
struct BA2 { u8 a:2; u8 b:3; u8 c:3; u8 pad[7]; };
struct W84 { u32 a:15; u32 b:3; u32 c:14; u8 pad[4]; };
struct B85 { u8 a:3; u8 b:5; u8 pad[7]; };
extern u8 gUnk_0201F6D8[];
extern u16 gCardIdToNumber[];
struct TrunkEntry { u8 pad[9]; u8 low:2; u8 main:2; u8 side:2; u8 extra:2; u8 tail[2]; };
extern u8 gSaveData[];
extern const u8 gDeckEditAnimScripts[];
void MemClear16(void *, u32);
void ResetBgScroll(void);
void OamListClear(void *);
void Ease_Init(int, int, int, void *);
void ClearKatakanaFlag(void *);
void ObjAffineInit(void *);
void DeckEdit_ResetFrameSlots(void *);
void DeckEdit_ResetCardMove(void *);
void AnimBlockInit(const void *, void *);
void DeckEdit_SetListCard(u16, u8, u8, u16);
void DeckEdit_CalcScrollBar(u16, u16, u16 *);

/* Return the extracted count before testing it, preserving both bit reads. */
static inline int TransferMainCount(struct TrunkEntry *entry) { return entry->main; }
static inline int TransferSideCount(struct TrunkEntry *entry) { return entry->side; }
int SideDeckSwap_Init(void)
{
    u16 i, j;
    struct B7C *b7c;
    struct BA2 *ba2;
    struct W84 *w84;
    MemClear16(&gDeckEdit, 0x1C5C);
    gMain.vblankFlags = 1;
    ResetBgScroll();
    REG16(0x12) = 0; REG16(0x10) = 0;
    REG16(0x16) = 0; REG16(0x14) = 0;
    REG16(0x1A) = 0; REG16(0x18) = 0;
    REG16(0x1E) = 0; REG16(0x1C) = 0;
    REG16(0x28) = 0; REG16(0x2A) = 0;
    REG16(0x3C) = 0; REG16(0x3E) = 0;
    REG16(0) &= 0xE0FF;
    OamListClear(&gDeckEdit);
    gDeckEdit.h632 = 0;
    gDeckEdit.h630 = 0;
    gDeckEdit.h63A = 0;
    gDeckEdit.h638 = 0;
    gDeckEdit.h63E = 0;
    gDeckEdit.h63C = 0;
    for (i = 0; i <= 2; i++) {
        gDeckEdit.col[i] = 0;
        gDeckEdit.row[i] = 0;
        for (j = 0; j <= 1; j++) gDeckEdit.count[j][i] = 0;
    }
    gDeckEdit.cursor = 2;
    gDeckEdit.b1C1D = 2;
    gDeckEdit.b634 = 0;
    gDeckEdit.b635 = 0;
    gDeckEdit.flag1710 = 0;
    gDeckEdit.h18AC = 0xFC00;
    Ease_Init(0, 0, 0, (u8 *)&gDeckEdit + 0x628);
    ClearKatakanaFlag((u8 *)&gDeckEdit + 0x640);
    ObjAffineInit((u8 *)&gDeckEdit + 0x18B0);
    /* Card-number ROM view; the mask is formed before the table base. */
    for (i = 1; ((const u16 *)0x08622AB4)[i & 0x7FF] != 0xFFFF; ) {
        if ((u16)(((const u16 *)0x08622AB4)[i & 0x7FF] - 0x780) > 0x4F) {
            u8 *trunk = gSaveData;
            struct TrunkEntry *entry = (struct TrunkEntry *)(trunk + i * 4);
            if (TransferMainCount(entry) != 0)
                DeckEdit_SetListCard(i, 1, gDeckEdit.row[1], gDeckEdit.count[gDeckEdit.row[1]][1]++);
            if (TransferSideCount(entry) != 0)
                DeckEdit_SetListCard(i, 2, gDeckEdit.row[2], gDeckEdit.count[gDeckEdit.row[2]][2]++);
        }
        i++;
        if (i > 0x334) break;
    }
    DeckEdit_CountSideDeckMonsters();
    DeckEdit_CalcScrollBar(gDeckEdit.count[gDeckEdit.row[gDeckEdit.cursor]][gDeckEdit.cursor], gDeckEdit.col[gDeckEdit.cursor], gDeckEdit.slide);
    gDeckEdit.dirty4 = 1;
    gDeckEdit.dirty6 = 1;
    if (gDeckEdit.count[gDeckEdit.row[gDeckEdit.cursor]][gDeckEdit.cursor] > 5)
        gDeckEdit.b1BB7 = gDeckEdit.b1BB5 = 1;
    else gDeckEdit.b1BB7 = gDeckEdit.b1BB5 = 0;
    DeckEdit_ResetFrameSlots(gUnk_0201F6D8);
    DeckEdit_ResetCardMove(gUnk_0201F6D8 + 0x68);
    b7c = (struct B7C *)(gUnk_0201F6D8 + 0x7C);
    b7c->a = 0; b7c->b = 0;
    ba2 = (struct BA2 *)(gUnk_0201F6D8 + 0xA2);
    ba2->a = 1;
    w84 = (struct W84 *)((u8 *)b7c + 8);
    w84->b = 2;
    ((struct B85 *)((u8 *)w84 + 1))->a = 3;
    ((struct B85 *)((u8 *)w84 + 1))->a = 1;
    ba2->b = 0;
    AnimBlockInit(gDeckEditAnimScripts, gUnk_0201F6D8 - 0x4A0);
    return 1;
}

u16 SideDeckSwap_EnterListView(void)
{
    DeckEdit_InitListView();
    if (++gDeckEdit.cursor == 3)
        gDeckEdit.cursor = 1;
    gDeckEdit.active = 0;
    gDeckEdit.mode = 0;
    gDeckEdit.menu.bits.choice = 0;
    gDeckEdit.menu.bytes.phase = 3;
    DeckEdit_StartListSlide(2);
    DeckEdit_DrawStatementLabels(gDeckEdit.cursor);
    gDeckEdit.menu.bits.choice = gDeckEdit.cursor + 1;
    gDeckEdit.menu.bytes.phase = 3;
    return 1;
}
void SideDeckSwap_HandleListSwitch(u8 *cursor)
{
    if (gDeckEdit.selector == 0) {
        if (gMain.keys & 0x100) {
            if (++*cursor == 3)
                *cursor = 1;
            gDeckEdit.active = 0;
            gDeckEdit.mode = 0;
            gDeckEdit.menu.bits.choice = 0;
            gDeckEdit.menu.bytes.phase = 3;
            DeckEdit_StartListSlide(2);
            DeckEdit_DrawStatementLabels(*cursor);
            gDeckEdit.menu.bits.choice = *cursor + 1;
            gDeckEdit.menu.bytes.phase = 3;
            PlaySE(0);
        } else if (gMain.keys & 0x200) {
            if (*cursor != 1)
                (*cursor)--;
            else
                *cursor = 2;
            gDeckEdit.active = 0;
            gDeckEdit.mode = 0;
            gDeckEdit.menu.bits.choice = 0;
            gDeckEdit.menu.bytes.phase = 3;
            DeckEdit_StartListSlide(3);
            DeckEdit_DrawStatementLabels(*cursor);
            gDeckEdit.menu.bits.choice = *cursor + 1;
            gDeckEdit.menu.bytes.phase = 3;
            PlaySE(0);
        }
    }
}
u16 SideDeckSwap_FindCardInList(u16 card)
{
    u16 col;
    for (col = 0; col < gDeckEdit.count[gDeckEdit.row[gDeckEdit.cursor]][gDeckEdit.cursor]; col++) {
        if (DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.row[gDeckEdit.cursor], col) == card)
            return col;
    }
    return 0;
}
void SideDeckSwap_ExchangeCards(void)
{
    u16 card2, card1;
    u32 count;
    u32 copy;

    if (DeckEdit_IsFusionMonster(DeckEdit_GetListCard(1, gDeckEdit.row[1], gDeckEdit.col[1])))
        RemoveCardFromSavedFusionDeck(DeckEdit_GetListCard(1, gDeckEdit.row[1], gDeckEdit.col[1]));
    else
        RemoveCardFromSavedDeck(DeckEdit_GetListCard(1, gDeckEdit.row[1], gDeckEdit.col[1]));
    RemoveCardFromSavedSideDeck(DeckEdit_GetListCard(2, gDeckEdit.row[2], gDeckEdit.col[2]));
    if (DeckEdit_IsFusionMonster(DeckEdit_GetListCard(2, gDeckEdit.row[2], gDeckEdit.col[2])))
        AddCardToSavedFusionDeck(DeckEdit_GetListCard(2, gDeckEdit.row[2], gDeckEdit.col[2]));
    else
        AddCardToSavedDeck(DeckEdit_GetListCard(2, gDeckEdit.row[2], gDeckEdit.col[2]));
    AddCardToSavedSideDeck(DeckEdit_GetListCard(1, gDeckEdit.row[1], gDeckEdit.col[1]));
    card2 = DeckEdit_GetListCard(2, gDeckEdit.row[2], gDeckEdit.col[2]);
    card1 = DeckEdit_GetListCard(1, gDeckEdit.row[1], gDeckEdit.col[1]);
    gDeckEdit.row[1] = 0;
    gDeckEdit.row[2] = 0;
    gDeckEdit.marks[0] = 0;
    gDeckEdit.marks[3] = 0;
    gDeckEdit.marks[1] = 0;
    gDeckEdit.marks[4] = 0;
    gDeckEdit.menu.bytes.phase = 3;
    DeckEdit_BuildCardLists();
    switch (gDeckEdit.cursor) {
    case 1:
        gDeckEdit.col[1] = SideDeckSwap_FindCardInList(card2);
        count = gDeckEdit.count[gDeckEdit.row[2]][2];
        copy = count;
        /* FAKEMATCH: keep the ROM's separate comparison and decrement values. */
        __asm__ __volatile__("" : "+r"(copy));
        if (copy == gDeckEdit.col[2]) {
            if (copy == 0)
                gDeckEdit.col[2] = copy;
            else
                gDeckEdit.col[2] = count - 1;
        }
        break;
    case 2:
        gDeckEdit.col[2] = SideDeckSwap_FindCardInList(card1);
        count = gDeckEdit.count[gDeckEdit.row[1]][1];
        copy = count;
        __asm__ __volatile__("" : "+r"(copy));
        if (copy == gDeckEdit.col[1]) {
            if (copy == 0)
                gDeckEdit.col[1] = copy;
            else
                gDeckEdit.col[1] = count - 1;
        }
        break;
    }
    DeckEdit_StartListSlide(2);
    DeckEdit_CountSideDeckMonsters();
}
void SideDeckSwap_UpdateExchange(void)
{
    if (DeckEdit_GetSelectedCardCopies() != 0) {
        switch (gDeckEdit.selector) {
        case 0: break;
        case 1:
            switch (gDeckEdit.menu.bits.choice) {
            case 2:
                gDeckEdit.menu.bits.choice = 3;
                gDeckEdit.menu.bytes.phase = 3;
                gDeckEdit.cursor = 2;
                DeckEdit_StartListSlide(2);
                DeckEdit_DrawStatementLabels(gDeckEdit.cursor);
                break;
            case 3:
                gDeckEdit.menu.bits.choice = 2;
                gDeckEdit.menu.bytes.phase = 3;
                gDeckEdit.cursor = 1;
                DeckEdit_StartListSlide(2);
                DeckEdit_DrawStatementLabels(gDeckEdit.cursor);
                break;
            }
            gDeckEdit.selector++;
            break;
        case 2: break;
        case 3:
            gDeckEdit.exchange = 1;
            gDeckEdit.selector = 4;
            break;
        case 4:
            if (gDeckEdit.exchange == 0) {
                gDeckEdit.exchange = -1;
                SideDeckSwap_ExchangeCards();
                gDeckEdit.selector = 0;
            }
            break;
        }
    }
}
void SideDeckSwap_CancelExchange(void)
{
    gDeckEdit.selector = 0;
    switch (gDeckEdit.menu.bits.choice) {
    case 2:
        gDeckEdit.menu.bits.choice = 3;
        gDeckEdit.menu.bytes.phase = 3;
        gDeckEdit.cursor = 2;
        DeckEdit_StartListSlide(2);
        DeckEdit_DrawStatementLabels(gDeckEdit.cursor);
        break;
    case 3:
        gDeckEdit.menu.bits.choice = 2;
        gDeckEdit.menu.bytes.phase = 3;
        gDeckEdit.cursor = 1;
        DeckEdit_StartListSlide(2);
        DeckEdit_DrawStatementLabels(gDeckEdit.cursor);
        break;
    }
}
/* Private reconstruction. Original callers pass these extra workspace arguments. */
extern u8 gUnk_0201E148[], gUnk_0201F740[];
extern u16 gUnk_0201E140[];
extern u8 gUnk_0201F73C;
extern const u16 gDeckEditEaseCurve[];
extern const u8 gUnk_081A6524[], gUnk_081A6EA4[];
void CpuFastSet(const void *, void *, u32);
void DeckEdit_DrawListRowName(u32, void *, int, int, void *, int);
void DeckEdit_DrawCursorRowName(u32, void *, int, int, void *);
void DeckEdit_DrawNoCardsText(u32, void *, int, int, void *);
void DeckEdit_DrawCardIcons(int);
void DeckEdit_DrawAtkDef(void *, int, int, void *);
void DeckEdit_DrawLevelStars(void *, int, int, int);
void FillMapRectWrap(int, void *, int, int, int, int, void *);
void DeckEdit_TweenFrameSlots(int, int, int, void *, void *);
void Ease_Tick(void *);
int MulFix8(int, int);
void SetBldAlpha(int);
void FadeStart(int, int, int, void *);
void DeckEdit_ScrollListUp(u16 *), DeckEdit_ScrollListDown(u16 *);
void Ease_Start(int, int, int, void *);
void LoadCardArt8bpp(u32, u32, int);
void DeckEdit_PlaceCardArt(int, int, int, int);
void DeckEdit_InitFrameSlot(int, u32, int, void *, void *);
void OamListAddSpriteGroup(const void *, int, int, int, int, int, int, int, int, int, int, void *);
void DeckEdit_DrawScrollBar(int, int, void *);
void DeckEdit_DrawFrameSlots(void *, void *);
void ObjAffineApply(void *), DeckEdit_UpdateCardMove(void *);
void DeckEdit_UpdatePanelHighlight(void *, void *, void *);
void DeckEdit_UpdateCommandMenuAnim(void *), DeckEdit_DrawCommandMenu(void *);
void DeckEdit_DrawCardCounts(int), AnimBlockTick(void *);
void AnimBlockDraw(void *, int, int, int, int, int, int, int, int, void *);
void DeckEdit_UpdateNameIndexLetters(void), DeckEdit_DrawNameIndexTab(void);
void OamListFlush(void *), FadeTick(void *);

#define D gDeckEdit
#define DROW(list) D.row[list]
#define DCOUNT(list) D.count[DROW(list)][list]
#define CURRENT_CARD DeckEdit_GetListCard(D.cursor, DROW(D.cursor), D.col[D.cursor])
#define EASE(amount) (MulFix8(amount, gDeckEditEaseCurve[D.ramp.current]) >> 8)
#define REFRESH_SCROLL() \
    REG16(0x1C) = D.h630; REG16(0x1E) = D.h632; \
    REG16(0x14) = D.h638; REG16(0x16) = D.h63A; \
    REG16(0x10) = D.h63C; REG16(0x12) = D.h63E
#define FINISH_SCROLL() \
    if (D.b1BB5 != 0) { D.b1BB5 = 1; D.dirty4 = 1; } \
    if (D.b1BB7 != 0) { D.b1BB7 = 1; D.dirty6 = 1; } \
    D.b635 = 0
#define DRAW_SLOTS() \
    scratch.row = D.col[D.cursor] - 2; \
    for (i = 0; i <= 4; i++) { \
        if ((s16)scratch.row >= 0 && scratch.row < DCOUNT(D.cursor)) \
            DeckEdit_InitFrameSlot(i, DeckEdit_GetListCard(D.cursor, DROW(D.cursor), scratch.row), i + 1, &D.slots, D.sprites); \
        else D.slots.entry[i].visible = 0; \
        scratch.row++; \
    } \
    D.slots.entry[5].visible = 0; D.slots.count = 5; D.selectionTimer = 30

/* SideDeckSwap_Update is a near copy of the matched TradeCardSelect_Update (code_0807093C); these views
 * mirror that unit's declarations so the shared code compiles identically. The menu arm
 * (menuOpen == 1) differs: selector-driven exchange choices 2/3 and a 3-bit choice cycle. */
struct B3Frame {
    u8 pad0[0x618];
    u8 transition[6];           /* +0x618 */
    u8 transitionState;         /* +0x61E */
    u8 pad61F[0x628 - 0x61F];
    u8 tweenState;              /* +0x628 */
    u8 pad629;
    s16 tweenStep;              /* +0x62A */
    u8 pad62C[0x1710 - 0x62C];
    u8 redraw : 1;              /* +0x1710 bit 0 */
    u8 redrawOther : 7;
    u8 pad1711[0x17DA - 0x1711];
    u8 frameDirty;              /* +0x17DA */
    u8 pad17DB[0x1866 - 0x17DB];
    s8 menuOwner;               /* +0x1866 */
    u8 pad1867[0x1BB4 - 0x1867];
    u8 previousArrowDirty;      /* +0x1BB4 */
    u8 previousArrowState;      /* +0x1BB5 */
    u8 nextArrowDirty;          /* +0x1BB6 */
    u8 nextArrowState;          /* +0x1BB7 */
    u8 rowCount;                /* +0x1BB8 */
    u8 pad1BB9[0x1C14 - 0x1BB9];
    u8 rowAnimationState;       /* +0x1C14 */
    u8 pad1C15[0x1C20 - 0x1C15];
    u8 menuAnimationState;      /* +0x1C20 */
    u8 pad1C21[0x1C3C - 0x1C21];
    u32 menuOtherLow : 15;      /* +0x1C3C */
    u16 menuSelected : 3;       /* bits 15-17 */
    u32 menuOtherHigh : 14;
    u8 pad1C40[0x1C58 - 0x1C40];
    u16 rowAnimationTimer;      /* +0x1C58 */
};
struct B3MenuRaw { u8 pad0[0x1C3C]; u32 word; };
struct B3PhaseFields { u8 pad0[0x1C3D]; u8 phase : 3; u8 rest3D : 5; };
struct B3RowFrame { u8 pad0[12]; u8 active; u8 padD[3]; };
struct B3RowFields { u8 pad0[0x1BB8]; struct B3RowFrame rows[6]; };
struct B3Tween { u8 state; u8 pad1; s16 value; u8 pad4[9]; u8 direction; };
#define B3_FRAME (*(struct B3Frame *)&gDeckEdit)
#define B3_MENU_RAW (((struct B3MenuRaw *)&gDeckEdit)->word)
#define B3_PHASE (((struct B3PhaseFields *)&gDeckEdit)->phase)
#define B3_ROWS ((struct B3RowFields *)&gDeckEdit)->rows
#define B3_TWEEN (*(struct B3Tween *)gUnk_0201E148)
#define B3_PSTATE ((u8 *)&gDeckEdit)
#define B3_CURLIST gUnk_0201F73C
#define B3_CNT(list) (gDeckEdit.count[gDeckEdit.row[list]][list])
#define B3_TWEEN_STATE B3_FRAME.tweenState
#define B3_TWEEN_STEP B3_FRAME.tweenStep
#define B3_CURVE gDeckEditEaseCurve[B3_TWEEN_STEP]
#define B3_LOOKUP ((u32 (*)(u8, u8, int))DeckEdit_GetListCard)
#define B3_ROW_DRAW ((void (*)(int, u8 *, int, int, u32, int))DeckEdit_DrawListRowName)
#define B3_SLOT ((void (*)(int, int, int, u8 *, u8 *))DeckEdit_InitFrameSlot)
#define B3_DETAIL ((void (*)(int, u8 *, int, int, void *))DeckEdit_DrawCursorRowName)
#define B3_CLEAR ((void (*)(int, u8 *, int, int, void *))DeckEdit_DrawNoCardsText)
#define B3_CARD ((void (*)(int, u32, int))LoadCardArt8bpp)
#define B3_MISC ((void (*)(int, int, int, int))DeckEdit_PlaceCardArt)
#define B3_EASE ((s32 (*)(s32, u16))MulFix8)
#define B3_BLEND ((void (*)(u32))SetBldAlpha)
#define B3_SLIDE ((void (*)(u16, u16, u16 *))DeckEdit_CalcScrollBar)
#define B3_SLIDE_DRAW ((void (*)(u16, u16, u16 *))DeckEdit_DrawScrollBar)
#define B3_TICK ((void (*)(s16, u8, u8, u8 *, u8 *))DeckEdit_TweenFrameSlots)
#define B3_SOUND ((void (*)(int))PlaySE)
#define B3_FADE FadeStart
/* FAKEMATCH: same callee under other return types, so the menu transition calls are not cross-jumped. */
#define B3_FADE_INT ((int (*)(u32, u32, u32, void *))FadeStart)
#define B3_FADE_U16 ((u16 (*)(u32, u32, u32, void *))FadeStart)
#define B3_TAIL0 ((void (*)(void))DeckEdit_UpdateCardMove)
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
    struct DeckState *state;

    keys = gMain.keys & 0x3FF;
    Ease_Tick(gUnk_0201E148);
    /* Crossing the midpoint of a horizontal page slide changes the visible rows. */
    if ((*((u8 *)&gUnk_0201E148 + 0x10E8) & 1) &&
        ((B3_TWEEN.value == 4 && B3_TWEEN.direction == 3) ||
         (B3_TWEEN.value == 3 && B3_TWEEN.direction == 4))) {
        B3_FRAME.redraw = 0;
        clear = 0;
        CpuFastSet((const void *)&clear, (void *)0x0600D000, 0x01000200);
        if ((s16)gUnk_0201E140[B3_CURLIST] - 2 >= 0) {
            B3_ROW_DRAW(B3_LOOKUP(B3_CURLIST, gDeckEdit.row[B3_CURLIST], gUnk_0201E140[B3_CURLIST] - 2),
                         (u8 *)0x0600D000, 0, (u8)gDeckEdit.h63A >> 3, (u32)(B3_PSTATE + 0x640), 1);
        }
        if ((s16)gUnk_0201E140[B3_CURLIST] - 1 >= 0) {
            B3_ROW_DRAW(B3_LOOKUP(B3_CURLIST, gDeckEdit.row[B3_CURLIST], gUnk_0201E140[B3_CURLIST] - 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.h63A + 0x10) & 0xFF) >> 3, (u32)(B3_PSTATE + 0x640), 2);
        }
        if ((s16)gUnk_0201E140[B3_CURLIST] + 1 < B3_CNT(B3_CURLIST)) {
            B3_ROW_DRAW(B3_LOOKUP(B3_CURLIST, gDeckEdit.row[B3_CURLIST], gUnk_0201E140[B3_CURLIST] + 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.h63A + 0x48) & 0xFF) >> 3, (u32)(B3_PSTATE + 0x640), 4);
        }
        if ((s16)gUnk_0201E140[B3_CURLIST] + 2 < B3_CNT(B3_CURLIST)) {
            B3_ROW_DRAW(B3_LOOKUP(B3_CURLIST, gDeckEdit.row[B3_CURLIST], gUnk_0201E140[B3_CURLIST] + 2),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.h63A + 0x58) & 0xFF) >> 3, (u32)(B3_PSTATE + 0x640), 5);
        }
        FillMapRectWrap(0, (void *)0x0600C000, 0, ((gDeckEdit.h63E + 0x20) & 0xFF) >> 3, 30, 6, B3_PSTATE + 0x640);
        if (B3_CNT(B3_CURLIST) != 0) {
            B3_DETAIL(B3_LOOKUP(B3_CURLIST, gDeckEdit.row[B3_CURLIST], gUnk_0201E140[B3_CURLIST]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.h63E + 0x20) & 0xFF) >> 3, B3_PSTATE + 0x640);
            DeckEdit_DrawCardIcons(0);
            DeckEdit_DrawAtkDef((u8 *)0x0600C000, 11, ((gDeckEdit.h63E + 0x38) & 0xFF) >> 3, B3_PSTATE + 0x640);
            DeckEdit_DrawLevelStars((u8 *)0x0600C000, 17, ((gDeckEdit.h63E + 0x38) & 0xFF) >> 3, 6);
        } else {
            B3_CLEAR(B3_LOOKUP(B3_CURLIST, gDeckEdit.row[B3_CURLIST], gUnk_0201E140[B3_CURLIST]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.h63E + 0x20) & 0xFF) >> 3, B3_PSTATE + 0x640);
        }
    }
    B3_TICK(B3_TWEEN_STEP, B3_TWEEN_STATE, gDeckEdit.b635, B3_PSTATE + 0x18B0, B3_PSTATE + 0x1BB8);

    /* Directions 1/2 move vertically; 3/4 move a five-card page horizontally. */
    switch (gDeckEdit.b635) {
    case 1:
    case 2:
        switch (B3_TWEEN_STATE) {
        case 1:
            REG16(0x1C) = gDeckEdit.h630;
            REG16(0x1E) = gDeckEdit.h632 + (B3_EASE(0x5000, B3_CURVE) >> 8);
            REG16(0x14) = gDeckEdit.h638;
            REG16(0x16) = gDeckEdit.h63A + (B3_EASE(0x1000, B3_CURVE) >> 8);
            REG16(0x10) = gDeckEdit.h63C;
            REG16(0x12) = gDeckEdit.h63E + (B3_EASE(0x2800, B3_CURVE) >> 8);
            break;
        case 2:
            B3_TWEEN_STATE = 0;
            gDeckEdit.h632 += B3_EASE(0x5000, B3_CURVE) >> 8;
            gDeckEdit.h63A += B3_EASE(0x1000, B3_CURVE) >> 8;
            gDeckEdit.h63E += B3_EASE(0x2800, B3_CURVE) >> 8;
            if (B3_FRAME.previousArrowState) {
                B3_FRAME.previousArrowState = 1;
                B3_FRAME.previousArrowDirty |= 1;
            }
            if (B3_FRAME.nextArrowState) {
                B3_FRAME.nextArrowState = 1;
                B3_FRAME.nextArrowDirty |= 1;
            }
            gDeckEdit.b635 = 0;
            /* fall through */
        default:
            REG16(0x1C) = gDeckEdit.h630;
            REG16(0x1E) = gDeckEdit.h632;
            REG16(0x14) = gDeckEdit.h638;
            REG16(0x16) = gDeckEdit.h63A;
            REG16(0x10) = gDeckEdit.h63C;
            REG16(0x12) = gDeckEdit.h63E;
            break;
        }
        break;
    case 3:
    case 4:
        switch (B3_TWEEN_STATE) {
        case 1:
            REG16(0x1C) = gDeckEdit.h630 + (B3_EASE(0x5000, B3_CURVE) >> 8);
            REG16(0x1E) = gDeckEdit.h632;
            horizontalOffset = B3_EASE(0x4000, B3_CURVE) >> 8;
            blend = B3_CURVE >> 3;
            REG16(0x50) = 0x3F43;
            if (B3_TWEEN_STEP <= 3) {
                REG16(0x14) = horizontalOffset;
                REG16(0x16) = gDeckEdit.h63A;
                REG16(0x10) = horizontalOffset;
                REG16(0x12) = gDeckEdit.h63E;
                B3_BLEND(blend >> 1);
            } else {
                REG16(0x14) = horizontalOffset + 0xFFC0;
                REG16(0x16) = gDeckEdit.h63A;
                REG16(0x10) = horizontalOffset + 0xFFC0;
                REG16(0x12) = gDeckEdit.h63E;
                B3_BLEND((0x20 - blend) >> 1);
            }
            break;
        case 2:
            B3_TWEEN_STATE = 0;
            gDeckEdit.h630 += B3_EASE(0x5000, B3_CURVE) >> 8;
            if (B3_FRAME.previousArrowState) {
                B3_FRAME.previousArrowState = 1;
                B3_FRAME.previousArrowDirty |= 1;
            }
            if (B3_FRAME.nextArrowState) {
                B3_FRAME.nextArrowState = 1;
                B3_FRAME.nextArrowDirty |= 1;
            }
            gDeckEdit.b635 = 0;
            /* fall through */
        default:
            REG16(0x1C) = gDeckEdit.h630;
            REG16(0x1E) = gDeckEdit.h632;
            REG16(0x14) = gDeckEdit.h638;
            REG16(0x16) = gDeckEdit.h63A;
            REG16(0x10) = gDeckEdit.h63C;
            REG16(0x12) = gDeckEdit.h63E;
            REG16(0x50) = 0x3FC8;
            REG16(0x52) = 0x1000;
            break;
        }
        break;
    }

    if (B3_FRAME.transitionState == 0) {
        switch (gDeckEdit.menuOpen) {
        case 0:
            if (B3_TWEEN_STATE != 1) {
                switch (keys) {
                case 2:
                    B3_FADE(0, 0x180, 0, B3_PSTATE + 0x618);
                    gDeckEdit.exitMode = 0;
                    B3_SOUND(2);
                    break;
                case 0x40:
                    DeckEdit_ScrollListUp(&row);
                    goto moved;
                case 0x80:
                    DeckEdit_ScrollListDown(&row);
                    goto moved;
                default:
                    if (B3_CNT(gDeckEdit.cursor) > 5) {
                        switch (keys) {
                        case 0x10:
                            if (gDeckEdit.col[gDeckEdit.cursor] + 5 > B3_CNT(gDeckEdit.cursor) - 1)
                                gDeckEdit.col[gDeckEdit.cursor] = 0;
                            else
                                gDeckEdit.col[gDeckEdit.cursor] += 5;
                            gDeckEdit.h18AC = 0xFC00;
                            Ease_Start(0, 6, 1, B3_PSTATE + 0x628);
                            gDeckEdit.b634 ^= 1;
                            B3_CARD(B3_LOOKUP(gDeckEdit.cursor, gDeckEdit.row[gDeckEdit.cursor], gDeckEdit.col[gDeckEdit.cursor]),
                                         0x06008000 + gDeckEdit.b634 * 0x1680, gDeckEdit.b634);
                            B3_MISC(((gDeckEdit.h630 & 0xFF) >> 3) + 29, ((gDeckEdit.h632 & 0xFF) >> 3) + 2, gDeckEdit.b634, 1);
                            gDeckEdit.b635 = 3;
                            B3_FRAME.redraw = 1;
                            B3_SLIDE(B3_CNT(gDeckEdit.cursor), gDeckEdit.col[gDeckEdit.cursor], gDeckEdit.slide);
                            if (B3_FRAME.nextArrowState) {
                                B3_FRAME.nextArrowState = 2;
                                B3_FRAME.nextArrowDirty |= 1;
                            }
                            row = gDeckEdit.col[gDeckEdit.cursor] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < B3_CNT(gDeckEdit.cursor)) {
                                    B3_SLOT(slot, B3_LOOKUP(gDeckEdit.cursor, gDeckEdit.row[gDeckEdit.cursor], row),
                                                 slot + 1, B3_PSTATE + 0x1BB8, B3_PSTATE + 0x18B0);
                                } else {
                                    B3_ROWS[slot].active = 0;
                                }
                            }
                            B3_FRAME.rowAnimationState = 0;
                            B3_FRAME.rowCount = 5;
                            B3_FRAME.rowAnimationTimer = 30;
                        moved:
                            B3_SOUND(0);
                            break;
                        case 0x20:
                            if (gDeckEdit.col[gDeckEdit.cursor] <= 4)
                                gDeckEdit.col[gDeckEdit.cursor] = B3_CNT(gDeckEdit.cursor) - 1;
                            else
                                gDeckEdit.col[gDeckEdit.cursor] -= 5;
                            gDeckEdit.h18AC = 0xFC00;
                            Ease_Start(6, 0, -1, B3_PSTATE + 0x628);
                            gDeckEdit.b634 ^= 1;
                            B3_CARD(B3_LOOKUP(gDeckEdit.cursor, gDeckEdit.row[gDeckEdit.cursor], gDeckEdit.col[gDeckEdit.cursor]),
                                         0x06008000 + gDeckEdit.b634 * 0x1680, gDeckEdit.b634);
                            B3_MISC(((gDeckEdit.h630 & 0xFF) >> 3) + 9, ((gDeckEdit.h632 & 0xFF) >> 3) + 2, gDeckEdit.b634, 1);
                            gDeckEdit.h630 -= 0x50;
                            gDeckEdit.b635 = 4;
                            B3_FRAME.redraw = 1;
                            B3_SLIDE(B3_CNT(gDeckEdit.cursor), gDeckEdit.col[gDeckEdit.cursor], gDeckEdit.slide);
                            if (B3_FRAME.previousArrowState) {
                                B3_FRAME.previousArrowState = 2;
                                B3_FRAME.previousArrowDirty |= 1;
                            }
                            row = gDeckEdit.col[gDeckEdit.cursor] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < B3_CNT(gDeckEdit.cursor)) {
                                    B3_SLOT(slot, B3_LOOKUP(gDeckEdit.cursor, gDeckEdit.row[gDeckEdit.cursor], row),
                                                 slot + 1, B3_PSTATE + 0x1BB8, B3_PSTATE + 0x18B0);
                                } else {
                                    B3_ROWS[slot].active = 0;
                                }
                            }
                            B3_FRAME.rowAnimationState = 0;
                            B3_FRAME.rowCount = 5;
                            B3_FRAME.rowAnimationTimer = 30;
                            B3_SOUND(0);
                            break;
                        }
                    }
                    break;
                }
            }
            if (keys == 1) {
                B3_PHASE = 1;
                B3_SOUND(1);
                break;
            }
            SideDeckSwap_HandleListSwitch(&gUnk_0201F73C);
            break;
        case 1:
            if (B3_FRAME.menuAnimationState == 0 && B3_FRAME.menuOwner == -1) {
                switch (keys) {
                case 0x10:
                    if (gDeckEdit.selector != 0)
                        break;
                    if (++B3_FRAME.menuSelected == 1)
                        B3_FRAME.menuSelected++;
                    switch (gDeckEdit.cursor + 1) {
                    case 2:
                        if ((B3_MENU_RAW & 0x38000) == 0x18000)
                            B3_FRAME.menuSelected++;
                        break;
                    case 3:
                        if ((B3_MENU_RAW & 0x38000) == 0x10000)
                            B3_FRAME.menuSelected++;
                        break;
                    }
                    if ((B3_MENU_RAW & 0x38000) == 0x38000)
                        B3_FRAME.menuSelected = 0;
                    /* FAKEMATCH: one more use of the GCSE copy of &gDeckEdit lifts its allocation
                     * priority above the menu-arm constants, so it keeps r6 as in the ROM. */
                    asm("" : : "r"(&gDeckEdit));
                    B3_PHASE = 3;
                    goto menu_moved;
                case 0x20:
                    if (gDeckEdit.selector != 0)
                        break;
                    if ((B3_MENU_RAW & 0x38000) == 0) {
                        B3_FRAME.menuSelected = 6;
                    } else {
                        if (--B3_FRAME.menuSelected == 1)
                            B3_FRAME.menuSelected = (u16)(B3_FRAME.menuSelected - 1);
                        switch (gDeckEdit.cursor + 1) {
                        case 2:
                            if ((B3_MENU_RAW & 0x38000) == 0x18000)
                                B3_FRAME.menuSelected = (u16)(B3_FRAME.menuSelected - 1);
                            break;
                        case 3:
                            if ((B3_MENU_RAW & 0x38000) == 0x10000)
                                B3_FRAME.menuSelected = (u16)(B3_FRAME.menuSelected - 2);
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
                        B3_PHASE = 3;
                        asm("" : : "r"(busy));
                    }
                menu_moved:
                    B3_SOUND(0);
                    break;
                case 1:
                    switch (B3_FRAME.menuSelected) {
                    case 0:
                        gDeckEdit.exitMode = 2;
                        B3_FADE_INT(0, 0x180, 0, B3_PSTATE + 0x618);
                        B3_SOUND(1);
                        break;
                    case 2:
                        if (DeckEdit_GetSelectedCardCopies() != 0) {
                            gDeckEdit.selector++;
                            B3_SOUND(1);
                        }
                        break;
                    case 3:
                        if (DeckEdit_GetSelectedCardCopies() != 0) {
                            gDeckEdit.selector++;
                            B3_SOUND(1);
                        }
                        break;
                    case 4:
                        gDeckEdit.exitMode = 1;
                        B3_FADE_U16(0, 0x180, 0, B3_PSTATE + 0x618);
                        B3_SOUND(1);
                        break;
                    case 5:
                        gDeckEdit.exitMode = 3;
                        B3_FADE(0, 0x180, 0, B3_PSTATE + 0x618);
                        B3_SOUND(1);
                        break;
                    case 6:
                        B3_FADE(0, 0x180, 0, B3_PSTATE + 0x618);
                        gDeckEdit.exitMode = 0;
                        B3_SOUND(1);
                        break;
                    }
                    break;
                case 2:
                    if (gDeckEdit.selector == 2) {
                        SideDeckSwap_CancelExchange();
                        B3_SOUND(2);
                    }
                    break;
                default:
                    SideDeckSwap_HandleListSwitch(&gUnk_0201F73C);
                    if (*((u8 *)&gUnk_0201F73C - 0x15F4) != 1) {
                        switch (keys) {
                        case 0x40: DeckEdit_ScrollListUp(&row); break;
                        case 0x80: DeckEdit_ScrollListDown(&row); break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }

    SideDeckSwap_UpdateExchange();
    OamListAddSpriteGroup(gUnk_081A6524, 5, 12, -1, -1, 3, 2, 0, 0, 0, 0, &gDeckEdit);
    B3_FRAME.frameDirty = 1;
    B3_SLIDE_DRAW(gDeckEdit.col[gDeckEdit.cursor], B3_CNT(gDeckEdit.cursor), gDeckEdit.slide);
    DeckEdit_DrawFrameSlots(B3_PSTATE + 0x1BBC, &gDeckEdit);
    for (slot = 1; slot <= 6; slot++)
        ObjAffineApply(B3_PSTATE + 0x18B0 + slot * 24);
    tail = gUnk_0201F740;
    B3_TAIL0();
    list = tail - 4;
    DeckEdit_UpdatePanelHighlight(list, tail - 3, objects = tail - 0x508);
    DeckEdit_UpdateCommandMenuAnim(tail + 0x1C);
    DeckEdit_DrawCommandMenu(tail + 0x1C);
    OamListAddSpriteGroup(gUnk_081A6EA4, 0, 1, -1, -1, 0, 0, 1, 1, 0, 0, state = (struct DeckState *)(tail - 0x1C20));
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
        REG16(0x50) = 0x3FC8;
        REG16(0x52) = 0x1000;
        goto done;
    }
    goto fade;
complete:
    return 1;
fade:
    if (tail[-0x15F8] == 0 && tail[-0x1602] == 0) {
        if (*(s16 *)(tail - 0x374) >= 0) {
            REG16(0x50) = 0x3FC8;
            REG16(0x54) = *(s16 *)(tail - 0x374) >> 8;
        } else {
            REG16(0x50) = 0x3F88;
            REG16(0x54) = -*(s16 *)(tail - 0x374) >> 8;
        }
        if ((s16)gDeckEdit.h18AC > 0x3FF)
            gDeckEdit.h18AC = 0x400;
        else
            gDeckEdit.h18AC += 0x30;
    }
done:
    return 0;
}

