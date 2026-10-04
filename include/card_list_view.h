#ifndef GUARD_CARD_LIST_VIEW_H
#define GUARD_CARD_LIST_VIEW_H

/*
 * Duel card-list viewer: the full-screen list opened from the duel field for a graveyard, the fusion deck, the
 * banished cards or the deck, and by card effects for a list of targets ('Designation'). State in gCardListView
 * (struct CardListView, 0x0201D810); CardListView_Open starts it and DuelMainStep calls CardListView_Run each frame.
 *
 * Units: turn_order_steps (drawing and screen setup, 0x08029F04..), card_list_viewer (runner, input, open).
 * Wiki: wiki/functions/turn-order-steps-c.md, wiki/functions/card-list-viewer-c.md.
 *
 * Matching notes for units that migrate to this header:
 *  - About 20 units declare gCardListView with their own local struct (ListView, SelBlk, PickCursor, ...); keep a
 *    local view where the canonical one changes the code. card_list_viewer itself uses struct CardListView.
 *  - gCardListViewCards is a second symbol for gCardListView.cards (0x0201D81C); units read it as u32[], u16[] or
 *    local structs. Keep each unit's access form.
 *  - CardListView_Open is called with u16/u32 views of its parameters; those units keep a commented local alias
 *    prototype (build/readability/proto_mismatches.txt). CardListView_DrawButtons matches through the header
 *    prototype in card_list_viewer.
 */

#include "global.h"
#include "duel.h"       /* struct DuelCard */

/* What the viewer shows (gCardListView.mode); also selects the title graphic. */
enum CardListViewMode {
    CARDLIST_MODE_YOUR_GRAVEYARD = 0,       /* area 14, player 0 */
    CARDLIST_MODE_OPPONENT_GRAVEYARD = 1,   /* area 14, player 1 */
    CARDLIST_MODE_FUSION_DECK = 2,          /* area 12 */
    CARDLIST_MODE_BANISHED = 3,             /* area 15, title 'Excepted Card' */
    CARDLIST_MODE_TARGETS = 4,              /* area -1, title 'Designation': effect targets */
    CARDLIST_MODE_DECK = 5,                 /* area 13 */
};

/* Index into gCardListViewSteps (gCardListView.step). */
enum CardListViewStep {
    CARDLIST_STEP_INIT = 0,                 /* CardListView_InitScreen */
    CARDLIST_STEP_INPUT = 1,                /* CardListView_HandleInput */
    CARDLIST_STEP_EXIT = 2,                 /* CardListView_Exit */
};

/* gCardListView.state during CardListView_HandleInput (0 and >= 5 take input). */
enum CardListViewInputState {
    CARDLIST_INPUT_IDLE = 0,
    CARDLIST_INPUT_FADE_OUT = 1,            /* Card View: fade out */
    CARDLIST_INPUT_OPEN_DETAIL = 2,         /* CardDetail_Init */
    CARDLIST_INPUT_WAIT_DETAIL = 3,         /* wait for CardDetail_Run */
    CARDLIST_INPUT_REINIT = 4,              /* re-run CardListView_InitScreen */
};

/* Buttons of the top bar (gCardListView.button; bit index in buttonMask). */
enum CardListViewButton {
    CARDLIST_BUTTON_CARD_VIEW = 0,
    CARDLIST_BUTTON_EXIT = 1,
    CARDLIST_BUTTON_ARRANGE = 2,            /* only plays a sound */
    CARDLIST_BUTTON_DECIDE = 3,             /* target lists */
};

/* gCardListView.cursorMoveDir, and the length of the cursor-box slide (cursorMoveTimer counts it down). */
enum CardListCursorMove {
    CARDLIST_CURSOR_IDLE = 0,
    CARDLIST_CURSOR_UP = 1,
    CARDLIST_CURSOR_DOWN = 2,
};
#define CARDLIST_CURSOR_SLIDE_FRAMES 4

/* The area argument of CardListView_Open for a list of effect targets (CollectEffectTargets of cardNumber)
 * instead of a pile. */
#define CARDLIST_AREA_EFFECT_TARGETS (-1)

/* Where an effect target comes from (gCardListView.sources[], written by the effect target collector). These are
   masks, not the viewer's area numbers 12-15. */
enum CardListSource {
    CARDLIST_SRC_HAND = 1,
    CARDLIST_SRC_DECK = 2,
    CARDLIST_SRC_GRAVEYARD = 4,
    CARDLIST_SRC_FUSION_DECK = 8,
    CARDLIST_SRC_BANISHED = 16,
};

/* gCardListView (0x0201D810): state of the card-list viewer. */
struct CardListView {
    u8 active:1;                    /* +0x000 bit 0: viewer running (CardListView_Run returns 1) */
    u8 player:1;                    /* +0x000 bit 1: owner of the listed pile, 0 you / 1 opponent */
    u8 textDirty:1;                 /* +0x000 bit 2: convert the name canvas to tiles at 0x06004200 next update */
    u8 portraitPage:1;              /* +0x000 bit 3: alternating portrait tile/palette slot */
    u8 showSprites:1;               /* +0x000 bit 4: draw the button bar and the status icons */
    u8 mode:3;                      /* +0x000 bits 5-7: enum CardListViewMode */
    u8 step;                        /* +0x001: enum CardListViewStep */
    u8 state;                       /* +0x002: state of the current step (enum CardListViewInputState in step 1) */
    u8 initState;                   /* +0x003: state of CardListView_InitScreen */
    u8 unk4;                        /* +0x004: cleared by the runner, otherwise unused */
    u8 cursorRow:2;                 /* +0x005 bits 0-1: cursor row on screen; the chosen entry is cards[top + cursorRow] */
    u8 cursorMoveTimer:3;           /* +0x005 bits 2-4: frames left of the cursor-box slide (4 -> 0) */
    u8 cursorMoveDir:2;             /* +0x005 bits 5-6: 1 up, 2 down, 0 idle */
    u8 unk5_7:1;
    u16 top;                        /* +0x006: first visible entry */
    u8 button:2;                    /* +0x008 bits 0-1: selected enum CardListViewButton */
    u8 buttonMask:4;                /* +0x008 bits 2-5: enabled buttons, bit per enum CardListViewButton */
    u8 unk8_6:2;
    u8 unk9[3];
    u32 cards[0x80];                /* +0x00C: struct DuelCard words of the entries (id = bits 0-11). Alias symbol
                                       gCardListViewCards */
    u16 sources[0x80];              /* +0x20C: target lists only: enum CardListSource of each entry */
    u16 count;                      /* +0x30C: number of entries */
};

/* Compile-time layout checks (agbcc pads every struct to 4 bytes). */
typedef char card_list_view_h_check_size[sizeof(struct CardListView) == 0x310 ? 1 : -1];
typedef char card_list_view_h_check_top[(u32)&((struct CardListView *)0)->top == 0x6 ? 1 : -1];
typedef char card_list_view_h_check_cards[(u32)&((struct CardListView *)0)->cards == 0xC ? 1 : -1];
typedef char card_list_view_h_check_sources[(u32)&((struct CardListView *)0)->sources == 0x20C ? 1 : -1];
typedef char card_list_view_h_check_count[(u32)&((struct CardListView *)0)->count == 0x30C ? 1 : -1];

extern struct CardListView gCardListView;          /* 0x0201D810 */
extern struct DuelCard gCardListViewCards[0x80];   /* 0x0201D81C: the same memory as gCardListView.cards */

/* 0x0819A7B8: the viewer's steps (enum CardListViewStep), NULL-terminated. */
extern u16 (*const gCardListViewSteps[])(void);
/* 0x0819A788: BG1 scroll offsets of the cursor-box slide, [cursorMoveDir][cursorMoveTimer]: up 15, 13, 10, 5;
 * down -15, -13, -10, -5 (row 0 unused). */
extern const s32 gCardListViewCursorSlide[][4];

/* Opening and running the viewer. */
void CardListView_Open(int player, int area, int cardNumber, int arg); /* area 12-15 = a pile (enum DuelArea),
                                                      CARDLIST_AREA_EFFECT_TARGETS = the effect targets collected
                                                      for card number cardNumber */
u16 CardListView_Run(void);                         /* per frame while active: update, then the current step */
u16 CardListView_InitScreen(void);                  /* step 0: screen setup state machine, 1 when faded in */
u16 CardListView_HandleInput(void);                 /* step 1: cursor, buttons, Card View; 1 = close */
u16 CardListView_Exit(void);                        /* step 2: fade back to the duel screen, 1 when done */
void CardListView_Update(void);                     /* per-frame part: text tiles, cursor-box slide, sprites */

/* Drawing. */
void CardListView_DrawPage(void);                   /* names of the four visible entries */
void CardListView_DrawNames(struct DuelCard *cards, s32 count); /* up to 4 names; '[ Unknown ]' for face-down banished */
void CardListView_DrawSelectedInfo(void);           /* CARD PROPERTY panel of the selected entry */
void CardListView_DrawCardInfo(u32 *card);          /* CARD PROPERTY panel: portrait, icons, ATK/DEF, level */
void CardListView_DrawSelectedCursorFrame(void);    /* cursor box for the selected entry's owner */
void CardListView_DrawCursorFrame(u32 isOpponent);  /* YOU (0) or OPPONENT (1) cursor box on BG1 */
void CardListView_DrawCardStatus(struct DuelCard *card); /* CARD STATUS icons of an entry */
void CardListView_DrawButtons(s32 button, u16 buttonMask); /* top button bar, `button` highlighted */

#endif /* GUARD_CARD_LIST_VIEW_H */
