	thumb_func_start BattleStage_PayAttackCosts
BattleStage_PayAttackCosts: @ 0x0804BA1C
	push {r4, r5, r6, lr}
	add r6, r0, #0
	ldr r2, _0804BA9C @ =0x02018450
	ldrb r1, [r2]
	mov r0, #0x10
	and r0, r1
	cmp r0, #0
	bne _0804BA94
	mov r0, #0x10
	orr r0, r1
	strb r0, [r2]
	mov r0, #1
	sub r0, r0, r6
	ldr r5, _0804BAA0 @ =0x00000427
	add r1, r5, #0
	bl CountActiveCardsOnField
	add r4, r0, #0
	cmp r4, #0
	ble _0804BA5C
	lsl r0, r5, #1
	ldr r1, _0804BAA4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r6, #0
	bl ShowCardEffect
	add r0, r6, #0
	add r1, r4, #0
	mov r2, #1
	bl SendTopDeckCardsToGraveyard
_0804BA5C:
	ldr r5, _0804BAA8 @ =0x0000042A
	mov r0, #0
	add r1, r5, #0
	bl CountActiveCardsOnField
	add r4, r0, #0
	mov r0, #1
	add r1, r5, #0
	bl CountActiveCardsOnField
	add r4, r4, r0
	cmp r4, #0
	ble _0804BA94
	lsl r0, r5, #1
	ldr r1, _0804BAA4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r6, #0
	bl ShowCardEffect
	lsl r1, r4, #5
	sub r1, r1, r4
	lsl r1, r1, #2
	add r1, r1, r4
	lsl r1, r1, #2
	add r0, r6, #0
	bl LoseLifePoints
_0804BA94:
	mov r0, #1
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0804BA9C: .4byte 0x02018450
_0804BAA0: .4byte 0x00000427
_0804BAA4: .4byte gCardNumberToId
_0804BAA8: .4byte 0x0000042A
	thumb_func_end BattleStage_PayAttackCosts

