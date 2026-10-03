	thumb_func_start DuelCmd_BanishMonsterUntilEndPhase
DuelCmd_BanishMonsterUntilEndPhase: @ 0x08012D7C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	ldr r4, _08012DC0 @ =0x020185C0
	ldrh r0, [r4]
	lsr r7, r0, #0xF
	ldrh r6, [r4, #2]
	ldr r1, _08012DC4 @ =0x0000080A
	add r1, r1, r4
	mov sl, r1
	ldrb r2, [r1]
	lsl r0, r2, #0x19
	lsr r5, r0, #0x19
	cmp r5, #0
	beq _08012DD0
	cmp r5, #1
	beq _08012DF2
	ldr r5, _08012DC8 @ =0x00000814
	add r0, r4, r5
	add r1, r6, #0
	bl AddCardToBanishedTemporarily
	bl DrawAllAreaTiles
	ldr r0, _08012DCC @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _08012EA2
_08012DC0: .4byte 0x020185C0
_08012DC4: .4byte 0x0000080A
_08012DC8: .4byte 0x00000814
_08012DCC: .4byte 0x0000080D
_08012DD0:
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
	b _08012EA4
_08012DF2:
	ldr r0, _08012EB4 @ =0x00000814
	add r0, r0, r4
	mov r8, r0
	add r0, r7, #0
	and r0, r5
	ldr r1, _08012EB8 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	str r2, [sp, #8]
	ldr r0, _08012EBC @ =0x0201930C
	mov r9, r0
	add r1, r2, #0
	add r1, r9
	mov r0, #0x94
	add r4, r6, #0
	mul r4, r0
	add r1, r1, r4
	mov r0, r8
	bl CopyDuelCard
	ldr r1, [sp, #8]
	add r4, r4, r1
	add r4, r9
	ldr r0, _08012EC0 @ =0xFFFFF000
	ldrh r2, [r4]
	and r0, r2
	strh r0, [r4]
	add r0, r7, #0
	add r1, r6, #0
	bl ClearZoneTiles
	and r7, r5
	mov r0, #2
	neg r0, r0
	mov r9, r0
	ldr r0, [sp, #0]
	mov r1, r9
	and r0, r1
	orr r0, r7
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	ldr r1, _08012EC4 @ =0x000001FF
	and r6, r1
	lsl r1, r6, #5
	ldr r4, _08012EC8 @ =0xFFFFC01F
	and r0, r4
	orr r0, r1
	ldr r3, _08012ECC @ =0xFFFFBFFF
	and r0, r3
	mov r1, #0x80
	lsl r1, r1, #8
	orr r0, r1
	str r0, [sp, #0]
	mov r2, r8
	ldr r0, [r2]
	lsl r2, r0, #0x13
	lsr r2, r2, #0x1F
	and r2, r5
	ldr r1, [sp, #4]
	mov r5, r9
	and r1, r5
	orr r1, r2
	mov r2, #0x1E
	orr r1, r2
	and r1, r4
	and r1, r3
	ldr r2, _08012ED0 @ =0xFFFF7FFF
	and r1, r2
	str r1, [sp, #4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl DuelAnim_MoveCard
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
_08012EA2:
	strb r0, [r1]
_08012EA4:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08012EB4: .4byte 0x00000814
_08012EB8: .4byte 0x00000D64
_08012EBC: .4byte 0x0201930C
_08012EC0: .4byte 0xFFFFF000
_08012EC4: .4byte 0x000001FF
_08012EC8: .4byte 0xFFFFC01F
_08012ECC: .4byte 0xFFFFBFFF
_08012ED0: .4byte 0xFFFF7FFF
	thumb_func_end DuelCmd_BanishMonsterUntilEndPhase

