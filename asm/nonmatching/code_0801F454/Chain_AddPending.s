	thumb_func_start Chain_AddPending
Chain_AddPending: @ 0x0801FBCC
	push {lr}
	add r3, r0, #0
	add r2, r1, #0
	mov r0, #0
	add r1, r3, #0
	bl Chain_Add
	pop {r0}
	bx r0
	thumb_func_end Chain_AddPending
	.align 2, 0

