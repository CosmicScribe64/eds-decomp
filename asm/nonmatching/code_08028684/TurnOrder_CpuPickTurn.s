	thumb_func_start TurnOrder_CpuPickTurn
TurnOrder_CpuPickTurn: @ 0x080289DC
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r1, #1
	and r0, r1
	bx lr
	thumb_func_end TurnOrder_CpuPickTurn
	.align 2, 0

