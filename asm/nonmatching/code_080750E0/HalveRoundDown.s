	thumb_func_start HalveRoundDown
HalveRoundDown: @ 0x080754A4
	push {lr}
	add r1, r0, #0
	lsl r0, r1, #2
	add r0, r0, r1
	add r1, r0, #4
	add r0, r1, #0
	mov r1, #0xA
	bl __divsi3
	pop {r1}
	bx r1
	thumb_func_end HalveRoundDown
	.align 2, 0

