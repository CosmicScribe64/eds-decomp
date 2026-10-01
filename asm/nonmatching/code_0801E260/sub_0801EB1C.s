	thumb_func_start sub_0801EB1C
sub_0801EB1C: @ 0x0801EB1C
	ldr r2, _0801EB4C @ =0x020185C0
	ldr r0, _0801EB50 @ =0x04000006
	ldrh r1, [r0]
	ldr r0, _0801EB54 @ =0x03000040
	ldr r3, _0801EB58 @ =0x0000485E
	add r0, r0, r3
	ldrh r0, [r0]
	add r1, r0, r1
	mov r0, #0xF
	and r1, r0
	mov r0, #0x81
	lsl r0, r0, #4
	add r2, r2, r0
	ldr r0, [r2]
	lsl r1, r1, #1
	add r1, r1, r0
	ldrh r1, [r1]
	ldr r0, _0801EB5C @ =0x04000010
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	add r0, #8
	strh r1, [r0]
	bx lr
_0801EB4C: .4byte 0x020185C0
_0801EB50: .4byte 0x04000006
_0801EB54: .4byte 0x03000040
_0801EB58: .4byte 0x0000485E
_0801EB5C: .4byte 0x04000010
	thumb_func_end sub_0801EB1C

