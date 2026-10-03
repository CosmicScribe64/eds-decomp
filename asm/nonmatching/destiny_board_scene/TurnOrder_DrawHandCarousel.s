	thumb_func_start TurnOrder_DrawHandCarousel
TurnOrder_DrawHandCarousel: @ 0x08027D34
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x38
	str r0, [sp, #0x28]
	str r1, [sp, #0x2C]
	ldr r4, [sp, #0x58]
	ldr r0, [sp, #0x5C]
	ldr r1, [sp, #0x60]
	lsl r2, r2, #0x18
	lsr r7, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x30]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r0, #0xFF
	sub r0, r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x34]
	mov r2, #0
	neg r5, r4
	lsl r4, r4, #1
_08027D70:
	cmp r2, r3
	bne _08027D7E
	mov r0, sp
	add r0, r0, r2
	add r0, #0x24
	strb r5, [r0]
	b _08027D86
_08027D7E:
	mov r0, sp
	add r0, r0, r2
	add r0, #0x24
	strb r4, [r0]
_08027D86:
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #2
	bls _08027D70
	add r0, r7, r1
	add r6, r0, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r6, r0
	sub r0, #0x40
	cmp r0, #0x7F
	bls _08027DAA
	mov r0, #1
	ldr r1, [sp, #0x30]
	and r0, r1
	cmp r0, #0
	beq _08027DB2
_08027DAA:
	mov r2, #1
	mov r8, r2
	mov r9, r2
	b _08027DB8
_08027DB2:
	mov r0, #0
	mov r8, r0
	mov r9, r0
_08027DB8:
	ldr r1, _08027E38 @ =0x08087BA4
	mov sl, r1
	add r4, r6, #0
	asr r4, r4, #8
	lsl r4, r4, #8
	sub r4, r6, r4
	lsl r0, r4, #1
	add r0, sl
	mov r2, #0
	ldsh r0, [r0, r2]
	mov r1, #0xC0
	lsl r1, r1, #6
	bl MulFix8
	add r5, r0, #0
	asr r5, r5, #8
	add r5, #0x58
	add r4, #0x40
	lsl r4, r4, #1
	add r4, sl
	mov r1, #0
	ldsh r0, [r4, r1]
	mov r1, #0x80
	lsl r1, r1, #5
	bl MulFix8
	add r3, r0, #0
	asr r3, r3, #8
	add r3, #0x32
	add r0, sp, #0x24
	ldrb r0, [r0, #2]
	add r3, r3, r0
	ldr r2, [sp, #0x28]
	ldrh r1, [r2, #4]
	mov r0, #0x20
	str r0, [sp, #0]
	mov r2, #0x40
	str r2, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	ldr r2, [sp, #0x2C]
	ldrb r0, [r2, #2]
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _08027E3C @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, r8
	add r2, r5, #0
	bl OamListAddSprite
	add r2, r0, #0
	ldr r1, [r2]
	mov r0, r9
	cmp r0, #0
	beq _08027E44
	ldr r0, _08027E40 @ =0x02000700
	b _08027E46
	.align 2, 0
_08027E38: .4byte gSineTable
_08027E3C: .4byte 0x02020310
_08027E40: .4byte 0x02000700
_08027E44:
	ldr r0, _08027EB4 @ =0x02000300
_08027E46:
	orr r1, r0
	str r1, [r2]
	ldr r5, _08027EB8 @ =0x02020310
	ldr r2, _08027EBC @ =0x00000634
	add r1, r5, r2
	mov r0, #0x80
	lsl r0, r0, #8
	strh r0, [r1]
	ldr r4, _08027EC0 @ =0x08087BA4
	add r1, r6, #0
	add r0, r1, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r6, r0
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r4
	mov r1, #0
	ldsh r4, [r0, r1]
	mov r0, #0x40
	add r1, r4, #0
	bl MulFix8
	add r0, #0xC0
	mov r2, #0xC6
	lsl r2, r2, #3
	add r1, r5, r2
	strh r0, [r1]
	mov r0, #0x40
	add r1, r4, #0
	bl MulFix8
	add r0, #0xC0
	ldr r2, _08027EC4 @ =0x00000632
	add r1, r5, r2
	strh r0, [r1]
	ldr r1, [sp, #0x34]
	add r0, r7, r1
	add r6, r0, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r6, r0
	sub r0, #0x40
	cmp r0, #0x7F
	bls _08027EAA
	mov r0, #1
	ldr r2, [sp, #0x30]
	and r0, r2
	cmp r0, #0
	beq _08027EC8
_08027EAA:
	mov r0, #1
	mov r8, r0
	mov r9, r0
	b _08027ECE
	.align 2, 0
_08027EB4: .4byte 0x02000300
_08027EB8: .4byte 0x02020310
_08027EBC: .4byte 0x00000634
_08027EC0: .4byte gSineTable
_08027EC4: .4byte 0x00000632
_08027EC8:
	mov r1, #0
	mov r8, r1
	mov r9, r1
_08027ECE:
	ldr r2, _08027F4C @ =0x08087BA4
	mov sl, r2
	add r4, r6, #0
	asr r4, r4, #8
	lsl r4, r4, #8
	sub r4, r6, r4
	lsl r0, r4, #1
	add r0, sl
	mov r1, #0
	ldsh r0, [r0, r1]
	mov r1, #0xC0
	lsl r1, r1, #6
	bl MulFix8
	add r5, r0, #0
	asr r5, r5, #8
	add r5, #0x58
	add r4, #0x40
	lsl r4, r4, #1
	add r4, sl
	mov r2, #0
	ldsh r0, [r4, r2]
	mov r1, #0x80
	lsl r1, r1, #5
	bl MulFix8
	add r3, r0, #0
	asr r3, r3, #8
	add r3, #0x32
	add r0, sp, #0x24
	ldrb r0, [r0, #1]
	add r3, r3, r0
	ldr r0, [sp, #0x28]
	ldrh r1, [r0, #2]
	mov r2, #0x20
	str r2, [sp, #0]
	mov r0, #0x40
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	ldr r2, [sp, #0x2C]
	ldrb r0, [r2, #1]
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _08027F50 @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, r8
	add r2, r5, #0
	bl OamListAddSprite
	add r2, r0, #0
	ldr r1, [r2]
	mov r0, r9
	cmp r0, #0
	beq _08027F58
	ldr r0, _08027F54 @ =0x04000700
	b _08027F5A
_08027F4C: .4byte gSineTable
_08027F50: .4byte 0x02020310
_08027F54: .4byte 0x04000700
_08027F58:
	ldr r0, _08027FC4 @ =0x04000300
_08027F5A:
	orr r1, r0
	str r1, [r2]
	ldr r5, _08027FC8 @ =0x02020310
	ldr r2, _08027FCC @ =0x0000064C
	add r1, r5, r2
	mov r0, #0x80
	lsl r0, r0, #8
	strh r0, [r1]
	ldr r4, _08027FD0 @ =0x08087BA4
	add r1, r6, #0
	add r0, r1, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r6, r0
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r4
	mov r1, #0
	ldsh r4, [r0, r1]
	mov r0, #0x40
	add r1, r4, #0
	bl MulFix8
	add r0, #0xC0
	mov r2, #0xC9
	lsl r2, r2, #3
	add r1, r5, r2
	strh r0, [r1]
	mov r0, #0x40
	add r1, r4, #0
	bl MulFix8
	add r0, #0xC0
	ldr r2, _08027FD4 @ =0x0000064A
	add r1, r5, r2
	strh r0, [r1]
	add r0, r7, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r7, r0
	sub r0, #0x40
	cmp r0, #0x7F
	bls _08027FBA
	mov r0, #1
	ldr r1, [sp, #0x30]
	and r0, r1
	cmp r0, #0
	beq _08027FD8
_08027FBA:
	mov r2, #1
	mov r8, r2
	mov r9, r2
	b _08027FDE
	.align 2, 0
_08027FC4: .4byte 0x04000300
_08027FC8: .4byte 0x02020310
_08027FCC: .4byte 0x0000064C
_08027FD0: .4byte gSineTable
_08027FD4: .4byte 0x0000064A
_08027FD8:
	mov r0, #0
	mov r8, r0
	mov r9, r0
_08027FDE:
	ldr r6, _0802805C @ =0x08087BA4
	add r4, r7, #0
	asr r4, r4, #8
	lsl r4, r4, #8
	sub r4, r7, r4
	lsl r0, r4, #1
	add r0, r0, r6
	mov r1, #0
	ldsh r0, [r0, r1]
	mov r1, #0xC0
	lsl r1, r1, #6
	bl MulFix8
	add r5, r0, #0
	asr r5, r5, #8
	add r5, #0x58
	add r4, #0x40
	lsl r4, r4, #1
	add r4, r4, r6
	mov r2, #0
	ldsh r0, [r4, r2]
	mov r1, #0x80
	lsl r1, r1, #5
	bl MulFix8
	add r3, r0, #0
	asr r3, r3, #8
	add r3, #0x32
	add r0, sp, #0x24
	ldrb r0, [r0]
	add r3, r0, r3
	ldr r0, [sp, #0x28]
	ldrh r1, [r0]
	mov r2, #0x20
	str r2, [sp, #0]
	mov r0, #0x40
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	ldr r2, [sp, #0x2C]
	ldrb r0, [r2]
	str r0, [sp, #0xC]
	mov r0, #0x80
	lsl r0, r0, #2
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _08028060 @ =0x02020310
	str r0, [sp, #0x20]
	mov r0, r8
	add r2, r5, #0
	bl OamListAddSprite
	add r2, r0, #0
	ldr r1, [r2]
	mov r0, r9
	cmp r0, #0
	beq _08028064
	mov r0, #0xE0
	lsl r0, r0, #3
	b _08028068
_0802805C: .4byte gSineTable
_08028060: .4byte 0x02020310
_08028064:
	mov r0, #0xC0
	lsl r0, r0, #2
_08028068:
	orr r1, r0
	str r1, [r2]
	ldr r5, _080280C0 @ =0x02020310
	ldr r2, _080280C4 @ =0x0000061C
	add r1, r5, r2
	mov r0, #0x80
	lsl r0, r0, #8
	strh r0, [r1]
	ldr r2, _080280C8 @ =0x08087BA4
	add r0, r7, #0
	asr r0, r0, #8
	lsl r0, r0, #8
	sub r0, r7, r0
	add r0, #0x40
	lsl r0, r0, #1
	add r0, r0, r2
	mov r1, #0
	ldsh r4, [r0, r1]
	mov r0, #0x40
	add r1, r4, #0
	bl MulFix8
	add r0, #0xC0
	mov r2, #0xC3
	lsl r2, r2, #3
	add r1, r5, r2
	strh r0, [r1]
	mov r0, #0x40
	add r1, r4, #0
	bl MulFix8
	add r0, #0xC0
	ldr r2, _080280CC @ =0x0000061A
	add r1, r5, r2
	strh r0, [r1]
	add sp, #0x38
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080280C0: .4byte 0x02020310
_080280C4: .4byte 0x0000061C
_080280C8: .4byte gSineTable
_080280CC: .4byte 0x0000061A
	thumb_func_end TurnOrder_DrawHandCarousel

