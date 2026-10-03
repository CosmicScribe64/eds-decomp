	thumb_func_start DiscardHandCard
DiscardHandCard: @ 0x080193D4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	add r4, r1, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	lsl r3, r3, #0x10
	lsr r7, r3, #0x10
	ldr r6, _08019418 @ =0x00000453
	mov r0, #0
	add r1, r6, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _08019402
	mov r0, #1
	add r1, r6, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _08019420
_08019402:
	mov r0, #0xC1
	cmp r5, #0
	beq _0801940A
	ldr r0, _0801941C @ =0x000080C1
_0801940A:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	add r2, r7, #0
	mov r3, #0
	bl DuelCmd_Push
	b _0801953A
_08019418: .4byte 0x00000453
_0801941C: .4byte 0x000080C1
_08019420:
	mov r0, #0xC0
	cmp r5, #0
	beq _08019428
	ldr r0, _0801946C @ =0x000080C0
_08019428:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	add r2, r7, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r6, #1
	add r2, r5, #0
	and r2, r6
	lsl r0, r4, #2
	ldr r1, _08019470 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08019474 @ =0x02019968
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	ldr r0, _08019478 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _0801947C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08019480 @ =0x00000215
	cmp r1, r0
	beq _08019490
	cmp r1, r0
	bgt _08019484
	sub r0, #0x47
	cmp r1, r0
	beq _080194B8
	b _080194F6
	.align 2, 0
_0801946C: .4byte 0x000080C0
_08019470: .4byte 0x00000D64
_08019474: .4byte 0x02019968
_08019478: .4byte 0x000007FF
_0801947C: .4byte gCardIdToNumber
_08019480: .4byte 0x00000215
_08019484:
	ldr r0, _0801948C @ =0x000004DA
	cmp r1, r0
	beq _080194E0
	b _080194F6
_0801948C: .4byte 0x000004DA
_08019490:
	mov r0, r8
	cmp r0, #0
	beq _080194F6
	mov r0, #0x73
	cmp r5, #0
	beq _0801949E
	ldr r0, _080194B4 @ =0x00008073
_0801949E:
	add r1, r3, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	sub r0, r6, r5
	mov r1, #0xFA
	lsl r1, r1, #2
	bl LoseLifePoints
	b _080194F6
_080194B4: .4byte 0x00008073
_080194B8:
	mov r1, r8
	cmp r1, #0
	beq _080194F6
	mov r0, #0x73
	cmp r5, #0
	beq _080194C6
	ldr r0, _080194DC @ =0x00008073
_080194C6:
	add r1, r3, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	mov r1, #2
	bl DrawCards
	b _080194F6
	.align 2, 0
_080194DC: .4byte 0x00008073
_080194E0:
	lsl r0, r2, #0x1F
	mov r1, #0x1F
	and r1, r5
	lsl r1, r1, #0x10
	ldr r2, _08019544 @ =0x3A600000
	orr r1, r2
	orr r0, r1
	orr r0, r3
	mov r1, #0
	bl Chain_AddPending
_080194F6:
	mov r0, #1
	sub r0, r0, r5
	ldr r6, _08019548 @ =0x0000040E
	add r1, r6, #0
	bl CountActiveCardsOnField
	add r4, r0, #0
	cmp r4, #0
	ble _08019532
	mov r2, #0x73
	cmp r5, #0
	beq _08019510
	ldr r2, _0801954C @ =0x00008073
_08019510:
	lsl r0, r6, #1
	ldr r1, _08019550 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	lsl r1, r4, #5
	sub r1, r1, r4
	lsl r1, r1, #2
	add r1, r1, r4
	lsl r1, r1, #2
	add r0, r5, #0
	bl LoseLifePoints
_08019532:
	add r0, r5, #0
	mov r1, #1
	bl LoseLpOnSendToGraveyard
_0801953A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08019544: .4byte 0x3A600000
_08019548: .4byte 0x0000040E
_0801954C: .4byte 0x00008073
_08019550: .4byte gCardNumberToId
	thumb_func_end DiscardHandCard

