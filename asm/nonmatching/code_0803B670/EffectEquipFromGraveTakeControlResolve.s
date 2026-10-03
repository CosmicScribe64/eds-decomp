	thumb_func_start EffectEquipFromGraveTakeControlResolve
EffectEquipFromGraveTakeControlResolve: @ 0x0803BCE4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r5, r0, #0
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803BE20 @ =0x000005EA
	bl CountGraveyardCardsByNumber
	cmp r0, #0
	bne _0803BD04
	b _0803BE0E
_0803BD04:
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeSpellTrapZone
	mov r4, #1
	neg r4, r4
	mov sl, r4
	cmp r0, sl
	beq _0803BE0E
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	cmp r0, sl
	beq _0803BE0E
	mov r4, #7
	ldrb r3, [r5, #0xA]
	and r4, r3
	cmp r4, #1
	bne _0803BE0E
	ldrb r7, [r5, #0xC]
	ldrh r0, [r5, #0xC]
	lsr r0, r0, #8
	mov r9, r0
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	str r0, [sp, #0]
	ldrb r0, [r5, #2]
	lsl r3, r0, #0x1F
	lsr r0, r3, #0x1F
	cmp r7, r0
	beq _0803BE0E
	add r1, r7, #0
	and r1, r4
	mov r0, #0x94
	mov r2, r9
	mul r2, r0
	ldr r0, _0803BE24 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0803BE28 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803BE0E
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0803BE0E
	lsr r0, r3, #0x1F
	bl FindFreeSpellTrapZone
	mov r8, r0
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r5]
	ldr r6, _0803BE2C @ =0x02017F84
	add r2, r6, #0
	bl GetGraveyardCardById
	add r0, r4, #0
	ldrb r3, [r5, #2]
	and r0, r3
	mov r3, #0xD3
	cmp r0, #0
	beq _0803BD9A
	ldr r3, _0803BE30 @ =0x000080D3
_0803BD9A:
	ldrh r1, [r6]
	ldrh r2, [r6, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r5, #2]
	and r4, r0
	mov r1, #0x77
	mov ip, r1
	cmp r4, #0
	beq _0803BDB6
	ldr r3, _0803BE34 @ =0x00008077
	mov ip, r3
_0803BDB6:
	mov r0, r8
	lsl r4, r0, #0x18
	lsr r4, r4, #0x18
	mov r1, #0x80
	lsl r1, r1, #1
	add r0, r1, #0
	add r1, r4, #0
	orr r1, r0
	ldrh r2, [r6]
	ldrh r3, [r6, #2]
	mov r0, ip
	bl DuelCmd_Push
	ldrb r3, [r5, #2]
	lsl r1, r3, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	lsl r4, r4, #8
	orr r1, r4
	mov r4, r9
	lsl r2, r4, #8
	orr r7, r2
	add r2, r7, #0
	bl EquipCard
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _0803BE0E
	ldr r3, [sp, #0]
	cmp r3, sl
	beq _0803BE0E
	ldrb r4, [r5, #2]
	lsl r3, r4, #0x1F
	lsr r0, r3, #0x1F
	ldrh r1, [r5, #0xC]
	add r3, r0, #0
	ldr r4, [sp, #0]
	lsl r2, r4, #0x18
	lsr r2, r2, #0x10
	orr r2, r3
	bl MoveFieldCard
_0803BE0E:
	mov r0, #0
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0803BE20: .4byte 0x000005EA
_0803BE24: .4byte 0x00000D64
_0803BE28: .4byte 0x0201930C
_0803BE2C: .4byte 0x02017F84
_0803BE30: .4byte 0x000080D3
_0803BE34: .4byte 0x00008077
	thumb_func_end EffectEquipFromGraveTakeControlResolve

