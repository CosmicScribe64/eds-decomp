	thumb_func_start TurnOrder_ShowWaitSign
TurnOrder_ShowWaitSign: @ 0x08027C90
	ldr r0, _08027C9C @ =0x02020310
	ldr r1, _08027CA0 @ =0x00000926
	add r0, r0, r1
	mov r1, #1
	strb r1, [r0]
	bx lr
_08027C9C: .4byte 0x02020310
_08027CA0: .4byte 0x00000926
	thumb_func_end TurnOrder_ShowWaitSign

