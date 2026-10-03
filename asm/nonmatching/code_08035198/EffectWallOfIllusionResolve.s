	thumb_func_start EffectWallOfIllusionResolve
EffectWallOfIllusionResolve: @ 0x08035764
	push {lr}
	ldrb r2, [r0, #6]
	ldrh r0, [r0, #6]
	lsr r1, r0, #8
	add r0, r2, #0
	mov r2, #0
	bl ReturnFieldCardToHand
	mov r0, #0
	pop {r1}
	bx r1
	thumb_func_end EffectWallOfIllusionResolve
	.align 2, 0

