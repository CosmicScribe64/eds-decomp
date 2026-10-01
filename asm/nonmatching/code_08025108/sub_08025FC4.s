	thumb_func_start sub_08025FC4
sub_08025FC4: @ 0x08025FC4
	ldr r1, _08025FDC @ =0x0201F820
	ldr r0, _08025FE0 @ =0x00000AEA
	add r2, r1, r0
	mov r0, #0
	strb r0, [r2]
	ldr r0, _08025FE4 @ =0x00000ACD
	add r1, r1, r0
	mov r0, #2
	strb r0, [r1]
	mov r0, #1
	bx lr
	.align 2, 0
_08025FDC: .4byte 0x0201F820
_08025FE0: .4byte 0x00000AEA
_08025FE4: .4byte 0x00000ACD
	thumb_func_end sub_08025FC4

