	thumb_func_start EffectJustDessertsResolve
EffectJustDessertsResolve: @ 0x08035580
	push {r4, r5, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r5, #1
	sub r0, r5, r0
	bl CountMonsters
	add r2, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _080355B8
	cmp r2, #0
	ble _080355B8
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	lsl r1, r2, #5
	sub r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r2
	lsl r1, r1, #2
	bl LoseLifePoints
_080355B8:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectJustDessertsResolve

