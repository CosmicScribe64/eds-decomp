	thumb_func_start DebugPrintf
DebugPrintf: @ 0x0801A7DC
	push {r0, r1, r2, r3}
	add sp, #0x10
	bx lr
	thumb_func_end DebugPrintf
	.align 2, 0

