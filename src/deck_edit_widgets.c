/*
 * deck_edit_widgets (0x08065E6C-0x0806704C): card-view widgets of the deck-edit list screens and the
 * card-move animation that carries a card between lists (wiki/functions/deck-edit-widgets-c.md).
 *
 * The drawing helpers serve the list view of deck_edit_list / deck_edit_panel:
 *   - DeckEdit_DrawLevelStars: the cursor card's level stars on the detail panel.
 *   - DeckEdit_CalcScrollBar / DeckEdit_DrawScrollBar: the scroll bar at the right edge of the list.
 *   - DeckEdit_LoadCardBoxTiles: copies the 12 card-row BG tile blocks into VRAM.
 *   - DeckEdit_ResetFrameSlots / DeckEdit_TweenFrameSlots / DeckEdit_ScrollFrameSlots /
 *     DeckEdit_DrawFrameSlots / DeckEdit_InitFrameSlot: the ring of six card-frame sprites left of
 *     the list; scrolling tweens each live slot between two ring positions (gFrameSlotY /
 *     gFrameSlotScale), and GetCardFrameIndex picks a card's frame graphic.
 * The rest is the card move started from the command bar: DeckEdit_StartCardMove arms it,
 * DeckEdit_BeginCardMove begins the pick-up animation (and hides the slot of a last copy), and
 * DeckEdit_UpdateCardMove runs the enum CardMoveStep machine that flies the card sprite to the
 * destination list icon and finally moves one copy in gSaveData.
 *
 * The state is gDeckEdit (0x0201DB20, struct DeckEdit in deck_edit.h), reached through the local
 * views below: each function only matches with its own declared field types and access forms, so
 * the views stay local instead of including deck_edit.h (build/readability/issues/deck_edit_widgets.md).
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE / CARD_STATS_KIND / CARD_STATS_LEVEL */
#include "constants/card_stats.h"   /* enum CardType, enum CardKind, enum CardFrame */
#include "constants/cards.h"        /* CARD_THE_MONARCHY, CARD_SET_SAIL_FOR_THE_KINGDOM, CARD_GLORY_OF_THE_KINGS_HAND, CARD_OBELISK_THE_TORMENTOR, CARD_SLIFER_THE_SKY_DRAGON, CARD_THE_WINGED_DRAGON_OF_RA */
#include "constants/sound.h"        /* SE_CONFIRM, SE_ERROR */
#include "legacy/gba.h"                    /* CpuSet */

/* ---- BEGIN deck_edit.h stand-in (pre-H0) ----
 * include/deck_edit.h cannot be included here: it pulls in sprite.h, util.h and palette.h, whose
 * prototypes conflict with the wide caller views this unit's matched code uses (see the local
 * prototypes below). The enums the state machines switch on are copied unchanged from
 * include/deck_edit.h so they read semantically; swap to the real header at the H0 milestone.
 * (build/readability/issues/deck_edit_widgets.md) */
enum DeckEditList { DECKEDIT_LIST_TRUNK = 0, DECKEDIT_LIST_MAIN_DECK = 1, DECKEDIT_LIST_SIDE_DECK = 2 };
enum CardMoveStep { CARD_MOVE_IDLE = 0, CARD_MOVE_CHECK = 1, CARD_MOVE_PICK_UP = 2, CARD_MOVE_FLY = 3, CARD_MOVE_LAND = 4, CARD_MOVE_COMMIT = 5 };
enum CommandMenuAnim { CMDMENU_IDLE = 0, CMDMENU_OPEN = 1, CMDMENU_CLOSE = 2, CMDMENU_REFRESH = 3 };
/* ---- END deck_edit.h stand-in ---- */

/* sound.h does not declare PlaySE; the units declare it themselves. */
void PlaySE(u32 seId);

/* ---- ROM data used only here ---- */
extern const void *gCardFrameSprites[];         /* 0x081A7144: frame sprite templates by frame index */
extern u16 gFrameSlotY[];                       /* 0x08087464: sprite y of ring positions 0..6 */
extern u16 gFrameSlotScale[];                   /* 0x08087472: 8.8 scale of ring positions 0..6 */
extern u8 gCardFrameAnimIds[];                  /* 0x08087480: pick-up animation index per enum CardFrame */
extern const u8 gCardMoveSprite[];              /* 0x081A6D84: flying card sprite of DeckEdit_UpdateCardMove */
extern u16 gDeckEditEaseCurve[];                /* 0x080875D2: [7] ease-in-out factors in 8.8 */
extern const u8 gScrollArrowTiles[];            /* 0x08087450: scroll-bar arrow tiles */
/* The 12 card-row BG tile blocks copied by DeckEdit_LoadCardBoxTiles. */
extern const u8 gDeckEditCardBoxTiles0[];
extern const u8 gDeckEditCardBoxTiles1[];
extern const u8 gDeckEditCardBoxTiles2[];
extern const u8 gDeckEditCardBoxTiles3[];
extern const u8 gDeckEditCardBoxTiles4[];
extern const u8 gDeckEditCardBoxTiles5[];
extern const u8 gDeckEditSmallCardBoxTiles0[];
extern const u8 gDeckEditSmallCardBoxTiles1[];
extern const u8 gDeckEditSmallCardBoxTiles2[];
extern const u8 gDeckEditSmallCardBoxTiles3[];
extern const u8 gDeckEditSmallCardBoxTiles4[];
extern const u8 gDeckEditSmallCardBoxTiles5[];

/* Wide caller views of helpers whose headers declare narrower prototypes (util.h has
 * MulFix8(s16, s16) and struct Ease * forms of Ease_Start / Ease_Tick; sprite.h has its own
 * OamListAddSprite* and ObjAffineApply shapes); the matched code calls them this way. */
extern int DivFix8(int a, int b);
extern int MulFix8(int a, int b);
extern u16 *OamListAddSpriteGroup(const void *a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l);
extern void OamListAddSprite(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m);
extern void ObjAffineApply(void *p);
extern void Ease_Start(int a, int b, int c, void *d);
extern void Ease_Tick(void *p);

/* Card collection in gSaveData (declared in save.h; IsBelowCardCopyLimit returns u32 there, but
 * this unit only tests the result as a boolean and matches with the u16 view). */
extern u16 IsBelowCardCopyLimit(u16 id);
extern void RemoveCardFromTrunk(u16 id);
extern void RemoveCardFromSavedFusionDeck(u16 id);
extern void RemoveCardFromSavedDeck(u16 id);
extern void RemoveCardFromSavedSideDeck(u16 id);
extern void AddCardToTrunk(u16 id);
extern void AddCardToSavedFusionDeck(u16 id);
extern void AddCardToSavedDeck(u16 id);
extern void AddCardToSavedSideDeck(u16 id);
extern void DeckEdit_BuildCardLists(void);
extern void DeckEdit_StartListSlide(u32 a);     /* deck_edit.h declares (u8); this unit pushes a u32 */
extern void DeckEdit_CountSideDeckMonsters(void);
extern u8 GetCardFrameIndex(u16 id);
extern u16 DeckEdit_GetListCard(u8 list, u8 row, u16 index);

/* ---- Local views kept for matching (build/readability/issues/deck_edit_widgets.md) ---- */

/* gDeckEdit list cursors (DeckEdit_DrawLevelStars, DeckEdit_BeginCardMove): canonical names from
 * deck_edit.h; only the fields read here are declared. */
struct ListCursorView {
    u8 pad0[0x620];
    u16 listPos[15];                /* +0x620: listPos[3], the rest of the array covers the scroll state */
    u16 bg0Vofs;                    /* +0x63E */
    u8 pad640[0x14A0 - 0x640];
    u8 listRow[0x1C1C - 0x14A0];    /* +0x14A0: listRow[3], padded out to curList */
    u8 curList;                     /* +0x1C1C: enum DeckEditList shown */
};
extern struct ListCursorView gDeckEdit;

/* The anim cells and the frame-slot ring head, as raw-byte views of gDeckEdit: the cells are the
 * 20-byte struct AnimState entries at +0x1726 (byte 0 = active), the ring head is
 * gDeckEdit.frameSlots.head (+0x1BB8). */
struct AnimCellState {
    u8 pad0[0x1726];
    u8 cells[0x1BB8 - 0x1726];      /* +0x1726: 20-byte cells; byte 0 is the "touched" flag */
    u8 frameSlotsHead;              /* +0x1BB8 */
    u8 pad1BB9[0x1C1C - 0x1BB9];
    u8 curList;                     /* +0x1C1C */
};
extern struct AnimCellState gDeckEditAnimCells asm("gDeckEdit");

/* gDeckEdit.objAffine[0] (+0x18B0), the scroll-bar thumb matrix written by DeckEdit_DrawScrollBar.
 * The pad is load-bearing: scaleX/scaleY must stay at +0x18B0/+0x18B2 in gDeckEdit. */
struct ScrollBarAffine {
    u8 pad0[0x18B0];
    u16 scaleX;                     /* +0x18B0 */
    u16 scaleY;                     /* +0x18B2 */
};
extern struct ScrollBarAffine gDeckEditScrollBarAffine asm("gDeckEdit");

/* The scroll bar at the right edge of the list (deck_edit.h struct DeckEditScrollBar): thumb
 * extents in 8.8 and the two arrow cells' dirty flags and frame indices. */
struct ScrollBarView {
    u16 thumbLen;                   /* +0x0 */
    u16 thumbPos;                   /* +0x2 */
    u8 upArrowDirty : 1;            /* +0x4 bit 0: redraw the up-arrow cell */
    u8 unk4_1 : 7;
    u8 upArrowFrame;                /* +0x5: gScrollArrowTiles index */
    u8 downArrowDirty : 1;          /* +0x6 bit 0: redraw the down-arrow cell */
    u8 unk6_1 : 7;
    u8 downArrowFrame;              /* +0x7: gScrollArrowTiles index - 3 */
};

/* Deck sizes of gSaveData (save.h: deckSize +0x20C8, sideDeckSize +0x20CA, fusionDeckSize +0x20CC),
 * the per-category limits DeckEdit_UpdateCardMove checks before a move. */
struct SaveDeckSizes {
    u8 pad0[0x20C8];
    u16 deckSize;                   /* +0x20C8 */
    u16 sideDeckSize;               /* +0x20CA */
    u16 fusionDeckSize;             /* +0x20CC */
};
extern struct SaveDeckSizes gSaveDeckSizes asm("gSaveData");

/* One card of the collection, read through a trunk pointer (save.h struct TrunkEntry at
 * gSaveData + 8 + id * 4): the count is kept in a 2-byte struct so the compiler uses ldrh (a
 * 4-byte struct would use ldr), and the copies in a plain byte (bits 2-3 deck, 4-5 side, 6-7
 * fusion). */
struct TrunkCountEntry { u8 pad0[8]; u16 count : 10; };
struct TrunkCopiesByte { u8 pad0[9]; u8 copies; };
#define TRUNK_COUNT(t, id) (((struct TrunkCountEntry *)((t) + (u16)(id) * 4))->count)
#define TRUNK_COPIES(t, id) (((struct TrunkCopiesByte *)((t) + (u16)(id) * 4))->copies)

/* The card tables are read through their literal ROM addresses: per include/card_data.h the
 * integer-constant form and the symbol form (gCardStats / gCardIdToNumber) generate different
 * code, and this unit matches with the literal form. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE_OF(id) ((int)((CARD_STATS(id) & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT))

/* One raw 16-byte frame-slot entry of the ring (deck_edit.h struct FrameSlot); the slot code
 * below accesses the fields through byte offsets. */
struct FrameSlotEntry { u8 bytes[16]; };

/* Flight targets of the moved card, one per enum DeckEditList (gCardMoveTargets). */
struct CardMoveTarget { u16 x, y; };
extern struct CardMoveTarget gCardMoveTargets[];   /* 0x08087488 */



/* Draw the cursor card's level stars (tile 0x19A) into the BG map buffer, `perRow` per row
 * starting at (col, row). Trap/Magic/Ticket cards show none, Divine cards show 10. */
void DeckEdit_DrawLevelStars(u8 *map, u16 col, u16 row, u8 perRow)
{
    u16 col0 = col;
    u8 i = 0;
    u32 id;
    const u32 *stats;
    int type;
    id = DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]);
    stats = &((const u32 *)0x08621DE0)[(id << 21) >> 21];   /* CARD_ID_MASK in shift form */
    type = CARD_STATS_TYPE(*stats);
    for (;;) {
        u8 n = i;
        u32 stars;
        int idx;
        i++;
        switch (type) {
        case CARD_TYPE_TRAP:
        case CARD_TYPE_MAGIC:
        case CARD_TYPE_TICKET:
            stars = 0;
            break;
        case CARD_TYPE_DIVINE:
            stars = 10;
            break;
        default:
            stars = CARD_STATS_LEVEL(*stats);
            break;
        }
        if (!(n < stars))
            break;
        /* FAKEMATCH (permuter): the mask goes through the dead `id` */
        id = 0x1F;
        idx = col & id;
        idx += (row & 0x1F) * 32;
        *(u16 *)(map + idx * 2) = 0x19A;
        col++;
        if (i == perRow) {
            col = col0;
            row++;
        }
    }
}

/* Scroll-bar geometry for a list of `count` cards with the cursor at `pos`: the thumb length
 * (8.8, 0xC0 / (count - 1)) and the thumb offset (thumb step * pos) into bar (struct
 * DeckEditScrollBar). */
void DeckEdit_CalcScrollBar(u16 count, u16 pos, u16 *bar)
{
    u16 n = count - 1;
    u16 thumbStep;
    int posStep;
    if (n != 0) {
        thumbStep = DivFix8(0xC0, n);
        posStep = DivFix8((0x5800 - thumbStep) >> 8, n);
    } else {
        thumbStep = 0xC0;
        posStep = 0x57;
    }
    bar[1] = posStep * pos;
    bar[0] = thumbStep;
}
/* Draw the scroll-bar thumb sprites for cursor position `pos` in a list of `count` cards, and
 * flush the arrow cells whose dirty flags are set (BG2 map cells (29, 0) and (29, 13)). */
void DeckEdit_DrawScrollBar(u16 pos, u16 count, struct ScrollBarView *bar)
{
    int len = bar->thumbLen >> 8;
    int top = bar->thumbPos >> 8;
    int botY;
    if (count == pos + 1 && top + len <= 0x57)
        top++;
    if (len != 0 && count > 3) {
        int scale = len * 8;
        int arrowY;
        int shrink = MulFix8(0x10, 0x100 - scale);
        arrowY = top - 4;
        arrowY -= shrink;
        OamListAddSprite(0, 0x89, 0xE4, arrowY & 0xFF, 8, 0x20, 4, 7, 0, 0x300, 0, 0, (int)&gDeckEditScrollBarAffine);
        gDeckEditScrollBarAffine.scaleY = scale + 0x10;
        ObjAffineApply(&gDeckEditScrollBarAffine.scaleX);
    } else if (count <= 3) {
        OamListAddSprite(0, 0x89, 0xE4, 0xC, 8, 0x20, 4, 7, 0, 0x300, 0, 0, (int)&gDeckEditScrollBarAffine);
        OamListAddSprite(0, 0x89, 0xE4, 0x24, 8, 0x20, 4, 7, 0, 0x300, 0, 0, (int)&gDeckEditScrollBarAffine);
        gDeckEditScrollBarAffine.scaleY = 0x200;
        ObjAffineApply(&gDeckEditScrollBarAffine.scaleX);
        top = 0;
        len = 0x58;
    }
    OamListAddSprite(0, 0xF, 0xE8, (top + 8) & 0xFF, 8, 8, 4, 7, 0, 0, 0, 0, (int)&gDeckEditScrollBarAffine);
    botY = top + 0xC;
    OamListAddSprite(0, 0x2F, 0xE8, (botY + len) & 0xFF, 8, 8, 4, 7, 0, 0, 0, 0, (int)&gDeckEditScrollBarAffine);
    if (bar->upArrowDirty) {
        bar->upArrowDirty = 0;
        *(u16 *)0x0600E03A = 0x5000 | gScrollArrowTiles[bar->upArrowFrame];
    }
    if (bar->downArrowDirty) {
        bar->downArrowDirty = 0;
        *(u16 *)0x0600E37A = 0x5000 | gScrollArrowTiles[bar->downArrowFrame + 3];
    }
}
/* Copy 12 graphics blocks (0x0870xxxx) into the tile area at dst: 0xC0 halfwords each for the
 * first six, 0x40 for the rest; the 9th and 10th share dst + 0xA00. */
void DeckEdit_LoadCardBoxTiles(u8 *dst)
{
    u8 *p;
    CpuSet(gDeckEditCardBoxTiles0, dst, 0xC0);
    CpuSet(gDeckEditCardBoxTiles1, dst + 0x180, 0xC0);
    CpuSet(gDeckEditCardBoxTiles2, dst + 0x300, 0xC0);
    CpuSet(gDeckEditCardBoxTiles3, dst + 0x480, 0xC0);
    CpuSet(gDeckEditCardBoxTiles4, dst + 0x600, 0xC0);
    CpuSet(gDeckEditCardBoxTiles5, dst + 0x780, 0xC0);
    CpuSet(gDeckEditSmallCardBoxTiles0, dst + 0x900, 0x40);
    CpuSet(gDeckEditSmallCardBoxTiles1, dst + 0x980, 0x40);
    CpuSet(gDeckEditSmallCardBoxTiles2, p = dst + 0xA00, 0x40);
    CpuSet(gDeckEditSmallCardBoxTiles3, p, 0x40);
    CpuSet(gDeckEditSmallCardBoxTiles4, dst + 0xA80, 0x40);
    CpuSet(gDeckEditSmallCardBoxTiles5, dst + 0xB00, 0x40);
}
/* All six frame slots off, ring head back to slot 5 (gDeckEdit.frameSlots.head). */
void DeckEdit_ResetFrameSlots(u8 *ring)
{
    u8 i;
    for (i = 0; i <= 5; i++)
        *(u8 *)((u32)ring + (i << 4) + 0xC) = 0;   /* slot[i].active; base-first adds is the matching order */
    ring[0] = 5;
}
/* Per-frame tween of the frame slots: each live slot's y (slot +0x6 in ring offsets) eases
 * toward its target by gDeckEditEaseCurve[step]; the affine cells in `affines` (24 bytes each)
 * get the slot's scale position. easeState 1 only computes, 2 also commits the target into the
 * slot and clears the entered slot. dir mirrors the step (DeckEdit_ScrollFrameSlots passes
 * 6 - step for one direction). */
void DeckEdit_TweenFrameSlots(u8 step, u8 easeState, u8 dir, u8 *affines, u8 *ring)
{
    u8 m = step;
    u8 i;
    switch (dir) {
    case 1:
        m = 6 - m;
    case 2:
        step = 6 - step;
        break;
    default:
        return;
    }
    switch (easeState) {
    case 1:
        for (i = 0; i <= 5; i++) {
            u8 *slot = ring + i * 16;
            if (slot[0xC] != 0) {
                u16 *cell;
                *(u16 *)(slot + 6) = *(u16 *)(slot + 8) + MulFix8(*(s16 *)(slot + 0xA), gDeckEditEaseCurve[step]);
                cell = (u16 *)(affines + (i + 1) * 24);
                cell[0] = cell[1] = *(u16 *)(slot + 0xE) + *(u16 *)(slot + 0x10) * m;
            }
        }
        break;
    case 2:
        for (i = 0; i <= 5; i++) {
            u8 *slot = ring + i * 16;
            if (slot[0xC] != 0) {
                s16 *cell;
                *(u16 *)(slot + 6) = *(u16 *)(slot + 8) + MulFix8(*(s16 *)(slot + 0xA), gDeckEditEaseCurve[step]);
                cell = (s16 *)(affines + (i + 1) * 24);
                cell[0] = cell[1] = *(u16 *)(slot + 0xE) + *(u16 *)(slot + 0x10) * m;
                {
                    /* Matching: the int temporary keeps the ROM's ldrsh reload of the cell. */
                    int t = *cell;
                    *(u16 *)(slot + 0xE) = t;
                }
            }
        }
        ring[ring[0] * 16 + 0xC] = 0;
        break;
    }
}

/* Card frame kind (0..9, enum CardFrame plus the ticket value 9) for card `id`; same logic as
 * the tail of DeckEdit_DrawCursorRowName. The three Championship tickets draw as normal
 * monsters and the Egyptian Gods by number; the second number/kind switches are the ROM's
 * duplicated tail (the 7/8 returns there are unreachable). */
u8 GetCardFrameIndex(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case CARD_THE_MONARCHY:
    case CARD_SET_SAIL_FOR_THE_KINGDOM:
    case CARD_GLORY_OF_THE_KINGS_HAND:
        return CARD_FRAME_NORMAL;
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_FRAME_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_FRAME_EFFECT;
    default:
        switch (CARD_TYPE_OF(id)) {
        case CARD_TYPE_TRAP:
            return CARD_FRAME_TRAP;
        case CARD_TYPE_MAGIC:
            return CARD_FRAME_MAGIC;
        }
        switch (CARD_NUMBER(id)) {
        case CARD_OBELISK_THE_TORMENTOR:
            return CARD_FRAME_RITUAL;
        case CARD_SLIFER_THE_SKY_DRAGON:
        case CARD_THE_WINGED_DRAGON_OF_RA:
            return CARD_FRAME_EFFECT;
        default:
            switch (CARD_TYPE_OF(id)) {
            case CARD_TYPE_MAGIC:
                return 7;   /* enum CardKind CARD_KIND_MAGIC; unreachable, handled above */
            case CARD_TYPE_TRAP:
                return 8;   /* CARD_KIND_TRAP; unreachable */
            case CARD_TYPE_TICKET:
                return 9;   /* CARD_KIND_TICKET */
            default:
                return CARD_STATS_KIND(CARD_STATS(id));
            }
        }
    }
}
/* Start a one-card scroll of the frame-slot ring: dir 1 (up) enters a slot at the top, dir 2
 * (down) at the bottom; newCardId 0xFFFF only rotates the ring. Slot fields are reached through
 * ring offsets (slot base = ring + 4 + i * 16): +4 pos, +5 frame, +6 y, +8 startY, +0xA deltaY,
 * +0xC active, +0xE affineIdx, +0x10 scaleStep, +0x12 next index. The two loops keep separate
 * slot pointers: one shared pointer would change the register allocation. */
void DeckEdit_ScrollFrameSlots(u8 dir, u16 newCardId, u8 unusedCount, u8 *ring, u8 *affines)
{
    u8 head = ring[0];
    u8 i;
    u8 *slot;
    u16 v;
    int m;
    u8 *cell;
    switch (dir) {
    case 1:
    {
        if (newCardId != 0xFFFF) {
            u8 *in = (u8 *)&((struct FrameSlotEntry *)ring)[head];
            in[4] = 0;
            in[0xC] = 1;
            *(u16 *)(in + 6) = gFrameSlotY[0];
            in[5] = GetCardFrameIndex(newCardId);
            v = gFrameSlotScale[0];
            *(u16 *)(in + 0xE) = v;
            m = head + 1;
            in[0x12] = m;
            cell = affines + m * 24;
            *(u16 *)(cell + 2) = v;
            *(u16 *)cell = v;
        }
        if (ring[0] != 0)
            ring[0] = ring[0] - 1;
        else
            ring[0] = 5;
        for (i = 0; i <= 5; i++) {
            slot = ring + i * 16;
            if (slot[0xC] != 0) {
                slot[4] = slot[4] + 1;
                *(u16 *)(slot + 0xA) = gFrameSlotY[slot[4]] - *(u16 *)(slot + 6);
                *(u16 *)(slot + 8) = *(u16 *)(slot + 6);
                *(u16 *)(slot + 0x10) = (gFrameSlotScale[slot[4]] - *(u16 *)(slot + 0xE)) / 6;
            }
        }
    break;
    }
    case 2:
    {
        if (newCardId != 0xFFFF) {
            u8 *in = (u8 *)&((struct FrameSlotEntry *)ring)[head];
            in[4] = 6;
            in[0xC] = 1;
            *(u16 *)(in + 6) = gFrameSlotY[6];
            in[5] = GetCardFrameIndex(newCardId);
            v = gFrameSlotScale[6];
            *(u16 *)(in + 0xE) = v;
            m = head + 1;
            in[0x12] = m;
            cell = affines + m * 24;
            *(u16 *)(cell + 2) = v;
            *(u16 *)cell = v;
        }
        ring[0] = (ring[0] + 1) % 6;
        for (i = 0; i <= 5; i++) {
            slot = ring + i * 16;
            if (slot[0xC] != 0) {
                slot[4] = slot[4] - 1;
                *(u16 *)(slot + 0xA) = *(u16 *)(slot + 6) - gFrameSlotY[slot[4]];
                *(u16 *)(slot + 8) = *(u16 *)(slot + 6) - *(u16 *)(slot + 0xA);
                *(u16 *)(slot + 0x10) = (gFrameSlotScale[slot[4]] - *(u16 *)(slot + 0xE)) / 6;
            }
        }
        break;
    }
    }
}
/* Emit the sprites of the live frame slots (slots = &gDeckEdit.frameSlots.slot[0]); the slot's
 * affine index goes into OAM attr1. The i * 16 + base adds order is the matching one here. */
void DeckEdit_DrawFrameSlots(u8 *slots, int oamList)
{
    u8 i;
    for (i = 0; i <= 5; i++) {
        u8 *slot = (u8 *)(i * 16 + (u32)slots);
        if (slot[8] != 0) {
            u16 *o = OamListAddSpriteGroup(gCardFrameSprites[slot[1]], 5, 1, -3, *(s16 *)(slot + 2), 4, 0, 0, 0, 0, 0, oamList);
            o[0] |= 0x100;
            o[1] |= slot[0xE] << 9;
        }
    }
}
/* Place card `cardId` at ring position `pos` of slot `slot` (used when the view is built). */
void DeckEdit_InitFrameSlot(u8 slot, u16 cardId, u8 pos, u8 *ring, u8 *affines)
{
    u8 *entry;
    u16 v;
    int n;
    u8 *cell;
    u8 frame = GetCardFrameIndex(cardId);
    entry = ring + slot * 16;
    entry[5] = frame;
    entry[0xC] = 1;
    entry[4] = pos;
    *(u16 *)(entry + 6) = gFrameSlotY[pos];
    v = gFrameSlotScale[pos];
    *(u16 *)(entry + 0xE) = v;
    n = slot + 1;
    entry[0x12] = n;
    cell = affines + n * 24;
    *(u16 *)(cell + 2) = v;
    *(u16 *)cell = v;
}
/* CardMove.step = CARD_MOVE_IDLE. */
void DeckEdit_ResetCardMove(u8 *move)
{
    *move = 0;
}
/* Arm a card move: step CARD_MOVE_CHECK, remember the card's frame and the destination list,
 * and set the flight offsets from the destination's icon position (gCardMoveTargets). */
void DeckEdit_StartCardMove(u8 frame, u8 destList, u8 *move)
{
    move[0] = CARD_MOVE_CHECK;
    move[0xC] = frame;
    move[0xD] = destList;
    *(u16 *)(move + 0xE) = gCardMoveTargets[destList].x + 3;
    *(u16 *)(move + 0x10) = gCardMoveTargets[destList].y - 0x28;
}
/* Begin the pick-up animation of the cursor card (DeckEdit_UpdateCardMove CARD_MOVE_CHECK).
 * Marks the card's frame cell touched; if the cursor holds the card's last copy (the per-list
 * copy count, read from the trunk entry, is 1), also clears the ring flag of the slot it sat in.
 * The editor keeps curList in 0..2 and every branch materializes the trunk pointer before
 * reading it; no compiler hints are needed. */
void DeckEdit_BeginCardMove(u8 *move)
{
    u32 copies;
    u8 *trunk;
    u32 id;
    int frameKind;
    u32 stateBase = (u32)&gDeckEdit;
    u32 categoryIndex;
    u8 *category;

    categoryIndex = gCardFrameAnimIds[move[0xC]];
    category = (u8 *)(stateBase + categoryIndex * 20);
    category[0x1726] = 1;
    switch (gDeckEdit.curList) {
    case DECKEDIT_LIST_TRUNK:
        trunk = (u8 *)&gSaveDeckSizes;

        copies = TRUNK_COUNT(trunk, DeckEdit_GetListCard(0, gDeckEdit.listRow[0], gDeckEdit.listPos[0]));
        break;
    case DECKEDIT_LIST_MAIN_DECK:
        id = (u16)DeckEdit_GetListCard(1, gDeckEdit.listRow[1], gDeckEdit.listPos[1]);
        switch (CARD_NUMBER(id)) {
        case CARD_OBELISK_THE_TORMENTOR:
            frameKind = CARD_KIND_RITUAL;
            break;
        case CARD_SLIFER_THE_SKY_DRAGON:
        case CARD_THE_WINGED_DRAGON_OF_RA:
            frameKind = CARD_KIND_EFFECT;
            break;
        default:
            switch (CARD_TYPE_OF(id)) {
            case CARD_TYPE_MAGIC:
                frameKind = CARD_KIND_MAGIC;
                break;
            case CARD_TYPE_TRAP:
                frameKind = CARD_KIND_TRAP;
                break;
            case CARD_TYPE_TICKET:
                frameKind = CARD_KIND_TICKET;
                break;
            default:
                frameKind = CARD_STATS_KIND(CARD_STATS(id));
                break;
            }
            break;
        }
        if (frameKind == CARD_KIND_FUSION) {
            trunk = (u8 *)&gSaveDeckSizes;

            copies = TRUNK_COPIES(trunk, DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList])) >> 6;
        } else {
            trunk = (u8 *)&gSaveDeckSizes;

            copies = ((u32)TRUNK_COPIES(trunk, DeckEdit_GetListCard(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList])) << 28) >> 30;
        }
        break;
    case DECKEDIT_LIST_SIDE_DECK:
        trunk = (u8 *)&gSaveDeckSizes;

        copies = ((u32)TRUNK_COPIES(trunk, DeckEdit_GetListCard(2, gDeckEdit.listRow[2], gDeckEdit.listPos[2])) << 26) >> 30;
        break;
    }
    if (copies == 1) {
        struct AnimCellState *ring = &gDeckEditAnimCells;
        u32 address = ((ring->frameSlotsHead + 3) % 6) * 16;
        address += (u32)ring;
        address += 0x1BC4;
        *(u8 *)address = 0;
    }
    move[0]++;
}

/* 1 if card `id` (not a monster-frame kind 0x15..0x17) has frame kind 2, else 0. */
u16 DeckEdit_IsFusionMonster(u16 id)
{
    int kind;
    switch (CARD_TYPE_OF(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    }
    switch (CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR:
        kind = CARD_KIND_RITUAL;
        break;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        kind = CARD_KIND_EFFECT;
        break;
    default:
        switch (CARD_TYPE_OF(id)) {
        case CARD_TYPE_MAGIC:
            kind = CARD_KIND_MAGIC;
            break;
        case CARD_TYPE_TRAP:
            kind = CARD_KIND_TRAP;
            break;
        case CARD_TYPE_TICKET:
            kind = CARD_KIND_TICKET;
            break;
        default:
            kind = CARD_STATS_KIND(CARD_STATS(id));
            break;
        }
        break;
    }
    if (kind == CARD_KIND_FUSION)
        return 1;
    return 0;
}
/* gDeckEdit as DeckEdit_UpdateCardMove reads it (field names from deck_edit.h). The anim cells
 * are the 20-byte struct AnimState entries of gDeckEdit.anims: agbcc aligns the struct to 4, so
 * the cell array starts at +0x1724 and active lands on +0x1726. The command-menu bytes at
 * +0x1C3C.. are gDeckEdit.commandMenu (filter/sort are the per-list List Filter choices). */
struct AnimCell {
    u8 pad0[2];
    s8 active;                      /* +0x1726 */
    u8 timer;
    u8 pad4[16];
};
struct CardMoveScreenView {
    u8 pad0[0x620];
    u16 listPos[3];                 /* +0x620 */
    u8 pad626[0x1494 - 0x626];
    u16 listCount[2][3];            /* +0x1494 */
    u8 listRow[3];                  /* +0x14A0 */
    u8 pad14A3[0x1724 - 0x14A3];
    struct AnimCell cells[63];      /* +0x1724, 20 bytes each */
    u8 pad1C10[0x1C1C - 0x1C10];
    u8 curList;                     /* +0x1C1C */
    u8 pad1C1D[0x1C3C - 0x1C1D];
    u8 commandTimer;                /* +0x1C3C: commandMenu.timer */
    u8 commandAnim : 3;             /* +0x1C3D bits 0-2: enum CommandMenuAnim */
    u8 rest : 5;
    u8 unk1C3E;
    u8 filter[3];                   /* +0x1C3F: commandMenu.filter */
    u8 sort[3];                     /* +0x1C42: commandMenu.sort */
};
/* The anim cell's active byte as a full-byte bitfield: the `|= 0xFF` store below keeps the
 * ROM's dead ldrb only through this form. */
struct AnimCellActive { u8 active : 8; };
#define DE (*(struct CardMoveScreenView *)&gDeckEdit)
#define CARDC() DeckEdit_GetListCard(DE.curList, DE.listRow[DE.curList], DE.listPos[DE.curList])
/* Per-frame card-move machine (move[0] = enum CardMoveStep, run after DeckEdit_StartCardMove):
 * CARD_MOVE_CHECK gates on the per-category limits in gSaveData (fusion deck 20, deck 60, side
 * deck 15, plus the copy limit when moving out of the trunk) and either starts the move
 * (DeckEdit_BeginCardMove + SE_CONFIRM) or cancels (SE_ERROR); CARD_MOVE_PICK_UP claims the
 * card's anim cell and starts the flight ease; CARD_MOVE_FLY flies the card sprite until the
 * ease finishes; CARD_MOVE_LAND restarts the destination list icon; CARD_MOVE_COMMIT moves one
 * copy in gSaveData, clears the destination's filter/sort marks, and refreshes the lists.
 * Trunk pointers are scoped per case so &listRow gets r5 and the trunk r4. */
void DeckEdit_UpdateCardMove(u8 *move)
{
    u16 isFusion;
    u32 copies;
    u32 id;
    int frameKind;

    switch (move[0]) {
    case CARD_MOVE_IDLE:
        return;
    case CARD_MOVE_CHECK:
        switch (move[0xD]) {
        case DECKEDIT_LIST_TRUNK:
            DeckEdit_BeginCardMove(move);
            PlaySE(SE_CONFIRM);
            break;
        case DECKEDIT_LIST_MAIN_DECK:
            isFusion = DeckEdit_IsFusionMonster(CARDC());
            if (isFusion != 0) {
                if (gSaveDeckSizes.fusionDeckSize < 0x14) {   /* FUSION_DECK_MAX_CARDS */
                    switch (DE.curList) {
                    case DECKEDIT_LIST_TRUNK:
                        if (IsBelowCardCopyLimit(DeckEdit_GetListCard(0, DE.listRow[0], DE.listPos[0]))) {
                            DeckEdit_BeginCardMove(move);
                            PlaySE(SE_CONFIRM);
                        } else {
                            move[0] = CARD_MOVE_IDLE;
                            PlaySE(SE_ERROR);
                        }
                        break;
                    case DECKEDIT_LIST_MAIN_DECK:
                    case DECKEDIT_LIST_SIDE_DECK:
                        DeckEdit_BeginCardMove(move);
                        PlaySE(SE_CONFIRM);
                        break;
                    }
                } else {
                    move[0] = CARD_MOVE_IDLE;
                    PlaySE(SE_ERROR);
                }
            } else {
                if (gSaveDeckSizes.deckSize < 0x3C) {   /* DECK_MAX_CARDS */
                    switch (DE.curList) {
                    case DECKEDIT_LIST_TRUNK:
                        if (IsBelowCardCopyLimit(DeckEdit_GetListCard(0, DE.listRow[0], DE.listPos[0]))) {
                            DeckEdit_BeginCardMove(move);
                            PlaySE(SE_CONFIRM);
                        } else {
                            move[0] = CARD_MOVE_IDLE;
                            PlaySE(SE_ERROR);
                        }
                        break;
                    case DECKEDIT_LIST_MAIN_DECK:
                    case DECKEDIT_LIST_SIDE_DECK:
                        DeckEdit_BeginCardMove(move);
                        PlaySE(SE_CONFIRM);
                        break;
                    }
                } else {
                    move[0] = CARD_MOVE_IDLE;
                    PlaySE(SE_ERROR);
                }
            }
            break;
        case DECKEDIT_LIST_SIDE_DECK:
            if (gSaveDeckSizes.sideDeckSize < 0xF) {   /* SIDE_DECK_MAX_CARDS */
                switch (DE.curList) {
                case DECKEDIT_LIST_TRUNK:
                    if (IsBelowCardCopyLimit(DeckEdit_GetListCard(0, DE.listRow[0], DE.listPos[0]))) {
                        DeckEdit_BeginCardMove(move);
                        PlaySE(SE_CONFIRM);
                    } else {
                        move[0] = CARD_MOVE_IDLE;
                        PlaySE(SE_ERROR);
                    }
                    break;
                case DECKEDIT_LIST_MAIN_DECK:
                case DECKEDIT_LIST_SIDE_DECK:
                    DeckEdit_BeginCardMove(move);
                    PlaySE(SE_CONFIRM);
                    break;
                }
            } else {
                move[0] = CARD_MOVE_IDLE;
                PlaySE(SE_ERROR);
            }
            break;
        }
    case CARD_MOVE_PICK_UP:
        {
            u32 stateBase = (u32)&DE;
            u32 k = gCardFrameAnimIds[move[0xC]];
            s8 *cell = (s8 *)(stateBase + k * 20) + 0x1726;
            if (*cell != 0)
                return;
            ((struct AnimCellActive *)cell)->active |= 0xFF;
        }
        Ease_Start(0, 6, 1, move + 4);
        move[0]++;
    case CARD_MOVE_FLY: {
        int x, y;
        x = (MulFix8(*(s16 *)(move + 0xE) << 8, gDeckEditEaseCurve[*(s16 *)(move + 6)]) >> 8) - 3;
        y = (MulFix8(*(s16 *)(move + 0x10) << 8, gDeckEditEaseCurve[*(s16 *)(move + 6)]) >> 8) + 0x28;
        OamListAddSpriteGroup(gCardMoveSprite, 0, 1, x, y, 4, 0, 0, 0, 0, 0, (int)&DE);
        Ease_Tick(move + 4);
        if (move[4] != 2)
            return;
        move[0]++;
        break;
    }
    case CARD_MOVE_LAND:
        DE.cells[move[0xD] + 10].active = 1;
        DE.cells[move[0xD] + 10].timer = 0;
        move[0]++;
        break;
    case CARD_MOVE_COMMIT:
        if (DE.cells[move[0xD] + 10].active != 0)
            return;
        move[0] = CARD_MOVE_IDLE;
        switch (DE.curList) {
        case DECKEDIT_LIST_TRUNK:
            RemoveCardFromTrunk(CARDC());
            break;
        case DECKEDIT_LIST_MAIN_DECK:
            if (DeckEdit_IsFusionMonster(CARDC()))
                RemoveCardFromSavedFusionDeck(CARDC());
            else
                RemoveCardFromSavedDeck(CARDC());
            break;
        case DECKEDIT_LIST_SIDE_DECK:
            RemoveCardFromSavedSideDeck(CARDC());
            break;
        }
        switch (move[0xD]) {
        case DECKEDIT_LIST_TRUNK:
            AddCardToTrunk(CARDC());
            break;
        case DECKEDIT_LIST_MAIN_DECK:
            if (DeckEdit_IsFusionMonster(CARDC()))
                AddCardToSavedFusionDeck(CARDC());
            else
                AddCardToSavedDeck(CARDC());
            break;
        case DECKEDIT_LIST_SIDE_DECK:
            AddCardToSavedSideDeck(CARDC());
            break;
        }
        DE.listRow[move[0xD]] = 0;
        DE.filter[move[0xD]] = 0;
        DE.sort[move[0xD]] = 0;
        DE.commandAnim = CMDMENU_REFRESH;
        switch (DE.curList) {
        case DECKEDIT_LIST_TRUNK:
            {
                u8 *trunk = (u8 *)&gSaveDeckSizes;
                copies = TRUNK_COUNT(trunk, DeckEdit_GetListCard(0, DE.listRow[0], DE.listPos[0]));
            }
            break;
        case DECKEDIT_LIST_MAIN_DECK:
            id = (u16)DeckEdit_GetListCard(1, DE.listRow[1], DE.listPos[1]);
            switch (CARD_NUMBER(id)) {
            case CARD_OBELISK_THE_TORMENTOR:
                frameKind = CARD_KIND_RITUAL;
                break;
            case CARD_SLIFER_THE_SKY_DRAGON:
            case CARD_THE_WINGED_DRAGON_OF_RA:
                frameKind = CARD_KIND_EFFECT;
                break;
            default:
                switch (CARD_TYPE_OF(id)) {
                case CARD_TYPE_MAGIC:
                    frameKind = CARD_KIND_MAGIC;
                    break;
                case CARD_TYPE_TRAP:
                    frameKind = CARD_KIND_TRAP;
                    break;
                case CARD_TYPE_TICKET:
                    frameKind = CARD_KIND_TICKET;
                    break;
                default:
                    frameKind = CARD_STATS_KIND(CARD_STATS(id));
                    break;
                }
                break;
            }
            if (frameKind == CARD_KIND_FUSION) {
                u8 *trunk = (u8 *)&gSaveDeckSizes;
                copies = TRUNK_COPIES(trunk, CARDC()) >> 6;
            } else {
                u8 *trunk = (u8 *)&gSaveDeckSizes;
                copies = ((u32)TRUNK_COPIES(trunk, CARDC()) << 28) >> 30;
            }
            break;
        case DECKEDIT_LIST_SIDE_DECK:
            {
                u8 *trunk = (u8 *)&gSaveDeckSizes;
                copies = ((u32)TRUNK_COPIES(trunk, DeckEdit_GetListCard(2, DE.listRow[2], DE.listPos[2])) << 26) >> 30;
            }
            break;
        }
        DeckEdit_BuildCardLists();
        if (DE.listCount[DE.listRow[DE.curList]][DE.curList] == DE.listPos[DE.curList]) {
            if (DE.listCount[DE.listRow[DE.curList]][DE.curList] == 0)
                DE.listPos[DE.curList] = 0;
            else
                DE.listPos[DE.curList] = DE.listCount[DE.listRow[DE.curList]][DE.curList] - 1;
        }
        if (copies == 0)
            DeckEdit_StartListSlide(2);
        DeckEdit_CountSideDeckMonsters();
        break;
    default:
        move[0] = CARD_MOVE_IDLE;
        break;
    }
}
#undef DE
#undef CARDC
