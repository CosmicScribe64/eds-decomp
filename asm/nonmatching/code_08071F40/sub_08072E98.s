	thumb_func_start sub_08072E98
sub_08072E98: @ 0x08072E98
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r0, r0, #5
	ldr r3, _08072EAC @ =0x0300045C
	add r0, r0, r3
	lsr r1, r1, #0xF
	add r0, r0, r1
	strh r2, [r0]
	bx lr
	.align 2, 0
_08072EAC: .4byte 0x0300045C
	thumb_func_end sub_08072E98

