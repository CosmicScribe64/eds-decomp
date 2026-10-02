#include "global.h"
#include "gba.h"

/* gMain (0x03000040) fields used here. */
struct Main {
    u8 pad0[0x40E];
    u16 vblankFlags;    /* +0x40E */
    u8 pad410[0x4878 - 0x410];
};
extern struct Main gUnk_03000040;

/* Deck edit card-list state at 0x0201DB20 (see code_08068180/code_0806A92C/code_0806ED44). */
struct DeckState {
    u8 pad0[0x620];
    u16 arr620[3];      /* +0x620 list scroll positions */
    u8 pad626[0x630 - 0x626];
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
    u8 pad14A3[0x18AC - 0x14A3];
    u16 h18AC;          /* +0x18AC */
    u8 pad18AE[0x1BB0 - 0x18AE];
    u16 f1BB0;          /* +0x1BB0 list slide state */
    u8 pad1BB2[0x1C1C - 0x1BB2];
    u8 cursor;          /* +0x1C1C */
    u8 pad1C1D[0x1C34 - 0x1C1D];
    u8 active : 1;      /* +0x1C34 bit 0 */
    u8 mode34 : 4;      /* bits 1-4 */
    u8 rest34 : 3;
    u8 pad1C35[0x1C3D - 0x1C35];
    u8 phase : 3;       /* +0x1C3D bits 0-2 */
    u8 rest3D : 5;
    u8 pad1C3E[0x1C48 - 0x1C3E];
    u8 f1C48_0 : 1;     /* +0x1C48 bit 0 */
    u8 mode48 : 4;      /* bits 1-4 */
    u8 rest48 : 3;
    u8 pad1C49[0x1C55 - 0x1C49];
    u8 b1C55;           /* +0x1C55 */
    u8 pad1C56[0x1C5C - 0x1C56];
};
extern struct DeckState gUnk_0201DB20;

void sub_08075278(void *dst, u32 size); /* MemClear16 */
void sub_08066244(void *p);
void sub_080666AC(void *p);
void sub_08065F34(u16 a, u16 b, u16 *out);
void sub_080679A8(u8 x);

/* Deck Edit step: reset the card list state and re-sync the list slide. */
int sub_0806D51C(void)
{
    gUnk_03000040.vblankFlags = 1;
    gUnk_0201DB20.h632 = 0;
    gUnk_0201DB20.h630 = 0;
    gUnk_0201DB20.h63A = 0;
    gUnk_0201DB20.h638 = 0;
    gUnk_0201DB20.h63E = 0;
    gUnk_0201DB20.h63C = 0;
    sub_08066244((u8 *)&gUnk_0201DB20 + 0x1BB8);
    sub_080666AC((u8 *)&gUnk_0201DB20 + 0x1C20);
    gUnk_0201DB20.h18AC = 0xFC00;
    gUnk_0201DB20.b1C55 = 0;
    gUnk_0201DB20.mode48 = 4;
    sub_08065F34(gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor]][gUnk_0201DB20.cursor],
                 gUnk_0201DB20.arr620[gUnk_0201DB20.cursor], &gUnk_0201DB20.f1BB0);
    gUnk_0201DB20.active = 0;
    gUnk_0201DB20.mode34 = 0;
    gUnk_0201DB20.mode48 = 4;
    gUnk_0201DB20.phase = 3;
    return 1;
}

/* Zero the 0x20-byte sprite entry at dst + idx * 0x20. */
void sub_0806D634(void *dst, int idx)
{
    sub_08075278((u8 *)dst + (idx << 5), 0x20);
}

extern const u8 gUnk_086E26D0[], gUnk_086E3030[], gUnk_086ED3B0[], gUnk_086E5030[], gUnk_086E7030[], gUnk_086E9030[], gUnk_086EB030[];
extern const u8 gUnk_086ED030[], gUnk_086ED0B0[], gUnk_086ED190[], gUnk_086ED1B0[], gUnk_08704EE8[];
extern const u8 gUnk_081A70FC[];
extern u8 gUnk_0201F73C;
extern u16 gUnk_0201E140[];
extern u8 gUnk_0201F3D0[];
void sub_0807A908(const void *src, void *dst, u32 w, u32 h);
void sub_08077CEC(const void *src, void *dst, u32 n);
void sub_08066164(u8 *dst);
void sub_080653F0(u8 *dst);
void sub_08078670(const void *a, void *b);
void sub_08065108(int idx, u8 *map, int col, int row, u32 unused, int slot);
void sub_0806664C(int slot, int x, int kind, u8 *base, u8 *arr);
void sub_0806518C(int id, u8 *map, int col, int row, void *p);
void sub_08065384(int unused, u8 *map, int col, int row, void *p);
extern u32 sub_08068D1C(u8 list, u8 row, int col);
void sub_0807AFFC(int a, u32 b, int c);
void sub_08068E20(int a, int b, int c, int d);
void sub_080657F8(int pos);
void sub_08065AB4(u8 *map, int col, int row, void *p);
void sub_08065E6C(u8 *map, int col, int row, int perRow);
void sub_080787F4(u32 a, u32 b, u32 c, void *p);
void sub_08079474(u8 *dst, u32 unused, u8 pal, int a, int b);
extern void CpuFastSet(const void *src, void *dst, u32 cnt);
extern void CpuSet(const void *src, void *dst, u32 cnt);
#define PSTATE ((u8 *)&gUnk_0201DB20)
#define OBJS ((u8 *)&gUnk_0201DB20 + 0x1718)
struct FB { u8 f : 8; };
#define OBJF(off) (((struct FB *)(OBJS + (off)))->f)
#define CURLIST gUnk_0201F73C
#define CNT(list) (gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[list]][list])

/* Deck Edit card-list view init: clears VRAM, loads graphics and palettes, draws the visible rows of the list. */
int sub_0806D644(void)
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
    sub_0807A908(gUnk_086E26D0, (void *)0x0600E000, 0x1E, 0x14);
    CpuFastSet(gUnk_086E3030, (void *)0x06000000, 0x800);
    CpuFastSet(gUnk_086ED3B0, (void *)0x06004000, 0x800);
    sub_08077CEC(gUnk_086E5030, (void *)0x06010000, 0x10);
    sub_08077CEC(gUnk_086E7030, (void *)0x06010200, 0x10);
    sub_08077CEC(gUnk_086E9030, (void *)0x06014000, 0x10);
    sub_08077CEC(gUnk_086EB030, (void *)0x06014200, 0x10);
    sub_0806D634((void *)0x06010000, 0x8A);
    sub_0806D634((void *)0x06010000, 0xAA);
    sub_0806D634((void *)0x06010000, 0xCA);
    sub_0806D634((void *)0x06010000, 0xEA);
    sub_08066164((u8 *)0x06006000);
    sub_08078670(gUnk_081A70FC, OBJS);
    OBJF(0xE) |= 0xFF;
    /* FAKEMATCH: staging this constant pointer preserves later address allocation. */
    rowObjects = gUnk_0201F3D0;
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
    CpuFastSet(gUnk_086ED030, (void *)0x05000080, 0x20);
    CpuFastSet(gUnk_086ED0B0, (void *)0x05000020, 0x18);
    CpuFastSet(gUnk_086ED190, (void *)0x05000060, 8);
    CpuSet(gUnk_086ED1B0, (void *)0x05000200, 0x100);
    *(u16 *)0x05000044 = 0x7758;
    sub_080653F0((u8 *)0x06002000);
    CpuSet(gUnk_08704EE8, (void *)0x05000000, 0x10);
    k = 1;
    for (j = 0; j < 2; j++) {
        if ((s16)gUnk_0201DB20.arr620[CURLIST] + (s16)k - 3 >= 0) {
            sub_08065108(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST] + (s16)k - 3),
                         (u8 *)0x0600D000, 0, ((s16)k - 1) * 2, (u32)(PSTATE + 0x640), -(s16)k + 2);
            sub_0806664C((s16)k - 1, sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST] + (s16)k - 3),
                         (s16)k, PSTATE + 0x1BB8, rowObjects);
        }
        k = (s16)k + 1;
    }
    for (j = 0; j < 2; j = next) {
        int pos = gUnk_0201E140[CURLIST] + j;
        int limit = CNT(CURLIST) - 1;
        next = j + 1;
        if (pos < limit) {
            sub_08065108(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201E140[CURLIST] + next),
                         (u8 *)0x0600D000, 0, j * 2 + 9, (u32)(PSTATE + 0x640), j + 4);
            sub_0806664C(j + 3, sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201E140[CURLIST] + j + 1),
                         j + 4, PSTATE + 0x1BB8, rowObjects);
        }
    }
    if (CNT(CURLIST) != 0) {
        sub_0806518C(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST]),
                     (u8 *)0x0600C000, 0, (v = (gUnk_0201DB20.h63E + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
        sub_0806664C(2, sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST]),
                     3, PSTATE + 0x1BB8, PSTATE + 0x18B0);
        sub_0807AFFC(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST]),
                     0x06008000 + gUnk_0201DB20.b634 * 0x1680, gUnk_0201DB20.b634);
        sub_08068E20(0x13, ((gUnk_0201DB20.h632 & 0xFF) >> 3) + 2, gUnk_0201DB20.b634, 1);
        sub_080657F8(0);
        sub_08065AB4((u8 *)0x0600C000, 0xB, 7, PSTATE + 0x640);
        sub_08065E6C((u8 *)0x0600C000, 0x11, 7, 6);
    } else {
        sub_08065384(sub_08068D1C(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST]),
                     (u8 *)0x0600C000, 0, (v = (gUnk_0201DB20.h63E + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
    }
    sub_080679A8(gUnk_0201DB20.cursor);
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
    sub_080787F4(0, -0x180, 0, PSTATE + 0x618);
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = 0;
    REG_DISPCNT = 0x7F00;
    sub_08079474((u8 *)0x06006000, 0x300, 1, 0, 0);
    return 1;
}

/* Deck Edit step: draw the card list view, then run the browse loop. */
int sub_0806DBA4(void)
{
    sub_0806D644();
    return 1;
}

/* helpers for the sub_0806DBB0 draft */
/* Additional local declarations for the card-list frame handler. */
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
    u8 pad17DB[0x1BB4 - 0x17DB];
    u8 previousArrowDirty;      /* +0x1BB4 */
    u8 previousArrowState;      /* +0x1BB5 */
    u8 nextArrowDirty;          /* +0x1BB6 */
    u8 nextArrowState;          /* +0x1BB7 */
    u8 rowCount;                /* +0x1BB8 */
    u8 pad1BB9[0x1C14 - 0x1BB9];
    u8 rowAnimationState;       /* +0x1C14 */
    u8 pad1C15[0x1C20 - 0x1C15];
    u8 menuAnimationState;      /* +0x1C20 */
    u8 pad1C21[0x1C3B - 0x1C21];
    u8 selectedCard;            /* +0x1C3B */
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
struct MainKeyFields {
    u32 random;
    u16 heldKeys;
    u16 newKeys;
};
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
struct DeckMenuWord {
    u32 otherLow : 15;
    u32 selected : 3;          /* DeckState +0x1C3C bits 15-17 */
    u32 otherHigh : 14;
};
extern struct DeckScrollTween gUnk_0201E148;
extern u8 gUnk_0201F740[];
extern const u16 gUnk_080875D2[];
extern const u16 gUnk_08622AB4[];
extern const u32 gUnk_08621DE0[];
extern const u8 gUnk_081A6524[], gUnk_081A6EA4[];
void sub_08065F78(u16 position, u16 count, u16 *state);
void sub_08066260(s16 step, u8 tweenState, u8 direction, u8 *objects, u8 *rows);
void sub_080665D4(u8 *rows, struct DeckState *state);
void sub_080666B4(u8 selected, int list, u8 *state);
void sub_0806699C(void);
void sub_0806704C(u8 *list, u8 *row, void *objects);
void sub_08067474(u8 *list);
void sub_08067660(void *state);
void sub_08067908(void *state);
void sub_080679E0(u16 *position);
void sub_08067DA4(u16 *position);
void sub_08068180(u8 list);
u32 sub_08068434(void);
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

/* Unnamed fields retain their byte offsets until their purpose is established. */
#define DECK_BYTE(off) (PSTATE[(off)])
#define DECK_HALF(off) (*(u16 *)(PSTATE + (off)))
#define DECK_SIGNED_HALF(off) (*(s16 *)(PSTATE + (off)))
#define DECK_MENU (*(struct DeckMenuWord *)(PSTATE + 0x1C3C))
#define DECK_TWEEN_STATE DECK_FRAME.tweenState
#define DECK_TWEEN_STEP DECK_FRAME.tweenStep
#define DECK_SCROLL_CURVE gUnk_080875D2[DECK_TWEEN_STEP]

/* Deck Edit card-list frame: animate, handle list/menu input, draw, and fade. */
int sub_0806DBB0(void)
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
    struct DeckState *state;

    keys = ((struct MainKeyFields *)&gUnk_03000040)->newKeys & 0x3FF;
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
                    gUnk_0201DB20.mode48 = 0;
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
                gUnk_0201DB20.phase = 1;
                goto confirm_sound;
            }
            sub_08067474(&gUnk_0201F73C);
            break;
        case 1:
            if (DECK_FRAME.menuAnimationState == 0) {
                switch (keys) {
                case DPAD_RIGHT:
                    if (++DECK_FRAME.menuSelected == gUnk_0201DB20.cursor + 1)
                        DECK_FRAME.menuSelected++;
                    if (DECK_FRAME.menuSelected == 7)
                        DECK_FRAME.menuSelected = 0;
                    DECK_FRAME.menuSelected = DECK_FRAME.menuSelected;
                    gUnk_0201DB20.phase = 3;
                    sub_08077AEC(0);
                    break;
                case DPAD_LEFT:
                    if (DECK_FRAME.menuSelected == 0) {
                        DECK_FRAME.menuSelected = 6;
                    } else {
                        if (--DECK_FRAME.menuSelected == gUnk_0201DB20.cursor + 1)
                            DECK_FRAME.menuSelected--;
                    }
                    {
                        /* FAKEMATCH: same registers as the DPAD_RIGHT tail so the two tails merge */
                        register u8 *b asm("r2") = (u8 *)&gUnk_0201DB20;
                        register u32 busy asm("r3");
                        struct DeckPhaseByte *p;
                        asm("" : "=r"(busy));
                        p = (struct DeckPhaseByte *)(b + 0x1C3D);
                        asm("" : : "r"(busy));
                        p->phase = 3;
                    }
                    sub_08077AEC(0);
                    break;
                case A_BUTTON:
                    switch (DECK_FRAME.menuSelected) {
                    case 0:
                        if (CNT(gUnk_0201DB20.cursor) != 0) {
                            gUnk_0201DB20.mode48 = 2;
                            sub_080787F4(0, 0x180, 0, PSTATE + 0x618);
                            /* FAKEMATCH: keep this transition call separate from the other menu cases. */
                            asm("");
                            goto confirm_sound;
                        }
                        break;
                    case 1:
                        if ((u16)sub_08068434() == 0)
                            goto error_sound;
                        sub_080666B4(DECK_FRAME.selectedCard, 0, PSTATE + 0x1C20);
                        break;
                    case 2:
                        if ((u16)sub_08068434() == 0)
                            goto error_sound;
                        sub_080666B4(DECK_FRAME.selectedCard, 1, PSTATE + 0x1C20);
                        break;
                    case 3:
                        if ((u16)sub_08068434() == 0)
                            goto error_sound;
                        card = sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]);
                        number = ((const u16 *)0x08622AB4)[card & 0x7FF];
                        switch (number) {
                        case 0x776:
                            kind = 3;
                            break;
                        case 0x777:
                        case 0x778:
                            kind = 1;
                            break;
                        default:
                            switch ((int)((((const u32 *)0x08621DE0)[card & 0x7FF] & 0x01F00000) >> 20)) {
                            case 22: kind = 7; break;
                            case 21: kind = 8; break;
                            case 23: kind = 9; break;
                            default: kind = (((const u32 *)0x08621DE0)[card & 0x7FF] & 0xC0000) >> 18; break;
                            }
                        }
                        if (kind != 2) {
                            sub_080666B4(DECK_FRAME.selectedCard, 2, PSTATE + 0x1C20);
                        } else {
error_sound:
                            sub_08077AEC(3);
                        }
                        break;
                    case 4:
                        gUnk_0201DB20.mode48 = 1;
                        sub_080787F4(0, 0x180, 0, PSTATE + 0x618);
                        /* FAKEMATCH: keep this transition call separate from the other menu cases. */
                        asm("");
                        goto confirm_sound;
                    case 5:
                        gUnk_0201DB20.mode48 = 3;
                        sub_080787F4(0, 0x180, 0, PSTATE + 0x618);
                        goto confirm_sound;
                    case 6:
                        gUnk_0201DB20.phase = 2;
                        sub_080787F4(0, 0x180, 0, PSTATE + 0x618);
                        gUnk_0201DB20.mode48 = 0;
confirm_sound:
                        sub_08077AEC(1);
                        goto draw_frame;
                    }
                    break;
                case B_BUTTON:
                    gUnk_0201DB20.phase = 2;
                    sub_08077AEC(2);
                    break;
                default:
                    sub_08067474(&gUnk_0201F73C);
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

