	thumb_func_start sub_0807CB64
sub_0807CB64: @ 0x0807CB64
	push {lr}
	mov r0, #2
	bl sub_08075A6C
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end sub_0807CB64

