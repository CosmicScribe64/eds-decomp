	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/card_canvas/UnloadDuelUiGfx.s"
	.include "asm/nonmatching/card_canvas/LoadCardFrame.s"
	.include "asm/nonmatching/card_canvas/LoadCardPicture.s"
	.include "asm/nonmatching/card_canvas/DrawCardInfo.s"
	.include "asm/nonmatching/card_canvas/GetCardIconObjTile.s"
	.include "asm/nonmatching/card_canvas/GetCardIconBgTile.s"
	.include "asm/nonmatching/card_canvas/GetZoneArea.s"
	.include "asm/nonmatching/card_canvas/GetHandCardX.s"
	.include "asm/nonmatching/card_canvas/GetAreaX.s"
	.include "asm/nonmatching/card_canvas/GetAreaY.s"
	.include "asm/nonmatching/card_canvas/GetPack_HBlank.s"
	.include "asm/nonmatching/card_canvas/GetPack_ScrollBg.s"
	.include "asm/nonmatching/card_canvas/GetPack_DrawCardSprites.s"
	.include "asm/nonmatching/card_canvas/GetPack_DrawCardRow.s"
