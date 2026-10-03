	thumb_func_start IsSeEnabled
IsSeEnabled: @ 0x08077A44
	ldr r1, _08077A54 @ =0x02011C20
	ldr r0, _08077A58 @ =0x00002152
	add r1, r1, r0
	mov r0, #1
	ldrh r1, [r1]
	and r0, r1
	bx lr
	.align 2, 0
_08077A54: .4byte 0x02011C20
_08077A58: .4byte 0x00002152
	thumb_func_end IsSeEnabled

