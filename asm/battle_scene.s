	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/battle_scene/BattleScene_Update.s"
	.include "asm/nonmatching/battle_scene/DuelCursor_GetCardId.s"
	.include "asm/nonmatching/battle_scene/TextCellsResetMap.s"
	.include "asm/nonmatching/battle_scene/TextCellsClear.s"
	.include "asm/nonmatching/battle_scene/TextCellsPutSjisString.s"
	.include "asm/nonmatching/battle_scene/TextCellsPutString.s"
	.include "asm/nonmatching/battle_scene/TextCellsPutNumber.s"
	.include "asm/nonmatching/battle_scene/TextCellsCopyBgTile.s"
	.include "asm/nonmatching/battle_scene/TextCellsLoadIcon.s"
	.include "asm/nonmatching/battle_scene/DuelInfo_DrawCardNameCentered.s"
	.include "asm/nonmatching/battle_scene/DuelInfo_DrawCard.s"
	.include "asm/nonmatching/battle_scene/DuelInfo_DrawSpellZone.s"
	.include "asm/nonmatching/battle_scene/DuelInfo_DrawLabelNumber.s"
	.include "asm/nonmatching/battle_scene/DuelInfo_DrawMonsterZone.s"
