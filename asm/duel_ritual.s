	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_ritual/IsJinzoOrRoyalDecreeActive.s"
	.include "asm/nonmatching/duel_ritual/IsImperialOrderActive.s"
	.include "asm/nonmatching/duel_ritual/UpdateSpellTrapNegation.s"
	.include "asm/nonmatching/duel_ritual/FindRitualRecipe.s"
	.include "asm/nonmatching/duel_ritual/HandHasRitualMonster.s"
	.include "asm/nonmatching/duel_ritual/SumHandLevelsExcept.s"
	.include "asm/nonmatching/duel_ritual/SumTributableMonsterLevels.s"
	.include "asm/nonmatching/duel_ritual/DrawRitualStarGauge.s"
	.include "asm/nonmatching/duel_ritual/RitualTributeSelectStep.s"
	.include "asm/nonmatching/duel_ritual/EffectRitualSummonPrepare.s"
	.include "asm/nonmatching/duel_ritual/EffectRitualSummonResolve.s"
	.include "asm/nonmatching/duel_ritual/CanReviveGraveyardCard.s"
