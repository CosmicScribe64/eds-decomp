	thumb_func_start sub_0802F4C8
sub_0802F4C8: @ 0x0802F4C8
	ldr r1, _0802F4DC @ =0x020192E0
	ldr r0, _0802F4E0 @ =0x00001B12
	add r1, r1, r0
	mov r0, #0x1C
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #8
	beq _0802F4E4
	mov r0, #0
	b _0802F4E6
_0802F4DC: .4byte 0x020192E0
_0802F4E0: .4byte 0x00001B12
_0802F4E4:
	mov r0, #1
_0802F4E6:
	bx lr
	thumb_func_end sub_0802F4C8

