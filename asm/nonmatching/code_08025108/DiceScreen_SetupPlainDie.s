	thumb_func_start DiceScreen_SetupPlainDie
DiceScreen_SetupPlainDie: @ 0x0802600C
	ldr r1, _08026024 @ =0x0201F820
	ldr r0, _08026028 @ =0x00000AEA
	add r2, r1, r0
	mov r3, #0
	mov r0, #2
	strb r0, [r2]
	ldr r0, _0802602C @ =0x00000ACD
	add r1, r1, r0
	strb r3, [r1]
	mov r0, #1
	bx lr
	.align 2, 0
_08026024: .4byte 0x0201F820
_08026028: .4byte 0x00000AEA
_0802602C: .4byte 0x00000ACD
	thumb_func_end DiceScreen_SetupPlainDie

