	thumb_func_start Record_DrawSprites
Record_DrawSprites: @ 0x0800406C
	push {lr}
	bl Record_DrawResultMarkers
	bl Record_DrawPageArrows
	pop {r0}
	bx r0
	thumb_func_end Record_DrawSprites
	.align 2, 0

