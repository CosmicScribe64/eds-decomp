	thumb_func_start sub_080039C0
sub_080039C0: @ 0x080039C0
	push {lr}
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r2, #0x90
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	bl sub_08003850
	mov r0, #1
	bl sub_08075AE4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end sub_080039C0

