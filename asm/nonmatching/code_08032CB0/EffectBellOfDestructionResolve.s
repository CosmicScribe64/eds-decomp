	thumb_func_start EffectBellOfDestructionResolve
EffectBellOfDestructionResolve: @ 0x08033404
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _080334D2
	mov r7, #7
	ldrb r0, [r4, #0xA]
	and r7, r0
	cmp r7, #1
	bne _080334D2
	ldrb r6, [r4, #0xC]
	ldrh r1, [r4, #0xC]
	lsr r5, r1, #8
	add r1, r6, #0
	and r1, r7
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r0, _08033488 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0803348C @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080334D2
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _080334D2
	add r0, r6, #0
	add r1, r5, #0
	bl GetZoneCardAtk
	mov r8, r0
	add r0, r6, #0
	add r1, r5, #0
	bl DestroyFieldCardByEffect
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r6, #0
	add r2, r5, #0
	bl OnCardDestroyedByEffect
	ldr r0, _08033490 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08033494 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08033498 @ =0x000003AB
	cmp r1, r0
	beq _080334A0
	ldr r0, _0803349C @ =0x000005AB
	cmp r1, r0
	beq _080334BC
	b _080334D2
_08033488: .4byte 0x00000D64
_0803348C: .4byte 0x0201930C
_08033490: .4byte 0x000007FF
_08033494: .4byte gCardIdToNumber
_08033498: .4byte 0x000003AB
_0803349C: .4byte 0x000005AB
_080334A0:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r7, r0
	mov r1, r8
	bl LoseLifePoints
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	mov r1, r8
	bl LoseLifePoints
	b _080334D2
_080334BC:
	ldrb r4, [r4, #2]
	and r7, r4
	mov r0, #0x44
	cmp r7, #0
	beq _080334C8
	ldr r0, _080334E0 @ =0x00008044
_080334C8:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_080334D2:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080334E0: .4byte 0x00008044
	thumb_func_end EffectBellOfDestructionResolve

