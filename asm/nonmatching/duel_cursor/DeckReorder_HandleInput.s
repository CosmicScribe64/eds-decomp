	thumb_func_start DeckReorder_HandleInput
DeckReorder_HandleInput: @ 0x080538C8
	push {r4, r5, r6, lr}
	ldr r5, _0805391C @ =0x02017A40
	ldr r0, _08053920 @ =0x0000053C
	add r4, r5, r0
	ldr r0, [r4]
	lsl r0, r0, #0xC
	lsr r0, r0, #0x18
	cmp r0, #0xA
	beq _0805392C
	cmp r0, #0x14
	beq _08053984
	ldr r1, _08053924 @ =0x03000040
	mov r0, #0x20
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _080538EC
	b _080539DC
_080538EC:
	mov r0, #0
	bl PlaySE
	ldrb r0, [r4]
	add r0, #4
	mov r1, #5
	bl __modsi3
	strb r0, [r4]
	bl TextCellsClear
	ldrb r4, [r4]
	lsl r0, r4, #2
	ldr r2, _08053928 @ =0x00000544
	add r1, r5, r2
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r1, #1
	bl DuelInfo_DrawCard
	b _080539DC
	.align 2, 0
_0805391C: .4byte 0x02017A40
_08053920: .4byte 0x0000053C
_08053924: .4byte 0x03000040
_08053928: .4byte 0x00000544
_0805392C:
	ldr r0, _0805394C @ =0x0000053E
	add r3, r5, r0
	ldrh r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x18
	cmp r0, #0xF
	bgt _08053954
	add r0, #1
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #4
	ldr r1, _08053950 @ =0xFFFFF00F
	and r1, r2
	orr r1, r0
	strh r1, [r3]
	b _08053A98
_0805394C: .4byte 0x0000053E
_08053950: .4byte 0xFFFFF00F
_08053954:
	ldrb r2, [r4]
	lsl r1, r2, #2
	mov r3, #0xA8
	lsl r3, r3, #3
	add r0, r5, r3
	add r0, r1, r0
	add r3, #4
	add r2, r5, r3
	add r1, r1, r2
	bl SwapDuelCards
	ldrb r0, [r4]
	sub r0, #1
	strb r0, [r4]
	ldr r0, [r4]
	ldr r1, _08053980 @ =0xFFF00FFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #5
	orr r0, r1
	str r0, [r4]
	b _08053A98
_08053980: .4byte 0xFFF00FFF
_08053984:
	ldr r0, _080539A4 @ =0x0000053E
	add r3, r5, r0
	ldrh r2, [r3]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x18
	cmp r0, #0xF
	bgt _080539AC
	add r0, #1
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #4
	ldr r1, _080539A8 @ =0xFFFFF00F
	and r1, r2
	orr r1, r0
	strh r1, [r3]
	b _08053A98
_080539A4: .4byte 0x0000053E
_080539A8: .4byte 0xFFFFF00F
_080539AC:
	ldrb r2, [r4]
	lsl r1, r2, #2
	mov r3, #0xA9
	lsl r3, r3, #3
	add r0, r5, r3
	add r0, r1, r0
	sub r3, #4
	add r2, r5, r3
	add r1, r1, r2
	bl SwapDuelCards
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	ldr r0, [r4]
	ldr r1, _080539D8 @ =0xFFF00FFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #5
	orr r0, r1
	str r0, [r4]
	b _08053A98
_080539D8: .4byte 0xFFF00FFF
_080539DC:
	ldr r6, _08053A44 @ =0x03000040
	mov r0, #0x10
	ldrh r1, [r6, #6]
	and r0, r1
	cmp r0, #0
	beq _08053A1A
	mov r0, #0
	bl PlaySE
	ldr r5, _08053A48 @ =0x02017A40
	ldr r2, _08053A4C @ =0x0000053C
	add r4, r5, r2
	ldrb r0, [r4]
	add r0, #1
	mov r1, #5
	bl __modsi3
	strb r0, [r4]
	bl TextCellsClear
	ldrb r4, [r4]
	lsl r0, r4, #2
	ldr r3, _08053A50 @ =0x00000544
	add r5, r5, r3
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r1, #1
	bl DuelInfo_DrawCard
_08053A1A:
	mov r0, #0x80
	lsl r0, r0, #2
	ldrh r6, [r6, #6]
	and r0, r6
	cmp r0, #0
	beq _08053A5E
	ldr r5, _08053A48 @ =0x02017A40
	ldr r0, _08053A4C @ =0x0000053C
	add r4, r5, r0
	ldrb r0, [r4]
	cmp r0, #0
	beq _08053A58
	mov r0, #6
	bl PlaySE
	ldr r0, [r4]
	ldr r1, _08053A54 @ =0xFFF00FFF
	and r0, r1
	mov r1, #0xA0
	lsl r1, r1, #8
	b _08053A88
_08053A44: .4byte 0x03000040
_08053A48: .4byte 0x02017A40
_08053A4C: .4byte 0x0000053C
_08053A50: .4byte 0x00000544
_08053A54: .4byte 0xFFF00FFF
_08053A58:
	mov r0, #3
	bl PlaySE
_08053A5E:
	ldr r1, _08053A9C @ =0x03000040
	mov r0, #0x80
	lsl r0, r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08053ABA
	ldr r5, _08053AA0 @ =0x02017A40
	ldr r0, _08053AA4 @ =0x0000053C
	add r4, r5, r0
	ldrb r1, [r4]
	cmp r1, #3
	bhi _08053AB4
	mov r0, #6
	bl PlaySE
	ldr r0, [r4]
	ldr r1, _08053AA8 @ =0xFFF00FFF
	and r0, r1
	mov r1, #0xA0
	lsl r1, r1, #9
_08053A88:
	orr r0, r1
	str r0, [r4]
	ldr r2, _08053AAC @ =0x0000053E
	add r1, r5, r2
	ldr r0, _08053AB0 @ =0xFFFFF00F
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
_08053A98:
	mov r0, #0
	b _08053AE2
_08053A9C: .4byte 0x03000040
_08053AA0: .4byte 0x02017A40
_08053AA4: .4byte 0x0000053C
_08053AA8: .4byte 0xFFF00FFF
_08053AAC: .4byte 0x0000053E
_08053AB0: .4byte 0xFFFFF00F
_08053AB4:
	mov r0, #3
	bl PlaySE
_08053ABA:
	ldr r1, _08053AE8 @ =0x03000040
	mov r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08053A98
	mov r0, #1
	bl PlaySE
	ldr r2, _08053AEC @ =0x02017A40
	ldr r0, _08053AF0 @ =0x0000053C
	add r2, r2, r0
	ldr r0, [r2]
	ldr r1, _08053AF4 @ =0xFFF00FFF
	and r0, r1
	mov r1, #0xF0
	lsl r1, r1, #8
	orr r0, r1
	str r0, [r2]
	mov r0, #1
_08053AE2:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08053AE8: .4byte 0x03000040
_08053AEC: .4byte 0x02017A40
_08053AF0: .4byte 0x0000053C
_08053AF4: .4byte 0xFFF00FFF
	thumb_func_end DeckReorder_HandleInput

