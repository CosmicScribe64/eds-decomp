	thumb_func_start LoadObjTileBlock4x4
LoadObjTileBlock4x4: @ 0x080263B0
	push {lr}
	lsl r2, r2, #5
	add r0, r0, r2
	lsl r1, r1, #5
	ldr r2, _080263C4 @ =0x06014000
	add r1, r1, r2
	bl CopyObjTileBlock4x4
	pop {r0}
	bx r0
_080263C4: .4byte 0x06014000
	thumb_func_end LoadObjTileBlock4x4

