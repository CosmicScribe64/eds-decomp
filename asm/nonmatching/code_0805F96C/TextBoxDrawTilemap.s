	thumb_func_start TextBoxDrawTilemap
TextBoxDrawTilemap: @ 0x0805FEA4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r2, _08060060 @ =0x0201AE60
	ldrh r1, [r2, #0xE]
	sub r0, r0, r1
	sub r7, r0, #2
	ldrh r3, [r2, #8]
	ldr r4, _08060064 @ =0x0000FFFF
	add r1, r3, r4
	sub r0, #4
	lsl r0, r0, #5
	add r1, r1, r0
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	ldr r6, _08060068 @ =0x000082CE
	mov sl, r6
	ldr r0, _0806006C @ =0x000082D7
	str r0, [sp, #0]
	cmp r7, #0x13
	bhi _0805FEFA
	mov r3, #0
	add r1, r2, #0
	ldrh r0, [r1, #0xC]
	add r0, #2
	cmp r3, r0
	bge _0805FEFA
	ldr r0, _08060070 @ =0x03000040
	mov r5, #0
	lsl r1, r4, #1
	ldr r6, _08060074 @ =0x00002C1C
	add r0, r0, r6
	add r1, r1, r0
_0805FEEC:
	strh r5, [r1]
	add r1, #2
	add r3, #1
	ldrh r0, [r2, #0xC]
	add r0, #2
	cmp r3, r0
	blt _0805FEEC
_0805FEFA:
	add r7, #1
	add r0, r4, #0
	add r0, #0x20
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r7, #0x13
	bhi _0805FF4C
	ldr r2, _08060070 @ =0x03000040
	lsl r1, r4, #1
	ldr r3, _08060074 @ =0x00002C1C
	add r0, r2, r3
	add r1, r1, r0
	mov r6, sl
	strh r6, [r1]
	mov r3, #1
	ldr r5, _08060060 @ =0x0201AE60
	mov r0, #2
	add r0, sl
	mov r8, r0
	ldrh r6, [r5, #0xC]
	cmp r3, r6
	bgt _0805FF38
	mov r0, sl
	add r0, #1
	add r1, #2
_0805FF2C:
	strh r0, [r1]
	add r1, #2
	add r3, #1
	ldrh r6, [r5, #0xC]
	cmp r3, r6
	ble _0805FF2C
_0805FF38:
	ldr r1, _08060060 @ =0x0201AE60
	ldrh r1, [r1, #0xC]
	add r0, r1, r4
	add r0, #1
	lsl r0, r0, #1
	ldr r3, _08060074 @ =0x00002C1C
	add r1, r2, r3
	add r0, r0, r1
	mov r6, r8
	strh r6, [r0]
_0805FF4C:
	add r7, #1
	add r0, r4, #0
	add r0, #0x20
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r3, #0
	ldr r0, _08060060 @ =0x0201AE60
	ldrh r0, [r0, #0xE]
	cmp r3, r0
	bge _0805FFCC
	ldr r1, _08060078 @ =0x03002C5C
	mov r9, r1
	ldr r2, _08060060 @ =0x0201AE60
	mov r8, r2
	mov r6, r9
	str r6, [sp, #4]
_0805FF6C:
	add r3, #1
	mov ip, r3
	cmp r7, #0x13
	bhi _0805FFB8
	lsl r2, r4, #1
	mov r0, r9
	add r1, r2, r0
	mov r0, sl
	add r0, #3
	strh r0, [r1]
	mov r3, #1
	mov r5, r8
	mov r6, sl
	add r6, #5
	ldrh r1, [r5, #0xC]
	cmp r3, r1
	bgt _0805FFAA
	ldr r1, [sp, #4]
	add r0, r2, r1
	add r2, r0, #2
_0805FF94:
	ldr r1, [sp, #0]
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
	strh r1, [r2]
	add r2, #2
	add r3, #1
	ldrh r0, [r5, #0xC]
	cmp r3, r0
	ble _0805FF94
_0805FFAA:
	mov r1, r8
	ldrh r1, [r1, #0xC]
	add r0, r1, r4
	add r0, #1
	lsl r0, r0, #1
	add r0, r9
	strh r6, [r0]
_0805FFB8:
	add r7, #1
	add r0, r4, #0
	add r0, #0x20
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r3, ip
	ldr r2, _08060060 @ =0x0201AE60
	ldrh r2, [r2, #0xE]
	cmp r3, r2
	blt _0805FF6C
_0805FFCC:
	add r3, r7, #1
	mov r8, r3
	mov r6, #0x20
	add r6, r6, r4
	mov r9, r6
	cmp r7, #0x13
	bhi _0806001C
	ldr r2, _08060070 @ =0x03000040
	lsl r1, r4, #1
	ldr r7, _08060074 @ =0x00002C1C
	add r0, r2, r7
	add r1, r1, r0
	mov r0, sl
	add r0, #6
	strh r0, [r1]
	mov r3, #1
	ldr r5, _08060060 @ =0x0201AE60
	mov r6, sl
	add r6, #8
	ldrh r0, [r5, #0xC]
	cmp r3, r0
	bgt _0806000A
	mov r0, sl
	add r0, #7
	add r1, #2
_0805FFFE:
	strh r0, [r1]
	add r1, #2
	add r3, #1
	ldrh r7, [r5, #0xC]
	cmp r3, r7
	ble _0805FFFE
_0806000A:
	ldr r1, _08060060 @ =0x0201AE60
	ldrh r1, [r1, #0xC]
	add r0, r1, r4
	add r0, #1
	lsl r0, r0, #1
	ldr r3, _08060074 @ =0x00002C1C
	add r1, r2, r3
	add r0, r0, r1
	strh r6, [r0]
_0806001C:
	mov r4, r9
	lsl r0, r4, #0x10
	lsr r4, r0, #0x10
	mov r6, r8
	cmp r6, #0x13
	bhi _0806004E
	mov r3, #0
	ldr r2, _08060060 @ =0x0201AE60
	ldrh r0, [r2, #0xC]
	add r0, #2
	cmp r3, r0
	bge _0806004E
	ldr r0, _08060070 @ =0x03000040
	mov r5, #0
	lsl r1, r4, #1
	ldr r7, _08060074 @ =0x00002C1C
	add r0, r0, r7
	add r1, r1, r0
_08060040:
	strh r5, [r1]
	add r1, #2
	add r3, #1
	ldrh r0, [r2, #0xC]
	add r0, #2
	cmp r3, r0
	blt _08060040
_0806004E:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08060060: .4byte 0x0201AE60
_08060064: .4byte 0x0000FFFF
_08060068: .4byte 0x000082CE
_0806006C: .4byte 0x000082D7
_08060070: .4byte 0x03000040
_08060074: .4byte 0x00002C1C
_08060078: .4byte 0x03002C5C
	thumb_func_end TextBoxDrawTilemap

