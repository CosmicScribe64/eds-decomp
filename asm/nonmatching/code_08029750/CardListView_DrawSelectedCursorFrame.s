	thumb_func_start CardListView_DrawSelectedCursorFrame
CardListView_DrawSelectedCursorFrame: @ 0x0802A4A4
	push {lr}
	ldr r0, _0802A4C8 @ =0x0201D810
	ldrb r2, [r0, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r2, [r0, #6]
	add r1, r2, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	bl CardListView_DrawCursorFrame
	pop {r0}
	bx r0
	.align 2, 0
_0802A4C8: .4byte 0x0201D810
	thumb_func_end CardListView_DrawSelectedCursorFrame

