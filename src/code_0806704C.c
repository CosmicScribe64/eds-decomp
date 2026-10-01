#include "global.h"
#include "gba.h"

/* Deck-edit scene state at 0x0201DB20 (see code_08064AF0 / code_08065E6C / code_08068180). Several views of the same
   symbol are declared with asm() names so that each function sees only the fields it uses. */
struct Cell {               /* 20-byte cells at +0x1726 */
    u8 b0;
    u8 b1;
    u8 pad2[18];
};
struct StCells {
    u8 pad0[0x1726];
    struct Cell cell[1];    /* +0x1726, 20 bytes each */
};
extern struct StCells gUnk_0201DB20_cells asm("gUnk_0201DB20");
struct StBits {             /* window / kind bits */
    u8 pad0[0x1C3D];
    u8 lo3 : 3;             /* +0x1C3D bits 0-2 */
    u8 b3 : 2;
    u8 win : 2;             /* +0x1C3D bits 5-6 */
    u8 b7 : 1;
};
extern struct StBits gUnk_0201DB20_bits asm("gUnk_0201DB20");
/* Slide / animation record (hypothesis), see sub_08067660: the bits below are u32 bitfields of the first word. */
struct Slide {
    u32 ctr : 8;            /* +0 */
    u32 dir : 3;            /* +1 bits 0-2: 1 open, 2 close */
    u32 prev : 2;           /* +1 bits 3-4: previous phase */
    u32 phase : 2;          /* +1 bits 5-6: 0..2 */
    u32 sel : 3;            /* bits 15-17 */
    u32 rest : 6;
    u8 arr3[3];             /* +3: per-cursor card index */
    u8 arr6[3];             /* +6 */
};
struct SlideA {
    u8 ctr;
    u8 dir : 3;
    u8 prev : 2;
    u8 phase : 2;
    u8 b7 : 1;
};
#define PA(p) ((struct SlideA *)(p))
extern void sub_0807AA4C(const void *src, u32 dst, int w, int h, int a, int b, int c);
extern const u8 gUnk_086F1B10[][12];
struct St3C {
    u8 pad0[0x1C3C];
    union {
        struct {
            u32 lo15 : 15;
            u32 sel : 3;        /* bits 15-17: selected panel */
            u32 hi : 14;
        } w;
        struct {
            u8 b0;
            u8 lo3 : 3;         /* +0x1C3D bits 0-2 */
            u8 b3 : 2;
            u8 win : 2;
            u8 b7 : 1;
        } b;
    } u;
    u8 pad1C40[0x1C5A - 0x1C40];
    u8 mode : 2;                /* +0x1C5A bits 0-1 */
    u8 rest : 6;
};
extern struct St3C gUnk_0201DB20_w asm("gUnk_0201DB20");
struct MainKeys { u8 u0[6]; u16 keys; };
extern struct MainKeys gUnk_03000040;
extern const u8 gUnk_086EF3B0[][0x400];
extern const u8 gUnk_086F0FB0[];
extern const u8 gUnk_086F13B0[];
struct StF48 {
    u8 pad0[0x1C48];
    u8 b0 : 1;
    u8 rest : 7;
};
extern struct StF48 gUnk_0201DB20_f48 asm("gUnk_0201DB20");
struct StP {
    u8 pad0[0x1494];
    u16 cnt1494[2][3];          /* +0x1494 */
    u8 pad149A[0x1712 - 0x149A - 6];
    u16 f1712[2];               /* +0x1712 */
    u16 f1714;
};
extern struct StP gUnk_0201DB20_p asm("gUnk_0201DB20");
extern u16 sub_08068D1C(u8 list, u8 row, u16 col);
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_KIND(id) ((int)((CARD_STATS(id) & 0x1F00000) >> 20))
struct Cnt9 { u8 pad9[9]; u8 lo4 : 4; u8 f4 : 2; u8 hi : 2; };
#define TRUNK ((u8 *)0x02011C20)
#define TRUNK_F4(t, id) (((struct Cnt9 *)((u16)(id) * 4 + (t)))->f4)
struct DeckSlot { u8 b[16]; };
struct DeckPage {
    u8 pad0[0x620];
    u16 arr620[3];              /* +0x620 */
    u8 pad626[2];
    u8 f628[8];                 /* +0x628 (address passed on) */
    u16 f630;
    u16 f632;
    u8 f634;
    u8 f635;
    u8 pad636[0x1494 - 0x636];
    u16 cnt1494[2][3];          /* +0x1494 */
    u8 arr14A0[0x1710 - 0x14A0];
    u8 f1710;
    u8 pad1711[0x18AC - 0x1711];
    u16 f18AC;
    u8 pad18AE[2];
    u8 f18B0[0x1BB0 - 0x18B0];  /* +0x18B0 (address passed on) */
    u16 f1BB0;
    u8 pad1BB2[2];
    u8 f1BB4;
    u8 pad1BB5[3];
    struct DeckSlot slot[5];    /* +0x1BB8 */
    u8 pad1C08[0x1C14 - 0x1C08];
    u8 f1C14;
    u8 pad1C15[7];
    u8 cursor;                  /* +0x1C1C */
    u8 pad1C1D[0x1C58 - 0x1C1D];
    u32 lo1C58 : 16;
    u32 mode : 2;               /* +0x1C5A bits 0-1 */
    u32 rest1C5A : 14;
};
extern struct DeckPage gUnk_0201DB20_d asm("gUnk_0201DB20");
#define D gUnk_0201DB20_d
extern void sub_0807B100(int a, int b, int c, void *d);
extern void sub_0807AFFC(u16 id, u32 dst);
extern void sub_08064E28(u8 col, u8 row, u8 set, u8 wrap, u32 base);
extern void sub_08065F34(u32 a, u32 b, void *out);
extern void sub_0806664C(u8 slot, u16 id, u8 kind, void *slots, void *out);
extern const u8 gUnk_086F17B0[][60];
extern const u8 gUnk_086F17E8[][60];
extern const u8 gUnk_086F17D4[][120];
extern const u8 gUnk_086F17E0[][120];
extern const u8 gUnk_086E26D0[][60];
extern const u8 gUnk_081A6EAC[];
extern const u8 gUnk_081A6D8C[][40];
extern void sub_0807A9C0(const void *src, u32 dst, int a, int b, int c, int d, int e);
extern u16 *sub_08077EF4(const void *a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l);
extern void sub_08067540(void);
struct MainCb {
    u8 pad0[0x414];
    void (*cb)(void);       /* +0x414 */
};
extern struct MainCb gUnk_03000040_cb asm("gUnk_03000040");
extern void sub_08077AEC(u16 id);
extern void sub_080671E8(u8 a);
extern void sub_080679A8(u8 x);

/* Card-list scroll state (sub_080679E0 / sub_08067DA4) */
struct ScrollSt {
    u8 pad0[0x628];
    u8 f628[8];                  /* +0x628 window rect passed to sub_0807B100 */
    u16 f630;                    /* +0x630 */
    u16 f632;                    /* +0x632 */
    u8 f634;                     /* +0x634 VRAM page toggle */
    u8 f635;                     /* +0x635 slide direction code */
    u8 pad636[0x63A - 0x636];
    u16 f63A;                    /* +0x63A */
    u8 pad63C[0x63E - 0x63C];
    u16 f63E;                    /* +0x63E */
    u8 f640[0x1494 - 0x640];     /* +0x640 map scratch */
    u16 cnt1494[2][3];           /* +0x1494 counter [row][cursor] */
    u8 arr14A0[0x18AC - 0x14A0];
    u16 f18AC;                   /* +0x18AC */
    u8 f18B0[0x1BB0 - 0x18B0];   /* +0x18B0 out buffer */
    u16 f1BB0;                   /* +0x1BB0 slide extents */
    u8 pad1BB2[2];
    u8 f1BB4;                    /* +0x1BB4 dirty flag */
    u8 f1BB5;                    /* +0x1BB5 */
    u8 f1BB6;                    /* +0x1BB6 */
    u8 f1BB7;                    /* +0x1BB7 */
    u8 f1BB8[0x1C1C - 0x1BB8];   /* +0x1BB8 5 slots */
    u8 cursor;                   /* +0x1C1C */
    u8 pad1C1D[0x1C58 - 0x1C1D];
    u16 f1C58;                   /* +0x1C58 scroll counter */
};
extern struct ScrollSt gUnk_0201DB20_s asm("gUnk_0201DB20");
extern u16 gUnk_0201E140[];
extern u8 gUnk_0201EFC0[];
extern u16 gUnk_0201EFB4[];
extern u8 gUnk_0201E160[];
extern u8 gUnk_0201F73C;
extern s16 gUnk_08087494[];
extern s16 gUnk_0808749C[];
extern void sub_08079834(s32, s32, s32, s32, s32, s32, u8 *);
extern void sub_0806518C(u16, s32, s32, u32, u8 *);
extern void sub_08065108(u16, s32, s32, u32, u8 *, s32);
extern void sub_08065AB4(s32, s32, u32, u8 *);
extern void sub_08065E6C(s32, s32, u32, s32);
extern void sub_080657F8(s32);
extern void sub_08065058(s32);
extern void sub_08065384(u16, s32, s32, u32, u8 *);
extern void sub_08066478(u8, u16, u16, u8 *, u8 *);

#if 0 /* NONMATCHING: identical except that the target loads the literal 0x1727
       * for the byte-1 stores (r2), while gcc derives it from 0x1726
       * (`adds r2,#1`). Tried separate typed views, u8 views and pointer
       * locals. */
/* Move the selection from panel *b to panel *a: marks the old cells (`0xFF`) and the new ones (b0 = 1, b1 = 0), then
   sets the animation bytes `s[idx * 20 + 0xE]` (hypothesis). */
void sub_0806704C(u8 *a, u8 *b, u8 *s)
{
    if (*a != *b) {
        gUnk_0201DB20_cells.cell[*a + 0xD].b0 = 1;
        gUnk_0201DB20_cells.cell[*a + 0xD].b1 = 0;
        gUnk_0201DB20_cells.cell[*a + 6].b0 = 1;
        gUnk_0201DB20_cells.cell[*a + 6].b1 = 0;
        gUnk_0201DB20_cells.cell[*b + 0xD].b0 |= 0xFF;
        gUnk_0201DB20_cells.cell[*b + 6].b0 |= 0xFF;
        *b = *a;
    }
    s[(*a + 0xD) * 0xE + 20] = 0xFF;
    s[(*a + 6) * 20 + 0xE] = 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806704C", sub_0806704C); /* 0x0806704C size 0xC0 */
#if 0 /* NONMATCHING: identical instructions, register allocation only. The
       * target keeps the base `&state+0x1712` in r4 and j in r5 (built
       * swapped), and loads the trunk literal `0x02011C20` before the
       * `lsl 16; lsr 14` of the id (built after). */
/* Count, for both decks' pages, the card copies of the non-Trap/Magic cards of list 2 (2-bit field at bits 4-5 of the
   trunk byte +9) into f1712[row]; the first sum is also stored at +0x1714 (hypothesis). */
void sub_0806710C(void)
{
    u16 i;
    for (i = 0; i <= 1; i++) {
        u16 j;
        gUnk_0201DB20_p.f1712[i] = 0;
        for (j = 0; j < gUnk_0201DB20_p.cnt1494[i][2]; j++) {
            s16 id = sub_08068D1C(2, i, j);
            switch (CARD_KIND(id)) {
            case 0x15:
            case 0x16:
                break;
            default:
                id = sub_08068D1C(2, i, j);
                gUnk_0201DB20_p.f1712[i] += TRUNK_F4(TRUNK, id);
                break;
            }
        }
    }
    gUnk_0201DB20_p.f1714 = gUnk_0201DB20_p.f1712[0];
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806704C", sub_0806710C); /* 0x0806710C size 0xDC */
#if 0 /* NONMATCHING: same instructions and control flow; register allocation of two temporaries (f634 byte in r2 / r4 in the target, r3 / r7+copy in built: built CSEs `D.f634` into a register kept across the fill branch, the target reloads it) and the literal 0x635 (target loads it, built derives it with `adds r7,#3` from 0x632) */
/* Redraw the card panel after a page change: `a` = 2 / 3 is the direction. Sets up the window scroll, draws the current
   card's graphics (or clears them), places the page frame, then re-initialises the 5 card slots around the cursor. */
void sub_080671E8(u8 a)
{
    u16 j;
    u8 k;
    D.f18AC = 0xFC00;
    if (a == 2)
        sub_0807B100(0, 6, 1, D.f628);
    else
        sub_0807B100(6, 0, -1, D.f628);
    D.f634 ^= 1;
    if (D.cnt1494[D.arr14A0[D.cursor]][D.cursor] != 0) {
        sub_0807AFFC(sub_08068D1C(D.cursor, D.arr14A0[D.cursor], D.arr620[D.cursor]), D.f634 * 0x1680 + 0x06008000);
    } else {
        u16 zero = 0;
        CpuFastSet(&zero, (void *)(D.f634 * 0x1680 + 0x06008000), 0x010005A0);
    }
    if (a == 3) {
        sub_08064E28(((D.f630 & 0xFF) >> 3) + 9, ((D.f632 & 0xFF) >> 3) + 2, D.f634, 1, 0);
        D.f630 -= 0x50;
        D.f635 = 4;
    } else {
        sub_08064E28(((D.f630 & 0xFF) >> 3) + 0x1D, ((D.f632 & 0xFF) >> 3) + 2, D.f634, 1, 0);
        D.f635 = 3;
    }
    D.f1710 |= 1;
    sub_08065F34(D.cnt1494[D.arr14A0[D.cursor]][D.cursor], D.arr620[D.cursor], &D.f1BB0);
    j = D.arr620[D.cursor] - 2;
    for (k = 0; k <= 4; j++, k++) {
        if ((s16)j >= 0 && j < D.cnt1494[D.arr14A0[D.cursor]][D.cursor])
            sub_0806664C(k, sub_08068D1C(D.cursor, D.arr14A0[D.cursor], j), k + 1, &D.slot[0], D.f18B0);
        else
            D.slot[k].b[0xC] = 0;
    }
    D.f1C14 = 0;
    D.slot[0].b[0] = 5;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806704C", sub_080671E8); /* 0x080671E8 size 0x28C */
/* Key callback of the 3-panel selector: R (0x100) selects the next panel, L (0x200) the previous one (wrapping). */
void sub_08067474(u8 *idx)
{
    if (gUnk_03000040.keys & 0x100) {
        struct St3C *st;
        (*idx)++;
        if (*idx == 3)
            *idx = 0;
        st = &gUnk_0201DB20_w;
        st->u.w.sel = 0;
        st->u.b.lo3 = 3;
        sub_080671E8(2);
        sub_080679A8(*idx);
        sub_08077AEC(0);
    } else if (gUnk_03000040.keys & 0x200) {
        struct St3C *st;
        if (*idx != 0)
            *idx = *idx - 1;
        else
            *idx = 2;
        st = &gUnk_0201DB20_w;
        st->u.w.sel = 0;
        st->u.b.lo3 = 3;
        sub_080671E8(3);
        sub_080679A8(*idx);
        sub_08077AEC(0);
    }
}
#if 0 /* NONMATCHING: same control flow; target keeps the default path's extraction separate from case 1's (different register for the table literal: r2 vs r1) so they are not cross-jumped together; built merges them (-0x14 bytes) */
/* Load the 2 x 0x400-byte tile blocks of the selected panel (hypothesis) into OBJ VRAM 0x06010800 and set the window. */
void sub_08067540(void)
{
    struct St3C *st;
    const u8 *src;
    u8 *dst = (u8 *)0x06010800;
    s8 i;
    st = &gUnk_0201DB20_w;
    switch (st->mode) {
    default:
        src = gUnk_086EF3B0[st->u.w.sel];
        break;
    case 1:
        switch (st->u.w.sel) {
        case 2:
        case 3:
            src = gUnk_086F0FB0;
            break;
        default:
            src = gUnk_086EF3B0[gUnk_0201DB20_w.u.w.sel];
            break;
        }
        break;
    case 2:
        if (st->u.w.sel == 6) {
            src = gUnk_086F13B0;
            break;
        }
        src = gUnk_086EF3B0[st->u.w.sel];
        break;
    }
    for (i = 0; i <= 1; i++) {
        CpuFastSet(src, dst, 0x80);
        dst += 0x400;
        src += 0x200;
    }
    *(u16 *)0x04000042 = 0xF0;
    *(u16 *)0x04000046 = (gUnk_0201DB20_w.u.b.win << 11) | 0x70;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806704C", sub_08067540); /* 0x08067540 size 0xF0 */
/* Set the WIN1 window registers for the deck-edit card view (hypothesis). */
void sub_08067630(void)
{
    *(u16 *)0x04000042 = 0xF0;
    *(u16 *)0x04000046 = (gUnk_0201DB20_bits.win << 11) | 0x70;
}
#if 0 /* NONMATCHING: structure, constants and call sequence match; the ~300-line diff is register allocation of the hoisted constants (target: state base in r8 loaded into r0 first, base+2 in r9 scheduled after the `f & 0x60` test, 7 in sl) and the operand order of `(2 - phase) * 60 + table[...]` (target computes the phase term first) */
/* Draw one frame of the slide animation of the panel record `p`: tiles of the frame, card and arrow pieces for the current
   phase (sub_0807AA4C), the two sprite pairs (sub_08077EF4), and selects the HBlank/VBlank callback. */
void sub_08067660(struct Slide *p)
{
    s16 base;
    int b2;
    u16 off;
    base = 0;
    if (D.mode == 2)
        base = 4;
    if (PA(p)->dir != 0) {
        b2 = base + 2;
        if (PA(p)->phase != 0) {
            sub_0807AA4C(gUnk_086F17B0[b2 - p->phase], 0x0600E000, 0x12, p->phase, 0x1E, 3, 2);
            sub_0807AA4C(gUnk_086F17E8[2 - p->phase], 0x0600E038, 1, p->phase, 0x1E, 3, 2);
        }
        off = (2 - p->phase) * 60;
        sub_0807AA4C(off + gUnk_086F17D4[p->arr3[D.cursor]], 0x0600E024, 6, p->phase, 0x1E, 3, 2);
        off = (2 - p->phase) * 60;
        sub_0807AA4C(off + gUnk_086F17E0[p->arr6[D.cursor]], 0x0600E030, 4, p->phase, 0x1E, 3, 2);
        off = (b2 - p->phase) * 60;
        sub_0807AA4C((const void *)(off + (u32)&((const u16 *)gUnk_086F17B0)[p->sel * 2 + 0x40]), p->sel * 4 + 8 + 0x0600E000, 2, p->phase, 0x1E, 3, 2);
        if ((u8)p->phase <= 1)
            sub_0807A9C0(gUnk_086E26D0[p->phase], 0x0600E000 + (p->phase << 6), 0x1E, 2 - p->phase, 0x1E, 0, 0);
        p->prev = p->phase;
        if (PA(p)->dir == 3 || PA(p)->dir == 1) {
            gUnk_03000040_cb.cb = sub_08067540;
            if (PA(p)->dir == 3)
                p->dir = 0;
        } else {
            gUnk_03000040_cb.cb = sub_08067630;
        }
        D.f1BB4 |= 1;
    } else {
        gUnk_03000040_cb.cb = 0;
    }
    sub_08077EF4(gUnk_081A6EAC, 0, 1, 0, (-((2 - p->phase) * 8)) & 0xFF, 1, 0, 1, 0, 0, 0, (int)&D);
    if (PA(p)->phase == 2)
        sub_08077EF4(gUnk_081A6D8C[p->sel], 0, 5, 0, -1, 0, 0, 0, 0, 0, 0, (int)&D);
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806704C", sub_08067660); /* 0x08067660 size 0x2A8 */
void sub_08067908(struct Slide *p)
{
    if (PA(p)->dir != 0) {
        p->ctr--;
        if (p->ctr == 0xFF) {
            p->ctr = 0;
            switch (p->dir) {
            case 1:
                if (PA(p)->phase != 2) {
                    p->phase = p->phase + 2;
                } else {
                    p->dir = 0;
                    gUnk_0201DB20_f48.b0 = 1;
                }
                break;
            case 2:
                if (PA(p)->phase != 0) {
                    p->phase = p->phase - 2;
                } else {
                    p->dir = 0;
                    gUnk_0201DB20_f48.b0 = 0;
                }
                break;
            }
        }
    }
}
/* Copy a 6x6 block of tiles of the card-info panel number `x` into the map at 0x0600E3B0 (hypothesis). */
void sub_080679A8(u8 x)
{
    sub_0807AA4C(gUnk_086F1B10[x], 0x0600E3B0, 6, 6, 0x1E, 3, 2);
}
#if 0 /* NONMATCHING: structure and all calls match; 0x18 bytes short. First diff +0xA: target frame is `sub sp,#20` and keeps the first table value in [sp,#16], built allocates 24 and spills the second table value too; the target also keeps `&state+0x1C1C` in sl, built uses r7 + separate offset literal. Register allocation only. */
/* Scroll the card list up by one row (cursor column `st->cursor`): two tilemap scrolls, redraw the
   card entering at the top, shift the item counters, then refresh the side panel. `*out` gets the
   card id that left, or 0xFFFF. */
void sub_080679E0(u16 *out)
{
    struct ScrollSt *st = &gUnk_0201DB20_s;
    s16 t;

    if (gUnk_0201E140[st->cursor] == 0)
        return;
    st->f1C58 = 0x1E;
    st->f18AC = 0xFC00;
    sub_0807B100(6, 0, -1, st->f628);
    gUnk_0201E140[st->cursor]--;
    st->f634 ^= 1;
    t = gUnk_0808749C[0];
    sub_08079834(0, 0x0600C000, 0, ((st->f63E + t) & 0xFF) >> 3, 0x1E, 5, gUnk_0201E160);
    if (gUnk_0201E140[st->cursor] < st->cnt1494[gUnk_0201EFC0[st->cursor]][st->cursor]) {
        sub_0806518C(sub_08068D1C(st->cursor, gUnk_0201EFC0[st->cursor], gUnk_0201E140[st->cursor]),
                     0x0600C000, 0, ((st->f63E + t) & 0xFF) >> 3, gUnk_0201E160);
        sub_0807AFFC(sub_08068D1C(st->cursor, gUnk_0201EFC0[st->cursor], gUnk_0201E140[st->cursor]),
                     st->f634 * 0x1680 + 0x06008000);
        sub_08064E28(((st->f630 & 0xFF) >> 3) + 0x13, (u8)(((st->f632 & 0xFF) >> 3) - 8), st->f634, 1, 0);
    }
    st->f63E -= 0x28;
    st->f632 -= 0x50;
    st->f635 = 1;
    t = gUnk_08087494[0];
    sub_08079834(0, 0x0600D000, 3, ((st->f63A + t) & 0xFF) >> 3, 0x1B, 2, gUnk_0201E160);
    if ((s16)gUnk_0201E140[st->cursor] - 2 >= 0) {
        sub_08065108(sub_08068D1C(st->cursor, gUnk_0201EFC0[st->cursor], (u16)(gUnk_0201E140[st->cursor] - 2)),
                     0x0600D000, 0, ((st->f63A + t) & 0xFF) >> 3, gUnk_0201E160, 0);
        *out = sub_08068D1C(st->cursor, gUnk_0201EFC0[st->cursor], (u16)(gUnk_0201E140[st->cursor] - 2));
    } else {
        *out = 0xFFFF;
    }
    if (gUnk_0201E140[gUnk_0201F73C] + 1 < st->cnt1494[gUnk_0201EFC0[gUnk_0201F73C]][gUnk_0201F73C])
        sub_08065108(sub_08068D1C(gUnk_0201F73C, gUnk_0201EFC0[gUnk_0201F73C], (u16)(gUnk_0201E140[gUnk_0201F73C] + 1)),
                     0x0600D000, 0, ((st->f63A + t) & 0xFF) >> 3, st->f640, 3);
    sub_08065AB4(0x0600C000, 0xB, ((st->f63E + 0x38) & 0xFF) >> 3, st->f640);
    sub_08065E6C(0x0600C000, 0x11, ((st->f63E + 0x38) & 0xFF) >> 3, 6);
    st->f63A -= 0x10;
    sub_080657F8(0);
    sub_08065F34(st->cnt1494[gUnk_0201EFC0[gUnk_0201F73C]][gUnk_0201F73C],
                 gUnk_0201E140[gUnk_0201F73C], &st->f1BB0);
    if (st->f1BB5 != 0) {
        st->f1BB5 = 2;
        st->f1BB4 |= 1;
    }
    sub_08066478(st->f635, *out, st->cnt1494[gUnk_0201EFC0[gUnk_0201F73C]][gUnk_0201F73C], st->f1BB8, st->f18B0);
    sub_08065058(1);
    sub_08077AEC(0);
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806704C", sub_080679E0); /* 0x080679E0 size 0x3C4 */
#if 0 /* NONMATCHING: only register allocation/spills; built is 8 bytes short. Target frame `sub sp,#20` spills the 0xFFFF constant to [sp,#16] and keeps `base+0x14A0` in sl; built frame `sub sp,#16`, uses a pool constant and absolute 0x0201E140/0x0201EFC0 literals. Calls and control flow identical (23 bl either side). */
/* Mirror of sub_080679E0: scroll the card list down by one row. */
void sub_08067DA4(u16 *out)
{
    struct ScrollSt *st = &gUnk_0201DB20_s;
    s16 t;

    if (gUnk_0201E140[gUnk_0201F73C] >= gUnk_0201EFB4[st->arr14A0[gUnk_0201F73C] * 3 + gUnk_0201F73C] - 1)
        return;
    st->f1C58 = 0x1E;
    st->f18AC = 0xFC00;
    sub_0807B100(0, 6, 1, st->f628);
    st->f634 ^= 1;
    sub_0807AFFC(sub_08068D1C(gUnk_0201F73C, st->arr14A0[gUnk_0201F73C], gUnk_0201E140[gUnk_0201F73C]), st->f634 * 0x1680 + 0x06008000);
    gUnk_0201E140[gUnk_0201F73C]++;
    sub_08064E28(((st->f630 & 0xFF) >> 3) + 0x13, ((st->f632 & 0xFF) >> 3) + 0xC, st->f634, 1, 0);
    st->f635 = 2;
    t = gUnk_08087494[2];
    sub_08079834(0, 0x0600D000, 3, ((st->f63A + t) & 0xFF) >> 3, 0x1B, 3, st->f640);
    if (gUnk_0201E140[gUnk_0201F73C] + 2 < gUnk_0201EFB4[st->arr14A0[gUnk_0201F73C] * 3 + gUnk_0201F73C]) {
        sub_08065108(sub_08068D1C(gUnk_0201F73C, st->arr14A0[gUnk_0201F73C], (u16)(gUnk_0201E140[gUnk_0201F73C] + 2)),
                     0x0600D000, 0, ((st->f63A + t) & 0xFF) >> 3, st->f640, 6);
        *out = sub_08068D1C(gUnk_0201F73C, st->arr14A0[gUnk_0201F73C], (u16)(gUnk_0201E140[gUnk_0201F73C] + 2));
    } else {
        *out = 0xFFFF;
    }
    sub_08079834(0, 0x0600D000, 3, ((st->f63A + t) & 0xFF) >> 3, 0x1B, 3, st->f640);
    t = gUnk_08087494[3];
    if ((s16)gUnk_0201E140[gUnk_0201F73C] - 1 >= 0)
        sub_08065108(sub_08068D1C(gUnk_0201F73C, st->arr14A0[gUnk_0201F73C], (u16)(gUnk_0201E140[gUnk_0201F73C] - 1)),
                     0x0600D000, 0, ((st->f63A + t) & 0xFF) >> 3, st->f640, 3);
    t = gUnk_0808749C[1];
    sub_08079834(0, 0x0600C000, 0, ((t + st->f63E) & 0xFF) >> 3, 0x1E, 6, st->f640);
    if (gUnk_0201EFB4[st->arr14A0[gUnk_0201F73C] * 3 + gUnk_0201F73C] != 0) {
        sub_0806518C(sub_08068D1C(gUnk_0201F73C, st->arr14A0[gUnk_0201F73C], gUnk_0201E140[gUnk_0201F73C]),
                     0x0600C000, 0, ((st->f63E + t) & 0xFF) >> 3, st->f640);
        sub_08065AB4(0x0600C000, 0xB, ((st->f63E + 0x60) & 0xFF) >> 3, st->f640);
        sub_08065E6C(0x0600C000, 0x11, ((st->f63E + 0x60) & 0xFF) >> 3, 6);
        sub_080657F8(5);
        sub_08065F34(gUnk_0201EFB4[st->arr14A0[gUnk_0201F73C] * 3 + gUnk_0201F73C], gUnk_0201E140[gUnk_0201F73C], &st->f1BB0);
        if (st->f1BB7 != 0) {
            st->f1BB7 = 2;
            st->f1BB6 |= 1;
        }
        sub_08066478(st->f635, *out, gUnk_0201EFB4[st->arr14A0[gUnk_0201F73C] * 3 + gUnk_0201F73C], st->f1BB8, st->f18B0);
    } else {
        sub_08065384(sub_08068D1C(gUnk_0201F73C, st->arr14A0[gUnk_0201F73C], gUnk_0201E140[gUnk_0201F73C]),
                     0x0600C000, 0, ((st->f63E + t) & 0xFF) >> 3, st->f640);
    }
    sub_08065058(2);
    sub_08077AEC(0);
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0806704C", sub_08067DA4); /* 0x08067DA4 size 0x3DC */
