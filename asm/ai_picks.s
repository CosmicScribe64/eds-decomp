	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/ai_picks/AiPickCardListEntry.s"
	.include "asm/nonmatching/ai_picks/AiGetStrongestMonsterScore.s"
	.include "asm/nonmatching/ai_picks/AiFindStrongestMonster.s"
	.include "asm/nonmatching/ai_picks/AiFindWeakestMonster.s"
	.include "asm/nonmatching/ai_picks/SumMonsterAtk.s"
	.include "asm/nonmatching/ai_picks/AiCanChangePosition.s"
	.include "asm/nonmatching/ai_picks/AiShouldSetMonster.s"
	.include "asm/nonmatching/ai_picks/AiCountExodiaInDeck.s"
	.include "asm/nonmatching/ai_picks/AiCountExodiaInGraveyard.s"
	.include "asm/nonmatching/ai_picks/AiCountExodiaOnField.s"
	.include "asm/nonmatching/ai_picks/AiPickOpponentHandCard.s"
	.include "asm/nonmatching/ai_picks/AiEvalAttack.s"
	.include "asm/nonmatching/ai_picks/AiFindAttackTarget.s"
	.include "asm/nonmatching/ai_picks/AiCanBeatMonster.s"
	.include "asm/nonmatching/ai_picks/AiCanBeatAnyMonster.s"
	.include "asm/nonmatching/ai_picks/AiChooseAttack.s"
	.include "asm/nonmatching/ai_picks/AiBackupDuelState.s"
	.include "asm/nonmatching/ai_picks/AiRestoreDuelState.s"
	.include "asm/nonmatching/ai_picks/AiSimSetAttackPositions.s"
