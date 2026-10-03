	thumb_func_start DivFix8
DivFix8: @ 0x0807B504
	push {lr}
	lsl r0, r0, #0x10
	asr r0, r0, #8
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	bl Div
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end DivFix8
	.align 2, 0

