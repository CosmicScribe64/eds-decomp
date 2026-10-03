	thumb_func_start SetOamAffineShear
SetOamAffineShear: @ 0x080761CC
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	ldr r3, _080761EC @ =0x03004470
	lsr r0, r0, #0xB
	add r3, r0, r3
	strh r1, [r3, #6]
	strh r2, [r3, #0xE]
	lsl r2, r2, #0x10
	asr r2, r2, #0x10
	neg r2, r2
	strh r2, [r3, #0x16]
	strh r1, [r3, #0x1E]
	bx lr
_080761EC: .4byte 0x03004470
	thumb_func_end SetOamAffineShear

