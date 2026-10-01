	thumb_func_start sub_08027CA4
sub_08027CA4: @ 0x08027CA4
	ldr r0, _08027CB0 @ =0x02020310
	ldr r1, _08027CB4 @ =0x00000926
	add r0, r0, r1
	mov r1, #0xFF
	strb r1, [r0]
	bx lr
_08027CB0: .4byte 0x02020310
_08027CB4: .4byte 0x00000926
	thumb_func_end sub_08027CA4

