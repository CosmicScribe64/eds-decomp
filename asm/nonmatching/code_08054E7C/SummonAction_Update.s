	thumb_func_start SummonAction_Update
SummonAction_Update: @ 0x08055728
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, _08055760 @ =0x0201CF90
	ldrb r1, [r2, #0xE]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	bne _0805573C
	b _080559E4
_0805573C:
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _0805574C
	ldrb r2, [r2]
	lsl r0, r2, #0x1F
	cmp r0, #0
	bne _080557B0
_0805574C:
	lsl r0, r1, #0x1B
	lsr r0, r0, #0x1D
	sub r0, #1
	cmp r0, #5
	bhi _080557CC
	lsl r0, r0, #2
	ldr r1, _08055764 @ =0x08055768
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08055760: .4byte 0x0201CF90
_08055764: .4byte 0x08055768
_08055768:
	.4byte _08055780
	.4byte _08055788
	.4byte _08055790
	.4byte _08055798
	.4byte _080557A0
	.4byte _080557A8
_08055780:
	bl ExecuteSummonAction
	lsl r0, r0, #0x10
	b _080557BA
_08055788:
	bl ExecuteSummonActionAskPosition
	lsl r0, r0, #0x10
	b _080557BA
_08055790:
	bl SummonStep_Flip
	lsl r0, r0, #0x10
	b _080557BA
_08055798:
	bl SummonStep_Special
	lsl r0, r0, #0x10
	b _080557BA
_080557A0:
	bl SummonStep_SpecialChoosePosition
	lsl r0, r0, #0x10
	b _080557BA
_080557A8:
	bl SummonStep_SpecialFromHand
	lsl r0, r0, #0x10
	b _080557BA
_080557B0:
	ldr r0, _080557C4 @ =0x02017FB0
	ldr r1, _080557C8 @ =0x00000306
	add r0, r0, r1
	ldrb r0, [r0]
	lsr r0, r0, #7
_080557BA:
	cmp r0, #0
	bne _080557CC
	mov r0, #1
	b _080559EC
	.align 2, 0
_080557C4: .4byte 0x02017FB0
_080557C8: .4byte 0x00000306
_080557CC:
	ldr r4, _080558D0 @ =0x0201CF90
	mov r0, #2
	neg r0, r0
	ldrb r2, [r4, #0xE]
	and r0, r2
	strb r0, [r4, #0xE]
	ldrb r0, [r4]
	lsl r1, r0, #0x1F
	lsr r3, r1, #0x1F
	lsl r0, r0, #0x1A
	lsr r2, r0, #0x1B
	mov r0, #0x94
	add r1, r2, #0
	mul r1, r0
	ldr r0, _080558D4 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r0, _080558D8 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	bne _080557FE
	b _080559C4
_080557FE:
	mov r8, r0
	ldrb r1, [r1, #6]
	lsl r0, r1, #0x1E
	lsr r7, r0, #0x1F
	add r0, r3, #0
	mov r1, #0
	bl DuelCursor_Select
	mov r0, #0x20
	ldrh r1, [r4, #0xC]
	and r0, r1
	cmp r0, #0
	beq _08055856
	ldrb r2, [r4]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r6, _080558DC @ =0x00000595
	add r1, r6, #0
	bl CountActiveCardsOnField
	add r5, r0, #0
	cmp r5, #0
	ble _08055856
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	mov r2, #0x73
	cmp r0, #0
	beq _08055838
	ldr r2, _080558E0 @ =0x00008073
_08055838:
	lsl r0, r6, #1
	ldr r1, _080558E4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r4, [r4]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	bl DrawCards
_08055856:
	cmp r7, #0
	beq _080558B0
	mov r4, #0xC2
	lsl r4, r4, #3
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08055876
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _080558B0
_08055876:
	ldr r5, _080558D0 @ =0x0201CF90
	ldrb r2, [r5]
	lsl r0, r2, #0x1F
	mov r2, #0x73
	cmp r0, #0
	beq _08055884
	ldr r2, _080558E0 @ =0x00008073
_08055884:
	lsl r0, r4, #1
	ldr r1, _080558E4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r1, [r5]
	lsl r0, r1, #0x1F
	mov r2, #0x96
	cmp r0, #0
	beq _080558A2
	ldr r2, _080558E8 @ =0x00008096
_080558A2:
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_080558B0:
	ldr r0, _080558EC @ =0x000007FF
	mov r2, r8
	and r0, r2
	lsl r0, r0, #1
	ldr r1, _080558F0 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080558F4 @ =0x000002EF
	cmp r1, r0
	beq _0805591C
	cmp r1, r0
	bgt _080558F8
	cmp r1, #0x62
	beq _08055904
	b _0805595C
	.align 2, 0
_080558D0: .4byte 0x0201CF90
_080558D4: .4byte 0x00000D64
_080558D8: .4byte 0x0201930C
_080558DC: .4byte 0x00000595
_080558E0: .4byte 0x00008073
_080558E4: .4byte gCardNumberToId
_080558E8: .4byte 0x00008096
_080558EC: .4byte 0x000007FF
_080558F0: .4byte gCardIdToNumber
_080558F4: .4byte 0x000002EF
_080558F8:
	ldr r0, _08055900 @ =0x00000464
	cmp r1, r0
	beq _08055954
	b _0805595C
_08055900: .4byte 0x00000464
_08055904:
	ldr r0, _08055918 @ =0x0201CF90
	ldrb r1, [r0]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	bl ApplyPumpkingBoost
	b _0805595C
	.align 2, 0
_08055918: .4byte 0x0201CF90
_0805591C:
	cmp r7, #0
	beq _08055970
	ldr r0, _08055948 @ =0x0201CF90
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	mov r2, #0x73
	cmp r0, #0
	beq _0805592E
	ldr r2, _0805594C @ =0x00008073
_0805592E:
	lsl r0, r1, #1
	ldr r1, _08055950 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
	bl DisableFaceUpTraps
	b _0805595C
	.align 2, 0
_08055948: .4byte 0x0201CF90
_0805594C: .4byte 0x00008073
_08055950: .4byte gCardNumberToId
_08055954:
	cmp r7, #0
	beq _08055970
	bl ApplyKotodama
_0805595C:
	cmp r7, #0
	beq _08055970
	ldr r0, _08055990 @ =0x0201CF90
	ldrb r1, [r0]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	bl ApplyKotodamaToZone
_08055970:
	ldr r4, _08055990 @ =0x0201CF90
	ldrb r2, [r4]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	bl ApplyDragonCaptureJar
	ldrb r1, [r4, #0xE]
	lsl r0, r1, #0x1B
	lsr r0, r0, #0x1D
	cmp r0, #1
	blt _080559A8
	cmp r0, #2
	ble _08055994
	cmp r0, #3
	beq _080559A4
	b _080559A8
_08055990: .4byte 0x0201CF90
_08055994:
	ldrb r4, [r4, #1]
	lsl r0, r4, #0x19
	mov r1, #8
	cmp r0, #0
	bge _080559A0
	mov r1, #5
_080559A0:
	add r3, r1, #0
	b _080559AA
_080559A4:
	mov r3, #6
	b _080559AA
_080559A8:
	mov r3, #7
_080559AA:
	ldr r0, _080559F8 @ =0x0201CF90
	ldrb r1, [r0]
	lsl r2, r1, #0x1F
	lsr r2, r2, #0x1F
	mov r0, #1
	sub r0, r0, r2
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	lsl r1, r1, #8
	orr r2, r1
	add r1, r3, #0
	bl EventResponse_Request
_080559C4:
	ldr r1, _080559F8 @ =0x0201CF90
	mov r0, #2
	ldrb r2, [r1, #0xE]
	and r0, r2
	cmp r0, #0
	beq _080559E4
	ldrb r1, [r1]
	lsl r0, r1, #0x1F
	cmp r0, #0
	bne _080559E4
	ldr r0, _080559FC @ =0x0000F05B
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelLink_SendMessage
_080559E4:
	ldr r0, _080559F8 @ =0x0201CF90
	ldrb r0, [r0, #0xE]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
_080559EC:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080559F8: .4byte 0x0201CF90
_080559FC: .4byte 0x0000F05B
	thumb_func_end SummonAction_Update

