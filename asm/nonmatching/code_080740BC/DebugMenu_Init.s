	thumb_func_start DebugMenu_Init
DebugMenu_Init: @ 0x08074554
	push {lr}
	ldr r0, _08074588 @ =0x03000040
	ldr r1, _0807458C @ =0x0000040E
	add r0, r0, r1
	mov r1, #3
	strh r1, [r0]
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r2, #0xA0
	lsl r2, r2, #1
	add r0, r2, #0
	strh r0, [r1]
	bl ResetVideo
	ldr r1, _08074590 @ =0x04000008
	mov r0, #5
	strh r0, [r1]
	bl ResetBgScroll
	bl SetBrightnessBlack
	bl LoadSystemGfx
	mov r0, #1
	pop {r1}
	bx r1
_08074588: .4byte 0x03000040
_0807458C: .4byte 0x0000040E
_08074590: .4byte 0x04000008
	thumb_func_end DebugMenu_Init

