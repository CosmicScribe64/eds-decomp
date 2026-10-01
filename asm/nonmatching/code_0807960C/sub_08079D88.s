	thumb_func_start sub_08079D88
sub_08079D88: @ 0x08079D88
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	add r7, r0, #0
	mov sl, r3
	ldr r0, [sp, #0x44]
	ldr r3, [sp, #0x48]
	ldr r4, [sp, #0x4C]
	ldr r5, [sp, #0x50]
	ldr r6, [sp, #0x54]
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r9, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0x10]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x14]
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	str r3, [sp, #0x18]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov r8, r4
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	mov r0, #0
	str r0, [sp, #0x1C]
	ldrb r0, [r7]
	cmp r0, #0
	beq _08079E40
	add r0, r2, #0
	add r0, #1
	lsl r0, r0, #0x10
	str r0, [sp, #0x20]
_08079DDA:
	ldrb r1, [r7]
	lsl r0, r1, #8
	ldrb r2, [r7, #1]
	orr r0, r2
	mov r3, r8
	lsr r1, r3, #1
	ldr r2, [sp, #0x1C]
	add r4, r2, #0
	mul r4, r1
	add r1, r4, #1
	add r1, r9
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r3, [sp, #0x18]
	str r3, [sp, #0]
	mov r2, r8
	str r2, [sp, #4]
	str r5, [sp, #8]
	str r6, [sp, #0xC]
	ldr r3, [sp, #0x20]
	lsr r2, r3, #0x10
	mov r3, sl
	bl sub_08079A48
	ldrb r1, [r7]
	lsl r0, r1, #8
	ldrb r2, [r7, #1]
	orr r0, r2
	add r4, r9
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	ldr r3, [sp, #0x14]
	str r3, [sp, #0]
	mov r1, r8
	str r1, [sp, #4]
	str r5, [sp, #8]
	str r6, [sp, #0xC]
	add r1, r4, #0
	ldr r2, [sp, #0x10]
	mov r3, sl
	bl sub_08079A48
	add r7, #2
	ldr r0, [sp, #0x1C]
	add r0, #2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x1C]
	ldrb r0, [r7]
	cmp r0, #0
	bne _08079DDA
_08079E40:
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08079D88

