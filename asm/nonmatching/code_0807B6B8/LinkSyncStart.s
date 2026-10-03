	thumb_func_start LinkSyncStart
LinkSyncStart: @ 0x0807BCF4
	mov r1, #0
	strb r1, [r0, #8]
	bx lr
	thumb_func_end LinkSyncStart
	.align 2, 0

