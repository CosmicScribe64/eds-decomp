	thumb_func_start EffectEquippedTributeCheck
EffectEquippedTributeCheck: @ 0x0802B2FC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r3, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r7, r0, #0x18
	lsr r6, r1, #0x18
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	mul r0, r6
	ldr r1, _0802B364 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802B368 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	mov r0, #0x47
	str r0, [sp, #0]
	ldr r4, _0802B36C @ =0x00000115
	cmp r2, #0
	bne _0802B336
	b _0802B46E
_0802B336:
	cmp r6, #4
	ble _0802B33C
	b _0802B46E
_0802B33C:
	ldrb r1, [r3, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r7, r0
	beq _0802B348
	b _0802B46E
_0802B348:
	ldr r0, _0802B370 @ =0x000007FF
	ldrh r3, [r3]
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _0802B374 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, #0x38
	beq _0802B38C
	cmp r1, #0x38
	bgt _0802B378
	cmp r1, #0x37
	beq _0802B386
	b _0802B46E
_0802B364: .4byte 0x00000D64
_0802B368: .4byte 0x0201930C
_0802B36C: .4byte 0x00000115
_0802B370: .4byte 0x000007FF
_0802B374: .4byte gCardIdToNumber
_0802B378:
	cmp r1, #0x42
	beq _0802B392
	mov r0, #0xB8
	lsl r0, r0, #1
	cmp r1, r0
	beq _0802B398
	b _0802B46E
_0802B386:
	mov r0, #1
	mov r8, r0
	b _0802B3A4
_0802B38C:
	mov r1, #3
	mov r8, r1
	b _0802B3A4
_0802B392:
	mov r0, #5
	mov r8, r0
	b _0802B3A4
_0802B398:
	mov r1, #1
	neg r1, r1
	mov r8, r1
	ldr r0, _0802B3D0 @ =0x0000028B
	str r0, [sp, #0]
	ldr r4, _0802B3D4 @ =0x0000016D
_0802B3A4:
	ldr r0, _0802B3D8 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _0802B3DC @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r4
	bne _0802B46E
	ldr r4, _0802B3E0 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802B46E
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _0802B3E8
	b _0802B46E
_0802B3D0: .4byte 0x0000028B
_0802B3D4: .4byte 0x0000016D
_0802B3D8: .4byte 0x000007FF
_0802B3DC: .4byte gCardIdToNumber
_0802B3E0: .4byte 0x0000058A
_0802B3E4:
	mov r0, #1
	b _0802B470
_0802B3E8:
	mov r5, #0
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r7, _0802B480 @ =0x00000D64
	add r4, r2, #0
	mul r4, r7
	add r0, r1, r4
	ldr r6, _0802B484 @ =0x0201930C
	add r3, r0, r6
	add r0, r3, #0
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r5, r0
	bge _0802B46E
	mov sl, r2
	mov ip, r1
	mov r9, r3
	add r0, r4, #0
	add r0, #0x4A
	add r0, ip
	add r3, r0, r6
_0802B418:
	lsl r1, r5, #1
	mov r0, r9
	add r0, #0xA
	add r0, r0, r1
	ldrh r0, [r0]
	ldrb r2, [r3]
	cmp r2, #1
	bne _0802B456
	lsl r1, r0, #0x18
	lsr r1, r1, #0x18
	lsr r0, r0, #8
	and r1, r2
	mov r2, #0x94
	mul r0, r2
	mul r1, r7
	add r0, r0, r1
	add r1, r0, r6
	ldr r0, [r1]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r2, _0802B488 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r2, [sp, #0]
	cmp r0, r2
	bne _0802B456
	ldrb r1, [r1, #6]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, r8
	bgt _0802B3E4
_0802B456:
	add r3, #2
	add r5, #1
	ldr r7, _0802B480 @ =0x00000D64
	mov r0, sl
	mul r0, r7
	add r0, ip
	ldr r6, _0802B484 @ =0x0201930C
	add r0, r0, r6
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r5, r0
	blt _0802B418
_0802B46E:
	mov r0, #0
_0802B470:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0802B480: .4byte 0x00000D64
_0802B484: .4byte 0x0201930C
_0802B488: .4byte gCardIdToNumber
	thumb_func_end EffectEquippedTributeCheck

