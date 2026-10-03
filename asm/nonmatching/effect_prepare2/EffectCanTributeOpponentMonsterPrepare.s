	thumb_func_start EffectCanTributeOpponentMonsterPrepare
EffectCanTributeOpponentMonsterPrepare: @ 0x0802F200
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r7, r0, #0
	ldr r5, _0802F2BC @ =0x020192E0
	ldr r0, _0802F2C0 @ =0x00001B12
	add r1, r5, r0
	mov r0, #0x1C
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #8
	beq _0802F21E
	b _0802F436
_0802F21E:
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CountMonsters
	cmp r0, #0
	bne _0802F22E
	b _0802F436
_0802F22E:
	ldr r4, _0802F2C4 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _0802F23E
	b _0802F436
_0802F23E:
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	ble _0802F24C
	b _0802F436
_0802F24C:
	mov r6, #0
	ldrb r0, [r7, #2]
	mov r9, r0
	add r5, #0x2C
	mov ip, r5
	mov r1, r9
	lsl r4, r1, #0x1F
	mov r0, #1
	mov r8, r0
	ldr r5, _0802F2C8 @ =0x00000D64
	ldr r1, _0802F2CC @ =0x000007FF
	mov sl, r1
_0802F264:
	lsr r0, r4, #0x1F
	mov r1, r8
	and r1, r0
	mov r0, #0x94
	add r3, r6, #0
	mul r3, r0
	add r0, r1, #0
	mul r0, r5
	add r0, r3, r0
	add r0, ip
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0802F2E0
	lsr r1, r4, #0x1F
	mov r0, r8
	and r0, r1
	add r1, r0, #0
	mul r1, r5
	add r1, r3, r1
	add r1, ip
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802F2E0
	mov r0, sl
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _0802F2D0 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0802F2D4 @ =0x00000105
	cmp r1, r0
	bne _0802F2AE
	b _0802F402
_0802F2AE:
	cmp r1, r0
	bgt _0802F2D8
	cmp r1, #0x58
	bne _0802F2B8
	b _0802F402
_0802F2B8:
	b _0802F2E0
	.align 2, 0
_0802F2BC: .4byte 0x020192E0
_0802F2C0: .4byte 0x00001B12
_0802F2C4: .4byte 0x0000058A
_0802F2C8: .4byte 0x00000D64
_0802F2CC: .4byte 0x000007FF
_0802F2D0: .4byte gCardIdToNumber
_0802F2D4: .4byte 0x00000105
_0802F2D8:
	ldr r0, _0802F370 @ =0x000001FF
	cmp r1, r0
	bne _0802F2E0
	b _0802F402
_0802F2E0:
	add r6, #1
	cmp r6, #4
	ble _0802F264
	ldr r4, _0802F374 @ =0x020192E4
	mov r0, r9
	lsl r2, r0, #0x1F
	mov r1, #1
	lsr r0, r2, #0x1F
	ldr r3, _0802F378 @ =0x00000D64
	mul r0, r3
	add r0, r0, r4
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1B
	cmp r0, #0
	bge _0802F300
	b _0802F436
_0802F300:
	mov r6, #0
	lsr r0, r2, #0x1F
	and r1, r0
	add r0, r1, #0
	mul r0, r3
	add r0, r0, r4
	ldrb r0, [r0, #2]
	cmp r6, r0
	blt _0802F314
	b _0802F436
_0802F314:
	ldr r1, _0802F37C @ =0x000007FF
	mov r8, r1
	mov r0, #0xF8
	lsl r0, r0, #0x11
	mov r9, r0
_0802F31E:
	ldrb r0, [r7, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	lsl r2, r6, #2
	ldr r0, _0802F378 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802F380 @ =0x02019968
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	add r0, r4, #0
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0802F384 @ =0x08621DE0
	add r5, r0, r1
	ldr r0, [r5]
	mov r1, r9
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0802F41E
	add r0, r4, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _0802F41E
	ldr r0, [r5]
	mov r1, r9
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0802F390
	cmp r0, #0x17
	ble _0802F388
	cmp r0, #0x18
	beq _0802F38C
	b _0802F390
	.align 2, 0
_0802F370: .4byte 0x000001FF
_0802F374: .4byte 0x020192E4
_0802F378: .4byte 0x00000D64
_0802F37C: .4byte 0x000007FF
_0802F380: .4byte 0x02019968
_0802F384: .4byte gCardStats
_0802F388:
	mov r0, #0
	b _0802F3A6
_0802F38C:
	mov r0, #0xA
	b _0802F3A6
_0802F390:
	add r0, r4, #0
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0802F3CC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0802F3A6:
	cmp r0, #4
	bls _0802F41E
	add r0, r4, #0
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0802F3CC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, r9
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0802F3D8
	cmp r0, #0x17
	ble _0802F3D0
	cmp r0, #0x18
	beq _0802F3D4
	b _0802F3D8
_0802F3CC: .4byte gCardStats
_0802F3D0:
	mov r0, #0
	b _0802F3EC
_0802F3D4:
	mov r0, #0xA
	b _0802F3EC
_0802F3D8:
	mov r0, r8
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _0802F408 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0802F3EC:
	cmp r0, #6
	bhi _0802F40C
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	beq _0802F41E
_0802F402:
	mov r0, #1
	b _0802F438
	.align 2, 0
_0802F408: .4byte gCardStats
_0802F40C:
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	bl CountTributableMonsters
	cmp r0, #0
	bgt _0802F402
_0802F41E:
	add r6, #1
	ldr r2, _0802F448 @ =0x020192E4
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802F44C @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r6, r0
	bge _0802F436
	b _0802F31E
_0802F436:
	mov r0, #0
_0802F438:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802F448: .4byte 0x020192E4
_0802F44C: .4byte 0x00000D64
	thumb_func_end EffectCanTributeOpponentMonsterPrepare

