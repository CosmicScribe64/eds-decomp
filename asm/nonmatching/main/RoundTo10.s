	thumb_func_start RoundTo10
RoundTo10: @ 0x08075474
	push {lr}
	add r0, #5
	mov r1, #0xA
	bl __divsi3
	add r1, r0, #0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
	pop {r1}
	bx r1
	thumb_func_end RoundTo10
	.align 2, 0

