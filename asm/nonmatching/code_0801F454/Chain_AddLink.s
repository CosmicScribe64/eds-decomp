	thumb_func_start Chain_AddLink
Chain_AddLink: @ 0x0801FBE0
	push {lr}
	add r3, r0, #0
	add r2, r1, #0
	mov r0, #1
	add r1, r3, #0
	bl Chain_Add
	pop {r0}
	bx r0
	thumb_func_end Chain_AddLink
	.align 2, 0

