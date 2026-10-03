	thumb_func_start ResetVideo
ResetVideo: @ 0x08073574
	push {lr}
	bl ClearBgMapBuffers
	bl LoadSystemGfx
	ldr r1, _080735C8 @ =0x0000027E
	mov r0, #0
	bl SetTextArea
	ldr r0, _080735CC @ =0x04000028
	mov r1, #0
	str r1, [r0]
	add r0, #4
	str r1, [r0]
	sub r0, #0xC
	mov r3, #0x80
	lsl r3, r3, #1
	add r2, r3, #0
	strh r2, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r2, [r0]
	add r0, #0x12
	str r1, [r0]
	add r0, #4
	str r1, [r0]
	sub r0, #0xC
	strh r2, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r2, [r0]
	ldr r1, _080735D0 @ =0x04000008
	mov r0, #4
	strh r0, [r1]
	pop {r0}
	bx r0
_080735C8: .4byte 0x0000027E
_080735CC: .4byte 0x04000028
_080735D0: .4byte 0x04000008
	thumb_func_end ResetVideo

