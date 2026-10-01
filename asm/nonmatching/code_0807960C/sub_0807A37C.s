	thumb_func_start sub_0807A37C
sub_0807A37C: @ 0x0807A37C
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsl r3, r0, #1
	add r3, r3, r0
	lsl r3, r3, #2
	add r3, r2, r3
	add r2, r2, r1
	ldrb r1, [r2]
	strb r1, [r3, #0x1C]
	strb r0, [r2]
	bx lr
	thumb_func_end sub_0807A37C
	.align 2, 0

