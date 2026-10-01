	thumb_func_start sub_0802BB94
sub_0802BB94: @ 0x0802BB94
	lsl r1, r1, #0x10
	lsl r2, r1, #8
	lsr r2, r2, #0x18
	lsr r3, r1, #0x18
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	cmp r2, r0
	beq _0802BBD8
	mov r1, #1
	and r1, r2
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _0802BBD0 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802BBD4 @ =0x0201930C
	add r1, r2, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802BBD8
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0802BBD8
	mov r0, #1
	b _0802BBDA
_0802BBD0: .4byte 0x00000D64
_0802BBD4: .4byte 0x0201930C
_0802BBD8:
	mov r0, #0
_0802BBDA:
	bx lr
	thumb_func_end sub_0802BB94

