	thumb_func_start sub_0806D634
sub_0806D634: @ 0x0806D634
	push {lr}
	lsl r1, r1, #5
	add r0, r0, r1
	mov r1, #0x20
	bl sub_08075278
	pop {r0}
	bx r0
	thumb_func_end sub_0806D634

