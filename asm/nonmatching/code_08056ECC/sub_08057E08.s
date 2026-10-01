	thumb_func_start sub_08057E08
sub_08057E08: @ 0x08057E08
	ldr r1, _08057E2C @ =0x040000D4
	ldr r0, _08057E30 @ =0x020192E4
	str r0, [r1]
	ldr r0, _08057E34 @ =0x02015F14
	str r0, [r1, #4]
	ldr r0, _08057E38 @ =0x80000D86
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _08057E2A
_08057E22:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _08057E22
_08057E2A:
	bx lr
_08057E2C: .4byte 0x040000D4
_08057E30: .4byte 0x020192E4
_08057E34: .4byte 0x02015F14
_08057E38: .4byte 0x80000D86
	thumb_func_end sub_08057E08

