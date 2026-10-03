	thumb_func_start DuelCmd_PlaceMonsterFromHand
DuelCmd_PlaceMonsterFromHand: @ 0x08010C14
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	ldr r4, _08010C68 @ =0x020185C0
	ldrh r0, [r4]
	lsr r0, r0, #0xF
	mov r8, r0
	ldrh r1, [r4, #2]
	str r1, [sp, #0xC]
	ldrh r1, [r4, #4]
	add r2, r1, #0
	mov r3, #0xF
	and r3, r2
	str r3, [sp, #0x10]
	mov r0, #0xF0
	and r0, r2
	lsr r0, r0, #4
	mov r9, r0
	lsr r1, r1, #8
	mov r0, #1
	and r0, r1
	str r0, [sp, #0x14]
	mov r0, #2
	and r1, r0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x19
	mov sl, r1
	ldr r1, _08010C6C @ =0x02018DCA
	ldrb r1, [r1]
	lsl r0, r1, #0x19
	lsr r7, r0, #0x19
	cmp r7, #1
	beq _08010C80
	cmp r7, #1
	bgt _08010C70
	cmp r7, #0
	beq _08010C76
	b _08010D64
	.align 2, 0
_08010C68: .4byte 0x020185C0
_08010C6C: .4byte 0x02018DCA
_08010C70:
	cmp r7, #2
	beq _08010D4C
	b _08010D64
_08010C76:
	mov r0, r8
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	b _08010D10
_08010C80:
	ldr r1, _08010D2C @ =0x00000814
	add r0, r4, r1
	mov r1, r8
	and r1, r7
	ldr r2, _08010D30 @ =0x00000D64
	add r5, r1, #0
	mul r5, r2
	ldr r6, _08010D34 @ =0x02019968
	add r1, r5, r6
	mov r2, r9
	lsl r4, r2, #2
	add r1, r1, r4
	bl CopyDuelCard
	add r4, r4, r5
	add r4, r4, r6
	ldr r0, _08010D38 @ =0xFFFFF000
	ldrh r3, [r4]
	and r0, r3
	strh r0, [r4]
	add r2, r7, #0
	mov r0, r8
	and r2, r0
	mov r1, #2
	neg r1, r1
	mov ip, r1
	ldr r0, [sp, #4]
	and r0, r1
	orr r0, r2
	mov r3, #0x1F
	neg r3, r3
	mov r8, r3
	and r0, r3
	mov r1, #0x16
	orr r0, r1
	mov r3, r9
	lsl r1, r3, #5
	ldr r4, _08010D3C @ =0xFFFFC01F
	and r0, r4
	orr r0, r1
	ldr r5, _08010D40 @ =0xFFFFBFFF
	and r0, r5
	ldr r1, [sp, #0x14]
	and r1, r7
	lsl r6, r1, #0xF
	ldr r3, _08010D44 @ =0xFFFF7FFF
	and r0, r3
	orr r0, r6
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	mov r1, ip
	and r0, r1
	orr r0, r2
	mov r2, r8
	and r0, r2
	ldr r2, [sp, #0x10]
	lsl r1, r2, #5
	and r0, r4
	orr r0, r1
	mov r1, sl
	and r1, r7
	lsl r1, r1, #0xE
	and r0, r5
	orr r0, r1
	and r0, r3
	orr r0, r6
	str r0, [sp, #8]
	add r2, sp, #8
	ldr r0, [sp, #0xC]
	add r1, sp, #4
	bl DuelAnim_MoveCard
_08010D10:
	ldr r3, _08010D48 @ =0x02018DCA
	ldrb r2, [r3]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _08010D78
	.align 2, 0
_08010D2C: .4byte 0x00000814
_08010D30: .4byte 0x00000D64
_08010D34: .4byte 0x02019968
_08010D38: .4byte 0xFFFFF000
_08010D3C: .4byte 0xFFFFC01F
_08010D40: .4byte 0xFFFFBFFF
_08010D44: .4byte 0xFFFF7FFF
_08010D48: .4byte 0x02018DCA
_08010D4C:
	mov r0, r8
	bl CompactHand
	ldr r0, _08010D88 @ =0x00000814
	add r2, r4, r0
	ldr r1, [sp, #0x14]
	str r1, [sp, #0]
	mov r0, r8
	ldr r1, [sp, #0x10]
	mov r3, sl
	bl PlaceMonsterCard
_08010D64:
	bl DrawAllAreaTiles
	ldr r1, _08010D8C @ =0x020185C0
	ldr r2, _08010D90 @ =0x0000080D
	add r1, r1, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
_08010D78:
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08010D88: .4byte 0x00000814
_08010D8C: .4byte 0x020185C0
_08010D90: .4byte 0x0000080D
	thumb_func_end DuelCmd_PlaceMonsterFromHand

