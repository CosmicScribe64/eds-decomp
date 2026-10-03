	thumb_func_start EffectTributeRecoverGraveMagicPrepare
EffectTributeRecoverGraveMagicPrepare: @ 0x0802FC58
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r4, _0802FC8C @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802FC88
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802FC88
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802FC90 @ =0x0000059F
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	bgt _0802FC94
_0802FC88:
	mov r0, #0
	b _0802FC96
_0802FC8C: .4byte 0x0000058A
_0802FC90: .4byte 0x0000059F
_0802FC94:
	mov r0, #1
_0802FC96:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectTributeRecoverGraveMagicPrepare

