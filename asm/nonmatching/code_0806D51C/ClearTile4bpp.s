	thumb_func_start ClearTile4bpp
ClearTile4bpp: @ 0x0806D634
	push {lr}
	lsl r1, r1, #5
	add r0, r0, r1
	mov r1, #0x20
	bl MemClear16
	pop {r0}
	bx r0
	thumb_func_end ClearTile4bpp

