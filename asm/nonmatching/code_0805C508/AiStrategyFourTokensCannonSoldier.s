	thumb_func_start AiStrategyFourTokensCannonSoldier
AiStrategyFourTokensCannonSoldier: @ 0x0805C938
	push {r4, r5, r6, lr}
	ldr r5, _0805C94C @ =0x02015EF0
	ldrb r0, [r5, #2]
	cmp r0, #1
	beq _0805CA00
	cmp r0, #1
	bgt _0805C950
	cmp r0, #0
	beq _0805C956
	b _0805CB24
_0805C94C: .4byte 0x02015EF0
_0805C950:
	cmp r0, #2
	beq _0805CA2C
	b _0805CB24
_0805C956:
	ldr r1, _0805C9BC @ =0x000001FF
	mov r0, #1
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bne _0805C964
	b _0805CA70
_0805C964:
	ldr r0, _0805C9C0 @ =0x000004DD
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805C9E8
	ldr r2, _0805C9C4 @ =0x020192E0
	ldr r3, _0805C9C8 @ =0x00001B30
	add r1, r2, r3
	ldr r0, _0805C9CC @ =0xFFFFFC03
	ldrh r6, [r1]
	and r0, r6
	strh r0, [r1]
	ldrb r1, [r5, #0xB]
	lsl r0, r1, #2
	ldr r3, _0805C9D0 @ =0x000013EC
	add r1, r2, r3
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r6, _0805C9D4 @ =0x00001B28
	add r1, r2, r6
	strh r0, [r1]
	ldr r0, _0805C9D8 @ =0x00001B33
	add r1, r2, r0
	mov r3, #2
	ldrb r0, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r1, _0805C9DC @ =0x00001B34
	add r4, r2, r1
	ldrb r6, [r5, #0xB]
	lsl r1, r6, #1
	ldr r0, _0805C9E0 @ =0xFFFFFE01
	ldrh r6, [r4]
	and r0, r6
	orr r0, r1
	strh r0, [r4]
	ldr r0, _0805C9E4 @ =0x00001B2C
	add r2, r2, r0
	ldrb r1, [r2]
	orr r3, r1
	strb r3, [r2]
	b _0805CA1A
_0805C9BC: .4byte 0x000001FF
_0805C9C0: .4byte 0x000004DD
_0805C9C4: .4byte 0x020192E0
_0805C9C8: .4byte 0x00001B30
_0805C9CC: .4byte 0xFFFFFC03
_0805C9D0: .4byte 0x000013EC
_0805C9D4: .4byte 0x00001B28
_0805C9D8: .4byte 0x00001B33
_0805C9DC: .4byte 0x00001B34
_0805C9E0: .4byte 0xFFFFFE01
_0805C9E4: .4byte 0x00001B2C
_0805C9E8:
	ldr r1, _0805C9F8 @ =0x02015F00
	ldr r2, _0805C9FC @ =0x00001B24
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0805CA7E
_0805C9F8: .4byte 0x02015F00
_0805C9FC: .4byte 0x00001B24
_0805CA00:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl CardMenu_PlaySpellTrapFromHand
	ldr r1, _0805CA24 @ =0x020192E0
	ldr r6, _0805CA28 @ =0x00001B2C
	add r1, r1, r6
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0805CA80
_0805CA1A:
	ldrb r0, [r5, #2]
	add r0, #1
	strb r0, [r5, #2]
	b _0805CA80
	.align 2, 0
_0805CA24: .4byte 0x020192E0
_0805CA28: .4byte 0x00001B2C
_0805CA2C:
	ldr r4, _0805CA58 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0805CA70
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _0805CA64
	ldr r1, _0805CA5C @ =0x02015F00
	ldr r3, _0805CA60 @ =0x00001B24
	add r1, r1, r3
	mov r0, #2
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	b _0805CA7E
	.align 2, 0
_0805CA58: .4byte 0x0000058A
_0805CA5C: .4byte 0x02015F00
_0805CA60: .4byte 0x00001B24
_0805CA64:
	ldr r1, _0805CA84 @ =0x000001FF
	mov r0, #1
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bne _0805CA90
_0805CA70:
	ldr r1, _0805CA88 @ =0x02015F00
	ldr r0, _0805CA8C @ =0x00001B24
	add r1, r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0805CA7E:
	strb r0, [r1]
_0805CA80:
	mov r0, #0
	b _0805CB26
_0805CA84: .4byte 0x000001FF
_0805CA88: .4byte 0x02015F00
_0805CA8C: .4byte 0x00001B24
_0805CA90:
	ldr r5, _0805CACC @ =0x0201A070
	ldr r4, _0805CAD0 @ =0x000007FF
	add r1, r5, #0
	mov r6, #0x94
	lsl r6, r6, #2
	add r3, r5, r6
	ldr r2, _0805CAD4 @ =0x08622AB4
_0805CA9E:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _0805CABC
	and r0, r4
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r6, _0805CAD8 @ =0xFFFFF880
	add r0, r0, r6
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _0805CAE0
_0805CABC:
	add r1, #0x94
	cmp r1, r3
	ble _0805CA9E
	mov r1, #0
	ldr r0, _0805CADC @ =0x02015EF0
	strb r1, [r0, #2]
	b _0805CA80
	.align 2, 0
_0805CACC: .4byte 0x0201A070
_0805CAD0: .4byte 0x000007FF
_0805CAD4: .4byte gCardIdToNumber
_0805CAD8: .4byte 0xFFFFF880
_0805CADC: .4byte 0x02015EF0
_0805CAE0:
	ldr r1, _0805CB18 @ =0x000001FF
	mov r0, #1
	bl FindFaceUpMonsterByNumber
	add r4, r0, #0
	ldr r0, _0805CB1C @ =0x00008008
	lsl r2, r4, #0x18
	lsr r2, r2, #0x10
	mov r1, #1
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x1F
	and r0, r4
	lsl r0, r0, #0x10
	mov r1, #0x94
	mul r1, r4
	add r1, r1, r5
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	ldr r2, _0805CB20 @ =0x80400000
	orr r1, r2
	orr r0, r1
	mov r1, #0
	bl Chain_AddPending
	b _0805CA80
_0805CB18: .4byte 0x000001FF
_0805CB1C: .4byte 0x00008008
_0805CB20: .4byte 0x80400000
_0805CB24:
	mov r0, #1
_0805CB26:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end AiStrategyFourTokensCannonSoldier

