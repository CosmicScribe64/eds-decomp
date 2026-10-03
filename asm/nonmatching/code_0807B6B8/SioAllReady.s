	thumb_func_start SioAllReady
SioAllReady: @ 0x0807BF1C
	ldr r0, _0807BF2C @ =0x04000128
	ldrh r1, [r0]
	mov r0, #8
	and r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bx lr
	.align 2, 0
_0807BF2C: .4byte 0x04000128
	thumb_func_end SioAllReady

