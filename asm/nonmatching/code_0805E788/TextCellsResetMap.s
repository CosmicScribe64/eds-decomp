	thumb_func_start TextCellsResetMap
TextCellsResetMap: @ 0x0805ED78
	ldr r0, _0805ED90 @ =0x03000040
	mov r2, #0x3F
	ldr r1, _0805ED94 @ =0x000002CD
	ldr r3, _0805ED98 @ =0x0000311A
	add r0, r0, r3
_0805ED82:
	strh r1, [r0]
	sub r1, #1
	sub r0, #2
	sub r2, #1
	cmp r2, #0
	bge _0805ED82
	bx lr
_0805ED90: .4byte 0x03000040
_0805ED94: .4byte 0x000002CD
_0805ED98: .4byte 0x0000311A
	thumb_func_end TextCellsResetMap

