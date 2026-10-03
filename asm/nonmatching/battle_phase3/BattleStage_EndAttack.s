	thumb_func_start BattleStage_EndAttack
BattleStage_EndAttack: @ 0x0804DB6C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	mov r0, #0
	mov r8, r0
	bl Duel_CheckWin
	cmp r0, #0
	beq _0804DB84
	mov r0, #1
	b _0804DC6A
_0804DB84:
	mov r4, #1
	sub r6, r4, r5
	ldr r7, _0804DC30 @ =0x02018450
	ldrb r0, [r7, #1]
	lsl r1, r0, #0x1C
	lsr r1, r1, #0x1D
	add r0, r6, #0
	bl ApplyKotodamaToZone
	add r2, r6, #0
	and r2, r4
	ldrb r1, [r7, #1]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1D
	mov r1, #0x94
	mul r0, r1
	ldr r1, _0804DC34 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0804DC38 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0804DBFA
	add r0, r5, #0
	bl CountFreeMonsterZones
	cmp r0, #0
	ble _0804DBFA
	ldrb r0, [r7, #1]
	lsl r1, r0, #0x1C
	lsr r1, r1, #0x1D
	ldr r2, _0804DC3C @ =0x000004DB
	add r0, r6, #0
	bl CountZoneLinksFromCard
	cmp r0, #0
	beq _0804DBFA
	add r4, r6, #0
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	ldrb r7, [r7, #1]
	lsl r0, r7, #0x1C
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r4, r0
	add r0, r5, #0
	bl FindFreeMonsterZone
	lsl r2, r5, #0x18
	lsl r0, r0, #0x18
	lsr r2, r2, #8
	orr r2, r0
	lsr r2, r2, #0x10
	add r0, r6, #0
	add r1, r4, #0
	bl MoveFieldCard
_0804DBFA:
	ldr r4, _0804DC40 @ =0x00000602
	add r0, r5, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _0804DC16
	mov r0, #1
	sub r0, r0, r5
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _0804DC1A
_0804DC16:
	mov r1, #1
	mov r8, r1
_0804DC1A:
	mov r0, r8
	cmp r0, #0
	beq _0804DC48
	ldr r0, _0804DC44 @ =0x086249F8
	ldrh r1, [r0]
	add r0, r5, #0
	bl ShowCardEffect
	mov r0, #1
	b _0804DC6A
	.align 2, 0
_0804DC30: .4byte 0x02018450
_0804DC34: .4byte 0x00000D64
_0804DC38: .4byte 0x0201930C
_0804DC3C: .4byte 0x000004DB
_0804DC40: .4byte 0x00000602
_0804DC44: .4byte gUnk_086249F8
_0804DC48:
	ldr r2, _0804DC74 @ =0x020192E0
	ldr r1, _0804DC78 @ =0x00001B14
	add r3, r2, r1
	ldr r0, [r3]
	ldr r1, _0804DC7C @ =0xFFFE01FF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #2
	orr r0, r1
	str r0, [r3]
	ldr r0, _0804DC80 @ =0x00001B16
	add r2, r2, r0
	ldr r0, _0804DC84 @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
	mov r0, #0
_0804DC6A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0804DC74: .4byte 0x020192E0
_0804DC78: .4byte 0x00001B14
_0804DC7C: .4byte 0xFFFE01FF
_0804DC80: .4byte 0x00001B16
_0804DC84: .4byte 0xFFFFFE01
	thumb_func_end BattleStage_EndAttack

