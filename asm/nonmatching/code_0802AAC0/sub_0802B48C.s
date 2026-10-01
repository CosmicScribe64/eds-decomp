	thumb_func_start sub_0802B48C
sub_0802B48C: @ 0x0802B48C
	lsl r1, r1, #0x10
	lsl r2, r1, #8
	lsr r2, r2, #0x18
	lsr r3, r1, #0x18
	mov r0, #1
	and r2, r0
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	ldr r0, _0802B4C0 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0802B4C4 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, r0, #0
	cmp r2, #0
	beq _0802B4BA
	sub r0, r3, #5
	cmp r0, #5
	bls _0802B4C8
_0802B4BA:
	mov r0, #0
	b _0802B4FE
	.align 2, 0
_0802B4C0: .4byte 0x00000D64
_0802B4C4: .4byte 0x0201930C
_0802B4C8:
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802B4FC
	ldr r0, _0802B4F4 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0802B4F8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	mov r1, #0
	cmp r0, #0x15
	bne _0802B4EE
	mov r1, #1
_0802B4EE:
	add r0, r1, #0
	b _0802B4FE
	.align 2, 0
_0802B4F4: .4byte 0x000007FF
_0802B4F8: .4byte gUnk_08621DE0
_0802B4FC:
	mov r0, #1
_0802B4FE:
	bx lr
	thumb_func_end sub_0802B48C

