	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/code_080629F0/GetPackCommonSlot.s"
	.include "asm/nonmatching/code_080629F0/RollPackRarity.s"
	.include "asm/nonmatching/code_080629F0/PickPackSlotCard.s"
	.include "asm/nonmatching/code_080629F0/GeneratePackCards.s"
	.include "asm/nonmatching/code_080629F0/IsCardNumberOwned.s"
	.include "asm/nonmatching/code_080629F0/GetPack_SelectAndGenerate.s"
	.include "asm/nonmatching/code_080629F0/GetPack_InitScene.s"
	.include "asm/nonmatching/code_080629F0/GetPack_FadeIn.s"
	.include "asm/nonmatching/code_080629F0/GetPack_RevealCards.s"
	.include "asm/nonmatching/code_080629F0/GetPack_HandleInput.s"
	.include "asm/nonmatching/code_080629F0/GetPack_FadeOutAndAddCards.s"
	.include "asm/nonmatching/code_080629F0/GetPack_ShowCardDetail.s"
