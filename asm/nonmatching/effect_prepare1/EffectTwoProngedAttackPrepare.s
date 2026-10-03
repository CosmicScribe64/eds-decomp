	thumb_func_start EffectTwoProngedAttackPrepare
EffectTwoProngedAttackPrepare: @ 0x0802DF5C
	push {r4, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CountMonsters
	cmp r0, #1
	ble _0802DF84
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	bl CountMonsters
	cmp r0, #0
	ble _0802DF84
	mov r0, #1
	b _0802DF86
_0802DF84:
	mov r0, #0
_0802DF86:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectTwoProngedAttackPrepare

