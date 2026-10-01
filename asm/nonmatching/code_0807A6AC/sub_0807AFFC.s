	thumb_func_start sub_0807AFFC
sub_0807AFFC: @ 0x0807AFFC
	push {lr}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	bl sub_0807AEF0
	pop {r0}
	bx r0
	thumb_func_end sub_0807AFFC
	.align 2, 0

