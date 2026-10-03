	thumb_func_start SetTextMode
SetTextMode: @ 0x080770BC
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r3, _080770D8 @ =0x02011C20
	mov r1, #0x7F
	add r2, r0, #0
	and r2, r1
	strb r2, [r3, #4]
	cmp r0, #0
	bne _080770D4
	mov r0, #0x80
	orr r2, r0
	strb r2, [r3, #4]
_080770D4:
	bx lr
	.align 2, 0
_080770D8: .4byte 0x02011C20
	thumb_func_end SetTextMode

