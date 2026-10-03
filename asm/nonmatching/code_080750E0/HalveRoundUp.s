	thumb_func_start HalveRoundUp
HalveRoundUp: @ 0x0807548C
	push {lr}
	add r1, r0, #0
	lsl r0, r1, #2
	add r0, r0, r1
	add r1, r0, #5
	add r0, r1, #0
	mov r1, #0xA
	bl __divsi3
	pop {r1}
	bx r1
	thumb_func_end HalveRoundUp
	.align 2, 0

