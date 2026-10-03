	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_card_anim/DuelAnim_UpdateChangePosition.s"
	.include "asm/nonmatching/duel_card_anim/DuelAnim_UpdateFlip.s"
	.include "asm/nonmatching/duel_card_anim/DuelAnim_UpdateMoveCard.s"
	.include "asm/nonmatching/duel_card_anim/DuelAnim_UpdateSwapCards.s"
	.include "asm/nonmatching/duel_card_anim/DuelAnim_UpdateZoneEffect.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_HBlank.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_ResetBgAffine.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_DrawSmallNumber.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_DrawBigNumber.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_DrawAtk.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_DrawDef.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_DrawValues.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_DrawDamage.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_LoadCardArt.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_LoadCardFrame.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_SetCardArtMap.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_SetCardFrameMap.s"
	.include "asm/nonmatching/duel_card_anim/BattleScene_Init.s"
