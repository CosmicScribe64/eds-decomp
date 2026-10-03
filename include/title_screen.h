#ifndef GUARD_TITLE_SCREEN_H
#define GUARD_TITLE_SCREEN_H

/*
 * Boot license screens and the title screen (units title_screen and title_menu).
 *
 * CB_License runs gLicenseSteps (Nintendo notice, Konami logo, KCEJ logo); the last step installs CB_Title,
 * which runs gTitleSteps[gMain.seqIndexTop] (enum TitleStep): New Game / Continue, the delete-save prompt and
 * the New Game intro. Both step tables are local to their units.
 */

#include "global.h"

/* Title screen state at 0x0201527C, cleared by Title_Init. */
struct TitleState {
    u16 bgScroll;               /* +0x00: BG3 scroll, decremented every frame by Title_VBlank */
    u16 savePresent:1;          /* +0x02 bit 0: IsSaveChecksumValid() at Title_Init */
    u16 continueSelected:1;     /* +0x02 bit 1: menu cursor, 1 = Continue, 0 = New Game */
};

typedef char title_screen_h_check_size[sizeof(struct TitleState) == 0x4 ? 1 : -1];

/* Index into gTitleSteps (gMain.seqIndexTop under CB_Title). Title_ConfirmDeleteSave jumps back to
 * TITLE_STEP_SETUP when the player cancels. */
enum TitleStep {
    TITLE_STEP_INIT = 0,
    TITLE_STEP_SETUP = 1,
    TITLE_STEP_FADE_IN = 2,
    TITLE_STEP_HANDLE_INPUT = 3,
    TITLE_STEP_FADE_OUT = 4,
    TITLE_STEP_CONFIRM_DELETE_SAVE = 5,
    TITLE_STEP_START_GAME = 6,
};

/* Title screen state (title_screen and title_menu). */
extern struct TitleState gTitleState;

/* ---- Boot license screens (gLicenseSteps; each step returns 1 when done) ---- */

/* Step 0: white brightness, display off, video reset, default BG0-3CNT and a white backdrop. */
u16 License_InitVideo(void);
/* Step 1: "LICENSED BY NINTENDO" with an outline in BG1; fade in from white, hold 120 frames, fade out. */
u16 License_ShowNintendoNotice(void);
/* Step 2: the Konami logo image; fade in from white, hold 120 frames, fade out to white. */
u16 License_ShowKonamiLogo(void);
/* Step 3: the KCEJ logo; fade in, hold, fade to black, then clear the blank callbacks and install CB_Title. */
u16 License_ShowKcejLogo(void);
/* Boot scene callback: runs gLicenseSteps[gMain.seqIndexTop]; in practice never returns 1 because
 * License_ShowKcejLogo installs CB_Title itself. */
u16 CB_License(void);

/* ---- Title screen: callbacks and drawing ---- */

/* HBlank handler: wavy logo, REG_BG1HOFS = gMain.hblankScroll[(VCOUNT + frameCounter) & 15]. */
void Title_HBlank(void);
/* VBlank callback: decrements gTitleState.bgScroll and scrolls BG3 diagonally (VOFS = scroll >> 2). */
void Title_VBlank(void);
/* Adds the "New Game" and "Continue" label sprites; the unselected option uses the dimmed tiles. */
void Title_DrawMenu(void);
/* Display off (DISPCNT = 0) and the default BG0-3CNT (0x84, 0x105, 0x206, 0x307). */
void Title_InitBgCnt(void);
/* One-shot setup: menu label tiles, the logo, flame, coin and BG3 grid images and the grid map. */
void Title_LoadGraphics(void);
/* Adds the 5x4 grid of 64x32 sprites that shows the full-screen delete-save prompt picture. */
void Title_DrawDeletePrompt(void);

/* ---- Title screen steps (gTitleSteps, enum TitleStep; each returns 1 when done) ---- */

/* TITLE_STEP_INIT: clears gTitleState, savePresent = continueSelected = IsSaveChecksumValid(). */
u16 Title_Init(void);
/* TITLE_STEP_SETUP: video/BG reset, Title_LoadGraphics, title song. */
u16 Title_Setup(void);
/* TITLE_STEP_FADE_IN: fade in from black with the wavy logo, then flash to white and show the menu. */
u16 Title_FadeIn(void);
/* TITLE_STEP_HANDLE_INPUT: Left/Right toggle New Game/Continue when a save exists; A confirms. */
u16 Title_HandleInput(void);
/* TITLE_STEP_FADE_OUT: draws the menu and fades to black, then clears the VBlank callback. */
u16 Title_FadeOut(void);
/* TITLE_STEP_CONFIRM_DELETE_SAVE: only for New Game with a save present; START goes on (the save is
 * wiped by Title_StartGame), B goes back to TITLE_STEP_SETUP. */
u16 Title_ConfirmDeleteSave(void);
/* TITLE_STEP_START_GAME: Continue returns 1 at once; New Game plays Tea's intro (dialogue event 100) and
 * runs the starter deck choice. */
u16 Title_StartGame(void);
/* Title scene callback: runs gTitleSteps[gMain.seqIndexTop], advancing and resetting the sub-states when a
 * step returns 1; returns 1 at the NULL terminator. */
u16 CB_Title(void);

#endif /* GUARD_TITLE_SCREEN_H */
