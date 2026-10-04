	thumb_func_start AiChooseStrategy
AiChooseStrategy: @ 0x0805BC24
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	ldr r1, _0805BC58 @ =0x02015F00
	ldr r0, _0805BC5C @ =0x00001B24
	add r1, r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r7, #0
	mov r9, r7
_0805BC44:
	mov r6, #1
	mov r0, r9
	cmp r0, #8
	bls _0805BC4E
	b _0805C05C
_0805BC4E:
	lsl r0, r0, #2
	ldr r1, _0805BC60 @ =0x0805BC64
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0805BC58: .4byte 0x02015F00
_0805BC5C: .4byte 0x00001B24
_0805BC60: .4byte 0x0805BC64
_0805BC64:
	.4byte _0805BC88
	.4byte _0805BD18
	.4byte _0805BD70
	.4byte _0805BDE0
	.4byte _0805BE8C
	.4byte _0805C080
	.4byte _0805BE84
	.4byte _0805BEFC
	.4byte _0805BF30
_0805BC88:
	ldr r4, _0805BD00 @ =0x020192E0
	ldr r1, _0805BD04 @ =0x00001B10
	add r0, r4, r1
	ldrh r0, [r0]
	cmp r0, #0
	bne _0805BC96
	mov r6, #0
_0805BC96:
	mov r2, #0xD7
	lsl r2, r2, #4
	add r1, r4, r2
	mov r0, #0x10
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805BCA8
	mov r6, #0
_0805BCA8:
	ldr r7, _0805BD08 @ =0x00000D68
	add r5, r4, r7
	ldr r0, _0805BD0C @ =0x00001388
	ldrh r1, [r5]
	cmp r1, r0
	bhi _0805BCB6
	mov r6, #0
_0805BCB6:
	mov r0, #1
	bl CountFreeMonsterZones
	cmp r0, #1
	bgt _0805BCC2
	mov r6, #0
_0805BCC2:
	mov r1, #0xA4
	lsl r1, r1, #2
	mov r0, #1
	bl AiFindHandCardByNumber
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _0805BCDE
	ldrh r4, [r4, #4]
	ldrh r5, [r5]
	cmp r4, r5
	bls _0805BCDE
	mov r6, #0
_0805BCDE:
	ldr r1, _0805BD10 @ =0x000001A3
	mov r0, #1
	bl AiFindHandCardByNumber
	mov r4, #1
	neg r4, r4
	cmp r0, r4
	bne _0805BCF0
	mov r6, #0
_0805BCF0:
	ldr r1, _0805BD14 @ =0x0000017B
	mov r0, #1
	bl FindFusionDeckCardByNumber
	cmp r0, r4
	beq _0805BCFE
	b _0805C05E
_0805BCFE:
	b _0805C080
_0805BD00: .4byte 0x020192E0
_0805BD04: .4byte 0x00001B10
_0805BD08: .4byte 0x00000D68
_0805BD0C: .4byte 0x00001388
_0805BD10: .4byte 0x000001A3
_0805BD14: .4byte 0x0000017B
_0805BD18:
	ldr r0, _0805BD60 @ =0x020192E0
	ldr r2, _0805BD64 @ =0x00001B10
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, #0
	bne _0805BD26
	mov r6, #0
_0805BD26:
	mov r0, #1
	bl CountFreeMonsterZones
	cmp r0, #0
	bne _0805BD32
	mov r6, #0
_0805BD32:
	ldr r4, _0805BD68 @ =0x0000034D
	mov r0, #1
	add r1, r4, #0
	bl AiFindHandCardByNumber
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _0805BD46
	mov r6, #0
_0805BD46:
	lsl r0, r4, #1
	ldr r7, _0805BD6C @ =0x08623DF4
	add r0, r0, r7
	ldrh r1, [r0]
	mov r0, #1
	bl CanSummonFromHand
	cmp r0, #0
	bne _0805BD5A
	mov r6, #0
_0805BD5A:
	bl DebugPrintFlush
	b _0805C05E
_0805BD60: .4byte 0x020192E0
_0805BD64: .4byte 0x00001B10
_0805BD68: .4byte 0x0000034D
_0805BD6C: .4byte gCardNumberToId
_0805BD70:
	ldr r0, _0805BDCC @ =0x020192E0
	ldr r1, _0805BDD0 @ =0x00001B10
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	bne _0805BD7E
	mov r6, #0
_0805BD7E:
	ldr r4, _0805BDD4 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _0805BD8E
	mov r6, #0
_0805BD8E:
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _0805BD9C
	mov r6, #0
_0805BD9C:
	ldr r1, _0805BDD8 @ =0x000001FF
	mov r0, #1
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bne _0805BDAA
	mov r6, #0
_0805BDAA:
	mov r0, #1
	bl CountFreeMonsterZones
	cmp r0, #1
	bgt _0805BDB6
	mov r6, #0
_0805BDB6:
	ldr r1, _0805BDDC @ =0x000004DD
	mov r0, #1
	bl AiFindHandCardByNumber
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	beq _0805BDC8
	b _0805C05E
_0805BDC8:
	b _0805C080
	.align 2, 0
_0805BDCC: .4byte 0x020192E0
_0805BDD0: .4byte 0x00001B10
_0805BDD4: .4byte 0x0000058A
_0805BDD8: .4byte 0x000001FF
_0805BDDC: .4byte 0x000004DD
_0805BDE0:
	mov r0, #1
	bl CountFreeMonsterZones
	cmp r0, #1
	bgt _0805BDEC
	mov r6, #0
_0805BDEC:
	ldr r1, _0805BE74 @ =0x0000013D
	mov r0, #1
	bl AiFindHandCardByNumber
	mov r7, #1
	neg r7, r7
	cmp r0, r7
	bne _0805BDFE
	mov r6, #0
_0805BDFE:
	mov r0, #0
	mov r1, #0x3D
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _0805BE0C
	b _0805C05E
_0805BE0C:
	ldr r5, _0805BE78 @ =0x000004E1
	mov r0, #0
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _0805BE1C
	b _0805C05E
_0805BE1C:
	mov r0, #1
	mov r1, #0x3D
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _0805BE2A
	b _0805C05E
_0805BE2A:
	mov r0, #1
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	beq _0805BE38
	b _0805C05E
_0805BE38:
	ldr r1, _0805BE7C @ =0x020192E4
	ldr r2, _0805BE80 @ =0x00000D6C
	add r1, r1, r2
	mov r0, #0x10
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805BE4A
	b _0805C080
_0805BE4A:
	mov r0, #1
	mov r1, #0x3D
	bl AiFindHandCardByNumber
	add r4, r0, #0
	cmp r4, r7
	bne _0805BE66
	mov r0, #1
	add r1, r5, #0
	bl AiFindHandCardByNumber
	cmp r0, r4
	bne _0805BE66
	mov r6, #0
_0805BE66:
	mov r0, #1
	bl CountFreeMonsterZones
	cmp r0, #2
	ble _0805BE72
	b _0805C05E
_0805BE72:
	b _0805C080
_0805BE74: .4byte 0x0000013D
_0805BE78: .4byte 0x000004E1
_0805BE7C: .4byte 0x020192E4
_0805BE80: .4byte 0x00000D6C
_0805BE84:
	ldr r4, _0805BE88 @ =0x000005EA
	b _0805BEFE
_0805BE88: .4byte 0x000005EA
_0805BE8C:
	mov r6, #0
	ldr r1, _0805BEF0 @ =0x00000522
	mov r0, #1
	bl AiFindHandCardByNumber
	cmp r0, #0
	bge _0805BE9C
	b _0805C05E
_0805BE9C:
	mov r0, #1
	bl CountMonsters
	cmp r0, #0
	bgt _0805BEA8
	b _0805C05E
_0805BEA8:
	mov r4, #0
_0805BEAA:
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _0805BEF4 @ =0x0201A070
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0805BED8
	mov r0, #1
	add r1, r4, #0
	mov r2, sp
	bl GetZoneCardStats
	mov r0, sp
	ldrb r1, [r0, #2]
	mov r0, #0x1F
	and r0, r1
	cmp r0, #7
	bne _0805BED8
	ldr r0, [sp, #4]
	add r8, r0
_0805BED8:
	add r4, #1
	cmp r4, #4
	ble _0805BEAA
	ldr r1, _0805BEF8 @ =0x020192E4
	mov r2, r8
	lsl r0, r2, #1
	ldrh r1, [r1]
	cmp r1, r0
	ble _0805BEEC
	b _0805C05E
_0805BEEC:
	b _0805C062
	.align 2, 0
_0805BEF0: .4byte 0x00000522
_0805BEF4: .4byte 0x0201A070
_0805BEF8: .4byte 0x020192E4
_0805BEFC:
	ldr r4, _0805BF28 @ =0x000005EB
_0805BEFE:
	mov r0, #1
	add r1, r4, #0
	bl AiFindHandCardByNumber
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _0805BF10
	b _0805C080
_0805BF10:
	lsl r0, r4, #1
	ldr r7, _0805BF2C @ =0x08623DF4
	add r0, r0, r7
	ldrh r1, [r0]
	mov r0, #1
	bl CanSummonFromHand
	cmp r0, #0
	beq _0805BF24
	b _0805C05E
_0805BF24:
	b _0805C080
	.align 2, 0
_0805BF28: .4byte 0x000005EB
_0805BF2C: .4byte gCardNumberToId
_0805BF30:
	mov r6, #0
	mov r5, #0
	ldr r0, _0805C02C @ =0x020192E4
	ldr r1, _0805C030 @ =0x00000D66
	add r0, r0, r1
	ldrb r2, [r0]
	cmp r6, r2
	bge _0805BF7C
	add r7, r0, #0
_0805BF42:
	lsl r0, r5, #2
	ldr r1, _0805C034 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldr r0, _0805C038 @ =0x000007FF
	add r1, r0, #0
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0805C03C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl IsToonMonster
	cmp r0, #0
	beq _0805BF74
	mov r0, #1
	add r1, r4, #0
	bl CanSummonFromHand
	cmp r0, #0
	beq _0805BF74
	mov r6, #1
_0805BF74:
	add r5, #1
	ldrb r2, [r7]
	cmp r5, r2
	blt _0805BF42
_0805BF7C:
	ldr r1, _0805C040 @ =0x000003BA
	mov r0, #1
	bl AiFindHandCardByNumber
	cmp r0, #0
	blt _0805C05E
	bl AiBackupDuelState
	ldr r0, _0805C044 @ =0x0201A070
	ldr r7, _0805C048 @ =0x08624568
	mov ip, r7
	ldr r1, _0805C04C @ =0x00000375
	add r3, r0, r1
	mov r7, #0xB9
	lsl r7, r7, #2
	add r2, r0, r7
	ldr r1, _0805C050 @ =0xFFFFF000
	mov sl, r1
	mov r5, #9
	neg r5, r5
	ldr r7, _0805C054 @ =0x00000534
	add r4, r0, r7
_0805BFA8:
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0805BFD4
	ldr r0, _0805C058 @ =0x00000FFF
	add r1, r0, #0
	mov r7, ip
	ldrh r7, [r7]
	and r1, r7
	mov r0, sl
	ldrh r7, [r2]
	and r0, r7
	orr r0, r1
	strh r0, [r2]
	mov r0, #2
	ldrb r1, [r2, #6]
	orr r0, r1
	strb r0, [r2, #6]
	add r0, r5, #0
	ldrb r7, [r3]
	and r0, r7
	strb r0, [r3]
_0805BFD4:
	add r3, #0x94
	add r2, #0x94
	cmp r2, r4
	ble _0805BFA8
	mov r5, #0
	ldr r0, _0805C02C @ =0x020192E4
	ldr r1, _0805C030 @ =0x00000D66
	add r0, r0, r1
	ldrb r2, [r0]
	cmp r5, r2
	bge _0805C026
	add r7, r0, #0
_0805BFEC:
	lsl r0, r5, #2
	ldr r1, _0805C034 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldr r0, _0805C038 @ =0x000007FF
	add r1, r0, #0
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _0805C03C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl IsToonMonster
	cmp r0, #0
	beq _0805C01E
	mov r0, #1
	add r1, r4, #0
	bl CanSummonFromHand
	cmp r0, #0
	beq _0805C01E
	mov r6, #1
_0805C01E:
	add r5, #1
	ldrb r2, [r7]
	cmp r5, r2
	blt _0805BFEC
_0805C026:
	bl AiRestoreDuelState
	b _0805C05E
_0805C02C: .4byte 0x020192E4
_0805C030: .4byte 0x00000D66
_0805C034: .4byte 0x0201A6CC
_0805C038: .4byte 0x000007FF
_0805C03C: .4byte gCardIdToNumber
_0805C040: .4byte 0x000003BA
_0805C044: .4byte 0x0201A070
_0805C048: .4byte gCardNumberToId_ToonWorld
_0805C04C: .4byte 0x00000375
_0805C050: .4byte 0xFFFFF000
_0805C054: .4byte 0x00000534
_0805C058: .4byte 0x00000FFF
_0805C05C:
	mov r6, #0
_0805C05E:
	cmp r6, #0
	beq _0805C080
_0805C062:
	ldr r0, _0805C078 @ =0x02015F00
	ldr r7, _0805C07C @ =0x00001B24
	add r0, r0, r7
	mov r1, r9
	lsl r2, r1, #1
	mov r1, #1
	orr r1, r2
	strb r1, [r0]
	mov r0, #1
	b _0805C08E
	.align 2, 0
_0805C078: .4byte 0x02015F00
_0805C07C: .4byte 0x00001B24
_0805C080:
	mov r2, #1
	add r9, r2
	mov r7, r9
	cmp r7, #8
	bgt _0805C08C
	b _0805BC44
_0805C08C:
	mov r0, #0
_0805C08E:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AiChooseStrategy
	.align 2, 0

