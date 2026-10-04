	thumb_func_start CardMenu_PlaySpellTrapFromHand
CardMenu_PlaySpellTrapFromHand: @ 0x08049048
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r2, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r9, r1
	ldr r4, _080490BC @ =0x020192E0
	ldr r0, _080490C0 @ =0x00001B30
	add r5, r4, r0
	ldrh r1, [r5]
	lsl r0, r1, #0x16
	lsr r0, r0, #0x18
	cmp r0, #0
	beq _080490CC
	cmp r0, #1
	bne _08049074
	b _08049218
_08049074:
	mov r2, r8
	cmp r2, #0
	bne _0804907C
	b _08049370
_0804907C:
	ldr r3, _080490C4 @ =0x00001B33
	add r0, r4, r3
	ldrb r0, [r0]
	lsl r5, r0, #0x1E
	mov r2, #1
	lsr r1, r5, #0x1F
	add r3, #1
	add r0, r4, r3
	ldr r0, [r0]
	lsl r7, r0, #0xF
	lsr r0, r7, #0x18
	mov r3, #0x94
	mov r8, r3
	mov r3, r8
	mul r3, r0
	add r0, r3, #0
	ldr r3, _080490C8 @ =0x00000D64
	mov ip, r3
	mov r3, ip
	mul r3, r1
	add r1, r3, #0
	add r0, r0, r1
	add r3, r4, #0
	add r3, #0x2C
	add r0, r0, r3
	ldr r0, [r0]
	lsl r0, r0, #0xD
	cmp r0, #0
	blt _080490B8
	b _08049296
_080490B8:
	b _08049258
	.align 2, 0
_080490BC: .4byte 0x020192E0
_080490C0: .4byte 0x00001B30
_080490C4: .4byte 0x00001B33
_080490C8: .4byte 0x00000D64
_080490CC:
	ldr r0, _08049130 @ =0x00001B33
	add r5, r4, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	bl FindFreeSpellTrapZone
	ldr r2, _08049134 @ =0x00001B34
	add r3, r4, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #9
	ldr r1, [r3]
	ldr r2, _08049138 @ =0xFFFE01FF
	and r1, r2
	orr r1, r0
	str r1, [r3]
	ldr r3, _0804913C @ =0x00001B28
	add r1, r4, r3
	ldr r0, _08049140 @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08049144 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08049186
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	cmp r0, #2
	bne _08049186
	mov r4, #0
	add r7, r5, #0
	mov r6, #1
_08049122:
	cmp r4, #0
	beq _08049148
	ldrb r2, [r7]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1F
	sub r2, r6, r0
	b _0804914E
_08049130: .4byte 0x00001B33
_08049134: .4byte 0x00001B34
_08049138: .4byte 0xFFFE01FF
_0804913C: .4byte 0x00001B28
_08049140: .4byte 0x000007FF
_08049144: .4byte gCardStats
_08049148:
	ldrb r3, [r7]
	lsl r0, r3, #0x1E
	lsr r2, r0, #0x1F
_0804914E:
	add r0, r2, #0
	and r0, r6
	ldr r1, _080491F0 @ =0x00000D64
	mul r0, r1
	ldr r5, _080491F4 @ =0x020198D4
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0804916C
	add r0, r2, #0
	mov r1, #0xA
	mov r2, #0
	bl DestroyFieldCard
_0804916C:
	add r4, #1
	cmp r4, #1
	ble _08049122
	mov r0, #0xAA
	lsl r0, r0, #5
	add r2, r5, r0
	ldr r0, [r2]
	ldr r1, _080491F8 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0xA0
	lsl r1, r1, #5
	orr r0, r1
	str r0, [r2]
_08049186:
	ldr r5, _080491FC @ =0x020192E0
	ldr r1, _08049200 @ =0x00001B33
	add r7, r5, r1
	mov r0, #2
	ldrb r2, [r7]
	and r0, r2
	mov r6, #0xC5
	cmp r0, #0
	beq _0804919A
	ldr r6, _08049204 @ =0x000080C5
_0804919A:
	ldr r3, _08049208 @ =0x00001B28
	add r0, r5, r3
	ldrh r1, [r0]
	ldr r0, _0804920C @ =0x00001B34
	add r4, r5, r0
	ldrh r2, [r4]
	lsr r0, r2, #1
	mov r3, #0xF
	add r2, r3, #0
	and r2, r0
	lsl r2, r2, #4
	ldr r0, [r4]
	lsr r0, r0, #9
	and r3, r0
	orr r2, r3
	mov r0, #1
	mov r3, r8
	and r0, r3
	lsl r0, r0, #8
	orr r2, r0
	add r0, r6, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r7, [r7]
	lsl r0, r7, #0x1E
	lsr r0, r0, #0x1F
	bl PayChainEnergyCost
	ldr r0, _08049210 @ =0x00001B30
	add r3, r5, r0
	ldrh r2, [r3]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _08049214 @ =0xFFFFFC03
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _080493A8
_080491F0: .4byte 0x00000D64
_080491F4: .4byte 0x020198D4
_080491F8: .4byte 0xFFFE01FF
_080491FC: .4byte 0x020192E0
_08049200: .4byte 0x00001B33
_08049204: .4byte 0x000080C5
_08049208: .4byte 0x00001B28
_0804920C: .4byte 0x00001B34
_08049210: .4byte 0x00001B30
_08049214: .4byte 0xFFFFFC03
_08049218:
	ldr r1, _0804924C @ =0x00001B33
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldr r2, _08049250 @ =0x00001B34
	add r1, r4, r2
	ldr r2, [r1]
	lsl r2, r2, #0xF
	lsr r2, r2, #0x18
	mov r1, #0
	bl DuelCursor_Select
	ldrh r2, [r5]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _08049254 @ =0xFFFFFC03
	and r0, r2
	orr r0, r1
	strh r0, [r5]
	b _080493A8
	.align 2, 0
_0804924C: .4byte 0x00001B33
_08049250: .4byte 0x00001B34
_08049254: .4byte 0xFFFFFC03
_08049258:
	lsr r0, r5, #0x1F
	and r2, r0
	lsr r0, r7, #0x18
	mov r1, r8
	mul r1, r0
	add r0, r1, #0
	mov r1, ip
	mul r1, r2
	add r0, r0, r1
	add r0, r0, r3
	ldr r0, [r0]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	lsr r1, r5, #0x1F
	cmp r0, r1
	beq _08049296
	add r0, r1, #0
	ldr r1, _080492DC @ =0x0862467A
	ldrh r1, [r1]
	bl ShowCardEffect
	ldr r0, _080492E0 @ =0x020192E0
	ldr r2, _080492E4 @ =0x00001B33
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	mov r1, #0xFA
	lsl r1, r1, #3
	bl LoseLifePoints
_08049296:
	mov r3, r9
	cmp r3, #0
	beq _080492F0
	ldr r3, _080492E0 @ =0x020192E0
	ldr r1, _080492E4 @ =0x00001B33
	add r0, r3, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldrb r2, [r6, #3]
	lsr r1, r2, #2
	lsl r1, r1, #0x19
	orr r0, r1
	ldr r2, _080492E8 @ =0x00001B34
	add r1, r3, r2
	ldr r2, [r1]
	lsl r2, r2, #0xF
	lsr r2, r2, #0x18
	mov r1, #0x1F
	and r1, r2
	lsl r1, r1, #0x10
	orr r0, r1
	ldr r1, _080492EC @ =0x00001B28
	add r3, r3, r1
	ldrh r3, [r3]
	orr r0, r3
	ldrh r2, [r6, #8]
	lsl r1, r2, #0x10
	ldrh r6, [r6, #6]
	orr r1, r6
	bl Chain_AddLink
	b _08049370
	.align 2, 0
_080492DC: .4byte gCardNumberToId_Graverobber
_080492E0: .4byte 0x020192E0
_080492E4: .4byte 0x00001B33
_080492E8: .4byte 0x00001B34
_080492EC: .4byte 0x00001B28
_080492F0:
	cmp r6, #0
	bne _08049334
	ldr r3, _08049324 @ =0x020192E0
	ldr r1, _08049328 @ =0x00001B33
	add r0, r3, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldr r2, _0804932C @ =0x00001B34
	add r1, r3, r2
	ldr r2, [r1]
	lsl r2, r2, #0xF
	lsr r2, r2, #0x18
	mov r1, #0x1F
	and r1, r2
	lsl r1, r1, #0x10
	orr r0, r1
	ldr r1, _08049330 @ =0x00001B28
	add r3, r3, r1
	ldrh r3, [r3]
	orr r0, r3
	mov r1, #0
	bl Chain_AddPending
	b _08049370
_08049324: .4byte 0x020192E0
_08049328: .4byte 0x00001B33
_0804932C: .4byte 0x00001B34
_08049330: .4byte 0x00001B28
_08049334:
	ldr r3, _080493B4 @ =0x020192E0
	ldr r2, _080493B8 @ =0x00001B33
	add r0, r3, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldrb r2, [r6, #3]
	lsr r1, r2, #2
	lsl r1, r1, #0x19
	orr r0, r1
	ldr r2, _080493BC @ =0x00001B34
	add r1, r3, r2
	ldr r2, [r1]
	lsl r2, r2, #0xF
	lsr r2, r2, #0x18
	mov r1, #0x1F
	and r1, r2
	lsl r1, r1, #0x10
	orr r0, r1
	ldr r1, _080493C0 @ =0x00001B28
	add r3, r3, r1
	ldrh r3, [r3]
	orr r0, r3
	ldrh r2, [r6, #8]
	lsl r1, r2, #0x10
	ldrh r6, [r6, #6]
	orr r1, r6
	bl Chain_AddPending
_08049370:
	ldr r3, _080493B4 @ =0x020192E0
	ldr r1, _080493C4 @ =0x00001B12
	add r0, r3, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1D
	cmp r0, #1
	bls _0804939A
	add r2, r3, #4
	add r1, #0x21
	add r0, r3, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldr r1, _080493C8 @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	mov r0, #0x20
	ldrb r2, [r1, #9]
	orr r0, r2
	strb r0, [r1, #9]
_0804939A:
	ldr r0, _080493CC @ =0x00001B2C
	add r1, r3, r0
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080493A8:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080493B4: .4byte 0x020192E0
_080493B8: .4byte 0x00001B33
_080493BC: .4byte 0x00001B34
_080493C0: .4byte 0x00001B28
_080493C4: .4byte 0x00001B12
_080493C8: .4byte 0x00000D64
_080493CC: .4byte 0x00001B2C
	thumb_func_end CardMenu_PlaySpellTrapFromHand

