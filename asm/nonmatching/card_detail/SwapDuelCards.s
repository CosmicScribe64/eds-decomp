	thumb_func_start SwapDuelCards
SwapDuelCards: @ 0x08007560
	ldr r3, [r0]
	ldr r2, [r1]
	str r2, [r0]
	str r3, [r1]
	bx lr
	thumb_func_end SwapDuelCards
	.align 2, 0

