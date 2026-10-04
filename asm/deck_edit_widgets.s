	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_DrawLevelStars.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_CalcScrollBar.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_DrawScrollBar.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_LoadCardBoxTiles.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_ResetFrameSlots.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_TweenFrameSlots.s"
	.include "asm/nonmatching/deck_edit_widgets/GetCardFrameIndex.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_ScrollFrameSlots.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_DrawFrameSlots.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_InitFrameSlot.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_ResetCardMove.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_StartCardMove.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_BeginCardMove.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_IsFusionMonster.s"
	.include "asm/nonmatching/deck_edit_widgets/DeckEdit_UpdateCardMove.s"
