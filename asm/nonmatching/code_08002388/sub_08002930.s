	thumb_func_start sub_08002930
sub_08002930: @ 0x08002930
	push {lr}
	mov r0, #4
	bl sub_08075A6C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end sub_08002930

