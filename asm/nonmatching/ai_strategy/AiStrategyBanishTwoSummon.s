	thumb_func_start AiStrategyBanishTwoSummon
AiStrategyBanishTwoSummon: @ 0x0805D080
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r1, _0805D09C @ =0x02015EF0
	ldrb r0, [r1, #2]
	cmp r0, #1
	beq _0805D0B2
	cmp r0, #1
	bgt _0805D0A0
	cmp r0, #0
	beq _0805D0A8
	b _0805D244
	.align 2, 0
_0805D09C: .4byte 0x02015EF0
_0805D0A0:
	cmp r0, #2
	bne _0805D0A6
	b _0805D1B8
_0805D0A6:
	b _0805D244
_0805D0A8:
	mov r0, #2
	strb r0, [r1, #3]
	ldrb r0, [r1, #2]
	add r0, #1
	strb r0, [r1, #2]
_0805D0B2:
	ldr r1, _0805D100 @ =0x000005EB
	mov r0, #1
	mov r2, #0
	bl CollectEffectTargets
	add r6, r0, #0
	mov r7, #1
	neg r7, r7
	ldr r5, _0805D104 @ =0x0000270F
	mov r4, #0
	cmp r4, r6
	bge _0805D184
	ldr r3, _0805D108 @ =0x000007FF
	ldr r0, _0805D10C @ =0x0201D81C
	mov r8, r0
	mov r1, #0xF8
	lsl r1, r1, #0x11
	mov ip, r1
_0805D0D6:
	lsl r0, r4, #2
	add r0, r8
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0805D110 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, ip
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805D11E
	cmp r0, #0x17
	ble _0805D114
	cmp r0, #0x18
	beq _0805D118
	b _0805D11E
_0805D100: .4byte 0x000005EB
_0805D104: .4byte 0x0000270F
_0805D108: .4byte 0x000007FF
_0805D10C: .4byte 0x0201D81C
_0805D110: .4byte gCardStats
_0805D114:
	mov r0, #0
	b _0805D134
_0805D118:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805D134
_0805D11E:
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0805D158 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805D134:
	cmp r5, r0
	ble _0805D17E
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0805D158 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, ip
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805D166
	cmp r0, #0x17
	ble _0805D15C
	cmp r0, #0x18
	beq _0805D160
	b _0805D166
_0805D158: .4byte gCardStats
_0805D15C:
	mov r0, #0
	b _0805D17A
_0805D160:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805D17A
_0805D166:
	and r2, r3
	lsl r0, r2, #2
	ldr r2, _0805D1AC @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805D17A:
	add r5, r0, #0
	add r7, r4, #0
_0805D17E:
	add r4, #1
	cmp r4, r6
	blt _0805D0D6
_0805D184:
	cmp r7, #0
	blt _0805D228
	lsl r1, r7, #2
	ldr r0, _0805D1B0 @ =0x0201D81C
	add r1, r1, r0
	mov r0, #1
	bl BanishGraveyardCard
	ldr r1, _0805D1B4 @ =0x02015EF0
	ldrb r0, [r1, #3]
	sub r0, #1
	strb r0, [r1, #3]
	lsl r0, r0, #0x18
	cmp r0, #0
	bne _0805D238
	ldrb r0, [r1, #2]
	add r0, #1
	strb r0, [r1, #2]
	b _0805D238
	.align 2, 0
_0805D1AC: .4byte gCardStats
_0805D1B0: .4byte 0x0201D81C
_0805D1B4: .4byte 0x02015EF0
_0805D1B8:
	mov r4, #0
	ldr r1, _0805D1F4 @ =0x020192E4
	ldr r2, _0805D1F8 @ =0x00000D66
	add r0, r1, r2
	ldrb r0, [r0]
	cmp r4, r0
	bge _0805D1EC
	ldr r6, _0805D1FC @ =0x000005EB
	add r2, r0, #0
	ldr r0, _0805D200 @ =0x000013E8
	add r1, r1, r0
	ldr r5, _0805D204 @ =0x000007FF
	ldr r3, _0805D208 @ =0x08622AB4
_0805D1D2:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #1
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, r6
	beq _0805D214
	add r1, #4
	add r4, #1
	cmp r4, r2
	blt _0805D1D2
_0805D1EC:
	ldr r1, _0805D20C @ =0x02015F00
	ldr r2, _0805D210 @ =0x00001B24
	add r1, r1, r2
	b _0805D22E
_0805D1F4: .4byte 0x020192E4
_0805D1F8: .4byte 0x00000D66
_0805D1FC: .4byte 0x000005EB
_0805D200: .4byte 0x000013E8
_0805D204: .4byte 0x000007FF
_0805D208: .4byte gCardIdToNumber
_0805D20C: .4byte 0x02015F00
_0805D210: .4byte 0x00001B24
_0805D214:
	mov r0, #1
	bl FindFreeMonsterZone
	add r2, r0, #0
	mov r0, #1
	str r0, [sp, #0]
	add r1, r4, #0
	mov r3, #0
	bl QueueSpecialSummonFromHand
_0805D228:
	ldr r1, _0805D23C @ =0x02015F00
	ldr r0, _0805D240 @ =0x00001B24
	add r1, r1, r0
_0805D22E:
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0805D238:
	mov r0, #0
	b _0805D246
_0805D23C: .4byte 0x02015F00
_0805D240: .4byte 0x00001B24
_0805D244:
	mov r0, #1
_0805D246:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AiStrategyBanishTwoSummon
	.align 2, 0

