	thumb_func_start AgbMain
AgbMain: @ 0x08075F64
	push {lr}
	bl GameInit
	bl MainLoop
	pop {r0}
	bx r0
	thumb_func_end AgbMain
	.align 2, 0

