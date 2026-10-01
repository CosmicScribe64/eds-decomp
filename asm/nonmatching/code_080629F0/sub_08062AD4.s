	thumb_func_start sub_08062AD4
sub_08062AD4: @ 0x08062AD4
	push {r4, r5, lr}
	lsl r4, r1, #3
	add r4, r4, r0
	ldr r5, [r4, #4]
	bl sub_08076F9C
	add r1, r5, #0
	bl __modsi3
	ldr r1, [r4]
	lsl r0, r0, #1
	add r0, r0, r1
	ldrh r0, [r0]
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08062AD4

