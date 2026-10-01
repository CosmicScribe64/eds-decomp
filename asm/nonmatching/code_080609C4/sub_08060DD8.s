	thumb_func_start sub_08060DD8
sub_08060DD8: @ 0x08060DD8
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0xB
	add r0, r0, r1
	lsl r0, r0, #1
	ldr r1, _08060E28 @ =0x03001C5C
	add r2, r0, r1
	mov r0, #0
	strh r0, [r2]
	strh r0, [r2, #2]
	strh r0, [r2, #4]
	strh r0, [r2, #6]
	add r1, r2, #0
	add r1, #0x40
	strh r0, [r1]
	add r1, #2
	strh r0, [r1]
	add r1, #2
	strh r0, [r1]
	add r1, #2
	strh r0, [r1]
	add r1, #0x3A
	strh r0, [r1]
	add r1, #2
	strh r0, [r1]
	add r1, #2
	strh r0, [r1]
	add r1, #2
	strh r0, [r1]
	add r1, #0x3A
	strh r0, [r1]
	add r1, #2
	strh r0, [r1]
	add r1, #2
	strh r0, [r1]
	add r1, #2
	strh r0, [r1]
	bx lr
	.align 2, 0
_08060E28: .4byte 0x03001C5C
	thumb_func_end sub_08060DD8

