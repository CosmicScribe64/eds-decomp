	thumb_func_start sub_0802408C
sub_0802408C: @ 0x0802408C
	ldr r2, _080240A4 @ =0x0201CFB0
	ldrb r1, [r2, #4]
	strb r1, [r2, #5]
	strb r0, [r2, #6]
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r2, #7]
	and r0, r1
	mov r1, #4
	orr r0, r1
	strb r0, [r2, #7]
	bx lr
_080240A4: .4byte 0x0201CFB0
	thumb_func_end sub_0802408C

