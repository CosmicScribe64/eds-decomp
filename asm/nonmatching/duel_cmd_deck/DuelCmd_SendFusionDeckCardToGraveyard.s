	thumb_func_start DuelCmd_SendFusionDeckCardToGraveyard
DuelCmd_SendFusionDeckCardToGraveyard: @ 0x0800FA04
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0xC
	ldr r5, _0800FA48 @ =0x020185C0
	ldrh r0, [r5]
	lsr r6, r0, #0xF
	ldrh r1, [r5, #4]
	lsl r0, r1, #0x10
	ldrh r2, [r5, #2]
	orr r0, r2
	str r0, [sp, #0]
	ldr r0, _0800FA4C @ =0x0000080A
	add r7, r5, r0
	ldrb r1, [r7]
	lsl r0, r1, #0x19
	lsr r4, r0, #0x19
	cmp r4, #0
	beq _0800FA54
	cmp r4, #1
	beq _0800FA5E
	ldr r2, _0800FA50 @ =0x00000814
	add r0, r5, r2
	bl AddCardToGraveyard
	bl DrawAllAreaTiles
	add r0, r6, #0
	mov r1, #0xC
	mov r2, #0
	bl DuelCursor_Select
	b _0800FAF0
_0800FA48: .4byte 0x020185C0
_0800FA4C: .4byte 0x0000080A
_0800FA50: .4byte 0x00000814
_0800FA54:
	add r0, r6, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	b _0800FAC6
_0800FA5E:
	add r0, r6, #0
	mov r1, sp
	bl RemoveCardFromFusionDeck
	cmp r0, #0
	beq _0800FAEC
	ldr r0, _0800FAE0 @ =0x00000814
	add r0, r0, r5
	mov r9, r0
	mov r1, sp
	bl CopyDuelCard
	and r6, r4
	mov r8, r6
	mov r6, #2
	neg r6, r6
	ldr r0, [sp, #4]
	and r0, r6
	mov r1, r8
	orr r0, r1
	mov r5, #0x1F
	neg r5, r5
	and r0, r5
	mov r1, #0x18
	orr r0, r1
	ldr r4, _0800FAE4 @ =0xFFFFC01F
	and r0, r4
	ldr r3, _0800FAE8 @ =0xFFFFBFFF
	and r0, r3
	mov r2, #0x80
	lsl r2, r2, #8
	orr r0, r2
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	and r0, r6
	mov r1, r8
	orr r0, r1
	and r0, r5
	mov r1, #0x1C
	orr r0, r1
	and r0, r4
	and r0, r3
	orr r0, r2
	str r0, [sp, #8]
	mov r2, r9
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r1, sp, #4
	add r2, sp, #8
	bl DuelAnim_MoveCard
_0800FAC6:
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
	b _0800FAFE
	.align 2, 0
_0800FAE0: .4byte 0x00000814
_0800FAE4: .4byte 0xFFFFC01F
_0800FAE8: .4byte 0xFFFFBFFF
_0800FAEC:
	bl DrawAllAreaTiles
_0800FAF0:
	ldr r0, _0800FB0C @ =0x0000080D
	add r1, r5, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800FAFE:
	add sp, #0xC
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800FB0C: .4byte 0x0000080D
	thumb_func_end DuelCmd_SendFusionDeckCardToGraveyard

