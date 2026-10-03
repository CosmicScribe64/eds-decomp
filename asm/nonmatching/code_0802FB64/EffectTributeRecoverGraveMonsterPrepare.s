	thumb_func_start EffectTributeRecoverGraveMonsterPrepare
EffectTributeRecoverGraveMonsterPrepare: @ 0x0802FBF4
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r4, _0802FC28 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802FC24
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802FC24
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802FC2C @ =0x0000058D
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	bgt _0802FC30
_0802FC24:
	mov r0, #0
	b _0802FC32
_0802FC28: .4byte 0x0000058A
_0802FC2C: .4byte 0x0000058D
_0802FC30:
	mov r0, #1
_0802FC32:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectTributeRecoverGraveMonsterPrepare

