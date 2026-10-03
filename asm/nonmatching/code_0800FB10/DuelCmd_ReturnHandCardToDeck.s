	thumb_func_start DuelCmd_ReturnHandCardToDeck
DuelCmd_ReturnHandCardToDeck: @ 0x08010A5C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	ldr r4, _08010A8C @ =0x020185C0
	ldrh r0, [r4]
	lsr r7, r0, #0xF
	ldrh r2, [r4, #2]
	ldr r1, _08010A90 @ =0x0000080A
	add r1, r1, r4
	mov sl, r1
	ldrb r3, [r1]
	lsl r0, r3, #0x19
	lsr r6, r0, #0x19
	cmp r6, #1
	beq _08010ABC
	cmp r6, #1
	bgt _08010A94
	cmp r6, #0
	beq _08010A9A
	b _08010BAA
	.align 2, 0
_08010A8C: .4byte 0x020185C0
_08010A90: .4byte 0x0000080A
_08010A94:
	cmp r6, #2
	beq _08010B84
	b _08010BAA
_08010A9A:
	add r0, r7, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	mov r5, sl
	ldrb r2, [r5]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5]
	b _08010BBE
_08010ABC:
	ldr r0, _08010B68 @ =0x00000814
	add r0, r0, r4
	mov r9, r0
	add r0, r7, #0
	and r0, r6
	ldr r1, _08010B6C @ =0x00000D64
	add r3, r0, #0
	mul r3, r1
	str r3, [sp, #8]
	ldr r5, _08010B70 @ =0x02019968
	add r1, r3, r5
	lsl r4, r2, #2
	add r1, r1, r4
	mov r0, r9
	str r2, [sp, #0xC]
	bl CopyDuelCard
	ldr r0, [sp, #8]
	add r4, r4, r0
	add r4, r4, r5
	ldr r0, _08010B74 @ =0xFFFFF000
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	and r7, r6
	mov r3, #2
	neg r3, r3
	mov r8, r3
	ldr r0, [sp, #0]
	and r0, r3
	orr r0, r7
	mov r5, #0x1F
	neg r5, r5
	mov ip, r5
	and r0, r5
	mov r1, #0x16
	orr r0, r1
	ldr r1, _08010B78 @ =0x000001FF
	ldr r2, [sp, #0xC]
	and r2, r1
	lsl r1, r2, #5
	ldr r7, _08010B7C @ =0xFFFFC01F
	and r0, r7
	orr r0, r1
	ldr r4, _08010B80 @ =0xFFFFBFFF
	and r0, r4
	mov r3, #0x80
	lsl r3, r3, #8
	orr r0, r3
	str r0, [sp, #0]
	mov r1, r9
	ldr r0, [r1]
	lsl r2, r0, #0x13
	lsr r2, r2, #0x1F
	and r2, r6
	ldr r1, [sp, #4]
	mov r5, r8
	and r1, r5
	orr r1, r2
	mov r2, ip
	and r1, r2
	mov r2, #0x1A
	orr r1, r2
	and r1, r7
	and r1, r4
	orr r1, r3
	str r1, [sp, #4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl DuelAnim_MoveCard
	mov r3, sl
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
	b _08010BBE
_08010B68: .4byte 0x00000814
_08010B6C: .4byte 0x00000D64
_08010B70: .4byte 0x02019968
_08010B74: .4byte 0xFFFFF000
_08010B78: .4byte 0x000001FF
_08010B7C: .4byte 0xFFFFC01F
_08010B80: .4byte 0xFFFFBFFF
_08010B84:
	add r0, r7, #0
	bl CompactHand
	ldrh r0, [r4, #4]
	cmp r0, #0
	beq _08010BA0
	ldr r5, _08010B9C @ =0x00000814
	add r1, r4, r5
	add r0, r7, #0
	bl AddCardToDeckTop
	b _08010BAA
_08010B9C: .4byte 0x00000814
_08010BA0:
	ldr r0, _08010BD0 @ =0x00000814
	add r1, r4, r0
	add r0, r7, #0
	bl AddCardToDeckBottom
_08010BAA:
	bl DrawAllAreaTiles
	ldr r1, _08010BD4 @ =0x020185C0
	ldr r2, _08010BD8 @ =0x0000080D
	add r1, r1, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
_08010BBE:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08010BD0: .4byte 0x00000814
_08010BD4: .4byte 0x020185C0
_08010BD8: .4byte 0x0000080D
	thumb_func_end DuelCmd_ReturnHandCardToDeck

