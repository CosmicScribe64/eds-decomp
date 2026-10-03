	thumb_func_start Timer_Start
Timer_Start: @ 0x0807B0C8
	mov r2, #1
	strb r2, [r0]
	strh r1, [r0, #2]
	bx lr
	thumb_func_end Timer_Start

