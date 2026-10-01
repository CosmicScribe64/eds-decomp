	thumb_func_start sub_08026194
sub_08026194: @ 0x08026194
	ldr r0, _080261A0 @ =0x02020310
	ldr r1, _080261A4 @ =0x00000AAC
	add r0, r0, r1
	mov r1, #0
	strb r1, [r0]
	bx lr
_080261A0: .4byte 0x02020310
_080261A4: .4byte 0x00000AAC
	thumb_func_end sub_08026194

