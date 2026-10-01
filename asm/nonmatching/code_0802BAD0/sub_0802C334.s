	thumb_func_start sub_0802C334
sub_0802C334: @ 0x0802C334
	push {r4, r5, r6, r7, lr}
	add r3, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r6, r0, #0x18
	lsr r5, r1, #0x18
	mov r1, #1
	and r1, r6
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r0, _0802C3A8 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802C3AC @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	add r4, r1, #0
	ldrb r7, [r3, #2]
	lsl r0, r7, #0x1F
	lsr r0, r0, #0x1F
	cmp r6, r0
	bne _0802C3B8
	cmp r5, #4
	bgt _0802C3B8
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0802C3B8
	cmp r1, #0
	beq _0802C3B8
	ldrh r0, [r3]
	add r1, r6, #0
	add r2, r5, #0
	bl sub_0802B1B8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802C3B8
	ldr r0, _0802C3B0 @ =0x000007FF
	and r4, r0
	lsl r0, r4, #1
	ldr r1, _0802C3B4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0x15
	beq _0802C3A4
	add r0, r6, #0
	add r1, r5, #0
	bl sub_0800C8BC
	cmp r0, #0x13
	bne _0802C3B8
_0802C3A4:
	mov r0, #1
	b _0802C3BA
_0802C3A8: .4byte 0x00000D64
_0802C3AC: .4byte 0x0201930C
_0802C3B0: .4byte 0x000007FF
_0802C3B4: .4byte gUnk_08622AB4
_0802C3B8:
	mov r0, #0
_0802C3BA:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802C334

