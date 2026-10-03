	thumb_func_start AiStrategyNone
AiStrategyNone: @ 0x0805D4B4
	ldr r1, _0805D4C8 @ =0x02015F00
	ldr r0, _0805D4CC @ =0x00001B24
	add r1, r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r0, #0
	bx lr
_0805D4C8: .4byte 0x02015F00
_0805D4CC: .4byte 0x00001B24
	thumb_func_end AiStrategyNone

