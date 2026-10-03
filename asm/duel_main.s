	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_main/Chain_Resolve.s"
	.include "asm/nonmatching/duel_main/Chain_Update.s"
	.include "asm/nonmatching/duel_main/HasExodiaInHand.s"
	.include "asm/nonmatching/duel_main/HasDestinyBoardComplete.s"
	.include "asm/nonmatching/duel_main/Duel_CheckWin.s"
	.include "asm/nonmatching/duel_main/DuelPhase_Init.s"
	.include "asm/nonmatching/duel_main/DuelPhase_ShowResult.s"
	.include "asm/nonmatching/duel_main/DuelMainStep.s"
