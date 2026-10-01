	thumb_func_start sub_0807BF54
sub_0807BF54: @ 0x0807BF54
	ldr r0, _0807BF64 @ =0x04000128
	ldrh r1, [r0]
	mov r0, #0x80
	and r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bx lr
	.align 2, 0
_0807BF64: .4byte 0x04000128
	thumb_func_end sub_0807BF54

