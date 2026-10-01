	thumb_func_start sub_0807E764
sub_0807E764: @ 0x0807E764
	ldr r2, _0807E778 @ =0x03005210
	ldr r1, _0807E77C @ =0x00000193
	add r3, r2, r1
	mov r1, #0
	strb r1, [r3]
	mov r3, #0xC9
	lsl r3, r3, #1
	add r1, r2, r3
	strb r0, [r1]
	bx lr
_0807E778: .4byte 0x03005210
_0807E77C: .4byte 0x00000193
	thumb_func_end sub_0807E764

