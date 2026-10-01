	thumb_func_start sub_0801D140
sub_0801D140: @ 0x0801D140
	ldr r0, _0801D150 @ =0x02011C20
	ldr r1, _0801D154 @ =0x00002150
	add r0, r0, r1
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	mov r0, #1
	bx lr
_0801D150: .4byte 0x02011C20
_0801D154: .4byte 0x00002150
	thumb_func_end sub_0801D140

