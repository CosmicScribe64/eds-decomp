	thumb_func_start sub_0802F4E8
sub_0802F4E8: @ 0x0802F4E8
	mov r3, #0
	ldr r2, _0802F50C @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	eor r0, r1
	ldr r1, _0802F510 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldr r1, _0802F514 @ =0x00000BB8
	ldrh r0, [r0]
	cmp r0, r1
	bhi _0802F506
	mov r3, #1
_0802F506:
	add r0, r3, #0
	bx lr
	.align 2, 0
_0802F50C: .4byte 0x020192E4
_0802F510: .4byte 0x00000D64
_0802F514: .4byte 0x00000BB8
	thumb_func_end sub_0802F4E8

