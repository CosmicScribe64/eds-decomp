	thumb_func_start sub_08065034
sub_08065034: @ 0x08065034
	ldr r0, _0806504C @ =0x0201DB20
	ldr r1, _08065050 @ =0x00001C34
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0x32
	mul r0, r1
	ldr r1, _08065054 @ =0x0000019B
	add r0, r0, r1
	bx lr
	.align 2, 0
_0806504C: .4byte 0x0201DB20
_08065050: .4byte 0x00001C34
_08065054: .4byte 0x0000019B
	thumb_func_end sub_08065034

