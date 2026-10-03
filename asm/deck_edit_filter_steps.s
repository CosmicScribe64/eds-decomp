	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/deck_edit_filter_steps/DeckEdit_RunListFilter.s"
	.include "asm/nonmatching/deck_edit_filter_steps/ProhibitCardSelect_RunListFilter.s"
	.include "asm/nonmatching/deck_edit_filter_steps/SideDeckSwap_Init.s"
	.include "asm/nonmatching/deck_edit_filter_steps/SideDeckSwap_EnterListView.s"
	.include "asm/nonmatching/deck_edit_filter_steps/SideDeckSwap_HandleListSwitch.s"
	.include "asm/nonmatching/deck_edit_filter_steps/SideDeckSwap_FindCardInList.s"
	.include "asm/nonmatching/deck_edit_filter_steps/SideDeckSwap_ExchangeCards.s"
	.include "asm/nonmatching/deck_edit_filter_steps/SideDeckSwap_UpdateExchange.s"
	.include "asm/nonmatching/deck_edit_filter_steps/SideDeckSwap_CancelExchange.s"
	.include "asm/nonmatching/deck_edit_filter_steps/SideDeckSwap_Update.s"
