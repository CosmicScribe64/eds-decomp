#ifndef GUARD_CAMPAIGN_H
#define GUARD_CAMPAIGN_H

/*
 * Campaign (main-menu slot 0) and Link Battle (slot 1): the day loop around a duel, opponent selection,
 * pre-duel dialogue, rewards, magazine deliveries, and the unlock rules for opponents and packs.
 *
 * A Campaign day runs gCampaignSteps (enum CampaignStep) from CB_Campaign: pick an opponent (unless an
 * event fixes one), decide who goes first, set up and run the duel, record the result, give rewards and
 * magazines, then advance gSaveData.days by one. Link Battle runs gLinkBattleSteps (enum LinkBattleStep).
 *
 * Code: campaign.c, campaign_steps.c, link_battle.c, campaign_select.c and main_menu.c (opponent select),
 * booster_get_pack.c (unlock checks). Prototypes are the definitions as compiled; units that call a
 * function through a different local prototype keep that view (build/readability/proto_mismatches.txt).
 */

#include "global.h"
#include "util.h" /* struct Coords16 (gOpponentSelectSlotPos) */

/*
 * gCampaignSteps index (gMain.seqIndexCampaign). CB_Campaign runs the step and moves to the next one when
 * it returns nonzero; at a NULL entry (9, 11) it fades to black and the day ends.
 */
enum CampaignStep {
    CAMPAIGN_STEP_START_DAY = 0,       /* Campaign_StartDay: date, events, special duels */
    CAMPAIGN_STEP_SELECT_OPPONENT = 1, /* Campaign_SelectOpponent */
    CAMPAIGN_STEP_TURN_ORDER = 2,      /* Campaign_DecideTurnOrder */
    CAMPAIGN_STEP_SETUP_DUEL = 3,      /* Campaign_SetupDuel */
    CAMPAIGN_STEP_DUEL = 4,            /* DuelMainStep */
    CAMPAIGN_STEP_RESULT = 5,          /* Campaign_ShowDuelResult (an open match goes back 3 steps) */
    CAMPAIGN_STEP_REWARDS = 6,         /* Campaign_GiveRewards */
    CAMPAIGN_STEP_MAGAZINES = 7,       /* Campaign_DeliverMagazines */
    CAMPAIGN_STEP_ADVANCE_DAY = 8,     /* Campaign_AdvanceDay; entry 9 is NULL */
    CAMPAIGN_STEP_DECK_TOO_SMALL = 10, /* Campaign_DeckTooSmall; entry 11 is NULL */
};

/*
 * gLinkBattleSteps index (gMain.seqState0), run by CB_LinkBattle. A link error during steps 1-3
 * (gDuel.linkError) jumps to LINKBATTLE_STEP_LINK_ERROR. Entries 5, 9 and 11 are NULL (scene ends).
 */
enum LinkBattleStep {
    LINKBATTLE_STEP_INIT = 0,           /* LinkBattle_Init: load the deck (40 cards or more) */
    LINKBATTLE_STEP_TURN_ORDER = 1,     /* LinkBattle_DecideTurnOrder: rock-paper-scissors over the link */
    LINKBATTLE_STEP_CONNECT = 2,        /* LinkBattle_Connect: exchange decks, start the duel */
    LINKBATTLE_STEP_DUEL = 3,           /* DuelMainStep */
    LINKBATTLE_STEP_FINISH = 4,         /* LinkBattle_Finish */
    LINKBATTLE_STEP_LINK_ERROR = 6,     /* LinkBattle_ShowLinkError */
    LINKBATTLE_STEP_ERROR_DIALOGUE = 7, /* LinkBattle_RunErrorDialogue */
    LINKBATTLE_STEP_ERROR_SHUTDOWN = 8, /* LinkBattle_ErrorShutdown */
    LINKBATTLE_STEP_DECK_TOO_SMALL = 10, /* LinkBattle_DeckTooSmall */
};

/*
 * gOpponentSelectSteps index (gMain.seqIndex1), run by OpponentSelect_Run. HandleInput writes 5, 6 and 8
 * directly; the page-turn steps write 2. Entries 4, 7 and 10 are NULL (the runner returns 1 at 4).
 */
enum OpponentSelectStep {
    OPPSEL_STEP_INIT = 0,      /* OpponentSelect_Init */
    OPPSEL_STEP_FADE_IN = 1,   /* OpponentSelect_FadeIn */
    OPPSEL_STEP_INPUT = 2,     /* OpponentSelect_HandleInput */
    OPPSEL_STEP_FADE_OUT = 3,  /* OpponentSelect_FadeOut, opponent chosen */
    OPPSEL_STEP_DONE = 4,      /* NULL: OpponentSelect_Run returns 1 */
    OPPSEL_STEP_PREV_PAGE = 5, /* OpponentSelect_PrevPage (L) */
    OPPSEL_STEP_NEXT_PAGE = 6, /* OpponentSelect_NextPage (R) */
    OPPSEL_STEP_CANCEL = 8,    /* OpponentSelect_FadeOut after B */
    OPPSEL_STEP_EXIT = 9,      /* OpponentSelect_ExitToMainMenu */
};

/* A full-screen 256-colour Mode-4 picture: gOpponentSelectPageBgs[5] holds one per page. */
struct Mode4Bitmap {
    const u16 *pal;      /* +0x0 256-colour palette (0x200 bytes) */
    const u8 *bitmap;    /* +0x4 240x160 8bpp bitmap (0x9600 bytes) */
};

/*
 * Dialogue ids an opponent says after a duel, by outcome: one row per opponent of gOpponentResultTexts
 * (0x080817FC), read by Campaign_ShowDuelResult. The examples are Yugi's texts.
 */
struct OpponentResultTexts {
    u16 win;             /* +0x0 the player won (1014) */
    u16 lose;            /* +0x2 the player lost (1013) */
    u16 draw;            /* +0x4 draw (21009) */
    u16 matchDuelWon;    /* +0x6 match still open, the player won this duel (1011) */
    u16 matchDuelNotWon; /* +0x8 match still open after a loss or draw (1010) */
    u16 win5th;          /* +0xA win when duelRecords.wins was 4 before it (1004) */
    u16 win10th;         /* +0xC win when wins was 9 (1017) */
    u16 pad;             /* +0xE always 0 */
};

/* Opponent-select pages: five portraits per page (gOpponentSelectDuelists[page * 5 + slot]); slot 0 of the
 * last page (4) is empty. */
#define OPPONENTS_PER_PAGE  5
#define OPPONENT_SELECT_LAST_PAGE 4

/*
 * gOpponentSelect (0x0201F7E0, 0x34 bytes): the Campaign opponent-select screen. Cleared by
 * OpponentSelect_Init (page 0, slot 0). Slots are a ring of five portraits (gOpponentSelectSlotPos).
 */
struct OpponentSelect {
    u8 page:3;           /* +0x00 bits 0-2: page 0-4 (row of gOpponentSelectDuelists) */
    u16 cursor:3;        /* bits 3-5: selected slot 0-4 on the page */
    u16 targetSlot:3;    /* bits 6-8: slot the cursor sprite moves to; a change restarts the ease */
    u16 moveTimer:4;     /* bits 9-12: ease frames left, 15 -> 0; 0 = idle wobble */
    u32 unk13:4;         /* bits 13-16: only ever written 0 */
    u32 unk17:15;        /* bits 17-31: unused */
    u16 trailX[8];       /* +0x04 cursor x history: [0] current, [1..7] semi-transparent afterimages */
    u16 trailY[8];       /* +0x14 cursor y history */
    s32 moveStartX;      /* +0x24 cursor x when the current ease started */
    s32 moveStartY;      /* +0x28 cursor y when the current ease started */
    s32 targetX;         /* +0x2C target slot x + 16 */
    s32 targetY;         /* +0x30 target slot y */
};

/* Compile-time layout checks (agbcc pads every struct to a multiple of 4 bytes). */
typedef char campaign_h_check_bitmap[sizeof(struct Mode4Bitmap) == 0x8 ? 1 : -1];
typedef char campaign_h_check_texts[sizeof(struct OpponentResultTexts) == 0x10 ? 1 : -1];
typedef char campaign_h_check_win10[(u32)&((struct OpponentResultTexts *)0)->win10th == 0xC ? 1 : -1];
typedef char campaign_h_check_sel[sizeof(struct OpponentSelect) == 0x34 ? 1 : -1];
typedef char campaign_h_check_sel_trail[(u32)&((struct OpponentSelect *)0)->trailX == 0x4 ? 1 : -1];
typedef char campaign_h_check_sel_trail_y[(u32)&((struct OpponentSelect *)0)->trailY == 0x14 ? 1 : -1];
typedef char campaign_h_check_sel_start[(u32)&((struct OpponentSelect *)0)->moveStartX == 0x24 ? 1 : -1];
typedef char campaign_h_check_sel_target[(u32)&((struct OpponentSelect *)0)->targetY == 0x30 ? 1 : -1];

/* Opponent-select screen state (0x0201F7E0); written by the OpponentSelect_* steps and drawing code. */
extern struct OpponentSelect gOpponentSelect;

/* 0x081983AC: top-left of each slot's 64x64 portrait: 0 (89,91) bottom centre, 1 (36,58) left,
 * 2 (1,5) top left, 3 (177,5) top right, 4 (142,58) right. */
extern const struct Coords16 gOpponentSelectSlotPos[5];

/* 0x08081AE4: u16[25] dialogue id per opponent for the first meeting (empty win/loss/draw record); also the
 * default pre-duel text. */
extern const u16 gOpponentFirstMeetingText[];

/* 0x081984CC: the "Win", "Lose", "Draw" labels of the win/loss/draw record (opponent select and the main
 * menu's record display). */
extern const u8 *gWinLoseDrawLabels[3];

/* --- Campaign day loop (gCampaignSteps, run by CB_Campaign) --- */

/* Scene callback (main-menu slot 0): runs gCampaignSteps[gMain.seqIndexCampaign]; fades out at NULL. */
u16 CB_Campaign(void);
/* Step 0: today's date and events; picks the event duel (tournament, Duel Ceremony, Rare Hunter, ...)
 * or leaves the opponent free; with fewer than 40 deck cards goes to CAMPAIGN_STEP_DECK_TOO_SMALL. */
int Campaign_StartDay(void);
/* Step 1: runs OpponentSelect_Run (unless the event fixed the opponent), then the pre-duel dialogue. */
u16 Campaign_SelectOpponent(void);
/* Step 2: rock-paper-scissors for the first duel of a match; later duels let the previous loser choose. */
u16 Campaign_DecideTurnOrder(void);
/* Step 3: Duel_Setup, loads and shuffles the player's deck and loads the opponent's deck. */
u16 Campaign_SetupDuel(void);
/* Step 5 (skipped for opponents 0, 25 and 31): match score (best of 3) and the opponent's result text. */
u16 Campaign_ShowDuelResult(void);
/* Step 6: records the duel, then gives the rewards for the result and gMain.events (cards, packs). */
u16 Campaign_GiveRewards(void);
/* Step 7: on Weekly Jump / V Jump days, delivers the magazine and its pack. */
u16 Campaign_DeliverMagazines(void);
/* Step 8: gSaveData.days++ (every Campaign duel moves the calendar one day); returns 1. */
u16 Campaign_AdvanceDay(void);
/* Step 10: the "deck must contain 40 cards" dialogue; returns 1 when it is closed. */
u16 Campaign_DeckTooSmall(void);

/* --- Campaign helpers --- */

/* Starts the chosen opponent's pre-duel dialogue and sets the duel rules for the day's event. */
int Campaign_StartPreDuelDialogue(void);
/* Records the duel's win/loss/draw in gSaveData.duelRecords and notes the unlocked packs and level. */
void Campaign_RecordDuelResult(void);
/* Gives one copy of every card ID 1-820 the trunk does not hold yet (first meeting with Simon). */
void GiveMissingCards(void);
/* 1 if the player owns at least minCount of the 60 cards in gRareCardNumbers. */
u16 HasEnoughRareCards(int minCount);
/* A random card ID from gRareCardNumbers that the player owns (loops forever if none is owned). */
u16 PickRandomOwnedRareCard(void);

/* --- Unlocks and progress (booster_get_pack.c) --- */

/* Campaign level 1-5: the highest N whose IsCampaignLevelNUnlocked is true (1 if none). */
u32 GetCampaignLevel(void);
/* 1 if duelists 1-5 all have 2+ wins: opens opponent page 2 (duelists 6-10). */
u16 IsCampaignLevel2Unlocked(void);
/* 1 if duelists 6-10 all have 3+ wins: opens page 3 (duelists 11-15). */
u16 IsCampaignLevel3Unlocked(void);
/* 1 if duelists 11-15 all have 4+ wins: opens page 4 (duelists 16-20). */
u16 IsCampaignLevel4Unlocked(void);
/* 1 if duelists 16-20 all have 5+ wins: opens page 5 (Duel Computer). */
u16 IsCampaignLevel5Unlocked(void);
/* 1 if every card ID 1-820 is owned (trunk or deck/side lists); needed for Grandpa. */
u16 IsCardCollectionComplete(void);
/* 1 if duelist duelistId (1-24) can be picked: by campaign level, Championship titles or collection. */
u16 IsOpponentUnlocked(u16 duelistId);
/* 1 if pack packId may be picked on the Get-a-pack list (by campaign level or total wins). */
u16 IsPackUnlocked(u32 packId);

/* --- Link Battle (gLinkBattleSteps, run by CB_LinkBattle) --- */

/* Scene callback (main-menu slot 1): polls link messages during the duel and runs the steps. */
int CB_LinkBattle(void);
/* Step 0: loads the player's deck; fewer than 40 cards goes to LINKBATTLE_STEP_DECK_TOO_SMALL. */
int LinkBattle_Init(void);
/* Step 1: clears gDuel.linkError and runs the linked rock-paper-scissors (TurnOrder_RunRpsLink). */
u16 LinkBattle_DecideTurnOrder(void);
/* Step 2: opens the link, sets up the duel and exchanges decks (12 sub-steps). */
int LinkBattle_Connect(void);
/* Step 4: after the duel, fades out, clears the link-duel flag and shuts the link down. */
int LinkBattle_Finish(void);
/* Step 6: sends the abort message, fades out, stops the music and starts the error dialogue. */
int LinkBattle_ShowLinkError(void);
/* Step 7: keeps sending the abort message until the error dialogue is closed. */
u16 LinkBattle_RunErrorDialogue(void);
/* Step 8: LinkShutdown(); returns 1. */
int LinkBattle_ErrorShutdown(void);
/* Step 10: the "deck must contain 40 cards" dialogue; returns 1 when it is closed. */
int LinkBattle_DeckTooSmall(void);

/* --- Opponent select (gOpponentSelectSteps, run by OpponentSelect_Run) --- */

/* Runs one frame of the opponent-select steps; returns 1 once an opponent is confirmed (gMain.opponent).
 * Called by Campaign_SelectOpponent; B leaves the Campaign instead. */
u16 OpponentSelect_Run(void);
/* Step 0: clears gOpponentSelect, sets up Mode 4, loads the graphics and the first page. */
u16 OpponentSelect_Init(void);
/* Step 1: draws the cursor (no trail), locked covers and record; fades in. */
u16 OpponentSelect_FadeIn(void);
/* Step 2: LEFT/RIGHT move around the slot ring, L/R turn the page, A picks, B cancels. */
u16 OpponentSelect_HandleInput(void);
/* Steps 3 and 8: redraws and fades to black; returns 1 when black. */
u16 OpponentSelect_FadeOut(void);
/* Step 5 (L): slides and fades out the page, loads page - 1, slides it in. */
u16 OpponentSelect_PrevPage(void);
/* Step 6 (R): the same towards page + 1. */
u16 OpponentSelect_NextPage(void);
/* Step 9: SetMainCallback(CB_MainMenu), which leaves the Campaign. */
u16 OpponentSelect_ExitToMainMenu(void);

/* --- Opponent select drawing --- */

/* Loads a page: background palette and bitmap, the names and record labels rendered into OBJ tiles. */
void OpponentSelect_LoadPage(s32 page);
/* Shifts the Mode-4 background by 2 * scroll pixels (REG_BG2X = scroll << 9). */
void OpponentSelect_SetBgScroll(s32 scroll);
/* Draws the 64x64 selection ring at (x, y) from 8 mirrored sprites. */
void OpponentSelect_DrawSelectionRing(s32 x, s32 y);
/* Draws the left page arrow (unless on page 0) and the right one (if the next page is unlocked). */
void OpponentSelect_DrawPageArrows(void);
/* Draws a stone cover over each locked opponent of the page, shifted by the page-turn scroll. */
void OpponentSelect_DrawLockedCovers(s32 scroll);
/* Puts the cursor straight onto slot: whole trail at the slot, no ease. */
void OpponentSelect_SnapCursor(u32 slot);
/* Moves the cursor towards slot (15-frame ease) and draws it; hideTrail != 0 draws only the head. */
void OpponentSelect_DrawCursor(s32 slot, u16 hideTrail);
/* Draws value (clamped to 0-99) as two 8x8 digit sprites at (x, y). */
void OpponentSelect_DrawNumber(s32 x, s32 y, s32 value);
/* Draws the duelist's name banner and Win/Lose/Draw record (duelistId 1-24). */
void OpponentSelect_DrawDuelistInfo(u16 duelistId);

/* --- Empty stubs in link_battle.c (no callers; compiled-out debug code) --- */

void sub_0801A7CC(void);
void sub_0801A7D0(void);
void sub_0801A7D4(void);
void sub_0801A7D8(void);
void sub_0801A7E4(void);
void sub_0801A7EC(void);
void sub_0801A7F0(void);

#endif /* GUARD_CAMPAIGN_H */
