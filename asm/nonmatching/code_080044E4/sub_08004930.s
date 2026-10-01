	thumb_func_start sub_08004930
sub_08004930: @ 0x08004930
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r6, r1, #0
	add r4, r2, #0
	mov r2, #1
	bl sub_080042D8
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_080042D8
	sub r4, #1
	add r0, r4, #0
	mov r1, #7
	bl __udivsi3
	add r0, #1
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08004930
	.align 2, 0

