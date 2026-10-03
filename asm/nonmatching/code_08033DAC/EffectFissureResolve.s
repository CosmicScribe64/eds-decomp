	thumb_func_start EffectFissureResolve
EffectFissureResolve: @ 0x08034044
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r6, r0, #0
	ldr r0, _080340FC @ =0x0001869F
	mov sl, r0
	mov r7, #1
	neg r7, r7
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _08034142
	mov r4, #0
	mov r5, #1
	ldr r0, _08034100 @ =0x00000D64
	mov r9, r0
	ldr r1, _08034104 @ =0x0201930C
	mov r8, r1
_0803406E:
	ldrb r0, [r6, #2]
	lsl r2, r0, #0x1F
	lsr r1, r2, #0x1F
	sub r1, r5, r1
	and r1, r5
	mov r0, #0x94
	add r3, r4, #0
	mul r3, r0
	mov r0, r9
	mul r0, r1
	add r0, r3, r0
	add r0, r8
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080340B8
	lsr r0, r2, #0x1F
	sub r0, r5, r0
	and r0, r5
	mov r1, r9
	mul r1, r0
	add r1, r3, r1
	add r1, r8
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080340B8
	lsr r0, r2, #0x1F
	sub r0, r5, r0
	add r1, r4, #0
	bl GetZoneCardAtk
	cmp r0, sl
	bge _080340B8
	mov sl, r0
	add r7, r4, #0
_080340B8:
	add r4, #1
	cmp r4, #4
	ble _0803406E
	mov r0, #1
	neg r0, r0
	cmp r7, r0
	beq _08034142
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r4, #1
	sub r0, r4, r0
	add r1, r7, #0
	bl IsZoneTargetable
	cmp r0, #0
	beq _08034108
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r4, r0
	add r1, r7, #0
	bl DestroyFieldCardByEffect
	ldrb r6, [r6, #2]
	lsl r1, r6, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	sub r1, r4, r1
	add r2, r7, #0
	bl OnCardDestroyedByEffect
	b _08034142
	.align 2, 0
_080340FC: .4byte 0x0001869F
_08034100: .4byte 0x00000D64
_08034104: .4byte 0x0201930C
_08034108:
	ldrb r6, [r6, #2]
	lsl r3, r6, #0x1F
	lsr r0, r3, #0x1F
	sub r0, r4, r0
	and r0, r4
	mov r1, #0x94
	add r2, r7, #0
	mul r2, r1
	ldr r6, _08034154 @ =0x00000D64
	mul r0, r6
	add r0, r2, r0
	ldr r5, _08034158 @ =0x0201930C
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08034142
	lsr r0, r3, #0x1F
	add r1, r0, #0
	sub r1, r4, r1
	and r1, r4
	mul r1, r6
	add r1, r2, r1
	add r1, r1, r5
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl ShowCardEffect
_08034142:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08034154: .4byte 0x00000D64
_08034158: .4byte 0x0201930C
	thumb_func_end EffectFissureResolve

