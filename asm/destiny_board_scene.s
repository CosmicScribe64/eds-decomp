	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/destiny_board_scene/DestinyBoardScene_LaunchLetters.s"
	.include "asm/nonmatching/destiny_board_scene/DestinyBoardScene_ResetLetters.s"
	.include "asm/nonmatching/destiny_board_scene/DestinyBoardScene_Init.s"
	.include "asm/nonmatching/destiny_board_scene/DestinyBoardScene_Load.s"
	.include "asm/nonmatching/destiny_board_scene/DestinyBoardScene_Update.s"
	.include "asm/nonmatching/destiny_board_scene/DestinyBoardScene_DisableHBlank.s"
	.include "asm/nonmatching/destiny_board_scene/DestinyBoardScene_Run.s"
	.include "asm/nonmatching/destiny_board_scene/TurnOrder_ShowWaitSign.s"
	.include "asm/nonmatching/destiny_board_scene/TurnOrder_HideWaitSign.s"
	.include "asm/nonmatching/destiny_board_scene/Scroller_Move.s"
	.include "asm/nonmatching/destiny_board_scene/Scroller_SnapToStop.s"
	.include "asm/nonmatching/destiny_board_scene/Scroller_StopAtEnds.s"
	.include "asm/nonmatching/destiny_board_scene/TurnOrder_DrawHandCarousel.s"
	.include "asm/nonmatching/destiny_board_scene/TurnOrder_DrawBanner.s"
	.include "asm/nonmatching/destiny_board_scene/TurnOrder_DrawOpponentCard.s"
	.include "asm/nonmatching/destiny_board_scene/TurnOrder_DrawTurnChoice.s"
	.include "asm/nonmatching/destiny_board_scene/TurnOrder_DrawTurnChoiceConfirm.s"
