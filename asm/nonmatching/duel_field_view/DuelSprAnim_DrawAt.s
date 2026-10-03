	thumb_func_start DuelSprAnim_DrawAt
DuelSprAnim_DrawAt: @ 0x08024228
	push {lr}
	add r3, r2, #0
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	ldr r2, _08024244 @ =0x0201D7F8
	bl SprAnimDrawFrame
	pop {r0}
	bx r0
	.align 2, 0
_08024244: .4byte 0x0201D7F8
	thumb_func_end DuelSprAnim_DrawAt

