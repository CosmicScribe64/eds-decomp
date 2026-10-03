#ifndef GUARD_DEBUG_H
#define GUARD_DEBUG_H

/*
 * Leftover developer code: the unreferenced debug menu (src/text_canvas.c), its card viewers
 * (src/card_detail.c) and cheats, and the stripped debug print API (DebugPrintf and DebugPrintFlush are
 * empty in the retail ROM).
 *
 * The debug menu's tables (gDebugMenuItems 0x081A73A0, gDebugMenuSteps 0x081A768C, gCalendarEventNames
 * 0x08087720) and the debug strings are used by one unit each and stay local externs there.
 */

#include "global.h"

/* ------------------------------------------------------------------------------------------------------ */
/* Types                                                                                                  */
/* ------------------------------------------------------------------------------------------------------ */

/* Index into gDebugMenuItems (the menu cursor). B in DebugMenu_HandleInput jumps to DEBUG_ITEM_TITLE. */
enum DebugMenuItemId {
    DEBUG_ITEM_MENU,            /* 0 */
    DEBUG_ITEM_CARD_DETAIL,     /* 1: CB_DebugCardDetail */
    DEBUG_ITEM_AUTO_DETAIL,     /* 2: CB_DebugAutoDetail */
    DEBUG_ITEM_BUSTUP,          /* 3 */
    DEBUG_ITEM_AUTO_BUSTUP,     /* 4 */
    DEBUG_ITEM_GET_ALL_CARDS,   /* 5: CB_DebugGetAllCards */
    DEBUG_ITEM_GET_A_PACK,      /* 6 */
    DEBUG_ITEM_NEXT_LEVEL,      /* 7: CB_DebugNextLevel */
    DEBUG_ITEM_LICENSE,         /* 8 */
    DEBUG_ITEM_TITLE            /* 9 */
};

/* Index into gDebugMenuSteps (gMain.seqIndexCampaign while CB_DebugMenu runs). */
enum DebugMenuStep {
    DEBUG_MENU_STEP_INIT,           /* 0: DebugMenu_Init */
    DEBUG_MENU_STEP_DRAW,           /* 1: DebugMenu_DrawAndFadeIn */
    DEBUG_MENU_STEP_HANDLE_INPUT,   /* 2: DebugMenu_HandleInput */
    DEBUG_MENU_STEP_LAUNCH          /* 3: DebugMenu_Launch */
};

/* One gDebugMenuItems entry: a label and the main callback that DebugMenu_Launch installs. */
struct DebugMenuItem {
    char name[0x40];            /* +0x00 label drawn by DebugMenu_DrawAndFadeIn */
    u32 (*callback)(void);      /* +0x40 scene callback; NULL ends the list */
};

/* One gCalendarEventNames row: a CalendarEvent bit and its debug label (DebugMenu_DrawCalendarEvents). */
struct CalendarEventName {
    u32 mask;                   /* +0x00 enum CalendarEvent bit */
    char name[0x20];            /* +0x04 label */
};

STATIC_ASSERT(sizeof(struct DebugMenuItem) == 0x44, DebugMenuItemSize);
STATIC_ASSERT(sizeof(struct CalendarEventName) == 0x24, CalendarEventNameSize);

/* ------------------------------------------------------------------------------------------------------ */
/* Functions                                                                                              */
/* ------------------------------------------------------------------------------------------------------ */

/* Debug print stubs (src/link_battle.c): compiled out in the retail ROM */
/* printf-style debug output; the variadic prologue spills r0-r3 and returns. */
void DebugPrintf(const char *fmt, ...);
/* Empty; called after DebugPrintf (hypothesis: the flush half of the print API, like AGBPrintFlush). */
void DebugPrintFlush(void);
/* Empty; called once per frame by MainLoop (hypothesis: a stripped debug hook). */
void DebugHook_Nop(void);
/* Assertion run first by every trunk/deck mutator: reports card ID 0 or 1901..1999 (no effect in retail). */
void DebugCheckCardId(u16 cardId);

/* Debug menu (src/text_canvas.c): CB_DebugMenu runs gDebugMenuSteps[gMain.seqIndexCampaign] */
/* Unreferenced main callback of the developer menu; always returns 0. */
u32 CB_DebugMenu(void);
/* Step 0: video setup (BG0 + OBJ 1D), black screen, system font; returns 1. */
u32 DebugMenu_Init(void);
/* Step 1: draws the language, build number 2108, the item list, date and calendar events, then fades in. */
u16 DebugMenu_DrawAndFadeIn(void);
/* Step 2: Up/Down move the cursor, A launches, B picks Title; Left/Right/L/R change gSaveData.days. */
u32 DebugMenu_HandleInput(void);
/* Step 3: SetMainCallback(gDebugMenuItems[cursor].callback); returns 0. */
u32 DebugMenu_Launch(void);
/* Draws today's date (YYYY/MM/DD) on row 1; the holiday/event/weekday lookups it makes are discarded. */
void DebugMenu_DrawDate(void);
/* Draws the names of today's calendar events (and the Rare Hunter notice every 60 days) from row 2. */
void DebugMenu_DrawCalendarEvents(void);
/* Draws the language label of gSaveData.language ("ENG:" ...) at row 1, column 1. */
void DebugMenu_DrawLanguage(void);

/* Debug menu items: main callbacks (src/text_canvas.c, src/card_detail.c) */
/* 'Card Detail': runs gDebugCardDetailSteps; returns 1 at the end of the list. */
u16 CB_DebugCardDetail(void);
/* 'Auto Detail': runs gDebugAutoDetailSteps; returns 1 at the end of the list. */
u16 CB_DebugAutoDetail(void);
/* Plays the Exodia win scene at 1/8 speed (unreferenced); returns 1 when it ends. */
u32 CB_DebugExodiaScene(void);
/* Plays the Destiny Board scene (unreferenced); returns 1 when it ends. */
u32 CB_DebugDestinyBoardScene(void);
/* InitSaveData, then returns 1 (the main menu's SetMainCallback writes the cleared save). */
u32 CB_DebugInitSaveData(void);
/* 'Get all card': DebugGetAllCards; returns 1. */
u32 CB_DebugGetAllCards(void);
/* 'Next Level': adds the wins that clear the next opponent group (or 20 wins to duelists 1-20); returns 1. */
u32 CB_DebugNextLevel(void);

/* Debug card viewers (src/card_detail.c), steps of gDebugCardDetailSteps / gDebugAutoDetailSteps */
/* Shared step 0: sets up Card Detail on card ID 800 and fades in. */
u16 DebugCardDetail_Init(void);
/* Left/Right step the card ID by 1, held R/L by 10 (1..820); A/B fade out and return 1. */
u16 DebugCardDetail_Browse(void);
/* Shows card IDs 1..820 one after another, then fades out and returns 1. */
u16 DebugAutoDetail_Cycle(void);

/* Cheats (src/sprite.c) */
/* Adds copies of every non-token card ID 1..0x334 until 3 are owned. */
void DebugGetAllCards(void);

#endif /* GUARD_DEBUG_H */
