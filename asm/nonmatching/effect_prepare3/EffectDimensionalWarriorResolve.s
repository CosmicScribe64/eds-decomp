	thumb_func_start EffectDimensionalWarriorResolve
EffectDimensionalWarriorResolve: @ 0x080307AC
	push {r4, r5, lr}
	ldrb r2, [r0, #6]
	ldrh r3, [r0, #6]
	lsr r1, r3, #8
	ldrb r4, [r0, #8]
	ldrh r0, [r0, #8]
	lsr r5, r0, #8
	add r0, r2, #0
	mov r2, #0
	bl BanishFieldCard
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0
	bl BanishFieldCard
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectDimensionalWarriorResolve

