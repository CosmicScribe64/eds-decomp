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
#if 0 /* NONMATCHING: 539 normalized diff lines remain. The remaining
       * differences are allocation, shared tails and literal pools. Word
       * caller views and marked register hints reproduce the ROM ABI. */

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
s32 sub_08070F18(void) {
    /* FAKEMATCH: preserve the ROM register for the signed next-row comparisons. */
    register s32 next1 asm("r2");
    /* FAKEMATCH: second redraw guard uses the same signed-comparison register. */
    register s32 next2 asm("r2");
    struct { s32 fill; u16 scroll; } frame;
    u32 sp28;
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
    /* FAKEMATCH: signed cursor temporary preserves the unsigned guard and final halfword store. */
    s16 temp_r0_6;
    u32 temp_r2_5;
    s16 temp_r3_6;
    u16 temp_r4_2;
    s16 temp_r6;
    u32 var_r0_2;
    s32 var_r0_4;
    u8 temp_r0;
    u32 alphaShift;
    u32 temp_r0_2;
    u32 temp_r0_4;
    s32 temp_r0_5;
    u32 temp_r1_4;
    u8 *temp_r6_2;
    u8 temp_r1_3;
    s32 temp_r1_7;
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
                alphaShift = (u32) (gUnk_080875D2[(s16)gUnk_0201DB20.h62A] >> 3) << 24;
                temp_r0 = alphaShift >> 24;
                (*(vu16 *)(0x04000050 + 0)) = 0x3F43;
                if ((s32)(s16) gUnk_0201DB20.h62A <= 3) {
                    *(vu16 *)0x04000014 = (u16) temp_r2_4;
                    *(vu16 *)0x04000016 = gUnk_0201DB20.h63A;
                    *(vu16 *)0x04000010 = (u16) temp_r2_4;
                    *(vu16 *)0x04000012 = gUnk_0201DB20.h63E;
                    sub_0807B4A8(alphaShift >> 25);
                } else {
                    REG_BG1HOFS = temp_r2_5 = temp_r2_4 + 0xFFC0;
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
    { struct DeckState *input = &gUnk_0201DB20;
      struct B7C *inputMode;
    if (input->pad0[0x61E] != 0) {

    } else {
        inputMode = (struct B7C *)((u8 *)input + 0x1C48);
        temp_r0_2 = inputMode->a;
        switch (temp_r0_2) {                        /* switch 4; irregular */
        case 0:                                     /* switch 4 */
            if (gUnk_0201DB20.b628 == 1) {

            } else {
                switch (sp28) {                     /* switch 5; irregular */
                case 0x2:                           /* switch 5 */
                    sub_080787F4(0, 0x180, 0, &gUnk_0201DB20.pad0[0x618]);
                    inputMode->b = 0;
                    sub_08077AEC(2);
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
                            sub_08065F34(*(u16 *)((gUnk_0201DB20.cursor * 2) + (gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor] * 6) + (u32)gUnk_0201DB20.cnt1494[0]), gUnk_0201DB20.arr620[gUnk_0201DB20.cursor], &gUnk_0201DB20.f1BB0);
                            if (gUnk_0201DB20.b1BB7 != 0) {
                                gUnk_0201DB20.b1BB7 = 2;
                                gUnk_0201DB20.b1BB6_0 = 1;
                            }
                            frame.scroll = gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] - 2;
                            var_r5_2 = 0;
                            do {
                                temp_r3_6 = frame.scroll;
                                if ((temp_r3_6 >= 0) && (temp_r3_7 = gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], ((u16) temp_r3_6 < (u32) ((u16 *)gUnk_0201DB20.cnt1494)[(temp_r3_7 * 3) + gUnk_0201DB20.cursor]))) {
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
                            sub_08065F34(*(u16 *)((gUnk_0201DB20.cursor * 2) + (gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor] * 6) + (u32)gUnk_0201DB20.cnt1494[0]), gUnk_0201DB20.arr620[gUnk_0201DB20.cursor], &gUnk_0201DB20.f1BB0);
                            if (gUnk_0201DB20.b1BB5 != 0) {
                                gUnk_0201DB20.b1BB5 = 2;
                                gUnk_0201DB20.b1BB4_0 = 1;
                            }
                            frame.scroll = gUnk_0201DB20.arr620[gUnk_0201DB20.cursor] - 2;
                            var_r5 = 0;
                            do {
                                temp_r6 = frame.scroll;
                                if ((temp_r6 >= 0) && (temp_r3_5 = gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], ((u16) temp_r6 < (u32) gUnk_0201DB20.cnt1494[0][(temp_r3_5 * 3) + gUnk_0201DB20.cursor]))) {
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
                gUnk_0201DB20.pad1C1E[0x1F] = (-8 & gUnk_0201DB20.pad1C1E[0x1F]) | 1;
                goto block_136;
            }
            sub_08070F14_arg(&gUnk_0201F73C);
            break;
        case 1:                                     /* switch 4 */
            if (input->pad1C1E[2] != 0) {

            } else if ((s8) input->pad1711[0x155] != -1) {

            } else {
                switch (sp28) {                     /* switch 7; irregular */
                case 16:                            /* switch 7 */
                    ((struct W84 *)(((u8 *)input) + 0x1C3C))->b++;
                    if (((struct W84 *)(((u8 *)input) + 0x1C3C))->b == 1) {
                        ((struct W84 *)(((u8 *)input) + 0x1C3C))->b = 4;
                    } else if (((struct W84 *)(((u8 *)input) + 0x1C3C))->b == 7) {
                        ((struct W84 *)(((u8 *)input) + 0x1C3C))->b = 0;
                    }
                    var_r0 = -8 & input->pad1C1E[0x1F];
block_121:
                    input->pad1C1E[0x1F] = var_r0 | 3;
                    sub_08077AEC(0);
                    break;
                case 32:                            /* switch 7 */
                    temp_r0_4 = (*(u32 *)((u8 *)input + 0x1C3C)) << 0xE;
                    temp_r1_4 = temp_r0_4 >> 0x1D;
                    if (temp_r1_4 != 0) {
                        if (temp_r1_4 == 4) {
                            (*(u32 *)((u8 *)input + 0x1C3C)) = (s32) ((*(u32 *)((u8 *)input + 0x1C3C)) & 0xFFFC7FFF);
                        } else {
                            (*(u32 *)((u8 *)input + 0x1C3C)) = (s32) ((0xFFFC7FFF & (*(u32 *)((u8 *)input + 0x1C3C))) | (((u16) ((temp_r0_4 >> 0x1D) - 1) & 7) << 0xF));
                        }
                    } else {
                        (*(u32 *)((u8 *)input + 0x1C3C)) = (s32) ((0xFFFC7FFF & (*(u32 *)((u8 *)input + 0x1C3C))) | 0x30000);
                    }
                    var_r0 = -8 & input->pad1C1E[0x1F];
                    goto block_121;
                case 1:                             /* switch 7 */
                    temp_r0_5 = (u32) ((*(u32 *)((u8 *)input + 0x1C3C)) << 0xE) >> 0x1D;
                    switch (temp_r0_5) {            /* switch 8; irregular */
                    case 0:                         /* switch 8 */
                        if (input->cnt1494[0][(input->arr14A0[input->cursor] * 3) + input->cursor] != 0) {
                            inputMode->b = 2;
                            sub_080787F4(0, 0x180, 0, &input->pad0[0x618]);
                            goto block_136;
                        } else {
                            goto block_137;
                        }
                        break;
                    case 4:                         /* switch 8 */
                        inputMode->b = 1;
                        sub_080787F4(0, 0x180, 0, &input->pad0[0x618]);
                        goto block_136;
                    case 5:                         /* switch 8 */
                        inputMode->b = 3;
                        sub_080787F4(0, 0x180, 0, &input->pad0[0x618]);
                        goto block_136;
                    case 6:                         /* switch 8 */
                        if (sub_08068D1C(input->cursor, input->arr14A0[input->cursor], input->arr620[input->cursor]) != 0) {
                            temp_r3_3 = input->arr14A0[input->cursor];
                            if (*(u16 *)((input->cursor * 2) + (temp_r3_3 * 6) + (u32)input->cnt1494[0]) != 0) {
                                gUnk_03000040.unk4872 = sub_08068D1C(input->cursor, temp_r3_3, input->arr620[input->cursor]);
                                sub_080787F4(0, 0x180, 0, &input->pad0[0x618]);
                                inputMode->b = 0;
                                goto block_136;
                            }
                        }
                        goto block_137;
block_136:
                        sub_08077AEC(1);
                        break;
block_137:
                        sub_08077AEC(3);
                        break;
                    }
                    break;
                case 2:                             /* switch 7 */
                    input->pad1C1E[0x1F] = (-8 & input->pad1C1E[0x1F]) | 2;
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
    }
    sub_0806B190();
    sub_08077EF4(&gUnk_081A6524, 5, 0xC, -1, -1, 3, 2, 0, 0, 0, 0, &gUnk_0201DB20);
    gUnk_0201DB20.pad1711[0xC9] = 1;
    sub_08065F78(gUnk_0201DB20.arr620[gUnk_0201DB20.cursor], *(u16 *)((gUnk_0201DB20.cursor * 2) + (gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor] * 6) + (u32)gUnk_0201DB20.cnt1494[0]), &gUnk_0201DB20.f1BB0);
    sub_080665D4(&gUnk_0201DB20.pad1BB8[4], &gUnk_0201DB20);
    var_r5_3 = 1;
    do {
        sub_0807B5A0((var_r5_3 * 0x18) + &gUnk_0201DB20.pad18AE[2]);
        var_r5_3 += 1;
    } while ((u32) var_r5_3 <= 6U);
    sub_0806699C(gUnk_0201F740);
    temp_r6_2 = gUnk_0201F740 - 4;
    temp_r5_3 = gUnk_0201F740 + 0xFFFFFAF8;
    sub_0806704C(temp_r6_2, gUnk_0201F740 - 3, temp_r5_3);
    temp_r4_3 = gUnk_0201F740 + 0x1C;
    sub_08067908(temp_r4_3);
    sub_08067660(temp_r4_3);
    temp_r4_4 = gUnk_0201F740 + 0xFFFFE3E0;
    sub_08077EF4(&gUnk_081A6EA4, 0, 1, -1, -1, 0, 0, 1, 1, 0, 0, temp_r4_4);
    sub_08068180(*temp_r6_2);
    sub_0807871C(temp_r5_3);
    sub_08078534(temp_r5_3, 0, 0, 0, 0, 0, 3, 0, 0, temp_r4_4);
    sub_08068B90();
    sub_08068C48();
    sub_0807A298(temp_r4_4);
    sub_0807A2EC(temp_r4_4);
    sub_0807883C(gUnk_0201F740 + 0xFFFFE9F8);
    temp_r1_7 = (*(u8 *)(0xFFFFE9FE + (u32)gUnk_0201F740));
    if (temp_r1_7 != 2) {
        if (temp_r1_7 == 3) {
            (*(u8 *)(0xFFFFE9FE + (u32)gUnk_0201F740)) = 0U;
            (*(vu16 *)(0x04000050 + 0)) = 0x3FC8;
            (*(vu16 *)(0x04000050 + 2)) = 0x1000;
        } else if (((*(u8 *)(0xFFFFEA08 + (u32)gUnk_0201F740)) == 0) && (temp_r1_7 == 0)) {
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
            if ((s32) (s16) gUnk_0201DB20.h18AC > 0x3FF) {
                var_r0_4 = 0x400;
            } else {
                var_r0_4 = gUnk_0201DB20.h18AC + 0x30;
            }
            gUnk_0201DB20.h18AC = var_r0_4;
        }
        return 0;
    }
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0807093C", sub_08070F18); /* 0x08070F18 size 0x1028 */
