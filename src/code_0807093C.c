#include "global.h"
#include "gba.h"

/* gMain (0x03000040) fields used here. */
struct Main {
    u8 pad0[6];
    u16 keys;          /* +6: current key bits */
    u8 pad8[0x40E - 8];
    u16 vblankFlags;    /* +0x40E */
    u8 pad410[0x485A - 0x410];
    u8 sub1;            /* +0x485A */
    u8 pad485B[0x4872 - 0x485B];
    u16 unk4872;        /* +0x4872 */
    u8 mode4874 : 2;    /* +0x4874 bits 0-1: card filter mode (hypothesis) */
    u8 rest4874 : 6;
    u8 pad4875[0x4878 - 0x4875];
};
extern struct Main gMain;

extern struct Main gMain;

/* Deck edit scene state at 0x0201DB20 (see code_08068180). */
struct DeckState {
    u8 pad0[0x620];
    u16 arr620[3];      /* +0x620 */
    u8 pad626[2];
    u8 b628;
    u8 pad629;
    s16 h62A;
    u8 pad62C[4];
    u16 h630;           /* +0x630 */
    u16 h632;
    u8 b634;
    u8 b635;
    u16 h636;
    u16 h638;
    u16 h63A;
    u16 h63C;
    u16 h63E;
    u8 pad640[0x1494 - 0x640];
    u16 cnt1494[2][3];  /* +0x1494 counters [row][cursor] */
    u8 arr14A0[3];      /* +0x14A0 */
    u8 pad14A3[0x1710 - 0x14A3];
    u8 f1710_0 : 1;     /* +0x1710 bit 0 */
    u8 f1710_1 : 7;
    u8 pad1711[0x18AC - 0x1711];
    u16 h18AC;          /* +0x18AC */
    u8 pad18AE[0x1BB0 - 0x18AE];
    u16 f1BB0;          /* +0x1BB0 */
    u16 pad1BB2;
    u8 b1BB4_0 : 1;
    u8 b1BB4_1 : 7;
    u8 b1BB5;
    u8 b1BB6_0 : 1;
    u8 b1BB6_1 : 7;
    u8 b1BB7;
    u8 pad1BB8[0x1C1C - 0x1BB8];
    u8 cursor;          /* +0x1C1C */
    u8 b1C1D;
    u8 pad1C1E[0x1C48 - 0x1C1E];
    u8 f1C48_0 : 1;     /* +0x1C48 bit 0 */
    u8 mode : 4;        /* +0x1C48 bits 1-4 */
    u8 f1C48_5 : 3;
    u8 pad1C49[0x1C5A - 0x1C49];
    u8 f1C5A_0 : 2;     /* +0x1C5A bits 0-1 */
    u8 sel : 3;         /* bits 2-4 */
    u8 flag : 1;        /* bit 5 */
    u8 f1C5A_6 : 2;
    u8 pad1C5B[3];
};
/* Panel state at 0x0201F6D8 (hypothesis: deck-edit card panel); bitfield bytes at +0x7C, +0x84 (word), +0x85, +0xA2. */
struct Panel {
    u8 pad0[0x100];
};
extern struct DeckState gDeckEdit;
struct B7C {
    u8 a : 1;
    u8 b : 4;
    u8 c : 3;
    u8 pad[7];
};
struct BA2 {
    u8 a : 2;
    u8 b : 3;
    u8 c : 3;
    u8 pad[7];
};
struct W84 {
    u32 a : 15;
    u32 b : 3;
    u32 c : 14;
    u8 pad[4];
};
struct B85 {
    u8 a : 3;
    u8 b : 5;
    u8 pad[7];
};
extern struct Panel gUnk_0201F6D8;
/* Save-data card trunk entry (0x02011C20 + 8, u32 per card id). */
struct TrunkEntry {
    u16 owned : 10;
    u8 f1 : 2;
    u8 f2 : 2;
    u8 f3 : 2;
};
struct Trunk {
    u8 pad0[8];
    struct TrunkEntry e[1];
};
extern struct Trunk gSaveData;
extern u16 gCardIdToNumber[];
/* Integer-address indexing preserves the target card-list loop allocation. */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
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
extern const u8 gDeckEditAnimScripts[];

/* Menu state at 0x02017A40: +0x3E6 step, +0x3E7 sub-step. */
struct MenuState {
    u8 pad0[0x3E6];
    u8 step;            /* +0x3E6 */
    u8 sub;             /* +0x3E7 */
    u8 pad3E8[0x400 - 0x3E8];
};
extern struct MenuState gChain;
extern u16 gCardDetail;
extern u16 DeckEdit_GetListCard(u8 list, u8 row, u16 col);
extern void CardDetail_Init(u16 id, int a, int b);

/* Deck Edit exit: like DeckEdit_SwitchScreen but the step goes into gChain.step. */
int ProhibitCardSelect_SwitchScreen(void)
{
    gMain.sub1 = 0;
    gChain.sub = 0;
    switch (gDeckEdit.mode) {
    case 0:
        return 1;
    case 1:
        gChain.step = 5;
        return 0;
    case 2:
        gChain.step = 0xD;
        gCardDetail &= 1;
        CardDetail_Init(DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor]), 0, 0);
        return 0;
    case 3:
        gChain.step = 9;
        return 0;
    case 4:
        gChain.step = 1;
        return 0;
    default:
        return 1;
    }
}

/* Deck Edit scene init (variant of ProhibitCardSelect_Init with a card-list filter by gMain+0x4874 mode): clears the state block, resets BG scroll, builds the three card lists. */
int TradeCardSelect_Init(void)
{
    u16 i;
    u16 j;
    struct B7C *b7c;
    struct BA2 *ba2;
    struct W84 *w84;
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
    gDeckEdit.h632 = 0;
    gDeckEdit.h630 = 0;
    gDeckEdit.h63A = 0;
    gDeckEdit.h638 = 0;
    gDeckEdit.h63E = 0;
    gDeckEdit.h63C = 0;
    for (i = 0; i <= 2; i++) {
        gDeckEdit.arr620[i] = 0;
        gDeckEdit.arr14A0[i] = 0;
        for (j = 0; j <= 1; j++)
            gDeckEdit.cnt1494[j][i] = 0;
    }
    gDeckEdit.b1C1D = 0;
    gDeckEdit.cursor = 0;
    gDeckEdit.b634 = 0;
    gDeckEdit.b635 = 0;
    gDeckEdit.f1710_0 = 0;
    gDeckEdit.h18AC = 0xFC00;
    Ease_Init(0, 0, 0, (u8 *)&gDeckEdit + 0x628);
    ClearKatakanaFlag((u8 *)&gDeckEdit + 0x640);
    ObjAffineInit((u8 *)&gDeckEdit + 0x18B0);
    switch (gMain.mode4874) {
    case 1:
        for (i = 1; i <= 0x334 && CARD_NUMBER(i) != 0xFFFF; i++) {
            if ((u16)(CARD_NUMBER(i) - 0x4BA) <= 0x315 && (u16)(CARD_NUMBER(i) - 0x76C) > 0x13)
                continue;
            if (gSaveData.e[i].owned)
                DeckEdit_SetListCard(i, 0, gDeckEdit.arr14A0[0], gDeckEdit.cnt1494[gDeckEdit.arr14A0[0]][0]++);
            if (gSaveData.e[i].f1 || gSaveData.e[i].f3)
                DeckEdit_SetListCard(i, 1, gDeckEdit.arr14A0[1], gDeckEdit.cnt1494[gDeckEdit.arr14A0[1]][1]++);
            if (gSaveData.e[i].f2)
                DeckEdit_SetListCard(i, 2, gDeckEdit.arr14A0[2], gDeckEdit.cnt1494[gDeckEdit.arr14A0[2]][2]++);
        }
        break;
    case 0:
        for (i = 1; i <= 0x334 && CARD_NUMBER(i) != 0xFFFF; i++) {
            if ((u16)(CARD_NUMBER(i) - 0x780) > 0x4F) {
                if (gSaveData.e[i].owned)
                    DeckEdit_SetListCard(i, 0, gDeckEdit.arr14A0[0], gDeckEdit.cnt1494[gDeckEdit.arr14A0[0]][0]++);
                if (gSaveData.e[i].f1 || gSaveData.e[i].f3)
                    DeckEdit_SetListCard(i, 1, gDeckEdit.arr14A0[1], gDeckEdit.cnt1494[gDeckEdit.arr14A0[1]][1]++);
                if (gSaveData.e[i].f2)
                    DeckEdit_SetListCard(i, 2, gDeckEdit.arr14A0[2], gDeckEdit.cnt1494[gDeckEdit.arr14A0[2]][2]++);
            }
        }
        break;
    }
    DeckEdit_CountSideDeckMonsters();
    DeckEdit_CalcScrollBar(gDeckEdit.cnt1494[gDeckEdit.arr14A0[gDeckEdit.cursor]][gDeckEdit.cursor],
                 gDeckEdit.arr620[gDeckEdit.cursor], &gDeckEdit.f1BB0);
    gDeckEdit.b1BB4_0 = 1;
    gDeckEdit.b1BB6_0 = 1;
    if (gDeckEdit.cnt1494[gDeckEdit.arr14A0[gDeckEdit.cursor]][gDeckEdit.cursor] > 5)
        gDeckEdit.b1BB7 = gDeckEdit.b1BB5 = 1;
    else
        gDeckEdit.b1BB7 = gDeckEdit.b1BB5 = 0;
    DeckEdit_ResetFrameSlots(&gUnk_0201F6D8);
    DeckEdit_ResetCardMove((u8 *)&gUnk_0201F6D8 + 0x68);
    b7c = (struct B7C *)((u8 *)&gUnk_0201F6D8 + 0x7C);
    b7c->a = 0;
    b7c->b = 0;
    ba2 = (struct BA2 *)((u8 *)&gUnk_0201F6D8 + 0xA2);
    ba2->a = 2;
    w84 = (struct W84 *)((u8 *)b7c + 8);
    w84->b = 0;
    ((struct B85 *)((u8 *)w84 + 1))->a = 3;
    ba2->b = 0;
    AnimBlockInit(gDeckEditAnimScripts, (u8 *)&gUnk_0201F6D8 - 0x4A0);
    gMain.unk4872 = 0;
    return 1;
}

void CardSelect_HandleListSwitch(void)
{
}
/* TradeCardSelect_Update is a near copy of the matched DeckEdit_Update (code_0806D51C); the local
 * declarations below mirror that unit so the shared code compiles identically. */
struct DeckFrameFields {
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
struct DeckMenuRawFields {
    u8 pad0[0x1C3C];
    u32 word;
};
#define DECK_MENU_RAW (((struct DeckMenuRawFields *)&gDeckEdit)->word)
struct DeckPhaseByte { u8 phase : 3; u8 rest : 5; };
struct DeckRowFrame {
    u8 pad0[12];
    u8 active;
    u8 padD[3];
};
struct DeckRowFields {
    u8 pad0[0x1BB8];
    struct DeckRowFrame rows[6];
};
#define DECK_ROWS ((struct DeckRowFields *)&gDeckEdit)->rows
#define DECK_FRAME (*(struct DeckFrameFields *)&gDeckEdit)
struct DeckScrollTween {
    u8 state;                 /* +0x00 (DeckState +0x628) */
    u8 pad1;
    s16 value;                /* +0x02 current tween step */
    u8 pad4[9];
    u8 direction;             /* +0x0D (DeckState +0x635) */
};
extern struct DeckScrollTween gUnk_0201E148;
extern u8 gUnk_0201F740[];
extern u8 gUnk_0201F73C;
extern u16 gUnk_0201E140[];
extern const u16 gDeckEditEaseCurve[];
extern const u8 gUnk_081A6524[], gUnk_081A6EA4[];
void DeckEdit_DrawScrollBar(u16 position, u16 count, u16 *state);
void DeckEdit_TweenFrameSlots(s16 step, u8 tweenState, u8 direction, u8 *objects, u8 *rows);
void DeckEdit_DrawFrameSlots(u8 *rows, struct DeckState *state);
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
#define CNT(list) (gDeckEdit.cnt1494[gDeckEdit.arr14A0[list]][list])
#define DECK_TWEEN_STATE DECK_FRAME.tweenState
#define DECK_TWEEN_STEP DECK_FRAME.tweenStep
#define DECK_SCROLL_CURVE gDeckEditEaseCurve[DECK_TWEEN_STEP]
#define DeckEdit_GetListCard sub_08068D1C_word
#define CardSelect_HandleListSwitch sub_08070F14_arg

/* Deck Edit (variant) card-list frame: animate, handle list/menu input, draw, and fade. */
struct DeckPhaseFields {
    u8 pad0[0x1C3D];
    u8 phase : 3;               /* +0x1C3D bits 0-2 */
    u8 rest3D : 5;
};
#define DECK_PHASE (((struct DeckPhaseFields *)&gDeckEdit)->phase)
/* FAKEMATCH: same callee under other return types, so the three menu transition calls are not cross-jumped. */
#define sub_080787F4_int ((int (*)(u32, u32, u32, void *))FadeStart)
#define sub_080787F4_u16 ((u16 (*)(u32, u32, u32, void *))FadeStart)
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
    struct DeckState *state;

    keys = gMain.keys & 0x3FF;
    Ease_Tick(&gUnk_0201E148);
    /* Crossing the midpoint of a horizontal page slide changes the visible rows. */
    if ((*((u8 *)&gUnk_0201E148 + 0x10E8) & 1) &&
        ((gUnk_0201E148.value == 4 && gUnk_0201E148.direction == 3) ||
         (gUnk_0201E148.value == 3 && gUnk_0201E148.direction == 4))) {
        DECK_FRAME.redraw = 0;
        clear = 0;
        CpuFastSet((const void *)&clear, (void *)0x0600D000, 0x01000200);
        if ((s16)gUnk_0201E140[CURLIST] - 2 >= 0) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.arr14A0[CURLIST], gUnk_0201E140[CURLIST] - 2),
                         (u8 *)0x0600D000, 0, (u8)gDeckEdit.h63A >> 3, (u32)(PSTATE + 0x640), 1);
        }
        if ((s16)gUnk_0201E140[CURLIST] - 1 >= 0) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.arr14A0[CURLIST], gUnk_0201E140[CURLIST] - 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.h63A + 0x10) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 2);
        }
        if ((s16)gUnk_0201E140[CURLIST] + 1 < CNT(CURLIST)) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.arr14A0[CURLIST], gUnk_0201E140[CURLIST] + 1),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.h63A + 0x48) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 4);
        }
        if ((s16)gUnk_0201E140[CURLIST] + 2 < CNT(CURLIST)) {
            DeckEdit_DrawListRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.arr14A0[CURLIST], gUnk_0201E140[CURLIST] + 2),
                         (u8 *)0x0600D000, 0, ((gDeckEdit.h63A + 0x58) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 5);
        }
        FillMapRectWrap(0, 0x0600C000, 0, ((gDeckEdit.h63E + 0x20) & 0xFF) >> 3, 30, 6, PSTATE + 0x640);
        if (CNT(CURLIST) != 0) {
            DeckEdit_DrawCursorRowName(DeckEdit_GetListCard(CURLIST, gDeckEdit.arr14A0[CURLIST], gUnk_0201E140[CURLIST]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.h63E + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
            DeckEdit_DrawCardIcons(0);
            DeckEdit_DrawAtkDef((u8 *)0x0600C000, 11, ((gDeckEdit.h63E + 0x38) & 0xFF) >> 3, PSTATE + 0x640);
            DeckEdit_DrawLevelStars((u8 *)0x0600C000, 17, ((gDeckEdit.h63E + 0x38) & 0xFF) >> 3, 6);
        } else {
            DeckEdit_DrawNoCardsText(DeckEdit_GetListCard(CURLIST, gDeckEdit.arr14A0[CURLIST], gUnk_0201E140[CURLIST]),
                         (u8 *)0x0600C000, 0, ((gDeckEdit.h63E + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
        }
    }
    DeckEdit_TweenFrameSlots(DECK_TWEEN_STEP, DECK_TWEEN_STATE, gDeckEdit.b635, PSTATE + 0x18B0, PSTATE + 0x1BB8);

    /* Directions 1/2 move vertically; 3/4 move a five-card page horizontally. */
    switch (gDeckEdit.b635) {
    case 1:
    case 2:
        switch (DECK_TWEEN_STATE) {
        case 1:
            REG_BG3HOFS = gDeckEdit.h630;
            REG_BG3VOFS = gDeckEdit.h632 + (MulFix8(0x5000, DECK_SCROLL_CURVE) >> 8);
            REG_BG1HOFS = gDeckEdit.h638;
            REG_BG1VOFS = gDeckEdit.h63A + (MulFix8(0x1000, DECK_SCROLL_CURVE) >> 8);
            REG_BG0HOFS = gDeckEdit.h63C;
            REG_BG0VOFS = gDeckEdit.h63E + (MulFix8(0x2800, DECK_SCROLL_CURVE) >> 8);
            break;
        case 2:
            DECK_TWEEN_STATE = 0;
            gDeckEdit.h632 += MulFix8(0x5000, DECK_SCROLL_CURVE) >> 8;
            gDeckEdit.h63A += MulFix8(0x1000, DECK_SCROLL_CURVE) >> 8;
            gDeckEdit.h63E += MulFix8(0x2800, DECK_SCROLL_CURVE) >> 8;
            if (DECK_FRAME.previousArrowState) {
                DECK_FRAME.previousArrowState = 1;
                DECK_FRAME.previousArrowDirty |= 1;
            }
            if (DECK_FRAME.nextArrowState) {
                DECK_FRAME.nextArrowState = 1;
                DECK_FRAME.nextArrowDirty |= 1;
            }
            gDeckEdit.b635 = 0;
            /* fall through */
        default:
            REG_BG3HOFS = gDeckEdit.h630;
            REG_BG3VOFS = gDeckEdit.h632;
            REG_BG1HOFS = gDeckEdit.h638;
            REG_BG1VOFS = gDeckEdit.h63A;
            REG_BG0HOFS = gDeckEdit.h63C;
            REG_BG0VOFS = gDeckEdit.h63E;
            break;
        }
        break;
    case 3:
    case 4:
        switch (DECK_TWEEN_STATE) {
        case 1:
            REG_BG3HOFS = gDeckEdit.h630 + (MulFix8(0x5000, DECK_SCROLL_CURVE) >> 8);
            REG_BG3VOFS = gDeckEdit.h632;
            horizontalOffset = MulFix8(0x4000, DECK_SCROLL_CURVE) >> 8;
            blend = DECK_SCROLL_CURVE >> 3;
            REG_BLDCNT = 0x3F43;
            if (DECK_TWEEN_STEP <= 3) {
                REG_BG1HOFS = horizontalOffset;
                REG_BG1VOFS = gDeckEdit.h63A;
                REG_BG0HOFS = horizontalOffset;
                REG_BG0VOFS = gDeckEdit.h63E;
                SetBldAlpha(blend >> 1);
            } else {
                REG_BG1HOFS = horizontalOffset + 0xFFC0;
                REG_BG1VOFS = gDeckEdit.h63A;
                REG_BG0HOFS = horizontalOffset + 0xFFC0;
                REG_BG0VOFS = gDeckEdit.h63E;
                SetBldAlpha((0x20 - blend) >> 1);
            }
            break;
        case 2:
            DECK_TWEEN_STATE = 0;
            gDeckEdit.h630 += MulFix8(0x5000, DECK_SCROLL_CURVE) >> 8;
            if (DECK_FRAME.previousArrowState) {
                DECK_FRAME.previousArrowState = 1;
                DECK_FRAME.previousArrowDirty |= 1;
            }
            if (DECK_FRAME.nextArrowState) {
                DECK_FRAME.nextArrowState = 1;
                DECK_FRAME.nextArrowDirty |= 1;
            }
            gDeckEdit.b635 = 0;
            /* fall through */
        default:
            REG_BG3HOFS = gDeckEdit.h630;
            REG_BG3VOFS = gDeckEdit.h632;
            REG_BG1HOFS = gDeckEdit.h638;
            REG_BG1VOFS = gDeckEdit.h63A;
            REG_BG0HOFS = gDeckEdit.h63C;
            REG_BG0VOFS = gDeckEdit.h63E;
            REG_BLDCNT = 0x3FC8;
            REG_BLDALPHA = 0x1000;
            break;
        }
        break;
    }

    if (DECK_FRAME.transitionState == 0) {
        switch (gDeckEdit.f1C48_0) {
        case 0:
            if (DECK_TWEEN_STATE != 1) {
                switch (keys) {
                case B_BUTTON:
                    FadeStart(0, 0x180, 0, PSTATE + 0x618);
                    gDeckEdit.mode = 0;
                    PlaySE(2);
                    break;
                case DPAD_UP:
                    DeckEdit_ScrollListUp(&row);
                    break;
                case DPAD_DOWN:
                    DeckEdit_ScrollListDown(&row);
                    break;
                default:
                    if (CNT(gDeckEdit.cursor) > 5) {
                        switch (keys) {
                        case DPAD_RIGHT:
                            if (gDeckEdit.arr620[gDeckEdit.cursor] + 5 > CNT(gDeckEdit.cursor) - 1)
                                gDeckEdit.arr620[gDeckEdit.cursor] = 0;
                            else
                                gDeckEdit.arr620[gDeckEdit.cursor] += 5;
                            gDeckEdit.h18AC = 0xFC00;
                            Ease_Start(0, 6, 1, PSTATE + 0x628);
                            gDeckEdit.b634 ^= 1;
                            LoadCardArt8bpp(DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor]),
                                         0x06008000 + gDeckEdit.b634 * 0x1680, gDeckEdit.b634);
                            DeckEdit_PlaceCardArt(((gDeckEdit.h630 & 0xFF) >> 3) + 29, ((gDeckEdit.h632 & 0xFF) >> 3) + 2, gDeckEdit.b634, 1);
                            gDeckEdit.b635 = 3;
                            DECK_FRAME.redraw = 1;
                            DeckEdit_CalcScrollBar(CNT(gDeckEdit.cursor), gDeckEdit.arr620[gDeckEdit.cursor], &gDeckEdit.f1BB0);
                            if (DECK_FRAME.nextArrowState) {
                                DECK_FRAME.nextArrowState = 2;
                                DECK_FRAME.nextArrowDirty |= 1;
                            }
                            row = gDeckEdit.arr620[gDeckEdit.cursor] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < CNT(gDeckEdit.cursor)) {
                                    DeckEdit_InitFrameSlot(slot, DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], row),
                                                 slot + 1, PSTATE + 0x1BB8, PSTATE + 0x18B0);
                                } else {
                                    DECK_ROWS[slot].active = 0;
                                }
                            }
                            DECK_FRAME.rowAnimationState = 0;
                            DECK_FRAME.rowCount = 5;
                            DECK_FRAME.rowAnimationTimer = 30;
                            PlaySE(0);
                            break;
                        case DPAD_LEFT:
                            if (gDeckEdit.arr620[gDeckEdit.cursor] <= 4)
                                gDeckEdit.arr620[gDeckEdit.cursor] = CNT(gDeckEdit.cursor) - 1;
                            else
                                gDeckEdit.arr620[gDeckEdit.cursor] -= 5;
                            gDeckEdit.h18AC = 0xFC00;
                            Ease_Start(6, 0, -1, PSTATE + 0x628);
                            gDeckEdit.b634 ^= 1;
                            LoadCardArt8bpp(DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor]),
                                         0x06008000 + gDeckEdit.b634 * 0x1680, gDeckEdit.b634);
                            DeckEdit_PlaceCardArt(((gDeckEdit.h630 & 0xFF) >> 3) + 9, ((gDeckEdit.h632 & 0xFF) >> 3) + 2, gDeckEdit.b634, 1);
                            gDeckEdit.h630 -= 0x50;
                            gDeckEdit.b635 = 4;
                            DECK_FRAME.redraw = 1;
                            DeckEdit_CalcScrollBar(CNT(gDeckEdit.cursor), gDeckEdit.arr620[gDeckEdit.cursor], &gDeckEdit.f1BB0);
                            if (DECK_FRAME.previousArrowState) {
                                DECK_FRAME.previousArrowState = 2;
                                DECK_FRAME.previousArrowDirty |= 1;
                            }
                            row = gDeckEdit.arr620[gDeckEdit.cursor] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < CNT(gDeckEdit.cursor)) {
                                    DeckEdit_InitFrameSlot(slot, DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], row),
                                                 slot + 1, PSTATE + 0x1BB8, PSTATE + 0x18B0);
                                } else {
                                    DECK_ROWS[slot].active = 0;
                                }
                            }
                            DECK_FRAME.rowAnimationState = 0;
                            DECK_FRAME.rowCount = 5;
                            DECK_FRAME.rowAnimationTimer = 30;
                            PlaySE(0);
                            break;
                        }
                    }
                    break;
                }
            }
            if (keys == A_BUTTON) {
                DECK_PHASE = 1;
                goto confirm_sound;
            }
            CardSelect_HandleListSwitch(&gUnk_0201F73C);
            break;
        case 1:
            if (DECK_FRAME.menuAnimationState == 0 && DECK_FRAME.menuOwner == -1) {
                switch (keys) {
                case DPAD_RIGHT:
                    if (++DECK_FRAME.menuSelected == 1)
                        DECK_FRAME.menuSelected = 4;
                    else if ((DECK_MENU_RAW & 0x38000) == 0x38000)
                        DECK_FRAME.menuSelected = 0;
                    {
                        /* FAKEMATCH: r5 stays busy over the add so the offset reload takes r0 */
                        register u32 busy asm("r5");
                        pp = (u8 *)&gDeckEdit;
                        asm("" : "=r"(busy));
                        pp += 0x1C3D;
                        asm("" : : "r"(busy));
                    }
                    ((struct DeckPhaseByte *)pp)->phase = 3;
                    PlaySE(0);
                    break;
                case DPAD_LEFT:
                    switch (DECK_FRAME.menuSelected) {
                    case 4:
                        DECK_FRAME.menuSelected = 0;
                        break;
                    case 0:
                        DECK_FRAME.menuSelected = 6;
                        break;
                    default:
                        DECK_FRAME.menuSelected = (u16)(DECK_FRAME.menuSelected - 1);
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
                    ((struct DeckPhaseByte *)pp)->phase = 3;
                    PlaySE(0);
                    break;
                case A_BUTTON:
                    switch (DECK_FRAME.menuSelected) {
                    case 0:
                        if (CNT(gDeckEdit.cursor) == 0)
                            goto error_sound;
                        gDeckEdit.mode = 2;
                        sub_080787F4_int(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case 4:
                        gDeckEdit.mode = 1;
                        sub_080787F4_u16(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case 5:
                        gDeckEdit.mode = 3;
                        FadeStart(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case 6:
                        if (DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor]) != 0
                            && CNT(gDeckEdit.cursor) != 0) {
                            gMain.unk4872 = DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor]);
                            FadeStart(0, 0x180, 0, PSTATE + 0x618);
                            gDeckEdit.mode = 0;
confirm_sound:
                            PlaySE(1);
                            goto draw_frame;
                        }
error_sound:
                        PlaySE(3);
                        break;
                    }
                    break;
                case B_BUTTON:
                    ((struct DeckPhaseByte *)(PSTATE + 0x1C3D))->phase = 2;
                    PlaySE(2);
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
    DeckEdit_DrawScrollBar(gDeckEdit.arr620[gDeckEdit.cursor], CNT(gDeckEdit.cursor), &gDeckEdit.f1BB0);
    DeckEdit_DrawFrameSlots(PSTATE + 0x1BBC, &gDeckEdit);
    for (slot = 1; slot <= 6; slot++)
        ObjAffineApply(PSTATE + 0x18B0 + slot * 24);
    tail = gUnk_0201F740;
    DeckEdit_UpdateCardMove();
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
        if ((s16)gDeckEdit.h18AC > 0x3FF)
            gDeckEdit.h18AC = 0x400;
        else
            gDeckEdit.h18AC += 0x30;
    }
done:
    return 0;
}

