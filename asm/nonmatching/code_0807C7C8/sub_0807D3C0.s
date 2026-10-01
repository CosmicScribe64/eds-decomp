	thumb_func_start sub_0807D3C0
sub_0807D3C0: @ 0x0807D3C0
	push {lr}
	bl sub_0807D348
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end sub_0807D3C0
	.align 2, 0

