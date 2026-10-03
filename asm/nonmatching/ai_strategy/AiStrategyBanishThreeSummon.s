	thumb_func_start AiStrategyBanishThreeSummon
AiStrategyBanishThreeSummon: @ 0x0805CEAC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r1, _0805CEC8 @ =0x02015EF0
	ldrb r0, [r1, #2]
	cmp r0, #1
	beq _0805CEDE
	cmp r0, #1
	bgt _0805CECC
	cmp r0, #0
	beq _0805CED4
	b _0805D070
	.align 2, 0
_0805CEC8: .4byte 0x02015EF0
_0805CECC:
	cmp r0, #2
	bne _0805CED2
	b _0805CFE4
_0805CED2:
	b _0805D070
_0805CED4:
	mov r0, #3
	strb r0, [r1, #3]
	ldrb r0, [r1, #2]
	add r0, #1
	strb r0, [r1, #2]
_0805CEDE:
	ldr r1, _0805CF2C @ =0x000005EA
	mov r0, #1
	mov r2, #0
	bl CollectEffectTargets
	add r6, r0, #0
	mov r7, #1
	neg r7, r7
	ldr r5, _0805CF30 @ =0x0000270F
	mov r4, #0
	cmp r4, r6
	bge _0805CFB0
	ldr r3, _0805CF34 @ =0x000007FF
	ldr r0, _0805CF38 @ =0x0201D81C
	mov r8, r0
	mov r1, #0xF8
	lsl r1, r1, #0x11
	mov ip, r1
_0805CF02:
	lsl r0, r4, #2
	add r0, r8
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0805CF3C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, ip
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805CF4A
	cmp r0, #0x17
	ble _0805CF40
	cmp r0, #0x18
	beq _0805CF44
	b _0805CF4A
_0805CF2C: .4byte 0x000005EA
_0805CF30: .4byte 0x0000270F
_0805CF34: .4byte 0x000007FF
_0805CF38: .4byte 0x0201D81C
_0805CF3C: .4byte gCardStats
_0805CF40:
	mov r0, #0
	b _0805CF60
_0805CF44:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805CF60
_0805CF4A:
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0805CF84 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805CF60:
	cmp r5, r0
	ble _0805CFAA
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0805CF84 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, ip
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805CF92
	cmp r0, #0x17
	ble _0805CF88
	cmp r0, #0x18
	beq _0805CF8C
	b _0805CF92
_0805CF84: .4byte gCardStats
_0805CF88:
	mov r0, #0
	b _0805CFA6
_0805CF8C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805CFA6
_0805CF92:
	and r2, r3
	lsl r0, r2, #2
	ldr r2, _0805CFD8 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805CFA6:
	add r5, r0, #0
	add r7, r4, #0
_0805CFAA:
	add r4, #1
	cmp r4, r6
	blt _0805CF02
_0805CFB0:
	cmp r7, #0
	blt _0805D054
	lsl r1, r7, #2
	ldr r0, _0805CFDC @ =0x0201D81C
	add r1, r1, r0
	mov r0, #1
	bl BanishGraveyardCard
	ldr r1, _0805CFE0 @ =0x02015EF0
	ldrb r0, [r1, #3]
	sub r0, #1
	strb r0, [r1, #3]
	lsl r0, r0, #0x18
	cmp r0, #0
	bne _0805D064
	ldrb r0, [r1, #2]
	add r0, #1
	strb r0, [r1, #2]
	b _0805D064
	.align 2, 0
_0805CFD8: .4byte gCardStats
_0805CFDC: .4byte 0x0201D81C
_0805CFE0: .4byte 0x02015EF0
_0805CFE4:
	mov r4, #0
	ldr r1, _0805D020 @ =0x020192E4
	ldr r2, _0805D024 @ =0x00000D66
	add r0, r1, r2
	ldrb r0, [r0]
	cmp r4, r0
	bge _0805D018
	ldr r6, _0805D028 @ =0x000005EA
	add r2, r0, #0
	ldr r0, _0805D02C @ =0x000013E8
	add r1, r1, r0
	ldr r5, _0805D030 @ =0x000007FF
	ldr r3, _0805D034 @ =0x08622AB4
_0805CFFE:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #1
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, r6
	beq _0805D040
	add r1, #4
	add r4, #1
	cmp r4, r2
	blt _0805CFFE
_0805D018:
	ldr r1, _0805D038 @ =0x02015F00
	ldr r2, _0805D03C @ =0x00001B24
	add r1, r1, r2
	b _0805D05A
_0805D020: .4byte 0x020192E4
_0805D024: .4byte 0x00000D66
_0805D028: .4byte 0x000005EA
_0805D02C: .4byte 0x000013E8
_0805D030: .4byte 0x000007FF
_0805D034: .4byte gCardIdToNumber
_0805D038: .4byte 0x02015F00
_0805D03C: .4byte 0x00001B24
_0805D040:
	mov r0, #1
	bl FindFreeMonsterZone
	add r2, r0, #0
	mov r0, #1
	str r0, [sp, #0]
	add r1, r4, #0
	mov r3, #0
	bl QueueSpecialSummonFromHand
_0805D054:
	ldr r1, _0805D068 @ =0x02015F00
	ldr r0, _0805D06C @ =0x00001B24
	add r1, r1, r0
_0805D05A:
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0805D064:
	mov r0, #0
	b _0805D072
_0805D068: .4byte 0x02015F00
_0805D06C: .4byte 0x00001B24
_0805D070:
	mov r0, #1
_0805D072:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AiStrategyBanishThreeSummon
	.align 2, 0

