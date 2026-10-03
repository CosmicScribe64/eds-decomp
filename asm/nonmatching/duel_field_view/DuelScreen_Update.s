	thumb_func_start DuelScreen_Update
DuelScreen_Update: @ 0x08024440
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r0, #0
	mov r9, r0
	ldr r1, _08024674 @ =0x0201CFB0
	mov r0, #4
	ldrb r2, [r1]
	and r0, r2
	add r6, r1, #0
	cmp r0, #0
	beq _0802452A
	ldr r0, _08024678 @ =0x00000808
	add r4, r6, r0
	mov r0, #1
	ldrb r1, [r4]
	and r0, r1
	cmp r0, #0
	beq _0802449A
	ldr r0, _0802467C @ =0x060091C0
	add r1, r6, #0
	add r1, #8
	mov r2, #0x80
	lsl r2, r2, #4
	bl CopyDoubleWords
	mov r0, #2
	ldrb r2, [r4]
	and r0, r2
	cmp r0, #0
	beq _08024490
	bl TextCellsResetMap
	mov r0, #3
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
_08024490:
	mov r0, #2
	neg r0, r0
	ldrb r2, [r4]
	and r0, r2
	strb r0, [r4]
_0802449A:
	add r5, r6, #0
	mov r7, #4
	add r0, r7, #0
	ldrb r4, [r5]
	and r0, r4
	cmp r0, #0
	beq _0802452A
	ldr r4, _08024680 @ =0x0201AE60
	mov r0, #2
	ldrb r1, [r4]
	and r0, r1
	cmp r0, #0
	beq _080244CC
	ldr r0, _08024684 @ =0x06009AE0
	add r1, r4, #0
	add r1, #0x24
	mov r2, #0xD8
	lsl r2, r2, #5
	bl CopyDoubleWords
	mov r0, #3
	neg r0, r0
	ldrb r2, [r4]
	and r0, r2
	strb r0, [r4]
_080244CC:
	add r0, r7, #0
	ldrb r4, [r5]
	and r0, r4
	cmp r0, #0
	beq _0802452A
	ldrb r3, [r5, #7]
	lsl r0, r3, #0x1C
	lsr r0, r0, #0x1C
	cmp r0, #0
	ble _08024516
	ldrb r1, [r5, #6]
	ldrb r4, [r5, #5]
	sub r2, r1, r4
	sub r1, r0, #1
	mov r0, #0xF
	and r1, r0
	mov r0, #0x10
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r5, #7]
	ldr r1, _08024688 @ =0x081A429C
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1B
	add r0, r0, r1
	ldrh r0, [r0]
	mul r2, r0
	add r0, r2, #0
	cmp r2, #0
	bge _0802450A
	add r0, #0xFF
_0802450A:
	asr r2, r0, #8
	ldrb r1, [r6, #5]
	add r0, r1, r2
	strb r0, [r6, #4]
	mov r2, #1
	mov r9, r2
_08024516:
	ldr r1, _0802468C @ =0x03000040
	ldr r3, _08024674 @ =0x0201CFB0
	ldrb r2, [r3, #4]
	ldr r4, _08024690 @ =0x00004422
	add r0, r1, r4
	strh r2, [r0]
	ldrb r0, [r3, #4]
	ldr r6, _08024694 @ =0x00004424
	add r1, r1, r6
	strh r0, [r1]
_0802452A:
	ldr r4, _08024674 @ =0x0201CFB0
	ldr r0, _08024678 @ =0x00000808
	add r0, r0, r4
	mov r8, r0
	ldrb r1, [r0]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _0802454A
	mov r0, #5
	neg r0, r0
	and r0, r1
	mov r1, r8
	strb r0, [r1]
	bl DuelScreen_DrawCursorInfo
_0802454A:
	mov r2, r8
	ldrh r6, [r2]
	mov r0, #0xF0
	lsl r0, r0, #2
	and r0, r6
	cmp r0, #0
	beq _080245F8
	ldr r0, _08024698 @ =0x0000080C
	add r7, r4, r0
	ldr r1, _0802469C @ =0x0000081C
	add r0, r4, r1
	ldr r2, _080246A0 @ =0x00000814
	add r1, r4, r2
	ldr r3, [r0]
	ldr r1, [r1]
	mov sl, r1
	sub r3, r3, r1
	str r3, [r7]
	mov r0, #0x81
	lsl r0, r0, #4
	add r0, r0, r4
	mov ip, r0
	mov r1, #0x82
	lsl r1, r1, #4
	add r0, r4, r1
	add r2, #4
	add r1, r4, r2
	ldr r2, [r0]
	ldr r1, [r1]
	mov r9, r1
	sub r2, r2, r1
	mov r4, ip
	str r2, [r4]
	lsl r0, r6, #0x16
	lsr r0, r0, #0x1C
	sub r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #6
	ldr r5, _080246A4 @ =0xFFFFFC3F
	and r5, r6
	orr r5, r0
	mov r6, r8
	strh r5, [r6]
	ldr r4, _08024688 @ =0x081A429C
	lsl r1, r5, #0x16
	lsr r0, r1, #0x1C
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	mul r0, r3
	str r0, [r7]
	lsr r1, r1, #0x1C
	lsl r1, r1, #1
	add r1, r1, r4
	ldrh r1, [r1]
	mul r2, r1
	mov r1, ip
	str r2, [r1]
	cmp r0, #0
	bge _080245C6
	add r0, #0xFF
_080245C6:
	asr r1, r0, #8
	str r1, [r7]
	add r0, r2, #0
	cmp r0, #0
	bge _080245D2
	add r0, #0xFF
_080245D2:
	asr r0, r0, #8
	add r1, sl
	str r1, [r7]
	add r0, r9
	mov r2, ip
	str r0, [r2]
	mov r4, #1
	mov r9, r4
	mov r6, #0xF0
	lsl r6, r6, #2
	and r5, r6
	cmp r5, #0
	bne _080245F8
	mov r0, #4
	mov r1, r8
	ldrb r1, [r1]
	orr r0, r1
	mov r2, r8
	strb r0, [r2]
_080245F8:
	ldr r5, _08024674 @ =0x0201CFB0
	mov r4, #0x81
	lsl r4, r4, #4
	add r0, r5, r4
	ldr r0, [r0]
	ldrb r6, [r5, #4]
	sub r1, r0, r6
	cmp r1, #0x9F
	bhi _08024656
	ldr r2, _08024678 @ =0x00000808
	add r0, r5, r2
	ldrb r4, [r0]
	mov r0, #0x20
	and r0, r4
	mov r2, #0
	cmp r0, #0
	beq _0802461C
	mov r2, #0x30
_0802461C:
	mov r0, #0x10
	and r0, r4
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	neg r0, r0
	asr r3, r0, #0x1F
	mov r0, #0x40
	and r3, r0
	mov r0, #2
	ldrb r6, [r5]
	and r0, r6
	cmp r0, #0
	beq _08024656
	mov r0, #8
	and r0, r4
	cmp r0, #0
	beq _08024656
	ldr r4, _08024698 @ =0x0000080C
	add r0, r5, r4
	ldr r0, [r0]
	add r0, #8
	lsl r1, r1, #0x10
	orr r0, r1
	mov r1, #0x80
	lsl r1, r1, #0x11
	orr r3, r1
	mov r1, #0x80
	bl AddAffineSprite
_08024656:
	bl DuelAnim_Update
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08024664
	mov r6, #1
	mov r9, r6
_08024664:
	mov r0, r9
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08024674: .4byte 0x0201CFB0
_08024678: .4byte 0x00000808
_0802467C: .4byte 0x060091C0
_08024680: .4byte 0x0201AE60
_08024684: .4byte 0x06009AE0
_08024688: .4byte gDuelScreenLerpWeights
_0802468C: .4byte 0x03000040
_08024690: .4byte 0x00004422
_08024694: .4byte 0x00004424
_08024698: .4byte 0x0000080C
_0802469C: .4byte 0x0000081C
_080246A0: .4byte 0x00000814
_080246A4: .4byte 0xFFFFFC3F
	thumb_func_end DuelScreen_Update

