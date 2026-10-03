#ifndef GUARD_CARD_MENU_H
#define GUARD_CARD_MENU_H

/*
 * The card command menu: the row of command icons (Card View, Set, Summon, Activate, Attack, ...) that opens
 * over the card under the cursor. CardMenu_GetAvailableCommands builds the command mask (enum
 * CardMenuCommandMask in constants/duel.h) when the menu opens; CardMenu_Update lets the player pick a
 * command and sets gDuel.cardMenu.confirmed; CardMenu_Execute then runs it. Several commands are step machines
 * on gDuel.cardMenu.step that run again every frame until they clear confirmed. The CPU fills gDuel.cardMenu
 * and gDuel.cardMenuCard itself and uses the same executors. The menu state, struct CardMenu, is in duel.h.
 *
 * Every prototype is the function's definition as compiled. Units that call a function through another
 * local declaration keep that view as a commented local alias prototype (build/readability/proto_mismatches.txt).
 */

#include "global.h"

struct ChainEntry;

/* gDuel.cardMenu.state, the steps of CardMenu_Update. */
enum CardMenuState {
    CARDMENU_STATE_INIT = 0,            /* put the cursor on command 0 or the last available one */
    CARDMENU_STATE_SLIDE_IN = 1,
    CARDMENU_STATE_INPUT = 2,           /* Left/Right pick, A confirms, B closes */
    CARDMENU_STATE_SLIDE_OUT = 3,
    CARDMENU_STATE_CLOSE = 4,
    CARDMENU_STATE_DETAIL_FADE = 10,    /* Card View: open Card Detail */
    CARDMENU_STATE_DETAIL = 11,
    CARDMENU_STATE_RETURN = 12
};

/* ---- The menu ---- */

/* Per-frame state machine of the menu (enum CardMenuState); confirms a command or opens Card View. */
void CardMenu_Update(void);
/* Command mask for the card under the cursor, stored in gDuel.cardMenu.available when the menu opens. */
u16 CardMenu_GetAvailableCommands(void);
/* Draw the available commands' icons in a centred row; the selected one pulses. */
void CardMenu_DrawIcons(void);
/* Draw the selected command's name label above the icons. */
void CardMenu_DrawLabel(void);
/* Draw the card under the cursor sliding and zooming to the top of the screen. */
void CardMenu_DrawCardPreview(void);

/* ---- Available commands ---- */

/* Commands for a hand card (Set, Summon, Special Summon, Activate), Main Phases only. */
int CardMenu_GetHandCardCommands(u16 cardId, int player);
/* Commands for player 0's monster in zone (position changes, Flip, Activate, Attack). */
u16 CardMenu_GetMonsterCommands(u16 cardId, int player, int zone);
/* CARDMENU_MASK_ACTIVATE or 0 for player 0's card in spell/trap zone zone + 5 (5 = the field zone). */
int CardMenu_GetSpellTrapCommands(u16 cardId, int player, int zone);
/* Can the face-up monster in (player, zone) use its activated effect now? */
int CanActivateMonsterEffect(u16 cardId, int player, int zone);

/* ---- Running a command ---- */

/* Run the confirmed command (gDuel.cardMenu.command, enum CardMenuCommand). */
void CardMenu_Execute(void);
/* Set / Summon / Special Summon of the hand card gDuel.cardMenuCard, with the tribute picks and the
 * special summon procedures (step machine on gDuel.cardMenu.step). */
void CardMenu_SummonMonster(u16 faceUp, u16 special);
/* Set or activate the hand Magic/Trap gDuel.cardMenuCard; asChainLink adds it to the chain being built
 * as a response to trigger (Chain_AskResponse), else it starts a new chain. */
void CardMenu_PlaySpellTrapFromHand(u16 activate, u16 asChainLink, struct ChainEntry *trigger);
/* Flip command: queue the Flip Summon of the selected monster. */
void CardMenu_FlipSummon(void);
/* Def Pos (1) / Atk Pos (2) commands: change the selected monster's battle position. */
void CardMenu_ChangePosition(u16 command);
/* Fusion command on the Fusion Deck: run Polymerization's resolve as card key 1547 (cannot appear in EDS). */
void CardMenu_FusionSummon(void);

#endif /* GUARD_CARD_MENU_H */
