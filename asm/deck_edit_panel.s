	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/deck_edit_panel/PackList_DebugNop.s"
	.include "asm/nonmatching/deck_edit_panel/PackList_Init.s"
	.include "asm/nonmatching/deck_edit_panel/PackList_HandleInput.s"
	.include "asm/nonmatching/deck_edit_panel/PackList_FadeOut.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_DrawPortraitTilemap.s"
	.include "asm/nonmatching/deck_edit_panel/RenderOutlinedFontTiles.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_GetListRowVram.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_GetCursorRowVram.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_GetListRowTile.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_GetCursorRowTile.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_RotateListRowRing.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_FlipCursorRowPage.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_DrawListRowName.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_DrawCursorRowName.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_DrawNoCardsText.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_LoadCardIconTiles.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_DrawCardIcon.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_DrawCardIcons.s"
	.include "asm/nonmatching/deck_edit_panel/DeckEdit_DrawAtkDef.s"
