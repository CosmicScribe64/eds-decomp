	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/bg_image/LoadBgImage.s"
	.include "asm/nonmatching/bg_image/LoadBgImage4bppMap4Rel.s"
	.include "asm/nonmatching/bg_image/LoadBgImage4bppGfx.s"
	.include "asm/nonmatching/bg_image/LoadBgImage4bppMap1.s"
	.include "asm/nonmatching/bg_image/LoadBgImage4bpp.s"
	.include "asm/nonmatching/bg_image/LoadBgImage4bppToMap.s"
	.include "asm/nonmatching/bg_image/LoadBgImage4bppMap1Rel.s"
	.include "asm/nonmatching/bg_image/ClearBgMapBuffers.s"
	.include "asm/nonmatching/bg_image/ClearBgMapBuffer0.s"
	.include "asm/nonmatching/bg_image/FillMapRect.s"
	.include "asm/nonmatching/bg_image/CopyBgTileBufferToVram.s"
	.include "asm/nonmatching/bg_image/ResetVideo.s"
	.include "asm/nonmatching/bg_image/LinkSioInit.s"
	.include "asm/nonmatching/bg_image/LinkSioStop.s"
	.include "asm/nonmatching/bg_image/LinkSioSend.s"
	.include "asm/nonmatching/bg_image/LinkSioRecvMultiBlock.s"
	.include "asm/nonmatching/bg_image/LinkSioRecvSingle.s"
	.include "asm/nonmatching/bg_image/LinkSioRecvDebug.s"
	.include "asm/nonmatching/bg_image/LinkSioRecv.s"
