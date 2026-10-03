	thumb_func_start ExodiaScene_StartPieceFlight
ExodiaScene_StartPieceFlight: @ 0x080261A8
	push {r4, r5, r6, lr}
	sub sp, #4
	add r5, r0, #0
	mov r4, #0
	ldr r6, _080261DC @ =0x080823B0
_080261B2:
	lsl r1, r4, #2
	add r1, r1, r6
	mov r2, #2
	ldsh r0, [r1, r2]
	mov r2, #0
	ldsh r1, [r1, r2]
	str r5, [sp, #0]
	add r5, #0x14
	mov r2, #0x68
	mov r3, #0x40
	bl LineInit
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #4
	bls _080261B2
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_080261DC: .4byte gExodiaPieceStartPos
	thumb_func_end ExodiaScene_StartPieceFlight

