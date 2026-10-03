#ifndef GUARD_MAIN_MENU_H
#define GUARD_MAIN_MENU_H

/*
 * Main menu and the Record ("Duel Score") screen (unit main_menu).
 *
 * CB_MainMenu runs gMainMenuSteps[gMain.seqIndex1]; MainMenu_Launch then installs
 * gMainMenuTable[gMainMenuCursor] (enum MainMenuItem) as the main callback. CB_Record (item 3) runs
 * gRecordSteps[gMain.seqState2]. The step and launch tables are local to the unit.
 */

#include "global.h"

/* Record screen state at 0x0201F814, cleared by Record_Init. Pages are 5 duelists each (page 4 has 4);
 * a page turn draws the target page into the hidden 256-px half of BG1/BG2 and scrolls over 16 frames. */
struct RecordScreen {
    u16 shownHalf:1;            /* +0x00 bit 0: half of BG1/BG2 on screen (0 = HOFS 0, 1 = HOFS 256) */
    u16 scrollDir:2;            /* +0x00 bits 1-2: enum RecordScroll; 0 = idle */
    u16 scrollTimer:10;         /* +0x00 bits 3-12: page-scroll frames left, 16 -> 0 */
    u16 page:3;                 /* +0x00 bits 13-15: shown page 0-4 (rows show duelists page*5+1 ..) */
    u16 unk2;                   /* +0x02: cleared, otherwise unused */
};

typedef char main_menu_h_check_size[sizeof(struct RecordScreen) == 0x4 ? 1 : -1];
typedef char main_menu_h_check_unk2[(u32)&((struct RecordScreen *)0)->unk2 == 0x2 ? 1 : -1];

/* Main menu item: gMainMenuCursor, index into gMainMenuTable. */
enum MainMenuItem {
    MAIN_MENU_CAMPAIGN = 0,
    MAIN_MENU_LINK_BATTLE = 1,
    MAIN_MENU_DECK_EDIT = 2,
    MAIN_MENU_RECORD = 3,
    MAIN_MENU_CALENDAR = 4,
    MAIN_MENU_CARD_TRADING = 5,
    MAIN_MENU_PASSWORD = 6,
    MAIN_MENU_COUNT = 7,
};

/* Page scroll of the Record screen: gRecordScreen.scrollDir; scrollDir - 1 indexes gRecordScrollHofs. */
enum RecordScroll {
    RECORD_SCROLL_NONE = 0,
    RECORD_SCROLL_NEXT = 1,     /* page + 1 */
    RECORD_SCROLL_PREV = 2,     /* page - 1 */
};

/* Selected main menu item (enum MainMenuItem), kept between visits. */
extern u16 gMainMenuCursor;
/* Record screen state. */
extern struct RecordScreen gRecordScreen;

/* ---- Main menu (gMainMenuSteps; each step returns 1 when done) ---- */

/* Draws the "MENU" header and the 7 item rows; the row under gMainMenuCursor uses the highlighted tiles. */
void MainMenu_DrawItems(void);
/* Step 0 (3 sub-states): video reset, BG1CNT, menu music, OBJ palette/tiles and the sky image. */
u16 MainMenu_Init(void);
/* Step 1: DISPCNT = BG1 | OBJ, draw the items, fade in from black. */
u16 MainMenu_FadeIn(void);
/* Step 2: Up/Down wrap gMainMenuCursor over the 7 items; A plays SE 1, fades out the BGM and returns 1. */
u16 MainMenu_HandleInput(void);
/* Step 3: fade to black, then SetMainCallback(gMainMenuTable[gMainMenuCursor]); always returns 0. */
u16 MainMenu_Launch(void);
/* Main menu scene callback: runs gMainMenuSteps[gMain.seqIndex1]; returns 1 at the NULL terminator. */
u16 CB_MainMenu(void);

/* ---- Record screen (gRecordSteps; each step returns 1 when done) ---- */

/* Step 0: clears gRecordScreen, black screen, BG0CNT = 4, BG1/BG2CNT 512x256 for the page scroll. */
u16 Record_Init(void);
/* Step 1: marker/arrow OBJ graphics, the digit tiles, the "DUEL SCORE" frame and the first page. */
u16 Record_LoadGfx(void);
/* Step 2: draw the sprites, enable BG0-3 and OBJ, fade in from black. */
u16 Record_FadeIn(void);
/* Step 3: runs a page scroll (BG1/BG2 HOFS from gRecordScrollHofs); Right/R and Left/L start one when that
 * page exists, A or B leaves. */
u16 Record_HandleInput(void);
/* Step 4: draw the sprites and fade to black; the NULL terminator then returns to the main menu. */
u16 Record_FadeOut(void);
/* Record scene callback (main menu item 3): runs gRecordSteps[gMain.seqState2]. Unlike the other runners
 * it does not reset seqState1/2 between steps. */
u16 CB_Record(void);

/* Returns 1 if the shown page is not the first. */
u16 Record_HasPrevPage(void);
/* Returns whether the next page is unlocked: page N + 1 opens when all five duelists of page N have more
 * than N + 1 wins (same test as the opponent-select R button). */
u16 Record_HasNextPage(void);
/* Draws the animated left/right page arrows when there is a previous/next page. */
void Record_DrawPageArrows(void);
/* Unless scrolling, puts an animated marker under WIN, DRAW or LOSE for each row of the shown page. */
void Record_DrawResultMarkers(void);
/* Per-frame sprites of the Record screen: result markers, then page arrows. */
void Record_DrawSprites(void);
/* Writes value as 3 decimal digits with leading zeros into gMain.bgMapBuffer[screenblock], ending at
 * tileX + 2 on row tileY. */
void Record_DrawNumber(s32 screenblock, s32 tileX, s32 tileY, s32 value);
/* Draws page `page` (Win/Draw/Lose frame, names, numbers) into BG half `half` of BG1/BG2. Rows are ordered
 * by duelist id (page*5 + row + 1). */
void Record_DrawPage(s32 page, s32 half);

#endif /* GUARD_MAIN_MENU_H */
