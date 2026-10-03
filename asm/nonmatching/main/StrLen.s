	thumb_func_start StrLen
StrLen: @ 0x080753CC
	add r1, r0, #0
	mov r2, #0
	b _080753D6
_080753D2:
	add r2, #1
	add r1, #1
_080753D6:
	ldrb r0, [r1]
	cmp r0, #0
	bne _080753D2
	add r0, r2, #0
	bx lr
	thumb_func_end StrLen

