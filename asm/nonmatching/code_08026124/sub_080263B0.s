	thumb_func_start sub_080263B0
sub_080263B0: @ 0x080263B0
	push {lr}
	lsl r2, r2, #5
	add r0, r0, r2
	lsl r1, r1, #5
	ldr r2, _080263C4 @ =0x06014000
	add r1, r1, r2
	bl sub_08026388
	pop {r0}
	bx r0
_080263C4: .4byte 0x06014000
	thumb_func_end sub_080263B0

