	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/summon_checks/FiveCardMenu_Draw.s"
	.include "asm/nonmatching/summon_checks/FiveCardMenu_HandleInput.s"
	.include "asm/nonmatching/summon_checks/DuelPrompt_PickOneOfFiveCards.s"
	.include "asm/nonmatching/summon_checks/CanSummonValkyrion.s"
	.include "asm/nonmatching/summon_checks/CanSummonKey1257.s"
	.include "asm/nonmatching/summon_checks/CanPayBanishSummonCost.s"
	.include "asm/nonmatching/summon_checks/CanSummonFromHand.s"
	.include "asm/nonmatching/summon_checks/SummonPositionMenu_Draw.s"
	.include "asm/nonmatching/summon_checks/SummonPositionMenu_HandleInput.s"
	.include "asm/nonmatching/summon_checks/ExecuteSummonAction.s"
	.include "asm/nonmatching/summon_checks/ExecuteSummonActionAskPosition.s"
