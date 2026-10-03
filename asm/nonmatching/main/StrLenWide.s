	thumb_func_start StrLenWide
StrLenWide: @ 0x080753E0
	add r1, r0, #0
	mov r2, #0
	b _080753EA
_080753E6:
	add r1, #2
	add r2, #1
_080753EA:
	ldrb r0, [r1]
	cmp r0, #0
	bne _080753E6
	add r0, r2, #0
	bx lr
	thumb_func_end StrLenWide

