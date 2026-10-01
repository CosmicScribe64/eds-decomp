	thumb_func_start sub_08011C98
sub_08011C98: @ 0x08011C98
	ldr r1, _08011CAC @ =0x020185C0
	ldr r0, _08011CB0 @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	bx lr
	.align 2, 0
_08011CAC: .4byte 0x020185C0
_08011CB0: .4byte 0x0000080D
	thumb_func_end sub_08011C98

