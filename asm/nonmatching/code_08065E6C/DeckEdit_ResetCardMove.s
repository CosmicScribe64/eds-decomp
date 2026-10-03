	thumb_func_start DeckEdit_ResetCardMove
DeckEdit_ResetCardMove: @ 0x080666AC
	mov r1, #0
	strb r1, [r0]
	bx lr
	thumb_func_end DeckEdit_ResetCardMove
	.align 2, 0

