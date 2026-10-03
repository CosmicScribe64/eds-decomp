	thumb_func_start EffectSebeksBlessingPrepare
EffectSebeksBlessingPrepare: @ 0x0802EFA0
	add r2, r0, #0
	ldrb r1, [r2, #3]
	lsr r0, r1, #2
	cmp r0, #0xD
	bne _0802EFBE
	mov r1, #0xF
	ldrb r3, [r2, #2]
	lsl r0, r3, #0x1F
	ldrb r2, [r2, #8]
	and r1, r2
	lsr r0, r0, #0x1F
	cmp r1, r0
	beq _0802EFBE
	mov r0, #1
	b _0802EFC0
_0802EFBE:
	mov r0, #0
_0802EFC0:
	bx lr
	thumb_func_end EffectSebeksBlessingPrepare
	.align 2, 0

