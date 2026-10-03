	thumb_func_start StepList_Init
StepList_Init: @ 0x0807B088
	mov r2, #0
	strb r2, [r1]
	str r0, [r1, #4]
	bx lr
	thumb_func_end StepList_Init

