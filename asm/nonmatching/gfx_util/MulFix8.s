	thumb_func_start MulFix8
MulFix8: @ 0x0807B4D0
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	mul r0, r1
	lsl r0, r0, #8
	asr r0, r0, #0x10
	bx lr
	thumb_func_end MulFix8

