	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/campaign_steps/Campaign_SelectOpponent.s"
	.include "asm/nonmatching/campaign_steps/Campaign_DecideTurnOrder.s"
	.include "asm/nonmatching/campaign_steps/Campaign_SetupDuel.s"
	.include "asm/nonmatching/campaign_steps/Campaign_RecordDuelResult.s"
	.include "asm/nonmatching/campaign_steps/Campaign_GiveRewards.s"
	.include "asm/nonmatching/campaign_steps/Campaign_ShowDuelResult.s"
