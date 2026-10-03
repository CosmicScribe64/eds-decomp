	thumb_func_start CB_DebugMenu
CB_DebugMenu: @ 0x08074A34
	push {r4, r5, lr}
	ldr r1, _08074A80 @ =0x081A768C
	ldr r4, _08074A84 @ =0x03000040
	ldr r0, _08074A88 @ =0x00004857
	add r5, r4, r0
	ldrb r2, [r5]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08074A7A
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08074A78
	ldrb r2, [r5]
	cmp r2, #1
	bhi _08074A74
	ldr r3, _08074A8C @ =0x00004858
	add r0, r4, r3
	mov r1, #0
	strb r1, [r0]
	add r3, #1
	add r0, r4, r3
	strb r1, [r0]
	add r3, #1
	add r0, r4, r3
	strb r1, [r0]
	add r3, #1
	add r0, r4, r3
	strb r1, [r0]
_08074A74:
	add r0, r2, #1
	strb r0, [r5]
_08074A78:
	mov r0, #0
_08074A7A:
	pop {r4, r5}
	pop {r1}
	bx r1
_08074A80: .4byte gDebugMenuSteps
_08074A84: .4byte 0x03000040
_08074A88: .4byte 0x00004857
_08074A8C: .4byte 0x00004858
	thumb_func_end CB_DebugMenu

