	thumb_func_start Scroller_StopAtEnds
Scroller_StopAtEnds: @ 0x08027D1C
	add r1, r0, #0
	ldrb r0, [r1]
	cmp r0, #0x30
	bne _08027D28
	mov r0, #0
	strb r0, [r1, #1]
_08027D28:
	ldrb r0, [r1]
	cmp r0, #0
	bne _08027D30
	strb r0, [r1, #1]
_08027D30:
	bx lr
	thumb_func_end Scroller_StopAtEnds
	.align 2, 0

