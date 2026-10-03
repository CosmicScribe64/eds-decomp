	thumb_func_start DuelCmd_SetMagicalHatsCard
DuelCmd_SetMagicalHatsCard: @ 0x08012C4C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0xC
	ldr r1, _08012D30 @ =0x020185C0
	ldrh r0, [r1]
	lsr r6, r0, #0xF
	ldrb r5, [r1, #2]
	ldrh r2, [r1, #2]
	lsr r4, r2, #8
	mov r0, #1
	and r4, r0
	ldrh r2, [r1, #6]
	lsl r0, r2, #0x10
	ldrh r2, [r1, #4]
	orr r0, r2
	str r0, [sp, #4]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r2, _08012D34 @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	ldr r0, _08012D38 @ =0x0000077F
	add r7, r1, #0
	cmp r2, r0
	bls _08012C88
	add r0, #0x50
	cmp r2, r0
	bhi _08012C88
	mov r4, #1
_08012C88:
	ldr r0, _08012D3C @ =0x0000080A
	add r0, r0, r7
	mov r8, r0
	ldrb r1, [r0]
	lsl r0, r1, #0x19
	cmp r0, #0
	bne _08012D58
	str r4, [sp, #0]
	add r0, r6, #0
	add r1, r5, #0
	add r2, sp, #4
	mov r3, #1
	bl PlaceMonsterCard
	ldr r0, [sp, #4]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r2, _08012D40 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _08012CD6
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08012D44 @ =0x00000D64
	mul r0, r6
	add r1, r1, r0
	ldr r0, _08012D48 @ =0x0201930C
	add r1, r1, r0
	add r1, #0x8C
	mov r0, #2
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_08012CD6:
	mov r0, #0x10
	bl PlaySE
	mov r1, #2
	neg r1, r1
	ldr r0, [sp, #8]
	and r0, r1
	orr r0, r6
	sub r1, #0x1D
	and r0, r1
	lsl r2, r5, #5
	ldr r1, _08012D4C @ =0xFFFFC01F
	and r0, r1
	orr r0, r2
	mov r1, #0x80
	lsl r1, r1, #7
	orr r0, r1
	lsl r2, r4, #0xF
	ldr r1, _08012D50 @ =0xFFFF7FFF
	and r0, r1
	orr r0, r2
	str r0, [sp, #8]
	add r0, sp, #8
	ldr r1, _08012D54 @ =0x0869771C
	mov r2, #0
	mov r3, #0
	bl DuelAnim_PlayZoneEffect
	add r0, r6, #0
	add r1, r5, #0
	bl ClearZoneTiles
	mov r0, r8
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	mov r1, r8
	b _08012D68
_08012D30: .4byte 0x020185C0
_08012D34: .4byte gCardIdToNumber
_08012D38: .4byte 0x0000077F
_08012D3C: .4byte 0x0000080A
_08012D40: .4byte gCardStats
_08012D44: .4byte 0x00000D64
_08012D48: .4byte 0x0201930C
_08012D4C: .4byte 0xFFFFC01F
_08012D50: .4byte 0xFFFF7FFF
_08012D54: .4byte gSmokePuffAnim
_08012D58:
	bl DrawAllAreaTiles
	ldr r2, _08012D78 @ =0x0000080D
	add r1, r7, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_08012D68:
	strb r0, [r1]
	add sp, #0xC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08012D78: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetMagicalHatsCard

