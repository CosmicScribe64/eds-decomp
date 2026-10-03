	thumb_func_start DuelScreen_InitBgCnt
DuelScreen_InitBgCnt: @ 0x0806041C
	ldr r1, _0806043C @ =0x04000008
	mov r0, #7
	strh r0, [r1]
	add r1, #2
	ldr r2, _08060440 @ =0x00008106
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08060444 @ =0x00008305
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08060448 @ =0x00000504
	add r0, r2, #0
	strh r0, [r1]
	bx lr
_0806043C: .4byte 0x04000008
_08060440: .4byte 0x00008106
_08060444: .4byte 0x00008305
_08060448: .4byte 0x00000504
	thumb_func_end DuelScreen_InitBgCnt

