	thumb_func_start sub_0807B4A8
sub_0807B4A8: @ 0x0807B4A8
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r3, _0807B4BC @ =0x04000052
	lsl r2, r0, #8
	mov r1, #0x10
	sub r1, r1, r0
	orr r2, r1
	strh r2, [r3]
	bx lr
	.align 2, 0
_0807B4BC: .4byte 0x04000052
	thumb_func_end sub_0807B4A8

