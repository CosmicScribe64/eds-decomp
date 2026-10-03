	thumb_func_start EffectDestroyAllOnSummonPrepare
EffectDestroyAllOnSummonPrepare: @ 0x0802FB64
	push {lr}
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802FB84
	ldrb r3, [r0, #3]
	lsr r2, r3, #2
	cmp r2, #7
	bgt _0802FB84
	cmp r2, #5
	blt _0802FB84
	mov r2, #0
	bl EffectDarkHolePrepare
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0802FB86
_0802FB84:
	mov r0, #0
_0802FB86:
	pop {r1}
	bx r1
	thumb_func_end EffectDestroyAllOnSummonPrepare
	.align 2, 0

