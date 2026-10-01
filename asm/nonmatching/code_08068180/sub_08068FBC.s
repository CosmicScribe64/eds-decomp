	thumb_func_start sub_08068FBC
sub_08068FBC: @ 0x08068FBC
	ldr r3, _08068FE4 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r2, _08068FE8 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r2, #0xF8
	lsl r2, r2, #0x11
	and r0, r2
	lsr r0, r0, #0x14
	and r1, r3
	lsl r1, r1, #2
	ldr r3, _08068FE8 @ =0x08621DE0
	add r1, r1, r3
	ldr r1, [r1]
	and r1, r2
	lsr r1, r1, #0x14
	sub r0, r0, r1
	lsr r0, r0, #0x1F
	bx lr
_08068FE4: .4byte 0x000007FF
_08068FE8: .4byte gUnk_08621DE0
	thumb_func_end sub_08068FBC

