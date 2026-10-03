	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/deck_edit_stats/DeckStats_ClearState.s"
	.include "asm/nonmatching/deck_edit_stats/GetCardCopiesInList.s"
	.include "asm/nonmatching/deck_edit_stats/DeckStats_CountCategory.s"
	.include "asm/nonmatching/deck_edit_stats/DeckStats_Compute.s"
	.include "asm/nonmatching/deck_edit_stats/DeckStats_DrawNumbers.s"
	.include "asm/nonmatching/deck_edit_stats/DeckStats_Init.s"
	.include "asm/nonmatching/deck_edit_stats/DeckStats_Update.s"
	.include "asm/nonmatching/deck_edit_stats/DeckEdit_RunStatistics.s"
	.include "asm/nonmatching/deck_edit_stats/DeckEdit_Init.s"
