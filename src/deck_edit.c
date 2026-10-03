#include "global.h"
#include "gba.h"

/* gMain (0x03000040) fields used here. */
struct Main {
    u8 pad0[6];
    u16 keys;          /* +6: current key bits */
    u8 pad8[0x40E - 8];
    u16 vblankFlags;    /* +0x40E */
    u8 pad410[0x4859 - 0x410];
    u8 step;            /* +0x4859 */
    u8 sub1;            /* +0x485A */
    u8 sub2;            /* +0x485B */
    u8 pad485C[0x4872 - 0x485C];
    u16 unk4872;        /* +0x4872 */
    u8 mode4874 : 2;    /* +0x4874 bits 0-1 */
    u8 rest4874 : 6;
    u8 pad4875[0x4878 - 0x4875];
};
extern struct Main gMain;

/* Deck edit scene state at 0x0201DB20 (see deck_edit_cards). */
struct DeckState {
    u8 pad0[0x620];
    u16 arr620[3];      /* +0x620 */
    u8 pad626[2];
    u8 b628;             /* +0x628: animation state */
    u8 pad629;
    s16 h62A;            /* +0x62A: signed animation phase */
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
extern struct DeckState gDeckEdit;
extern u16 gCardDetail;
extern u8 gUnk_0201E138[];
extern void FadeTick(void *p);
extern u16 DeckEdit_GetListCard(u8 list, u8 row, u16 col);
extern void CardDetail_Init(u16 id, int a, int b);
extern u16 (*const gDeckEditSteps[])(void);

int DeckEdit_TickFadeIn(void)
{
    FadeTick(gUnk_0201E138);
    if (gUnk_0201E138[6] == 3)
        return 1;
    return 0;
}
int DeckEdit_SwitchScreen(void)
{
    gMain.sub1 = 0;
    switch (gDeckEdit.mode) {
    case 0:
        return 1;
    case 1:
        gMain.step = 5;
        return 0;
    case 2:
        gMain.step = 0xD;
        gCardDetail &= 1;
        CardDetail_Init(DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor]), 0, 0);
        return 0;
    case 3:
        gMain.step = 9;
        return 0;
    case 4:
        gMain.step = 1;
        return 0;
    default:
        return 1;
    }
}
int TradeCardSelect_SwitchScreen(void)
{
    gMain.sub1 = 0;
    switch (gDeckEdit.mode) {
    case 0:
        return 1;
    case 1:
        gMain.sub2 = 5;
        return 0;
    case 2:
        gMain.sub2 = 0xD;
        gCardDetail &= 1;
        CardDetail_Init(DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor]), 0, 0);
        return 0;
    case 3:
        gMain.sub2 = 9;
        return 0;
    case 4:
        gMain.sub2 = 1;
        return 0;
    default:
        return 1;
    }
}
/* Deck Edit scene callback: runs step gDeckEditSteps[gMain.step]; advances when it returns non-zero, returns 1 at the end of the table. */
u16 CB_DeckEdit(void)
{
    if (gMain.step == 0)
        gMain.mode4874 = 0;
    gDeckEdit.flag = 0;
    if (gDeckEditSteps[gMain.step] != 0) {
        if (gDeckEditSteps[gMain.step]())
            gMain.step++;
        return 0;
    }
    return 1;
}
extern u16 (*const gSideDeckSwapSteps[])(void);
extern u16 (*const gTradeCardSelectSteps[])(void);
extern u16 (*const gProhibitCardSelectSteps[])(void);
extern const u8 gStrDebugSwapSelectorChangedFmt[];
extern void DebugPrintf(const void *a, int b, int c);
extern void DebugPrintFlush(void);

u16 SideDeckSwap_Run(void)
{
    struct Main *m = &gMain;
    u8 *step;
    struct Main *runner;
    u8 state;
    u16 (*fn)(void);
    u8 old;
    state = m->step;
    runner = m;
    switch (state) {
    case 0:
        m->mode4874 = 0;
        break;
    case 1:
        gDeckEdit.flag = 1;
        break;
    }
    fn = gSideDeckSwapSteps[*(step = &runner->step)];
    if (fn != 0) {
        old = gDeckEdit.sel;
        if (fn())
            (*step)++;
        if (old != gDeckEdit.sel) {
            DebugPrintf(gStrDebugSwapSelectorChangedFmt, old, gDeckEdit.sel);
            DebugPrintFlush();
        }
        return 0;
    }
    return 1;
} /* 0x0806EF74 size 0xA8 */
/* Sub-step runner on gMain+0x485B (table 0x081A72E4). */
u16 TradeCardSelect_Run(void)
{
    if (gTradeCardSelectSteps[gMain.sub2] != 0) {
        if (gTradeCardSelectSteps[gMain.sub2]())
            gMain.sub2++;
        return 0;
    }
    return 1;
}
extern u8 gUnk_02017A40_b[] asm("gChain");
#define STEP2 (gUnk_02017A40_b[0x3E6])

/* Step runner on the byte 0x02017A40+0x3E6 (table 0x081A7330). */
u16 ProhibitCardSelect_Run(void)
{
    if (STEP2 == 0)
        gMain.mode4874 = 2;
    if (gProhibitCardSelectSteps[STEP2] != 0) {
        if (gProhibitCardSelectSteps[STEP2]())
            STEP2++;
        return 0;
    }
    return 1;
}
u16 ProhibitCardSelect_StartAndRun(void)
{
    struct Main *m = &gMain;
    u8 *sub = &m->sub2;
    u8 v = *sub;
    if (v == 0) {
        STEP2 = v;
        (*sub)++;
        return 0;
    }
    return ProhibitCardSelect_Run();
}
/* Panel state at 0x0201F6D8 (hypothesis: deck-edit card panel); bitfield bytes at +0x7C, +0x84 (word), +0x85, +0xA2. */
struct Panel {
    u8 pad0[0x100];
};
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
extern const u8 gDeckEditAnimScripts[];

/* Deck Edit scene init: clears the state block, resets the BG scroll registers, builds the card lists. */
int ProhibitCardSelect_Init(void)
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
    for (i = 1; CARD_NUMBER(i) != 0xFFFF; ) {
        if ((u16)(CARD_NUMBER(i) - 0x76C) > 0x63) {
            DeckEdit_SetListCard(i, 0, gDeckEdit.arr14A0[0], gDeckEdit.cnt1494[gDeckEdit.arr14A0[0]][0]++);
        }
        i++;
        if (i > 0x334)
            break;
    }
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

void sub_0806F400(void)
{
}
extern const u8 gProhibitSelectFrameMap[], gProhibitSelectBgTiles[], gDeckEditLabelTiles[], gDeckEditObjTiles[], gDeckEditCardStackObjTiles[], gDeckEditCardIconObjTiles[], gDeckEditCardFrameObjTiles[];
extern const u8 gProhibitSelectBgPals4to7[], gProhibitSelectBgPals1to3[], gProhibitSelectBgPal3[], gDeckEditObjPal[], gUnk_08704EE8[];
extern u8 gUnk_0201F73C;
extern u16 gUnk_0201E140[];
extern u8 gUnk_0201F3D0[];
void CopyMapRect(const void *src, void *dst, u32 w, u32 h);
void CopyTileSheetTo2D(const void *src, void *dst, u32 n);
void sub_08066164(u8 *dst);
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
    extern u32 f404_lookup(u8, u8, int) asm("DeckEdit_GetListCard");
    extern void f404_row(int, u8 *, int, int, u32, int) asm("DeckEdit_DrawListRowName");
    extern void f404_obj(int, int, int, u8 *, u8 *) asm("DeckEdit_InitFrameSlot");
    extern void f404_detail(int, u8 *, int, int, void *) asm("DeckEdit_DrawCursorRowName");
    extern void f404_clear(int, u8 *, int, int, void *) asm("DeckEdit_DrawNoCardsText");
    /* FAKEMATCH: the staged array address preserves the second loop's register allocation. */
    u8 (*objectRows)[];
    extern void f404_card(int, u32, int) asm("LoadCardArt8bpp");
    extern void f404_misc(int, int, int, int) asm("DeckEdit_PlaceCardArt");
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
    sub_08066164((u8 *)0x06006000);
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
    objectRows = &gUnk_0201F3D0;
    OBJF(0x13A) |= 0xFF;
    OBJF(0x14E) |= 0xFF;
    CpuFastSet(gProhibitSelectBgPals4to7, (void *)0x05000080, 0x20);
    CpuFastSet(gProhibitSelectBgPals1to3, (void *)0x05000020, 0x18);
    CpuFastSet(gProhibitSelectBgPal3, (void *)0x05000060, 8);
    CpuSet(gDeckEditObjPal, (void *)0x05000200, 0x100);
    *(u16 *)0x05000044 = 0x7758;
    DeckEdit_LoadCardIconTiles((u8 *)0x06002000);
    CpuSet(gUnk_08704EE8, (void *)0x05000000, 0x10);
#define CURLIST gUnk_0201F73C
#define CNT(list) (gDeckEdit.cnt1494[gDeckEdit.arr14A0[list]][list])
    k = 1;
    for (j = 0; j < 2; j++) {
        if ((s16)gDeckEdit.arr620[CURLIST] + (s16)k - 3 >= 0) {
            f404_row(f404_lookup(CURLIST, gDeckEdit.arr14A0[CURLIST], gDeckEdit.arr620[CURLIST] + (s16)k - 3),
                         (u8 *)0x0600D000, 0, ((s16)k - 1) * 2, (u32)(PSTATE + 0x640), -(s16)k + 2);
            f404_obj((s16)k - 1, f404_lookup(CURLIST, gDeckEdit.arr14A0[CURLIST], gDeckEdit.arr620[CURLIST] + (s16)k - 3),
                         (s16)k, PSTATE + 0x1BB8, *objectRows);
            k = (s16)k + 1;
        }
    }
    for (j = 0; j < 2; j = next) {
        int row = gUnk_0201E140[CURLIST] + j;
        int count = CNT(CURLIST) - 1;
        next = j + 1;
        if (row < count) {
            f404_row(f404_lookup(CURLIST, gDeckEdit.arr14A0[CURLIST], gUnk_0201E140[CURLIST] + next),
                         (u8 *)0x0600D000, 0, j * 2 + 9, (u32)(PSTATE + 0x640), j + 4);
            f404_obj(j + 3, f404_lookup(CURLIST, gDeckEdit.arr14A0[CURLIST], gUnk_0201E140[CURLIST] + j + 1),
                         j + 4, PSTATE + 0x1BB8, *objectRows);
        }
    }
    if (CNT(CURLIST) != 0) {
        f404_detail(f404_lookup(CURLIST, gDeckEdit.arr14A0[CURLIST], gDeckEdit.arr620[CURLIST]),
                     (u8 *)0x0600C000, 0, ((gDeckEdit.h63E + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
        f404_obj(2, f404_lookup(CURLIST, gDeckEdit.arr14A0[CURLIST], gDeckEdit.arr620[CURLIST]),
                     3, PSTATE + 0x1BB8, PSTATE + 0x18B0);
        f404_card(f404_lookup(CURLIST, gDeckEdit.arr14A0[CURLIST], gDeckEdit.arr620[CURLIST]),
                     0x06008000 + gDeckEdit.b634 * 0x1680, gDeckEdit.b634);
        f404_misc(0x13, ((gDeckEdit.h632 & 0xFF) >> 3) + 2, gDeckEdit.b634, 1);
        DeckEdit_DrawCardIcons(0);
        DeckEdit_DrawAtkDef((u8 *)0x0600C000, 0xB, 7, PSTATE + 0x640);
        DeckEdit_DrawLevelStars((u8 *)0x0600C000, 0x11, 7, 6);
    } else {
        f404_clear(f404_lookup(CURLIST, gDeckEdit.arr14A0[CURLIST], gDeckEdit.arr620[CURLIST]),
                     (u8 *)0x0600C000, 0, ((gDeckEdit.h63E + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
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
    FadeStart(0, -0x180, 0, gUnk_0201E138);
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
extern const u16 gDeckEditEaseCurve[];
extern const u8 gUnk_081A6524[], gUnk_081A6EA4[];
void DeckEdit_DrawScrollBar(u16 position, u16 count, u16 *state);
void DeckEdit_TweenFrameSlots(s16 step, u8 tweenState, u8 direction, u8 *objects, u8 *rows);
void DeckEdit_DrawFrameSlots(u8 *rows, struct DeckState *state);
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

struct DeckPhaseFields {
    u8 pad0[0x1C3D];
    u8 phase : 3;               /* +0x1C3D bits 0-2 */
    u8 rest3D : 5;
};
#define DECK_PHASE (((struct DeckPhaseFields *)&gDeckEdit)->phase)
/* FAKEMATCH: same callee under other return types, so the three menu transition calls are not cross-jumped. */
#define sub_080787F4_int ((int (*)(u32, u32, u32, void *))FadeStart)
#define sub_080787F4_u16 ((u16 (*)(u32, u32, u32, void *))FadeStart)
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
                    PlaySE(3);
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
                    switch (++DECK_FRAME.menuSelected) {
                    case 1:
                        DECK_FRAME.menuSelected = 4;
                        break;
                    case 7:
                        DECK_FRAME.menuSelected = 0;
                        break;
                    case 5:
                        DECK_FRAME.menuSelected = 6;
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
                    case 6:
                        DECK_FRAME.menuSelected = 4;
                        break;
                    default:
                        DECK_FRAME.menuSelected = (u16)(DECK_FRAME.menuSelected - 1);
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
                    ((struct DeckPhaseByte *)pp)->phase = 3;
                    PlaySE(0);
                    break;
                case A_BUTTON:
                    switch (DECK_FRAME.menuSelected) {
                    case 0:
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
    OamListAddSpriteGroup(gUnk_081A6524, 5, 11, -1, -1, 3, 2, 0, 0, 0, 0, &gDeckEdit);
    DECK_FRAME.frameDirty = 1;
    DeckEdit_DrawScrollBar(gDeckEdit.arr620[gDeckEdit.cursor], CNT(gDeckEdit.cursor), &gDeckEdit.f1BB0);
    DeckEdit_DrawFrameSlots(PSTATE + 0x1BBC, &gDeckEdit);
    for (slot = 1; slot <= 6; slot++)
        ObjAffineApply(PSTATE + 0x18B0 + slot * 24);
    tail = gUnk_0201F740;
    DeckEdit_UpdateCardMove(tail);
    DeckEdit_UpdateCommandMenuAnim(tail + 0x1C);
    DeckEdit_DrawCommandMenu(tail + 0x1C);
    OamListAddSpriteGroup(gUnk_081A6EA4, 0, 1, -1, -1, 0, 0, 1, 1, 0, 0, state = (struct DeckState *)(tail - 0x1C20));
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
        if ((s16)gDeckEdit.h18AC > 0x3FF)
            gDeckEdit.h18AC = 0x400;
        else
            gDeckEdit.h18AC += 0x30;
    }
done:
    return 0;
}

