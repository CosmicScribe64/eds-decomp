	thumb_func_start sub_08021CCC
sub_08021CCC: @ 0x08021CCC
	ldr r0, _08021CE0 @ =0x020192E0
	ldr r2, _08021CE4 @ =0x00001B43
	add r1, r0, r2
	mov r2, #0
	strb r2, [r1]
	ldr r1, _08021CE8 @ =0x00001B44
	add r0, r0, r1
	strb r2, [r0]
	bx lr
	.align 2, 0
_08021CE0: .4byte 0x020192E0
_08021CE4: .4byte 0x00001B43
_08021CE8: .4byte 0x00001B44
	thumb_func_end sub_08021CCC

