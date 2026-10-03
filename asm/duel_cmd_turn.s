	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_TurnStart.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_TurnEnd.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_ShowEndTurnHand.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_SkipNextDrawPhase.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_SkipNextStandbyPhase.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_SkipNextTurn.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_SetExtraBattlePhase.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_SetPositionChangeLock.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_SetSummonLocks.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_SetMagicTrapLockTurns.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_SetStatChangesReversed.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_SetAtkDefSwapped.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_AdjustDelayedSummonCount.s"
	.include "asm/nonmatching/duel_cmd_turn/sub_08014B5C.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_ShowCardDetail.s"
	.include "asm/nonmatching/duel_cmd_turn/DuelCmd_ShowCardAssemble.s"
