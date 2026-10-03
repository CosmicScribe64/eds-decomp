	thumb_func_start DuelCmd_BanishDeckCard
DuelCmd_BanishDeckCard: @ 0x0800F8F8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0xC
	ldr r4, _0800F940 @ =0x020185C0
	ldrh r0, [r4]
	lsr r5, r0, #0xF
	ldrh r1, [r4, #4]
	lsl r0, r1, #0x10
	ldrh r2, [r4, #2]
	orr r0, r2
	str r0, [sp, #0]
	ldr r0, _0800F944 @ =0x0000080A
	add r7, r4, r0
	ldrb r1, [r7]
	lsl r0, r1, #0x19
	lsr r6, r0, #0x19
	cmp r6, #0
	beq _0800F950
	cmp r6, #1
	beq _0800F95A
	ldr r2, _0800F948 @ =0x00000814
	add r0, r4, r2
	bl AddCardToBanished
	bl DrawAllAreaTiles
	add r0, r5, #0
	mov r1, #0xF
	mov r2, #0
	bl DuelCursor_Select
	ldr r0, _0800F94C @ =0x0000080D
	add r1, r4, r0
	b _0800F9E8
	.align 2, 0
_0800F940: .4byte 0x020185C0
_0800F944: .4byte 0x0000080A
_0800F948: .4byte 0x00000814
_0800F94C: .4byte 0x0000080D
_0800F950:
	add r0, r5, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	b _0800F9BA
_0800F95A:
	add r0, r5, #0
	mov r1, sp
	bl RemoveCardFromDeck
	cmp r0, #0
	beq _0800F9E0
	ldr r0, _0800F9D4 @ =0x00000814
	add r0, r0, r4
	mov r8, r0
	mov r1, sp
	bl CopyDuelCard
	and r6, r5
	mov r5, #2
	neg r5, r5
	ldr r0, [sp, #4]
	and r0, r5
	orr r0, r6
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	mov r1, #0x1A
	orr r0, r1
	ldr r4, _0800F9D8 @ =0xFFFFC01F
	and r0, r4
	ldr r3, _0800F9DC @ =0xFFFFBFFF
	and r0, r3
	mov r2, #0x80
	lsl r2, r2, #8
	orr r0, r2
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	and r0, r5
	orr r0, r6
	mov r1, #0x1E
	orr r0, r1
	and r0, r4
	and r0, r3
	orr r0, r2
	str r0, [sp, #8]
	mov r1, r8
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r1, sp, #4
	add r2, sp, #8
	bl DuelAnim_MoveCard
_0800F9BA:
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
	b _0800F9F2
	.align 2, 0
_0800F9D4: .4byte 0x00000814
_0800F9D8: .4byte 0xFFFFC01F
_0800F9DC: .4byte 0xFFFFBFFF
_0800F9E0:
	bl DrawAllAreaTiles
	ldr r2, _0800FA00 @ =0x0000080D
	add r1, r4, r2
_0800F9E8:
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800F9F2:
	add sp, #0xC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800FA00: .4byte 0x0000080D
	thumb_func_end DuelCmd_BanishDeckCard

