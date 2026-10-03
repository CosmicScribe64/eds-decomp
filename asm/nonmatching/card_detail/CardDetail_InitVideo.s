	thumb_func_start CardDetail_InitVideo
CardDetail_InitVideo: @ 0x0800696C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r0, _08006A48 @ =0x03000040
	mov r8, r0
	ldr r1, _08006A4C @ =0x0000040E
	add r1, r8
	mov r6, #0
	ldr r0, _08006A50 @ =0x00000803
	strh r0, [r1]
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r0, #0x40
	strh r0, [r1]
	bl ResetVideo
	ldr r0, _08006A54 @ =0x0400004C
	strh r6, [r0]
	ldr r1, _08006A58 @ =0x04000008
	mov r0, #0x44
	strh r0, [r1]
	add r1, #2
	mov r2, #0xC3
	lsl r2, r2, #1
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r7, _08006A5C @ =0x00000285
	add r0, r7, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08006A60 @ =0x00008405
	add r0, r2, #0
	strh r0, [r1]
	bl ResetBgScroll
	ldr r7, _08006A64 @ =0x02013D90
	mov r9, r7
	mov r0, r9
	str r6, [r0, #0x34]
	bl SetBrightnessBlack
	bl LoadSystemGfx
	ldr r0, _08006A68 @ =0x05000260
	ldr r1, _08006A6C @ =0x0863840C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08006A70 @ =0x06010600
	ldr r1, _08006A74 @ =0x0863842C
	mov r2, #0x80
	lsl r2, r2, #2
	bl CopyDoubleWords
	ldr r0, _08006A78 @ =0x00000414
	add r0, r8
	str r6, [r0]
	ldr r5, _08006A7C @ =0x04000208
	strh r6, [r5]
	ldr r3, _08006A80 @ =0x04000200
	ldrh r1, [r3]
	ldr r2, _08006A84 @ =0x0000FFFD
	mov ip, r2
	mov r0, ip
	and r0, r1
	strh r0, [r3]
	mov r2, #1
	strh r2, [r5]
	strh r6, [r5]
	ldrh r1, [r3]
	mov r0, ip
	and r0, r1
	strh r0, [r3]
	ldr r4, _08006A88 @ =0x03000000
	str r6, [r4, #4]
	strh r2, [r5]
	mov r2, #1
	add r0, r2, #0
	ldrb r7, [r7]
	and r0, r7
	cmp r0, #0
	beq _08006A38
	ldr r1, _08006A8C @ =0x00004834
	add r1, r8
	ldr r0, _08006A90 @ =0x0000FFE0
	strh r0, [r1]
	strh r6, [r5]
	ldrh r1, [r3]
	mov r0, ip
	and r0, r1
	strh r0, [r3]
	ldr r0, _08006A94 @ =0x08005819
	str r0, [r4, #4]
	strh r2, [r5]
	strh r6, [r5]
	ldrh r0, [r3]
	mov r1, #2
	orr r0, r1
	strh r0, [r3]
	strh r2, [r5]
_08006A38:
	mov r0, #1
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08006A48: .4byte 0x03000040
_08006A4C: .4byte 0x0000040E
_08006A50: .4byte 0x00000803
_08006A54: .4byte 0x0400004C
_08006A58: .4byte 0x04000008
_08006A5C: .4byte 0x00000285
_08006A60: .4byte 0x00008405
_08006A64: .4byte 0x02013D90
_08006A68: .4byte 0x05000260
_08006A6C: .4byte gCardStatDigitsPal
_08006A70: .4byte 0x06010600
_08006A74: .4byte gCardStatDigitsGfx
_08006A78: .4byte 0x00000414
_08006A7C: .4byte 0x04000208
_08006A80: .4byte 0x04000200
_08006A84: .4byte 0x0000FFFD
_08006A88: .4byte 0x03000000
_08006A8C: .4byte 0x00004834
_08006A90: .4byte 0x0000FFE0
_08006A94: .4byte CardDetail_HBlank
	thumb_func_end CardDetail_InitVideo

