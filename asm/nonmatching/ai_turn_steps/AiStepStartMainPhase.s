	thumb_func_start AiStepStartMainPhase
AiStepStartMainPhase: @ 0x0805A89C
	push {lr}
	ldr r0, _0805A8C0 @ =0x02015F00
	ldr r1, _0805A8C4 @ =0x00001B28
	bl MemClear16
	ldr r0, _0805A8C8 @ =0x00008052
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	bl AiChooseStrategy
	cmp r0, #0
	bne _0805A8CC
	mov r0, #1
	b _0805A8DE
	.align 2, 0
_0805A8C0: .4byte 0x02015F00
_0805A8C4: .4byte 0x00001B28
_0805A8C8: .4byte 0x00008052
_0805A8CC:
	ldr r0, _0805A8E4 @ =0x02015EF0
	mov r2, #0
	mov r1, #8
	strb r1, [r0, #1]
	strb r2, [r0, #2]
	strb r2, [r0, #3]
	strb r2, [r0, #4]
	strb r2, [r0, #5]
	mov r0, #0
_0805A8DE:
	pop {r1}
	bx r1
	.align 2, 0
_0805A8E4: .4byte 0x02015EF0
	thumb_func_end AiStepStartMainPhase

