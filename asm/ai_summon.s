	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/ai_summon/AiSimSummon.s"
	.include "asm/nonmatching/ai_summon/AiSimBattlePhase.s"
	.include "asm/nonmatching/ai_summon/AiSimBattleDamage.s"
	.include "asm/nonmatching/ai_summon/AiPlanAttack.s"
	.include "asm/nonmatching/ai_summon/AiChooseSummonWithTribute.s"
	.include "asm/nonmatching/ai_summon/AiChooseSummonNoTribute.s"
	.include "asm/nonmatching/ai_summon/AiShouldActivateSetCard.s"
	.include "asm/nonmatching/ai_summon/AiChainSetCard.s"
	.include "asm/nonmatching/ai_summon/AiTryChainResponse.s"
	.include "asm/nonmatching/ai_summon/AddCardNumberToDeckTop.s"
