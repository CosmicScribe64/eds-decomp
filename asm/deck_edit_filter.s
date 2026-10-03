	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/deck_edit_filter/DeckEdit_FilterAndSortList.s"
	.include "asm/nonmatching/deck_edit_filter/ListFilter_Reset.s"
	.include "asm/nonmatching/deck_edit_filter/ListFilter_Init.s"
	.include "asm/nonmatching/deck_edit_filter/ListFilter_DrawCursor.s"
	.include "asm/nonmatching/deck_edit_filter/ListFilter_DrawCursorFlash.s"
	.include "asm/nonmatching/deck_edit_filter/ListFilter_ShowNowFiltering.s"
	.include "asm/nonmatching/deck_edit_filter/ListFilter_CopyBarTileColumns.s"
	.include "asm/nonmatching/deck_edit_filter/ListFilter_DrawProgressBar.s"
	.include "asm/nonmatching/deck_edit_filter/ListFilter_ProgressBarVBlank.s"
	.include "asm/nonmatching/deck_edit_filter/ListFilter_Update.s"
