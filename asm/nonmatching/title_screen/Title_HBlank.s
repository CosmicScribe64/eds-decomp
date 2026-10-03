	thumb_func_start Title_HBlank
Title_HBlank: @ 0x08004ABC
	push {r4, lr}
	ldr r3, _08004AE4 @ =0x04000014
	ldr r2, _08004AE8 @ =0x03000040
	ldr r0, _08004AEC @ =0x04000006
	ldrh r0, [r0]
	ldr r4, _08004AF0 @ =0x0000485E
	add r1, r2, r4
	ldrh r1, [r1]
	add r0, r1, r0
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08004AF4 @ =0x00004836
	add r2, r2, r1
	add r0, r0, r2
	ldrh r0, [r0]
	strh r0, [r3]
	pop {r4}
	pop {r0}
	bx r0
_08004AE4: .4byte 0x04000014
_08004AE8: .4byte 0x03000040
_08004AEC: .4byte 0x04000006
_08004AF0: .4byte 0x0000485E
_08004AF4: .4byte 0x00004836
	thumb_func_end Title_HBlank

