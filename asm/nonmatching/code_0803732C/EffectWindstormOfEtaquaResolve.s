	thumb_func_start EffectWindstormOfEtaquaResolve
EffectWindstormOfEtaquaResolve: @ 0x0803821C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r7, r0, #0
	mov r0, #0
	mov sl, r0
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	bne _080382E8
	mov r4, #0xA4
	lsl r4, r4, #1
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08038252
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _08038256
_08038252:
	mov r0, #1
	mov sl, r0
_08038256:
	mov r5, #0
	mov r6, #1
	ldr r1, _080382F8 @ =0x00000D64
	mov r9, r1
	ldr r0, _080382FC @ =0x0201930C
	mov r8, r0
_08038262:
	ldrb r1, [r7, #2]
	lsl r3, r1, #0x1F
	lsr r1, r3, #0x1F
	sub r1, r6, r1
	and r1, r6
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	mov r0, r9
	mul r0, r1
	add r0, r2, r0
	add r0, r8
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080382E2
	lsr r0, r3, #0x1F
	sub r0, r6, r0
	and r0, r6
	mov r1, r9
	mul r1, r0
	add r1, r2, r1
	add r1, r8
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080382E2
	mov r4, #1
	mov r0, sl
	cmp r0, #0
	beq _080382CC
	lsr r0, r3, #0x1F
	sub r0, r4, r0
	and r0, r4
	mov r1, r9
	mul r1, r0
	add r1, r2, r1
	add r1, r8
	add r0, r4, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080382CC
	lsr r0, r3, #0x1F
	sub r0, r4, r0
	add r1, r5, #0
	bl GetZoneCardType
	eor r0, r4
	neg r1, r0
	orr r1, r0
	lsr r4, r1, #0x1F
_080382CC:
	cmp r4, #0
	beq _080382E2
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r6, r0
	add r1, r5, #0
	mov r2, #0
	mov r3, #0
	bl ChangeBattlePosition
_080382E2:
	add r5, #1
	cmp r5, #4
	ble _08038262
_080382E8:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_080382F8: .4byte 0x00000D64
_080382FC: .4byte 0x0201930C
	thumb_func_end EffectWindstormOfEtaquaResolve

