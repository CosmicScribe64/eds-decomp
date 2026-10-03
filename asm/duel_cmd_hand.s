	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_RemoveCardFromHand.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_PlaceMonsterFromHand.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_PlaceSpellTrapFromHand.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_CompactHand.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_AddCardToHand.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_BanishHandCardFaceDown.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_ReturnBanishedCardToHand.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_ExchangeHandCards.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_SendHandFusionMaterialToGraveyard.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_BanishHandFusionMaterial.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_ReturnSpellTrapToHand.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_ClearZoneCardNoRedraw.s"
	.include "asm/nonmatching/duel_cmd_hand/DuelCmd_SendSpellTrapToGraveyard.s"
