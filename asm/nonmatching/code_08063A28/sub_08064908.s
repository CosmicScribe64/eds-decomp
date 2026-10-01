	thumb_func_start sub_08064908
sub_08064908: @ 0x08064908
	ldr r2, _08064920 @ =0x04000050
	ldr r3, _08064924 @ =0x00000442
	add r1, r3, #0
	strh r1, [r2]
	add r2, #2
	mov r1, #0x10
	sub r1, r1, r0
	lsl r1, r1, #8
	orr r1, r0
	strh r1, [r2]
	bx lr
	.align 2, 0
_08064920: .4byte 0x04000050
_08064924: .4byte 0x00000442
	thumb_func_end sub_08064908

