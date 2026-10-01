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
extern struct Main gUnk_03000040;

/* Deck edit scene state at 0x0201DB20 (see code_08068180). */
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
extern struct DeckState gUnk_0201DB20;
extern u16 gUnk_02013D90;
extern u8 gUnk_0201E138[];
extern void sub_0807883C(void *p);
extern u16 sub_08068D1C(u8 list, u8 row, u16 col);
extern void sub_0800688C(u16 id, int a, int b);
extern u16 (*const gUnk_081A725C[])(void);

int sub_0806ED44(void)
{
    sub_0807883C(gUnk_0201E138);
    if (gUnk_0201E138[6] == 3)
        return 1;
    return 0;
}
int sub_0806ED64(void)
{
    gUnk_03000040.sub1 = 0;
    switch (gUnk_0201DB20.mode) {
    case 0:
        return 1;
    case 1:
        gUnk_03000040.step = 5;
        return 0;
    case 2:
        gUnk_03000040.step = 0xD;
        gUnk_02013D90 &= 1;
        sub_0800688C(sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]), 0, 0);
        return 0;
    case 3:
        gUnk_03000040.step = 9;
        return 0;
    case 4:
        gUnk_03000040.step = 1;
        return 0;
    default:
        return 1;
    }
}
int sub_0806EE34(void)
{
    gUnk_03000040.sub1 = 0;
    switch (gUnk_0201DB20.mode) {
    case 0:
        return 1;
    case 1:
        gUnk_03000040.sub2 = 5;
        return 0;
    case 2:
        gUnk_03000040.sub2 = 0xD;
        gUnk_02013D90 &= 1;
        sub_0800688C(sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]), 0, 0);
        return 0;
    case 3:
        gUnk_03000040.sub2 = 9;
        return 0;
    case 4:
        gUnk_03000040.sub2 = 1;
        return 0;
    default:
        return 1;
    }
}
/* Deck Edit scene callback: runs step gUnk_081A725C[gMain.step]; advances when it returns non-zero, returns 1 at the end of the table. */
u16 sub_0806EF04(void)
{
    if (gUnk_03000040.step == 0)
        gUnk_03000040.mode4874 = 0;
    gUnk_0201DB20.flag = 0;
    if (gUnk_081A725C[gUnk_03000040.step] != 0) {
        if (gUnk_081A725C[gUnk_03000040.step]())
            gUnk_03000040.step++;
        return 0;
    }
    return 1;
}
extern u16 (*const gUnk_081A72A0[])(void);
extern u16 (*const gUnk_081A72E4[])(void);
extern u16 (*const gUnk_081A7330[])(void);
extern const u8 gUnk_080875EC[];
extern void sub_0801A7DC(const void *a, int b, int c);
extern void sub_0801A7E8(void);

u16 sub_0806EF74(void)
{
    struct Main *m = &gUnk_03000040;
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
        gUnk_0201DB20.flag = 1;
        break;
    }
    fn = gUnk_081A72A0[*(step = &runner->step)];
    if (fn != 0) {
        old = gUnk_0201DB20.sel;
        if (fn())
            (*step)++;
        if (old != gUnk_0201DB20.sel) {
            sub_0801A7DC(gUnk_080875EC, old, gUnk_0201DB20.sel);
            sub_0801A7E8();
        }
        return 0;
    }
    return 1;
} /* 0x0806EF74 size 0xA8 */
/* Sub-step runner on gMain+0x485B (table 0x081A72E4). */
u16 sub_0806F01C(void)
{
    if (gUnk_081A72E4[gUnk_03000040.sub2] != 0) {
        if (gUnk_081A72E4[gUnk_03000040.sub2]())
            gUnk_03000040.sub2++;
        return 0;
    }
    return 1;
}
extern u8 gUnk_02017A40_b[] asm("gUnk_02017A40");
#define STEP2 (gUnk_02017A40_b[0x3E6])

/* Step runner on the byte 0x02017A40+0x3E6 (table 0x081A7330). */
u16 sub_0806F05C(void)
{
    if (STEP2 == 0)
        gUnk_03000040.mode4874 = 2;
    if (gUnk_081A7330[STEP2] != 0) {
        if (gUnk_081A7330[STEP2]())
            STEP2++;
        return 0;
    }
    return 1;
}
u16 sub_0806F0BC(void)
{
    struct Main *m = &gUnk_03000040;
    u8 *sub = &m->sub2;
    u8 v = *sub;
    if (v == 0) {
        STEP2 = v;
        (*sub)++;
        return 0;
    }
    return sub_0806F05C();
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
extern const u8 gUnk_081A70FC[];

/* Deck Edit scene init: clears the state block, resets the BG scroll registers, builds the card lists. */
int sub_0806F0F8(void)
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
    for (i = 1; CARD_NUMBER(i) != 0xFFFF; ) {
        if ((u16)(CARD_NUMBER(i) - 0x76C) > 0x63) {
            sub_08068DA4(i, 0, gUnk_0201DB20.arr14A0[0], gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[0]][0]++);
        }
        i++;
        if (i > 0x334)
            break;
    }
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

void sub_0806F400(void)
{
}
extern const u8 gUnk_087025D8[], gUnk_08702C08[], gUnk_086ED3B0[], gUnk_086E5030[], gUnk_086E7030[], gUnk_086E9030[], gUnk_086EB030[];
extern const u8 gUnk_08702A88[], gUnk_08702B08[], gUnk_08702BE8[], gUnk_086ED1B0[], gUnk_08704EE8[];
extern u8 gUnk_0201F73C;
extern u16 gUnk_0201E140[];
extern u8 gUnk_0201F3D0[];
void sub_0807A908(const void *src, void *dst, u32 w, u32 h);
void sub_08077CEC(const void *src, void *dst, u32 n);
void sub_08066164(u8 *dst);
void sub_080653F0(u8 *dst);
void sub_08065108(u16 idx, u8 *map, u16 col, u16 row, u32 unused, u8 slot);
void sub_0806664C(u8 slot, int x, u8 kind, u8 *base, u8 *arr);
void sub_0806518C(u16 id, u8 *map, u16 col, u16 row, void *p);
void sub_08065384(u32 unused, u8 *map, u16 col, u16 row, void *p);
extern u16 sub_08068D1C(u8 list, u8 row, u16 col);
void sub_0807AFFC(u16 a, u32 b, u16 c);
void sub_08068E20(u8 a, u8 b, u8 c, u8 d);
void sub_080657F8(u16 pos);
void sub_08065AB4(u8 *map, u16 col, u16 row, void *p);
void sub_08065E6C(u8 *map, u16 col, u16 row, u8 perRow);
void sub_080787F4(u32 a, u32 b, u32 c, void *p);
void sub_08079474(u8 *dst, u32 unused, u8 pal, int a, int b);
extern void CpuFastSet(const void *src, void *dst, u32 cnt);
extern void CpuSet(const void *src, void *dst, u32 cnt);
#define PSTATE ((u8 *)&gUnk_0201DB20)
#define OBJS ((u8 *)&gUnk_0201DB20 + 0x1718)
struct FB { u8 f : 8; };
#define OBJF(off) (((struct FB *)(OBJS + (off)))->f)

/* Deck Edit card-list view init: clears VRAM, loads graphics and palettes, draws the visible rows of the list. */
int sub_0806F404(void)
{
    /* FAKEMATCH: promoted caller views preserve the ROM register arguments;
     * the existing callees decode their narrow values on entry. */
    extern u32 f404_lookup(u8, u8, int) asm("sub_08068D1C");
    extern void f404_row(int, u8 *, int, int, u32, int) asm("sub_08065108");
    extern void f404_obj(int, int, int, u8 *, u8 *) asm("sub_0806664C");
    extern void f404_detail(int, u8 *, int, int, void *) asm("sub_0806518C");
    extern void f404_clear(int, u8 *, int, int, void *) asm("sub_08065384");
    /* FAKEMATCH: the staged array address preserves the second loop's register allocation. */
    u8 (*objectRows)[];
    extern void f404_card(int, u32, int) asm("sub_0807AFFC");
    extern void f404_misc(int, int, int, int) asm("sub_08068E20");
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
    sub_0807A908(gUnk_087025D8, (void *)0x0600E000, 0x1E, 0x14);
    CpuFastSet(gUnk_08702C08, (void *)0x06000000, 0x800);
    CpuFastSet(gUnk_086ED3B0, (void *)0x06004000, 0x800);
    sub_08077CEC(gUnk_086E5030, (void *)0x06010000, 0x10);
    sub_08077CEC(gUnk_086E7030, (void *)0x06010200, 0x10);
    sub_08077CEC(gUnk_086E9030, (void *)0x06014000, 0x10);
    sub_08077CEC(gUnk_086EB030, (void *)0x06014200, 0x10);
    sub_08066164((u8 *)0x06006000);
    sub_08078670(gUnk_081A70FC, OBJS);
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
    CpuFastSet(gUnk_08702A88, (void *)0x05000080, 0x20);
    CpuFastSet(gUnk_08702B08, (void *)0x05000020, 0x18);
    CpuFastSet(gUnk_08702BE8, (void *)0x05000060, 8);
    CpuSet(gUnk_086ED1B0, (void *)0x05000200, 0x100);
    *(u16 *)0x05000044 = 0x7758;
    sub_080653F0((u8 *)0x06002000);
    CpuSet(gUnk_08704EE8, (void *)0x05000000, 0x10);
#define CURLIST gUnk_0201F73C
#define CNT(list) (gUnk_0201DB20.cnt1494[gUnk_0201DB20.arr14A0[list]][list])
    k = 1;
    for (j = 0; j < 2; j++) {
        if ((s16)gUnk_0201DB20.arr620[CURLIST] + (s16)k - 3 >= 0) {
            f404_row(f404_lookup(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST] + (s16)k - 3),
                         (u8 *)0x0600D000, 0, ((s16)k - 1) * 2, (u32)(PSTATE + 0x640), -(s16)k + 2);
            f404_obj((s16)k - 1, f404_lookup(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST] + (s16)k - 3),
                         (s16)k, PSTATE + 0x1BB8, *objectRows);
            k = (s16)k + 1;
        }
    }
    for (j = 0; j < 2; j = next) {
        int row = gUnk_0201E140[CURLIST] + j;
        int count = CNT(CURLIST) - 1;
        next = j + 1;
        if (row < count) {
            f404_row(f404_lookup(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201E140[CURLIST] + next),
                         (u8 *)0x0600D000, 0, j * 2 + 9, (u32)(PSTATE + 0x640), j + 4);
            f404_obj(j + 3, f404_lookup(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201E140[CURLIST] + j + 1),
                         j + 4, PSTATE + 0x1BB8, *objectRows);
        }
    }
    if (CNT(CURLIST) != 0) {
        f404_detail(f404_lookup(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST]),
                     (u8 *)0x0600C000, 0, ((gUnk_0201DB20.h63E + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
        f404_obj(2, f404_lookup(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST]),
                     3, PSTATE + 0x1BB8, PSTATE + 0x18B0);
        f404_card(f404_lookup(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST]),
                     0x06008000 + gUnk_0201DB20.b634 * 0x1680, gUnk_0201DB20.b634);
        f404_misc(0x13, ((gUnk_0201DB20.h632 & 0xFF) >> 3) + 2, gUnk_0201DB20.b634, 1);
        sub_080657F8(0);
        sub_08065AB4((u8 *)0x0600C000, 0xB, 7, PSTATE + 0x640);
        sub_08065E6C((u8 *)0x0600C000, 0x11, 7, 6);
    } else {
        f404_clear(f404_lookup(CURLIST, gUnk_0201DB20.arr14A0[CURLIST], gUnk_0201DB20.arr620[CURLIST]),
                     (u8 *)0x0600C000, 0, ((gUnk_0201DB20.h63E + 0x20) & 0xFF) >> 3, PSTATE + 0x640);
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
    sub_080787F4(0, -0x180, 0, gUnk_0201E138);
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
#if 0 /* NONMATCHING: twin-based input handler; 344 normalized diff lines
       * remain. The remaining differences are allocation, shared tails and
       * literal pools. F934-specific selection, menu actions and rendering
       * calls are preserved. Word caller views and marked register hints
       * reproduce the ROM ABI. */

#define DIN8(o) (*(u8 *)(PSTATE + (o)))
#define DIN16(o) (*(u16 *)(PSTATE + (o)))
#define DIN32(o) (*(u32 *)(PSTATE + (o)))
extern u8 gUnk_0201E148[], gUnk_0201F740[];
extern const u16 gUnk_080875D2[];
extern const u8 gUnk_081A6524[], gUnk_081A6EA4[];
void sub_08065F78(u16, u16, u16 *);
void sub_08066260(s16, u8, u8, u8 *, u8 *);
void sub_080665D4(void *, void *);
void sub_0806699C(void *);
void sub_08067660(void *), sub_08067908(void *);
void sub_080679E0(u16 *), sub_08067DA4(u16 *);
void sub_08068B90(void), sub_08068C48(void);
void sub_08070F14_arg(void *) __asm__("sub_08070F14");
void sub_08077AEC(int);
void sub_08077EF4(const void *, int, int, int, int, int, int, int, int, int, int, void *);
void sub_08078534(void *, int, int, int, int, int, int, int, int, void *);
void sub_0807871C(void *);
void sub_08079834(int, int, int, int, int, int, void *);
void sub_0807A298(void *);
void sub_0807B100(int, int, int, void *), sub_0807B114(void *);
void sub_0807B4A8(u32);
s32 sub_0807B4D0(s32, u16);
void sub_0807B5A0(void *);

extern u8 gUnk_0201F73C;
extern u16 gUnk_0201E140[];
void sub_08065108(u16, u8 *, u16, u16, u32, u8);
void sub_0806518C(u16, u8 *, u16, u16, void *);
void sub_08065384(u32, u8 *, u16, u16, void *);
void sub_080657F8(u16);
void sub_08065AB4(u8 *, u16, u16, void *);
void sub_08065E6C(u8 *, u16, u16, u8);
void sub_0806664C(u8, int, u8, u8 *, u8 *);
void sub_0806704C(u8 *, u8 *, void *);
void sub_08068180(u8), sub_0806B190(void);
void sub_08068E20(u8, u8, u8, u8);
void sub_080787F4(u32, u32, u32, void *), sub_0807883C(void *);
void sub_0807AFFC(u16, u32, u16);
void CpuFastSet(const void *, void *, u32);
#define PSTATE ((u8 *)&gUnk_0201DB20)
/* FAKEMATCH: word caller views preserve callee narrowing while matching the ROM calls. */
extern s32 sub_08068D1C_word(s32, s32, s32) __asm__("sub_08068D1C");
extern void sub_08065108_word(s32, u8 *, s32, s32, u32, s32) __asm__("sub_08065108");
extern void sub_0806518C_word(s32, u8 *, s32, s32, void *) __asm__("sub_0806518C");
extern void sub_08065AB4_word(u8 *, s32, s32, void *) __asm__("sub_08065AB4");
extern void sub_08065E6C_word(u8 *, s32, s32, s32) __asm__("sub_08065E6C");
#define sub_08068D1C sub_08068D1C_word
#define sub_08065108 sub_08065108_word
#define sub_0806518C sub_0806518C_word
#define sub_08065AB4 sub_08065AB4_word
#define sub_08065E6C sub_08065E6C_word
extern void sub_08065384_word(s32, u8 *, s32, s32, void *) __asm__("sub_08065384");
#define sub_08065384 sub_08065384_word
extern void sub_08068E20_word(u32, u32, u32, u32) __asm__("sub_08068E20");
extern void sub_0806664C_word(u32, int, u32, u8 *, u8 *) __asm__("sub_0806664C");
extern void sub_0807AFFC_word(s32, u32, u32) __asm__("sub_0807AFFC");
#define sub_08068E20 sub_08068E20_word
#define sub_0806664C sub_0806664C_word
#define sub_0807AFFC sub_0807AFFC_word
s32 sub_0806F934(void) {
    /* FAKEMATCH: preserve the ROM register for the signed next-row comparisons. */
    register s32 next1 asm("r2");
    /* FAKEMATCH: second redraw guard uses the same signed-comparison register. */
    register s32 next2 asm("r2");
    struct { s32 fill; u16 scroll; } frame;
    u32 sp28;
    struct B7C *inputMode;
    s16 *temp_r2;
    s16 *temp_r2_2;
    s16 *temp_r5;
    s16 *temp_r5_2;
    volatile s16 *var_r1;
    s32 temp_r0_3;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r2_10;
    s32 temp_r2_11;
    s32 temp_r2_3;
    u32 temp_r4;
    s32 var_r0;
    s32 var_r0_3;
    u8 *temp_r4_4;
    u16 *temp_r1_5;
    u16 *temp_r1_6;
    u16 *temp_r3_4;
    /* FAKEMATCH: signed cursor temporary preserves allocation; both paths store the same low halfword. */
    s16 temp_r0_6;
    u32 temp_r2_5;
    s16 temp_r3_6;
    u16 temp_r4_2;
    s16 temp_r6;
    u32 var_r0_2;
    u16 var_r0_4;
    u8 temp_r0;
    u32 alphaShift;
    u32 temp_r0_2;
    u32 temp_r0_4;
    s32 temp_r0_5;
    u32 temp_r1_4;
    u8 *temp_r6_2;
    u8 temp_r1_3;
    u8 temp_r1_7;
    u8 temp_r2_4;
    u8 temp_r3;
    u8 temp_r3_2;
    u8 temp_r3_3;
    u8 temp_r3_5;
    u8 temp_r3_7;
    u8 var_r5;
    u8 var_r5_2;
    u8 var_r5_3;
    void *temp_r2_6;
    void *temp_r2_7;
    void *temp_r2_8;
    void *temp_r2_9;
    void *temp_r4_3;
    void *temp_r5_3;

    sp28 = gUnk_03000040.keys & 0x3FF;
    sub_0807B114(&gUnk_0201E148);
    if (1 & gUnk_0201E148[0x10E8]) {
        int direction = *(s16 *)(gUnk_0201E148 + 2);
        if ((direction == 4 && gUnk_0201E148[0xD] == 3) ||
            (direction == 3 && gUnk_0201E148[0xD] == 4)) {
                gUnk_0201DB20.f1710_0 = 0;
                frame.fill = 0;
                CpuFastSet(&frame.fill, (void *)0x0600D000, 0x01000200U);
                temp_r2 = (s16 *)((gUnk_0201F73C * 2) + (u32)gUnk_0201E140);
                if ((s32) (*temp_r2 - 2) >= 0) {
                    sub_08065108(sub_08068D1C(gUnk_0201F73C, gUnk_0201DB20.arr14A0[gUnk_0201F73C], (u16) *temp_r2 - 2), (u8 *)0x0600D000, 0, (u8) gUnk_0201DB20.h63A >> 3, (u32)gUnk_0201DB20.pad640, 1);
                }
                temp_r2_2 = (s16 *)((gUnk_0201F73C * 2) + (u32)gUnk_0201E140);
                if ((s32) (*temp_r2_2 - 1) >= 0) {
                    sub_08065108(sub_08068D1C(gUnk_0201F73C, gUnk_0201DB20.arr14A0[gUnk_0201F73C], (u16) *temp_r2_2 - 1), (u8 *)0x0600D000, 0, (0xFF & (gUnk_0201DB20.h63A + 0x10)) >> 3, (u32)gUnk_0201DB20.pad640, 2);
                }
                temp_r1 = gUnk_0201F73C * 2;
                temp_r5 = (s16 *)(temp_r1 + (u32)gUnk_0201E140);
                next1 = *temp_r5 + 1;
                temp_r3 = gUnk_0201DB20.arr14A0[gUnk_0201F73C];
                temp_r1 += temp_r3 * 6;
                temp_r1 += (u32)gUnk_0201DB20.cnt1494[0];
                
                if (next1 < (s32) *(u16 *)temp_r1) {
                    sub_08065108(sub_08068D1C(gUnk_0201F73C, temp_r3, (u16) *temp_r5 + 1), (u8 *)0x0600D000, 0, (0xFF & (gUnk_0201DB20.h63A + 0x48)) >> 3, (u32)gUnk_0201DB20.pad640, 4);
                }
                temp_r1_2 = gUnk_0201F73C * 2;
                temp_r5_2 = (s16 *)(temp_r1_2 + (u32)gUnk_0201E140);
                next2 = *temp_r5_2 + 2;
                temp_r3_2 = gUnk_0201DB20.arr14A0[gUnk_0201F73C];
                temp_r1_2 += temp_r3_2 * 6;
                temp_r1_2 += (u32)gUnk_0201DB20.cnt1494[0];
                
                if (next2 < (s32) *(u16 *)temp_r1_2) {
                    sub_08065108(sub_08068D1C(gUnk_0201F73C, temp_r3_2, (u16) *temp_r5_2 + 2), (u8 *)0x0600D000, 0, (0xFF & (gUnk_0201DB20.h63A + 0x58)) >> 3, (u32)gUnk_0201DB20.pad640, 5);
                }
                sub_08079834(0, 0x0600C000, 0, (0xFF & (gUnk_0201DB20.h63E + 0x20)) >> 3, 0x1E, 6, gUnk_0201DB20.pad640);
                temp_r2_3 = gUnk_0201F73C * 2;
                temp_r1_3 = gUnk_0201DB20.arr14A0[gUnk_0201F73C];
                /* FAKEMATCH: stage the count offset to preserve the ROM ADD operand order. */
                temp_r0_3 = temp_r2_3 + temp_r1_3 * 6;
                if (*(u16 *)(temp_r0_3 + (u32)gUnk_0201DB20.cnt1494[0]) != 0) {
                    sub_0806518C(sub_08068D1C(gUnk_0201F73C, temp_r1_3, *(u16 *)(temp_r2_3 + (u32)gUnk_0201E140)), (u8 *)0x0600C000, 0, (0xFF & (gUnk_0201DB20.h63E + 0x20)) >> 3, gUnk_0201DB20.pad640);
                    sub_080657F8(0);
                    sub_08065AB4((u8 *)0x0600C000, 0xB, (0xFF & (gUnk_0201DB20.h63E + 0x38)) >> 3, gUnk_0201DB20.pad640);
                    sub_08065E6C((u8 *)0x0600C000, 0x11, (0xFF & (gUnk_0201DB20.h63E + 0x38)) >> 3, 6);
                } else {
                    sub_08065384(sub_08068D1C(gUnk_0201F73C, temp_r1_3, *(u16 *)(temp_r2_3 + (u32)gUnk_0201E140)), (u8 *)0x0600C000, 0, (0xFF & (gUnk_0201DB20.h63E + 0x20)) >> 3, gUnk_0201DB20.pad640);
                }
        }
    }
    sub_08066260((s16)gUnk_0201DB20.h62A, gUnk_0201DB20.b628, gUnk_0201DB20.b635, &gUnk_0201DB20.pad18AE[2], gUnk_0201DB20.pad1BB8);
    switch (gUnk_0201DB20.b635) {
    case 1:
    case 2:
        switch (gUnk_0201DB20.b628) {          /* switch 3; irregular */
        case 1:                                     /* switch 3 */
            (*(vu16 *)(0x0400001C + 0)) = (u16) gUnk_0201DB20.h630;
            *(vu16 *)0x0400001E = (u16) (gUnk_0201DB20.h632 + (sub_0807B4D0(0x5000, gUnk_080875D2[(s16)gUnk_0201DB20.h62A]) >> 8));
            *(vu16 *)0x04000014 = (u16) gUnk_0201DB20.h638;
            *(vu16 *)0x04000016 = (u16) (gUnk_0201DB20.h63A + (sub_0807B4D0(0x1000, gUnk_080875D2[(s16)gUnk_0201DB20.h62A]) >> 8));
            *(vu16 *)0x04000010 = (u16) gUnk_0201DB20.h63C;
            *(vu16 *)0x04000012 = (u16) (gUnk_0201DB20.h63E + (sub_0807B4D0(0x2800, gUnk_080875D2[(s16)gUnk_0201DB20.h62A]) >> 8));
            break;
        case 2:                                     /* switch 3 */
            gUnk_0201DB20.b628 = 0;
            gUnk_0201DB20.h632 += sub_0807B4D0(0x5000, gUnk_080875D2[(s16)gUnk_0201DB20.h62A]) >> 8;
            gUnk_0201DB20.h63A += sub_0807B4D0(0x1000, gUnk_080875D2[(s16)gUnk_0201DB20.h62A]) >> 8;
            gUnk_0201DB20.h63E += sub_0807B4D0(0x2800, gUnk_080875D2[(s16)gUnk_0201DB20.h62A]) >> 8;
            if (gUnk_0201DB20.b1BB5 != 0) {
                gUnk_0201DB20.b1BB5 = 1;
                gUnk_0201DB20.b1BB4_0 = 1;
            }
            if (gUnk_0201DB20.b1BB7 != 0) {
                gUnk_0201DB20.b1BB7 = 1;
                gUnk_0201DB20.b1BB6_0 = 1;
            }
            gUnk_0201DB20.b635 = 0;
            /* fallthrough */
        default:                                    /* switch 3 */
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
            switch (gUnk_0201DB20.b628) {      /* switch 2; irregular */
            case 1:                                 /* switch 2 */
                (*(vu16 *)(0x0400001C + 0)) = (u16) (gUnk_0201DB20.h630 + (sub_0807B4D0(0x5000, gUnk_080875D2[(s16)gUnk_0201DB20.h62A]) >> 8));
                *(vu16 *)0x0400001E = gUnk_0201DB20.h632;
                temp_r2_4 = (u8) (sub_0807B4D0(0x4000, gUnk_080875D2[(s16)gUnk_0201DB20.h62A]) >> 8);
                /* FAKEMATCH: retain the shared shifted alpha value for both blend branches. */
                alphaShift = (u32)((u16)gUnk_080875D2[(s16)gUnk_0201DB20.h62A] >> 3) << 24;
                temp_r0 = alphaShift >> 24;
                (*(vu16 *)(0x04000050 + 0)) = 0x3F43;
                if ((s32)(s16) gUnk_0201DB20.h62A <= 3) {
                    *(vu16 *)0x04000014 = (u16) temp_r2_4;
                    *(vu16 *)0x04000016 = gUnk_0201DB20.h63A;
                    *(vu16 *)0x04000010 = (u16) temp_r2_4;
                    *(vu16 *)0x04000012 = gUnk_0201DB20.h63E;
                    sub_0807B4A8(alphaShift >> 25);
                } else {
                    *(vu16 *)0x04000014 = temp_r2_5 = temp_r2_4 + 0xFFC0;
                    *(vu16 *)0x04000016 = (u16) gUnk_0201DB20.h63A;
                    *(vu16 *)0x04000010 = temp_r2_5;
                    *(vu16 *)0x04000012 = (u16) gUnk_0201DB20.h63E;
                    sub_0807B4A8((u32) ((s32) (0x20 - (u8) temp_r0) >> 1));
                }
                break;
            case 2:                                 /* switch 2 */
                gUnk_0201DB20.b628 = 0;
                gUnk_0201DB20.h630 += sub_0807B4D0(0x5000, gUnk_080875D2[(s16)gUnk_0201DB20.h62A]) >> 8;
                if (gUnk_0201DB20.b1BB5 != 0) {
                    gUnk_0201DB20.b1BB5 = 1;
                    gUnk_0201DB20.b1BB4_0 = 1;
                }
                if (gUnk_0201DB20.b1BB7 != 0) {
                    gUnk_0201DB20.b1BB7 = 1;
                    gUnk_0201DB20.b1BB6_0 = 1;
                }
                gUnk_0201DB20.b635 = 0;
                /* fallthrough */
            default:                                /* switch 2 */
                REG_BG3HOFS = gUnk_0201DB20.h630;
                REG_BG3VOFS = gUnk_0201DB20.h632;
                REG_BG1HOFS = gUnk_0201DB20.h638;
                REG_BG1VOFS = gUnk_0201DB20.h63A;
                REG_BG0HOFS = gUnk_0201DB20.h63C;
                REG_BG0VOFS = gUnk_0201DB20.h63E;
                (*(vu16 *)(0x04000050 + 0)) = 0x3FC8;
                (*(vu16 *)(0x04000050 + 2)) = 0x1000;
                break;
            }
        break;
    }
    if (gUnk_0201DB20.pad0[0x61E] != 0) {

    } else {
        inputMode = (struct B7C *)(PSTATE + 0x1C48);
        temp_r0_2 = inputMode->a;
        switch (temp_r0_2) {                        /* switch 4; irregular */
        case 0:                                     /* switch 4 */
            if (gUnk_0201DB20.b628 == 1) {

            } else {
                switch (sp28) {                     /* switch 5; irregular */
                case 0x2:                           /* switch 5 */
                    sub_08077AEC(3);
                    break;
                case 0x40:                          /* switch 5 */
                    sub_080679E0(&frame.scroll);
                    break;
                case 0x80:                          /* switch 5 */
                    sub_08067DA4(&frame.scroll);
                    break;
                default:                            /* switch 5 */
                    temp_r3_4 = (u16 *)((gUnk_0201DB20.cursor * 2) + (gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor] * 6) + (u32)gUnk_0201DB20.cnt1494[0]);
                    temp_r4_2 = *temp_r3_4;
                    if ((u32) temp_r4_2 <= 5U) {

                    } else {
                        switch (sp28) {             /* switch 6; irregular */
                        case 16:                    /* switch 6 */
                            temp_r1_6 = &gUnk_0201DB20.arr620[gUnk_0201DB20.cursor];
                            temp_r2_10 = *temp_r1_6 + 5;
                            if (temp_r2_10 > (s32) (*temp_r3_4 - 1)) {
                                *temp_r1_6 = 0;
                            } else {
                                *temp_r1_6 = (u16) temp_r2_10;
                            }
                            gUnk_0201DB20.h18AC = 0xFC00;
                            sub_0807B100(0, 6, 1, &gUnk_0201DB20.b628);
                            gUnk_0201DB20.b634 ^= 1;
                            sub_0807AFFC(sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]), (gUnk_0201DB20.b634 * 0x1680) + 0x06008000, gUnk_0201DB20.b634);
                            sub_08068E20(((u32) (0xFF & gUnk_0201DB20.h630) >> 3) + 0x1D, ((u32) (0xFF & gUnk_0201DB20.h632) >> 3) + 2, gUnk_0201DB20.b634, 1);
                            gUnk_0201DB20.b635 = 3;
                            gUnk_0201DB20.f1710_0 = 1;
{ u32 column = gUnk_0201DB20.cursor * 2;
        u8 row = gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor];
        u32 offset = column + row * 6;
        sub_08065F34(*(u16 *)(offset + (u32)gUnk_0201DB20.cnt1494[0]), *(u16 *)(column + (u32)gUnk_0201DB20.arr620), &gUnk_0201DB20.f1BB0); }
                            if (gUnk_0201DB20.b1BB7 != 0) {
                                gUnk_0201DB20.b1BB7 = 2;
                                gUnk_0201DB20.b1BB6_0 = 1;
                            }
                            frame.scroll = gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] - 2;
                            var_r5_2 = 0;
                            do {
                                temp_r3_6 = frame.scroll;
                                if (((s16)temp_r3_6 >= 0) && (temp_r3_7 = gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], ((u16) temp_r3_6 < (u32) ((u16 *)gUnk_0201DB20.cnt1494)[(temp_r3_7 * 3) + gUnk_0201DB20.cursor]))) {
                                    sub_0806664C(var_r5_2, sub_08068D1C(gUnk_0201DB20.cursor, temp_r3_7, frame.scroll), var_r5_2 + 1, gUnk_0201DB20.pad1BB8, &gUnk_0201DB20.pad18AE[2]);
                                } else {
                                    DIN8(0x1BC4 + var_r5_2 * 0x10) = 0;
                                }
                                frame.scroll += 1;
                                var_r5_2 += 1;
                            } while ((u32) var_r5_2 <= 4U);
                            gUnk_0201DB20.pad1BB8[0x5C] = 0;
                            gUnk_0201DB20.pad1BB8[0] = 5;
                            DIN16(0x1C58) = 0x1E;
                            sub_08077AEC(0);
                            break;
                        case 32:                    /* switch 6 */
                            temp_r1_5 = &gUnk_0201DB20.arr620[gUnk_0201DB20.cursor];
                            temp_r0_6 = *temp_r1_5;
                            if ((u32) temp_r0_6 <= 4U) {
                                var_r0_2 = temp_r4_2 - 1;
                            } else {
                                var_r0_2 = temp_r0_6 - 5;
                            }
                            *temp_r1_5 = var_r0_2;
                            gUnk_0201DB20.h18AC = 0xFC00;
                            sub_0807B100(6, 0, -1, &gUnk_0201DB20.b628);
                            gUnk_0201DB20.b634 ^= 1;
                            sub_0807AFFC(sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]), (gUnk_0201DB20.b634 * 0x1680) + 0x06008000, gUnk_0201DB20.b634);
                            sub_08068E20(((u32) (0xFF & gUnk_0201DB20.h630) >> 3) + 9, ((u32) (0xFF & gUnk_0201DB20.h632) >> 3) + 2, gUnk_0201DB20.b634, 1);
                            gUnk_0201DB20.h630 -= 0x50;
                            gUnk_0201DB20.b635 = 4;
                            gUnk_0201DB20.f1710_0 = 1;
{ u32 column = gUnk_0201DB20.cursor * 2;
        u8 row = gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor];
        u32 offset = column + row * 6;
        sub_08065F34(*(u16 *)(offset + (u32)gUnk_0201DB20.cnt1494[0]), *(u16 *)(column + (u32)gUnk_0201DB20.arr620), &gUnk_0201DB20.f1BB0); }
                            if (gUnk_0201DB20.b1BB5 != 0) {
                                gUnk_0201DB20.b1BB5 = 2;
                                gUnk_0201DB20.b1BB4_0 = 1;
                            }
                            frame.scroll = gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] - 2;
                            var_r5 = 0;
                            do {
                                temp_r6 = frame.scroll;
                                if (((s16)temp_r6 >= 0) && (temp_r3_5 = gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], ((u16) temp_r6 < (u32) ((u16 *)gUnk_0201DB20.cnt1494)[(temp_r3_5 * 3) + gUnk_0201DB20.cursor]))) {
                                    sub_0806664C(var_r5, sub_08068D1C(gUnk_0201DB20.cursor, temp_r3_5, frame.scroll), var_r5 + 1, gUnk_0201DB20.pad1BB8, &gUnk_0201DB20.pad18AE[2]);
                                } else {
                                    DIN8(0x1BC4 + var_r5 * 0x10) = 0;
                                }
                                frame.scroll += 1;
                                var_r5 += 1;
                            } while ((u32) var_r5 <= 4U);
                            gUnk_0201DB20.pad1BB8[0x5C] = 0;
                            gUnk_0201DB20.pad1BB8[0] = 5;
                            DIN16(0x1C58) = 0x1E;
                            sub_08077AEC(0);
                            break;
                        }
                    }
                    break;
                }
            }
            if (sp28 == 1) {
                ((struct B85 *)(PSTATE + 0x1C3D))->a = 1;
                sub_08077AEC(1); break;
            }
            sub_08070F14_arg(&gUnk_0201F73C);
            break;
        case 1:                                     /* switch 4 */
            if (gUnk_0201DB20.pad1C1E[2] != 0) {

            } else if ((s8) gUnk_0201DB20.pad1711[0x155] != -1) {

            } else {
                switch (sp28) {                     /* switch 7; irregular */
                case 16:                            /* switch 7 */
                    temp_r0_3 = ++((struct W84 *)(PSTATE + 0x1C3C))->b;
                    /* FAKEMATCH: preserve the redundant selector mask after the packed-word write. */
                    asm("" : "+r"(temp_r0_3));
                    switch (temp_r0_3 & 7) {
                    case 1:
                        ((struct W84 *)(PSTATE + 0x1C3C))->b = 4;
                        break;
                    case 7:
                        ((struct W84 *)(PSTATE + 0x1C3C))->b = 0;
                        break;
                    case 5:
                        ((struct W84 *)(PSTATE + 0x1C3C))->b = 6;
                        break;
                    }
                    var_r0 = -8 & gUnk_0201DB20.pad1C1E[0x1F];
                    goto block_121;
                case 32:                            /* switch 7 */
                    switch (((struct W84 *)(PSTATE + 0x1C3C))->b) {
                    case 4:
                        ((struct W84 *)(PSTATE + 0x1C3C))->b = 0;
                        break;
                    case 0:
                        ((struct W84 *)(PSTATE + 0x1C3C))->b = 6;
                        break;
                    case 6:
                        ((struct W84 *)(PSTATE + 0x1C3C))->b = 4;
                        break;
                    default:
                        ((struct W84 *)(PSTATE + 0x1C3C))->b = (u16)(((struct W84 *)(PSTATE + 0x1C3C))->b - 1);
                        break;
                    }
                    var_r0 = -8 & gUnk_0201DB20.pad1C1E[0x1F];
block_121:
                    gUnk_0201DB20.pad1C1E[0x1F] = var_r0 | 3;
                    sub_08077AEC(0);
                    break;
                case 1:                             /* switch 7 */
                    temp_r0_5 = (u32) (DIN32(0x1C3C) << 0xE) >> 0x1D;
                    switch (temp_r0_5) {            /* switch 8; irregular */
                    case 0:                         /* switch 8 */
                        inputMode->b = 2;
                        sub_080787F4(0, 0x180, 0, &gUnk_0201DB20.pad0[0x618]);
                        sub_08077AEC(1);
                        break;
                    case 4:                         /* switch 8 */
                        inputMode->b = 1;
                        sub_080787F4(0, 0x180, 0, &gUnk_0201DB20.pad0[0x618]);
                        sub_08077AEC(1); break;
                    case 5:                         /* switch 8 */
                        inputMode->b = 3;
                        sub_080787F4(0, 0x180, 0, &gUnk_0201DB20.pad0[0x618]);
                        sub_08077AEC(1); break;
                    case 6:                         /* switch 8 */
                        if (sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]) != 0) {
                            temp_r3_3 = gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor];
                            if (*(u16 *)((gUnk_0201DB20.cursor * 2) + (temp_r3_3 * 6) + (u32)gUnk_0201DB20.cnt1494[0]) != 0) {
                                gUnk_03000040.unk4872 = sub_08068D1C(gUnk_0201DB20.cursor, temp_r3_3, gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]);
                                sub_080787F4(0, 0x180, 0, &gUnk_0201DB20.pad0[0x618]);
                                inputMode->b = 0;
                                sub_08077AEC(1); break;
                            }
                        }
                        break;
                    }
                    break;
                case 2:                             /* switch 7 */
                    ((struct B85 *)(PSTATE + 0x1C3D))->a = 2;
                    sub_08077AEC(2);
                    break;
                default:                            /* switch 7 */
                    sub_08070F14_arg(&gUnk_0201F73C);
                    if ((*(u8 *)((u8 *)&gUnk_0201F73C - 0x15F4)) != 1) {
                        switch (sp28) {             /* switch 9; irregular */
                        case 0x40:                  /* switch 9 */
                            sub_080679E0(&frame.scroll);
                            break;
                        case 0x80:                  /* switch 9 */
                            sub_08067DA4(&frame.scroll);
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
    sub_08077EF4(&gUnk_081A6524, 5, 0xB, -1, -1, 3, 2, 0, 0, 0, 0, &gUnk_0201DB20);
    gUnk_0201DB20.pad1711[0xC9] = 1;
    { u32 column = gUnk_0201DB20.cursor * 2;
      u16 scroll = gUnk_0201DB20.arr620[gUnk_0201DB20.cursor];
      u8 row = gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor];
      u32 offset = column + row * 6;
      sub_08065F78(scroll, *(u16 *)(offset + (u32)gUnk_0201DB20.cnt1494[0]), &gUnk_0201DB20.f1BB0); }
    sub_080665D4(&gUnk_0201DB20.pad1BB8[4], &gUnk_0201DB20);
    var_r5_3 = 1;
    do {
        sub_0807B5A0((var_r5_3 * 0x18) + &gUnk_0201DB20.pad18AE[2]);
        var_r5_3 += 1;
    } while ((u32) var_r5_3 <= 6U);
    sub_0806699C(gUnk_0201F740);
    temp_r4_3 = gUnk_0201F740 + 0x1C;
    sub_08067908(temp_r4_3);
    sub_08067660(temp_r4_3);
    sub_08077EF4(&gUnk_081A6EA4, 0, 1, -1, -1, 0, 0, 1, 1, 0, 0, temp_r4_4 = gUnk_0201F740 + 0xFFFFE3E0);
    temp_r5_3 = gUnk_0201F740 + 0xFFFFFAF8;
    sub_0807871C(temp_r5_3);
    sub_08078534(temp_r5_3, 0, 0, 0, 0, 0, 3, 0, 0, temp_r4_4);
    sub_08068B90();
    sub_08068C48();
    sub_0807A298(temp_r4_4);
    sub_0807A2EC(temp_r4_4);
    sub_0807883C(gUnk_0201F740 + 0xFFFFE9F8);
    temp_r1_7 = (*(u8 *)(0xFFFFE9FE + (u32)gUnk_0201F740));
    switch (temp_r1_7) {
    case 3: {
            (*(u8 *)(0xFFFFE9FE + (u32)gUnk_0201F740)) = 0U;
            (*(vu16 *)(0x04000050 + 0)) = 0x3FC8;
            (*(vu16 *)(0x04000050 + 2)) = 0x1000;
        }
        break;
    case 2: return 1;
    default:
        if (((*(u8 *)(0xFFFFEA08 + (u32)gUnk_0201F740)) == 0) && (temp_r1_7 == 0)) {
            temp_r2_11 = (s32)((u32)(*(u16 *)(0xFFFFFC8C + (u32)gUnk_0201F740)) << 0x10);
            if (temp_r2_11 >= 0) {
                (*(vu16 *)(0x04000050 + 0)) = 0x3FC8;
                var_r1 = (void *)0x04000050 + 4;
                var_r0_3 = temp_r2_11 >> 0x18;
            } else {
                (*(vu16 *)(0x04000050 + 0)) = 0x3F88;
                var_r1 = (void *)0x04000050 + 4;
                var_r0_3 = (s32) (0 - (s16) (*(u16 *)(0xFFFFFC8C + (u32)gUnk_0201F740))) >> 8;
            }
            *var_r1 = (s16) var_r0_3;
            { struct DeckState *state = &gUnk_0201DB20;
            if ((s16)state->h18AC > 0x3FF) state->h18AC = 0x400;
            else state->h18AC += 0x30; }
        }
        break;
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806ED44", sub_0806F934); /* 0x0806F934 size 0x1008 */
