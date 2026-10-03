	thumb_func_start CopyDoubleWords
CopyDoubleWords: @ 0x080752B0
	push {r4, lr}
	add r4, r1, #0
	add r3, r0, #0
	add r0, r2, #7
	lsr r2, r0, #3
	cmp r2, #0
	beq _080752C8
_080752BE:
	ldmia r4!, {r0, r1}
	stmia r3!, {r0, r1}
	sub r2, #1
	cmp r2, #0
	bne _080752BE
_080752C8:
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end CopyDoubleWords
	.align 2, 0

