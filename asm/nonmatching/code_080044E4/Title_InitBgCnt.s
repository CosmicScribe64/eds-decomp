	thumb_func_start Title_InitBgCnt
Title_InitBgCnt: @ 0x08004FA4
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r0, #0
	strh r0, [r1]
	add r1, #8
	mov r0, #0x84
	strh r0, [r1]
	add r1, #2
	ldr r2, _08004FCC @ =0x00000105
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08004FD0 @ =0x00000206
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08004FD4 @ =0x00000307
	add r0, r2, #0
	strh r0, [r1]
	bx lr
_08004FCC: .4byte 0x00000105
_08004FD0: .4byte 0x00000206
_08004FD4: .4byte 0x00000307
	thumb_func_end Title_InitBgCnt

