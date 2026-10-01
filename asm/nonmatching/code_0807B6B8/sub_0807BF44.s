	thumb_func_start sub_0807BF44
sub_0807BF44: @ 0x0807BF44
	ldr r0, _0807BF50 @ =0x04000128
	ldrh r1, [r0]
	mov r2, #0x80
	orr r1, r2
	strh r1, [r0]
	bx lr
_0807BF50: .4byte 0x04000128
	thumb_func_end sub_0807BF44

