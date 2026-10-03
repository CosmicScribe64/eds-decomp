	thumb_func_start EffectDragonSeekerResolve
EffectDragonSeekerResolve: @ 0x08031BC8
	push {r4, r5, r6, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08031C0A
	mov r0, #7
	ldrb r1, [r4, #0xA]
	and r0, r1
	cmp r0, #1
	bne _08031C0A
	ldrb r6, [r4, #0xC]
	ldrh r0, [r4, #0xC]
	lsr r5, r0, #8
	lsl r1, r5, #8
	orr r1, r6
	add r0, r4, #0
	bl EffectDragonSeekerCheck
	cmp r0, #0
	beq _08031C0A
	add r0, r6, #0
	add r1, r5, #0
	bl DestroyFieldCardByEffect
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	add r1, r6, #0
	add r2, r5, #0
	bl OnCardDestroyedByEffect
_08031C0A:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectDragonSeekerResolve
	.align 2, 0

