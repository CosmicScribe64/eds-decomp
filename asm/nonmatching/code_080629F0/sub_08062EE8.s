	thumb_func_start sub_08062EE8
sub_08062EE8: @ 0x08062EE8
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r2, r1, #0
	ldr r0, _08062EF8 @ =0x0000FFFF
	cmp r1, r0
	bne _08062EFC
	mov r0, #0
	b _08062F2A
_08062EF8: .4byte 0x0000FFFF
_08062EFC:
	ldr r0, _08062F10 @ =0x000007CF
	cmp r1, r0
	bhi _08062F18
	add r0, #0x30
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08062F14 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _08062F2A
_08062F10: .4byte 0x000007CF
_08062F14: .4byte gUnk_08623DF4
_08062F18:
	ldr r1, _08062F58 @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _08062F5C @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08062F60 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_08062F2A:
	lsl r0, r0, #0x10
	ldr r1, _08062F64 @ =0x02011C20
	lsr r0, r0, #0xE
	add r1, r0, r1
	ldrh r2, [r1, #8]
	lsl r0, r2, #0x16
	cmp r0, #0
	bne _08062F68
	ldrb r1, [r1, #9]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	bne _08062F68
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #0
	bne _08062F68
	lsr r0, r1, #6
	cmp r0, #0
	bne _08062F68
	mov r0, #0
	b _08062F6A
	.align 2, 0
_08062F58: .4byte 0xFFFFF830
_08062F5C: .4byte 0x000007FF
_08062F60: .4byte gUnk_08623DF4
_08062F64: .4byte 0x02011C20
_08062F68:
	mov r0, #1
_08062F6A:
	bx lr
	thumb_func_end sub_08062EE8

