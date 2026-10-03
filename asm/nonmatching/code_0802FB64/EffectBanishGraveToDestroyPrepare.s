	thumb_func_start EffectBanishGraveToDestroyPrepare
EffectBanishGraveToDestroyPrepare: @ 0x0802FEA4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r4, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802FF7E
	cmp r1, #0
	bne _0802FF7E
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	ldr r1, _0802FEE0 @ =0x000005E7
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802FF7E
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	bl CountGraveyardMonsters
	add r6, r0, #0
	cmp r6, #0
	bne _0802FEE8
	b _0802FF7E
	.align 2, 0
_0802FEE0: .4byte 0x000005E7
_0802FEE4:
	mov r0, #1
	b _0802FF80
_0802FEE8:
	mov r4, #0
	ldr r1, _0802FF44 @ =0x0201930C
	mov ip, r1
	mov r0, #1
	mov r9, r0
	ldr r1, _0802FF48 @ =0x00000D64
	mov r8, r1
	ldr r7, _0802FF4C @ =0x000007FF
_0802FEF8:
	mov r3, #0
	add r0, r4, #0
	mov r1, r9
	and r0, r1
	mov r5, r8
	mul r5, r0
_0802FF04:
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	add r1, r1, r5
	add r1, ip
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802FF72
	cmp r2, #0
	beq _0802FF72
	add r0, r2, #0
	and r0, r7
	lsl r0, r0, #2
	ldr r1, _0802FF50 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0802FF5C
	cmp r0, #0x17
	ble _0802FF54
	cmp r0, #0x18
	beq _0802FF58
	b _0802FF5C
_0802FF44: .4byte 0x0201930C
_0802FF48: .4byte 0x00000D64
_0802FF4C: .4byte 0x000007FF
_0802FF50: .4byte gCardStats
_0802FF54:
	mov r0, #0
	b _0802FF6E
_0802FF58:
	mov r0, #0xA
	b _0802FF6E
_0802FF5C:
	and r2, r7
	lsl r0, r2, #2
	ldr r1, _0802FF8C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0802FF6E:
	cmp r0, r6
	ble _0802FEE4
_0802FF72:
	add r3, #1
	cmp r3, #4
	ble _0802FF04
	add r4, #1
	cmp r4, #1
	ble _0802FEF8
_0802FF7E:
	mov r0, #0
_0802FF80:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0802FF8C: .4byte gCardStats
	thumb_func_end EffectBanishGraveToDestroyPrepare

