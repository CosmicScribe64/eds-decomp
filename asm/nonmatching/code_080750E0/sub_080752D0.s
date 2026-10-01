	thumb_func_start sub_080752D0
sub_080752D0: @ 0x080752D0
	add r2, r0, #0
	b _080752DA
_080752D4:
	strb r0, [r2]
	add r1, #1
	add r2, #1
_080752DA:
	ldrb r0, [r1]
	cmp r0, #0
	bne _080752D4
	mov r0, #0
	strb r0, [r2]
	bx lr
	thumb_func_end sub_080752D0
	.align 2, 0

