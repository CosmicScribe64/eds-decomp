	thumb_func_start CanEffectTargetZone
CanEffectTargetZone: @ 0x080470C0
	push {r4, r5, r6, lr}
	add r4, r0, #0
	add r5, r1, #0
	add r6, r2, #0
	cmp r4, #0
	beq _0804710A
	ldrh r0, [r4]
	bl FindCardEffect
	add r2, r0, #0
	cmp r2, #0
	blt _080470EA
	ldr r1, _080470F0 @ =0x0819A9D4
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #3
	add r1, #8
	add r0, r0, r1
	ldr r2, [r0]
	cmp r2, #0
	bne _080470F4
_080470EA:
	mov r0, #1
	b _0804710C
	.align 2, 0
_080470F0: .4byte gCardEffects
_080470F4:
	lsl r1, r5, #0x18
	lsl r0, r6, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r1, r1, #0x10
	add r0, r4, #0
	bl _call_via_r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0804710C
_0804710A:
	mov r0, #0
_0804710C:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end CanEffectTargetZone
	.align 2, 0

