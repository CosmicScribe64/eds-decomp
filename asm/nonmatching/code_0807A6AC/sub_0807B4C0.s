	thumb_func_start sub_0807B4C0
sub_0807B4C0: @ 0x0807B4C0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r1, _0807B4CC @ =0x04000054
	strh r0, [r1]
	bx lr
	.align 2, 0
_0807B4CC: .4byte 0x04000054
	thumb_func_end sub_0807B4C0

