	thumb_func_start sub_0807C4A8
sub_0807C4A8: @ 0x0807C4A8
	push {lr}
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r2, #0xF0
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	add r1, #0x4C
	mov r0, #0
	strh r0, [r1]
	mov r0, #2
	bl sub_08075AE4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end sub_0807C4A8
	.align 2, 0

