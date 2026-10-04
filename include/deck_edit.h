#ifndef GUARD_DECK_EDIT_H
#define GUARD_DECK_EDIT_H

/*
 * Deck Edit and the screens built from the same code: the Campaign side-deck swap, the Card Trading picker and
 * the Prohibition picker, plus their sub-screens (List Filter, Statistics). All of them keep their state in
 * gDeckEdit (struct DeckEdit, 0x0201DB20) and run one of four step tables with the same layout (enum DeckEditStep).
 *
 * Units: deck_edit, deck_edit_cards, deck_edit_filter, deck_edit_filter_steps, deck_edit_list, deck_edit_panel,
 * deck_edit_prohibit, deck_edit_stats, deck_edit_view, deck_edit_widgets. Wiki: wiki/functions/deck-edit-*-c.md.
 *
 * Matching notes for units that migrate to this header:
 *  - Prototypes below are the definitions as compiled today. Several callers use another view (u32 instead of
 *    u8 parameters, void * instead of a struct pointer, or no argument at all); such a unit keeps a commented
 *    local alias prototype (`T Name(...) asm("Sym");`). See build/readability/proto_mismatches.txt.
 *  - Many units reach gDeckEdit fields through address-suffixed alias symbols (gUnk_0201E140 = listPos, ...;
 *    each is named in the field comments) or through integer addresses. Those forms are matching choices: keep
 *    them, do not turn them into field accesses (or the reverse) without checking the unit still matches.
 *  - Bitfield containers are u8 here; a unit that needs a u32 view of the same bits keeps a local view.
 */

#include "global.h"
#include "util.h"       /* struct Ease */
#include "palette.h"    /* struct Fade */
#include "sprite.h"     /* struct OamList, struct ObjAffine, struct AnimBlock, struct AnimState */
#include "text.h"       /* struct TextFlags */

/* ---------------------------------------------------------------------------------------------------------- */
/* Enums                                                                                                       */
/* ---------------------------------------------------------------------------------------------------------- */

/* The three card lists of the list view (gDeckEdit.curList; list argument of DeckEdit_GetListCard etc.). */
enum DeckEditList {
    DECKEDIT_LIST_TRUNK = 0,        /* every owned card */
    DECKEDIT_LIST_MAIN_DECK = 1,    /* cards with main-deck or fusion-deck copies */
    DECKEDIT_LIST_SIDE_DECK = 2,    /* cards with side-deck copies */
};

/* Which card-list screen runs (gMain.cardListMode, +0x4874 bits 0-1); the shared list builder filters by it. */
enum CardListMode {
    CARD_LIST_MODE_DECK_EDIT = 0,   /* Deck Edit, also the side-deck swap */
    CARD_LIST_MODE_TRADE = 1,       /* Card Trading picker: hides card numbers 1210-1899 and 1920-1999 */
    CARD_LIST_MODE_PROHIBIT = 2,    /* Prohibition picker */
};

/* Step index into gDeckEditSteps / gSideDeckSwapSteps / gTradeCardSelectSteps / gProhibitCardSelectSteps
   (all four tables share this layout; the *_SwitchScreen steps write it). */
enum DeckEditStep {
    DECKEDIT_STEP_INIT = 0,
    DECKEDIT_STEP_ENTER_LIST_VIEW = 1,
    DECKEDIT_STEP_UPDATE = 2,
    DECKEDIT_STEP_SWITCH_SCREEN = 3,
    DECKEDIT_STEP_END = 4,          /* NULL entry: the runner returns 1 */
    DECKEDIT_STEP_LIST_FILTER = 5,  /* List Filter runner, then ResetListView and SwitchScreen */
    DECKEDIT_STEP_STATISTICS = 9,   /* Statistics runner, then ResetListView and SwitchScreen */
    DECKEDIT_STEP_CARD_VIEW = 13,   /* CardDetail_Run, then ResetListView and SwitchScreen */
};

/* Where the screen goes after its fade-out (gDeckEdit.exitMode), read by the *_SwitchScreen steps. */
enum DeckEditExitMode {
    DECKEDIT_EXIT_LEAVE = 0,        /* end the screen */
    DECKEDIT_EXIT_LIST_FILTER = 1,
    DECKEDIT_EXIT_CARD_VIEW = 2,    /* Card Detail of the card under the cursor */
    DECKEDIT_EXIT_STATISTICS = 3,
    DECKEDIT_EXIT_LIST_VIEW = 4,    /* back to the list view (after a sub-screen) */
};

/* Commands of the command bar opened with A (DeckEditCommandMenu.choice). */
enum DeckEditCommand {
    DECKEDIT_CMD_CARD_VIEW = 0,
    DECKEDIT_CMD_TO_TRUNK = 1,
    DECKEDIT_CMD_TO_MAIN_DECK = 2,  /* 'Select A Swapping Card' in the side-deck swap */
    DECKEDIT_CMD_TO_SIDE_DECK = 3,  /* 'Select A Swapping Card' in the side-deck swap */
    DECKEDIT_CMD_LIST_FILTER = 4,
    DECKEDIT_CMD_STATISTICS = 5,
    DECKEDIT_CMD_EXIT = 6,          /* 'Decide' in the two pickers */
};

/* Command-bar variant (gDeckEdit.menuVariant): selects the special labels drawn by DeckEdit_CommandLabelVBlank. */
enum DeckEditMenuVariant {
    DECKEDIT_MENU_EDIT = 0,
    DECKEDIT_MENU_SIDE_SWAP = 1,    /* commands 2/3 read 'Select A Swapping Card' */
    DECKEDIT_MENU_PICK_CARD = 2,    /* command 6 reads 'Decide' */
};

/* Command-bar animation (DeckEditCommandMenu.anim, gDeckEdit + 0x1C3D bits 0-2). */
enum CommandMenuAnim {
    CMDMENU_IDLE = 0,
    CMDMENU_OPEN = 1,
    CMDMENU_CLOSE = 2,
    CMDMENU_REFRESH = 3,            /* redraw once and reload the label tiles in VBlank */
};

/* The same field as CommandMenuAnim, under the names the list handlers use when they request a slide. */
enum DeckEditMenuSlideCmd {
    MENU_SLIDE_OPEN = 1,
    MENU_SLIDE_CLOSE = 2,
    MENU_SLIDE_REFRESH = 3,
};

/* Running scroll animation of the list view (gDeckEdit.scrollDir; dir argument of DeckEdit_RotateListRowRing and
   DeckEdit_ScrollFrameSlots). */
enum DeckEditScrollDir {
    DECKEDIT_SCROLL_NONE = 0,
    DECKEDIT_SCROLL_UP = 1,         /* DeckEdit_ScrollListUp */
    DECKEDIT_SCROLL_DOWN = 2,       /* DeckEdit_ScrollListDown */
    DECKEDIT_SCROLL_PAGE_RIGHT = 3,
    DECKEDIT_SCROLL_PAGE_LEFT = 4,
};

/* gDeckEdit.scrollDir as the list-switch code (deck_edit_list) sets it: the horizontal values 3/4 also slide to
   the next/previous list (DeckEdit_StartListSlide). */
enum DeckEditSlide {
    DECKEDIT_SLIDE_NONE = 0,
    DECKEDIT_SLIDE_UP = 1,
    DECKEDIT_SLIDE_DOWN = 2,
    DECKEDIT_SLIDE_NEXT_LIST = 3,   /* R */
    DECKEDIT_SLIDE_PREV_LIST = 4,   /* L */
};

/* Card-move animation steps (CardMove.step), run by DeckEdit_UpdateCardMove. */
enum CardMoveStep {
    CARD_MOVE_IDLE = 0,
    CARD_MOVE_CHECK = 1,            /* check the destination limits (falls through into PICK_UP) */
    CARD_MOVE_PICK_UP = 2,          /* wait for the frame animation, start the flight ease */
    CARD_MOVE_FLY = 3,              /* card sprite flies to the destination list icon */
    CARD_MOVE_LAND = 4,             /* restart the destination icon animation */
    CARD_MOVE_COMMIT = 5,           /* move one copy in gSaveData and rebuild the lists */
};

/* First argument of DeckEdit_DrawCardIcon: which icon table. */
enum CardIconSet {
    CARD_ICON_SET_ATTRIBUTE = 0,
    CARD_ICON_SET_TYPE = 1,
    CARD_ICON_SET_SPELL_SUBTYPE = 2,
    CARD_ICON_SET_KIND = 3,         /* Effect / Fusion / Ritual */
};

/* Icon index in CARD_ICON_SET_ATTRIBUTE after the CardAttribute values 1-6. */
enum AttributeIcon {
    ATTRIBUTE_ICON_MAGIC = 8,
    ATTRIBUTE_ICON_TRAP = 9,
    ATTRIBUTE_ICON_DIVINE = 10,
};

/* Side-deck swap progress (gDeckEdit.swapSelector). */
enum SideDeckSwapState {
    SWAP_STATE_IDLE = 0,
    SWAP_STATE_FIRST_PICKED = 1,    /* switch to the other list */
    SWAP_STATE_PICK_SECOND = 2,     /* B in the command bar cancels here */
    SWAP_STATE_SECOND_PICKED = 3,   /* start anims[16] */
    SWAP_STATE_ANIMATING = 4,       /* exchange the cards when anims[16] ends */
};

/* List Filter categories (filter argument of DeckEdit_FilterAndSortList; DeckEditCommandMenu.filter). */
enum ListFilter {
    LIST_FILTER_ALL = 0,
    LIST_FILTER_NORMAL = 1,
    LIST_FILTER_EFFECT = 2,
    LIST_FILTER_FUSION = 3,
    LIST_FILTER_MAGIC = 4,          /* skips the sort page */
    LIST_FILTER_TRAP = 5,           /* skips the sort page */
    LIST_FILTER_RITUAL = 6,
    LIST_FILTER_MONSTERS = 7,       /* not reachable from the cursor tables */
};

/* List Filter sort keys (sort argument of DeckEdit_FilterAndSortList; DeckEditCommandMenu.sort). */
enum ListSort {
    LIST_SORT_NAME = 0,             /* card ID order, which is alphabetical */
    LIST_SORT_ATK = 1,              /* descending */
    LIST_SORT_DEF = 2,              /* descending */
    LIST_SORT_TYPE = 3,             /* ascending */
    LIST_SORT_ATTRIBUTE = 4,        /* ascending */
    LIST_SORT_LEVEL = 5,            /* descending */
};

/* List Filter screen phase (gDeckEdit.subPhase). */
enum ListFilterPhase {
    LIST_FILTER_PHASE_FILTER = 0,   /* filter page */
    LIST_FILTER_PHASE_SORT = 1,     /* sort page */
    LIST_FILTER_PHASE_APPLY = 2,    /* 'Now Filtering' with the progress bar */
    LIST_FILTER_PHASE_DONE = 3,
};

/* Statistics rows (category argument of DeckStats_CountCategory; row index + 1). */
enum DeckStatsCategory {
    DECK_STATS_NORMAL = 1,
    DECK_STATS_EFFECT = 2,
    DECK_STATS_FUSION = 3,
    DECK_STATS_MAGIC = 4,
    DECK_STATS_TRAP = 5,
    DECK_STATS_RITUAL = 6,
};

/* ---------------------------------------------------------------------------------------------------------- */
/* Structs                                                                                                     */
/* ---------------------------------------------------------------------------------------------------------- */

/* One row of the three card lists (gDeckEdit.lists[row]): card IDs, row 0 full, row 1 filtered/sorted. */
struct DeckEditListRow {
    u16 trunk[821];                 /* +0x000: list 0 (Trunk) */
    u16 deck[80];                   /* +0x66A: list 1 (Main Deck + fusion deck) */
    u16 side[15];                   /* +0x70A: list 2 (Side Deck) */
};

/* Scroll bar on the right edge of the list view (gDeckEdit.scrollBar), written by DeckEdit_CalcScrollBar and
   drawn by DeckEdit_DrawScrollBar. The arrows sit at the top and bottom of the track (BG2 map cells (29, 0) and
   (29, 13)). */
struct DeckEditScrollBar {
    u16 thumbLen;                   /* +0x0: thumb length, 8.8 px (192 / (count - 1)) */
    u16 thumbPos;                   /* +0x2: thumb offset from the top of the track, 8.8 px */
    u8 upArrowDirty:1;              /* +0x4 bit 0: redraw the up-arrow cell */
    u8 unk4_1:7;
    u8 upArrowFrame;                /* +0x5: 0 hidden (list <= 5 cards), 1 shown, 2 pressed; gScrollArrowTiles index */
    u8 downArrowDirty:1;            /* +0x6 bit 0: redraw the down-arrow cell */
    u8 unk6_1:7;
    u8 downArrowFrame;              /* +0x7: same states; gScrollArrowTiles index - 3 */
};

/* One card-frame sprite left of the list (FrameSlotRing.slot[]): its position tweens between rows. */
struct FrameSlot {
    u8 pos;                         /* +0x0: row position 0..6 (3 = cursor row) */
    u8 frame;                       /* +0x1: enum CardFrame of the card */
    s16 y;                          /* +0x2: current y */
    u16 startY;                     /* +0x4: tween start */
    s16 deltaY;                     /* +0x6: tween distance */
    u8 active;                      /* +0x8: drawn when non-zero */
    u8 unk9;
    u16 scale;                      /* +0xA: 8.8 scale at the tween start */
    s16 scaleStep;                  /* +0xC: (target scale - scale) / 6 */
    u8 affineIdx;                   /* +0xE: OBJ affine index (slot + 1) */
    u8 unkF;
};

/* Ring of the six card-frame sprites (gDeckEdit.frameSlots). */
struct FrameSlotRing {
    u8 head;                        /* +0x00: slot that receives the next card entering the view (reset to 5) */
    u8 unk1[3];
    struct FrameSlot slot[6];       /* +0x04: slot[5].active is the byte +0x1C14 of gDeckEdit */
};

/* Animation of a card moving to another list (gDeckEdit.cardMove), started by DeckEdit_StartCardMove. */
struct CardMove {
    u8 step;                        /* +0x00: enum CardMoveStep */
    u8 unk1[3];
    struct Ease ease;               /* +0x04: flight progress 0..6 */
    u8 frame;                       /* +0x0C: enum CardFrame of the moved card (selects its pick-up animation) */
    u8 destList;                    /* +0x0D: enum DeckEditList the card goes to */
    s16 targetX;                    /* +0x0E: flight end x - start x */
    s16 targetY;                    /* +0x10: flight end y - start y */
};

/* Command bar of the list view (gDeckEdit.commandMenu, +0x1C3C), animated by DeckEdit_UpdateCommandMenuAnim and
   drawn by DeckEdit_DrawCommandMenu. It also holds each list's List Filter choice for the bar's labels. */
struct DeckEditCommandMenu {
    u8 timer;                       /* +0x0: step countdown (0 = one step per frame) */
    u8 anim:3;                      /* +0x1 bits 0-2: enum CommandMenuAnim */
    u8 prevRows:2;                  /* +0x1 bits 3-4: rows drawn last time */
    u8 rows:2;                      /* +0x1 bits 5-6: visible rows of the bar, 0..2 (WIN1V top = rows * 8) */
    u32 choice:3;                   /* +0x1 bit 7 .. +0x2 bit 1 (word bits 15-17): enum DeckEditCommand */
    u32 unk2_2:6;
    u8 filter[3];                   /* +0x3: enum ListFilter of each list (left label) */
    u8 sort[3];                     /* +0x6: enum ListSort of each list (right label) */
};

/* gDeckEdit (0x0201DB20, 0x1C5C bytes): the state of every card-list screen. The sub-screens (List Filter,
   Statistics) reuse the +0x1C49..+0x1C54 bytes for their own state. */
struct DeckEdit {
    struct OamList oamList;         /* +0x0000: OBJ layer lists (the last argument of the OamList sprite helpers) */
    struct Fade fade;               /* +0x0618: screen fade; fade.state 2 = faded out, 3 = faded in. Alias gUnk_0201E138 */
    u16 listPos[3];                 /* +0x0620: selected card index per enum DeckEditList. Alias gUnk_0201E140 */
    u16 unk626;
    struct Ease scrollEase;         /* +0x0628: 0..6 ease of the running scroll, page or list slide; cur indexes
                                       gDeckEditEaseCurve; state 1 running, 2 just finished. Alias gUnk_0201E148 */
    u16 bg3Hofs;                    /* +0x0630: BG3 (card art) HOFS shadow; -0x50 per slide to the previous list */
    u16 bg3Vofs;                    /* +0x0632: BG3 VOFS shadow; -0x50 per card scrolled */
    u8 cardArtPage;                 /* +0x0634: card art double buffer 0/1 at VRAM 0x06008000 + page * 0x1680 */
    u8 scrollDir;                   /* +0x0635: enum DeckEditScrollDir (enum DeckEditSlide in the list-switch code) */
    u16 unk636;
    u16 bg1Hofs;                    /* +0x0638: BG1 (card names, map 0x0600D000) HOFS shadow */
    u16 bg1Vofs;                    /* +0x063A: BG1 VOFS shadow; -0x10 per card */
    u16 bg0Hofs;                    /* +0x063C: BG0 (detail panel, map 0x0600C000) HOFS shadow */
    u16 bg0Vofs;                    /* +0x063E: BG0 VOFS shadow; -0x28 per card; DeckEdit_DrawCardIcons turns it into
                                       a map row */
    struct TextFlags textFlags;     /* +0x0640: kana flags of the map-text drawers (ClearKatakanaFlag at init); also
                                       passed as a trailing argument that some drawers ignore. Alias gUnk_0201E160 */
    struct DeckEditListRow lists[2]; /* +0x0644: card IDs [row]; row 0 = full lists, row 1 = filtered/sorted copy.
                                       Read through DeckEdit_GetListCard / DeckEdit_SetListCard */
    u16 listCount[2][3];            /* +0x1494: [row][list] number of cards */
    u8 listRow[3];                  /* +0x14A0: row shown per list: 0 full, 1 filtered/sorted. Alias gUnk_0201EFC0 */
    u8 unk14A3;
    u16 sortScratch[(0x1710 - 0x14A4) / 2]; /* +0x14A4: non-monsters held back during a sorted 'All' filter; the
                                       filter code also reaches the list rows at negative offsets from its alias
                                       gUnk_0201EFC4 */
    u8 redrawOnPageSlide:1;         /* +0x1710 bit 0: redraw the rows and detail panel when a horizontal slide
                                       passes its midpoint (set by DeckEdit_StartListSlide) */
    u8 unk1710_1:7;
    u8 unk1711;
    u16 sideMonsterCount[2];        /* +0x1712: [row] monster copies in the side deck ([1] is overwritten with [0]) */
    u16 unk1716;
    struct AnimBlock anims;         /* +0x1718: AnimBlockInit(gDeckEditAnimScripts): [0..5] card-frame pick-up
                                       animations, [6..8] selected list panel, [9] forced on every frame, [10..12]
                                       Trunk/Main/Side icons (card landing), [13..15] list panels, [16] side-deck
                                       swap. The List Filter screen loads its 14 option animations here instead */
    s16 brightness;                 /* +0x18AC: blend ramp, 0xFC00 (-0x400) after a redraw, +0x30 per idle frame up
                                       to 0x400; >= 0 brightens, < 0 darkens, BLDY = |v| >> 8 */
    u16 unk18AE;
    struct ObjAffine objAffine[32]; /* +0x18B0: OBJ affine records; [0] scroll-bar thumb, [1..6] frame slots.
                                       Alias gUnk_0201F3D0 */
    struct DeckEditScrollBar scrollBar; /* +0x1BB0 */
    struct FrameSlotRing frameSlots; /* +0x1BB8: card-frame sprites left of the list. Alias gUnk_0201F6D8, which
                                       code also uses as a base for cardMove (+0x68), +0x1C34 (+0x7C), commandMenu
                                       (+0x84) and +0x1C5A (+0xA2), and anims (-0x4A0) */
    u8 curList;                     /* +0x1C1C: enum DeckEditList shown. Alias gUnk_0201F73C */
    u8 prevList;                    /* +0x1C1D: list highlighted last frame (DeckEdit_UpdatePanelHighlight) */
    u8 unk1C1E[2];
    struct CardMove cardMove;       /* +0x1C20: alias gUnk_0201F740, which the frame handlers use as a base for
                                       the rest of the state (-0x1608 fade, -0x508 anims, +0x1C commandMenu) */
    u8 cursorRowPage:1;             /* +0x1C34 bit 0: cursor-row text page 0/1 (DeckEdit_FlipCursorRowPage) */
    u8 listRowRing:4;               /* +0x1C34 bits 1-4: rotation 0..6 of the 7 row-name buffers */
    u8 unk1C34_5:3;
    u8 unk1C35[6];
    u8 cursorCardFrame;             /* +0x1C3B: enum CardFrame of the cursor card (DeckEdit_DrawCursorRowName);
                                       passed to DeckEdit_StartCardMove */
    struct DeckEditCommandMenu commandMenu; /* +0x1C3C */
    u8 menuOpen:1;                  /* +0x1C48 bit 0: command bar fully open: input goes to the bar, not the list */
    u8 exitMode:4;                  /* +0x1C48 bits 1-4: enum DeckEditExitMode */
    u8 unk1C48_5:3;
    u8 filterSel;                   /* +0x1C49: List Filter cursor (enum ListFilter) */
    u8 sortSel;                     /* +0x1C4A: List Filter sort cursor (enum ListSort) */
    u8 subPhase;                    /* +0x1C4B: List Filter phase (enum ListFilterPhase) */
    u16 bgScrollX;                  /* +0x1C4C: sub-screen BG3 pattern scroll X, 8.8, +0x80 per frame */
    u16 bgScrollY;                  /* +0x1C4E: sub-screen BG3 pattern scroll Y, 8.8 */
    u8 panelAlpha;                  /* +0x1C50: BG2 panel blend level, 16 hidden .. 8 shown. Alias gUnk_0201F770 */
    s8 panelAlphaStep;              /* +0x1C51: -1 while the panel blends in, +1 while it blends out, 0 idle */
    u8 panelShown;                  /* +0x1C52: 1 while the panel is fully shown */
    u8 inputLock;                   /* +0x1C53: List Filter confirm timer (counts to 13 after A); Statistics ignores
                                       A/B while it is non-zero */
    u8 animCounter;                 /* +0x1C54: List Filter progress-bar counter (ListFilter_ProgressBarVBlank) */
    u8 nameIndexCache[2];           /* +0x1C55: last two name letters drawn on the name-index tab, read and written
                                       as a u16 at this odd address: the ARM7 strh lands on +0x1C54 and the ldrh
                                       returns a rotated halfword, so the cache never hits (the letters are redrawn
                                       every frame) and the store clobbers animCounter */
    u8 unk1C57;
    u16 nameTabTimer;               /* +0x1C58: frames the name-index tab stays visible (30 after a scroll or page) */
    u8 menuVariant:2;               /* +0x1C5A bits 0-1: enum DeckEditMenuVariant */
    u8 swapSelector:3;              /* +0x1C5A bits 2-4: enum SideDeckSwapState */
    u8 sideSwapMode:1;              /* +0x1C5A bit 5: side-deck swap running: fusion-deck copies stay out of list 1
                                       (set by SideDeckSwap_Run, cleared by CB_DeckEdit every frame) */
    u8 unk1C5A_6:2;
    u8 sortOverride;                /* +0x1C5B: 3-5 replace the chosen sort; never written (debug leftover?) */
};

/* One row of the Statistics table (struct DeckStatsRow[7] in gScratchBuffer, 0x02030000; row 6 = SUM). */
struct DeckStatsRow {
    u16 count;                      /* +0x0: copies in the category (row 6: total) */
    u16 percent;                    /* +0x2: rounded share of the total (row 6: 100) */
};

/* A pending (lo, hi) range of QuickSortS16 (stack in gScratchBuffer). */
struct SortRange {
    s16 lo;
    s16 hi;
};

/* Sprite entry of gListFilterCursorSprites / gListFilterFlashSprites (ListFilter_DrawCursor). */
struct SpriteDef {
    const void *gfx;                /* +0x0: OAM template list for the sprite-sheet emitter */
    u8 count;                       /* +0x4: its third argument */
    u8 unk5[3];
};

/* Compile-time layout checks (agbcc pads every struct to 4 bytes). */
typedef char deck_edit_h_check_row[sizeof(struct DeckEditListRow) == 0x728 ? 1 : -1];
typedef char deck_edit_h_check_bar[sizeof(struct DeckEditScrollBar) == 0x8 ? 1 : -1];
typedef char deck_edit_h_check_slot[sizeof(struct FrameSlot) == 0x10 ? 1 : -1];
typedef char deck_edit_h_check_ring[sizeof(struct FrameSlotRing) == 0x64 ? 1 : -1];
typedef char deck_edit_h_check_move[sizeof(struct CardMove) == 0x14 ? 1 : -1];
typedef char deck_edit_h_check_move_ease[(u32)&((struct CardMove *)0)->ease == 0x4 ? 1 : -1];
typedef char deck_edit_h_check_move_y[(u32)&((struct CardMove *)0)->targetY == 0x10 ? 1 : -1];
typedef char deck_edit_h_check_menu[sizeof(struct DeckEditCommandMenu) == 0xC ? 1 : -1];
typedef char deck_edit_h_check_menu_filter[(u32)&((struct DeckEditCommandMenu *)0)->filter == 0x3 ? 1 : -1];
typedef char deck_edit_h_check_menu_sort[(u32)&((struct DeckEditCommandMenu *)0)->sort == 0x6 ? 1 : -1];
typedef char deck_edit_h_check_stats[sizeof(struct DeckStatsRow) == 0x4 ? 1 : -1];
typedef char deck_edit_h_check_range[sizeof(struct SortRange) == 0x4 ? 1 : -1];
typedef char deck_edit_h_check_spritedef[sizeof(struct SpriteDef) == 0x8 ? 1 : -1];
typedef char deck_edit_h_check_size[sizeof(struct DeckEdit) == 0x1C5C ? 1 : -1];
typedef char deck_edit_h_check_fade[(u32)&((struct DeckEdit *)0)->fade == 0x618 ? 1 : -1];
typedef char deck_edit_h_check_ease[(u32)&((struct DeckEdit *)0)->scrollEase == 0x628 ? 1 : -1];
typedef char deck_edit_h_check_text[(u32)&((struct DeckEdit *)0)->textFlags == 0x640 ? 1 : -1];
typedef char deck_edit_h_check_lists[(u32)&((struct DeckEdit *)0)->lists == 0x644 ? 1 : -1];
typedef char deck_edit_h_check_count[(u32)&((struct DeckEdit *)0)->listCount == 0x1494 ? 1 : -1];
typedef char deck_edit_h_check_scratch[(u32)&((struct DeckEdit *)0)->sortScratch == 0x14A4 ? 1 : -1];
typedef char deck_edit_h_check_side[(u32)&((struct DeckEdit *)0)->sideMonsterCount == 0x1712 ? 1 : -1];
typedef char deck_edit_h_check_anims[(u32)&((struct DeckEdit *)0)->anims == 0x1718 ? 1 : -1];
typedef char deck_edit_h_check_bright[(u32)&((struct DeckEdit *)0)->brightness == 0x18AC ? 1 : -1];
typedef char deck_edit_h_check_affine[(u32)&((struct DeckEdit *)0)->objAffine == 0x18B0 ? 1 : -1];
typedef char deck_edit_h_check_scroll[(u32)&((struct DeckEdit *)0)->scrollBar == 0x1BB0 ? 1 : -1];
typedef char deck_edit_h_check_slots[(u32)&((struct DeckEdit *)0)->frameSlots == 0x1BB8 ? 1 : -1];
typedef char deck_edit_h_check_cur[(u32)&((struct DeckEdit *)0)->curList == 0x1C1C ? 1 : -1];
typedef char deck_edit_h_check_cmove[(u32)&((struct DeckEdit *)0)->cardMove == 0x1C20 ? 1 : -1];
typedef char deck_edit_h_check_frame[(u32)&((struct DeckEdit *)0)->cursorCardFrame == 0x1C3B ? 1 : -1];
typedef char deck_edit_h_check_cmenu[(u32)&((struct DeckEdit *)0)->commandMenu == 0x1C3C ? 1 : -1];
typedef char deck_edit_h_check_fsel[(u32)&((struct DeckEdit *)0)->filterSel == 0x1C49 ? 1 : -1];
typedef char deck_edit_h_check_alpha[(u32)&((struct DeckEdit *)0)->panelAlpha == 0x1C50 ? 1 : -1];
typedef char deck_edit_h_check_name[(u32)&((struct DeckEdit *)0)->nameIndexCache == 0x1C55 ? 1 : -1];
typedef char deck_edit_h_check_tab[(u32)&((struct DeckEdit *)0)->nameTabTimer == 0x1C58 ? 1 : -1];
typedef char deck_edit_h_check_sortov[(u32)&((struct DeckEdit *)0)->sortOverride == 0x1C5B ? 1 : -1];

/* ---------------------------------------------------------------------------------------------------------- */
/* Globals                                                                                                     */
/* ---------------------------------------------------------------------------------------------------------- */

extern struct DeckEdit gDeckEdit;                  /* 0x0201DB20: state of every card-list screen */

/* ROM data used by two or more units (single-unit tables stay local externs in their unit). */
extern const u16 gDeckEditEaseCurve[];             /* [7] ease-in-out factors in 8.8: 0, 32, 64, 128, 192, 224, 256 */
extern const u8 gDeckEditDigitSprites[];           /* 8-byte digit sprite descriptors (base + digit * 8) */
extern const u8 gDeckEditAnimScripts[];            /* NULL-terminated list of the 17 AnimSeq pointers of gDeckEdit.anims */
extern const u8 gDeckEditFrameMap[][60];           /* 30x40 BG map of the list frame: rows 0-19 BG2, the rest the
                                                      command-bar slide */
extern const u8 gDeckEditObjTiles[];               /* OBJ tiles at 0x06010000: list tabs, cursor, digits */
extern const u8 gDeckEditCardStackObjTiles[];      /* OBJ tiles at 0x06010200: card stack / deck box */
extern const u8 gDeckEditCardIconObjTiles[];       /* OBJ tiles at 0x06014000: small card backs / icons */
extern const u8 gDeckEditCardFrameObjTiles[];      /* OBJ tiles at 0x06014200: coloured card-frame thumbnails */
extern const u8 gDeckEditObjPal[];                 /* the 16 OBJ palettes of the card-list screens */
extern const u8 gDeckEditLabelTiles[];             /* 0x2000 bytes of BG tiles at 0x06004000: command bar and labels */
extern const u8 gListFilterSortPageMap[];          /* 30x28 BG0 map of the List Filter sort page */

/* ---------------------------------------------------------------------------------------------------------- */
/* Functions                                                                                                   */
/* ---------------------------------------------------------------------------------------------------------- */

/* Scene runners: each runs its step table and returns 1 at the NULL entry. */
u16 CB_DeckEdit(void);                              /* main-menu Deck Edit (gDeckEditSteps on gMain.seqIndex1) */
u16 SideDeckSwap_Run(void);                         /* Campaign side-deck swap (gSideDeckSwapSteps) */
u16 TradeCardSelect_Run(void);                      /* Card Trading picker (gTradeCardSelectSteps on gMain.seqState2) */
u16 ProhibitCardSelect_Run(void);                   /* Prohibition picker (step in gChain.targetWork) */
u16 ProhibitCardSelect_StartAndRun(void);           /* first call resets the step, later calls run (unused) */

/* Deck Edit steps (also used by the other tables where noted). */
int DeckEdit_Init(void);                            /* step 0: clear gDeckEdit, build the three lists from the trunk */
int DeckEdit_EnterListView(void);                   /* step 1: DeckEdit_InitListView(); return 1 */
int DeckEdit_InitListView(void);                    /* list-view graphics, rows, detail panel and fade-in */
int DeckEdit_Update(void);                          /* step 2: per-frame list and command-bar input and drawing */
int DeckEdit_SwitchScreen(void);                    /* step 3: jump to the step of gDeckEdit.exitMode (seqIndex1) */
int DeckEdit_ResetListView(void);                   /* after a sub-screen: reset the view, exitMode = LIST_VIEW */
int DeckEdit_TickFadeIn(void);                      /* 1 when the fade-in has finished (unused) */
u16 DeckEdit_RunListFilter(void);                   /* step 5: run gListFilterSteps on gMain.seqState1 */
u16 DeckEdit_RunStatistics(void);                   /* step 9: run gDeckStatsSteps on gMain.seqState1 */

/* Campaign side-deck swap (gSideDeckSwapSteps). */
int SideDeckSwap_Init(void);                        /* step 0: build the main-deck and side-deck lists */
u16 SideDeckSwap_EnterListView(void);               /* step 1: list view, starting on the main deck */
int SideDeckSwap_Update(void);                      /* step 2: frame handler (twin of DeckEdit_Update) */
void SideDeckSwap_HandleListSwitch(u8 *curList);    /* R/L alternate main and side deck while no swap runs */
u16 SideDeckSwap_FindCardInList(u16 cardId);        /* index of cardId in the current list's shown row, 0 if absent */
void SideDeckSwap_ExchangeCards(void);              /* swap the two picked cards in gSaveData and rebuild the lists */
void SideDeckSwap_UpdateExchange(void);             /* advance gDeckEdit.swapSelector once per frame */
void SideDeckSwap_CancelExchange(void);             /* B after the first pick: back to the first list */

/* Card Trading and Prohibition pickers (gTradeCardSelectSteps, gProhibitCardSelectSteps). */
int TradeCardSelect_Init(void);                     /* step 0: lists filtered by gMain.cardListMode */
int TradeCardSelect_Update(void);                   /* step 2: frame handler; 'Decide' stores gMain.pickedCardId */
int TradeCardSelect_SwitchScreen(void);             /* step 3: like DeckEdit_SwitchScreen on gMain.seqState2 */
int ProhibitCardSelect_Init(void);                  /* step 0: every card ID outside numbers 1900-1999 in list 0 */
int ProhibitCardSelect_InitListView(void);          /* step 1: list view with the prohibit banner */
int ProhibitCardSelect_Update(void);                /* step 2: frame handler; B cannot cancel */
int ProhibitCardSelect_SwitchScreen(void);          /* step 3: like DeckEdit_SwitchScreen on gChain.targetWork */
u16 ProhibitCardSelect_RunListFilter(void);         /* step 5: List Filter on gChain.targetWork2 */
/* Empty R/L handler of the pickers. Callers pass &gDeckEdit.curList, so they keep a local one-argument view. */
void CardSelect_HandleListSwitch(void);
void sub_0806F400(void);                            /* empty function */

/* Card lists. */
void DeckEdit_BuildCardLists(void);                 /* rebuild row 0 of the three lists from gSaveData.trunk */
u16 DeckEdit_GetListCard(u8 list, u8 row, u16 index);           /* card ID at lists[row].<list>[index] */
void DeckEdit_SetListCard(u16 cardId, u8 list, u8 row, u16 index); /* write the entry DeckEdit_GetListCard reads */
u16 *DeckEdit_GetActiveListRow(int list);           /* pointer to the shown row of a list (unused) */
u32 DeckEdit_GetSelectedCardCopies(void);           /* copies of the cursor card in the current list */
u32 GetCardCopiesInList(int list, int cardId);      /* copies of a card counted for list 0/1/2 (args narrowed to u16) */
u16 DeckEdit_IsFusionMonster(u16 cardId);           /* 1 if the card is a fusion monster (goes to the fusion deck) */
u8 GetCardFrameIndex(u16 cardId);                   /* enum CardFrame of a card (Tickets normal, Gods by number) */
void DeckEdit_CountSideDeckMonsters(void);          /* gDeckEdit.sideMonsterCount from the side-deck list */

/* List Filter screen (gListFilterSteps) and sorting. */
void DeckEdit_FilterAndSortList(u8 list, u8 filter, u8 sort);   /* fill row 1 of a list: enum ListFilter, ListSort */
void QuickSortS16(int n, s16 *arr, u16 (*cmp)(s16, s16));       /* in-place sort; cmp != 0 when a goes before b */
int CompareCardsByAtk(int a, int b);                /* 1 if ATK(a) > ATK(b) (non-monsters 0, Gods 4000) */
int CompareCardsByDef(int a, int b);                /* 1 if DEF(a) > DEF(b) */
u32 CompareCardsByType(int a, int b);               /* 1 if type(a) < type(b) */
u32 CompareCardsByAttribute(int a, int b);          /* 1 if attribute(a) < attribute(b) */
int CompareCardsByLevel(int a, int b);              /* 1 if level(a) > level(b) (non-monsters 0, Gods 10) */
int ListFilter_Reset(void);                         /* step 0: clear the cursors, phase and scroll */
int ListFilter_Init(void);                          /* step 1: graphics, option animations, fade-in */
int ListFilter_Update(void);                        /* step 2: filter and sort pages, then apply the filter */
void ListFilter_DrawCursor(u8 option);              /* cursor sprite of an option (0-6 filters, 7-13 sorts) */
void ListFilter_DrawCursorFlash(u8 option);         /* flash sprite of the confirmed option */
void ListFilter_ShowNowFiltering(void);             /* 'Now Filtering' text and the empty progress bar */
void ListFilter_CopyBarTileColumns(u16 *src, u16 *dst, u8 width); /* copy the first `width` pixel columns of a tile */
void ListFilter_DrawProgressBar(const void *src, void *dst, u8 width); /* progress bar `width` pixels long */
void ListFilter_ProgressBarVBlank(void);            /* VBlank callback: advance the progress bar while sorting */

/* Statistics screen (gDeckStatsSteps). */
u16 DeckStats_ClearState(void);                     /* step 0: clear the sub-screen fields */
int DeckStats_Init(void);                           /* step 1: graphics, fade-in, DeckStats_Compute */
u16 DeckStats_Update(void);                         /* step 2: numbers, panel blend, exit on A/B */
u32 DeckStats_CountCategory(u8 list, u8 category);  /* copies in a list's shown row of an enum DeckStatsCategory */
void DeckStats_Compute(void);                       /* fill the struct DeckStatsRow[7] table in gScratchBuffer */
void DeckStats_DrawNumbers(void);                   /* count and percent sprites of every row */

/* List view: scrolling, list switching and the panels below the list. */
void DeckEdit_ScrollListUp(u16 *enteringCard);      /* move the cursor up one card and start the scroll */
void DeckEdit_ScrollListDown(u16 *enteringCard);    /* move the cursor down one card and start the scroll */
void DeckEdit_StartListSlide(u8 dir);               /* slide to the next (2, R) or previous (3, L) list */
void DeckEdit_HandleShoulderKeys(u8 *list);         /* R/L: switch the list and start the slide */
void DeckEdit_DrawStatementLabels(u8 list);         /* 'STATEMENT' label block of a list */
void DeckEdit_DrawCardCounts(u8 list);              /* copy, deck size and STATEMENT number sprites */
void DeckEdit_UpdateNameIndexLetters(void);         /* render the cursor card's first two letters for the tab */
void DeckEdit_DrawNameIndexTab(void);               /* name-index tab next to the scroll bar (name sort only) */
void DeckEdit_PlaceCardArt(u8 col, u8 row, u8 page, u8 wrap);   /* place a 9x10-tile card art page on the art BG */
void ClearTile4bpp(void *tileBase, int index);      /* clear one 4bpp tile */

/* Command bar. The definitions in deck_edit_list still take the unit's local views (struct CellArr_x for the
   AnimState array, struct Slide for the command menu, same layouts); they switch to these types when it migrates. */
void DeckEdit_UpdatePanelHighlight(u8 *list, u8 *prevList, struct AnimState *anims); /* list panel highlight */
void DeckEdit_CommandLabelVBlank(void);             /* VBlank: load the selected command's label tiles, set WIN1 */
void DeckEdit_CommandWindowVBlank(void);            /* VBlank: WIN1 below the visible rows of the bar */
void DeckEdit_DrawCommandMenu(struct DeckEditCommandMenu *menu);       /* draw the bar for its animation state */
void DeckEdit_UpdateCommandMenuAnim(struct DeckEditCommandMenu *menu); /* open/close the bar, set menuOpen */

/* Card rows and the detail panel (names, icons, ATK/DEF, level). */
void DeckEdit_DrawPortraitTilemap(u8 col, u8 row, u8 page, u8 wrap, u8 tileBase); /* 9x10 portrait map on BG3 */
u32 DeckEdit_GetListRowVram(u8 slot);               /* VRAM address of a row-name buffer (7-buffer ring) */
u32 DeckEdit_GetCursorRowVram(void);                /* VRAM address of the cursor-row text page */
u16 DeckEdit_GetListRowTile(u8 slot);               /* first tile number of a row-name buffer */
u32 DeckEdit_GetCursorRowTile(void);                /* first tile number of the cursor-row text page */
void DeckEdit_RotateListRowRing(u8 dir);            /* rotate the row-name ring after a scroll (1 up, 2 down) */
int DeckEdit_FlipCursorRowPage(void);               /* toggle cursorRowPage (FAKEMATCH: returns nothing) */
void DeckEdit_DrawListRowName(u16 cardId, u8 *map, u16 col, u16 row, u32 unused, u8 slot); /* name of a list row */
void DeckEdit_DrawCursorRowName(u16 cardId, u8 *map, u16 col, u16 row); /* cursor card's name; sets its frame */
void DeckEdit_DrawNoCardsText(u32 unusedCardId, u8 *map, u16 col, u16 row); /* 'There are no cards.' */
void DeckEdit_LoadCardIconTiles(u8 *dst);           /* copy the 2x2-tile card icons and ATK/DEF/star tiles */
void DeckEdit_DrawCardIcon(u8 set, u8 idx, u16 *map, u8 col, u8 row, u8 pal, u16 tileBase); /* one icon, enum CardIconSet */
void DeckEdit_DrawCardIcons(u16 rowOffset);         /* attribute/type/kind icons of the cursor card */
void DeckEdit_DrawAtkDef(u16 *map, u16 col, u16 row); /* ATK/DEF of the cursor card */
void DeckEdit_DrawLevelStars(u8 *map, u16 col, u16 row, u8 perRow); /* level stars of the cursor card */
void sub_08066164(u8 *dst);                         /* copy 12 BG tile blocks (card-row graphics) to dst */

/* Scroll bar, card-frame slots and the card-move animation. Parameters typed u8 * point to the struct named in
   the comment (the definitions take u8 *). */
void DeckEdit_CalcScrollBar(u16 count, u16 pos, u16 *bar);      /* bar: struct DeckEditScrollBar (thumbLen, thumbPos) */
void DeckEdit_DrawScrollBar(u16 pos, u16 count, struct DeckEditScrollBar *bar); /* thumb sprites and arrow cells */
void DeckEdit_ResetFrameSlots(u8 *ring);            /* struct FrameSlotRing: all slots off, head = 5 */
void DeckEdit_TweenFrameSlots(u8 step, u8 easeState, u8 dir, u8 *affines, u8 *ring); /* per-frame slot tween */
void DeckEdit_ScrollFrameSlots(u8 dir, u16 newCardId, u8 unusedCount, u8 *ring, u8 *affines); /* start a scroll step */
void DeckEdit_DrawFrameSlots(u8 *slots, int oamList); /* &ring->slot[0]: emit the frame sprites */
void DeckEdit_InitFrameSlot(u8 slot, u16 cardId, u8 pos, u8 *ring, u8 *affines); /* place a card at a position */
void DeckEdit_ResetCardMove(u8 *move);              /* struct CardMove: step = CARD_MOVE_IDLE */
void DeckEdit_StartCardMove(u8 frame, u8 destList, u8 *move); /* step = CARD_MOVE_CHECK toward destList */
void DeckEdit_BeginCardMove(u8 *move);              /* start the pick-up animation, hide the last copy's slot */
void DeckEdit_UpdateCardMove(u8 *move);             /* per-frame card-move state machine (enum CardMoveStep) */

#endif /* GUARD_DECK_EDIT_H */
