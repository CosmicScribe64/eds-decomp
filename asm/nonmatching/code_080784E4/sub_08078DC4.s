	thumb_func_start sub_08078DC4
sub_08078DC4: @ 0x08078DC4
	push {r4, r5, r6, lr}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	mov r9, r0
	add r6, r1, #0
	mov r8, r2
	add r4, r3, #0
	ldr r3, [sp, #0x18]
	ldr r5, [sp, #0x1C]
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	mov r0, r8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r8, r0
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	lsl r5, r5, #0x18
	mov r1, r9
	ldrb r1, [r1]
	lsl r0, r1, #8
	mov r2, r9
	ldrb r2, [r2, #1]
	orr r0, r2
	add r1, r6, #1
	mov r2, r8
	add r2, #1
	lsr r5, r5, #0x10
	orr r3, r5
	bl sub_08074E20
	mov r1, r9
	ldrb r1, [r1]
	lsl r0, r1, #8
	mov r2, r9
	ldrb r2, [r2, #1]
	orr r0, r2
	orr r4, r5
	add r1, r6, #0
	mov r2, r8
	add r3, r4, #0
	bl sub_08074E20
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end sub_08078DC4

