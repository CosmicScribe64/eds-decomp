/*
 * deck_edit_list (0x0806704C-0x08068180): list switching, the command bar and one-row scrolling of the
 * card-list screens (Deck Edit, the side-deck swap and the two card pickers; wiki/functions/deck-edit-list-c.md).
 *
 * All of these screens keep their state in gDeckEdit (0x0201DB20, struct DeckEdit in deck_edit.h) and show
 * one of three card lists (enum DeckEditList) at a time. This unit holds the helpers around the list view:
 *   - DeckEdit_UpdatePanelHighlight: move the highlight between the list panels (anims[6..8] and [13..15]).
 *   - DeckEdit_CountSideDeckMonsters: gDeckEdit.sideMonsterCount from the side-deck list's trunk copies.
 *   - DeckEdit_StartListSlide / DeckEdit_HandleShoulderKeys: R/L slide to the next/previous list, redraw
 *     the card panel and the 5 frame slots around the cursor.
 *   - DeckEdit_DrawCommandMenu / DeckEdit_UpdateCommandMenuAnim: the A-button command bar's slide
 *     animation; DeckEdit_CommandLabelVBlank loads the selected command's label tiles during the slide,
 *     DeckEdit_CommandWindowVBlank only sets the WIN1 window under the bar.
 *   - DeckEdit_DrawStatementLabels: the 'STATEMENT' label block of a list.
 *   - DeckEdit_ScrollListUp / DeckEdit_ScrollListDown: scroll the shown list by one card and redraw the
 *     card entering the view, the side panel and the frame slots.
 *
 * The state is reached through several local views of gDeckEdit (see below): each function only matches
 * with its own declared field types and access forms, so the views stay local instead of including
 * deck_edit.h (build/readability/issues/deck_edit_list.md).
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE, gCardStats (read by literal address below) */
#include "constants/card_stats.h"   /* enum CardType (CARD_TYPE_TRAP, CARD_TYPE_MAGIC) */
#include "constants/sound.h"        /* SE_CURSOR */
#include "legacy/gba.h"                    /* R_BUTTON, L_BUTTON */
#include "legacy/main.h"                   /* struct Main gMain, newKeys, vblankCallback */

/* Deck-edit scene state at 0x0201DB20 (see deck_edit_panel / deck_edit_widgets / deck_edit_cards). Several views of the same
   symbol are declared with asm() names so that each function sees only the fields it uses. */
struct PanelCell {           /* 20-byte cells at +0x1726: the active/timer bytes of anims[k] (AnimState +0xE/+0xF) */
    u8 active;
    u8 timer;
    u8 pad2[18];
};
struct ListPanelCellView {
    u8 pad0[0x1726];
    struct PanelCell cell[1]; /* +0x1726, 20 bytes each */
};
extern struct ListPanelCellView gDeckEditPanelCells asm("gDeckEdit");
struct CommandMenuBitView { /* command-menu anim/rows bits at +0x1C3D */
    u8 pad0[0x1C3D];
    u8 anim : 3;            /* +0x1C3D bits 0-2 */
    u8 b3 : 2;
    u8 rows : 2;            /* +0x1C3D bits 5-6 */
    u8 b7 : 1;
};
extern struct CommandMenuBitView gDeckEditMenuBits asm("gDeckEdit");
/* Command-bar record (struct DeckEditCommandMenu in deck_edit.h at +0x1C3C), see DeckEdit_DrawCommandMenu:
   the bits below are u32 bitfields of the first word. */
struct CommandMenuSlide {
    u32 timer : 8;          /* +0 */
    u32 anim : 3;           /* +1 bits 0-2: 1 open, 2 close */
    u32 prevRows : 2;       /* +1 bits 3-4: previous phase */
    u32 rows : 2;           /* +1 bits 5-6: 0..2 */
    u32 choice : 3;         /* bits 15-17 */
    u32 rest : 6;
    u8 filter[3];           /* +3: per-list filter index */
    u8 sort[3];             /* +6: per-list sort index */
};
struct CommandMenuSlideA {
    u8 timer;
    u8 anim : 3;
    u8 prevRows : 2;
    u8 rows : 2;
    u8 b7 : 1;
};
#define PA(p) ((struct CommandMenuSlideA *)(p))
extern void CopyMapRectSetPalette(const void *src, u32 dst, int w, int h, int a, int b, int c);
extern const u8 gDeckEditMenuTilemap[][60];      /* 0x086F17B0: command menu background map */
extern const u8 gUnk_086F17E8[][60];             /* 0x086F17E8 */
extern const u8 gUnk_086F17D4[][120];            /* 0x086F17D4 */
extern const u8 gUnk_086F17E0[][120];            /* 0x086F17E0 */
extern const u8 gUnk_086F1B10[][12]; /* 0x086F1B10: statement label tile maps, 12-byte rows */
/* Word view of the command-menu word at +0x1C3C (DeckEdit_CommandLabelVBlank / DeckEdit_CommandWindowVBlank). */
struct CommandMenuWordView {
    u8 pad0[0x1C3C];
    union {
        struct {
            u32 lo15 : 15;
            u32 choice : 3;     /* bits 15-17: selected panel */
            u32 hi : 14;
        } w;
        struct {
            u8 b0;
            u8 anim : 3;        /* +0x1C3D bits 0-2 */
            u8 b3 : 2;
            u8 rows : 2;
            u8 b7 : 1;
        } b;
    } u;
    u8 pad1C40[0x1C5A - 0x1C40];
    u8 menuVariant : 2;         /* +0x1C5A bits 0-1 */
    u8 rest : 6;
};
extern struct CommandMenuWordView gDeckEditMenuWord asm("gDeckEdit");
extern const u8 gDeckEditCommandLabelGfx[][0x400];
extern const u8 gDeckEditSwapLabelGfx[];
extern const u8 gDeckEditDecideLabelGfx[];
struct MenuOpenView {       /* gDeckEdit.menuOpen at +0x1C48 */
    u8 pad0[0x1C48];
    u8 menuOpen : 1;
    u8 rest : 7;
};
extern struct MenuOpenView gDeckEditMenuOpen asm("gDeckEdit");
struct SideDeckCountView {
    u8 pad0[0x1494];
    u16 listCounts[2][3];       /* +0x1494 */
    u8 pad149A[0x1712 - 0x149A - 6];
    u16 sideMonsterCount[2];    /* +0x1712 */
    u16 f1714;
};
extern struct SideDeckCountView gDeckEditSideCounts asm("gDeckEdit");
extern u16 DeckEdit_GetListCard(u8 list, u8 row, u16 col);
/* gCardStats (0x08621DE0) read by literal address: the symbol form folds the base into the index add. */
#define CARD_STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
struct FrameSlotCell { u8 bytes[16]; };
/* List-view fields of gDeckEdit (deck_edit.h names; one of the local views this unit keeps, see the module comment). */
struct DeckEditListView {
    u8 pad0[0x620];
    u16 listPos[3];             /* +0x620 */
    u8 pad626[2];
    u8 scrollEase[8];           /* +0x628 struct Ease (address passed to Ease_Start) */
    u16 bg3Hofs;                /* +0x630 */
    u16 bg3Vofs;                /* +0x632 */
    u8 cardArtPage;             /* +0x634 */
    u8 scrollDir;               /* +0x635 */
    u8 pad636[0x1494 - 0x636];
    u16 listCounts[2][3];       /* +0x1494 */
    u8 listRow[0x1710 - 0x14A0]; /* +0x14A0 */
    u8 redrawOnPageSlide;      /* +0x1710 bit 0 */
    u8 pad1711[0x18AC - 0x1711];
    u16 brightness;             /* +0x18AC */
    u8 pad18AE[2];
    u8 objAffineStaging[0x1BB0 - 0x18B0]; /* +0x18B0 (address passed to DeckEdit_InitFrameSlot) */
    u16 scrollBar;              /* +0x1BB0 thumbLen (address passed to DeckEdit_CalcScrollBar) */
    u8 pad1BB2[2];
    u8 scrollBarDirty;          /* +0x1BB4 scrollBar+4: upArrowDirty byte */
    u8 pad1BB5[3];
    struct FrameSlotCell frameSlots[5]; /* +0x1BB8 */
    u8 pad1C08[0x1C14 - 0x1C08];
    u8 frameSwitchState;        /* +0x1C14 (struct FrameSwitchState) */
    u8 pad1C15[7];
    u8 curList;                 /* +0x1C1C */
    u8 pad1C1D[0x1C58 - 0x1C1D];
    u32 nameTabTimer : 16;      /* +0x1C58 */
    u32 menuVariant : 2;        /* +0x1C5A bits 0-1 */
    u32 rest1C5A : 14;
};
extern struct DeckEditListView gDeckEditListView asm("gDeckEdit");
#define D gDeckEditListView
extern void Ease_Start(int a, int b, int c, void *d);
extern void LoadCardArt8bpp(u16 id, u32 dst);
extern void DeckEdit_DrawPortraitTilemap(u8 col, u8 row, u8 set, u8 wrap, u32 base);
extern void DeckEdit_CalcScrollBar(u32 a, u32 b, void *out);
extern void DeckEdit_InitFrameSlot(u8 slot, u16 id, u8 kind, void *slots, void *out);
extern const u8 gUnk_081A6EAC[]; /* 0x081A6EAC: sprite templates */
extern const u8 gDeckEditCommandLabelSprites[][40];
extern void CopyMapRectAddOffset(const void *src, u32 dst, int a, int b, int c, int d, int e);
extern u16 *OamListAddSpriteGroup(const void *a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l);
extern void DeckEdit_CommandLabelVBlank(void);
extern void PlaySE(u16 id);
extern void DeckEdit_StartListSlide(u8 a);
extern void DeckEdit_DrawStatementLabels(u8 x);

/* Card-list scroll state (DeckEdit_ScrollListUp / DeckEdit_ScrollListDown): base view the two scroll views cast from. */
struct DeckEditScrollBase {
    u8 pad0[0x628];
    u8 scrollEase[8];            /* +0x628 struct Ease passed to Ease_Start */
    u16 bg3Hofs;                 /* +0x630 */
    u16 bg3Vofs;                 /* +0x632 */
    u8 cardArtPage;              /* +0x634 */
    u8 scrollDir;                /* +0x635 */
    u8 pad636[0x63A - 0x636];
    u16 bg1Vofs;                 /* +0x63A */
    u8 pad63C[0x63E - 0x63C];
    u16 bg0Vofs;                 /* +0x63E */
    u8 textFlags[0x1494 - 0x640]; /* +0x640 text tilemap flags */
    u16 listCounts[2][3];        /* +0x1494 [row][list] */
    u8 listRow[0x18AC - 0x14A0];
    u16 brightness;              /* +0x18AC */
    u8 objAffineStaging[0x1BB0 - 0x18B0]; /* +0x18B0 out buffer */
    u16 scrollBar;               /* +0x1BB0 */
    u8 pad1BB2[2];
    u8 upArrowDirty;             /* +0x1BB4 */
    u8 upArrowFrame;             /* +0x1BB5 */
    u8 downArrowDirty;           /* +0x1BB6 */
    u8 downArrowFrame;           /* +0x1BB7 */
    u8 frameSlots[0x1C1C - 0x1BB8]; /* +0x1BB8 5 slots */
    u8 curList;                  /* +0x1C1C */
    u8 pad1C1D[0x1C58 - 0x1C1D];
    u16 nameTabTimer;            /* +0x1C58 */
};
extern struct DeckEditScrollBase gDeckEditScroll asm("gDeckEdit");
extern u16 gUnk_0201E140[];      /* 0x0201E140: per-list scroll positions (aliases gDeckEdit.listPos) */
extern u8 gUnk_0201EFC0[];       /* 0x0201EFC0: per-list shown rows (aliases gDeckEdit.listRow) */
extern u8 gUnk_0201E160[];       /* 0x0201E160: text tilemap flags buffer (gDeckEdit.textFlags) */
extern u8 gUnk_0201F73C;         /* 0x0201F73C: alias of gDeckEdit.curList */
extern s16 gUnk_08087494[];      /* 0x08087494: scroll offset table */
extern s16 gUnk_0808749C[];      /* 0x0808749C: scroll offset table */
extern void FillMapRectWrap(s32, s32, s32, s32, s32, s32, u8 *);
extern void DeckEdit_DrawCursorRowName(u16, s32, s32, u32, u8 *);
extern void DeckEdit_DrawListRowName(u16, s32, s32, u32, u8 *, s32);
extern void DeckEdit_DrawAtkDef(s32, s32, u32, u8 *);
extern void DeckEdit_DrawLevelStars(s32, s32, u32, s32);
extern void DeckEdit_DrawCardIcons(s32);
extern void DeckEdit_RotateListRowRing(s32);
extern void DeckEdit_DrawNoCardsText(u16, s32, s32, u32, u8 *);
extern void DeckEdit_ScrollFrameSlots(u8, u16, u16, u8 *, u8 *);

/* Move the selection from panel *b to panel *a: marks the old cells (`0xFF`) and the new ones (active = 1, timer = 0), then
   sets the animation bytes `s[idx * 20 + 0xE]` (hypothesis). Structs are 4-aligned under agbcc, so the 20-byte cell
   array starts at +0x1724 with the flag bytes at +2/+3. */
struct AnimCellShifted {
    u8 pad0[2];
    u8 active;              /* +2 */
    u8 timer;               /* +3 */
    u8 pad4[10];
    u8 activeMark;          /* +0xE */
    u8 padF[5];
};
struct AnimCellsShiftedView {
    u8 pad0[0x1724];
    struct AnimCellShifted cell[1]; /* +0x1724, 20 bytes each */
};
#define CX_PANEL (*(struct AnimCellsShiftedView *)&gDeckEditPanelCells)
struct AnimCellArray { struct AnimCellShifted cell[1]; };
void DeckEdit_UpdatePanelHighlight(u8 *a, u8 *b, struct AnimCellArray *s)
{
    if (*a != *b) {
        CX_PANEL.cell[*a + 0xD].active = 1;
        CX_PANEL.cell[*a + 0xD].timer = 0;
        CX_PANEL.cell[*a + 6].active = 1;
        CX_PANEL.cell[*a + 6].timer = 0;
        CX_PANEL.cell[*b + 0xD].active |= 0xFF;
        CX_PANEL.cell[*b + 6].active |= 0xFF;
        *b = *a;
    }
    s->cell[*a + 0xD].activeMark = 0xFF;
    s->cell[*a + 6].activeMark = 1;
}
/* Save image 0x02011C20: 4-byte trunk entries from +0x08, indexed by card ID; sideCopies = bits 4-5 of byte +1 (copies in
   the deck, hypothesis). Indexing the symbol as a struct array keeps the base load after the call (a constant or a
   (u8 *) cast is folded into the add; a hoisted pointer moves to sl). */
struct TrunkEntryView { u8 b0; u8 lo4 : 4; u8 sideCopies : 2; u8 hi : 2; u16 w2; };
struct SaveTrunkView { u8 pad0[8]; struct TrunkEntryView trunk[0x800]; };
extern struct SaveTrunkView gSaveData;
/* For both rows: sideMonsterCount[row] = sum of trunk sideCopies over the non-Trap/Magic cards of list 2
   (listCounts[row][2] cards via DeckEdit_GetListCard(2, row, j)); then sideMonsterCount[1] = sideMonsterCount[0]
   (the store is at +0x1714). */
void DeckEdit_CountSideDeckMonsters(void)
{
    u16 i;
    for (i = 0; i <= 1; i++) {
        u16 j;
        gDeckEditSideCounts.sideMonsterCount[i] = 0;
        for (j = 0; j < gDeckEditSideCounts.listCounts[i][2]; j++) {
            s16 id = DeckEdit_GetListCard(2, i, j);
            switch ((int)CARD_STATS_TYPE(CARD_STATS_WORD(id))) {
            case CARD_TYPE_TRAP:
            case CARD_TYPE_MAGIC:
                break;
            default:
                id = DeckEdit_GetListCard(2, i, j);
                gDeckEditSideCounts.sideMonsterCount[i] += gSaveData.trunk[(u16)id].sideCopies;
                break;
            }
        }
    }
    gDeckEditSideCounts.sideMonsterCount[1] = gDeckEditSideCounts.sideMonsterCount[0];
}
/* Redraw the card panel after a page change: `a` = 2 / 3 is the direction. Sets up the window scroll, draws the current
   card's graphics (or clears them), places the page frame, then re-initialises the 5 card slots around the cursor. */
/* LoadCardArt8bpp really takes a third argument, the VRAM page (see its definition in gfx_util); passing D.cardArtPage
   there puts the page byte in r2 and the `*3` temporary in r3. The unit-wide prototype has two parameters, so this
   call goes through a cast to the three-parameter type. */
typedef void (*LoadCardArtPageFn)(u16 id, u32 dst, u16 page);
void DeckEdit_StartListSlide(u8 a)
{
    u16 j;
    u8 k;
    D.brightness = 0xFC00;
    if (a == 2)
        Ease_Start(0, 6, 1, D.scrollEase);
    else
        Ease_Start(6, 0, -1, D.scrollEase);
    D.cardArtPage ^= 1;
    if (D.listCounts[D.listRow[D.curList]][D.curList] != 0) {
        ((LoadCardArtPageFn)LoadCardArt8bpp)(DeckEdit_GetListCard(D.curList, D.listRow[D.curList], D.listPos[D.curList]), D.cardArtPage * 0x1680 + 0x06008000, D.cardArtPage);
    } else {
        u32 zero = 0;
        CpuFastSet(&zero, (void *)(D.cardArtPage * 0x1680 + 0x06008000), 0x010005A0);
    }
    if (a == 3) {
        DeckEdit_DrawPortraitTilemap(((D.bg3Hofs & 0xFF) >> 3) + 9, ((D.bg3Vofs & 0xFF) >> 3) + 2, D.cardArtPage, 1, 0);
        D.bg3Hofs -= 0x50;
        D.scrollDir = 4;
    } else {
        DeckEdit_DrawPortraitTilemap(((D.bg3Hofs & 0xFF) >> 3) + 0x1D, ((D.bg3Vofs & 0xFF) >> 3) + 2, D.cardArtPage, 1, 0);
        D.scrollDir = 3;
    }
    D.redrawOnPageSlide |= 1;
    DeckEdit_CalcScrollBar(D.listCounts[D.listRow[D.curList]][D.curList], D.listPos[D.curList], &D.scrollBar);
    j = D.listPos[D.curList] - 2;
    for (k = 0; k <= 4; j++, k++) {
        if ((s16)j >= 0 && j < D.listCounts[D.listRow[D.curList]][D.curList])
            DeckEdit_InitFrameSlot(k, DeckEdit_GetListCard(D.curList, D.listRow[D.curList], j), k + 1, &D.frameSlots[0], D.objAffineStaging);
        else
            D.frameSlots[k].bytes[0xC] = 0;
    }
    D.frameSwitchState = 0;
    D.frameSlots[0].bytes[0] = 5;
}
/* Key callback of the 3-panel selector: R (R_BUTTON) selects the next panel, L (L_BUTTON) the previous one (wrapping). */
void DeckEdit_HandleShoulderKeys(u8 *idx)
{
    if (gMain.newKeys & R_BUTTON) {
        struct CommandMenuWordView *st;
        (*idx)++;
        if (*idx == 3)
            *idx = 0;
        st = &gDeckEditMenuWord;
        st->u.w.choice = 0;
        st->u.b.anim = 3;
        DeckEdit_StartListSlide(2);
        DeckEdit_DrawStatementLabels(*idx);
        PlaySE(SE_CURSOR);
    } else if (gMain.newKeys & L_BUTTON) {
        struct CommandMenuWordView *st;
        if (*idx != 0)
            *idx = *idx - 1;
        else
            *idx = 2;
        st = &gDeckEditMenuWord;
        st->u.w.choice = 0;
        st->u.b.anim = 3;
        DeckEdit_StartListSlide(3);
        DeckEdit_DrawStatementLabels(*idx);
        PlaySE(SE_CURSOR);
    }
}
#define LABEL_TILES ((const u8 *)0x086EF3B0) /* 0x400-byte tile blocks; [7] and [8] are the special panels */
/* The same word as CommandMenuWordView.u.w seen as one 18-bit field: the panel number is its top 3 bits (15-17). */
struct CommandMenuWordLow18 {
    u8 pad0[0x1C3C];
    u32 low18 : 18;
    u32 hi : 14;
};
/* Load the 2 x 0x400-byte tile blocks of the selected panel (hypothesis) into OBJ VRAM 0x06010800 and set the window. */
void DeckEdit_CommandLabelVBlank(void)
{
    struct CommandMenuWordView *st;
    const u8 *src;
    u8 *dst = (u8 *)0x06010800;
    u8 i;
    st = &gDeckEditMenuWord;
    switch (st->menuVariant) {
    default:
        src = LABEL_TILES + st->u.w.choice * 0x400;
        break;
    case 1:
        switch (st->u.w.choice) {
        case 2:
        case 3:
            src = gDeckEditSwapLabelGfx;
            break;
        default:
            src = LABEL_TILES + gDeckEditMenuWord.u.w.choice * 0x400;
            break;
        }
        break;
    case 2:
        /* the panel compare goes through the 18-bit view (lsr #29 of the shared lsl #14); the index rereads sel */
        if ((((struct CommandMenuWordLow18 *)st)->low18 >> 15) == 6) {
            src = gDeckEditDecideLabelGfx;
            break;
        }
        src = LABEL_TILES + st->u.w.choice * 0x400;
        break;
    }
    for (i = 0; i <= 1; i++) {
        CpuFastSet(src, dst, 0x80);
        src += 0x200;
        dst += 0x400;
    }
    *(u16 *)0x04000042 = 0xF0;
    *(u16 *)0x04000046 = (gDeckEditMenuWord.u.b.rows << 11) | 0x70;
}
/* Set the WIN1 window registers for the deck-edit card view (hypothesis). */
void DeckEdit_CommandWindowVBlank(void)
{
    *(u16 *)0x04000042 = 0xF0;
    *(u16 *)0x04000046 = (gDeckEditMenuBits.rows << 11) | 0x70;
}
/* The real prototype (gfx_util.c) takes u8 sizes: calling through it narrows the height argument, which is why
   each call re-extracts p->rows from the shared shift. CopyMapRectAddOffset keeps the unit's int prototype (no narrowing). */
typedef void (*TileCopyIntFn)(const void *src, u32 dst, u8 w, u8 h, u8 srcW, u8 pal, u8 hi);
#define TILECOPY_INT ((TileCopyIntFn)CopyMapRectSetPalette)
/* Wider views of the first word of the record, used for the second extraction of a field that shares its shift.
   FAKEMATCH: sel read as (bits 0-17) >> 15 and phase as (bits 11-14) >> 2 keep CSE from reusing the first
   extraction. The 4-bit view is padded to 12 bytes so it is BLKmode and read with ldrb like struct CommandMenuSlide. */
struct CommandMenuSlide18 { u32 low18 : 18; u32 hi : 14; };
struct CommandMenuSlideP4 { u32 timer : 8; u32 anim : 3; u32 rows4 : 4; u32 hi : 17; u8 pad[8]; };
#define CMD_CHOICE(p) (((struct CommandMenuSlide18 *)(p))->low18 >> 15)
#define CMD_PHASE4(p) (((struct CommandMenuSlideP4 *)(p))->rows4 >> 2)
/* Tile maps in ROM, 30 tiles (60 bytes) per row, addressed as integers: the add's constant is then reloaded at each
   use (ldr into the next round-robin reload register) instead of living in a pseudo. */
#define TILEMAP_ADDR(addr) ((const u8 (*)[60])(addr))
/* Draw one frame of the slide animation of the panel record `p` (the word at 0x0201DB20+0x1C3C): the frame, card and
   arrow pieces for the current phase (CopyMapRectSetPalette), the phase<=1 extra rows (CopyMapRectAddOffset), selects the HBlank/VBlank
   callback, then places the two sprite pairs (OamListAddSpriteGroup). */
void DeckEdit_DrawCommandMenu(struct CommandMenuSlide *p)
{
    int base;
    int b2;
    int b3;
    int i1, i2, i3, i4, i5;
    if (D.menuVariant == 2)
        base = 4;
    else
        base = 0;
    if (PA(p)->anim != 0) {
        /* FAKEMATCH: each temp receives the extracted phase and is then overwritten, so the height argument
           re-extracts it from the shared shift (lsl #25 / two lsr #30). base + 2 is written twice (b2, b3) so
           that PRE computes it once, just before the phase test. */
        if (PA(p)->rows != 0) {
            b2 = base + 2;
            i1 = p->rows;
            i1 = b2 - i1;
            TILECOPY_INT(TILEMAP_ADDR(0x086F17B0)[i1], 0x0600E000, 0x12, p->rows, 0x1E, 3, 2);
            i2 = p->rows;
            i2 = 2 - i2;
            TILECOPY_INT(TILEMAP_ADDR(0x086F17E8)[i2], 0x0600E038, 1, p->rows, 0x1E, 3, 2);
        }
        i3 = p->rows;
        i3 = (2 - i3) * 60;
        TILECOPY_INT((const void *)(i3 + 0x086F17D4 + p->filter[D.curList] * 120), 0x0600E024, 6, p->rows, 0x1E, 3, 2);
        i4 = p->rows;
        i4 = (2 - i4) * 60;
        TILECOPY_INT((const void *)(i4 + 0x086F17E0 + p->sort[D.curList] * 120), 0x0600E030, 4, p->rows, 0x1E, 3, 2);
        b3 = base + 2;
        i5 = p->rows;
        i5 = (b3 - i5) * 60;
        TILECOPY_INT((const void *)(i5 + 0x086F17B0 + (p->choice * 2 + 0x40) * 2),
                         0x0600E000 + (CMD_CHOICE(p) * 2 + 4) * 2, 2, p->rows, 0x1E, 3, 2);
        /* FAKEMATCH: reusing i1 (live since the first block) makes it the CSE class head, so the compare keeps its
           own register and the body works on the copy. */
        if (p->rows <= 1u) {
            i1 = p->rows;
            CopyMapRectAddOffset(TILEMAP_ADDR(0x086E26D0)[i1], 0x0600E000 + (i1 << 6), 0x1E, 2 - CMD_PHASE4(p), 0x1E, 0, 0);
        }
        p->prevRows = p->rows;
        if (PA(p)->anim == 3 || PA(p)->anim == 1) {
            gMain.vblankCallback = DeckEdit_CommandLabelVBlank;
            if (PA(p)->anim == 3)
                p->anim = 0;
        } else {
            gMain.vblankCallback = DeckEdit_CommandWindowVBlank;
        }
        D.scrollBarDirty |= 1;
    } else {
        gMain.vblankCallback = 0;
    }
    OamListAddSpriteGroup(gUnk_081A6EAC, 0, 1, 0, (-((2 - p->rows) * 8)) & 0xFF, 1, 0, 1, 0, 0, 0, (int)&D);
    if (PA(p)->rows == 2)
        OamListAddSpriteGroup(gDeckEditCommandLabelSprites[p->choice], 0, 5, -1, -1, 0, 0, 0, 0, 0, 0, (int)&D);
}
void DeckEdit_UpdateCommandMenuAnim(struct CommandMenuSlide *p)
{
    if (PA(p)->anim != 0) {
        p->timer--;
        if (p->timer == 0xFF) {
            p->timer = 0;
            switch (p->anim) {
            case 1:
                if (PA(p)->rows != 2) {
                    p->rows = p->rows + 2;
                } else {
                    p->anim = 0;
                    gDeckEditMenuOpen.menuOpen = 1;
                }
                break;
            case 2:
                if (PA(p)->rows != 0) {
                    p->rows = p->rows - 2;
                } else {
                    p->anim = 0;
                    gDeckEditMenuOpen.menuOpen = 0;
                }
                break;
            }
        }
    }
}
/* Copy a 6x6 block of tiles of the card-info panel number `x` into the map at 0x0600E3B0 (hypothesis). */
void DeckEdit_DrawStatementLabels(u8 x)
{
    CopyMapRectSetPalette(gUnk_086F1B10[x], 0x0600E3B0, 6, 6, 0x1E, 3, 2);
}
/* struct DeckEditScrollBase above lacks the 2 bytes after brightness (objAffineStaging would land at 0x18AE); this copy has the right offsets. */
struct ScrollListUpView {
    u8 pad0[0x628];
    u8 scrollEase[8];            /* +0x628 struct Ease passed to Ease_Start */
    u16 bg3Hofs;                 /* +0x630 */
    u16 bg3Vofs;                 /* +0x632 */
    u8 cardArtPage;              /* +0x634 */
    u8 scrollDir;                /* +0x635 */
    u8 pad636[0x63A - 0x636];
    u16 bg1Vofs;                 /* +0x63A */
    u8 pad63C[0x63E - 0x63C];
    u16 bg0Vofs;                 /* +0x63E */
    u8 textFlags[0x1494 - 0x640]; /* +0x640 */
    u16 listCounts[2][3];        /* +0x1494 [row][list] */
    u8 listRow[0x18AC - 0x14A0];
    u16 brightness;              /* +0x18AC */
    u8 pad18AE[2];
    u8 objAffineStaging[0x1BB0 - 0x18B0]; /* +0x18B0 out buffer */
    u16 scrollBar;               /* +0x1BB0 */
    u8 pad1BB2[2];
    u8 upArrowDirty;             /* +0x1BB4 */
    u8 upArrowFrame;             /* +0x1BB5 */
    u8 downArrowDirty;           /* +0x1BB6 */
    u8 downArrowFrame;           /* +0x1BB7 */
    u8 frameSlots[0x1C1C - 0x1BB8]; /* +0x1BB8 5 slots */
    u8 curList;                  /* +0x1C1C */
    u8 pad1C1D[0x1C58 - 0x1C1D];
    u16 nameTabTimer;            /* +0x1C58 */
};
/* Callee prototypes with the real u16 row parameter (the unit-wide ones take s32/u32, which turns the
   row's `>> 3` into asr); LoadCardArt8bpp really takes the VRAM page as a third argument (see DeckEdit_StartListSlide). */
typedef void (*CursorRowNameFn)(u16, s32, s32, u16, u8 *);
typedef void (*ListRowNameFn)(u16, s32, s32, u16, u8 *, s32);
#define UP_CURSOR_ROW ((CursorRowNameFn)DeckEdit_DrawCursorRowName)
#define UP_LIST_ROW ((ListRowNameFn)DeckEdit_DrawListRowName)
#define UP_CARD_ART ((LoadCardArtPageFn)LoadCardArt8bpp)
typedef void (*AtkDefFn)(s32, s32, u16, u8 *);
typedef void (*LevelStarsFn)(s32, s32, u16, s32);
#define UP_ATK_DEF ((AtkDefFn)DeckEdit_DrawAtkDef)
#define UP_LEVEL_STARS ((LevelStarsFn)DeckEdit_DrawLevelStars)
/* Scroll the card list up by one row (shown list UP.curList): two tilemap scrolls, redraw the card
   entering at the top, shift the item counters, then refresh the side panel. `*out` gets the card id
   that left, or 0xFFFF. */
void DeckEdit_ScrollListUp(u16 *out)
{
#define UP (*(struct ScrollListUpView *)&gDeckEditScroll)
    s32 t1, t2, t3;

    if (gUnk_0201E140[UP.curList] == 0)
        return;
    UP.nameTabTimer = 0x1E;
    UP.brightness = 0xFC00;
    Ease_Start(6, 0, -1, UP.scrollEase);
    gUnk_0201E140[UP.curList]--;
    UP.cardArtPage ^= 1;
    t1 = gUnk_0808749C[0];
    FillMapRectWrap(0, 0x0600C000, 0, ((UP.bg0Vofs + t1) & 0xFF) >> 3, 0x1E, 5, gUnk_0201E160);
    if (gUnk_0201E140[UP.curList] < UP.listCounts[gUnk_0201EFC0[UP.curList]][UP.curList]) {
        UP_CURSOR_ROW(DeckEdit_GetListCard(UP.curList, gUnk_0201EFC0[UP.curList], gUnk_0201E140[UP.curList]),
                     0x0600C000, 0, ((UP.bg0Vofs + t1) & 0xFF) >> 3, gUnk_0201E160);
        UP_CARD_ART(DeckEdit_GetListCard(UP.curList, gUnk_0201EFC0[UP.curList], gUnk_0201E140[UP.curList]),
                 UP.cardArtPage * 0x1680 + 0x06008000, UP.cardArtPage);
        DeckEdit_DrawPortraitTilemap(((UP.bg3Hofs & 0xFF) >> 3) + 0x13, (u8)(((UP.bg3Vofs & 0xFF) >> 3) - 8), UP.cardArtPage, 1, 0);
    }
    UP.bg0Vofs -= 0x28;
    UP.bg3Vofs -= 0x50;
    UP.scrollDir = 1;
    /* t2 is read inside the argument list, table operand first: the 0x0600D000 pseudo is then set
       before the table load, and the load before the bg1Vofs address. */
    FillMapRectWrap(0, 0x0600D000, 3, (((t2 = gUnk_08087494[0]) + UP.bg1Vofs) & 0xFF) >> 3, 0x1B, 2, gUnk_0201E160);
    if ((s16)gUnk_0201E140[UP.curList] - 2 >= 0) {
        UP_LIST_ROW(DeckEdit_GetListCard(UP.curList, gUnk_0201EFC0[UP.curList], (u16)(gUnk_0201E140[UP.curList] - 2)),
                     0x0600D000, 0, ((UP.bg1Vofs + t2) & 0xFF) >> 3, gUnk_0201E160, 0);
        *out = DeckEdit_GetListCard(UP.curList, gUnk_0201EFC0[UP.curList], (u16)(gUnk_0201E140[UP.curList] - 2));
    } else {
        *out = 0xFFFF;
    }
    t3 = gUnk_08087494[1];
    FillMapRectWrap(0, 0x0600D000, 3, ((UP.bg1Vofs + t3) & 0xFF) >> 3, 0x1B, 2, UP.textFlags);
    if (gUnk_0201E140[gUnk_0201F73C] + 1 < UP.listCounts[gUnk_0201EFC0[gUnk_0201F73C]][gUnk_0201F73C])
        UP_LIST_ROW(DeckEdit_GetListCard(gUnk_0201F73C, gUnk_0201EFC0[gUnk_0201F73C], (u16)(gUnk_0201E140[gUnk_0201F73C] + 1)),
                     0x0600D000, 0, ((UP.bg1Vofs + t3) & 0xFF) >> 3, UP.textFlags, 3);
    UP.bg1Vofs -= 0x10;
    UP_ATK_DEF(0x0600C000, 0xB, ((UP.bg0Vofs + 0x38) & 0xFF) >> 3, UP.textFlags);
    UP_LEVEL_STARS(0x0600C000, 0x11, ((UP.bg0Vofs + 0x38) & 0xFF) >> 3, 6);
    DeckEdit_DrawCardIcons(0);
    DeckEdit_CalcScrollBar(UP.listCounts[gUnk_0201EFC0[gUnk_0201F73C]][gUnk_0201F73C],
                 gUnk_0201E140[gUnk_0201F73C], &UP.scrollBar);
    {
        /* FAKEMATCH: addressing upArrowDirty through the upArrowFrame pointer puts the offset constant in r0 and the
           address in r1, as in the ROM; plain UP.upArrowDirty |= 1 swaps them. */
        u8 *p = &UP.upArrowFrame;
        if (*p != 0) {
            *p = 2;
            p[-1] |= 1;
        }
    }
    DeckEdit_ScrollFrameSlots(UP.scrollDir, *out, ((u16 *)UP.listCounts)[gUnk_0201EFC0[gUnk_0201F73C] * 3 + gUnk_0201F73C],
                 UP.frameSlots, UP.objAffineStaging);
    DeckEdit_RotateListRowRing(1);
    PlaySE(SE_CURSOR);
}
#undef UP
struct ScrollListDownView {
    u8 pad0[0x620];
    u16 listPos[3];              /* +0x620 */
    u8 pad626[2];
    u8 scrollEase[8];            /* +0x628 */
    u16 bg3Hofs;                 /* +0x630 */
    u16 bg3Vofs;                 /* +0x632 */
    u8 cardArtPage;              /* +0x634 */
    u8 scrollDir;                /* +0x635 */
    u8 pad636[0x63A - 0x636];
    u16 bg1Vofs;                 /* +0x63A */
    u8 pad63C[0x63E - 0x63C];
    u16 bg0Vofs;                 /* +0x63E */
    u8 textFlags[0x1494 - 0x640]; /* +0x640 */
    u16 listCounts[2][3];        /* +0x1494 */
    u8 listRow[0x18AC - 0x14A0]; /* +0x14A0 */
    u16 brightness;              /* +0x18AC */
    u8 pad18AE[2];
    u8 objAffineStaging[0x1BB0 - 0x18B0]; /* +0x18B0 */
    u16 scrollBar;               /* +0x1BB0 */
    u8 pad1BB2[4];
    u8 downArrowDirty;           /* +0x1BB6 */
    u8 downArrowFrame;           /* +0x1BB7 */
    u8 frameSlots[0x1C1C - 0x1BB8]; /* +0x1BB8 */
    u8 curList;                  /* +0x1C1C */
    u8 pad1C1D[0x1C58 - 0x1C1D];
    u16 nameTabTimer;            /* +0x1C58 */
};
#define DOWN (*(struct ScrollListDownView *)&gDeckEditScroll)
#define DOWN_LIST_ROW ((void (*)(u16, u32, u16, u16, u8 *, u8))DeckEdit_DrawListRowName)
#define DOWN_CURSOR_ROW ((void (*)(u16, u32, u16, u16, u8 *))DeckEdit_DrawCursorRowName)
#define DOWN_NO_CARDS ((void (*)(u16, u32, u16, u16, u8 *))DeckEdit_DrawNoCardsText)
#define DOWN_ATK_DEF ((void (*)(u32, u16, u16, u8 *))DeckEdit_DrawAtkDef)
#define DOWN_LEVEL_STARS ((void (*)(u32, u16, u16, u8))DeckEdit_DrawLevelStars)
/* Scroll the card list down by one row (shown list DOWN.curList). */
void DeckEdit_ScrollListDown(u16 *out)
{
    s32 t1, t2, t3;

    if (DOWN.listPos[DOWN.curList] >= DOWN.listCounts[DOWN.listRow[DOWN.curList]][DOWN.curList] - 1)
        return;
    DOWN.nameTabTimer = 0x1E;
    DOWN.brightness = 0xFC00;
    Ease_Start(0, 6, 1, DOWN.scrollEase);
    DOWN.cardArtPage ^= 1;
    ((LoadCardArtPageFn)LoadCardArt8bpp)(DeckEdit_GetListCard(DOWN.curList, DOWN.listRow[DOWN.curList], ++DOWN.listPos[DOWN.curList]),
                                 DOWN.cardArtPage * 0x1680 + 0x06008000, DOWN.cardArtPage);
    DeckEdit_DrawPortraitTilemap(((DOWN.bg3Hofs & 0xFF) >> 3) + 0x13, ((DOWN.bg3Vofs & 0xFF) >> 3) + 0xC, DOWN.cardArtPage, 1, 0);
    DOWN.scrollDir = 2;
    FillMapRectWrap(0, 0x0600D000, 3, (((t1 = gUnk_08087494[2]) + DOWN.bg1Vofs) & 0xFF) >> 3, 0x1B, 3, DOWN.textFlags);
    if (DOWN.listPos[DOWN.curList] + 2 < DOWN.listCounts[DOWN.listRow[DOWN.curList]][DOWN.curList]) {
        DOWN_LIST_ROW(DeckEdit_GetListCard(DOWN.curList, DOWN.listRow[DOWN.curList], DOWN.listPos[DOWN.curList] + 2),
                     0x0600D000, 0, ((DOWN.bg1Vofs + t1) & 0xFF) >> 3, DOWN.textFlags, 6);
        *out = DeckEdit_GetListCard(DOWN.curList, DOWN.listRow[DOWN.curList], DOWN.listPos[DOWN.curList] + 2);
    } else {
        *out = 0xFFFF;
    }
    FillMapRectWrap(0, 0x0600D000, 3, (((t2 = gUnk_08087494[3]) + DOWN.bg1Vofs) & 0xFF) >> 3, 0x1B, 3, DOWN.textFlags);
    if ((s16)DOWN.listPos[DOWN.curList] - 1 >= 0)
        DOWN_LIST_ROW(DeckEdit_GetListCard(DOWN.curList, DOWN.listRow[DOWN.curList], DOWN.listPos[DOWN.curList] - 1),
                     0x0600D000, 0, ((DOWN.bg1Vofs + t2) & 0xFF) >> 3, DOWN.textFlags, 3);
    FillMapRectWrap(0, 0x0600C000, 0, (((t3 = gUnk_0808749C[1]) + DOWN.bg0Vofs) & 0xFF) >> 3, 0x1E, 6, DOWN.textFlags);
    if (DOWN.listCounts[DOWN.listRow[DOWN.curList]][DOWN.curList] != 0) {
        DOWN_CURSOR_ROW(DeckEdit_GetListCard(DOWN.curList, DOWN.listRow[DOWN.curList], DOWN.listPos[DOWN.curList]),
                     0x0600C000, 0, ((DOWN.bg0Vofs + t3) & 0xFF) >> 3, DOWN.textFlags);
        DOWN_ATK_DEF(0x0600C000, 0xB, ((DOWN.bg0Vofs + 0x60) & 0xFF) >> 3, DOWN.textFlags);
        DOWN_LEVEL_STARS(0x0600C000, 0x11, ((DOWN.bg0Vofs + 0x60) & 0xFF) >> 3, 6);
        DeckEdit_DrawCardIcons(5);
        DeckEdit_CalcScrollBar(DOWN.listCounts[DOWN.listRow[DOWN.curList]][DOWN.curList], DOWN.listPos[DOWN.curList], &DOWN.scrollBar);
        if (DOWN.downArrowFrame != 0) {
            DOWN.downArrowFrame = 2;
            DOWN.downArrowDirty |= 1;
        }
        DeckEdit_ScrollFrameSlots(DOWN.scrollDir, *out, ((u16 *)DOWN.listCounts)[DOWN.listRow[DOWN.curList] * 3 + DOWN.curList], DOWN.frameSlots, DOWN.objAffineStaging);
    } else {
        DOWN_NO_CARDS(DeckEdit_GetListCard(DOWN.curList, DOWN.listRow[DOWN.curList], DOWN.listPos[DOWN.curList]),
                     0x0600C000, 0, ((DOWN.bg0Vofs + t3) & 0xFF) >> 3, DOWN.textFlags);
    }
    DeckEdit_RotateListRowRing(2);
    PlaySE(SE_CURSOR);
}
