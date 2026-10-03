	thumb_func_start AiStrategyDoubleMachineAtk
AiStrategyDoubleMachineAtk: @ 0x0805CDA4
	push {r4, r5, r6, lr}
	ldr r5, _0805CDB8 @ =0x02015EF0
	ldrb r0, [r5, #2]
	cmp r0, #1
	beq _0805CE5C
	cmp r0, #1
	bgt _0805CDBC
	cmp r0, #0
	beq _0805CDC2
	b _0805CEA4
_0805CDB8: .4byte 0x02015EF0
_0805CDBC:
	cmp r0, #2
	beq _0805CE88
	b _0805CEA4
_0805CDC2:
	ldr r0, _0805CE1C @ =0x00000522
	bl AiTryPlaySpellTrap
	cmp r0, #0
	beq _0805CE44
	ldr r2, _0805CE20 @ =0x020192E0
	ldr r0, _0805CE24 @ =0x00001B30
	add r1, r2, r0
	ldr r0, _0805CE28 @ =0xFFFFFC03
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldrb r6, [r5, #0xB]
	lsl r0, r6, #2
	ldr r3, _0805CE2C @ =0x000013EC
	add r1, r2, r3
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	ldr r6, _0805CE30 @ =0x00001B28
	add r1, r2, r6
	strh r0, [r1]
	ldr r0, _0805CE34 @ =0x00001B33
	add r1, r2, r0
	mov r3, #2
	ldrb r0, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r1, _0805CE38 @ =0x00001B34
	add r4, r2, r1
	ldrb r6, [r5, #0xB]
	lsl r1, r6, #1
	ldr r0, _0805CE3C @ =0xFFFFFE01
	ldrh r6, [r4]
	and r0, r6
	orr r0, r1
	strh r0, [r4]
	ldr r0, _0805CE40 @ =0x00001B2C
	add r2, r2, r0
	ldrb r1, [r2]
	orr r3, r1
	strb r3, [r2]
	b _0805CE76
	.align 2, 0
_0805CE1C: .4byte 0x00000522
_0805CE20: .4byte 0x020192E0
_0805CE24: .4byte 0x00001B30
_0805CE28: .4byte 0xFFFFFC03
_0805CE2C: .4byte 0x000013EC
_0805CE30: .4byte 0x00001B28
_0805CE34: .4byte 0x00001B33
_0805CE38: .4byte 0x00001B34
_0805CE3C: .4byte 0xFFFFFE01
_0805CE40: .4byte 0x00001B2C
_0805CE44:
	ldr r1, _0805CE54 @ =0x02015F00
	ldr r2, _0805CE58 @ =0x00001B24
	add r1, r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0805CE96
_0805CE54: .4byte 0x02015F00
_0805CE58: .4byte 0x00001B24
_0805CE5C:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl CardMenu_PlaySpellTrapFromHand
	ldr r1, _0805CE80 @ =0x020192E0
	ldr r6, _0805CE84 @ =0x00001B2C
	add r1, r1, r6
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0805CE98
_0805CE76:
	ldrb r0, [r5, #2]
	add r0, #1
	strb r0, [r5, #2]
	b _0805CE98
	.align 2, 0
_0805CE80: .4byte 0x020192E0
_0805CE84: .4byte 0x00001B2C
_0805CE88:
	ldr r1, _0805CE9C @ =0x02015F00
	ldr r0, _0805CEA0 @ =0x00001B24
	add r1, r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_0805CE96:
	strb r0, [r1]
_0805CE98:
	mov r0, #0
	b _0805CEA6
_0805CE9C: .4byte 0x02015F00
_0805CEA0: .4byte 0x00001B24
_0805CEA4:
	mov r0, #1
_0805CEA6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end AiStrategyDoubleMachineAtk

