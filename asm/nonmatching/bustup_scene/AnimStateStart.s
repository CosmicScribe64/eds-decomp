	thumb_func_start AnimStateStart
AnimStateStart: @ 0x0800098C
	mov r1, #1
	strb r1, [r0, #0xE]
	bx lr
	thumb_func_end AnimStateStart
	.align 2, 0

