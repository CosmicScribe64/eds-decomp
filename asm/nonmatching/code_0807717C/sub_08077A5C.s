	thumb_func_start sub_08077A5C
sub_08077A5C: @ 0x08077A5C
	ldr r0, _08077A6C @ =0x02011C20
	ldr r1, _08077A70 @ =0x00002152
	add r0, r0, r1
	ldrh r0, [r0]
	lsr r0, r0, #1
	mov r1, #1
	and r0, r1
	bx lr
_08077A6C: .4byte 0x02011C20
_08077A70: .4byte 0x00002152
	thumb_func_end sub_08077A5C

