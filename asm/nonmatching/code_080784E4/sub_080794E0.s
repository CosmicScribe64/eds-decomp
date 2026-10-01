	thumb_func_start sub_080794E0
sub_080794E0: @ 0x080794E0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	str r3, [sp, #4]
	ldr r3, [sp, #0x30]
	ldr r4, [sp, #0x34]
	ldr r5, [sp, #0x38]
	ldr r6, [sp, #0x3C]
	ldr r7, [sp, #0x40]
	mov r8, r7
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov sl, r1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r9, r3
	mov r0, r9
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	str r4, [sp, #8]
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	str r5, [sp, #0xC]
	lsl r6, r6, #0x10
	lsr r6, r6, #0x10
	mov r1, r8
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r8, r1
	cmp r2, #0
	beq _08079532
	cmp r2, #1
	beq _08079584
	b _080795FA
_08079532:
	mov r5, #0
	cmp r5, sl
	bcs _080795FA
_08079538:
	add r0, r7, #0
	mov r1, #0xA
	bl __umodsi3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r7, #0
	mov r1, #0xA
	bl __udivsi3
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	add r0, r6, r4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, r9
	sub r2, r1, #1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r9, r2
	ldr r3, [sp, #8]
	lsl r2, r3, #5
	add r1, r1, r2
	lsl r1, r1, #1
	ldr r2, [sp, #4]
	add r1, r2, r1
	mov r2, #1
	str r2, [sp, #0]
	ldr r2, [sp, #0xC]
	mov r3, r8
	bl sub_080792A0
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, sl
	bcc _08079538
	b _080795FA
_08079584:
	cmp r7, #0
	bne _080795A2
	ldr r3, [sp, #8]
	lsl r1, r3, #5
	add r1, r0, r1
	lsl r1, r1, #1
	ldr r7, [sp, #4]
	add r1, r7, r1
	str r2, [sp, #0]
	add r0, r6, #0
	ldr r2, [sp, #0xC]
	mov r3, r8
	bl sub_080792A0
	b _080795FA
_080795A2:
	mov r5, #0
	cmp r5, sl
	bcs _080795FA
_080795A8:
	add r0, r7, #0
	mov r1, #0xA
	bl __umodsi3
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r7, #0
	mov r1, #0xA
	bl __udivsi3
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	cmp r4, #0
	bne _080795C8
	cmp r7, #0
	beq _080795FA
_080795C8:
	add r0, r6, r4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, r9
	sub r2, r1, #1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r9, r2
	ldr r3, [sp, #8]
	lsl r2, r3, #5
	add r1, r1, r2
	lsl r1, r1, #1
	ldr r2, [sp, #4]
	add r1, r2, r1
	mov r2, #1
	str r2, [sp, #0]
	ldr r2, [sp, #0xC]
	mov r3, r8
	bl sub_080792A0
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, sl
	bcc _080795A8
_080795FA:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_080794E0
	.align 2, 0

