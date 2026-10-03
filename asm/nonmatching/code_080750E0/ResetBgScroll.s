	thumb_func_start ResetBgScroll
ResetBgScroll: @ 0x080757AC
	push {lr}
	bl ResetBgHofs
	bl ResetBgVofs
	pop {r0}
	bx r0
	thumb_func_end ResetBgScroll
	.align 2, 0

