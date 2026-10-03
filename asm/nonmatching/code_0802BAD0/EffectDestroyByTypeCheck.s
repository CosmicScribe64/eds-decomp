	thumb_func_start EffectDestroyByTypeCheck
EffectDestroyByTypeCheck: @ 0x0802BBDC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	str r0, [sp, #0]
	lsl r1, r1, #0x10
	ldrh r0, [r0]
	mov r8, r0
	lsl r0, r1, #8
	lsr r7, r0, #0x18
	lsr r5, r1, #0x18
	mov r0, #1
	and r0, r7
	ldr r1, _0802BC98 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	mov r9, r2
	ldr r1, _0802BC9C @ =0x0201930C
	add r1, r9
	mov r0, #0x94
	add r6, r5, #0
	mul r6, r0
	add r1, r1, r6
	mov sl, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	str r0, [sp, #4]
	add r0, r7, #0
	add r1, r5, #0
	bl GetZoneCardType
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r5, #4
	bgt _0802BD14
	mov r1, r9
	add r0, r6, r1
	ldr r2, _0802BC9C @ =0x0201930C
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802BD14
	ldr r1, [sp, #0]
	ldrh r0, [r1]
	add r1, r7, #0
	add r2, r5, #0
	bl CanCardTargetZone
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802BD14
	add r0, r7, #0
	add r1, r5, #0
	bl IsZoneTargetable
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802BD14
	mov r0, #2
	mov r2, sl
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0802BD14
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _0802BD14
	mov r1, r8
	cmp r1, #0
	beq _0802BD14
	ldr r0, _0802BCA0 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r2, _0802BCA4 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0802BCA8 @ =0x00000295
	cmp r1, r0
	beq _0802BCF8
	cmp r1, r0
	bgt _0802BCB8
	sub r0, #6
	cmp r1, r0
	beq _0802BCE0
	cmp r1, r0
	bgt _0802BCAC
	sub r0, #3
	cmp r1, r0
	beq _0802BCD8
	b _0802BD14
_0802BC98: .4byte 0x00000D64
_0802BC9C: .4byte 0x0201930C
_0802BCA0: .4byte 0x000007FF
_0802BCA4: .4byte gCardIdToNumber
_0802BCA8: .4byte 0x00000295
_0802BCAC:
	ldr r0, _0802BCB4 @ =0x00000293
	cmp r1, r0
	beq _0802BCF2
	b _0802BD14
_0802BCB4: .4byte 0x00000293
_0802BCB8:
	ldr r0, _0802BCD0 @ =0x00000297
	cmp r1, r0
	beq _0802BD04
	cmp r1, r0
	blt _0802BCFE
	ldr r0, _0802BCD4 @ =0x0000040C
	cmp r1, r0
	beq _0802BD0A
	add r0, #1
	cmp r1, r0
	beq _0802BD10
	b _0802BD14
_0802BCD0: .4byte 0x00000297
_0802BCD4: .4byte 0x0000040C
_0802BCD8:
	cmp r4, #0xF
	bne _0802BD14
_0802BCDC:
	mov r0, #1
	b _0802BD16
_0802BCE0:
	add r0, r7, #0
	add r1, r5, #0
	mov r2, #0
	mov r3, #0
	bl CountZoneEquips
	cmp r0, #0
	ble _0802BD14
	b _0802BCDC
_0802BCF2:
	cmp r4, #7
	bne _0802BD14
	b _0802BCDC
_0802BCF8:
	cmp r4, #0xA
	bne _0802BD14
	b _0802BCDC
_0802BCFE:
	cmp r4, #6
	bne _0802BD14
	b _0802BCDC
_0802BD04:
	cmp r4, #8
	bne _0802BD14
	b _0802BCDC
_0802BD0A:
	cmp r4, #0x12
	bne _0802BD14
	b _0802BCDC
_0802BD10:
	cmp r4, #3
	beq _0802BCDC
_0802BD14:
	mov r0, #0
_0802BD16:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectDestroyByTypeCheck
	.align 2, 0

