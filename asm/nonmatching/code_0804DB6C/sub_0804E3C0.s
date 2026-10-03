	thumb_func_start sub_0804E3C0
sub_0804E3C0: @ 0x0804E3C0
	push {r4, r5, lr}
	ldr r1, _0804E400 @ =0x0201CFB0
	mov r2, #0x81
	lsl r2, r2, #4
	add r0, r1, r2
	ldr r0, [r0]
	ldrb r1, [r1, #4]
	sub r0, r0, r1
	ldr r5, _0804E404 @ =0x002800A0
	cmp r0, #0x47
	bgt _0804E3D8
	ldr r5, _0804E408 @ =0x007000A0
_0804E3D8:
	ldr r1, _0804E40C @ =0x000040C0
	ldr r2, _0804E410 @ =0x0000F364
	ldr r4, _0804E414 @ =0x081A4424
	ldr r0, _0804E418 @ =0x03000040
	ldr r3, _0804E41C @ =0x0000485E
	add r0, r0, r3
	ldrh r0, [r0]
	lsr r0, r0, #1
	mov r3, #0xF
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	lsl r3, r0, #0x10
	add r0, r5, #0
	bl AddAffineSprite
	pop {r4, r5}
	pop {r0}
	bx r0
_0804E400: .4byte 0x0201CFB0
_0804E404: .4byte 0x002800A0
_0804E408: .4byte 0x007000A0
_0804E40C: .4byte 0x000040C0
_0804E410: .4byte 0x0000F364
_0804E414: .4byte gPulseScaleCurve
_0804E418: .4byte 0x03000040
_0804E41C: .4byte 0x0000485E
	thumb_func_end sub_0804E3C0

