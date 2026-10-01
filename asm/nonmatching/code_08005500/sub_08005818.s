	thumb_func_start sub_08005818
sub_08005818: @ 0x08005818
	ldr r1, _08005848 @ =0x03000040
	ldr r0, _0800584C @ =0x04000006
	ldrh r3, [r0]
	ldr r2, _08005850 @ =0x00004862
	add r0, r1, r2
	strh r3, [r0]
	lsl r0, r3, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x9F
	bhi _08005844
	ldr r2, _08005854 @ =0x04000014
	mov r0, #0xF
	and r0, r3
	lsl r0, r0, #1
	ldr r3, _08005858 @ =0x00004836
	add r1, r1, r3
	add r0, r0, r1
	ldrh r1, [r0]
	strh r1, [r2]
	ldr r1, _0800585C @ =0x04000018
	ldrh r0, [r0]
	strh r0, [r1]
_08005844:
	bx lr
	.align 2, 0
_08005848: .4byte 0x03000040
_0800584C: .4byte 0x04000006
_08005850: .4byte 0x00004862
_08005854: .4byte 0x04000014
_08005858: .4byte 0x00004836
_0800585C: .4byte 0x04000018
	thumb_func_end sub_08005818

