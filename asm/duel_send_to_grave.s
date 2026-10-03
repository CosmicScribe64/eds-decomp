	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_send_to_grave/UpdateMonsterControl.s"
	.include "asm/nonmatching/duel_send_to_grave/SendFieldCardToGrave.s"
	.include "asm/nonmatching/duel_send_to_grave/QueueAddZoneLink.s"
	.include "asm/nonmatching/duel_send_to_grave/QueueRemoveZoneLink.s"
	.include "asm/nonmatching/duel_send_to_grave/EquipCard.s"
	.include "asm/nonmatching/duel_send_to_grave/MoveEquipCard.s"
	.include "asm/nonmatching/duel_send_to_grave/QueueRemoveLinksToZone.s"
	.include "asm/nonmatching/duel_send_to_grave/RemoveTargetLinksTo.s"
	.include "asm/nonmatching/duel_send_to_grave/DestroyLinkedCards.s"
	.include "asm/nonmatching/duel_send_to_grave/DestroyAbsorbedMonsters.s"
	.include "asm/nonmatching/duel_send_to_grave/TributeMonster.s"
	.include "asm/nonmatching/duel_send_to_grave/SendFusionMaterialToGrave.s"
