	thumb_func_start sub_0802E9E0
sub_0802E9E0: @ 0x0802E9E0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	ldr r3, _0802EA00 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802EA04 @ =0x00000D64
	mul r0, r1
	add r0, r0, r3
	ldrb r0, [r0, #2]
	sub r2, r0, r2
	cmp r2, #1
	ble _0802EA08
	mov r0, #1
	b _0802EA0A
	.align 2, 0
_0802EA00: .4byte 0x020192E4
_0802EA04: .4byte 0x00000D64
_0802EA08:
	mov r0, #0
_0802EA0A:
	bx lr
	thumb_func_end sub_0802E9E0

