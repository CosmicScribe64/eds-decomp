#include "global.h"

struct DeckState {
    u8 pad[0x618];
    u8 anim[6];
    u8 animState;
    u8 pad61F[0x1C1C - 0x61F];
    u8 cursor;
    u8 pad1C1D[0x1C3D - 0x1C1D];
    u8 phase:3; u8 rest3D:5;
    u8 pad1C3E[0x1C48 - 0x1C3E];
    u8 low48:1; u8 mode:4; u8 high48:3;
    u8 b49, b4A, b4B;
    u16 h4C, h4E;
    u8 b50;
    s8 b51;
    u8 b52, b53;
};
extern struct DeckState gUnk_0201DB20;
struct Main { u8 pad[6]; u16 keys; u8 pad8[0x485A - 8]; u8 step; };
extern struct Main gUnk_03000040;
extern u16 (*const gUnk_081A724C[])(void);
struct TrunkEntry {
    u8 pad[8];
    u16 owned : 10;
    u16 main : 2;
    u16 side : 2;
    u16 extra : 2;
};
extern u8 gUnk_02011C20[];
extern u32 gUnk_02011C20_words[] asm("gUnk_02011C20");

u16 sub_0806C4E4(void)
{
    gUnk_0201DB20.h4E = 0;
    gUnk_0201DB20.h4C = 0;
    gUnk_0201DB20.b49 = 0;
    gUnk_0201DB20.b4A = 0;
    gUnk_0201DB20.b52 = 0;
    gUnk_0201DB20.b53 = 0;
    gUnk_0201DB20.b4B = 0;
    return 1;
}

/* Word arguments are explicitly narrowed, including the default selector
 * return preserved by the ROM. FAKEMATCH: the initialized selector binding
 * retains its original scratch register; no instructions are supplied. */
u32 sub_0806C534(int listWord, int cardWord)
{
    int list = (u16)listWord;
    register int selector asm("r3") = list;
    u32 id;

    id = (u16)cardWord;

    if (list == 1) goto main_count;
    if (list > 1) goto above_one;
    if (list == 0) goto owned_count;
    return list;
above_one:
    if (selector == 2) goto side_count;
    return list;
owned_count:
    {
        u32 base = (u32)gUnk_02011C20;
        struct TrunkEntry *entry;

        entry = (struct TrunkEntry *)(id * 4 + base);
        return entry->owned;
    }
main_count:
    {
        u32 base = (u32)gUnk_02011C20;
        u8 *entry;
        u32 packed;
        u32 main;

        entry = (u8 *)(id * 4 + base);
        packed = entry[9];
        main = (packed << 28) >> 30;

        return main + (packed >> 6);
    }
side_count:
    {
        u32 base = (u32)gUnk_02011C20;
        struct TrunkEntry *entry;

        entry = (struct TrunkEntry *)(id * 4 + base);
        return entry->side;
    }
}

#if 0 /* NONMATCHING: built 0x34 bytes short. The ROM keeps a separate StatisticsKind range test (number<0x776 / number>0x778, reached via goto) and reloads the gUnk_08621DE0 base at each use; agbcc shares the base register and CSEs across the two CARD_TYPE evaluations per card. */
struct CountState {
    u8 pad[0x644];
    union {
        u16 l0[1][0x394];
        struct { u8 pad[0x66A]; u16 cards[1][0x394]; } l1;
        struct { u8 pad[0x70A]; u16 cards[1][0x394]; } l2;
    } lists;
    u8 pad1478[0x1C];
    u16 count[2][3];
    u8 row[3];
};
extern struct CountState gUnk_0201DB20_count asm("gUnk_0201DB20");
extern const u32 gUnk_08621DE0[];
extern const u16 gUnk_08622AB4[];
u32 sub_0806C534(int, int);
#define CARD_TYPE(id) ((int)((gUnk_08621DE0[(id) & 0x7FF] & 0x1F00000) >> 20))
static inline int StatisticsKind(u16 card)
{
    int number = gUnk_08622AB4[card & 0x7FF];
    if (number == 0x776) return 3;
    if (number > 0x778) goto compute;
    if (number < 0x776) goto compute;
    return 1;
compute:
    switch (CARD_TYPE(card)) {
    case 0x16: return 7;
    case 0x15: return 8;
    case 0x17: return 9;
    }
    return (gUnk_08621DE0[card & 0x7FF] & 0xC0000) >> 18;
}
#define COUNT_KIND(kindValue) \
    for (i = 0; i < gUnk_0201DB20_count.count[row][list]; i++) { \
        u16 card = cards[i]; \
        u8 type = CARD_TYPE(card); \
        if (type < 0x15 || type > 0x16) { \
            if (StatisticsKind(card) == (kindValue)) \
                total += sub_0806C534(list, cards[i]); \
        } \
    }
#define COUNT_TYPE(typeValue) \
    for (i = 0; i < gUnk_0201DB20_count.count[row][list]; i++) { \
        u16 card = cards[i]; \
        if (CARD_TYPE(card) == (typeValue)) total += sub_0806C534(list, card); \
    }
u32 sub_0806C590(u8 list, u8 category)
{
    s16 total = 0;
    u8 row = gUnk_0201DB20_count.row[list];
    u16 *cards;
    u16 i;
    switch (list) {
    case 0: cards = gUnk_0201DB20_count.lists.l0[row]; break;
    case 1: cards = gUnk_0201DB20_count.lists.l1.cards[row]; break;
    case 2: cards = gUnk_0201DB20_count.lists.l2.cards[row]; break;
    }
    switch (category) {
    case 1: COUNT_KIND(0); break;
    case 2: COUNT_KIND(1); break;
    case 3: COUNT_KIND(2); break;
    case 4: COUNT_TYPE(0x16); break;
    case 5: COUNT_TYPE(0x15); break;
    case 6: COUNT_KIND(3); break;
    }
    return total;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806C4E4", sub_0806C590); /* 0x0806C590 size 0x5D8 */
struct Save { u8 pad[0x20C6]; u16 total, main, side, extra; };
extern struct Save gUnk_02011C20_s asm("gUnk_02011C20");
struct CountRow { u16 count, percent; };
extern struct CountRow gUnk_02030000[];
void sub_080686E8(void);
u32 sub_0806C590(u8, u8);
int sub_0807B504(int, int);

/* FAKEMATCH: initialized bindings and empty allocation hints preserve the
 * original count-load and sum order. Case 0/2 share a halfword load; case 1
 * derives the extra-copy address from the main-copy offset. No instructions
 * are supplied by the hints. Caller-saved bindings are dead before calls. */
void sub_0806CB68(void)
{
    struct CountRow *rows = gUnk_02030000;
    int value, percent;
    u16 quotient;
    u32 last;
    u8 *cursor;
    sub_080686E8();
    {
        u16 *count;
        switch (gUnk_0201DB20.cursor) {
        case 0: {
            register u32 base asm("r0") = (u32)&gUnk_02011C20_s;
            register u32 off asm("r2") = 0x20C6;
            asm("" : : "r"(base), "r"(off));
            base += off;
            count = (u16 *)base;
            goto read_count;
        }
        case 1: {
            register u32 base asm("r0") = (u32)&gUnk_02011C20_s;
            register u32 off asm("r2") = 0x20C8;
            u16 *m = (u16 *)(base + off);
            off += 4;
            base += off;
            rows[6].count = *m + *(u16 *)base;
            break;
        }
        case 2: {
            u32 base = (u32)&gUnk_02011C20_s;
            u32 off = 0x20CA;
            base += off;
            count = (u16 *)base;
        }
        read_count:
            rows[6].count = *count;
            break;
        }
    }
    rows[6].percent = 100;
    {
        register u32 base asm("r4") = (u32)&gUnk_0201DB20;
        register u32 off asm("r2") = 0x1C1C;
        asm("" : : "r"(off));
        base += off;
        cursor = (u8 *)base;
    }
    rows[0].count = sub_0806C590(*cursor, 1);
    rows[1].count = sub_0806C590(*cursor, 2);
    rows[2].count = sub_0806C590(*cursor, 3);
    rows[3].count = sub_0806C590(*cursor, 4);
    rows[4].count = sub_0806C590(*cursor, 5);
    last = sub_0806C590(*cursor, 6);
    rows[5].count = last;
    {
        register int a asm("r1") = rows[1].count;
        register int b asm("r2") = rows[0].count;
        int c;
        asm("" : : "r"(a), "r"(b));
        a += b;
        b = rows[2].count;
        asm("" : : "r"(b));
        b += a;
        c = rows[3].count;
        asm("" : : "r"(c));
        c += b;
        a = rows[4].count;
        asm("" : : "r"(a));
        a += c;
        last += a;
        rows[6].count = last;
    }
    {
        int input = rows[0].count;
        quotient = sub_0807B504(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[0].percent = percent;
    }
    {
        int input = rows[1].count;
        asm("" : : "r"(input));
        quotient = sub_0807B504(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[1].percent = percent;
    }
    {
        int input = rows[2].count;
        asm("" : : "r"(input));
        quotient = sub_0807B504(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[2].percent = percent;
    }
    {
        int input = rows[3].count;
        asm("" : : "r"(input));
        quotient = sub_0807B504(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[3].percent = percent;
    }
    {
        int input = rows[4].count;
        asm("" : : "r"(input));
        quotient = sub_0807B504(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[4].percent = percent;
    }
    {
        int input = rows[5].count;
        asm("" : : "r"(input));
        quotient = sub_0807B504(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[5].percent = percent;
    }
}

extern const u8 gUnk_081A6EB4[];
void sub_0807B864();
/* FAKEMATCH: the do-while(0) wrapper changes agbcc's loop-invariant hoisting
   so the loop constants land in the same registers as the ROM. */
void sub_0806CD14(void)
{
    u8 i;
    do {
        for (i = 0; i <= 5; i++) {
            struct CountRow *row = &gUnk_02030000[i];
            sub_0807B864(row->count, 4, 1, 0xA8, i * 16 + 0x24, gUnk_081A6EB4, 1, 8, 0, 0, 0, &gUnk_0201DB20);
            sub_0807B864(row->percent, 3, 1, 0xC8, i * 16 + 0x24, gUnk_081A6EB4, 1, 8, 0, 0, 0, &gUnk_0201DB20);
        }
    } while (0);
    sub_0807B864(gUnk_02030000[6].count, 4, 1, 0x98, 0x8C, gUnk_081A6EB4, 1, 8, 0, 0, 0, &gUnk_0201DB20);
}
void CpuFastSet(const void *, void *, u32);
void CpuSet(const void *, void *, u32);
void sub_0807A9C0(u16 *, u16 *, u8, u8, u8, u8, u8);
void sub_0807A908(void *, void *, u8, u8);
/* ASM crop helper decodes its word-valued scalar arguments on entry. */
void sub_0807ADE8(u16 *, int, int, int, void *, int, int, int, int, int);
void sub_08077CEC(u8 *, u8 *, u16);
struct Fade;
void sub_080787F4(u8, s16, u8, struct Fade *);
extern const u8 gUnk_08701B24[], gUnk_08701BA4[], gUnk_08702054[], gUnk_086FCA40[], gUnk_08702504[];
extern const u8 gUnk_086FDB24[], gUnk_086FFB24[], gUnk_086E5030[], gUnk_086FD924[], gUnk_086ED1B0[];
extern const u16 gUnk_08623326;
extern const u32 gUnk_08621DE0[];
extern u8 gUnk_0201F770;
#define REG16(off) (*(volatile u16 *)(0x04000000 + (off)))
void sub_0806CB68(void);
/* FAKEMATCH: two initialized bindings retain the shared tile source and
 * fade-speed store allocation. Three empty constraints preserve the original
 * discarded card-type computation and its range tests without instructions. */
int sub_0806CDD4(void)
{
    u32 zero0 = 0, zero1;
    u16 x, y;
    register const u8 *tiles asm("r10");
    CpuFastSet(&zero0, (void *)0x06000000, 0x01004000);
    zero1 = 0;
    CpuFastSet(&zero1, (void *)0x06010000, 0x01002000);
    for (y = 0; y <= 3; y++) {
        for (x = 0; x <= 3; x++)
            sub_0807A9C0((u16 *)gUnk_08701B24, (void *)(0x0600F000 + (x * 8 + y * 256) * 2), 8, 8, 8, 0, 0);
    }
    sub_0807A908((void *)gUnk_08701BA4, (void *)0x0600E000, 30, 20);
    sub_0807A908((void *)gUnk_08702054, (void *)0x0600D000, 30, 20);
    sub_0807A908((void *)gUnk_086FCA40, (void *)0x0600C000, 30, 20);
    tiles = gUnk_08702504;
    sub_0807ADE8((u16 *)tiles, 0, gUnk_0201DB20.cursor * 5, 7, (void *)0x0600D000, 20, 0, 7, 5, 0);
    sub_0807ADE8((u16 *)tiles, 0, gUnk_0201DB20.cursor * 5, 7, (void *)0x0600C000, 20, 0, 7, 5, 0);
    CpuFastSet(gUnk_086FDB24, (void *)0x06000000, 0x800);
    CpuFastSet(gUnk_086FFB24, (void *)0x06002000, 0x800);
    sub_08077CEC((u8 *)gUnk_086E5030, (void *)0x06010000, 16);
    CpuFastSet(gUnk_086FD924, (void *)0x05000000, 0x80);
    CpuSet(gUnk_086ED1B0, (void *)0x05000200, 0x100);
    REG16(8) = 0x1800;
    REG16(10) = 0x1A01;
    REG16(12) = 0x1C02;
    REG16(14) = 0x1E02;
    {
        int step = -0x180;
        u32 base = (u32)&gUnk_0201DB20;
        u32 off = 0x618;

        sub_080787F4(0, step, 0, (void *)(base + off));
    }
    REG16(0x10) = 0; REG16(0x12) = 0;
    REG16(0x14) = 0; REG16(0x16) = 0;
    REG16(0x18) = 0; REG16(0x1A) = 0;
    REG16(0x1C) = 0; REG16(0x1E) = 0;
    {
        register int speed asm("r0") = 16;
        gUnk_0201F770 = speed;
    }
    REG16(0) = 0x1A00;
    sub_0806CB68();
    {
        u32 card = 0x439;
        int number;
        int threshold = 0x776;
        asm("" : "+r"(card));
        number = gUnk_08623326;
        asm("" : "+r"(threshold));
        if (number == threshold) goto done_type;
        if (number < threshold) goto compute_type;
        if (number <= 0x778) goto done_type;
    compute_type:
        {
            u32 off = card * 4;
            u32 value = (*(const u32 *)((u32)gUnk_08621DE0 + off) & 0x1F00000) >> 20;
            asm("" : : "r"(value));
        }
    done_type: ;
    }
    return 1;
}

void sub_0806CD14(void);
struct Animation {
    u8 pad[6]; u8 state;
    u8 pad7[0x1634 - 7];
    u16 scrollX, scrollY;
    u8 pad1638, close, pad163A, busy;
};
extern struct Animation gUnk_0201E138;
extern u8 gUnk_0201F770;
void sub_0807883C(void *);
void sub_08077AEC(u16);
void sub_0807B4A8(u16);
struct Fade;
void sub_080787F4(u8, s16, u8, struct Fade *);
void sub_0807A298(void *);
void sub_0807A2EC(void *);

u16 sub_0806D010(void)
{
    u32 keys = gUnk_03000040.keys & 0x3FF;
    sub_0807883C(&gUnk_0201E138);
    sub_0806CD14();
    gUnk_0201E138.scrollX += 0x80;
    gUnk_0201E138.scrollY += 0x80;
    REG16(0x1C) = gUnk_0201E138.scrollX >> 8;
    REG16(0x1E) = gUnk_0201E138.scrollY >> 8;
    if (gUnk_0201E138.state == 0 && gUnk_0201E138.busy == 0) {
        switch (keys) {
        case 1:
        case 2:
            gUnk_0201E138.close = 1;
            sub_08077AEC(2);
            break;
        }
    }
    if (gUnk_0201DB20.animState == 2) {
        gUnk_0201DB20.mode = 4;
        gUnk_0201DB20.phase = 3;
        return 1;
    }
    if (gUnk_0201DB20.animState == 3) {
        REG16(0x50) = 0x3F44;
        sub_0807B4A8(16);
        REG16(0) |= 0x400;
        gUnk_0201DB20.animState = 0;
        gUnk_0201DB20.b51 = -1;
        gUnk_0201DB20.b52 = 1;
    }
    if (gUnk_0201DB20.b51 != 0) {
        gUnk_0201DB20.b50 += gUnk_0201DB20.b51;
        if (gUnk_0201DB20.b50 == 8)
            gUnk_0201DB20.b51 = 0;
        if (gUnk_0201DB20.b50 == 16) {
            REG16(0) &= 0xFBFF;
            sub_080787F4(0, 0x180, 0, (struct Fade *)((u8 *)&gUnk_0201DB20 + 0x618));
            gUnk_0201DB20.b51 = 0;
            gUnk_0201DB20.b52 = 0;
        }
        sub_0807B4A8(gUnk_0201DB20.b50);
    }
    sub_0807A298(&gUnk_0201DB20);
    sub_0807A2EC(&gUnk_0201DB20);
    return 0;
}
u16 sub_0806D198(void)
{
    if (gUnk_081A724C[gUnk_03000040.step]) {
        if (gUnk_081A724C[gUnk_03000040.step]())
            gUnk_03000040.step++;
        return 0;
    }
    return 1;
}
#if 0 /* NONMATCHING: byte size matches (0x344) but register allocation differs. The ROM keeps INIT in r5 and the three count-column bases (&count[0][0..2]) in sl/r9/r8, recomputing &INIT.row per use; agbcc hoists &INIT.row and tests the owned bitfield with ands rather than lsls. */
struct InitState {
    u8 pad[0x620]; u16 col[3];
    u8 pad626[0x630 - 0x626];
    u16 h630, h632; u8 b634, b635;
    u16 h636, h638, h63A, h63C, h63E;
    u8 pad640[0x1494 - 0x640];
    u16 count[2][3]; u8 row[3];
    u8 pad14A3[0x1710 - 0x14A3];
    u8 active:1; u8 rest:7;
    u8 pad1711[0x18AC - 0x1711]; u16 h18AC;
    u8 pad18AE[0x1BB0 - 0x18AE]; u16 slide[2];
    u8 dirty4:1; u8 rest4:7; u8 b1BB5;
    u8 dirty6:1; u8 rest6:7; u8 b1BB7;
    u8 pad1BB8[0x1C1C - 0x1BB8]; u8 cursor, previous;
};
extern struct InitState gUnk_0201DB20_init asm("gUnk_0201DB20");
struct InitMain { u8 pad[0x40E]; u16 flags; };
extern struct InitMain gUnk_03000040_init asm("gUnk_03000040");
extern u16 gUnk_08622AB4[];
extern u8 gUnk_0201F6D8[];
struct PanelFlags { u8 active:1; u8 mode:4; u8 rest:3; u8 pad[7]; };
void sub_08075278(void *, u32);
void sub_0807B0EC(int, int, int, void *);
void sub_080788A0(void *);
void sub_0807B534(void *);
void sub_08068DA4(u16, u8, u8, u16);
void sub_0806710C(void);
void sub_08065F34(u16, u16, u16 *);
void sub_08066244(void *);
void sub_080666AC(void *);
#define INIT gUnk_0201DB20_init
int sub_0806D1D8(void)
{
    u16 i, j;
    u16 *count0, *count1, *count2;
    struct PanelFlags *panel;
    sub_08075278(&INIT, 0x1C5C);
    gUnk_03000040_init.flags = 1;
    REG16(0x12) = 0; REG16(0x10) = 0;
    REG16(0x16) = 0; REG16(0x14) = 0;
    REG16(0x1A) = 0; REG16(0x18) = 0;
    REG16(0x1E) = 0; REG16(0x1C) = 0;
    REG16(0x28) = 0; REG16(0x2A) = 0;
    REG16(0x3C) = 0; REG16(0x3E) = 0;
    REG16(0) &= 0xE0FF;
    sub_0807A2EC(&INIT);
    INIT.h632 = 0; INIT.h630 = 0;
    INIT.h63A = 0; INIT.h638 = 0;
    INIT.h63E = 0; INIT.h63C = 0;
    for (i = 0; i <= 2; i++) {
        INIT.col[i] = 0; INIT.row[i] = 0;
        for (j = 0; j <= 1; j++) INIT.count[j][i] = 0;
    }
    INIT.previous = 0; INIT.cursor = 0;
    INIT.b634 = 0; INIT.b635 = 0;
    INIT.active = 0; INIT.h18AC = 0xFC00;
    sub_0807B0EC(0, 0, 0, (u8 *)&INIT + 0x628);
    sub_080788A0((u8 *)&INIT + 0x640);
    sub_0807B534((u8 *)&INIT + 0x18B0);
    count0 = &INIT.count[0][0];
    count2 = &INIT.count[0][2];
    count1 = &INIT.count[0][1];
    for (i = 1; gUnk_08622AB4[i & 0x7FF] != 0xFFFF; ) {
        if ((u16)(gUnk_08622AB4[i & 0x7FF] - 0x780) > 0x4F) {
            struct TrunkEntry *entry = (struct TrunkEntry *)(gUnk_02011C20_words + i);
            if (entry->owned)
                sub_08068DA4(i, 0, INIT.row[0], count0[INIT.row[0] * 3]++);
            if (entry->main || entry->extra)
                sub_08068DA4(i, 1, INIT.row[1], count1[INIT.row[1] * 3]++);
            if (entry->side)
                sub_08068DA4(i, 2, INIT.row[2], count2[INIT.row[2] * 3]++);
        }
        i++;
        if (i > 0x334) break;
    }
    sub_0806710C();
    sub_08065F34(INIT.count[INIT.row[INIT.cursor]][INIT.cursor], INIT.col[INIT.cursor], INIT.slide);
    INIT.dirty4 = 1; INIT.dirty6 = 1;
    if (INIT.count[INIT.row[INIT.cursor]][INIT.cursor] > 5)
        INIT.b1BB7 = INIT.b1BB5 = 1;
    else INIT.b1BB7 = INIT.b1BB5 = 0;
    sub_08066244(gUnk_0201F6D8);
    sub_080666AC(gUnk_0201F6D8 + 0x68);
    panel = (struct PanelFlags *)(gUnk_0201F6D8 + 0x7C);
    panel->active = 0; panel->mode = 0;
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806C4E4", sub_0806D1D8); /* 0x0806D1D8 size 0x344 */
