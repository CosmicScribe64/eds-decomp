	thumb_func_start EndPhase_DestroyLowLevelMonsters
EndPhase_DestroyLowLevelMonsters: @ 0x0804E780
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r0, #0
	mov r0, #0
	mov r9, r0
	mov r4, #0
	ldr r1, _0804E7E0 @ =0x0201930C
	mov sl, r1
	mov r7, #1
	add r0, r5, #0
	and r0, r7
	ldr r1, _0804E7E4 @ =0x00000D64
	mul r0, r1
	ldr r6, _0804E7E8 @ =0x000007FF
	mov r2, sl
	add r3, r0, r2
_0804E7A6:
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0804E81C
	mov r0, #2
	ldrb r2, [r3, #6]
	and r0, r2
	cmp r0, #0
	beq _0804E81C
	add r2, r1, #0
	add r0, r2, #0
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _0804E7EC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0804E7F8
	cmp r0, #0x17
	ble _0804E7F0
	cmp r0, #0x18
	beq _0804E7F4
	b _0804E7F8
	.align 2, 0
_0804E7E0: .4byte 0x0201930C
_0804E7E4: .4byte 0x00000D64
_0804E7E8: .4byte 0x000007FF
_0804E7EC: .4byte gCardStats
_0804E7F0:
	mov r0, #0
	b _0804E80A
_0804E7F4:
	mov r0, #0xA
	b _0804E80A
_0804E7F8:
	and r2, r6
	lsl r0, r2, #2
	ldr r2, _0804E8B8 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0804E80A:
	cmp r0, #3
	bhi _0804E81C
	add r0, r7, #0
	ldrb r1, [r3, #8]
	and r0, r1
	cmp r0, #0
	bne _0804E81C
	mov r2, #1
	mov r9, r2
_0804E81C:
	add r3, #0x94
	add r4, #1
	cmp r4, #4
	ble _0804E7A6
	ldr r4, _0804E8BC @ =0x0000052A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _0804E842
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _0804E842
	mov r0, #0
	mov r9, r0
_0804E842:
	mov r1, r9
	cmp r1, #0
	beq _0804E85E
	mov r2, #0x73
	cmp r5, #0
	beq _0804E850
	ldr r2, _0804E8C0 @ =0x00008073
_0804E850:
	ldr r0, _0804E8C4 @ =0x08624848
	ldrh r1, [r0]
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_0804E85E:
	mov r4, #0
	mov r2, #1
	mov r8, r2
	add r0, r5, #0
	and r0, r2
	ldr r1, _0804E8C8 @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
	ldr r0, _0804E8CC @ =0x0201930C
	mov sl, r0
	ldr r1, _0804E8D0 @ =0x000007FF
	add r7, r1, #0
_0804E876:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _0804E8CC @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0804E930
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804E930
	add r0, r2, #0
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _0804E8B8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0804E8DC
	cmp r0, #0x17
	ble _0804E8D4
	cmp r0, #0x18
	beq _0804E8D8
	b _0804E8DC
	.align 2, 0
_0804E8B8: .4byte gCardStats
_0804E8BC: .4byte 0x0000052A
_0804E8C0: .4byte 0x00008073
_0804E8C4: .4byte gUnk_08624848
_0804E8C8: .4byte 0x00000D64
_0804E8CC: .4byte 0x0201930C
_0804E8D0: .4byte 0x000007FF
_0804E8D4:
	mov r0, #0
	b _0804E8EE
_0804E8D8:
	mov r0, #0xA
	b _0804E8EE
_0804E8DC:
	and r2, r7
	lsl r0, r2, #2
	ldr r2, _0804E918 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0804E8EE:
	cmp r0, #3
	bhi _0804E91C
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	add r1, r1, r6
	add r1, sl
	mov r0, r8
	ldrb r1, [r1, #8]
	and r0, r1
	cmp r0, #0
	bne _0804E91C
	mov r0, r9
	cmp r0, #0
	beq _0804E91C
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
	b _0804E930
_0804E918: .4byte gCardStats
_0804E91C:
	mov r0, #0xA6
	cmp r5, #0
	beq _0804E924
	ldr r0, _0804E944 @ =0x000080A6
_0804E924:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_0804E930:
	add r4, #1
	cmp r4, #4
	ble _0804E876
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0804E944: .4byte 0x000080A6
	thumb_func_end EndPhase_DestroyLowLevelMonsters

