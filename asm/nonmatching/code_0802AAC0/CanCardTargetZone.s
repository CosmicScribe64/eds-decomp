	thumb_func_start CanCardTargetZone
CanCardTargetZone: @ 0x0802B1B8
	push {r4, r5, r6, r7, lr}
	add r5, r1, #0
	add r3, r2, #0
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	mov r6, #1
	and r1, r6
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _0802B1E4 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802B1E8 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	cmp r4, #0
	bne _0802B1EC
	mov r0, #0
	b _0802B270
_0802B1E4: .4byte 0x00000D64
_0802B1E8: .4byte 0x0201930C
_0802B1EC:
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _0802B1FA
	mov r0, #1
	b _0802B270
_0802B1FA:
	add r0, r5, #0
	add r1, r3, #0
	bl GetZoneCardType
	cmp r0, #1
	bne _0802B226
	mov r5, #0xB9
	lsl r5, r5, #2
	mov r0, #0
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _0802B218
	mov r6, #0
_0802B218:
	mov r0, #1
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _0802B226
	mov r6, #0
_0802B226:
	ldr r5, _0802B278 @ =0x000007FF
	and r4, r5
	lsl r0, r4, #1
	ldr r1, _0802B27C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0802B280 @ =0x0000052E
	cmp r1, r0
	beq _0802B23E
	add r0, #3
	cmp r1, r0
	bne _0802B26E
_0802B23E:
	bl GetFaceUpFieldMagicNumber
	ldr r1, _0802B284 @ =0x0000014D
	cmp r0, r1
	bne _0802B26E
	add r0, r7, #0
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _0802B288 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0802B26E
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	cmp r0, #3
	beq _0802B26E
	mov r6, #0
_0802B26E:
	add r0, r6, #0
_0802B270:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802B278: .4byte 0x000007FF
_0802B27C: .4byte gCardIdToNumber
_0802B280: .4byte 0x0000052E
_0802B284: .4byte 0x0000014D
_0802B288: .4byte gCardStats
	thumb_func_end CanCardTargetZone

