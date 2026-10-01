	thumb_func_start sub_08008524
sub_08008524: @ 0x08008524
	push {lr}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r2, #1
	neg r2, r2
	bl sub_0800849C
	pop {r1}
	bx r1
	thumb_func_end sub_08008524
	.align 2, 0

