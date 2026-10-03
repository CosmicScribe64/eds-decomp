	thumb_func_start ChainListScreen_DrawHeader
ChainListScreen_DrawHeader: @ 0x0801A198
	push {r4, lr}
	mov r4, #0
_0801A19C:
	lsl r0, r4, #5
	lsl r2, r4, #0x12
	lsr r2, r2, #0x10
	mov r1, #0x81
	lsl r1, r1, #7
	bl AddSprite
	add r4, #1
	cmp r4, #7
	ble _0801A19C
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end ChainListScreen_DrawHeader
	.align 2, 0

