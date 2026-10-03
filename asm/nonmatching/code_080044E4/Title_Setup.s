	thumb_func_start Title_Setup
Title_Setup: @ 0x08005310
	push {r4, r5, lr}
	ldr r5, _08005330 @ =0x03000040
	ldr r0, _08005334 @ =0x00004858
	add r4, r5, r0
	ldrb r1, [r4]
	cmp r1, #0
	beq _08005338
	cmp r1, #1
	beq _0800533E
	bl Title_LoadGraphics
	mov r0, #0
	bl PlayBGMNoTrack
	mov r0, #1
	b _0800535E
_08005330: .4byte 0x03000040
_08005334: .4byte 0x00004858
_08005338:
	mov r0, #0x80
	lsl r0, r0, #0x13
	b _08005354
_0800533E:
	bl SetBrightnessBlack
	bl ResetVideo
	bl ResetBgScroll
	bl Title_InitBgCnt
	ldr r1, _08005364 @ =0x0000040E
	add r0, r5, r1
	mov r1, #3
_08005354:
	strh r1, [r0]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
_0800535E:
	pop {r4, r5}
	pop {r1}
	bx r1
_08005364: .4byte 0x0000040E
	thumb_func_end Title_Setup

