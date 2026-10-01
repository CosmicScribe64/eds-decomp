	thumb_func_start sub_080126D4
sub_080126D4: @ 0x080126D4
	ldr r1, _080126E8 @ =0x020185C0
	ldr r0, _080126EC @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	bx lr
	.align 2, 0
_080126E8: .4byte 0x020185C0
_080126EC: .4byte 0x0000080D
	thumb_func_end sub_080126D4

