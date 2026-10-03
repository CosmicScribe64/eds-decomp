	thumb_func_start License_InitVideo
License_InitVideo: @ 0x08004AF8
	push {r4, r5, lr}
	ldr r1, _08004B10 @ =0x03000040
	ldr r0, _08004B14 @ =0x00004858
	add r5, r1, r0
	ldrb r4, [r5]
	cmp r4, #0
	beq _08004B18
	cmp r4, #1
	beq _08004B24
	mov r0, #1
	b _08004B64
	.align 2, 0
_08004B10: .4byte 0x03000040
_08004B14: .4byte 0x00004858
_08004B18:
	bl SetBrightnessWhite
	mov r0, #0x80
	lsl r0, r0, #0x13
	strh r4, [r0]
	b _08004B5C
_08004B24:
	ldr r2, _08004B6C @ =0x0000040E
	add r1, r1, r2
	mov r0, #3
	strh r0, [r1]
	bl ResetVideo
	bl ResetBgScroll
	ldr r1, _08004B70 @ =0x04000008
	mov r0, #0x84
	strh r0, [r1]
	add r1, #2
	ldr r2, _08004B74 @ =0x00000105
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08004B78 @ =0x00000206
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08004B7C @ =0x00000307
	add r0, r2, #0
	strh r0, [r1]
	mov r1, #0xA0
	lsl r1, r1, #0x13
	ldr r2, _08004B80 @ =0x0000FFFF
	add r0, r2, #0
	strh r0, [r1]
_08004B5C:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
_08004B64:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08004B6C: .4byte 0x0000040E
_08004B70: .4byte 0x04000008
_08004B74: .4byte 0x00000105
_08004B78: .4byte 0x00000206
_08004B7C: .4byte 0x00000307
_08004B80: .4byte 0x0000FFFF
	thumb_func_end License_InitVideo

