	thumb_func_start OffsetNonZeroPixelsAndCopy
OffsetNonZeroPixelsAndCopy: @ 0x08077C50
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r7, r0, #0
	mov r9, r1
	lsl r2, r2, #0x10
	lsr r0, r2, #0x10
	mov r8, r0
	lsl r3, r3, #0x18
	lsr r6, r3, #0x18
	lsl r5, r6, #0x18
	lsl r0, r6, #0x10
	orr r5, r0
	lsl r0, r6, #8
	orr r5, r0
	orr r5, r6
	lsr r2, r2, #0x12
	mov ip, r2
	mov r4, #0
	cmp r4, ip
	bcs _08077CD2
_08077C7C:
	lsl r1, r4, #2
	add r3, r1, r7
	ldr r2, [r3]
	ldrb r0, [r3]
	cmp r0, #0
	beq _08077CAE
	mov r0, #0xFF
	lsl r0, r0, #8
	and r0, r2
	cmp r0, #0
	beq _08077CAE
	mov r0, #0xFF
	lsl r0, r0, #0x10
	and r0, r2
	cmp r0, #0
	beq _08077CAE
	mov r0, #0xFF
	lsl r0, r0, #0x18
	and r0, r2
	cmp r0, #0
	beq _08077CAE
	add r0, r2, r5
	str r0, [r3]
	add r4, #1
	b _08077CCA
_08077CAE:
	mov r2, #0
	add r4, #1
	add r3, r1, r7
_08077CB4:
	add r1, r3, r2
	ldrb r0, [r1]
	cmp r0, #0
	beq _08077CC0
	add r0, r6, r0
	strb r0, [r1]
_08077CC0:
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #3
	bls _08077CB4
_08077CCA:
	lsl r0, r4, #0x10
	lsr r4, r0, #0x10
	cmp r4, ip
	bcc _08077C7C
_08077CD2:
	mov r0, r8
	lsr r2, r0, #2
	add r0, r7, #0
	mov r1, r9
	bl CpuFastSet
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end OffsetNonZeroPixelsAndCopy
	.align 2, 0

