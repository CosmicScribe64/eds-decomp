	thumb_func_start SetTextModeLatin
SetTextModeLatin: @ 0x080770DC
	push {lr}
	mov r0, #1
	bl SetTextMode
	pop {r0}
	bx r0
	thumb_func_end SetTextModeLatin

