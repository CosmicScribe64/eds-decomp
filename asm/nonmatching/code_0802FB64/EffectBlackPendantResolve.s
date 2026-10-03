	thumb_func_start EffectBlackPendantResolve
EffectBlackPendantResolve: @ 0x08030A28
	push {lr}
	add r2, r0, #0
	ldrb r3, [r2, #2]
	mov r0, #0xE
	and r0, r3
	cmp r0, #6
	beq _08030A42
	add r0, r2, #0
	bl EffectEquipResolve
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08030A5E
_08030A42:
	mov r0, #4
	ldrb r2, [r2, #4]
	and r0, r2
	cmp r0, #0
	bne _08030A5C
	lsl r1, r3, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0xFA
	lsl r1, r1, #1
	bl LoseLifePoints
_08030A5C:
	mov r0, #0
_08030A5E:
	pop {r1}
	bx r1
	thumb_func_end EffectBlackPendantResolve
	.align 2, 0

