	thumb_func_start ClearBlend
ClearBlend: @ 0x080757F4
	ldr r1, _08075810 @ =0x03000040
	ldr r0, _08075814 @ =0x00004832
	add r1, r1, r0
	mov r0, #0x40
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _08075818 @ =0x04000050
	mov r1, #0
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	bx lr
_08075810: .4byte 0x03000040
_08075814: .4byte 0x00004832
_08075818: .4byte 0x04000050
	thumb_func_end ClearBlend

