	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/booster_pack/GetPackCommonSlot.s"
	.include "asm/nonmatching/booster_pack/RollPackRarity.s"
	.include "asm/nonmatching/booster_pack/PickPackSlotCard.s"
	.include "asm/nonmatching/booster_pack/GeneratePackCards.s"
	.include "asm/nonmatching/booster_pack/IsCardNumberOwned.s"
	.include "asm/nonmatching/booster_pack/GetPack_SelectAndGenerate.s"
	.include "asm/nonmatching/booster_pack/GetPack_InitScene.s"
	.include "asm/nonmatching/booster_pack/GetPack_FadeIn.s"
	.include "asm/nonmatching/booster_pack/GetPack_RevealCards.s"
	.include "asm/nonmatching/booster_pack/GetPack_HandleInput.s"
	.include "asm/nonmatching/booster_pack/GetPack_FadeOutAndAddCards.s"
	.include "asm/nonmatching/booster_pack/GetPack_ShowCardDetail.s"
