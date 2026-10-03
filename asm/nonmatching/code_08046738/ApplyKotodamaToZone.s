	thumb_func_start ApplyKotodamaToZone
ApplyKotodamaToZone: @ 0x08046B54
	push {r4, r5, r6, lr}
	add r4, r0, #0
	add r6, r1, #0
	ldr r5, _08046BA0 @ =0x00000464
	mov r0, #0
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _08046B74
	mov r0, #1
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _08046B98
_08046B74:
	add r0, r4, #0
	add r1, r6, #0
	bl CountOtherFaceUpSameNameMonsters
	cmp r0, #0
	ble _08046B98
	lsl r0, r5, #1
	ldr r1, _08046BA4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r4, #0
	bl ShowCardEffect
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #1
	bl DestroyFieldCard
_08046B98:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08046BA0: .4byte 0x00000464
_08046BA4: .4byte gCardNumberToId
	thumb_func_end ApplyKotodamaToZone

