#ifndef GUARD_DUEL_PROMPT_H
#define GUARD_DUEL_PROMPT_H

/*
 * Duel prompts: questions and picks that one player must answer before the duel goes on (discard, tribute,
 * choose a type or an attribute, pick a card, ...). An effect posts a prompt with a DuelPrompt_Post* function
 * (gDuel.promptKind = enum DuelPromptKind in constants/duel.h, arguments in gDuel.promptArgs); every frame
 * DuelMainStep calls DuelPrompt_Run, which runs the kind's handler until it returns 1 and leaves the answer
 * in gDuel.promptResult. gDuel.promptStep is the handler's step. Handlers decide at once for the CPU
 * (player 1); in a link duel a prompt for player 1 is answered on the partner's GBA (LINKMSG_PROMPT and
 * LINKMSG_PROMPT_RESULT). The menus draw through text-box callbacks (TextBoxSetMenu mode 5: a draw callback
 * and an input callback that returns 1 when done).
 *
 * Every prototype is the function's definition as compiled. Units that call a function through another
 * local declaration keep that view as a commented local alias prototype (build/readability/proto_mismatches.txt).
 */

#include "global.h"

/* gDuel.promptStep in DuelPrompt_Discard and DuelPrompt_DiscardCost (which stops after DISCARD_STEP_PICK). */
enum DiscardPromptStep {
    DISCARD_STEP_INIT = 0,                  /* scroll to the hand, gChain.handPickCount = count */
    DISCARD_STEP_PICK = 1,                  /* CPU picks, or 'Discard from your hand.' for the human */
    DISCARD_STEP_FORCED_REQUISITION = 2,    /* the opponent discards too (Forced Requisition) */
    DISCARD_STEP_TRIGGER = 3                /* post RESPONSE_DISCARDED to the non-turn player */
};

/* ---- Posting and running prompts ---- */

/* Fill promptPlayer, promptKind and promptArgs[0..1], then DuelPrompt_Start. */
void DuelPrompt_Post(int player, int kind, u16 arg, u16 value);
/* Like DuelPrompt_Post with up to 8 argument halfwords copied into promptArgs. */
void DuelPrompt_PostData(int player, int kind, const u16 *args, int count);
/* Mark the filled prompt active; forward a prompt for player 1 to the link partner (except
 * PROMPT_DISCARD_RANDOM). */
void DuelPrompt_Start(void);
/* Run the pending prompt's handler (or wait for the partner's answer); 1 while it is running. */
u32 DuelPrompt_Run(void);

/* PROMPT_DISCARD: discard `count` hand cards, then the discard trigger. */
void DuelPrompt_PostDiscard(int player, int count, u16 monstersOnly, u16 byOpponent);
/* PROMPT_DISCARD_COST: discard `count` hand cards as a cost (no trigger). */
void DuelPrompt_PostDiscardCost(int player, int count, u16 monstersOnly, u16 byOpponent);
/* PROMPT_DISCARD_RANDOM: discard `count` random hand cards; adds to an active one instead. */
void DuelPrompt_PostRandomDiscard(int player, u16 byOpponent, int count);
/* PROMPT_BANISH_RANDOM: banish `count` random hand cards; adds to an active one instead. */
void DuelPrompt_PostRandomBanish(int player, int count);
/* PROMPT_BANISH_RANDOM_FACE_DOWN: banish one random hand card face down. */
void DuelPrompt_PostRandomBanishFaceDown(int player);
/* PROMPT_TRIBUTE: the player tributes one of their monsters. */
void DuelPrompt_PostTribute(int player);
/* PROMPT_SET_MONSTER_FROM_HAND, if the hand holds a monster of level 4 or lower that can be Set; 1 if posted. */
u32 DuelPrompt_TryPostSetMonster(int player);

/* ---- Prompt handlers, by enum DuelPromptKind (1 when finished) ---- */

/* 1: discard `count` hand cards (enum DiscardPromptStep); monstersOnly is read from promptArgs[1]. */
int DuelPrompt_Discard(int player, int count, int monstersOnly, u16 byOpponent);
/* 2: the discard steps of DuelPrompt_Discard only. */
int DuelPrompt_DiscardCost(int player, int count, int monstersOnly, u16 byOpponent);
/* 3: discard `count` random hand cards (the cursor hops for 10 frames per card). */
int DuelPrompt_DiscardRandom(int player, u16 byOpponent, int count);
/* 4: banish `count` random hand cards. */
int DuelPrompt_BanishRandom(int player, int unused, int count);
/* 5: banish `count` random hand cards face down. */
int DuelPrompt_BanishRandomFaceDown(int player, int unused, int count);
/* 6: pick a card in the opponent's hand (Confiscation, The Forceful Sentry); slot in promptResult. */
int DuelPrompt_PickOpponentHandCard(int player);
/* 7: tribute one of the player's monsters. */
int DuelPrompt_Tribute(int player);
/* 8: choose a monster type; promptResult = type - 1. */
int DuelPrompt_SelectType(void);
/* 9: choose an attribute; promptResult = attribute - 1. */
int DuelPrompt_SelectAttribute(void);
/* 10: choose two different attributes (promptResult[0], [1]). */
int DuelPrompt_SelectTwoAttributes(void);
/* 11: pick one of the two attributes in promptArgs[0..1]. */
int DuelPrompt_PickOneOfTwoAttributes(void);
/* 12: pick one of the five card IDs in promptArgs (Painful Choice); the CPU picks at random. */
int DuelPrompt_PickOneOfFiveCards(void);
/* 13: Set a monster of level 4 or lower from the hand. */
int DuelPrompt_SetMonsterFromHand(int player);
/* 14: pick a graveyard monster from the card list; its card word is split over promptResult[0..1]. */
int DuelPrompt_SelectGraveyardMonster(s32 player, u16 cardId, u16 collectorArg);
/* 15: announce a card and ask its Yes/No question (Inspection, keys 0x5ED/0x5EF). */
u32 DuelPrompt_ConfirmCardEffect(int player, u16 cardNumber);
/* 16: pick one of the opponent's monsters (other than currentSlot) as the new attack target. */
u32 DuelPrompt_SelectOpponentReplacementTarget(int unusedPlayer, u32 currentSlot);
/* 17: offer to discard a Magic card from the hand (the CPU discards its first one). */
u32 DuelPrompt_OfferDiscardMagic(int player);
/* 18: 'Do you wish to Special-Summon %s to the field?' (cardId 0 finishes at once). */
u32 DuelPrompt_ConfirmSpecialSummon(int unusedPlayer, u16 cardId);
/* 19: 'Your opponent has Special Summoned %s. ...' Yes/No; shows the raw format string (original bug). */
u32 DuelPrompt_ConfirmGraveyardSummon(int unusedPlayer);
/* 20: pick one of the player's own monsters (other than currentSlot) as the new attack target. */
u32 DuelPrompt_SelectOwnReplacementTarget(int unusedPlayer, u32 currentSlot);

/* ---- Discard prompt (text-box callbacks) ---- */

/* Player 0's hand pick: discard the selected card if allowed; 1 when a card was discarded or the hand is empty. */
u16 DiscardPrompt_TryDiscardSelected(u16 monstersOnly, u16 byOpponent);
/* Draw callback: one marker per card still to discard (gChain.handPickCount). */
void DiscardPrompt_DrawRemaining(void);
/* Input callback: pick, discard and count down handPickCount; 1 when done. */
int DiscardPrompt_HandleInput(void);
/* Dead code: hand cards that may be discarded (monstersOnly: monsters that are not planted or graverobbed). */
int CountDiscardableHandCards(int player, u16 monstersOnly);

/* ---- Type and attribute menus (text-box callbacks) ---- */

/* Draw callback: the name of type gTextBox.result + 1, rendered when gTextBox.timer is 0. */
void TypeMenu_Draw(void);
/* Input callback: Left/Right through the 20 types, A confirms; 1 when done. */
int TypeMenu_HandleInput(void);
/* Draw callback: the name of attribute gTextBox.result + 1, rendered when gTextBox.timer is 0. */
void AttributeMenu_Draw(void);
/* Input callback: Left/Right through the 6 attributes, A confirms; 1 when done. */
int AttributeMenu_HandleInput(void);
/* Input callback of the second pick: skips the attribute in promptResult[0]; 1 when done. */
int AttributeMenu_HandleInputExcludeFirst(void);

/* ---- Five-card pick (text-box callbacks) ---- */

/* Draw callback: the five cards of promptArgs[0..4]; the selected one pulses. */
void FiveCardMenu_Draw(void);
/* Input callback: Left/Right move, A confirms; 1 when done. */
u16 FiveCardMenu_HandleInput(void);

/* ---- Top-of-deck reorder (Big Eye; state in gChain.scratch.deckReorder) ---- */

/* Step machine on deckReorder.mode (enum DeckReorderMode): the human uses the callbacks below, the CPU
 * moves its key cards up; 1 when done. */
int DeckReorder_Run(int player);
/* Draw callback: the five cards, with the swap animation in modes 10 and 20. */
void DeckReorder_Draw(void);
/* Input callback: Left/Right select, L/R move the card, A confirms; 1 when done. */
int DeckReorder_HandleInput(void);
/* Draw the five cards (card backs when hidden); the selected one pulses. */
void DeckReorder_DrawCards(int hidden);
/* Draw the five cards with cards `from` and `to` trading places along gCardJumpArc. */
void DeckReorder_DrawSwap(int hidden, int from, int to);

#endif /* GUARD_DUEL_PROMPT_H */
