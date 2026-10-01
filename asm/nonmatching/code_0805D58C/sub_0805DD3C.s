	thumb_func_start sub_0805DD3C
sub_0805DD3C: @ 0x0805DD3C
	ldr r0, _0805DD60 @ =0x04000028
	mov r1, #0
	str r1, [r0]
	add r0, #4
	str r1, [r0]
	sub r0, #0xC
	mov r3, #0x80
	lsl r3, r3, #1
	add r2, r3, #0
	strh r2, [r0]
	add r0, #0x18
	str r1, [r0]
	add r0, #4
	str r1, [r0]
	sub r0, #0xC
	strh r2, [r0]
	bx lr
	.align 2, 0
_0805DD60: .4byte 0x04000028
	thumb_func_end sub_0805DD3C

