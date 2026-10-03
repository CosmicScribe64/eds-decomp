	thumb_func_start PackList_InitVideo
PackList_InitVideo: @ 0x08064290
	push {r4, r5, lr}
	ldr r4, _08064304 @ =0x03000040
	ldr r0, _08064308 @ =0x0000040E
	add r1, r4, r0
	mov r5, #0
	mov r0, #0x21
	strh r0, [r1]
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r0, #0x40
	strh r0, [r1]
	add r1, #8
	mov r0, #0x84
	strh r0, [r1]
	add r1, #2
	ldr r2, _0806430C @ =0x00004185
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08064310 @ =0x00000386
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	add r2, #0xFE
	add r0, r2, #0
	strh r0, [r1]
	bl ResetVideo
	ldr r0, _08064314 @ =0x0400004C
	strh r5, [r0]
	bl SetBrightnessBlack
	bl ResetBgScroll
	ldr r0, _08064318 @ =0x00000414
	add r4, r4, r0
	str r5, [r4]
	ldr r4, _0806431C @ =0x04000208
	strh r5, [r4]
	ldr r2, _08064320 @ =0x04000200
	ldrh r3, [r2]
	ldr r1, _08064324 @ =0x0000FFFD
	add r0, r1, #0
	and r0, r3
	strh r0, [r2]
	mov r3, #1
	strh r3, [r4]
	strh r5, [r4]
	ldrh r0, [r2]
	and r1, r0
	strh r1, [r2]
	ldr r0, _08064328 @ =0x03000000
	str r5, [r0, #4]
	strh r3, [r4]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08064304: .4byte 0x03000040
_08064308: .4byte 0x0000040E
_0806430C: .4byte 0x00004185
_08064310: .4byte 0x00000386
_08064314: .4byte 0x0400004C
_08064318: .4byte 0x00000414
_0806431C: .4byte 0x04000208
_08064320: .4byte 0x04000200
_08064324: .4byte 0x0000FFFD
_08064328: .4byte 0x03000000
	thumb_func_end PackList_InitVideo

