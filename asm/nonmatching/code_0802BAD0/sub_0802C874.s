	thumb_func_start sub_0802C874
sub_0802C874: @ 0x0802C874
	push {lr}
	add r1, r0, #0
	ldrb r2, [r1, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r1, #2]
	lsl r1, r1, #0x16
	lsr r1, r1, #0x1A
	bl sub_08017FF4
	mov r0, #1
	pop {r1}
	bx r1
	thumb_func_end sub_0802C874
	.align 2, 0

