	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/code_08050A70/DuelPhase_TurnEnd.s"
	.include "asm/nonmatching/code_08050A70/DuelLink_RunCardPrompt.s"
	.include "asm/nonmatching/code_08050A70/DuelLink_RunRemoteChainA.s"
	.include "asm/nonmatching/code_08050A70/DuelLink_RunRemoteChainB.s"
	.include "asm/nonmatching/code_08050A70/DuelLink_RunRemoteResolve.s"
	.include "asm/nonmatching/code_08050A70/DuelLink_RunPartnerRequests.s"
	.include "asm/nonmatching/code_08050A70/DuelPhase_OpponentTurn.s"
	.include "asm/nonmatching/code_08050A70/CountDiscardableHandCards.s"
	.include "asm/nonmatching/code_08050A70/DiscardPrompt_TryDiscardSelected.s"
	.include "asm/nonmatching/code_08050A70/DiscardPrompt_DrawRemaining.s"
	.include "asm/nonmatching/code_08050A70/DiscardPrompt_HandleInput.s"
	.include "asm/nonmatching/code_08050A70/DuelPrompt_Discard.s"
	.include "asm/nonmatching/code_08050A70/DuelPrompt_DiscardCost.s"
