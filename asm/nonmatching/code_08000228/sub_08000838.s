	thumb_func_start sub_08000838
sub_08000838: @ 0x08000838
	mov r3, #0x80
	lsl r3, r3, #0x13
	ldrh r1, [r3]
	mov r2, #0x11
	neg r2, r2
	and r2, r1
	add r0, #0x20
	ldrb r0, [r0]
	cmp r0, #0
	beq _08000850
	mov r0, #0x10
	orr r2, r0
_08000850:
	strh r2, [r3]
	bx lr
	thumb_func_end sub_08000838

