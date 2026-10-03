	thumb_func_start SendFusionMaterialToGrave
SendFusionMaterialToGrave: @ 0x08018280
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r6, r0, #0
	mov r8, r1
	add r1, r6, #0
	mov r0, #1
	and r1, r0
	mov r0, #0x94
	mov r2, r8
	mul r2, r0
	ldr r3, _080182BC @ =0x00000D64
	add r7, r1, #0
	mul r7, r3
	add r0, r2, r7
	ldr r1, _080182C0 @ =0x0201930C
	mov sl, r1
	add r0, sl
	str r0, [sp, #0]
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	mov r3, r8
	cmp r3, #4
	ble _080182C4
	mov r0, #0
	b _080184C6
_080182BC: .4byte 0x00000D64
_080182C0: .4byte 0x0201930C
_080182C4:
	cmp r5, #0
	bne _080182CA
	b _080184C4
_080182CA:
	mov r1, sl
	add r0, r7, r1
	add r0, r0, r2
	mov r9, r0
	ldr r4, _08018310 @ =0x00000453
	mov r0, #0
	add r1, r4, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _080182EC
	mov r0, #1
	add r1, r4, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _08018318
_080182EC:
	mov r0, #0x7A
	cmp r6, #0
	beq _080182F4
	ldr r0, _08018314 @ =0x0000807A
_080182F4:
	mov r2, r8
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	add r0, r6, #0
	mov r1, r8
	mov r2, #1
	bl DestroyLinkedCards
	b _080184C6
	.align 2, 0
_08018310: .4byte 0x00000453
_08018314: .4byte 0x0000807A
_08018318:
	mov r0, #0xA5
	cmp r6, #0
	beq _08018320
	ldr r0, _08018388 @ =0x000080A5
_08018320:
	mov r3, r8
	lsl r1, r3, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r4, _0801838C @ =0x000007FF
	and r4, r5
	lsl r0, r4, #2
	ldr r1, _08018390 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08018354
	mov r1, sl
	sub r1, #0x28
	add r1, r7, r1
	mov r0, #8
	ldrb r2, [r1, #0xB]
	orr r0, r2
	strb r0, [r1, #0xB]
_08018354:
	add r0, r6, #0
	mov r1, #1
	bl LoseLpOnSendToGraveyard
	lsl r0, r4, #1
	ldr r3, _08018394 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	mov r0, #0xA0
	lsl r0, r0, #1
	cmp r1, r0
	beq _080183EE
	cmp r1, r0
	bgt _080183AC
	sub r0, #0xA
	cmp r1, r0
	beq _080183EE
	cmp r1, r0
	bgt _08018398
	cmp r1, #0x2F
	beq _080183D8
	sub r0, #7
	cmp r1, r0
	beq _080183EE
	b _08018482
	.align 2, 0
_08018388: .4byte 0x000080A5
_0801838C: .4byte 0x000007FF
_08018390: .4byte gCardStats
_08018394: .4byte gCardIdToNumber
_08018398:
	ldr r0, _080183A8 @ =0x00000139
	cmp r1, r0
	bgt _08018482
	sub r0, #1
	cmp r1, r0
	blt _08018482
	b _080183EE
	.align 2, 0
_080183A8: .4byte 0x00000139
_080183AC:
	ldr r0, _080183C4 @ =0x0000045C
	cmp r1, r0
	beq _0801845C
	cmp r1, r0
	bgt _080183CC
	ldr r0, _080183C8 @ =0x000001CD
	cmp r1, r0
	beq _08018414
	add r0, #0x70
	cmp r1, r0
	beq _080183D8
	b _08018482
_080183C4: .4byte 0x0000045C
_080183C8: .4byte 0x000001CD
_080183CC:
	ldr r0, _08018408 @ =0x000004DA
	cmp r1, r0
	beq _080183D8
	add r0, #0xF
	cmp r1, r0
	bne _08018482
_080183D8:
	mov r1, r9
	ldr r0, [r1]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldr r1, _0801840C @ =0x3C600000
	orr r1, r5
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
_080183EE:
	mov r2, r9
	ldr r0, [r2]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldr r1, _08018410 @ =0x28600000
	orr r1, r5
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
	b _08018482
	.align 2, 0
_08018408: .4byte 0x000004DA
_0801840C: .4byte 0x3C600000
_08018410: .4byte 0x28600000
_08018414:
	add r2, r6, #0
	mov r0, #1
	and r2, r0
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	ldr r3, _08018450 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r1, r1, r0
	add r1, sl
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	bne _08018482
	mov r0, #0x73
	cmp r6, #0
	beq _0801843C
	ldr r0, _08018454 @ =0x00008073
_0801843C:
	add r1, r5, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldr r1, _08018458 @ =0x00001388
	add r0, r6, #0
	bl LoseLifePoints
	b _08018482
_08018450: .4byte 0x00000D64
_08018454: .4byte 0x00008073
_08018458: .4byte 0x00001388
_0801845C:
	mov r0, #0x20
	ldr r1, [sp, #0]
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	bne _08018482
	mov r2, r9
	ldr r0, [r2]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	mov r3, #1
	and r0, r3
	lsl r0, r0, #0x1F
	ldr r1, _080184B0 @ =0x3C600000
	orr r1, r5
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
_08018482:
	ldr r0, _080184B4 @ =0x000007FF
	and r5, r0
	lsl r0, r5, #1
	ldr r1, _080184B8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080184BC @ =0x000002D9
	cmp r1, r0
	beq _0801849A
	ldr r0, _080184C0 @ =0x00000534
	cmp r1, r0
	bne _080184A2
_0801849A:
	add r0, r6, #0
	mov r1, r8
	bl QueueRemoveLinksToZone
_080184A2:
	add r0, r6, #0
	mov r1, r8
	mov r2, #0
	bl DestroyLinkedCards
	mov r0, #1
	b _080184C6
_080184B0: .4byte 0x3C600000
_080184B4: .4byte 0x000007FF
_080184B8: .4byte gCardIdToNumber
_080184BC: .4byte 0x000002D9
_080184C0: .4byte 0x00000534
_080184C4:
	mov r0, #0
_080184C6:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end SendFusionMaterialToGrave
	.align 2, 0

