	thumb_func_start DuelScreen_VBlank
DuelScreen_VBlank: @ 0x08060400
	ldr r0, _08060414 @ =0x0201CFB0
	ldrh r1, [r0, #2]
	sub r1, #1
	strh r1, [r0, #2]
	ldr r0, _08060418 @ =0x04000012
	strh r1, [r0]
	sub r0, #2
	strh r1, [r0]
	bx lr
	.align 2, 0
_08060414: .4byte 0x0201CFB0
_08060418: .4byte 0x04000012
	thumb_func_end DuelScreen_VBlank

