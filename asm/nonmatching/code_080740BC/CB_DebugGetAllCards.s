	thumb_func_start CB_DebugGetAllCards
CB_DebugGetAllCards: @ 0x08074480
	push {lr}
	bl DebugGetAllCards
	mov r0, #1
	pop {r1}
	bx r1
	thumb_func_end CB_DebugGetAllCards

