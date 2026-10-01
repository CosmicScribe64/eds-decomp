	thumb_func_start sub_08075778
sub_08075778: @ 0x08075778
	ldr r0, _080757A0 @ =0x03000040
	mov r2, #0
	mov r1, #3
	ldr r3, _080757A4 @ =0x00004426
	add r0, r0, r3
_08075782:
	strh r2, [r0]
	sub r0, #2
	sub r1, #1
	cmp r1, #0
	bge _08075782
	mov r1, #0
	ldr r0, _080757A8 @ =0x04000012
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	bx lr
_080757A0: .4byte 0x03000040
_080757A4: .4byte 0x00004426
_080757A8: .4byte 0x04000012
	thumb_func_end sub_08075778

