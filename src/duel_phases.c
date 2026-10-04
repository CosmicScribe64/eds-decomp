#include "global.h"
#include "gba.h"                           /* A_BUTTON, B_BUTTON, DPAD_UP, DPAD_DOWN */
#include "main.h"                          /* gMain.newKeys */
#include "util.h"                   /* Random, FormatStr, FormatInt, HalveRoundUp */
#include "sprite.h"                 /* AddSprite */
#include "card_data.h"              /* gCardIdToNumber, gCardNumberToId, gCardNames, CARD_ID_MASK */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/duel.h"         /* enum DuelArea, BattleStage, ZoneLinkKind, PromptKind, ... */
#include "constants/duel_cmds.h"    /* DUEL_CMD_* */
#include "constants/sound.h"        /* SE_* */
#include "duel.h"                   /* gDuel, gDuelPlayers, gDuelZones, struct DuelCard / DuelZone, the zone queries */
#include "sound.h"                  /* PlaySE */

/*
 * Turn start, Standby Phase and Main Phase of a duel turn (wiki/functions/duel-phases-c.md).
 *
 *  - DuelPhase_TurnStart (duel step 2) ends a skipped turn at once; otherwise it pushes DUEL_CMD_TURN_START,
 *    updates the monsters (UpdateMonstersAtTurnStart) and clears the turn player's per-turn flags.
 *  - DuelPhase_Standby (duel step 4, also run for the CPU) applies the automatic Standby Phase effects
 *    (ApplyStandbyPhaseEffects) and a few delayed ones, offers the optional activations, and charges the
 *    upkeep of the cards that need LP or a Tribute to stay on the field (GetMaintenanceLpCost).
 *  - DuelPhase_Main (duel step 5, human player only) runs the Main Phase, the 'End your Main Phase?' menu
 *    (PhaseMenu_DrawCursor / PhaseMenu_HandleInput are its text-box callbacks) and the Battle Phase.
 *
 * Card numbers below that have no CARD_ name in constants/cards.h (1431, 1437, 1438, 1441, 1446) are effect-table
 * rows without handlers, and CARD_14xx names are effect keys of cards that the EDS ROM does not contain; their
 * meanings come from what the code does with them.
 */

#include "duel_flow.h"              /* gDuelCtrl, enum *Step, the functions defined here */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_link.h"              /* DuelLink_SendMessage, LINKMSG_TURN_END */
#include "duel_screen.h"            /* gDuelScreen, DuelScreen_ScrollToZone, DuelCursor_* */
#include "duel_actions.h"           /* ShowCardEffect, LoseLifePoints, DestroyFieldCard, ... */
#include "duel_prompt.h"            /* DuelPrompt_Post */
#include "battle.h"                 /* CanEnterBattlePhase, BattlePhase_Run */
#include "effect.h"                 /* ApplyPumpkingBoost, CanActivateEffectInZone, ... */
#include "summon.h"                 /* QueueSpecialSummon */
#include "chain.h"                  /* Chain_AddPending */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "ai.h"                     /* gAiState */

/* ---- ROM data used only here ---- */

extern const u8 gStrEndMainPhaseMenu[];         /* "End your Main Phase?" with the 3-choice menu: Enter Battle Phase /
                                                 * Complete turn / Continue Main Phase */
extern const u8 gStrEndYourTurn[];              /* "End your turn?" */
extern const u8 gStrCompleteStandbyPhase[];     /* "Complete Standby Phase?" */
extern const char gStrMaintainLpCostFmt[];      /* "To maintain %s on the Field requires %dLP. Do you wish to pay
                                                 * the amount?" */
extern const char gStrMaintainTributeFmt[];     /* "To maintain %s of the Field requires 1 monster as Tribute. Do
                                                 * you wish to tribute a monster?" */
extern const u8 gStrSelectMonsterAsTribute[];   /* "Select a monster as Tribute." */
extern const u16 gCardNumberToId_LightforceSword[];               /* = &gCardNumberToId[CARD_LIGHTFORCE_SWORD]: Lightforce Sword's ID */
extern const u16 gCardNumberToId_1536[];               /* = &gCardNumberToId[CARD_1536]: 0, the dice effect's "card" */

/* ---- Local views kept for matching (build/readability/HEADERS.md) ---- */

/* The card tables of card_data.h through their constant addresses: the ROM loads the table address as a
 * literal and indexes it directly instead of going through the symbol. */
#define CARD_NUMBER_OF(id)  (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])   /* gCardIdToNumber */
#define CARD_ID_OF(number)  (((const u16 *)0x08623DF4)[(number)])              /* gCardNumberToId */
#define CARD_NAME_OF(id)    ((const char *)0x0822C720 + ((id) << 6))           /* gCardNames record */

/*
 * The card word of a zone as one u32, and its card ID (bits 0-11). Matching: the ROM loads the whole word
 * (ldr) and shifts the ID out of it; zone->card.id would load only the halfword (ldrh).
 */
#define ZONE_WORD(zone)         (*(u32 *)&(zone)->card)
#define CARD_WORD_ID(word)      (((word) << 20) >> 20)
#define ZONE_ID(zone)           CARD_WORD_ID(ZONE_WORD(zone))
#define ZONE_HAS_CARD(zone)     ((ZONE_WORD(zone) << 20) != 0)

/*
 * Zone `zone` of `player` as a sum of integers on a table address (0x94 is sizeof(struct DuelZone), 0xD64 the
 * player stride). Matching: the ROM adds the zone and player offsets to the address separately
 * (gDuelZones[p].zones[z] and gDuelPlayers[p].zones[z] fold them), and the order of the two products in the sum
 * decides agbcc's multiply order, so each spelling stays where it is used.
 *   ZONE_PZ / ZONE_ZP   from the gDuelZones symbol (player term first / zone term first)
 *   ZONE_VIA_PLAYERS    from gDuelPlayers[0].zones, zone term first
 *   DUEL_ZONE_*         from &gDuel + 0x2C (= &gDuel.players[0].zones[0]): zone first, player first, and the
 *                       base before the products
 */
#define ZONE_PZ(player, zone)           ((struct DuelZone *)((player) * 0xD64 + (zone) * 0x94 + (u32)&gDuelZones[0]))
#define ZONE_ZP(player, zone)           ((struct DuelZone *)((zone) * 0x94 + (player) * 0xD64 + (u32)&gDuelZones[0]))
#define ZONE_VIA_PLAYERS(player, zone)  ((struct DuelZone *)((zone) * 0x94 + (player) * 0xD64 + (u32)gDuelPlayers[0].zones))
#define DUEL_ZONE_ZP(player, zone)      ((struct DuelZone *)((zone) * 0x94 + (player) * 0xD64 + (u8 *)&gDuel + 0x2C))
#define DUEL_ZONE_PZ(player, zone)      ((struct DuelZone *)((player) * 0xD64 + (zone) * 0x94 + (u8 *)&gDuel + 0x2C))
#define DUEL_ZONE_BASE_FIRST(player, zone) \
    ((struct DuelZone *)((u8 *)&gDuel + 0x2C + (zone) * 0x94 + (player) * 0xD64))

/*
 * Player `p` indexed through a pointer cast of gDuelPlayers. Matching: gDuelPlayers[p] on the array itself
 * lets agbcc fold the index differently; through the pointer the ROM's base load and multiply order result.
 */
#define PLAYER_VIA_PTR(p)       (((struct DuelPlayer *)gDuelPlayers)[p])

/*
 * gDuel reached through the gDuelPlayers symbol (gDuel + 4): the ROM forms some addresses of gDuel fields
 * from the players array.
 */
#define GDUEL_VIA_PLAYERS       ((struct DuelState *)((u8 *)gDuelPlayers - 4))
#define PHASE_STEP_VIA_PLAYERS  (GDUEL_VIA_PLAYERS->phaseStep)

/*
 * turnCounter (+0x06 bits 2-5) of a zone, extracted with shifts as the ROM does. Matching: comparing the
 * bitfield DuelZone.turnCounter compiles to the same instructions but gives another register allocation.
 * The same struct reads the flag byte as a whole (Dark Zebra's position test).
 */
struct DuelZoneFlags6 { u8 pad[6]; u8 flags6; };
#define ZONE_TURN_COUNTER(zone) ((u32)(((struct DuelZoneFlags6 *)(zone))->flags6 << 26) >> 28)

/* Matching: skipTurn (+0x9 bit 3) and skipStandbyPhase (+0x9 bit 1) as signed 1-bit fields: the ROM tests each
 * as the sign of the byte shifted left (lsl; bge) instead of masking the bit. */
struct DuelPlayerSkipTurnView { u8 pad0[9]; u8 unk9_0:3; s8 skipTurn:1; u8 unk9_4:4; };
struct DuelPlayerSkipStandbyView { u8 pad0[9]; u8 handRevealed:1; s8 skipStandbyPhase:1; u8 unk9_2:6; };

/*
 * Matching: local alias prototypes where the unit calls a function through another view than the header's.
 *  - HasActivatableStandbyCard tests CanActivateEffectInZone as an int (the u16 return adds a narrowing).
 *  - DuelPhase_Standby calls FindFaceUpCardOnField2 with two arguments (the header has a third, skipZone).
 *  - CountBlastJugglerTargets passes two ints to EffectBlastJugglerCheck, whose handler parameters are (struct ChainEntry *,
 *    u16 pos).
 */
int CanActivateEffectInZoneInt(int player, int zone, u16 event) asm("CanActivateEffectInZone");
int FindFaceUpCardOnField2Two(int player, u16 cardNo) asm("FindFaceUpCardOnField2");
int EffectBlastJugglerCheckInts(int player, int zone) asm("EffectBlastJugglerCheck");

/* Monster level of a card ID: 0 for Trap, Magic and Ticket cards, 10 for the Divine cards, else the level in
 * the stats word (gCardStats through its constant address). */
static inline int GetCardLevel(u16 cardId)
{
    int type = (((const u32 *)0x08621DE0)[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT;
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return (((const u32 *)0x08621DE0)[cardId & CARD_ID_MASK] & CARD_STATS_LEVEL_MASK) >> CARD_STATS_LEVEL_SHIFT;
    }
}

/* Card number to card ID; 0xFFFF maps to 0, an alternate-art number (2000 + n) to n's ID + 1. */
static inline u16 CardNumberToId(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number < CARD_NUMBER_ALT_ART)
        return ((const u16 *)0x08623DF4)[number & CARD_ID_MASK];
    return ((const u16 *)0x08623DF4)[(number - CARD_NUMBER_ALT_ART) & CARD_ID_MASK] + 1;
}

/*
 * FAKEMATCH: a dead 64-factor product (LOOP_PAD) pads the maintenance loop of DuelPhase_Standby step 101.
 * flow deletes it before register allocation, so it emits nothing, but loop.c counts its insns: the first
 * loop pass then sees ~207 insns (without it 144), finds &zone and the 0xD64 constant not worth hoisting, and
 * leaves them to the second pass. That gives the ROM preheader order (E0 reload, &zone, 0xD64, player*0xD64).
 * The ROM loop evidently had ~201-210 insns at loop time; the window is 58-70 factors (more pushes the second
 * pass over 200 insns).
 */
#define PAD8(x) x*x*x*x*x*x*x*x
#define LOOP_PAD(x) (PAD8(x)*PAD8(x)*PAD8(x)*PAD8(x)*PAD8(x)*PAD8(x)*PAD8(x)*PAD8(x))

#define TEXTBOX_XY(x, y)        ((x) | ((y) << 8))      /* pos / size word of TextBoxOpen: low byte x, high byte y */
#define PHASE_MENU_CURSOR_ATTR2 0x431F                  /* OBJ tile 0x31F, palette 4 */
#define PHASE_MENU_BLINK_FRAMES 60                      /* a confirmed choice blinks for this many frames */

/* The turn player's monster zone `zone` (UpdateMonstersAtTurnStart). */
#define OWN_ZONE(zone)          ZONE_PZ(player & 1, zone)

/*
 * Turn-start update of one player's monsters (both players are updated at the start of a turn). For each
 * face-up monster: clear the attack lock (cannotAttack). Castle of Dark Illusions, if its effect is not used
 * yet and it has been face up for at most 5 turns, links every face-up Zombie on either side that was placed
 * no later than it (QueueAddZoneLink, ZONE_LINK_CONTINUOUS). Pumpking the King of Ghosts, face up for at most
 * 3 turns, gets its boost (ApplyPumpkingBoost).
 */
void UpdateMonstersAtTurnStart(int player)
{
    int zone;
    for (zone = ZONE_MONSTER_0; zone < MONSTER_ZONE_COUNT; zone++) {
        if (ZONE_HAS_CARD(OWN_ZONE(zone)) && OWN_ZONE(zone)->isFaceUp) {
            int castleReady = 0;
            if (OWN_ZONE(zone)->cannotAttack)
                OWN_ZONE(zone)->cannotAttack = 0;
            switch (CARD_NUMBER_OF(ZONE_ID(OWN_ZONE(zone)))) {
            case CARD_CASTLE_OF_DARK_ILLUSIONS:
                if (!OWN_ZONE(zone)->effectUnused)
                    castleReady = 1;
                break;
            case CARD_PUMPKING_THE_KING_OF_GHOSTS:
                if (ZONE_TURN_COUNTER(OWN_ZONE(zone)) <= 3)
                    ApplyPumpkingBoost(player, zone);
                break;
            }
            if (castleReady != 0 && ZONE_TURN_COUNTER(OWN_ZONE(zone)) <= 5) {
                int scanPlayer, scanZone;
                for (scanPlayer = 0; scanPlayer < 2; scanPlayer++) {
                    for (scanZone = ZONE_MONSTER_0; scanZone < MONSTER_ZONE_COUNT; scanZone++) {
                        if (ZONE_HAS_CARD(ZONE_PZ(scanPlayer & 1, scanZone))
                            && ZONE_PZ(scanPlayer & 1, scanZone)->isFaceUp
                            && GetZoneCardType(scanPlayer, scanZone) == CARD_TYPE_ZOMBIE
                            && ZONE_PZ(scanPlayer & 1, scanZone)->serial <= OWN_ZONE(zone)->serial)
                            /* The locations are player | zone << 8 (DUEL_LOC); Matching: written with the player
                             * term first, which DUEL_LOC does not. */
                            QueueAddZoneLink(player, (u8)player | (u8)zone << 8,
                                             (u8)scanPlayer | (u8)scanZone << 8, ZONE_LINK_CONTINUOUS);
                    }
                }
            }
        }
    }
}
#undef OWN_ZONE

/*
 * Duel step 2: the start of the turn player's turn (enum TurnStartStep). Step 0 scrolls the screen to the
 * hand. Step 1 either consumes a skipped turn (clear skipTurn, reset the CPU's state, or tell the link partner,
 * and add 5 to the duel step so that DuelMainStep's +1 lands on DUEL_STEP_OPPONENT_TURN, past the turn end)
 * or pushes DUEL_CMD_TURN_START. Step 2 updates both players' monsters. The last step clears the turn player's
 * per-turn flags. Returns 1 when the step is finished.
 */
int DuelPhase_TurnStart(void)
{
    struct DuelState *duel = &gDuel;
    struct DuelPlayer *players;

    switch (duel->phaseStep) {
    case TURN_START_STEP_SCROLL:
        DuelScreen_ScrollToZone(duel->turnPlayer, DUEL_AREA_HAND);
        duel->phaseStep++;
        return 0;
    case TURN_START_STEP_BEGIN:
        players = duel->players;
        if (((struct DuelPlayerSkipTurnView *)&players[duel->turnPlayer & 1])->skipTurn < 0) {
            players[duel->turnPlayer & 1].skipTurn = 0;
            /* Single player, human turn skipped: the CPU's turn follows at once, so reset its AI state. */
            if (!gDuelCtrl.isLinkDuel && !duel->turnPlayer) {
                gAiState.turnPhase = 0;
                gAiState.step = 0;
            }
            if (gDuelCtrl.isLinkDuel)
                DuelLink_SendMessage(LINKMSG_TURN_END, 0, 0, 0);
            gDuelCtrl.phase += 5;
            return 1;
        }
        DuelCmd_Push(duel->turnPlayer ? DUEL_CMD_TURN_START | DUEL_CMD_PLAYER : DUEL_CMD_TURN_START, 0, 0, 0);
        duel->phaseStep++;
        return 0;
    case TURN_START_STEP_MONSTERS:
        UpdateMonstersAtTurnStart(duel->turnPlayer);
        UpdateMonstersAtTurnStart(1 - duel->turnPlayer);
        duel->phaseStep++;
        return 0;
    default:
        gDuelPlayers[GDUEL_VIA_PLAYERS->turnPlayer].battlePhaseDone = 0;
        gDuelPlayers[GDUEL_VIA_PLAYERS->turnPlayer].magicTrapActivatedThisTurn = 0;
        gDuelPlayers[GDUEL_VIA_PLAYERS->turnPlayer].normalSummonUsed = 0;
        gDuelPlayers[GDUEL_VIA_PLAYERS->turnPlayer].summonedThisTurn = 0;
        gDuelPlayers[GDUEL_VIA_PLAYERS->turnPlayer].monsterSentToGraveThisTurn = 0;
        gDuelPlayers[GDUEL_VIA_PLAYERS->turnPlayer].banishCostFromField = 0;
        return 1;
    }
}

/*
 * Text-box draw callback of the 3-choice phase menu: one cursor sprite in front of the chosen line (12 pixels
 * per line), which blinks while the choice is confirmed. The box slides in from the top, so the sprite is moved
 * up by the rows of the box that are not revealed yet (revealRow).
 */
void PhaseMenu_DrawCursor(void)
{
    struct TextBox *box = &gTextBox;
    u32 x = (box->x + 1) * 8;
    int y = (box->y + 2) * 8 + box->result * 12 - 2;
    y -= (box->height + box->y - box->revealRow + 2) * 8;
    if (box->menuState == PHASE_MENU_STATE_CONFIRMED) {
        if (box->menuTimer & 2)
            AddSprite((y << 16) | x, SPRITE_SHAPE_8x8, PHASE_MENU_CURSOR_ATTR2);
    } else {
        AddSprite((y << 16) | x, SPRITE_SHAPE_8x8, PHASE_MENU_CURSOR_ATTR2);
    }
}

/*
 * Text-box input callback of the 3-choice phase menu (the answer is gTextBox.result, enum PhaseMenuChoice).
 * Up and Down move the cursor with wrap-around, A confirms the choice, B chooses PHASE_MENU_CONTINUE. A
 * confirmed choice blinks for 60 frames (menuTimer), then the menu is done. Returns 1 when done.
 */
int PhaseMenu_HandleInput(void)
{
    /* FAKEMATCH: the state goes through a u32 copy and is stored back as copy + 1; a plain u8 state or
     * menuState++ lets the compiler fold the stored value to 2. */
    u32 state = gTextBox.menuState;
    switch ((u8)state) {
    case PHASE_MENU_STATE_CONFIRMED:
        if (gTextBox.menuTimer < PHASE_MENU_BLINK_FRAMES)
            gTextBox.menuTimer++;
        else
            gTextBox.menuState = state + 1;
        break;
    case PHASE_MENU_STATE_DONE:
        return 1;
    default:
        if (gMain.newKeys & DPAD_DOWN) {
            PlaySE(SE_CURSOR);
            if (gTextBox.result < PHASE_MENU_CONTINUE)
                gTextBox.result++;
            else
                gTextBox.result = PHASE_MENU_NEXT_PHASE;
        }
        if (gMain.newKeys & DPAD_UP) {
            PlaySE(SE_CURSOR);
            if (gTextBox.result != PHASE_MENU_NEXT_PHASE)
                gTextBox.result--;
            else
                gTextBox.result = PHASE_MENU_CONTINUE;
        }
        if (gMain.newKeys & A_BUTTON) {
            PlaySE(SE_CONFIRM);
            gTextBox.menuState = PHASE_MENU_STATE_CONFIRMED;
            gTextBox.menuTimer = 0;
        }
        if (gMain.newKeys & B_BUTTON) {
            PlaySE(SE_CANCEL);
            gTextBox.result = PHASE_MENU_CONTINUE;
            gTextBox.menuState = PHASE_MENU_STATE_CONFIRMED;
            gTextBox.menuTimer = 0;
        }
        break;
    }
    return 0;
}

/*
 * Duel step 5: the human player's Main Phase and Battle Phase (enum MainPhaseStep). Step 0 pushes
 * DUEL_CMD_MAIN1_PHASE (the banner) and closes the card menu. In the default step the player acts on the field
 * through the card menu; B while no menu is open asks to end the phase (MAIN_STEP_ASK_END). That opens the
 * 3-choice phase menu when the Battle Phase can be entered, otherwise a plain Yes/No 'End your turn?'. The
 * menu choice enters the Battle Phase (MAIN_STEP_BATTLE_PHASE, which goes back to the field afterwards unless
 * the turn was completed in the Battle Phase menu), ends the turn (return 1), or goes back to the field.
 * Returns 1 when the turn is over.
 */
int DuelPhase_Main(void)
{
    switch (gDuel.phaseStep) {
    case MAIN_STEP_ENTER:
        DuelCmd_Push(DUEL_CMD_MAIN1_PHASE, 0, 0, 0);
        gDuel.cardMenu.open = 0;
        gDuel.cardMenu.confirmed = 0;
        gDuel.phaseStep++;
        return 0;
    case MAIN_STEP_ASK_END:
        if (CanEnterBattlePhase(0)) {
            TextBoxOpen(TEXTBOX_XY(5, 2), TEXTBOX_XY(21, 6), TEXTBOX_FLAGS_DEFAULT, gStrEndMainPhaseMenu);
            TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, PhaseMenu_DrawCursor, (u16 (*)(void))PhaseMenu_HandleInput);
            gDuel.phaseStep = MAIN_STEP_MENU_ANSWER;
        } else {
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 4), TEXTBOX_FLAGS_DEFAULT, gStrEndYourTurn);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            /* FAKEMATCH: written as an increment (to MAIN_STEP_END_TURN_ANSWER) to match the ROM's add. */
            gDuel.phaseStep = gDuel.phaseStep + 1;
        }
        return 0;
    case MAIN_STEP_END_TURN_ANSWER:
        if (gTextBox.result != 0)
            return 1;
        gDuel.phaseStep = MAIN_STEP_FIELD;
        return 0;
    case MAIN_STEP_MENU_ANSWER:
        if (gTextBox.result != PHASE_MENU_NEXT_PHASE) {
            if (gTextBox.result == PHASE_MENU_END_TURN)
                return 1;
            gDuel.phaseStep = MAIN_STEP_FIELD;
        } else {
            /* Enter the Battle Phase: BattlePhase_Run starts at BATTLE_STAGE_START. */
            gDuel.endTurnAfterBattle = 0;
            gDuel.battleStage = BATTLE_STAGE_START;
            /* FAKEMATCH: written as an increment (to MAIN_STEP_BATTLE_PHASE) to match the ROM's add. */
            gDuel.phaseStep = gDuel.phaseStep + 1;
            /* FAKEMATCH: keep this completed store separate from the
             * step=1 tail shared by other cases. Emits no instructions. */
            __asm__ volatile("" ::: "memory");
        }
        return 0;
    case MAIN_STEP_BATTLE_PHASE:
        if (BattlePhase_Run(0)) {
            if (gDuel.endTurnAfterBattle)
                return 1;
            gDuel.phaseStep = MAIN_STEP_FIELD;
        }
        return 0;
    default:
        if (DuelScreen_HandleInput() == 0 && (gMain.newKeys & B_BUTTON))
            gDuel.phaseStep = MAIN_STEP_ASK_END;
        return 0;
    }
    return 0;
}

/* LP a card costs to keep on the field during each Standby Phase, by card number (0 = no LP cost). */
int GetMaintenanceLpCost(u16 cardNumber)
{
    switch (cardNumber) {
    case CARD_MIRROR_WALL:
        return 2000;
    case CARD_IMPERIAL_ORDER:
        return 700;
    case CARD_1420:             /* not an EDS card */
        return 1000;
    case CARD_TOON_WORLD:
    case CARD_1424:             /* not an EDS card */
        return 500;
    default:
        return 0;
    }
}

/* Dead code (no callers): count the monster zones other than (skipPlayer, skipZone) for which
 * EffectBlastJugglerCheck succeeds. */
int CountBlastJugglerTargets(int skipPlayer, int skipZone)
{
    int count = 0;
    int player, zone;
    for (player = 0; player < 2; player++) {
        for (zone = ZONE_MONSTER_0; zone < MONSTER_ZONE_COUNT; zone++) {
            if (player == skipPlayer && zone == skipZone)
                continue;
            if (EffectBlastJugglerCheckInts(player, zone) != 0)
                count++;
        }
    }
    return count;
}

/*
 * 1 if the player has a card with an optional Standby Phase effect that can be activated now
 * (CanActivateEffectInZone for RESPONSE_OWN_STANDBY): a face-up Patrol Robo, Blast Juggler or Jigen Bakudan in a
 * monster zone, or a face-down Curse of Fiend in a spell/trap zone. The scan covers zones 0-9.
 */
int HasActivatableStandbyCard(int player)
{
    int zone = 0;
    /* FAKEMATCH: retain this initialized player offset in the ROM's
     * callee-saved register; player itself stays live for the predicate. */
    register u32 playerOffset __asm__("r6") = (player & 1) * 0xD64;
    for (; zone < 10; zone++) {
        struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + playerOffset + (u32)gDuelZones);
        u32 cardId = ZONE_ID(z);
        if (cardId != 0) {
            /* Matching: this scan reads gCardIdToNumber through its symbol; CARD_NUMBER_OF would use the constant
             * address and change the code. */
            switch (((const u16 *)gCardIdToNumber)[(u16)cardId & CARD_ID_MASK]) {
            case CARD_PATROL_ROBO:
            case CARD_BLAST_JUGGLER:
            case CARD_JIGEN_BAKUDAN:
                if (zone > ZONE_MONSTER_4)
                    continue;
                /* Matching: the zone is addressed again here; reusing z changes the register allocation. */
                if (!(((struct DuelZone *)(zone * 0x94 + playerOffset + (u32)gDuelZones))->isFaceUp))
                    continue;
                break;
            case CARD_CURSE_OF_FIEND:
                if (z->isFaceUp || zone <= ZONE_MONSTER_4)
                    continue;
                break;
            default:
                continue;
            }
            if (CanActivateEffectInZoneInt(player, zone, RESPONSE_OWN_STANDBY))
                return 1;
        }
    }
    return 0;
}

/*
 * The automatic Standby Phase effects of the turn player, in four zone scans:
 *  1. Monster zones held in removedMask that are empty: the monster banished until the End Phase comes back.
 *  2. Own face-up monsters 0-4: Mushroom Man #2 costs 300 LP; Dark Zebra, when it is the only monster, goes to
 *     Defense Position and is locked there; keys 1437 (attack position), 1438 (defense position) and 1441 give
 *     LP; key 1419 costs 500 LP per link; Blast Sphere destroys the monster it is linked to and deals its ATK
 *     as damage; Kiseitai gives the opponent half the ATK of its monster per link. Then
 *     DamageOpponentPerBanishedMonster.
 *  3. Own active spell/trap zones 5-10 (the scan includes the Field zone): key 1417 costs 500 LP per link,
 *     Messenger of Peace costs 100 LP, key 1311 costs 500 LP and key 1319 only shows its card.
 *  4. The opponent's active spell/trap zones 5-9: Snatch Steal and The Eye of Truth (with a Magic in the
 *     player's hand) give the player 1000 LP; key 1302 and key 1311 cost the player 500 LP.
 * Finally the player gains 200 LP for each key-1446 card in the graveyard.
 */
void ApplyStandbyPhaseEffects(int player)
{
    int zone;
    u32 returnedKey = CARD_1319;
    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        if (((gDuelPlayers[player & 1].removedMask >> zone) & 1)
            && !ZONE_HAS_CARD(&gDuelPlayers[player & 1].zones[zone])) {
            ShowCardEffect(player, CARD_ID_OF(returnedKey));
            DuelCmd_Push(player ? DUEL_CMD_RETURN_BANISHED_MONSTER | DUEL_CMD_PLAYER : DUEL_CMD_RETURN_BANISHED_MONSTER,
                         (u16)zone, 1, 0);
        }
    }
    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        struct DuelZone *z = ZONE_VIA_PLAYERS(player & 1, zone);
        u16 cardId = ZONE_ID(z);
        u32 isFaceUp = z->isFaceUp;
        u32 isDefense = z->isDefense;
        if (cardId && isFaceUp) {
            switch (CARD_NUMBER_OF(cardId)) {
            case CARD_MUSHROOM_MAN_2:
                ShowCardEffect(player, cardId);
                LoseLifePoints(player, 300);
                break;
            case CARD_DARK_ZEBRA: {
                int monsterCount = CountMonsters(player);
                if (monsterCount == 1) {
                    ShowCardEffect(player, cardId);
                    if (!(((struct DuelZoneFlags6 *)z)->flags6 & monsterCount)) /* monsterCount is 1: tests isDefense */
                        ChangeBattlePosition(player, zone, 0, 0);
                    z->positionLocked = 1;
                }
                break;
            }
            case 1437:          /* effect-table row without handlers (not an EDS card): +1000 LP in attack position */
                if (isDefense)
                    break;
                goto gain1000;
            case 1438:          /* likewise: +1000 LP in defense position */
                if (!isDefense)
                    break;
            gain1000:
                ShowCardEffect(player, cardId);
                GainLifePoints(player, 1000);
                break;
            case 1441:          /* likewise: +800 LP */
                ShowCardEffect(player, cardId);
                GainLifePoints(player, 800);
                break;
            }
            {
                int key = CARD_1419;
                if (CountActiveZoneLinksFromCard(player, zone, key)) {
                    ShowCardEffect(player, CARD_ID_OF(key));
                    LoseLifePoints(player, CountActiveZoneLinksFromCard(player, zone, key) * 500);
                }
            }
            {
                int key = CARD_BLAST_SPHERE;
                if (CountZoneLinksFromCard(player, zone, key)) {
                    int atk = GetZoneCardAtk(player, zone);
                    ShowCardEffect(player, CARD_ID_OF(key));
                    DestroyFieldCard(player, zone, 1);
                    LoseLifePoints(player, atk);
                }
            }
            {
                int key = CARD_KISEITAI;
                if (CountZoneLinksFromCard(player, zone, key)) {
                    int atk = GetZoneCardAtk(player, zone);
                    ShowCardEffect(player, CARD_ID_OF(key));
                    atk = HalveRoundUp(atk);
                    GainLifePoints(1 - player, atk * CountZoneLinksFromCard(player, zone, key));
                }
            }
        }
    }
    DamageOpponentPerBanishedMonster(player);
    for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
        int p = player & 1;
        struct DuelZone *z;
        u16 cardId;
        u32 isFaceUp;
        u32 isDisabled;
        /* FAKEMATCH: the do/while(0) wrapper puts loop notes after the zone reads, so the
         * first CSE pass starts a new block there and 0xD64/base stay in the loop; the
         * post-loop CSE then reuses them for the Messenger of Peace case as in the ROM. */
        do {
            z = ZONE_VIA_PLAYERS(p, zone);
            cardId = ZONE_ID(z);
            isFaceUp = z->isFaceUp;
            isDisabled = z->isDisabled;
        } while (0);
        if (cardId && isFaceUp && !isDisabled) {
            int linkCount;
            int key = CARD_1417;
            linkCount = CountActiveZoneLinksFromCard(player, zone, key);
            if (linkCount > 0) {
                ShowCardEffect(player, CARD_ID_OF(key));
                LoseLifePoints(player, linkCount * 500);
            }
            switch (CARD_NUMBER_OF(cardId)) {
            case CARD_MESSENGER_OF_PEACE:
                /* Original bug, kept: this compares player itself to 99, so it always reads player 0's LP (the
                 * ROM tests gDuelPlayers[player > 99].lifePoints), whoever owns the card. */
                if (gDuelPlayers[player > 99].lifePoints) {
                    ShowCardEffect(player, cardId);
                    DuelCmd_Push(player ? DUEL_CMD_LOSE_LP | DUEL_CMD_PLAYER : DUEL_CMD_LOSE_LP, 100, 1, 0);
                }
                break;
            case CARD_1311:
                ShowCardEffect(player, cardId);
                LoseLifePoints(player, 500);
                break;
            case CARD_1319:
                ShowCardEffect(player, cardId);
                break;
            }
        }
    }
    for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
        int opponent = 1 - player;
        struct DuelZone *z = ZONE_VIA_PLAYERS(opponent & 1, zone);
        u16 cardId = ZONE_ID(z);
        u32 isFaceUp = z->isFaceUp;
        u32 isDisabled = z->isDisabled;
        if (cardId && isFaceUp && !isDisabled) {
            switch (CARD_NUMBER_OF(cardId)) {
            case CARD_THE_EYE_OF_TRUTH:
                if (FindMagicInHand(player) == -1)
                    break;
            case CARD_SNATCH_STEAL:
                ShowCardEffect(opponent, cardId);
                GainLifePoints(player, 1000);
                break;
            case CARD_1302:
                ShowCardEffect(opponent, cardId);
                LoseLifePoints(player, 500);
                break;
            case CARD_1311:
                ShowCardEffect(player, cardId);
                LoseLifePoints(player, 500);
                break;
            }
        }
    }
    {
        int key = 1446;     /* effect-table row without handlers (not an EDS card) */
        if (CountGraveyardCardsByNumber(player, key) > 0) {
            ShowCardEffect(player, CARD_ID_OF(key));
            GainLifePoints(player, 200 * CountGraveyardCardsByNumber(player, key));
        }
    }
}

/*
 * Duel step 4: the Standby Phase (enum StandbyStep; the CPU runs it through gAiTurnPhases). A turn player
 * whose skipStandbyPhase flag is set loses it and finishes at once. Otherwise: the banner (and the dice effect
 * of key 1536, step 20), the automatic effects, Sinister Serpent, the delayed Special Summon of key 1405,
 * Lightforce Sword's face-down cards coming back after 4 turns, the opponent's Inspection (pay 500 LP, look at
 * the player's hand) and key-1517 monster, a token of key 1426, then the player's optional activations ("Complete
 * Standby Phase?"). Steps 100 and up charge the upkeep of cards that need LP (GetMaintenanceLpCost: asked of
 * the human, decided by LP for the CPU) or a Tribute (The Regulation of Tribe, key 1431) to stay on the field,
 * then count up the turn counters of Germ Infection and Stim-Pack (step 102).
 * The dice effect: roll a die and destroy every monster of that level (a 6 also destroys higher levels).
 * Returns 1 when the step is finished.
 *
 * Matching: the command word of every DuelCmd_Push in this function is built as `cmd = X; if (player)
 * cmd = X | DUEL_CMD_PLAYER;` (a ?: expression compiles differently).
 */
int DuelPhase_Standby(void)
{
    struct DuelCard card; /* Written by RemoveGraveyardCardByNumber on success before it is consumed. */
    char text[128];
    char format[128];
    u32 player;
    struct DuelState *duel;
    u8 *playersBase;
    struct DuelPlayerSkipStandbyView *turnPlayerFlags;
    u16 cardId;
    player = gDuel.turnPlayer;
    duel = &gDuel;
    playersBase = (u8 *)duel->players;
    turnPlayerFlags = (struct DuelPlayerSkipStandbyView *)(playersBase + player * 0xD64);
    if (turnPlayerFlags->skipStandbyPhase < 0) {
        turnPlayerFlags->skipStandbyPhase = 0;
        goto done;
    }
    switch (gDuel.phaseStep) {
    case STANDBY_STEP_ENTER: {
        u16 cmd = DUEL_CMD_STANDBY_PHASE;
        if (player)
            cmd = DUEL_CMD_STANDBY_PHASE | DUEL_CMD_PLAYER;
        DuelCmd_Push(cmd, 0, 0, 0);
        gDuel.phaseStep = STANDBY_STEP_DICE;
        return 0;
    }
    case STANDBY_STEP_EFFECTS:
        ApplyStandbyPhaseEffects(player);
        gDuel.phaseSubStep = 0;
        gDuel.phaseSubCounter = 0;
        gDuel.phaseCounter = 0;
        gDuel.phaseStep++;
        return 0;
    case STANDBY_STEP_SINISTER_SERPENT:
        if (SinisterSerpentStandbyStep(player)) {
            gDuel.phaseSubStep = 0;
            gDuel.phaseSubCounter = 0;
            gDuel.phaseCounter = 0;
            gDuel.phaseStep++;
        }
        return 0;
    case STANDBY_STEP_DELAYED_SUMMON:
        /* One Special Summon of key 1405's monster from the graveyard per pending delayed summon. */
        if (gDuelPlayers[player].delayedSummonCount) {
            u16 cmd = DUEL_CMD_ADJUST_DELAYED_SUMMON_COUNT;
            if (player)
                cmd = DUEL_CMD_ADJUST_DELAYED_SUMMON_COUNT | DUEL_CMD_PLAYER;
            DuelCmd_Push(cmd, 0, 0, 0);
            if (CountFreeMonsterZones(player) > 0 && RemoveGraveyardCardByNumber(player, CARD_1405, (u32 *)&card)) {
                ShowCardEffect(player, CardNumberToId(CARD_1405));
                QueueSpecialSummon(player, &card, 1, 1, ZONE_STATUS_FROM_GRAVEYARD);
                return 0;
            }
        }
        gDuel.phaseStep++;
        gDuel.phaseSubStep = 0;
        return 0;
    case STANDBY_STEP_RETURN_BANISHED:
        /* Lightforce Sword: a card banished face down comes back to the hand when its turn count (the high
         * byte of banishedInfo) reaches 4. */
        for (; gDuel.phaseSubStep < gDuel.players[player].banishedCount; gDuel.phaseSubStep++) {
            u32 info = gDuel.players[player & 1].banishedInfo[gDuel.phaseSubStep];
            if (info == ((4 << 8) | BANISH_FACE_DOWN)) {
                u16 cmd;
                ShowCardEffect(player, gCardNumberToId_LightforceSword[0]);
                cmd = DUEL_CMD_RETURN_BANISHED_CARD_TO_HAND;
                if (player)
                    cmd = DUEL_CMD_RETURN_BANISHED_CARD_TO_HAND | DUEL_CMD_PLAYER;
                DuelCmd_Push(cmd, gDuel.phaseSubStep, 1, 0);
                return 0;
            }
        }
        gDuel.phaseSubStep = 0;
        gDuel.phaseSubCounter = 0;
        gDuel.phaseCounter = 0;
        gDuel.phaseStep++;
        return 0;
    case STANDBY_STEP_INSPECTION_ASK: {
        /* The opponent's Inspection may be paid for (500 LP) to look at the player's hand. */
        int other = 1 - player;
        struct DuelPlayer *pl;
        if (CountActiveCardsOnField(other, CARD_INSPECTION)
            && (pl = gDuelPlayers, pl[other & 1].lifePoints > 499) && pl[player & 1].handCount) {
            DuelPrompt_Post(other, PROMPT_CONFIRM_CARD_EFFECT, CARD_INSPECTION, 0);
            PHASE_STEP_VIA_PLAYERS++;
        } else
            gDuel.phaseStep = STANDBY_STEP_KEY1517_ASK;
        return 0;
    }
    case STANDBY_STEP_INSPECTION_ANSWER:
        if (gDuel.promptResult) {
            int other, index;
            /* Chain trigger word: kind << 21 | event << 25 (see struct ChainEntry). */
            u32 kind = (CHAIN_KIND_SPELL_TRAP << 21) | (RESPONSE_OPPONENT_STANDBY << 25);
            u16 cmd = DUEL_CMD_LOSE_LP;
            if (player != 1)
                cmd = DUEL_CMD_LOSE_LP | DUEL_CMD_PLAYER;
            DuelCmd_Push(cmd, 500, 0, 0);
            other = 1 - player;
            index = FindFaceUpCardOnField2Two(other, CARD_INSPECTION);
            Chain_AddPending(((u32)(other & 1) << 31) | (((index & 31) << 16) | kind) | CardNumberToId(CARD_INSPECTION), 0);
            gDuel.phaseStep = STANDBY_STEP_INSPECTION_ASK;
        } else
            gDuel.phaseStep = STANDBY_STEP_KEY1517_ASK;
        return 0;
    case STANDBY_STEP_KEY1517_ASK: {
        int other = 1 - player;
        if (CountFaceUpMonstersByNumber(other, CARD_1517) && CountMonstersFiltered(player, 1, 0) > 0) {
            DuelPrompt_Post(other, PROMPT_CONFIRM_CARD_EFFECT, CARD_1517, 0);
            gDuel.phaseStep++;
        } else {
            gDuel.phaseCounter = ZONE_SPELL_0;
            gDuel.phaseStep = STANDBY_STEP_TOKEN;
        }
        return 0;
    }
    case STANDBY_STEP_KEY1517_ANSWER:
        if (gDuel.promptResult) {
            int other = 1 - player;
            u32 kind;
            int index = FindFaceUpCardOnField2Two(other, CARD_1517);
            Chain_AddPending(((u32)(other & 1) << 31)
                                 | (((index & 31) << 16) | (kind = (CHAIN_KIND_MONSTER << 21) | (RESPONSE_OPPONENT_STANDBY << 25)))
                                 | CardNumberToId(CARD_1517),
                             0);
        }
        gDuel.phaseCounter = ZONE_SPELL_0;
        gDuel.phaseStep = STANDBY_STEP_TOKEN;
        return 0;
    case STANDBY_STEP_TOKEN:
        /* A face-up, enabled key-1426 card in a spell/trap zone summons a token into a free monster zone. */
        for (; gDuel.phaseCounter <= ZONE_SPELL_4; gDuel.phaseCounter++) {
            u32 zone = gDuel.phaseCounter;
            u8 *zonesBase = (u8 *)&gDuel + 0x2C;    /* = &gDuel.players[0].zones[0] */
            struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + player * 0xD64 + zonesBase);
            u32 id = ZONE_ID(z);
            if (id && z->isFaceUp && !z->isDisabled && CARD_NUMBER_OF(id) == CARD_1426 && CountFreeMonsterZones(player) > 0) {
                u16 cmd;
                ShowCardEffect(player, ZONE_ID(DUEL_ZONE_ZP(player, gDuel.phaseCounter)));
                cmd = DUEL_CMD_SUMMON_TOKEN;
                if (player)
                    cmd = DUEL_CMD_SUMMON_TOKEN | DUEL_CMD_PLAYER;
                DuelCmd_Push(cmd, (u8)FindFreeMonsterZone(player) | (gDuel.phaseCounter << 8), TOKEN_KIND_3, 0);
                gDuel.phaseCounter++;
                return 0;
            }
        }
        DuelCursor_Refresh();
        gDuel.phaseStep++;
        return 0;
    case STANDBY_STEP_FIELD:
        /* The player may activate optional Standby effects with the field cursor; B asks to complete the phase. */
        if ((u16)HasActivatableStandbyCard(player) == 0) {
            DuelCursor_Refresh();
            gDuel.phaseStep = STANDBY_STEP_MAINTENANCE_START;
        } else if (DuelScreen_HandleInput() == 0 && (gMain.newKeys & B_BUTTON)) {
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(19, 7), TEXTBOX_FLAGS_DEFAULT, gStrCompleteStandbyPhase);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            gDuel.phaseStep++;
        }
        return 0;
    case STANDBY_STEP_CONFIRM_ANSWER:
        switch (gTextBox.result) {
        case 0:
            gDuel.phaseStep = STANDBY_STEP_FIELD;
            break;
        case 1:
            gDuel.phaseStep = STANDBY_STEP_MAINTENANCE_START;
            break;
        }
        return 0;
    case STANDBY_STEP_DICE: {
        const u16 *diceCardId = gCardNumberToId_1536;
        int diceCount = CountActiveCardsOnField(player, CARD_1536);
        if (diceCount > 0)
            do {
                int die = Random() % 6 + 1;
                int scanPlayer, zone;
                u16 cmd;
                ShowCardEffect(player, diceCardId[0]);
                cmd = DUEL_CMD_ROLL_PLAIN_DIE;
                if (player)
                    cmd = DUEL_CMD_ROLL_PLAIN_DIE | DUEL_CMD_PLAYER;
                DuelCmd_Push(cmd, die, 0, 0);
                cmd = DUEL_CMD_OPEN_DUEL_SCREEN;
                if (player)
                    cmd = DUEL_CMD_OPEN_DUEL_SCREEN | DUEL_CMD_PLAYER;
                DuelCmd_Push(cmd, 0, 0, 0);
                for (scanPlayer = 0; scanPlayer <= 1; scanPlayer++) {
                    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                        struct DuelZone *z = ZONE_ZP(scanPlayer & 1, zone);
                        u32 id = ZONE_ID(z);
                        if (id && z->isFaceUp) {
                            int level = GetCardLevel(id);
                            register int destroy asm("r0") = 0; /* FAKEMATCH: the ROM gives destroy r0 and level r1; global alloc otherwise hands level r0 first */
                            if (level == die)
                                destroy = 1;
                            if (level > 5 && die == 6)
                                destroy = 1;
                            if (destroy)
                                DestroyFieldCard(scanPlayer, zone, 1);
                        }
                    }
                }
            } while (--diceCount);
        gDuel.phaseStep = STANDBY_STEP_EFFECTS;
        return 0;
    }
    case STANDBY_STEP_DICE_AGAIN:
        gDuel.phaseStep = STANDBY_STEP_DICE;
        break;
    case STANDBY_STEP_MAINTENANCE_START:
        gDuel.phaseCounter = 0;
        gDuel.phaseStep++;
        /* fall through */
    case STANDBY_STEP_MAINTENANCE_SCAN:
        /* Upkeep: for each face-up card of the player in zones 0-9 that costs LP or a Tribute, either ask
         * (steps 110 and 120) or, when it cannot be paid, destroy the card. phaseCounter is the zone. */
        for (; gDuel.phaseCounter <= ZONE_SPELL_4; gDuel.phaseCounter++) {
            struct DuelZone *z = DUEL_ZONE_ZP(player, gDuel.phaseCounter);
            if (ZONE_ID(z) && z->isFaceUp) {
                int destroy;
                cardId = ZONE_ID(z);
                destroy = LOOP_PAD(cardId); /* FAKEMATCH: dead, see LOOP_PAD */
                destroy = 0;
                switch (CARD_NUMBER_OF(cardId)) {
                case CARD_THE_REGULATION_OF_TRIBE:
                    if (CountTributableMonsters(player, -1) > 0) {
                        gDuel.phaseStep = STANDBY_STEP_ASK_TRIBUTE;
                        return 0;
                    }
                    destroy = 1;
                    break;
                case 1431:      /* effect-table row without handlers: like The Regulation of Tribe, but the card itself is
                                 * excluded from the Tributes */
                    if (CountTributableMonsters(player, gDuel.phaseCounter) > 0) {
                        gDuel.phaseStep = STANDBY_STEP_ASK_TRIBUTE;
                        return 0;
                    }
                    destroy = 1;
                    break;
                case CARD_TOON_WORLD:
                case CARD_MIRROR_WALL:
                case CARD_IMPERIAL_ORDER:
                case CARD_1420:
                case CARD_1424:
                    if (PLAYER_VIA_PTR(player).lifePoints < GetMaintenanceLpCost(CARD_NUMBER_OF(cardId)))
                        destroy = 1;
                    else {
                        PHASE_STEP_VIA_PLAYERS = STANDBY_STEP_ASK_PAY_LP;
                        return 0;
                    }
                    break;
                }
                if (destroy) {
                    u16 cmd = DUEL_CMD_SHOW_CARD_SCATTER;
                    if (player)
                        cmd = DUEL_CMD_SHOW_CARD_SCATTER | DUEL_CMD_PLAYER;
                    DuelCmd_Push(cmd, cardId, 1, 0);
                    DestroyFieldCard(player, gDuel.phaseCounter, 1);
                }
            }
        }
        gDuel.phaseStep++;
        return 0;
    case STANDBY_STEP_TURN_COUNTERS: {
        /* Germ Infection and Stim-Pack count the turns they have been face up. */
        int zone;
        for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
            struct DuelZone *z = ZONE_ZP(player, zone);
            u32 id = ZONE_ID(z);
            if (id && z->isFaceUp) {
                int number = CARD_NUMBER_OF(id);
                if (number == CARD_GERM_INFECTION || number == CARD_STIM_PACK) {
                    u16 cmd = DUEL_CMD_INCREMENT_ZONE_TURN_COUNTER;
                    if (player)
                        cmd = DUEL_CMD_INCREMENT_ZONE_TURN_COUNTER | DUEL_CMD_PLAYER;
                    DuelCmd_Push(cmd, zone, 1, 0);
                }
            }
        }
        gDuel.phaseStep++;
        return 0;
    }
    case STANDBY_STEP_ASK_PAY_LP: {
        /* Pay the upkeep of the card in zone phaseCounter? The CPU decides by its LP (Toon World: only with a
         * Toon monster and more than 1000 LP; Mirror Wall: more than 7000; else more than cost + 1000) and
         * the answer goes into gTextBox.result; the human gets a Yes/No box. */
        cardId = ZONE_ID(DUEL_ZONE_PZ(player & 1, gDuel.phaseCounter));
        if (player) {
            switch (CARD_NUMBER_OF(cardId)) {
            case CARD_TOON_WORLD: {
                int found = 0;
                if (AiFindHandCardByNumber(1, CARD_MANGA_RYU_RAN))
                    found = 1;
                if (AiFindHandCardByNumber(1, CARD_TOON_MERMAID))
                    found = 1;
                if (AiFindHandCardByNumber(1, CARD_TOON_SUMMONED_SKULL))
                    found = 1;
                if (AiFindHandCardByNumber(1, CARD_BLUE_EYES_TOON_DRAGON))
                    found = 1;
                if (CountFaceUpMonstersByNumber(1, CARD_MANGA_RYU_RAN))
                    found = 1;
                if (CountFaceUpMonstersByNumber(1, CARD_TOON_MERMAID))
                    found = 1;
                if (CountFaceUpMonstersByNumber(1, CARD_TOON_SUMMONED_SKULL))
                    found = 1;
                if (CountFaceUpMonstersByNumber(1, CARD_BLUE_EYES_TOON_DRAGON))
                    found = 1;
                gTextBox.result = 0;
                if (found && gDuel.players[1].lifePoints > 1000)
                    gTextBox.result = 1;
                break;
            }
            case CARD_MIRROR_WALL:
                if (gDuel.players[1].lifePoints > 7000)
                    gTextBox.result = 1;
                else
                    gTextBox.result = 0;
                break;
            default:
                if (gDuel.players[1].lifePoints > GetMaintenanceLpCost(CARD_NUMBER_OF(cardId)) + 1000)
                    gTextBox.result = 1;
                else
                    gTextBox.result = 0;
                break;
            }
        } else {
            FormatStr(format, gStrMaintainLpCostFmt, CARD_NAME_OF(cardId));
            FormatInt(text, format, GetMaintenanceLpCost(CARD_NUMBER_OF(cardId)));
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(19, 6), TEXTBOX_FLAGS_DEFAULT, text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
        }
        ShowActivatedCard(player, cardId);       /* DUEL_CMD_SHOW_CARD_ZOOM_IN: show the card while the question is asked */
        gDuel.phaseStep++;
        return 0;
    }
    case STANDBY_STEP_PAY_LP_ANSWER:
        if (gTextBox.result) {
            u16 cmd;
            cardId = ZONE_ID(DUEL_ZONE_BASE_FIRST(player, gDuel.phaseCounter));
            cmd = DUEL_CMD_LOSE_LP;
            if (player)
                cmd = DUEL_CMD_LOSE_LP | DUEL_CMD_PLAYER;
            DuelCmd_Push(cmd, GetMaintenanceLpCost(CARD_NUMBER_OF(cardId)), 1, 0);
            if (CARD_NUMBER_OF(cardId) == CARD_TOON_WORLD) {
                u16 cmd2 = DUEL_CMD_UPDATE_ZONE_LP_PAID;
                if (player)
                    cmd2 = DUEL_CMD_UPDATE_ZONE_LP_PAID | DUEL_CMD_PLAYER;
                DuelCmd_Push(cmd2, gDuel.phaseCounter, 500, 0);
            }
        } else
            DestroyFieldCard(player, gDuel.phaseCounter, 1);
        gDuel.phaseCounter++;
        gDuel.phaseStep = STANDBY_STEP_MAINTENANCE_SCAN;
        return 0;
    case STANDBY_STEP_ASK_TRIBUTE: {
        u32 id = ZONE_ID(DUEL_ZONE_BASE_FIRST(player, gDuel.phaseCounter));
        FormatStr(text, gStrMaintainTributeFmt, (const char *)gCardNames + (id << 6));
        TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(19, 8), TEXTBOX_FLAGS_DEFAULT, text);
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
        gDuel.phaseStep++;
        return 0;
    }
    case STANDBY_STEP_TRIBUTE_ANSWER:
        if (gTextBox.result) {
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrSelectMonsterAsTribute);
            gDuel.phaseStep++;
            return 0;
        } else {
            DestroyFieldCard(player, gDuel.phaseCounter, 1);
            gDuel.phaseCounter++;
            gDuel.phaseStep = STANDBY_STEP_MAINTENANCE_SCAN;
        }
        return 0;
    case STANDBY_STEP_PICK_TRIBUTE:
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            TributeMonster(gDuelScreen.selPlayer, gDuelScreen.selIndex);
            gDuel.phaseCounter++;
            gDuel.phaseStep = STANDBY_STEP_MAINTENANCE_SCAN;
        }
        return 0;
    default:
        goto done;
    }
    return 0;
done:
    return 1;
}
