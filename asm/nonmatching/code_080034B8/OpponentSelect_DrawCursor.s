	thumb_func_start OpponentSelect_DrawCursor
OpponentSelect_DrawCursor: @ 0x080034B8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r3, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r9, r1
	mov r4, #7
	ldr r6, _0800357C @ =0x0201F7E0
	ldr r5, _08003580 @ =0x081983AC
	add r1, r6, #0
	add r1, #0x10
_080034D4:
	ldrh r0, [r1]
	strh r0, [r1, #2]
	ldrh r0, [r1, #0x10]
	strh r0, [r1, #0x12]
	sub r1, #2
	sub r4, #1
	cmp r4, #0
	bne _080034D4
	add r4, r6, #0
	lsl r2, r3, #2
	add r1, r2, r5
	mov r5, #0
	ldsh r0, [r1, r5]
	add r0, #0x10
	str r0, [r4, #0x2C]
	mov r5, #2
	ldsh r0, [r1, r5]
	str r0, [r4, #0x30]
	ldrh r5, [r4]
	lsl r0, r5, #0x17
	lsr r0, r0, #0x1D
	mov sl, r2
	cmp r3, r0
	beq _0800352A
	mov r0, #7
	and r3, r0
	lsl r1, r3, #6
	ldr r0, _08003584 @ =0xFFFFFE3F
	and r0, r5
	orr r0, r1
	strh r0, [r4]
	mov r0, #0x1E
	ldrb r1, [r4, #1]
	orr r0, r1
	strb r0, [r4, #1]
	ldr r0, [r4]
	ldr r1, _08003588 @ =0xFFFE1FFF
	and r0, r1
	str r0, [r4]
	ldrh r0, [r4, #4]
	str r0, [r4, #0x24]
	ldrh r0, [r4, #0x14]
	str r0, [r4, #0x28]
_0800352A:
	ldrb r5, [r4, #1]
	mov r0, #0x1E
	and r0, r5
	cmp r0, #0
	beq _0800358C
	ldr r0, [r4, #0x24]
	ldr r7, [r4, #0x2C]
	sub r3, r0, r7
	ldr r0, [r4, #0x28]
	ldr r6, [r4, #0x30]
	sub r2, r0, r6
	lsl r1, r5, #0x1B
	lsr r0, r1, #0x1C
	mul r3, r0
	mul r2, r0
	add r0, r3, #0
	cmp r3, #0
	bge _08003550
	add r0, #0xF
_08003550:
	asr r3, r0, #4
	add r0, r2, #0
	cmp r2, #0
	bge _0800355A
	add r0, #0xF
_0800355A:
	asr r2, r0, #4
	add r0, r7, r3
	strh r0, [r4, #4]
	add r0, r6, r2
	strh r0, [r4, #0x14]
	lsr r1, r1, #0x1C
	sub r1, #1
	mov r0, #0xF
	and r1, r0
	lsl r1, r1, #1
	mov r0, #0x1F
	neg r0, r0
	and r0, r5
	orr r0, r1
	strb r0, [r4, #1]
	b _080035BA
	.align 2, 0
_0800357C: .4byte 0x0201F7E0
_08003580: .4byte gOpponentSelectSlotPos
_08003584: .4byte 0xFFFFFE3F
_08003588: .4byte 0xFFFE1FFF
_0800358C:
	ldr r3, _080035F0 @ =0x081983C0
	ldr r1, _080035F4 @ =0x03000040
	ldr r2, _080035F8 @ =0x0000485E
	add r1, r1, r2
	ldrh r4, [r1]
	lsr r0, r4, #1
	mov r2, #0x1F
	and r0, r2
	lsl r0, r0, #2
	add r0, r0, r3
	ldrh r5, [r6, #0x2C]
	ldrh r0, [r0]
	add r0, r5, r0
	strh r0, [r6, #4]
	ldrh r1, [r1]
	lsr r0, r1, #1
	and r0, r2
	lsl r0, r0, #2
	add r0, r0, r3
	ldrh r1, [r6, #0x30]
	ldrh r0, [r0, #2]
	add r0, r1, r0
	strh r0, [r6, #0x14]
_080035BA:
	mov r4, #0
	ldr r2, _080035FC @ =0x04000050
	mov r8, r2
	ldr r7, _08003600 @ =0x04000052
	ldr r5, _08003604 @ =0x0201F7E0
	add r6, r5, #0
	add r6, #0x14
_080035C8:
	cmp r4, #0
	bne _08003608
	mov r0, r9
	cmp r0, #0
	bne _080035D8
	mov r1, r8
	strh r4, [r1]
	strh r4, [r7]
_080035D8:
	ldrh r2, [r5, #0x14]
	lsl r0, r2, #0x10
	ldrh r1, [r5, #4]
	orr r0, r1
	mov r1, #0x81
	lsl r1, r1, #7
	mov r2, #0x82
	lsl r2, r2, #1
	bl AddSprite8bpp
	b _08003638
	.align 2, 0
_080035F0: .4byte gOpponentCursorWobble
_080035F4: .4byte 0x03000040
_080035F8: .4byte 0x0000485E
_080035FC: .4byte 0x04000050
_08003600: .4byte 0x04000052
_08003604: .4byte 0x0201F7E0
_08003608:
	mov r2, r9
	cmp r2, #0
	bne _08003638
	mov r1, #0xF4
	lsl r1, r1, #4
	add r0, r1, #0
	mov r2, r8
	strh r0, [r2]
	ldr r1, _08003664 @ =0x00000808
	add r0, r1, #0
	strh r0, [r7]
	lsl r1, r4, #1
	add r0, r5, #4
	add r1, r1, r0
	ldrh r2, [r6]
	lsl r0, r2, #0x10
	ldrh r1, [r1]
	orr r0, r1
	mov r1, #0x81
	lsl r1, r1, #7
	mov r2, #0x82
	lsl r2, r2, #1
	bl AddSprite8bppAlpha
_08003638:
	add r6, #2
	add r4, #1
	cmp r4, #7
	ble _080035C8
	ldr r1, _08003668 @ =0x081983AC
	add r1, sl
	mov r4, #0
	ldsh r0, [r1, r4]
	mov r5, #2
	ldsh r1, [r1, r5]
	bl OpponentSelect_DrawSelectionRing
	bl OpponentSelect_DrawPageArrows
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08003664: .4byte 0x00000808
_08003668: .4byte gOpponentSelectSlotPos
	thumb_func_end OpponentSelect_DrawCursor

