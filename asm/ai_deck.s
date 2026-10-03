	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/ai_deck/RemoveAllDeckCardsByNumber.s"
	.include "asm/nonmatching/ai_deck/CountDeckCardsByNumber.s"
	.include "asm/nonmatching/ai_deck/RemoveOverLimitDeckCards.s"
	.include "asm/nonmatching/ai_deck/LoadOpponentDeck.s"
	.include "asm/nonmatching/ai_deck/AiHasUsableSpellTrap.s"
	.include "asm/nonmatching/ai_deck/AiSelectUsableSpellTrap.s"
	.include "asm/nonmatching/ai_deck/AiTryPlaySpellTrap.s"
	.include "asm/nonmatching/ai_deck/AiActivateMonsterEffects.s"
	.include "asm/nonmatching/ai_deck/AiActivateExodiaTraps.s"
	.include "asm/nonmatching/ai_deck/AiRiskSetCounter.s"
	.include "asm/nonmatching/ai_deck/AiIsOpponentThreatening.s"
	.include "asm/nonmatching/ai_deck/AiPlaySpells.s"
