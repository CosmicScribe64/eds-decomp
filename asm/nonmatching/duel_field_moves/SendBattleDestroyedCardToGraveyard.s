	thumb_func_start SendBattleDestroyedCardToGraveyard
SendBattleDestroyedCardToGraveyard: @ 0x08018690
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov sl, r0
	add r5, r1, #0
	add r7, r2, #0
	add r4, r3, #0
	cmp r7, #4
	ble _080186A8
	b _080189EE
_080186A8:
	ldr r6, _080186D0 @ =0x00000453
	mov r0, #0
	add r1, r6, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _080186C2
	mov r0, #1
	add r1, r6, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _080186D4
_080186C2:
	add r0, r5, #0
	add r1, r7, #0
	add r2, r4, #0
	bl BanishBattleDestroyedCard
	b _080189EE
	.align 2, 0
_080186D0: .4byte 0x00000453
_080186D4:
	mov r0, #0x13
	ldr r1, _08018724 @ =0x02017ECA
	strh r0, [r1]
	ldr r0, [r4]
	lsl r1, r0, #0x13
	lsr r1, r1, #0x1F
	mov r8, r1
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	ldr r6, _08018728 @ =0x000007FF
	add r0, r2, #0
	and r0, r6
	lsl r0, r0, #1
	ldr r3, _0801872C @ =0x08622AB4
	mov r9, r3
	add r0, r9
	mov r1, #0xEF
	lsl r1, r1, #1
	ldrh r0, [r0]
	cmp r0, r1
	bne _08018734
	add r0, r5, #0
	add r1, r2, #0
	bl ShowCardEffect
	mov r0, #0x6A
	cmp r5, #0
	beq _0801870E
	ldr r0, _08018730 @ =0x0000806A
_0801870E:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #1
	bl DestroyLinkedCards
	b _080189EE
_08018724: .4byte 0x02017ECA
_08018728: .4byte 0x000007FF
_0801872C: .4byte gCardIdToNumber
_08018730: .4byte 0x0000806A
_08018734:
	mov r0, #0x20
	ldrb r1, [r4, #2]
	orr r0, r1
	strb r0, [r4, #2]
	mov r0, #0x7C
	cmp r5, #0
	beq _08018744
	ldr r0, _080187BC @ =0x0000807C
_08018744:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #2
	ldr r2, _080187C0 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0801877E
	ldr r2, _080187C4 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _080187C8 @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	mov r0, #8
	ldrb r3, [r1, #0xB]
	orr r0, r3
	strb r0, [r1, #0xB]
_0801877E:
	ldr r3, [r4]
	lsl r0, r3, #0x14
	lsr r4, r0, #0x14
	add r0, r4, #0
	and r0, r6
	lsl r0, r0, #1
	add r0, r9
	ldrh r1, [r0]
	ldr r0, _080187CC @ =0x0000045B
	add r2, r3, #0
	cmp r1, r0
	bgt _08018824
	sub r0, #1
	cmp r1, r0
	blt _0801879E
	b _08018880
_0801879E:
	mov r0, #0xA0
	lsl r0, r0, #1
	cmp r1, r0
	beq _08018880
	cmp r1, r0
	bgt _080187E8
	sub r0, #0xA
	cmp r1, r0
	beq _08018880
	cmp r1, r0
	bgt _080187D0
	cmp r1, #0x2F
	beq _08018880
	sub r0, #7
	b _08018862
_080187BC: .4byte 0x0000807C
_080187C0: .4byte gCardStats
_080187C4: .4byte 0x020192E4
_080187C8: .4byte 0x00000D64
_080187CC: .4byte 0x0000045B
_080187D0:
	ldr r0, _080187E4 @ =0x00000139
	cmp r1, r0
	ble _080187D8
	b _080189DC
_080187D8:
	sub r0, #1
	cmp r1, r0
	bge _080187E0
	b _080189DC
_080187E0:
	b _08018880
	.align 2, 0
_080187E4: .4byte 0x00000139
_080187E8:
	ldr r0, _08018800 @ =0x000002D9
	cmp r1, r0
	bne _080187F0
	b _080189D4
_080187F0:
	cmp r1, r0
	bgt _08018808
	ldr r0, _08018804 @ =0x000001CD
	cmp r1, r0
	bne _080187FC
	b _08018914
_080187FC:
	add r0, #0x70
	b _08018862
_08018800: .4byte 0x000002D9
_08018804: .4byte 0x000001CD
_08018808:
	ldr r0, _08018818 @ =0x00000454
	cmp r1, r0
	beq _08018880
	cmp r1, r0
	bgt _0801881C
	sub r0, #0x40
	b _08018862
	.align 2, 0
_08018818: .4byte 0x00000454
_0801881C:
	ldr r0, _08018820 @ =0x00000456
	b _08018862
_08018820: .4byte 0x00000456
_08018824:
	ldr r0, _08018848 @ =0x000004DA
	cmp r1, r0
	bgt _08018854
	sub r0, #1
	cmp r1, r0
	bge _08018880
	sub r0, #0x79
	cmp r1, r0
	bgt _0801884C
	sub r0, #1
	cmp r1, r0
	bge _08018880
	sub r0, #3
	cmp r1, r0
	bne _08018844
	b _08018960
_08018844:
	add r0, #1
	b _08018862
_08018848: .4byte 0x000004DA
_0801884C:
	ldr r0, _08018850 @ =0x00000463
	b _08018862
_08018850: .4byte 0x00000463
_08018854:
	ldr r0, _08018868 @ =0x00000534
	cmp r1, r0
	bne _0801885C
	b _080189D4
_0801885C:
	cmp r1, r0
	bgt _0801886C
	sub r0, #0x4B
_08018862:
	cmp r1, r0
	beq _08018880
	b _080189DC
_08018868: .4byte 0x00000534
_0801886C:
	ldr r0, _0801887C @ =0x0000057D
	cmp r1, r0
	beq _08018880
	add r0, #0x6D
	cmp r1, r0
	bne _0801887A
	b _080189B0
_0801887A:
	b _080189DC
_0801887C: .4byte 0x0000057D
_08018880:
	mov r0, r8
	lsl r4, r0, #0x1F
	ldr r1, _080188EC @ =0x02017A40
	ldr r3, _080188F0 @ =0x0000048A
	add r1, r1, r3
	mov r0, #0x3F
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #0x19
	orr r4, r0
	lsl r0, r2, #0x14
	ldr r3, _080188F4 @ =0x0000FFFF
	lsr r0, r0, #0x14
	mov r1, #0xC0
	lsl r1, r1, #0xF
	orr r0, r1
	orr r4, r0
	ldr r1, _080188F8 @ =0x020192E0
	ldr r2, _080188FC @ =0x00001B12
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r2, r0, #0x1E
	lsr r0, r2, #0x1F
	cmp r5, r0
	bne _080188C0
	add r3, r0, #0
	ldr r0, _08018900 @ =0x02018450
	ldrh r0, [r0]
	lsl r0, r0, #0x17
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r3, r0
_080188C0:
	ldr r2, _080188FC @ =0x00001B12
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r2, r0, #0x1E
	lsr r1, r2, #0x1F
	mov r0, #1
	sub r0, r0, r1
	cmp r5, r0
	bne _08018904
	add r0, r1, #0
	mov r1, #1
	sub r1, r1, r0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldr r0, _08018900 @ =0x02018450
	ldrb r0, [r0, #1]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r1, r0
	lsl r1, r1, #0x10
	b _08018906
_080188EC: .4byte 0x02017A40
_080188F0: .4byte 0x0000048A
_080188F4: .4byte 0x0000FFFF
_080188F8: .4byte 0x020192E0
_080188FC: .4byte 0x00001B12
_08018900: .4byte 0x02018450
_08018904:
	ldr r1, _08018910 @ =0xFFFF0000
_08018906:
	orr r1, r3
	add r0, r4, #0
	bl Chain_AddPending
	b _080189DC
_08018910: .4byte 0xFFFF0000
_08018914:
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _08018950 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08018954 @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	bne _080189DC
	mov r0, #0x73
	mov r3, r8
	cmp r3, #0
	beq _0801893C
	ldr r0, _08018958 @ =0x00008073
_0801893C:
	add r1, r4, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldr r1, _0801895C @ =0x00001388
	mov r0, r8
	bl LoseLifePoints
	b _080189DC
_08018950: .4byte 0x00000D64
_08018954: .4byte 0x0201930C
_08018958: .4byte 0x00008073
_0801895C: .4byte 0x00001388
_08018960:
	mov r6, #1
	add r2, r5, #0
	and r2, r6
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _080189A4 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _080189A8 @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	bne _080189DC
	lsl r0, r3, #0x13
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0x1F
	mov r1, #0x3F
	ldr r2, _080189AC @ =0x02017ECA
	ldrh r2, [r2]
	and r1, r2
	lsl r1, r1, #0x19
	orr r0, r1
	mov r1, #0xC0
	lsl r1, r1, #0xF
	orr r4, r1
	orr r0, r4
	mov r1, #0
	bl Chain_AddPending
	b _080189DC
_080189A4: .4byte 0x00000D64
_080189A8: .4byte 0x0201930C
_080189AC: .4byte 0x02017ECA
_080189B0:
	cmp sl, r5
	bne _080189DC
	add r0, r5, #0
	add r1, r4, #0
	bl ShowActivatedCard
	mov r0, #0x4C
	cmp r5, #0
	beq _080189C4
	ldr r0, _080189D0 @ =0x0000804C
_080189C4:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _080189DC
_080189D0: .4byte 0x0000804C
_080189D4:
	add r0, r5, #0
	add r1, r7, #0
	bl QueueRemoveLinksToZone
_080189DC:
	add r0, r5, #0
	mov r1, #1
	bl LoseLpOnSendToGraveyard
	add r0, r5, #0
	add r1, r7, #0
	mov r2, #1
	bl DestroyLinkedCards
_080189EE:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end SendBattleDestroyedCardToGraveyard

