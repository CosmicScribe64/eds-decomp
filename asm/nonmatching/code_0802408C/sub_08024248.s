	thumb_func_start sub_08024248
sub_08024248: @ 0x08024248
	push {lr}
	add r3, r2, #0
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	ldr r2, _08024264 @ =0x0201D7F8
	bl sub_08076BEC
	pop {r0}
	bx r0
	.align 2, 0
_08024264: .4byte 0x0201D7F8
	thumb_func_end sub_08024248

