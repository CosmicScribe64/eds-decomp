	thumb_func_start sub_08004F08
sub_08004F08: @ 0x08004F08
	ldr r0, _08004F20 @ =0x0201527C
	ldrh r1, [r0]
	sub r1, #1
	strh r1, [r0]
	ldr r2, _08004F24 @ =0x0400001E
	lsl r0, r1, #0x10
	lsr r0, r0, #0x12
	strh r0, [r2]
	ldr r0, _08004F28 @ =0x0400001C
	strh r1, [r0]
	bx lr
	.align 2, 0
_08004F20: .4byte 0x0201527C
_08004F24: .4byte 0x0400001E
_08004F28: .4byte 0x0400001C
	thumb_func_end sub_08004F08

