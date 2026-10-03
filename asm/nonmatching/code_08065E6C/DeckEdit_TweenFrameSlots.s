	thumb_func_start DeckEdit_TweenFrameSlots
DeckEdit_TweenFrameSlots: @ 0x08066260
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r7, r3, #0
	ldr r3, [sp, #0x1C]
	mov r9, r3
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	add r6, r3, #0
	cmp r2, #1
	beq _08066286
	cmp r2, #2
	beq _0806628E
	b _0806634A
_08066286:
	mov r0, #6
	sub r0, r0, r6
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
_0806628E:
	mov r0, #6
	sub r0, r0, r3
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r1, #1
	beq _080662A0
	cmp r1, #2
	beq _080662F0
	b _0806634A
_080662A0:
	mov r2, #0
	ldr r1, _080662EC @ =0x080875D2
	lsl r0, r3, #1
	add r0, r0, r1
	mov r8, r0
_080662AA:
	lsl r0, r2, #4
	mov r1, r9
	add r4, r1, r0
	ldrb r0, [r4, #0xC]
	add r5, r2, #1
	cmp r0, #0
	beq _080662E0
	mov r2, #0xA
	ldsh r0, [r4, r2]
	mov r3, r8
	ldrh r1, [r3]
	bl MulFix8
	ldrh r1, [r4, #8]
	add r0, r1, r0
	strh r0, [r4, #6]
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #3
	add r0, r7, r0
	ldrh r2, [r4, #0x10]
	add r1, r2, #0
	mul r1, r6
	ldrh r4, [r4, #0xE]
	add r1, r4, r1
	strh r1, [r0, #2]
	strh r1, [r0]
_080662E0:
	lsl r0, r5, #0x18
	lsr r2, r0, #0x18
	cmp r2, #5
	bls _080662AA
	b _0806634A
	.align 2, 0
_080662EC: .4byte gDeckEditEaseCurve
_080662F0:
	mov r2, #0
	ldr r1, _08066358 @ =0x080875D2
	lsl r0, r3, #1
	add r0, r0, r1
	mov r8, r0
_080662FA:
	lsl r0, r2, #4
	mov r3, r9
	add r4, r3, r0
	ldrb r0, [r4, #0xC]
	add r5, r2, #1
	cmp r0, #0
	beq _08066336
	mov r1, #0xA
	ldsh r0, [r4, r1]
	mov r2, r8
	ldrh r1, [r2]
	bl MulFix8
	ldrh r3, [r4, #8]
	add r0, r3, r0
	strh r0, [r4, #6]
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #3
	add r0, r7, r0
	ldrh r2, [r4, #0x10]
	add r1, r2, #0
	mul r1, r6
	ldrh r3, [r4, #0xE]
	add r1, r3, r1
	strh r1, [r0, #2]
	strh r1, [r0]
	mov r1, #0
	ldsh r0, [r0, r1]
	strh r0, [r4, #0xE]
_08066336:
	lsl r0, r5, #0x18
	lsr r2, r0, #0x18
	cmp r2, #5
	bls _080662FA
	mov r2, r9
	ldrb r2, [r2]
	lsl r0, r2, #4
	add r0, r9
	mov r1, #0
	strb r1, [r0, #0xC]
_0806634A:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08066358: .4byte gDeckEditEaseCurve
	thumb_func_end DeckEdit_TweenFrameSlots

