	thumb_func_start sub_08022758
sub_08022758: @ 0x08022758
	push {r4, r5, lr}
	add r5, r0, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x10
	neg r0, r2
	orr r0, r2
	lsr r4, r0, #0x1F
	cmp r3, #0
	beq _08022770
	mov r0, #2
	orr r4, r0
_08022770:
	lsl r2, r1, #0x10
	lsr r2, r2, #0x10
	add r0, r5, #0
	mov r1, #2
	add r3, r4, #0
	bl sub_08022678
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_08022758

