	thumb_func_start DuelCmd_PlantInOpponentDeck
DuelCmd_PlantInOpponentDeck: @ 0x080124E8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r4, _08012544 @ =0x020185C0
	ldrh r0, [r4]
	lsr r7, r0, #0xF
	ldrh r6, [r4, #2]
	sub r1, r7, #1
	mov sl, r1
	ldr r2, _08012548 @ =0x02018DCA
	ldrb r2, [r2]
	lsl r0, r2, #0x19
	lsr r5, r0, #0x19
	cmp r5, #0
	beq _08012560
	cmp r5, #1
	beq _08012570
	ldr r0, _0801254C @ =0x00000814
	add r1, r4, r0
	mov r0, sl
	bl AddCardToDeckTop
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r0, _08012550 @ =0x00000D64
	mul r0, r7
	add r1, r1, r0
	ldr r0, _08012554 @ =0x0201930C
	add r1, r1, r0
	ldr r0, _08012558 @ =0xFFFFF000
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	bl DrawAllAreaTiles
	ldr r0, _0801255C @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _0801263C
_08012544: .4byte 0x020185C0
_08012548: .4byte 0x02018DCA
_0801254C: .4byte 0x00000814
_08012550: .4byte 0x00000D64
_08012554: .4byte 0x0201930C
_08012558: .4byte 0xFFFFF000
_0801255C: .4byte 0x0000080D
_08012560:
	add r0, r6, #0
	bl GetZoneArea
	add r1, r0, #0
	add r0, r7, #0
	bl DuelScreen_ScrollToZone
	b _08012624
_08012570:
	add r0, r7, #0
	and r0, r5
	ldr r1, _08012650 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	mov r9, r2
	ldr r0, _08012654 @ =0x0201930C
	mov r8, r0
	mov r1, r9
	add r1, r8
	mov r0, #0x94
	add r4, r6, #0
	mul r4, r0
	add r1, r1, r4
	ldr r0, _08012658 @ =0x02018DD4
	bl CopyDuelCard
	mov r0, #2
	ldr r1, _08012658 @ =0x02018DD4
	ldrb r1, [r1, #2]
	orr r0, r1
	ldr r2, _08012658 @ =0x02018DD4
	strb r0, [r2, #2]
	add r0, r7, #0
	add r1, r6, #0
	bl ClearZoneTiles
	and r7, r5
	ldr r2, [sp, #0]
	mov r0, #2
	neg r0, r0
	and r2, r0
	orr r2, r7
	mov r1, #0x1F
	neg r1, r1
	mov ip, r1
	and r2, r1
	ldr r0, _0801265C @ =0x000001FF
	and r6, r0
	lsl r0, r6, #5
	ldr r7, _08012660 @ =0xFFFFC01F
	and r2, r7
	orr r2, r0
	str r2, [sp, #0]
	add r4, r9
	add r4, r8
	ldrb r1, [r4, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	and r0, r5
	lsl r0, r0, #0xE
	ldr r3, _08012664 @ =0xFFFFBFFF
	add r1, r3, #0
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	ldrb r4, [r4, #6]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	and r0, r5
	lsl r0, r0, #0xF
	ldr r2, _08012668 @ =0xFFFF7FFF
	and r1, r2
	orr r1, r0
	str r1, [sp, #0]
	mov r2, sl
	and r2, r5
	ldr r0, [sp, #4]
	mov r1, #2
	neg r1, r1
	and r0, r1
	orr r0, r2
	mov r2, ip
	and r0, r2
	mov r1, #0x1A
	orr r0, r1
	and r0, r7
	and r0, r3
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #4]
	ldr r1, _08012658 @ =0x02018DD4
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl DuelAnim_MoveCard
_08012624:
	ldr r0, _0801266C @ =0x02018DCA
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
	ldr r1, _0801266C @ =0x02018DCA
_0801263C:
	strb r0, [r1]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08012650: .4byte 0x00000D64
_08012654: .4byte 0x0201930C
_08012658: .4byte 0x02018DD4
_0801265C: .4byte 0x000001FF
_08012660: .4byte 0xFFFFC01F
_08012664: .4byte 0xFFFFBFFF
_08012668: .4byte 0xFFFF7FFF
_0801266C: .4byte 0x02018DCA
	thumb_func_end DuelCmd_PlantInOpponentDeck

