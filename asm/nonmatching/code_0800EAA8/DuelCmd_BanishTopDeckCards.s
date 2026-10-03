	thumb_func_start DuelCmd_BanishTopDeckCards
DuelCmd_BanishTopDeckCards: @ 0x0800F3D0
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	ldr r5, _0800F3FC @ =0x020185C0
	ldrh r0, [r5]
	lsr r4, r0, #0xF
	mov r8, r4
	ldr r1, _0800F400 @ =0x0000080A
	add r7, r5, r1
	ldrb r2, [r7]
	lsl r0, r2, #0x19
	lsr r6, r0, #0x19
	cmp r6, #1
	beq _0800F446
	cmp r6, #1
	bgt _0800F404
	cmp r6, #0
	beq _0800F40A
	b _0800F51C
	.align 2, 0
_0800F3FC: .4byte 0x020185C0
_0800F400: .4byte 0x0000080A
_0800F404:
	cmp r6, #2
	beq _0800F4D0
	b _0800F51C
_0800F40A:
	add r0, r4, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	mov r1, #0x7F
	ldrh r0, [r5, #2]
	and r1, r0
	lsl r1, r1, #7
	ldr r0, _0800F434 @ =0xFFFFC07F
	ldrh r2, [r7]
	and r0, r2
	orr r0, r1
	strh r0, [r7]
	lsl r0, r0, #0x12
	lsr r0, r0, #0x19
	cmp r0, #0
	bne _0800F43C
	ldr r0, _0800F438 @ =0x0000080D
	add r1, r5, r0
	b _0800F522
	.align 2, 0
_0800F434: .4byte 0xFFFFC07F
_0800F438: .4byte 0x0000080D
_0800F43C:
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	b _0800F502
_0800F446:
	ldr r0, _0800F4AC @ =0x00000814
	add r0, r0, r5
	mov r9, r0
	add r0, r4, #0
	mov r1, #0
	mov r2, r9
	bl TakeDeckCardAt
	cmp r0, #0
	beq _0800F4B8
	and r6, r4
	mov r5, #2
	neg r5, r5
	ldr r0, [sp, #0]
	and r0, r5
	orr r0, r6
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	mov r1, #0x1A
	orr r0, r1
	ldr r4, _0800F4B0 @ =0xFFFFC01F
	and r0, r4
	ldr r3, _0800F4B4 @ =0xFFFFBFFF
	and r0, r3
	mov r2, #0x80
	lsl r2, r2, #8
	orr r0, r2
	str r0, [sp, #0]
	ldr r0, [sp, #4]
	and r0, r5
	orr r0, r6
	mov r1, #0x1E
	orr r0, r1
	and r0, r4
	and r0, r3
	orr r0, r2
	str r0, [sp, #4]
	mov r1, r9
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl DuelAnim_MoveCard
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	b _0800F502
_0800F4AC: .4byte 0x00000814
_0800F4B0: .4byte 0xFFFFC01F
_0800F4B4: .4byte 0xFFFFBFFF
_0800F4B8:
	mov r0, r8
	mov r1, #0xD
	mov r2, #0
	bl DuelCursor_Select
	bl DrawAllAreaTiles
	ldr r2, _0800F4CC @ =0x0000080D
	add r1, r5, r2
	b _0800F522
_0800F4CC: .4byte 0x0000080D
_0800F4D0:
	ldr r1, _0800F514 @ =0x00000814
	add r0, r5, r1
	bl AddCardToBanished
	bl DrawAllAreaTiles
	ldrh r2, [r7]
	lsl r1, r2, #0x12
	lsr r1, r1, #0x19
	sub r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #7
	ldr r0, _0800F518 @ =0xFFFFC07F
	and r0, r2
	orr r0, r1
	strh r0, [r7]
	lsl r0, r0, #0x12
	lsr r0, r0, #0x19
	cmp r0, #0
	ble _0800F51C
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	sub r1, #1
_0800F502:
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
	b _0800F52C
	.align 2, 0
_0800F514: .4byte 0x00000814
_0800F518: .4byte 0xFFFFC07F
_0800F51C:
	ldr r1, _0800F53C @ =0x020185C0
	ldr r2, _0800F540 @ =0x0000080D
	add r1, r1, r2
_0800F522:
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800F52C:
	add sp, #8
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800F53C: .4byte 0x020185C0
_0800F540: .4byte 0x0000080D
	thumb_func_end DuelCmd_BanishTopDeckCards

