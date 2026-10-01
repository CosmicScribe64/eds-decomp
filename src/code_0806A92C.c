#include "global.h"

struct Main { u8 pad[6]; u16 keys; u8 pad8[0x40E - 8]; u16 vblankFlags; u8 pad410[0x485A - 0x410]; u8 step; };
struct Popup { u8 pad[0x3E7]; u8 step; };
extern struct Main gUnk_03000040;
extern struct Popup gUnk_02017A40;
extern u16 (*const gUnk_081A723C[])(void);
struct DeckState {
    u8 pad0[0x620];
    u16 col[3];
    u8 pad626[0x630 - 0x626];
    u16 h630, h632;
    u8 b634, b635;
    u16 h636, h638, h63A, h63C, h63E;
    u8 pad640[0x1494 - 0x640];
    u16 count[2][3];
    u8 row[3];
    u8 pad14A3[0x1710 - 0x14A3];
    u8 flag1710 : 1;
    u8 rest1710 : 7;
    u8 pad1711[0x1866 - 0x1711];
    s8 exchange;
    u8 pad1867[0x18AC - 0x1867];
    u16 h18AC;
    u8 pad18AE[0x1BB0 - 0x18AE];
    u16 slide[2];
    u8 dirty4 : 1; u8 rest4 : 7;
    u8 b1BB5;
    u8 dirty6 : 1; u8 rest6 : 7;
    u8 b1BB7;
    u8 pad1BB8[0x1C1C - 0x1BB8];
    u8 cursor;
    u8 b1C1D;
    u8 pad1C1E[0x1C34 - 0x1C1E];
    u8 active : 1;
    u8 mode : 4;
    u8 rest34 : 3;
    u8 pad1C35[7];
    union {
        struct { u32 low : 15; u32 choice : 3; u32 high : 14; } bits;
        struct { u8 first; u8 phase : 3; u8 rest : 5; u8 tail[2]; } bytes;
    } menu;
    u8 marks[6];
    u8 pad1C46[0x1C5A - 0x1C46];
    u8 low5A : 2;
    u8 selector : 3;
    u8 high5A : 3;
};
extern struct DeckState gUnk_0201DB20;
u32 sub_08068D1C(u8 list, u8 row, u16 col);
void sub_080671E8(u8);
void sub_080679A8(u8);
void sub_0806D644(void);
void sub_08077AEC(u16);
u32 sub_080668DC(u32);
void sub_08077784(u16);
void sub_0807766C(u16);
void sub_080776F8(u16);
void sub_080775BC(u16);
void sub_080774EC(u16);
void sub_08077554(u16);
void sub_080686E8(void);
void sub_0806710C(void);
u16 sub_08068434(void);
void sub_0806AFA4(void);

u16 sub_0806A92C(void)
{
    if (gUnk_081A723C[gUnk_03000040.step]) {
        if (gUnk_081A723C[gUnk_03000040.step]())
            gUnk_03000040.step++;
        return 0;
    }
    return 1;
}

u16 sub_0806A96C(void)
{
    if (gUnk_081A723C[gUnk_02017A40.step]) {
        if (gUnk_081A723C[gUnk_02017A40.step]())
            gUnk_02017A40.step++;
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
extern u16 gUnk_08622AB4[];
struct TrunkEntry { u8 pad[9]; u8 low:2; u8 main:2; u8 side:2; u8 extra:2; u8 tail[2]; };
extern u8 gUnk_02011C20[];
extern const u8 gUnk_081A70FC[];
void sub_08075278(void *, u32);
void sub_080757AC(void);
void sub_0807A2EC(void *);
void sub_0807B0EC(int, int, int, void *);
void sub_080788A0(void *);
void sub_0807B534(void *);
void sub_08066244(void *);
void sub_080666AC(void *);
void sub_08078670(const void *, void *);
void sub_08068DA4(u16, u8, u8, u16);
void sub_08065F34(u16, u16, u16 *);

/* Return the extracted count before testing it, preserving both bit reads. */
static inline int TransferMainCount(struct TrunkEntry *entry) { return entry->main; }
static inline int TransferSideCount(struct TrunkEntry *entry) { return entry->side; }
int sub_0806A9AC(void)
{
    u16 i, j;
    struct B7C *b7c;
    struct BA2 *ba2;
    struct W84 *w84;
    sub_08075278(&gUnk_0201DB20, 0x1C5C);
    gUnk_03000040.vblankFlags = 1;
    sub_080757AC();
    REG16(0x12) = 0; REG16(0x10) = 0;
    REG16(0x16) = 0; REG16(0x14) = 0;
    REG16(0x1A) = 0; REG16(0x18) = 0;
    REG16(0x1E) = 0; REG16(0x1C) = 0;
    REG16(0x28) = 0; REG16(0x2A) = 0;
    REG16(0x3C) = 0; REG16(0x3E) = 0;
    REG16(0) &= 0xE0FF;
    sub_0807A2EC(&gUnk_0201DB20);
    gUnk_0201DB20.h632 = 0;
    gUnk_0201DB20.h630 = 0;
    gUnk_0201DB20.h63A = 0;
    gUnk_0201DB20.h638 = 0;
    gUnk_0201DB20.h63E = 0;
    gUnk_0201DB20.h63C = 0;
    for (i = 0; i <= 2; i++) {
        gUnk_0201DB20.col[i] = 0;
        gUnk_0201DB20.row[i] = 0;
        for (j = 0; j <= 1; j++) gUnk_0201DB20.count[j][i] = 0;
    }
    gUnk_0201DB20.cursor = 2;
    gUnk_0201DB20.b1C1D = 2;
    gUnk_0201DB20.b634 = 0;
    gUnk_0201DB20.b635 = 0;
    gUnk_0201DB20.flag1710 = 0;
    gUnk_0201DB20.h18AC = 0xFC00;
    sub_0807B0EC(0, 0, 0, (u8 *)&gUnk_0201DB20 + 0x628);
    sub_080788A0((u8 *)&gUnk_0201DB20 + 0x640);
    sub_0807B534((u8 *)&gUnk_0201DB20 + 0x18B0);
    /* Card-number ROM view; the mask is formed before the table base. */
    for (i = 1; ((const u16 *)0x08622AB4)[i & 0x7FF] != 0xFFFF; ) {
        if ((u16)(((const u16 *)0x08622AB4)[i & 0x7FF] - 0x780) > 0x4F) {
            u8 *trunk = gUnk_02011C20;
            struct TrunkEntry *entry = (struct TrunkEntry *)(trunk + i * 4);
            if (TransferMainCount(entry) != 0)
                sub_08068DA4(i, 1, gUnk_0201DB20.row[1], gUnk_0201DB20.count[gUnk_0201DB20.row[1]][1]++);
            if (TransferSideCount(entry) != 0)
                sub_08068DA4(i, 2, gUnk_0201DB20.row[2], gUnk_0201DB20.count[gUnk_0201DB20.row[2]][2]++);
        }
        i++;
        if (i > 0x334) break;
    }
    sub_0806710C();
    sub_08065F34(gUnk_0201DB20.count[gUnk_0201DB20.row[gUnk_0201DB20.cursor]][gUnk_0201DB20.cursor], gUnk_0201DB20.col[gUnk_0201DB20.cursor], gUnk_0201DB20.slide);
    gUnk_0201DB20.dirty4 = 1;
    gUnk_0201DB20.dirty6 = 1;
    if (gUnk_0201DB20.count[gUnk_0201DB20.row[gUnk_0201DB20.cursor]][gUnk_0201DB20.cursor] > 5)
        gUnk_0201DB20.b1BB7 = gUnk_0201DB20.b1BB5 = 1;
    else gUnk_0201DB20.b1BB7 = gUnk_0201DB20.b1BB5 = 0;
    sub_08066244(gUnk_0201F6D8);
    sub_080666AC(gUnk_0201F6D8 + 0x68);
    b7c = (struct B7C *)(gUnk_0201F6D8 + 0x7C);
    b7c->a = 0; b7c->b = 0;
    ba2 = (struct BA2 *)(gUnk_0201F6D8 + 0xA2);
    ba2->a = 1;
    w84 = (struct W84 *)((u8 *)b7c + 8);
    w84->b = 2;
    ((struct B85 *)((u8 *)w84 + 1))->a = 3;
    ((struct B85 *)((u8 *)w84 + 1))->a = 1;
    ba2->b = 0;
    sub_08078670(gUnk_081A70FC, gUnk_0201F6D8 - 0x4A0);
    return 1;
}

u16 sub_0806AD10(void)
{
    sub_0806D644();
    if (++gUnk_0201DB20.cursor == 3)
        gUnk_0201DB20.cursor = 1;
    gUnk_0201DB20.active = 0;
    gUnk_0201DB20.mode = 0;
    gUnk_0201DB20.menu.bits.choice = 0;
    gUnk_0201DB20.menu.bytes.phase = 3;
    sub_080671E8(2);
    sub_080679A8(gUnk_0201DB20.cursor);
    gUnk_0201DB20.menu.bits.choice = gUnk_0201DB20.cursor + 1;
    gUnk_0201DB20.menu.bytes.phase = 3;
    return 1;
}
void sub_0806ADBC(u8 *cursor)
{
    if (gUnk_0201DB20.selector == 0) {
        if (gUnk_03000040.keys & 0x100) {
            if (++*cursor == 3)
                *cursor = 1;
            gUnk_0201DB20.active = 0;
            gUnk_0201DB20.mode = 0;
            gUnk_0201DB20.menu.bits.choice = 0;
            gUnk_0201DB20.menu.bytes.phase = 3;
            sub_080671E8(2);
            sub_080679A8(*cursor);
            gUnk_0201DB20.menu.bits.choice = *cursor + 1;
            gUnk_0201DB20.menu.bytes.phase = 3;
            sub_08077AEC(0);
        } else if (gUnk_03000040.keys & 0x200) {
            if (*cursor != 1)
                (*cursor)--;
            else
                *cursor = 2;
            gUnk_0201DB20.active = 0;
            gUnk_0201DB20.mode = 0;
            gUnk_0201DB20.menu.bits.choice = 0;
            gUnk_0201DB20.menu.bytes.phase = 3;
            sub_080671E8(3);
            sub_080679A8(*cursor);
            gUnk_0201DB20.menu.bits.choice = *cursor + 1;
            gUnk_0201DB20.menu.bytes.phase = 3;
            sub_08077AEC(0);
        }
    }
}
u16 sub_0806AF1C(u16 card)
{
    u16 col;
    for (col = 0; col < gUnk_0201DB20.count[gUnk_0201DB20.row[gUnk_0201DB20.cursor]][gUnk_0201DB20.cursor]; col++) {
        if (sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.row[gUnk_0201DB20.cursor], col) == card)
            return col;
    }
    return 0;
}
void sub_0806AFA4(void)
{
    u16 card2, card1;
    u32 count;
    u32 copy;

    if (sub_080668DC(sub_08068D1C(1, gUnk_0201DB20.row[1], gUnk_0201DB20.col[1])))
        sub_08077784(sub_08068D1C(1, gUnk_0201DB20.row[1], gUnk_0201DB20.col[1]));
    else
        sub_0807766C(sub_08068D1C(1, gUnk_0201DB20.row[1], gUnk_0201DB20.col[1]));
    sub_080776F8(sub_08068D1C(2, gUnk_0201DB20.row[2], gUnk_0201DB20.col[2]));
    if (sub_080668DC(sub_08068D1C(2, gUnk_0201DB20.row[2], gUnk_0201DB20.col[2])))
        sub_080775BC(sub_08068D1C(2, gUnk_0201DB20.row[2], gUnk_0201DB20.col[2]));
    else
        sub_080774EC(sub_08068D1C(2, gUnk_0201DB20.row[2], gUnk_0201DB20.col[2]));
    sub_08077554(sub_08068D1C(1, gUnk_0201DB20.row[1], gUnk_0201DB20.col[1]));
    card2 = sub_08068D1C(2, gUnk_0201DB20.row[2], gUnk_0201DB20.col[2]);
    card1 = sub_08068D1C(1, gUnk_0201DB20.row[1], gUnk_0201DB20.col[1]);
    gUnk_0201DB20.row[1] = 0;
    gUnk_0201DB20.row[2] = 0;
    gUnk_0201DB20.marks[0] = 0;
    gUnk_0201DB20.marks[3] = 0;
    gUnk_0201DB20.marks[1] = 0;
    gUnk_0201DB20.marks[4] = 0;
    gUnk_0201DB20.menu.bytes.phase = 3;
    sub_080686E8();
    switch (gUnk_0201DB20.cursor) {
    case 1:
        gUnk_0201DB20.col[1] = sub_0806AF1C(card2);
        count = gUnk_0201DB20.count[gUnk_0201DB20.row[2]][2];
        copy = count;
        /* FAKEMATCH: keep the ROM's separate comparison and decrement values. */
        __asm__ __volatile__("" : "+r"(copy));
        if (copy == gUnk_0201DB20.col[2]) {
            if (copy == 0)
                gUnk_0201DB20.col[2] = copy;
            else
                gUnk_0201DB20.col[2] = count - 1;
        }
        break;
    case 2:
        gUnk_0201DB20.col[2] = sub_0806AF1C(card1);
        count = gUnk_0201DB20.count[gUnk_0201DB20.row[1]][1];
        copy = count;
        __asm__ __volatile__("" : "+r"(copy));
        if (copy == gUnk_0201DB20.col[1]) {
            if (copy == 0)
                gUnk_0201DB20.col[1] = copy;
            else
                gUnk_0201DB20.col[1] = count - 1;
        }
        break;
    }
    sub_080671E8(2);
    sub_0806710C();
}
void sub_0806B190(void)
{
    if (sub_08068434() != 0) {
        switch (gUnk_0201DB20.selector) {
        case 0: break;
        case 1:
            switch (gUnk_0201DB20.menu.bits.choice) {
            case 2:
                gUnk_0201DB20.menu.bits.choice = 3;
                gUnk_0201DB20.menu.bytes.phase = 3;
                gUnk_0201DB20.cursor = 2;
                sub_080671E8(2);
                sub_080679A8(gUnk_0201DB20.cursor);
                break;
            case 3:
                gUnk_0201DB20.menu.bits.choice = 2;
                gUnk_0201DB20.menu.bytes.phase = 3;
                gUnk_0201DB20.cursor = 1;
                sub_080671E8(2);
                sub_080679A8(gUnk_0201DB20.cursor);
                break;
            }
            gUnk_0201DB20.selector++;
            break;
        case 2: break;
        case 3:
            gUnk_0201DB20.exchange = 1;
            gUnk_0201DB20.selector = 4;
            break;
        case 4:
            if (gUnk_0201DB20.exchange == 0) {
                gUnk_0201DB20.exchange = -1;
                sub_0806AFA4();
                gUnk_0201DB20.selector = 0;
            }
            break;
        }
    }
}
void sub_0806B2F8(void)
{
    gUnk_0201DB20.selector = 0;
    switch (gUnk_0201DB20.menu.bits.choice) {
    case 2:
        gUnk_0201DB20.menu.bits.choice = 3;
        gUnk_0201DB20.menu.bytes.phase = 3;
        gUnk_0201DB20.cursor = 2;
        sub_080671E8(2);
        sub_080679A8(gUnk_0201DB20.cursor);
        break;
    case 3:
        gUnk_0201DB20.menu.bits.choice = 2;
        gUnk_0201DB20.menu.bytes.phase = 3;
        gUnk_0201DB20.cursor = 1;
        sub_080671E8(2);
        sub_080679A8(gUnk_0201DB20.cursor);
        break;
    }
}
INCLUDE_ASM("asm/nonmatching/code_0806A92C", sub_0806B3B0); /* 0x0806B3B0 size 0x1134 */
