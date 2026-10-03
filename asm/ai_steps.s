	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/ai_steps/AiStepMainPhase.s"
	.include "asm/nonmatching/ai_steps/AiStepChangePositions.s"
	.include "asm/nonmatching/ai_steps/AiStepBattle.s"
	.include "asm/nonmatching/ai_steps/AiRunStep.s"
	.include "asm/nonmatching/ai_steps/AiChooseStrategy.s"
	.include "asm/nonmatching/ai_steps/AiStrategyCyberStein.s"
