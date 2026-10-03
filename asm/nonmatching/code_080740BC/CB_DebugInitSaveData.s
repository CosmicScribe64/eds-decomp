	thumb_func_start CB_DebugInitSaveData
CB_DebugInitSaveData: @ 0x08074474
	push {lr}
	bl InitSaveData
	mov r0, #1
	pop {r1}
	bx r1
	thumb_func_end CB_DebugInitSaveData

