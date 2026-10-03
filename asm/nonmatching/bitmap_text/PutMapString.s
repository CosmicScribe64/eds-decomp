	thumb_func_start PutMapString
PutMapString: @ 0x0807960C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1C
	add r5, r0, #0
	mov sl, r1
	ldr r0, [sp, #0x3C]
	ldr r1, [sp, #0x40]
	ldr r4, [sp, #0x44]
	ldr r6, [sp, #0x48]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0x18]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0xC]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r9, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	str r4, [sp, #0x10]
	mov r4, #0
	mov r0, #1
	ldrb r1, [r6]
	orr r0, r1
	strb r0, [r6]
	ldrb r0, [r5]
	cmp r0, #0
	beq _080796EE
	ldr r7, [sp, #0x10]
	cmp r4, r7
	bcs _080796EE
	ldr r0, [sp, #0xC]
	mov r1, #0x1F
	and r0, r1
	lsl r0, r0, #0x10
	str r0, [sp, #0x14]
_08079662:
	ldrb r0, [r5]
	cmp r0, #0xA
	beq _08079678
	cmp r0, #0xD
	bne _08079682
	mov r7, #2
	neg r7, r7
	add r0, r7, #0
	ldrb r1, [r6]
	and r0, r1
	b _0807967E
_08079678:
	mov r0, #1
	ldrb r7, [r6]
	orr r0, r7
_0807967E:
	strb r0, [r6]
	add r5, #1
_08079682:
	ldrb r0, [r5]
	add r0, #0x22
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #1
	bhi _080796B6
	ldrb r1, [r5]
	add r5, #1
	ldr r2, [sp, #0x18]
	sub r2, #1
	mov r0, #0x1F
	and r2, r0
	ldr r3, [sp, #0xC]
	sub r3, #1
	and r3, r0
	lsl r3, r3, #0x10
	mov r7, r9
	str r7, [sp, #0]
	mov r0, r8
	str r0, [sp, #4]
	str r6, [sp, #8]
	mov r0, sl
	lsr r3, r3, #0x10
	bl PutMapChar
	b _080796E2
_080796B6:
	ldrb r1, [r5]
	add r5, #1
	ldr r2, [sp, #0x18]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x18]
	mov r7, #0x1F
	and r2, r7
	mov r0, r9
	str r0, [sp, #0]
	mov r7, r8
	str r7, [sp, #4]
	str r6, [sp, #8]
	mov r0, sl
	ldr r7, [sp, #0x14]
	lsr r3, r7, #0x10
	bl PutMapChar
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
_080796E2:
	ldrb r0, [r5]
	cmp r0, #0
	beq _080796EE
	ldr r0, [sp, #0x10]
	cmp r4, r0
	bcc _08079662
_080796EE:
	add sp, #0x1C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end PutMapString
	.align 2, 0

