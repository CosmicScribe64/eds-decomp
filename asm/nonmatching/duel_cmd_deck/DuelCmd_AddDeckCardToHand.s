	thumb_func_start DuelCmd_AddDeckCardToHand
DuelCmd_AddDeckCardToHand: @ 0x0800F544
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	ldr r0, _0800F59C @ =0x020185C0
	mov r9, r0
	ldrh r1, [r0]
	lsr r7, r1, #0xF
	ldrh r2, [r0, #4]
	lsl r1, r2, #0x10
	ldrh r3, [r0, #2]
	orr r1, r3
	str r1, [sp, #0]
	ldr r0, _0800F5A0 @ =0x0000080A
	add r0, r9
	mov sl, r0
	ldrb r2, [r0]
	lsl r0, r2, #0x19
	lsr r6, r0, #0x19
	cmp r6, #0
	beq _0800F5A8
	cmp r6, #1
	beq _0800F5B2
	add r0, r7, #0
	mov r1, sp
	bl AddCardToHand
	bl DrawAllAreaTiles
	add r0, r7, #0
	mov r1, #0xB
	mov r2, #0
	bl DuelCursor_Select
	ldr r1, _0800F5A4 @ =0x0000080D
	add r1, r9
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0800F652
	.align 2, 0
_0800F59C: .4byte 0x020185C0
_0800F5A0: .4byte 0x0000080A
_0800F5A4: .4byte 0x0000080D
_0800F5A8:
	add r0, r7, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	b _0800F63A
_0800F5B2:
	lsl r0, r1, #0x13
	lsr r0, r0, #0x1F
	mov r1, sp
	bl RemoveCardFromDeck
	cmp r0, #0
	bne _0800F5D4
	ldr r1, _0800F5D0 @ =0x0000080D
	add r1, r9
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _0800F652
	.align 2, 0
_0800F5D0: .4byte 0x0000080D
_0800F5D4:
	add r2, r7, #0
	and r2, r6
	mov r4, #2
	neg r4, r4
	ldr r0, [sp, #4]
	and r0, r4
	orr r0, r2
	mov r3, #0x1F
	neg r3, r3
	and r0, r3
	mov r1, #0x1A
	orr r0, r1
	ldr r1, _0800F664 @ =0xFFFFC01F
	mov ip, r1
	and r0, r1
	sub r1, #0x20
	mov r8, r1
	and r0, r1
	ldr r5, _0800F668 @ =0xFFFF7FFF
	and r0, r5
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	and r0, r4
	orr r0, r2
	and r0, r3
	mov r1, #0x16
	orr r0, r1
	str r0, [sp, #8]
	ldr r2, _0800F66C @ =0x020192E4
	and r7, r6
	ldr r1, _0800F670 @ =0x00000D64
	mul r1, r7
	add r1, r1, r2
	ldrb r1, [r1, #2]
	lsl r1, r1, #5
	mov r2, ip
	and r0, r2
	orr r0, r1
	mov r3, r8
	and r0, r3
	and r0, r5
	str r0, [sp, #8]
	ldr r0, _0800F674 @ =0x00000814
	add r0, r9
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r1, sp, #4
	add r2, sp, #8
	bl DuelAnim_MoveCard
_0800F63A:
	mov r0, sl
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
	mov r1, sl
_0800F652:
	strb r0, [r1]
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800F664: .4byte 0xFFFFC01F
_0800F668: .4byte 0xFFFF7FFF
_0800F66C: .4byte 0x020192E4
_0800F670: .4byte 0x00000D64
_0800F674: .4byte 0x00000814
	thumb_func_end DuelCmd_AddDeckCardToHand

