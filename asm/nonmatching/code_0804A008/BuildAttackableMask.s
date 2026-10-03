	thumb_func_start BuildAttackableMask
BuildAttackableMask: @ 0x0804A848
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r4, r0, #0
	mov r8, r1
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0]
	mov r0, #1
	and r1, r0
	ldr r2, _0804A918 @ =0x00000D64
	add r5, r1, #0
	mul r5, r2
	add r7, r4, r5
	mov r1, #0
	strh r1, [r7, #0x24]
	mov r1, r8
	sub r0, r0, r1
	ldr r1, _0804A91C @ =0x0000015B
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _0804A906
	ldr r4, _0804A920 @ =0x000004CE
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _0804A906
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	mov r9, r0
	cmp r0, #0
	bne _0804A906
	ldr r0, _0804A924 @ =0x020192E4
	add r5, r5, r0
	mov sl, r5
	ldrh r6, [r5]
	ldr r5, _0804A928 @ =0x0000042A
	mov r0, #0
	add r1, r5, #0
	bl CountActiveCardsOnField
	add r4, r0, #0
	mov r0, #1
	add r1, r5, #0
	bl CountActiveCardsOnField
	add r4, r4, r0
	lsl r0, r4, #5
	sub r0, r0, r4
	lsl r0, r0, #2
	add r0, r0, r4
	lsl r0, r0, #2
	cmp r6, r0
	blt _0804A906
	ldr r0, [sp, #0]
	cmp r0, #0
	beq _0804A8CE
	mov r1, r9
	strh r1, [r7, #0x26]
_0804A8CE:
	mov r5, #0
	mov r6, #1
	add r4, r7, #0
	mov r7, sl
_0804A8D6:
	mov r0, r8
	add r1, r5, #0
	mov r2, #1
	bl CanMonsterAttack
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0804A900
	ldr r0, [sp, #0]
	cmp r0, #0
	bne _0804A8F6
	ldrh r0, [r7, #0x26]
	asr r0, r5
	and r0, r6
	cmp r0, #0
	bne _0804A900
_0804A8F6:
	add r0, r6, #0
	lsl r0, r5
	ldrh r1, [r4, #0x24]
	orr r0, r1
	strh r0, [r4, #0x24]
_0804A900:
	add r5, #1
	cmp r5, #4
	ble _0804A8D6
_0804A906:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0804A918: .4byte 0x00000D64
_0804A91C: .4byte 0x0000015B
_0804A920: .4byte 0x000004CE
_0804A924: .4byte 0x020192E4
_0804A928: .4byte 0x0000042A
	thumb_func_end BuildAttackableMask

