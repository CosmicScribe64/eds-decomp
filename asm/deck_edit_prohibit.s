	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/deck_edit_prohibit/ProhibitCardSelect_SwitchScreen.s"
	.include "asm/nonmatching/deck_edit_prohibit/TradeCardSelect_Init.s"
	.include "asm/nonmatching/deck_edit_prohibit/CardSelect_HandleListSwitch.s"
	.include "asm/nonmatching/deck_edit_prohibit/TradeCardSelect_Update.s"
