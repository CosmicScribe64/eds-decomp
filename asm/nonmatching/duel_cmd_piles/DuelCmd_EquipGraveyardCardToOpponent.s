	thumb_func_start DuelCmd_EquipGraveyardCardToOpponent
DuelCmd_EquipGraveyardCardToOpponent: @ 0x08010538
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r4, _080105B4 @ =0x020185C0
	ldrh r0, [r4]
	lsr r7, r0, #0xF
	ldrh r1, [r4, #2]
	ldr r2, _080105B8 @ =0x0000080A
	add r2, r2, r4
	mov sl, r2
	ldrb r3, [r2]
	lsl r0, r3, #0x19
	lsr r6, r0, #0x19
	cmp r6, #0
	beq _080105C4
	cmp r6, #1
	beq _080105E6
	mov r0, #1
	sub r0, r0, r7
	ldrh r5, [r2]
	lsl r1, r5, #0x12
	lsr r1, r1, #0x19
	sub r1, #5
	ldr r3, _080105BC @ =0x00000814
	add r2, r4, r3
	mov r3, #1
	bl PlaceSpellTrapCard
	mov r2, #1
	sub r2, r2, r7
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	ldr r5, _080105C0 @ =0x00000817
	add r0, r4, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1D
	lsl r0, r0, #8
	orr r0, r2
	mov r3, sl
	ldrh r3, [r3]
	lsl r1, r3, #0x12
	lsr r1, r1, #0x19
	lsl r1, r1, #8
	orr r1, r2
	mov r2, #0xA
	bl AddZoneLink
	bl DrawAllAreaTiles
	sub r5, #0xA
	add r1, r4, r5
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _08010698
	.align 2, 0
_080105B4: .4byte 0x020185C0
_080105B8: .4byte 0x0000080A
_080105BC: .4byte 0x00000814
_080105C0: .4byte 0x00000817
_080105C4:
	add r0, r7, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
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
	b _08010698
_080105E6:
	ldr r5, _080106A8 @ =0x00000814
	add r5, r5, r4
	mov r9, r5
	add r0, r7, #0
	mov r2, r9
	bl TakeGraveyardCardAt
	ldr r0, _080106AC @ =0x00000817
	add r1, r4, r0
	mov r4, #2
	neg r4, r4
	add r0, r4, #0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	sub r0, r6, r7
	bl FindFreeSpellTrapZone
	mov r1, #0x7F
	and r0, r1
	lsl r0, r0, #7
	ldr r2, _080106B0 @ =0xFFFFC07F
	mov r3, sl
	ldrh r3, [r3]
	and r2, r3
	orr r2, r0
	mov r5, sl
	strh r2, [r5]
	add r1, r7, #0
	and r1, r6
	ldr r0, [sp, #0]
	and r0, r4
	orr r0, r1
	mov r3, #0x1F
	neg r3, r3
	and r0, r3
	mov r1, #0x1C
	orr r0, r1
	ldr r1, _080106B4 @ =0xFFFFC01F
	mov ip, r1
	and r0, r1
	ldr r5, _080106B8 @ =0xFFFFBFFF
	mov r8, r5
	and r0, r5
	mov r5, #0x80
	lsl r5, r5, #8
	orr r0, r5
	str r0, [sp, #0]
	mov r1, #1
	sub r1, r1, r7
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	and r1, r6
	ldr r0, [sp, #4]
	and r0, r4
	orr r0, r1
	and r0, r3
	mov r1, #0xA
	orr r0, r1
	lsl r2, r2, #0x12
	lsr r2, r2, #0x19
	lsl r2, r2, #5
	mov r1, ip
	and r0, r1
	orr r0, r2
	mov r2, r8
	and r0, r2
	orr r0, r5
	str r0, [sp, #4]
	mov r3, r9
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl DuelAnim_MoveCard
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
_08010698:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080106A8: .4byte 0x00000814
_080106AC: .4byte 0x00000817
_080106B0: .4byte 0xFFFFC07F
_080106B4: .4byte 0xFFFFC01F
_080106B8: .4byte 0xFFFFBFFF
	thumb_func_end DuelCmd_EquipGraveyardCardToOpponent

