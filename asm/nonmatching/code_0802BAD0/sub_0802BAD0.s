	thumb_func_start sub_0802BAD0
sub_0802BAD0: @ 0x0802BAD0
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r5, r0, #0x18
	lsr r4, r1, #0x18
	add r0, r5, #0
	add r1, r4, #0
	bl sub_0800C8BC
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r4, #4
	bgt _0802BB22
	mov r1, #1
	and r1, r5
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	ldr r0, _0802BB28 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802BB2C @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802BB22
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0802BB22
	ldrh r0, [r7]
	add r1, r5, #0
	add r2, r4, #0
	bl sub_0802B1B8
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0802BB30
_0802BB22:
	mov r0, #0
	b _0802BB38
	.align 2, 0
_0802BB28: .4byte 0x00000D64
_0802BB2C: .4byte 0x0201930C
_0802BB30:
	mov r0, #0
	cmp r6, #1
	bne _0802BB38
	mov r0, #1
_0802BB38:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802BAD0
	.align 2, 0

