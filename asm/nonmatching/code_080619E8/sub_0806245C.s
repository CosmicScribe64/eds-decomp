	thumb_func_start sub_0806245C
sub_0806245C: @ 0x0806245C
	ldr r2, _08062494 @ =0x03000040
	ldr r0, _08062498 @ =0x02015160
	mov ip, r0
	mov r3, #0x8B
	lsl r3, r3, #1
	add r3, ip
	ldrh r1, [r3]
	add r0, r1, #1
	strh r0, [r3]
	ldr r3, _0806249C @ =0x0000442E
	add r0, r2, r3
	strh r1, [r0]
	mov r1, #0x8C
	lsl r1, r1, #1
	add r1, ip
	ldrh r3, [r1]
	add r0, r3, #1
	strh r0, [r1]
	ldr r0, _080624A0 @ =0x00004426
	add r2, r2, r0
	strh r3, [r2]
	mov r1, #0x8D
	lsl r1, r1, #1
	add r1, ip
	ldrh r0, [r1]
	sub r0, #1
	strh r0, [r1]
	bx lr
_08062494: .4byte 0x03000040
_08062498: .4byte 0x02015160
_0806249C: .4byte 0x0000442E
_080624A0: .4byte 0x00004426
	thumb_func_end sub_0806245C

