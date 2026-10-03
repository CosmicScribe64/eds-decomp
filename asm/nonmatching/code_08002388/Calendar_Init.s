	thumb_func_start Calendar_Init
Calendar_Init: @ 0x0800257C
	push {r4, r5, r6, lr}
	sub sp, #4
	mov r1, sp
	mov r0, #0
	strh r0, [r1]
	ldr r1, _08002698 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r2, _0800269C @ =0x0201F7D0
	str r2, [r1, #4]
	ldr r0, _080026A0 @ =0x81000006
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r3, #0x80
	lsl r3, r3, #0x18
	add r6, r2, #0
	ldr r4, _080026A4 @ =0x02011C20
	cmp r0, #0
	bge _080025AE
	add r2, r3, #0
_080025A6:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _080025A6
_080025AE:
	ldr r0, _080026A8 @ =0x00002150
	add r1, r4, r0
	ldrh r0, [r1]
	mov r4, #0
	strh r0, [r6]
	ldrh r0, [r1]
	strh r0, [r6, #2]
	bl SetBrightnessBlack
	bl ResetBgScroll
	bl ResetVideo
	ldr r0, _080026AC @ =0x0400004C
	strh r4, [r0]
	add r0, #4
	strh r4, [r0]
	add r0, #4
	strh r4, [r0]
	sub r0, #0x4C
	mov r1, #3
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r0, #4
	strh r0, [r1]
	ldr r0, _080026B0 @ =0x03000040
	ldr r1, _080026B4 @ =0x0000040E
	add r0, r0, r1
	mov r1, #1
	strh r1, [r0]
	mov r0, #0xA0
	lsl r0, r0, #0x13
	ldr r1, _080026B8 @ =0x087F3D18
	mov r2, #0xF0
	lsl r2, r2, #1
	bl MemCopy16
	ldr r0, _080026BC @ =0x050001E0
	ldr r1, _080026C0 @ =0x0822C300
	mov r2, #0x20
	bl MemCopy16
	mov r0, #0xC0
	lsl r0, r0, #0x13
	ldr r4, _080026C4 @ =0x087EA718
	mov r5, #0x96
	lsl r5, r5, #8
	add r1, r4, #0
	add r2, r5, #0
	bl MemCopy16
	ldr r0, _080026C8 @ =0x0600A000
	add r1, r4, #0
	add r2, r5, #0
	bl MemCopy16
	ldr r0, _080026CC @ =0x050002C0
	ldr r1, _080026D0 @ =0x087F3F18
	mov r2, #0x20
	bl MemCopy16
	ldr r0, _080026D4 @ =0x06014000
	ldr r1, _080026D8 @ =0x087F4118
	mov r2, #0xC0
	lsl r2, r2, #4
	bl MemCopy16
	ldr r0, _080026DC @ =0x05000300
	ldr r1, _080026E0 @ =0x087F7DF8
	mov r2, #0x20
	bl MemCopy16
	ldr r0, _080026E4 @ =0x06015400
	ldr r1, _080026E8 @ =0x087F7E18
	mov r2, #0x80
	lsl r2, r2, #4
	bl MemCopy16
	ldr r0, _080026EC @ =0x050002E0
	ldr r1, _080026F0 @ =0x087F5DD8
	mov r2, #0x20
	bl MemCopy16
	mov r0, #4
	neg r0, r0
	ldrb r1, [r6, #8]
	and r0, r1
	strb r0, [r6, #8]
	ldr r0, _080026F4 @ =0x05000200
	ldr r1, _080026F8 @ =0x087F4D18
	mov r2, #0xC0
	bl MemCopy16
	ldr r0, _080026FC @ =0x06015C00
	ldr r1, _08002700 @ =0x087F4DD8
	mov r2, #0x80
	lsl r2, r2, #5
	bl MemCopy16
	ldrh r0, [r6, #2]
	bl Calendar_SetCursorDate
	bl Calendar_UpdateEventNames
	mov r0, #1
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08002698: .4byte 0x040000D4
_0800269C: .4byte 0x0201F7D0
_080026A0: .4byte 0x81000006
_080026A4: .4byte 0x02011C20
_080026A8: .4byte 0x00002150
_080026AC: .4byte 0x0400004C
_080026B0: .4byte 0x03000040
_080026B4: .4byte 0x0000040E
_080026B8: .4byte gCalendarBgPal
_080026BC: .4byte 0x050001E0
_080026C0: .4byte gSystemFontPal
_080026C4: .4byte gCalendarBgBitmap
_080026C8: .4byte 0x0600A000
_080026CC: .4byte 0x050002C0
_080026D0: .4byte gCalendarNumberPal
_080026D4: .4byte 0x06014000
_080026D8: .4byte gCalendarNumberTiles
_080026DC: .4byte 0x05000300
_080026E0: .4byte gCalendarWeekdayPal
_080026E4: .4byte 0x06015400
_080026E8: .4byte gCalendarWeekdayTiles
_080026EC: .4byte 0x050002E0
_080026F0: .4byte gCalendarMonthNamePal
_080026F4: .4byte 0x05000200
_080026F8: .4byte gCalendarIconPal
_080026FC: .4byte 0x06015C00
_08002700: .4byte gCalendarIconTiles
	thumb_func_end Calendar_Init

