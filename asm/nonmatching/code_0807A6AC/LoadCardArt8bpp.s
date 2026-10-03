	thumb_func_start LoadCardArt8bpp
LoadCardArt8bpp: @ 0x0807AFFC
	push {lr}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	bl UnpackCardArt8bpp
	pop {r0}
	bx r0
	thumb_func_end LoadCardArt8bpp
	.align 2, 0

