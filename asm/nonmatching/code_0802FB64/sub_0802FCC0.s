	thumb_func_start sub_0802FCC0
sub_0802FCC0: @ 0x0802FCC0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802FCDA
	ldr r2, _0802FCE0 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802FCE4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	bne _0802FCE8
_0802FCDA:
	mov r0, #0
	b _0802FCEA
	.align 2, 0
_0802FCE0: .4byte 0x020192E4
_0802FCE4: .4byte 0x00000D64
_0802FCE8:
	mov r0, #1
_0802FCEA:
	bx lr
	thumb_func_end sub_0802FCC0

