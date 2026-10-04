	thumb_func_start ApplyKotodama
ApplyKotodama: @ 0x08046AD0
	push {r4, r5, r6, r7, lr}
	ldr r4, _08046B4C @ =0x00000464
	mov r0, #0
	add r1, r4, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _08046AEC
	mov r0, #1
	add r1, r4, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _08046B44
_08046AEC:
	mov r7, #0
	mov r5, #0
_08046AF0:
	mov r4, #0
	add r6, r5, #1
_08046AF4:
	add r0, r5, #0
	add r1, r4, #0
	bl CountOtherFaceUpSameNameMonsters
	cmp r0, #0
	beq _08046B02
	mov r7, #1
_08046B02:
	add r4, #1
	cmp r4, #4
	ble _08046AF4
	add r5, r6, #0
	cmp r5, #1
	ble _08046AF0
	cmp r7, #0
	beq _08046B44
	ldr r0, _08046B50 @ =0x086246BC
	ldrh r1, [r0]
	mov r0, #0
	bl ShowCardEffect
	mov r5, #0
_08046B1E:
	mov r4, #0
	add r6, r5, #1
_08046B22:
	add r0, r5, #0
	add r1, r4, #0
	bl CountOtherFaceUpSameNameMonsters
	cmp r0, #0
	beq _08046B38
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
_08046B38:
	add r4, #1
	cmp r4, #4
	ble _08046B22
	add r5, r6, #0
	cmp r5, #1
	ble _08046B1E
_08046B44:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08046B4C: .4byte 0x00000464
_08046B50: .4byte gCardNumberToId_Kotodama
	thumb_func_end ApplyKotodama

