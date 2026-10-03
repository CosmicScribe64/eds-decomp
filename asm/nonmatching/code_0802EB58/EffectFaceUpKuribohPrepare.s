	thumb_func_start EffectFaceUpKuribohPrepare
EffectFaceUpKuribohPrepare: @ 0x0802F450
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r4, _0802F484 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802F47E
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0802F47E
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0x39
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _0802F488
_0802F47E:
	mov r0, #0
	b _0802F48A
	.align 2, 0
_0802F484: .4byte 0x0000058A
_0802F488:
	mov r0, #1
_0802F48A:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectFaceUpKuribohPrepare

