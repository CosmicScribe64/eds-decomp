	thumb_func_start LinkBattle_ErrorShutdown
LinkBattle_ErrorShutdown: @ 0x0801AC7C
	push {lr}
	bl LinkShutdown
	mov r0, #1
	pop {r1}
	bx r1
	thumb_func_end LinkBattle_ErrorShutdown

