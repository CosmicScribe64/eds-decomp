	thumb_func_start Timer_Reset
Timer_Reset: @ 0x0807B0C0
	mov r1, #0
	strb r1, [r0]
	strh r1, [r0, #2]
	bx lr
	thumb_func_end Timer_Reset

