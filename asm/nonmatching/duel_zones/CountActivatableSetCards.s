	thumb_func_start CountActivatableSetCards
CountActivatableSetCards: @ 0x08009280
	push {lr}
	add r3, r0, #0
	lsl r2, r1, #0x10
	lsr r2, r2, #0x10
	ldr r0, _08009294 @ =0x020192E4
	add r1, r3, #0
	bl CountActivatableSetCardsIn
	pop {r1}
	bx r1
_08009294: .4byte 0x020192E4
	thumb_func_end CountActivatableSetCards

