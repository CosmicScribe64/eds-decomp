	thumb_func_start AiChooseSummonNoTribute
AiChooseSummonNoTribute: @ 0x08058358
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov r0, #1
	neg r0, r0
	str r0, [sp, #0]
	mov r1, #0
	str r1, [sp, #4]
	bl AiBackupDuelState
	bl AiSimSetAttackPositions
	bl AiSimBattlePhase
	mov r8, r0
	bl AiRestoreDuelState
	mov r6, #0
	ldr r0, _08058400 @ =0x020192E4
	ldr r2, _08058404 @ =0x00000D66
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r6, r0
	blt _08058390
	b _080584EA
_08058390:
	ldr r3, _08058408 @ =0x02015F00
	mov sl, r3
	ldr r0, _0805840C @ =0x00001B21
	add r0, sl
	mov r9, r0
_0805839A:
	lsl r0, r6, #2
	ldr r1, _08058410 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	mov r0, #1
	add r1, r5, #0
	bl CanSummonFromHand
	cmp r0, #0
	bne _080583B4
	b _080584DA
_080583B4:
	ldr r1, _08058414 @ =0x000007FF
	add r0, r1, #0
	add r4, r5, #0
	and r4, r0
	lsl r0, r4, #1
	ldr r2, _08058418 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, #2
	bl AiIsKeyCard
	cmp r0, #0
	beq _080583D0
	b _080584DA
_080583D0:
	add r0, r5, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	beq _080583DC
	b _080584DA
_080583DC:
	mov r2, #0
	mov r7, #0
	lsl r0, r4, #2
	ldr r3, _0805841C @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08058428
	cmp r0, #0x17
	ble _08058420
	cmp r0, #0x18
	beq _08058424
	b _08058428
	.align 2, 0
_08058400: .4byte 0x020192E4
_08058404: .4byte 0x00000D66
_08058408: .4byte 0x02015F00
_0805840C: .4byte 0x00001B21
_08058410: .4byte 0x0201A6CC
_08058414: .4byte 0x000007FF
_08058418: .4byte gCardIdToNumber
_0805841C: .4byte gCardStats
_08058420:
	mov r0, #0
	b _0805843E
_08058424:
	mov r0, #0xA
	b _0805843E
_08058428:
	ldr r1, _080584FC @ =0x000007FF
	add r0, r1, #0
	and r5, r0
	lsl r0, r5, #2
	ldr r3, _08058500 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0805843E:
	cmp r0, #4
	bls _08058444
	mov r2, #1
_08058444:
	cmp r2, #0
	bne _080584DA
	bl AiBackupDuelState
	add r0, r6, #0
	mov r1, #0
	bl AiSimSummon
	bl AiSimSetAttackPositions
	bl AiSimBattlePhase
	add r4, r0, #0
	mov r0, #1
	bl CountMonsters
	mov r5, #0xD9
	lsl r5, r5, #5
	add r5, sl
	mov r1, #7
	and r0, r1
	lsl r0, r0, #6
	ldr r2, _08058504 @ =0xFFFFFE3F
	add r1, r2, #0
	ldrh r3, [r5]
	and r1, r3
	orr r1, r0
	strh r1, [r5]
	mov r0, #1
	bl CountMonsters
	mov r1, #7
	and r0, r1
	lsl r0, r0, #1
	mov r2, #0xF
	neg r2, r2
	add r1, r2, #0
	mov r3, r9
	ldrb r3, [r3]
	and r1, r3
	orr r1, r0
	mov r0, r9
	strb r1, [r0]
	bl AiRestoreDuelState
	cmp r8, r4
	bge _080584A4
	mov r7, #1
_080584A4:
	cmp r4, r8
	bne _080584C4
	ldrb r1, [r5]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	ldr r2, [sp, #4]
	cmp r2, r0
	bge _080584C4
	mov r3, #0xE0
	lsl r3, r3, #1
	add r0, r3, #0
	ldrh r5, [r5]
	and r0, r5
	cmp r0, #0
	beq _080584C4
	mov r7, #1
_080584C4:
	cmp r7, #0
	beq _080584DA
	cmp r4, #0
	ble _080584CE
	mov r8, r4
_080584CE:
	ldr r1, _08058508 @ =0x02017A20
	ldrb r1, [r1]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	str r0, [sp, #4]
	str r6, [sp, #0]
_080584DA:
	add r6, #1
	ldr r0, _0805850C @ =0x020192E4
	ldr r2, _08058510 @ =0x00000D66
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r6, r0
	bge _080584EA
	b _0805839A
_080584EA:
	ldr r0, [sp, #0]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_080584FC: .4byte 0x000007FF
_08058500: .4byte gCardStats
_08058504: .4byte 0xFFFFFE3F
_08058508: .4byte 0x02017A20
_0805850C: .4byte 0x020192E4
_08058510: .4byte 0x00000D66
	thumb_func_end AiChooseSummonNoTribute

