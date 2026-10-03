#ifndef GUARD_DUEL_FLOW_H
#define GUARD_DUEL_FLOW_H

/*
 * Duel flow: the duel-step machine and the phases of a turn.
 *
 * DuelMainStep runs once per frame. When the duel screen, the duel commands, the text box, the pending prompt,
 * the summon and the chain are all idle, it calls the handler of the current duel step,
 * gDuelPhaseTable[gDuelCtrl.phase] (enum DuelStep in constants/duel.h), and moves to the next step when the
 * handler returns 1. gDuel.phaseStep is the handler's own step (the *Step enums below). Steps 2-7 are the
 * turn player's turn (turn start, Draw, Standby, Main with the Battle Phase, End, turn end); step 8 is the
 * opponent's turn (the CPU through AiRunTurn, or the link partner), after which the duel goes back to step 2.
 * The CPU's turn table gAiTurnPhases reuses these handlers, except for the Main Phase.
 *
 * Every prototype is the function's definition as compiled. Units that call a function through another
 * local declaration keep that view as a commented local alias prototype (build/readability/proto_mismatches.txt).
 */

#include "global.h"

/* gDuel.phaseStep in DuelPhase_TurnStart (duel step 2). */
enum TurnStartStep {
    TURN_START_STEP_SCROLL = 0,             /* scroll to the turn player's hand */
    TURN_START_STEP_BEGIN = 1,              /* skipped turn, or DUEL_CMD_TURN_START */
    TURN_START_STEP_MONSTERS = 2,           /* UpdateMonstersAtTurnStart for both players */
    TURN_START_STEP_RESET_FLAGS = 3         /* clear the turn player's per-turn flags */
};

/* gDuel.phaseStep in DuelPhase_Draw (duel step 3). */
enum DrawPhaseStep {
    DRAW_STEP_ENTER = 0,                    /* Draw Phase banner */
    DRAW_STEP_SELECT_DECK = 1,              /* the CPU draws at once; the human gets the cursor on the deck */
    DRAW_STEP_SHOW_CURSOR = 2,
    DRAW_STEP_WAIT_INPUT = 3,               /* the deck's Draw command (or R) advances */
    DRAW_STEP_DRAW = 4                      /* draw one card */
};

/* gDuel.phaseStep in DuelPhase_Standby (duel step 4). Order: 0, 20, 1-11, 100, 101 (with 110-111 and
 * 120-122 per card), 102. */
enum StandbyStep {
    STANDBY_STEP_ENTER = 0,                 /* Standby Phase banner */
    STANDBY_STEP_EFFECTS = 1,               /* ApplyStandbyPhaseEffects */
    STANDBY_STEP_SINISTER_SERPENT = 2,
    STANDBY_STEP_DELAYED_SUMMON = 3,        /* key 1405 */
    STANDBY_STEP_RETURN_BANISHED = 4,       /* Lightforce Sword's face-down banished cards come back */
    STANDBY_STEP_INSPECTION_ASK = 5,        /* the opponent's Inspection (pay 500 LP) */
    STANDBY_STEP_INSPECTION_ANSWER = 6,
    STANDBY_STEP_KEY1517_ASK = 7,
    STANDBY_STEP_KEY1517_ANSWER = 8,
    STANDBY_STEP_TOKEN = 9,                 /* key 1426 token */
    STANDBY_STEP_FIELD = 10,                /* optional activations, 'Complete Standby Phase?' */
    STANDBY_STEP_CONFIRM_ANSWER = 11,
    STANDBY_STEP_DICE = 20,                 /* key 1536: destroy the monsters of the rolled level */
    STANDBY_STEP_DICE_AGAIN = 21,
    STANDBY_STEP_MAINTENANCE_START = 100,   /* maintenance costs (GetMaintenanceLpCost) */
    STANDBY_STEP_MAINTENANCE_SCAN = 101,
    STANDBY_STEP_TURN_COUNTERS = 102,       /* Germ Infection / Stim-Pack turn counters */
    STANDBY_STEP_ASK_PAY_LP = 110,
    STANDBY_STEP_PAY_LP_ANSWER = 111,
    STANDBY_STEP_ASK_TRIBUTE = 120,         /* The Regulation of Tribe / key 1431 tribute maintenance */
    STANDBY_STEP_TRIBUTE_ANSWER = 121,
    STANDBY_STEP_PICK_TRIBUTE = 122
};

/* gDuel.phaseStep in DuelPhase_Main (duel step 5, human only). */
enum MainPhaseStep {
    MAIN_STEP_ENTER = 0,                    /* Main Phase 1 banner */
    MAIN_STEP_FIELD = 1,                    /* field play through the card menu; B -> MAIN_STEP_ASK_END */
    MAIN_STEP_ASK_END = 10,                 /* the phase menu, or 'End your turn?' when no Battle Phase */
    MAIN_STEP_END_TURN_ANSWER = 11,
    MAIN_STEP_MENU_ANSWER = 20,             /* enum PhaseMenuChoice */
    MAIN_STEP_BATTLE_PHASE = 21             /* BattlePhase_Run until done, then Main Phase 2 or the end */
};

/* gTextBox.result of the 3-choice phase menu (end of Main Phase 1 / of the Battle Phase). */
enum PhaseMenuChoice {
    PHASE_MENU_NEXT_PHASE = 0,              /* enter / end the Battle Phase */
    PHASE_MENU_END_TURN = 1,                /* complete the turn */
    PHASE_MENU_CONTINUE = 2                 /* stay in the phase (also chosen by B) */
};

/* gTextBox.menuState of the phase menu (PhaseMenu_HandleInput / PhaseMenu_DrawCursor). */
enum PhaseMenuState {
    PHASE_MENU_STATE_SELECT = 0,
    PHASE_MENU_STATE_CONFIRMED = 1,         /* the cursor blinks for 60 frames */
    PHASE_MENU_STATE_DONE = 2
};

/* gDuel.phaseStep in DuelPhase_End (duel step 6). */
enum EndPhaseStep {
    END_STEP_ENTER = 0,                     /* End Phase banner */
    END_STEP_MUSHROOM_MAN_2 = 1,            /* Mushroom Man #2 control transfer, The Wicked Worm Beast */
    END_STEP_LOW_LEVEL_CHECK = 2,           /* EndPhase_DestroyLowLevelMonsters (key 1322) */
    END_STEP_OPPONENT_LINKS = 3,            /* key 1548 links on the opponent's side */
    END_STEP_SPELL_TRAP_COUNTERS = 4,       /* Swords of Revealing Light, Destiny Board (opponent's cards) */
    END_STEP_KEY1519_ASK = 5,
    END_STEP_KEY1519_ANSWER = 6,
    END_STEP_OWN_LINKS = 10,                /* key 1548 links on the player's monsters */
    END_STEP_DESTROY_LINKED = 11,
    END_STEP_GRAVEYARD_RETURNS = 20,        /* key 1421's graveyard cards return to the hand */
    END_STEP_DESTROYED_TRIGGER_SELF = 21,   /* key 1514 trigger for the player */
    END_STEP_DESTROYED_TRIGGER_OPPONENT = 22,
    END_STEP_HAND_LIMIT = 23                /* discard down to 6 cards */
};

/* gDuel.phaseStep in DuelPhase_TurnEnd (duel step 7). */
enum TurnEndStep {
    TURN_END_STEP_GRAVEROBBED_OWN = 0,      /* the turn player's Graverobber cards go away, one per call */
    TURN_END_STEP_GRAVEROBBED_OPPONENT = 1,
    TURN_END_STEP_COUNTDOWN_MESSAGES = 2,   /* 'turn(s) remaining before ... is destroyed' (human's turn) */
    TURN_END_STEP_CMD_TURN_END = 3,         /* DUEL_CMD_TURN_END */
    TURN_END_STEP_CMD_END_HAND = 4,         /* DUEL_CMD_SHOW_END_TURN_HAND */
    TURN_END_STEP_FINISH = 5                /* reset the AI or send LINKMSG_TURN_END; turnCount++ */
};

/* gDuelCtrl (0x02015EE8, 8 bytes, cleared by Duel_Setup): the current duel step and duel-wide flags. */
struct DuelCtrl {
    u8 phase;                   /* +0x0: current duel step (enum DuelStep), index into gDuelPhaseTable */
    u8 isLinkDuel:1;            /* +0x1 bit 0: Link Battle against another GBA */
    u8 unk1_1:7;
    u8 unk2[2];
    u32 aiFlags;                /* +0x4: enum AiFlag bits of the CPU opponent (set by LoadOpponentDeck) */
};

/* One entry of gOpponentDuelBGM (0x08198F20, 24 entries): the duel music of an opponent (PlayDuelBGM). */
struct OpponentBGM {
    u16 opponent;               /* +0x0: duelist id (gMain.opponent) */
    u16 bgm;                    /* +0x2: song id passed to PlayBGM */
};

extern struct DuelCtrl gDuelCtrl;                   /* 0x02015EE8 */

/* "Select display position of card.": prompt of the summon position menu (summon.h). */
extern const u8 gStrSelectDisplayPosition[];        /* 0x08086370 */
/* 16 affine scales (0x100 down to 0xC0 and back to 0xF8): the pulse of a selected sprite, indexed by
 * the frame counter. */
extern const u16 gPulseScaleCurve[];                /* 0x081A4424 */

/* ---- The duel ---- */

/* Clear the duel state (gDuelCtrl, gDuel, gChain, gBattle, gSummonAction, screens, link); result = draw.
 * Returns 1. */
u32 Duel_Setup(void);
/* Per-frame duel loop: screen, commands, prompts, summon, chain, then the duel-step handler and
 * Duel_CheckWin. Returns 1 when the duel is over (after saving the result). */
u32 DuelMainStep(void);
/* Decide the duel (0 LP, deck out, Exodia, Destiny Board) during steps 2-8; 1 when it is over. */
u16 Duel_CheckWin(void);
/* 1 if the player holds all five Exodia pieces. */
u16 HasExodiaInHand(int player);
/* 1 if keys 0x5F8 and 0x605-0x608 are all active on the player's field (not EDS cards: never true). */
u16 HasDestinyBoardComplete(int player);
/* Start or fade the duel music (opponent's song, championship or link BGM); cheap when already playing. */
void PlayDuelBGM(void);
/* Put the event's Field Magic (gMain.startField) on top of the CPU's deck and play it face up. */
void SetupStartFieldCard(void);

/* ---- Duel steps (gDuelPhaseTable) ---- */

/* Step 0: set the first player; a link duel where the partner starts goes to step 8. */
u32 DuelPhase_Init(void);
/* Step 1: reset the duel (8000 LP), the start Field card, deal five cards each, Start Duel banner. */
u32 DuelPhase_Opening(void);
/* Step 2: start of a turn (enum TurnStartStep); a skipped turn jumps past the turn end. */
int DuelPhase_TurnStart(void);
/* Step 3: the Draw Phase (enum DrawPhaseStep). */
int DuelPhase_Draw(void);
/* Step 4: the Standby Phase (enum StandbyStep). */
int DuelPhase_Standby(void);
/* Step 5: the human's Main Phases and Battle Phase (enum MainPhaseStep). */
int DuelPhase_Main(void);
/* Step 6: the End Phase (enum EndPhaseStep). */
int DuelPhase_End(void);
/* Step 7: end of the turn (enum TurnEndStep). */
int DuelPhase_TurnEnd(void);
/* Step 8: the opponent's turn (AiRunTurn, or the link partner); then back to step 2. */
int DuelPhase_OpponentTurn(void);
/* Step 9: the result scenes and banner; then the duel ends. */
u32 DuelPhase_ShowResult(void);

/* ---- Turn start and Standby Phase ---- */

/* Turn-start updates of the player's face-up monsters (Castle of Dark Illusions, Pumpking). */
void UpdateMonstersAtTurnStart(int player);
/* The automatic Standby Phase effects for the turn player (LP changes, returns, destructions). */
void ApplyStandbyPhaseEffects(int player);
/* 1 if the player has an optional Standby Phase effect to activate (Patrol Robo, Blast Juggler, ...). */
int HasActivatableStandbyCard(int player);
/* LP a card costs to keep each Standby Phase (Mirror Wall 2000, Imperial Order 700, Toon World 500). */
int GetMaintenanceLpCost(u16 cardNumber);

/* ---- Phase menu (end of Main Phase 1 or of the Battle Phase) ---- */

/* Text-box draw callback: the cursor of the 3-choice menu (blinks when confirmed). */
void PhaseMenu_DrawCursor(void);
/* Text-box input callback: Up/Down, A confirms, B picks PHASE_MENU_CONTINUE; 1 when done. */
int PhaseMenu_HandleInput(void);

/* ---- End Phase ---- */

/* Return every face-up The Wicked Worm Beast of the player to the hand. */
void EndPhase_ReturnWickedWormBeast(int player);
/* Offer to pay 500 LP to give each face-up Mushroom Man #2 to the opponent; 1 when no card is left. */
int EndPhase_TransferMushroomMan2(int player);
/* Key 1322: destroy the player's newly face-up monsters of level 3 or lower. */
void EndPhase_DestroyLowLevelMonsters(int player);

/* ---- Unused ---- */

/* Unreferenced: wait for the partner's reply to a response request, then 60 frames; 1 when done. */
u32 sub_0801FE54(void);
/* Dead code: returns 0. */
u32 sub_08021CC8(void);
/* Dead code: clears gDuel bytes +0x1B43 and +0x1B44. */
void sub_08021CCC(void);
/* Dead code: draw two card sprites (the second one face down unless showSecond); the selected one pulses. */
void sub_08021CEC(u16 cardId, u16 showSecond, u16 selected);
/* Dead code: request a link interrupt when none is pending. */
void sub_08022914(void);
/* Dead code: clear gDuel +0x1B14 bits 2-8; returns 0. */
u32 sub_0802295C(void);
/* Empty and unreferenced. */
void sub_0804E3BC(void);
/* Dead code: draw a pulsing icon above or below the cursor (meaning unknown). */
void sub_0804E3C0(void);
/* Dead code: count the monsters other than (skipPlayer, skipZone) that EffectBlastJugglerCheck accepts. */
int sub_0804F6A8(int skipPlayer, int skipZone);

/* Compile-time layout checks (agbcc pads every struct to a multiple of 4 bytes). */
typedef char duel_flow_h_check_ctrl[sizeof(struct DuelCtrl) == 0x8 ? 1 : -1];
typedef char duel_flow_h_check_ai_flags[(u32)&((struct DuelCtrl *)0)->aiFlags == 0x4 ? 1 : -1];
typedef char duel_flow_h_check_bgm[sizeof(struct OpponentBGM) == 0x4 ? 1 : -1];

#endif /* GUARD_DUEL_FLOW_H */
