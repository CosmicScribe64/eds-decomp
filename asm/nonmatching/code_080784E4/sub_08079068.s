	thumb_func_start sub_08079068
sub_08079068: @ 0x08079068
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2C
	mov r8, r1
	add r4, r2, #0
	ldr r1, [sp, #0x4C]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0]
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r9, r1
	bl sub_08072584
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r0, _080790F4 @ =0x081D0200
	add r0, r1, r0
	str r0, [sp, #4]
	mov r7, r8
	add r7, #0x20
	mov r2, #0xA
	mov r1, #0
	str r1, [sp, #0xC]
	add r3, r4, #7
	str r3, [sp, #0x18]
	add r4, #8
	str r4, [sp, #0x1C]
	mov r4, r9
	lsl r4, r4, #4
	mov r0, r9
	orr r0, r4
	str r0, [sp, #0x10]
	mov r1, r9
	lsl r0, r1, #8
	ldr r3, [sp, #0x10]
	orr r3, r0
	str r3, [sp, #0x10]
_080790C4:
	mov r4, #0
	str r4, [sp, #8]
_080790C8:
	mov r0, #0x80
	ldr r1, _080790F8 @ =0x02011C20
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _080790FC
	mov r3, r9
	lsl r0, r3, #0xC
	ldr r4, [sp, #0x10]
	orr r0, r4
	lsl r1, r3, #0x10
	orr r0, r1
	lsl r1, r3, #0x14
	orr r0, r1
	lsl r1, r3, #0x18
	orr r0, r1
	lsl r1, r3, #0x1C
	orr r0, r1
	add r6, r0, #0
	add r5, r0, #0
	b _08079110
	.align 2, 0
_080790F4: .4byte gUnk_081D0200
_080790F8: .4byte 0x02011C20
_080790FC:
	mov r1, r8
	ldrh r1, [r1, #2]
	lsl r0, r1, #0x10
	mov r3, r8
	ldrh r5, [r3]
	orr r5, r0
	ldrh r4, [r7, #2]
	lsl r0, r4, #0x10
	ldrh r6, [r7]
	orr r6, r0
_08079110:
	mov r3, #0
	mov r0, r8
	add r0, #2
	str r0, [sp, #0x14]
	add r1, r7, #2
	str r1, [sp, #0x20]
	sub r2, #1
	str r2, [sp, #0x24]
	ldr r4, [sp, #4]
	ldrh r4, [r4]
	mov sl, r4
	ldr r0, [sp, #4]
	ldrb r0, [r0]
	lsl r0, r0, #8
	str r0, [sp, #0x28]
	lsr r0, r4, #8
	ldr r1, [sp, #0x28]
	orr r1, r0
	str r1, [sp, #0x28]
_08079136:
	mov r0, sl
	asr r0, r3
	mov r4, #1
	and r0, r4
	mov r2, r9
	cmp r0, #0
	beq _08079146
	ldr r2, [sp, #0]
_08079146:
	ldr r0, [sp, #0x18]
	sub r1, r0, r3
	lsl r1, r1, #2
	mov r4, #0xF
	mov ip, r4
	mov r0, ip
	lsl r0, r1
	add r4, r5, #0
	bic r4, r0
	add r0, r4, #0
	lsl r2, r1
	add r5, r0, #0
	orr r5, r2
	ldr r0, [sp, #0x1C]
	sub r1, r0, r3
	ldr r0, [sp, #0x28]
	asr r0, r1
	mov r1, #1
	and r0, r1
	mov r2, r9
	cmp r0, #0
	beq _08079174
	ldr r2, [sp, #0]
_08079174:
	lsl r1, r3, #2
	mov r0, ip
	lsl r0, r1
	add r4, r6, #0
	bic r4, r0
	add r0, r4, #0
	lsl r2, r1
	add r6, r0, #0
	orr r6, r2
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #7
	bls _08079136
	mov r0, r8
	strh r5, [r0]
	ldr r1, [sp, #0x14]
	mov r8, r1
	lsr r0, r5, #0x10
	strh r0, [r1]
	mov r3, #2
	add r8, r3
	strh r6, [r7]
	ldr r7, [sp, #0x20]
	lsr r0, r6, #0x10
	strh r0, [r7]
	add r7, #2
	ldr r4, [sp, #0x24]
	lsl r0, r4, #0x18
	lsr r2, r0, #0x18
	cmp r2, #0
	beq _080791E2
	ldr r0, [sp, #4]
	add r0, #2
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	add r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #8]
	cmp r0, #7
	bhi _080791CA
	b _080790C8
_080791CA:
	mov r0, #0xA8
	lsl r0, r0, #2
	add r8, r0
	add r7, r7, r0
	ldr r0, [sp, #0xC]
	add r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0xC]
	cmp r0, #1
	bhi _080791E2
	b _080790C4
_080791E2:
	add sp, #0x2C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_08079068
	.align 2, 0

