	thumb_func_start EffectGracefulDicePrepare
EffectGracefulDicePrepare: @ 0x0802F068
	push {lr}
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	mov r2, #0
	bl CountMonstersFiltered
	mov r1, #0
	cmp r0, #0
	ble _0802F080
	mov r1, #1
_0802F080:
	add r0, r1, #0
	pop {r1}
	bx r1
	thumb_func_end EffectGracefulDicePrepare
	.align 2, 0

