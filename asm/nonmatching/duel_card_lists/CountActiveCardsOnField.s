	thumb_func_start CountActiveCardsOnField
CountActiveCardsOnField: @ 0x08008524
	push {lr}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	neg r2, r2
	bl CountActiveCardsOnFieldExcept
	pop {r1}
	bx r1
	thumb_func_end CountActiveCardsOnField
	.align 2, 0

