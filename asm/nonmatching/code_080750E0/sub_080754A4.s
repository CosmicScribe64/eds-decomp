	thumb_func_start sub_080754A4
sub_080754A4: @ 0x080754A4
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
	thumb_func_end sub_080754A4
	.align 2, 0

