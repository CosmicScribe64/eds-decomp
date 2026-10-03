	thumb_func_start DebugMenu_Launch
DebugMenu_Launch: @ 0x080749E8
	push {r4, r5, lr}
	mov r0, #0x80
	lsl r0, r0, #0x13
	mov r5, #0
	strh r5, [r0]
	ldr r2, _08074A20 @ =0x081A73A0
	ldr r4, _08074A24 @ =0x03000040
	ldr r0, _08074A28 @ =0x00004859
	add r1, r4, r0
	ldrb r3, [r1]
	lsl r0, r3, #4
	add r0, r0, r3
	lsl r0, r0, #2
	add r2, #0x40
	add r0, r0, r2
	ldr r0, [r0]
	bl SetMainCallback
	ldr r0, _08074A2C @ =0x00004857
	add r4, r4, r0
	strb r5, [r4]
	ldr r0, _08074A30 @ =0x02017A30
	strb r5, [r0, #0xB]
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08074A20: .4byte gDebugMenuItems
_08074A24: .4byte 0x03000040
_08074A28: .4byte 0x00004859
_08074A2C: .4byte 0x00004857
_08074A30: .4byte 0x02017A30
	thumb_func_end DebugMenu_Launch

