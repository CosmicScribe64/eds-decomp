	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/battle_phase1/BattleStage_DeclareAttack.s"
	.include "asm/nonmatching/battle_phase1/BattleStage_PayAttackCosts.s"
	.include "asm/nonmatching/battle_phase1/BattleStage_RespondToAttack.s"
	.include "asm/nonmatching/battle_phase1/BattleStage_RevealDefender.s"
	.include "asm/nonmatching/battle_phase1/BattleStage_DamageCalc.s"
