	thumb_func_start PayChainEnergyCost
PayChainEnergyCost: @ 0x08046A74
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r6, _08046AC4 @ =0x00000436
	add r1, r6, #0
	bl CountActiveCardsOnField
	add r4, r0, #0
	mov r0, #1
	sub r0, r0, r5
	add r1, r6, #0
	bl CountActiveCardsOnField
	add r4, r4, r0
	cmp r4, #0
	ble _08046ABC
	lsl r0, r6, #1
	ldr r1, _08046AC8 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r5, #0
	bl ShowCardEffect
	mov r0, #0x43
	cmp r5, #0
	beq _08046AA8
	ldr r0, _08046ACC @ =0x00008043
_08046AA8:
	lsl r1, r4, #5
	sub r1, r1, r4
	lsl r1, r1, #2
	add r1, r1, r4
	lsl r1, r1, #0x12
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_08046ABC:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08046AC4: .4byte 0x00000436
_08046AC8: .4byte gCardNumberToId
_08046ACC: .4byte 0x00008043
	thumb_func_end PayChainEnergyCost

