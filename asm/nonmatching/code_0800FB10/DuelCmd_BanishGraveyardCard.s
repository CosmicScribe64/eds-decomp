	thumb_func_start DuelCmd_BanishGraveyardCard
DuelCmd_BanishGraveyardCard: @ 0x08010038
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0xC
	ldr r7, _08010088 @ =0x020185C0
	ldrh r0, [r7]
	lsr r4, r0, #0xF
	ldrh r1, [r7, #4]
	lsl r0, r1, #0x10
	ldrh r2, [r7, #2]
	orr r0, r2
	str r0, [sp, #0]
	ldr r0, _0801008C @ =0x0000080A
	add r0, r0, r7
	mov r8, r0
	ldrb r1, [r0]
	lsl r0, r1, #0x19
	lsr r6, r0, #0x19
	cmp r6, #0
	beq _08010094
	cmp r6, #1
	beq _0801009E
	mov r0, sp
	bl AddCardToBanished
	bl DrawAllAreaTiles
	add r0, r4, #0
	mov r1, #0xF
	mov r2, #0
	bl DuelCursor_Select
	ldr r2, _08010090 @ =0x0000080D
	add r1, r7, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _08010108
	.align 2, 0
_08010088: .4byte 0x020185C0
_0801008C: .4byte 0x0000080A
_08010090: .4byte 0x0000080D
_08010094:
	add r0, r4, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	b _080100F0
_0801009E:
	add r0, r4, #0
	mov r1, sp
	bl RemoveCardFromGraveyard
	and r6, r4
	mov r5, #2
	neg r5, r5
	ldr r0, [sp, #4]
	and r0, r5
	orr r0, r6
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	mov r1, #0x1C
	orr r0, r1
	ldr r4, _08010118 @ =0xFFFFC01F
	and r0, r4
	ldr r3, _0801011C @ =0xFFFFBFFF
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
	ldr r2, _08010120 @ =0x00000814
	add r0, r7, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r1, sp, #4
	add r2, sp, #8
	bl DuelAnim_MoveCard
_080100F0:
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
_08010108:
	strb r0, [r1]
	add sp, #0xC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08010118: .4byte 0xFFFFC01F
_0801011C: .4byte 0xFFFFBFFF
_08010120: .4byte 0x00000814
	thumb_func_end DuelCmd_BanishGraveyardCard

