	thumb_func_start CanActivateEffectOfCard
CanActivateEffectOfCard: @ 0x0802CFA0
	push {r4, r5, lr}
	sub sp, #0x14
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r5, sp
	mov r3, #1
	and r0, r3
	ldrb r4, [r5, #2]
	mov r3, #2
	neg r3, r3
	and r3, r4
	orr r3, r0
	strb r3, [r5, #2]
	mov r0, sp
	strh r1, [r0]
	mov r1, #0
	bl CanActivateEffect
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add sp, #0x14
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end CanActivateEffectOfCard

