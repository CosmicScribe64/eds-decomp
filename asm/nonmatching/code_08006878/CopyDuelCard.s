	thumb_func_start CopyDuelCard
CopyDuelCard: @ 0x08007558
	ldr r1, [r1]
	str r1, [r0]
	bx lr
	thumb_func_end CopyDuelCard
	.align 2, 0

