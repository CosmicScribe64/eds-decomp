	thumb_func_start Title_Init
Title_Init: @ 0x0800527C
	push {r4, r5, lr}
	ldr r4, _08005294 @ =0x03000040
	ldr r0, _08005298 @ =0x00004858
	add r5, r4, r0
	ldrb r0, [r5]
	cmp r0, #1
	beq _080052D8
	cmp r0, #1
	bgt _0800529C
	cmp r0, #0
	beq _080052A2
	b _08005308
_08005294: .4byte 0x03000040
_08005298: .4byte 0x00004858
_0800529C:
	cmp r0, #2
	beq _080052E0
	b _08005308
_080052A2:
	ldr r4, _080052D4 @ =0x0201527C
	add r0, r4, #0
	mov r1, #4
	bl MemClear16
	bl IsSaveChecksumValid
	mov r2, #1
	and r0, r2
	mov r1, #2
	neg r1, r1
	ldrb r3, [r4, #2]
	and r1, r3
	orr r1, r0
	mov r0, #1
	and r0, r1
	and r0, r2
	lsl r0, r0, #1
	mov r2, #3
	neg r2, r2
	and r1, r2
	orr r1, r0
	strb r1, [r4, #2]
	b _080052F8
	.align 2, 0
_080052D4: .4byte 0x0201527C
_080052D8:
	mov r0, #0x80
	lsl r0, r0, #0x13
	mov r1, #0
	b _080052F6
_080052E0:
	bl SetBrightnessBlack
	bl ResetVideo
	bl ResetBgScroll
	bl Title_InitBgCnt
	ldr r1, _08005304 @ =0x0000040E
	add r0, r4, r1
	mov r1, #3
_080052F6:
	strh r1, [r0]
_080052F8:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _0800530A
	.align 2, 0
_08005304: .4byte 0x0000040E
_08005308:
	mov r0, #1
_0800530A:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end Title_Init

