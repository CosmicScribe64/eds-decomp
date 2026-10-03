	thumb_func_start MulFix8Wide
MulFix8Wide: @ 0x0807B4E0
	push {r4, r5, lr}
	add r4, r0, #0
	add r2, r1, #0
	asr r5, r4, #0x1F
	asr r3, r2, #0x1F
	add r1, r5, #0
	add r0, r4, #0
	bl __muldi3
	lsl r5, r1, #0x18
	lsr r4, r0, #8
	add r2, r5, #0
	orr r2, r4
	add r0, r2, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end MulFix8Wide
	.align 2, 0

