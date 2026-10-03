	thumb_func_start CardListView_HandleInput
CardListView_HandleInput: @ 0x0802AC2C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, _0802AC60 @ =0x0201D810
	ldrb r1, [r2, #5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1E
	ldrh r3, [r2, #6]
	add r5, r3, r0
	lsl r0, r5, #2
	add r1, r2, #0
	add r1, #0xC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldrb r1, [r2, #2]
	add r4, r2, #0
	cmp r1, #2
	beq _0802AC88
	cmp r1, #2
	bgt _0802AC64
	cmp r1, #1
	beq _0802AC6E
	b _0802ACB0
	.align 2, 0
_0802AC60: .4byte 0x0201D810
_0802AC64:
	cmp r1, #3
	beq _0802AC92
	cmp r1, #4
	beq _0802AC98
	b _0802ACB0
_0802AC6E:
	mov r0, #4
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0802AC7C
	b _0802AECA
_0802AC7C:
	mov r0, #0x11
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	b _0802ACA8
_0802AC88:
	mov r1, #0
	mov r2, #0
	bl CardDetail_Init
	b _0802ACA8
_0802AC92:
	bl CardDetail_Run
	b _0802AC9C
_0802AC98:
	bl CardListView_InitScreen
_0802AC9C:
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0802ACA4
	b _0802AECA
_0802ACA4:
	mov r0, #0
	strb r0, [r4, #3]
_0802ACA8:
	ldrb r0, [r4, #2]
	add r0, #1
	strb r0, [r4, #2]
	b _0802AECA
_0802ACB0:
	add r3, r4, #0
	ldrb r2, [r3, #5]
	mov r0, #0x60
	and r0, r2
	cmp r0, #0
	beq _0802ACBE
	b _0802AECA
_0802ACBE:
	ldr r1, _0802ACF0 @ =0x03000040
	mov r0, #0x40
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802AD14
	cmp r5, #0
	ble _0802AD0E
	mov r0, #3
	and r0, r2
	cmp r0, #0
	beq _0802ACF4
	mov r0, #0x61
	neg r0, r0
	and r0, r2
	mov r1, #0x20
	orr r0, r1
	mov r1, #0x1D
	neg r1, r1
	and r0, r1
	mov r1, #0x10
	orr r0, r1
	strb r0, [r3, #5]
	b _0802AD06
	.align 2, 0
_0802ACF0: .4byte 0x03000040
_0802ACF4:
	ldrh r0, [r4, #6]
	sub r0, #1
	strh r0, [r4, #6]
	bl CardListView_DrawPage
	bl CardListView_DrawSelectedInfo
	bl CardListView_DrawSelectedCursorFrame
_0802AD06:
	mov r0, #0
	bl PlaySE
	b _0802AD14
_0802AD0E:
	mov r0, #3
	bl PlaySE
_0802AD14:
	ldr r1, _0802AD54 @ =0x03000040
	mov r0, #0x80
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802AD7C
	ldr r2, _0802AD58 @ =0x0201D810
	mov r3, #0xC3
	lsl r3, r3, #2
	add r0, r2, r3
	ldrh r0, [r0]
	sub r0, #1
	cmp r5, r0
	bge _0802AD76
	ldrb r1, [r2, #5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1E
	cmp r0, #2
	bhi _0802AD5C
	mov r0, #0x61
	neg r0, r0
	and r0, r1
	mov r1, #0x40
	orr r0, r1
	mov r1, #0x1D
	neg r1, r1
	and r0, r1
	mov r1, #0x10
	orr r0, r1
	strb r0, [r2, #5]
	b _0802AD6E
	.align 2, 0
_0802AD54: .4byte 0x03000040
_0802AD58: .4byte 0x0201D810
_0802AD5C:
	ldrh r0, [r2, #6]
	add r0, #1
	strh r0, [r2, #6]
	bl CardListView_DrawPage
	bl CardListView_DrawSelectedInfo
	bl CardListView_DrawSelectedCursorFrame
_0802AD6E:
	mov r0, #0
	bl PlaySE
	b _0802AD7C
_0802AD76:
	mov r0, #3
	bl PlaySE
_0802AD7C:
	ldr r1, _0802AE20 @ =0x03000040
	mov r0, #0x10
	ldrh r2, [r1, #6]
	and r0, r2
	mov r8, r1
	cmp r0, #0
	beq _0802ADBE
	mov r0, #0
	bl PlaySE
	ldr r7, _0802AE24 @ =0x0201D810
	mov r6, #4
	neg r6, r6
	ldrb r3, [r7, #8]
	mov r5, #3
	mov r4, #1
_0802AD9C:
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1E
	add r0, #1
	and r0, r5
	add r2, r6, #0
	and r2, r3
	orr r2, r0
	add r3, r2, #0
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1C
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	asr r1, r0
	and r1, r4
	cmp r1, #0
	beq _0802AD9C
	strb r2, [r7, #8]
_0802ADBE:
	mov r0, #0x20
	mov r3, r8
	ldrh r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	beq _0802ADFE
	mov r0, #0
	bl PlaySE
	ldr r7, _0802AE24 @ =0x0201D810
	mov r6, #4
	neg r6, r6
	ldrb r3, [r7, #8]
	mov r5, #3
	mov r4, #1
_0802ADDC:
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1E
	sub r0, #1
	and r0, r5
	add r2, r6, #0
	and r2, r3
	orr r2, r0
	add r3, r2, #0
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1C
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	asr r1, r0
	and r1, r4
	cmp r1, #0
	beq _0802ADDC
	strb r2, [r7, #8]
_0802ADFE:
	mov r2, #2
	add r0, r2, #0
	mov r1, r8
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802AE28
	ldr r0, _0802AE24 @ =0x0201D810
	ldrb r0, [r0, #8]
	lsl r1, r0, #0x1A
	lsr r1, r1, #0x1C
	add r0, r2, #0
	and r0, r1
	cmp r0, #0
	beq _0802AE28
	mov r0, #2
	b _0802AEC2
_0802AE20: .4byte 0x03000040
_0802AE24: .4byte 0x0201D810
_0802AE28:
	mov r5, #1
	add r0, r5, #0
	mov r2, r8
	ldrh r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0802AECA
	ldr r2, _0802AE4C @ =0x0201D810
	ldrb r3, [r2, #8]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1E
	cmp r0, #1
	beq _0802AEB4
	cmp r0, #1
	bgt _0802AE50
	cmp r0, #0
	beq _0802AE5A
	b _0802AECA
_0802AE4C: .4byte 0x0201D810
_0802AE50:
	cmp r0, #2
	beq _0802AEB8
	cmp r0, #3
	beq _0802AEC0
	b _0802AECA
_0802AE5A:
	ldrb r1, [r2]
	mov r0, #0xE0
	and r0, r1
	cmp r0, #0x60
	bne _0802AEA0
	lsl r0, r1, #0x1E
	lsr r4, r0, #0x1F
	ldrb r1, [r2, #5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1E
	ldrh r2, [r2, #6]
	add r0, r2, r0
	ldr r3, _0802AE98 @ =0x020192E4
	lsl r0, r0, #1
	add r1, r4, #0
	and r1, r5
	ldr r2, _0802AE9C @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	sub r2, #0xA0
	add r3, r3, r2
	add r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #2
	bne _0802AEA0
	cmp r4, #0
	beq _0802AEA0
	mov r0, #3
	bl PlaySE
	b _0802AECA
_0802AE98: .4byte 0x020192E4
_0802AE9C: .4byte 0x00000D64
_0802AEA0:
	mov r0, #1
	bl PlaySE
	ldr r1, _0802AEB0 @ =0x0201D810
	mov r0, #1
	strb r0, [r1, #2]
	b _0802AECA
	.align 2, 0
_0802AEB0: .4byte 0x0201D810
_0802AEB4:
	mov r0, #2
	b _0802AEC2
_0802AEB8:
	mov r0, #1
	bl PlaySE
	b _0802AECA
_0802AEC0:
	mov r0, #1
_0802AEC2:
	bl PlaySE
	mov r0, #1
	b _0802AECC
_0802AECA:
	mov r0, #0
_0802AECC:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end CardListView_HandleInput
	.align 2, 0

