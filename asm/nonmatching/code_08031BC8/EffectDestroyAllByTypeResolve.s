	thumb_func_start EffectDestroyAllByTypeResolve
EffectDestroyAllByTypeResolve: @ 0x0803231C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _08032384
	mov r2, #0
_08032330:
	cmp r2, #0
	beq _0803233C
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r5, r0, #0x1F
	b _08032346
_0803233C:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r5, r1, r0
_08032346:
	mov r4, #0
	lsl r0, r5, #0x18
	add r2, #1
	mov r8, r2
	lsr r7, r0, #0x18
_08032350:
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	orr r1, r7
	add r0, r6, #0
	bl EffectDestroyByTypeCheck
	cmp r0, #0
	beq _08032378
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	add r2, r4, #0
	bl OnCardDestroyedByEffect
_08032378:
	add r4, #1
	cmp r4, #4
	ble _08032350
	mov r2, r8
	cmp r2, #1
	ble _08032330
_08032384:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectDestroyAllByTypeResolve

