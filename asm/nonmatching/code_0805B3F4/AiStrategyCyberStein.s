	thumb_func_start AiStrategyCyberStein
AiStrategyCyberStein: @ 0x0805C0A0
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	ldr r0, _0805C0BC @ =0x02015EF0
	ldrb r1, [r0, #2]
	add r2, r0, #0
	cmp r1, #9
	bls _0805C0B0
	b _0805C4FC
_0805C0B0:
	lsl r0, r1, #2
	ldr r1, _0805C0C0 @ =0x0805C0C4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0805C0BC: .4byte 0x02015EF0
_0805C0C0: .4byte 0x0805C0C4
_0805C0C4:
	.4byte _0805C0EC
	.4byte _0805C158
	.4byte _0805C188
	.4byte _0805C158
	.4byte _0805C238
	.4byte _0805C2D8
	.4byte _0805C390
	.4byte _0805C3A4
	.4byte _0805C46C
	.4byte _0805C4C8
_0805C0EC:
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl CountSpellTrapsFiltered
	cmp r0, #0
	ble _0805C146
	ldr r0, _0805C11C @ =0x0000029F
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805C130
	ldr r2, _0805C120 @ =0x020192E0
	ldr r0, _0805C124 @ =0x00001B30
	add r1, r2, r0
	ldr r0, _0805C128 @ =0xFFFFFC03
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r5, _0805C12C @ =0x02015EF0
	ldrb r6, [r5, #0xB]
	lsl r0, r6, #2
	b _0805C1BC
_0805C11C: .4byte 0x0000029F
_0805C120: .4byte 0x020192E0
_0805C124: .4byte 0x00001B30
_0805C128: .4byte 0xFFFFFC03
_0805C12C: .4byte 0x02015EF0
_0805C130:
	ldr r0, _0805C150 @ =0x00000425
	bl AiTryPlaySpellTrap
	cmp r0, #0
	bne _0805C1A8
	mov r0, #0x87
	lsl r0, r0, #3
	bl AiTryPlaySpellTrap
	cmp r0, #0
	bne _0805C1A8
_0805C146:
	ldr r1, _0805C154 @ =0x02015EF0
	ldrb r0, [r1, #2]
	add r0, #2
	b _0805C4EC
	.align 2, 0
_0805C150: .4byte 0x00000425
_0805C154: .4byte 0x02015EF0
_0805C158:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl CardMenu_PlaySpellTrapFromHand
	ldr r1, _0805C17C @ =0x020192E0
	ldr r2, _0805C180 @ =0x00001B2C
	add r1, r1, r2
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0805C1FC
	ldr r1, _0805C184 @ =0x02015EF0
	ldrb r0, [r1, #2]
	sub r0, #1
	strb r0, [r1, #2]
	b _0805C1FC
_0805C17C: .4byte 0x020192E0
_0805C180: .4byte 0x00001B2C
_0805C184: .4byte 0x02015EF0
_0805C188:
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	ble _0805C22C
	ldr r0, _0805C200 @ =0x0000014F
	bl AiTryPlaySpellTrap
	cmp r0, #0
	bne _0805C1A8
	mov r0, #0xA8
	lsl r0, r0, #1
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805C22C
_0805C1A8:
	ldr r2, _0805C204 @ =0x020192E0
	ldr r3, _0805C208 @ =0x00001B30
	add r1, r2, r3
	ldr r0, _0805C20C @ =0xFFFFFC03
	ldrh r6, [r1]
	and r0, r6
	strh r0, [r1]
	ldr r5, _0805C210 @ =0x02015EF0
	ldrb r1, [r5, #0xB]
	lsl r0, r1, #2
_0805C1BC:
	ldr r3, _0805C214 @ =0x000013EC
	add r1, r2, r3
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r6, _0805C218 @ =0x00001B28
	add r1, r2, r6
	strh r0, [r1]
	ldr r0, _0805C21C @ =0x00001B33
	add r1, r2, r0
	mov r3, #2
	ldrb r0, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r1, _0805C220 @ =0x00001B34
	add r4, r2, r1
	ldrb r6, [r5, #0xB]
	lsl r1, r6, #1
	ldr r0, _0805C224 @ =0xFFFFFE01
	ldrh r6, [r4]
	and r0, r6
	orr r0, r1
	strh r0, [r4]
	ldr r0, _0805C228 @ =0x00001B2C
	add r2, r2, r0
	ldrb r1, [r2]
	orr r3, r1
	strb r3, [r2]
	ldrb r0, [r5, #2]
	add r0, #1
	strb r0, [r5, #2]
_0805C1FC:
	mov r0, #0
	b _0805C4FE
_0805C200: .4byte 0x0000014F
_0805C204: .4byte 0x020192E0
_0805C208: .4byte 0x00001B30
_0805C20C: .4byte 0xFFFFFC03
_0805C210: .4byte 0x02015EF0
_0805C214: .4byte 0x000013EC
_0805C218: .4byte 0x00001B28
_0805C21C: .4byte 0x00001B33
_0805C220: .4byte 0x00001B34
_0805C224: .4byte 0xFFFFFE01
_0805C228: .4byte 0x00001B2C
_0805C22C:
	ldr r1, _0805C234 @ =0x02015EF0
	ldrb r0, [r1, #2]
	add r0, #2
	b _0805C4EC
_0805C234: .4byte 0x02015EF0
_0805C238:
	mov r0, #1
	bl FindFreeMonsterZone
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _0805C260
	ldr r1, _0805C258 @ =0x02015F00
	ldr r2, _0805C25C @ =0x00001B24
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0805C45E
	.align 2, 0
_0805C258: .4byte 0x02015F00
_0805C25C: .4byte 0x00001B24
_0805C260:
	mov r0, #1
	bl FindFreeMonsterZone
	ldr r1, _0805C2B4 @ =0x02015F00
	ldr r6, _0805C2B8 @ =0x00001B25
	add r1, r1, r6
	strb r0, [r1]
	mov r1, #0
	ldr r2, _0805C2BC @ =0x020192E4
	ldr r3, _0805C2C0 @ =0x00000D66
	add r0, r2, r3
	ldrb r0, [r0]
	cmp r1, r0
	bge _0805C2A4
	ldr r6, _0805C2C4 @ =0x000001A3
	add r3, r0, #0
	ldr r0, _0805C2C8 @ =0x000013E8
	add r2, r2, r0
	ldr r5, _0805C2CC @ =0x000007FF
	ldr r4, _0805C2D0 @ =0x08622AB4
_0805C288:
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, r6
	bne _0805C29C
	b _0805C4D4
_0805C29C:
	add r2, #4
	add r1, #1
	cmp r1, r3
	blt _0805C288
_0805C2A4:
	ldr r1, _0805C2B4 @ =0x02015F00
	ldr r2, _0805C2D4 @ =0x00001B24
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0805C45E
_0805C2B4: .4byte 0x02015F00
_0805C2B8: .4byte 0x00001B25
_0805C2BC: .4byte 0x020192E4
_0805C2C0: .4byte 0x00000D66
_0805C2C4: .4byte 0x000001A3
_0805C2C8: .4byte 0x000013E8
_0805C2CC: .4byte 0x000007FF
_0805C2D0: .4byte gCardIdToNumber
_0805C2D4: .4byte 0x00001B24
_0805C2D8:
	ldr r4, _0805C2FC @ =0x02015F00
	ldr r6, _0805C300 @ =0x00001B25
	add r5, r4, r6
	ldrb r0, [r5]
	add r2, r0, #0
	mov r7, #0x94
	mul r0, r7
	ldr r6, _0805C304 @ =0x0201A070
	add r3, r0, r6
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	bne _0805C30C
	ldr r0, _0805C308 @ =0x00001B24
	add r1, r4, r0
	b _0805C456
	.align 2, 0
_0805C2FC: .4byte 0x02015F00
_0805C300: .4byte 0x00001B25
_0805C304: .4byte 0x0201A070
_0805C308: .4byte 0x00001B24
_0805C30C:
	mov r0, #2
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	bne _0805C328
	ldr r3, _0805C324 @ =0x00001B24
	add r1, r4, r3
	mov r0, #2
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	b _0805C45E
_0805C324: .4byte 0x00001B24
_0805C328:
	ldr r0, _0805C348 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0805C34C @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0805C350 @ =0x000001A3
	ldrh r0, [r0]
	cmp r0, r1
	beq _0805C358
	ldr r2, _0805C354 @ =0x00001B24
	add r1, r4, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0805C45E
_0805C348: .4byte 0x000007FF
_0805C34C: .4byte gCardIdToNumber
_0805C350: .4byte 0x000001A3
_0805C354: .4byte 0x00001B24
_0805C358:
	ldr r0, _0805C388 @ =0x00008008
	lsl r2, r2, #8
	mov r1, #1
	mov r3, #0
	bl DuelCmd_Push
	ldrb r2, [r5]
	mov r1, #0x1F
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #0x10
	add r1, r2, #0
	mul r1, r7
	add r1, r1, r6
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	ldr r2, _0805C38C @ =0x80400000
	orr r1, r2
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
	b _0805C4E6
_0805C388: .4byte 0x00008008
_0805C38C: .4byte 0x80400000
_0805C390:
	ldr r1, _0805C3A0 @ =0x0000017B
	mov r0, #1
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _0805C450
	b _0805C4E6
	.align 2, 0
_0805C3A0: .4byte 0x0000017B
_0805C3A4:
	ldr r5, _0805C3C0 @ =0x020192E4
	ldrh r1, [r5]
	ldr r0, _0805C3C4 @ =0x000013EB
	cmp r1, r0
	bls _0805C3B8
	ldr r3, _0805C3C8 @ =0x00000D64
	add r0, r5, r3
	ldrh r0, [r0]
	cmp r1, r0
	bcs _0805C3CC
_0805C3B8:
	mov r0, #8
	strb r0, [r2, #2]
	b _0805C1FC
	.align 2, 0
_0805C3C0: .4byte 0x020192E4
_0805C3C4: .4byte 0x000013EB
_0805C3C8: .4byte 0x00000D64
_0805C3CC:
	mov r0, #0xA4
	lsl r0, r0, #2
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805C450
	ldr r6, _0805C42C @ =0x00001B2C
	add r1, r5, r6
	ldr r0, _0805C430 @ =0xFFFFFC03
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldr r4, _0805C434 @ =0x02015EF0
	ldrb r3, [r4, #0xB]
	lsl r0, r3, #2
	ldr r6, _0805C438 @ =0x000013E8
	add r1, r5, r6
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r2, _0805C43C @ =0x00001B24
	add r1, r5, r2
	strh r0, [r1]
	ldr r3, _0805C440 @ =0x00001B2F
	add r1, r5, r3
	mov r2, #2
	ldrb r0, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r6, _0805C444 @ =0x00001B30
	add r3, r5, r6
	ldrb r0, [r4, #0xB]
	lsl r1, r0, #1
	ldr r0, _0805C448 @ =0xFFFFFE01
	ldrh r6, [r3]
	and r0, r6
	orr r0, r1
	strh r0, [r3]
	ldr r1, _0805C44C @ =0x00001B28
	add r0, r5, r1
	ldrb r3, [r0]
	orr r2, r3
	strb r2, [r0]
	ldrb r0, [r4, #2]
	add r0, #1
	strb r0, [r4, #2]
	b _0805C1FC
_0805C42C: .4byte 0x00001B2C
_0805C430: .4byte 0xFFFFFC03
_0805C434: .4byte 0x02015EF0
_0805C438: .4byte 0x000013E8
_0805C43C: .4byte 0x00001B24
_0805C440: .4byte 0x00001B2F
_0805C444: .4byte 0x00001B30
_0805C448: .4byte 0xFFFFFE01
_0805C44C: .4byte 0x00001B28
_0805C450:
	ldr r1, _0805C464 @ =0x02015F00
	ldr r6, _0805C468 @ =0x00001B24
	add r1, r1, r6
_0805C456:
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0805C45E:
	strb r0, [r1]
	b _0805C1FC
	.align 2, 0
_0805C464: .4byte 0x02015F00
_0805C468: .4byte 0x00001B24
_0805C46C:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl CardMenu_PlaySpellTrapFromHand
	ldr r3, _0805C4AC @ =0x020192E0
	ldr r6, _0805C4B0 @ =0x00001B2C
	add r1, r3, r6
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805C488
	b _0805C1FC
_0805C488:
	ldr r0, _0805C4B4 @ =0x00001B14
	add r2, r3, r0
	ldr r0, [r2]
	ldr r1, _0805C4B8 @ =0xFFFE01FF
	and r0, r1
	str r0, [r2]
	ldr r2, _0805C4BC @ =0x00001B16
	add r1, r3, r2
	ldr r0, _0805C4C0 @ =0xFFFFFE01
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r1, _0805C4C4 @ =0x02015EF0
	ldrb r0, [r1, #2]
	add r0, #1
	strb r0, [r1, #2]
	b _0805C1FC
	.align 2, 0
_0805C4AC: .4byte 0x020192E0
_0805C4B0: .4byte 0x00001B2C
_0805C4B4: .4byte 0x00001B14
_0805C4B8: .4byte 0xFFFE01FF
_0805C4BC: .4byte 0x00001B16
_0805C4C0: .4byte 0xFFFFFE01
_0805C4C4: .4byte 0x02015EF0
_0805C4C8:
	mov r0, #1
	bl BattlePhase_Run
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0805C4FE
_0805C4D4:
	ldr r0, _0805C4F0 @ =0x02015F00
	ldr r6, _0805C4F4 @ =0x00001B25
	add r0, r0, r6
	ldrb r2, [r0]
	mov r0, #1
	str r0, [sp, #0]
	mov r3, #0
	bl QueueNormalSummon
_0805C4E6:
	ldr r1, _0805C4F8 @ =0x02015EF0
	ldrb r0, [r1, #2]
	add r0, #1
_0805C4EC:
	strb r0, [r1, #2]
	b _0805C1FC
_0805C4F0: .4byte 0x02015F00
_0805C4F4: .4byte 0x00001B25
_0805C4F8: .4byte 0x02015EF0
_0805C4FC:
	mov r0, #1
_0805C4FE:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AiStrategyCyberStein
	.align 2, 0

