	thumb_func_start sub_08076144
sub_08076144: @ 0x08076144
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r2, _0807615C @ =0x03004470
	lsr r0, r0, #0xB
	add r2, r0, r2
	strh r1, [r2, #6]
	mov r0, #0
	strh r0, [r2, #0xE]
	strh r0, [r2, #0x16]
	strh r1, [r2, #0x1E]
	bx lr
_0807615C: .4byte 0x03004470
	thumb_func_end sub_08076144

