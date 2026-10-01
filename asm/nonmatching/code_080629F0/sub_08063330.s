	thumb_func_start sub_08063330
sub_08063330: @ 0x08063330
	push {lr}
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0xF8
	lsl r3, r3, #5
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	bl sub_0806245C
	mov r0, #4
	bl sub_08075AE4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end sub_08063330

