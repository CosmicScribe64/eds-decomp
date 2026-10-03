	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/code_080590E4/RemoveAllDeckCardsByNumber.s"
	.include "asm/nonmatching/code_080590E4/CountDeckCardsByNumber.s"
	.include "asm/nonmatching/code_080590E4/RemoveOverLimitDeckCards.s"
	.include "asm/nonmatching/code_080590E4/LoadOpponentDeck.s"
	.include "asm/nonmatching/code_080590E4/AiHasUsableSpellTrap.s"
	.include "asm/nonmatching/code_080590E4/AiSelectUsableSpellTrap.s"
	.include "asm/nonmatching/code_080590E4/AiTryPlaySpellTrap.s"
	.include "asm/nonmatching/code_080590E4/AiActivateMonsterEffects.s"
	.include "asm/nonmatching/code_080590E4/AiActivateExodiaTraps.s"
	.include "asm/nonmatching/code_080590E4/AiRiskSetCounter.s"
	.include "asm/nonmatching/code_080590E4/AiIsOpponentThreatening.s"
	.include "asm/nonmatching/code_080590E4/AiPlaySpells.s"
