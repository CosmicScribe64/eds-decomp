	thumb_func_start sub_0802CFD0
sub_0802CFD0: @ 0x0802CFD0
	push {r4, r5, r6, lr}
	sub sp, #0x14
	lsl r2, r2, #0x10
	mov r6, sp
	mov r3, #1
	add r4, r0, #0
	and r4, r3
	ldrb r5, [r6, #2]
	mov r3, #2
	neg r3, r3
	and r3, r5
	orr r3, r4
	strb r3, [r6, #2]
	mov r5, sp
	mov r4, #1
	and r4, r0
	mov r0, #0x94
	mul r0, r1
	ldr r3, _0802D040 @ =0x00000D64
	mul r3, r4
	add r0, r0, r3
	ldr r3, _0802D044 @ =0x0201930C
	add r0, r0, r3
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	strh r0, [r5]
	mov r4, sp
	mov r0, #0x3F
	and r1, r0
	lsl r1, r1, #4
	ldrh r3, [r4, #2]
	ldr r0, _0802D048 @ =0xFFFFFC0F
	and r0, r3
	orr r0, r1
	strh r0, [r4, #2]
	mov r3, sp
	lsr r2, r2, #0xE
	ldrb r1, [r3, #3]
	mov r0, #3
	and r0, r1
	orr r0, r2
	strb r0, [r3, #3]
	mov r0, sp
	ldrh r0, [r0]
	cmp r0, #0
	beq _0802D04C
	mov r0, sp
	mov r1, #0
	mov r2, #0
	bl sub_0802CE38
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0802D04E
	.align 2, 0
_0802D040: .4byte 0x00000D64
_0802D044: .4byte 0x0201930C
_0802D048: .4byte 0xFFFFFC0F
_0802D04C:
	mov r0, #0
_0802D04E:
	add sp, #0x14
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0802CFD0
	.align 2, 0

