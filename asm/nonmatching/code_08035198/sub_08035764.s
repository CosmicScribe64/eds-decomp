	thumb_func_start sub_08035764
sub_08035764: @ 0x08035764
	push {lr}
	ldrb r2, [r0, #6]
	ldrh r0, [r0, #6]
	lsr r1, r0, #8
	add r0, r2, #0
	mov r2, #0
	bl sub_08018AE8
	mov r0, #0
	pop {r1}
	bx r1
	thumb_func_end sub_08035764
	.align 2, 0

