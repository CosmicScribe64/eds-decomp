	thumb_func_start sub_08079404
sub_08079404: @ 0x08079404
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	add r6, r0, #0
	mov r9, r1
	add r1, r2, #0
	ldr r0, [sp, #0x20]
	ldr r2, [sp, #0x24]
	ldr r5, [sp, #0x28]
	ldr r4, [sp, #0x2C]
	lsl r3, r3, #0x10
	lsr r7, r3, #0x10
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r8, r0
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	str r4, [sp, #0]
	add r0, r6, #0
	add r3, r5, #0
	bl sub_080791F4
	mov r1, #0
	b _08079446
_08079440:
	add r0, r1, #1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
_08079446:
	ldrb r0, [r6]
	add r6, #1
	cmp r0, #0
	bne _08079440
	add r0, r1, #1
	asr r0, r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0]
	add r0, r7, #0
	mov r1, r9
	mov r2, r8
	add r3, r4, #0
	bl sub_080792A0
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08079404
	.align 2, 0

