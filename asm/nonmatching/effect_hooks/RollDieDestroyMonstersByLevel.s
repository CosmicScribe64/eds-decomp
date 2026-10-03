	thumb_func_start RollDieDestroyMonstersByLevel
RollDieDestroyMonstersByLevel: @ 0x08046F20
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	add r4, r0, #0
	mov r5, #0xC0
	lsl r5, r5, #3
	add r1, r5, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08046F3C
	b _0804703E
_08046F3C:
	bl Random
	mov r1, #6
	bl __modsi3
	add r6, r0, #1
	lsl r0, r5, #1
	ldr r1, _08046FD8 @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r4, #0
	bl ShowCardEffect
	mov r0, #0xE4
	cmp r4, #0
	beq _08046F5E
	ldr r0, _08046FDC @ =0x000080E4
_08046F5E:
	lsl r1, r6, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x12
	cmp r4, #0
	beq _08046F72
	ldr r0, _08046FE0 @ =0x00008012
_08046F72:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r7, #0
	mov r0, #1
	mov r9, r0
	ldr r1, _08046FE4 @ =0x000007FF
	add r3, r1, #0
_08046F86:
	mov r4, #0
	add r0, r7, #1
	mov r8, r0
	add r5, r7, #0
	mov r1, r9
	and r5, r1
_08046F92:
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _08046FE8 @ =0x00000D64
	mul r0, r5
	add r1, r1, r0
	ldr r0, _08046FEC @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08047032
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08047032
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08046FF0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08046FFC
	cmp r0, #0x17
	ble _08046FF4
	cmp r0, #0x18
	beq _08046FF8
	b _08046FFC
_08046FD8: .4byte gCardNumberToId
_08046FDC: .4byte 0x000080E4
_08046FE0: .4byte 0x00008012
_08046FE4: .4byte 0x000007FF
_08046FE8: .4byte 0x00000D64
_08046FEC: .4byte 0x0201930C
_08046FF0: .4byte gCardStats
_08046FF4:
	mov r0, #0
	b _0804700E
_08046FF8:
	mov r0, #0xA
	b _0804700E
_08046FFC:
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _0804704C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0804700E:
	mov r1, #0
	cmp r0, r6
	bne _08047016
	mov r1, #1
_08047016:
	cmp r0, #5
	ble _08047020
	cmp r6, #6
	bne _08047020
	mov r1, #1
_08047020:
	cmp r1, #0
	beq _08047032
	add r0, r7, #0
	add r1, r4, #0
	mov r2, #1
	str r3, [sp, #0]
	bl DestroyFieldCard
	ldr r3, [sp, #0]
_08047032:
	add r4, #1
	cmp r4, #4
	ble _08046F92
	mov r7, r8
	cmp r7, #1
	ble _08046F86
_0804703E:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0804704C: .4byte gCardStats
	thumb_func_end RollDieDestroyMonstersByLevel

