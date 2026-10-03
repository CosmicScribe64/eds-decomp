	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/campaign/Campaign_DeliverMagazines.s"
	.include "asm/nonmatching/campaign/Campaign_AdvanceDay.s"
	.include "asm/nonmatching/campaign/Campaign_DeckTooSmall.s"
	.include "asm/nonmatching/campaign/CB_Campaign.s"
	.include "asm/nonmatching/campaign/CalcBattle.s"
	.include "asm/nonmatching/campaign/CardMenu_DrawIcons.s"
	.include "asm/nonmatching/campaign/CardMenu_DrawLabel.s"
	.include "asm/nonmatching/campaign/CardMenu_DrawCardPreview.s"
	.include "asm/nonmatching/campaign/CardMenu_Update.s"
