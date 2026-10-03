	thumb_func_start DiceScreen_SetupGracefulDice
DiceScreen_SetupGracefulDice: @ 0x08025FE8
	ldr r1, _08026000 @ =0x0201F820
	ldr r0, _08026004 @ =0x00000AEA
	add r2, r1, r0
	mov r3, #0
	mov r0, #1
	strb r0, [r2]
	ldr r0, _08026008 @ =0x00000ACD
	add r1, r1, r0
	strb r3, [r1]
	mov r0, #1
	bx lr
	.align 2, 0
_08026000: .4byte 0x0201F820
_08026004: .4byte 0x00000AEA
_08026008: .4byte 0x00000ACD
	thumb_func_end DiceScreen_SetupGracefulDice

