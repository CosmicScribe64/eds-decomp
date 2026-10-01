	thumb_func_start sub_0802BF38
sub_0802BF38: @ 0x0802BF38
	lsl r1, r1, #0x10
	lsl r2, r1, #8
	lsr r2, r2, #0x18
	lsr r3, r1, #0x18
	mov r0, #1
	and r2, r0
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	ldr r0, _0802BF90 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0802BF94 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, r0, #0
	cmp r2, #0
	beq _0802BFA0
	sub r0, r3, #5
	cmp r0, #5
	bhi _0802BFA0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802BFA0
	ldr r0, _0802BF98 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _0802BF9C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	mov r1, #0
	cmp r0, #0x15
	bne _0802BF8C
	mov r1, #1
_0802BF8C:
	add r0, r1, #0
	b _0802BFA2
_0802BF90: .4byte 0x00000D64
_0802BF94: .4byte 0x0201930C
_0802BF98: .4byte 0x000007FF
_0802BF9C: .4byte gUnk_08621DE0
_0802BFA0:
	mov r0, #0
_0802BFA2:
	bx lr
	thumb_func_end sub_0802BF38

