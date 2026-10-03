	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_DiscardRandom.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_BanishRandom.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_BanishRandomFaceDown.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_Tribute.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_SetMonsterFromHand.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_SelectGraveyardMonster.s"
	.include "asm/nonmatching/duel_prompt_handlers/TypeMenu_Draw.s"
	.include "asm/nonmatching/duel_prompt_handlers/TypeMenu_HandleInput.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_SelectType.s"
	.include "asm/nonmatching/duel_prompt_handlers/AttributeMenu_Draw.s"
	.include "asm/nonmatching/duel_prompt_handlers/AttributeMenu_HandleInput.s"
	.include "asm/nonmatching/duel_prompt_handlers/AttributeMenu_HandleInputExcludeFirst.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_SelectAttribute.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_SelectTwoAttributes.s"
	.include "asm/nonmatching/duel_prompt_handlers/TextBoxHandleChoiceInputCpu.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_PickOneOfTwoAttributes.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelPrompt_PickOpponentHandCard.s"
	.include "asm/nonmatching/duel_prompt_handlers/DuelCursor_IsValidTarget.s"
