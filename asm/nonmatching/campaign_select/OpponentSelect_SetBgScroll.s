	thumb_func_start OpponentSelect_SetBgScroll
OpponentSelect_SetBgScroll: @ 0x08002D34
	ldr r1, _08002D44 @ =0x04000028
	lsl r0, r0, #9
	strh r0, [r1]
	add r1, #2
	lsl r0, r0, #4
	lsr r0, r0, #0x14
	strh r0, [r1]
	bx lr
_08002D44: .4byte 0x04000028
	thumb_func_end OpponentSelect_SetBgScroll

