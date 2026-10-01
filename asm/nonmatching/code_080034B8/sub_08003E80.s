	thumb_func_start sub_08003E80
sub_08003E80: @ 0x08003E80
	push {lr}
	bl sub_0800406C
	mov r0, #2
	bl sub_08075A6C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end sub_08003E80

