	thumb_func_start sub_0802CCA0
sub_0802CCA0: @ 0x0802CCA0
	push {lr}
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	mov r2, #1
	mov r3, #0
	bl sub_08022758
	mov r0, #1
	pop {r1}
	bx r1
	thumb_func_end sub_0802CCA0

