	thumb_func_start EffectFusionSageResolve
EffectFusionSageResolve: @ 0x080355C0
	push {r4, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08035608 @ =0x000003EB
	bl AddDeckCardToHand
	cmp r0, #0
	bne _080355E4
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803560C @ =0x0000040A
	bl AddDeckCardToHand
	cmp r0, #0
	beq _080355FE
_080355E4:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x60
	cmp r0, #0
	beq _080355F2
	ldr r1, _08035610 @ =0x00008060
_080355F2:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_080355FE:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08035608: .4byte 0x000003EB
_0803560C: .4byte 0x0000040A
_08035610: .4byte 0x00008060
	thumb_func_end EffectFusionSageResolve

