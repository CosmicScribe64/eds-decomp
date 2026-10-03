	thumb_func_start DuelSprAnim_DrawFlip
DuelSprAnim_DrawFlip: @ 0x08024268
	push {r4, lr}
	add r4, r1, #0
	add r3, r2, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	ldr r1, _08024284 @ =0x0201D7F8
	add r2, r4, #0
	bl SprAnimDrawFrameAtFlip
	pop {r4}
	pop {r0}
	bx r0
_08024284: .4byte 0x0201D7F8
	thumb_func_end DuelSprAnim_DrawFlip

