	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/deck_edit_list/DeckEdit_UpdatePanelHighlight.s"
	.include "asm/nonmatching/deck_edit_list/DeckEdit_CountSideDeckMonsters.s"
	.include "asm/nonmatching/deck_edit_list/DeckEdit_StartListSlide.s"
	.include "asm/nonmatching/deck_edit_list/DeckEdit_HandleShoulderKeys.s"
	.include "asm/nonmatching/deck_edit_list/DeckEdit_CommandLabelVBlank.s"
	.include "asm/nonmatching/deck_edit_list/DeckEdit_CommandWindowVBlank.s"
	.include "asm/nonmatching/deck_edit_list/DeckEdit_DrawCommandMenu.s"
	.include "asm/nonmatching/deck_edit_list/DeckEdit_UpdateCommandMenuAnim.s"
	.include "asm/nonmatching/deck_edit_list/DeckEdit_DrawStatementLabels.s"
	.include "asm/nonmatching/deck_edit_list/DeckEdit_ScrollListUp.s"
	.include "asm/nonmatching/deck_edit_list/DeckEdit_ScrollListDown.s"
