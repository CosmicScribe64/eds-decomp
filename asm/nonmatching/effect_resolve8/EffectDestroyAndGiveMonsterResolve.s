	thumb_func_start EffectDestroyAndGiveMonsterResolve
EffectDestroyAndGiveMonsterResolve: @ 0x08038EF0
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08038FAA
	mov r0, #7
	ldrb r1, [r4, #0xA]
	and r0, r1
	cmp r0, #2
	bne _08038FAA
	ldrb r5, [r4, #0xC]
	ldrh r0, [r4, #0xC]
	lsr r6, r0, #8
	ldrb r1, [r4, #0xE]
	mov r9, r1
	ldrh r0, [r4, #0xE]
	lsr r0, r0, #8
	mov r8, r0
	ldr r0, _08038F90 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08038F9C
	cmp r0, #0x80
	bne _08038FAA
	mov r0, #1
	mov ip, r0
	add r1, r5, #0
	and r1, r0
	mov r7, #0x94
	add r0, r6, #0
	mul r0, r7
	ldr r3, _08038F94 @ =0x00000D64
	mul r1, r3
	add r0, r0, r1
	ldr r2, _08038F98 @ =0x0201930C
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08038FAA
	mov r1, r9
	mov r0, ip
	and r1, r0
	mov r0, r8
	mul r0, r7
	mul r1, r3
	add r0, r0, r1
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08038FAA
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	cmp r5, r0
	beq _08038FAA
	cmp r9, r0
	bne _08038FAA
	add r0, r5, #0
	add r1, r6, #0
	bl DestroyFieldCardByEffect
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	add r2, r6, #0
	bl OnCardDestroyedByEffect
	mov r0, #0x7F
	b _08038FAC
_08038F90: .4byte 0x02017A40
_08038F94: .4byte 0x00000D64
_08038F98: .4byte 0x0201930C
_08038F9C:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r4, #0xE]
	ldrh r2, [r4, #0xC]
	bl MoveFieldCard
_08038FAA:
	mov r0, #0
_08038FAC:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectDestroyAndGiveMonsterResolve

