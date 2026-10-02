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

/* Move the selection from panel *b to panel *a: marks the old cells (`0xFF`) and the new ones (b0 = 1, b1 = 0), then
   sets the animation bytes `s[idx * 20 + 0xE]` (hypothesis). Structs are 4-aligned under agbcc, so the 20-byte cell
   array starts at +0x1724 with the flag bytes at +2/+3. */
struct Cell_x {
    u8 pad0[2];
    u8 b0;                  /* +2 */
    u8 b1;                  /* +3 */
    u8 pad4[10];
    u8 fE;                  /* +0xE */
    u8 padF[5];
};
struct StCells_x {
    u8 pad0[0x1724];
    struct Cell_x cell[1];  /* +0x1724, 20 bytes each */
};
#define CX_0806704C (*(struct StCells_x *)&gUnk_0201DB20_cells)
struct CellArr_x { struct Cell_x cell[1]; };
void sub_0806704C(u8 *a, u8 *b, struct CellArr_x *s)
{
    if (*a != *b) {
        CX_0806704C.cell[*a + 0xD].b0 = 1;
        CX_0806704C.cell[*a + 0xD].b1 = 0;
        CX_0806704C.cell[*a + 6].b0 = 1;
        CX_0806704C.cell[*a + 6].b1 = 0;
        CX_0806704C.cell[*b + 0xD].b0 |= 0xFF;
        CX_0806704C.cell[*b + 6].b0 |= 0xFF;
        *b = *a;
    }
    s->cell[*a + 0xD].fE = 0xFF;
    s->cell[*a + 6].fE = 1;
}
/* Save image 0x02011C20: 4-byte trunk entries from +0x08, indexed by card ID; f4 = bits 4-5 of byte +1 (copies in
   the deck, hypothesis). Indexing the symbol as a struct array keeps the base load after the call (a constant or a
   (u8 *) cast is folded into the add; a hoisted pointer moves to sl). */
struct TrEnt_x { u8 b0; u8 lo4 : 4; u8 f4 : 2; u8 hi : 2; u16 w2; };
struct Save_x { u8 pad0[8]; struct TrEnt_x trunk[0x800]; };
extern struct Save_x gUnk_02011C20;
/* For both rows: f1712[row] = sum of trunk f4 over the non-Trap/Magic cards of list 2 (cnt1494[row][2] cards via
   sub_08068D1C(2, row, j)); then f1712[1] = f1712[0] (the store is at +0x1714). */
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
                gUnk_0201DB20_p.f1712[i] += gUnk_02011C20.trunk[(u16)id].f4;
                break;
            }
        }
    }
    gUnk_0201DB20_p.f1712[1] = gUnk_0201DB20_p.f1712[0];
}
/* Redraw the card panel after a page change: `a` = 2 / 3 is the direction. Sets up the window scroll, draws the current
   card's graphics (or clears them), places the page frame, then re-initialises the 5 card slots around the cursor. */
/* sub_0807AFFC really takes a third argument, the VRAM page (see its definition in code_0807A6AC); passing D.f634
   there puts the page byte in r2 and the `*3` temporary in r3. The unit-wide prototype has two parameters, so this
   call goes through a cast to the three-parameter type. */
typedef void (*Fn0807AFFC_x)(u16 id, u32 dst, u16 page);
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
        ((Fn0807AFFC_x)sub_0807AFFC)(sub_08068D1C(D.cursor, D.arr14A0[D.cursor], D.arr620[D.cursor]), D.f634 * 0x1680 + 0x06008000, D.f634);
    } else {
        u32 zero = 0;
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
#define TBL_8067540 ((const u8 *)0x086EF3B0) /* 0x400-byte tile blocks; [7] and [8] are the special panels */
/* The same word as St3C.u.w seen as one 18-bit field: the panel number is its top 3 bits (15-17). */
struct St3C_18 {
    u8 pad0[0x1C3C];
    u32 lo18 : 18;
    u32 hi : 14;
};
/* Load the 2 x 0x400-byte tile blocks of the selected panel (hypothesis) into OBJ VRAM 0x06010800 and set the window. */
void sub_08067540(void)
{
    struct St3C *st;
    const u8 *src;
    u8 *dst = (u8 *)0x06010800;
    u8 i;
    st = &gUnk_0201DB20_w;
    switch (st->mode) {
    default:
        src = TBL_8067540 + st->u.w.sel * 0x400;
        break;
    case 1:
        switch (st->u.w.sel) {
        case 2:
        case 3:
            src = gUnk_086F0FB0;
            break;
        default:
            src = TBL_8067540 + gUnk_0201DB20_w.u.w.sel * 0x400;
            break;
        }
        break;
    case 2:
        /* the panel compare goes through the 18-bit view (lsr #29 of the shared lsl #14); the index rereads sel */
        if ((((struct St3C_18 *)st)->lo18 >> 15) == 6) {
            src = gUnk_086F13B0;
            break;
        }
        src = TBL_8067540 + st->u.w.sel * 0x400;
        break;
    }
    for (i = 0; i <= 1; i++) {
        CpuFastSet(src, dst, 0x80);
        src += 0x200;
        dst += 0x400;
    }
    *(u16 *)0x04000042 = 0xF0;
    *(u16 *)0x04000046 = (gUnk_0201DB20_w.u.b.win << 11) | 0x70;
}
/* Set the WIN1 window registers for the deck-edit card view (hypothesis). */
void sub_08067630(void)
{
    *(u16 *)0x04000042 = 0xF0;
    *(u16 *)0x04000046 = (gUnk_0201DB20_bits.win << 11) | 0x70;
}
/* The real prototype (code_0807A6AC.c) takes u8 sizes: calling through it narrows the height argument, which is why
   each call re-extracts p->phase from the shared shift. sub_0807A9C0 keeps the unit's int prototype (no narrowing). */
typedef void (*TileCopyFn_8067660)(const void *src, u32 dst, u8 w, u8 h, u8 srcW, u8 pal, u8 hi);
#define TILECOPY_8067660 ((TileCopyFn_8067660)sub_0807AA4C)
/* Wider views of the first word of the record, used for the second extraction of a field that shares its shift.
   FAKEMATCH: sel read as (bits 0-17) >> 15 and phase as (bits 11-14) >> 2 keep CSE from reusing the first
   extraction. The 4-bit view is padded to 12 bytes so it is BLKmode and read with ldrb like struct Slide. */
struct Slide18_8067660 { u32 lo18 : 18; u32 hi : 14; };
struct SlideP4_8067660 { u32 ctr : 8; u32 lo3 : 3; u32 pp : 4; u32 hi : 17; u8 pad[8]; };
#define SEL18_8067660(p) (((struct Slide18_8067660 *)(p))->lo18 >> 15)
#define PHASE4_8067660(p) (((struct SlideP4_8067660 *)(p))->pp >> 2)
/* Tile maps in ROM, 30 tiles (60 bytes) per row, addressed as integers: the add's constant is then reloaded at each
   use (ldr into the next round-robin reload register) instead of living in a pseudo. */
#define TILEMAP_8067660(addr) ((const u8 (*)[60])(addr))
/* Draw one frame of the slide animation of the panel record `p` (the word at 0x0201DB20+0x1C3C): the frame, card and
   arrow pieces for the current phase (sub_0807AA4C), the phase<=1 extra rows (sub_0807A9C0), selects the HBlank/VBlank
   callback, then places the two sprite pairs (sub_08077EF4). */
void sub_08067660(struct Slide *p)
{
    int base;
    int b2;
    int b3;
    int i1, i2, i3, i4, i5;
    if (D.mode == 2)
        base = 4;
    else
        base = 0;
    if (PA(p)->dir != 0) {
        /* FAKEMATCH: each temp receives the extracted phase and is then overwritten, so the height argument
           re-extracts it from the shared shift (lsl #25 / two lsr #30). base + 2 is written twice (b2, b3) so
           that PRE computes it once, just before the phase test. */
        if (PA(p)->phase != 0) {
            b2 = base + 2;
            i1 = p->phase;
            i1 = b2 - i1;
            TILECOPY_8067660(TILEMAP_8067660(0x086F17B0)[i1], 0x0600E000, 0x12, p->phase, 0x1E, 3, 2);
            i2 = p->phase;
            i2 = 2 - i2;
            TILECOPY_8067660(TILEMAP_8067660(0x086F17E8)[i2], 0x0600E038, 1, p->phase, 0x1E, 3, 2);
        }
        i3 = p->phase;
        i3 = (2 - i3) * 60;
        TILECOPY_8067660((const void *)(i3 + 0x086F17D4 + p->arr3[D.cursor] * 120), 0x0600E024, 6, p->phase, 0x1E, 3, 2);
        i4 = p->phase;
        i4 = (2 - i4) * 60;
        TILECOPY_8067660((const void *)(i4 + 0x086F17E0 + p->arr6[D.cursor] * 120), 0x0600E030, 4, p->phase, 0x1E, 3, 2);
        b3 = base + 2;
        i5 = p->phase;
        i5 = (b3 - i5) * 60;
        TILECOPY_8067660((const void *)(i5 + 0x086F17B0 + (p->sel * 2 + 0x40) * 2),
                         0x0600E000 + (SEL18_8067660(p) * 2 + 4) * 2, 2, p->phase, 0x1E, 3, 2);
        /* FAKEMATCH: reusing i1 (live since the first block) makes it the CSE class head, so the compare keeps its
           own register and the body works on the copy. */
        if (p->phase <= 1u) {
            i1 = p->phase;
            sub_0807A9C0(TILEMAP_8067660(0x086E26D0)[i1], 0x0600E000 + (i1 << 6), 0x1E, 2 - PHASE4_8067660(p), 0x1E, 0, 0);
        }
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
        sub_08077EF4(gUnk_081A6D8C[p->sel], 0, 5, -1, -1, 0, 0, 0, 0, 0, 0, (int)&D);
}
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
/* struct ScrollSt above lacks the 2 bytes after f18AC (f18B0 lands at 0x18AE); this copy has the right offsets. */
struct ScrollSt79E0 {
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
    u8 pad18AE[2];
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
/* Callee prototypes with the real u16 row parameter (the unit-wide ones take s32/u32, which turns the
   row's `>> 3` into asr); sub_0807AFFC really takes the VRAM page as a third argument (see sub_080671E8). */
typedef void (*Fn518C_79E0)(u16, s32, s32, u16, u8 *);
typedef void (*Fn5108_79E0)(u16, s32, s32, u16, u8 *, s32);
#define CALL518C ((Fn518C_79E0)sub_0806518C)
#define CALL5108 ((Fn5108_79E0)sub_08065108)
typedef void (*FnAFFC_79E0)(u16 id, u32 dst, u16 page);
#define CALLAFFC ((FnAFFC_79E0)sub_0807AFFC)
typedef void (*Fn5AB4_79E0)(s32, s32, u16, u8 *);
typedef void (*Fn5E6C_79E0)(s32, s32, u16, s32);
#define CALL5AB4 ((Fn5AB4_79E0)sub_08065AB4)
#define CALL5E6C ((Fn5E6C_79E0)sub_08065E6C)
/* Scroll the card list up by one row (cursor column `cursor`): two tilemap scrolls, redraw the card
   entering at the top, shift the item counters, then refresh the side panel. `*out` gets the card id
   that left, or 0xFFFF. */
void sub_080679E0(u16 *out)
{
#define S79E0 (*(struct ScrollSt79E0 *)&gUnk_0201DB20_s)
    s32 t1, t2, t3;

    if (gUnk_0201E140[S79E0.cursor] == 0)
        return;
    S79E0.f1C58 = 0x1E;
    S79E0.f18AC = 0xFC00;
    sub_0807B100(6, 0, -1, S79E0.f628);
    gUnk_0201E140[S79E0.cursor]--;
    S79E0.f634 ^= 1;
    t1 = gUnk_0808749C[0];
    sub_08079834(0, 0x0600C000, 0, ((S79E0.f63E + t1) & 0xFF) >> 3, 0x1E, 5, gUnk_0201E160);
    if (gUnk_0201E140[S79E0.cursor] < S79E0.cnt1494[gUnk_0201EFC0[S79E0.cursor]][S79E0.cursor]) {
        CALL518C(sub_08068D1C(S79E0.cursor, gUnk_0201EFC0[S79E0.cursor], gUnk_0201E140[S79E0.cursor]),
                     0x0600C000, 0, ((S79E0.f63E + t1) & 0xFF) >> 3, gUnk_0201E160);
        CALLAFFC(sub_08068D1C(S79E0.cursor, gUnk_0201EFC0[S79E0.cursor], gUnk_0201E140[S79E0.cursor]),
                 S79E0.f634 * 0x1680 + 0x06008000, S79E0.f634);
        sub_08064E28(((S79E0.f630 & 0xFF) >> 3) + 0x13, (u8)(((S79E0.f632 & 0xFF) >> 3) - 8), S79E0.f634, 1, 0);
    }
    S79E0.f63E -= 0x28;
    S79E0.f632 -= 0x50;
    S79E0.f635 = 1;
    /* t2 is read inside the argument list, table operand first: the 0x0600D000 pseudo is then set
       before the table load, and the load before the f63A address. */
    sub_08079834(0, 0x0600D000, 3, (((t2 = gUnk_08087494[0]) + S79E0.f63A) & 0xFF) >> 3, 0x1B, 2, gUnk_0201E160);
    if ((s16)gUnk_0201E140[S79E0.cursor] - 2 >= 0) {
        CALL5108(sub_08068D1C(S79E0.cursor, gUnk_0201EFC0[S79E0.cursor], (u16)(gUnk_0201E140[S79E0.cursor] - 2)),
                     0x0600D000, 0, ((S79E0.f63A + t2) & 0xFF) >> 3, gUnk_0201E160, 0);
        *out = sub_08068D1C(S79E0.cursor, gUnk_0201EFC0[S79E0.cursor], (u16)(gUnk_0201E140[S79E0.cursor] - 2));
    } else {
        *out = 0xFFFF;
    }
    t3 = gUnk_08087494[1];
    sub_08079834(0, 0x0600D000, 3, ((S79E0.f63A + t3) & 0xFF) >> 3, 0x1B, 2, S79E0.f640);
    if (gUnk_0201E140[gUnk_0201F73C] + 1 < S79E0.cnt1494[gUnk_0201EFC0[gUnk_0201F73C]][gUnk_0201F73C])
        CALL5108(sub_08068D1C(gUnk_0201F73C, gUnk_0201EFC0[gUnk_0201F73C], (u16)(gUnk_0201E140[gUnk_0201F73C] + 1)),
                     0x0600D000, 0, ((S79E0.f63A + t3) & 0xFF) >> 3, S79E0.f640, 3);
    S79E0.f63A -= 0x10;
    CALL5AB4(0x0600C000, 0xB, ((S79E0.f63E + 0x38) & 0xFF) >> 3, S79E0.f640);
    CALL5E6C(0x0600C000, 0x11, ((S79E0.f63E + 0x38) & 0xFF) >> 3, 6);
    sub_080657F8(0);
    sub_08065F34(S79E0.cnt1494[gUnk_0201EFC0[gUnk_0201F73C]][gUnk_0201F73C],
                 gUnk_0201E140[gUnk_0201F73C], &S79E0.f1BB0);
    {
        /* FAKEMATCH: addressing f1BB4 through the f1BB5 pointer puts the offset constant in r0 and the
           address in r1, as in the ROM; plain S79E0.f1BB4 |= 1 swaps them. */
        u8 *p = &S79E0.f1BB5;
        if (*p != 0) {
            *p = 2;
            p[-1] |= 1;
        }
    }
    sub_08066478(S79E0.f635, *out, ((u16 *)S79E0.cnt1494)[gUnk_0201EFC0[gUnk_0201F73C] * 3 + gUnk_0201F73C],
                 S79E0.f1BB8, S79E0.f18B0);
    sub_08065058(1);
    sub_08077AEC(0);
}
#undef S79E0
struct DnSt_x {
    u8 pad0[0x620];
    u16 arr620[3];               /* +0x620 */
    u8 pad626[2];
    u8 f628[8];                  /* +0x628 */
    u16 f630;                    /* +0x630 */
    u16 f632;                    /* +0x632 */
    u8 f634;                     /* +0x634 */
    u8 f635;                     /* +0x635 */
    u8 pad636[0x63A - 0x636];
    u16 f63A;                    /* +0x63A */
    u8 pad63C[0x63E - 0x63C];
    u16 f63E;                    /* +0x63E */
    u8 f640[0x1494 - 0x640];     /* +0x640 */
    u16 cnt1494[2][3];           /* +0x1494 */
    u8 arr14A0[0x18AC - 0x14A0]; /* +0x14A0 */
    u16 f18AC;                   /* +0x18AC */
    u8 pad18AE[2];
    u8 f18B0[0x1BB0 - 0x18B0];   /* +0x18B0 */
    u16 f1BB0;                   /* +0x1BB0 */
    u8 pad1BB2[4];
    u8 f1BB6;                    /* +0x1BB6 */
    u8 f1BB7;                    /* +0x1BB7 */
    u8 f1BB8[0x1C1C - 0x1BB8];   /* +0x1BB8 */
    u8 cursor;                   /* +0x1C1C */
    u8 pad1C1D[0x1C58 - 0x1C1D];
    u16 f1C58;                   /* +0x1C58 */
};
#define DN (*(struct DnSt_x *)&gUnk_0201DB20_s)
#define DN_65108 ((void (*)(u16, u32, u16, u16, u8 *, u8))sub_08065108)
#define DN_6518C ((void (*)(u16, u32, u16, u16, u8 *))sub_0806518C)
#define DN_65384 ((void (*)(u16, u32, u16, u16, u8 *))sub_08065384)
#define DN_65AB4 ((void (*)(u32, u16, u16, u8 *))sub_08065AB4)
#define DN_65E6C ((void (*)(u32, u16, u16, u8))sub_08065E6C)
/* Scroll the card list down by one row (cursor column DN.cursor). */
void sub_08067DA4(u16 *out)
{
    s32 t1, t2, t3;

    if (DN.arr620[DN.cursor] >= DN.cnt1494[DN.arr14A0[DN.cursor]][DN.cursor] - 1)
        return;
    DN.f1C58 = 0x1E;
    DN.f18AC = 0xFC00;
    sub_0807B100(0, 6, 1, DN.f628);
    DN.f634 ^= 1;
    ((Fn0807AFFC_x)sub_0807AFFC)(sub_08068D1C(DN.cursor, DN.arr14A0[DN.cursor], ++DN.arr620[DN.cursor]),
                                 DN.f634 * 0x1680 + 0x06008000, DN.f634);
    sub_08064E28(((DN.f630 & 0xFF) >> 3) + 0x13, ((DN.f632 & 0xFF) >> 3) + 0xC, DN.f634, 1, 0);
    DN.f635 = 2;
    sub_08079834(0, 0x0600D000, 3, (((t1 = gUnk_08087494[2]) + DN.f63A) & 0xFF) >> 3, 0x1B, 3, DN.f640);
    if (DN.arr620[DN.cursor] + 2 < DN.cnt1494[DN.arr14A0[DN.cursor]][DN.cursor]) {
        DN_65108(sub_08068D1C(DN.cursor, DN.arr14A0[DN.cursor], DN.arr620[DN.cursor] + 2),
                     0x0600D000, 0, ((DN.f63A + t1) & 0xFF) >> 3, DN.f640, 6);
        *out = sub_08068D1C(DN.cursor, DN.arr14A0[DN.cursor], DN.arr620[DN.cursor] + 2);
    } else {
        *out = 0xFFFF;
    }
    sub_08079834(0, 0x0600D000, 3, (((t2 = gUnk_08087494[3]) + DN.f63A) & 0xFF) >> 3, 0x1B, 3, DN.f640);
    if ((s16)DN.arr620[DN.cursor] - 1 >= 0)
        DN_65108(sub_08068D1C(DN.cursor, DN.arr14A0[DN.cursor], DN.arr620[DN.cursor] - 1),
                     0x0600D000, 0, ((DN.f63A + t2) & 0xFF) >> 3, DN.f640, 3);
    sub_08079834(0, 0x0600C000, 0, (((t3 = gUnk_0808749C[1]) + DN.f63E) & 0xFF) >> 3, 0x1E, 6, DN.f640);
    if (DN.cnt1494[DN.arr14A0[DN.cursor]][DN.cursor] != 0) {
        DN_6518C(sub_08068D1C(DN.cursor, DN.arr14A0[DN.cursor], DN.arr620[DN.cursor]),
                     0x0600C000, 0, ((DN.f63E + t3) & 0xFF) >> 3, DN.f640);
        DN_65AB4(0x0600C000, 0xB, ((DN.f63E + 0x60) & 0xFF) >> 3, DN.f640);
        DN_65E6C(0x0600C000, 0x11, ((DN.f63E + 0x60) & 0xFF) >> 3, 6);
        sub_080657F8(5);
        sub_08065F34(DN.cnt1494[DN.arr14A0[DN.cursor]][DN.cursor], DN.arr620[DN.cursor], &DN.f1BB0);
        if (DN.f1BB7 != 0) {
            DN.f1BB7 = 2;
            DN.f1BB6 |= 1;
        }
        sub_08066478(DN.f635, *out, ((u16 *)DN.cnt1494)[DN.arr14A0[DN.cursor] * 3 + DN.cursor], DN.f1BB8, DN.f18B0);
    } else {
        DN_65384(sub_08068D1C(DN.cursor, DN.arr14A0[DN.cursor], DN.arr620[DN.cursor]),
                     0x0600C000, 0, ((DN.f63E + t3) & 0xFF) >> 3, DN.f640);
    }
    sub_08065058(2);
    sub_08077AEC(0);
}
