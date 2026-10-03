	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_turn_end/DuelPhase_TurnEnd.s"
	.include "asm/nonmatching/duel_turn_end/DuelLink_RunCardPrompt.s"
	.include "asm/nonmatching/duel_turn_end/DuelLink_RunRemoteChainA.s"
	.include "asm/nonmatching/duel_turn_end/DuelLink_RunRemoteChainB.s"
	.include "asm/nonmatching/duel_turn_end/DuelLink_RunRemoteResolve.s"
	.include "asm/nonmatching/duel_turn_end/DuelLink_RunPartnerRequests.s"
	.include "asm/nonmatching/duel_turn_end/DuelPhase_OpponentTurn.s"
	.include "asm/nonmatching/duel_turn_end/CountDiscardableHandCards.s"
	.include "asm/nonmatching/duel_turn_end/DiscardPrompt_TryDiscardSelected.s"
	.include "asm/nonmatching/duel_turn_end/DiscardPrompt_DrawRemaining.s"
	.include "asm/nonmatching/duel_turn_end/DiscardPrompt_HandleInput.s"
	.include "asm/nonmatching/duel_turn_end/DuelPrompt_Discard.s"
	.include "asm/nonmatching/duel_turn_end/DuelPrompt_DiscardCost.s"
