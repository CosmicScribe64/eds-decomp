	thumb_func_start sub_08066244
sub_08066244: @ 0x08066244
	add r2, r0, #0
	mov r1, #0
	mov r3, #0
_0806624A:
	lsl r0, r1, #4
	add r0, r2, r0
	strb r3, [r0, #0xC]
	add r0, r1, #1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, #5
	bls _0806624A
	mov r0, #5
	strb r0, [r2]
	bx lr
	thumb_func_end sub_08066244

