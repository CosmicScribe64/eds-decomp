	thumb_func_start sub_08068E20
sub_08068E20: @ 0x08068E20
	push {r4, lr}
	sub sp, #4
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r4, #0
	str r4, [sp, #0]
	bl sub_08064E28
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end sub_08068E20

