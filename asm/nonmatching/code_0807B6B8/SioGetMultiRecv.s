	thumb_func_start SioGetMultiRecv
SioGetMultiRecv: @ 0x0807BEF8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x17
	ldr r1, _0807BF04 @ =0x04000120
	add r0, r0, r1
	ldrh r0, [r0]
	bx lr
_0807BF04: .4byte 0x04000120
	thumb_func_end SioGetMultiRecv

