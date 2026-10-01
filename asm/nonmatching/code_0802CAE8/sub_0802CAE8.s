	thumb_func_start sub_0802CAE8
sub_0802CAE8: @ 0x0802CAE8
	ldr r2, _0802CB04 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802CB08 @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	mov r0, #0x10
	ldrb r2, [r1, #9]
	orr r0, r2
	strb r0, [r1, #9]
	mov r0, #1
	bx lr
	.align 2, 0
_0802CB04: .4byte 0x020192E4
_0802CB08: .4byte 0x00000D64
	thumb_func_end sub_0802CAE8

