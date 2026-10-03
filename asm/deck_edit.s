	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/deck_edit/DeckEdit_TickFadeIn.s"
	.include "asm/nonmatching/deck_edit/DeckEdit_SwitchScreen.s"
	.include "asm/nonmatching/deck_edit/TradeCardSelect_SwitchScreen.s"
	.include "asm/nonmatching/deck_edit/CB_DeckEdit.s"
	.include "asm/nonmatching/deck_edit/SideDeckSwap_Run.s"
	.include "asm/nonmatching/deck_edit/TradeCardSelect_Run.s"
	.include "asm/nonmatching/deck_edit/ProhibitCardSelect_Run.s"
	.include "asm/nonmatching/deck_edit/ProhibitCardSelect_StartAndRun.s"
	.include "asm/nonmatching/deck_edit/ProhibitCardSelect_Init.s"
	.include "asm/nonmatching/deck_edit/sub_0806F400.s"
	.include "asm/nonmatching/deck_edit/ProhibitCardSelect_InitListView.s"
	.include "asm/nonmatching/deck_edit/ProhibitCardSelect_Update.s"
