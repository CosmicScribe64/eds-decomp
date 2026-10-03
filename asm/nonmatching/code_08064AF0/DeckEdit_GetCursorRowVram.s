	thumb_func_start DeckEdit_GetCursorRowVram
DeckEdit_GetCursorRowVram: @ 0x08064FCC
	ldr r0, _08064FEC @ =0x0201DB20
	ldr r1, _08064FF0 @ =0x00001C34
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0x32
	mul r0, r1
	ldr r1, _08064FF4 @ =0x0000019B
	add r0, r0, r1
	lsl r0, r0, #5
	mov r1, #0xC0
	lsl r1, r1, #0x13
	add r0, r0, r1
	bx lr
	.align 2, 0
_08064FEC: .4byte 0x0201DB20
_08064FF0: .4byte 0x00001C34
_08064FF4: .4byte 0x0000019B
	thumb_func_end DeckEdit_GetCursorRowVram

