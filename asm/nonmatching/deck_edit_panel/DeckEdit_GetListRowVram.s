	thumb_func_start DeckEdit_GetListRowVram
DeckEdit_GetListRowVram: @ 0x08064F90
	push {lr}
	add r1, r0, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldr r0, _08064FC0 @ =0x0201DB20
	ldr r2, _08064FC4 @ =0x00001C34
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1B
	lsr r0, r0, #0x1C
	add r0, r0, r1
	mov r1, #7
	bl __modsi3
	add r1, r0, #0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r1
	lsl r0, r0, #5
	ldr r1, _08064FC8 @ =0x06006180
	add r0, r0, r1
	pop {r1}
	bx r1
_08064FC0: .4byte 0x0201DB20
_08064FC4: .4byte 0x00001C34
_08064FC8: .4byte 0x06006180
	thumb_func_end DeckEdit_GetListRowVram

