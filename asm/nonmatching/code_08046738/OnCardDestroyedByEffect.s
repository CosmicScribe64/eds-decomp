	thumb_func_start OnCardDestroyedByEffect
OnCardDestroyedByEffect: @ 0x08046CB0
	push {r4, r5, r6, r7, lr}
	add r3, r0, #0
	add r4, r1, #0
	add r7, r2, #0
	mov r1, #1
	and r1, r4
	mov r0, #0x94
	add r2, r7, #0
	mul r2, r0
	ldr r0, _08046D24 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08046D28 @ =0x0201930C
	add r6, r2, r0
	ldr r0, [r6]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08046D2C @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08046D30 @ =0x000005EA
	ldrh r0, [r0]
	cmp r0, r1
	bne _08046D1E
	cmp r3, r4
	beq _08046D1E
	ldr r5, _08046D34 @ =0x00000453
	mov r0, #0
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _08046D1E
	mov r0, #1
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _08046D1E
	cmp r7, #4
	bgt _08046D1E
	ldr r1, [r6]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r4, #0
	bl sub_080197C0
	mov r0, #0x4C
	cmp r4, #0
	beq _08046D14
	ldr r0, _08046D38 @ =0x0000804C
_08046D14:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08046D1E:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08046D24: .4byte 0x00000D64
_08046D28: .4byte 0x0201930C
_08046D2C: .4byte gCardIdToNumber
_08046D30: .4byte 0x000005EA
_08046D34: .4byte 0x00000453
_08046D38: .4byte 0x0000804C
	thumb_func_end OnCardDestroyedByEffect

