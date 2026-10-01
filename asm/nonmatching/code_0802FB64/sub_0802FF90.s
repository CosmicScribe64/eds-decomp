	thumb_func_start sub_0802FF90
sub_0802FF90: @ 0x0802FF90
	add r1, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802FFE0
	mov r0, #0xFC
	ldrb r2, [r1, #3]
	and r0, r2
	cmp r0, #0x38
	bne _0802FFE0
	ldrh r2, [r1, #8]
	mov r3, #0xF
	ldrb r1, [r1, #2]
	lsl r1, r1, #0x1F
	add r0, r3, #0
	and r0, r2
	lsr r1, r1, #0x1F
	cmp r0, r1
	beq _0802FFE0
	lsr r1, r2, #8
	add r2, r1, #0
	and r2, r3
	lsr r1, r1, #4
	mov r0, #1
	and r2, r0
	mov r0, #0x94
	mul r0, r1
	ldr r1, _0802FFD8 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802FFDC @ =0x0201930C
	add r0, r0, r1
	ldrb r0, [r0, #6]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	b _0802FFE2
	.align 2, 0
_0802FFD8: .4byte 0x00000D64
_0802FFDC: .4byte 0x0201930C
_0802FFE0:
	mov r0, #0
_0802FFE2:
	bx lr
	thumb_func_end sub_0802FF90

