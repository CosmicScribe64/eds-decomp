	thumb_func_start sub_0803A378
sub_0803A378: @ 0x0803A378
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r4, r0, #0
	mov r5, #0
	mov r0, #1
	mov r8, r0
	ldr r7, _0803A408 @ =0x00000D64
	ldr r6, _0803A40C @ =0x0201930C
_0803A38A:
	ldrb r1, [r4, #2]
	lsl r2, r1, #0x1F
	lsr r0, r2, #0x1F
	mov r1, r8
	and r1, r0
	mov r0, #0x94
	add r3, r5, #0
	mul r3, r0
	add r0, r1, #0
	mul r0, r7
	add r0, r3, r0
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803A3F4
	lsr r1, r2, #0x1F
	mov r0, r8
	and r0, r1
	add r1, r0, #0
	mul r1, r7
	add r1, r3, r1
	add r1, r1, r6
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803A3F4
	lsr r0, r2, #0x1F
	add r1, r5, #0
	bl sub_0800C8BC
	cmp r0, #7
	bne _0803A3F4
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	bl sub_0802B28C
	cmp r0, #0
	beq _0803A3F4
	ldrb r0, [r4, #2]
	lsl r3, r0, #0x1F
	lsr r0, r3, #0x1F
	ldrh r1, [r4]
	add r3, r0, #0
	lsl r2, r5, #0x18
	lsr r2, r2, #0x10
	orr r2, r3
	mov r3, #3
	bl sub_08017AB4
_0803A3F4:
	add r5, #1
	cmp r5, #4
	ble _0803A38A
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803A408: .4byte 0x00000D64
_0803A40C: .4byte 0x0201930C
	thumb_func_end sub_0803A378

