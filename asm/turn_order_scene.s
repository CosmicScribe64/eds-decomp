	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/turn_order_scene/TurnOrder_DrawChosenTurnBanner.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_DrawDuelLogo.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_DrawUnusedSprite.s"
	.include "asm/nonmatching/turn_order_scene/JudgeRockPaperScissors.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_UpdateChoiceBob.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_CpuPickTurn.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_Init.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_LoadObjTiles.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_Load.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_ChooseHand.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_ShowResult.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_ChooseTurn.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_AnimateTurnChoice.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_ShowDuelLogo.s"
	.include "asm/nonmatching/turn_order_scene/TurnOrder_FlashWhite.s"
