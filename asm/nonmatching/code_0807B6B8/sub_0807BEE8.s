	thumb_func_start sub_0807BEE8
sub_0807BEE8: @ 0x0807BEE8
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r1, _0807BEF4 @ =0x0400012A
	strh r0, [r1]
	bx lr
	.align 2, 0
_0807BEF4: .4byte 0x0400012A
	thumb_func_end sub_0807BEE8

