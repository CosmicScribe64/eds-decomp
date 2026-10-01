	thumb_func_start sub_080752E8
sub_080752E8: @ 0x080752E8
	add r2, r0, #0
	b _080752EE
_080752EC:
	add r2, #1
_080752EE:
	ldrb r0, [r2]
	cmp r0, #0
	bne _080752EC
	b _080752FC
_080752F6:
	strb r0, [r2]
	add r1, #1
	add r2, #1
_080752FC:
	ldrb r0, [r1]
	cmp r0, #0
	bne _080752F6
	mov r0, #0
	strb r0, [r2]
	bx lr
	thumb_func_end sub_080752E8

