	thumb_func_start TributeMonster
TributeMonster: @ 0x08017FF4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r0, #0
	add r7, r1, #0
	mov r1, #1
	and r1, r5
	mov r0, #0x94
	add r2, r7, #0
	mul r2, r0
	mov r9, r2
	ldr r2, _08018048 @ =0x00000D64
	add r0, r1, #0
	mul r0, r2
	mov r8, r0
	mov r0, r9
	add r0, r8
	ldr r1, _0801804C @ =0x0201930C
	mov sl, r1
	add r0, sl
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	cmp r7, #4
	bgt _08018044
	ldr r4, _08018050 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _08018044
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _08018054
_08018044:
	mov r0, #0
	b _08018272
_08018048: .4byte 0x00000D64
_0801804C: .4byte 0x0201930C
_08018050: .4byte 0x0000058A
_08018054:
	cmp r6, #0
	bne _0801805A
	b _08018270
_0801805A:
	mov r0, r8
	add r0, sl
	add r9, r0
	ldr r4, _0801809C @ =0x00000453
	mov r0, #0
	add r1, r4, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _0801807A
	mov r0, #1
	add r1, r4, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _080180A4
_0801807A:
	mov r0, #0x7A
	cmp r5, #0
	beq _08018082
	ldr r0, _080180A0 @ =0x0000807A
_08018082:
	lsl r1, r7, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #1
	bl DestroyLinkedCards
	b _08018272
	.align 2, 0
_0801809C: .4byte 0x00000453
_080180A0: .4byte 0x0000807A
_080180A4:
	mov r0, #0x93
	cmp r5, #0
	beq _080180AC
	ldr r0, _08018110 @ =0x00008093
_080180AC:
	lsl r1, r7, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r4, _08018114 @ =0x000007FF
	and r4, r6
	lsl r0, r4, #2
	ldr r2, _08018118 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _080180DE
	mov r1, sl
	sub r1, #0x28
	add r1, r8
	mov r0, #8
	ldrb r2, [r1, #0xB]
	orr r0, r2
	strb r0, [r1, #0xB]
_080180DE:
	add r0, r5, #0
	mov r1, #1
	bl LoseLpOnSendToGraveyard
	lsl r0, r4, #1
	ldr r1, _0801811C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0xA0
	lsl r0, r0, #1
	cmp r1, r0
	beq _08018178
	cmp r1, r0
	bgt _08018134
	sub r0, #0xA
	cmp r1, r0
	beq _08018178
	cmp r1, r0
	bgt _08018120
	cmp r1, #0x2F
	beq _08018160
	sub r0, #7
	cmp r1, r0
	beq _08018178
	b _08018214
_08018110: .4byte 0x00008093
_08018114: .4byte 0x000007FF
_08018118: .4byte gCardStats
_0801811C: .4byte gCardIdToNumber
_08018120:
	ldr r0, _08018130 @ =0x00000139
	cmp r1, r0
	bgt _08018214
	sub r0, #1
	cmp r1, r0
	blt _08018214
	b _08018178
	.align 2, 0
_08018130: .4byte 0x00000139
_08018134:
	ldr r0, _0801814C @ =0x0000045C
	cmp r1, r0
	beq _080181DC
	cmp r1, r0
	bgt _08018154
	ldr r0, _08018150 @ =0x000001CD
	cmp r1, r0
	beq _08018194
	add r0, #0x70
	cmp r1, r0
	beq _08018160
	b _08018214
_0801814C: .4byte 0x0000045C
_08018150: .4byte 0x000001CD
_08018154:
	ldr r0, _08018170 @ =0x000004DA
	cmp r1, r0
	beq _08018160
	add r0, #0xF
	cmp r1, r0
	bne _08018214
_08018160:
	mov r2, r9
	ldr r0, [r2]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldr r1, _08018174 @ =0x3C600000
	b _08018184
	.align 2, 0
_08018170: .4byte 0x000004DA
_08018174: .4byte 0x3C600000
_08018178:
	mov r1, r9
	ldr r0, [r1]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldr r1, _08018190 @ =0x28600000
_08018184:
	orr r1, r6
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
	b _08018214
_08018190: .4byte 0x28600000
_08018194:
	add r0, r5, #0
	mov r2, #1
	and r0, r2
	mov r2, #0x94
	add r1, r7, #0
	mul r1, r2
	ldr r2, _080181D0 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r1, sl
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	bne _08018214
	mov r0, #0x73
	cmp r5, #0
	beq _080181BA
	ldr r0, _080181D4 @ =0x00008073
_080181BA:
	add r1, r6, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldr r1, _080181D8 @ =0x00001388
	add r0, r5, #0
	bl LoseLifePoints
	b _08018214
	.align 2, 0
_080181D0: .4byte 0x00000D64
_080181D4: .4byte 0x00008073
_080181D8: .4byte 0x00001388
_080181DC:
	add r0, r5, #0
	mov r1, #1
	and r0, r1
	mov r2, #0x94
	add r1, r7, #0
	mul r1, r2
	ldr r2, _08018258 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r1, sl
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	bne _08018214
	mov r1, r9
	ldr r0, [r1]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	mov r2, #1
	and r0, r2
	lsl r0, r0, #0x1F
	ldr r1, _0801825C @ =0x3C600000
	orr r1, r6
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
_08018214:
	ldr r0, _08018260 @ =0x000007FF
	and r6, r0
	lsl r0, r6, #1
	ldr r1, _08018264 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08018268 @ =0x000002D9
	cmp r1, r0
	beq _0801822C
	ldr r0, _0801826C @ =0x00000534
	cmp r1, r0
	bne _08018234
_0801822C:
	add r0, r5, #0
	add r1, r7, #0
	bl QueueRemoveLinksToZone
_08018234:
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #0
	bl DestroyLinkedCards
	mov r0, #1
	sub r0, r0, r5
	lsl r2, r5, #0x18
	lsl r1, r7, #0x18
	lsr r2, r2, #8
	orr r2, r1
	lsr r2, r2, #0x10
	mov r1, #0x1E
	bl EventResponse_Request
	mov r0, #1
	b _08018272
	.align 2, 0
_08018258: .4byte 0x00000D64
_0801825C: .4byte 0x3C600000
_08018260: .4byte 0x000007FF
_08018264: .4byte gCardIdToNumber
_08018268: .4byte 0x000002D9
_0801826C: .4byte 0x00000534
_08018270:
	mov r0, #0
_08018272:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end TributeMonster

