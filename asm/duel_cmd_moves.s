	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_SetMagicalHatsCard.s"
	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_BanishMonsterUntilEndPhase.s"
	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_ReturnBanishedMonster.s"
	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_SendFusionMaterialToGrave.s"
	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_SetZoneLevelCheckFlag.s"
	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_ClearZoneLinks2.s"
	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_ShowDuelResult.s"
	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_Surrender.s"
	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_ShowJustAMomentBanner.s"
	.include "asm/nonmatching/duel_cmd_moves/DrawLpChangeAmount.s"
	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_ChangeLifePoints.s"
	.include "asm/nonmatching/duel_cmd_moves/DuelCmd_SetNegationFlag.s"
