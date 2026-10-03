	thumb_func_start AiChooseSummonWithTribute
AiChooseSummonWithTribute: @ 0x080580C8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	str r0, [sp, #0]
	mov r0, #1
	neg r0, r0
	str r0, [sp, #4]
	mov r1, #0
	str r1, [sp, #8]
	bl AiBackupDuelState
	bl AiSimSetAttackPositions
	bl AiSimBattlePhase
	mov r8, r0
	bl AiRestoreDuelState
	mov r2, #0
	mov r9, r2
	ldr r0, _0805816C @ =0x020192E4
	ldr r3, _08058170 @ =0x00000D66
	add r0, r0, r3
	ldrb r1, [r0]
	cmp r9, r1
	blt _08058104
	b _0805832C
_08058104:
	mov r2, r9
	lsl r0, r2, #2
	ldr r1, _08058174 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	mov r0, #1
	add r1, r6, #0
	bl CanSummonFromHand
	cmp r0, #0
	bne _08058120
	b _0805831E
_08058120:
	add r4, r6, #0
	ldr r3, _08058178 @ =0x000007FF
	and r4, r3
	lsl r0, r4, #1
	ldr r1, _0805817C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #2
	bl AiIsKeyCard
	cmp r0, #0
	beq _0805813A
	b _0805831E
_0805813A:
	add r0, r6, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	beq _08058146
	b _0805831E
_08058146:
	mov r7, #0
	mov r2, #0
	mov sl, r2
	mov r5, #0
	lsl r0, r4, #2
	ldr r3, _08058180 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805818C
	cmp r0, #0x17
	ble _08058184
	cmp r0, #0x18
	beq _08058188
	b _0805818C
_0805816C: .4byte 0x020192E4
_08058170: .4byte 0x00000D66
_08058174: .4byte 0x0201A6CC
_08058178: .4byte 0x000007FF
_0805817C: .4byte gCardIdToNumber
_08058180: .4byte gCardStats
_08058184:
	mov r0, #0
	b _080581A2
_08058188:
	mov r0, #0xA
	b _080581A2
_0805818C:
	add r0, r6, #0
	ldr r1, _080581CC @ =0x000007FF
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _080581D0 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_080581A2:
	cmp r0, #1
	blt _080581D4
	cmp r0, #4
	ble _08058202
	cmp r0, #6
	bgt _080581D4
	mov r4, #1
	neg r4, r4
	add r0, r4, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r5, r0, #0
	cmp r5, r4
	bne _080581C2
	mov r7, #1
_080581C2:
	mov r0, #8
	orr r5, r0
	lsl r0, r5, #0x10
	lsr r5, r0, #0x10
	b _08058202
_080581CC: .4byte 0x000007FF
_080581D0: .4byte gCardStats
_080581D4:
	mov r4, #1
	neg r4, r4
	add r0, r4, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r5, r0, #0
	mov r1, #0
	bl AiPickTributeMonster
	add r1, r0, #0
	cmp r5, r4
	beq _080581F2
	cmp r1, r4
	bne _080581F4
_080581F2:
	mov r7, #1
_080581F4:
	mov r0, #7
	and r1, r0
	mov r0, #8
	orr r1, r0
	lsl r0, r5, #0x14
	lsr r5, r0, #0x10
	orr r5, r1
_08058202:
	cmp r7, #0
	beq _08058208
	b _0805831E
_08058208:
	bl AiBackupDuelState
	mov r0, r9
	add r1, r5, #0
	bl AiSimSummon
	bl AiSimSetAttackPositions
	bl AiSimBattlePhase
	add r5, r0, #0
	mov r0, #1
	bl CountMonsters
	ldr r3, _080582B4 @ =0x02015F00
	mov r1, #0xD9
	lsl r1, r1, #5
	add r4, r3, r1
	mov r1, #7
	and r0, r1
	lsl r0, r0, #6
	ldr r2, _080582B8 @ =0xFFFFFE3F
	add r1, r2, #0
	ldrh r3, [r4]
	and r1, r3
	orr r1, r0
	strh r1, [r4]
	mov r0, #1
	bl CountMonsters
	mov r1, #7
	and r0, r1
	lsl r0, r0, #1
	mov r2, #0xF
	neg r2, r2
	add r1, r2, #0
	ldr r3, _080582BC @ =0x02017A21
	ldrb r3, [r3]
	and r1, r3
	orr r1, r0
	ldr r0, _080582BC @ =0x02017A21
	strb r1, [r0]
	bl AiRestoreDuelState
	cmp r8, r5
	bge _08058268
	mov r1, #1
	mov sl, r1
_08058268:
	cmp r5, r8
	bne _0805828A
	ldrb r2, [r4]
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1D
	ldr r3, [sp, #8]
	cmp r3, r0
	bge _0805828A
	mov r1, #0xE0
	lsl r1, r1, #1
	add r0, r1, #0
	ldrh r4, [r4]
	and r0, r4
	cmp r0, #0
	beq _0805828A
	mov r2, #1
	mov sl, r2
_0805828A:
	ldr r0, _080582C0 @ =0x02015EE8
	ldr r0, [r0, #4]
	mov r1, #0x80
	lsl r1, r1, #2
	and r0, r1
	cmp r0, #0
	beq _08058304
	ldr r3, _080582C4 @ =0x000007FF
	and r6, r3
	lsl r0, r6, #1
	ldr r1, _080582C8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080582CC @ =0x0000023D
	cmp r1, r0
	beq _080582D6
	cmp r1, r0
	bgt _080582D0
	cmp r1, #0x2F
	beq _080582D6
	b _08058304
_080582B4: .4byte 0x02015F00
_080582B8: .4byte 0xFFFFFE3F
_080582BC: .4byte 0x02017A21
_080582C0: .4byte 0x02015EE8
_080582C4: .4byte 0x000007FF
_080582C8: .4byte gCardIdToNumber
_080582CC: .4byte 0x0000023D
_080582D0:
	ldr r0, _08058344 @ =0x00000463
	cmp r1, r0
	bne _08058304
_080582D6:
	ldr r0, _08058348 @ =0x020192E4
	ldr r2, _0805834C @ =0x00000D64
	add r0, r0, r2
	ldrh r0, [r0]
	add r1, r0, r5
	mov r0, #0xFA
	lsl r0, r0, #2
	cmp r1, r0
	ble _08058304
	bl AiCountExodiaInDeck
	cmp r0, #0
	ble _08058304
	bl AiCountExodiaInGraveyard
	cmp r0, #0
	bne _08058304
	bl AiCountExodiaOnField
	cmp r0, #0
	bne _08058304
	mov r3, #1
	mov sl, r3
_08058304:
	mov r0, sl
	cmp r0, #0
	beq _0805831E
	cmp r5, #0
	ble _08058310
	mov r8, r5
_08058310:
	ldr r1, _08058350 @ =0x02017A20
	ldrb r1, [r1]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	str r0, [sp, #8]
	mov r2, r9
	str r2, [sp, #4]
_0805831E:
	mov r3, #1
	add r9, r3
	ldr r0, _08058354 @ =0x0201A04A
	ldrb r0, [r0]
	cmp r9, r0
	bge _0805832C
	b _08058104
_0805832C:
	mov r1, r8
	ldr r2, [sp, #0]
	str r1, [r2]
	ldr r0, [sp, #4]
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08058344: .4byte 0x00000463
_08058348: .4byte 0x020192E4
_0805834C: .4byte 0x00000D64
_08058350: .4byte 0x02017A20
_08058354: .4byte 0x0201A04A
	thumb_func_end AiChooseSummonWithTribute

