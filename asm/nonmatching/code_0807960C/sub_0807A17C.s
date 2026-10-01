	thumb_func_start sub_0807A17C
sub_0807A17C: @ 0x0807A17C
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	cmp r0, #0
	beq _0807A18C
	add r2, r1, #0
_0807A18C:
	add r0, r2, #0
	bx lr
	thumb_func_end sub_0807A17C

