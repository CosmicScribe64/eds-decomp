	thumb_func_start sub_08078CA8
sub_08078CA8: @ 0x08078CA8
	str r0, [r1, #4]
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r0, #0
	mov r2, #2
	strb r2, [r1, #0x10]
	strb r0, [r1, #8]
	strb r2, [r1, #0x1C]
	strb r0, [r1, #0xD]
	strb r0, [r1, #0xC]
	strb r0, [r1, #0x1D]
	bx lr
	thumb_func_end sub_08078CA8
	.align 2, 0

