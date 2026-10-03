	thumb_func_start ChainListScreen_Start
ChainListScreen_Start: @ 0x0801A7B4
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r2, _0801A7C8 @ =0x020185B8
	str r0, [r2]
	mov r0, #1
	and r1, r0
	strb r1, [r2, #4]
	mov r0, #0
	strb r0, [r2, #5]
	bx lr
_0801A7C8: .4byte 0x020185B8
	thumb_func_end ChainListScreen_Start

