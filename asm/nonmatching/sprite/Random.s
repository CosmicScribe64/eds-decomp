	thumb_func_start Random
Random: @ 0x08076F9C
	ldr r2, _08076FB8 @ =0x03000040
	ldr r1, [r2]
	ldr r0, _08076FBC @ =0x000343FD
	mul r0, r1
	ldr r1, _08076FC0 @ =0x00269EC3
	add r0, r0, r1
	lsl r1, r0, #0x10
	lsr r0, r0, #0x10
	orr r0, r1
	str r0, [r2]
	lsl r0, r0, #1
	lsr r0, r0, #0x11
	bx lr
	.align 2, 0
_08076FB8: .4byte 0x03000040
_08076FBC: .4byte 0x000343FD
_08076FC0: .4byte 0x00269EC3
	thumb_func_end Random

