	thumb_func_start DuelCmd_SendTopDeckCardsToGraveyard
DuelCmd_SendTopDeckCardsToGraveyard: @ 0x0800F294
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	ldr r1, _0800F2C0 @ =0x020185C0
	ldrh r0, [r1]
	lsr r5, r0, #0xF
	mov r8, r5
	ldr r2, _0800F2C4 @ =0x0000080A
	add r7, r1, r2
	ldrb r2, [r7]
	lsl r0, r2, #0x19
	lsr r4, r0, #0x19
	add r6, r1, #0
	cmp r4, #1
	beq _0800F2D8
	cmp r4, #1
	bgt _0800F2C8
	cmp r4, #0
	beq _0800F2CE
	b _0800F3B0
_0800F2C0: .4byte 0x020185C0
_0800F2C4: .4byte 0x0000080A
_0800F2C8:
	cmp r4, #2
	beq _0800F370
	b _0800F3B0
_0800F2CE:
	add r0, r5, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	b _0800F398
_0800F2D8:
	ldrh r0, [r6, #2]
	cmp r0, #0
	bne _0800F2E8
	ldr r0, _0800F2E4 @ =0x0000080D
	add r1, r6, r0
	b _0800F3B4
_0800F2E4: .4byte 0x0000080D
_0800F2E8:
	ldr r0, _0800F34C @ =0x00000814
	add r0, r0, r6
	mov r9, r0
	add r0, r5, #0
	mov r1, #0
	mov r2, r9
	bl TakeDeckCardAt
	cmp r0, #0
	beq _0800F358
	and r5, r4
	mov r8, r5
	mov r6, #2
	neg r6, r6
	ldr r0, [sp, #0]
	and r0, r6
	orr r0, r5
	mov r5, #0x1F
	neg r5, r5
	and r0, r5
	mov r1, #0x1A
	orr r0, r1
	ldr r4, _0800F350 @ =0xFFFFC01F
	and r0, r4
	ldr r3, _0800F354 @ =0xFFFFBFFF
	and r0, r3
	mov r2, #0x80
	lsl r2, r2, #8
	orr r0, r2
	str r0, [sp, #0]
	ldr r0, [sp, #4]
	and r0, r6
	mov r1, r8
	orr r0, r1
	and r0, r5
	mov r1, #0x1C
	orr r0, r1
	and r0, r4
	and r0, r3
	orr r0, r2
	str r0, [sp, #4]
	mov r2, r9
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl DuelAnim_MoveCard
	b _0800F398
_0800F34C: .4byte 0x00000814
_0800F350: .4byte 0xFFFFC01F
_0800F354: .4byte 0xFFFFBFFF
_0800F358:
	mov r0, r8
	mov r1, #0xD
	mov r2, #0
	bl DuelCursor_Select
	bl DrawAllAreaTiles
	ldr r0, _0800F36C @ =0x0000080D
	add r1, r6, r0
	b _0800F3B4
_0800F36C: .4byte 0x0000080D
_0800F370:
	ldr r1, _0800F394 @ =0x00000814
	add r0, r6, r1
	bl AddCardToGraveyard
	bl DrawAllAreaTiles
	ldrh r0, [r6, #2]
	sub r0, #1
	strh r0, [r6, #2]
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0800F398
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	sub r1, #1
	b _0800F3A0
	.align 2, 0
_0800F394: .4byte 0x00000814
_0800F398:
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
_0800F3A0:
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
	b _0800F3BE
_0800F3B0:
	ldr r2, _0800F3CC @ =0x0000080D
	add r1, r6, r2
_0800F3B4:
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800F3BE:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800F3CC: .4byte 0x0000080D
	thumb_func_end DuelCmd_SendTopDeckCardsToGraveyard

