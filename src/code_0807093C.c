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
extern struct Main gUnk_03000040;

extern struct Main gUnk_03000040;

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
extern struct DeckState gUnk_0201DB20;
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
extern struct Trunk gUnk_02011C20;
extern u16 gUnk_08622AB4[];
/* Integer-address indexing preserves the target card-list loop allocation. */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
void sub_08075278(void *dst, u32 size);
void sub_0807A2EC(void *p);
void sub_0807B0EC(int a, int b, int c, void *p);
void sub_080788A0(void *p);
void sub_0807B534(void *p);
void sub_08066244(void *p);
void sub_080666AC(void *p);
void sub_08078670(const void *a, void *b);
void sub_08068DA4(u16 val, u8 list, u8 row, u16 col);
void sub_08065F34(u16 a, u16 b, u16 *out);
void sub_0806710C(void);
extern const u8 gUnk_081A70FC[];

/* Menu state at 0x02017A40: +0x3E6 step, +0x3E7 sub-step. */
struct MenuState {
    u8 pad0[0x3E6];
    u8 step;            /* +0x3E6 */
    u8 sub;             /* +0x3E7 */
    u8 pad3E8[0x400 - 0x3E8];
};
extern struct MenuState gUnk_02017A40;
extern u16 gUnk_02013D90;
extern u16 sub_08068D1C(u8 list, u8 row, u16 col);
extern void sub_0800688C(u16 id, int a, int b);

/* Deck Edit exit: like sub_0806ED64 but the step goes into gUnk_02017A40.step. */
int sub_0807093C(void)
{
    gUnk_03000040.sub1 = 0;
    gUnk_02017A40.sub = 0;
    switch (gUnk_0201DB20.mode) {
    case 0:
        return 1;
    case 1:
        gUnk_02017A40.step = 5;
        return 0;
    case 2:
        gUnk_02017A40.step = 0xD;
        gUnk_02013D90 &= 1;
        sub_0800688C(sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]), 0, 0);
        return 0;
    case 3:
        gUnk_02017A40.step = 9;
        return 0;
    case 4:
        gUnk_02017A40.step = 1;
        return 0;
    default:
        return 1;
    }
}

/* Deck Edit scene init (variant of sub_0806F0F8 with a card-list filter by gMain+0x4874 mode): clears the state block, resets BG scroll, builds the three card lists. */
int sub_08070A1C(void)
{
    u16 i;
    u16 j;
    struct B7C *b7c;
    struct BA2 *ba2;
    struct W84 *w84;
    sub_08075278(&gUnk_0201DB20, 0x1C5C);
    gUnk_03000040.vblankFlags = 1;
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
    sub_0807A2EC(&gUnk_0201DB20);
    gUnk_0201DB20.h632 = 0;
    gUnk_0201DB20.h630 = 0;
    gUnk_0201DB20.h63A = 0;
    gUnk_0201DB20.h638 = 0;
    gUnk_0201DB20.h63E = 0;
    gUnk_0201DB20.h63C = 0;
    for (i = 0; i <= 2; i++) {
        gUnk_0201DB20.arr620[i] = 0;
        gUnk_0201DB20.arr14A0[i] = 0;
        for (j = 0; j <= 1; j++)
            gUnk_0201DB20.cnt1494[j][i] = 0;
    }
    gUnk_0201DB20.b1C1D = 0;
    gUnk_0201DB20.cursor = 0;
    gUnk_0201DB20.b634 = 0;
    gUnk_0201DB20.b635 = 0;
    gUnk_0201DB20.f1710_0 = 0;
    gUnk_0201DB20.h18AC = 0xFC00;
    sub_0807B0EC(0, 0, 0, (u8 *)&gUnk_0201DB20 + 0x628);
    sub_080788A0((u8 *)&gUnk_0201DB20 + 0x640);
    sub_0807B534((u8 *)&gUnk_0201DB20 + 0x18B0);
    switch (gUnk_03000040.mode4874) {
    case 1:
        for (i = 1; i <= 0x334 && CARD_NUMBER(i) != 0xFFFF; i++) {
            if ((u16)(CARD_NUMBER(i) - 0x4BA) <= 0x315 && (u16)(CARD_NUMBER(i) - 0x76C) > 0x13)
                continue;
            if (gUnk_02011C20.e[i].owned)
                sub_08068DA4(i, 0, gUnk_0201DB20.arr14A0[0], gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[0]][0]++);
            if (gUnk_02011C20.e[i].f1 || gUnk_02011C20.e[i].f3)
                sub_08068DA4(i, 1, gUnk_0201DB20.arr14A0[1], gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[1]][1]++);
            if (gUnk_02011C20.e[i].f2)
                sub_08068DA4(i, 2, gUnk_0201DB20.arr14A0[2], gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[2]][2]++);
        }
        break;
    case 0:
        for (i = 1; i <= 0x334 && CARD_NUMBER(i) != 0xFFFF; i++) {
            if ((u16)(CARD_NUMBER(i) - 0x780) > 0x4F) {
                if (gUnk_02011C20.e[i].owned)
                    sub_08068DA4(i, 0, gUnk_0201DB20.arr14A0[0], gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[0]][0]++);
                if (gUnk_02011C20.e[i].f1 || gUnk_02011C20.e[i].f3)
                    sub_08068DA4(i, 1, gUnk_0201DB20.arr14A0[1], gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[1]][1]++);
                if (gUnk_02011C20.e[i].f2)
                    sub_08068DA4(i, 2, gUnk_0201DB20.arr14A0[2], gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[2]][2]++);
            }
        }
        break;
    }
    sub_0806710C();
    sub_08065F34(gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor]][gUnk_0201DB20.cursor],
                 gUnk_0201DB20.arr620[gUnk_0201DB20.cursor], &gUnk_0201DB20.f1BB0);
    gUnk_0201DB20.b1BB4_0 = 1;
    gUnk_0201DB20.b1BB6_0 = 1;
    if (gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor]][gUnk_0201DB20.cursor] > 5)
        gUnk_0201DB20.b1BB7 = gUnk_0201DB20.b1BB5 = 1;
    else
        gUnk_0201DB20.b1BB7 = gUnk_0201DB20.b1BB5 = 0;
    sub_08066244(&gUnk_0201F6D8);
    sub_080666AC((u8 *)&gUnk_0201F6D8 + 0x68);
    b7c = (struct B7C *)((u8 *)&gUnk_0201F6D8 + 0x7C);
    b7c->a = 0;
    b7c->b = 0;
    ba2 = (struct BA2 *)((u8 *)&gUnk_0201F6D8 + 0xA2);
    ba2->a = 2;
    w84 = (struct W84 *)((u8 *)b7c + 8);
    w84->b = 0;
    ((struct B85 *)((u8 *)w84 + 1))->a = 3;
    ba2->b = 0;
    sub_08078670(gUnk_081A70FC, (u8 *)&gUnk_0201F6D8 - 0x4A0);
    gUnk_03000040.unk4872 = 0;
    return 1;
}

void sub_08070F14(void)
{
}
/* sub_08070F18 is a near copy of the matched sub_0806DBB0 (code_0806D51C); the local
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
#define DECK_MENU_RAW (((struct DeckMenuRawFields *)&gUnk_0201DB20)->word)
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
#define DECK_ROWS ((struct DeckRowFields *)&gUnk_0201DB20)->rows
#define DECK_FRAME (*(struct DeckFrameFields *)&gUnk_0201DB20)
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
extern const u16 gUnk_080875D2[];
extern const u8 gUnk_081A6524[], gUnk_081A6EA4[];
void sub_08065F78(u16 position, u16 count, u16 *state);
void sub_08066260(s16 step, u8 tweenState, u8 direction, u8 *objects, u8 *rows);
void sub_080665D4(u8 *rows, struct DeckState *state);
void sub_0806699C(void);
void sub_0806704C(u8 *list, u8 *row, void *objects);
void sub_08067660(void *state);
void sub_08067908(void *state);
void sub_080679E0(u16 *position);
void sub_08067DA4(u16 *position);
void sub_08068180(u8 list);
void sub_0806B190(void);
void sub_08068B90(void);
void sub_08068C48(void);
void sub_08077AEC(int sound);
void sub_08077EF4(const void *script, int a, int b, int c, int d, int e,
                 int f, int g, int h, int i, int j, void *state);
void sub_08078534(void *objects, int a, int b, int c, int d, int e,
                 int f, int g, int h, void *state);
void sub_0807871C(void *objects);
void sub_0807883C(void *state);
void sub_08079834(int tile, u32 map, int col, int row, int width, int height, void *work);
void sub_0807A298(void *state);
void sub_0807A2EC(void *state);
void sub_0807B100(int from, int to, int increment, void *state);
void sub_0807B114(void *state);
void sub_0807B4A8(u32 level);
s32 sub_0807B4D0(s32 scale, u16 value);
void sub_0807B5A0(void *object);
void sub_08065108(int idx, u8 *map, int col, int row, u32 unused, int slot);
void sub_0806664C(int slot, int x, int kind, u8 *base, u8 *arr);
void sub_0806518C(int id, u8 *map, int col, int row, void *p);
void sub_08065384(int unused, u8 *map, int col, int row, void *p);
void sub_0807AFFC(int a, u32 b, int c);
void sub_08068E20(int a, int b, int c, int d);
void sub_080657F8(int pos);
void sub_08065AB4(u8 *map, int col, int row, void *p);
void sub_08065E6C(u8 *map, int col, int row, int perRow);
void sub_080787F4(u32 a, u32 b, u32 c, void *p);
/* The unit's shared prototype narrows the column; the ROM passes it as a word. */
extern u32 sub_08068D1C_word(u8 list, u8 row, int col) __asm__("sub_08068D1C");
/* sub_08070F14 is the empty function above; the ROM still passes it the list pointer. */
void sub_08070F14_arg(void *) __asm__("sub_08070F14");
#define PSTATE ((u8 *)&gUnk_0201DB20)
#define CURLIST gUnk_0201F73C
#define CNT(list) (gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[list]][list])
#define DECK_TWEEN_STATE DECK_FRAME.tweenState
#define DECK_TWEEN_STEP DECK_FRAME.tweenStep
#define DECK_SCROLL_CURVE gUnk_080875D2[DECK_TWEEN_STEP]
#define sub_08068D1C sub_08068D1C_word
#define sub_08070F14 sub_08070F14_arg

/* Deck Edit (variant) card-list frame: animate, handle list/menu input, draw, and fade. */
struct DeckPhaseFields {
    u8 pad0[0x1C3D];
    u8 phase : 3;               /* +0x1C3D bits 0-2 */
    u8 rest3D : 5;
};
#define DECK_PHASE (((struct DeckPhaseFields *)&gUnk_0201DB20)->phase)
/* FAKEMATCH: same callee under other return types, so the three menu transition calls are not cross-jumped. */
#define sub_080787F4_int ((int (*)(u32, u32, u32, void *))sub_080787F4)
#define sub_080787F4_u16 ((u16 (*)(u32, u32, u32, void *))sub_080787F4)
int sub_08070F18(void)
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

    keys = gUnk_03000040.keys & 0x3FF;
    sub_0807B114(&gUnk_0201E148);
    /* Crossing the midpoint of a horizontal page slide changes the visible rows. */
    if ((*((u8 *)&gUnk_0201E148 + 0x10E8) & 1) &&
        ((gUnk_0201E148.value == 4 && gUnk_0201E148.direction == 3) ||
         (gUnk_0201E148.value == 3 && gUnk_0201E148.direction == 4))) {
        DECK_FRAME.redraw = 0;
        clear = 0;
        CpuFastSet((const void *)&clear, (void *)0x0600D000, 0x01000200);
        if ((s16)gUnk_0201E140[CURLIST] - 2 >= 0) {
            sub_08065108(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201E140[CURLIST] - 2),
                         (u8 *)0x0600D000, 0, (u8)gUnk_0201DB20.h63A >> 3, (u32)(PSTATE + 0x640), 1);
        }
        if ((s16)gUnk_0201E140[CURLIST] - 1 >= 0) {
            sub_08065108(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201E140[CURLIST] - 1),
                         (u8 *)0x0600D000, 0, ((gUnk_0201DB20.h63A + 0x10) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 2);
        }
        if ((s16)gUnk_0201E140[CURLIST] + 1 < CNT(CURLIST)) {
            sub_08065108(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201E140[CURLIST] + 1),
                         (u8 *)0x0600D000, 0, ((gUnk_0201DB20.h63A + 0x48) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 4);
        }
        if ((s16)gUnk_0201E140[CURLIST] + 2 < CNT(CURLIST)) {
            sub_08065108(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201E140[CURLIST] + 2),
                         (u8 *)0x0600D000, 0, ((gUnk_0201DB20.h63A + 0x58) & 0xFF) >> 3, (u32)(PSTATE + 0x640), 5);
        }
        sub_08079834(0, 0x0600C000, 0, ((gUnk_0201DB20.h63E + 0x20) & 0xFF) >> 3, 30, 6, PSTATE + 0x640);
        if (CNT(CURLIST) != 0) {
            sub_0806518C(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201E140[CURLIST]),
                         (u8 *)0x0600C000, 0, ((gUnk_0201DB20.h63E + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
            sub_080657F8(0);
            sub_08065AB4((u8 *)0x0600C000, 11, ((gUnk_0201DB20.h63E + 0x38) & 0xFF) >> 3, PSTATE + 0x640);
            sub_08065E6C((u8 *)0x0600C000, 17, ((gUnk_0201DB20.h63E + 0x38) & 0xFF) >> 3, 6);
        } else {
            sub_08065384(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201E140[CURLIST]),
                         (u8 *)0x0600C000, 0, ((gUnk_0201DB20.h63E + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
        }
    }
    sub_08066260(DECK_TWEEN_STEP, DECK_TWEEN_STATE, gUnk_0201DB20.b635, PSTATE + 0x18B0, PSTATE + 0x1BB8);

    /* Directions 1/2 move vertically; 3/4 move a five-card page horizontally. */
    switch (gUnk_0201DB20.b635) {
    case 1:
    case 2:
        switch (DECK_TWEEN_STATE) {
        case 1:
            REG_BG3HOFS = gUnk_0201DB20.h630;
            REG_BG3VOFS = gUnk_0201DB20.h632 + (sub_0807B4D0(0x5000, DECK_SCROLL_CURVE) >> 8);
            REG_BG1HOFS = gUnk_0201DB20.h638;
            REG_BG1VOFS = gUnk_0201DB20.h63A + (sub_0807B4D0(0x1000, DECK_SCROLL_CURVE) >> 8);
            REG_BG0HOFS = gUnk_0201DB20.h63C;
            REG_BG0VOFS = gUnk_0201DB20.h63E + (sub_0807B4D0(0x2800, DECK_SCROLL_CURVE) >> 8);
            break;
        case 2:
            DECK_TWEEN_STATE = 0;
            gUnk_0201DB20.h632 += sub_0807B4D0(0x5000, DECK_SCROLL_CURVE) >> 8;
            gUnk_0201DB20.h63A += sub_0807B4D0(0x1000, DECK_SCROLL_CURVE) >> 8;
            gUnk_0201DB20.h63E += sub_0807B4D0(0x2800, DECK_SCROLL_CURVE) >> 8;
            if (DECK_FRAME.previousArrowState) {
                DECK_FRAME.previousArrowState = 1;
                DECK_FRAME.previousArrowDirty |= 1;
            }
            if (DECK_FRAME.nextArrowState) {
                DECK_FRAME.nextArrowState = 1;
                DECK_FRAME.nextArrowDirty |= 1;
            }
            gUnk_0201DB20.b635 = 0;
            /* fall through */
        default:
            REG_BG3HOFS = gUnk_0201DB20.h630;
            REG_BG3VOFS = gUnk_0201DB20.h632;
            REG_BG1HOFS = gUnk_0201DB20.h638;
            REG_BG1VOFS = gUnk_0201DB20.h63A;
            REG_BG0HOFS = gUnk_0201DB20.h63C;
            REG_BG0VOFS = gUnk_0201DB20.h63E;
            break;
        }
        break;
    case 3:
    case 4:
        switch (DECK_TWEEN_STATE) {
        case 1:
            REG_BG3HOFS = gUnk_0201DB20.h630 + (sub_0807B4D0(0x5000, DECK_SCROLL_CURVE) >> 8);
            REG_BG3VOFS = gUnk_0201DB20.h632;
            horizontalOffset = sub_0807B4D0(0x4000, DECK_SCROLL_CURVE) >> 8;
            blend = DECK_SCROLL_CURVE >> 3;
            REG_BLDCNT = 0x3F43;
            if (DECK_TWEEN_STEP <= 3) {
                REG_BG1HOFS = horizontalOffset;
                REG_BG1VOFS = gUnk_0201DB20.h63A;
                REG_BG0HOFS = horizontalOffset;
                REG_BG0VOFS = gUnk_0201DB20.h63E;
                sub_0807B4A8(blend >> 1);
            } else {
                REG_BG1HOFS = horizontalOffset + 0xFFC0;
                REG_BG1VOFS = gUnk_0201DB20.h63A;
                REG_BG0HOFS = horizontalOffset + 0xFFC0;
                REG_BG0VOFS = gUnk_0201DB20.h63E;
                sub_0807B4A8((0x20 - blend) >> 1);
            }
            break;
        case 2:
            DECK_TWEEN_STATE = 0;
            gUnk_0201DB20.h630 += sub_0807B4D0(0x5000, DECK_SCROLL_CURVE) >> 8;
            if (DECK_FRAME.previousArrowState) {
                DECK_FRAME.previousArrowState = 1;
                DECK_FRAME.previousArrowDirty |= 1;
            }
            if (DECK_FRAME.nextArrowState) {
                DECK_FRAME.nextArrowState = 1;
                DECK_FRAME.nextArrowDirty |= 1;
            }
            gUnk_0201DB20.b635 = 0;
            /* fall through */
        default:
            REG_BG3HOFS = gUnk_0201DB20.h630;
            REG_BG3VOFS = gUnk_0201DB20.h632;
            REG_BG1HOFS = gUnk_0201DB20.h638;
            REG_BG1VOFS = gUnk_0201DB20.h63A;
            REG_BG0HOFS = gUnk_0201DB20.h63C;
            REG_BG0VOFS = gUnk_0201DB20.h63E;
            REG_BLDCNT = 0x3FC8;
            REG_BLDALPHA = 0x1000;
            break;
        }
        break;
    }

    if (DECK_FRAME.transitionState == 0) {
        switch (gUnk_0201DB20.f1C48_0) {
        case 0:
            if (DECK_TWEEN_STATE != 1) {
                switch (keys) {
                case B_BUTTON:
                    sub_080787F4(0, 0x180, 0, PSTATE + 0x618);
                    gUnk_0201DB20.mode = 0;
                    sub_08077AEC(2);
                    break;
                case DPAD_UP:
                    sub_080679E0(&row);
                    break;
                case DPAD_DOWN:
                    sub_08067DA4(&row);
                    break;
                default:
                    if (CNT(gUnk_0201DB20.cursor) > 5) {
                        switch (keys) {
                        case DPAD_RIGHT:
                            if (gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] + 5 > CNT(gUnk_0201DB20.cursor) - 1)
                                gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] = 0;
                            else
                                gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] += 5;
                            gUnk_0201DB20.h18AC = 0xFC00;
                            sub_0807B100(0, 6, 1, PSTATE + 0x628);
                            gUnk_0201DB20.b634 ^= 1;
                            sub_0807AFFC(sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]),
                                         0x06008000 + gUnk_0201DB20.b634 * 0x1680, gUnk_0201DB20.b634);
                            sub_08068E20(((gUnk_0201DB20.h630 & 0xFF) >> 3) + 29, ((gUnk_0201DB20.h632 & 0xFF) >> 3) + 2, gUnk_0201DB20.b634, 1);
                            gUnk_0201DB20.b635 = 3;
                            DECK_FRAME.redraw = 1;
                            sub_08065F34(CNT(gUnk_0201DB20.cursor), gUnk_0201DB20.arr620[gUnk_0201DB20.cursor], &gUnk_0201DB20.f1BB0);
                            if (DECK_FRAME.nextArrowState) {
                                DECK_FRAME.nextArrowState = 2;
                                DECK_FRAME.nextArrowDirty |= 1;
                            }
                            row = gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < CNT(gUnk_0201DB20.cursor)) {
                                    sub_0806664C(slot, sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], row),
                                                 slot + 1, PSTATE + 0x1BB8, PSTATE + 0x18B0);
                                } else {
                                    DECK_ROWS[slot].active = 0;
                                }
                            }
                            DECK_FRAME.rowAnimationState = 0;
                            DECK_FRAME.rowCount = 5;
                            DECK_FRAME.rowAnimationTimer = 30;
                            sub_08077AEC(0);
                            break;
                        case DPAD_LEFT:
                            if (gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] <= 4)
                                gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] = CNT(gUnk_0201DB20.cursor) - 1;
                            else
                                gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] -= 5;
                            gUnk_0201DB20.h18AC = 0xFC00;
                            sub_0807B100(6, 0, -1, PSTATE + 0x628);
                            gUnk_0201DB20.b634 ^= 1;
                            sub_0807AFFC(sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]),
                                         0x06008000 + gUnk_0201DB20.b634 * 0x1680, gUnk_0201DB20.b634);
                            sub_08068E20(((gUnk_0201DB20.h630 & 0xFF) >> 3) + 9, ((gUnk_0201DB20.h632 & 0xFF) >> 3) + 2, gUnk_0201DB20.b634, 1);
                            gUnk_0201DB20.h630 -= 0x50;
                            gUnk_0201DB20.b635 = 4;
                            DECK_FRAME.redraw = 1;
                            sub_08065F34(CNT(gUnk_0201DB20.cursor), gUnk_0201DB20.arr620[gUnk_0201DB20.cursor], &gUnk_0201DB20.f1BB0);
                            if (DECK_FRAME.previousArrowState) {
                                DECK_FRAME.previousArrowState = 2;
                                DECK_FRAME.previousArrowDirty |= 1;
                            }
                            row = gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] - 2;
                            for (slot = 0; slot < 5; row++, slot++) {
                                if ((s16)row >= 0 && row < CNT(gUnk_0201DB20.cursor)) {
                                    sub_0806664C(slot, sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], row),
                                                 slot + 1, PSTATE + 0x1BB8, PSTATE + 0x18B0);
                                } else {
                                    DECK_ROWS[slot].active = 0;
                                }
                            }
                            DECK_FRAME.rowAnimationState = 0;
                            DECK_FRAME.rowCount = 5;
                            DECK_FRAME.rowAnimationTimer = 30;
                            sub_08077AEC(0);
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
            sub_08070F14(&gUnk_0201F73C);
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
                        pp = (u8 *)&gUnk_0201DB20;
                        asm("" : "=r"(busy));
                        pp += 0x1C3D;
                        asm("" : : "r"(busy));
                    }
                    ((struct DeckPhaseByte *)pp)->phase = 3;
                    sub_08077AEC(0);
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
                        pp = (u8 *)&gUnk_0201DB20;
                        asm("" : "=r"(busy));
                        pp += 0x1C3D;
                        asm("" : : "r"(busy));
                    }
                    ((struct DeckPhaseByte *)pp)->phase = 3;
                    sub_08077AEC(0);
                    break;
                case A_BUTTON:
                    switch (DECK_FRAME.menuSelected) {
                    case 0:
                        if (CNT(gUnk_0201DB20.cursor) == 0)
                            goto error_sound;
                        gUnk_0201DB20.mode = 2;
                        sub_080787F4_int(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case 4:
                        gUnk_0201DB20.mode = 1;
                        sub_080787F4_u16(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case 5:
                        gUnk_0201DB20.mode = 3;
                        sub_080787F4(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case 6:
                        if (sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]) != 0
                            && CNT(gUnk_0201DB20.cursor) != 0) {
                            gUnk_03000040.unk4872 = sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]);
                            sub_080787F4(0, 0x180, 0, PSTATE + 0x618);
                            gUnk_0201DB20.mode = 0;
confirm_sound:
                            sub_08077AEC(1);
                            goto draw_frame;
                        }
error_sound:
                        sub_08077AEC(3);
                        break;
                    }
                    break;
                case B_BUTTON:
                    ((struct DeckPhaseByte *)(PSTATE + 0x1C3D))->phase = 2;
                    sub_08077AEC(2);
                    break;
                default:
                    sub_08070F14(&gUnk_0201F73C);
                    if (*((u8 *)&gUnk_0201F73C - 0x15F4) != 1) {
                        switch (keys) {
                        case DPAD_UP: sub_080679E0(&row); break;
                        case DPAD_DOWN: sub_08067DA4(&row); break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }

draw_frame:
    sub_0806B190();
    sub_08077EF4(gUnk_081A6524, 5, 12, -1, -1, 3, 2, 0, 0, 0, 0, &gUnk_0201DB20);
    DECK_FRAME.frameDirty = 1;
    sub_08065F78(gUnk_0201DB20.arr620[gUnk_0201DB20.cursor], CNT(gUnk_0201DB20.cursor), &gUnk_0201DB20.f1BB0);
    sub_080665D4(PSTATE + 0x1BBC, &gUnk_0201DB20);
    for (slot = 1; slot <= 6; slot++)
        sub_0807B5A0(PSTATE + 0x18B0 + slot * 24);
    tail = gUnk_0201F740;
    sub_0806699C();
    list = tail - 4;
    sub_0806704C(list, tail - 3, objects = tail - 0x508);
    sub_08067908(tail + 0x1C);
    sub_08067660(tail + 0x1C);
    sub_08077EF4(gUnk_081A6EA4, 0, 1, -1, -1, 0, 0, 1, 1, 0, 0, state = (struct DeckState *)(tail - 0x1C20));
    sub_08068180(*list);
    sub_0807871C(objects);
    sub_08078534(objects, 0, 0, 0, 0, 0, 3, 0, 0, state);
    sub_08068B90();
    sub_08068C48();
    sub_0807A298(state);
    sub_0807A2EC(state);
    sub_0807883C(tail - 0x1608);
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
        if ((s16)gUnk_0201DB20.h18AC > 0x3FF)
            gUnk_0201DB20.h18AC = 0x400;
        else
            gUnk_0201DB20.h18AC += 0x30;
    }
done:
    return 0;
}

