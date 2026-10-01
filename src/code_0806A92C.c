#include "global.h"

struct Main { u8 pad[6]; u16 keys; u8 pad8[0x40E - 8]; u16 vblankFlags; u8 pad410[0x485A - 0x410]; u8 step; };
struct Popup { u8 pad[0x3E7]; u8 step; };
extern struct Main gUnk_03000040;
extern struct Popup gUnk_02017A40;
extern u16 (*const gUnk_081A723C[])(void);
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
extern struct DeckState gUnk_0201DB20;
u32 sub_08068D1C(int list, int row, int col);
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
/* Private reconstruction. Original callers pass these extra workspace arguments. */
extern u8 gUnk_0201E148[], gUnk_0201F740[];
extern u16 gUnk_0201E140[];
extern u8 gUnk_0201F73C;
extern const u16 gUnk_080875D2[];
extern const u8 gUnk_081A6524[], gUnk_081A6EA4[];
void CpuFastSet(const void *, void *, u32);
void sub_08065108(u32, void *, int, int, void *, int);
void sub_0806518C(u32, void *, int, int, void *);
void sub_08065384(u32, void *, int, int, void *);
void sub_080657F8(int);
void sub_08065AB4(void *, int, int, void *);
void sub_08065E6C(void *, int, int, int);
void sub_08079834(int, void *, int, int, int, int, void *);
void sub_08066260(int, int, int, void *, void *);
void sub_0807B114(void *);
int sub_0807B4D0(int, int);
void sub_0807B4A8(int);
void sub_080787F4(int, int, int, void *);
void sub_080679E0(u16 *), sub_08067DA4(u16 *);
void sub_0807B100(int, int, int, void *);
void sub_0807AFFC(u32, u32, int);
void sub_08068E20(int, int, int, int);
void sub_0806664C(int, u32, int, void *, void *);
void sub_08077EF4(const void *, int, int, int, int, int, int, int, int, int, int, void *);
void sub_08065F78(int, int, void *);
void sub_080665D4(void *, void *);
void sub_0807B5A0(void *), sub_0806699C(void *);
void sub_0806704C(void *, void *, void *);
void sub_08067908(void *), sub_08067660(void *);
void sub_08068180(int), sub_0807871C(void *);
void sub_08078534(void *, int, int, int, int, int, int, int, int, void *);
void sub_08068B90(void), sub_08068C48(void);
void sub_0807A298(void *), sub_0807883C(void *);

#define D gUnk_0201DB20
#define DROW(list) D.row[list]
#define DCOUNT(list) D.count[DROW(list)][list]
#define CURRENT_CARD sub_08068D1C(D.cursor, DROW(D.cursor), D.col[D.cursor])
#define EASE(amount) (sub_0807B4D0(amount, gUnk_080875D2[D.ramp.current]) >> 8)
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
            sub_0806664C(i, sub_08068D1C(D.cursor, DROW(D.cursor), scratch.row), i + 1, &D.slots, D.sprites); \
        else D.slots.entry[i].visible = 0; \
        scratch.row++; \
    } \
    D.slots.entry[5].visible = 0; D.slots.count = 5; D.selectionTimer = 30

#if 0 /* NONMATCHING: prior reviewed-frontier draft, 0x111C vs 0x1134, first diff +0x66A, 493 diff lines; see build/fable/sub_0806B3B0/NOTES.md */
int sub_0806B3B0(void)
{
    struct { u32 zero; u16 row; } scratch;
    u32 keys = gUnk_03000040.keys & 0x3FF;
    u8 i;
    sub_0807B114(gUnk_0201E148);
    if (gUnk_0201E148[0x10E8] & 1) {
        int direction = *(s16 *)(gUnk_0201E148 + 2);
        if ((direction == 4 && gUnk_0201E148[0xD] == 3) ||
            (direction == 3 && gUnk_0201E148[0xD] == 4)) {
            D.flag1710 = 0;
            scratch.zero = 0;
            CpuFastSet(&scratch.zero, (void *)0x0600D000, 0x01000200);
            if ((s16)gUnk_0201E140[gUnk_0201F73C] - 2 >= 0)
                sub_08065108(sub_08068D1C(gUnk_0201F73C, DROW(gUnk_0201F73C), gUnk_0201E140[gUnk_0201F73C] - 2), (void *)0x0600D000, 0, (u8)D.h63A >> 3, D.cardsWork, 1);
            if ((s16)gUnk_0201E140[gUnk_0201F73C] - 1 >= 0)
                sub_08065108(sub_08068D1C(gUnk_0201F73C, DROW(gUnk_0201F73C), gUnk_0201E140[gUnk_0201F73C] - 1), (void *)0x0600D000, 0, ((D.h63A + 0x10) & 0xFF) >> 3, D.cardsWork, 2);
            if ((s16)gUnk_0201E140[gUnk_0201F73C] + 1 < DCOUNT(gUnk_0201F73C))
                sub_08065108(sub_08068D1C(gUnk_0201F73C, DROW(gUnk_0201F73C), gUnk_0201E140[gUnk_0201F73C] + 1), (void *)0x0600D000, 0, ((D.h63A + 0x48) & 0xFF) >> 3, D.cardsWork, 4);
            if ((s16)gUnk_0201E140[gUnk_0201F73C] + 2 < DCOUNT(gUnk_0201F73C))
                sub_08065108(sub_08068D1C(gUnk_0201F73C, DROW(gUnk_0201F73C), gUnk_0201E140[gUnk_0201F73C] + 2), (void *)0x0600D000, 0, ((D.h63A + 0x58) & 0xFF) >> 3, D.cardsWork, 5);
            sub_08079834(0, (void *)0x0600C000, 0, ((D.h63E + 0x20) & 0xFF) >> 3, 30, 6, D.cardsWork);
            if (DCOUNT(gUnk_0201F73C) != 0) {
                sub_0806518C(sub_08068D1C(gUnk_0201F73C, DROW(gUnk_0201F73C), gUnk_0201E140[gUnk_0201F73C]), (void *)0x0600C000, 0, ((D.h63E + 0x20) & 0xFF) >> 3, D.cardsWork);
                sub_080657F8(0);
                sub_08065AB4((void *)0x0600C000, 11, ((D.h63E + 0x38) & 0xFF) >> 3, D.cardsWork);
                sub_08065E6C((void *)0x0600C000, 17, ((D.h63E + 0x38) & 0xFF) >> 3, 6);
            } else {
                sub_08065384(sub_08068D1C(gUnk_0201F73C, DROW(gUnk_0201F73C), gUnk_0201E140[gUnk_0201F73C]), (void *)0x0600C000, 0, ((D.h63E + 0x20) & 0xFF) >> 3, D.cardsWork);
            }
        }
    }
    sub_08066260(D.ramp.current, D.ramp.phase, D.b635, D.sprites, &D.slots);
    switch (D.b635) {
    case 1: case 2:

        switch (D.ramp.phase) {
        case 1:
            REG16(0x1C) = D.h630;
            REG16(0x1E) = D.h632 + EASE(0x5000);
            REG16(0x14) = D.h638;
            REG16(0x16) = D.h63A + EASE(0x1000);
            REG16(0x10) = D.h63C;
            REG16(0x12) = D.h63E + EASE(0x2800);
            break;
        case 2:
            D.ramp.phase = 0;
            D.h632 += EASE(0x5000);
            D.h63A += EASE(0x1000);
            D.h63E += EASE(0x2800);
            FINISH_SCROLL();
        default:
            REFRESH_SCROLL();
            break;
        }
        break;
    case 3: case 4:

        switch (D.ramp.phase) {
        case 1: {
            u8 x, alpha;
            REG16(0x1C) = D.h630 + EASE(0x5000);
            REG16(0x1E) = D.h632;
            x = EASE(0x4000);
            alpha = gUnk_080875D2[D.ramp.current] >> 3;
            REG16(0x50) = 0x3F43;
            if (D.ramp.current <= 3) {
                REG16(0x14) = x; REG16(0x16) = D.h63A;
                REG16(0x10) = x; REG16(0x12) = D.h63E;
                sub_0807B4A8(alpha >> 1);
            } else {
                
                REG16(0x14) = x + 0xFFC0; REG16(0x16) = D.h63A;
                REG16(0x10) = x + 0xFFC0; REG16(0x12) = D.h63E;
                sub_0807B4A8((0x20 - alpha) >> 1);
            }
            break;
        }
        case 2:
            D.ramp.phase = 0;
            D.h630 += EASE(0x5000);
            FINISH_SCROLL();
        default:
            REFRESH_SCROLL();
            REG16(0x50) = 0x3FC8;
            REG16(0x52) = 0x1000;
            break;
        }
        break;
    }
    if (D.animState == 0) {
        switch (D.menuOpen) {
        case 0:
            if (D.ramp.phase != 1) {
                switch (keys) {
                case 2:
                    sub_080787F4(0, 0x180, 0, D.fade);
                    D.exitMode = 0;
                    sub_08077AEC(2);
                    break;
                case 0x40:
                    sub_080679E0(&scratch.row);
                    goto moved;
                case 0x80:
                    sub_08067DA4(&scratch.row);
                    goto moved;
                default:
                    if (DCOUNT(D.cursor) > 5) {
                        switch (keys) {
                        case 0x10:
                            if (D.col[D.cursor] + 5 > DCOUNT(D.cursor) - 1)
                                D.col[D.cursor] = 0;
                            else D.col[D.cursor] += 5;
                            D.h18AC = 0xFC00;
                            sub_0807B100(0, 6, 1, &D.ramp);
                            D.b634 ^= 1;
                            sub_0807AFFC(CURRENT_CARD, 0x06008000 + D.b634 * 0x1680, D.b634);
                            sub_08068E20(((D.h630 & 0xFF) >> 3) + 0x1D, ((D.h632 & 0xFF) >> 3) + 2, D.b634, 1);
                            D.b635 = 3;
                            D.flag1710 = 1;
                            sub_08065F34(DCOUNT(D.cursor), D.col[D.cursor], D.slide);
                            if (D.b1BB7 != 0) { D.b1BB7 = 2; D.dirty6 = 1; }
                            DRAW_SLOTS();
                        moved:
                            sub_08077AEC(0);
                            break;
                        case 0x20:
                            if (D.col[D.cursor] <= 4) D.col[D.cursor] = DCOUNT(D.cursor) - 1;
                            else D.col[D.cursor] -= 5;
                            D.h18AC = 0xFC00;
                            sub_0807B100(6, 0, -1, &D.ramp);
                            D.b634 ^= 1;
                            sub_0807AFFC(CURRENT_CARD, 0x06008000 + D.b634 * 0x1680, D.b634);
                            sub_08068E20(((D.h630 & 0xFF) >> 3) + 9, ((D.h632 & 0xFF) >> 3) + 2, D.b634, 1);
                            D.h630 -= 0x50;
                            D.b635 = 4;
                            D.flag1710 = 1;
                            sub_08065F34(DCOUNT(D.cursor), D.col[D.cursor], D.slide);
                            if (D.b1BB5 != 0) { D.b1BB5 = 2; D.dirty4 = 1; }
                            DRAW_SLOTS();
                            sub_08077AEC(0);
                            break;
                        }
                    }
                    break;
                }
            }
            if (keys == 1) {
                D.menu.bytes.phase = 1;
                sub_08077AEC(1); break;
            }
            sub_0806ADBC(&gUnk_0201F73C);
            break;
        case 1:
            if (D.inputBusy == 0 && D.exchange == -1) {
                switch (keys) {
                case 0x10:
                    if (D.selector == 0) {
                        int next = D.menu.bits.choice + 1;
                        D.menu.bits.choice = next;
                        if ((next & 7) == 1) D.menu.bits.choice++;
                        switch (D.cursor + 1) {
                        case 2:
                            if ((D.menu.word & 0x38000) == 0x18000) D.menu.bits.choice++;
                            break;
                        case 3:
                            if ((D.menu.word & 0x38000) == 0x10000) D.menu.bits.choice++;
                            break;
                        }
                        if ((D.menu.word & 0x38000) == 0x38000) D.menu.bits.choice = 0;
                        D.menu.bytes.phase = 3;
                        goto menuMoved;
                    }
                    break;
                case 0x20:
                    if (D.selector == 0) {
                        if ((D.menu.word & 0x38000) == 0) D.menu.bits.choice = 6;
                        else {
                            u16 next = D.menu.bits.choice - 1;
                            D.menu.bits.choice = next;
                            if ((next & 7) == 1)
                                D.menu.bits.choice = (u16)(D.menu.bits.choice - 1);
                            switch (D.cursor + 1) {
                            case 2:
                                if ((D.menu.word & 0x38000) == 0x18000)
                                    D.menu.bits.choice = (u16)(D.menu.bits.choice - 1);
                                break;
                            case 3:
                                if ((D.menu.word & 0x38000) == 0x10000)
                                    D.menu.bits.choice = (u16)(D.menu.bits.choice - 2);
                                break;
                            }
                        }
                        D.menu.bytes.phase = 3;
                    menuMoved:
                        sub_08077AEC(0);
                    }
                    break;
                case 1:
                    switch (D.menu.bits.choice) {
                    case 0:
                        D.exitMode = 2;
                        sub_080787F4(0, 0x180, 0, D.fade);
                        sub_08077AEC(1); break;
                    case 2:
                        if (sub_08068434() != 0) {
                            D.selector++;
                            sub_08077AEC(1); break;
                        }
                        break;
                    case 3:
                        if (sub_08068434() != 0) {
                            D.selector++;
                            sub_08077AEC(1); break;
                        }
                        break;
                    case 4:
                        D.exitMode = 1;
                        sub_080787F4(0, 0x180, 0, D.fade);
                        sub_08077AEC(1); break;
                    case 5:
                        D.exitMode = 3;
                        sub_080787F4(0, 0x180, 0, D.fade);
                        sub_08077AEC(1); break;
                    case 6:
                        sub_080787F4(0, 0x180, 0, D.fade);
                        D.exitMode = 0;
                        sub_08077AEC(1);
                        break;
                    }
                    break;
                case 2:
                    if (D.selector == 2) {
                        sub_0806B2F8();
                        sub_08077AEC(2);
                    }
                    break;
                default:
                    sub_0806ADBC(&gUnk_0201F73C);
                    if (D.ramp.phase != 1) {
                        switch (keys) {
                        case 0x40: sub_080679E0(&scratch.row); break;
                        case 0x80: sub_08067DA4(&scratch.row); break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
    sub_0806B190();
    sub_08077EF4(gUnk_081A6524, 5, 12, -1, -1, 3, 2, 0, 0, 0, 0, &D);
    D.draw17DA = 1;
    sub_08065F78(D.col[D.cursor], DCOUNT(D.cursor), D.slide);
    sub_080665D4(D.slots.entry, &D);
    for (i = 1; i <= 6; i++) sub_0807B5A0(D.sprites + i * 24);
    sub_0806699C(gUnk_0201F740);
    sub_0806704C(gUnk_0201F740 - 4, gUnk_0201F740 - 3, gUnk_0201F740 - 0x508);
    sub_08067908(gUnk_0201F740 + 0x1C);
    sub_08067660(gUnk_0201F740 + 0x1C);
    sub_08077EF4(gUnk_081A6EA4, 0, 1, -1, -1, 0, 0, 1, 1, 0, 0, gUnk_0201F740 - 0x1C20);
    sub_08068180(gUnk_0201F740[-4]);
    sub_0807871C(gUnk_0201F740 - 0x508);
    sub_08078534(gUnk_0201F740 - 0x508, 0, 0, 0, 0, 0, 3, 0, 0, gUnk_0201F740 - 0x1C20);
    sub_08068B90(); sub_08068C48();
    sub_0807A298(gUnk_0201F740 - 0x1C20);
    sub_0807A2EC(gUnk_0201F740 - 0x1C20);
    sub_0807883C(gUnk_0201F740 - 0x1608);
    if (gUnk_0201F740[-0x1602] != 2) {
    if (gUnk_0201F740[-0x1602] == 3) {
        gUnk_0201F740[-0x1602] = 0;
        REG16(0x50) = 0x3FC8; REG16(0x52) = 0x1000;
    } else if (gUnk_0201F740[-0x15F8] == 0 && gUnk_0201F740[-0x1602] == 0) {
        s16 *fade = (s16 *)(gUnk_0201F740 - 0x374);
        if (*fade >= 0) {
            REG16(0x50) = 0x3FC8;
            REG16(0x54) = *fade >> 8;
        } else {
            REG16(0x50) = 0x3F88;
            REG16(0x54) = -*fade >> 8;
        }
        if ((s16)D.h18AC > 0x3FF) D.h18AC = 0x400;
        else D.h18AC += 0x30;
    }
    return 0;
    }
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806A92C", sub_0806B3B0); /* 0x0806B3B0 size 0x1134 */

