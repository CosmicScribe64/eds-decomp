/*
 * deck_edit_filter (0x08069284-0x0806A92B): the List Filter screen of the deck-edit card
 * lists (wiki/functions/deck-edit-filter-c.md).
 *
 * DeckEdit_FilterAndSortList rebuilds the three card lists, copies one list (enum DeckEditList)
 * to its filtered row keeping the cards of one enum ListFilter category, then sorts it by
 * enum ListSort (QuickSortS16 with the CompareCardsBy* comparators of deck_edit_cards).
 * The rest is the screen itself: ListFilter_Reset and ListFilter_Init set it up,
 * ListFilter_Update runs the filter page (LIST_FILTER_PHASE_FILTER), the sort page
 * (LIST_FILTER_PHASE_SORT) and the 'Now Filtering' apply page (LIST_FILTER_PHASE_APPLY)
 * over gDeckEdit, and ListFilter_ProgressBarVBlank animates the progress bar while the
 * rebuild runs. The matching shapes (the u16 loop counters, the literal card-table
 * addresses, the word-form helper calls) are documented where they live.
 */
#include "global.h"
#include "bg.h"                     /* CopyMapRect, CopyMapRectAddOffset, CopyTileSheetTo2D (CropMapBlock: word form below) */
#include "card_data.h"              /* CARD_ID_MASK (the card tables below are read through literal addresses) */
#include "constants/card_stats.h"   /* enum CardType, enum CardKind, CARD_STATS_TYPE_*, CARD_STATS_KIND_* */
#include "constants/cards.h"        /* CARD_OBELISK_THE_TORMENTOR, CARD_SLIFER_THE_SKY_DRAGON, CARD_THE_WINGED_DRAGON_OF_RA */
#include "constants/sound.h"        /* SE_CURSOR, SE_CONFIRM, SE_CANCEL */
#include "legacy/gba.h"                    /* REG_BG0CNT..., REG_DISPCNT, A_BUTTON, B_BUTTON, DPAD_*, CpuSet, CpuFastSet */
#include "deck_edit.h"              /* struct DeckEdit gDeckEdit, struct SpriteDef, enum ListFilter, enum ListSort, enum ListFilterPhase, the screen prototypes */
#include "legacy/main.h"                   /* struct Main gMain */
#include "palette.h"                /* struct Fade, FadeStart, FadeTick, SetBldAlpha, enum FadeState */
#include "sprite.h"                 /* struct AnimBlock, struct AnimState, AnimBlockInit, AnimBlockTick, AnimBlockDraw, OamListFlush, OamListClear (OamListAddSpriteGroup: word form below) */

/* sound.h does not declare PlaySE; the units declare it themselves. */
void PlaySE(u32 seId);

/* ---- ROM data used only here ---- */
extern u16 gDeckEditSortScratch[];   /* 0x0201EFC4 = gDeckEdit.sortScratch: base of the list-row
                                 pointer arithmetic in DeckEdit_FilterAndSortList */
extern const struct SpriteDef gListFilterCursorSprites[];   /* 0x081A715C */
extern const struct SpriteDef gListFilterFlashSprites[];    /* 0x081A71CC */
extern const u8 gListFilterAnimScripts[];       /* 0x081A6118 */
extern const u8 gListFilterBgPatternMap[];      /* 0x086FC060 */
extern const u8 gListFilterPanelMap[];                /* 0x086FC0E0 */
extern const u8 gListFilterFilterPageMap[];     /* 0x086FC590 */
extern const u8 gListFilterSortPageMap[];       /* 0x086FCA40 */
extern const u8 gListFilterListIconMap[];       /* 0x086FD850 */
extern const u8 gListFilterNowFilteringMap[];   /* 0x086FD580 */
extern const u8 gListFilterBarFrameMap[];       /* 0x086FD0D0 */
extern const u8 gListFilterBarFillTiles[];      /* 0x086F41A0 */
extern const u8 gListFilterBgTiles0[];          /* 0x086F2060 */
extern const u8 gListFilterBgTiles1[];          /* 0x086F4060 */
extern const u8 gListFilterBgTiles2[];          /* 0x086F6060 */
extern const u8 gListFilterObjTiles0[];         /* 0x086F8060 */
extern const u8 gListFilterObjTiles1[];         /* 0x086FA060 */
extern const u8 gListFilterBgPal[];             /* 0x086F1C60 */
extern const u8 gListFilterObjPal[];            /* 0x086F1E60 */
extern const u8 gListFilterNav[][4];            /* 0x0808756C: filter cursor neighbours */
extern const u8 gListFilterNavNoFusion[][4];    /* 0x08087588: same without the Fusion option */
extern const u8 gListSortNav[][4];              /* 0x080875A4: sort cursor neighbours */
extern const u8 gListSortOptionAnims[];         /* 0x080875BC: sort option -> animation index */

/* ---- Local views kept for matching (build/readability/issues/deck_edit_filter.md) ---- */

/* The card tables are read through their literal ROM addresses: per include/card_data.h
 * the integer-constant form and the symbol form (gCardStats / gCardIdToNumber) generate
 * different code, and this unit matches with the literal form. */
#define CARD_STATS(id)  (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])   /* gCardStats */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])   /* gCardIdToNumber */
#define CARD_TYPE(id)   ((int)((CARD_STATS(id) & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT))

/* OamListAddSpriteGroup as ListFilter_DrawCursor and ListFilter_DrawCursorFlash call it: the
 * (-1, -1) position is passed as words without narrowing; with sprite.h's u16 x/y the -1 is
 * loaded from the literal pool instead of negated in a register (same form as dice_scene). */
extern u16 *OamListAddSpriteGroupWide(u16 *tmpls, u32 layer, u32 count, s32 x, s32 y, u32 mode, u32 priority,
                                      u32 sheetX, u32 sheetY, u32 format, u32 attr0Flags, void *list)
    asm("OamListAddSpriteGroup");

/* CropMapBlock with word-valued scalar arguments: include/bg.h's u16 sx/sy make the compiler
 * narrow curList * 5 and emit the add in the other operand order (same form as deck_edit_stats). */
extern void CropMapBlockWord(u16 *srcMap, int sx, int sy, int srcW, void *dstMap, int dx, int dy,
                             int w, int h, int mapSize) asm("CropMapBlock");

/* Frame kind of a card (enum CardKind): direct returns keep jump2 from threading the `== k`
 * test. The three God cards are keyed by number: Obelisk counts as Ritual, Slifer and Ra
 * as Effect (include/constants/card_stats.h). */
static inline u8 FS_FrameKind(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR: return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA: return CARD_KIND_EFFECT;
    default:
        switch (CARD_TYPE(id)) {
        case CARD_TYPE_MAGIC: return CARD_KIND_MAGIC;
        case CARD_TYPE_TRAP: return CARD_KIND_TRAP;
        case CARD_TYPE_TICKET: return CARD_KIND_TICKET;
        default: return (CARD_STATS(id) & CARD_STATS_KIND_MASK) >> CARD_STATS_KIND_SHIFT;
        }
    }
}
#define FS_COUNT gDeckEdit.listCount[0][list]
#define FS_KIND(kindValue) \
    for (i = 0; i < FS_COUNT; i++) { \
        u16 card = src[i]; \
        switch ((u8)CARD_TYPE(card)) { \
        case CARD_TYPE_TRAP: \
        case CARD_TYPE_MAGIC: \
            break; \
        default: \
            if (FS_FrameKind(src[i]) == (kindValue)) \
                dst[n++] = src[i]; \
            break; \
        } \
    }
#define FS_TYPE(typeValue) \
    for (i = 0; i < FS_COUNT; i++) { \
        u16 card = src[i]; \
        if ((u8)CARD_TYPE(card) == (typeValue)) \
            dst[n++] = card; \
    }
/* Deck-edit filter and sort of card list `list`: rebuilds the lists, copies list `list` (u16 ids) to a scratch array,
   keeps the cards of category `filter` (enum ListFilter) and sorts by `sort` (enum ListSort). The list rows are
   reached at negative offsets from gDeckEditSortScratch (the matching pointer form; wiki/functions/deck-edit-filter-c.md). */
void DeckEdit_FilterAndSortList(u8 list, u8 filter, u8 sort)
{
    u16 *src;
    u16 *dst;
    int n = 0;
    u16 n2 = 0;
    u16 i;
    u16 *base = gDeckEditSortScratch;

    DeckEdit_BuildCardLists();
    switch (list) {
    case DECKEDIT_LIST_TRUNK:
        src = base - 0x730;
        dst = base - 0x39C;
        break;
    case DECKEDIT_LIST_MAIN_DECK:
        src = base - 0x3FB;
        dst = base - 0x67;
        break;
    case DECKEDIT_LIST_SIDE_DECK:
        src = base - 0x3AB;
        dst = base - 0x17;
        break;
    }
    CpuSet(src, dst, gDeckEdit.listCount[0][list]);
    switch (filter) {
    case LIST_FILTER_ALL:
        if (sort == LIST_SORT_NAME)
            break;
        for (i = 0; i < FS_COUNT; i++) {
            u16 card = src[i];

            switch (CARD_TYPE(card)) {
            case CARD_TYPE_TRAP:
            case CARD_TYPE_MAGIC:
            case CARD_TYPE_TICKET:
            case CARD_TYPE_DIVINE:
                base[n2++] = card;
                break;
            default:
                dst[n++] = src[i];
                break;
            }
        }
        break;
    case LIST_FILTER_NORMAL:
        FS_KIND(CARD_KIND_NORMAL);
        break;
    case LIST_FILTER_EFFECT:
        FS_KIND(CARD_KIND_EFFECT);
        break;
    case LIST_FILTER_FUSION:
        FS_KIND(CARD_KIND_FUSION);
        break;
    case LIST_FILTER_MAGIC:
        FS_TYPE(CARD_TYPE_MAGIC);
        break;
    case LIST_FILTER_TRAP:
        FS_TYPE(CARD_TYPE_TRAP);
        break;
    case LIST_FILTER_RITUAL:
        for (i = 0; i < FS_COUNT; i++) {
            if (FS_FrameKind(src[i]) == CARD_KIND_RITUAL)
                dst[n++] = src[i];
        }
        break;
    case LIST_FILTER_MONSTERS:
        for (i = 0; i < FS_COUNT; i++) {
            u16 card = src[i];

            switch (CARD_TYPE(card)) {
            case CARD_TYPE_TRAP:
            case CARD_TYPE_MAGIC:
            case CARD_TYPE_TICKET:
            case CARD_TYPE_DIVINE:
                break;
            default:
                dst[n++] = src[i];
                break;
            }
        }
        break;
    }
    if (filter == LIST_FILTER_ALL) {
        if (sort == LIST_SORT_NAME) {
            gDeckEdit.listCount[1][list] = gDeckEdit.listCount[0][list];
            n = gDeckEdit.listCount[0][list];
        } else {
            gDeckEdit.listCount[1][list] = n;
        }
    } else {
        gDeckEdit.listCount[1][list] = n;
    }
    switch (sort) {
    case LIST_SORT_NAME:
        gDeckEdit.listRow[list] = 1;
        break;
    case LIST_SORT_ATK:
        QuickSortS16(n, (s16 *)dst, (u16 (*)(s16, s16))CompareCardsByAtk);
        gDeckEdit.listRow[list] = 1;
        break;
    case LIST_SORT_DEF:
        QuickSortS16(n, (s16 *)dst, (u16 (*)(s16, s16))CompareCardsByDef);
        gDeckEdit.listRow[list] = 1;
        break;
    case LIST_SORT_TYPE:
        QuickSortS16(n, (s16 *)dst, (u16 (*)(s16, s16))CompareCardsByType);
        gDeckEdit.listRow[list] = 1;
        break;
    case LIST_SORT_ATTRIBUTE:
        QuickSortS16(n, (s16 *)dst, (u16 (*)(s16, s16))CompareCardsByAttribute);
        gDeckEdit.listRow[list] = 1;
        break;
    case LIST_SORT_LEVEL:
        QuickSortS16(n, (s16 *)dst, (u16 (*)(s16, s16))CompareCardsByLevel);
        gDeckEdit.listRow[list] = 1;
        break;
    }
    if (filter == LIST_FILTER_ALL && sort != LIST_SORT_NAME) {
        int total;

        i = 0;
        total = n2 + n;
        for (; i < n2; i++)
            dst[n + i] = base[i];
        gDeckEdit.listCount[1][list] = total;
    }
    gDeckEdit.listPos[list] = 0;
}
/* Clears the flag block at +0x1C49..+0x1C53 of the scene state; returns 1. */
int ListFilter_Reset(void)
{
    gDeckEdit.bgScrollY = 0;
    gDeckEdit.bgScrollX = 0;
    gDeckEdit.filterSel = 0;
    gDeckEdit.sortSel = 0;
    gDeckEdit.panelShown = 0;
    gDeckEdit.inputLock = 0;
    gDeckEdit.subPhase = 0;
    return 1;
}
/* Scene init: clears VRAM, loads the tile maps / graphics / palettes of the screen, marks 14 objects and sets up the BG registers. */
int ListFilter_Init(void)
{
    vu32 z1;
    vu32 z2;
    u16 i;
    u16 j;

    z1 = 0;
    CpuFastSet((void *)&z1, (void *)0x06000000, 0x01004000);
    z2 = 0;
    CpuFastSet((void *)&z2, (void *)0x06010000, 0x01002000);
    for (i = 0; i <= 3; i++) {
        for (j = 0; j <= 3; j++)
            CopyMapRectAddOffset((u16 *)gListFilterBgPatternMap, (u16 *)0x0600F000 + (j * 8 + i * 256), 8, 8, 8, 0, 0);
    }
    CopyMapRect((void *)gListFilterPanelMap, (void *)0x0600E000, 0x1E, 0x14);
    CopyMapRect((void *)gListFilterFilterPageMap, (void *)0x0600D000, 0x1E, 0x14);
    CopyMapRect((void *)gListFilterSortPageMap, (void *)0x0600C000, 0x1E, 0x14);
    CropMapBlockWord((u16 *)gListFilterListIconMap, 0, gDeckEdit.curList * 5, 7, (void *)0x0600D000, 0x14, 0, 7, 5, 0);
    CropMapBlockWord((u16 *)gListFilterListIconMap, 0, gDeckEdit.curList * 5, 7, (void *)0x0600C000, 0x14, 0, 7, 5, 0);
    CpuFastSet(gListFilterBgTiles0, (void *)0x06000000, 0x800);
    CpuFastSet(gListFilterBgTiles1, (void *)0x06002000, 0x800);
    CpuFastSet(gListFilterBgTiles2, (void *)0x06004000, 0x800);
    CopyTileSheetTo2D((u8 *)gListFilterObjTiles0, (void *)0x06010000, 0x10);
    CopyTileSheetTo2D((u8 *)gListFilterObjTiles1, (void *)0x06010200, 0x10);
    AnimBlockInit((struct AnimSeq **)gListFilterAnimScripts, (u8 *)&gDeckEdit.anims);
    gDeckEdit.anims.anims[0].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[1].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[2].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[3].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[4].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[5].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[6].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[7].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[8].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[9].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[10].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[11].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[12].active |= ANIM_HIDDEN;
    gDeckEdit.anims.anims[13].active |= ANIM_HIDDEN;
    CpuFastSet(gListFilterBgPal, (void *)0x05000000, 0x80);
    CpuFastSet(gListFilterObjPal, (void *)0x05000200, 0x80);
    REG_BG0CNT = 0x1800;
    REG_BG1CNT = 0x1A01;
    REG_BG2CNT = 0x1C02;
    REG_BG3CNT = 0x1E02;
    FadeStart(0, -0x180, 0, &gDeckEdit.fade);
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = 0;
    gDeckEdit.panelAlpha = 0x10;
    REG_DISPCNT = 0x1A00;
    return 1;
}
/* Creates the sprite group for descriptor `idx` of table gListFilterCursorSprites (0x081A715C). */
void ListFilter_DrawCursor(u8 idx)
{
    OamListAddSpriteGroupWide((u16 *)gListFilterCursorSprites[idx].gfx, 1, gListFilterCursorSprites[idx].count, -1, -1, 3, 2, 0, 0, 0, 0x400, &gDeckEdit.oamList);
}
/* Same with table gListFilterFlashSprites (0x081A71CC). */
void ListFilter_DrawCursorFlash(u8 idx)
{
    OamListAddSpriteGroupWide((u16 *)gListFilterFlashSprites[idx].gfx, 1, gListFilterFlashSprites[idx].count, -1, -1, 3, 2, 0, 0, 0, 0, &gDeckEdit.oamList);
}
/* Loads two 30x8 tile maps into BG map rows and clears the vblank callback. */
void ListFilter_ShowNowFiltering(void)
{
    CopyMapRect((void *)gListFilterNowFilteringMap, (void *)0x0600C200, 30, 8);
    CopyMapRect((void *)gListFilterBarFrameMap, (void *)0x0600E200, 30, 8);
    gMain.vblankCallback = NULL;
}
/* Converts 8 entries of 4 bytes from `src` to `dst` (u16 pairs) by `mode` (1..8): mask/zero-extend the first half-word
   and optionally swap it behind the second one. */
void ListFilter_CopyBarTileColumns(u16 *src, u16 *dst, u8 mode)
{
    int i;

    for (i = 0; i <= 7; i++) {
        switch (mode) {
        case 1:
            dst[0] = 0xF & src[0];
            break;
        case 2:
            dst[0] = (u8)src[0];
            break;
        case 3:
            dst[0] = 0xFFF & src[0];
            break;
        case 4:
            dst[0] = src[0];
            break;
        case 5:
            dst[0] = src[1];
            dst[1] = 0xF & src[0];
            break;
        case 6:
            dst[0] = src[1];
            dst[1] = (u8)src[0];
            break;
        case 7:
            dst[0] = src[1];
            dst[1] = 0xFFF & src[0];
            break;
        case 8:
            dst[0] = src[1];
            dst[1] = src[0];
            break;
        }
        dst += 2;
        src += 2;
    }
}
/* Copies a block of (n >> 3) groups of 8 entries plus (n & 7) entries with ListFilter_CopyBarTileColumns, in two planes 0x200 bytes apart. */
void ListFilter_DrawProgressBar(const void *src, void *dst, u8 n)
{
    u8 groups = n >> 3;
    u8 rest = n & 7;
    u8 i;

    for (i = 0; i < groups; i++) {
        ListFilter_CopyBarTileColumns((u16 *)src, dst, 8);
        ListFilter_CopyBarTileColumns((u16 *)((u8 *)src + 0x200), (u16 *)((u8 *)dst + 0x200), 8);
        dst = (u8 *)dst + 0x20;
    }
    ListFilter_CopyBarTileColumns((u16 *)src, dst, rest);
    ListFilter_CopyBarTileColumns((u16 *)((u8 *)src + 0x200), (u16 *)((u8 *)dst + 0x200), rest);
}
/* Advances gDeckEdit.animCounter and redraws the bar at (counter * 3) & 0x7F groups. */
void ListFilter_ProgressBarVBlank(void)
{
    gDeckEdit.animCounter++;
    ListFilter_DrawProgressBar(gListFilterBarFillTiles, (void *)0x06003C00, (gDeckEdit.animCounter * 3) & 0x7F);
}
/* Filter/sort menu frame handler: scrolls BG3, moves the filter (LIST_FILTER_PHASE_FILTER) and sort
   (LIST_FILTER_PHASE_SORT) cursors through the neighbour tables, applies the choice with
   DeckEdit_FilterAndSortList (LIST_FILTER_PHASE_APPLY), runs the 13-frame confirm timer and the fades.
   Returns 1 when the exit transition is done. The `case LIST_SORT_TYPE..LIST_SORT_LEVEL` switches give
   the ROM's signed bound tests (as `case 4..5` / `case 4..6` in d_ui_pick; wiki/functions/deck-edit-filter-c.md). */
int ListFilter_Update(void)
{
    u32 keys;

    keys = gMain.newKeys & 0x3FF;
    FadeTick(&gDeckEdit.fade);
    gDeckEdit.bgScrollX += 0x80;
    gDeckEdit.bgScrollY += 0x80;
    REG_BG3HOFS = gDeckEdit.bgScrollX >> 8;
    REG_BG3VOFS = gDeckEdit.bgScrollY >> 8;
    if (gDeckEdit.inputLock == 0) {
        switch (gDeckEdit.subPhase) {
        case LIST_FILTER_PHASE_FILTER:
            switch (keys) {
            case DPAD_UP:
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_HIDDEN;
                if (gDeckEdit.sideSwapMode)
                    gDeckEdit.filterSel = gListFilterNavNoFusion[gDeckEdit.filterSel][0];
                else
                    gDeckEdit.filterSel = gListFilterNav[gDeckEdit.filterSel][0];
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_PLAYING;
                PlaySE(SE_CURSOR);
                break;
            case DPAD_DOWN:
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_HIDDEN;
                if (gDeckEdit.sideSwapMode)
                    gDeckEdit.filterSel = gListFilterNavNoFusion[gDeckEdit.filterSel][1];
                else
                    gDeckEdit.filterSel = gListFilterNav[gDeckEdit.filterSel][1];
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_PLAYING;
                PlaySE(SE_CURSOR);
                break;
            case DPAD_RIGHT:
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_HIDDEN;
                if (gDeckEdit.sideSwapMode)
                    gDeckEdit.filterSel = gListFilterNavNoFusion[gDeckEdit.filterSel][2];
                else
                    gDeckEdit.filterSel = gListFilterNav[gDeckEdit.filterSel][2];
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_PLAYING;
                PlaySE(SE_CURSOR);
                break;
            case DPAD_LEFT:
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_HIDDEN;
                if (gDeckEdit.sideSwapMode)
                    gDeckEdit.filterSel = gListFilterNavNoFusion[gDeckEdit.filterSel][3];
                else
                    gDeckEdit.filterSel = gListFilterNav[gDeckEdit.filterSel][3];
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_PLAYING;
                PlaySE(SE_CURSOR);
                break;
            case B_BUTTON:
                if (gDeckEdit.fade.state == FADE_STATE_IDLE)
                    gDeckEdit.panelAlphaStep = 1;
                PlaySE(SE_CANCEL);
                break;
            default:
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_PLAYING;
                break;
            case A_BUTTON:
                gDeckEdit.inputLock = 1;
                PlaySE(SE_CONFIRM);
                break;
            }
            if (gDeckEdit.panelShown != 0 && gDeckEdit.panelAlpha <= 8)
                ListFilter_DrawCursor(gDeckEdit.filterSel);
            break;
        case LIST_FILTER_PHASE_SORT:
            switch (keys) {
            case DPAD_UP:
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_HIDDEN;
                gDeckEdit.sortSel = gListSortNav[gDeckEdit.sortSel][0];
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_PLAYING;
                PlaySE(SE_CURSOR);
                break;
            case DPAD_DOWN:
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_HIDDEN;
                gDeckEdit.sortSel = gListSortNav[gDeckEdit.sortSel][1];
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_PLAYING;
                PlaySE(SE_CURSOR);
                break;
            case DPAD_RIGHT:
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_HIDDEN;
                gDeckEdit.sortSel = gListSortNav[gDeckEdit.sortSel][2];
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_PLAYING;
                PlaySE(SE_CURSOR);
                break;
            case DPAD_LEFT:
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_HIDDEN;
                gDeckEdit.sortSel = gListSortNav[gDeckEdit.sortSel][3];
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_PLAYING;
                PlaySE(SE_CURSOR);
                break;
            case B_BUTTON:
                gDeckEdit.subPhase = LIST_FILTER_PHASE_FILTER;
                REG_DISPCNT |= 0x200;
                REG_DISPCNT &= 0xFEFF;
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_HIDDEN;
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_FINISHED;
                PlaySE(SE_CURSOR);
                break;
            case A_BUTTON:
                gDeckEdit.inputLock = 1;
                PlaySE(SE_CONFIRM);
                break;
            default:
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_PLAYING;
                break;
            }
            if (gDeckEdit.panelShown != 0 && gDeckEdit.panelAlpha <= 8)
                ListFilter_DrawCursor(gListSortOptionAnims[gDeckEdit.sortSel]);
            break;
        case LIST_FILTER_PHASE_APPLY:
            switch (keys) {
            case B_BUTTON:
                if (gDeckEdit.fade.state == FADE_STATE_IDLE) {
                    gDeckEdit.panelAlphaStep = 1;
                    PlaySE(SE_CANCEL);
                }
                break;
            case A_BUTTON:
                ListFilter_DrawProgressBar(gListFilterBarFillTiles, (void *)0x06003C00, (gDeckEdit.bgScrollX >> 8) & 0x7F);
                PlaySE(SE_CONFIRM);
                break;
            default:
                gDeckEdit.anims.anims[10].active = ANIM_PLAYING;
                gDeckEdit.animCounter = 0;
                gMain.vblankCallback = ListFilter_ProgressBarVBlank;
                switch (gDeckEdit.sortOverride) {
                case LIST_SORT_TYPE:
                case LIST_SORT_ATTRIBUTE:
                case LIST_SORT_LEVEL:
                    DeckEdit_FilterAndSortList(gDeckEdit.curList, gDeckEdit.filterSel, gDeckEdit.sortOverride);
                    gDeckEdit.sortOverride = 0;
                    gDeckEdit.commandMenu.sort[gDeckEdit.curList] = LIST_SORT_ATK;
                    gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
                    break;
                default:
                    DeckEdit_FilterAndSortList(gDeckEdit.curList, gDeckEdit.filterSel, gDeckEdit.sortSel);
                    break;
                }
                gMain.vblankCallback = NULL;
                ListFilter_DrawProgressBar(gListFilterBarFillTiles, (void *)0x06003C00, 0x80);
                gDeckEdit.panelAlphaStep = 1;
                gDeckEdit.subPhase = LIST_FILTER_PHASE_DONE;
                break;
            }
            break;
        }
    }
    if (gDeckEdit.inputLock != 0) {
        switch (gDeckEdit.subPhase) {
        case LIST_FILTER_PHASE_FILTER:
            if (gDeckEdit.inputLock == 13) {
                gDeckEdit.inputLock = 0;
                gDeckEdit.anims.anims[gDeckEdit.filterSel].active = ANIM_HIDDEN;
                REG_DISPCNT &= 0xFDFF;
                REG_DISPCNT |= 0x100;
                switch (gDeckEdit.filterSel) {
                case 4:
                case 5:
                    gDeckEdit.commandMenu.filter[gDeckEdit.curList] = gDeckEdit.filterSel;
                    gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
                    gDeckEdit.commandMenu.sort[gDeckEdit.curList] = gDeckEdit.sortSel;
                    gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
                    gDeckEdit.inputLock = 0;
                    ListFilter_ShowNowFiltering();
                    gDeckEdit.subPhase = LIST_FILTER_PHASE_APPLY;
                    break;
                default:
                    gDeckEdit.subPhase = LIST_FILTER_PHASE_SORT;
                    gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_FINISHED;
                    break;
                }
            } else if (++gDeckEdit.inputLock <= 11) {
                ListFilter_DrawCursorFlash(gDeckEdit.filterSel);
            }
            break;
        case LIST_FILTER_PHASE_SORT:
            if (gDeckEdit.inputLock == 13) {
                gDeckEdit.commandMenu.filter[gDeckEdit.curList] = gDeckEdit.filterSel;
                gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
                gDeckEdit.commandMenu.sort[gDeckEdit.curList] = gDeckEdit.sortSel;
                gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
                gDeckEdit.inputLock = 0;
                gDeckEdit.anims.anims[gListSortOptionAnims[gDeckEdit.sortSel]].active = ANIM_HIDDEN;
                ListFilter_ShowNowFiltering();
                gDeckEdit.subPhase = LIST_FILTER_PHASE_APPLY;
            } else if (++gDeckEdit.inputLock <= 11) {
                ListFilter_DrawCursorFlash(gListSortOptionAnims[gDeckEdit.sortSel]);
            }
            break;
        case LIST_FILTER_PHASE_APPLY:
            break;
        }
    }
    if (gDeckEdit.fade.state == FADE_STATE_FADED_OUT) {
        gDeckEdit.exitMode = DECKEDIT_EXIT_LIST_VIEW;
        gDeckEdit.commandMenu.anim = CMDMENU_REFRESH;
        return 1;
    }
    if (gDeckEdit.fade.state == FADE_STATE_FADED_IN) {
        REG_BLDCNT = 0x3F44;
        SetBldAlpha(0x10);
        REG_DISPCNT |= 0x400;
        gDeckEdit.fade.state = FADE_STATE_IDLE;
        gDeckEdit.panelAlphaStep = -1;
        gDeckEdit.panelShown = 1;
    }
    if (gDeckEdit.panelAlphaStep != 0) {
        gDeckEdit.panelAlpha += gDeckEdit.panelAlphaStep;
        if (gDeckEdit.panelAlpha == 8)
            gDeckEdit.panelAlphaStep = 0;
        if (gDeckEdit.panelAlpha == 16) {
            REG_DISPCNT &= 0xFBFF;
            FadeStart(0, 0x180, 0, &gDeckEdit.fade);
            gDeckEdit.panelAlphaStep = 0;
            gDeckEdit.panelShown = 0;
        }
        SetBldAlpha(gDeckEdit.panelAlpha);
    }
    AnimBlockTick((u8 *)&gDeckEdit.anims);
    AnimBlockDraw((u8 *)&gDeckEdit.anims, 0, 0, 0, 0, 0, 3, 0, 0, (u32)&gDeckEdit.oamList);
    OamListFlush(&gDeckEdit.oamList);
    OamListClear((u8 *)&gDeckEdit.oamList);
    return 0;
}
