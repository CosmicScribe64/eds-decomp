	thumb_func_start AgbMain
AgbMain: @ 0x08075F64
	push {lr}
	bl sub_08075DF4
	bl sub_08075D70
	pop {r0}
	bx r0
	thumb_func_end AgbMain
	.align 2, 0

