	thumb_func_start sub_0801FBE0
sub_0801FBE0: @ 0x0801FBE0
	push {lr}
	add r3, r0, #0
	add r2, r1, #0
	mov r0, #1
	add r1, r3, #0
	bl sub_0801FA90
	pop {r0}
	bx r0
	thumb_func_end sub_0801FBE0
	.align 2, 0

