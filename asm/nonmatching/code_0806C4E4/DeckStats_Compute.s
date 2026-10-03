	thumb_func_start DeckStats_Compute
DeckStats_Compute: @ 0x0806CB68
	push {r4, r5, lr}
	ldr r5, _0806CB88 @ =0x02030000
	bl DeckEdit_BuildCardLists
	ldr r0, _0806CB8C @ =0x0201DB20
	ldr r1, _0806CB90 @ =0x00001C1C
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #1
	beq _0806CBAC
	cmp r0, #1
	bgt _0806CB94
	cmp r0, #0
	beq _0806CB9A
	b _0806CBD2
	.align 2, 0
_0806CB88: .4byte 0x02030000
_0806CB8C: .4byte 0x0201DB20
_0806CB90: .4byte 0x00001C1C
_0806CB94:
	cmp r0, #2
	beq _0806CBC8
	b _0806CBD2
_0806CB9A:
	ldr r0, _0806CBA4 @ =0x02011C20
	ldr r2, _0806CBA8 @ =0x000020C6
	add r0, r0, r2
	b _0806CBCE
	.align 2, 0
_0806CBA4: .4byte 0x02011C20
_0806CBA8: .4byte 0x000020C6
_0806CBAC:
	ldr r0, _0806CBC0 @ =0x02011C20
	ldr r2, _0806CBC4 @ =0x000020C8
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldrh r1, [r1]
	ldrh r0, [r0]
	add r0, r1, r0
	b _0806CBD0
	.align 2, 0
_0806CBC0: .4byte 0x02011C20
_0806CBC4: .4byte 0x000020C8
_0806CBC8:
	ldr r0, _0806CD04 @ =0x02011C20
	ldr r1, _0806CD08 @ =0x000020CA
	add r0, r0, r1
_0806CBCE:
	ldrh r0, [r0]
_0806CBD0:
	strh r0, [r5, #0x18]
_0806CBD2:
	mov r0, #0x64
	strh r0, [r5, #0x1A]
	ldr r4, _0806CD0C @ =0x0201DB20
	ldr r2, _0806CD10 @ =0x00001C1C
	add r4, r4, r2
	ldrb r0, [r4]
	mov r1, #1
	bl DeckStats_CountCategory
	strh r0, [r5]
	ldrb r0, [r4]
	mov r1, #2
	bl DeckStats_CountCategory
	strh r0, [r5, #4]
	ldrb r0, [r4]
	mov r1, #3
	bl DeckStats_CountCategory
	strh r0, [r5, #8]
	ldrb r0, [r4]
	mov r1, #4
	bl DeckStats_CountCategory
	strh r0, [r5, #0xC]
	ldrb r0, [r4]
	mov r1, #5
	bl DeckStats_CountCategory
	strh r0, [r5, #0x10]
	ldrb r0, [r4]
	mov r1, #6
	bl DeckStats_CountCategory
	strh r0, [r5, #0x14]
	ldrh r1, [r5, #4]
	ldrh r2, [r5]
	add r1, r1, r2
	ldrh r2, [r5, #8]
	add r2, r2, r1
	ldrh r3, [r5, #0xC]
	add r3, r3, r2
	ldrh r1, [r5, #0x10]
	add r1, r1, r3
	add r0, r0, r1
	strh r0, [r5, #0x18]
	ldrh r1, [r5]
	lsl r0, r1, #2
	ldrh r1, [r5, #0x18]
	bl DivFix8
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, r0, r1
	asr r1, r0, #8
	mov r4, #0xFF
	and r0, r4
	cmp r0, #0x7F
	ble _0806CC50
	add r1, #1
_0806CC50:
	strh r1, [r5, #2]
	ldrh r0, [r5, #4]
	lsl r0, r0, #2
	ldrh r1, [r5, #0x18]
	bl DivFix8
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, r0, r1
	asr r1, r0, #8
	and r0, r4
	cmp r0, #0x7F
	ble _0806CC72
	add r1, #1
_0806CC72:
	strh r1, [r5, #6]
	ldrh r0, [r5, #8]
	lsl r0, r0, #2
	ldrh r1, [r5, #0x18]
	bl DivFix8
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, r0, r1
	asr r1, r0, #8
	and r0, r4
	cmp r0, #0x7F
	ble _0806CC94
	add r1, #1
_0806CC94:
	strh r1, [r5, #0xA]
	ldrh r0, [r5, #0xC]
	lsl r0, r0, #2
	ldrh r1, [r5, #0x18]
	bl DivFix8
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, r0, r1
	asr r1, r0, #8
	and r0, r4
	cmp r0, #0x7F
	ble _0806CCB6
	add r1, #1
_0806CCB6:
	strh r1, [r5, #0xE]
	ldrh r0, [r5, #0x10]
	lsl r0, r0, #2
	ldrh r1, [r5, #0x18]
	bl DivFix8
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, r0, r1
	asr r1, r0, #8
	and r0, r4
	cmp r0, #0x7F
	ble _0806CCD8
	add r1, #1
_0806CCD8:
	strh r1, [r5, #0x12]
	ldrh r0, [r5, #0x14]
	lsl r0, r0, #2
	ldrh r1, [r5, #0x18]
	bl DivFix8
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r0, r0, r1
	asr r1, r0, #8
	and r0, r4
	cmp r0, #0x7F
	ble _0806CCFA
	add r1, #1
_0806CCFA:
	strh r1, [r5, #0x16]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0806CD04: .4byte 0x02011C20
_0806CD08: .4byte 0x000020CA
_0806CD0C: .4byte 0x0201DB20
_0806CD10: .4byte 0x00001C1C
	thumb_func_end DeckStats_Compute

