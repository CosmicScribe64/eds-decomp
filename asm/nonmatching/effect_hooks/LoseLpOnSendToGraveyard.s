	thumb_func_start LoseLpOnSendToGraveyard
LoseLpOnSendToGraveyard: @ 0x08046C20
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r7, r1, #0
	ldr r6, _08046C64 @ =0x0000051A
	add r1, r6, #0
	bl CountActiveCardsOnField
	add r4, r0, #0
	mov r0, #1
	sub r0, r0, r5
	add r1, r6, #0
	bl CountActiveCardsOnField
	add r4, r4, r0
	cmp r4, #0
	ble _08046C5E
	lsl r0, r6, #1
	ldr r1, _08046C68 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r5, #0
	bl ShowCardEffect
	lsl r0, r7, #2
	add r0, r0, r7
	lsl r1, r0, #4
	sub r1, r1, r0
	lsl r1, r1, #2
	add r0, r5, #0
	bl LoseLifePoints
_08046C5E:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08046C64: .4byte 0x0000051A
_08046C68: .4byte gCardNumberToId
	thumb_func_end LoseLpOnSendToGraveyard

