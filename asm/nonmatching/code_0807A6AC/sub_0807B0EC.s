	thumb_func_start sub_0807B0EC
sub_0807B0EC: @ 0x0807B0EC
	push {r4, lr}
	mov r4, #0
	strb r4, [r3]
	strh r0, [r3, #2]
	strh r1, [r3, #4]
	strh r2, [r3, #6]
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end sub_0807B0EC
	.align 2, 0

