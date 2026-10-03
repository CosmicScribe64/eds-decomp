	thumb_func_start DuelCmd_Nop9F
DuelCmd_Nop9F: @ 0x08012888
	ldr r1, _0801289C @ =0x020185C0
	ldr r0, _080128A0 @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	bx lr
	.align 2, 0
_0801289C: .4byte 0x020185C0
_080128A0: .4byte 0x0000080D
	thumb_func_end DuelCmd_Nop9F

