	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/summon_action/SummonStep_Flip.s"
	.include "asm/nonmatching/summon_action/SummonStep_Special.s"
	.include "asm/nonmatching/summon_action/SummonStep_SpecialChoosePosition.s"
	.include "asm/nonmatching/summon_action/SummonStep_SpecialFromHand.s"
	.include "asm/nonmatching/summon_action/SummonAction_Update.s"
	.include "asm/nonmatching/summon_action/SummonAction_Start.s"
	.include "asm/nonmatching/summon_action/SummonAction_StartFromLink.s"
	.include "asm/nonmatching/summon_action/QueueNormalSummon.s"
	.include "asm/nonmatching/summon_action/QueueNormalSummonChoosePosition.s"
