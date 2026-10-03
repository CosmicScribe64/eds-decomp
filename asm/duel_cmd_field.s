	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_cmd_field/DuelCmd_ChangePosition.s"
	.include "asm/nonmatching/duel_cmd_field/DuelCmd_FlipCard.s"
	.include "asm/nonmatching/duel_cmd_field/DuelCmd_SendToGraveyard.s"
	.include "asm/nonmatching/duel_cmd_field/DuelCmd_Banish.s"
	.include "asm/nonmatching/duel_cmd_field/DuelCmd_BanishFlagged.s"
	.include "asm/nonmatching/duel_cmd_field/DuelCmd_ReturnToHand.s"
	.include "asm/nonmatching/duel_cmd_field/DuelCmd_ReturnToDeck.s"
	.include "asm/nonmatching/duel_cmd_field/DuelCmd_MoveToZone.s"
	.include "asm/nonmatching/duel_cmd_field/DuelCmd_SwapZones.s"
