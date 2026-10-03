	thumb_func_start AiStepMainPhase
AiStepMainPhase: @ 0x0805B3F4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	ldr r0, _0805B414 @ =0x02015EF0
	ldrb r1, [r0, #0xA]
	add r2, r0, #0
	cmp r1, #0xA
	bls _0805B408
	b _0805B874
_0805B408:
	lsl r0, r1, #2
	ldr r1, _0805B418 @ =0x0805B41C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0805B414: .4byte 0x02015EF0
_0805B418: .4byte 0x0805B41C
_0805B41C:
	.4byte _0805B448
	.4byte _0805B694
	.4byte _0805B6A8
	.4byte _0805B6C8
	.4byte _0805B6E8
	.4byte _0805B874
	.4byte _0805B874
	.4byte _0805B874
	.4byte _0805B874
	.4byte _0805B874
	.4byte _0805B708
_0805B448:
	ldr r5, _0805B47C @ =0x020192E4
	ldr r0, _0805B480 @ =0x00000D6C
	add r1, r5, r0
	mov r0, #0x10
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0805B462
	mov r0, #1
	bl CanNormalSummon
	cmp r0, #0
	bne _0805B48C
_0805B462:
	ldr r1, _0805B484 @ =0x02015EF0
	ldrb r2, [r1, #1]
	cmp r2, #3
	bls _0805B46C
	b _0805B874
_0805B46C:
	ldr r2, _0805B488 @ =0x00001B0C
	add r0, r5, r2
	ldrh r0, [r0]
	cmp r0, #0
	beq _0805B478
	b _0805B868
_0805B478:
	b _0805B874
	.align 2, 0
_0805B47C: .4byte 0x020192E4
_0805B480: .4byte 0x00000D6C
_0805B484: .4byte 0x02015EF0
_0805B488: .4byte 0x00001B0C
_0805B48C:
	ldr r1, _0805B4B0 @ =0x0000015B
	mov r4, #1
	neg r4, r4
	mov r0, #0
	add r2, r4, #0
	bl CountActiveCardsOnFieldExcept
	cmp r0, #0
	ble _0805B4A0
	b _0805B866
_0805B4A0:
	ldr r1, _0805B4B4 @ =0x00001B0C
	add r0, r5, r1
	ldrh r0, [r0]
	cmp r0, #0
	bne _0805B4BC
	ldr r1, _0805B4B8 @ =0x02015EF0
	mov r0, #0xA
	b _0805B86A
_0805B4B0: .4byte 0x0000015B
_0805B4B4: .4byte 0x00001B0C
_0805B4B8: .4byte 0x02015EF0
_0805B4BC:
	mov r0, #1
	bl AiPlanAttack
	cmp r0, #0
	beq _0805B530
	mov r0, #1
	bl CanNormalSummon
	cmp r0, #0
	bne _0805B4D2
	b _0805B874
_0805B4D2:
	bl AiChooseSummonNoTribute
	add r7, r0, #0
	cmp r7, r4
	bgt _0805B4DE
	b _0805B874
_0805B4DE:
	lsl r0, r7, #2
	ldr r2, _0805B528 @ =0x000013E8
	add r1, r5, r2
	add r0, r0, r1
	ldr r4, [r0]
	lsl r4, r4, #0x15
	lsr r4, r4, #0x14
	ldr r0, _0805B52C @ =0x08622AB4
	add r4, r4, r0
	ldrh r0, [r4]
	mov r1, #1
	bl HasFlipEffect
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	ldrh r0, [r4]
	mov r1, #0
	bl HasFlipEffect
	cmp r0, #0
	beq _0805B50A
	mov r5, #1
_0805B50A:
	mov r0, #1
	bl FindFreeMonsterZone
	add r2, r0, #0
	mov r0, #0
	cmp r5, #0
	bne _0805B51A
	mov r0, #1
_0805B51A:
	str r0, [sp, #0]
	mov r0, #1
	add r1, r7, #0
	mov r3, #0
	bl QueueNormalSummon
	b _0805B874
_0805B528: .4byte 0x000013E8
_0805B52C: .4byte gCardIdToNumber
_0805B530:
	mov r0, #1
	bl CanNormalSummon
	cmp r0, #0
	bne _0805B53C
	b _0805B874
_0805B53C:
	add r0, sp, #4
	bl AiChooseSummonWithTribute
	add r7, r0, #0
	cmp r7, r4
	bgt _0805B54A
	b _0805B688
_0805B54A:
	lsl r0, r7, #2
	ldr r2, _0805B5A0 @ =0x000013E8
	add r1, r5, r2
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	ldr r5, _0805B5A4 @ =0x000007FF
	and r5, r6
	lsl r4, r5, #1
	ldr r0, _0805B5A8 @ =0x08622AB4
	add r4, r4, r0
	ldrh r0, [r4]
	mov r1, #1
	bl HasFlipEffect
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	ldrh r0, [r4]
	mov r1, #0
	bl HasFlipEffect
	cmp r0, #0
	beq _0805B580
	mov r1, #1
	mov r8, r1
_0805B580:
	lsl r0, r5, #2
	ldr r2, _0805B5AC @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805B5B8
	cmp r0, #0x17
	ble _0805B5B0
	cmp r0, #0x18
	beq _0805B5B4
	b _0805B5B8
	.align 2, 0
_0805B5A0: .4byte 0x000013E8
_0805B5A4: .4byte 0x000007FF
_0805B5A8: .4byte gCardIdToNumber
_0805B5AC: .4byte gCardStats
_0805B5B0:
	mov r0, #0
	b _0805B5CC
_0805B5B4:
	mov r0, #0xA
	b _0805B5CC
_0805B5B8:
	ldr r0, _0805B5DC @ =0x000007FF
	and r6, r0
	lsl r0, r6, #2
	ldr r1, _0805B5E0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0805B5CC:
	cmp r0, #6
	bhi _0805B652
	lsl r0, r0, #2
	ldr r1, _0805B5E4 @ =0x0805B5E8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0805B5DC: .4byte 0x000007FF
_0805B5E0: .4byte gCardStats
_0805B5E4: .4byte 0x0805B5E8
_0805B5E8:
	.4byte _0805B688
	.4byte _0805B604
	.4byte _0805B604
	.4byte _0805B604
	.4byte _0805B604
	.4byte _0805B624
	.4byte _0805B624
_0805B604:
	mov r0, #1
	bl FindFreeMonsterZone
	add r2, r0, #0
	mov r0, #0
	mov r1, r8
	cmp r1, #0
	bne _0805B616
	mov r0, #1
_0805B616:
	str r0, [sp, #0]
	mov r0, #1
	add r1, r7, #0
	mov r3, #0
	bl QueueNormalSummon
	b _0805B874
_0805B624:
	mov r4, #1
	neg r4, r4
	add r0, r4, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r5, r0, #0
	cmp r5, r4
	beq _0805B688
	mov r3, #0x90
	orr r3, r5
_0805B63A:
	mov r0, #0
	mov r2, r8
	cmp r2, #0
	bne _0805B644
	mov r0, #1
_0805B644:
	str r0, [sp, #0]
	mov r0, #1
	add r1, r7, #0
	add r2, r5, #0
	bl QueueNormalSummon
	b _0805B874
_0805B652:
	mov r4, #1
	neg r4, r4
	add r0, r4, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r5, r0, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r2, r0, #0
	cmp r5, r4
	beq _0805B688
	cmp r2, r4
	beq _0805B688
	mov r1, #0x70
	neg r1, r1
	add r0, r1, #0
	add r1, r5, #0
	orr r1, r0
	lsl r1, r1, #0x18
	orr r2, r0
	lsl r0, r2, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r3, r1, #0x10
	b _0805B63A
_0805B688:
	ldr r1, _0805B690 @ =0x02015EF0
	ldrb r0, [r1, #0xA]
	add r0, #1
	b _0805B86A
_0805B690: .4byte 0x02015EF0
_0805B694:
	mov r0, #0
	strb r0, [r2, #6]
	strb r0, [r2, #7]
	strb r0, [r2, #8]
	strb r0, [r2, #9]
	ldrb r0, [r2, #0xA]
	add r0, #1
	strb r0, [r2, #0xA]
_0805B6A4:
	mov r0, #0
	b _0805B876
_0805B6A8:
	bl AiActivateMonsterEffects
	cmp r0, #0
	beq _0805B6BC
	ldr r1, _0805B6B8 @ =0x02015EF0
	mov r0, #0
	strb r0, [r1, #0xA]
	b _0805B876
_0805B6B8: .4byte 0x02015EF0
_0805B6BC:
	ldr r1, _0805B6C4 @ =0x02015EF0
	ldrb r0, [r1, #0xA]
	add r0, #1
	b _0805B86A
_0805B6C4: .4byte 0x02015EF0
_0805B6C8:
	bl AiActivateExodiaTraps
	cmp r0, #0
	beq _0805B6A4
	ldr r0, _0805B6E4 @ =0x02015EF0
	mov r1, #0
	strb r1, [r0, #6]
	strb r1, [r0, #7]
	strb r1, [r0, #8]
	strb r1, [r0, #9]
	ldrb r1, [r0, #0xA]
	add r1, #1
	strb r1, [r0, #0xA]
	b _0805B6A4
_0805B6E4: .4byte 0x02015EF0
_0805B6E8:
	bl AiPlaySpells
	cmp r0, #0
	beq _0805B6A4
	ldr r0, _0805B704 @ =0x02015EF0
	mov r1, #0
	strb r1, [r0, #6]
	strb r1, [r0, #7]
	strb r1, [r0, #8]
	strb r1, [r0, #9]
	mov r1, #0xA
	strb r1, [r0, #0xA]
	b _0805B6A4
	.align 2, 0
_0805B704: .4byte 0x02015EF0
_0805B708:
	ldr r4, _0805B758 @ =0x020192E4
	ldr r0, _0805B75C @ =0x00000D6C
	add r1, r4, r0
	mov r0, #0x10
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805B71A
	b _0805B874
_0805B71A:
	bl AiPickMonsterToSet
	add r7, r0, #0
	cmp r7, #0
	bge _0805B726
	b _0805B874
_0805B726:
	lsl r2, r7, #2
	ldr r1, _0805B760 @ =0x000013E8
	add r0, r4, r1
	add r0, r2, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	ldr r0, _0805B764 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0805B768 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	add r5, r2, #0
	cmp r0, #0x15
	blt _0805B774
	cmp r0, #0x17
	ble _0805B76C
	cmp r0, #0x18
	beq _0805B770
	b _0805B774
_0805B758: .4byte 0x020192E4
_0805B75C: .4byte 0x00000D6C
_0805B760: .4byte 0x000013E8
_0805B764: .4byte 0x000007FF
_0805B768: .4byte gCardStats
_0805B76C:
	mov r0, #0
	b _0805B788
_0805B770:
	mov r0, #0xA
	b _0805B788
_0805B774:
	ldr r0, _0805B798 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r2, _0805B79C @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0805B788:
	cmp r0, #6
	bhi _0805B826
	lsl r0, r0, #2
	ldr r1, _0805B7A0 @ =0x0805B7A4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0805B798: .4byte 0x000007FF
_0805B79C: .4byte gCardStats
_0805B7A0: .4byte 0x0805B7A4
_0805B7A4:
	.4byte _0805B874
	.4byte _0805B7C0
	.4byte _0805B7C0
	.4byte _0805B7C0
	.4byte _0805B7C0
	.4byte _0805B808
	.4byte _0805B808
_0805B7C0:
	mov r0, #1
	bl FindFreeMonsterZone
	cmp r0, #0
	blt _0805B866
	mov r4, #0
	ldr r0, _0805B7FC @ =0x0201A6CC
	add r0, r5, r0
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0805B800 @ =0x08622AB4
	add r0, r0, r1
	ldrh r2, [r0]
	cmp r2, #0xF
	beq _0805B7E6
	ldr r0, _0805B804 @ =0x000001FF
	cmp r2, r0
	bne _0805B7E8
_0805B7E6:
	mov r4, #1
_0805B7E8:
	mov r0, #1
	bl FindFreeMonsterZone
	add r2, r0, #0
	str r4, [sp, #0]
	mov r0, #1
	add r1, r7, #0
	mov r3, #0
	b _0805B862
	.align 2, 0
_0805B7FC: .4byte 0x0201A6CC
_0805B800: .4byte gCardIdToNumber
_0805B804: .4byte 0x000001FF
_0805B808:
	mov r4, #1
	neg r4, r4
	add r0, r4, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r5, r0, #0
	cmp r5, r4
	beq _0805B866
	mov r3, #0x90
	orr r3, r5
	mov r0, #0
	str r0, [sp, #0]
	mov r0, #1
	b _0805B85E
_0805B826:
	mov r4, #1
	neg r4, r4
	add r0, r4, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r5, r0, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r2, r0, #0
	cmp r5, r4
	beq _0805B866
	cmp r2, r4
	beq _0805B866
	mov r1, #0x70
	neg r1, r1
	add r0, r1, #0
	add r3, r5, #0
	orr r3, r0
	lsl r3, r3, #0x18
	orr r2, r0
	lsl r0, r2, #0x18
	lsr r3, r3, #8
	orr r3, r0
	lsr r3, r3, #0x10
	mov r0, #1
	str r0, [sp, #0]
_0805B85E:
	add r1, r7, #0
	add r2, r5, #0
_0805B862:
	bl QueueNormalSummon
_0805B866:
	ldr r1, _0805B870 @ =0x02015EF0
_0805B868:
	mov r0, #1
_0805B86A:
	strb r0, [r1, #0xA]
	b _0805B6A4
	.align 2, 0
_0805B870: .4byte 0x02015EF0
_0805B874:
	mov r0, #1
_0805B876:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AiStepMainPhase
	.align 2, 0

