	thumb_func_start DestinyBoardScene_DisableHBlank
DestinyBoardScene_DisableHBlank: @ 0x08027C34
	ldr r3, _08027C4C @ =0x04000208
	mov r0, #0
	strh r0, [r3]
	ldr r2, _08027C50 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _08027C54 @ =0x0000FFFD
	and r0, r1
	strh r0, [r2]
	mov r0, #1
	strh r0, [r3]
	mov r0, #1
	bx lr
_08027C4C: .4byte 0x04000208
_08027C50: .4byte 0x04000200
_08027C54: .4byte 0x0000FFFD
	thumb_func_end DestinyBoardScene_DisableHBlank

