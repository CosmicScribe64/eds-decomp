#include "global.h"
#include "gba.h"

/* Card-list state at 0x0201DB20 (see wiki code-08064af0, code-08065e6c). */
extern u8 gUnk_0201DB20[];

#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_KIND(id) ((int)((CARD_STATS(id) & 0x1F00000) >> 20))
/* Scene state at 0x0201DB20 (see wiki code-08064af0). The three card lists start at +0x644, +0xCAE, +0xD4E
   (0x728 bytes per row); modelled as a union so every access is a field of the one symbol. */
struct PageState {
    u8 pad0[0x620];
    u16 arr620[15];                 /* +0x620 */
    u16 scroll;                     /* +0x63E */
    u8 pad640[4];
    union {
        u16 l0[1][0x394];           /* +0x644 */
        struct { u8 p[0xCAE - 0x644]; u16 a[1][0x394]; } l1;
        struct { u8 p[0xD4E - 0x644]; u16 a[1][0x394]; } l2;
    } lists;
    u8 pad_[0x1C];                  /* (agbcc rounds the union member structs up to 4 bytes) */
    u16 cnt1494[2][3];              /* +0x1494 per-row counters [row][cursor] */
    u8 arr14A0[0x1712 - 0x14A0];    /* +0x14A0 */
    u16 f1712[(0x1BB0 - 0x1712) / 2];
    u16 f1BB0;
    u16 f1BB2;
    u8 pad1BB4[0x1C1C - 0x1BB4];
    u8 cursor;                      /* +0x1C1C */
    u8 pad1C1D[0x1C42 - 0x1C1D];
    u8 f1C42[0x1C58 - 0x1C42];
    s16 f1C58;
};
extern struct PageState gUnk_0201DB20_p asm("gUnk_0201DB20");
/* Complete two-row views of the overlapping card lists. */
struct CardList0View { u8 pad[0x644]; u16 cards[2][0x394]; };
struct CardList1View { u8 pad[0xCAE]; u16 cards[2][0x394]; };
struct CardList2View { u8 pad[0xD4E]; u16 cards[2][0x394]; };
extern const u8 gUnk_0822C720[];
extern u16 gUnk_0201F775_h asm("gUnk_0201F775");
extern void sub_080791F4(void *src, void *dst, u32 a, u32 b, u32 c);
extern u16 *sub_08077EF4(const void *a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l);
extern const u8 gUnk_081A70F4[];
extern const u8 gUnk_080874A0[];
struct Cnt { u8 pad8[8]; u16 n : 10; };
struct Cnt9 { u8 pad9[9]; u8 b9; };
extern u32 gTrunk32[] asm("gUnk_02011C20");
#define TN(id) (((struct Cnt *)(gTrunk32 + (id)))->n)
#define T9(id) (((struct Cnt9 *)(gTrunk32 + (id)))->b9)
#define TRUNK ((u8 *)0x02011C20)
#define TRUNK_N(t, id) (((struct Cnt *)((t) + (u16)(id) * 4))->n)
#define TRUNK_B9(t, id) (((struct Cnt9 *)((t) + (u16)(id) * 4))->b9)
struct SaveM {
    u8 pad0[0x20C6];
    u16 f20C6;
    u16 f20C8;
    u16 f20CA;
};
extern struct SaveM gUnk_02011C20_s asm("gUnk_02011C20");
extern u8 gUnk_0201EFC0[];
extern u16 gUnk_02013CE8[];
extern u16 gUnk_02013CEC[];
extern const u8 gUnk_081A6EB4[];
extern void sub_0807B864(int a, int b, int c, int d, int e, const void *f, int g, int h, int i, int j, int k, int l);
extern u16 sub_08068D1C(u8 list, u8 row, u16 col);
extern u16 sub_080668DC(u16 id);
extern void sub_08064E28(u8 col, u8 row, u8 set, u8 wrap, u32 base);

#define NUMCALL(a, b, c, d, e) sub_0807B864(a, b, c, d, e, gUnk_081A6EB4, 1, 8, 0, 0, 0, (int)st)
#define CUR() sub_08068D1C(list, gUnk_0201EFC0[list], st->arr620[list])
/* u16 at byte offset `off` of the trunk block; the same symbol as TN/T9, so CSE shares its base. */
#define T16(off) (*(u16 *)((u8 *)gTrunk32 + (off)))
/* Draw the card-count numbers of the deck-edit side panel for list `list` (hypothesis). */
void sub_08068180(u8 list)
{
    struct PageState *st = &gUnk_0201DB20_p;
    NUMCALL(TN(CUR()), 2, 1, 0x30, 0x88);
    NUMCALL(((u32)(T9(CUR()) << 28) >> 30) + (T9(CUR()) >> 6), 2, 1, 0x68, 0x77);
    NUMCALL(gUnk_02013CE8[0] + gUnk_02013CEC[0], 2, 1, 0x68, 0x88);
    CUR(); /* result unused in the ROM too */
    NUMCALL((u32)(T9(CUR()) << 26) >> 30, 2, 1, 0xA8, 0x77);
    NUMCALL(T16(0x20CA), 2, 1, 0xA8, 0x88);
    switch (list) {
    case 0:
        NUMCALL(T16(0x20C6) + gUnk_02013CE8[0] + T16(0x20CA) + gUnk_02013CEC[0], 4, 1, 0xE0, 0x88);
        break;
    case 1:
        NUMCALL(gUnk_02013CE8[0], 2, 1, 0xE0, 0x78);
        NUMCALL(gUnk_02013CEC[0], 2, 1, 0xE0, 0x8B);
        break;
    case 2:
        NUMCALL(st->f1712[st->arr14A0[list]], 2, 1, 0xE0, 0x79);
        NUMCALL(T16(0x20CA) - st->f1712[st->arr14A0[list]], 2, 1, 0xE0, 0x8A);
        break;
    }
}
#undef NUMCALL
#undef CUR
#undef T16
#define ST gUnk_0201DB20_p
/* Copies of the current card (by cursor) the deck still holds / may add, from the trunk entry's 2-bit fields (hypothesis). */
/* Editor rows are 0..1 and each selected column belongs to its list.
 * sub_080668DC is pure, so the nested selectors remain in 0..2. */
u32 sub_08068434(void)
{
    struct PageState *st = &ST;
    u8 *cursor = &st->cursor;
    switch (*cursor) {
    case 0: {
        u32 row = ST.arr14A0[0];
        u32 *t;
        if (ST.cnt1494[row][0] == 0)
            return 0;
        t = gTrunk32;
        return ((struct Cnt *)(t + ((struct CardList0View *)&ST)->cards[row][ST.arr620[0]]))->n;
    }
    case 1: {
        u32 rowOffset = 0x14A1;
        u8 *rows = (u8 *)((u32)&ST + rowOffset);
        u16 col = ST.arr620[1];
        if (sub_080668DC(((struct CardList1View *)&ST)->cards[*rows][col])) {
            u32 *t;
            u32 id;
            u8 cur = *cursor;
            u32 twice = cur * 2;
            u16 col;
            u8 row;
            row = ST.arr14A0[cur];
            if (ST.cnt1494[row][cur] == 0)
                return 0;
            t = gTrunk32;
            col = *(u16 *)((u32)&ST.arr620 + twice);
            switch (cur) {
            case 0:
                id = ((struct CardList0View *)&ST)->cards[row][col];
                break;
            case 1:
                id = ((struct CardList1View *)&ST)->cards[row][col];
                break;
            case 2:
                id = ((struct CardList2View *)&ST)->cards[row][col];
                break;
            }
            return ((struct Cnt9 *)(t + (u16)id))->b9 >> 6;
        } else {
            u32 *t;
            u32 id;
            u8 cur = *cursor;
            u32 twice = cur * 2;
            u16 col;
            u8 row;
            row = ST.arr14A0[cur];
            if (ST.cnt1494[row][cur] == 0)
                return 0;
            t = gTrunk32;
            col = *(u16 *)((u32)&ST.arr620 + twice);
            switch (cur) {
            case 0:
                id = ((struct CardList0View *)&ST)->cards[row][col];
                break;
            case 1:
                id = ((struct CardList1View *)&ST)->cards[row][col];
                break;
            case 2:
                id = ((struct CardList2View *)&ST)->cards[row][col];
                break;
            }
            {
                /* FAKEMATCH: retain the selected ID while forming its trunk offset. */
                register u32 copy asm("r1") = id;
                register u32 offset asm("r0");
                asm("" : : "r"(copy));
                offset = (u16)copy * 4;
                return ((u32)((struct Cnt9 *)((u32)t + offset))->b9 << 28) >> 30;
            }
        }
    }
    case 2: {
        /* FAKEMATCH: staged state addresses and the live row-selector offset. */
        register u32 count asm("r0");
        u32 co;
        u32 *t;
        register u32 ro asm("r2") = 0x14A2;
        u32 row;
        count = (u32)&ST;
        count += ro;
        row = *(u8 *)count;
        asm("" : : "r"(ro));
        count = (u32)&ST;
        count += row * 6;
        co = 0x1498;
        if (*(u16 *)(count + co) == 0)
            return 0;
        t = gTrunk32;
        {
            u32 off = 0x624;
            u32 col;
            count = (u32)&ST;
            count += off;
            col = *(u16 *)count;
            return ((u32)((struct Cnt9 *)(t + ((struct CardList2View *)&ST)->cards[row][col]))->b9 << 26) >> 30;
        }
    }
    }
    return 0;
}

#undef ST
/* Pointer to the row (0x728 bytes) of card list `list` selected by the per-list row index arr14A0[list]. */
u16 *sub_08068668(int list)
{
    switch ((u8)list) {
    case 0:
        return gUnk_0201DB20_p.lists.l0[gUnk_0201DB20_p.arr14A0[0]];
    case 1:
        return gUnk_0201DB20_p.lists.l1.a[gUnk_0201DB20_p.arr14A0[1]];
    case 2:
        return gUnk_0201DB20_p.lists.l2.a[gUnk_0201DB20_p.arr14A0[2]];
    }
}
#define L0_86E8 (((struct CardList0View *)gUnk_0201DB20)->cards)
#define L1_86E8 (((struct CardList1View *)gUnk_0201DB20)->cards)
#define L2_86E8 (((struct CardList2View *)gUnk_0201DB20)->cards)
#define CNT_86E8 (gUnk_0201DB20_p.cnt1494)
/* Second row of card list `list`. */
static inline u16 *Row1_86E8(u8 list)
{
    switch (list) {
    case 0:
        return L0_86E8[1];
    case 1:
        return L1_86E8[1];
    case 2:
        return L2_86E8[1];
    }
}
static inline int Has2_86E8(u16 id)
{
    if (gUnk_0201DB20[0x1C5A] & 0x20)
        return (T9(id) << 28) >> 30;
    return ((T9(id) << 28) >> 30) || (T9(id) >> 6);
}
struct Mode86E8 { u8 pad[0x4874]; u8 mode : 2; };
extern struct Mode86E8 gMain_86E8 asm("gUnk_03000040");
#if 0 /* NONMATCHING (score 352): NONMATCHING: score 352; private rebuild_lists draft (RebuildRead/RebuildWrite
       * inline switches) gives right size 0x4A8 and 0xC frame; register allocation/branch layout differ */
/* Rebuild visible collection/deck rows, then compact the selected saved rows. */
struct ListMainMode { u8 pad[0x4874]; u8 mode : 2; u8 rest : 6; };
extern struct ListMainMode gListMainMode asm("gUnk_03000040");
extern const u16 gUnk_08622AB4[];
#define REBUILD_NUMBER(i) (gUnk_08622AB4[(i) & 0x7FF])
#define REBUILD_OWN(i) ((u32)TN(i) << 22)
#define REBUILD_DECK(i) (((u32)T9(i) << 28) >> 30)
#define REBUILD_EXTRA(i) (T9(i) >> 6)
#define REBUILD_SIDE(i) (((u32)T9(i) << 26) >> 30)
#define REBUILD_ST gUnk_0201DB20_p
static inline u16 RebuildRead(u8 list, u8 row, u16 col)
{
    switch (list) {
    case 0: return REBUILD_ST.lists.l0[row][col];
    case 1: return REBUILD_ST.lists.l1.a[row][col];
    case 2: return REBUILD_ST.lists.l2.a[row][col];
    }
}
static inline void RebuildWrite(u16 val, u8 list, u8 row, u16 col)
{
    switch (list) {
    case 0: REBUILD_ST.lists.l0[row][col] = val; break;
    case 1: REBUILD_ST.lists.l1.a[row][col] = val; break;
    case 2: REBUILD_ST.lists.l2.a[row][col] = val; break;
    }
}
void sub_080686E8(void)
{
    u16 i;
    for (i = 0; i <= 2; i++)
        REBUILD_ST.cnt1494[0][i] = 0;
    switch (gListMainMode.mode) {
    case 1:
        for (i = 1; i <= 0x334; i++) {
            u16 number = REBUILD_NUMBER(i);
            if ((u16)(number - 0x4BA) > 0x315 || (u16)(number - 0x76C) <= 0x13) {
                if (REBUILD_OWN(i) != 0)
                    REBUILD_ST.lists.l0[0][REBUILD_ST.cnt1494[0][0]++] = i;
                if ((s32)((u32)gUnk_0201DB20[0x1C5A] << 26) < 0) {
                    if (REBUILD_DECK(i) != 0)
                        REBUILD_ST.lists.l1.a[0][REBUILD_ST.cnt1494[0][1]++] = i;
                } else if (REBUILD_DECK(i) != 0 || REBUILD_EXTRA(i) != 0) {
                    REBUILD_ST.lists.l1.a[0][REBUILD_ST.cnt1494[0][1]++] = i;
                }
                if (REBUILD_SIDE(i) != 0)
                    REBUILD_ST.lists.l2.a[0][REBUILD_ST.cnt1494[0][2]++] = i;
            }
        }
        break;
    case 0:
        for (i = 1; i <= 0x334; i++) {
            if ((u16)(REBUILD_NUMBER(i) - 0x780) > 0x4F) {
                if (REBUILD_OWN(i) != 0)
                    REBUILD_ST.lists.l0[0][REBUILD_ST.cnt1494[0][0]++] = i;
                if ((s32)((u32)gUnk_0201DB20[0x1C5A] << 26) < 0) {
                    if (REBUILD_DECK(i) != 0)
                        REBUILD_ST.lists.l1.a[0][REBUILD_ST.cnt1494[0][1]++] = i;
                } else if (REBUILD_DECK(i) != 0 || REBUILD_EXTRA(i) != 0) {
                    REBUILD_ST.lists.l1.a[0][REBUILD_ST.cnt1494[0][1]++] = i;
                }
                if (REBUILD_SIDE(i) != 0)
                    REBUILD_ST.lists.l2.a[0][REBUILD_ST.cnt1494[0][2]++] = i;
            }
        }
        break;
    case 2:
        for (i = 1; i <= 0x334; i++) {
            if ((u16)(REBUILD_NUMBER(i) - 0x76C) > 0x63)
                REBUILD_ST.lists.l0[0][REBUILD_ST.cnt1494[0][0]++] = i;
        }
        break;
    }
    for (i = 0; i <= 2; i++) {
        if (REBUILD_ST.arr14A0[i] == 1) {
            u16 n = 0;
            u16 j;
            for (j = 0; j < REBUILD_ST.cnt1494[1][i]; j++) {
                switch (i) {
                case 0:
                    if (REBUILD_OWN(RebuildRead(i, 1, j)))
                        RebuildWrite(RebuildRead(i, 1, j), i, 1, n++);
                    break;
                case 1:
                    if (REBUILD_DECK(RebuildRead(i, 1, j)) || REBUILD_EXTRA(RebuildRead(i, 1, j)))
                        RebuildWrite(RebuildRead(i, 1, j), i, 1, n++);
                    break;
                case 2:
                    if (REBUILD_SIDE(RebuildRead(i, 1, j)))
                        RebuildWrite(RebuildRead(i, 1, j), i, 1, n++);
                    break;
                }
            }
            REBUILD_ST.cnt1494[1][i] = n;
        }
    }
}
#undef REBUILD_NUMBER
#undef REBUILD_OWN
#undef REBUILD_DECK
#undef REBUILD_EXTRA
#undef REBUILD_SIDE
#undef REBUILD_ST
#endif
INCLUDE_ASM("asm/nonmatching/code_08068180", sub_080686E8); /* 0x080686E8 size 0x4A8 */
/* Load the graphic of the currently selected card into VRAM 0x06012FE0 if it changed. */
/* The selected list is 0, 1 or 2, as in the adjacent list accessors. */
void sub_08068B90(void)
{
    u32 id;
    u16 *pp;
    const u8 *tb;
    int w;
    u8 row = gUnk_0201DB20_p.arr14A0[gUnk_0201DB20_p.cursor];
    u16 col = gUnk_0201DB20_p.arr620[gUnk_0201DB20_p.cursor];
    u16 v;
    u16 buf[2];
    switch (gUnk_0201DB20_p.cursor) {
    case 0:
        id = gUnk_0201DB20_p.lists.l0[row][col];
        break;
    case 1:
        id = gUnk_0201DB20_p.lists.l1.a[row][col];
        break;
    case 2:
        id = gUnk_0201DB20_p.lists.l2.a[row][col];
        break;
    }
    tb = ((const u8 *)0x0822C720) + (u16)id * 64;
    pp = &gUnk_0201F775_h;
    v = *(u16 *)tb;
    w = *pp;
    if (v != w) {
        *pp = v;
        buf[0] = v;
        buf[1] = 0;
        sub_080791F4(buf, (void *)0x06012FE0, 1, 0, 0);
    }
}

void sub_08068C48(void)
{
    int r7;
    if (gUnk_0201DB20_p.f1C42[gUnk_0201DB20_p.cursor] == 0) {
        u16 cnt = gUnk_0201DB20_p.cnt1494[*((u8 *)&gUnk_0201DB20_p + 0x14A0 + gUnk_0201DB20_p.cursor)][gUnk_0201DB20_p.cursor];
        if (cnt <= 3)
            r7 = 0x30;
        else
            r7 = (u32)(gUnk_0201DB20_p.f1BB2 + (gUnk_0201DB20_p.f1BB0 >> 1)) >> 8;
        if (gUnk_0201DB20_p.f1C58 != 0) {
            gUnk_0201DB20_p.f1C58--;
            sub_08077EF4(gUnk_081A70F4, 0, 1, 0xD8, r7 + 0x1FD, 4, 0, 0, 0, 0, 0, (int)&gUnk_0201DB20_p);
            sub_08077EF4(gUnk_080874A0, 0, 1, 0xDB, r7 + 1, 4, 0, 0, 0, 0, 0, (int)&gUnk_0201DB20_p);
        }
    }
}
/* Read entry `col` of row `row` of card list `list` (0..2) of the list state: 0x728 bytes per row. */
u16 sub_08068D1C(u8 list, u8 row, u16 col)
{
    switch (list) {
    case 0:
        return ((struct CardList0View *)&gUnk_0201DB20_p)->cards[row][col];
    case 1:
        return ((struct CardList1View *)&gUnk_0201DB20_p)->cards[row][col];
    case 2:
        return ((struct CardList2View *)&gUnk_0201DB20_p)->cards[row][col];
    }
}
/* Write `val` into the same cell sub_08068D1C reads. */
void sub_08068DA4(u16 val, u8 list, u8 row, u16 col)
{
    switch (list) {
    case 0:
        gUnk_0201DB20_p.lists.l0[row][col] = val;
        break;
    case 1:
        gUnk_0201DB20_p.lists.l1.a[row][col] = val;
        break;
    case 2:
        gUnk_0201DB20_p.lists.l2.a[row][col] = val;
        break;
    }
}
void sub_08068E20(u8 a, u8 b, u8 c, u8 d)
{
    sub_08064E28(a, b, c, d, 0);
}
/* 1 if card a is "worth" more than card b (monster ATK * 10 in stats bits 9..17; 0 for kinds 0x15-0x17, 4000 for 0x18). */
int sub_08068E44(int a0, int b0)
{
    u16 b = b0;
    u16 a = a0;
    int va;
    int vb;
    switch (CARD_KIND(a)) {
    case 0x15:
    case 0x16:
    case 0x17:
        va = 0;
        break;
    case 0x18:
        va = 4000;
        break;
    default:
        va = ((CARD_STATS(a) << 14) >> 23) * 10;
        break;
    }
    switch (CARD_KIND(b)) {
    case 0x15:
    case 0x16:
    case 0x17:
        vb = 0;
        break;
    case 0x18:
        vb = 4000;
        break;
    default:
        vb = ((CARD_STATS(b) << 14) >> 23) * 10;
        break;
    }
    return va - vb > 0;
}
/* 1 if card a is "worth" more than card b by the low 9 stat bits * 10 (same kind rules as sub_08068E44). */
int sub_08068EFC(int a0, int b0)
{
    u16 b = b0;
    u16 a = a0;
    int va;
    int vb;
    switch (CARD_KIND(a)) {
    case 0x15:
    case 0x16:
    case 0x17:
        va = 0;
        break;
    case 0x18:
        va = 4000;
        break;
    default:
        va = (CARD_STATS(a) & 0x1FF) * 10;
        break;
    }
    switch (CARD_KIND(b)) {
    case 0x15:
    case 0x16:
    case 0x17:
        vb = 0;
        break;
    case 0x18:
        vb = 4000;
        break;
    default:
        vb = (CARD_STATS(b) & 0x1FF) * 10;
        break;
    }
    return va - vb > 0;
}
/* 1 if card a has a lower "kind" than card b. */
u32 sub_08068FBC(int a, int b)
{
    unsigned long long base = 0x08621DE0; /* FAKEMATCH (decomp-permuter): a 64-bit temp stops agbcc from CSEing the table address */
    int ka = (int)((((const u32 *)(u32)base)[a & 0x7FF] & 0x1F00000) >> 20);
    return (u32)(ka - CARD_KIND(b)) >> 31;
}

/* 1 if card a has a lower attribute (stats >> 29) than card b. */
u32 sub_08068FEC(int a, int b)
{
    unsigned long long base = 0x08621DE0; /* FAKEMATCH (decomp-permuter): see sub_08068FBC */
    return (u32)((((const u32 *)(u32)base)[a & 0x7FF] >> 29) - (CARD_STATS(b) >> 29)) >> 31;
}

/* 1 if card a has a higher level (stats >> 25 & 0xF; 10 for kind 0x18, 0 for kinds 0x15-0x17) than card b. */
int sub_08069014(int a0, int b0)
{
    u16 b = b0;
    u16 a = a0;
    int va;
    int vb;
    switch (CARD_KIND(a)) {
    case 0x15:
    case 0x16:
    case 0x17:
        va = 0;
        break;
    case 0x18:
        va = 10;
        break;
    default:
        va = (CARD_STATS(a) & 0x1E000000) >> 25;
        break;
    }
    switch (CARD_KIND(b)) {
    case 0x15:
    case 0x16:
    case 0x17:
        vb = 0;
        break;
    case 0x18:
        vb = 10;
        break;
    default:
        vb = (CARD_STATS(b) & 0x1E000000) >> 25;
        break;
    }
    return va - vb > 0;
}
struct Range { s16 lo; s16 hi; };
extern struct Range gUnk_02030000[];
/* The original swap macro has no braces, so under an unbraced `if` only `t = a` is conditional.
   The median-of-three below relies on that (the pivot is not a true median). */
#define SORT_SWAP(a, b) t = a; a = b; b = t
/* Quicksort of n s16 values in arr (insertion sort for ranges of <= 20); cmp(a, b) != 0 means a orders before b.
   The pending (lo, hi) ranges live in an explicit stack at 0x02030000. */
void sub_080690C4(int n, s16 *arr, u16 (*cmp)(s16, s16))
{
    struct Range *stack = gUnk_02030000;
    s16 lo, hi, i, j, x;
    s16 top;
    u16 t;

    stack[0].lo = 0;
    stack[0].hi = n - 1;
    top = 1;
    do {
        top--;
        lo = stack[top].lo;
        hi = stack[top].hi;
        if (lo < hi) {
            if (hi - lo > 20) {
                s16 y, z;
                i = lo - 1;
                j = hi + 1;
                x = arr[(lo + hi) / 2];
                y = arr[lo];
                z = arr[hi];
                if (x < y) SORT_SWAP(x, y);
                if (x > z) {
                    SORT_SWAP(x, z);
                    if (x > y) SORT_SWAP(x, y);
                }
                for (;;) {
                    while (cmp(arr[++i], x))
                        ;
                    while (cmp(x, arr[--j]))
                        ;
                    if (i >= j)
                        break;
                    SORT_SWAP(arr[i], arr[j]);
                }
                stack[top].lo = j + 1;
                stack[top].hi = hi;
                top++;
                stack[top].lo = lo;
                stack[top].hi = i - 1;
                top++;
            } else {
                for (i = lo + 1; i <= hi; i++) {
                    x = arr[i];
                    j = i - 1;
                    while (j >= lo && cmp(x, arr[j])) {
                        arr[j + 1] = arr[j];
                        j--;
                    }
                    arr[j + 1] = x;
                }
            }
        }
    } while (top > 0);
}
