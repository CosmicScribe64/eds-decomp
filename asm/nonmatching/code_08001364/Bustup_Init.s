	thumb_func_start Bustup_Init
Bustup_Init: @ 0x08001364
	push {lr}
	mov r0, #0
	bl Bustup_InitState
	mov r0, #1
	pop {r1}
	bx r1
	thumb_func_end Bustup_Init
	.align 2, 0

