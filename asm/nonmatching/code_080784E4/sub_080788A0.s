	thumb_func_start sub_080788A0
sub_080788A0: @ 0x080788A0
	mov r1, #2
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	strb r1, [r0]
	bx lr
	thumb_func_end sub_080788A0

