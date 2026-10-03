	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/ai_turn_steps/AiPickMonsterToSet.s"
	.include "asm/nonmatching/ai_turn_steps/AiStepStartMainPhase.s"
	.include "asm/nonmatching/ai_turn_steps/AiStepSetSpellTraps.s"
	.include "asm/nonmatching/ai_turn_steps/AiStepPlaySimpleSpells.s"
	.include "asm/nonmatching/ai_turn_steps/AiStepFlipSummon.s"
