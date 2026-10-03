	thumb_func_start OpponentSelect_Init
OpponentSelect_Init: @ 0x08002980
	push {r4, lr}
	sub sp, #4
	mov r1, sp
	mov r0, #0
	strh r0, [r1]
	ldr r1, _08002A20 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r0, _08002A24 @ =0x0201F7E0
	str r0, [r1, #4]
	ldr r0, _08002A28 @ =0x8100001A
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _080029AC
_080029A4:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _080029A4
_080029AC:
	bl SetBrightnessBlack
	bl ResetBgScroll
	bl ResetVideo
	ldr r0, _08002A2C @ =0x0400004C
	mov r1, #0
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
	add r0, #4
	strh r1, [r0]
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
	ldr r0, _08002A30 @ =0x03000040
	ldr r1, _08002A34 @ =0x0000040E
	add r0, r0, r1
	mov r1, #1
	strh r1, [r0]
	ldr r0, _08002A38 @ =0x05000200
	ldr r1, _08002A3C @ =0x0871B650
	mov r2, #0x60
	bl MemCopy16
	ldr r0, _08002A40 @ =0x06014000
	ldr r1, _08002A44 @ =0x0871B850
	mov r2, #0x80
	lsl r2, r2, #5
	bl MemCopy16
	ldr r4, _08002A24 @ =0x0201F7E0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	bl OpponentSelect_SnapCursor
	ldrb r4, [r4]
	lsl r0, r4, #0x1D
	lsr r0, r0, #0x1D
	bl OpponentSelect_LoadPage
	mov r0, #1
	add sp, #4
	pop {r4}
	pop {r1}
	bx r1
_08002A20: .4byte 0x040000D4
_08002A24: .4byte 0x0201F7E0
_08002A28: .4byte 0x8100001A
_08002A2C: .4byte 0x0400004C
_08002A30: .4byte 0x03000040
_08002A34: .4byte 0x0000040E
_08002A38: .4byte 0x05000200
_08002A3C: .4byte gOpponentSelectObjPal
_08002A40: .4byte 0x06014000
_08002A44: .4byte gOpponentSelectObjTiles
	thumb_func_end OpponentSelect_Init

