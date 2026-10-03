	thumb_func_start DeckEdit_CommandWindowVBlank
DeckEdit_CommandWindowVBlank: @ 0x08067630
	ldr r1, _08067650 @ =0x04000042
	mov r0, #0xF0
	strh r0, [r1]
	ldr r2, _08067654 @ =0x04000046
	ldr r0, _08067658 @ =0x0201DB20
	ldr r1, _0806765C @ =0x00001C3D
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x1E
	lsl r0, r0, #0xB
	mov r1, #0x70
	orr r0, r1
	strh r0, [r2]
	bx lr
	.align 2, 0
_08067650: .4byte 0x04000042
_08067654: .4byte 0x04000046
_08067658: .4byte 0x0201DB20
_0806765C: .4byte 0x00001C3D
	thumb_func_end DeckEdit_CommandWindowVBlank

