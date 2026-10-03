	thumb_func_start Timer_Tick
Timer_Tick: @ 0x0807B0D0
	add r1, r0, #0
	ldrb r0, [r1]
	cmp r0, #1
	bne _0807B0E8
	ldrh r0, [r1, #2]
	sub r0, #1
	strh r0, [r1, #2]
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0807B0E8
	mov r0, #2
	strb r0, [r1]
_0807B0E8:
	bx lr
	thumb_func_end Timer_Tick
	.align 2, 0

