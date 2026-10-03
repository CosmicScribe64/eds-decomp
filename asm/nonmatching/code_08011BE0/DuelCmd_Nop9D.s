	thumb_func_start DuelCmd_Nop9D
DuelCmd_Nop9D: @ 0x08012760
	ldr r1, _08012774 @ =0x020185C0
	ldr r0, _08012778 @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	bx lr
	.align 2, 0
_08012774: .4byte 0x020185C0
_08012778: .4byte 0x0000080D
	thumb_func_end DuelCmd_Nop9D

