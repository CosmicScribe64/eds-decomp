	thumb_func_start sub_080757BC
sub_080757BC: @ 0x080757BC
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r3, _080757E4 @ =0x03000040
	ldr r1, _080757E8 @ =0x00004832
	add r3, r3, r1
	mov r1, #0x40
	neg r1, r1
	ldrb r2, [r3]
	and r1, r2
	mov r2, #0x1F
	orr r1, r2
	strb r1, [r3]
	ldr r2, _080757EC @ =0x04000050
	strh r0, [r2]
	ldr r0, _080757F0 @ =0x04000054
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1A
	strh r1, [r0]
	bx lr
	.align 2, 0
_080757E4: .4byte 0x03000040
_080757E8: .4byte 0x00004832
_080757EC: .4byte 0x04000050
_080757F0: .4byte 0x04000054
	thumb_func_end sub_080757BC

