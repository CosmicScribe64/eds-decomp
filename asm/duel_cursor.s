	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_cursor/DuelCursor_FindTargetHorizontal.s"
	.include "asm/nonmatching/duel_cursor/DuelCursor_FindTarget.s"
	.include "asm/nonmatching/duel_cursor/DuelCursor_PickTarget.s"
	.include "asm/nonmatching/duel_cursor/DuelCursor_PickAny.s"
	.include "asm/nonmatching/duel_cursor/DeckReorder_DrawCards.s"
	.include "asm/nonmatching/duel_cursor/DeckReorder_DrawSwap.s"
	.include "asm/nonmatching/duel_cursor/DeckReorder_Draw.s"
	.include "asm/nonmatching/duel_cursor/DeckReorder_HandleInput.s"
	.include "asm/nonmatching/duel_cursor/DeckReorder_Run.s"
