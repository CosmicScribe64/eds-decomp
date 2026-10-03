	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/deck_edit_cards/DeckEdit_DrawCardCounts.s"
	.include "asm/nonmatching/deck_edit_cards/DeckEdit_GetSelectedCardCopies.s"
	.include "asm/nonmatching/deck_edit_cards/DeckEdit_GetActiveListRow.s"
	.include "asm/nonmatching/deck_edit_cards/DeckEdit_BuildCardLists.s"
	.include "asm/nonmatching/deck_edit_cards/DeckEdit_UpdateNameIndexLetters.s"
	.include "asm/nonmatching/deck_edit_cards/DeckEdit_DrawNameIndexTab.s"
	.include "asm/nonmatching/deck_edit_cards/DeckEdit_GetListCard.s"
	.include "asm/nonmatching/deck_edit_cards/DeckEdit_SetListCard.s"
	.include "asm/nonmatching/deck_edit_cards/DeckEdit_PlaceCardArt.s"
	.include "asm/nonmatching/deck_edit_cards/CompareCardsByAtk.s"
	.include "asm/nonmatching/deck_edit_cards/CompareCardsByDef.s"
	.include "asm/nonmatching/deck_edit_cards/CompareCardsByType.s"
	.include "asm/nonmatching/deck_edit_cards/CompareCardsByAttribute.s"
	.include "asm/nonmatching/deck_edit_cards/CompareCardsByLevel.s"
	.include "asm/nonmatching/deck_edit_cards/QuickSortS16.s"
